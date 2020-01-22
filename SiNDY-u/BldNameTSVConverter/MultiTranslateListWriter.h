#pragma once

#include "stdafx.h"
#include "TsvWriter.h"

/**
 * @brief 多言語翻訳リストファイル名フォーマット（フォルダ込み）
 */
const CString FILENAME_FORMAT = _T("%s\\%s_%.4d.txt");

/**
 * @brief 多言語翻訳リスト書込クラス（派生クラス）
 */
class MultiTranslateListWriter : public TsvWriter
{
public:
	/**
	 * @brief コンストラクタ
	 * @param[in] outputDir       ファイル出力先フォルダ
	 * @param[in] maxRecNum       最大書込レコード数
	 * @param[in] grpcode         データ分類コード
	 * @param[in] addColumnList   追加カラムリスト
	 */
	MultiTranslateListWriter(
				const CString& outputDir,
				int maxRecNum,
				const CString& grpcode,
				const std::vector<CString>& addColumnList);

	/**
	 * @brief デストラクタ
	 */
	virtual ~MultiTranslateListWriter() {}

	/**
	 * @brief ファイルオープン
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool open();

	/**
	 * @brief ファイルへの書き込み
	 * @param[in] writeStrList 書き込む文字列のリスト
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool writeRecord( const std::vector<CString>& writeStrList );

private:

	CString m_outputDir;                   //!< ファイル出力先フォルダ
	CString m_grpcode;                     //!< データ分類コード
	int m_maxRecNum;                       //!< ファイル内最大書込みレコード数
	int m_writeRecCounter;                 //!< 書込み件数カウンタ
	int m_fileSepCounter;                  //!< ファイル分割カウンタ
	std::vector<CString> m_headerList;     //!< 多言語翻訳リストヘッダ一覧
};

