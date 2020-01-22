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

#include <map>
#include <string>

/**
 * @class	CTmpCodeCreator
 * @brief	�����t���Z���p�Ɉꎞ�I�ɃR�[�h����������N���X
 * @note	�����[�X�f�[�^�̏Z���R�[�h�Ƃ͑S�����֌W�B
 *			�����܂ł��̃`�F�b�N���ł̂ݗL���ƂȂ�
 */
class CTmpCodeCreator
{
public:
	CTmpCodeCreator(void){}
	~CTmpCodeCreator(void){}

	/**
	 * @brief	���R�[�h�擾
	 * @param	strAddrCode	[in]	11���Z���R�[�h
	 * @param	strPntName	[in]	�����n�Ԗ�
	 * @return	���R�[�h
	 */
	std::string GetTmpCode( const std::string& strAddrCode, const std::string& strPntName );

private:

	/**
	 * @brief	�V�R�[�h�\��
	 * @param	strAddrCode	[in]	11���Z���R�[�h
	 * @param	strPntName	[in]	�����n�Ԗ�
	 * @return	�V���R�[�h
	 */
	std::string insertNewTmpCode( const std::string& strAddrCode, const std::string& strPntName );

	typedef std::map< std::string, std::string >	STRINGMAP;
	typedef std::map< std::string, STRINGMAP >		TMPCODEMAP;
	TMPCODEMAP	m_TmpCodeMap;	//!< ���R�[�h�}�b�v	
};
