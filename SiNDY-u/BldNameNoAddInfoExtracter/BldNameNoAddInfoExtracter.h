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

#include "Arguments.h"
#include "TsvWriter.h"
#include "AddrMgr.h"
#include <TDC/sindylib_base/TableFinder.h>

namespace sindy{
	class CRowContainer;
	class CTableContainer;
}

/**
 * @brief   ���C���N���X
 */
class BldNameNoAddInfoExtracter
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	BldNameNoAddInfoExtracter( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~BldNameNoAddInfoExtracter(){};

	/**
	 * @brief   ���s
	 * @note �v���I�ȃG���[���������ꍇ�͗�O�𓊂���
	 */
	void execute();

private:

	/**
	 * @brief   ������
	 * @note ���s�����ꍇ�͗�O�𓊂���
	 */
	void init();
	
	/**
	* @brief �R�t���f�ރ|�C���g���擾����
	* @note bldName ���⊮�pPOI�R���̏ꍇ�́A�_�~�[������ĕԂ�
	* @param bldName [in] �r������
	* @parem poiT [in] POI_POINT_ORG�e�[�u��
	* @return �f�ރ|�C���g
	*/
	sindy::CSPRowContainer getSrcPoint( const sindy::CRowContainer& bldName, sindy::CTableContainer& poiT );
	
	/**
	* @brief POI�R����������ǂ݂��擾����
	* @param bldName [in] �r������
	* @parem poiT [in] POI_POINT_ORG�e�[�u��
	* @return POI�̓ǂ�
	*/
	CString getPoiYomi( const sindy::CRowContainer& bldName, sindy::CTableContainer& poiT );


private:
	Arguments m_args;                    //!< ����
	sindy::CTableFinder m_bldNameFinder; //!< �r�����̃t�@�C���_
	sindy::CTableFinder m_adminFinder;   //!< �s���E�p�t�@�C���_
	sindy::CTableFinder m_poiFinder;     //!< POI_ORG
	TsvWriter m_tsvWriter;               //!< ���ʃ��X�g�o�͗p
	AddrMgr m_addrDb;                    //!< �Z��DB����p
	CString m_poiUser;                   //!< POI�̐ڑ����[�U��
};

