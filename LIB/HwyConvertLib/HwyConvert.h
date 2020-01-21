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
#include "type.h"
#include <boost/noncopyable.hpp>

namespace hwy_convert_lib
{
	// TODO: SindyHighwayConvert�ȊO����g�p�����ꍇ�Astd::cout,cerr�ւ̏o�͂��ז��Ȃ̂ŁA�o�͂��Ȃ��悤�ɂ���B

	using namespace in::schema;

	/**
	 * @brief HWY�ϊ��N���X
	 * @note	�{���C�u�����̒��S�ƂȂ�N���X�B
	 *			�{���C�u������HWY�ϊ��c�[���iSindyHighwayConvert.exe�j�̃v���W�F�N�g�̈ꕔ�����C�u�������������̂ŁA
	 *			��{�I�ɂ͂��̍\���t�@�C�������̂܂܎g�p���Ă��邪�A
	 *			SindyHighwayConvert�ȊO�̃v���W�F�N�g����ȒP�Ɏg�p�ł���悤�ȃC���^�[�t�F�C�X��񋟂���K�v������A
	 *			���̖�ڂ�S���̂����̃N���X�ł���B
	 */
	class HwyConvert : boost::noncopyable
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 * @param[in]	roadDBStr		���HDB�ڑ���񕶎���
		 * @param[in]	addrDBStr		�Z��DB�ڑ���񕶎���
		 * @note �u�ڑ���񕶎���v�́Asindy::create_workspace_name()���T�|�[�g���Ă��镶����B
		 * @see	http://orias.mr.ipc.pioneer.co.jp/pukiwiki/index.php?sindy_workspace
		 */
		HwyConvert(
			const std::string& roadDBStr,
			const std::string& addrDBStr
		);

		//----- �f�[�^�擾�֐��Q -----
		// shared_ptr< const SINDY2HIGHWAY >�Ȃǂ�Ԃ����Ǝv�������A
		// SINDY2HIGHWAY�Ȃǂ̃����o�֐����S�Ĕ�const�Ȃ̂ŁA��������Ǝg���Ȃ��Ȃ��Ă��܂��B
		// �d�����Ȃ��̂Ŕ�const�̂܂ܕԂ��B�g�p���͒��ӂ��ꂽ���B
			/**
			 * @brief highway.txt�n�f�[�^���쐬���ĕԂ�
			 * @return	�쐬�����f�[�^�i���s�����ꍇ��NULL�j
			 */
			SPSINDY2HIGHWAY			highway();

			/**
			 * @brief gs.txt�n�f�[�^���쐬���ĕԂ�
			 * @return	�쐬�����f�[�^�i���s�����ꍇ��NULL�j
			 */
			SPSINDY2GS				gs();

			/**
			 * @brief road.code.doc�n�f�[�^���쐬���ĕԂ�
			 * @return	�쐬�����f�[�^�i���s�����ꍇ��NULL�j
			 */
			SPSINDY2ROADCODE		roadCode();

			/**
			 * @brief roadpoint.txt�n�f�[�^���쐬���ĕԂ�
			 * @param[in]	pFNCTPath	�{�ݖ��̕ϊ��e�[�u���iFNCT=FacilNameChangeTable�j�t�@�C���̃p�X�i�w�肳��Ȃ���Εϊ����Ȃ��j
			 * @return	�쐬�����f�[�^�i���s�����ꍇ��NULL�j
			 * @note	pFNCTPath�̌^��uh::tstring�ł͂Ȃ�wstring�Ȃ̂́AMBCS/Unicode�����̍\������g�p�ł���悤�ɂ��邽�߁B
			 */
			SPSINDY2ROADPOINT		roadPoint(
				const std::wstring* pFNCTPath = NULL
			);

			/**
			 * @brief hwynode.txt�n�f�[�^���쐬���ĕԂ�
			 * @param[in]	format	�t�H�[�}�b�g
			 * @note	2013/4/2���݁AETC�����ʗ����Ή��t�H�[�}�b�g�͖��̗p�Ȃ̂ŁAformat�̃f�t�H���g�͔�Ή��ɂ��Ă����B
			 * @return	�쐬�����f�[�^�i���s�����ꍇ��NULL�j
			 */
			SPSINDY2HWYNODE			hwyNode(
				 hwynode::file_format::EFormat format = hwynode::file_format::hwynode_smartic_added
			);

			/**
			 * @brief hwysame.txt�n�f�[�^���쐬���ĕԂ�
			 * @param[in]	pFNCTPath	�{�ݖ��̕ϊ��e�[�u���iFNCT=FacilNameChangeTable�j�t�@�C���̃p�X�i�w�肳��Ȃ���Εϊ����Ȃ��j
			 * @return	�쐬�����f�[�^�i���s�����ꍇ��NULL�j
			 */
			SPSINDY2HWYSAME			hwySame(
				const std::wstring* pFNCTPath = NULL
			);

			/**
			 * @brief tollgate_lane.txt�n�f�[�^���쐬���ĕԂ�
			 * @return	�쐬�����f�[�^�i���s�����ꍇ��NULL�j
			 */
			SPSINDY2TOLLGATELANE	tollGateLane();

	private:
		SPSINDY2INHWYINFO		m_pHwyInfo;				//!< HWY���
		SPSindyDBConnection		m_pRoadDBConnection;	//!< ���HDB�ڑ����
		SPSindyDBConnection		m_pAddrDBConnection;	//!< �Z��DB�ڑ����

		SPSINDY2HIGHWAY			m_pHighway;				//!< highway.txt�n�f�[�^
		SPSINDY2GS				m_pGS;					//!< gs.txt�n�f�[�^
		SPSINDY2ROADCODE		m_pRoadCode;			//!< road.code.doc�n�f�[�^
		SPSINDY2ROADPOINT		m_pRoadPoint;			//!< roadpoint.txt�n�f�[�^�i�{�ݖ��̕ύX�Ȃ��j
		SPSINDY2ROADPOINT		m_pRoadPointFNC;		//!< roadpoint.txt�n�f�[�^�i�{�ݖ��̕ύX����j
		SPSINDY2HWYNODE			m_pHwyNode;				//!< hwynode.txt�n�f�[�^
		SPSINDY2HWYSAME			m_pHwySame;				//!< hwysame.txt�n�f�[�^�i�{�ݖ��̕ύX�Ȃ��j
		SPSINDY2HWYSAME			m_pHwySameFNC;			//!< hwysame.txt�n�f�[�^�i�{�ݖ��̕ύX����j
		SPSINDY2TOLLGATELANE	m_pTollGateLane;		//!< tollgate_lane.txt�n�f�[�^
	};

} // namespace hwy_convert_lib
