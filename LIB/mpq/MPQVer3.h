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
** MPQVer3.h
**
**		MPQ IO for format ver.3 class header file
**
**		Programmed by Y.Chinen
**
**      EDITION:
**			2008/10/30	作成
*/

#ifndef ___MPQ_VER3___
#define ___MPQ_VER3___

#include "MPQVer1.h"


class MPQVer3 : public MPQVer1
{
public:
	struct Info
	{
		unsigned	eRes_			: 10;
		unsigned	eDeletion_End	: 1;	//!< 削除対象区間終端フラグ
		unsigned	eDeletion_Sct	: 1;	//!< 削除対象フラグ

		unsigned	eTunnel_Res		: 2;
		unsigned	eTunnel_End		: 1;    //!< トンネル区間終端フラグ
		unsigned	eTunnel_Sct		: 1;    //!< トンネル区間フラグ

		unsigned	eComment_Res	: 4;
		unsigned	eComment_End	: 1;	//!< コメント区間終端フラグ
		
		unsigned	eObject_Res		: 1;
		unsigned	eObject_End		: 1;    //!< 専用オブジェクト終端フラグ
		unsigned	eObject_Sct		: 1;    //!< 専用オブジェクトフラグ

		unsigned	eHeight_Res		: 2;
		unsigned	eHeight_MEnd	: 1;	//!< マッチング用高さ区間終端フラグ
		unsigned	eHeight_MFlag	: 1;	//!< マッチング用高さフラグ
		unsigned	eHeight_GType	: 1;    //!< 地盤高取得タイプ
		unsigned	eHeight_GFlag	: 1;    //!< 地盤高取得フラグ
		unsigned	eHeight_DEnd	: 1;    //!< 描画用高さ区間終端フラグ
		unsigned	eHeight_DSct	: 1;    //!< 描画用高さフラグ

		Info(void);
	};


public:
	MPQVer3(void) { }
	~MPQVer3(void) { }

	virtual int	getVer(void) const { return 3; }

	static Info GetInfo(const MPQ::Attr& cAttr);
	static void SetInfo(MPQ::Attr* cAttr, const Info& cInfo);
};

#endif	// ___MPQ_VER3___
