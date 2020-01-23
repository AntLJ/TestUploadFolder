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
#include <boost/logic/tribool.hpp>
#include <shapelib/SHPHandle.h>
#include <shapelib/DBFHandle.h>
#include "OutputOptions.h"
#include "CurveInfoData.h"
#include "AngleAccumulating.h"
#include "CacheManager.h"

namespace curve_info
{
	using namespace boost::filesystem;
	using namespace adlib;

	/**
	 * @brief �J�[�u���쐬�N���X
	 * @note	�o�͍ς݃��b�V���̋�ԌQ�͉�����Ă��悢���A�f�[�^�ʂ����قǑ傫���Ȃ��̂ō��̂Ƃ���͉�����Ă��Ȃ��B
	 *			
	 */
	class CurveInfoCreator
	{
	public:
	// ���J��`
		typedef std::vector< SectionPtr > Sections;				///< ��ԌQ
		typedef std::map< MeshCode, Sections > SectionsByMesh;	///< ���b�V�����̋�ԌQ

	private:
	// ����J�ϐ�
		// �O������^����ꂽ���
		MeshCodes				m_BaseMeshList;					///< �o�͑ΏۂQ�����b�V�����X�g
		path					m_OutputDirPath;				///< Shape�t�@�C���o�̓f�B���N�g���p�X
		mutable std::ofstream	m_LogStream;					///< ���O�o�̓X�g���[��
		mutable std::ofstream	m_VerificationLogStream;		///< ���ؗp���O�o�̓X�g���[��
		OutputOptions			m_OutputOptions;				///< �o�̓I�v�V����

		// ���ʕ�
		SectionsByMesh m_SectionsByMesh;						///< ���b�V�����̋�ԌQ

		// ����p�ϐ�
		long m_SuccessfulMeshCount;								///< ����I�����b�V�����J�E���^
		CacheManagerPtr m_pCacheManager;						///< �L���b�V���}�l�[�W��
		long m_MeshCode;										///< �������̃��b�V���R�[�h
		LinkIDs m_ProcessedLinkIDs;								///< �����ς݃����NID�W��
		ISpatialReferencePtr m_pSpRef;						///< ��ԎQ�Ɓi�ŏ��ɒ��ڂ��������N����擾����j

	public:
	// ���J�֐�
		/**
		 * @brief �R���X�g���N�^
		 * @param[in]	cOutputOptions	�o�̓I�v�V����
		 */
		CurveInfoCreator( OutputOptions cOutputOptions ) :
			m_OutputOptions( cOutputOptions ),
			m_SuccessfulMeshCount( 0 ),
			m_MeshCode( 0 )
		{}

		/// �f�X�g���N�^
		~CurveInfoCreator(void) {}

		/**
		 * @brief ������
		 * @note	�R���X�g���N�^�̒���ɕK���Ă΂�邱�Ƃ��O��
		 * @param[in]	cRoadDB						���H�f�[�^�x�[�X
		 * @param[in]	cMeshDB						�Q�����b�V���f�[�^�x�[�X
		 * @param[in]	cBaseMeshListFilePath		�o�͑ΏۂQ�����b�V���R�[�h�̃��X�g�̃t�@�C���p�X
		 * @param[in]	cOutputDirPath				Shape�t�@�C���o�̓f�B���N�g���̃p�X
		 * @param[in]	cLogFilePath				���O�t�@�C���̃p�X
		 * @param[in]	cVerificationLogFilePath	���ؗp���O�t�@�C���̃p�X
		 */
		void init(
			tstring cRoadDB,
			tstring cMeshDB,
			path cBaseMeshListFilePath,
			path cOutputDirPath,
			path cLogFilePath,
			path cVerificationLogFilePath
		);

		/**
		 * @brief �f�[�^�ϊ������s����
		 * @return	�S�Ẵ��b�V���̏���������ɏI���������ۂ�
		 */
		bool convert();

	private:
	// ����J�֐�
		/// �������̃��b�V���̃L���b�V����Ԃ�
		const CSPTableFinder currentMeshCache() { return m_pCacheManager->cache( m_MeshCode, m_OutputOptions.m_QueryWhereClause ); }

		/**
		 * @brief �Q�����b�V���P�ʂ̕ϊ�
		 * @param[in]	cMeshCode	�Ώۃ��b�V���R�[�h
		 */
		void convertMesh( long cMeshCode );

