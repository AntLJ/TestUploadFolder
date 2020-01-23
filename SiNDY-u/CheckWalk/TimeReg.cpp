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

// TimeReg.cpp: CTimeRegクラスの定義
// RoadCheck.SnwRCCheckTimeReg.h がベース
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeReg.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
CTimeReg::CTimeReg(
	long startMonth,
	long startDay,
	long startHour,
	long startMin,
	long endMonth,
	long endDay,
	long endHour,
	long endMin,
	long weekCode
)
{
	// 値のセット
	m_startMonth = startMonth;
	m_startDay = startDay;
	m_startHour = startHour;
	m_startMin = startMin;
	m_endMonth = endMonth;
	m_endDay = endDay;
	m_endHour = endHour;
	m_endMin = endMin;
	m_weekCode = weekCode;

	setNormalizedEndTime();
}

void CTimeReg::setNormalizedEndTime()
{
	eEndTime2 = m_endHour + (m_endHour < m_startHour ? 24 : 0);
	eEndMonth2 = m_endMonth + (m_endMonth < m_startMonth ? 12 : 0);
}

// デストラクタ

CTimeReg::~CTimeReg() {}

// 論理チェック

bool CTimeReg::check() const
{
	// エラーチェック(範囲チェック)
	if(m_startHour < 0 || m_startHour > 24) return(false);
	if(m_endHour < 0 || m_endHour > 24) return(false);

	if(m_startMin < 0 || m_startMin > 59) return(false);
	if(m_endMin < 0 || m_endMin > 59) return(false);

	if(m_startHour == 24) return(false);

	if(m_endHour == 24 && m_endMin != 0) return(false);
	
	if(m_endHour == 0 && m_endMin == 0) return(false);

	if(m_startMonth < 0 || m_startMonth > 12) return(false);
	if(m_endMonth < 0 || m_endMonth > 12) return(false);

	if((m_startMonth == 0 && m_endMonth != 0) || (m_startMonth != 0 && m_endMonth == 0))
		return(false);

	if(m_startDay < 1 || m_startDay > GetLastDay(m_startMonth)) return(false);
	if(m_endDay < 1 || m_endDay > GetLastDay(m_endMonth)) return(false);

	if(m_startHour == m_endHour && m_startMin == m_endMin)	return false;

	if((m_startMin % 10) != 0 && m_startMin != 15 && m_startMin != 45) return(false);
	if((m_endMin % 10) != 0 && m_endMin != 15 && m_endMin != 45) return(false);

	// 全日（＝通年）チェック
	if( m_weekCode == sindy::schema::day_code::kAlldays &&
		m_startHour == 0 && m_startMin == 0 && m_endHour == 24 && m_endMin == 0 &&
		m_startMonth == 1 && m_startDay == 1 && m_endMonth == 12 && m_endDay == 31)
		return(false);

	return(true);
}

// 曜日フラグの取得

int CTimeReg::getWeekFlag() const
{
	int aFlag = 0;
	switch(m_weekCode)
	{
	case  0: aFlag = 0xffffffff; break;
	case  1: aFlag = 0x11000000; break;
	case  2: aFlag = 0x00111111; break;
	case  3: aFlag = 0x10000000; break;
	case  4: aFlag = 0x01111111; break;
	case  5: aFlag = 0x00111110; break;
	case  6: aFlag = 0x04000000; break;
	case  7: aFlag = 0x00400000; break;
	case  8: aFlag = 0x00040000; break;
	case  9: aFlag = 0x00004000; break;
	case 10: aFlag = 0x00000400; break;
	case 11: aFlag = 0x00000040; break;
	case 12: aFlag = 0x00000004; break;
	case 13: aFlag = 0x11000001; break;
	case 14: aFlag = 0x00111110; break;
	case 15: aFlag = 0xffffffff; break;
	case 16: aFlag = 0x10111111; break;
	case 17: aFlag = 0x22000000; break;
	default: break;
	}
	return(aFlag);
}

// 条件を含んでいるチェック

bool CTimeReg::contain(const CTimeReg& cCond) const
{
	if(! containWeek(cCond)) return(false);
	if(! containDay(cCond)) return(false);
	if(! containTime(cCond)) return(false);
	return(true);
}

// 日付条件を含んでいるチェック

bool CTimeReg::containDay(const CTimeReg& cCond) const
{
	if(m_startMonth > cCond.m_startMonth) return(false);
	if(m_startMonth == cCond.m_startMonth && m_startDay > cCond.m_startDay) return(false);
	if(m_endMonth < cCond.m_endMonth) return(false);
	if(m_endMonth == cCond.m_endMonth && m_endDay < cCond.m_endDay) return(false);
	return(true);
}

// 時間を含んでいる

bool CTimeReg::containTime(const CTimeReg& cCond) const
{
	if(m_startHour > cCond.m_startHour) return(false);
	if(m_startHour == cCond.m_startHour && m_startMin > cCond.m_startMin) return(false);

	// 終了時間は正規化されたもので比較（bug 12019）
	// 例） 0:20 => 24:20
	if(m_endHour < cCond.eEndTime2) return(false);
	if(m_endHour == cCond.m_endHour && m_endMin < cCond.m_endMin) return(false);
	return(true);
}

