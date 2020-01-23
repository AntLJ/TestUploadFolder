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

/**
 * @file Rule.cpp
 * CRuleクラス実装ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "Rule.h"
#include "Workspace.h"
#include "SiNDYeSettings.h"
#include "util.h"
#include "GlobalFiles.h"
#include "GlobalRules.h"
#include "GlobalFunctions.h"
#include "sindycomutil.h"
#include "ListString.h"
#include "ListLong.h"
#include "GeometryRule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" const GUID __declspec(selectany) CLSID_Start =               {0x571419eb,0xfa89,0x4bd5,{0x8c,0xf0,0x9c,0xd6,0xbc,0xa3,0xf1,0x66}};
#include "ModifyCheck.h"
static sindy::CModifyCheck g_cModifyCheck;

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR AREA_CANNOT_BE_EDITED = _T("編集可能エリアではありません：%s");
	LPCTSTR AREA_IS_LOCKED        = _T("メッシュロックエリアです：%s");
	LPCTSTR OBJECT_WAS_DELETED    = _T("[%s] : オブジェクト %d は削除されました。TABLE[D%s] STATE[%d]");
	LPCTSTR WORK_PURPOSE_NOT_SET  = _T("作業目的が設定されていません");
	LPCTSTR WORK_PURPOSE_OF       = _T("[%s] : %d は %s が %s に『%s』の作業目的で %s しています。TABLE[A%s] STATE[%d]");
#else
	LPCTSTR AREA_CANNOT_BE_EDITED = _T("This is not an area that can be edited.：%s");
	LPCTSTR AREA_IS_LOCKED        = _T("This area is locked.：%s");
	LPCTSTR OBJECT_WAS_DELETED    = _T("[%s]:object %d was deleted. TABLE[D%s] STATE[%d]");
	LPCTSTR WORK_PURPOSE_NOT_SET  = _T("The work purpose has not been set");
	LPCTSTR WORK_PURPOSE_OF       = _T("[%s]:%s %s %d in the work purpose of [%s] at %s. TABLE[A%s] STATE[%d]");
#endif	// SINDY_FOR_ENGLISH	
}

using namespace winutil;
namespace sindy {
using namespace errorcode;

CRule::CRule() : 
	m_lWorkPurposeID(-1),
	m_bAnnoRuleCached(false),
	m_bEditRuleCached(false),
	m_bBGClassCached(false),
	m_hMutex(NULL)
{
}

CRule::~CRule()
{
	ClearRuleCache();
}

// ログイン情報を書き込む
void CRule::SetLoginInfo( long lPurposeID, LPCTSTR lpcszUserName )
{
	m_lWorkPurposeID = lPurposeID;
	m_strOperatorName = lpcszUserName;	// メンバに直接文字列をコピーする場合はインラインではだめ

	const workpurpose::CWorkPurpose* pRule = GetWorkPurpose();
	LOGASSERTE_IF( pRule, sindyErr_RuleNotFound )
	{
		// 自動付与属性を初期化する
		TCHAR strBuffer[1024] = _T("");
		if( pRule->GetAutoAttrGiving( 1023, strBuffer ) )
			m_cAutoValue.Init( strBuffer );

		// [bug 2648] トレースログにログイン情報を書き込む
		ZeroMemory( strBuffer, 1024 );
		pRule->GetPurpose( 1023, strBuffer );

		TRACEMESSAGE( _T("LOGIN INFO : PURPOSE:%s RULE:%s USER:%s\n"), strBuffer, m_cSiNDYeSettings.GetUsePGDB() ? m_cSiNDYeSettings.GetPGDBFileName() : m_cSiNDYeSettings.GetServerName(), m_strOperatorName );
	}
}

// 現在のMXDファイル名を取得する
LPCTSTR CRule::GetMXDFileName( int nLen, LPTSTR lpszMXD ) const
{
	LPCTSTR lpszRet = NULL;	// 返り値

	LOGASSERTE_IF( _MXD_MAX_LEN  <= nLen && NULL != lpszMXD, sindyErr_ArgLimitOver )
	{
		LOGASSERT_IF( IsLogined(), sindyErr_NoLogin )
		{
			const workpurpose::CWorkPurpose* pRule = GetWorkPurpose();
			LOGASSERTE_IF( pRule, sindyErr_RuleNotFound )
				lpszRet = pRule->GetMXD( _MXD_MAX_LEN, lpszMXD );
		}
	}
	return lpszRet;
}
// 現在の自動属性付与値を取得する
LPCTSTR CRule::GetAutoAttrGiving( int nLen, LPTSTR lpszAutoAttrGiving ) const
{
	LPCTSTR lpszRet = NULL;	// 返り値

	LOGASSERTE_IF( _MXD_MAX_LEN  <= nLen && NULL != lpszAutoAttrGiving, sindyErr_ArgLimitOver )
	{
		LOGASSERT_IF( IsLogined(), sindyErr_NoLogin )
		{
			const workpurpose::CWorkPurpose* pRule = GetWorkPurpose();
			LOGASSERTE_IF( pRule, sindyErr_RuleNotFound )
				lpszRet = pRule->GetAutoAttrGiving( _MXD_MAX_LEN, lpszAutoAttrGiving );
		}
	}
	return lpszRet;
}

// ルールのキャッシュを作成する
sindyErrCode CRule::CreateRuleCache( bool bAnnoRule/* = true*/, bool bEditRule/* = true*/, bool bBGClass/* = true*/, long lPurposeID/* = -1*/ )
{
	// 排他処理
	WaitForSingleObject( m_hMutex, INFINITE );
	m_hMutex = CreateMutex( NULL, FALSE, NULL );

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( ( bAnnoRule && ( ! m_bAnnoRuleCached ) ) || 
		( bEditRule && ( ! m_bEditRuleCached ) ) ||
		( bBGClass  && ( ! m_bBGClassCached  ) ) )
	{
		TRACEMESSAGE( _T("CRule::CreateRuleCache() : Started.\n") );

		// SiNDY-eの設定を取得する
		m_cSiNDYeSettings.Load();	// エラーでも良い
		CString strRefName = ( m_cSiNDYeSettings.GetUsePGDB() ) ? _T("") : _T("REFERENCE.");	// SDEの場合はREFERENCE.を付ける

		// ルールデータベースに接続する
		CWorkspace cWork;
		if( m_cSiNDYeSettings.GetUsePGDB() )
			cWork.ConnectToPGDB( m_cSiNDYeSettings.GetPGDBFileName() );
		else
			cWork.ConnectToSDE( _T("ronly"), _T("ronly"), _T("SDE.DEFAULT"), _T("5151"), m_cSiNDYeSettings.GetServerName() );

		if( cWork != NULL )
		{
			// 注記ルールテーブルを読み込んでキャッシュする
			if( bAnnoRule )
			{
				// 注記ルールテーブル
				if( ! m_cAnnotationClassTable.IsCached() )
				{
					// ルールテーブルに接続
					if( m_cAnnotationClassTable == NULL )
					{
						CTable cTable;
						cTable.SetObject( cWork.OpenTable( strRefName + schema::annotation_class::kTableName ) );
						m_cAnnotationClassTable = cTable;
					}
					if( m_cAnnotationClassTable != NULL )
					{
						// キャッシュ作成後DBから切り離す
						if( 0 == m_cAnnotationClassTable.CreateTableCache(true) )
							emErr = sindyErr_RuleCantCached;
					}
					else
						emErr = sindyErr_TableNotFound;	// テーブル読み込みエラー
				}

				m_bAnnoRuleCached = true;
			}
			// 背景種別ルールを読み込んでキャッシュする
			if( bBGClass )
			{
				// 背景テーブル
				if( ! m_cBackGroundClassTable.IsCached() )
				{
					if( m_cBackGroundClassTable == NULL )
						m_cBackGroundClassTable = cWork.OpenTable( strRefName + schema::background_class::kTableName );

					if( m_cBackGroundClassTable != NULL )
					{
						if( 0 == m_cBackGroundClassTable.CreateTableCache(true) )
							emErr = sindyErr_RuleCantCached;
					}
					else
						emErr = sindyErr_TableNotFound;
				}

				m_bBGClassCached = true;
			}
			// 編集ルールテーブルを読み込んでキャッシュする
			if( emErr == sindyErr_NoErr && bEditRule )
			{
				// 作業目的管理テーブル
				if( ! m_cWorkPurposeTable.IsCached() )
				{
					if( m_cWorkPurposeTable == NULL )
						m_cWorkPurposeTable = cWork.OpenTable( strRefName + schema::workpurpose::kTableName );

					if( m_cWorkPurposeTable != NULL )
					{
						if( 0 == m_cWorkPurposeTable.CreateTableCache(true, lPurposeID) )
							emErr = sindyErr_RuleCantCached;
					}
					else
						emErr = sindyErr_TableNotFound;
				}
				// レイヤ名管理テーブル
				if( ! m_cLayerNamesTable.IsCached() )
				{
					if( m_cLayerNamesTable == NULL )
						m_cLayerNamesTable = cWork.OpenTable( strRefName + schema::layernames::kTableName );

					if( m_cLayerNamesTable != NULL )
					{
						if( 0 == m_cLayerNamesTable.CreateTableCache(true) )
							emErr = sindyErr_RuleCantCached;
					}
					else
						emErr = sindyErr_TableNotFound;
				}
				// 作業者名管理テーブル
				if( ! m_cOperatorTable.IsCached() )
				{
					if( m_cOperatorTable == NULL )
						m_cOperatorTable = cWork.OpenTable( strRefName + schema::soperator::kTableName );

					if( m_cOperatorTable != NULL )
						m_cOperatorTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 接続ユーザ名管理テーブル
				if( ! m_cConnectUserTable.IsCached() )
				{
					if( m_cConnectUserTable == NULL )
						m_cConnectUserTable = cWork.OpenTable( strRefName + schema::connectuser::kTableName );

					if( m_cConnectUserTable != NULL )
						m_cConnectUserTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 削除可能レイヤ管理テーブル
				if( ! m_cDeletableLayersTable.IsCached() )
				{
					if( m_cDeletableLayersTable == NULL )
						m_cDeletableLayersTable = cWork.OpenTable( strRefName + schema::deletablelayers::kTableName );

					if( m_cDeletableLayersTable != NULL )
						m_cDeletableLayersTable.CreateTableCache(true, lPurposeID);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 作成可能レイヤ管理テーブル
				if( ! m_cCreatableLayersTable.IsCached() )
				{
					if( m_cCreatableLayersTable == NULL )
						m_cCreatableLayersTable = cWork.OpenTable( strRefName + schema::creatablelayers::kTableName );

					if( m_cCreatableLayersTable != NULL )
						m_cCreatableLayersTable.CreateTableCache(true, lPurposeID);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 移動可能レイヤ管理テーブル
				if( ! m_cMovableLayersTable.IsCached() )
				{
					if( m_cMovableLayersTable == NULL )
						m_cMovableLayersTable = cWork.OpenTable( strRefName + schema::movablelayers::kTableName );

					if( m_cMovableLayersTable != NULL )
						m_cMovableLayersTable.CreateTableCache(true, lPurposeID);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 属性編集可能レイヤ管理テーブル
				if( ! m_cEditAttrLayersTable.IsCached() )
				{
					if( m_cEditAttrLayersTable == NULL )
						m_cEditAttrLayersTable = cWork.OpenTable( strRefName + schema::editattrlayers::kTableName );

					if( m_cEditAttrLayersTable != NULL )
						m_cEditAttrLayersTable.CreateTableCache(true, lPurposeID);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 道路属性名称管理テーブル
				if( ! m_cRoadAttributeTable.IsCached() )
				{
					if( m_cRoadAttributeTable == NULL )
						m_cRoadAttributeTable = cWork.OpenTable( strRefName + schema::roadattribute::kTableName );

					if( m_cRoadAttributeTable != NULL )
						m_cRoadAttributeTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 道路リンクの道路種別管理テーブル
				if( ! m_cRoadDispClassTable.IsCached() )
				{
					if( m_cRoadDispClassTable == NULL )
						m_cRoadDispClassTable = cWork.OpenTable( strRefName + schema::roaddispclass::kTableName );

					if( m_cRoadDispClassTable != NULL )
					{
						if( 0 == m_cRoadDispClassTable.CreateTableCache(true) )
							emErr = sindyErr_RuleCantCached;
					}
					else
						emErr = sindyErr_TableNotFound;
				}
				// 道路リンクの経路種別管理テーブル
				if( ! m_cRoadNaviClassTable.IsCached() )
				{
					if( m_cRoadNaviClassTable == NULL )
						m_cRoadNaviClassTable = cWork.OpenTable( strRefName + schema::roadnaviclass::kTableName );

					if( m_cRoadNaviClassTable != NULL )
					{
						if( 0 == m_cRoadNaviClassTable.CreateTableCache(true) )
							emErr = sindyErr_RuleCantCached;
					}
					else
						emErr = sindyErr_TableNotFound;
				}
				// 共有削除レイヤ名管理テーブル
				if( ! m_cSharedDeleteLayersTable.IsCached() )
				{
					if( m_cSharedDeleteLayersTable == NULL )
						m_cSharedDeleteLayersTable = cWork.OpenTable( strRefName + schema::shareddeletelayers::kTableName );

					if( m_cSharedDeleteLayersTable != NULL )
						m_cSharedDeleteLayersTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 共有移動レイヤ名管理テーブル
				if( ! m_cSharedMoveLayersTable.IsCached() )
				{
					if( m_cSharedMoveLayersTable == NULL )
						m_cSharedMoveLayersTable = cWork.OpenTable( strRefName + schema::sharedmovelayers::kTableName );

					if( m_cSharedMoveLayersTable != NULL )
						m_cSharedMoveLayersTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 道路リンクの道路種別と経路種別の関連管理テーブル
				if( ! m_cDispNaviRelationTable.IsCached() )
				{
					if( m_cDispNaviRelationTable == NULL )
						m_cDispNaviRelationTable = cWork.OpenTable( strRefName + schema::dispnavirelation::kTableName );

					if( m_cDispNaviRelationTable != NULL )
						m_cDispNaviRelationTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
				// 道路リンクの道路種別とその他の属性の関連管理テーブル
				if( ! m_cDispOtherRelationTable.IsCached() )
				{
					if( m_cDispOtherRelationTable == NULL )
						m_cDispOtherRelationTable = cWork.OpenTable( strRefName + schema::dispotherrelation::kTableName );

					if( m_cDispOtherRelationTable != NULL )
						m_cDispOtherRelationTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
	/*			// ルール編集履歴管理テーブル
				if( ! m_cEditHistoryTable.IsCached() )
				{
					if( m_cEditHistoryTable == NULL )
						m_cEditHistoryTable = pWorkspace->OpenTable( strRefName + schema::edithistory::kTableName );

					if( m_cEditHistoryTable != NULL )
					{
						if( 0 == m_cEditHistoryTable.CreateTableCache(true) )
							emErr = sindyErr_RuleCantCached;
					}
					else
						emErr = sindyErr_TableNotFound;
				}*/
				// 編集可能メッシュ管理テーブル
				if( ! m_cEditMeshTable.IsCached() )
				{
					if( m_cEditMeshTable == NULL )
						m_cEditMeshTable = cWork.OpenTable( strRefName + schema::editmesh::kTableName );

					if( m_cEditMeshTable != NULL )
						m_cEditMeshTable.CreateTableCache(true, lPurposeID);
					else
						emErr = sindyErr_TableNotFound;
				}
				// ロックエリア管理テーブル
				if( ! m_cLockAreaTable.IsCached() )
				{
					if( m_cLockAreaTable == NULL )
						m_cLockAreaTable = cWork.OpenTable( strRefName + schema::lockarea::kTableName );

					if( m_cLockAreaTable != NULL )
						m_cLockAreaTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
				// ロックメッシュ管理テーブル
				if( ! m_cLockMeshTable.IsCached() )
				{
					if( m_cLockMeshTable == NULL )
						m_cLockMeshTable = cWork.OpenTable( strRefName + schema::lockmesh::kTableName );

					if( m_cLockMeshTable != NULL )
						m_cLockMeshTable.CreateTableCache(true);
					else
						emErr = sindyErr_TableNotFound;
				}
				m_bEditRuleCached = true;
			}
		}
		else
			emErr = sindyErr_RuleCantCached;

		TRACEMESSAGE( _T("CRule::CreateRuleCache() : Finished.\n") );

		// [bug 2648] SiNDY-e バージョンその他をトレースログに出す
		TRACEMESSAGE( _T("INIT INFO : OS:%s MACHINE:%s VERSION:%s\n"), GetOperatingSystem(), GetMachineName(), m_cSiNDYeSettings.GetVersion() );
	}

	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
	m_hMutex = NULL;

	return emErr;
}

// ルールのキャッシュを作成しなおす
sindyErrCode CRule::RecreateRuleCache( bool bAnnoRule/* = true*/, bool bEditRule/* = true*/, bool bBGClass/* = true*/, long lPurposeID/* = -1*/ )
{
	ClearRuleCache();
	return CreateRuleCache( bAnnoRule, bEditRule, bBGClass, lPurposeID );
}

void CRule::ClearRuleCache( bool isFinal/* = false*/ )
{
	WaitForSingleObject( m_hMutex, INFINITE );

	m_cBackGroundClassTable.ClearTableCache();
	m_cAnnotationClassTable.ClearTableCache();
	m_cWorkPurposeTable.ClearTableCache();
	m_cLayerNamesTable.ClearTableCache();
	m_cOperatorTable.ClearTableCache();
	m_cConnectUserTable.ClearTableCache();
	m_cDeletableLayersTable.ClearTableCache();
	m_cCreatableLayersTable.ClearTableCache();
	m_cMovableLayersTable.ClearTableCache();
	m_cEditAttrLayersTable.ClearTableCache();
	m_cRoadAttributeTable.ClearTableCache();
	m_cRoadDispClassTable.ClearTableCache();
	m_cRoadNaviClassTable.ClearTableCache();
	m_cSharedDeleteLayersTable.ClearTableCache();
	m_cSharedMoveLayersTable.ClearTableCache();
	m_cDispNaviRelationTable.ClearTableCache();
	m_cDispOtherRelationTable.ClearTableCache();
	m_cEditHistoryTable.ClearTableCache();
	m_cEditMeshTable.ClearTableCache();
	m_cLockAreaTable.ClearTableCache();
	m_cLockMeshTable.ClearTableCache();

	if( isFinal )
	{
		m_cBackGroundClassTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cAnnotationClassTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cWorkPurposeTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cLayerNamesTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cOperatorTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cConnectUserTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cDeletableLayersTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cCreatableLayersTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cMovableLayersTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cEditAttrLayersTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cRoadAttributeTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cRoadDispClassTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cRoadNaviClassTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cSharedDeleteLayersTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cSharedMoveLayersTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cDispNaviRelationTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cDispOtherRelationTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cEditHistoryTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cEditMeshTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cLockAreaTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
		m_cLockMeshTable.SetObject( (ITable*)NULL, CSPFieldMap(), CSPTableNameString() );
	}
	m_bAnnoRuleCached = m_bEditRuleCached = false;
}

// 編集可能かどうかをチェックする
sindyeEditableType CRule::IsEditable( const CRow& cRow, sindyeEditType emEditType, int nLen, LPTSTR lpszMessage, bool check_modify/* = true*/ ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	sindyeEditableType emType = sindyeEditableEditable;	// 返り値

	CTableNameString& cNames( *cRow.GetNameString().get() );
	const CGeometry* pGeom = cRow.GetShape();
	if( pGeom )
	{
		IGeometryPtr ipGeom( (IGeometry*)*pGeom );

		// ロックエリア内かどうかをチェック
		if( ! IsLockArea( cNames.GetOwnerName(), cNames.GetTableName(), ipGeom ) )
		{
			sindyErrCode emErr = sindyErr_NoErr;
			// 編集可能エリアかどうかをチェック
			if( sindyErr_NoErr == ( emErr = IsEditableArea( cNames.GetTableName(), ipGeom ) ) )
			{
			}
			else
			{
				LOGASSERTE_IF( 0 <= _sntprintf( lpszMessage, nLen, AREA_CANNOT_BE_EDITED, cNames.GetTableName() ), sindyErr_BufferIsNotEnough );
				emType = sindyeEditableNoEditable;
			}
		}
		else 
		{
			LOGASSERTE_IF( 0 <= _sntprintf( lpszMessage, nLen, AREA_IS_LOCKED, cNames.GetTableName() ), sindyErr_BufferIsNotEnough );
			emType = sindyeEditableNoEditable;
		}
	}

	if( sindyeEditableEditable == emType )
	{
		if( ! ( check_modify && IsModified( cRow, nLen, lpszMessage ) ) )
		{
			// 編集可能かどうかをチェック
			switch( emEditType )
			{
				case sindyeEditCreate:		emType = IsCreatable( cRow, nLen, lpszMessage ); break;
				case sindyeEditMove:		emType = IsMovable( cRow, nLen, lpszMessage ); break;
				case sindyeEditDelete:		emType = IsDeletable( cRow, nLen, lpszMessage ); break;
				case sindyeEditAttribute:	emType = IsAttributeEditable( cRow, nLen, lpszMessage ); break;
				default: SASSERT(false); break;
			}
		}
		else
			emType = sindyeEditableNoEditable; // 誰かが編集した
	}

	return emType;
}

// 注記ルールレコードを取得する
const annotation_class::CAnnotationClass* CRule::GetAnnoRule( sindyTableType::ECode emTableType, schema::annotation::annotation_code::ECode emCode ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cAnnotationClassTable.GetRowByAnnoCode( emTableType, emCode );
}

// 注記ルールキャッシュテーブルを取得する
const annotation_class::CAnnotationClassTable* CRule::GetAnnotationClassTable() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return &m_cAnnotationClassTable;
}

// 背景ルールキャッシュテーブルを取得する
const background_class::CBackGroundClassTable* CRule::GetBackGroundClassTable() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	_ICursorPtr ipCursor = NULL;

	ipCursor = m_cBackGroundClassTable.Search( ipQueryFilter, true );
	return &m_cBackGroundClassTable;
}

// 編集可能メッシュテーブルを取得する
const editmesh::CEditMeshTable* CRule::GetEditMeshTable() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return &m_cEditMeshTable;
}

// 現在の作業目的の作業目的レコードを取得する
const workpurpose::CWorkPurpose* CRule::GetWorkPurpose() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cWorkPurposeTable.Find( m_lWorkPurposeID );
}

// 現在の作業目的IDを取得する
long CRule::GetWorkPurposeID() const
{
	return m_lWorkPurposeID;
}

// 現在の作業者名を取得する
LPCTSTR CRule::GetOperatorName() const
{
	return ( m_strOperatorName.IsEmpty() ) ? NULL : (LPCTSTR)m_strOperatorName;
}

// 編集可能レイヤの一覧を取得する
sindyErrCode CRule::GetMovableLayers( CListString& listLayers ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return ( IsLogined() ) ? m_cMovableLayersTable.GetMovableLayers( m_lWorkPurposeID, listLayers ) : errorcode::sindyErr_NoLogin;
}

// 削除可能レイヤの一覧を取得する
sindyErrCode CRule::GetDeletableLayers( CListString& listLayers ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return ( IsLogined() ) ? m_cDeletableLayersTable.GetDeletableLayers( m_lWorkPurposeID, listLayers ) : errorcode::sindyErr_NoLogin;
}

// 作成可能レイヤの一覧を取得する
sindyErrCode CRule::GetCreatableLayers( CListString& listLayers ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return ( IsLogined() ) ? m_cCreatableLayersTable.GetCreatableLayers( m_lWorkPurposeID, listLayers ) : errorcode::sindyErr_NoLogin;
}

// 属性編集可能レイヤの一覧を取得する
sindyErrCode CRule::GetEditAttrLayers( CListString& listLayers ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return ( IsLogined() ) ? m_cEditAttrLayersTable.GetEditAttrLayers( m_lWorkPurposeID, listLayers ) : errorcode::sindyErr_NoLogin;
}

// 作成可能レイヤかどうかチェックする
bool CRule::IsCreatableLayer( LPCTSTR lpcszLayer ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	bool bRet = false;	// 返り値
	LOGASSERT_IF( IsLogined(), sindyErr_NoLogin )
		bRet = m_cCreatableLayersTable.IsCreatableLayer( m_lWorkPurposeID, m_cLayerNamesTable.Find( lpcszLayer ) );

	return bRet;
}

// 作成可能レイヤかどうかチェックする
bool CRule::IsDeletableLayer( LPCTSTR lpcszLayer ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	bool bRet = false;	// 返り値
	LOGASSERT_IF( IsLogined(), sindyErr_NoLogin )
		bRet = m_cDeletableLayersTable.IsDeletableLayer( m_lWorkPurposeID, m_cLayerNamesTable.Find( lpcszLayer ) );

	return bRet;
}

// 移動可能レイヤかどうかチェックする
bool CRule::IsMovableLayer( LPCTSTR lpcszLayer ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	bool bRet = false;	// 返り値
	LOGASSERT_IF( IsLogined(), sindyErr_NoLogin )
		bRet = m_cMovableLayersTable.IsMovableLayer( m_lWorkPurposeID, m_cLayerNamesTable.Find( lpcszLayer ) );

	return bRet;
}

// 属性編集可能レイヤかどうかチェックする
bool CRule::IsEditAttrLayer( LPCTSTR lpcszLayer ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	bool bRet = false;	// 返り値
	LOGASSERT_IF( IsLogined(), sindyErr_NoLogin )
		bRet = m_cEditAttrLayersTable.IsEditAttrLayer( m_lWorkPurposeID, m_cLayerNamesTable.Find( lpcszLayer ) );

	return bRet;
}

// 作業目的テーブルの参照を取得する
const workpurpose::CWorkPurposeTable& CRule::GetWorkPurposeTable() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cWorkPurposeTable;
}

// レイヤ名テーブルの参照を取得する
const layernames::CLayerNamesTable& CRule::GetLayerNamesTable() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cLayerNamesTable;
}

// 作成可能レイヤキャッシュテーブルを取得する
const creatablelayers::CCreatableLayersTable& CRule::GetCreatableLayersTable() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cCreatableLayersTable;
}

// 編集済みかどうかをチェックする
bool CRule::IsModified( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	bool bRet = false;	// 返り値

#ifdef _USE_COM_MODIFYCHECK
	if( g_ipModifyCheck == NULL )
		g_ipModifyCheck.CreateInstance( CLSID_SiNDYModifyCheckObj );

	if( g_ipModifyCheck != NULL )
	{
		BSTR bstrMsg = 0;
		VARIANT_BOOL vbIsModified = VARIANT_FALSE;
		g_ipModifyCheck->IsModified( (_IRow*)cRow, &vbIsModified, &bstrMsg );
		if( vbIsModified )
		{
			USES_CONVERSION;
			lstrcpyn( lpszMessage, OLE2CT(bstrMsg), nLen );
			bRet = true;
		}
		SysFreeString(bstrMsg);
	}
#else
	CModifyCheck::CModifyInfo cInfo;
	if( true == ( bRet = g_cModifyCheck.IsModified( cRow, cInfo ) ) )
	{
		if( cInfo.m_bDeleted )
		{
			LOGASSERTE_IF( 0 <= _sntprintf( lpszMessage, nLen, OBJECT_WAS_DELETED,
				( cInfo.m_bIsLocal ) ? _T("LOCAL") : _T("SERVER"), cInfo.m_lOID,
				cInfo.GetTableName(), cInfo.m_lStateID ), sindyErr_BufferIsNotEnough );
		}
		else {
			TCHAR strPurpose[256];
			_tcscpy(strPurpose, WORK_PURPOSE_NOT_SET);	// Undo対策
			GetOperatingPurposeFromID( cInfo.m_lPurposeID, 256, strPurpose );
			LOGASSERTE_IF( 0 <= _sntprintf( lpszMessage, nLen, WORK_PURPOSE_OF, 
				( cInfo.m_bIsLocal ) ? _T("LOCAL") : _T("SERVER"), cInfo.m_lOID, cInfo.GetOperator(), cInfo.GetTime(), strPurpose, cInfo.GetUpdateType(), 
				cInfo.GetTableName(), cInfo.m_lStateID ), sindyErr_BufferIsNotEnough );
		}
	}
#endif // ifdef _USE_COM_MODIFYCHECK
	return bRet;
}

// 移動可能かどうかをチェックする
sindyeEditableType CRule::IsMovable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return ( IsLogined() ) ? m_cMovableLayersTable.IsMovable( GetWorkPurposeID(), m_cLayerNamesTable.Find( cRow.GetTableName() ), cRow, nLen, lpszMessage ) : sindyeEditableNoEditable;
}

// 削除可能かどうかをチェックする
sindyeEditableType CRule::IsDeletable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return ( IsLogined() ) ? m_cDeletableLayersTable.IsDeletable( GetWorkPurposeID(), m_cLayerNamesTable.Find( cRow.GetTableName() ), cRow, nLen, lpszMessage ) : sindyeEditableNoEditable;
}

// 作成可能かどうかをチェックする
sindyeEditableType CRule::IsCreatable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return ( IsLogined() ) ? m_cCreatableLayersTable.IsCreatable( GetWorkPurposeID(), m_cLayerNamesTable.Find( cRow.GetTableName() ), cRow, nLen, lpszMessage ) : sindyeEditableNoEditable;
}

// 属性編集可能かどうかをチェックする
sindyeEditableType CRule::IsAttributeEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return ( IsLogined() ) ? m_cEditAttrLayersTable.IsAttributeEditable( GetWorkPurposeID(), m_cLayerNamesTable.Find( cRow.GetTableName() ), cRow, nLen, lpszMessage ) : sindyeEditableNoEditable;
}

// 編集可能領域かどうかをチェックする
sindyErrCode CRule::IsEditableArea( LPCTSTR lpcszName, IGeometry* ipGeom ) const
{
	sindyErrCode emErr = sindyErr_NoErr; // 返値

	WaitForSingleObject( m_hMutex, INFINITE );

	if( IsLogined() )
	{
		if( ! m_cEditMeshTable.IsEditable( m_lWorkPurposeID, m_cLayerNamesTable.Find( lpcszName ), m_cOperatorTable.GetOperatorID( m_strOperatorName ), ipGeom ) )
			emErr = sindyErr_RuleNotAllowedEditByEditArea;
	}
	else
		emErr = sindyErr_NoLogin;	// ログインしていない

	return emErr;
}

// ロックされているメッシュかどうかをチェックする
sindyErrCode CRule::IsLockMesh( LPCTSTR lpcszConnUser, LPCTSTR lpcszName, long lCode ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	WaitForSingleObject( m_hMutex, INFINITE );

/*	if( IsLogined() )
	{*/
		long lConnUserID = m_cConnectUserTable.GetConnectUserID( lpcszConnUser );
		long lLayerNameID = m_cLayerNamesTable.Find( lpcszName );

		if( m_cLockMeshTable.IsLocked( lConnUserID, lLayerNameID, lCode ) )
			emErr = sindyErr_RuleNotAllowedEditByLockArea;
/*	}
	else
		emErr = sindyErr_NoLogin;	// ログインしていない*/

	return emErr;
}

// ロックされているエリアかどうかをチェックする
sindyErrCode CRule::IsLockArea( LPCTSTR lpcszConnUser, LPCTSTR lpcszName, IGeometry* ipGeom ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	WaitForSingleObject( m_hMutex, INFINITE );

	if( IsLogined() )
	{
		if( ipGeom )	// ipGeom が NULL の時は NoErr で返す
		{
			long lConnUserID = m_cConnectUserTable.GetConnectUserID( lpcszConnUser );
			long lLayerNameID = m_cLayerNamesTable.Find( lpcszName );

			if( m_cLockAreaTable.IsLocked( lConnUserID, lLayerNameID, ipGeom ) )
				emErr = sindyErr_RuleNotAllowedEditByLockArea;
		}
	}
	else
		emErr = sindyErr_NoLogin;	// ログインしていない

	return emErr;
}

// 保存可能な作業目的かどうかをチェックする
bool CRule::CanSaveEdit() const
{
	bool bRet = false;	// 返り値（安全性を考慮して、デフォルトでは保存不可とする）

	WaitForSingleObject( m_hMutex, INFINITE );

	if( IsLogined() )	// 未ログインの時にエラーを出さないように
	{
		const workpurpose::CWorkPurpose* pRule = GetWorkPurpose();
		LOGASSERTE_IF( pRule, sindyErr_RuleNotFound )
			bRet = pRule->GetCanSave();
	}

	return bRet;
}

// 作業目的IDから作業目的名を取得する
LPCTSTR CRule::GetOperatingPurposeFromID( long lID, int nLen, LPTSTR lpszName ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	const workpurpose::CWorkPurpose* pRule = m_cWorkPurposeTable.Find( lID );
	return ( pRule != NULL ) ? pRule->GetPurpose( nLen, lpszName ) : NULL;
}

// 渡されたレイヤ名に対応する共有移動対象レイヤ名をセパレータ付き文字列で返す
long CRule::GetSharedMovableLayer( LPCTSTR lpcszLayerName, CListString& listNames ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	CListLong listLayerNameID;
	long lLayerID = m_cLayerNamesTable.Find( lpcszLayerName );
	long lCount = 0;
	// レイヤ名がWorkEditorで登録されていないものもあるので、IDが-1の時には入ってきたレイヤ名を入れておく
	if( lLayerID >= 0 )
	{
		lCount = m_cSharedMoveLayersTable.GetSharedMoveLayers( lLayerID, listLayerNameID );
		LayerNameID2LayerName( listLayerNameID, listNames );
	}
	else
	{
		listNames.push_back( lpcszLayerName );
		lCount++;
	}

	return lCount;
}

// 渡されたレイヤ名に対応する共有削除対象レイヤ名をセパレータ付き文字列で返す
long CRule::GetSharedDeletableLayer( LPCTSTR lpcszLayerName, CListString& listNames ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	CListLong listLayerNameID;
	long lCount = m_cSharedDeleteLayersTable.GetSharedDeleteLayers( m_cLayerNamesTable.Find( lpcszLayerName ), listLayerNameID );
	LayerNameID2LayerName( listLayerNameID, listNames );
	return lCount;
}

// レイヤ名文字列作成用補助関数
void CRule::LayerNameID2LayerName( const CListLong& listLayerNameID, CListString& listNames ) const
{
//	bool bErr = false;

	WaitForSingleObject( m_hMutex, INFINITE );

	TCHAR strName[64];
	for( CListLong::const_iterator it = listLayerNameID.begin(); it != listLayerNameID.end(); ++it )
	{
		// レイヤ名取得＆文字列数計算
		lstrlen( m_cLayerNamesTable.Find( *it, 63, strName ) );
		listNames.push_back( strName );
	}
}

// 道路リンクの道路種別と経路種別間のルールを取得する
sindyeDispNaviRelation CRule::GetDispNaviRule( long lRoadClassID, long lNaviClassID ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	const dispnavirelation::CDispNaviRelation* pRule = m_cDispNaviRelationTable.Find( lRoadClassID, lNaviClassID );
	if( pRule != NULL )
		return (sindyeDispNaviRelation)pRule->GetRelValue();
	else
		return (sindyeDispNaviRelation)-1;
}

// 道路リンクの道路種別とその他の属性間のルールを取得する
sindyeDispOtherRelation CRule::GetDispOtherRule( long lRoadClassID, LPCTSTR lpcszFieldName ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );

	const dispotherrelation::CDispOtherRelation* pRule = m_cDispOtherRelationTable.Find( lRoadClassID, m_cRoadAttributeTable.GetFieldNameID( lpcszFieldName ) );
	if( pRule != NULL )
		return (sindyeDispOtherRelation)pRule->GetRelValue();
	else
		return (sindyeDispOtherRelation)-1;
}

