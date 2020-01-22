#pragma once

/**
* @brief   出力情報取得クラス [bug 12017]
*/
class Output
{
public:
	/**
	* @brief コンストラクタ
	*/
	Output() {}

	/**
	* @brief デストラクタ
	*/
	virtual ~Output() {}

	/**
	* @brief 現在時刻取得
	* @param lpSystemTime [in] システムタイム
	* @return CString 現在時刻文字列
	*/
	CString GetNow(LPSYSTEMTIME lpSystemTime = nullptr) const;

	/**
	* @brief バージョン情報取得
	* @return pair<CString,CString> バージョン情報（プロダクト、ファイル）
	*/
	std::pair<CString, CString> GetVersion();

	/**
	* @brief 実行ファイル名取得
	* @return CString ファイル名
	*/
	CString GetModuleName() const;

	/**
	* @brief ログヘッダ作成
	* @param lpcszOpParam [in] オプション引数一覧
	* @return CString ログヘッダ
	*/
	CString CreateLogHeader(LPCTSTR lpcszOpParam);
};