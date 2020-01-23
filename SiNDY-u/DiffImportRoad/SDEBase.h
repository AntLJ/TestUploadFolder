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
#include "Reflect.h"

typedef std::set<long> ID_SET;	///< ID�p�̃Z�b�g�i��Ƀ��j�[�N�����Ɏg�p�j

/**
 * @brief �����X�V�����̊��N���X
 */
class CSDEBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CSDEBase(void);

	/**
	 * @brief �f�X�g���N�^
	 */
	~CSDEBase(void);

	/**
	 * @brief �S���b�V�����̏����i�e�����̃��C�������j�B���f�ɑΉ�
	 */
	bool executeALL(int cMidSave = -1);

	/**
	 * @brief �e�평�����i�N���X���Ɉ�����ł������́j
	 */
	virtual bool init(void);

protected:
	enum REF_LAYER {
		TGT_LAYER = 0,
		FIX_LAYER = 1,
		PGDB_LAYER = 2
	};
	CLayerManage* m_LayerManage;
	CString m_ExecInfo;		///< �������e�i��ԏo�͗p�j
	double m_MaxX;			///< ���b�V�����E�i�œ��o�x�j
	double m_MaxY;			///< ���b�V�����E�i�Ŗk�ܓx�j
	double m_MinX;			///< ���b�V�����E�i�Ő��o�x�j
	double m_MinY;			///< ���b�V�����E�i�ē�ܓx�j
	ITablePtr m_ipTable[2];	///< ���ݏ������̃e�[�u���i�t�B�[�`���N���X�j[1]��LQ�p[0]�͂��̑�
	IPolygonPtr m_ipMeshPoly;	///< ���b�V���|���S��
	IGeometryPtr m_ipMeshBoundary;	///< ���b�V���|���S���i�g�j

	/**
	 * @brief �ҏW�J�n�֐�
	 *
	 * @param cMsg	[in]	���O�o�͗p������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnStartEdit(LPCTSTR cMsg) const;

	/**
	 * @brief �ҏW�I���E�ۑ��֐�
	 *
	 * @param cMsg	[in]	���O�o�͗p������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnStopEdit(LPCTSTR cMsg) const;


	/**
	 * @brief �ҏW�I���E�j���֐�
	 *
	 * @param cMsg	[in]	���O�o�͗p������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnAbortEdit(LPCTSTR cMsg) const;

	/**
	 * @brief ���ԕۑ��֐�
	 *
	 * @param cMsg	[in]	���O�o�͗p������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnMidSave(LPCTSTR cMsg) const;

	/**
	 * @brief �����J�n�̃��b�V���擾�i���f�����炻�̎�����j�֐�
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	MESH_SET::iterator fnGetFirstMesh(void) const;

	/**
	 * @brief ���b�V�����̊e�폈���֐��i�N���X���̋@�\�̈Ⴂ�̂قƂ�ǂ͂����j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	virtual bool fnExecMesh(void) = 0;

	/**
	 * @brief ���b�V���|���S���y�у��b�V�����E���C���擾�֐�
	 *�@�@�@�@���E��̌����Ȕ�r�����邽�߂̍ő�ŏ��l���擾
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnGetMesh(void);

	/**
	 * @brief COM�̃G���[�o�͊֐�
	 *
	 */
	void fnPrintComError(void) const;

	/**
	 * @brief ���ʑ��������̃R�s�[and/or�Z�b�g�֐�
	 *
	 * @param ipFeature			[out]	�R�s�[�E�Z�b�g�Ώۃt�B�[�`���iDB���j
	 * @param cSDEIndexMap		[in]	SDE�̃t�B�[���h���ƃt�B�[���h�C���f�b�N�X�̑Ή��}�b�v
	 * @param cPGDBIndexMap		[in]	PGDB�̃t�B�[���h���ƃt�B�[���h�C���f�b�N�X�̑Ή��}�b�v
	 * @param ipSrcFeature		[in]	�R�s�[���t�B�[�`���iPGDB���j�iNULL�̎��͏����l�i�Œ�l�j���Z�b�g�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnSetCommonAttr(_IRowPtr& ipRow, INDEX_MAP& cSDEIndexMap, INDEX_MAP& cPGDBIndexMap, const _IRowPtr& ipSrcRow) const;

	/**
	 * @brief ���O�n���h���i�W���^�C�v�j�p�̃��b�Z�[�W�o�͊֐�
	 *
	 * @param cType			[in]	���b�Z�[�W�^�C�v�i�G���[���x���j
	 * @param cLayerName	[in]	���C����
	 * @param cObjID		[in]	�I�u�W�F�N�gID
	 * @param cMsg			[in]	�i�G���[�j���b�Z�[�W
	 * @param cRefLayer		[in]	�Ώۃ��C���������^�C�v
	 * @param ipGeom		[in]	���W�o�͗p�W�I���g��
	 *
	 */
	void fnPrintLogStyre(int cType, LPCTSTR cLayerName, int cObjID, LPCTSTR cMsg, LPCTSTR cMsg2, REF_LAYER cRefLayer, IGeometryPtr ipGeom=NULL) const;

	/**
	 * @brief ��Ԍ����֐�
	 *
	 * @param ipGeom		[in]	�������̃W�I���g��
	 * @param SpatialRel	[in]	��Ԍ����̃^�C�v�i�����j
	 *
	 * @retval NULL�ȊO	�������ʂ̃t�B�[�`���J�[�\��
	 * @retval NULL		���s�A�G���[
	 */
	IFeatureCursorPtr fnSelectByShape(const IGeometryPtr& ipGeom, esriSpatialRelEnum SpatialRel = esriSpatialRelIntersects) const;
};
