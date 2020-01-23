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

// RNSComException.h: RNSComException クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if(0) // 使用不可 //

#if !defined(AFX_RNSCOMEXCEPTION_H__54853C52_E782_4A56_B6DC_853F5CEB94E8__INCLUDED_)
#define AFX_RNSCOMEXCEPTION_H__54853C52_E782_4A56_B6DC_853F5CEB94E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RNSException.h"

/**
 * RNS_COM_CALL がエラーを発見したときに発生する例外
 */
class RNSComException : public RNSException  
{
public:
	/// コンストラクタ
	RNSComException(HRESULT cHr, const char *iMessage = NULL, const char *iLocation = NULL);

	/// デストラクタ
	virtual ~RNSComException();

	// ----- 取得 ----- //
	/// リザルト取得
	HRESULT hr() const;

protected:
	/// エラー内容
	virtual const char* circumstance() const { return "com error issued"; }

private:
	HRESULT	mHr;

};

#endif // !defined(AFX_RNSCOMEXCEPTION_H__54853C52_E782_4A56_B6DC_853F5CEB94E8__INCLUDED_)

#endif
