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

#ifndef __SINDY_STD_LOG_HEADER_H__
#define __SINDY_STD_LOG_HEADER_H__

#include "SiNDYLogHeader.h"

namespace sindy {

/**
 * @brief 標準形式ログヘッダクラス
 */
class StdLogHeader : public SiNDYLogHeader
{
public:
// 定義
	/**
	 * @brief 標準形式ログヘッダ構築クラス
	 */
	class Builder
	{
	public:
	// 処理
		void operator()(SiNDYLogHeader& rLogHeader);

	private:
	// 変数
		static const LPCTSTR m_rgszColumn[];
	};

// 構築/消滅
	StdLogHeader();

// 仮想関数のオーバーライド
	virtual LPCTSTR getId() const { return id(); }
	virtual LPCTSTR getDescription() const { return _T("標準形式ログ"); }
	virtual LPCTSTR getSeps() const { return seps(); }

// 静的関数
	static LPCTSTR id() { return _T("SINDYSTDLOG"); }
	static LPCTSTR seps() { return _T("\t,|"); }
};

} // namespace sindy

#endif // __SINDY_STD_LOG_HEADER_H__
