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
**	MPQ.h
**
**		MapCube Handling library  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2003/8/5	�쐬
*/

#ifndef	___MPQ___
#define	___MPQ___

#include "ZCore.h"
#include <vector>
#include <map>
#include <string>

struct MPQIO;

class MPQ
{
public:
	// �Ǘ����
	struct Header
	{
		int		eVer;		//!< �t�H�[�}�b�g�o�[�W����
		int		eMesh;		//!< ���b�V���R�[�h
		int		eCreate;	//!< �t�@�C���쐬��
		int		eEdit;		//!< �ŏI�ҏW��
		int		eIN;		//!< ���h�m�쐬��
	};

public:
	// �����l
	enum { INVALID = (int) 0x80000000 };

public:
	// ���E�����l
	struct LR
	{
		int		eLeft;		//!< ���i���ԑ��j
		int		eRight;		//!< �E�i�Ό��ԑ��j

		LR(void) : eLeft(INVALID), eRight(INVALID) { }
		LR(int cLeft, int cRight) : eLeft(cLeft), eRight(cRight) { }

		bool	valid(void) const { return (INVALID!=eLeft) || (INVALID!=eRight); }
	};

public:
	//! MapCube ����
	class Attr
	{
		const int	mID;

		Attr(const Attr& cAttr);
		Attr&	operator = (const Attr& cAttr);

	public:
		int		eHeight;	//!< �n�ʂ���̑��΍� (ver3�ł́A�}�b�`���O�p������)
		int		eSea;		//!< �C����΍� (ver3�ł́A�`��p����)
		LR		eLane;		//!< �Ԑ���
		LR		eLine;		//!< �Ԑ����E���^�C�v
		LR		eFence;		//!< �����^�C�v
		LR		eSide;		//!< �H�����^�C�v
		int		eSurface;	//!< �H�ʃ^�C�v
		int		eLamp;		//!< �X�H���^�C�v
		int		eCenter;	//!< ���������у^�C�v
		int		eValue;
		string	eStruct;	//!< ���A���X�g���N�`��������
		string	eMemo;		//!< �R�����g������

		Attr(int cID);

		int		getID(void) const { return mID; }
		bool	valid(void) const;
	};

public:
	//! ��ԓ_
	struct Point
	{
		int		eX;		//!< �w���W
		int		eY;		//!< �x���W
		int		eID;	//!< �h�m�m�[�h�h�c
		bool	eIN;	//!< �ǉ���ԓ_�t���O�i�^�F�����炠��A�U�F�ǉ��j
		Attr*	eAttr;	//!< MapCube ����

		Point(void);
		Point(int cX, int cY);
	};

public:
	struct Link
	{
		enum Way
		{
			Free	= 0,	//!< �K������
			Ord		= 1,	//!< ����ʍs
			Stop	= 2		//!< �ʍs�֎~
		};

		int		eClass;			//!< �\�����
		Way		eWay;			//!< ����ʍs
		int		eValue;
		vector<Point>	eShape;	//!< ��ԓ_��

		Link(void);
	};

private:
	MPQIO*	mIO;

protected:
	Header			mHeader;
	vector<Link*>	mLink;
	vector<Attr*>	mAttr;

public:
	MPQ(MPQIO* cIO);
	virtual ~MPQ();

	bool	read (const char* cDir, int cMesh);
	bool	read (const char* cDir);
	bool	write(const char* cDir, int cMesh);

	int		getVer   (void) const { return mHeader.eVer; }
	int		getMesh  (void) const { return mHeader.eMesh; }
	int		getCreate(void) const { return mHeader.eCreate; }
	int		getEdit  (void) const { return mHeader.eEdit; }
	int		getINVer (void) const { return mHeader.eIN; }

	void	setMesh  (int cMesh) { mHeader.eMesh   = cMesh; }
	void	setCreate(int cDate) { mHeader.eCreate = cDate; }
	void	setEdit  (int cDate) { mHeader.eEdit   = cDate; }
	void	setINVer (int cDate) { mHeader.eIN     = cDate; }

	typedef vector<Link*>::iterator	Iter;
	Iter	begin(void) { return mLink.begin(); }
	Iter	end  (void) { return mLink.end  (); }

	virtual void	addLink(Link* cLink)	= 0;
	virtual Attr*	newAttr(void)			= 0;

	virtual Attr*	cloneAttr(const Attr& cAttr);
};

#endif	// ___MPQ___

