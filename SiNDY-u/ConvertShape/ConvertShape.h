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

#include "global.h"
#include "TransParam.h"
#include "FieldAttr.h"

class CConvertShape {
public:
	CConvertShape(){}
	~CConvertShape(){}

public:
	/**
	 * @brief ������
	 * @param cTransParam [in] �R�}���h���C������
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool Init(const CTransParam& cTransParam);

	bool run();

private:
	/**
	 * @brief ���b�V���֘A������
	 * @note  ���b�V���N���X�A���b�V���`��A�o�̓p�X�̐ݒ���s��
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool InitMesh();

	/**
	 * @brief ���b�V���֘A������
	 * @note  InitMesh()����Ăяo�����֐�
	 * @param strMeshlistPath [in] ���b�V�����X�g�ւ̃p�X
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool LoadMeshlist(const CString& strMeshlistPath);

	/**
	 * @brief ���b�V���N���X�̃e�[�u�����擾
	 * @note  ���b�V���R�[�h��4,6,8���Őڑ����郁�b�V���N���X���Ⴄ����
	 * @param lDigit [in] ���b�V���R�[�h�̌���
	 * @param strMeshCode [in] ���b�V���R�[�h
	 * @param strMeshTableName [out] ���b�V���N���X�̃e�[�u����
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool GetMeshTableName(long lDigit, const CString& strMeshCode, CString& strMeshTableName);

	/**
	 * @brief ���b�V���`��擾
	 * @note  ���b�V���R�[�h�̃��b�V���`��擾
	 * @param lMeshCode [in] ���b�V���R�[�h
	 * @param ipMeshGeom [out] ���b�V���`��
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool GetMeshGeom(long lMeshCode, IGeometryPtr& ipMeshGeom);

	/**
	 * @brief SHAPE�o�͐�̃p�X�쐬
	 * @param lMeshCode [in] ���b�V���R�[�h�̌���
	 * @param strMeshCode [in] ���b�V���R�[�h
	 * @param strOutputDir [out] SHAPE�o�͐�̃p�X
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool CreateOutputDir(long lMeshDigit, const CString& strMeshCode, CString& strOutputDir);

private:
	CTransParam m_cTransParam;
	CFieldAttr m_cFieldAttr;

	IWorkspacePtr m_ipTargetWork;
	ITablePtr m_ipTargetTable;
	std::list<PAIR_COL> m_listColName;
	IWorkspacePtr m_ipMeshWork;
	IFeatureClassPtr m_ipMeshClass;  //!< �����ɂ����City,Base,Middle�ɂȂ�
	//MESHLIST m_setMeshlist;
	std::map<long,std::pair<CString,CAdapt<IGeometryPtr>>> m_mapMeshGeom;
	std::map<ORG_COLNAME, CString> m_mapColValue;			// �����w��pmap
	std::map<ORG_COLNAME, TRANS_VAL> m_mapTransValue;		// �ۂߍ��ݗpmap
};
