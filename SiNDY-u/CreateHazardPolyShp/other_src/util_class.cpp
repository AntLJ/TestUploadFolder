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

#include "util_class.h"

string	util_class::
Str_to_SJIS		(	CComBSTR&	c_CStr,		///< 文字列(Unicode)
			 		int			c_iMode	)	///< 文字変換モード
{

	USES_CONVERSION;

	string		ret_str;

	if( c_iMode == Sjis ) {
		_bstr_t		a_bTmp1	( c_CStr, false );
		if( (char *)a_bTmp1 == NULL ) {
			ret_str	= "";
		}else {
			ret_str	= (char *)a_bTmp1;
		}
	}else {
		_bstr_t a_bTmpStr((*c_CStr));

		ret_str	= a_bTmpStr;
	}
	return	(ret_str);
}

//-----------------------------------------------------------
// メッシュコードのリストを作成する
// @return	成功	true
// @return	失敗	false
//-----------------------------------------------------------
bool	util_class::
MakeMeshList(	char*		c_cpRFName,	///< メッシュリストファイル名[IN]
				set<int>*	c_spMList)	///< メッシュリストへのポインタ[OUT]
{
	const int	bmax = 512;
	FILE		*a_fpRFp;		
	char		a_cpRBuf[bmax];
	int			a_iMeshCode;		// メッシュコード
	string		a_sDivStr[2];		// 分割した文字列
	int			a_iStrLen = 0;	

	a_fpRFp	= fopen(c_cpRFName, "r");
	if(a_fpRFp == NULL) {
		fprintf	( stderr, "メッシュテーブルファイル <%s> が開けません!!\n", c_cpRFName);
		return	( false );
	}

	while(fgets(a_cpRBuf, bmax, a_fpRFp) != NULL)
	{
		a_iStrLen	= strlen(a_cpRBuf);
		// 最後の"\n"を取り除く
		if(a_cpRBuf[a_iStrLen - 1] == '\n') {
			a_cpRBuf[a_iStrLen - 1] = '\0';
		}

		// コメント文を飛ばす
		if(a_cpRBuf[0] == '#' || a_cpRBuf[0] == '\0') {
			continue;
		}

		// メッシュコード
		a_iMeshCode	= atoi((char*)a_cpRBuf);
		// リストに追加
		c_spMList->insert(a_iMeshCode);
	}
	fclose	( a_fpRFp );
	return	( true );
}
