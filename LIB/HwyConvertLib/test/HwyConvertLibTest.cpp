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
 * @file
 * @note HwyConvert�N���X���g�p���āASindyHighwayConvert�ƑS�������o�̓t�@�C���������邩���e�X�g����B
 * @note �{�v���W�F�N�g��MBCS,Unicode�̗��\���ɑΉ��B�ǂ��炩����g�p�ł��鎖���m�F�B
 * @note �e�X�g�v���W�F�N�g����̎��s�ł̓R�}���h�v�����v�g���\������Ȃ����߁A
 *		 std::cout/cerr�ւ̏o�͂����鎖���ł��Ȃ��_�ɒ��ӁB�e�X�g���̂ɂ͓��Ɏx��͂Ȃ��B
 * @todo ���L�̃R�[�h�͈�ʂ�̏o�̓t�@�C�����o�͂��邾���ł���A
 *		 ���̌�A�o�̓t�@�C����R��ׂ��f�[�^�Ɣ�r����Ȃǂ��Č��؂�����K�v������B
 *		 �S�Ă����������ꂽ�ė��p���̂���e�X�g�ɂ���ɂ́A
 *		 �E�e�X�g�̍��i���������߂�i��r�Ώۃt�@�C�����ǂ��Ǘ����邩�A�����ɂȂ�Ηǂ��̂��Ȃǁj
 *		 �E�o�͂����t�@�C���Ɣ�r�Ώۃt�@�C����ǂݍ��ݔ�r����R�[�h��ǉ�����
 *		 �Ȃǂ��s���K�v������A�����͍���̉ۑ�Ƃ���B
 */

#include "stdafx.h"
#include "CppUnitTest.h"
#include <memory>
#include "libHwyConvert.h"
#include <arctl/coinitializer.h>
#include <boost/filesystem.hpp>
#include <TDC/useful_headers/tstring.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HwyConvertLibTest
{	
	using namespace hwy_convert_lib;

	typedef std::shared_ptr< arctl::coinitializer > SPCoInitializer;
	SPCoInitializer gpCoInitializer;
	typedef std::shared_ptr< HwyConvert > SPHwyConvert;
	SPHwyConvert gpHwyConvert;
	uh::tstring outDirPath = _T("..\\..\\test\\out\\");	// �e�X�g���s���̃J�����g�f�B���N�g���iHwyConvertLib\<�\����>�j����̑��΃p�X

	TEST_MODULE_INITIALIZE( CoInitialize )
	{
		 gpCoInitializer = SPCoInitializer( new arctl::coinitializer );
	}

	TEST_CLASS( HwyConvertLibTest )
	{
	public:
		TEST_CLASS_INITIALIZE( Create )
		{
			// �C���X�^���X����
			gpHwyConvert = SPHwyConvert( new HwyConvert(
				"FIX201301@onyx(SDE.DEFAULT)",
				"SINDY2012B@topaz2(SDE.FIX_SINDY2012B_121115_1211m)"
			) );
		}

		/// �u�{�ݖ��̕ϊ�����v�Ŏ��s
		TEST_METHOD( FNC )
		{
			execute( *gpHwyConvert, outDirPath + _T("FNC\\"), true );
		}

		/// �u�{�ݖ��̕ϊ��Ȃ��v�Ŏ��s
		TEST_METHOD( nonFNC )
		{
			execute( *gpHwyConvert, outDirPath + _T("nonFNC\\"), false );
		}
	private:
		/// HRESULT��S_OK�ł��鎖��assert����
		static void assertOK( HRESULT hr )
		{
			Assert::AreEqual( hr, S_OK );
		}

		/// �{�ݖ��̕ϊ��̗L�����w�肵�ĕϊ��Əo�͂��s��
		static void execute( HwyConvert& hwyConvert, const uh::tstring& outDirPath, bool facilNameChange )
		{
			// �f�B���N�g���p�X��ݒ肵�A�f�B���N�g�����Ȃ���΍쐬����
			uh::tstring highwayDirPath = outDirPath + _T("highway\\");
			uh::tstring kensakuDirPath = outDirPath + _T("kensaku\\");
			uh::tstring InfoDirPath = outDirPath + _T("info\\");
			namespace fs = boost::filesystem;
			fs::create_directories( fs::path( highwayDirPath ) );
			fs::create_directories( fs::path( kensakuDirPath ) );
			fs::create_directories( fs::path( InfoDirPath ) );

			uh::tstring fnctPath = _T( "\\\\win\\tdc\\Tools\\SiNDY-u\\road\\SindyHighwayConvert\\FacilNameChangeTable.txt" );

			using namespace in::schema;

			//----- �{�ݖ��̕ϊ����� -----//
			if ( facilNameChange )
			{
				{
					SPSINDY2HIGHWAY pHighway = hwyConvert.highway();
					Assert::IsNotNull( pHighway.get() );
					assertOK( pHighway->Write( uh::toStr( highwayDirPath + _T("highway.txt") ), highway::file_format::highway_smartic_added ) );

					// iterator��size�̃e�X�g
					for ( SINDY2HIGHWAY::Iter it = pHighway->begin(); it != pHighway->end(); ++it )
					{
						const SINDY2HIGHWAY::HighwayText& r = *it;
						long value = r.eRoadSeq;
					}
					std::size_t size = pHighway->size();
				}
				{
					SPSINDY2GS pGS = hwyConvert.gs();
					Assert::IsNotNull( pGS.get() );
					assertOK( pGS->Write( uh::toStr( kensakuDirPath + _T("gs.txt") ), SINDY2GS::gs, SINDY2GS::gs_new ) );
					assertOK( pGS->Write( uh::toStr( kensakuDirPath + _T("multiple_store.txt") ), SINDY2GS::multiple_store,	SINDY2GS::gs_new ) );

					// iterator��size�̃e�X�g
					for ( SINDY2GS::Iter it = pGS->begin(); it != pGS->end(); ++it )
					{
						const SINDY2GS::GS& r = *it;
						long value = r.eRoadCode;
					}
					std::size_t size = pGS->size();
				}
				{
					SPSINDY2ROADCODE pRoadCode = hwyConvert.roadCode();
					Assert::IsNotNull( pRoadCode.get() );
					using namespace road_code_doc;
					assertOK( pRoadCode->Write( uh::toStr( kensakuDirPath + _T("road.code.doc") ), file_type::road_code ) );
					assertOK( pRoadCode->Write( uh::toStr( kensakuDirPath + _T("road.code.doc.highway") ), file_type::road_code_highway ) );
					assertOK( pRoadCode->Write( uh::toStr( kensakuDirPath + _T("ferry.code.doc") ), file_type::ferry_code ) );
					assertOK( pRoadCode->WriteForSirius( uh::toStr( kensakuDirPath + _T("roadtype.txt") ), file_format::roadtype ) );
					assertOK( pRoadCode->WriteForSirius( uh::toStr( kensakuDirPath + _T("roadline.txt") ), file_format::roadline ) );

					// iterator��size�̃e�X�g
					for ( SINDY2ROADCODE::Iter it = pRoadCode->begin(); it != pRoadCode->end(); ++it )
					{
						const RoadCodeDoc& r = *it;
						long value = r.eClass;
					}
					std::size_t size = pRoadCode->size();
				}
				{
					SPSINDY2ROADPOINT pRoadPoint = hwyConvert.roadPoint(
						&uh::toWStr( fnctPath )
					);
					Assert::IsNotNull( pRoadPoint.get() );
					using namespace roadpoint;
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("roadpoint.txt") ), true, file_format::roadpoint_smartic_added, jct_data_type::jct_interflow ) );
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("roadpoint_normal.txt") ), false, file_format::roadpoint_smartic_added, jct_data_type::jct_interflow ) );
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("roadpoint.bunnki.txt") ), true, file_format::roadpoint_smartic_added, jct_data_type::jct_branch ) );
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("fclrep.txt") ), true, file_format::fclrep, jct_data_type::jct_interflow) );
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("fclpoint.txt") ), true, file_format::fclpoint, jct_data_type::jct_interflow) );
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("fclpoint.bunnki.txt") ), true, file_format::fclpoint, jct_data_type::jct_branch) );

					// iterator��size�̃e�X�g
					for ( SINDY2ROADPOINT::Iter it = pRoadPoint->begin( true ); it != pRoadPoint->end( true ); ++it )
					{
						const RoadPoint& r = *it;
						long value = r.ePrefMesh;
					}
					std::size_t size = pRoadPoint->size( true );
				}
				{
					SPSINDY2HWYNODE pHwyNode = hwyConvert.hwyNode();
					Assert::IsNotNull( pHwyNode.get() );
					using namespace hwynode;
					assertOK( pHwyNode->Write( uh::toStr( highwayDirPath + _T("tollguide.txt") ), file_type::tollguide, file_format::hwynode_smartic_added ) );
					assertOK( pHwyNode->Write( uh::toStr( highwayDirPath + _T("hwynode.txt") ), file_type::hwynode, file_format::hwynode_smartic_added ) );
					assertOK( pHwyNode->Write( uh::toStr( highwayDirPath + _T("hwynode_inout.txt") ), file_type::hwynode_inout, file_format::hwynode_smartic_added ) );
					assertOK( pHwyNode->Write( uh::toStr( highwayDirPath + _T("tollnode.txt") ), file_type::tollnode, file_format::hwynode_smartic_added ) );

					// iterator��size�̃e�X�g
					for ( SINDY2HWYNODE::Iter it = pHwyNode->begin(); it != pHwyNode->end(); ++it )
					{
						const HighwayNode& r = *it;
						long value = r.eMesh;
					}
					std::size_t size = pHwyNode->size();
				}
				{
					SPSINDY2HWYSAME pHwySame = hwyConvert.hwySame(
						&uh::toWStr( fnctPath )
					);
					Assert::IsNotNull( pHwySame.get() );
					assertOK( pHwySame->Write( uh::toStr( highwayDirPath + _T("hwysame.txt") ), SINDY2HWYSAME::hwysame_new ) );

					// iterator��size�̃e�X�g
					for ( SINDY2HWYSAME::Iter it = pHwySame->begin( true ); it != pHwySame->end( true ); ++it )
					{
						const SINDY2HWYSAME::HwySame& r = *it;
						long value = r.eRoadCode;
					}
					std::size_t size = pHwySame->size( true );
				}
				{
					SPSINDY2TOLLGATELANE pTollGateLane = hwyConvert.tollGateLane();
					Assert::IsNotNull( pTollGateLane.get() );
					assertOK( pTollGateLane->Write( uh::toStr( InfoDirPath + _T("tollgate_laneinfo.txt" ) ) ) );

					// iterator��size�̃e�X�g
					for ( SINDY2TOLLGATELANE::Iter it = pTollGateLane->begin(); it != pTollGateLane->end(); ++it )
					{
						const TollGateLaneInfo& r = *it;
						long value = r.m_HwyNodeOID;
					}
					std::size_t size = pTollGateLane->size();
				}
			}
			//----- �{�ݖ��̕ϊ��Ȃ� -----//
			// �{�ݖ��̕ϊ�����̏ꍇ�ƈقȂ�t�@�C�������o�́B
			else
			{
				{
					SPSINDY2HWYSAME pHwySame = hwyConvert.hwySame();
					Assert::IsNotNull( pHwySame.get() );
					assertOK( pHwySame->Write( uh::toStr( highwayDirPath + _T("hwysame.txt") ), SINDY2HWYSAME::hwysame_new ) );
				}
				{
					SPSINDY2ROADPOINT pRoadPoint = hwyConvert.roadPoint();
					Assert::IsNotNull( pRoadPoint.get() );
					using namespace roadpoint;
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("roadpoint.txt") ), true, file_format::roadpoint_smartic_added, jct_data_type::jct_interflow ) );
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("roadpoint.bunnki.txt") ), true, file_format::roadpoint_smartic_added, jct_data_type::jct_branch ) );
					assertOK( pRoadPoint->Write( uh::toStr( kensakuDirPath + _T("fclrep.txt") ), true, file_format::fclrep, jct_data_type::jct_interflow) );
				}
			}
		}

	};

}
