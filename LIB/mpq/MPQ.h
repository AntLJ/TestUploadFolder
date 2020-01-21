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
**			2003/8/5	作成
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
	// 管理情報
	struct Header
	{
		int		eVer;		//!< フォーマットバージョン
		int		eMesh;		//!< メッシュコード
		int		eCreate;	//!< ファイル作成日
		int		eEdit;		//!< 最終編集日
		int		eIN;		//!< 元ＩＮ作成日
	};

public:
	// 無効値
	enum { INVALID = (int) 0x80000000 };

public:
	// 左右属性値
	struct LR
	{
		int		eLeft;		//!< 左（自車側）
		int		eRight;		//!< 右（対向車側）

		LR(void) : eLeft(INVALID), eRight(INVALID) { }
		LR(int cLeft, int cRight) : eLeft(cLeft), eRight(cRight) { }

		bool	valid(void) const { return (INVALID!=eLeft) || (INVALID!=eRight); }
	};

public:
	//! MapCube 属性
	class Attr
	{
		const int	mID;

		Attr(const Attr& cAttr);
		Attr&	operator = (const Attr& cAttr);

	public:
		int		eHeight;	//!< 地面からの相対高 (ver3では、マッチング用高さゐ)
		int		eSea;		//!< 海抜絶対高 (ver3では、描画用高さ)
		LR		eLane;		//!< 車線数
		LR		eLine;		//!< 車線境界線タイプ
		LR		eFence;		//!< 高欄タイプ
		LR		eSide;		//!< 路肩線タイプ
		int		eSurface;	//!< 路面タイプ
		int		eLamp;		//!< 街路灯タイプ
		int		eCenter;	//!< 中央分離帯タイプ
		int		eValue;
		string	eStruct;	//!< リアルストラクチャ文字列
		string	eMemo;		//!< コメント文字列

		Attr(int cID);

		int		getID(void) const { return mID; }
		bool	valid(void) const;
	};

public:
	//! 補間点
	struct Point
	{
		int		eX;		//!< Ｘ座標
		int		eY;		//!< Ｙ座標
		int		eID;	//!< ＩＮノードＩＤ
		bool	eIN;	//!< 追加補間点フラグ（真：元からある、偽：追加）
		Attr*	eAttr;	//!< MapCube 属性

		Point(void);
		Point(int cX, int cY);
	};

public:
	struct Link
	{
		enum Way
		{
			Free	= 0,	//!< 規制無し
			Ord		= 1,	//!< 一方通行
			Stop	= 2		//!< 通行禁止
		};

		int		eClass;			//!< 表示種別
		Way		eWay;			//!< 一方通行
		int		eValue;
		vector<Point>	eShape;	//!< 補間点列

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

