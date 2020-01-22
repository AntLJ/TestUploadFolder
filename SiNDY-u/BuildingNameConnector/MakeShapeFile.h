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

#include <stdio.h>
#include <vector>
#include <set>
#include <stdio.h>
#include <io.h>
#include <string>
#include "stdafx.h"

using	namespace	std;

#ifndef	_MAKESHAPEFILE_H_
#define	_MAKESHAPEFILE_H_

/**
* @brief ジオメトリをShapeに出力する
*/
class	MakeShapeFile
{
	private:
		IFeatureClassPtr	m_ipFClass;
		IFeatureCursorPtr	m_ipFeatureCursor;
		long				m_lORG_ID_Indx; // 元データのID
		long				m_lBUILDING_ID_Indx; // 建物のID
		long				m_lNAME_Indx; // 建物名称
	
		/**
		* @brief ファイルのフルパス名から、シェイプファイル名を抽出する関数
		* @return 成功=true, 失敗=false
		*/
		bool    GetFeatureClass	(
		            const std::string& sourceFile, ///< [in] ソースファイル名
		            std::string&       className   ///< [in] パス名
				);

		/**
		* @brief フルパス名から、ファイル名を除いて、ディレクトリ名を返す関数
		* @return 成功=true, 失敗=false
		*/
		bool    GetFilePath	(
		            const std::string& sourceFile, ///< [in] ソースファイル名
		            std::string&       filePath    ///< [in] パス名
		        );
	public:

		/**
		* @brief コンストラクタ
		*/
		MakeShapeFile()
		{
			m_lORG_ID_Indx      = -1; // 元データのID
			m_lBUILDING_ID_Indx = -1; // 建物のID
			m_lNAME_Indx        = -1; // 建物名称
		}
		/**
		* @brief デストラクタ
		*/
		~MakeShapeFile(){}

		/**
		* @brief データをシェープファイルに変換
		* @return 成功=true, 失敗=false
		*/
		bool    Init (
		            std::string&     c_sOutFName, ///< [in] 出力シェイプファイル名
		            esriGeometryType c_eGeomType  ///< [in] ジオメトリタイプ
		        );

		/**
		* @brief データをシェープファイルに変換
		* @return 成功=true, 失敗=false
		*/
		bool    OuttoShapeFile (
		            IGeometryPtr&    c_ipOutputGeo,///< [in] シェイプファイルに出力したいジオメトリ
		            long             cOrg_ID,      ///< [in] 元データのID
                    long             cBuilding_ID, ///< [in] 建物ID
		            CString&         cName         ///< [in] 建物名称
		        );

};

//	ファイルのフルパス名から、シェイプファイル名を抽出する関数
inline bool	MakeShapeFile::
GetFeatureClass ( const std::string&  sourceFile, // ソースファイル名
                  std::string&        className ) // パス名
{
	std::string	tempName;
	std::string	temp;

	tempName	= sourceFile;

	char*	p1	= NULL;
	p1	= strrchr( (char*)tempName.c_str(), _T('\\'));
	if( p1 == NULL ) {
		className	= tempName;	// '\'マークがみつからないのでパス名を含まない
	}else {
		className	= p1+1;
	}
	return	( true );
}

//	フルパス名から、ファイル名を除いて、ディレクトリ名を返す関数
inline bool	MakeShapeFile::
GetFilePath	( const std::string&  sourceFile, // ソースファイル名
              std::string&        filePath )  // パス名
{
	std::string	temp;
	temp	= sourceFile;

	char*		pathEnd;
	pathEnd		= strrchr((char*)temp.c_str(), _T('\\'));
	if( pathEnd == NULL ) {
		return	( false );
	}
	*pathEnd	= _T('\0');		// 文字を区切る
	
	filePath	= temp;
	return	( true );
}

