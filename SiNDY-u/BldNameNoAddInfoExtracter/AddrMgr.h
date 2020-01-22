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
#include <WinLib/ADOBase.h>

/// �Z�����ێ��p�\����
struct AddrInfo
{
	CString addr_code;  //!< �Z���R�[�h�i11���j
	CString ken_kanji;  //!< �s���{��
	CString ken_kana;   //!< �s���{���i���~�j
	CString shi_kanji;  //!< �s�撬��
	CString shi_kana;   //!< �s�撬���i���~�j
	CString oaza_kana;  //!< �厚
	CString oaza_kanji; //!< �厚�i���~�j
	CString aza_kanji;  //!< ��
	CString aza_kana;   //!< ���i���~�j
};

class AddrMgr
{
public:

	AddrMgr(void){}
	virtual ~AddrMgr(void){}

	/**
	* @brief ������
	* @note �ڑ����s�����ꍇ�͗�O�𓊂���
	* @param conStr [in] �ڑ������i[���[�U��@�T�[�o��]�j
	*/
	void init( const CString& conStr );
	
	/**
	* @brief �s�撬�����x���ŏZ����������L���b�V������
	* @note �ڑ����s�����ꍇ�͗�O�𓊂���
	* @param cityCode [in] �Z���R�[�h�i5���j
	*/
	void cacheAddrInCity( const CString& cityCode );
	
	/**
	* @brief 11�����x���̏Z�������擾����
	* @note ��� cacheAddrInCity()����Ă���K�v������
	* @param cityCode [in] �Z���R�[�h�i11���j
	* @return �Ή�����Z�����
	*/
	AddrInfo& getAddrInfo( const CString& addrCode ); 

private:
	CADOBase m_ado;                      //!< �Z���T�[�o�p
	std::map<CString, AddrInfo> m_addrs; //!< �Z�����L���b�V���p
};

