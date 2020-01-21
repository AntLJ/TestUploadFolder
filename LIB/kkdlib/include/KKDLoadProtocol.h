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

// KKDLoadProtocol.h: KKDLoadProtocol クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDLOADPROTOCOL_H__7BBFC795_2256_42F5_B62A_D12A400D46F7__INCLUDED_)
#define AFX_KKDLOADPROTOCOL_H__7BBFC795_2256_42F5_B62A_D12A400D46F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DRMALoadProtocol.h>

/**
 * @brief KKDデータ読み取りプロトコル
 */
class KKDLoadProtocol : public DRMALoadProtocol  
{
public:
// プロトコルのオーバーライド
	virtual bool load(const char* cDirectoryName, int cMeshCode, const char* cSuffix = 0);
	virtual bool load(const char* cFileName) { return DRMALoadProtocol::load(cFileName); }
	virtual bool load(std::istream& cStream) { return DRMALoadProtocol::load(cStream); }
};

#endif // !defined(AFX_KKDLOADPROTOCOL_H__7BBFC795_2256_42F5_B62A_D12A400D46F7__INCLUDED_)
