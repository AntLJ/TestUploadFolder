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
#include "SDEManager.h"
#include <atl2/seal.h>

struct FacilInfo
{
	long	eRoad_Code;
	long	eRoad_Seq;
};

class CDoc
{
private:
	/**
	 * @brief DB�����N���X
	 */
	CSDEManager m_cSDEManager;
	/**
	 * @brief �ΏۃT�[�o��
	 */
	CString m_strServer;
	/**
	 * @brief �ΏۃC���X�^���X
	 */
	CString m_strInstance;
	/**
	 * @brief �Ώۃ��[�U��
	 */
	CString m_strUser;
	/**
	 * @brief �Ώۃp�X���[�h
	 */
	CString m_strPassword;
	/**
	 * @brief �Ώۃo�[�W����
	 */
	CString m_strVersion;
	/**
	 * @brief �o�̓f�B���N�g��
	 */
	CString m_strOutputDir;
	/**
	 * @brief �Ώ�DB���[�N�X�y�[�X
	 */
	IWorkspacePtr	m_ipWorkspace;
	/**
	 * @brief �o�̓X�g���[��
	 */
	ofstream m_ofstream;
public:
	/*
	 * @brief �R���X�g���N�^
	 */
	CDoc(void);
	/*
	 * @brief �f�X�g���N�^
	 */
	~CDoc(void);
	/**
	 * @brief �C���|�[�g�ݒ�̎擾
	 * @note  �ݒ�t�@�C����ǂݍ��݁A�C���|�[�g�̐ݒ���擾����
	 * @param strConfigFile [in] �ݒ�t�@�C����
	 * @retval true �擾����
	 * @retval false �擾���s
	 */
	bool getenv( const char* strConfigFile );
	/**
	 * @brief ���C���̏���
	 * @return bool
	 */
	bool Proc(void);
	/**
	 * @brief FACIL_INFO_POINT����X�}�[�gIC��ROAD_CODE,ROAD_SEQ�̎擾
	 * @param r_eFacilInfoVector [out] ROAD_CODE,ROAD_SEQ�i�[�p�x�N�^�[
	 * @return bool
	 */	
	bool GetSmartICPoint(vector<FacilInfo>& r_eFacilInfoVector);
	/**
	 * @brief HIGHWAY_NODE�̃W�I���g���擾
	 * @param r_eFacilInfoVector	[in]  ROAD_CODE,ROAD_NODE���i�[���ꂽ�x�N�^�[
	 * @param r_GeomVector			[out] �W�I���g���i�[�p�x�N�^�[
	 * @return bool
	 */
	bool GetHwyNodeGeom(const vector<FacilInfo>& r_eFacilInfoVector, vector<atl2::seal<IGeometryPtr>>& r_GeomVector);
	/**
	 * @brief ROAD_NODE �̃I�u�W�F�N�gID�̎擾
	 * @prama r_GeomVector	[in]�@HIGHWAY_NODE�@�W�I���g���i�[�x�N�^�[
	 * @param r_ObjectIDSet	[out] OBJECTID�i�[�p�Z�b�g
	 * @return bool
	 */
	bool GetRoadNodeOID(const vector<atl2::seal<IGeometryPtr> >& r_GeomVector, set<long>& r_ObjectIDSet);
	/**
	 * @brief SDE�̃I�[�v��
	 * @note �ݒ肳��Ă���SDE�ɐڑ����A�Ώۃ��C�����J��
	 * @retval true �擾����
	 * @retval false �擾���s
	 */
	bool OpenSDE(void);
	/**
	 * @brief SDE�N���[�Y
	 */
	void Close();
	/*
	 * @brief �o�̓X�g���[���̐ݒ�
	 */
	bool SetOfstream();
};
