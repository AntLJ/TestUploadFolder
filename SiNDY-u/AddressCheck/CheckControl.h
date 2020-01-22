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

#include "LayerCheckInclude.h"

/**
 * @class	CheckControl
 * @brief	����N���X
 */
class CheckControl
{
public:
	typedef vector< shared_ptr<LayerCheckBase> > CHECKLIST;    //!< �`�F�b�N�N���X���X�g

	/**
	 * @brief  �R���X�g���N�^
	 */
	CheckControl(void){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CheckControl(void){}

	/**
	* @brief	������
	* @param	[in] argument	        	�R�}���h���C������
	* @retval   true:����  false:���s
	*/
	bool init( Argument& argument );

	/**
	* @brief	�`�F�b�N
	* @retval   true:����  false:���s
	*/
	bool doCheck();

	/**
	* @brief	�`�F�b�N
	* @param	[in]  meshCodeList           ���b�V���R�[�h���X�g
	* @param	[in]  meshFeatureClassName   ���b�V���t�B�`���N���[�X��
	* @param	[in]  currentCheckLayerList  �Ή�����`�G�b�N���X�g
	* @retval   true:����  false:���s
	*/
	bool check( const MeshCodeList& meshCodeList, const CString& meshFeatureClassName, CHECKLIST& currentCheckLayerList );

private:
	/**
	 * @brief	���b�V���؂�ւ����̏�����
	 * @param	[in]  ulNextMesh    ���̃��b�V���R�[�h
	 * @param	[in] meshFeaturClassName ���b�V���t�B�`���N���[�X��
	 * @param	[out] ipMeshGeo		���b�V���W�I���g��
	 * @param	[out] ulMeshID		���b�V���I�u�W�F�N�gID
	 * @note	���b�V���؂�ւ����ɍs������
	 * @retval	true:����  false:���s
	 */
	bool initMesh( unsigned long ulNextMesh, const CString& meshFeaturClassName, IGeometryPtr& ipMeshGeo, long &meshID );

	/**
	 * @brief	���b�V���t�B�[�`���擾
	 * @param	[in] ulMeshCode		���b�V���R�[�h
	 * @param	[in] meshFeaturClassName ���b�V���t�B�`���N���[�X��
	 * @return	���b�V���t�B�[�`��
	 */
	IFeaturePtr getMeshFeature( unsigned long ulMeshCode, const CString&meshFeaturClassName );

	/**
	 * @brief	�`�F�b�N���C���C���X�^���X�Z�b�g
	 * @param	[in] layer		���C����
	 * @retval	true:����  false:���s
	 */
	bool setLayerInstance( const CString& layer );

private:
	CHECKLIST                      m_cityListLayerCheckInstance;   //!< �s�s�n�}�`�F�b�N���C�����X�g
	CHECKLIST                      m_baseListLayerCheckInstance;   //!< �x�[�X���b�V���`�F�b�N���C�����X�g

	MeshCodeList                   m_cityMeshCodeList;         //!< �s�s���b�V���R�[�h���X�g
	MeshCodeList                   m_baseMeshCodeList;         //!< �x�[�X���b�V���R�[�h���X�g
	TableFinder                    m_tableFinder;              //!< �e�[�u���Ǘ�
	SettingFile                    m_settingFile;              //!< �ݒ�t�@�C��
	map< CString, shared_ptr<ErrLogger> >     m_errorLogList;  //!< �G���[���O���X�g
	CString                        m_logDir;                   //!< ���O�t�H���_
	CString                        m_textAddressMasterPath;    //!< �}�X�^�t�H���_
};