/*// レイヤ名IDを取得する
long CRule::GetLayerNameID( LPCTSTR lpcszLayerName ) const
{
	const layernames::CLayerNames* pRule = m_cLayerNamesTable.Find( lpcszLayerName );
	if( pRule != NULL )
		return pRule->GetLayerNameID();
	else
		return -1;
}*/

// ログイン済みであるかどうかをチェックする
bool CRule::IsLogined() const
{
	return ( m_lWorkPurposeID < 0 ) ? false : true;
}

// ログインする
sindyErrCode CRule::Login()
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// ルールキャッシュを作成
	CreateRuleCache();

	IStartupDialogPtr ipLogin;
	LOGASSERTEERR_IF( SUCCEEDED( ipLogin.CreateInstance( CLSID_Start ) ), sindyErr_COMCreateInstanceFailed )
	{
		OLE_HANDLE hWnd = 0;
		// ArcMapベースの場合はそっちのウインドウハンドルを取得する
		IDispatchPtr ipDisp;
		if( SUCCEEDED( ipDisp.CreateInstance( CLSID_AppRef ) ) )
		{
			OLECHAR FAR* szMember = L"hWnd";
			DISPID dispid;
			LOGASSERT_IF( SUCCEEDED( ipDisp->GetIDsOfNames( IID_NULL, &szMember, 1, LOCALE_SYSTEM_DEFAULT, &dispid ) ), sindyErr_COMFunctionFailed )
			{
				DISPPARAMS dp = { NULL, NULL, 0, 0 };
				VARIANT vaResult;
				VariantInit(&vaResult);

				LOGASSERT_IF( SUCCEEDED( ipDisp->Invoke( dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dp, &vaResult, NULL, NULL ) ), sindyErr_COMFunctionFailed )
				{
					LOGASSERTE_IF( VT_HANDLE == vaResult.vt && vaResult.lVal, sindyErr_COMFunctionFailed )
					{
						hWnd = (OLE_HANDLE)vaResult.lVal;
					}
				}
			}
		}

		// ログイン
		switch( ipLogin->DoModal( hWnd ) )
		{
			case S_OK: break;
			case S_FALSE: emErr = sindyErr_LoginCanceled; break;
			default:
				LOGASSERT(false,sindyErr_LoginFailed);
				break;
		}
	}

	return emErr;
}

