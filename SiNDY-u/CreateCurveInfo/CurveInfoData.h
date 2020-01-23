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
 * @brief �{�c�[���Ǝ��̃f�[�^�Q�̒�`
 */
#pragma once

#include "adlib.h"
#include "CircleFitEvaluator.h"

namespace curve_info
{
	using namespace adlib;

	/**
	 * @brief �Z�O�����g�i�����j
	 * @note	Segment�R���e�i���ł̌����𑵂��邽�߁A�擾���iISegmentPtr�j�̌����Ƃ͖��֌W�Ɏn�I�_�������ł�����̂Ƃ���B
	 *			�u���z�Z�O�����g�v�iSDE�ɂ͑��݂��Ȃ����A�n�I�_������ݒ肵�ēƎ��ɍl�������́j�������B
	 */
	struct Segment
	{
		long m_LinkID;			///< �������郊���N�̃I�u�W�F�N�gID�i���z�Z�O�����g�̏ꍇ��0�j
		long m_Index;			///< �������郊���N���ł̃C���f�b�N�X�i���z�Z�O�����g�̏ꍇ��0�j
		IPointPtr m_pFromPoint;	///< �Z�O�����g�̎n�_�i�擾����FromPoint�܂���ToPoint�j
		IPointPtr m_pToPoint;	///< �Z�O�����g�̏I�_�i�擾����FromPoint�܂���ToPoint�j
		bool m_IsHighSpeedArea;		///< �������郊���N��������ł��邩�ۂ��i���z�Z�O�����g�̏ꍇ��false�j

		/**
		 * @brief �R���X�g���N�^
		 * @param[in]	cLinkID				�������郊���N�̃I�u�W�F�N�gID
		 * @param[in]	cIndex				�������郊���N���ł̃C���f�b�N�X
		 * @param[in]	cpFromPoint			�Z�O�����g�̎n�_�i�擾����FromPoint�܂���ToPoint�j
		 * @param[in]	cpToPoint			�Z�O�����g�̏I�_�i�擾����FromPoint�܂���ToPoint�j
		 * @param[in]	cIsHighSpeedArea	�������郊���N��������ł��邩�ۂ�
		 */
		Segment( long cLinkID, long cIndex, IPointPtr cpFromPoint, IPointPtr cpToPoint, bool cIsHighSpeedArea ) :
		m_LinkID( cLinkID ), m_Index( cIndex ), m_pFromPoint( cpFromPoint ), m_pToPoint( cpToPoint ), m_IsHighSpeedArea( cIsHighSpeedArea )
		{}

		/**
		 * @brief �R���X�g���N�^�i���z�Z�O�����g�p�j
		 * @param[in]	cpFromPoint	�Z�O�����g�̎n�_�i�擾����FromPoint�܂���ToPoint�j
		 * @param[in]	cpToPoint	�Z�O�����g�̏I�_�i�擾����FromPoint�܂���ToPoint�j
		 * @note	���z�Z�O�����g��m_IsHighSpeedArea�̒l�͈Ӗ��������Ȃ��̂ŁA�����ł�false�Ƃ��Ă���B
		 *			TODO: �����̐݌v�͊ԈႢ�̌��ɂȂ肻���Ȃ̂ŁA�v�C���B
		 *			�n�I�_�ȊO�̏���S�ă|�C���^�ɂ���Ȃǂ��āA���z�Z�O�����g�̏ꍇ�͖����l�ł��鎖�𖾎����ׂ��B
		 *			�āF�n�I�_�ȊO�̏���ʂ̍\���̂̃����o�ɂ��āA����ւ̃|�C���^��Segment�̃����o�Ƃ���B
		 */
		Segment( IPointPtr cpFromPoint, IPointPtr cpToPoint ) :
		m_LinkID( 0 ), m_Index( 0 ), m_pFromPoint( cpFromPoint ), m_pToPoint( cpToPoint ), m_IsHighSpeedArea( false )
		{}

		/**
		 * @brief �Z�O�����g�̒�����Ԃ�
		 * @return	�Z�O�����g�̒����i�x�j
		 */
		double length() const;

		/**
		 * @brief �Z�O�����g���L���ł��邩�ۂ���Ԃ�
		 * @note	�n�I�_������i������0�j�̃Z�O�����g�͖����Ƃ���B
		 * @return	���茋��
		 */
		bool valid() const;
	};
	typedef std::tr1::shared_ptr<Segment> SegmentPtr;
	typedef std::deque<SegmentPtr> Segments;	// std::list��iterator��boost::prior���g���Ȃ��̂ŁAdeque�ɕύX�B
	typedef Segments::const_iterator SegmentIt;

	typedef std::vector<LinkID> LinkIDArray;	///< �����NID��i�����t�������NID�Q�j

	/// ����ʍs�K���̎�ށi��{���̍\�������N�Q�Ɉ���ʍs�K�������݂��邩�ۂ��ƁA���̕����j
	namespace oneway_type
	{
		enum ECode
		{
			none,	///< �K���Ȃ�
			fore,	///< ��{���ɑ΂��ď�����
			back,	///< ��{���ɑ΂��ċt����
		};
	}

	/**
	 * @brief ��{���i����̂Ȃ��A�������Z�O�����g�̗�j
	 * @note	��{���̕����́A�ŏ��ɒǉ����������N��From�m�[�h����To�m�[�h�֌����������Ƃ���B
	 * @note	�\�������N�Q�͑S�āu�����悩�ۂ��v�������łȂ���΂Ȃ�Ȃ����̂Ƃ���B�iBug9619�j
	 * @note	�\�������N�Q�͑S�āu����ʍs�K���̗L���ƕ����v�������łȂ���΂Ȃ�Ȃ����̂Ƃ���B�iBug9642�j
	 */
	struct SingleRoad : public Segments
	{
		typedef double Ratio;							///< �n�_��0�A�I�_��1�Ƃ������́A��{����̈ʒu
		typedef std::pair< Ratio, Ratio > Range;		///< ��{����͈̔�

		bool m_IsHighSpeedArea;							///< �����悩�ۂ�
		oneway_type::ECode m_Oneway;					///< �\�������N�Q�Ɉ���ʍs�K�������݂��邩�ۂ��ƁA���̕���
		ISpatialReferencePtr m_pSpRef;					///< �g�p�����ԎQ��

	// �����L���b�V��
		mutable double m_Length;					///< ��{���̒���
		typedef std::map< Ratio, SegmentIt > RatioMap;
		mutable RatioMap m_RatioMap;				///< �Z�O�����g�J�n�ʒu�ƃZ�O�����g�̑Ή��\
		mutable IGeometryPtr m_Geometry;			///< �`��

		/// �R���X�g���N�^
		SingleRoad(
			bool cIsHighSpeedArea,
			oneway_type::ECode cOneway,
			ISpatialReferencePtr cpSpatialReference
		) :
			m_IsHighSpeedArea( cIsHighSpeedArea ),
			m_Oneway( cOneway ),
			m_Length( 0 ),
			m_pSpRef( cpSpatialReference )
		{}

		/**
		 * @brief �Ώۃ����N�̃Z�O�����g�Q�����g�̃R���e�i�ɒǉ�����
		 * @param[in]	cpLink			�Ώۃ����N
		 * @param[in]	cStartIsFrom	�Z�O�����g�Q��Ώۃ����N��From�m�[�h������ǉ����邩�ہiTo�m�[�h������ǉ�����j��
		 * @param[in]	cIsBack			�Z�O�����g�Q���R���e�i��Back���ɒǉ����邩�ہiFront���ɒǉ�����j��
		 */
		void addLink( IFeaturePtr cpLink, bool cStartIsFrom, bool cIsBack );

		/**
		 * @brief ��{�����\�����郊���NID���Ԃ�
		 * @return	�����NID��
		 */
		LinkIDArray linkIDArray() const;

		 /**
		  * @brief ��{���̒�����Ԃ�
		  * @note	����Ăяo�����̂݁A�Z�o�̃R�X�g�������B
		  * @return	�����i�x�j
		  */
		double length() const;

		/**
		 * @brief �Z�O�����g�J�n�ʒu�ƃZ�O�����g�̑Ή��\��Ԃ�
		 * @note	����Ăяo�����̂݁A�Z�o�̃R�X�g�������B
		 * @return	�Ή��\
		 */
		const RatioMap& ratioMap() const;

		/**
		 * @brief �w�肵���ʒu���܂ރZ�O�����g��Ԃ�
		 * @param[in]	cRatio	�ʒu
		 * @return	�Z�O�����g
		 */
		SegmentIt segment( Ratio cRatio ) const;

		/**
		 * @brief �w�肵���͈͂��܂���悤�ȃZ�O�����g�Q��Ԃ�
		 * @param[in]	cRange	�͈�
		 * @return	�Z�O�����g�Q
		 */
		Segments segments( Range cRange ) const;

		/**
		 * @brief ��{���S�̂̌`���Ԃ�
		 * @note	����Ăяo�����̂݁A�Z�o�̃R�X�g�������B
		 * @return	�`��
		 */
		IGeometryPtr geometry() const;

		/**
		 * @brief �Z�O�����g��ǉ�����
		 * @note	push_back()�Ƃ̈Ⴂ�́A�u�����ȃZ�O�����g�Ȃ�ǉ����Ȃ��v���B
		 * @param[in]	cpSegment	�ΏۃZ�O�����g
		 */
		void add( SegmentPtr cpSegment );
	};
	typedef std::tr1::shared_ptr< SingleRoad > SingleRoadPtr;

	/**
	 * @brief �����t����{���iDirectional Single Road = DSR�j
	 * @note	�ϋȓ_�ɂ����Ĉ�{������������鎖�ɂ��쐬�����B
	 *			�u�����t���v�Ƃ����̂́A�u���v�u�E�v�̂����ꂩ�ɕ��ނł��A�^�������ł�������A���E�����ɋȂ����Ă����肷�鎖�͂Ȃ��Ƃ����Ӗ��B
	 */
	struct DirectionalSR : public SingleRoad
	{
		bool m_IsLeftCurve;			///< ���J�[�u���ۂ�
		SingleRoad::Range m_Range;	///< ���̈�{���̂����A���̃f�[�^�ɊY������͈�

		/// �R���X�g���N�^
		DirectionalSR(
			bool cIsLeftCurve,
			SingleRoad::Range cRange,
			bool cIsHighSpeedArea,
			oneway_type::ECode cOneway,
			ISpatialReferencePtr cpSpRef
		) :
			m_IsLeftCurve( cIsLeftCurve ),
			m_Range( cRange ),
			SingleRoad( cIsHighSpeedArea, cOneway, cpSpRef )
		{}
	};
	typedef std::tr1::shared_ptr< DirectionalSR > DirectionalSRPtr;
	typedef std::vector< DirectionalSRPtr > DirectionalSRArray;

	/**
	 * @brief ���
	 * @note	�P�̃J�[�u��\�����B
	 */
	struct Section	// TODO: �N���X��
	{
	public:
	// ���J��`
		typedef std::vector<IPointPtr> Points;

//	private:
	// ����J�ϐ�
		//----- �o�͂̂��߂̕ϐ� -----//
		double	m_ROC;						///< �ȗ����a�im�j
		long	m_DistanceOf3P;				///< �ȗ����a�����肷��ۂɍ̗p���ꂽ�u�R�_�̊Ԋu�v�im�j
		long	m_RepLinkID;				///< ��\�_���݃����NID
		double	m_WayToRepPoint;			///< ��\�_�ʒu
		Points	m_Points;					///< �_��i�n�I�_�ȊO�̓����N�̍\���_�j	TODO: �n�I�_�����ʂɎ����Ă��悢�����H
		bool	m_IsHighSpeedArea;			///< �����悩�ۂ�

		//----- �f�o�b�O�p�̎Q�l��� -----//
		IPointPtr m_pRepPoint;				///< ��\�_

		//----- ���������p�ϐ� -----//
		bool	m_IsLeftCurve;			///< ���J�[�u���ۂ��i�t�H�[�}�b�g��͓_��̏����ŕ\�����A�v���O������ł͕ێ����A�_��̏�����SR�Ɠ����ɂ��Ă����j
		double	m_FitRegionAngle;		///< CircleFitEvaluator�ɂ��]���ŁA�t�B�b�g�̈悪�~���̂����ǂꂾ���̊p�x���߂Ă������iBug9638�Œǉ��j

	// ���J�֐�
		/// �R���X�g���N�^
		Section(
			bool cIsLeftCurve,
			bool cIsHighSpeedArea
		) :
			m_ROC( 0 ),
			m_DistanceOf3P( 0 ),
			m_RepLinkID( 0 ),
			m_WayToRepPoint( 0 ),
			m_IsLeftCurve( cIsLeftCurve ),
			m_IsHighSpeedArea( cIsHighSpeedArea ),
			m_FitRegionAngle( 0 )
		{}

		/**
		 * @brief ���J�[�u���ۂ���Ԃ�
		 * @return	���J�[�u���ۂ�
		 */
		bool isLeftCurve() const { return m_IsLeftCurve; }

		/**
		 * @brief �ȗ����a��ύX����
		 * @param[in]	cROC	�ȗ����a
		 */
		void setROC( double cROC ) { m_ROC = cROC; }

		/**
		 * @brief �ȗ����a��Ԃ�
		 * @return	�ȗ����a
		 */
		double ROC() const { return m_ROC; }
	};
	typedef std::tr1::shared_ptr<Section> SectionPtr;

	typedef tstring FieldName;
	typedef long FieldIndex;
	typedef std::map<FieldName, FieldIndex> FieldIndexMap;	///< �t�B�[���h���ƃC���f�b�N�X�̑Ή��\

	/**
	 * @brief �u�ȗ����a���Z�o����_�v�̏��
	 */
	struct ROCPointInfo
	{
		IPointPtr							m_pPoint;		///< �_
		SingleRoad::Ratio					m_Ratio;		///< ����SingleRoad��̈ʒu
		circle_fit_evaluator::ResultsPtr	m_pCFEResults;	///< CircleFitEvaluator�ɂ��]������

		 /// �R���X�g���N�^
		ROCPointInfo(
			IPointPtr cpPoint,
			circle_fit_evaluator::ResultsPtr cpCFEResults,
			SingleRoad::Ratio cRatio
		) : 
			m_pPoint( cpPoint ),
			m_pCFEResults( cpCFEResults ),
			m_Ratio( cRatio )
		{}
	};
	typedef std::tr1::shared_ptr< ROCPointInfo > ROCPointInfoPtr;

	/**
	 * @brief ROCPointInfo�̏W��
	 * @note	�_�ɂ�錟�����\�ɂ��邽�߁Atypedef�ł͂Ȃ��\���̂Ƃ��Ē�`�B
	 */
	struct ROCPointInfos : public std::vector< ROCPointInfoPtr >
	{
		typedef std::pair< double, double > LonLat;	///< �o�ܓx1
		typedef std::map< LonLat, ROCPointInfoPtr > LonLatMap;
		mutable LonLatMap m_pLonLatMap;	///< �_�ɂ�錟���̂��߂̃e�[�u��

		/**
		 * @brief �w�肳�ꂽ�_�ɑΉ�����v�f��Ԃ�
		 * @param[in]	cpPoint	�Ώۓ_
		 * @return	�v�f�i������Ȃ����NULL�j
		 */
		const ROCPointInfoPtr searchByPoint( IPointPtr cpPoint ) const
		{
			if (m_pLonLatMap.empty())
			{
				// �����p�e�[�u�����쐬����Ă��Ȃ���΍쐬����
				const ROCPointInfos& rROCPointInfos = *this;
				BOOST_FOREACH (const ROCPointInfoPtr pROCPointInfo, rROCPointInfos)
				{
					ATLASSERT( pROCPointInfo );
					m_pLonLatMap.insert( std::make_pair(
						std::make_pair( pROCPointInfo->m_pPoint->GetX(), pROCPointInfo->m_pPoint->GetY() ),
						pROCPointInfo
					));
				}
			}

			LonLatMap::const_iterator it = m_pLonLatMap.find( std::make_pair( cpPoint->GetX(), cpPoint->GetY() ) );
			return it != m_pLonLatMap.end() ? it->second : ROCPointInfoPtr();
		}
	};
	typedef std::tr1::shared_ptr< ROCPointInfos > ROCPointInfosPtr;

	/**
	 * @brief DSR���i�����t����{���Ƃ���Ɋ֘A������j
	 */
	// TODO: �ł���΃����o��S�ĎQ�Ƃɂ���
	struct DirectionalSRInfo
	{
		SingleRoadPtr		m_pSingleRoad;		///< ���̈�{��
		DirectionalSRPtr	m_pDirectionalSR;	///< �����t����{��
		ROCPointInfosPtr	m_pROCPointInfos;	///< ��{���ɑΉ�����ROCPointInfo��

		/// �R���X�g���N�^
		DirectionalSRInfo(
			SingleRoadPtr cpSingleRoad,
			DirectionalSRPtr cpDirectionalSR,
			ROCPointInfosPtr m_pROCPointInfos
		) :
			m_pSingleRoad( cpSingleRoad ),
			m_pDirectionalSR( cpDirectionalSR ),
			m_pROCPointInfos( m_pROCPointInfos )
		{}
	};

	/**
	 * @brief �_�̍��W
	 */
	struct Point
	{
		Point() : x(0), y(0) {}
		Point( double cX, double cY ) : x(cX), y(cY) {}

		double x;	///< X���W�i�x�j
		double y;	///< Y���W�i�x�j
	};
	typedef std::vector<Point> Points;

	/// ��Ԍ��
	struct SectionCandidate
	{
		SingleRoad::Range m_Range;					///< �͈�
		ROCPointInfos m_ROCPointInfos;				///< �͈͓��̓_����
		DirectionalSRInfo m_rDirectionalSRInfo;		///< ���̋�Ԍ�₪�܂܂��DSR

		/// �R���X�g���N�^
		SectionCandidate(
			DirectionalSRInfo crDirectionalSRInfo
		) :
			m_Range( std::make_pair( 0, 0 ) ),
			m_rDirectionalSRInfo( crDirectionalSRInfo )
		{}

		/// �R�s�[�R���X�g���N�^
		SectionCandidate(
			const SectionCandidate& rSectionCandidate
		) :
			m_Range( rSectionCandidate.m_Range ),
			m_ROCPointInfos( rSectionCandidate.m_ROCPointInfos ),
			m_rDirectionalSRInfo( rSectionCandidate.m_rDirectionalSRInfo )
		{}

	};
	typedef std::vector< SectionCandidate > SectionCandidates;
	typedef std::tr1::shared_ptr< SectionCandidate > SectionCandidatePtr;

} // namespace curve_info

