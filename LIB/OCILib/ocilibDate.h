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

#ifndef	__OCILIBDATE_H_
#define	__OCILIBDATE_H_

#include "stdafx.h"

class ocilibDate
{
public:
	ocilibDate(){};
	ocilibDate( sb2 year, sb2 month, ub1 day, ub1 hour = 0, ub1 minute = 0, ub1 second = 0 )
	{
		m_Year = year;
		m_Month = month;
		m_Day = day;
		m_Hour = hour;
		m_Minute = minute;
		m_Second = second;
	}

	ocilibDate( const OCIDate& ociDate ) { operator = (ociDate); };

	inline ocilibDate& operator =( const OCIDate& ociDate )
	{
		m_Year = ociDate.OCIDateYYYY;
		m_Month = ociDate.OCIDateMM;
		m_Day = ociDate.OCIDateDD;
		m_Hour = ociDate.OCIDateTime.OCITimeHH;
		m_Minute = ociDate.OCIDateTime.OCITimeMI;
		m_Second = ociDate.OCIDateTime.OCITimeSS;

		return (*this);
	};

	virtual ~ocilibDate(){};

public:
	inline sb2 Year() const { return m_Year; };
	inline sb2 Mon() const { return m_Month; };
	inline ub1 Day() const { return m_Day; };
	inline ub1 Hour() const { return m_Hour; };
	inline ub1 Minute() const { return m_Minute; };
	inline ub1 Second() const { return m_Second; };

	inline void Year( sb2 year ) { m_Year = year; };
	inline void Mon( sb2 month ) { m_Month = month; };
	inline void Day( ub1 day ) { m_Day = day; };
	inline void Hour( ub1 hour ) { m_Hour = hour; };
	inline void Minute( ub1 minute ) { m_Minute = minute; };
	inline void Second( ub1 second ) { m_Second = second; };

	inline void set( OCIDate& ociDate ) const
	{
		ociDate.OCIDateYYYY = m_Year;
		ociDate.OCIDateMM = static_cast<ub1>(m_Month);
		ociDate.OCIDateDD = m_Day;
		ociDate.OCIDateTime.OCITimeHH = m_Hour;
		ociDate.OCIDateTime.OCITimeMI = m_Minute;
		ociDate.OCIDateTime.OCITimeSS = m_Second;
	};

private:
	sb2			m_Year;
	sb2			m_Month;
	ub1			m_Day;
	ub1			m_Hour;
	ub1			m_Minute;
	ub1			m_Second;
};

#endif // __OCILIBDATE_H_
