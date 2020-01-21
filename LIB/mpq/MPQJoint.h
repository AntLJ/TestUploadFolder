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
**	MPQJoint.h
**
**		MapCube-Network joint info handling class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2003/9/3	çÏê¨'
*/

#ifndef	___MPQ_JOINT___
#define	___MPQ_JOINT___

#include "MPQ.h"
#include <set>

class MPQJoint
{
public:
	struct Joint
	{
		int		eID1;
		int		eID2;
		int		eFrom;
		int		eTo;
		int		eAttr;

		Joint(void) { }
		Joint(int cID1, int cID2) : eID1(cID1), eID2(cID2) { }

		bool operator < (const Joint& cJoint) const
		{
			if(eID1 < cJoint.eID1) return true;
			if(eID1 > cJoint.eID1) return false;
			return eID2 < cJoint.eID2;
		}
	};

private:
	struct Info
	{
		int		eAddr;
		int		eSize;
		int		eCount;
	};

	const char*		mExt;
	vector<MPQ::Attr*>	mAttr;
	multiset<Joint>		mJoint;

	bool	readAttr (istream& is, const Info& cInfo);
	bool	readJoint(istream& is, const Info& cInfo);

	bool	writeAttr (ostream& os, Info* cInfo);
	bool	writeJoint(ostream& os, Info* cInfo);

public:
	MPQJoint(const char* cExt="mpq") : mExt(cExt) { }
	~MPQJoint() { }

	bool	read (const char* cDir, int cMesh);
	bool	write(const char* cDir, int cMesh);
	void	clear(void);

	void	addAttr (MPQ::Attr* cAttr)    { mAttr.push_back(cAttr); }
	void	addJoint(const Joint& cJoint) { mJoint.insert(cJoint); }

	int		sizeAttr (void) const { return mAttr .size(); }
	int		sizeJoint(void) const { return mJoint.size(); }

	typedef vector<MPQ::Attr*>::const_iterator	AIter;
	AIter	beginAttr(void) const { return mAttr.begin(); }
	AIter	endAttr  (void) const { return mAttr.end  (); }
	MPQ::Attr*	getAttr(int cID);

	typedef multiset<Joint>::const_iterator	JIter;
	typedef pair<JIter, JIter>				JRange;
	JIter	beginJoint(void) const { return mJoint.begin(); }
	JIter	endJoint  (void) const { return mJoint.end  (); }
	JRange	rangeJoint(int cID1, int cID2) const { return mJoint.equal_range(Joint(cID1, cID2)); }
};

#endif	// ___MPQ_JOINT___

