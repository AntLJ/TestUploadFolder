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

#include "ShpMaker.h"
#include <imagehlp.h>

CShpMaker::CShpMaker(void)
{
}


CShpMaker::~CShpMaker(void)
{
	Clear();
}

bool CShpMaker::MakeShape(bool bIsNew, const std::list<PAIR_FIELD>& listField, const std::list<SHAPE_INFO>& listShpInfo, LPCTSTR lpszOutputFile)
{
	if( listShpInfo.empty() )
		return true;

	CString strMsg;

	// シェープ出力のディレクトリ取得(Workspace)
	CString strShpWorkspace( lpszOutputFile );
	long lidx = strShpWorkspace.ReverseFind( '\\' );
	if( lidx == -1 )
		return false;

	// シェープファイル名
	CString strFeatureName = strShpWorkspace.Mid( lidx + 1 );
	strShpWorkspace = strShpWorkspace.Left( lidx );

	IWorkspacePtr ipShpWorkspace = sindy::create_workspace(strShpWorkspace);
	if(!ipShpWorkspace){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgOpenWorkspace, strShpWorkspace);
		return false;
	}
	
	std::list<SHAPE_INFO>::const_iterator itTemp = listShpInfo.begin();		// 形状を持っている場合にフィールド作成のため渡す

	// メンバー変数[m_ipFeatureClass][m_mapFieldIndex]を確保する
	if( !CreateNewShape( bIsNew, ipShpWorkspace, strShpWorkspace, strFeatureName, listField, itTemp->m_ipGeom.m_T, m_ipTable, m_mapFieldIndex ) )
		return false;

	if( !AddData( listShpInfo ) )
		return false;

	return true;
}

bool CShpMaker::AddData(const std::list<SHAPE_INFO>& listShpInfo)
{
	if( !m_ipTable || m_mapFieldIndex.empty() )
		return false;

	CString strMsg;
	IFieldsPtr ipNewFields;		// フィーチャクラスを作成後、フィールドを再度取得する

	// テーブルとフィーチャクラス両方に対応するために、どちらも定義しておく
	_ICursorPtr ipCursor;

	bool bIsGeom = (IFeatureClassPtr)m_ipTable ? true : false;		// フィーチャクラスかどうかで切り分けを行う

	m_ipTable->get_Fields( &ipNewFields );
	if(bIsGeom){
		IFeatureCursorPtr ipFeatureCursor;
		if(FAILED(((IFeatureClassPtr)m_ipTable)->Insert(VARIANT_TRUE, &ipFeatureCursor))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetCursor, _T(""), m_ipTable);
			return false;
		}
		ipCursor = ipFeatureCursor;
	} else {
		if(FAILED(m_ipTable->Insert(VARIANT_TRUE, &ipCursor))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetCursor, _T(""), m_ipTable);
			return false;
		}
	}

	// フィーチャや属性のストア
	for(std::list<SHAPE_INFO>::const_iterator it = listShpInfo.begin(); it != listShpInfo.end(); ++it){
		IRowBufferPtr ipRowBuffer;
		if(bIsGeom){
			IFeatureBufferPtr ipFeatureBuffer;
			((IFeatureClassPtr)m_ipTable)->CreateFeatureBuffer(&ipFeatureBuffer);
			if(!ipFeatureBuffer){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetRowBuffer, _T(""), m_ipTable);
				return false;
			}
			if(S_OK != ipFeatureBuffer->putref_Shape(it->m_ipGeom.m_T)){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutRowShape, _T(""), m_ipTable);
				return false;
			}
			ipRowBuffer = ipFeatureBuffer;
		} else {
			m_ipTable->CreateRowBuffer(&ipRowBuffer);
			if(!ipRowBuffer){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetRowBuffer, _T(""), m_ipTable);
				return false;
			}
		}
		for(std::map<DEST_COLNAME, CString>::const_iterator itCol = it->m_mapValue.begin(); itCol != it->m_mapValue.end(); ++itCol){
			// フィールドのインデックスを取得する
			std::map<CString, long>::iterator itFieldIdx = m_mapFieldIndex.find(itCol->first);
			if(itFieldIdx == m_mapFieldIndex.end())
				continue;

			// フィールドのタイプを取得する
			IFieldPtr ipField;
			ipNewFields->get_Field(itFieldIdx->second, &ipField);
			esriFieldType esFieldType = esriFieldTypeInteger;
			ipField->get_Type(&esFieldType);

			CComVariant cComVal;
			switch(esFieldType)
			{
				case esriFieldTypeSmallInteger:
				case esriFieldTypeInteger:
				{
					if(!itCol->second.IsEmpty()){
						CComVariant cVal(_ttol(itCol->second));
						cComVal = cVal;
					}
					else
						cComVal = SHAPE_NUMERIC_NULL;
					break;
				}
				case esriFieldTypeString:
					if(!itCol->second.IsEmpty())
						cComVal = itCol->second;
					else
						cComVal = SHAPE_STRING_NULL;
					break;
				case esriFieldTypeSingle:
				case esriFieldTypeDouble:
				{
					if(!itCol->second.IsEmpty()){
						CComVariant cVal(_ttof(itCol->second));
						cComVal = cVal;
					}
					else
						cComVal = SHAPE_NUMERIC_NULL;
					break;
				}
				case esriFieldTypeDate:
				{
					cComVal = itCol->second;
					cComVal.ChangeType(VT_DATE);
					break;
				}
				default:
					CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgFieldType, itCol->first, m_ipTable);
					return false;
					break;
			}
			if(FAILED(ipRowBuffer->put_Value(itFieldIdx->second, cComVal))){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutRowAttr, itCol->first, m_ipTable);
				return false;
			}
		}
		CComVariant vaTemp;
		if(bIsGeom){
			if(FAILED(((IFeatureCursorPtr)ipCursor)->InsertFeature((IFeatureBufferPtr)ipRowBuffer, &vaTemp))){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAddRow, _T(""), m_ipTable);
				return false;
			}
		} else {
			if(FAILED(ipCursor->InsertRow(ipRowBuffer, &vaTemp))){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAddRow, _T(""), m_ipTable);
				return false;
			}
		}
	}

	if(FAILED(ipCursor->Flush())){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgFlush, _T(""), m_ipTable);
		return false;
	}

	return true;
}

