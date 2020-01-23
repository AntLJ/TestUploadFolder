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
// TimeReg.h: CTimeRegクラスのインターフェイス
// RoadCheck.SnwRCCheckTimeReg.h がベース
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

/**
 * @brief 時間規制クラス
 */
class CTimeReg
{
public:

	/// コンストラクタ
	CTimeReg(
		long startMonth,
		long startDay,
		long startHour,
		long startMin,
		long endMonth,
		long endDay,
		long endHour,
		long endMin,
		long weekCode
	);

	/// デストラクタ
	~CTimeReg();

	/// 比較演算子
	bool operator == (const CTimeReg& cCond) const;

	/// 論理チェック
	bool check() const;

	/// 規制条件が与えられた規制条件を含んでいるチェック
	bool contain(const CTimeReg& cCond) const;

	/// 規制条件が与えられた規制条件と交差するチェック
	bool cross(const CTimeReg& cCond) const;

	/// 通年チェック
	bool checkAllDay() const;

private:
	/// 正規化終了時間をセット
	void setNormalizedEndTime();

	/// 日付を含んでいる
	bool containDay(const CTimeReg& cCond) const;
	/// 時間を含んでいる
	bool containTime(const CTimeReg& cCond) const;
	/// 曜日を含んでいる
	bool containWeek(const CTimeReg& cCond) const;

	/// 日付が交差している
	bool crossDay(const CTimeReg& cCond) const;
	/// 時間が交差している
	bool crossTime(const CTimeReg& cCond) const;
	/// 曜日が交差している
	bool crossWeek(const CTimeReg& cCond) const;

	/// 曜日フラグの取得
	int getWeekFlag() const;

	/// 各月の最大日数の取得
	static int GetLastDay(long cMonth);

	long m_weekCode;   ///< 曜日コード
	long m_startMonth; ///< 開始月
	long m_startDay;   ///< 開始日
	long m_startHour;  ///< 開始時間
	long m_startMin;   ///< 開始分
	long m_endMonth;   ///< 終了月
	long m_endDay;     ///< 終了日
	long m_endHour;    ///< 終了時間
	long m_endMin;     ///< 終了分
	long eEndTime2;    ///< 正規化終了時間
	long eEndMonth2;   ///< 正規化終了月

};
