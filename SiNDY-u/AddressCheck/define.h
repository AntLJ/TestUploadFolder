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

/**
 * @file	define.h
 * @brief	�e�틤�ʒ�`�l
 * �����ɋ��ʂŎg�p�ł�����̂͒�`���Ă���
 */

#pragma once
#include "Message.h"
#include "Logger.h"


// �ڑ��v���p�e�B�L�[��`
const CString	DB_ADDRESS = _T("db_address");
const CString	DB_MESH	= _T("db_mesh");
const CString	DB_MAP = _T("db_map");

// �ݒ�t�@�C���A�C�e����`
const CString   LOG_NAME = _T("log_name");
const CString   NG_CHAR = _T("ng_char");
const CString   DEFAULT_LOGNAME = _T("\\%s_%d%02d%02d%02d%02d%02d.log");

// �f�t�H���g�̒l��`
const CString DEFAULT_CITYCODE = _T("00000");
const CString DEFAULT_ADDRCODE = _T("000000");
const CString DEFAULT_BLOCKCODE = _T("0");
const CString VILLA = _T("999999");

const long INVALID_VALUE = -1L;
const long ABOLITION_CODE = 2L;

typedef std::list<CString>			LAYERLIST;	//!<  ���C�������X�g
typedef std::map<CString, long>     FIELDSMAP;  //!<  �t�B�[���h�}�b�v

/**
* @brief	�Z���}�X�^
*/
struct AddressMaster
{
	long status;	            //!< �X�e�[�^�X
	CString addressName;        //!< �Z����
	CString addressCode;        //!< 11���Z���R�[�h

	/**
	 * @brief  �R���X�g���N�^
	 */
	AddressMaster():status(INVALID_VALUE){};

	/**
	 * @brief  �R���X�g���N�^
	 * @param	[in] inputStatus       �X�e�[�^�X
	 * @param	[in] inputAddressName  �Z����
	 * @param	[in] inputAddressCode  11���Z���R�[�h
	 */
	AddressMaster(long inputStatus, const CString& inputAddressName, const CString& inputAddressCode):status(inputStatus), addressName(inputAddressName), addressCode(inputAddressCode){}
};

/**
* @brief	�G���[���
*/
struct ErrorIdInfo
{
	long oId;        //!< �I�u�W�F�N�gID
	long errorCode;  //!< �G���[�R�[�h
	/**
	 * @brief  �R���X�g���N�^
	 * @param	[in] objectId        //!< �I�u�W�F�N�gID
	 * @param	[in] inputErrorCode  //!< �G���[�R�[�h
	 */
	ErrorIdInfo(long objectId, long inputErrorCode):oId(objectId), errorCode(inputErrorCode){};

	/**
	* @brief	���Z�q�̃I�[�o���[�h
	* @param	[in] errorInfo	     �G���[���
	* @retval	true:������
	* @retval	false:���܂��͓�����
	*/
	bool operator < (const ErrorIdInfo& errorInfo)const
	{
		if(oId < errorInfo.oId)	return true;
		if(oId > errorInfo.oId)	return false;

		if(errorCode < errorInfo.errorCode)	return true;
		return false;
	}
};

/**
* @brief	���o��
* @param	[in] lpcsz           ���
* @param	[in] ...             ���̑��t�����
*/
inline void printProgress( LPCTSTR lpcsz, ... )
{
	CString strMsg;
	if( lpcsz )
	{
		va_list vaList;
		va_start( vaList, lpcsz );
		strMsg.FormatV( lpcsz, vaList );
	}
	std::cout << CT2A(strMsg);
	RUNLOGGER << strMsg;
}


