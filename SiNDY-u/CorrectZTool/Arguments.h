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
	Arguments(void):m_slope_max( -1.0 ){}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~Arguments(void){}
	
	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse( int argc, _TCHAR* argv[] );

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool checkParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool checkParam( const char* const optName, double optValue );

public:
	uh::tstring  m_db;				//!< �����Ώ�DB
	uh::tstring  m_mesh_db;			//!< ���b�V���Q�Ɛ�DB
	uh::tstring  m_mesh_list;		//!< ���b�V�����X�g
	uh::tstring  m_run_log;			//!< ���s���O
	uh::tstring  m_err_log;			//!< �G���[���O
	double       m_slope_max;		//!< ���z�̍ő勖�e�l�i%�j
};
