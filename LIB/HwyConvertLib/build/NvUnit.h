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

/*
**	NvUnit.h
**
**		navi98 unitcode class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			1997/12/26	navi97 Ç©ÇÁà¯åp
**			1998/2/5	enum Level Çí«â¡
**						append getDivideCount/getUnitSize (non static)
**			1998/2/25	append GetOrigin
**			1998/3/17	ÇkÇUÇÃÉÅÉbÉVÉÖÉRÅ[ÉhÇ™ê≥ÇµÇ≠Ç»Ç¢ÉoÉOÇèCê≥
*/

#ifndef	___NAVI98_UNIT_CODE___
#define	___NAVI98_UNIT_CODE___

#include "NvObject.h"

// ÇjÇhÇvÇhÉÅÉbÉVÉÖÉRÅ[Éh
class KiwiMesh
{
public:
	struct Peace
	{
		short	eDeg	: 9;	// ìx
		ushort	eMin	: 7;	// ï™
		ushort	eSec;			// ïb
	};

public:
	Peace	eLon;
	Peace	eLat;

public:
	KiwiMesh(void) { }
	KiwiMesh(int cLon, int cLat);
};

class NvUnit
{
	static int	mOrigin;

public:
	enum Level {
		Level0	= 0,
		Level1	= 1,
		Level2	= 2,
		Level3	= 3,
		Level4	= 4,
		Level5	= 5,
		Level6	= 6,

		L0	= 0,
		L1	= 1,
		L2	= 2,
		L3	= 3,
		L4	= 4,
		L5	= 5,
		L6	= 6
	};

public:
	int		eMesh;
	int		eLevel;
	short	eX;
	short	eY;

public:
	NvUnit(void) { }
	NvUnit(int cMesh, int cLevel, int cX=0, int cY=0) : eMesh(cMesh), eLevel(cLevel), eX(cX), eY(cY) { }

	int		compare(const NvUnit& cUnitCode) const;
	bool	operator == (const NvUnit& cUnitCode) const { return compare(cUnitCode) == 0; }
	bool	operator <  (const NvUnit& cUnitCode) const { return compare(cUnitCode) <  0; }
	bool	operator >  (const NvUnit& cUnitCode) const { return compare(cUnitCode) >  0; }
	bool	operator != (const NvUnit& cUnitCode) const { return compare(cUnitCode) != 0; }

	NvUnit&	move (int cDeltaX, int cDeltaY) { *this = getMoved(cDeltaX, cDeltaY); return *this; }
	NvUnit&	up   (void) { *this = getUp();   return *this; }
	NvUnit&	down (void) { *this = getDown(); return *this; }
	NvUnit&	level(int cLevel) { *this = getLevel(cLevel); return *this; }

	NvUnit	getMoved(int cDeltaX, int cDeltaY) const;
	NvUnit	getUp   (void) const;
	NvUnit	getDown (void) const;
	NvUnit	getLevel(int cLevel) const;

	int			getMesh(void)        const;
	KiwiMesh	getKiwiMesh(void)    const;
	int			getDivideCount(void) const { return GetDivideCount(eLevel); }
	int			getUnitSize(void)    const { return GetUnitSize(eLevel); }

	int		getIndex(void) const;
	NvUnit&	setIndex(int cIndex);

	ZPoint	getDiff (const NvUnit& cUnitcode) const;
	int		getDiffX(const NvUnit& cUnitcode) const;
	int		getDiffY(const NvUnit& cUnitcode) const;

	ZPoint	getUniqValue(void) const;
	int		getUniqX(void) const;
	int		getUniqY(void) const;

	char*	getDirectory(char* cBuffer) const;
	char*	getFilebody (char* cBuffer) const;
	void	createReadDirectory (const char* cDirectory, char* cDirname)  const;
	void	createReadFilename  (const char* cDirectory, char* cFilename) const;
	bool	createWriteDirectory(const char* cDirectory, char* cDirname)  const;
	bool	createWriteFilename (const char* cDirectory, char* cFilename) const;

	char*	getPackDir (char* cBuffer) const;
	char*	getPackFile(char* cBuffer) const;
	void	createReadPackDir  (const char* cDirectory, char* cDirname)  const;
	void	createReadPackFile (const char* cDirectory, char* cFilename) const;
	bool	createWritePackDir (const char* cDirectory, char* cDirname)  const;
	bool	createWritePackFile(const char* cDirectory, char* cFilename) const;

	static int	GetDivideCount(int cLevel);
	static int	GetUnitSize(int cLevel);
	static int	GetOrigin(void)        { return mOrigin; }
	static void	SetOrigin(int cOrigin) { assert(0 == cOrigin%100); mOrigin = cOrigin; }

	friend ostream& operator << (ostream& os, const NvUnit& cUnit);
};

#endif	// ___NAVI98_UNIT_CODE___

