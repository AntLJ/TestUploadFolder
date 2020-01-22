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

namespace addr_to_xy{
	enum result {
		Addr,                   //!< �Z��������
		AddrLevel,				//!< �Z�����x��
		PrefCode,				//!< �s���{���R�[�h
		CityCode,				//!< �s�撬���R�[�h
		OazaCode,				//!< �厚�E�ʏ̃R�[�h
		ChomeCode,				//!< ���E���ڃR�[�h
		ChibanCode,				//!< �n�ԃR�[�h
		GouCode,				//!< ���R�[�h
		PrefName,				//!< �s���{����
		CityName,				//!< �s�撬����
		OazaName,				//!< �厚�E�ʏ̖�
		ChomeName,				//!< ���E���ږ�
		ChibanName,				//!< �n��
		GouName,				//!< ��
		Mesh,					//!< ���b�V���R�[�h
		X,                      //!< X
		Y,                      //!< Y
		IPCode,					//!< �h�o�R�[�h
		Precision,              //!< ���x
		ChibanF,                //!< ��\�_�E�n�Ԏ~�܂�t���O

		items                   //!< ���ڐ�
};

}

/**
* @brief XY�t�^�iaddr_to_xy�j���s�N���X
* @note oracle�݂̂��T�|�[�g
        �������ʂ̊e�Q�b�^�[�� BNAnalyzer.exe �̏����ɏ�����
*/
class AddrToXyProcesser
{
public:
	AddrToXyProcesser(void){
		m_result.resize( addr_to_xy::result::items );
	}
	~AddrToXyProcesser(void){}

	/**
	* @brief ������
	* @note  �Z���T�[�o�ڑ��Ɏ��s�����ꍇ�͗�O�𓊂���
	* @param svr [in] �T�[�o���Btsnnames.ora �Œ�`�����ڑ���
	* @param usr [in] addr_to_xy�̎��s�����������[�U��
	*/
	void init( const CString& svr, const CString& usr );

	/**
	* @brief addr_to_xy �����s����
	* @param addr [in] �Z��������
	* @return 20���R�[�h������ꂽ���ۂ�
	*/
	bool execFunc( const CString& addr );

	/**
	* @brief 20���R�[�h���擾����
	* @return 20���R�[�h
	*/
	CString getAddrCode() const;
	
	/**
	* @brief �o�ܓx���擾����
	* @return �o�ܓx
	*/
	WKSPoint getLonLat() const;
	
	/**
	* @brief �Z�����x�����擾����
	* @return �Z�����x��
	*/
	long getAddrLevel() const;
	
	/**
	* @brief �s���|�C���g�t���O���擾����
	* @return �s���|�C���g�t���O
	*/
	bool isPinpoint() const;

private:
	CADOBase m_ado;                  //!< ADO�Ǘ��p
	std::vector<CString> m_result;   //!< XY�t�^����
};

