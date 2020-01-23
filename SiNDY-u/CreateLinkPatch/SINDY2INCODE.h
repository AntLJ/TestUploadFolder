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
 *	SINDY2INCODE.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/30		新規作成
 */

#ifndef	___SINDY2INCODE_H___
#define	___SINDY2INCODE_H___

#include "SindyAttrdef.h"
//#include "SINDY2INRNWINFO.h"
#include "INAttrdef.h"

namespace SINDY2INCODE
{
	enum customer
	{
		pioneer			= 1,			// パイオニア
		clarion			= 2,			// クラリオン(TA)
		mitsubishi		= 3,			// 三菱電機
		xanavi			= 4,			// ザナビ
		other			= 5				// クラリオン(Kiwi)、Sony
	};

	struct Code
	{
		long			sindy_code;
		long			in_code;
	};

	extern Code road_manager[];			// 道路管理者
	extern Code link_class[];			// リンク種別
	extern Code bypass_flag[];			// バイパスフラグ
	extern Code caronly_flag[];			// 自動車専用道路フラグ
	extern Code road_width[];			// 道路幅員区分
	extern Code road_class[];			// 道路種別コード
	extern Code navi_class[];			// 経路種別コード
	extern Code wide_area[];			// 広域フラグ
	extern Code parking_class[];		// 駐車場種別
	extern Code parkinglink_class[];	// 駐車場リンク種別
	extern Code vics_class[];			// VICS区分
	extern Code segment_attr[];			// リンク内属性
	extern Code kubun_code[];			// 主従区分
	extern Code limit_speed[];			// 規制速度
	extern Code area_flag[];			// 細街路エリアフラグ

	extern Code signal_flag[];			// 信号機フラグ
	extern Code height_flag[];			// 道路高さフラグ

	extern Code dayofweek[];			// 曜日指定条件

	extern Code dirguide_class[];		// 地先種別
	extern Code guide_class[];			// 強制誘導案内コード

	extern HRESULT Convert(Code *cTable, long cSindyCode, long *cInCode);
	extern HRESULT Convert2(Code *cTable, long cInCode, long *cSindyCode);
	extern void wcs2mbs(BSTR bstrVal, std::string *cName);
	extern std::string SJIS2EUC(const char *cSjisString);
	extern std::string EUC2SJIS(const char *cEucString);
};


#endif	//___SINDY2INCODE_H___
