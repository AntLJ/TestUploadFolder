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

#include "CurveInfoData.h"

namespace sindy
{
	class CRoadLinkTable;
	class CRowContainer;
	typedef boost::shared_ptr< CRowContainer > CSPRowContainer;
	class CRoadLinkTable;
	typedef boost::shared_ptr< CRoadLinkTable > CSPRoadLinkTable;
}

namespace curve_info
{
	/// �J�[�u���֘A�̃��[�e�B���e�B
	namespace utility
	{
		static std::string fd = "\t";		///< �t�B�[���h�f���~�^

		/**
		 * @brief �Q�Z�O�����g�̂Ȃ��p�̊p�x��Ԃ�
		 * @param[in]	crSegment1	�ΏۃZ�O�����g1
		 * @param[in]	crSegment2	�ΏۃZ�O�����g2
		 * @return	�p�x�i�͈͂�(-180,180]�A�����v��肪���j
		 */
		double angle( const Segment& crSegment1, const Segment& crSegment2 );

#if 0	// ���g�p�����A�g�������Ȃ̂Ŏc���Ă���
		/**
		 * @brief �ΏۃZ�O�����g�̒��������[�g���P�ʂŕԂ�
		 * @param[in]	cpSegment	�ΏۃZ�O�����g
		 * @return	����(m)
		 */
		double meterLength( const SegmentPtr cpSegment );
#endif
		/// ���O�̃t�H�[�}�b�g�Ɉˑ����鏈���͂����ɏW�񂷂�
		namespace log_format
		{
			/// ���O�̃w�b�_�ɗ񋓂���t�B�[���h�����Ԃ�
			std::vector< std::string > fieldNames();

			std::string freeComment( std::string& crMessage );

			/**
			 * @brief ���O�̓_��񃌃R�[�h�̕�������쐬���ĕԂ�
			 * @param[in]	cLon		�o�x
			 * @param[in]	cLat		�ܓx
			 * @param[in]	cMessage	���b�Z�[�W
			 * @return	���R�[�h������
			 */
			std::string pointRecord( double cLon, double cLat, const std::string& crMessage );

			/**
			 * @brief ���O�̃����N��񃌃R�[�h�̕�������쐬���ĕԂ�
			 * @param[in]	cTable		�e�[�u����
			 * @param[in]	cOID		�I�u�W�F�N�gID
			 * @param[in]	cMessage	���b�Z�[�W
			 * @return	���R�[�h������
			 */
			std::string linkRecord( const std::string& cTable, long cOID, const std::string& crMessage );

		} // namespace log_record

		/**
		 * @brief �Q�_�Ԃ̋�����Ԃ�
		 * @note	�P�ɎO�����̒藝�ŎZ�o�������ʂ�Ԃ������Ȃ̂ŁA�����̒P�ʂ͖��Ȃ��B
		 */
		double distance( double x1, double y1, double x2, double y2 );

		/**
		 * @brief �����̒P�ʂ�x����m�ɕϊ����邽�߂Ɋ|����ׂ��W����Ԃ�
		 * @note	�w�肵���Q�_�Ԃ̋�����x��m�̂��ꂼ��ŎZ�o���A���̔�Ō��肷��B
		 * @param[in]	cLon1	�P�_�ڂ̌o�x
		 * @param[in]	cLat1	�P�_�ڂ̈ܓx
		 * @param[in]	cLon2	�Q�_�ڂ̌o�x
		 * @param[in]	cLat2	�Q�_�ڂ̈ܓx
		 */
		double degreeToMeterCoef( double cLon1, double cLat1, double cLon2, double cLat2 );

		/**
		 * @brief �~�ʂ��~�����߂�p�x��Ԃ�
		 * @note	�Ⴆ�Ή~�ʂ̒������~���Ɠ����Ȃ�A360���Ԃ�B
		 * @param[in]	cCircularArcLength	�~�ʂ̒���
		 * @param[in]	cRadius				�~�̔��a
		 * @return	�p�x�i�x�j
		 */
		double circularArcAngle( double cCircularArcLength, double cRadius );

		/**
		 * @brief �Ώۃ����N���ʂ�郊���N�ł��邩�ۂ��𔻒肷��iBug9642�Œǉ��j
		 * @note	�ʂ�Ȃ������N�Ƃ́A���������ʍs�֎~�܂��͒ʍs�s�̃����N�B
		 * @return	���茋��
		 */
		bool isPassable( IFeaturePtr cpLink );

		/**
		 * @brief �Ώۃ����N�P����Ώۃ����N�Q�֒ʍs�ł��邩�ۂ����E���܋֎~�K���ɂ���Ĕ��肷��
		 * @param[in]	cpLink1	�Ώۃ����N�P
		 * @param[in]	cpLink2	�Ώۃ����N�Q
		 * @param[in]	crLinkTable	�Ώۃ����N�P�A�Q���܂ރ����N�e�[�u���i�܂܂�Ȃ��ꍇ�͒ʍs�\�Ɣ��肳���j
		 * @return	���茋��
		 */
		bool isPassableByTurnreg( IFeaturePtr cpLink1, IFeaturePtr cpLink2, sindy::CRoadLinkTable& crLinkTable );

		/**
		 * @brief �Ώۃ����N��������ł��邩�ہi�ᑬ��ł���j���𔻒肷��
		 * @param[in]	cpLink	�Ώۃ����N
		 * @return	���茋��
		 */
		bool isHighSpeedArea( IFeaturePtr cpLink );

		namespace id_lists
		{
			typedef std::set< long > IDs;

