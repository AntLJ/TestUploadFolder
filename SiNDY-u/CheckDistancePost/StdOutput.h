#pragma once
#include "Output.h"

/**
* @brief   標準出力管理クラス [bug 12017]
*/
class StdOutput :
	public Output
{
public:
	/**
	* @brief コンストラクタ
	*/
	StdOutput() {}

	/**
	* @brief デストラクタ
	*/
	virtual ~StdOutput() {}

	/**
	* @brief 路線情報を出力する
	* @note 路線リストに指定された路線情報を出力する
	* @param msg[in] 路線情報
	*/
	void writeRoadInfoMsg(const CString& msg);

	/**
	* @brief エラーメッセージを出力する
	* @note 処理中に発生したエラーの内容を出力する
	* @param msg[in] エラーメッセージ
	*/
	void writeErrMsg(const CString& msg);

	/**
	* @brief コンソールにメッセージを出力する
	* @param msg[in] メッセージ
	*/
	void writeConsole(const CString& msg);
};
