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

#include "AddrToXyProcesser.h"
#include <WinLib/ADOBase.h>
#include <TDC/sindylib_base/TableFinder.h>

namespace sindy{
	class CRowContainer;
}

/**
* @brief �Z����͎��s�p�N���X
* @note ���t���[�ɂ�����ABNAnalyzer.exe �̏����𓥏P����
*       trunk\SiNDY-u\BNAnalyzer
*       �������ȏ��������݂��邪�A���X�̏����ɏ�������̂Ƃ���
*/
class AddrAnalyzer
{
public:
	AddrAnalyzer(void){}
	virtual ~AddrAnalyzer(void){}

	/**
	* @brief ������
	* @note �Z��DB�Ɋe���[�U�Őڑ�����B���s�����ꍇ�͗�O�𓊂���B
	* @param addrSvr [in] �Z���T�[�o�ڑ�������
	* @param xyUsr [in] XY�t�^���[�U�� 
	* @param workUsr [in] �����[�X��ƃ��[�U��
	* @param buildingDB [in] �ƌ`DB�̐ڑ�����
	*/
	void init( const CString& addrSvr, const CString& xyUsr, const CString& workUsr, const CString& buildingDB );

	/**
	* @brief �Z����͌��ʂ�Row�ɔ��f����
	* @param row [in] BUILDINGNAME_SRC_POINT �� Row
	* @return �Z����͂������������ۂ�
	*/
	bool updateRow( sindy::CRowContainer& row );

private:
	/**
	* @brief 20���R�[�h�ɑΉ�����Z���|�C���g�̐����擾����
	* @note �O�t���[�� count_point_per_addr.sql ���s�ɑ���
	* @param addrCode [in] �Ώۂ�20���R�[�h
	* @return addrCode �ɑΉ�����Z���|�C���g�i���|�C���g�Ȃǁj�̐�
	*/
	long countSameAddr( const CString& addrCode );
	
	/**
	* @brief ����̉ƌ`�ɏ���Ă���Z���|�C���g�̐����擾����
	* @note �O�t���[�� extract_build_id_with_plural_points.sql ���s�ɑ���
	* @param bldgSrcP [in] �ƌ`����������|�C���g
	* @return bldgSrcP ������Ă���ƌ`�Ɠ����ƌn��ɂ���Z���|�C���g�i���|�C���g�Ȃǁj�̐�
	*/
	long countBuildingAddrs( IPointPtr bldgSrcP );

	/**
	* @brief �J�E���g�p��SQL�œ����錋�ʂ��擾����
	* @param query [in] SQL�̃N�G��
	* @return SQL��COUNT����
	*/
	long getCountResult( const CString& query );

	CString m_xyUser;              //!< XY�t�^�Ŏg�p���郆�[�U��
	AddrToXyProcesser m_addr2XY;   //!< addr_to_xy���s�p
	CADOBase m_adoWorkUser;        //!< �Z���T�[�o�i�����[�X��Ɨp���[�U�j����p
	sindy::CTableFinder m_finder;  //!< �ƌ`DB�̃t�@�C���_
};

