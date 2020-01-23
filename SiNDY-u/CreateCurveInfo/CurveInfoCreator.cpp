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

#include "StdAfx.h"
#include <VersionInfo.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/timer.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <crd_cnv/crd_cnv.h>
#include "CurveInfoCreator.h"
#include "Notifier.h"
#include "CurveInfoSchema.h"
#include "CurveInfoUtility.h"
#include "CircleFitEvaluator.h"
#include <RowContainer.h>


namespace curve_info
{
	using namespace utility;
	using namespace spatial_reference;

	void CurveInfoCreator::init(
		tstring cRoadDB,
		tstring cMeshDB,
		path cBaseMeshListFilePath,
		path cOutputDirPath,
		path cLogFilePath,
		path cVerificationLogFilePath
	)
	{
		// �o�̓f�B���N�g�����Ȃ���΍쐬
		createDirectory( cOutputDirPath );

		m_OutputDirPath = cOutputDirPath;

		using namespace sindy::schema;

		// ���H�f�[�^�x�[�X�ɐڑ����A�����N�A�m�[�h�A�E���܋֎~�����N��̃t�B�[�`���N���X���擾
		const IWorkspacePtr ipRoadWorkspace( getWorkspace( cRoadDB, notifier::critical::asCannotGetWorkspace ) );
		IFeatureClassPtr ipRoadLinkClass = getFeatureClass( ipRoadWorkspace, road_link::kTableName, notifier::critical::asCannotGetTable );
		IFeatureClassPtr ipRoadNodeClass = getFeatureClass( ipRoadWorkspace, road_node::kTableName, notifier::critical::asCannotGetTable );

		// �Q�����b�V���f�[�^�x�[�X�ɐڑ����A�Q�����b�V���t�B�[�`���N���X���擾
		const IWorkspacePtr ipMeshWorkspace( getWorkspace( cMeshDB, notifier::critical::asCannotGetWorkspace ) );
		IFeatureClassPtr ipBaseMeshClass = getFeatureClass( ipMeshWorkspace, sindy::schema::basemesh::kTableName, notifier::critical::asCannotGetTable );

		// �L���b�V���}�l�[�W���𐶐�
		m_pCacheManager = CacheManagerPtr( new CacheManager( ipRoadLinkClass, ipRoadNodeClass, ipBaseMeshClass ) );

		// �Q�����b�V�����X�g��ǂݍ���
		m_BaseMeshList = loadMeshList( cBaseMeshListFilePath, notifier::critical::asCannotOpenMeshListFile, notifier::critical::asMeshListFileIsInvalid );

		// ���O�t�@�C�����̕ύX
		identifiedFileName( cLogFilePath );
		identifiedFileName( cVerificationLogFilePath );

		// ���O�o�̓X�g���[�����J��
		m_LogStream.open( CString( cLogFilePath.string().c_str() ) );
		if (! m_LogStream)
		{
			notifier::critical::asCannotOpenLogFile( cLogFilePath.string() );
		}
		if (m_OutputOptions.m_OutputVerificationLog)
		{
			m_VerificationLogStream.open( CString( cVerificationLogFilePath.string().c_str() ) );
			if (! m_VerificationLogStream)
			{
				notifier::critical::asCannotOpenLogFile( cVerificationLogFilePath.string() );
			}
		}
		
		// ���O�Ɏ��s���̏����o��
		outputExecInfo( m_LogStream, cRoadDB, cMeshDB );

		// ���ؗp���O�̃w�b�_���o��
		if (m_OutputOptions.m_OutputVerificationLog)
		{
			adlib::outputSindyFreeStyleLogHeader( m_VerificationLogStream, boost::assign::list_of
				("FLAG")("LAYER")("OBJECTID")("LON")("LAT")("ROC")("REPLINKID")("WAYTORP")("�R�_�̊Ԋu")("�p�x")("COMMENT")
			);
		}
	}

	void CurveInfoCreator::outputExecInfo( std::ostream& co, tstring cRoadDB, tstring cMeshDB ) const
	{
		CVersion aCVersion;
		CString aFileVersion = aCVersion.GetFileVersion();
		CString aProductVersion = aCVersion.GetProductVersion();

		TCHAR aModuleFileName[MAX_PATH+1]={0};
		::GetModuleFileName( NULL, aModuleFileName, MAX_PATH );
		path aModuleFilePath = aModuleFileName;

		// ���O�̃w�b�_�o��
		using namespace std;
		outputSindyFreeStyleLogHeader( co, utility::log_format::fieldNames() );

		co << "# �J�n���� : " << boost::lexical_cast<string>( boost::posix_time::second_clock::local_time() ) << endl;
		co << "# " << aModuleFilePath.leaf() << " F:" << CT2A( aFileVersion.GetString() ) << " P:" << CT2A( aProductVersion.GetString() ) << endl;
		co << "# ���H�f�[�^�x�[�X : " << cRoadDB << endl;
		co << "# �Q�����b�V���f�[�^�x�[�X : " << cMeshDB << endl;
		co << "# �Ώۃ��b�V���� : " << m_BaseMeshList.size() << endl;
		co << "# �̈�폜�̂��߂̍ő�ȗ����a�i�ᑬ��p�j�im�j : " << m_OutputOptions.maxROCForRegionDeletion( false ) << endl;
		co << "# �̈�폜�̂��߂̍ő�ȗ����a�i������p�j�im�j : " << m_OutputOptions.maxROCForRegionDeletion( true ) << endl;
		co << "# ��Ԃ̍ő�ȗ����a�i�ᑬ��p�j�im�j : " << m_OutputOptions.maxROCOfSection( false ) << endl;
		co << "# ��Ԃ̍ő�ȗ����a�i������p�j�im�j : " << m_OutputOptions.maxROCOfSection( true ) << endl;
		co << "# �����ƌ��Ȃ��p�x�i�x�j : " << m_OutputOptions.m_StraightAngle << endl;
		co << "# �ȗ����a�����߂�_�̊Ԋu(m) : " << m_OutputOptions.m_ROCPointInterval << endl;
		co << "# �J�[�u�F��p�x�i�x�j : " << m_OutputOptions.m_CurveCertifyAngle << endl;
		co << "# �~�ʂ��x�W�G�Ȑ��Ƀt�B�b�g���邩��]������_�̊Ԋu(m) : " << m_OutputOptions.m_FittingPointInterval << endl;
		co << "# �~�ʂ��x�W�G�Ȑ��Ƀt�B�b�g����ƌ��Ȃ�臒l(m) : " << m_OutputOptions.m_FittingThreshold << endl;
		co << "# ��ԂɑΉ�����~�ʂ̍ŏ��p�x�i�x�j : " << m_OutputOptions.m_SectionMinAngle << endl;
		co << "# �Ώۃ����N�w��N�G�� : " << (m_OutputOptions.m_QueryWhereClause.IsEmpty() ? "�Ȃ�" : CStringA(m_OutputOptions.m_QueryWhereClause)) << endl;
		co << "# �o�͒P�� : " << (m_OutputOptions.m_CreateEachMesh ? "���b�V�����Ƃɏo��" : "�P�g��Shape�t�@�C���Q�ɂ܂Ƃ߂ďo��") << endl;
		co << "# SHP05�̃f�B���N�g���\���ŏo�� : " << (m_OutputOptions.m_SHP05DirConfig ? "����" : "���Ȃ�") << endl;
		co << "# ���ؗp���O�쐬 : " << (m_OutputOptions.m_OutputVerificationLog ? "����" : "���Ȃ�") << endl;
		co << "# �����ߒ������O�t�@�C���ɏo�� : " << (m_OutputOptions.m_LogHandlingProcess ? "����" : "���Ȃ�") << endl;
		co << "# �t�B�b�g�]���ߒ������O�t�@�C���ɏo�� : " << (m_OutputOptions.m_LogFitEvaluatingProcess ? "����" : "���Ȃ�") << endl;
	}

	void CurveInfoCreator::outputElapsedTime( std::string cTask, bool cIsStart )
	{
		static boost::timer t;
		if (cIsStart)
		{
			t.restart();
			std::cerr << cTask;
			m_LogStream << "# " << cTask << " �J�n" << std::endl;
		}
		else
		{
			std::cerr << "(" << t.elapsed() << "sec)" << std::endl;
			m_LogStream << "# " << cTask << " �I��" << "(" << t.elapsed() << "sec)"<< std::endl;
		}
	}

