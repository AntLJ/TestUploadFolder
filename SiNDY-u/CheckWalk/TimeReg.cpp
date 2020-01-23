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

// TimeReg.cpp: CTimeReg�N���X�̒�`
// RoadCheck.SnwRCCheckTimeReg.h ���x�[�X
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeReg.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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
	// �l�̃Z�b�g
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

// �f�X�g���N�^

CTimeReg::~CTimeReg() {}

// �_���`�F�b�N

bool CTimeReg::check() const
{
	// �G���[�`�F�b�N(�͈̓`�F�b�N)
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

	// �S���i���ʔN�j�`�F�b�N
	if( m_weekCode == sindy::schema::day_code::kAlldays &&
		m_startHour == 0 && m_startMin == 0 && m_endHour == 24 && m_endMin == 0 &&
		m_startMonth == 1 && m_startDay == 1 && m_endMonth == 12 && m_endDay == 31)
		return(false);

	return(true);
}

// �j���t���O�̎擾

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

// �������܂�ł���`�F�b�N

bool CTimeReg::contain(const CTimeReg& cCond) const
{
	if(! containWeek(cCond)) return(false);
	if(! containDay(cCond)) return(false);
	if(! containTime(cCond)) return(false);
	return(true);
}

// ���t�������܂�ł���`�F�b�N

bool CTimeReg::containDay(const CTimeReg& cCond) const
{
	if(m_startMonth > cCond.m_startMonth) return(false);
	if(m_startMonth == cCond.m_startMonth && m_startDay > cCond.m_startDay) return(false);
	if(m_endMonth < cCond.m_endMonth) return(false);
	if(m_endMonth == cCond.m_endMonth && m_endDay < cCond.m_endDay) return(false);
	return(true);
}

// ���Ԃ��܂�ł���

bool CTimeReg::containTime(const CTimeReg& cCond) const
{
	if(m_startHour > cCond.m_startHour) return(false);
	if(m_startHour == cCond.m_startHour && m_startMin > cCond.m_startMin) return(false);

	// �I�����Ԃ͐��K�����ꂽ���̂Ŕ�r�ibug 12019�j
	// ��j 0:20 => 24:20
	if(m_endHour < cCond.eEndTime2) return(false);
	if(m_endHour == cCond.m_endHour && m_endMin < cCond.m_endMin) return(false);
	return(true);
}

// �j�����܂�ł���

bool CTimeReg::containWeek(const CTimeReg& cCond) const
{
	int aWeek1 = getWeekFlag();
	int aWeek2 = cCond.getWeekFlag();
	if(aWeek1 != (aWeek1 | aWeek2)) return(false);
	// �܂�ł�
	return(true);
}

// �����̌����`�F�b�N

bool CTimeReg::cross(const CTimeReg& cCond) const
{
	if(! crossWeek(cCond)) return(false);
	if(! crossDay(cCond)) return(false);
	if(! crossTime(cCond)) return(false);
	return(true);
}

// ���t�̌����`�F�b�N

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

// ���Ԃ̌����`�F�b�N

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

// �j���̌����`�F�b�N

bool CTimeReg::crossWeek(const CTimeReg& cCond) const
{
	int aWeek1 = getWeekFlag();
	int aWeek2 = cCond.getWeekFlag();
	if((aWeek1 & aWeek2) != 0) return(true);
	return(false);
}

// �e���̍ő������Ԃ�

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

// ��r���Z�q
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
	// 1/1�`12/31�����ʔNOK�A���̑��̓_��
	// ��F3/10�`3/9 NG
	// ��F4/1�`3/31 NG
	// ��F0/1�`0/31 NG
	bool bRet = true;

	int startLastDay = GetLastDay(m_startMonth);
	int endLastDay = GetLastDay(m_endMonth);
	if(0 < startLastDay && 0 < endLastDay && 0 < m_startDay && m_startDay <= startLastDay && 0 < m_endDay && m_endDay <= endLastDay) {
		if(m_startMonth == 0 && m_endMonth == 0) {	// 0���K��
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
