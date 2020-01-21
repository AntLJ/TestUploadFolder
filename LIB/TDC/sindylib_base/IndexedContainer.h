/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#pragma once

#include "ContainerBase.h"

/// VARIANTをキーにする場合の追加定義
struct VariantLess {
	bool operator()( const CComVariant& va1, const CComVariant& va2 ) const;
};

inline bool VariantLess::operator()( const CComVariant& va1, const CComVariant& va2 ) const {
	// VARIANT比較の前に、VTの比較をする
	if( va1.vt != va2.vt )
		return va1.vt < va2.vt;

	// VTが同じな場合、VT_EMPTYとVT_NULLは中身の比較をしてもしょうがないので
	// 同じとみなす
	switch( va1.vt )
	{
	case VT_NULL:
	case VT_EMPTY:
		return false;
		break;
	default:
		return va1 < va2;
		break;
	}
}

struct VariantEqual {
	bool operator()( const CComVariant& va1, const CComVariant& va2 ) const;
};

inline bool VariantEqual::operator()( const CComVariant& va1, const CComVariant& va2 ) const {
	// VARIANT比較の前に、VTの比較をする
	if( va1.vt != va2.vt )
		return false;

	// VTが同じな場合、VT_EMPTYとVT_NULLは中身の比較をしてもしょうがないので
	// 同じとみなす
	switch( va1.vt )
	{
	case VT_NULL:
	case VT_EMPTY:
		return true;
		break;
	default:
		return va1 == va2;
		break;
	}
}

//extern VariantLess _vaLess;
//extern VariantEqual _vaEqual;

namespace std {

bool operator < ( const sindy::CContainerBase::iterator& c1, const sindy::CContainerBase::iterator& c2 );
inline bool operator < ( const sindy::CContainerBase::iterator& c1, const sindy::CContainerBase::iterator& c2 )
{
	return &*c1 < &*c2;
}

} // std

namespace sindy {

/// CIndexedContainerで使用する自動キャッシュの振る舞い定義
enum sindyContainerAutoCacheType {
	sindyContainerAutoCacheNone,     //!< 自動キャッシュしない
	sindyContainerAutoCacheNoDetach, //!< 全て自動キャッシュする（Detachなし）
	sindyContainerAutoCacheDetach,   //!< 全て自動キャッシュする（Detachあり）
	sindyContainerAutoOnDemandCacheNoDetach, //!< 要求があった分だけ自動キャッシュする（Detachなし）
	sindyContainerAutoOnDemandCacheDetach,   //!< 要求があった分だけ自動キャッシュする（Detachあり）
};

typedef std::list<CString> KeyList;
typedef std::list<CComVariant> ValueList;

struct KeyListLess {
	bool operator()( const KeyList& va1, const KeyList& va2 ) const;
};

inline bool KeyListLess::operator()( const KeyList& va1, const KeyList& va2 ) const {
	if( va1.size() != va2.size() )
		return va1.size() < va2.size();

	KeyList::const_iterator it2;
	for( KeyList::const_iterator it1 = va1.begin(), it2 = va2.begin(); it1 != va1.end(); ++it1, ++it2 )
	{
		if( *it1 == *it2 )
			continue;

		return *it1 < *it2;
	}
	return false;
}

struct ValueListLess {
	bool operator()( const ValueList& va1, const ValueList& va2 ) const;
};

inline bool ValueListLess::operator()( const ValueList& va1, const ValueList& va2 ) const {
	if( va1.size() != va2.size() )
		return va1.size() < va2.size();

	ValueList::const_iterator it2;
	for( ValueList::const_iterator it1 = va1.begin(), it2 = va2.begin(); it1 != va1.end(); ++it1, ++it2 )
	{
		static VariantEqual vaEqual;
		static VariantLess   vaLess;
		if( vaEqual( *it1, *it2 ) )
			continue;

		return vaLess( *it1, *it2 );
	}
	return false;
}

/**
 * @class CIndexedContainer
 * @brief インデックスによるコンテナへのアクセスを
 * 目的としたクラス
 *
 * このクラスはRow系オブジェクト格納コンテナ専用です。\n
 * CSchemaSupportを継承している必要があります。\n
 * インデックスは、find_by_keyが呼ばれた時に始めて作成されます。
 * 一度インデックスが作成されると、それ以後のコンテナへのpush_backや
 * push_front等の増減時に自動更新されます。\n
 * find_by_keyでの第一引数はフィールド名のみ対応しています。
 * フィールド名以外で使用したい場合はget_key_valueをオーバーライド
 * することで対応できます。
 *
 * Row系オブジェクトの変更に追従することはできません。\n
 * 変更される恐れがある場合はdelete_indexでインデックスを削除してから
 * find_by_keyをする必要があります。\n
 * ただし、コンテナへの格納数が大きい場合は非効率なため、変更がない値
 * をキーとするように努める必要があります。
 */
class CIndexedContainer : public CContainerBase
{
public:
	typedef std::set<iterator> IteratorContainer;
	typedef std::map<KeyList,std::map<ValueList,IteratorContainer, ValueListLess>,KeyListLess> KeyCacheContainer;