// 編集閾値を取得する
int CRule::GetSelectTolerance() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cSiNDYeSettings.GetTolerance();
}

// 編集閾値をセットする
bool CRule::SetSelectTolerance( int nPixel )
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cSiNDYeSettings.SetTolerance(nPixel);
}

// トレースログファイル名を取得する
LPCTSTR CRule::GetTraceLogFileName( int nLen, LPTSTR strBuf ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	_tcscpy_s( strBuf, nLen, m_cSiNDYeSettings.GetTraceLogFileName() );
	return strBuf;
}

// トレースログファイル名を設定する
bool CRule::SetTraceLogFileName( LPCTSTR lpcszFileName )
{
	WaitForSingleObject( m_hMutex, INFINITE );
	if( m_cSiNDYeSettings.SetTraceLogFileName( lpcszFileName ) )
	{
		_tcscpy_s( g_strTracelog, _MAX_PATH, lpcszFileName ); // グローバルメンバにコピー
		return true;
	}
	return false;
}

// エラーログファイル名を取得する
LPCTSTR CRule::GetErrorLogFileName( int nLen, LPTSTR strBuf ) const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	_tcscpy_s( strBuf, nLen, m_cSiNDYeSettings.GetErrorLogFileName() );
	return strBuf;
}

// エラーログファイル名を設定する
bool CRule::SetErrorLogFileName( LPCTSTR lpcszFileName )
{
	WaitForSingleObject( m_hMutex, INFINITE );
	if( m_cSiNDYeSettings.SetErrorLogFileName( lpcszFileName ) )
	{
		_tcscpy_s( g_strErrlog, _MAX_PATH, lpcszFileName ); // グローバルメンバにコピー
		return true;
	}
	return false;
}

// 再描画バッファ値を取得する
int CRule::GetRefreshBuffer() const
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cSiNDYeSettings.GetRefreshBuffer();
}

// 再描画バッファ値を設定する
bool CRule::SetRefreshBuffer( int nPixel )
{
	WaitForSingleObject( m_hMutex, INFINITE );
	return m_cSiNDYeSettings.SetRefreshBuffer(nPixel);
}

/*// 整合性チェック用ルールを作成する
CLogicRule& CRule::CreateLogicRule( const CRow& cRow, CLogicRule& cRule ) const
{
	if( 0 == lstrcmpi( road_link::kTableName, cRow.GetTableName() ) )
	{
		cRule.m_dMaxDeg = 340.0;
		cRule.m_dMinDeg = 20.0;
		cRule.m_dMinLen = 0.21;
	}
	return cRule;
}*/

} // sindy
