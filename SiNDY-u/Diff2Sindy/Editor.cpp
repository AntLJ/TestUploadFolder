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
#include "Editor.h"
#include "Notifier.h"
#include <type_convert.h>
#include <sindymacroutil.h>

using namespace sindy;
using namespace errorcode;

ObjectType CEditor::CheckObjectType( const std::list<uh::tstring>& listTableName ) const
{
	ObjectType ret = ObjectTypeUnknown;
	IFeatureWorkspaceManagePtr manage( (IWorkspace*)m_work );
	for( const auto& name : listTableName )
	{
		INamePtr nameObj;
		auto table = m_work.OpenTable( name.c_str() );
		if( !table )
			continue; // 存在しないテーブルは飛ばす
		((IDatasetPtr)table)->get_FullName( &nameObj );
		VARIANT_BOOL asVersioned = VARIANT_FALSE;
		manage->IsRegisteredAsVersioned( nameObj, &asVersioned );
		ObjectType type = ( asVersioned ) ? ObjectTypeVersioned : ObjectTypeNonVersioned;
		if( ObjectTypeUnknown == ret )
			ret = type;
		else if( ret != type )
			return ObjectTypeMix;
	}
	return ret;
}

bool CEditor::StartEditing(bool undoredo, bool versioned/* = true*/)
{
	m_undoredo = undoredo;
	m_versioned = versioned;

	// 通常の編集開始で良い場合
	if( !( m_work.GetNameString()->IsSDE() && (!versioned) )  )
		return sindyErr_NoErr == m_work.StartEditing(undoredo);

	// ノンバージョン編集の場合
	IMultiuserWorkspaceEditPtr multi((IWorkspace*)m_work);
	if( FAILED( multi->StartMultiuserEditing( esriMESMNonVersioned ) ) )
	{
		notifier::critical::versatile(_T("StartMultiuserEditing(esriMESMNonVersioned)...FAILED!!"));
		return false;
	}
	else {
		if( m_work.m_bTrace )
			notifier::versatile(_T("StartMultiuserEditing(esriMESMNonVersioned)...OK"));
	}
	return true;
}

bool CEditor::StopEditing(bool save)
{
	IWorkspaceEditPtr ipWork( (IWorkspace*)m_work );
	if( ! ipWork )
		return false;

	HRESULT hr = ipWork->StopEditing( bool2VB(save) );
	if( FDO_E_VERSION_REDEFINED != hr )
	{
		if( S_OK == hr )
		{
			if( m_work.m_bTrace )
				notifier::versatile(_T("StopEditing(SaveEdits=true)...OK\n"));
		}
		else
			notifier::versatile(_T("StopEditing(SaveEdits=true)......FAILED!!\n"));

		return SUCCEEDED(hr);
	}
	// リコンサイルが必要な場合
	if( m_work.m_bTrace )
		notifier::versatile(_T("Reconciling..."));
	VARIANT_BOOL vbConflicts = VARIANT_FALSE;
	IVersionEdit4Ptr ipVerEdit( ipWork );
	hr = ipVerEdit->Reconcile4(
		CComBSTR(m_work.GetNameString()->GetVersionName()),
		VARIANT_TRUE,        // 排他的にリコンサイル処理する
		VARIANT_TRUE,        // コンフリクトが起きたらAbortする
		VARIANT_FALSE,       // TODO: ChildWinsってなに？？
		VARIANT_TRUE,        // カラムレベルでのリコンサイル処理をする
		&vbConflicts );
	switch( hr )
	{
	case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE: notifier::versatile(_T("ERROR : Reconcile : [FDO_E_RECONCILE_VERSION_NOT_AVAILABLE]")); break;
	case FDO_E_VERSION_BEING_EDITED:            notifier::versatile(_T("ERROR : Reconcile : [FDO_E_VERSION_BEING_EDITED]")); break;
	case FDO_E_VERSION_NOT_FOUND:               notifier::versatile(_T("ERROR : Reconcile : [FDO_E_VERSION_NOT_FOUND]")); break;
	case FDO_E_NOT_EDITING:                     notifier::versatile(_T("ERROR : Reconcile : [FDO_E_NOT_EDITING]")); break;
	case S_OK:
		// コンフリクトした場合
		if( vbConflicts )
		{
			notifier::versatile(_T("ERROR : Reconcile : コンフリクトが起きたため、編集を破棄しました。"));
			hr = E_FAIL;
		}
		else {
			if( m_work.m_bTrace )
				notifier::versatile(_T("Reconciling...OK"));
			return StopEditing( save ); // リコンサイル後は再度保存処理（StopEditing）が必要
		}
		break;
	default:
		std::wstringstream ss;
		ss << _T("ERROR : Reconcile : リコンサイルで原因不明のエラー[") << hr << _T("]");
		notifier::versatile(ss.str());
		break;
	}
	return SUCCEEDED(hr);

}

bool CEditor::SaveEditing()
{
	if( StopEditing(true) )
		return StartEditing(m_undoredo, m_versioned);
	notifier::critical::versatile(_T("編集終了に失敗"));
	return false;
}

bool CEditor::StartOperation()
{
//	// SDEでバージョン非対応の場合はStartOperationできない
//	if( m_work.GetNameString()->IsSDE() && !m_versioned )
//		return true;

	bool ret = ( sindyErr_NoErr == m_work.StartEditOperation() );
	if( !ret )
		notifier::critical::versatile(_T("編集セッション開始に失敗"));

	return ret;
}

bool CEditor::StopOperation()
{
//	// SDEでバージョン非対応の場合はStopOperationできない
//	if( m_work.GetNameString()->IsSDE() && !m_versioned )
//		return true;

	bool ret = ( sindyErr_NoErr == m_work.StopEditOperation() );
	if( !ret )
		notifier::critical::versatile(_T("編集セッション終了に失敗"));

	return ret;
}

bool CEditor::AbortOperation()
{
//	// SDEでバージョン非対応の場合はAbortOperationできない
//	if( m_work.GetNameString()->IsSDE() && !m_versioned )
//		return true;

	bool ret = ( sindyErr_NoErr == m_work.AbortEditOperation() );
	if( !ret )
		notifier::critical::versatile(_T("編集セッション破棄に失敗"));

	return ret;
}

bool CEditor::StartEditingByObjectType( const std::list<uh::tstring>& listTableName )
{
	// 編集対象がバージョン対応か非対応か確認する
	// バージョン対応と非対応が混ざっている場合は編集させない
	ObjectType type = CheckObjectType( listTableName );
	if( ObjectTypeMix == type )
	{
		std::wstringstream ss;
		ss << L"バージョン対応レイヤと非対応レイヤが混在している為、処理を開始できません：" <<  (LPCWSTR)CStringW(m_work.GetNameString()->GetConnectString());
		notifier::critical::versatile( uh::toTStr(ss.str()) );
		return false;
	}

	return StartEditing(false, (ObjectTypeVersioned==type) );
}
