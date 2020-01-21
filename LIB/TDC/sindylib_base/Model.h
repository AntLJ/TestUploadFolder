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

struct stModel {
	CString ParentName; //!< 親側の名前
	CString ParentKey;  //!< 親側のキー名
	CString Name;       //!< 子側の名前
	CString OuterKey;   //!< 子側のキー名
	CString ParentStrictKey; //!< 親側の制約キー名   例：feature_relとpoi_pointの場合はgeometrytype_c
	CComVariant ParentStrictValue; //!< 親側の制約値 例：feature_relとpoi_pointの場合は1L
};

class CModel
{
public:
	CModel();
	virtual ~CModel();

	typedef std::multimap<CString,stModel> TYPEDEF_MODEL;
	typedef std::pair<CString,stModel> TYPEDEF_MODEL_PAIR;
	typedef TYPEDEF_MODEL::iterator iterator;
	typedef TYPEDEF_MODEL::const_iterator const_iterator;

	/// モデルのトップから末端までをシリアルに取得する
	std::list<CString> GetOrderdNameList( LPCTSTR lpcszTopLevelParentName ) const;

	/// 親に当たるモデル名を取得する
	std::list<CString> GetParent( LPCTSTR lpcszName ) const;

	/// 子にあたるモデル名を取得する
	std::list<CString> GetChild( LPCTSTR lpcszName ) const;

	/// モデルを追加する
	void Add( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey ) {
		AddModel( lpcszChild, lpcszChildKey, lpcszParent, lpcszParentKey );
	}
	void Add( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey, LPCTSTR lpcszParentStrictKey, const CComVariant& vaStrictValue );

	/// 子のモデル名からキー等を取得する
	std::list<stModel> GetKeys( LPCTSTR lpcszName ) const;

	/// 親のモデル名からキー等を取得する
	std::list<stModel> GetKeysByParentName( LPCTSTR lpcszName ) const;

	/// 親子のモデル名からキー等を取得する
	stModel GetKey( LPCTSTR lpcszName, LPCTSTR lpcszParent ) const;

	iterator begin() { return m_model.begin(); }
	const_iterator begin() const { return m_model.begin(); }
	iterator end() { return m_model.end(); }
	const_iterator end() const { return m_model.end(); }
protected:
	TYPEDEF_MODEL::iterator AddModel( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey );
protected:
	TYPEDEF_MODEL m_model;
};