	CIndexedContainer(void);
	virtual ~CIndexedContainer(void);

	CIndexedContainer& operator=( const CIndexedContainer& con );

	virtual void push_back( const CSPContainerBase& cContainer, bool bSecureUnique = true );
	virtual void push_front( const CSPContainerBase& cSPContainer, bool bSecureUnique = true );
	virtual iterator insert( iterator it, const CSPContainerBase& cContainer, bool bSecureUnique = true );
	virtual void clear();
	virtual iterator erase( iterator it );
	virtual iterator erase( iterator first, iterator last );

	IteratorContainer& finds_by_key( LPCTSTR lpcszTag, const CComVariant& key );
	const IteratorContainer& finds_by_key( LPCTSTR lpcszTag, const CComVariant& key ) const;
	IteratorContainer& finds_by_key( const std::pair<CString,CString>& tag, const std::pair<CComVariant,CComVariant>& key );
	const IteratorContainer& finds_by_key( const std::pair<CString,CString>& tag, const std::pair<CComVariant,CComVariant>& key ) const;
	IteratorContainer& finds_by_key( const KeyList& keys, const ValueList& vals );
	const IteratorContainer& finds_by_key( const KeyList& keys, const ValueList& vals ) const;

	iterator find_by_key( LPCTSTR lpcszTag, const CComVariant& key ) {
		IteratorContainer& _lst = finds_by_key( lpcszTag, key );
		return ( _lst.empty() ? end() : *_lst.begin() );
	}
	const_iterator find_by_key( LPCTSTR lpcszTag, const CComVariant& key ) const {
		const IteratorContainer& _lst = finds_by_key( lpcszTag, key );
		return ( _lst.empty() ? end() : *_lst.begin() );
	}
	iterator find_by_key( const std::pair<CString,CString>& tag, const std::pair<CComVariant,CComVariant>& key ) {
		IteratorContainer& _lst = finds_by_key( tag, key );
		return ( _lst.empty() ? end() : *_lst.begin() );
	}
	const_iterator find_by_key( const std::pair<CString,CString>& tag, const std::pair<CComVariant,CComVariant>& key ) const {
		const IteratorContainer& _lst = finds_by_key( tag, key );
		return ( _lst.empty() ? end() : *_lst.begin() );
	}
	iterator find_by_key( const std::pair<CString,CString>& tag, const std::pair<long,CString>& key ) {
		IteratorContainer& _lst = finds_by_key( tag, std::make_pair(CComVariant(key.first),CComVariant(key.second)) );
		return ( _lst.empty() ? end() : *_lst.begin() );
	}
	const_iterator find_by_key( const std::pair<CString,CString>& tag, const std::pair<long,CString>& key ) const {
		const IteratorContainer& _lst = finds_by_key( tag, std::make_pair(CComVariant(key.first),CComVariant(key.second)) );
		return ( _lst.empty() ? end() : *_lst.begin() );
	}
	iterator find_by_key( const KeyList& key, const ValueList& val ) {
		IteratorContainer& _lst = finds_by_key( key, val );
		return ( _lst.empty() ? end() : *_lst.begin() );
	}
	const_iterator find_by_key( const KeyList& key, const ValueList& val ) const {
		const IteratorContainer& _lst = finds_by_key( key, val );
		return ( _lst.empty() ? end() : *_lst.begin() );
	}

	void delete_index( LPCTSTR lpcszTag ) const;
	void delete_index( const std::pair<CString,CString>& tag ) const;
	void delete_index( const KeyList& key ) const;
	void delete_index() const;
	void update_index( iterator it ) const {
		delete_key( it );
		add_key( it );
	}
	/**
	 * @brief 自動キャッシュ設定
	 * 
	 * 自動キャッシュ設定をすると、add_keyが呼ばれた際に
	 * コンテナが空の場合にのみ全件をキャッシュします。
	 * 自動キャッシュの振る舞いを変更したい場合はadd_keyを
	 * オーバーライドします。
	 * 一件でも存在するとこの機能は無効になりますので注意してください。
	 */
	void SetAutoCache( sindyContainerAutoCacheType emType ){ m_emCacheType = emType; }
	sindyContainerAutoCacheType GetAutoCache() const { return m_emCacheType; }
protected:
	void create_index( LPCTSTR lpcszTag ) const;
	void create_index( LPCTSTR lpcszTag, const CComVariant& key ) const;
	void create_index( const std::pair<CString,CString>& tag ) const;
	void create_index( const std::pair<CString,CString>& tag, const std::pair<CComVariant,CComVariant>& key ) const;
	void create_index( const KeyList& key ) const;
	void create_index( const KeyList& key, const ValueList& val ) const;
	virtual void add_key( iterator& it ) const;
	virtual void delete_key( iterator& it ) const;
protected:
	mutable KeyCacheContainer m_listIndex; //!< key: インデックスキーワード value: { key: インデックス値 value: イテレータ }
	sindyContainerAutoCacheType m_emCacheType; //!< 自動キャッシュタイプ（デフォルト：sindyContainerAutoCacheNone）
};

} // sindy
