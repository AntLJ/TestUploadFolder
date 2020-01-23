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


/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments(){}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~Arguments(){}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  opt_name  [in]  �I�v�V������
	 * @param  opt_value [in]  �I�v�V�����l
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam( const char* const opt_name, const uh::tstring& opt_value );

public:
	uh::tstring  m_RoadDB;    //!< ���HDB�ڑ����
	uh::tstring  m_AdamDB;    //!< ADAM DB�ڑ����
	uh::tstring  m_DiffDB;    //!< ���H�����N�܂��͎Ԑ������N�̌`�󍷕�FGDB

	uh::tstring  m_MeshListStr;     //!< ���b�V���R�[�h�A�܂��̓��b�V�����X�g�t�@�C���p�X
	std::vector<long> m_MeshList;   //!< ���b�V�����X�g

	uh::tstring  m_TargetRoadWhere;	//!< ���H�����N��������

	uh::tstring  m_LogDir;    //!< ���O�o�̓t�H���_�p�X
};