IFieldsPtr CShpMaker::CreateFields(const std::list<PAIR_FIELD>& listField, const IGeometryPtr& ipGeom)
{
	IFieldsEditPtr ipFields( CLSID_Fields );

	CString strMsg;
	for(std::list<PAIR_FIELD>::const_iterator it = listField.begin(); it != listField.end(); ++it){
		if(FAILED(ipFields->AddField(it->second.m_T))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAddField, _T(""), m_ipTable);
			return NULL;
		}
	}

	// 形状フィールドの作成
	if(ipGeom){
		IFieldEditPtr ipShapeField(CLSID_Field);
		if(FAILED(ipShapeField->put_Name(CComBSTR(_T("shape"))))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T(""), m_ipTable);
			return NULL;
		}
		if(FAILED(ipShapeField->put_Type(esriFieldTypeGeometry))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T(""), m_ipTable);
			return NULL;
		}
		IGeometryDefEditPtr ipGeomDefEdit(CLSID_GeometryDef);

		IUnknownCoordinateSystemPtr ipUnkCoordSys(CLSID_UnknownCoordinateSystem);
		if(FAILED(ipGeomDefEdit->putref_SpatialReference(ipUnkCoordSys))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T(""), m_ipTable);
			return NULL;
		}

		esriGeometryType esriType = esriGeometryNull;
		ipGeom->get_GeometryType(&esriType);
		if(FAILED(ipGeomDefEdit->put_GeometryType(esriType))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T(""), m_ipTable);
			return NULL;
		}

		if(FAILED(ipShapeField->putref_GeometryDef(ipGeomDefEdit))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T(""), m_ipTable);
			return NULL;
		}

		if(FAILED(ipFields->AddField(ipShapeField))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgAddField, _T(""), m_ipTable);
			return NULL;
		}
	}

	return ipFields;
}

bool CShpMaker::DeleteShape(const CString& strFile)
{
	CString strMsg;

	// 既にshpが存在している場合は削除
	static LPCTSTR lpszSuffix[] = {_T("shp"), _T("shx"), _T("dbf")};
	for(int i=0; i < 3; ++i){
		CString strChkPath;
		strChkPath.Format(_T("%s.%s"), strFile, lpszSuffix[i]);
		if(PathFileExists(strChkPath)){
			if(!DeleteFile(strChkPath)){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgDelFile, strChkPath);
				return false;
			}
		}
	}
	return true;
}

bool CShpMaker::CreateNewShape(bool bIsNew
							   , IWorkspacePtr ipWorkspace
							   , const CString& strShpDir
							   , const CString& strFeatureName
							   , const std::list<PAIR_FIELD>& listField
							   , const IGeometryPtr& ipGeom
							   , ITablePtr& ipOutTable
							   , std::map<CString, long>& mapOutFieldIndex)
{
	if( !ipWorkspace || strFeatureName.IsEmpty() )
		return false;

	CString strMsg;

	if(bIsNew){
		if(!DeleteShape( strShpDir + _T("\\") + strFeatureName ))
			return false;

		// フィールドの作成
		IFieldsPtr ipFields = CreateFields( listField, ipGeom );
		if(!ipFields){
			return false;
		}

		// 形状のある無しで処理を分ける
		if(ipGeom){
			IFeatureClassPtr ipFeatureC;
			((IFeatureWorkspacePtr)ipWorkspace)->CreateFeatureClass(CComBSTR(strFeatureName), ipFields, 0, 0, esriFTSimple, CComBSTR(_T("shape")), CComBSTR(_T("")), &ipFeatureC);
			ipOutTable = ipFeatureC;
		} else {
			((IFeatureWorkspacePtr)ipWorkspace)->CreateTable(CComBSTR(strFeatureName), ipFields, 0, 0, _T(""), &ipOutTable);
		}
		if(!ipOutTable){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgCreateTable, strFeatureName);
			return false;
		}
	} else {
		// 形状のある無しで処理を分ける
		if(ipGeom){
			IFeatureClassPtr ipFeatureC;
			((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(strFeatureName), &ipFeatureC);
			ipOutTable = ipFeatureC;
		}
		else
			((IFeatureWorkspacePtr)ipWorkspace)->OpenTable(CComBSTR(strFeatureName), &ipOutTable);

		if(!ipOutTable){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgCreateTable, strFeatureName);
			return false;
		}
	}

	IFieldsPtr ipFields;
	if(S_OK != ipOutTable->get_Fields(&ipFields) || !ipFields){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetFields, _T(""), ipOutTable);
		return false;
	}

	for(std::list<PAIR_FIELD>::const_iterator itCol = listField.begin(); itCol != listField.end(); ++itCol){
		long lidx = -1;
		if(FAILED(ipFields->FindField(CComBSTR(itCol->first), &lidx))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetFieldIndex, _T(""), ipOutTable);
			return false;
		}
		mapOutFieldIndex[itCol->first] = lidx;
	}

	return true;
}

