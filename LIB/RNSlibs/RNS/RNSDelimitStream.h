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

// RNSDelimitStream.h: RNSDelimitStream クラスのインターフェイス
//
// $Id: RNSDelimitStream.h,v 1.3 2003/07/18 04:14:33 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNSDELIMITSTREAM_H__0F35D20F_43DF_47CF_89BA_B90D1225E1B0__INCLUDED_)
#define AFX_RNSDELIMITSTREAM_H__0F35D20F_43DF_47CF_89BA_B90D1225E1B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <iostream>
#include <stdexcept>

/**
 * @brief 遅延デリミタ出力ストリーム
 *
 * 必要となるまでデリミタを出力しないストリーム。
 */
class RNSDelimitStream  
{
public:
// 構築/消滅
	RNSDelimitStream(std::ostream* pStream, const char* cDelimiter = "\t");

// 設定
	void setStream(std::ostream* pStream);
	void setDelimiter(const char* cDelimiter) { mDelimiter = cDelimiter; }

// 取得
	std::ostream& stream() { return *mStream; }
	int countDelimiter() { return mDelimNum; }

// 操作
	RNSDelimitStream& operator ++ ();

// 出力
	/**
	 * @brief デリミタを全て出力する
	 */
	void	outputDelimiter()
	{
		for(int i = 0; i < mDelimNum; ++i) {
			stream() << mDelimiter;
		}
		mDelimNum = 0;
	}

	/**
	 * @brief std::endlなどの処理
	 *
	 * std::ostreamのマニピュレータは文章の終わりを示すものしか無かったと思うので、デリミタ出力はなし。\n
	 * 勘違いだったら修正しよう。
	 *
	 * @param std::ostreamのマニピュレータ
	 * @return 自己
	 */
	RNSDelimitStream& operator <<( std::ostream& (*fcn)(std::ostream&) )
	{
		stream() << fcn;
		return *this;
	}

	/**
	 * @brief std::iosマニピュレータ
	 *
	 * std::iosのマニピュレータは書式変更を行うものしか無かったと思うので、デリミタ出力はなし。\n
	 * 勘違いだったら修正しよう。
	 *
	 * @param std::iosのマニピュレータ
	 * @return 自己
	 */
	RNSDelimitStream& operator <<( std::ios& (*fcn)(std::ios&) )
	{
		stream() << fcn;
		return *this;
	}

	/**
	 * @brief デリミタ出力後、cArgを出力
	 *
	 * @param cArg [in] 出力対象
	 * @retrurn 自己
	 */
	template<typename T>
	RNSDelimitStream& operator << (T cArg)
	{
		outputDelimiter();
		stream() << cArg;

		return *this;
	}

private:
	int mDelimNum; ///< スタックしているデリミタ数
	std::string mDelimiter; ///< デリミタ
	std::ostream* mStream; ///< 出力ストリーム
};

#endif // !defined(AFX_RNSDELIMITSTREAM_H__0F35D20F_43DF_47CF_89BA_B90D1225E1B0__INCLUDED_)
