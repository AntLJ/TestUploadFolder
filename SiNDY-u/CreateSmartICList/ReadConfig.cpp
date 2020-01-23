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

#include "StdAfx.h"
#include "ReadConfig.h"

namespace{
//文字列の末尾に\マークがあった場合はカットした文字列を返す
CString CutEndYenMark(CString str)
{
	int len = str.GetLength();
	int index = str.Find(_T("\\"), len-1);
	if(index != -1){
		CString str2 = 	str.Left(len-1);
		return str2;
	}
	return str;
}

/*
 * @brief	Meshlistよりメッシュコードの取得
 */
bool GetMeshlist(CString strMeshlistPath, std::set<long>& r_MeshlistSet)
{
	//ファイルの読み込み
	std::ifstream aStream(strMeshlistPath);

	if( !aStream ){
		std::cout << std::endl;
		std::cout << LPCTSTR(strMeshlistPath) << "\tを読み込むことができません." << std::endl;
		return false;
	}

	//1行ずつ読み込み
	char	aLineBuffer[128];
	while( aStream.good() ){
		aStream.getline(aLineBuffer,128);
		if(( strlen(aLineBuffer))&&( aLineBuffer[0] == '#')){
			continue;
		}
		int	aMeshcode = atoi(aLineBuffer);
		if( r_MeshlistSet.find(aMeshcode) == r_MeshlistSet.end() && aMeshcode < 700000 && aMeshcode > 300000 ){
			r_MeshlistSet.insert(aMeshcode); 
		}
		else if(aMeshcode != 0){
			std::cout << _T("\t指定されたメッシュコードが不正.") << std::endl;
			return false;
		}
	}
	aStream.close();

	return true;
}

/*
 *	@brief	設定ファイルの表示
 *	@param	eParams	[in]	パラメータ
 */
void dispConfig(const Parameters& eParams)
{
	std::cout << _T("道路接続サーバ:\t") << LPCTSTR(eParams.m_strNwDB) << std::endl;
	std::cout << _T("出力先:\t") << LPCTSTR(eParams.m_strOut) << std::endl;
	std::cout << _T("対象メッシュ:\t") << eParams.m_lMeshSet.size() << _T(" メッシュ") << std::endl;
	std::cout << _T("--") << std::endl;
}
} //anonymous-namespace


CReadConfig::CReadConfig(void)
{
}

CReadConfig::~CReadConfig(void)
{
}

//設定ファイル情報の取得
Parameters CReadConfig::getenv(const char* strConfigFile)
{
	Parameters eParams;

	FILE *fp;
	if(! fopen_s(&fp, strConfigFile, "r")){
		char    aStr[256];
		while(fgets(aStr, 256, fp) != NULL){
			CString strLine = aStr;
			strLine.Replace(_T('\n'), _T('\0'));

			if(strLine.Left(0) == _T("#")) 
				continue;

			if(aStr[0] == '#')	continue;

			int iStart = 0;
			CString strToken = NULL;
			strToken = strLine.Tokenize(_T("\t"),iStart);
			if(strToken == _T(""))
				continue;
			CString strToken2 = strLine.Tokenize(_T("\t"),iStart);

			//< # 対象NWDB名
			if(strToken == _T("NwDB")){
				eParams.m_strNwDB = strToken2;
			}
			//< # メッシュリストパス
			else if(strToken == _T("Meshlist")){
				if(! GetMeshlist(strToken2, eParams.m_lMeshSet))
					throw std::runtime_error(_T("メッシュリストの取得に失敗\t正しいメッシュリストを指定して下さい."));
			}
			//< # 出力
			else if(strToken == _T("Out")){
				CString tmp = strToken2;
				eParams.m_strOut = CutEndYenMark(tmp);
			}
			else{
				continue;
			}
		}
		(void)fclose(fp);
	}
	else{
		throw std::runtime_error(_T("設定ファイルの読み込みに失敗."));
	}
	if(eParams.m_strNwDB.GetLength() == 0){
		throw std::runtime_error(_T("道路接続サーバが未設定\tNwDBで指定して下さい."));
	}
	if(eParams.m_strOut.GetLength() == 0){
		throw std::runtime_error(_T("出力先が未設定\tOutで指定して下さい."));
	}
	if(eParams.m_lMeshSet.empty()){
		throw std::runtime_error(_T("メッシュリストが未設定\tMeshlistで指定して下さい."));
	}

	dispConfig(eParams);

	return eParams;

}