		/**
		 * @brief ���s���̏����o��
		 * @param[in]	co			�o�̓X�g���[��
		 * @param[in]	cRoadDB		���H�f�[�^�x�[�X
		 * @param[in]	cMeshDB		�Q�����b�V���f�[�^�x�[�X
		 */
		void outputExecInfo( std::ostream& co, tstring cRoadDB, tstring cMeshDB ) const;

		/**
		 * @brief �o�ߎ��Ԃ�std::cerr��m_LogStream�ɏo�͂���
		 * @param[in]	cTask		�^�X�N��
		 * @param[in]	cIsStart	�^�X�N�̊J�n���I�����itrue�F�J�n�Afalse�F�I���j
		 */
		void outputElapsedTime( std::string cTask, bool cIsStart );

		/**
		 * @brief �Ώۃ����N�������Ώۃ����N�ł��邩�ۂ��𔻒肷��
		 * @note	[Bug9694] �u�׊X�H��ΏۊO�ɂ���v�Ƃ����d�l��p�~�B
		 * @param[in]	cpLink	�Ώۃ����N
		 * @return	���茋��
		 */
		bool isTargetLink( IFeaturePtr cpLink );

		/**
		 * @brief �Ώۃ����N���܂ވ�{�����擾����
		 * @note	�܂�cpLink��crSingleRoad�ɒǉ����A���̗��[�ɂ��郊���N�Q�̒ǉ��������Ăяo��
		 * @param[in]	cpLink			�Ώۃ����N�iisTargetLink()��true�Ɣ��肳��Ă��鎖���O��j
		 * @return	��{��
		 */
		SingleRoadPtr getSingleRoad( IFeaturePtr cpLink );

			/**
			 * @brief �Ώۃ����N��cNodeID�̃m�[�h���ɂ����{���̃����N�Q���\������Z�O�����g�Q��crSingleRoad��cIsFront���ɍċA�I�ɒǉ�����
			 * @param[in]		cpLink			�Ώۃ����N
			 * @param[in]		cNodeID			�Ώۃm�[�hID
			 * @param[in]		cIsBack			�擾������{�������N�Q��ǉ�����̂́AcrSingleRoad��Back�����ہiFront���j��
			 * @param[in]		cMeshCode		�Ώۃ����N�����݂���Q�����b�V���̃��b�V���R�[�h
			 * @param[in,out]	crSingleRoad	�擾������{����Ԃ��ϐ�
			 */
			void getOneSideSingleRoad( IFeaturePtr cpLink, long cNodeID, bool cIsBack, long cMeshCode, SingleRoad& crSingleRoad );

			/**
			 * @brief �������u�J�[�u�����������̍ő�l�v�ȏ�ł��邩�ۂ��𔻒肷��
			 * @param[in]	�Ώے���
			 * @return	���茋��
			 */
			bool isLongStraight( double cLength ) const;

		/**
		 * @brief �p�x�ݐς̕��������ł��邩�ہi�E�ł���j���𔻒肷��
		 * @note	�p�x�ݐς̕��������E�ǂ��炩�݂̂ł��鎖��O��Ƃ��A�����łȂ��ꍇ�͗�O�𑗏o����B
		 * @param[in]	cDirection	�p�x�ݐς̕���
		 * @return	���茋��
		 */
		bool CurveInfoCreator::isLeft( angle_accumulating::curve_direction::ECode cDirection ) const;

		/**
		 * @brief ��{����ϋȓ_�ŕ������ADSR��ɂ��ĕԂ�
		 * @note	�����Ō����ϋȓ_�Ƃ́A�Ȑ��ł͂Ȃ��|�����C���`�󂩂�A�������Ȃ���ӏ��̂��ƁB
		 * @note	�ϋȓ_������ƍl������Z�O�����g�����̒��_�ŕ������邽�߁A���z�Z�O�����g���ł���B
		 * @param[in]	cpSingleRoad	��{��
		 * @return	DSR��
		 */
		DirectionalSRArray divideSingleRoad( SingleRoadPtr cpSingleRoad ) const;

		/**
		 * @brief ��{����𓙊Ԋu�ȓ_�Q�ŋ�؂�A�e�_�̋ȗ����a�̏����쐬����
		 * @return	�u�_�Ƌȗ����a�v�̗�
		 */
		ROCPointInfosPtr createROCPointInfos( const DirectionalSR& crDirectionalSR );

			/**
			 * @brief �^����ꂽ�|�����C���Ɋ܂܂�钷���Z�O�����g�ɍ\���_��ǉ�����
			 * @param[in,out]	cpPolyline	�Ώۃ|�����C��
			 */
			void addPointsOnLongSegment( IPolyline4Ptr cpPolyline ) const;