			/**
			 * @brief ID��񋓂�����������쐬����N���X
			 * @note	�uID��1000���񋓂���Where��Q�v�����̂Ɏg����̂�z��B
			 */
			class IDLists : public std::vector< std::string >
			{
			public:
				/**
				 * @brief �R���X�g���N�^
				 * @param[in]	crIDs		�Ώ�ID�Q
				 * @param[in]	cDelimiter	�f���~�^
				 * @param[in]	cMax		�񋓂��鐔�̍ő�l
				 */
				IDLists( const IDs& crIDs, const std::string& cDelimiter, unsigned cMax );
			};

		} // namespace id_lists

		// sindylib_base�֘A�̃V���^�b�N�X�V���K�[
		namespace sindylib_base
		{
			/// CSPRowContainer�ւ̃L���X�g
			template < typename T > sindy::CSPRowContainer row( T& t )
			{
				return boost::dynamic_pointer_cast< sindy::CRowContainer >( t );
			}

			/// _IRow*�ւ̃L���X�g
			template < typename T > _IRow* irow( T& t )
			{
				return (_IRow*)*boost::dynamic_pointer_cast< sindy::CRowContainer >( t );
			}

			/// CSPRoadLinkTable�ւ̃L���X�g
			template < typename T > sindy::CSPRoadLinkTable road_link_table( T& t )
			{
				return boost::dynamic_pointer_cast< CRoadLinkTable >( t );
			}
		}

		/**
		 * @brief �w�肳�ꂽ�f�B���N�g�����Ȃ���΍쐬����
		 * @param[in]	crDirPath	�Ώۃf�B���N�g���p�X
		 */
		void createDirectory( const boost::filesystem::path& crDirPath );

		/**
		 * @brief ��ԎQ�Ɗ֘A�̋@�\	�i[Bug9717]�Œǉ��j
		 * @note	���̃c�[���ɂ������ԎQ�Ɛݒ�̕��j�͉��L�̒ʂ�B
		 *			(1) �ŏ��ɒ��ڂ��������N�̋�ԎQ�Ƃ���Ƃ��A�S�Ă���ŏ�������B
		 *			(2) �`����܂ރC���X�^���X�𐶐�����ۂ͕K��newGeometry()���g���A�����Ɠ����ɋ�ԎQ�Ƃ��Z�b�g����B
		 *			(3) �W�I���g�����Z�n�̊֐����g�p��A��ԎQ�Ƃ��ς��\��������ꍇ��setSpatialReference()�ŃZ�b�g�������B
		 */
		namespace spatial_reference
		{
			/**
			 * @brief �ΏۃW�I���g���ɋ�ԎQ�Ƃ��Z�b�g����Ă��邩�ۂ��𔻒肷��
			 * @param[in]	cpGeom	�ΏۃW�I���g��
			 * @note	�L�q��Z�����邽�߂����̊֐��B
			 * @return	���茋��
			 */
			bool hasSpatialReference( const IGeometryPtr cpGeom );

			/**
			 * @brief �ΏۃW�I���g���ɋ�ԎQ�Ƃ��Z�b�g����Ă��Ȃ���΃Z�b�g����
			 * @note	�ړI�́A���s��ɑΏۃW�I���g���Ɏw�肵����ԎQ�Ƃ��Z�b�g����Ă��鎖��ۏႷ�鎖�B
			 * @param[in]	cpGeom			�ΏۃW�I���g��
			 * @param[in]	cpSpatialRef	�Z�b�g�����ԎQ��
			 * @exception		���s
			 * @todo	��O���o�͔O�̂��߁B�܂����s���Ȃ������m�F�ł�����assert�ɂ��ėǂ��B
			 */
			void setSpatialReference( IGeometryPtr cpGeom, const ISpatialReferencePtr cpSpatialRef );

			/**
			 * @brief �`����܂ރC���X�^���X�𐶐��iCreateInstance�j���A�w�肳�ꂽ��ԎQ�Ƃ��Z�b�g���ĕԂ�
			 * @note	��s�ŊȌ��ɏ����������^����������̂Ŋ֐��e���v���[�g�����Ă݂����A�Ăяo�����̂������̂Ŕ����Ȋ����c�B
			 * @param	TPtr	�������ꂽ�C���X�^���X���w����COM�C���^�[�t�F�C�X�|�C���^�̌^�iIPointPtr, IPolylinePtr�Ȃǁj
			 * @param[in]	clsid			��������C���X�^���X��CLSID
			 * @param[in]	cpSpatialRef	�Z�b�g�����ԎQ��
			 * @return	�������ꂽ�C���X�^���X
			 */
			template <typename TPtr>
			TPtr newGeometry( const CLSID& clsid, const ISpatialReferencePtr cpSpatialRef )
			{
				TPtr pInstance( clsid );
				setSpatialReference( pInstance, cpSpatialRef );
				return pInstance;
			}
		} // namespace spatial_reference

		/**
		 * @brief double�l�𕶎��񉻂��ĕԂ�
		 * @note	���x�w�肠��Bboost::lexical_cast��boost�̃o�[�W����1.34.0����16���A1.51.0���Ƃ���ȏ�i���m�F�j�Ɠ��삪������̂ŁA
		 *			���ʂ��قȂ鎖��h�����߂�std::stringstream���g�p���Đ��x�𖾎��I�Ɏw�肷��悤�ɂ����B
		 * @param[in]	cNum		�Ώ�double�l
		 * @param[in]	cPrecision	���x�i10�i���̌����B123.456�Ȃ�6���j
		 * @return	������
		 */
		std::string toStr( double cNum, std::streamsize cPrecision );

	} // namespace utility
} // namespace curve_info

