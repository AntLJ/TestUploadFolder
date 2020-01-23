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
#include "common.h"

bool GetLonLatData(const CString& strTableName, IGeometryPtr ipGeom, std::vector<LONLAT>& vecLonLat)
{
	IPointCollectionPtr ipPointCol(ipGeom);
	if(ipPointCol){
		long lCount = 0;
		if(!GetPointCount(strTableName, ipPointCol, lCount))
			return false;

		std::vector<WKSPoint> wksPoint(lCount);
		if(!GetQueryWKSPoints(strTableName, ipPointCol, lCount, wksPoint))
			return false;

		for(long l = 0; l < lCount; ++l){
			LONLAT cLonLat;
			cLonLat.eLon = wksPoint[l].X;
			cLonLat.eLat = wksPoint[l].Y;
			vecLonLat.push_back(cLonLat);
		}
	} else {
		IPointPtr ipPoint(ipGeom);
		if(!ipPoint)
			return false;

		LONLAT cLonLat;
		ipPoint->QueryCoords(&cLonLat.eLon, &cLonLat.eLat);
		vecLonLat.push_back(cLonLat);
	}
	return true;
}

bool GetWorkspace(const CString& strConnectInfo, IWorkspacePtr& ipWorkspace)
{
	ipWorkspace = sindy::create_workspace(strConnectInfo);
	if(!ipWorkspace){
		CString strMsg;
		strMsg.Format(_T("%s�̐ڑ��Ɏ��s���܂���"), strConnectInfo);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetFeatureClass(const CString& strTableName, IWorkspacePtr ipWorkspace, IFeatureClassPtr& ipFeatureClass)
{
	if(S_OK != IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(CComBSTR(strTableName), &ipFeatureClass)){
		CString strMsg;
		strMsg.Format(_T("%s�t�B�[�`���N���X�擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetTable(const CString& strTableName, IWorkspacePtr ipWorkspace, ITablePtr& ipTable)
{
	if(S_OK != IFeatureWorkspacePtr(ipWorkspace)->OpenTable(CComBSTR(strTableName), &ipTable)){
		CString strMsg;
		strMsg.Format(_T("%s�e�[�u���擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetFieldCount(const CString& strTableName, IFieldsPtr ipFields, long& lCount)
{
	if(S_OK != ipFields->get_FieldCount(&lCount)){
		CString strMsg;
		strMsg.Format(_T("%s�̃t�B�[���h�J�E���g�̎擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetFields(const CString& strTableName, IFeatureClassPtr ipFeatureClass, IFieldsPtr& ipFields)
{
	if(S_OK != ipFeatureClass->get_Fields(&ipFields)){
		CString strMsg;
		strMsg.Format(_T("%s�̃t�B�[���h�Q�̎擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetFields(const CString& strTableName, ITablePtr ipTable, IFieldsPtr& ipFields)
{
	if(S_OK != ipTable->get_Fields(&ipFields)){
		CString strMsg;
		strMsg.Format(_T("%s�̃t�B�[���h�Q�̎擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetField(const CString& strTableName, IFieldsPtr ipFields, long lIndex, IFieldPtr& ipField)
{
	if(S_OK != ipFields->get_Field(lIndex, &ipField)){
		CString strMsg;
		strMsg.Format(_T("%s�̃t�B�[���h�̎擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetSpatialFilter(IGeometryPtr ipSpGeom, ISpatialFilterPtr& ipSpFilter)
{
	if(S_OK != ipSpFilter->putref_Geometry(ipSpGeom) ||
		S_OK != ipSpFilter->put_SpatialRel(esriSpatialRelIntersects)){
			CString strMsg;
			strMsg.Format(_T("��Ԍ����`��ݒ�Ɏ��s���܂���"));
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
			return false;
	}
	return true;
}

bool GetFeatureCursor(const CString& strTableName, IFeatureClassPtr ipFeatureClass, ISpatialFilterPtr ipSpFilter, IFeatureCursorPtr& ipFeatureCursor)
{
	if(S_OK != ipFeatureClass->Search(ipSpFilter, VARIANT_FALSE, &ipFeatureCursor)){
		CString strMsg;
		strMsg.Format(_T("%s�̃t�B�[�`���J�[�\���擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetFeatureCount(const CString& strTableName, IFeatureClassPtr ipFeatureClass, ISpatialFilterPtr ipSpFilter, long& lCount)
{
	if(S_OK != ipFeatureClass->FeatureCount(ipSpFilter, &lCount)){
		CString strMsg;
		strMsg.Format(_T("%s�̃t�B�[�`�����擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetCursor(const CString& strTableName, ITablePtr ipTable, IQueryFilterPtr ipQFilter, _ICursorPtr& ipCursor)
{
	if(S_OK != ipTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor)){
		CString strMsg;
		strMsg.Format(_T("%s�̃J�[�\���擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetRowCount(const CString& strTableName, ITablePtr ipTable, IQueryFilterPtr ipQFilter, long& lCount)
{
	if(S_OK != ipTable->RowCount(ipQFilter, &lCount)){
		CString strMsg;
		strMsg.Format(_T("%s�̃��R�[�h���擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetValue(const CString& strTableName, const CString& strFieldName, IFeaturePtr ipFeature, long lIndex, CComVariant& vaValue)
{
	if(S_OK != ipFeature->get_Value(lIndex, &vaValue)){
		CString strMsg;
		strMsg.Format(_T("%s��%s�t�B�[���h�̒l�擾�Ɏ��s���܂���"), strTableName, strFieldName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetValue(const CString& strTableName, const CString& strFieldName, _IRowPtr ipRow, long lIndex, CComVariant& vaValue)
{
	if(S_OK != ipRow->get_Value(lIndex, &vaValue)){
		CString strMsg;
		strMsg.Format(_T("%s��%s�t�B�[���h�̒l�擾�Ɏ��s���܂���"), strTableName, strFieldName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetGeometry(const CString& strTableName, IFeaturePtr ipFeature, IGeometryPtr& ipGeometry)
{
	if(S_OK != ipFeature->get_ShapeCopy(&ipGeometry)){
		CString strMsg;
		strMsg.Format(_T("%s��OID:%ld�̌`��擾�Ɏ��s���܂���"), strTableName, ipFeature->GetOID());
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetPointCount(const CString& strTableName, IPointCollectionPtr ipPointCol, long& lCount)
{
	if(S_OK != ipPointCol->get_PointCount(&lCount)){
		CString strMsg;
		strMsg.Format(_T("%s�̍\���_���擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetQueryWKSPoints(const CString& strTableName, IPointCollectionPtr ipPointCol, long lSize, std::vector<WKSPoint>& vecWKSPoint)
{
	if(S_OK != ipPointCol->QueryWKSPoints(0, lSize, &vecWKSPoint[0])){
		CString strMsg;
		strMsg.Format(_T("%s��WKS�|�C���g�Q�擾�Ɏ��s���܂���"), strTableName);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}

bool GetMeshGeom(const CString& strMeshCode, IFeatureClassPtr ipMeshClass, IGeometryPtr& ipMeshGeom)
{
	CString strMsg;
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	CString strWhere;
	strWhere.Format(_T("%s=%s"), sindy::schema::citymesh::kCityMeshCode, strMeshCode); //!< ���b�V���R�[�h�́uMESHCODE�v�Ȃ̂łǂ���g���Ă�OK
	if(S_OK != ipQueryFilter->put_WhereClause(CComBSTR(strWhere))){
		strMsg.Format(_T("Where��̐ݒ�Ɏ��s���܂���"));
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	IFeatureCursorPtr ipFeatureCursor;
	if(S_OK != ipMeshClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor)){
		strMsg.Format(_T("���b�V���p�J�[�\���擾�Ɏ��s���܂���"));
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	IFeaturePtr ipFeature;
	if(S_OK != ipFeatureCursor->NextFeature(&ipFeature) || !ipFeature){
		strMsg.Format(_T("���b�V���t�B�[�`���擾�Ɏ��s���܂���"));
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	if(S_OK != ipFeature->get_ShapeCopy(&ipMeshGeom) || !ipMeshGeom){
		strMsg.Format(_T("���b�V���`��擾�Ɏ��s���܂���"));
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}
	return true;
}
