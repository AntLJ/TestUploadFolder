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

#ifndef __SINDY_FREE_STYLE_LOG_HEADER_H__
#define __SINDY_FREE_STYLE_LOG_HEADER_H__

#include "SiNDYLogHeader.h"

namespace sindy {

/**
 * @brief 自由形式ログヘッダクラス
 */
class FreeStyleLogHeader : public SiNDYLogHeader
{
public:
// 定義
	/**
	 * @brief 自由形式ログヘッダ構築クラス
	 */
	class Builder
	{
	public:
	// 処理
		bool operator()(SiNDYLogHeader& rLogHeader, std::istream& rStream);
		bool operator()(SiNDYLogHeader& rLogHeader, std::wistream& rStream);
		bool operator()(SiNDYLogHeader& rLogHeader, LPCSTR lpszHeaderLine);
		bool operator()(SiNDYLogHeader& rLogHeader, LPCWSTR lpszHeaderLine);

	private:
	// 変数
		static const LPCTSTR m_rgszColumn[];
		static const TCHAR m_szId[];
	};

// 構築/消滅
	FreeStyleLogHeader();
	FreeStyleLogHeader(std::istream& rStream);
	FreeStyleLogHeader(std::wistream& rStream);
	FreeStyleLogHeader(LPCTSTR lpszHeaderLine);

// 仮想関数のオーバーライド
	virtual LPCTSTR getId() const { return id(); }
	virtual LPCTSTR getDescription() const { return _T("自由形式ログ"); }
	virtual LPCTSTR getSeps() const { return seps(); }

// 静的関数
	static LPCTSTR id() { return _T("FREESTYLELOG"); }
	static LPCTSTR seps() { return _T("\t"); }
};

} // namespace sindy

#endif // __SINDY_FREE_STYLE_LOG_HEADER_H__
