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

//------------------------------------------------------
// 字句で文字列を分割する（元の文字列を分割しない）
// @return  取り出した文字列の個数
//------------------------------------------------------
int util_class::
DivbyToken(	char		*c_cpString,	///< 検索対象文字列
			char		*c_cpToken,		///< 区切り文字列
			int			c_iStrNum,		///< 先頭から何番目の文字列から取り出すか
			int			c_iN,			///< 何個の文字列を取り出すか
			string*		c_cpStrPtr)		///< 取り出した文字列のポインタ
{
	const int	bmax = 512;
	char	*pStr;                          // 文字列のポインタ
	char	pCopyBuf[bmax];                 // 取り出した文字列をコピーするバッファ
	int		i=1;                            // ループのカウント
	int		j=0;                            // 取り出した文字列のカウント
	char	*tmp_str	= NULL;             // データをコピーしておく
	int		len			= strlen(c_cpString);  // 文字列長

	tmp_str     = new char[len+1];
	memset      (tmp_str, '\0', len+1);
	strcpy      (tmp_str, c_cpString);

	while(1){

		// 文字列取り出し
		if(i == 1){
			pStr = strtok(tmp_str, c_cpToken);
		}else{
			pStr = strtok(NULL, c_cpToken);
		}
		if(pStr == NULL){
			delete [] tmp_str;
			return(j);  // 取り出した文字列の個数を返す
		}
		strcpy(pCopyBuf, pStr);     // strtokが返す文字列をコピーする

		if(j == c_iN){
			delete [] tmp_str;
			return(j);  // 取り出した文字列の個数を返す
		}
		if(i < c_iStrNum){    // 先頭からＮ番目からのデータの取り出しを行なう
			i++;
			continue;
		}
		c_cpStrPtr[j]	= pCopyBuf;
	//	strcpy(c_cpStrPtr[j].eStr, pCopyBuf);  //  取り出した文字列を外部の配列へコピーする
		i++;
		j++;
	}
}

/**
	UNICODEをSJISに変換する
*/
string	util_class::
Str_to_SJIS		(	CComBSTR&	c_CStr,		///< 文字列(Unicode)
			 		int			c_iMode	)	///< 文字変換モード
{

	USES_CONVERSION;

	string		ret_str;

	if( c_iMode == SJIS ) {
		_bstr_t		a_bTmp1	( c_CStr, false );
		if( (char *)a_bTmp1 == NULL ) {
			ret_str	= "";
		}else {
			ret_str	= (char *)a_bTmp1;
		}
	}else {
		if (c_CStr != 0) {
			ret_str = OLE2T(c_CStr);
		}
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