	bool CurveInfoCreator::convert()
	{
		// �Q�����b�V�����Ƃɏ���
		BOOST_FOREACH (long aMeshCode, m_BaseMeshList)
		{
			// ����9���b�V���ȊO�̃L���b�V�������
			m_pCacheManager->clearExcept( surroundingMeshCodes( aMeshCode ) );

			try
			{
				convertMesh( aMeshCode );
					
				// ���b�V�����Ƃɏo��
				if (m_OutputOptions.m_CreateEachMesh)
				{
					createShape( aMeshCode );
				}

				++m_SuccessfulMeshCount;
			}
			catch (std::runtime_error& e)
			{
				// ��O������������A�G���[���b�Z�[�W���o�͂��Ă��̃��b�V�����X�L�b�v
				notifier::asAbortedByUnexpectedData( std::cerr, aMeshCode );
				notifier::asAbortedByUnexpectedData( m_LogStream, aMeshCode );
				std::cerr << std::endl;		// �r���ł�߂�ƃv�����v�g�̕\�������r���[�ɂȂ�̂ŉ��s���Ă���
			}
		}

		try
		{
			// �P�g��Shape�t�@�C���Q�ɂ܂Ƃ߂ďo��
			if (! m_OutputOptions.m_CreateEachMesh)
			{
				createShape();
			}
		}
		catch (std::runtime_error& e)
		{
			notifier::asAbortedByUnexpectedData( m_LogStream );
			std::cerr << std::endl;
		}

		std::cerr << "--------------------------------------------------" << std::endl;
		m_LogStream << "#--------------------------------------------------" << std::endl;

		// �I�����������O�ɏo��
		m_LogStream << "# �I������ : " << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time() ) << std::endl;

