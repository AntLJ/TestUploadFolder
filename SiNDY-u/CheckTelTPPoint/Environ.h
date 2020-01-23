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

#ifndef __ENVIRON_H__
#define __ENVIRON_H__

/**
 * @class	CEnviron
 * @brief	���ϐ��p�����[�^
 */
class CEnviron
{
public:
	CString	m_strTPDBProp;			/// �Z���nSDE�ڑ��v���p�e�B(TelTP�|�C���g�p)
	CString	m_strAddrDBProp;		/// �Z���nSDE�ڑ��v���p�e�B(�s���E�p)
	CString	m_strCityDBProp;		/// �n�}�nSDE�ڑ��v���p�e�B(�s�s�n�})
	CString	m_strBaseDBProp;		/// �n�}�nSDE�ڑ��v���p�e�B(���k)
	CString	m_strTelTPFName;		/// �d�b�ԍ������|�C���g�t�B�[�`���N���X��
	CString	m_strCityAdminFName;	/// �s�s�n�}�s���E�t�B�[�`���N���X��
	CString	m_strCitySiteFName;		/// �s�s�n�}�w�i�t�B�[�`���N���X��	
	CString	m_strBaseSiteFName;		/// ���k�w�i�t�B�[�`���N���X��
	CString m_strPOIDBProp;			/// POI�nSDE�ڑ��v���p�e�B(Bug 5701�Ή� 07/11/30�ǉ�)
	CString m_strPOIFName;			/// POI�t�B�[�`���N���X��(Bug 5701�Ή� 07/11/30�ǉ�)
};

#endif
