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

#include "diffshapeadd.h"

// ��������Ă邯��...WKSPoint���p�ł�...
struct XY {
	double X;
	double Y;
	bool operator<(const XY& cXY) const {
		return (this->X != cXY.X ? this->X < cXY.X : this->Y < cXY.Y);
	}
	bool operator==(const XY& cXY) const {
		return (this->X == cXY.X && this->Y == cXY.Y);
	}
};

// ����SHP�̔F���p�i�����NID�̓��b�V���P�ʂŃ��j�[�N�̂��߁j
struct CLinkOID {
	long lMesh;
	long lFID;
	bool operator<(const CLinkOID& cLinkOID) const {
		return (this->lMesh != cLinkOID.lMesh ? this->lMesh < cLinkOID.lMesh : this->lFID < cLinkOID.lFID);
	}
	bool operator==(const CLinkOID& cLinkOID) const {
		return (this->lMesh == cLinkOID.lMesh && this->lFID == cLinkOID.lFID);
	}
};

// ����SHP�f�[�^�p�̍\����
struct CDiffData {
	CLinkOID cLinkOID;
	long lMID;
	long lMDivision;
	IGeometryPtr ipGeom;
};

class CDiffAddSDE {
public:
	CDiffAddSDE() : m_lOldNodeID(0) {}
	CDiffAddSDE(CInputData& cInputData) : m_lOldNodeID(0) { m_cInputData = cInputData; }
	~CDiffAddSDE(){}

public:
	bool run();

private:
	// �P�Ȃ�f�B���N�g����͏���
	bool get_diffdata();

	// ���[�N�X�y�[�X�I�[�v���̂�
	bool get_diffdata(const CString& strDiffWork, const CString& strMesh);

	// �����f�[�^��ێ�����
	bool get_diffdata_main(IWorkspace* ipDiffWork, IFeatureClass* ipDiffLinkClass, const CString& strMesh);

	// �����f�[�^��FromTo�m�[�h�̐V��ID�����ւ��Ȃ���C���|�[�g
	bool diff_import();

	// �����N�̃��b�V�����E�㏈���i2.0�Z�����鏈���j
	IGeometryPtr replaceMeshEdge(IGeometry* ipGeom, bool bFrom);

	// �����N�̃��b�V�����E�㏈�����ɔ�������s�v�m�[�h�폜����
	bool delete_createnode(long lNewOID);

	// �����N�̃��b�V�����E�㏈�����ɔ�������m�[�h�����X�V����
	bool update_createnode(long lNewOID, long lNodeCount);

	// �w��t�B�[���h�̒l��t�^���ăt�B�[�`���V�K�쐬
	IFeatureBufferPtr create_FeatureBuffer(IFeatureClass* ipFeatureClass, IGeometry* ipGeom, std::map<long,CComVariant>& mapFieldValue);

	// ���݂̎��Ԃ��擾
	CString GetNowTime();

	// ���R���T�C���|�X�g�p�֐�
	bool DoReconcile(IWorkspaceEdit* ipWorkspaceEdit);

	/**
	 * @brief	���܂��Ȃ�
	 * @note	SDE_LOGFILE�������I�ɍ쐬�����鏈���i�P��GetRows()���邾��������...�j
	 */
	void omajinai();

private:
	CInputData m_cInputData;
	crd_cnv m_cnv;

	// �C���|�[�g�p�̃J�[�\��
	IFeatureCursorPtr m_ipImportLinkCursor;
	IFeatureCursorPtr m_ipImportNodeCursor;

	// SDE�̋�ԎQ�Ɓi�C���|�[�g���ɍ����f�[�^�̌`��ɓ��č��ނ��߁j
	ISpatialReferencePtr m_ipLinkSpRef;
	ISpatialReferencePtr m_ipNodeSpRef;


	// �C���|�[�g�f�[�^�̐V��ID�Ή��}�b�v
	long m_lOldNodeID;
	std::map<long,long> m_mapOldNewNode;
	std::map<long,long> m_mapOldNewLink;
	std::map<XY,long> m_mapOldNodeXY;
	std::map<long,XY> m_mapOldNodeXYs;
	std::map<long,std::set<CLinkOID>> m_mapOldNodeLinks;
	std::map<CLinkOID,std::pair<long,long>> m_mapOldLinkFTNode;
	std::map<long,XY> m_mapOldNodeData;
	std::map<CLinkOID,CDiffData> m_mapOldLinkData;


	// SDE�̃t�B�[���h�C���f�b�N�X
	long m_lNodeClassIndex;
	long m_lRoadClassIndex;
	long m_lNaviClassIndex;
	long m_lMainLinkClassIndex;
	long m_lRoadWidthIndex;
	long m_lUpDownClassIndex;
	long m_lFromNodeIndex;
	long m_lToNodeIndex;
	long m_lMIDIndex;
	long m_lMDivision;

	// �V�K�ǉ����̏����l�ݒ�p
	std::map<long,CComVariant> m_mapNodeFieldValue;
	std::map<long,CComVariant> m_mapLinkFieldValue;
/*
> �ERoadClass_c		6(���̑���)
> �ENAVIClass_c		6(���̑���)
> �EMain_LinkClass_c	1(�{���㉺�񕪗������N)
> �ERoadWidth_c		2(����5.5m�ȏ�13.0m����)
> �EUPDOWNClass_c		0(�ΏۊO)
*/
};

