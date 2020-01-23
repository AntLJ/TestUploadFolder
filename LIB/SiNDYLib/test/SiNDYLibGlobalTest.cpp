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
 * @file SiNDYLibAheCheckOrFixAnnotationStringTest.cpp
 * @brief SiNDYLib�̃O���[�o���֐��ł���sindy::AheCheckOrFixAnnotationString���e�X�g����
 * �N���X�̒�`�t�@�C���ł��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "SiNDYLibGlobal.h"
#include <list>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

using namespace sindy::schema::annotation;
/**
 * @class CSiNDYLibAheCheckOrFixAnnotationStringTest
 * @brief SiNDYLib�̃O���[�o���֐��ł���sindy::AheCheckOrFixAnnotationString���e�X�g����
 * �N���X
 * <h3>�`�F�b�N���e�F<h3>
 * @li ���L������ƕ����񒷂̐������e�X�g
 * @li �S�p������g�p�\�����̃e�X�g
 * @li �W�����N�V�����A�C���^�[�`�F���W�A�t�F���[�^�[�~�i���̏ꍇ�̍ŏI������e�X�g
 * @li ��v�n�����ԍ��A�����ԍ��̏ꍇ�̃e�X�g
 * @li �R�x�W���̏ꍇ�̃e�X�g
 */
class CSiNDYLibAheCheckOrFixAnnotationStringTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CSiNDYLibAheCheckOrFixAnnotationStringTest);
  CPPUNIT_TEST(testAheCheckOrFixAnnotationString_StringLen);
  CPPUNIT_TEST(testAheCheckOrFixAnnotationString_WideChar);
  CPPUNIT_TEST(testAheCheckOrFixAnnotationString_JCICFT);
  CPPUNIT_TEST(testAheCheckOrFixAnnotationString_RouteNumber);
  CPPUNIT_TEST(testAheCheckOrFixAnnotationString_Mountain);
  CPPUNIT_TEST_SUITE_END();
public:
	CSiNDYLibAheCheckOrFixAnnotationStringTest(){}
	virtual ~CSiNDYLibAheCheckOrFixAnnotationStringTest(){}

	/**
	 * @brief ���L������ƕ����񒷂̐����e�X�g
	 */
	void testAheCheckOrFixAnnotationString_StringLen()
	{
		CSiNDYLibGlobal cClass;

		// ���L������FNULL�A���L�������F3�A���L��ʁF�C�̏ꍇsindyErr_AnnoStrLenNotSame���Ԃ�͂�
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrLenNotSame, cClass.CheckOrFixAnnotationString( NULL, 3, annotation_code::kSea ) );

		// ���L������F_T("������")�A���L�������F1�A���L��ʁF�C�̏ꍇsindyErr_AnnoStrLenNotSame���Ԃ�͂�
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrLenNotSame, cClass.CheckOrFixAnnotationString( _T("������"), 1, annotation_code::kSea ) );

		// ���L������F_T("�������P�Q")�A���L�������F10�A���L��ʁF�C�̏ꍇsindyErr_NoErr���Ԃ�͂�
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("�������P�Q"), 10, annotation_code::kSea ) );

		// ���L������F_T("�������P�Q")�A���L�������F7�A���L��ʁF�C�̏ꍇsindyErr_AnnoStrLenNotSame���Ԃ�͂�
		// ���̃`�F�b�N�͈ȑO�̎g�p���u���l��������A������ꍇ��2������1�Ɛ�����v�Ƃ������[�������݂��K�p����Ă��Ȃ����ǂ����̃`�F�b�N�ł��B
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrLenNotSame, cClass.CheckOrFixAnnotationString( _T("�������P�Q"), 7, annotation_code::kSea ) );

		// ���L������F_T("�������P�Q�R")�A���L�������F12�A���L��ʁF�C�̏ꍇsindyErr_NoErr���Ԃ�͂�
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("�������P�Q�R"), 12, annotation_code::kSea ) );

		// ���L������F_T("�������P�Q�R")�A���L�������F8�A���L��ʁF�C�̏ꍇsindyErr_AnnoStrLenNotSame���Ԃ�͂�
		// ���̃`�F�b�N�͈ȑO�̎g�p���u���l��������A������ꍇ��2������1�Ɛ����A�c����1�Ɛ�����v�Ƃ������[�������݂��K�p����Ă��Ȃ����ǂ����̃`�F�b�N�ł��B
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrLenNotSame, cClass.CheckOrFixAnnotationString( _T("�������P�Q�R"), 8, annotation_code::kSea ) );
	}
	/// �S�p������g�p�\�����̃e�X�g
	void testAheCheckOrFixAnnotationString_WideChar()
	{
		CSiNDYLibGlobal cClass;

		// �擪�ɑS�p0�����Ă͂����Ȃ� ->����Ȃ��ibug 3035�j
		//CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFirstIsWideZero, cClass.CheckOrFixAnnotationString( _T("�O������"), 8, annotation_code::kSea ) );
	}
	/// �W�����N�V�����A�C���^�[�`�F���W�A�t�F���[�^�[�~�i���̏ꍇ�̍ŏI������̃e�X�g
	void testAheCheckOrFixAnnotationString_JCICFT()
	{
#ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK
		CSiNDYLibGlobal cClass;

		// JC�̎�ʃ��X�g�쐬
		std::list<annotation_code::ECode> listJCICFT;
		listJCICFT.push_back( annotation_code::kHighwayCityHighwayJC );
		listJCICFT.push_back( annotation_code::kHighwayJC );
		listJCICFT.push_back( annotation_code::kCityHighwayJC );
		listJCICFT.push_back( annotation_code::kNationalTollJC );
		listJCICFT.push_back( annotation_code::kNationalDrivewayJC );
		listJCICFT.push_back( annotation_code::kMainTollJC );
		listJCICFT.push_back( annotation_code::kMainDrivewayJC );
		listJCICFT.push_back( annotation_code::kPrefTollJC );
		listJCICFT.push_back( annotation_code::kPrefDrivewayJC );
		listJCICFT.push_back( annotation_code::kOtherTollJC );
		listJCICFT.push_back( annotation_code::kOtherDrivewayJC );

		for( std::list<annotation_code::ECode>::const_iterator it = listJCICFT.begin(); it != listJCICFT.end(); ++it )
		{
			// ���L������F_T("������")�A���L�������F6�A���L��ʁFJC�A�s�s�n�}�̏ꍇ��sindyErr_AnnoStrJCButLast3IsNotJCT���Ԃ�͂�
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrJCButLast3IsNotJCT, cClass.CheckOrFixAnnotationString( _T("������"), 6, *it ) );

			// ���L������F_T("�������i�b")�A���L�������F10�A���L��ʁFJC�A�s�s�n�}�̏ꍇ��sindyErr_AnnoStrJCButLast3IsNotJCT���Ԃ�͂�
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrJCButLast3IsNotJCT, cClass.CheckOrFixAnnotationString( _T("�������i�b"), 10, *it ) );
			
			// ���L������F_T("�������i�b�s")�A���L�������F12�A���L��ʁFJC�A�s�s�n�}�̏ꍇ��sindyErr_NoErr���Ԃ�͂�
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("�������i�b�s"), 12, *it ) );

			// ���L������F_T("�������i�b")�A���L�������F10�A���L��ʁFJC�A�s�s�n�}�ȊO�̏ꍇ��sindyErr_NoErr���Ԃ�͂�
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("�������i�b"), 10, *it, false ) );

			// ���L������F_T("�������i�b�s")�A���L�������F12�A���L��ʁFJC�A�s�s�n�}�ȊO�̏ꍇ��sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT���Ԃ�͂�
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("�������i�b�s"), 12, *it, false ) );
		}

		// IC�̎�ʃ��X�g�쐬
		listJCICFT.clear();
		listJCICFT.push_back( annotation_code::kOtherHighwayIC );
		listJCICFT.push_back( annotation_code::kHighwayIC );
		listJCICFT.push_back( annotation_code::kNationalTollIC );
		listJCICFT.push_back( annotation_code::kDrivewayIC );
		listJCICFT.push_back( annotation_code::kMainTollIC );
		listJCICFT.push_back( annotation_code::kMainDrivewayIC );
		listJCICFT.push_back( annotation_code::kPrefTollIC );
		listJCICFT.push_back( annotation_code::kPrefDrivewayIC );
		listJCICFT.push_back( annotation_code::kOtherTollIC );
		listJCICFT.push_back( annotation_code::kOtherDrivewayIC );
		for( std::list<annotation_code::ECode>::const_iterator it = listJCICFT.begin(); it != listJCICFT.end(); ++it )
		{
			// ���L������F_T("������")�A���L�������F6�A���L��ʁFIC�̏ꍇ��sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT���Ԃ�͂�
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("������"), 6, *it ) );			// �s�s�n�}
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("������"), 6, *it, false ) );	// �s�s�n�}�ȊO
			
			// ���L������F_T("�������h�b")�A���L�������F10�A���L��ʁFIC�̏ꍇ��sindyErr_NoErr���Ԃ�͂�
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("�������h�b"), 10, *it ) );			// �s�s�n�}
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("�������h�b"), 10, *it, false ) );	// �s�s�n�}�ȊO
		}

		// ���L������F_T("������")�A���L�������F6�A���L��ʁFFT�̏ꍇ��sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT���Ԃ�͂�
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("������"), 6, annotation_code::kFerryTerminal ) );			// �s�s�n�}
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("������"), 6, annotation_code::kFerryTerminal, false ) );	// �s�s�n�}�ȊO
		
		// ���L������F_T("�������e�s")�A���L�������F10�A���L��ʁFIC�̏ꍇ��sindyErr_NoErr���Ԃ�͂�
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("�������e�s"), 10, annotation_code::kFerryTerminal ) );			// �s�s�n�}
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("�������e�s"), 10, annotation_code::kFerryTerminal, false ) );	// �s�s�n�}�ȊO
#endif // ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK
	}
	/// ��v�n�����ԍ��A�����ԍ��̏ꍇ�̃`�F�b�N
	void testAheCheckOrFixAnnotationString_RouteNumber()
	{
		CSiNDYLibGlobal cClass;
		TCHAR strNum[5];
		int nLen;

		// ��v�n�����ԍ��̏ꍇ�͔��p�����ŁA����1�`151�܂�
		for( int i = 1; i <= 151; ++i )
		{
			if( i < 10 ) nLen = 1; else if( i < 100 ) nLen = 2;	else nLen = 3;
			_itot( i, strNum, 10 );
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( strNum, nLen, annotation_code::kMainPrefRouteNumber ) );
		}
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( NULL, 0, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrRouteNumberOverFlow, cClass.CheckOrFixAnnotationString( _T("152"), 3, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("�P�O�O"), 6, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("100.0"), 5, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("0"), 1, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("01"), 2, annotation_code::kMainPrefRouteNumber ) );

		// �����ԍ��̏ꍇ�͔��p�����ŁA����1�`58�A101�`108�A112�`507�܂�
		for( int i = 1; i <= 507; ++i )
		{
			sindyErrCode ecode = sindyErr_NoErr;

			if( ( 59  <= i && i <= 100 ) || // 59  - 100 �܂ł͑ΏۊO
				( 109 <= i && i <= 111 ) )  // 109 - 111 �܂ł͑ΏۊO
				ecode = sindyErr_AnnoStrRouteNumberOverFlow;

			if( i < 10 ) nLen = 1; else if( i < 100 ) nLen = 2;	else nLen = 3;
			_itot( i, strNum, 10 );
			CPPUNIT_ASSERT_EQUAL( ecode, cClass.CheckOrFixAnnotationString( strNum, nLen, annotation_code::kNationalRouteNumber ) );
		}
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( NULL, 0, annotation_code::kNationalRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("�P�O�R"), 6, annotation_code::kNationalRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("103.0"), 5, annotation_code::kNationalRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("0"), 1, annotation_code::kNationalRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("01"), 2, annotation_code::kNationalRouteNumber ) );

	}
	/// �R�x�W���̏ꍇ�̃`�F�b�N
	void testAheCheckOrFixAnnotationString_Mountain()
	{
		CSiNDYLibGlobal cClass;

		// �R�x�W���̏ꍇ�͔��p�����{�s���I�h�ŁA������3�A�s�s�n�}�ȊO�A�������_���ʂ܂ő��݂��Ȃ���΂Ȃ�Ȃ�
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotWide, cClass.CheckOrFixAnnotationString( _T("123.4"), 5, annotation_code::kMountain, true, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("123.4"), 5, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("123.456789"), 10, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("���R"), 6, annotation_code::kMountain, true, false ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotWide, cClass.CheckOrFixAnnotationString( _T("123.4"), 5, annotation_code::kMountain, true, false ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotWide, cClass.CheckOrFixAnnotationString( _T("123.4"), 5, annotation_code::kMountain, false, false ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrMountainInvalid, cClass.CheckOrFixAnnotationString( _T("123."), 4, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrMountainInvalid, cClass.CheckOrFixAnnotationString( _T("123"), 3, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("0123.4"), 6, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("�P�Q�R.�S"), 9, annotation_code::kMountain, false, true ) );
	}
};

#ifdef _TEST_ALL
CPPUNIT_TEST_SUITE_REGISTRATION(CSiNDYLibAheCheckOrFixAnnotationStringTest);
#endif // ifdef _TEST_ALL
