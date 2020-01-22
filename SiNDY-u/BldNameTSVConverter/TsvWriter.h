#pragma once

#include "stdafx.h"

/**
 * @brief 実行モード
 */
enum Encoding
{
	Euc,          // EUC
	UTF8,         // UTF8（BOMなし）
	UTF8WithBom   // UTF8（BOM付き）
};

/**
 * @brief 改行コード
 */
enum LineFeed
{
	None,   // なし
	CrLf,   // CR+LF
	Lf      // LF
};

/**
 * @brief 改行コード定義
 */
const CString LINE_FEED_CODE_CRLF = _T("\r\n");
const CString LINE_FEED_CODE_LF = _T("\n");

/**
 * @brief UTF-8のBOM値定義
 */
const unsigned char BOM[3] = { 0xEF, 0xBB, 0xBF };

/**
 * @brief TSVファイル書込クラス
 */
class TsvWriter
{
public:

	/**
	 * @brief コンストラクタ
	 */
	TsvWriter() {}

	/**
	 * @brief デストラクタ
	 */
	virtual ~TsvWriter() {}

	/**
	 * @brief ファイルオープン
	 * @param file     [in] ファイルパス
	 * @param encoding [in] 文字エンコーディング
	 * @param lineFeed [in] 改行コード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool open( const CString& file, Encoding encoding, LineFeed lineFeed );

	/**
	 * @brief ファイルクローズ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	void close() { if( m_ofs.is_open() ) m_ofs.close(); };


	/**
	 * @brief ファイルへの書き込み
	 * @param writeStrList [in] 書き込む文字列のリスト
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool writeRecord( const std::vector<CString>& writeStrList );

	/**
	 * @brief ファイルへの書き込み
	 * @param writeStr [in] 書き込む文字列
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool writeRecord( const CString& writeStr );

protected:

	std::ofstream m_ofs; //!< 出力ファイルストリーム
	Encoding m_encoding; //!< エンコーディング
	LineFeed m_lineFeed; //!< 改行コード
};

