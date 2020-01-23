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

namespace
{
	// TODO: AddrLib で定義する
	namespace areacode_master
	{
		const CString kTableName = _T("AREACODE_MASTER");
		const CString kKenCode = _T("new_ken_code");
		const CString kShiCode = _T("new_shi_code");
		const CString kOazaCode = _T("new_oaza_code");
		const CString kAzaCode = _T("new_aza_code");
		const CString kZipCode = _T("zip_code");
		const CString kShikoDate = _T("shiko_date");
		const CString kHaishiDate = _T("haishi_date");
	}

	namespace shape_field
	{
		const CString kObjectId = _T("OBJECTID");
		const CString kZipCode = _T("ZIPCODE");
	}

	namespace table_field
	{
		const CString kPolygonId = _T("POLYGONID");
		const CString kPrefCode = _T("PREFCODE");
		const CString kCityCode = _T("CITYCODE");
		const CString kOazaCode = _T("OAZACODE");
		const CString kAzaCode = _T("AZACODE");
	}

	namespace error_message
	{
		const CString kNonExistingCode = _T("住所マスタに存在しない住所コード");
		const CString kFailMerge = _T("マージ失敗");
	}

	// ツール実行モード
	enum ExecuteMode
	{
		none, // 未定義
		shp, // Shp出力モード
		sde, // Sde出力モード
	};
}