		/**
		 * @brief ��Ԃ��쐬����
		 * @param[in]	crDirectionalSRInfo	DSR���
		 */
		void createSections( const DirectionalSRInfo& crDirectionalSRInfo );

		/**
		 * @brief �|�����C����
		 * @note	�����N�`��̂����A�x�W�G�Ȑ���́u�ȗ����a�����߂�_�v��ɑΉ����镔�������߁A�������ԂƂ���B
		 * @param[in]	crDirectionalSRInfo	DSR���
		 */
		void polylinize( const DirectionalSRInfo& crDirectionalSRInfo );

			/**
			 * @brief �Ώۋ�ԏ��̂����A��\�_�Ɋւ������ݒ肷��
			 * @param[in]		crSingleRoad		���ڒ��̈�{��
			 * @param[in]		crSectionCandidate	���ڒ��̋�Ԍ��
			 * @param[in,out]	cpSection			�Ώۋ�ԏ��
			 */
			void setRepPointInfo(
				const SingleRoad& crSingleRoad,
				const SectionCandidate& crSectionCandidate,
				SectionPtr cpSection ) const;

		/**
		 * @brief �w�肳�ꂽ���b�V�����o�͑Ώۃ��b�V���R�[�h�ł��邩�ۂ��𔻒肷��
		 * @param[in]	cMeshCode	�ΏۂQ�����b�V���R�[�h
		 * @return	���茋��
		 */
		bool isTargetMesh( long cMeshCode ) const;

		/**
		 * @brief �S���b�V���̃f�[�^���P�g��Shape�t�@�C���Ƃ��ďo�͂���
		 */
		void createShape() const;

		/**
		 * @brief �w�肳�ꂽ���b�V����Shape�t�@�C�����o�͂���
		 * @param[in]	cMeshCode	�ΏۂQ�����b�V���R�[�h
		 */
		void createShape( long cMeshCode ) const;

			/**
			 * @brief �w�肳�ꂽ��Ԃ̏���Shape�t�@�C���ɏo�͂���
			 * @param[in]		cpSection		�Ώۋ��
			 * @param[in,out]	crSHPHandle		Shape�n���h���N���X�̃C���X�^���X
			 * @param[in,out]	crDBFHandle		DBF�n���h���N���X�̃C���X�^���X
			 * @param[in]		crFieldIndexMap	�t�B�[���h���ƃC���f�b�N�X�̑Ή��\
			 * @param[in]		cMeshCode		�ΏۂQ�����b�V���R�[�h
			 */
			void createShapeSection(
				const SectionPtr cpSection,
				CSHPHandle& crSHPHandle,
				CDBFHandle& crDBFHandle, 
				FieldIndexMap& crFieldIndexMap,
				long cMeshCode) const;

			/**
			 * @brief �_��Ƒ�����Shape/dbf�n���h���N���X�̃C���X�^���X�ɏ�������
			 * @note	createShape()�̏��E�t���ʂ̏���
			 * @param[in,out]	crCSHPHandle	Shape�n���h���N���X�̃C���X�^���X
			 * @param[in,out]	crCDBFHandle	DBF�n���h���N���X�̃C���X�^���X
			 * @param[in]		crFieldIndexMap	�t�B�[���h���ƃC���f�b�N�X�̑Ή��\
			 * @param[in]		crPointArray	�_��
			 * @param[in]		crSection		�Ώ�Section
			 * @param[in]		cMeshCode		�ΏۂQ�����b�V���R�[�h�i���ؗp���O�ւ̏o�͗p�j
			 */
			void writeShape(
				CSHPHandle& crCSHPHandle,
				CDBFHandle& crCDBFHandle,
				FieldIndexMap& crFieldIndexMap,
				const CSHPPointArray& crPointArray,
				const Section& crSection,
				long cMeshCode) const;

			/**
			 * @brief ��ԌQ�R���e�i�ɋ�Ԃ�ǉ�����
			 * @note	��Ԃ��������郁�b�V���̋�ԌQ�ɒǉ�����B
			 * @param[in]	cpSection
			 */
			void addSection( SectionPtr cpSection );

		/**
		 * @brief ��ԎQ�Ƃ��܂��L�^���Ă��Ȃ���΁A�w�肳�ꂽ�t�B�[�`������擾���ċL�^����
		 * @param[in]	cpLink	�擾���t�B�[�`��
		 */
		void setSpatialReferenceBy( const IFeaturePtr cpLink )
		{
			if (! m_pSpRef)
			{
				m_pSpRef = cpLink->GetShape()->GetSpatialReference();
			}
		}
	};

} // namespace curve_info
