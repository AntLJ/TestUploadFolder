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

#include <set>
#include <stdio.h>
#include <windows.h>

using namespace std;

/// メッシュコードのコンテナ
struct MeshListCont
{

	char	eMeshCode[9];		///< メッシュコード
	long	eObjectID;			///< メッシュポリゴンのオブジェクトID

	/// コンストラクタ
	MeshListCont() {
		memset(eMeshCode, '\0', 9);
		eObjectID = 0;
	}

	/// コンストラクタ（初期値指定）
	MeshListCont(char *str, long cOID = 0) {
		memset(eMeshCode, '\0', 9);
		strcpy(eMeshCode, str);
		eObjectID = cOID;
	}

	/// コンストラクタ（初期値指定，整数値）
	MeshListCont(long cVal, long cOID = 0) {
		memset(eMeshCode, '\0', 9);
		sprintf(eMeshCode, "%ld", cVal);
		eObjectID = cOID;
	}

	/// 代入
	bool operator=(const char *str) {
		memset(eMeshCode, '\0', 9);
		strcpy(eMeshCode, str);
		eObjectID = 0;
	}

	/// 比較
	bool operator<(const MeshListCont &mlc) const {
		if(strcmp(eMeshCode, mlc.eMeshCode) < 0) return 1;
		else return 0;
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
	bool	Open(char *cFileName);

	/// メッシュリストファイルの作成
	/**
	* @param cFileName [in] ファイル名
	*
	* @retval true 成功
	* @retval false 失敗
	*/
	bool	Create(char *cFileName);

	/// リストへのメッシュコードの追加
	/**
	* @param cMeshCode [in] メッシュコード（文字列型）
	*
	* @retval true 成功
	* @retval false 失敗（入力値が範囲外）
	*/
	bool	Add(char *cMeshCode);

	/// リストへのメッシュコードの追加
	/**
	* @param cMeshCode [in] メッシュコード（整数型）
	*
	* @retval true 成功
	* @retval false 失敗（入力値が範囲外）
	*/
	bool	Add(LONG cMeshCode);

};

#endif // !defined(AFX_MESHLIST_H__D38CA68D_9F8C_4C73_96DD_293BE42C74C4__INCLUDED_)
