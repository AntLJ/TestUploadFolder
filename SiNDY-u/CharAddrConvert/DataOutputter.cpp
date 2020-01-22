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
#include "DataOutputter.h"

extern "C"
{
#include <WKF_LIB/wkf.h>
}

/**
 * @brief	文字列をSJIS->EUCコードへ変換
 * @param	lpcszData	[in]	文字列
 * @return	変換データ
 */
inline CString ConvDataSJIS2EUC( const _TCHAR* lpcszData )
{
	char szConvData[200] = "";
	return CONV_OK == wkfConvertKanjiCodeOfString(KC_SJIS, CT2A(lpcszData), KC_EUC, szConvData, sizeof(szConvData))?
		   CString(szConvData) : CString(_T(""));
}

namespace data_output
{
	// ナビ向けPntレコード出力
	void OutPntFile( std::ofstream& stream, const PNTMAP& rPntList )
	{
		for( PNTMAP::const_iterator itr = rPntList.begin(); itr != rPntList.end(); ++itr )
		{
			const CPntRec& cRec = itr->second;
			stream << cRec.m_cMXY.m_nMeshCode << "\t" << cRec.m_cMXY.m_nX << "\t" << cRec.m_cMXY.m_nY << "\t"								// MeshXY
				   << 51 << "\t" << 4 << "\t"																								// 座標提供会社・座標精度種別コード
				   << cRec.m_strAddrCode.Left(2) << "\t" << cRec.m_strAddrCode.Mid(2,3) << "\t"												// 都道府県・市区町村コード
				   << cRec.m_strAddrCode.Mid(5,3) << "\t" << cRec.m_strAddrCode.Mid(8, 3) << "\t" << cRec.m_strAddrCode.Right(5) << "\t"	// 大字・字・地番コード
				   << ConvDataSJIS2EUC(addr_func::GetZenkakuData(cRec.m_strPntName)) << "\t"												// 地番名称
				   << ConvDataSJIS2EUC(addr_func::GetZenkakuData(cRec.m_strPntYomi)) << "\t"												// 地番ヨミ
				   << cRec.m_strZipCode << "\t" << cRec.m_lRepDomariF << "\n";																// 郵便番号・代表点・地番止まりフラグ
			stream.flush();
		}
	}

	// 外部提供向けPntレコード出力
	void OutPntFile_Outside( std::ofstream& stream, const PNTMAP& rPntList )
	{
		for( PNTMAP::const_iterator itr = rPntList.begin(); itr != rPntList.end(); ++itr )
		{
			const CPntRec& cRec = itr->second;

			// 座標を小数点第11桁まで必ず表示する為
			CString strLonLat;
			strLonLat.Format( _T("%.11f\t%.11f"), cRec.m_dLon, cRec.m_dLat );
			stream << cRec.m_strAddrCode.Left(2) << "\t" << cRec.m_strAddrCode.Mid(2,3) << "\t"												// 都道府県・市区町村コード
				   << cRec.m_strAddrCode.Mid(5,3) << "\t" << cRec.m_strAddrCode.Mid(8, 3) << "\t" << cRec.m_strAddrCode.Right(5) << "\t"	// 大字・字・地番コード
				   << addr_func::GetZenkakuData(cRec.m_strPntName) << "\t" << addr_func::GetZenkakuData(cRec.m_strPntYomi) << "\t"			// 地番名称・ヨミ
				   << CT2A(strLonLat) << "\t"																								// 緯度・経度
				   << cRec.m_lRepDomariF << "\r\n";																							// 代表点・地番止まりフラグ
			stream.flush();
		}
	}

	// ナビ向けGouレコード出力
	void OutGouFile( std::ofstream& stream, const GOUMAP& rGouList )
	{
		for( GOUMAP::const_iterator itr = rGouList.begin(); itr != rGouList.end(); ++itr )
		{
			const CGouRec& cRec = itr->second;
			
			// 下4桁[0000]になっている場合は、出力しない
			if( cRec.m_strAddrCode.Right(4) != _T("0000") )
			{
				stream << cRec.m_cMXY.m_nMeshCode << "\t" << cRec.m_cMXY.m_nX << "\t" << cRec.m_cMXY.m_nY << "\t"						// MeshXY
					   << 51 << "\t" << 5 << "\t"																						// 座標提供会社・座標精度種別コード
					   << cRec.m_strAddrCode.Left(2) << "\t" << cRec.m_strAddrCode.Mid(2,3) << "\t"										// 都道府県・市区町村コード
					   << cRec.m_strAddrCode.Mid(5,3) << "\t" << cRec.m_strAddrCode.Mid(8, 3) << "\t"									// 大字・字コード
					   << cRec.m_strAddrCode.Mid(11, 5) << "\t" << cRec.m_strAddrCode.Right(4) << "\t"									// 地番・住居番号コード
					   << ConvDataSJIS2EUC(addr_func::GetZenkakuData(cRec.m_strGouName)) << "\t"										// 住居番号名称
					   << ConvDataSJIS2EUC(addr_func::GetZenkakuData(cRec.m_strGouYomi)) << "\t"										// 住居番号ヨミ
					   << cRec.m_strZipCode << "\n";																					// 郵便番号
				stream.flush();
			}
		}
	}

	// 外部提供向けGouレコード出力
	void OutGouFile_Outside( std::ofstream& stream, const GOUMAP& rGouList )
	{
		for( GOUMAP::const_iterator itr = rGouList.begin(); itr != rGouList.end(); ++itr )
		{
			const CGouRec& cRec = itr->second;
			
			// 下4桁[0000]になっている場合は、出力しない
			if( cRec.m_strAddrCode.Right(4) != _T("0000") )
			{
				// 座標を小数点第11桁まで必ず表示する為
				CString strLonLat;
				strLonLat.Format( _T("%.11f\t%.11f"), cRec.m_dLon, cRec.m_dLat );
				stream << cRec.m_strAddrCode.Left(2) << "\t" << cRec.m_strAddrCode.Mid(2,3) << "\t"										// 都道府県・市区町村コード
					   << cRec.m_strAddrCode.Mid(5,3) << "\t" << cRec.m_strAddrCode.Mid(8, 3) << "\t"									// 大字・字コード
					   << cRec.m_strAddrCode.Mid(11, 5) << "\t" << cRec.m_strAddrCode.Right(4) << "\t"									// 地番・住居番号コード
					   << addr_func::GetZenkakuData(cRec.m_strGouName) << "\t" << addr_func::GetZenkakuData(cRec.m_strGouYomi) << "\t"	// 住居番号名称・住居番号ヨミ
					   << CT2A(strLonLat) << "\r\n";																					// 緯度・経度
				stream.flush();
			}
		}
	}
}
