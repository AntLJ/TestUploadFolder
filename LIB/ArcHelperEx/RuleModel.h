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

#include <boost/tuple/tuple.hpp>

/**
 * @class CRuleModelNetwork
 * @brief ネットワークモデルのテーブル名を定義するクラス
 *
 * 新しいネットワークモデルを定義したい場合は、InitRule()に
 * 追加してください。
 */
class CRuleModelNetwork
{
public:
	CRuleModelNetwork();
	virtual ~CRuleModelNetwork();

	typedef std::list<std::pair<CString,CString> > TRuleContainer; //!< ルールコンテナの型
	typedef TRuleContainer::iterator iterator;
	typedef TRuleContainer::const_iterator const_iterator;
	iterator begin(){ return m_cRule.begin(); }
	const_iterator begin() const { return m_cRule.begin(); }
	iterator end(){ return m_cRule.end(); }
	const_iterator end() const { return m_cRule.end(); }

	bool IsNetworkLink( LPCTSTR lpcszName ) const;
	bool IsNetworkNode( LPCTSTR lpcszName ) const;
	CString GetPairName( LPCTSTR lpcszName ) const;
private:
	void InitRule();
	void ClearRule();
private:
	TRuleContainer m_cRule; //!< first:ライン second:ポイント
};

/**
 * @class CRuleRelationshipContain
 * @brief 包含関係のテーブル名を定義するクラス
 *
 * 新しい包含関係を定義したい場合は、InitRule()に
 * 追加してください。
 */
class CRuleRelationshipContain
{
public:
	CRuleRelationshipContain();
	virtual ~CRuleRelationshipContain();
	typedef std::list<std::pair<CString,CString> > TRuleValue; //!< ルールコンテナの値の型
	typedef TRuleValue::iterator value_iterator;
	typedef TRuleValue::const_iterator const_value_iterator;
	typedef std::map<CString,TRuleValue> TRuleContainer; //!< ルールコンテナの型
	typedef TRuleContainer::iterator iterator;
	typedef TRuleContainer::const_iterator const_iterator;
	iterator begin(){ return m_cRule.begin(); }
	const_iterator begin() const { return m_cRule.begin(); }
	iterator end(){ return m_cRule.end(); }
	const_iterator end() const { return m_cRule.end(); }
	value_iterator begin( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].begin(); }
	const_value_iterator begin( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].begin();
	}
	value_iterator end( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].end(); }
	const_value_iterator end( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].end();
	}
	iterator find( LPCTSTR lpcszTableName );
	iterator find_value( LPCTSTR lpcszTableName );
	/// 「含む」側テーブル名をキーとして検索
	const_iterator find( LPCTSTR lpcszTableName ) const;
	/**
	 * @brief 「含まれる」側テーブル名をキーとして検索
	 * 返されるイテレータは「含む」側である事に注意してください
	 */
	const_iterator find_value( LPCTSTR lpcszTableName ) const;
private:
	void InitRule();
	void ClearRule();
private:
	TRuleContainer m_cRule; // key:含む方 valuelist.first: 含まれる方 second:外部キー
};

/**
 * @class CRuleRelationTable
 * @brief 外部テーブルとの関連を定義するクラス
 *
 * 新しい関係を定義したい場合は、InitRule()に
 * 追加してください。
 */
class CRuleRelationTable
{
public:
	CRuleRelationTable();
	virtual ~CRuleRelationTable();

	typedef std::list<boost::tuple<CString,CString,CString> > TRuleValue; //!< ルールコンテナの値の型
	typedef TRuleValue::iterator value_iterator;
	typedef TRuleValue::const_iterator const_value_iterator;
	typedef std::map<CString,TRuleValue> TRuleContainer; //!< ルールコンテナの型
	typedef TRuleContainer::iterator iterator;
	typedef TRuleContainer::const_iterator const_iterator;
	iterator begin(){ return m_cRule.begin(); }
	const_iterator begin() const { return m_cRule.begin(); }
	iterator end(){ return m_cRule.end(); }
	const_iterator end() const { return m_cRule.end(); }
	value_iterator begin( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].begin(); }
	const_value_iterator begin( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].begin();
	}
	value_iterator end( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].end(); }
	const_value_iterator end( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].end();
	}
private:
	void InitRule();
	void ClearRule();
private:
	TRuleContainer m_cRule; // key:ベーステーブル名 valuelist.first:関連フラグフィールド名 valuelist.second:関連テーブル名 valuelist.third:外部キー
};

/**
 * @class CRuleLinkQueue
 * @brief リンク列とその関連を定義するクラス
 *
 * 新しい関係を定義したい場合は、InitRule()に
 * 追加してください。
 */
class CRuleLinkQueue
{
public:
	CRuleLinkQueue();
	virtual ~CRuleLinkQueue();

	typedef std::list<boost::tuple<CString,CString,CString,CString> > TRuleValue; //!< ルールコンテナの値の型
	typedef TRuleValue::iterator value_iterator;
	typedef TRuleValue::const_iterator const_value_iterator;
	typedef std::map<CString,TRuleValue> TRuleContainer; //!< ルールコンテナの型
	typedef TRuleContainer::iterator iterator;
	typedef TRuleContainer::const_iterator const_iterator;
	iterator begin(){ return m_cRule.begin(); }
	const_iterator begin() const { return m_cRule.begin(); }
	iterator end(){ return m_cRule.end(); }
	const_iterator end() const { return m_cRule.end(); }
	value_iterator begin( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].begin(); }
	const_value_iterator begin( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].begin();
	}
	value_iterator end( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].end(); }
	const_value_iterator end( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].end();
	}
private:
	void InitRule();
	void ClearRule();
private:
	TRuleContainer m_cRule;
};

extern const CRuleModelNetwork        g_cRuleModelNetwork;        //!< ネットワークモデル定義クラス
extern const CRuleRelationshipContain g_cRuleRelationshipContain; //!< 内包関連定義クラス
extern const CRuleRelationTable       g_cRuleRelationTable;       //!< 外部テーブル参照定義クラス
extern const CRuleLinkQueue           g_cRuleLinkQueue;           //!< リンクキュー定義クラス
