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


#include "stdafx.h"
#include "DeleteRows.h"
#include "WinLib/VersionInfo.h"
#include "sindy/workspace.h"
#include <ArcHelperEx/GlobalFunctions.h>

using namespace std;

/**
	* @brief	初期化
	* @param	cArgc	[in]	引数の数
	* @param	cArgv	[in]	引数
	* @return	bool
	*/
bool DeleteRows::Init()
{
	m_ofs.open( strLog );
	if( !m_ofs ){
		cerr << "#ERROR ログファイルのオープンに失敗" << endl;
		return false;
	}
	
	ifstream	fin;
	fin.open( strFile );
	if( !fin )
	{
		cerr << "#Error ファイルをオープンできません" << endl;
		return false;
	}

	int		i = 0;
	while(1)
	{
		char	acBuf[1024];
		i++;

		fin.getline( acBuf, 1024 );

		// 終端ならばループを抜ける
		if( fin.eof() )
			break;

		if( !fin )
		{
			cerr << "【 " << i << " 行目】読み込み失敗" << endl;
			fin.close();
			return false;
		}
		string	asBuf	= acBuf;
		if(asBuf.find_first_not_of("0123456789") != string::npos)
		{
			cerr << "【 " << i << " 行目】不正なID( " << asBuf << " )" << endl;
			fin.close();
			return false;
		}
		long	aOID = atol( asBuf.c_str() );
		m_lOIDList.push_back( aOID );
	}
	fin.close();

	fnPrintHeader();
	return true;
}

/**
	* @brief	実行
	*/
bool DeleteRows::Execute()
{
	if( !Init() ){
		cerr << "#ERROR 初期化に失敗" << endl;
	}

	// 入力FGDBワークスペースオープン
	m_ipWorkspace = sindy::create_workspace( strInput );

	if( !m_ipWorkspace ){
		cerr << "#ERROR DB接続失敗" << endl;
		return false;
	}
	m_ofs << "#DB:" << CT2CA(strInput) << endl;

	((IFeatureWorkspacePtr)m_ipWorkspace)->OpenTable( _bstr_t( strFcname ), &m_ipTable );
	if( !m_ipTable ){
		cerr << "#ERROR フィーチャクラスオープン失敗" << endl;
		return false;
	}

	m_ofs << "#FC:" << CT2CA(strFcname) << endl;

	if( !fnEditStart() ){
		return false;
	}

	fnDelete();

	if( !fnEditStop() ){
		return false;
	}

	m_ofs << "#正常終了" << endl;

	return true;
}

/**
	* @brief	ログにヘッダーを出力
	*/
void DeleteRows::fnPrintHeader()
{
	// 固定部分の出力
	m_ofs << "# FREESTYLELOG" << endl;

	CVersion cVer;
	m_ofs << "# " << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " << CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;

	m_ofs << "LAYER\tOBJECTID\tSTATUS" << endl;
}

/**
	* @brief	編集開始
	* @return	bool
	*/
bool DeleteRows::fnEditStart()
{
	// 編集開始（Undo Redo がいらないため、false）
	if( S_OK != m_ipWorkspace->StartEditing( VARIANT_FALSE ) || S_OK != m_ipWorkspace->StartEditOperation() ){
		m_ofs << "#ERROR 編集開始に失敗" << endl;
		return false;
	}
	return true;
}

/**
	* @brief	編集終了
	* @return	bool
	*/
bool DeleteRows::fnEditStop()
{
	// 編集確定
	if( S_OK != m_ipWorkspace->StopEditOperation() ){
		m_ofs << "#ERROR 編集保存に失敗" << endl;
		m_ipWorkspace->AbortEditOperation();
		return false;
	}
	
	// 編集終了（保存する必要があるため、true）
	if( S_OK != m_ipWorkspace->StopEditing( VARIANT_TRUE ) ){
		m_ofs << "#ERROR 編集終了に失敗" << endl;
		return false;
	}
	return true;
}

/**
	* @brief	削除処理
	* @return	bool
	*/
void DeleteRows::fnDelete()
{
	for( auto& oID : m_lOIDList ){

		CString strWhere;
		strWhere.Format( _T("OBJECTID = %d"), oID );

		IQueryFilterPtr ipQuery( AheInitQueryFilter(NULL, NULL, strWhere) );

		_ICursorPtr ipCursor;
		m_ipTable->Search( ipQuery, VARIANT_FALSE, &ipCursor );

		if( !ipCursor ){
			m_ofs << CT2CA(strFcname) << "\t" << oID << "\t検索失敗" << endl;
			continue;
		}

		_IRowPtr ipRow;
		if( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){

			if( SUCCEEDED( ipRow->Delete() )){
				m_ofs << CT2CA(strFcname) << "\t" << oID << "\t削除OK" << endl;
			}
			else{
				m_ofs << CT2CA(strFcname) << "\t" << oID << "\t削除失敗" << endl;
			}
		}
		else{
			m_ofs << CT2CA(strFcname) << "\t" << oID << "\tDB上に存在しない" << endl;
		}
	}
}

