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
#include "stdafx.h"
#include "Arguments.h"

#define FORTIME	0
#define FORVER	1

/**
 * @brief	���ݎ����𕶎���Ŏ擾
 * @return	������(yyyy/mm/dd hh/mm/ss)
 */
inline CString	GetCurDateStr( int mode = FORTIME )
{
	SYSTEMTIME	st;
	::GetLocalTime( &st );

	CString strTime;

	// ���ԗp�ɂ�yyyy/mm/dd hh:mm:ss
	if( mode == FORTIME )
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	// �o�[�W�����p�ɂ�yyyymmddhhmmss
	else
		strTime.Format( _T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

	return strTime;
}

/**
 * @class DeleteRows
 */
class DeleteRows
{
public:

	/**
	 * @brief	�R���X�g���N�^
	 */
	DeleteRows( uh::tstring	input, uh::tstring file, uh::tstring fcname, uh::tstring log ){
		strInput  = input.c_str();	
		strFile   = file.c_str();
		strFcname = fcname.c_str();
		strLog    = log.c_str();
	};

	~DeleteRows(){};

	/**
	 * @brief	������
	 * @param	cArgc	[in]	�����̐�
	 * @param	cArgv	[in]	����
	 * @return	bool
	 */
	bool Init();

	/**
	 * @brief	���s
	 */
	bool Execute();

private:

	/**
	 * @brief	���O�Ƀw�b�_�[���o��
	 */
	void fnPrintHeader();

	/**
	 * @brief	�ҏW�J�n
	 * @return	bool
	 */
	bool fnEditStart();

	/**
	 * @brief	�ҏW�I��
	 * @return	bool
	 */
	bool fnEditStop();

	/**
	 * @brief	�폜����
	 * @return	bool
	 */
	void fnDelete();



private:

	CString	strInput;	
	CString strFile;
	CString strFcname;
	CString strLog;

	IWorkspaceEditPtr m_ipWorkspace;

	ITablePtr         m_ipTable;

	std::ofstream     m_ofs;					// ���O�o�̓X�g���[��

	std::list<long>   m_lOIDList;				// OID���X�g��
};
