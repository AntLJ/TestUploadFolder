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


#include "stdafx.h"
#include "CheckConvert.h"
#include "CharAddrConvertFunc.h"

#include <sindy/schema.h>

CCheckConvert::CCheckConvert()
{
}

CCheckConvert::~CCheckConvert()
{
}

void  CCheckConvert::Init( IFeatureClass* ipGouPoint, IFeatureClass* ipCityAdmin, LPCTSTR lpcszConvertTablePath, LPCTSTR lpcszOutput )
{
	m_ipGouPointClass = ipGouPoint;
	m_ipCityAdminClass = ipCityAdmin;
	m_strConvertTable = lpcszConvertTablePath;
	m_strOutput = lpcszOutput;
}

bool CCheckConvert::run()
{
	bool bReturn = true;

	std::multimap<long,CString> mapError = m_cCharAddrConvert.Init( m_strConvertTable );
	if( mapError.empty() )
	{
		// 号ポイントから文字付き住所リスト取得
		std::cerr << "\n文字付き号ポイントを取得中...\n";
		std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapGouPoint = GetItems( _T("LIKE '#%'"), sindy::schema::gou_point::kGouNo, m_ipGouPointClass, 0 );
		long lGouPointSize = mapGouPoint.size();

		// 行政界から文字付き住所リスト取得
		std::cerr << "文字付き都市地図行政界を取得中...\n";
		std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapCityAdmin = GetItems( _T("IS NOT NULL"), sindy::schema::city_admin::kExtGaikuFugo, m_ipCityAdminClass, 1 );
		long lCityAdminSize = mapCityAdmin.size();

		// <long,<CString,long>> OID,読み,合致タイプ(0:なし,1:漢字読み,2:オブジェクト読み)
		std::map<long,std::pair<CString,long>> mapConvertedGouPoint;
		std::map<long,std::pair<CString,long>> mapConvertedCityAdmin;

		// 号ポイントのオブジェクトID読み変換
		std::cerr << "号ポイントのオブジェクトID読み変換中...\n";
		ConvertItemsFromObjectYomiTable( mapGouPoint, mapConvertedGouPoint, GouPoint_Layer );

		// 行政界のオブジェクトID読み変換
		std::cerr << "都市地図行政界ののオブジェクトID読み変換中...\n";
		ConvertItemsFromObjectYomiTable( mapCityAdmin, mapConvertedCityAdmin, CityAdmin_Layer );


		// 号ポイントの漢字読みテーブル変換
		std::cerr << "号ポイントの漢字読み変換中...\n";
		ConvertItemsFromKanjiYomiTable( mapGouPoint, mapConvertedGouPoint );

		// 行政界のの漢字読みテーブル変換
		std::cerr << "都市地図行政界の漢字読み変換中...\n";
		ConvertItemsFromKanjiYomiTable( mapCityAdmin, mapConvertedCityAdmin );

		if( mapConvertedGouPoint.size() == lGouPointSize && mapConvertedCityAdmin.size() == lCityAdminSize )
		{
			// 号ポイントの変換の結果を出力
			std::cerr << "号ポイントの変換結果を出力中...\n";
			m_bGouNG = WriteConvertResult( mapConvertedGouPoint, _T("convertGouPoint"), m_strNGGouFilePath );

			// 行政界の変換の結果を出力
			std::cerr << "都市地図行政界の変換結果を出力中...\n";
			m_bCityAdminNG = WriteConvertResult( mapConvertedCityAdmin, _T("convertCityAdmin"), m_strNGCityAdminFilePath );
		}
		else
		{
			std::cout << "#ERROR\t文字付き住所リストと読み変換後のリストの数が一致しません！\n";
			std::cerr << "結果の出力ができませんでした\n";
			bReturn = false;
		}
	}
	else
	{
		bReturn = false;
		std::cerr << "変換テーブルにエラーがあったので処理終了\n";
		for( std::multimap<long,CString>::const_iterator it = mapError.begin(); it != mapError.end(); ++it )
			std::cout << "#ERRROR\t" << it->first << "\t" << it->second << "\n";
	}

	return bReturn;
}

