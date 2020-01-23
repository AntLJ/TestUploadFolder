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

// DBUtility.h: CDBUtility クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBUTILITY_H__60E28852_A288_4034_96D6_7DB4606368D6__INCLUDED_)
#define AFX_DBUTILITY_H__60E28852_A288_4034_96D6_7DB4606368D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "schema.h"

namespace DBUtility
{

IWorkspacePtr OpenSDEWorkspace(LPCTSTR lpcszUser, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServer);
IFeatureClassPtr OpenFeatureClass(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName);
ITablePtr OpenTable(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName);
ISpatialReferencePtr GetSpatialReference(IFeatureClassPtr ipFeatureClass);
DOUBLE CalcDistance(IPointPtr ipPoint1, IPointPtr ipPoint2);

/**
* @brief	子バージョンを作成し、そのワークスペースを返す
*
* @note		既に同名のバージョンがあった場合は消してから新たに作り直す
*
* @param	ipWorkspace			[in]	親バージョンのワークスペース
* @param	strVersionName		[in]	作るバージョンのバージョン名
* @return	新しく作った子バージョンのワークスペース
*/
IWorkspacePtr CreateEditVersion( IWorkspacePtr ipWorkspace, LPCTSTR strVersionName );

/**
* @brief	あるフィーチャクラスと同じフィールドを持つフィーチャクラスを既存のデータセット内に新規作成する
*
* @param	lpcszName			[in]	作るフィーチャクラスの名前
* @param	ipImitativeFeature	[in]	このフィーチャクラスのフィールド構成を真似る
* @param	ipTargetWorkspace	[in]	このワークスペースに作成する。
                                        NULLならipImitativeFeatureと同じワークスペース（デフォルト）
* @param	lpcszDatasetname	[in]	このデータセット内に作成する。必ず存在する前提
                                        NULLならipImitativeFeature と同じデータセット（デフォルト）
* @return	作ったフィーチャクラス
*/
IFeatureClassPtr ImitateFeatureClass( LPCTSTR lpcszName, IFeatureClass* ipImitativeFeature, IWorkspace* ipTargetWorkspace = NULL, LPCTSTR lpcszDatasetname = NULL );

// フィーチャクラスからワークスペースを取得
IWorkspacePtr GetWorkspace( IFeatureClass* ipFeatureClass );

// フィーチャクラスを削除
void DeleteFeatureClass( IFeatureClass* ipFeatureClass );

// データセットを取得
IDatasetPtr GetDataset( IWorkspace* ipWorkspace, LPCTSTR lpcszDatasetName );

IFeatureDatasetPtr OpenDataset(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName);

IDatasetPtr CreateDataset( IWorkspacePtr ipWorkspace, LPCTSTR lpcszName);

// ワークスペース再接続
void ReOpenWorkspace( IWorkspace* ipWorkspace );

// フィーチャクラス作成
IFeatureClassPtr CreateFeatureClass(IWorkspacePtr ipWorkspace, IDataset* ipDataset, LPCTSTR lpcszLayerName, IFieldsPtr ipFields, esriFeatureType eType );

IFeatureClassPtr CreateFeatureClassInDataset(IDatasetPtr ipDataset, LPCTSTR lpcszLayerName, IFieldsPtr ipFields, esriFeatureType eType );


// フィールド作成
IFieldPtr CreateField( esriFieldType cType, CString cFieldName, LPCTSTR cAliasName, long cLength, long cPrecision, long cScale, VARIANT_BOOL cIsNullable, VARIANT_BOOL cEditable );

IFieldPtr CreateShapeField(LPCTSTR lpcszFieldName, esriGeometryType eGeometryType, bool bHasM, bool bHasZ );

IFieldsPtr CreateFieldsFromSchema( tFieldSchema tFieldinfo[], int nSize );

IFieldsPtr CreateFCFields( tFieldSchema tFieldinfo[] );


template <class T>
LONG GetFieldIndex(T ipFeature, LPCTSTR lpcszFieldName)
{
	IFieldsPtr ipFields = 0;
	ipFeature->get_Fields(&ipFields);
	LONG nIndex = 0;
	ipFields->FindField(CComBSTR(lpcszFieldName), &nIndex);
	return nIndex;
}

template <class T>
VOID PutValue(T ipFeature, LPCTSTR lpcszFieldName, VARIANT vaVal)
{
	ipFeature->put_Value(GetFieldIndex<T>(ipFeature, lpcszFieldName), vaVal);
	ipFeature->Store();
}

template <class T>
VOID GetValue(T ipFeature, LPCTSTR lpcszFieldName, VARIANT *vaVal)
{
	ipFeature->get_Value(GetFieldIndex<T>(ipFeature, lpcszFieldName), vaVal);
}

}

#endif // !defined(AFX_DBUTILITY_H__60E28852_A288_4034_96D6_7DB4606368D6__INCLUDED_)
