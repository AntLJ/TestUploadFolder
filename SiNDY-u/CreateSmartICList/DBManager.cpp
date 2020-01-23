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
#include "DBManager.h"
#include <atl2/seal.h>

CDBManager::CDBManager(void)
{
}

CDBManager::~CDBManager(void)
{
}

//where����w�肵�ăt�B�[�`���J�[�\���̎擾
IFeatureCursorPtr CDBManager::GetFeatureCursor(IFeatureClassPtr ipFeatureClass, CComBSTR bstrWhereClause)
{
	return ipFeatureClass->_Search(CreateQueryFilter(bstrWhereClause), VARIANT_FALSE);
}


//�w�肵���`���esriSpatialRelEnum�ł��������Ɉ�v����t�B�[�`���J�[�\���̎擾
IFeatureCursorPtr CDBManager::GetFeatureCursor(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom, esriSpatialRelEnum eStType, LPCTSTR lpcszSubFields /*NULL*/)
{	
	//�w��W�I���g���ɂ���f�[�^�݂̂���������
	ISpatialFilterPtr	ipSpatialFilter(CLSID_SpatialFilter);
						ipSpatialFilter->putref_Geometry(ipGeom);
						ipSpatialFilter->put_SpatialRel(eStType);
	if(lpcszSubFields != NULL)
		ipSpatialFilter->put_SubFields(CComBSTR(lpcszSubFields));

	// �e�[�u�����������ăJ�[�\���Ɋi�[
	return ipFeatureClass->_Search( ipSpatialFilter, VARIANT_FALSE);
}

//�N�G���t�B���^�̍쐬
IQueryFilterPtr CDBManager::CreateQueryFilter(CComBSTR bstrWhereClause)
{
	IQueryFilterPtr		ipQueryFilter;
	ipQueryFilter.CreateInstance(CLSID_QueryFilter);
	if( FAILED(ipQueryFilter->put_WhereClause(bstrWhereClause)))
		throw std::runtime_error(_T("Where��̐ݒ�Ɏ��s."));

	return ipQueryFilter;
}

//OID�̎擾
long CDBManager::GetOID(const _IRowPtr& ipRow)
{
	long lOID;
	ipRow->get_OID(&lOID);

	return lOID;
}

//�`��̎擾
IGeometryPtr CDBManager::GetGeom(const IFeaturePtr& ipFeature)
{
	IGeometryPtr ipGeom;

	ipFeature->get_ShapeCopy(&ipGeom);
	
	return ipGeom;
}

//OID�ƃt�B�[�`���N���X����`��̎擾
IGeometryPtr CDBManager::GetGeomFromOID(long lOID, IFeatureClassPtr ipFC)
{
	CString strWhereClause;
	strWhereClause.Format(_T("OBJECTID = %ld"), lOID);	

	return GetGeom((GetFeatureCursor(ipFC, (CComBSTR)strWhereClause)->_NextFeature()));
}

//�w�肵���`��Ɏw�肵�������Ńq�b�g�����`��̓���̏������Z�b�g�Ɋi�[���ĕԂ�
std::set<CAdapt< CComVariant>> CDBManager::GetHitValSet(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom, long lFieldIndex, esriSpatialRelEnum eStType)
{
	std::set<CAdapt< CComVariant>> vaSet;
	
	IFeatureCursorPtr ipFCr = GetFeatureCursor(ipFeatureClass, ipGeom, eStType);

	IFeaturePtr ipFeature;

	while((S_OK == ipFCr->NextFeature(&ipFeature))){
		vaSet.insert(GetValue(ipFeature, lFieldIndex));
	}

	return vaSet;
}

//�l�̎擾
CComVariant CDBManager::GetValue(_IRowPtr ipRow, long lFieldIndex)
{
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;

	if( lFieldIndex >= 0 ){
		ipRow->get_Value( lFieldIndex, &vaValue );
	}
	return vaValue;
}
