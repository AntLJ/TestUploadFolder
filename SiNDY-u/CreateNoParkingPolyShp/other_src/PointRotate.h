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

#ifndef	_POINTROTATE_H_
#define	_POINTROTATE_H_

#include <list>

class	PointRotate
{
private:
	struct	PtNum
	{
		long	m_lNum;
		double	m_dX;
		double	m_dY;

		PtNum () {
			m_lNum	= 0;
			m_dX	= 0.0;
			m_dY	= 0.0;
		}
		bool	operator<(const PtNum c_dDat ) const {
			bool	ret	= false;
			if( m_dX != c_dDat.m_dX ) {
				if( m_dX < c_dDat.m_dX ) {
					ret	= true;
				}
			}else
			if( m_dY != c_dDat.m_dY ) {
				if( m_dY < c_dDat.m_dY ) {
					ret	= true;
				}
			}else
			if( m_lNum != c_dDat.m_lNum ) {
				if( m_lNum < c_dDat.m_lNum ) {
					ret	= true;
				}
			}
			return	( ret );
		}
	};

public:
	PointRotate() {
	}
	~PointRotate() {
	}

	// “_—ñ‚ð‰ñ“]‚³‚¹‚Äˆê”ÔX,Y‚ª¬‚³‚¢‚Æ‚±‚ë‚É‚·‚é
	IGeometryPtr	ExecRotate	( IGeometryPtr&	c_ipGeo );
};



#endif // _POINTROTATE_H_
