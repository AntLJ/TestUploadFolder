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

// AChiban.cpp: AChiban �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AChiban.h"

namespace{
	LPCTSTR ANNOF_FIELD = _T("ANNO_F");	///< ���L�t���O�t�B�[���h�� 
}
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//�@�R���X�g���N�^ 
AChiban::AChiban()
:m_MeshCode(-1),
 m_AnnofIndex(-1)
{

}

//�@�f�X�g���N�^
AChiban::~AChiban()
{

}

//�@������
bool AChiban::init(IFeatureClassPtr ipChibanClass, bool cChgMode)
{
	m_ChgMode = cChgMode;
	m_ipRep = ipChibanClass;

	m_ipRep->FindField(CComBSTR(ANNOF_FIELD), &m_AnnofIndex);
	if(m_AnnofIndex < 0){
		_ftprintf(stderr, _T("#�t�B�[���h��������܂���B,%s\n"), ANNOF_FIELD);
		return false;
	}

	//���O�p�Ƀt�B�[�`���N���X�����擾
	CComBSTR aLayerName;
	((IDatasetPtr)m_ipRep)->get_Name(&aLayerName);
	m_LayerName = aLayerName;

	return true;
}

//�@���b�V�����E��̒n�ԑ�\�_�̒��L�t���O���I�t�ɂ���
bool AChiban::execAdjust(IFeaturePtr ipMesh, LONG cMeshCode)
{
	m_MeshCode = cMeshCode;
	_tprintf(_T("Now %d\r"), m_MeshCode);

	//���b�V���|���S�����烁�b�V���g���쐬
	IGeometryPtr ipGeom;
	ipMesh->get_Shape(&ipGeom);
	ITopologicalOperatorPtr ipTopo(ipGeom);
	IGeometryPtr ipBoundary;
	ipTopo->get_Boundary(&ipBoundary);

	//�`�F�b�N�y�ѕύX
	return fnCheck(ipBoundary);
}

//�@�w��W�I���g����̒n�ԑ�\�_�Œ��L�t���O�������Ă�����̂����o����B�ύX���[�h�̎��̓I�t�ɂ���
bool AChiban::fnCheck(IGeometryPtr ipBoundary)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR bstrName;
	m_ipRep->get_ShapeFieldName( &bstrName );
	ipFilter->put_GeometryField(bstrName);
	ipFilter->putref_Geometry(ipBoundary);
	ipFilter->put_SpatialRel(esriSpatialRelIntersects);

	bool aResult = false;
	IFeatureCursorPtr ipCursor;
	if(m_ChgMode){
		if(m_ipRep->Update(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
			_ftprintf(stderr, _T("#���E��̃|�C���g�������s,%d\n"), m_MeshCode);
			return false;
		}
	}else{
		if(m_ipRep->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
			_ftprintf(stderr, _T("#���E��̃|�C���g�������s,%d\n"), m_MeshCode);
			return false;
		}
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		CComVariant aAnnoF;
		ipFeature->get_Value(m_AnnofIndex, &aAnnoF);
		if(aAnnoF.lVal != 0){
			LONG aObjID;
			ipFeature->get_OID(&aObjID);
			if(m_ChgMode){
				//�ύX���[�h�̎������X�V
				ipFeature->put_Value(m_AnnofIndex, CComVariant(0));
				ipCursor->UpdateFeature(ipFeature);
				_ftprintf(stderr, _T("0\t%s\t%d\t\t\tInfo\t0\t�t���O�X�V�B\t\n"), m_LayerName, aObjID);
			}else{
				//�`�F�b�N���[�h�i�ύX���[�h�ł͂Ȃ��j�̎��́A���O�o�͂̂�
				_ftprintf(stderr, _T("0\t%s\t%d\t\t\tInfo\t0\t�t���O�X�V�\�B\t\n"), m_LayerName, aObjID);
			}
		}
	}
	return aResult;
}
