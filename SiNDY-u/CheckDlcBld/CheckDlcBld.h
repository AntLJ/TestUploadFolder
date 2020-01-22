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

#include "Arguments.h"

class CCheckDlcBld {
public:
	CCheckDlcBld(){}
	~CCheckDlcBld(){}

public:
	/**
	 * @brief �������֐�
	 * @param args   [in] �R�}���h���C������
	 * @return true  ��������
	 * @return false �������s
	 */
	bool init(const Arguments& args);

	/**
	 * @brief �������s�֐�
	 * @param bErr   [out] �`�F�b�N�����ɃG���[�����݂������ǂ����itrue=�G���[�����݂���, false=���݂��Ȃ��j
	 * @return true  ��������
	 * @return false �������s
	 */
	bool run(bool& bErr);

private:
	/**
	 * @brief ���b�V�����X�g�擾�֐�
	 * @param strMeshlist   [in] ���b�V�����X�g�̃p�X
	 * @param setMesh       [out] ���b�V�����X�g
	 * @return true  ��������
	 * @return false �������s
	 */
	bool getMeshlist(const CString& strMeshlist, std::set<CString>& setMesh);

	/**
	 * @brief shapefile�̃p�X�쐬�֐�
	 * @param strMeshlist   [in] strMesh�p�X
	 * @return shapefile�ւ̃p�X
	 */
	CString getShapeWorkspacePath(const CString& strMesh);

	/**
	 * @brief �����`����x�[�X�ɑ������`�󂪓����Ă��邩�`�F�b�N����֐�
	 * @param strBldName    [in] �������C����
	 * @param lBldOid       [in] �����t�B�[�`��OBJECTID
	 * @param ipBldGeom     [in] �����`��
	 * @param strDlcName    [in] ���������C����
	 * @param lDlcFid       [in] �������t�B�[�`��FID�iOBJECTID�ł͂Ȃ��j
	 * @param ipDlcGeom     [in] �������`��
	 * @param bMatch        [out] �����`��ɓ����Ă��邩�ǂ����itrue=����Ă���, false=����Ă��Ȃ��j
	 * @return true  ��������
	 * @return false �������s
	 */
	bool MatchingBldDlc(const CString& strBldName, long lBldOid, IGeometryPtr ipBldGeom, const CString& strDlcName, long lDlcFid, IGeometryPtr ipDlcGeom, bool& bMatch);

	/**
	 * @brief �����`����x�[�X�ɑ������`�󂪓����Ă��邩�`�F�b�N����֐�
	 * @param strBldName            [in] �������C����
	 * @param lBldOid               [in] �����t�B�[�`��OBJECTID
	 * @param ipBldGeom             [in] �����`��
	 * @param strDlcName            [in] ���������C����
	 * @param lDlcFid               [in] �������t�B�[�`��FID�iOBJECTID�ł͂Ȃ��j
	 * @param vecDlcSegGeom         [in] �������Z�O�����g���_�Q
	 * @param vecNotMatchDlcSegGeom [out] �}�b�`���O���Ȃ������������Z�O�����g���_
	 * @return true  ��������
	 * @return false �������s
	 */
	bool MatchingBldDlcSegment(const CString& strBldName, long lBldOid, IGeometryPtr ipBldGeom, const CString& strDlcName, long lDlcFid, const std::vector<IGeometryPtr>& vecDlcSegGeom, std::vector<IGeometryPtr>& vecNotMatchDlcSegGeom);

	/**
	 * @brief �`����Z�O�����g�P�ʂɕ������A�Z�O�����g�̒��_���擾����֐�
	 * @param strTableName          [in] ���C����
	 * @param lOID                  [in] �t�B�[�`��OID
	 * @param ipGeom                [in] �`��
	 * @param vecSegCenterPointGeom [out] �Z�O�����g�̒��_�`�󃊃X�g
	 * @return true  ��������
	 * @return false �������s
	 */
	bool GetSegmentCenterPoints(const CString& strTableName, long lOID, IGeometryPtr ipGeom, std::vector<IGeometryPtr>& vecSegCenterPointGeom);

	/**
	 * @brief �ߗׂ̌����Ō�������֐�
	 * @param strBldName        [in] �������C����
	 * @param lBldOid           [in] �����t�B�[�`��OBJECTID
	 * @param ipBldClass        [in] �����t�B�[�`���N���X
	 * @param lBldObjectIdIndex [in] ������OBJECTID�t�B�[���h�C���f�b�N�X
	 * @param strDlcName        [in] ���������C����
	 * @param lDlcFid           [in] �������t�B�[�`��FID�iOBJECTID�ł͂Ȃ��j
	 * @param ipDlcGeom         [in] �������`��
	 * @param vecDlcSegGeom     [in] �������Z�O�����g���_�Q
	 * @param bSomeMatch        [in] �ꕔ�}�b�`���Ă��邩�ǂ���
	 * @param bMsg              [in] �}�b�`���O���̃��b�Z�[�W���o�͂��邩�ǂ����itrue=�o�͂���, false=�o�͂��Ȃ��j
	 * @return true  ��������
	 * @return false �������s
	 */
	bool ArouondBld(const CString& strBldName, long lBldOid, IFeatureClassPtr ipBldClass, long lBldObjectIdIndex, const CString& strDlcName, long lDlcOid, IGeometryPtr ipDlcGeom, const std::vector<IGeometryPtr>& vecDlcSegGeom, bool bSomeMatch, bool bMsg = true);

private:
	ISpatialReferencePtr m_ipSpRef;   //!< ��ԎQ��
	std::set<CString> m_setMesh;      //!< ���b�V�����X�g
	CString m_strShapeDir;            //!< �`�F�b�N�Ώ�Shapefile�̃x�[�X�p�X
};
