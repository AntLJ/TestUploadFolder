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

// STL
#include <map>

// useful_headers
#include <TDC/useful_headers/tstring.h>

// boost
#include <boost/assign.hpp>



/**
 *	@brief エラー定義
 */
namespace err_def
{
	/**
	 *	@brief エラーコード
	 */
	enum Code
	{
		kErrNotLinkedStation             = 101,		//!< [ERROR] 駅情報が紐付けられていない
		kErrPolygonCOutOfValue           = 102,		//!< [ERROR] ポリゴン種別コードがコード値ドメイン範囲外
		kErrOutOfLowPlatformShape        = 103,		//!< [ERROR] ポリゴンがプラットフォーム全体ポリゴンの外に位置している
		kErrOutOfMidZoomStation          = 104,		//!< [ERROR] ポリゴンが駅簡易ポリゴンの外に位置している
		kErrLinkedNotExistPolygon        = 105,		//!< [ERROR] 存在しない駅ポリゴンへの駅情報紐付けレコードが存在する
		kErrLinkedNotExistStation        = 106,		//!< [ERROR] 存在しない駅への駅情報紐付けレコードが存在する
		kErrOutOfSameIdLowPlatformShape  = 107,		//!< [ERROR] ポリゴンが、同じ駅IDのプラットフォーム全体ポリゴンを持たないか、外に位置している
		kErrOutOfSameIdMidZoomStation    = 108,		//!< [ERROR] ポリゴンが、同じ駅IDの駅簡易ポリゴンを持たないか、外に位置している
		kWarnOutOfHighZoomStation        = 201,		//!< [WARN]  ポリゴンが駅詳細ポリゴンの外に位置している
		kWarnOutOfMidZoomStation         = 202,		//!< [WARN]  ポリゴンが駅簡易ポリゴンの外に位置している
		kWarnOutOfSameIdHighZoomStation  = 203,		//!< [WARN]  ポリゴンが、同じ駅IDの駅詳細ポリゴンを持たないか、外に位置している
		kWarnOutOfSameIdMidZoomStation   = 204		//!< [WARN]  ポリゴンが、同じ駅IDの駅簡易ポリゴンを持たないか、外に位置している
	};


	/**
	 *	@brief エラー定義
	 */
	const std::map<Code, uh::tstring> errDef = 
								boost::assign::map_list_of
										( kErrNotLinkedStation,             _T("駅情報が紐付けられていない") )
										( kErrPolygonCOutOfValue,           _T("ポリゴン種別コードがコード値ドメイン範囲外") )
										( kErrOutOfLowPlatformShape,        _T("ポリゴンがプラットフォーム全体ポリゴンの外に位置している") )
										( kErrOutOfMidZoomStation,          _T("ポリゴンが駅簡易ポリゴンの外に位置している") )
										( kErrLinkedNotExistPolygon,        _T("存在しない駅ポリゴンへの駅情報紐付けレコードが存在する") )
										( kErrLinkedNotExistStation,        _T("存在しない駅への駅情報紐付けレコードが存在する") )
										( kErrOutOfSameIdLowPlatformShape,  _T("ポリゴンが、同じ駅IDのプラットフォーム全体ポリゴンを持たないか、外に位置している") )
										( kErrOutOfSameIdMidZoomStation,    _T("ポリゴンが、同じ駅IDの駅簡易ポリゴンを持たないか、外に位置している") )
										( kWarnOutOfHighZoomStation,        _T("ポリゴンが駅詳細ポリゴンの外に位置している") )
										( kWarnOutOfMidZoomStation,         _T("ポリゴンが駅簡易ポリゴンの外に位置している") )
										( kWarnOutOfSameIdHighZoomStation,  _T("ポリゴンが、同じ駅IDの駅詳細ポリゴンを持たないか、外に位置している") )
										( kWarnOutOfSameIdMidZoomStation,   _T("ポリゴンが、同じ駅IDの駅簡易ポリゴンを持たないか、外に位置している") )
										;

	/// エラーコードとエラーメッセージのペア
	typedef std::pair<int, uh::tstring> ErrPair;

	/**
 	 *	@brief エラー情報を返す
	 *  @param code [in] エラーコード
	 *  @return エラー情報( key: エラーコード, value: エラーメッセージ )
	 */
	inline ErrPair err( Code code )
	{
		_ASSERTE( errDef.count(code) != 0 );
		return *errDef.find(code);
	}

} // err_def
