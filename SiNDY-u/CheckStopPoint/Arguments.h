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
	Arguments(){
		m_extract = false;
		m_check   = false;
	};

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

public:
	uh::tstring	m_db_road;      //!< ���HDB�ڑ��p(�`�F�b�N���[�h�ł͌o�N�ω���̓��H�����N�Ƃ��Ďg�p)
	uh::tstring m_db_road_past; //!< �o�N�ω��O�̓��HDB�ڑ��p
	uh::tstring m_db_stop;      //!< �ꎞ��~DB�ڑ��p
	uh::tstring m_db_mesh;      //!< ���b�V��DB�ڑ��p
	uh::tstring m_output;       //!< ���O�o�͗p
	uh::tstring m_meshlist;     //!< ���b�V�����X�g�p
	bool        m_extract;      //!< ���o���[�h
	bool        m_check;        //!< �`�F�b�N���[�h
};
