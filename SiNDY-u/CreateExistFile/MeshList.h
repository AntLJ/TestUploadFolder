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

// MeshList.h: MeshList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHLIST_H__D38CA68D_9F8C_4C73_96DD_293BE42C74C4__INCLUDED_)
#define AFX_MESHLIST_H__D38CA68D_9F8C_4C73_96DD_293BE42C74C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <string>
#include <set>
#include <stdio.h>
#include <windows.h>

using namespace std;

/// メッシュコードのコンテナ
struct MeshListCont
{

	string	eMeshCode;		///< メッシュコード

	/// コンストラクタ
	MeshListCont() {
	}

	/// コンストラクタ（初期値指定）
	MeshListCont(const string &str) {
		eMeshCode	= str;
	}

	/// 代入
	MeshListCont& operator = (const string &str) {
		eMeshCode	= str;
		return *this;
	}

	/// 比較
	bool operator < (const MeshListCont &mlc) const {
		return (eMeshCode < mlc.eMeshCode);
	}

};

typedef set<MeshListCont, less<MeshListCont> >				MeshListSet;
typedef set<MeshListCont, less<MeshListCont> >::iterator	MeshListSet_i;

/// メッシュリストクラス
class MeshList : public MeshListSet
{

protected:

	FILE	*mMeshListFP;		///< メッシュリストファイルのファイルポインタ

public:

	/// コンストラクタ
	MeshList();

	/// デストラクタ
	~MeshList();

	/// メッシュリストファイルのオープン
	/**
	* @param cFileName [in] ファイル名
	*
	* @retval true 成功
	* @retval false 失敗
	*/
	bool	Open(const char *cFileName);

	/// リストへのメッシュコードの追加
	/**
	* @param cMeshCode [in] メッシュコード（文字列型）
	*
	* @retval true 成功
	* @retval false 失敗（入力値が範囲外）
	*/
	bool	Add(const char *cMeshCode);

	/// リストへのメッシュコードの追加
	/**
	* @param cMeshCode [in] メッシュコード（整数型）
	*
	* @retval true 成功
	* @retval false 失敗（入力値が範囲外）
	*/
	bool	Add(long cMeshCode);

};

#endif // !defined(AFX_MESHLIST_H__D38CA68D_9F8C_4C73_96DD_293BE42C74C4__INCLUDED_)