// 曜日を含んでいる

bool CTimeReg::containWeek(const CTimeReg& cCond) const
{
	int aWeek1 = getWeekFlag();
	int aWeek2 = cCond.getWeekFlag();
	if(aWeek1 != (aWeek1 | aWeek2)) return(false);
	// 含んでる
	return(true);
}

// 条件の交差チェック

bool CTimeReg::cross(const CTimeReg& cCond) const
{
	if(! crossWeek(cCond)) return(false);
	if(! crossDay(cCond)) return(false);
	if(! crossTime(cCond)) return(false);
	return(true);
}

// 日付の交差チェック

bool CTimeReg::crossDay(const CTimeReg& cCond) const
{
	if((cCond.m_startMonth < m_startMonth || (cCond.m_startMonth == m_startMonth && cCond.m_startDay < m_startDay)) &&
	   (cCond.eEndMonth2 < m_startMonth || (cCond.m_endMonth == m_startMonth && cCond.m_endDay < m_startDay)))
	{
		return(false);
	}
	if((cCond.m_startMonth > eEndMonth2 || (cCond.m_startMonth == m_endMonth && cCond.m_startDay > m_endDay)) &&
	   (cCond.eEndMonth2 > eEndMonth2 || (cCond.m_endMonth == m_endMonth && cCond.m_endDay > m_endDay)))
	{
		return(false);
	}
	return(true);
}

// 時間の交差チェック

bool CTimeReg::crossTime(const CTimeReg& cCond) const
{

	if((cCond.m_startHour < m_startHour || (cCond.m_startHour == m_startHour && cCond.m_startMin < m_startMin)) &&
	   (cCond.eEndTime2 < m_startHour || (cCond.m_endHour == m_startHour && cCond.m_endMin <= m_startMin)))
	{
		return(false);
	}
	if((cCond.m_startHour > eEndTime2 || (cCond.m_startHour == m_endHour && cCond.m_startMin >= m_endMin)) &&
	   (cCond.eEndTime2 > eEndTime2 || (cCond.m_endHour == m_endHour && cCond.m_endMin > m_endMin)))
	{
		return(false);
	}
	return(true);
}

// 曜日の交差チェック

bool CTimeReg::crossWeek(const CTimeReg& cCond) const
{
	int aWeek1 = getWeekFlag();
	int aWeek2 = cCond.getWeekFlag();
	if((aWeek1 & aWeek2) != 0) return(true);
	return(false);
}

// 各月の最大日数を返す

int CTimeReg::GetLastDay(long cMon)
{
	switch(cMon)
	{
	case  0: return(31);
	case  1: return(31);
	case  2: return(29);
	case  3: return(31);
	case  4: return(30);
	case  5: return(31);
	case  6: return(30);
	case  7: return(31);
	case  8: return(31);
	case  9: return(30);
	case 10: return(31);
	case 11: return(30);
	case 12: return(31);
	default: return(-1);
	}
	return(-1);
}

// 比較演算子
bool CTimeReg::operator == (const CTimeReg& cCond) const
{
	if ( m_weekCode != cCond.m_weekCode ) return false;
	if ( m_startHour != cCond.m_startHour ) return false;
	if ( m_startMin != cCond.m_startMin ) return false;
	if ( m_endHour != cCond.m_endHour ) return false;
	if ( m_endMin != cCond.m_endMin ) return false;
	if ( m_startMonth != cCond.m_startMonth ) return false;
	if ( m_startDay != cCond.m_startDay ) return false;
	if ( m_endMonth != cCond.m_endMonth ) return false;
	if ( m_endDay != cCond.m_endDay ) return false;

	return true;
}

bool CTimeReg::checkAllDay(void) const
{
	// 1/1～12/31だけ通年OK、その他はダメ
	// 例：3/10～3/9 NG
	// 例：4/1～3/31 NG
	// 例：0/1～0/31 NG
	bool bRet = true;

	int startLastDay = GetLastDay(m_startMonth);
	int endLastDay = GetLastDay(m_endMonth);
	if(0 < startLastDay && 0 < endLastDay && 0 < m_startDay && m_startDay <= startLastDay && 0 < m_endDay && m_endDay <= endLastDay) {
		if(m_startMonth == 0 && m_endMonth == 0) {	// 0月規制
			if(m_startDay == 1 && m_endDay == endLastDay)
				bRet = false;
			else if((m_startDay - m_endDay) == 1)
				bRet = false;
		}
		else if(m_startMonth != 0 && m_endMonth != 0) {
			if(m_startDay == 1 && m_endDay == endLastDay && m_startMonth != 1) {
				if((m_startMonth - m_endMonth) == 1)
					bRet = false;
			}
			else if(m_startDay != 1 && (m_startDay - m_endDay) == 1 && (m_startMonth - m_endMonth) == 0)
				bRet = false;
		}
	}

	return bRet;
}
