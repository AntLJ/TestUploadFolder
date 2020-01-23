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
#include "MeshClipProcessor.h"
#include <TDC/sindylib_core/ErrorCode.h>
#include <TDC/sindylib_core/meshutil.h>
#include <WinLib/VersionInfo.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include <TDC/sindylib_base/TableContainer.h>
#include "sindy/schema.h"
#include "sindy/libschema.h"
#include "RowContainer.h"
#include "GeometryBase.h"
#include "ErrorObject2.h"
#include "Notifier.h"
#include "ClipUtility.h"

namespace sindy {
	extern bool g_bSetHistory;
}

using namespace sindy;

// 処理対象の空間検索クエリフィルタを作成
ISpatialFilterPtr createMeshSpatialFilter(IGeometryPtr pGeom)
{
	// 返却値
	IGeometryPtr ipBoundary;
	if( (IEnvelopePtr)pGeom )
		ipBoundary = pGeom;
	else
		ITopologicalOperatorPtr( pGeom )->get_Boundary(&ipBoundary);
	return AheInitSpatialFilter( nullptr, ipBoundary );
}

template <>
MeshClipProcessor<uh::tstring>::MeshClipProcessor(const CWorkspace& workspace, const ClippingDefinitionManager& m_ClipDef)
	: Processor<uh::tstring>(),
	m_workspace(workspace),
	m_mgr(m_ClipDef)
{
	CVersion version;
	m_internalName = uh::toTStr( version.GetInternalName().GetString());		//!< ツール名称
}

// 処理実行
template <>
bool MeshClipProcessor<uh::tstring>::proc(const uh::tstring& src)
{
	uh::tstring mesh(src);

	// ログ
	{
		std::wstringstream ss;
		ss << _T("クリッピング開始") <<  _T(" 対象メッシュ:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	// 編集開始
	if( !m_workspace.StartOperation() )
		notifier::critical::versatile(_T("編集開始に失敗"));

	// マージ定義ファイルに記載されたテーブル分だけループ
	for(unsigned  i = 0 ; i < m_mgr.size(); ++i)
	{

		ClippingDefinition clippingDef = m_mgr.get(i);

		// フィーチャクラスを取得
		IFeatureClassPtr pClipTableClass = m_workspace.Workspace().OpenSameOwnersTable(clippingDef.c_str());
		if( !pClipTableClass )
			continue;

		// コンテナ初期化
		CTableFinder finder;
		{
			CModel model;
			model.Add( AheGetFeatureClassName(pClipTableClass), sindy::schema::kObjectID, NULL, NULL );
			finder.InitCollection( (ITablePtr)pClipTableClass, model );
		}
		CTableContainer& con = *finder.FindTable(AheGetFeatureClassName(pClipTableClass));
		// 空間検索ファイルターを取得
		IGeometryPtr meshPolygon = MESH2ENV( _ttol(mesh.c_str()), AheGetSpatialReference( pClipTableClass ) );


		if( !doClip( con, meshPolygon ) )
			return false;

		// 保存実施
		CErrorObjectsBase err;
		bool backup = g_bSetHistory;
		g_bSetHistory = m_mgr.isSetHistory(); // 保存時に履歴を付与するかどうか
		con.Store(err);
		g_bSetHistory = backup;
		if( sindy::errorcode::sindyErr_NoErr != err.GetDefaultErrCode() )
		{
			for( auto& e : err )
			{
				for( auto& info : *e )
				{
					const auto& errObj = *e->GetErrorObject();
					notifier::reportObjInfo( (LPCTSTR)errObj.get<0>(), errObj.get<1>(), notifier::error_level::error, uh::toTStr(info->GetErrorMessage() ), std::wcerr );
				}
			}
			return false;
		}
	}

	// 編集終了
	if( !m_workspace.StopOperation() )
		notifier::critical::versatile(_T("編集終了に失敗"));

	// ログ
	{
		std::wstringstream ss;
		ss << _T("クリッピング終了") <<  _T(" 対象メッシュ:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	return true;
}

// ループ処理前実装メソッド
bool MeshClipProcessor<uh::tstring>::preprocess()
{
	// 接続確認
	if( !m_workspace.Workspace() )
	{
		notifier::critical::versatile(_T("出力先のDBに接続失敗"));
		return false;
	}

	// 編集対象がバージョン対応か非対応かに応じて、編集を開始する
	std::list<uh::tstring> listTarget;
	for( unsigned  i = 0 ; i < m_mgr.size(); ++i )
		listTarget.push_back( m_mgr.get(i) );
	
	return m_workspace.StartEditingByObjectType(listTarget); // エラーメッセージはCEditorで出す
}

// クリップ処理を実施する
template <>
bool MeshClipProcessor<uh::tstring>::doClip( sindy::CTableContainer& con, IEnvelopePtr mesh )
{
	ISpatialFilterPtr pMeshFilter = createMeshSpatialFilter( mesh );
	// メッシュに接するフィーチャを全て取得し、キャッシュする
	con._Select( (IQueryFilterPtr)pMeshFilter, false );
	con.CreateCache();

	// クリップ処理失敗を未然に防ぐため、メッシュ境界付近の構成点を
	// メッシュ境界にスナップさせる
	// TODO: これをやらないとメッシュ境界付近の構成点に引っ張られる
	// ことがあるが、問題が見えてから処理を追加することにする。
	// ちなみに、スナップする関数自体は実装済み

	// クリップ処理
	for( auto& row : con )
		Clip( con, *CAST_ROWC(row), mesh );

	return true;
}
