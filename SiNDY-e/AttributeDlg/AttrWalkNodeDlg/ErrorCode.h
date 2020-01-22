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

#pragma once

namespace sindy_error
{
namespace walk_node
{

/**
 * @brief エラーコードの定義
 */
enum class ErrorCode
{
	ROADNODEID_INPUT_ERROR = 0,       //!< 道路ノードIDが不要な種別なのに入力されている
	STATIONGATE_INPUT_ERROR,          //!< 駅出入口IDが不要な種別なのに入力されている
	STATIONGATE_NONE_INPUT_ERROR,     //!< 駅出入口IDが必要な種別なのに入力されていない
	UNDERGROUNDGATE_INPUT_ERROR,      //!< 地下街出入口IDが不要な種別なのに入力されている
	UNDERGROUNDGATE_NONE_INPUT_ERROR, //!< 地下街出入口IDが必要な種別なのに入力されていない
};

} // walk_node

} // sindy_error

/**
 * @brief エラーコード管理クラス
 * @note エラーコードに対するエラーメッセージもここで管理する
 */
class ErrorCodeKeeper
{
public:
	/**
	 * @brief コンストラクタ
	 */
	ErrorCodeKeeper(){}

	/**
	 * @brief 保持しているエラーを削除する
	 */
	void Reset();

	/**
	 * @brief エラーとして保持するものを追加する
	 * @param code [in] 追加するエラー
	 */
	void Push(sindy_error::walk_node::ErrorCode code);

	/**
	 * @brief エラーを一つでも持っているか否か
	 * @retval true  エラーを一つ以上持っている
	 * @retval false エラーを一つも持っていない
	 */
	bool IsError();

	/**
	 * @brief 保持しているエラーを文字列化して、listに入れて返す
	 * @note for文の中で何度も呼び出す、というような使用は想定していないので、
	 * 内部でいちいち文字列リストを作り直す
	 * @return 保持しているエラーを文字列化し、リスト化したもの
	 */
	std::vector<CString> GetStringList();
private:
	std::vector<sindy_error::walk_node::ErrorCode> m_errorCodes; //!< エラーコードを保持
};