//	データをシェープファイルに変換
inline bool	MakeShapeFile::
Init ( std::string&     c_sOutFName,  // 出力シェイプファイル名
       esriGeometryType c_eGeomType ) // ジオメトリタイプ
{

	HRESULT	hr;
	std::string	a_sPathName;	// ディレクトリ名
	std::string	a_sShpName;		// パスを抜いたシェイプファイル名

	if(!GetFeatureClass ( c_sOutFName, a_sShpName ) ){
		return	( false );
	}
	if(!GetFilePath	( c_sOutFName, a_sPathName ) ){
		return	( false );
	}
		
	//-------------------------------------------------------------------------------
	// Shapeファイルのオープン処理	
	//-------------------------------------------------------------------------------
	IWorkspaceFactoryPtr	a_ipWSFactory(CLSID_ShapefileWorkspaceFactory);
	IPropertySetPtr			a_ipPropertySet(CLSID_PropertySet);
	IWorkspacePtr			a_ipSWSpace;

	if (a_ipWSFactory == 0)
		return	( false );

	// ワークスペースをオープン
	CComVariant		aVar_Dir((char*)a_sPathName.c_str());
	_bstr_t			a_bTStr;
	a_bTStr			= _T("DATABASE");
	if( FAILED (a_ipPropertySet->SetProperty(a_bTStr, aVar_Dir) )) {
		return	( false );
	}
	// ワークスペースのオープン
	if( FAILED (a_ipWSFactory->Open(a_ipPropertySet, 0, &a_ipSWSpace) )) {
		return	( false );
	}
	if(a_ipSWSpace == NULL) {
		return	( false );
	}

	// フィーチャクラスのオープン
	IFeatureWorkspacePtr	ipFWorkspace(a_ipSWSpace);

	IFieldsPtr				ipFields(CLSID_Fields);
	IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
	IGeometryDefEditPtr		ipGDefEdit(ipGDef);
	ISpatialReferencePtr	ipSpRef(CLSID_UnknownCoordinateSystem);

	ipGDefEdit->put_GeometryType(c_eGeomType);
	// 空間参照を設定しないとprjファイルが出力されない
	if( FAILED (ipGDefEdit->putref_SpatialReference(ipSpRef)) ) {
		return	( false );
	}

	// ジオメトリ(Shape)・フィールド追加
	//-------------------------------------------------------------
	IFieldsEditPtr	ipFieldsEdit(ipFields);
	IFieldPtr		ipField(CLSID_Field);

	IFieldEditPtr	ipFieldEdit1(ipField);
	if( SUCCEEDED (ipFieldEdit1->put_Name(T2BSTR(_T("Shape"))) ) &&
		SUCCEEDED (ipFieldEdit1->put_Type(esriFieldTypeGeometry)) &&
		SUCCEEDED (ipFieldEdit1->put_IsNullable(VARIANT_FALSE)) &&
		SUCCEEDED (ipFieldEdit1->putref_GeometryDef(ipGDefEdit) ) ) {
	
		if(ipFieldsEdit->AddField(ipFieldEdit1) != S_OK) {
			return	( false );
		}
	}else {
		return	( false );
	}

	const long cStrMaxSize = 254;

	_bstr_t bsORG_ID = _T("ORG_ID");
	_bstr_t bsBUILDING_ID = _T("BLD_ID");
	_bstr_t bsNAME   = _T("NAME");

	{
		IFieldPtr		ipField2(CLSID_Field);
		IFieldEditPtr	ipFieldEdit2(ipField2);

		// 元データのOBJECTID
		if( SUCCEEDED (ipFieldEdit2->put_Name(bsORG_ID)) &&
			SUCCEEDED (ipFieldEdit2->put_Type(esriFieldTypeInteger) ) &&
			SUCCEEDED (ipFieldEdit2->put_IsNullable(VARIANT_FALSE) ) ) {

			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) {
				return	( false );
			}
		}else {
			return	( false );
		}
	}
	{
		IFieldPtr		ipField2(CLSID_Field);
		IFieldEditPtr	ipFieldEdit2(ipField2);

		// 紐付いた建物のID
		if( SUCCEEDED (ipFieldEdit2->put_Name(bsBUILDING_ID) ) &&
			SUCCEEDED (ipFieldEdit2->put_Type(esriFieldTypeInteger) ) &&
			SUCCEEDED (ipFieldEdit2->put_IsNullable(VARIANT_FALSE) ) ) {

			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) {
				return	( false );
			}
		}else {
			return	( false );
		}
	}
	{
		IFieldPtr		ipField2(CLSID_Field);
		IFieldEditPtr	ipFieldEdit2(ipField2);

		// 元データの名称
		if( SUCCEEDED (ipFieldEdit2->put_Name(bsNAME) ) &&
			SUCCEEDED (ipFieldEdit2->put_Type(esriFieldTypeString) ) &&
			SUCCEEDED (ipFieldEdit2->put_Precision(cStrMaxSize) ) &&
			SUCCEEDED (ipFieldEdit2->put_IsNullable(VARIANT_TRUE) ) ) {
			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) {
				return	( false );
			}
		}else {
			return	( false );
		}
	}


	//-------------------------------------------------------------

	_bstr_t		a_bSName, a_bGeoFName, a_sTmpStr;
	a_bSName	= (char*)a_sShpName.c_str();
	a_bGeoFName	= _T("Shape");
	a_sTmpStr	= _T("");

	if( FAILED ( ipFWorkspace->CreateFeatureClass(a_bSName, ipFields,
		NULL, NULL, esriFTSimple, a_bGeoFName, a_sTmpStr, &m_ipFClass) )){
		return	( false );
	}

	if( FAILED ( m_ipFClass->Insert(VARIANT_TRUE, &m_ipFeatureCursor) ) ) {
		return	( false );
	}

	// 属性フィールドのインデックスを取得
	if( FAILED(m_ipFClass->FindField(bsORG_ID, &m_lORG_ID_Indx ) ) ) {
		return false;
	}
	if( FAILED(m_ipFClass->FindField(bsBUILDING_ID, &m_lBUILDING_ID_Indx ) ) ) {
		return false;
	}
	if( FAILED(m_ipFClass->FindField(bsNAME, &m_lNAME_Indx ) ) ) {
		return false;
	}


	return	( true );
}

