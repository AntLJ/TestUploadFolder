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
#include "Arguments.h"

typedef std::set<long> ID_SET;	///< ID�p�̃Z�b�g�i��Ƀ��j�[�N�����Ɏg�p�j

/**
 * @brief �����X�V�����̊��N���X
 */
class CSDEBase
{
public:
	/**
	 * @brief �R���X�g���N�^�B
	 */
	CSDEBase(void);

	/**
	 * @brief �f�X�g���N�^�B
	 */
	~CSDEBase(void);

	/**
	* @brief �S���b�V�����̏����i�e�����̃��C�������j�B���f�ɑΉ��B
	* @param cMidSave [in] ���ԕۑ��܂ł̏������b�V�����B-1�͕ۑ��Ȃ��i�I�[�o�[�t���[���邭�炢�񂹂΂��ۑ�����邯�ǁj
	* @retval true ��������
	* @retval false �������s
	*/
	bool executeALL(int cMidSave = -1);

	/**
	 * @brief �e�평�����i�N���X���Ɉ�����ł������́j�B
	 * @param args [in] ���s���I�v�V����
	 * @param ipWorkspace [in] �C���|�[�g�惏�[�N�X�y�[�X
	 * @param ipSpRef [in] �C���|�[�g���ԎQ��
	 * @param ipFeatureClasses [in] �����Ώۃt�B�[�`���N���X�Q
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool init( const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );

protected:
	CString m_ExecInfo;                //!< �������e�i��ԏo�͗p�j
	double m_MaxX;                     //!< ���b�V�����E�i�œ��o�x�j
	double m_MaxY;                     //!< ���b�V�����E�i�Ŗk�ܓx�j
	double m_MinX;                     //!< ���b�V�����E�i�Ő��o�x�j
	double m_MinY;                     //!< ���b�V�����E�i�ē�ܓx�j
	IFeatureClassPtr m_ipFeatureClass; //!< ���ݏ������̃t�B�[�`���N���X
	bool m_testMode;                   //!< �e�X�g���[�h
	mutable CLogFile m_logFile;        //!< ���O�Ǘ��N���X
	IGeometryPtr m_ipMeshGeom;         //!< �������̃��b�V���W�I���g��
	ISpatialReferencePtr m_ipSpRef;    //!< �C���|�[�g���ԎQ��
	IWorkspaceEditPtr m_ipWorkspaceEdit; //!< �C���|�[�g�惏�[�N�X�y�[�X
	FCArray m_ipFeatureClasses;          //!< �����Ώۃt�B�[�`���N���X�Q
	bool m_sindyCImport;                 //!< �C���|�[�g�悪SiNDY-c�X�L�[�}���ǂ���

	/**
	 * @brief �ҏW�J�n�֐��B
	 *
	 * @param cMsg	[in]	���O�o�͗p������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnStartEdit(LPCTSTR cMsg) const;

	/**
	 * @brief �ҏW�I���E�ۑ��֐��B
	 *
	 * @param cMsg	[in]	���O�o�͗p������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnStopEdit(LPCTSTR cMsg) const;

	/**
	 * @brief ���ԕۑ��֐��B
	 *
	 * @param cMsg	[in]	���O�o�͗p������
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnMidSave(LPCTSTR cMsg) const;

	/**
	 * @brief �����J�n�̃��b�V���擾�i���f�����炻�̎�����j�֐��B
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	MESH_MAP::iterator fnGetFirstMesh(void) const;

	/**
	 * @brief ���b�V�����̊e�폈���֐��i�N���X���̋@�\�̈Ⴂ�̂قƂ�ǂ͂����j�B
	 *
	 * @param cCommand	[in]	���b�V�����̃R�}���h�i�ǉ��E�X�V�E�폜�FImport�ł����g��Ȃ��j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	virtual bool fnExecMesh(COMMAND cCommand) = 0;

	/**
	 * @brief ���b�V���|���S�����̓��b�V�����E���C���擾�֐��B
	 *�@�@�@�@���E��̌����Ȕ�r�����邽�߂̍ő�ŏ��l���擾
	 *
	 * @param cGetBoundary	[in]	true:���E false�F�|���S��
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnGetMesh(bool cGetBoundary);

	/**
	 * @brief COM�̃G���[�o�͊֐��B
	 *
	 */
	void fnPrintComError(void) const;

	/**
	 * @brief ���ʑ��������̃R�s�[and/or�Z�b�g�֐��B
	 *
	 * @param ipFeature		[out]	�R�s�[�E�Z�b�g�Ώۃt�B�[�`��
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param ipSrcFeature	[in]	�R�s�[���t�B�[�`���iNULL�̎��͏����l�i�Œ�l�j���Z�b�g�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnSetCommonAttr(IFeaturePtr ipFeature, long cIndex, const IFeaturePtr& ipSrcFeature) const;
	
	/**
	 * @brief �ʂ̃t�B�[�`������w�肵���������Z�b�g����
	 *
	 * @param ipFeature    [out] �R�s�[�E�Z�b�g�Ώۃt�B�[�`��
	 * @param ipSrcFeature [in]  �R�s�[���t�B�[�`��
	 * @param index        [in]  �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param fieldName    [in]  �t�B�[���h��
	 *
	 * @retval true  ����
	 * @retval false ���s�A�G���[
	 */
	bool fnSetAttrFromFeature( IFeaturePtr ipFeature, const IFeaturePtr& ipSrcFeature, long index, const CString& fieldName ) const;

	/**
	 * @brief ���O�n���h���i�W���^�C�v�j�p�̃��b�Z�[�W�o�͊֐��B
	 *
	 * @param cType		[in]	���b�Z�[�W�^�C�v�i�G���[���x���j
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cObjID	[in]	�I�u�W�F�N�gID
	 * @param cMsg		[in]	�i�G���[�j���b�Z�[�W
	 *
	 */
	void fnPrintLogStyle(ErrorCode cType, int cIndex, int cObjID, LPCTSTR cMsg) const;

	/**
	 * @brief ��Ԍ����֐��B
	 *
	 * @param ipGeom		[in]	�������̃W�I���g��
	 * @param SpatialRel	[in]	��Ԍ����̃^�C�v�i�����j
	 * @param subFields		[in]	���������iSubFields�j
	 *
	 * @retval NULL�ȊO	�������ʂ̃t�B�[�`���J�[�\��
	 * @retval NULL		���s�A�G���[
	 */
	IFeatureCursorPtr fnSelectByShape(const IGeometryPtr& ipGeom, esriSpatialRelEnum SpatialRel = esriSpatialRelIntersects, const CString& subFields = _T("")) const;
	
	// ���R�[�h���擾
	long fnGetSelectCountByShape( ID_SET& cIDList, const IGeometryPtr& ipGeom, esriSpatialRelEnum SpatialRel = esriSpatialRelIntersects );
};
