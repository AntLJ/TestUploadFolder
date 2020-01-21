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

// SINDY2GS.h: SINDY2GS クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2GS_H__5A3385F9_C91D_4C4F_8511_8613F3CAE6DB__INCLUDED_)
#define AFX_SINDY2GS_H__5A3385F9_C91D_4C4F_8511_8613F3CAE6DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"

class SINDY2GS : public SINDY2INHWY  
{
public:
	enum EType
	{
		gs				= 0,	/// ガソリンスタンド情報
		multiple_store	= 1		/// チェーン店情報
	};

	enum EFormat
	{
		gs_old	= 1,			/// 旧フォーマット(5.50以前)
		gs_new	= 2				/// 通常のフォーマット(5.61以降)
	};

	enum EDirection				/// 出力する上下線フラグの定義
	{
		direction_down	= 0,	/// 下り
		direction_up	= 1		/// 上り
	};

	struct AccessStore
	{
		long		eStoreCode;		/// チェーン店マスターコード

		long		eStartHour;		/// 営業開始時間
		long		eStartMin;		/// 営業開始分
		long		eEndHour;		/// 営業終了時間
		long		eEndMin;		/// 営業終了分

		bool		eSunday;		/// 曜日営業フラグ
		bool		eMonday;
		bool		eTuesday;
		bool		eWednesday;
		bool		eThursday;
		bool		eFriday;
		bool		eSaturday;

		bool		eBon;			/// 期間営業フラグ
		bool		eYearEnd;
		bool		eNewYear;
		bool		eGoldenWeek;

		bool		eHoliday;		/// 祝祭日営業フラグ
		EType		eStoreType;		/// 店舗種別コード
	};

	SINDY2GS(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2GS();

	virtual HRESULT Convert();
	virtual HRESULT Write(std::string cFileName, EType cType, EFormat cFormat);

	static HRESULT GetAccessStore(SINDY2INHWYINFO* cHwyInfo, IFeaturePtr cpAccessPoint, std::vector<AccessStore>* cpAccessStoreList);

public:
	struct GS
	{
		long		eRoadCode;	// 路線コード
		long		eSequence;	// 路線内シーケンス番号
		std::string	eName;		// 施設名称
		long		eLineDir;	// 上下線属性
		long		eGsType;	// ＧＳの種類

		std::vector<AccessStore>	eAccessStoreInfo;	/// ガソリンスタンドorチェーン店情報の列

		GS() : eRoadCode(0), eSequence(0), eName(""), eLineDir(0), eGsType(0) {}
		virtual ~GS() {}
	};

	typedef std::vector<GS> GsList;
private:
	GsList mGsList;

public:
	typedef GsList::const_iterator Iter;

	/// データ列の先頭を指すconst_iteratorを返す
	Iter begin() const { return mGsList.begin(); }

	/// データ列の末尾を指すconst_iteratorを返す
	Iter end() const { return mGsList.end(); }

	/// データ列のサイズを返す
	std::size_t size() const { return mGsList.size(); }
};

#endif // !defined(AFX_SINDY2GS_H__5A3385F9_C91D_4C4F_8511_8613F3CAE6DB__INCLUDED_)