void CCheckConvert::ConvertItemsFromObjectYomiTable( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, std::map<long,std::pair<CString,long>>& mapConvertedItems, GouOrCityAddrLayer LayerType )
{
	// オブジェクトID読み変換テーブルから読みを取得する
	std::map<ObjectLayerPair,KanjiYomiPair> mapObjectYomiItems = m_cCharAddrConvert.getObjectYomiItems();
	std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapRet;
	long lCount = 0, lSize = mapItems.size();
	for( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>::const_iterator it = mapItems.begin(); it != mapItems.end(); ++it )
	{
		lCount++;
		if(lCount % 100 == 0)
			std::cerr << "\t処理中: " << lCount << " / " << lSize << "\r";

		// 変換テーブルと同じものがあれば書き出し用リストに追加する
		bool bRet = false;
		std::string strYomi = m_cCharAddrConvert.GetYomiFromCharAddr_Object( LayerType, it->first, it->second.first, bRet );
		if( bRet )
			mapConvertedItems.insert( std::pair<long,std::pair<CString,long>>( it->first, std::pair<CString,long>( strYomi.c_str(), 2 ) ) );
		else
			mapRet.insert( *it );
	}
	std::cerr << "\t処理中: " << lCount << " / " << lSize << "\n";
	std::cerr << "\t変換件数: " << mapItems.size() - mapRet.size() << " 件\n";
	mapItems.swap( mapRet );

	std::cerr << std::endl;
}

void CCheckConvert::ConvertItemsFromKanjiYomiTable( const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, std::map<long,std::pair<CString,long>>& mapConvertedItems )
{
	long lCount = 0, lSize = mapItems.size();
	long lSuccess = 0;
	for( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>::const_iterator it = mapItems.begin(); it != mapItems.end(); ++it )
	{
		lCount++;
		if(lCount % 100 == 0)
			std::cerr << "\t処理中: " << lCount << " / " << lSize << "\r";

		bool bSuccess = true;
		CString strYomi = CA2T( m_cCharAddrConvert.GetYomiFromCharAddr_Kanji( it->second.first, bSuccess ).c_str() );
		if( bSuccess )
		{
			lSuccess++;
			mapConvertedItems.insert( std::pair<long,std::pair<CString,long>>( it->first, std::pair<CString,long>( strYomi, 1 ) ) );
		}
		else
			mapConvertedItems.insert( std::pair<long,std::pair<CString,long>>( it->first, std::pair<CString,long>( strYomi, 0 ) ) );
	}
	std::cerr << "\t処理中: " << lCount << " / " << lSize << "\n";
	std::cerr << "\t変換件数: " << lSuccess << " 件\n";
	std::cerr << std::endl;
}

bool CCheckConvert::WriteConvertResult( const std::map<long,std::pair<CString,long>>& mapConvertedItems, LPCTSTR lpcszOutputFile, CString& strNGFilePath )
{
	bool bNg = false;
	CString strOKFilePath;
	strOKFilePath.Format( _T("%s\\%s_OK.txt"), m_strOutput, lpcszOutputFile );
	strNGFilePath.Format( _T("%s\\%s_NG.txt"), m_strOutput, lpcszOutputFile );
	if( PathFileExists( strOKFilePath ) )
		strOKFilePath = CreateLogFile( m_strOutput, CString(lpcszOutputFile) + _T("_OK"), 1 );
	if( PathFileExists( strNGFilePath ) )
		strNGFilePath = CreateLogFile( m_strOutput, CString(lpcszOutputFile) + _T("_NG"), 1 );
	std::ofstream ofs_ok(static_cast<CT2W>(strOKFilePath));
	std::ofstream ofs_ng(static_cast<CT2W>(strNGFilePath));
	if( !ofs_ok.fail() && !ofs_ng.fail() )
	{
		for( std::map<long,std::pair<CString,long>>::const_iterator it = mapConvertedItems.begin(); it != mapConvertedItems.end(); ++it )
		{
			switch( it->second.second )
			{
			case 0:
				ofs_ng << "NG\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				bNg = true;
				break;
			case 1:
				ofs_ok << "OK(漢字読み)\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				break;
			case 2:
				ofs_ok << "OK(OID読み)\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				break;
			default:
				ofs_ng << "この出力はおかしい！\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				std::cout << "#ERROR\tこの出力はおかしい！\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				break;
			}
		}
	} else {
		std::cout << "#ERROR\t" << CT2A(strOKFilePath) << "\tログファイル作成失敗\n";
		std::cout << "#ERROR\t" << CT2A(strNGFilePath) << "\tログファイル作成失敗\n";
	}
	return bNg;
}

bool CCheckConvert::get_GouCityAdminNG(CString& strGouPointText, CString& strCityAdminText)
{
	strGouPointText = m_strNGGouFilePath;
	strCityAdminText = m_strNGCityAdminFilePath;

	// どっちかNGがある場合はtrue, どちらもNGが無い場合はfalse
	if(m_bGouNG || m_bCityAdminNG)
		return true;
	else
		return false;
}
