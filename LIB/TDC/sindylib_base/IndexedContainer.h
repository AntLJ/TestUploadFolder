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

/// VARIANT���L�[�ɂ���ꍇ�̒ǉ���`
struct VariantLess {
	bool operator()( const CComVariant& va1, const CComVariant& va2 ) const;
};

inline bool VariantLess::operator()( const CComVariant& va1, const CComVariant& va2 ) const {
	// VARIANT��r�̑O�ɁAVT�̔�r������
	if( va1.vt != va2.vt )
		return va1.vt < va2.vt;

	// VT�������ȏꍇ�AVT_EMPTY��VT_NULL�͒��g�̔�r�����Ă����傤���Ȃ��̂�
	// �����Ƃ݂Ȃ�
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
	// VARIANT��r�̑O�ɁAVT�̔�r������
	if( va1.vt != va2.vt )
		return false;

	// VT�������ȏꍇ�AVT_EMPTY��VT_NULL�͒��g�̔�r�����Ă����傤���Ȃ��̂�
	// �����Ƃ݂Ȃ�
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

/// CIndexedContainer�Ŏg�p���鎩���L���b�V���̐U�镑����`
enum sindyContainerAutoCacheType {
	sindyContainerAutoCacheNone,     //!< �����L���b�V�����Ȃ�
	sindyContainerAutoCacheNoDetach, //!< �S�Ď����L���b�V������iDetach�Ȃ��j
	sindyContainerAutoCacheDetach,   //!< �S�Ď����L���b�V������iDetach����j
	sindyContainerAutoOnDemandCacheNoDetach, //!< �v���������������������L���b�V������iDetach�Ȃ��j
	sindyContainerAutoOnDemandCacheDetach,   //!< �v���������������������L���b�V������iDetach����j
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
 * @brief �C���f�b�N�X�ɂ��R���e�i�ւ̃A�N�Z�X��
 * �ړI�Ƃ����N���X
 *
 * ���̃N���X��Row�n�I�u�W�F�N�g�i�[�R���e�i��p�ł��B\n
 * CSchemaSupport���p�����Ă���K�v������܂��B\n
 * �C���f�b�N�X�́Afind_by_key���Ă΂ꂽ���Ɏn�߂č쐬����܂��B
 * ��x�C���f�b�N�X���쐬�����ƁA����Ȍ�̃R���e�i�ւ�push_back��
 * push_front���̑������Ɏ����X�V����܂��B\n
 * find_by_key�ł̑������̓t�B�[���h���̂ݑΉ����Ă��܂��B
 * �t�B�[���h���ȊO�Ŏg�p�������ꍇ��get_key_value���I�[�o�[���C�h
 * ���邱�ƂőΉ��ł��܂��B
 *
 * Row�n�I�u�W�F�N�g�̕ύX�ɒǏ]���邱�Ƃ͂ł��܂���B\n
 * �ύX����鋰�ꂪ����ꍇ��delete_index�ŃC���f�b�N�X���폜���Ă���
 * find_by_key������K�v������܂��B\n
 * �������A�R���e�i�ւ̊i�[�����傫���ꍇ�͔�����Ȃ��߁A�ύX���Ȃ��l
 * ���L�[�Ƃ���悤�ɓw�߂�K�v������܂��B
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
	 * @brief �����L���b�V���ݒ�
	 * 
	 * �����L���b�V���ݒ������ƁAadd_key���Ă΂ꂽ�ۂ�
	 * �R���e�i����̏ꍇ�ɂ̂ݑS�����L���b�V�����܂��B
	 * �����L���b�V���̐U�镑����ύX�������ꍇ��add_key��
	 * �I�[�o�[���C�h���܂��B
	 * �ꌏ�ł����݂���Ƃ��̋@�\�͖����ɂȂ�܂��̂Œ��ӂ��Ă��������B
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
	mutable KeyCacheContainer m_listIndex; //!< key: �C���f�b�N�X�L�[���[�h value: { key: �C���f�b�N�X�l value: �C�e���[�^ }
	sindyContainerAutoCacheType m_emCacheType; //!< �����L���b�V���^�C�v�i�f�t�H���g�FsindyContainerAutoCacheNone�j
};

} // sindy
