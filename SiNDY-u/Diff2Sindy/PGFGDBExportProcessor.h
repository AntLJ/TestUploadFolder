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
#include <TDC/sindylib_core/Workspace.h>
#include "Processor.h"
#include "DBReplicater.h"
#include "TableNamesSearcher.h"
#include "Notifier.h"

using namespace sindy;

/**
 * @brief 複数PGFGDBエクスポートクラス
 */
template <typename T>
class PGFGDBExportProcessor : public Processor<T>
{
public:

	PGFGDBExportProcessor(const CWorkspace& workspace, const uh::tstring& folder) : Processor<T>(), m_workspace(workspace), m_strFolder(folder){};

	virtual ~PGFGDBExportProcessor(){};

	/**
	 * @brief  インポート処理実行
	 * @param  src  [in]  メッシュコード
	 * @retval true:処理成功
	 */
	virtual bool proc(const T& src);

	/**
	 * @brief  処理対象のワークスペースを返却
	 * @retval ワークスペース
	 */
	const CWorkspace& getWorkspace(){ return m_outputWorkspace; }

private:

	CWorkspace   m_workspace;			//!< 入力先のDB
	DBReplicater m_dbReplicater;		//!< レコードコピークラス
	uh::tstring m_strFolder;	//!< 出力先フォルダ

	/**
	 * @brief  レコードのコピーを実施する
	 * @param  srcWorkspace  [in]		コピー元ワークスペース
	 * @param  destWorkspace [in/out]   コピー先ワークスペース
	 * @retval true:処理成功
	 */
	bool copyTableData(IWorkspacePtr srcWorkspace, IWorkspacePtr destWorkspace);
};

template <typename T>
bool PGFGDBExportProcessor<T>::proc(const T& src)
{
	uh::tstring mesh(src);

// TODO: このへんから直す必要あり
	_ASSERT(_T("未実装"));
/*	// 出力先チェック

//	// メッシュコードに対応するPGFGDBファイルパスを取得
	auto it = std::find_if( m_paths.begin(), m_paths.end(), [mesh](const uh::tstring& str) { return str.find(mesh) != uh::tstring::npos; } );
	
	// メッシュリストに対応するファイルが見つからなかった場合
	if( it == m_paths.end() )
	{
		std::wstringstream ss;
		ss << _T("メッシュリストに対応するファイルが見つかりません メッシュコード : ") <<  mesh;
		std::cerr << uh::toStr(ss.str()) << std::endl;
		notifier::versatile(ss.str(), std::wcout);
		notifier::versatile(ss.str(), std::wcerr);
		return true;
	}

	uh::tstring filePath(*it);
	
	// ログ
	{
		std::wstringstream ss;
		ss << _T("エクスポート開始") <<  _T(" folder:") <<  m_strFolder;
		notifier::versatile(ss.str(), std::wcout);
	}

	CWorkspace pInputWorkspace(filePath.c_str());

	// レコードのコピーを実施
	if(!copyTableData((IWorkspace*)pInputWorkspace, (IWorkspace*)m_workspace))
		return false;

	// TODO: オプション次第で、メッシュ矩形で区切る
	

	// ログ
	{
		std::wstringstream ss;
		ss << _T("インポート終了") <<  _T(" file:") <<  filePath;
		notifier::versatile(ss.str(), std::wcout);
	}
	*/
	return true;
}

template <typename T>
bool PGFGDBExportProcessor<T>::copyTableData(IWorkspacePtr srcWorkspace, IWorkspacePtr destWorkspace)
{
// TODO: の関数はImportとことなり、エリア限定とする必要あり
	// データセット取得
	IEnumDatasetNamePtr ipEnumDataset;
	srcWorkspace->get_DatasetNames(esriDTAny,&ipEnumDataset);
	if ( !ipEnumDataset )
		return false;

	// コピー対象のテーブルを取得
	std::vector<uh::tstring> vTableNames;
	TableNamesSearcher searcher;
	searcher.getWorkspaceTableNames(ipEnumDataset, vTableNames);

	// 空フィルター、空リストを作成
	IQueryFilterPtr	ipFilter(CLSID_QueryFilter);
	std::list<uh::tstring> listColumns;

	// テーブルごとにコピー処理を回す
	for( const auto& tableName : vTableNames)
	{
		// ログ
		{
			std::wstringstream ss;
			ss << _T("インポート開始") <<  _T(" table:") <<  tableName;
			notifier::versatile(ss.str(), std::wcout);
		}

		// テーブルの内容をコピー
		if( !m_dbReplicater.InsertTable(ipFilter, (IWorkspace*)srcWorkspace, tableName, destWorkspace, tableName, listColumns, true) )
		{
			std::wstringstream ss;
			ss << _T("インポート失敗") <<  _T(" table:") <<  tableName;
			notifier::critical::versatile( uh::toStr(ss.str()) );
		}

		// ログ
		{
			std::wstringstream ss;
			ss << _T("インポート終了") <<  _T(" table:") <<  tableName;
			notifier::versatile(ss.str(), std::wcout);
		}
	}

	return true;
}
