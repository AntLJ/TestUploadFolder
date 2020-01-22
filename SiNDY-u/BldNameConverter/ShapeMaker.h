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
#include "LogSys.h"
#include "common.h"

/**
 * Shape�t�@�C���쐬�N���X
 */
class ShapeMaker
{
public:
	ShapeMaker(const CString& outRootDir, bool isTest, const IGeometryDefPtr ipGeometryDef);
	~ShapeMaker(void);

	/**
	 * @brief  Shape�ϊ�
	 * @param  cnvInfo [in] �o�͏��
	 * @param  cityMesh [in] �����Ώۃ��b�V��
	 * @retval true ����I��
	 * @retval false �ُ�I��
	 */
	bool makeShape(const IDRecordMap& cnvInfo, long cityMesh);

private:
	/**
	 * @brief  �o�̓f�B���N�g���쐬
	 * @param  cityMesh [in] �s�s�n�}���b�V��
	 * @param  shpName [in] Shape�t�@�C����
	 * @param  outputDir [out] �o�̓f�B���N�g��
	 * @retval true ����I��
	 * @retval false �ُ�I��
	 */
	bool makeDirAndDelFile(long cityMesh, LPCTSTR shpName, CString* outputDir);

	/**
	 * @brief  �o��dbf�쐬
	 * @param  dbfName [in] dbf�t�@�C����
	 * @param  ipWorkspace [in] ���[�N�X�y�[�X
	 * @param  ipTable [out] �e�[�u��
	 * @retval ture ����	
	 * @retval false ���s
	 */
	bool makeVacuityDbf(LPCTSTR dbfName, const IWorkspacePtr ipWorkspace, ITablePtr& ipTable);

	/**
	 * @brief  �o��Shape�쐬
	 * @param  shpName [in] shape�t�@�C����
	 * @param  ipWorkspace [in] ���[�N�X�y�[�X
	 * @param  ipFeatureClass [in] �t�B�[�`���N���X
	 * @retval true ����
	 * @retval false ���s
	 *
	 */
	bool makeVacuityShape(LPCTSTR shpName, const IWorkspacePtr ipWorkspace, IFeatureClassPtr& ipFeatureClass);

	/**
	 * @brief  �����t�B�[���h�Q�쐬�֐�
	 * @parma  fileName [in] shape(dbf)�t�@�C����
	 * @param  ipFields [in] IFieldsEditPtr
	 * @retval true ����
	 * @retval false ���s
	 */
	bool makeAttrFields(LPCTSTR fileName, IFieldsEditPtr& ipFileds);

	/**
	 * @brief  �`��t�B�[���h�쐬�֐�
	 * @param  shpFileName [in] �쐬Shape�t�@�C����
	 * @param  fieldName [in] �t�B�[���h��
	 * @param  fieldType [in] �t�B�[���h�^�C�v
	 * @param  ipGeoField [out] �쐬�����t�B�[���h�I�u�W�F�N�g
	 * @retval true ����
	 * @retval false ���s
	 */
	bool makeShapeField(LPCTSTR shpFileName, LPCTSTR fieldName, esriFieldType fieldType, IFieldPtr &ipGeoField);

	/**
	 * @brief  �����t�B�[���h�쐬�֐�
	 * @param  shpName [in] �쐬Shape��
	 * @param  fieldName [in] �t�B�[���h��
	 * @param  fieldType [in] �t�B�[���h�^�C�v
	 * @param  editalbe [in] �ҏW��
	 * @param  nullable [in] NULL����
	 * @param  fieldLength [in] �t�B�[���h��
	 * @param  ipField [out] �쐬�t�B�[���h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool makeAttrField(LPCTSTR shpName, LPCTSTR fieldName, esriFieldType fieldType, VARIANT_BOOL editable, VARIANT_BOOL nullable, long fieldLength, IFieldPtr &ipField);

	/**
	 * @brief  ���R�[�h�쐬�֐�
	 * @param  dbfName [in] dbf��
	 * @param  cnvInfo [in] �쐬���R�[�h���
	 * @param  ipTable [out] �e�[�u��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool importRecord(LPCTSTR dbfName, const IDRecordMap& cnvInfo, ITablePtr& ipTable);

	/**
	 * @brief  ���R�[�h�쐬�֐�
	 * @param  shpName [in] Shape��
	 * @param  cnvInfo [in] �쐬���R�[�h���
	 * @param  ipFeatureClass [in] �t�B�[�`���N���X
	 * @retval true ����
	 * @retval false ���s
	 */
 	bool importRecord(LPCTSTR shpName, const IDRecordMap& cnvInfo, IFeatureClassPtr& ipFeatureClass);

private:

	CString m_outRootDir;	//!< �o�͐�f�B���N�g����
	bool m_isTest;			//!< �e�X�g���[�h
	const IGeometryDefPtr m_ipGeometryDef;	//!< �o�͂���Shape�t�@�C���̃W�I���g����`
};