		return m_SuccessfulMeshCount == m_BaseMeshList.size();
	}

	bool CurveInfoCreator::isTargetLink( IFeaturePtr cpLink )
	{
		using namespace sindy::schema;

		// �\����ʂɂ�锻��
		long aRoadClass = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kRoadClass ) );
		switch (aRoadClass)
		{
			using namespace road_link::road_class;
			// �t�F���[�q�H�͑ΏۊO
			case kFerryNoDisp:
			case kFerryS2:
			case kFerryS3:
			case kFerryS4:
				return false;
			default:
				break;
		}

		// ���������ʍs�֎~�ƒʍs�s�̃����N�͑ΏۊO�Ƃ���iBug9642�j
		if (! isPassable( cpLink ))
			{ return false; }

		return true;
	}

	/**
	 * @brief �w�肳�ꂽ�����N�̈���ʍs�K���̎�ނ�Ԃ�
	 * @param[in]	cpLink	�Ώۃ����N
	 * @return	����ʍs�K��
	 */
	oneway_type::ECode onewayType( IFeaturePtr cpLink )
	{
		using namespace sindy::schema;
		long aOneway = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kOneway ) );
		switch (aOneway)
		{
			using namespace road_link::oneway;
			case kNoOneway:
			case kForeWithReg:			// �����t���K���͋K�������Ɠ�������
			case kReverseWithReg:
			case kForeReverseWithReg:
				return oneway_type::none;
			case kFore:
				return oneway_type::fore;
			case kReverse:
				return oneway_type::back;
			default:
				ATLASSERT(0);
				return oneway_type::none;
		}
	}

	namespace get_oneside_single_road
	{
		/**
		 * @brief �Ώۃt�B�[�`�������݂���Q�����b�V���̃��b�V���R�[�h�Q��Ԃ�
		 * @note	�u���b�V�����E��m�[�h���ڐG���Ă��郁�b�V���Q�v�����߂邽�߂ɍ쐬�B
		 * @note	���@�͊���l�����邪�A�����ł́u�m�[�h�����b�V���v�̋�Ԍ����̃R�X�g������قǍ����Ȃ��A
		 *			�@������b�V�����E�݂̂Ȃ̂Ŗ��Ȃ��ƍl���č̗p�B
		 * @param[in]	cpFeature			�Ώۃt�B�[�`��
		 * @param[in]	cpBaseMeshClass		�Q�����b�V���t�B�[�`���N���X
		 * @return	���b�V���R�[�h�Q
		 */
		MeshCodes existingMesh( IFeaturePtr cpFeature, IFeatureClassPtr cpBaseMeshClass )
		{
			using namespace sindy::schema;

			MeshCodes aMeshCodes;
			IFeatureCursorPtr ipFeatureCursor = cpBaseMeshClass->_Search( AheGetSpatialFilter( cpFeature->GetShape(), cpBaseMeshClass, esriSpatialRelIntersects ), VARIANT_TRUE );
			IFeaturePtr ipFeature;
			while (ipFeatureCursor->NextFeature( &ipFeature ) == S_OK && ipFeature)
			{
				long aMeshCode = ipFeature->GetValue( fieldIndex( ipFeature, basemesh::kTableName, basemesh::kMeshCode ) );
				aMeshCodes.insert( aMeshCode );
			}

			return aMeshCodes;
		}

		typedef CIndexedContainer::IteratorContainer IteratorContainer;

		/**
		 * @brief �w�肳�ꂽ�L���b�V�����ŁA�Ώۃm�[�h�ɐڑ�����Ă��郊���N�Q��Ԃ�
		 * @param[in]	cNodeID		�Ώۃm�[�h
		 * @param[in]	cpLinkTable	�����Ώۃ����N�L���b�V��
		 * @return	�����N�Q
		 */
		IteratorContainer connectedLinks( long cNodeID, const CSPTableContainer cpLinkTable )
		{
			using namespace sindy::schema;
			const IteratorContainer& itrsFrom = cpLinkTable->finds_by_key( road_link::kFromNodeID, cNodeID );
			const IteratorContainer& itrsTo = cpLinkTable->finds_by_key( road_link::kToNodeID, cNodeID );
			IteratorContainer itrs;
			std::copy( itrsFrom.begin(), itrsFrom.end(), std::inserter( itrs, itrs.begin() ) );
			std::copy( itrsTo.begin(), itrsTo.end(), std::inserter( itrs, itrs.begin() ) );
			return itrs;
		}

	} // namespace get_oneside_single_road

	void CurveInfoCreator::getOneSideSingleRoad( IFeaturePtr cpLink, long cNodeID, bool cIsBack, long cMeshCode, SingleRoad& crSingleRoad )
	{
		using namespace get_oneside_single_road;
		using namespace sindy::schema;
		using namespace utility::sindylib_base;

		const long aCurrentLinkID = cpLink->OID;
		const long aCurrentMeshCode = cMeshCode;

		// �Ώۃ����N�ɒʍs�֎~�K������������I��
		if (! isPassable( cpLink ))
			{ return; }

		long aNextMeshCode = cMeshCode;		///< ���̃����N�����݂���Q�����b�V���̃��b�V���R�[�h

		const CSPTableFinder& pTableFinder = m_pCacheManager->cache( cMeshCode, m_OutputOptions.m_QueryWhereClause );	///< ���ڃ��b�V���̃L���b�V��

		// �Ώۃm�[�h���Q�����b�V�����E��m�[�h�Ȃ�A���̃��b�V���͂��̌��������̃��b�V���Ƃ���
		const CSPTableContainer pNodeTable = pTableFinder->FindTable( road_node::kTableName );
		_IRowPtr ipNode = irow( *pNodeTable->find_by_key( kObjectID, cNodeID ) );
		long aNodeClass = ipNode->GetValue( fieldIndex( ipNode, road_node::kTableName, road_node::kNodeClass ) );
		if (aNodeClass == road_node::node_class::kEdge)
		{
			// ���Ɉ����ׂ����b�V������肷��
			MeshCodes aMeshCodes = existingMesh( ipNode, m_pCacheManager->meshClass() );		///< ���b�V�����E��m�[�h���ڐG���Ă��郁�b�V���Q
			ATLASSERT( aMeshCodes.size() == 2);	// ���b�V����4���Ƀm�[�h�����݂��Ă��鎖�͂Ȃ��iRC1001�j�̂ŁA�ڐG���Ă��郁�b�V���̐��͕K��2�ł���͂�
			BOOST_FOREACH (long aMeshCode, aMeshCodes)
			{
				// ���ڃ��b�V���Ɠ���łȂ��B��̃��b�V�������Ɉ����ׂ����b�V���ł���
				if (aMeshCode != cMeshCode)
				{
					aNextMeshCode = aMeshCode;
				}
			}
		}
		// TODO: ���b�V�����E��m�[�h��SR�Ɋ܂߂�ƁA���ꂪ���݂��鎖�ɂ���ăx�W�G�Ȑ��̌`�󂪕ω����A
		// ���݂��Ȃ��ꍇ�Ƃ͌��ʂ������ɕς���Ă��܂��B�����ɂ̓��b�V�����E��m�[�h�͏��O���������悢�B
		// �i�����_�m�[�h�⑮���ω��_�m�[�h�ɂ��������������邪�c�B�j

		const CSPTableFinder pNextMeshTableFinder = m_pCacheManager->cache( aNextMeshCode, m_OutputOptions.m_QueryWhereClause );	///< ���Ɉ����ׂ����b�V���̃L���b�V��
		const CSPRoadLinkTable pNextLinkTable = road_link_table( pNextMeshTableFinder->FindTable( road_link::kTableName ) );	///< ���̃����N���܂ރ����N�L���b�V��

		// ���ڃm�[�h�ɐڑ����郊���N�Q�̂��ꂼ��ɂ���
		IteratorContainer aConnectedLinks = connectedLinks( cNodeID, pNextLinkTable );
		long aTargetLinkCount = 0;													// �ڑ������N�Q�̂����A�����Ώۃ����N�̏����𖞂������̂̐�
		IFeaturePtr ipNextLink;													// �ڑ������N�Q�̂����A�Ō�Ɍ��������i�Q���H�̏ꍇ�͗B��́j�����Ώۃ����N
		BOOST_FOREACH (CContainerBase::iterator itLink, aConnectedLinks)
		{
			IFeaturePtr ipLink = irow( *itLink );
			ATLASSERT( ipLink );
			if (! ipLink) { continue; }
			long aLinkID = ipLink->OID;

			// ���ڒ��̃����N�Ȃ�X�L�b�v
			if (aCurrentLinkID == aLinkID)
				{ continue; }

			// �u�����悩�ۂ��v���쐬���̈�{���ƈ�v���Ȃ���΃X�L�b�v�iBug9619�j
			if (isHighSpeedArea( ipLink ) != crSingleRoad.m_IsHighSpeedArea)
				{ continue; }

			// �����Ώۃ����N�̏����𖞂����Ȃ���΃X�L�b�v
			if (! isTargetLink( ipLink ))
				{ continue; }
			++aTargetLinkCount;
			ipNextLink = ipLink;
		}

		// �����Ώۃ����N�݂̂��l�������A���̐����P�{�i�Q���H�j�łȂ���ΏI��
		if (aTargetLinkCount != 1)
			{ return; }
		// �����܂ŗ���΁A���̃����N�̌�₪ipNextLink�����P�ɍi����B

		// ���̃����N�������ς݃����N�Ȃ�I���iBug9701�j
		if (m_ProcessedLinkIDs.find( ipNextLink->OID ) != m_ProcessedLinkIDs.end())
			{ return; }

		// ���̃����N�ɒʍs�֎~�K������������I��
		if (! isPassable( ipNextLink ))
			{ return; }

		// ���b�V�����E�łȂ���ΉE���܋֎~�K�����`�F�b�N
		if (cMeshCode == aNextMeshCode)
		{
			// �u���ڃ����N�����̃����N�v�A�u���̃����N�����ڃ����N�v�̗������ʍs�ł��Ȃ���ΏI���iBug9642�j
			if (! isPassableByTurnreg( cpLink, ipNextLink, *pNextLinkTable ) &&
				! isPassableByTurnreg( ipNextLink, cpLink, *pNextLinkTable ))
				{ return; }
		}

		// �󋵊m�F
		using namespace sindy::schema;
		long aFromNodeID = ipNextLink->GetValue( fieldIndex( ipNextLink, road_link::kTableName, road_link::kFromNodeID ) );
		long aToNodeID = ipNextLink->GetValue( fieldIndex( ipNextLink, road_link::kTableName, road_link::kToNodeID ) );
		bool aConnectionNodeIsFrom = (aFromNodeID == cNodeID);				///< �쐬���̈�{���̒[�Ɛڑ�����Ă���̂̓����N��From�m�[�h�ł��邩�ہiTo�m�[�h�ł���j��
		long aEndNodeID = aConnectionNodeIsFrom ? aToNodeID : aFromNodeID;

		// ����ʍs�K���̎�ނ��쐬���̈�{���ƍ���Ȃ����̂ł���ΏI���iBug9642�j
		long aLinkOnewayType = onewayType( ipNextLink );
		if (aLinkOnewayType == oneway_type::none)
		{
			if (crSingleRoad.m_Oneway != oneway_type::none)
				{ return; }
		}
		else
		{
			bool aLinkOnewayIsFore =  ( aLinkOnewayType == oneway_type::fore );							///< �����N�̈���ʍs�����������ۂ�
			bool aSROnewayIsFore = ( crSingleRoad.m_Oneway == oneway_type::fore );						///< ��{���̈���ʍs�����������ۂ�
			bool aLinkOnewayIsFromCurrentNode = ( aConnectionNodeIsFrom == aLinkOnewayIsFore );			///< �����N�̈���ʍs�̕��������ڃm�[�h��From�Ƃ���������ۂ�
			bool aCurrentNodeIsSROnewayToSide = ( aSROnewayIsFore == cIsBack );							///< ���ڃm�[�h�͈�{���̈���ʍs�̌��������ł��邩�ۂ�
			bool aLinkOnewayIsOK = ( aCurrentNodeIsSROnewayToSide == aLinkOnewayIsFromCurrentNode );	///< ��{���Ƀ����N��ǉ����Ă��ǂ����ۂ�
			if (! aLinkOnewayIsOK)
				{ return; }
		}

		// �Q���H�̍s����ɂ���B��̃����N��crSingleRoad�ɒǉ�
		crSingleRoad.addLink( ipNextLink, aConnectionNodeIsFrom, cIsBack );
		m_ProcessedLinkIDs.insert( ipNextLink->OID );

		// �ċA
		getOneSideSingleRoad( ipNextLink, aEndNodeID, cIsBack, aNextMeshCode, crSingleRoad );
	}

	SingleRoadPtr CurveInfoCreator::getSingleRoad( IFeaturePtr cpLink )
	{
		SingleRoadPtr pSingleRoad( new SingleRoad(
			isHighSpeedArea( cpLink ),
			onewayType( cpLink ),
			AheGetSpatialReference( cpLink ) ) );
		SingleRoad& rSingleRoad = *pSingleRoad;		// TODO: shared_ptr���w���I�u�W�F�N�g�𒼐ڎQ�Ƃ���̂͂悭�Ȃ��H

		using namespace sindy::schema;
		long aFromNodeID = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kFromNodeID ) );
		long aToNodeID = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kToNodeID ) );

		// SingleRoad�̍ŏ��̃����N�Ƃ���cpLink��ǉ�
		rSingleRoad.addLink( cpLink, true, true );

		getOneSideSingleRoad( cpLink, aFromNodeID,	false, m_MeshCode,  rSingleRoad );		// From�m�[�h���̈�{�������N�Q��SingleRoad��Front���ɒǉ�
		getOneSideSingleRoad( cpLink, aToNodeID,	true, m_MeshCode,	rSingleRoad );		// To�m�[�h���̈�{�������N�Q��SingleRoad��Back���ɒǉ�

		return pSingleRoad;
	}

	bool CurveInfoCreator::isLeft( angle_accumulating::curve_direction::ECode cDirection ) const
	{
		switch (cDirection)
		{
			using namespace angle_accumulating::curve_direction;
			case left:
				return true;
			case right:
				return false;
			case straight:
			case both:
			default:
				notifier::safeAssert( false, m_LogStream );
				throw std::runtime_error("��{���̕�������Ɏ��s");
				break;
		}
	}

	/// CurveInfoCreator::divideSingleRoad�̈ꕔ
	namespace divide_single_road
	{
		/**
		 * @brief �w�肳�ꂽ�͈͂ɂ���Z�O�����g��̒��������߂�
		 * @param[in]	cBeginSegmentIt	�͈͂̍ŏ��̃Z�O�����g
		 * @param[in]	cEndSegmentIt	�͈͂̍Ō�̃Z�O�����g
		 * @return	�����i�x�j
		 */
		double length( SegmentIt citBeginSegment, SegmentIt citEndSegment )
		{
			double aLength = 0;
			for (SegmentIt it = citBeginSegment; it != boost::next( citEndSegment ); ++it)
			{
				aLength += (*it)->length();
			}
			return aLength;
		}

		/// �ϋȓ_�Ɋւ�����
		struct InflectionInfo
		{
			bool		m_CurrentDirectionIsLeft;			///< �Ō�Ɍ��������ϋȓ_�̌�A���E�ǂ���ɋȂ����Ă��邩
			SegmentIt	m_itInflectionSegment;				///< �ϋȓ_�Z�O�����g
			SegmentPtr	m_pSegmentNextToInflectionPoint;	///< �ϋȓ_�̒���̉��z�Z�O�����g

			/// �R���X�g���N�^
			InflectionInfo(
				bool cCurrentDirectionIsLeft,
				SegmentIt citInflectionSegment,
				SegmentPtr cpSegmentNextToInflectionPoint
			) :
				m_CurrentDirectionIsLeft( cCurrentDirectionIsLeft ),
				m_itInflectionSegment( citInflectionSegment ),
				m_pSegmentNextToInflectionPoint( cpSegmentNextToInflectionPoint )
			{}
		};
		typedef std::tr1::shared_ptr< InflectionInfo > InflectionInfoPtr;

	} // namespace divide_single_road

	DirectionalSRArray CurveInfoCreator::divideSingleRoad( SingleRoadPtr cpSingleRoad ) const
	{
		ATLASSERT( cpSingleRoad );
		const SingleRoad& rSingleRoad = *cpSingleRoad;	// TODO: shared_ptr���w�����̂��Q�Ƃ���̂͂悭�Ȃ��H

		using namespace angle_accumulating;
		using namespace divide_single_road;

		LRAccumulating aLRAccumulating(						///< ���E�̊p�x�ݐϏ��
			rSingleRoad,
			m_OutputOptions.m_StraightAngle,
			m_OutputOptions.m_CurveCertifyAngle,
			m_pSpRef );

		DirectionalSRArray aDSRArray;								///< ��{���𕪊����鎖�ɂ���č쐬�����DSR��
		SegmentIt itCurrentDSRBeginSegment = rSingleRoad.begin();	///< �쐬����DSR�̎n�[�Z�O�����g�i���z�Z�O�����g�͏����j
		double aWayFromOrigin1 = 0;									///< ���̈�{���̎n�_����A�쐬����DSR�̎n�_�܂ł̓��̂�i�x�j
		InflectionInfoPtr pInflectionInfo;							///< �Ō�Ɍ��������ϋȓ_�Ɋւ�����

		// ��{�����\������e�Z�O�����g�ɂ���
		for (SegmentIt itCurrentSegment = rSingleRoad.begin(); itCurrentSegment != rSingleRoad.end(); ++itCurrentSegment )
		{
#ifdef _DEBUG
			long aLinkID = (*itCurrentSegment)->m_LinkID;
#endif
			// �ŏ��̃Z�O�����g�Ȃ�X�L�b�v
			if (itCurrentSegment == rSingleRoad.begin())
				{ continue; }

			SegmentIt itPrevSegment = boost::prior( itCurrentSegment );
			double aAngle = angle( **itPrevSegment, **itCurrentSegment );	///< �O�̃Z�O�����g�Ƃ̂Ȃ��p

			// �p�x�ݐϏ�Ԃ��X�V
			aLRAccumulating.update( aAngle, itCurrentSegment );
			
			// �ϋȓ_���������Ă��Ȃ���΃X�L�b�v
			IPointPtr pInflectionPoint = aLRAccumulating.m_pInflectionPoint;	///< �ϋȓ_
			if (! pInflectionPoint)
				{ continue; }
			SegmentIt itInflectionSegment = aLRAccumulating.m_itInflectionSegment;	///< �ϋȓ_�Z�O�����g
			ATLASSERT( itInflectionSegment != rSingleRoad.end() );
			// �ϋȓ_������������A�����ň�{���𕪊�����

			// ���E�̌���
			// �ϋȓ_�������������_�̂Q�Z�O�����g�łǂ���ɋȂ����Ă��邩�𒲂ׁA�ϋȓ_�̑O�͂��̋t�A�ϋȓ_�̌�͏������ɋȂ����Ă���Ƃ���
			bool aIsLeft = isLeft( aLRAccumulating.m_CurrentDirection );

			// �ϋȓ_�Z�O�����g�̎n�_����ϋȓ_�܂łɓ����鉼�z�Z�O�����g���쐬����
			IPointPtr& ipFromPoint = (*itInflectionSegment)->m_pFromPoint;
			IPointPtr& ipToPoint = (*itInflectionSegment)->m_pToPoint;
			SegmentPtr ipBeforeInflectionSegment( new Segment( ipFromPoint, pInflectionPoint ) );	///< �ϋȓ_�̒��O�̉��z�Z�O�����g
			if (m_OutputOptions.m_LogHandlingProcess)
			{
				m_LogStream << utility::log_format::pointRecord( pInflectionPoint->GetX(), pInflectionPoint->GetY(), "�ϋȓ_" );
			}

			double aWayFromOrigin2 = length( rSingleRoad.begin(), boost::prior( itInflectionSegment ) ) + ipBeforeInflectionSegment->length();	///< ���̈�{���̎n�_����ϋȓ_�܂ł̓��̂�i�x�j
			SingleRoad::Range aRange = std::make_pair(
				aWayFromOrigin1 / rSingleRoad.length(), 
				aWayFromOrigin2 / rSingleRoad.length() );			///< ���̈�{����ɂ�����A�쐬����DSR�͈̔�

			// �V����DSR���쐬
			DirectionalSRPtr pCurrentDSR = DirectionalSRPtr( new DirectionalSR(
				! aIsLeft,
				aRange,
				rSingleRoad.m_IsHighSpeedArea,
				rSingleRoad.m_Oneway,
				rSingleRoad.m_pSpRef ) );
			// �P�O�̕ϋȓ_�����݂���ꍇ�A���̒���̉��z�Z�O�����g��ǉ�
			if (pInflectionInfo)
			{
				pCurrentDSR->add( pInflectionInfo->m_pSegmentNextToInflectionPoint );
			}
			// �n�[�Z�O�����g����ϋȓ_�Z�O�����g�̂P�O�܂ł̃Z�O�����g���ǉ�
			std::copy( itCurrentDSRBeginSegment, itInflectionSegment, std::back_inserter( *pCurrentDSR ) );
			// �ϋȓ_�̒��O�̉��z�Z�O�����g��ǉ�
			pCurrentDSR->add( ipBeforeInflectionSegment );

			// �쐬����DSR��o�^
			aDSRArray.push_back( pCurrentDSR );

			itCurrentDSRBeginSegment = boost::next( itInflectionSegment );
			aWayFromOrigin1 = aWayFromOrigin2;

			// �ϋȓ_����ϋȓ_�Z�O�����g�̏I�_�܂łɓ����鉼�z�Z�O�����g���쐬
			SegmentPtr pSegmentNextToInflectionPoint = SegmentPtr( new Segment( pInflectionPoint, ipToPoint ) );

			// �ϋȓ_�Ɋւ�������L�^
			pInflectionInfo = InflectionInfoPtr( new InflectionInfo( aIsLeft, itInflectionSegment, pSegmentNextToInflectionPoint ) );

			// �ݐϏ�Ԃ����������āA�u�ϋȓ_�Z�O�����g�Ƃ��̎��̃Z�O�����g�v����ĊJ
			itCurrentSegment = itInflectionSegment;
			aLRAccumulating.reset();
		}

		// �ϋȓ_���P�ȏ㌩�����Ă���ꍇ
		if (pInflectionInfo)
		{
			// �Ō�Ɍ��������ϋȓ_����A���̈�{���̏I�_�܂ł̃Z�O�����g��ɓ�����DSR���쐬
			SingleRoad::Range aRange = std::make_pair(
				aWayFromOrigin1 / rSingleRoad.length(), 
				1
			);
			DirectionalSRPtr pCurrentDSR = DirectionalSRPtr( new DirectionalSR(
				pInflectionInfo->m_CurrentDirectionIsLeft,
				aRange,
				rSingleRoad.m_IsHighSpeedArea,
				rSingleRoad.m_Oneway,
				rSingleRoad.m_pSpRef ) );
			// �ϋȓ_�Z�O�����g�̎��̃Z�O�����g����cpSingleRoad�̏I�_�܂ł̃Z�O�����g���A�쐬����DSR�ɒǉ�
			ATLASSERT( pInflectionInfo->m_itInflectionSegment != rSingleRoad.end() );	// �������Ă���͂�
			// �Ō�̕ϋȓ_�̒���̉��z�Z�O�����g��ǉ�
			pCurrentDSR->add( pInflectionInfo->m_pSegmentNextToInflectionPoint );

			// �ϋȓ_�Z�O�����g�̎����猳�̈�{���̍Ō�܂ł̃Z�O�����g���ǉ�
			std::copy( boost::next( pInflectionInfo->m_itInflectionSegment ), rSingleRoad.end(), std::back_inserter( *pCurrentDSR ) );
			// �쐬����DSR��o�^
			aDSRArray.push_back( pCurrentDSR );
		}
		// �ϋȓ_���P���������Ă��Ȃ��ꍇ
		else
		{
			// ���̈�{���́u�E�ɋȂ����Ă���v�u���ɋȂ����Ă���v�u�^�������v�̂����ꂩ�ł���͂��B
			// �u�^�������v�̏ꍇ�͂��̗̈�ɃJ�[�u�͑��݂��Ȃ����̂ƌ��Ȃ��A���̏��͔j������B
			if (aLRAccumulating.direction() == angle_accumulating::curve_direction::straight)
			{
				return aDSRArray;		// ��̂܂ܕԂ�
			}

			// ���̈�{���S�̂ɓ�����DSR���쐬
			SingleRoad::Range aRange = std::make_pair( 0, 1 );
			DirectionalSRPtr pCurrentDSR = DirectionalSRPtr( new DirectionalSR(
				isLeft( aLRAccumulating.direction() ),
				aRange,
				rSingleRoad.m_IsHighSpeedArea,
				rSingleRoad.m_Oneway,
				rSingleRoad.m_pSpRef ) );
			std::copy( rSingleRoad.begin(), rSingleRoad.end(), std::back_inserter( *pCurrentDSR ) );
			aDSRArray.push_back( pCurrentDSR );
		}

		return aDSRArray;
	}

	/**
	 * @brief CurveInfoCreator::createROCPointInfos�̈ꕔ
	 */
	namespace create_roc_point_infos
	{
		/**
		 * @brief �^����ꂽ�|�����C���̃Z�O�����g���̕��ϒl�����߂�
		 * @param[in]	cpPolyline	�Ώۃ|�����C��
		 * @return	�Z�O�����g���̕��ϒl
		 */
		double segmentLengthAverage( const IPolyline4Ptr cpPolyline )
		{
			double aSegmentLengthSum = 0;		///< �Z�O�����g���̘a
			ISegmentCollectionPtr ipSegmentCol( cpPolyline );
			for (long i = 0; i < ipSegmentCol->GetSegmentCount(); ++i)
			{
				_ISegmentPtr ipSegment = ipSegmentCol->GetSegment( i );
				aSegmentLengthSum += ipSegment->GetLength();
			}
			return aSegmentLengthSum / ipSegmentCol->GetSegmentCount();
		}
	} // namespace create_roc_point_infos

	ROCPointInfosPtr CurveInfoCreator::createROCPointInfos( const DirectionalSR& crDirectionalSR )
	{
		using namespace create_roc_point_infos;
		using namespace circle_fit_evaluator;

		// DSR�S�̂̃W�I���g�����쐬
		IPolycurve3Ptr ipPolycurve( crDirectionalSR.geometry() );
		ATLASSERT( ipPolycurve );

		// �����Z�O�����g��ɍ\���_��ǉ�
		addPointsOnLongSegment( ipPolycurve );

		// �x�W�G�Ȑ��Q��
		HRESULT hr = ipPolycurve->Smooth( 0 );
		ATLASSERT( hr == S_OK );
		IPolycurve3Ptr& ipBezierCurves = ipPolycurve;
		setSpatialReference( ipBezierCurves, m_pSpRef );

		static const CircleFitEvaluator::DistanceOf3PointsArray aDistanceOf3PointsArray = boost::assign::list_of
			( 10 )( 20 )( 30 )( 40 )( 60 )( 80 )( 100 )( 200 )( 500 )( 1000 );		///< �u�R�_�̊Ԋu�v�̌��̏W��	TODO: �R�}���h���C�������Ŏw��\�ɂ��ׂ��H

		// ����DSR�̑�P�Z�O�����g�̎n�_�ƏI�_�ɂ��A�x��m�̕ϊ��W��������
		SegmentPtr pFirstSegment = NONZERO( *crDirectionalSR.begin() );
		ATLASSERT( pFirstSegment->valid() );
		double aDegreeToMeterCoef = degreeToMeterCoef(													///< �����̒P�ʂ�x����m�ɕϊ����邽�߂Ɋ|����ׂ��W��
			pFirstSegment->m_pFromPoint->GetX(), pFirstSegment->m_pFromPoint->GetY(),
			pFirstSegment->m_pToPoint->GetX(), pFirstSegment->m_pToPoint->GetY()
		);
		if (m_OutputOptions.m_LogHandlingProcess)
		{
			m_LogStream << utility::log_format::freeComment( std::string("�x��m�̕ϊ��W��=") + utility::toStr( aDegreeToMeterCoef, 20 ) );
		}

		const double aROCPointInterval = m_OutputOptions.m_ROCPointInterval / aDegreeToMeterCoef;					///< �ȗ����a�����߂�_�i�����_�j�̊Ԋu�i�x�j
		const double aFittingPointInterval = m_OutputOptions.m_FittingPointInterval / aDegreeToMeterCoef;			///< �~�ʂ��x�W�G�Ȑ��Ƀt�B�b�g���邩��]������_�̊Ԋu�i�x�j
		const double aFittingThreshold = m_OutputOptions.m_FittingThreshold / aDegreeToMeterCoef;					///< �~�ʂ��x�W�G�Ȑ��Ƀt�B�b�g����ƌ��Ȃ�臒l�i�x�j
#ifdef _DEBUG
		double aBezierCurvesLength = ipBezierCurves->Length;
#endif
		const double aLengthByInterval = ipBezierCurves->Length / aROCPointInterval;					///< aROCPointInterval��1�Ƃ������̃x�W�G�Ȑ��Q�̒���
		const long aPointIndexMax =	static_cast<long>( aLengthByInterval );								///< �����_�̃C���f�b�N�X�̍ő�l

		const SingleRoad::Range rRange = crDirectionalSR.m_Range;
		const double aROCPointIntervalRatio = ( rRange.second - rRange.first ) / aLengthByInterval;		///< �P�_�i�ނ��ƂɁA���̈�{���̂ǂꂾ���̊�����i�ނ�

		ROCPointInfosPtr pROCPointInfos( new ROCPointInfos );

		// TODO: DSR�̎n�_�ł͂Ȃ����_����ɂ���ƁA���Ώ̐������܂�i���ʂ���ӂł���΂悢�A�Ƃ���Ȃ�K�v�Ȃ����c�j

		// �e�����_�ɂ���
		for (long aPointIndex = 0; aPointIndex <= aPointIndexMax; ++aPointIndex)
		{
			double aCurrentPointRatio = rRange.first + aROCPointIntervalRatio * aPointIndex;	///< ���̈�{����̒��ړ_�̈ʒu

			// ���ڂ���_�̈ʒu������
			IPointPtr ipPoint2 = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
			const double aDAC2 = aROCPointInterval * aPointIndex;		///< �R�_�̓��̑�Q�_�́A�y�W�G�Ȑ��Q�̎n�_����̓��̂�iDistanceAlongCurve�j
			hr = ipBezierCurves->QueryPoint( esriNoExtension, aDAC2, VARIANT_FALSE, ipPoint2 );
			ATLASSERT( hr == S_OK );
			if (! ipPoint2)
			{
				notifier::safeAssert(0, m_LogStream);
				continue;
			}
			setSpatialReference( ipPoint2, m_pSpRef );

			if (m_OutputOptions.m_LogHandlingProcess)
			{
				m_LogStream << utility::log_format::pointRecord( ipPoint2->GetX(), ipPoint2->GetY(), "�ȗ����a�����߂�_" );
			}

			// �]��
			CircleFitEvaluator aCircleFitEvaluator(
				ipBezierCurves,
				ipPoint2,
				aDAC2,
				aDistanceOf3PointsArray,
				aFittingPointInterval,
				aFittingThreshold,
				aDegreeToMeterCoef,
				m_LogStream,
				m_OutputOptions.m_LogFitEvaluatingProcess);
			// �]�����ׂ��ʒu�łȂ���΃X�L�b�v
			ResultsPtr pResults = aCircleFitEvaluator.evaluate();
			if (! pResults)
				{ continue; }

			notifier::safeAssert( 0 <= aCurrentPointRatio && aCurrentPointRatio <= 1, m_LogStream);	// TODO: double�̉��Z�덷�ɂ����1�𒴂��鎖�����邩���c�B

			// �_�Ƌȗ����a���L�^
			pROCPointInfos->push_back( ROCPointInfoPtr( new ROCPointInfo( ipPoint2, pResults, aCurrentPointRatio ) ) );
		}

		return pROCPointInfos;
	}

	void CurveInfoCreator::addPointsOnLongSegment( IPolyline4Ptr cpPolyline ) const
	{
		using namespace create_roc_point_infos;

		// �Z�O�����g���̕��ϒl����A�_�̊Ԋu�̋��e�l�����߂�
		double aLengthThreshold = segmentLengthAverage( cpPolyline ) * 2;	///< �Z�O�����g���u�����v�Ɣ��f����臒l
	
		// �|�����C�����\������e�Z�O�����g�ɂ���
		ISegmentCollectionPtr ipSegmentCol( cpPolyline );
		long aSegmentCount = ipSegmentCol->GetSegmentCount();
		for (long i = 0; i < aSegmentCount; ++i)
		{
			_ISegmentPtr ipSegment = ipSegmentCol->GetSegment( i );
			double aSegmentLength = ipSegment->GetLength();

			// �����Z�O�����g������΍\���_��ǉ�����
			if (aLengthThreshold < aSegmentLength)
			{
				IPointPtr ipNewPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				ipSegment->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipNewPoint );
				ATLASSERT( ipNewPoint );
				setSpatialReference( ipNewPoint, m_pSpRef );

				if (m_OutputOptions.m_LogHandlingProcess)
				{
					m_LogStream << utility::log_format::pointRecord( ipNewPoint->GetX(), ipNewPoint->GetY(), std::string( "�����Z�O�����g�ɒǉ����ꂽ�_ SegmentIndex=" ) + boost::lexical_cast< std::string >( i ) );
				}

				IPointCollectionPtr ipPointCol2 = newGeometry< IPointCollectionPtr >( CLSID_Polyline, m_pSpRef );
				ipPointCol2->AddPoint( ipNewPoint );
				IPointCollectionPtr( cpPolyline )->InsertPointCollection( i + 1, ipPointCol2 );
				setSpatialReference( ipPointCol2, m_pSpRef );

				// ���ڒ��̃Z�O�����g���ǉ����ꂽ�_�ɂ���ĕ�������Ăł����Q�Z�O�����g�̂����A�O�̂��̂��珈�����ĊJ�����悤�ɂ���
				// �i���ꂪ�܂�������΂���ɓ_���ǉ������悤�Ɂj
				--i;
				aSegmentCount = ipSegmentCol->GetSegmentCount();	// �Z�O�����g�����X�V
			}
		}
	}

	void CurveInfoCreator::writeShape(
		CSHPHandle& crCSHPHandle,
		CDBFHandle& crCDBFHandle,
		FieldIndexMap& crFieldIndexMap,
		const CSHPPointArray& crPointArray,
		const Section& crSection,
		long cMeshCode) const
	{
		long aOID = crCSHPHandle.write_Object( &CSHPObject( SHPT_ARC, crPointArray ) );
		long aROC = roundOff( crSection.ROC() );
		long aRepLinkID = crSection.m_RepLinkID;
		double aWayToRepPoint = crSection.m_WayToRepPoint;

		using namespace schema::curve_line;
		crCDBFHandle.write_Attribute( aOID, crFieldIndexMap[kROC],				aROC );
		crCDBFHandle.write_Attribute( aOID, crFieldIndexMap[kRepLinkID],		aRepLinkID );
		crCDBFHandle.write_Attribute( aOID, crFieldIndexMap[kWayToRepPoint],	aWayToRepPoint );

		// ���ؗp���O�ւ̏o��
		if (m_OutputOptions.m_OutputVerificationLog)
		{
			std::stringstream aLayerName;
			if (m_OutputOptions.m_CreateEachMesh)
			{
				aLayerName << cMeshCode << "_CRV";
			}
			else
			{
				aLayerName << "CRV";
			}

			// ���
			m_VerificationLogStream 
				<< "0" << fd
				<< aLayerName.str() << fd
				<< aOID << fd
				<< fd
				<< fd
				<< aROC << fd
				<< aRepLinkID << fd
				<< std::setprecision( 20 ) << aWayToRepPoint << fd
				<< crSection.m_DistanceOf3P << fd
				<< crSection.m_FitRegionAngle << fd
				<< std::endl;

			// ��\�_
			m_VerificationLogStream 
				<< "0" << fd
				<< "��\�_" << fd
				<< aOID << fd
				<< crSection.m_pRepPoint->GetX() << fd
				<< crSection.m_pRepPoint->GetY() << fd
				<< std::endl;
		}
	}

	void CurveInfoCreator::addSection( SectionPtr cpSection )
	{
		ATLASSERT( cpSection );

		// �Ώۋ�Ԃ̑�\�_���݃����N�����݂��郁�b�V���̋�ԌQ�ɑΏۋ�Ԃ�ǉ�
		long aMeshCode = m_pCacheManager->existingMeshCode( cpSection->m_RepLinkID );	///< ��\�_���݃����N�����݂��郁�b�V���R�[�h
		m_SectionsByMesh[ aMeshCode ].push_back( cpSection );
	}

	void CurveInfoCreator::createSections( const DirectionalSRInfo& crDirectionalSRInfo )
	{
		polylinize( crDirectionalSRInfo );
	}

	/**
	 * @brief CurveInfoCreator::polylinize()�̈ꕔ
	 */
	namespace polylinize
	{
		/**
		 * @brief DSR��񂩂��Ԍ��Q���쐬����
		 * @param[in]	crDirectionalSRInfo		DSR���
		 * @param[in]	crOutputOptions			�o�̓I�v�V����
		 * @param[out]	crSectionCandidates		��Ԍ��Q�̊i�[��
		 */
		void createSectionCandidates(
			const DirectionalSRInfo& crDirectionalSRInfo,
			const OutputOptions& crOutputOptions,
			SectionCandidates& crSectionCandidates)
		{
			ATLASSERT( crDirectionalSRInfo.m_pDirectionalSR );
			const ROCPointInfos& rROCPointInfos = *crDirectionalSRInfo.m_pROCPointInfos;

			crSectionCandidates.clear();
			SectionCandidatePtr pSectionCandidate;	///< �쐬���̋�Ԍ��

			// �_��̊e�_�ɂ���
			for (ROCPointInfos::const_iterator itROCPointInfo = rROCPointInfos.begin(); itROCPointInfo != rROCPointInfos.end(); ++itROCPointInfo)
			{
				const ROCPointInfoPtr pROCPointInfo = *itROCPointInfo;
				ATLASSERT( pROCPointInfo );
				const ROCPointInfo& rROCPointInfo = *pROCPointInfo;
				// ���ړ_�ɂ�����ȗ����a������
				// �_�ɂ�����ȗ����a�́A�i�����̓_�̕]�����ʌQ�𑍍��I�ɔ��f����u��Ԃ̋ȗ����a�v�ƈقȂ�j
				// ���̓_�݂̂ɂ�����]�����ʌQ�̂����A�ł����]���̂��̂��̗p����B
				double aROC = circle_fit_evaluator::bestFitResult( *rROCPointInfo.m_pCFEResults )->m_ROC;

				long aMaxROC = crOutputOptions.maxROCForRegionDeletion( crDirectionalSRInfo.m_pSingleRoad->m_IsHighSpeedArea );	///< �ő�ȗ����a

				// �ȗ����a���ő�ȗ����a�ȉ��̓_����������
				if (aROC <= aMaxROC)
				{
					// ��Ԍ�₪���쐬�Ȃ�쐬����
					if (! pSectionCandidate)
					{
						pSectionCandidate = SectionCandidatePtr( new SectionCandidate( crDirectionalSRInfo ) );
						pSectionCandidate->m_Range.first = rROCPointInfo.m_Ratio;
					}

					// ��Ԍ��ɓ_����ǉ�
					pSectionCandidate->m_ROCPointInfos.push_back( pROCPointInfo );
				}
				// �ȗ����a���ő�ȗ����a�𒴂���_����������
				else
				{
					// ��Ԍ�₪�쐬�ς݂Ȃ�
					if (pSectionCandidate)
					{
						pSectionCandidate->m_Range.second = (*boost::prior( itROCPointInfo ))->m_Ratio;	// �P�O�ŏI��
						crSectionCandidates.push_back( SectionCandidate( *pSectionCandidate ) );		// �R�s�[��ǉ�
						pSectionCandidate = SectionCandidatePtr();	// NULL�ɂ���
					}
				}

				// �Ō�̓_�Ȃ�
				if (itROCPointInfo == boost::prior( rROCPointInfos.end() ))
				{
					// ��Ԍ�₪�쐬�ς݂Ȃ�
					if (pSectionCandidate)
					{
						pSectionCandidate->m_Range.second = (*itROCPointInfo)->m_Ratio;	// �����ŏI��
//						ATLASSERT( pSectionCandidate->m_Range.first != pSectionCandidate->m_Range.second);	// TODO: �K�v�Ȃ炱���Ń`�F�b�N
						crSectionCandidates.push_back( SectionCandidate( *pSectionCandidate ) );		// �R�s�[��ǉ�
						pSectionCandidate = SectionCandidatePtr();	// NULL�ɂ���
					}
				}
			}
		}
	} // namespace polylinize

	void CurveInfoCreator::polylinize( const DirectionalSRInfo& crDirectionalSRInfo )
	{
		using namespace polylinize;

		ATLASSERT( crDirectionalSRInfo.m_pSingleRoad );
		ATLASSERT( crDirectionalSRInfo.m_pDirectionalSR );
		ATLASSERT( crDirectionalSRInfo.m_pROCPointInfos );

		const SingleRoad& rSingleRoad = *crDirectionalSRInfo.m_pSingleRoad;
		const ROCPointInfos& rROCPointInfos = *crDirectionalSRInfo.m_pROCPointInfos;
		const DirectionalSR& rDirectionalSR = *crDirectionalSRInfo.m_pDirectionalSR;

		// ��Ԍ��Q���쐬
		SectionCandidates aSectionCandidates;	///< ��Ԍ��Q
		createSectionCandidates( crDirectionalSRInfo, m_OutputOptions, aSectionCandidates );

		// �e��Ԍ��ɂ���
		BOOST_FOREACH (const SectionCandidate& rSectionCandidate, aSectionCandidates)
		{
			HRESULT hr;
			const SingleRoad& rSingleRoad = *rSectionCandidate.m_rDirectionalSRInfo.m_pSingleRoad;	///< ��Ԍ����܂ވ�{��

			// ��ԍ쐬
			SectionPtr pSection( new Section( rSectionCandidate.m_rDirectionalSRInfo.m_pDirectionalSR->m_IsLeftCurve, rSingleRoad.m_IsHighSpeedArea ) );

			// ��\�_�Ɋւ������ݒ�
			setRepPointInfo( rSingleRoad, rSectionCandidate, pSection );

			// ��Ԃ̓_����쐬
			{
				IPolylinePtr ipSRPolyline = rSingleRoad.geometry();	///< ��{���S�̂̌`��

				// ��Ԍ��̎n�I�_�����߂�
				IPointPtr ipBeginPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				IPointPtr ipEndPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				{
					hr = ipSRPolyline->QueryPoint( esriNoExtension, rSectionCandidate.m_Range.first, VARIANT_TRUE, ipBeginPoint );
					ATLASSERT( hr == S_OK );
					hr = ipSRPolyline->QueryPoint( esriNoExtension, rSectionCandidate.m_Range.second, VARIANT_TRUE, ipEndPoint );
					ATLASSERT( hr == S_OK );
				}
				setSpatialReference( ipBeginPoint, m_pSpRef );
				setSpatialReference( ipEndPoint, m_pSpRef );

				Segments aSegments = rSingleRoad.segments( rSectionCandidate.m_Range );		///< ��Ԍ����܂���Z�O�����g��
				for (SegmentIt itSegment = aSegments.begin(); itSegment != aSegments.end(); ++itSegment)
				{
					// �ŏ��̃Z�O�����g�Ȃ�A��Ԍ��̎n�_����Ԃɒǉ�
					if (itSegment == aSegments.begin())
					{
						pSection->m_Points.push_back( ipBeginPoint );
					}

					// �Ō�̃Z�O�����g�Ȃ�A��Ԍ��̏I�_����Ԃɒǉ�
					if (itSegment == boost::prior( aSegments.end() ))
					{
						pSection->m_Points.push_back( ipEndPoint );
					}
					// �Ō�̃Z�O�����g�ȊO�Ȃ�A�Z�O�����g�I�_����Ԃɒǉ�
					else
					{
						pSection->m_Points.push_back( (*itSegment)->m_pToPoint );
					}
				}
			}

			// �o�^
			if (3 <= pSection->m_Points.size())		// �\���_��3�_�����̋�Ԃ͍̗p���Ȃ�	TODO: ���ɂ��o�^���ׂ��łȂ��ꍇ������H
			{
				addSection( pSection );
			}
		}
	}

	void CurveInfoCreator::setRepPointInfo(
		const SingleRoad& crSingleRoad,
		const SectionCandidate& crSectionCandidate,
		SectionPtr cpSection ) const
	{
		using namespace polylinize;
		using namespace circle_fit_evaluator;
		using namespace sindy::schema;
		using namespace utility::sindylib_base;

		// ��Ԍ��͈̔͂Ɋ܂܂��e�_�̕]�����ʌQ���A�P�̃R���e�i�ɏW��
		Results aResults;		///< �]�����ʃR���e�i
		BOOST_FOREACH (const ROCPointInfoPtr cpROCPointInfo, crSectionCandidate.m_ROCPointInfos)
		{
			ResultsPtr pResults = cpROCPointInfo->m_pCFEResults;
			ATLASSERT( pResults );
			std::copy( pResults->begin(), pResults->end(), std::inserter( aResults, aResults.begin() ) );
		}
		// �ǂ̕]�����ʂ��̗p���邩�����肷��
		// ��Ԍ��͈̔͂Ɋ܂܂��_�Q�ɂ�����]�����ʌQ�̂����A�ł����]���̂��̂��̗p����B
		const ResultPtr pBestFitResult = bestFitResult( aResults, m_OutputOptions.m_LogHandlingProcess ? &m_LogStream : NULL );
		ATLASSERT( pBestFitResult );
		const ROCPointInfoPtr pRepPointInfo = crSectionCandidate.m_ROCPointInfos.searchByPoint( pBestFitResult->m_pTargetPoint );
		ATLASSERT( pRepPointInfo );
		const ROCPointInfo& rRepPointInfo = *pRepPointInfo;	///< ��\�_���

		long aRepLinkID = (*crSingleRoad.segment( rRepPointInfo.m_Ratio ))->m_LinkID;
		const CSPTableContainer pLinkTable = m_pCacheManager->cache( m_pCacheManager->existingMeshCode( aRepLinkID ), m_OutputOptions.m_QueryWhereClause )->FindTable( road_link::kTableName );
		IPolylinePtr ipRepLinkGeom = IFeaturePtr( irow( *pLinkTable->find_by_key( kObjectID, aRepLinkID ) ) )->GetShape();	///< ��\�_���݃����N�̌`��
		double aDistanceAlongCurve = 0;
		double aDistanceFromCurve = 0;		// �g�p���Ȃ�
		IPointPtr ipRepPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
		ipRepLinkGeom->QueryPointAndDistance( esriNoExtension, rRepPointInfo.m_pPoint, VARIANT_FALSE, ipRepPoint, &aDistanceAlongCurve, &aDistanceFromCurve, VARIANT_BOOL() );
		setSpatialReference( ipRepPoint, m_pSpRef );

		// ��\�����N�̎n�_�ƏI�_�ɂ��A�x��m�̕ϊ��W��������
		double aDegreeToMeterCoef = degreeToMeterCoef(
			ipRepLinkGeom->GetFromPoint()->GetX(),	ipRepLinkGeom->GetFromPoint()->GetY(),
			ipRepLinkGeom->GetToPoint()->GetX(),	ipRepLinkGeom->GetToPoint()->GetY()
		);
		if (m_OutputOptions.m_LogHandlingProcess)
		{
			m_LogStream << utility::log_format::freeComment( std::string("�x��m�̕ϊ��W��=") + utility::toStr( aDegreeToMeterCoef, 20 ) );
		}

		cpSection->m_RepLinkID = aRepLinkID;
		cpSection->m_WayToRepPoint = aDistanceAlongCurve * aDegreeToMeterCoef;
		cpSection->m_pRepPoint = ipRepPoint;
		cpSection->m_ROC = pBestFitResult->m_ROC;
		cpSection->m_DistanceOf3P = pBestFitResult->m_DistanceOf3Points;

		double aFitRegionLength = (pBestFitResult->m_FitCount - 1) * m_OutputOptions.m_FittingPointInterval;	///< �t�B�b�g�̈�̒���
		cpSection->m_FitRegionAngle = circularArcAngle( aFitRegionLength, pBestFitResult->m_ROC );
		ATLASSERT( cpSection->m_FitRegionAngle < 360 );
	}

	/// CurveInfoCreator::createShape()�̈ꕔ
	namespace create_shape
	{
		/**
		 * @brief Shape�t�@�C���̃X�L�[�}���`���AFieldIndexMap��Ԃ�
		 * @param[in,out]	crCDBFHandle	DBF�n���h���N���X�̃C���X�^���X
		 * @return	FieldIndexMap
		 */
		FieldIndexMap fieldIndexMap( CDBFHandle& crDBFHandle )
		{
			// TODO: �t�B�[���h�̌���������
			// TODO: �t�B�[���h�̕��я���Boost.Assign�̎d�l�Ɉˑ�����̂͊댯�Ȃ̂ŁA�v�C���B
			return boost::assign::map_list_of	// map_list_of�͉�������߂����i�炵���j�̂ŁA���ɏ������t�B�[���h�قǐ�
				( tstring( schema::curve_line::kWayToRepPoint ),	crDBFHandle.add_Field( CT2A( schema::curve_line::kWayToRepPoint ), FTDouble, 9, 2 ) )
				( tstring( schema::curve_line::kRepLinkID ),		crDBFHandle.add_Field( CT2A( schema::curve_line::kRepLinkID ), FTInteger, 9, 0 ) )
				( tstring( schema::curve_line::kROC ),				crDBFHandle.add_Field( CT2A( schema::curve_line::kROC ), FTInteger, 9, 0 ) )
			;
		}
	} // namespace create_shape

	bool CurveInfoCreator::isTargetMesh( long cMeshCode ) const
	{
		return m_BaseMeshList.find( cMeshCode ) != m_BaseMeshList.end();
	}

	void CurveInfoCreator::createShape() const
	{
		using namespace create_shape;

		// Shape�t�@�C���̃p�X��ݒ�
		path aFilePath = m_OutputDirPath / ( std::string("CRV") );
		CSHPHandle aSHPHandle;
		CDBFHandle aDBFHandle;
		aSHPHandle.create( aFilePath.string().c_str(), SHPT_ARC );
		aDBFHandle.create( aFilePath.string().c_str() );
		
		// Shape�t�@�C���̃X�L�[�}��ݒ�
		FieldIndexMap aFieldIndexMap = fieldIndexMap( aDBFHandle );

		// �S���b�V���̑S��Ԃɂ���
		BOOST_FOREACH (const SectionsByMesh::value_type& rElement, m_SectionsByMesh)
		{
			long aMeshCode = rElement.first;
			const Sections& rSections = rElement.second;

			// �o�͑Ώۃ��b�V���łȂ���΃X�L�b�v
			if (! isTargetMesh( aMeshCode ))
				{ continue; }

			// ��Ԃ��Ƃɏo��
			BOOST_FOREACH (const SectionPtr pSection, rSections)
			{
				createShapeSection( pSection, aSHPHandle, aDBFHandle, aFieldIndexMap, aMeshCode );
			}
		}
	}

	void CurveInfoCreator::createShape( long cMeshCode ) const
	{
		using namespace create_shape;

		// �o�͑Ώۃ��b�V���łȂ���ΏI��
		if (! isTargetMesh( cMeshCode ))
			{ return; }

		// ��Ԃ��P���Ȃ��ꍇ��Shape�t�@�C�����쐬���Ȃ��iBug9691�j
		SectionsByMesh::const_iterator it = m_SectionsByMesh.find( cMeshCode );
		if (it == m_SectionsByMesh.end())
		{
			notifier::asMeshWithNoSection( m_LogStream, cMeshCode );
			notifier::asMeshWithNoSection( std::cerr, cMeshCode );
			return;
		}

		// Shape�t�@�C���̃p�X��ݒ�
		path aMeshOutputDirPath = m_OutputDirPath;	///< ���Y���b�V���̏o�̓f�B���N�g���p�X
		std::string aMeshCode1 = boost::lexical_cast<std::string>( cMeshCode / 100 );	///< �P�����b�V���R�[�h
		std::string aMeshCode2 = boost::lexical_cast<std::string>( cMeshCode );			///< �Q�����b�V���R�[�h
		if (m_OutputOptions.m_SHP05DirConfig)
		{
			aMeshOutputDirPath /= path( aMeshCode1 ) / aMeshCode2;
			// �o�̓f�B���N�g�����Ȃ���΍쐬
			createDirectory( aMeshOutputDirPath );
		}
		path aFilePath = aMeshOutputDirPath / ( aMeshCode2 + "_CRV" );
		CSHPHandle aSHPHandle;
		CDBFHandle aDBFHandle;
		aSHPHandle.create( aFilePath.string().c_str(), SHPT_ARC );
		aDBFHandle.create( aFilePath.string().c_str() );

		// Shape�t�@�C���̃X�L�[�}��ݒ�
		FieldIndexMap aFieldIndexMap = fieldIndexMap( aDBFHandle );

		// �Ώۃ��b�V���̑S��Ԃ��o��
		const Sections& rSections = it->second;
		BOOST_FOREACH (const SectionPtr pSection, rSections)
		{
			createShapeSection( pSection, aSHPHandle, aDBFHandle, aFieldIndexMap, cMeshCode );
		}
	}

	void CurveInfoCreator::createShapeSection(
		const SectionPtr cpSection,
		CSHPHandle& crSHPHandle,
		CDBFHandle& crDBFHandle, 
		FieldIndexMap& crFieldIndexMap,
		long cMeshCode) const
	{
		// TODO: ���E�̃��[�v���P�ɂ������c�B

		ATLASSERT( cpSection->m_Points.size() != 0 );
		ATLASSERT( ! cpSection->m_Points.empty() );

		// �ȗ����a���u��Ԃ̍ő�ȗ����a�v�𒴂����Ԃ͍쐬���Ȃ��iBug9628�j
		if (m_OutputOptions.maxROCOfSection( cpSection->m_IsHighSpeedArea ) < cpSection->m_ROC)
			{ return; }

		// �ȗ����a�ɑ΂��ĒZ�������Ԃ͍쐬���Ȃ��iBug9638�j
		if (cpSection->m_FitRegionAngle < m_OutputOptions.m_SectionMinAngle)
			{ return; }

		// ���J�[�u�̏ꍇ�́A�i�[����Ă��鏇�ɓ_����쐬
		if (cpSection->isLeftCurve())
		{
			CSHPPointArray aPointArray;
			BOOST_FOREACH (const IPointPtr cpPoint, cpSection->m_Points)
			{
				aPointArray.push_back( CSHPPoint(
					cpPoint->GetX(),
					cpPoint->GetY()
				));
			}

			writeShape( crSHPHandle, crDBFHandle, crFieldIndexMap, aPointArray, *cpSection, cMeshCode );
		}
		// �E�J�[�u�̏ꍇ�́A�i�[����Ă��鏇�Ƃ͋t�ɓ_����쐬
		else
		{
			CSHPPointArray aPointArray;
			for (Section::Points::const_reverse_iterator itPoint = cpSection->m_Points.rbegin(); itPoint != cpSection->m_Points.rend(); ++itPoint)
			{
				aPointArray.push_back( CSHPPoint(
					(*itPoint)->GetX(),
					(*itPoint)->GetY()
				));
			}

			writeShape( crSHPHandle, crDBFHandle, crFieldIndexMap, aPointArray, *cpSection, cMeshCode );
		}
	}

	void CurveInfoCreator::convertMesh( long cMeshCode )
	{
		m_MeshCode = cMeshCode;

		std::cerr << "--------------------------------------------------" << std::endl;
		m_LogStream << "#--------------------------------------------------" << std::endl;
		std::cerr << "���b�V���R�[�h : " << cMeshCode << std::endl;
		m_LogStream << "# ���b�V���R�[�h : " << cMeshCode << std::endl;
		m_LogStream << "# ���b�V�������J�n���� : " << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time() ) << std::endl;

		std::string aProgressMsg = "�ǂݍ���";
		outputElapsedTime( aProgressMsg, true );

		using namespace sindy::schema;
		using namespace utility::sindylib_base;

		// �Ώۃ��b�V���̃f�[�^��ǂݍ���
		CSPTableFinder pTableFinder = m_pCacheManager->cache( cMeshCode, m_OutputOptions.m_QueryWhereClause );
		CSPTableContainer pLinkTable = pTableFinder->FindTable( road_link::kTableName );
		outputElapsedTime( aProgressMsg, false );
		// �����N����0���Ȃ�ʒm�iBug9697�j
		if (pLinkTable->empty())
		{
			notifier::asLinkNotFound( std::cerr, cMeshCode );
			notifier::asLinkNotFound( m_LogStream, cMeshCode );
		}

		long aLinkCount = 0;

		// �ǂݍ��񂾃����N�̂��ꂼ��ɂ���
		BOOST_FOREACH (CSPContainerBase pLink, *pLinkTable)
		{
			ATLASSERT( pLink );
			IFeaturePtr ipLink = irow( pLink );
			long aLinkID = ipLink->OID;

			// ��ԎQ�Ƃ��L�^�i�ŏ��ɒ��ڂ����P�����N�̂݁j
			setSpatialReferenceBy( ipLink );

			// �i���\��
			std::cerr << "Road_Link " << ++aLinkCount << " / " << pLinkTable->size() << "\r";

			// �����ς݃����N�Ȃ�X�L�b�v
			if (m_ProcessedLinkIDs.find( aLinkID ) != m_ProcessedLinkIDs.end())
				{ continue; }
			m_ProcessedLinkIDs.insert( aLinkID );

			// �����N���Ώۃ����N�̏����𖞂����Ȃ���΃X�L�b�v
			if (! isTargetLink( ipLink ))
				{ continue; }

			if (m_OutputOptions.m_LogHandlingProcess)
			{
				m_LogStream << CStringA( sindy::schema::road_link::kTableName ) << fd << aLinkID << fd << fd << fd << "���̃����N���܂ވ�{���̏����J�n" << std::endl;
			}

			// �Ώۃ����N���܂ވ�{�����擾
			SingleRoadPtr pSingleRoad = getSingleRoad( ipLink );		// TODO: �ł���Ȃ�const�Q�ƂɏC���B
			// ��{�����\������Z�O�����g����1�ȉ��Ȃ�X�L�b�v
			if (pSingleRoad->size() <= 1)
				{ continue; }

			// ��{����ϋȓ_�ŕ������ADSR�̗�ɂ���
			DirectionalSRArray aDirectionalSRArray = divideSingleRoad( pSingleRoad );

			// DSR���Ƃɏ���
			BOOST_FOREACH (const DirectionalSRPtr pDirectionalSR, aDirectionalSRArray)
			{
				// �u�_�Ƌȗ����a�v�̗�̏����쐬
				DirectionalSRInfo aDirectionalSRInfo( pSingleRoad, pDirectionalSR, createROCPointInfos( *pDirectionalSR ) );

				// ��Ԃ��쐬
				createSections( aDirectionalSRInfo );
			}
		}
		std::cerr << std::endl;

		// �K�v�Ȃ烍�O�ɃL���b�V���̏�Ԃ��o��
		if (m_OutputOptions.m_LogHandlingProcess)
		{
			m_LogStream << "# �L���b�V�����݃��b�V�� :";
			BOOST_FOREACH (long aMeshCode, m_pCacheManager->cachedMeshCodes())
			{
				m_LogStream << " " << aMeshCode;
			}
			m_LogStream << std::endl;
		}

		std::cerr << "���b�V���R�[�h : " << cMeshCode << " �I��" << std::endl;
		m_LogStream << "# ���b�V���R�[�h : " << cMeshCode << " �I��"  << std::endl;
		m_LogStream << "# ���b�V�������I������ : " << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time() ) << std::endl;
	}
} // namespace curve_info

