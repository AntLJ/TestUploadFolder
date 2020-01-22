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

#ifndef __LEVELOID_H_
#define __LEVELOID_H_
using namespace std;

class CLevelOID : public CButton
{
public:
	CLevelOID() : m_lOID(-1), m_lAngle(-1), m_pLQRowDef(NULL), m_hBitmap(NULL), m_nDrawTimes(0) {};

	~CLevelOID(){
		if( IsWindow() )
		{
			HBITMAP hBitmap = GetBitmap();
			if( hBitmap != NULL )
				DeleteObject( hBitmap );

			DestroyWindow();
			m_hWnd = NULL;
		}
	};
public:
	LONG			m_lOID; 
	map<LONG, LONG> m_mapLevelAngle;	//!< ソート用
	LONG			m_lAngle;			//!< 矢印向き判定用
	FEATUREDEF*		m_pLQRowDef;
	HBITMAP			m_hBitmap;
	INT				m_nDrawTimes;
};

inline bool __stdcall operator<(const CLevelOID& c1, const CLevelOID& c2)
{
	for( map<LONG, LONG>::const_iterator itC1 = c1.m_mapLevelAngle.begin(); itC1 != c1.m_mapLevelAngle.end(); itC1++ )
	{
		for( map<LONG, LONG>::const_iterator itC2 = c2.m_mapLevelAngle.begin(); itC2 != c2.m_mapLevelAngle.end(); itC2++ )
		{
			if( ( itC1->first == itC2->first ) && ( itC1->second != itC2->second ) )
			{
				if( itC1->second < itC2->second )
					ATLTRACE(_T("CLevelOID::operator < : LEVEL %d ANGLE %d < LEVEL %d ANGLE %d\n"), itC1->first, itC1->second, itC2->first, itC2->second );
				else
					ATLTRACE(_T("CLevelOID::operator < : LEVEL %d ANGLE %d > LEVEL %d ANGLE %d\n"), itC1->first, itC1->second, itC2->first, itC2->second );

				return ( itC1->second < itC2->second );
			}
		}
	}
	return false;
};
inline bool __stdcall operator>(const CLevelOID& c1, const CLevelOID& c2)
{
	for( map<LONG, LONG>::const_iterator itC1 = c1.m_mapLevelAngle.begin(); itC1 != c1.m_mapLevelAngle.end(); itC1++ )
	{
		for( map<LONG, LONG>::const_iterator itC2 = c2.m_mapLevelAngle.begin(); itC2 != c2.m_mapLevelAngle.end(); itC2++ )
		{
			if( ( itC1->first == itC2->first ) && ( itC1->second != itC2->second ) )
			{
				if( itC1->second < itC2->second )
					ATLTRACE(_T("CLevelOID::operator < : LEVEL %d ANGLE %d < LEVEL %d ANGLE %d\n"), itC1->first, itC1->second, itC2->first, itC2->second );
				else
					ATLTRACE(_T("CLevelOID::operator < : LEVEL %d ANGLE %d > LEVEL %d ANGLE %d\n"), itC1->first, itC1->second, itC2->first, itC2->second );

				return ( itC1->second > itC2->second );
			}
		}
	}
	return false;
};
inline bool __stdcall operator==(const CLevelOID& c1, const CLevelOID& c2)
{
	bool bEqual = false;

	for( map<LONG, LONG>::const_iterator itC1 = c1.m_mapLevelAngle.begin(); itC1 != c1.m_mapLevelAngle.end(); itC1++ )
	{
		for( map<LONG, LONG>::const_iterator itC2 = c2.m_mapLevelAngle.begin(); itC2 != c2.m_mapLevelAngle.end(); itC2++ )
		{
			if( itC1->first == itC2->first )
			{
				if( itC1->second > 0 && itC2->second > 0 )
					bEqual = ( itC1->second == itC2->second );
				else
					bEqual = false;
			}
		}
	}
	return bEqual;
};

#endif // ifdef __LEVELOID_H_