//	データをシェープファイルに変換
inline bool	MakeShapeFile::
OuttoShapeFile ( IGeometryPtr& c_ipOutputGeo, // シェイプファイルに出力したいジオメトリ
				 long          cOrg_ID,       // 元データのID
                 long          cBuilding_ID,  // 建物ID
				 CString&      cName )        // 建物名称
{
	//-----------------------------------------------------------------------------------
	// データ出力処理
	//-----------------------------------------------------------------------------------
	// 属性付与
	IFeatureBufferPtr	ipFeatureBuffer;
	if( FAILED (m_ipFClass->CreateFeatureBuffer(&ipFeatureBuffer) ) ) {
		return	( false );
	}
	IClonePtr ipClone;
	if( FAILED (((IClonePtr)c_ipOutputGeo)->Clone (&ipClone) ) ) {
		return (false);
	}
	if( FAILED ( ipFeatureBuffer->putref_Shape(IGeometryPtr(ipClone)) ) ) {
		return	( false );
	}
	if(FAILED (ipFeatureBuffer->put_Value(m_lORG_ID_Indx, CComVariant(cOrg_ID) ) ) ) {
		return	( false );
	}
	if(FAILED (ipFeatureBuffer->put_Value(m_lBUILDING_ID_Indx, CComVariant(cBuilding_ID) ) ) ) {
		return	( false );
	}
	if(FAILED (ipFeatureBuffer->put_Value(m_lNAME_Indx, CComVariant(cName) ) ) ) {
		return	( false );
	}
	_variant_t va;
	if(FAILED ( m_ipFeatureCursor->InsertFeature(ipFeatureBuffer, &va) ) ) {
		return	( false );
	}
	if( FAILED ( m_ipFeatureCursor->Flush() ) ) {
		return	( false );
	}
	return	( true );
}

#endif
