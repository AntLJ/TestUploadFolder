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

// CityList.h: CityList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CITYLIST_H__6EDD6F68_3FAE_4F51_80DE_B01C8CC00BBF__INCLUDED_)
#define AFX_CITYLIST_H__6EDD6F68_3FAE_4F51_80DE_B01C8CC00BBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <stdio.h>
#include <windows.h>
#include <string.h>

using namespace std;

/// 市区町村コードのコンテナ
struct CityListCont
{

	char	eCityCode[6];		///< 市区町村コード
	char	eAddrCode[7];		///< 住所コード（11桁コード下6桁）

	/// コンストラクタ
	CityListCont() {
		memset(eCityCode, '\0', 6);
		memset(eAddrCode, '\0', 7);
	}

	/// コンストラクタ（初期値指定）
	CityListCont(char *str) {
		strncpy(eCityCode, str, 5);
		eCityCode[5] = '\0';
		if( strlen( str ) > 5 )
		{
		strncpy(eAddrCode, &str[5], 6);
		eAddrCode[6] = '\0';
		}
	}

	/// 代入
	bool operator=(const char *str) {
		strncpy(eCityCode, str, 5);
		eCityCode[5] = '\0';
		if( strlen( str ) > 5 )
		{
		strncpy(eAddrCode, &str[5], 6);
		eAddrCode[6] = '\0';
		}
	}

	/// 比較
	bool operator<(const CityListCont &clc) const {
		if(strcmp(eCityCode, clc.eCityCode) != 0)
		{
			if(strcmp(eCityCode, clc.eCityCode) < 0) 
				return true;
		}
		else
		{
			if(strcmp(eAddrCode, clc.eAddrCode) != 0)
			{
				if(strcmp(eAddrCode, clc.eAddrCode) < 0)
					return true;
			}
		}
		return false;
	}

};

typedef set<CityListCont, less<CityListCont> >				CityListSet;
typedef set<CityListCont, less<CityListCont> >::iterator	CityListSet_i;

class CityList : public CityListSet
{

protected:

	FILE	*mCityListFP;		///< 市区町村コードリストのファイルポインタ

public:

	/// コンストラクタ
	CityList();

	/// デストラクタ
	~CityList();

	/// 市区町村コードリストファイルのオープン
	/**
	* @param cFileName [in] ファイル名
	*
	* @retval true 成功
	* @retval false 失敗
	*/
	bool	Open(char *cFileName);

	/// リストへの市区町村コードの追加
	/**
	* @param cMeshCode [in] 市区町村コード（文字列型）
	*
	* @retval true 成功
	* @retval false 失敗（入力値が範囲外）
	*/
	bool	Add(char *cCityCode);

	/// リストへの市区町村コードの追加
	/**
	* @param cCityCode [in] 市区町村コード（整数型）
	*
	* @retval true 成功
	* @retval false 失敗（入力値が範囲外）
	*/
	bool	Add(LONG cCityCode);

};

#endif // !defined(AFX_CITYLIST_H__6EDD6F68_3FAE_4F51_80DE_B01C8CC00BBF__INCLUDED_)
