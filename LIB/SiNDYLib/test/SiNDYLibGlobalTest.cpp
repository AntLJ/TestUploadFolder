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
 * @brief SiNDYLibのグローバル関数であるsindy::AheCheckOrFixAnnotationStringをテストする
 * クラスの定義ファイルです。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
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
 * @brief SiNDYLibのグローバル関数であるsindy::AheCheckOrFixAnnotationStringをテストする
 * クラス
 * <h3>チェック内容：<h3>
 * @li 注記文字列と文字列長の整合性テスト
 * @li 全角文字列使用可能文字のテスト
 * @li ジャンクション、インターチェンジ、フェリーターミナルの場合の最終文字列テスト
 * @li 主要地方道番号、国道番号の場合のテスト
 * @li 山岳標高の場合のテスト
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
	 * @brief 注記文字列と文字列長の整合テスト
	 */
	void testAheCheckOrFixAnnotationString_StringLen()
	{
		CSiNDYLibGlobal cClass;

		// 注記文字列：NULL、注記文字数：3、注記種別：海の場合sindyErr_AnnoStrLenNotSameが返るはず
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrLenNotSame, cClass.CheckOrFixAnnotationString( NULL, 3, annotation_code::kSea ) );

		// 注記文字列：_T("あああ")、注記文字数：1、注記種別：海の場合sindyErr_AnnoStrLenNotSameが返るはず
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrLenNotSame, cClass.CheckOrFixAnnotationString( _T("あああ"), 1, annotation_code::kSea ) );

		// 注記文字列：_T("あああ１２")、注記文字数：10、注記種別：海の場合sindyErr_NoErrが返るはず
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("あああ１２"), 10, annotation_code::kSea ) );

		// 注記文字列：_T("あああ１２")、注記文字数：7、注記種別：海の場合sindyErr_AnnoStrLenNotSameが返るはず
		// このチェックは以前の使用が「数値が偶数回連続する場合は2文字を1と数える」というルールが現在も適用されていないかどうかのチェックです。
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrLenNotSame, cClass.CheckOrFixAnnotationString( _T("あああ１２"), 7, annotation_code::kSea ) );

		// 注記文字列：_T("あああ１２３")、注記文字数：12、注記種別：海の場合sindyErr_NoErrが返るはず
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("あああ１２３"), 12, annotation_code::kSea ) );

		// 注記文字列：_T("あああ１２３")、注記文字数：8、注記種別：海の場合sindyErr_AnnoStrLenNotSameが返るはず
		// このチェックは以前の使用が「数値が偶数回連続する場合は2文字を1と数え、残り一つは1と数える」というルールが現在も適用されていないかどうかのチェックです。
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrLenNotSame, cClass.CheckOrFixAnnotationString( _T("あああ１２３"), 8, annotation_code::kSea ) );
	}
	/// 全角文字列使用可能文字のテスト
	void testAheCheckOrFixAnnotationString_WideChar()
	{
		CSiNDYLibGlobal cClass;

		// 先頭に全角0が来てはいけない ->いらない（bug 3035）
		//CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFirstIsWideZero, cClass.CheckOrFixAnnotationString( _T("０あああ"), 8, annotation_code::kSea ) );
	}
	/// ジャンクション、インターチェンジ、フェリーターミナルの場合の最終文字列のテスト
	void testAheCheckOrFixAnnotationString_JCICFT()
	{
#ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK
		CSiNDYLibGlobal cClass;

		// JCの種別リスト作成
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
			// 注記文字列：_T("あああ")、注記文字数：6、注記種別：JC、都市地図の場合はsindyErr_AnnoStrJCButLast3IsNotJCTが返るはず
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrJCButLast3IsNotJCT, cClass.CheckOrFixAnnotationString( _T("あああ"), 6, *it ) );

			// 注記文字列：_T("あああＪＣ")、注記文字数：10、注記種別：JC、都市地図の場合はsindyErr_AnnoStrJCButLast3IsNotJCTが返るはず
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrJCButLast3IsNotJCT, cClass.CheckOrFixAnnotationString( _T("あああＪＣ"), 10, *it ) );
			
			// 注記文字列：_T("あああＪＣＴ")、注記文字数：12、注記種別：JC、都市地図の場合はsindyErr_NoErrが返るはず
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("あああＪＣＴ"), 12, *it ) );

			// 注記文字列：_T("あああＪＣ")、注記文字数：10、注記種別：JC、都市地図以外の場合はsindyErr_NoErrが返るはず
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("あああＪＣ"), 10, *it, false ) );

			// 注記文字列：_T("あああＪＣＴ")、注記文字数：12、注記種別：JC、都市地図以外の場合はsindyErr_AnnoStrICJCFTButLast2IsNotICJCFTが返るはず
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("あああＪＣＴ"), 12, *it, false ) );
		}

		// ICの種別リスト作成
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
			// 注記文字列：_T("あああ")、注記文字数：6、注記種別：ICの場合はsindyErr_AnnoStrICJCFTButLast2IsNotICJCFTが返るはず
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("あああ"), 6, *it ) );			// 都市地図
			CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("あああ"), 6, *it, false ) );	// 都市地図以外
			
			// 注記文字列：_T("あああＩＣ")、注記文字数：10、注記種別：ICの場合はsindyErr_NoErrが返るはず
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("あああＩＣ"), 10, *it ) );			// 都市地図
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("あああＩＣ"), 10, *it, false ) );	// 都市地図以外
		}

		// 注記文字列：_T("あああ")、注記文字数：6、注記種別：FTの場合はsindyErr_AnnoStrICJCFTButLast2IsNotICJCFTが返るはず
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("あああ"), 6, annotation_code::kFerryTerminal ) );			// 都市地図
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT, cClass.CheckOrFixAnnotationString( _T("あああ"), 6, annotation_code::kFerryTerminal, false ) );	// 都市地図以外
		
		// 注記文字列：_T("あああＦＴ")、注記文字数：10、注記種別：ICの場合はsindyErr_NoErrが返るはず
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("あああＦＴ"), 10, annotation_code::kFerryTerminal ) );			// 都市地図
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("あああＦＴ"), 10, annotation_code::kFerryTerminal, false ) );	// 都市地図以外
#endif // ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK
	}
	/// 主要地方道番号、国道番号の場合のチェック
	void testAheCheckOrFixAnnotationString_RouteNumber()
	{
		CSiNDYLibGlobal cClass;
		TCHAR strNum[5];
		int nLen;

		// 主要地方道番号の場合は半角数字で、且つ1～151まで
		for( int i = 1; i <= 151; ++i )
		{
			if( i < 10 ) nLen = 1; else if( i < 100 ) nLen = 2;	else nLen = 3;
			_itot( i, strNum, 10 );
			CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( strNum, nLen, annotation_code::kMainPrefRouteNumber ) );
		}
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( NULL, 0, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrRouteNumberOverFlow, cClass.CheckOrFixAnnotationString( _T("152"), 3, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("１００"), 6, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("100.0"), 5, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("0"), 1, annotation_code::kMainPrefRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("01"), 2, annotation_code::kMainPrefRouteNumber ) );

		// 県道番号の場合は半角数字で、且つ1～58、101～108、112～507まで
		for( int i = 1; i <= 507; ++i )
		{
			sindyErrCode ecode = sindyErr_NoErr;

			if( ( 59  <= i && i <= 100 ) || // 59  - 100 までは対象外
				( 109 <= i && i <= 111 ) )  // 109 - 111 までは対象外
				ecode = sindyErr_AnnoStrRouteNumberOverFlow;

			if( i < 10 ) nLen = 1; else if( i < 100 ) nLen = 2;	else nLen = 3;
			_itot( i, strNum, 10 );
			CPPUNIT_ASSERT_EQUAL( ecode, cClass.CheckOrFixAnnotationString( strNum, nLen, annotation_code::kNationalRouteNumber ) );
		}
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( NULL, 0, annotation_code::kNationalRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("１０３"), 6, annotation_code::kNationalRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("103.0"), 5, annotation_code::kNationalRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("0"), 1, annotation_code::kNationalRouteNumber ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("01"), 2, annotation_code::kNationalRouteNumber ) );

	}
	/// 山岳標高の場合のチェック
	void testAheCheckOrFixAnnotationString_Mountain()
	{
		CSiNDYLibGlobal cClass;

		// 山岳標高の場合は半角数字＋ピリオドで、文字列3、都市地図以外、且つ小数点第一位まで存在しなければならない
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotWide, cClass.CheckOrFixAnnotationString( _T("123.4"), 5, annotation_code::kMountain, true, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("123.4"), 5, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("123.456789"), 10, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.CheckOrFixAnnotationString( _T("岩手山"), 6, annotation_code::kMountain, true, false ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotWide, cClass.CheckOrFixAnnotationString( _T("123.4"), 5, annotation_code::kMountain, true, false ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotWide, cClass.CheckOrFixAnnotationString( _T("123.4"), 5, annotation_code::kMountain, false, false ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrMountainInvalid, cClass.CheckOrFixAnnotationString( _T("123."), 4, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrMountainInvalid, cClass.CheckOrFixAnnotationString( _T("123"), 3, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrFistIsHalfZero, cClass.CheckOrFixAnnotationString( _T("0123.4"), 6, annotation_code::kMountain, false, true ) );
		CPPUNIT_ASSERT_EQUAL( sindyErr_AnnoStrIsNotHalfNumber, cClass.CheckOrFixAnnotationString( _T("１２３.４"), 9, annotation_code::kMountain, false, true ) );
	}
};

#ifdef _TEST_ALL
CPPUNIT_TEST_SUITE_REGISTRATION(CSiNDYLibAheCheckOrFixAnnotationStringTest);
#endif // ifdef _TEST_ALL
