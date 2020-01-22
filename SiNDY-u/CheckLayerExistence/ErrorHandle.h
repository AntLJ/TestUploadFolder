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

#if !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
#define AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * �G���[�R�[�h��`
 */
#define E_PGDB_NO_LAYER			1	// PGDB�t�@�C�����Ƀ��C��������܂���

/**
 * @class	CErrorHandle
 */
class CErrorHandle
{
public:
	
	CErrorHandle()
	{}
	
	virtual ~CErrorHandle()
	{}

	/**
	 * @brief	�t�B�[�`���N���X���Z�b�g
	 * @param	lpszFCName	[in]	�t�B�[�`���N���X��
	 */
	void setFCName( LPCTSTR lpszFCName )
	{ m_strFCName = lpszFCName; }

	bool open( LPCTSTR lpcszFile )
	{
		m_fout.open( static_cast<CT2CW>(lpcszFile), std::ios_base::out );
		return ( m_fout? true : false );
	}

	/**
	 * @brief	�w�b�_�[���
	 * @param	lpszToolVer	[in]	�c�[���o�[�W����
	 */
	void printHeader( LPCTSTR lpszVer, CString FileProp )
	{
		m_fout << "#" << lpszVer << std::endl;
		m_fout << "#" << FileProp << std::endl;
		m_fout << std::endl;
	}

	void printLog(ATL::CString FileName, std::string LayerName);
	void printLog(ATL::CString FileName, int MeshSuccessFlag);

	void printPGDBOpenError(ATL::CString FileName, ATL::CString ErrorMsg);

private:

	std::ofstream	m_fout;					/// ���O�t�@�C���o�̓X�g���[

	CString			m_strFCName;			/// �t�B�[�`���N���X��

	static const LPCTSTR	m_lpszErrMsg[];	/// �G���[���b�Z�[�W�ꗗ
};




#endif // !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
