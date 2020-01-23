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
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

public:
	uh::tstring m_PointConnect; //!< �|�C���g�̐ڑ��於
	uh::tstring m_PolyConnect;  //!< �|���S���̐ڑ��於
	uh::tstring m_Point;        //!< �|�C���g�̃t�B�[�`����
	uh::tstring m_Poly;         //!< �|���S���̃t�B�[�`����
	uh::tstring m_PointWhere;   //!< �|�C���g��WHERE��
	uh::tstring m_PolyWhere;    //!< �|���S����WHERE��
	uh::tstring m_RunLog;       //!< ���s���O
	uh::tstring m_ErrLog;       //!< �G���[���O
};
