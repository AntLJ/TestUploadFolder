#pragma once

#include "Arguments.h"
#include "CharacterCode.h"
#include <sstream>

/**
 * @brief 文字コードチェック基底クラス
 */
class CodeChecker
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CodeChecker(void){};

	/**
	 * @brief デストラクタ
	 */
	virtual ~CodeChecker(void){};

	/**
	 * @brief 入力ファイルリストを読み込む
	 * @param strFileListPath [in] 入力ファイルリストのパス
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool SetTargetFileList(const CString& strFileListPath);

	/**
	 * @brief 許容文字コード範囲リストを読み込む
	 * @param strCharCodeListPath [in] 文字コードリストのパス
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool SetCharacterCodeList(const CString& strCharCodeListPath);

	/**
	 * @brief 文字コードチェック対象ファイルを読み込んで文字コードチェックを行う
	 * @param isImmediateExit [in] 不正検出時即時終了フラグ
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	virtual bool CheckCharacterCode(bool isImmediateExit) = 0;

protected:
	/**
	 * @brief TSVレコードを列ごとの値に分解して格納する
	 * @param strTSVRecord [in] 任意の長さのTSVレコード1行
	 * @return TSVレコードを列ごとに分解して格納したVector
	 */
	std::vector<std::string> GetTSVParameter(std::string strTSVRecord);

	/**
	 * @brief 対象文字列が許容される文字列かどうか
	 * @param chkStr [in] ファイルから読み込んだ文字列
	 * @retval true 許容される文字列だった
	 * @retval false 許容されない文字列だった
	 */
	bool IsParmitted(const std::string& chkStr);

	/**
	 * @brief 文字コードのバイト列を16進数に変換する（例：0xE5→"E5"）
	 * @param org_str [in] 文字コードのバイト列
	 * @return 文字コードのバイト列を16進数の文字列として変換した文字列
	 */
	std::string ToHex(const std::string& org_str);

protected:
	std::vector<CString> m_vecTargetFileList;		//!< 文字コードチェック対象ファイルのリスト
	std::vector<CharacterCode> m_vecCharCodeList;	//!< 許容する文字コード範囲のリスト
	static const int m_intCharCodeMin = 0;			//!< 文字コード範囲ファイルの1列目
	static const int m_intCharCodeMax = 1;			//!< 文字コード範囲ファイルの2列目
};
