#pragma once

#include "CheckBase.h"
#include <TDC/sindylib_core/FieldMap.h>

class CheckTable
	: public CheckBase
{
public:
	CheckTable();
	virtual ~CheckTable();

	/**
	 * @brief 初期化
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Init();

	/**
	 * @brief チェック実行
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool Check();

private:
	/**
	 * @brief 住所コードの重複がないかのチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckAddrCodeDuplication();

	/**
	 * @brief 不正な整備状況コードを持つレコードがいないかのチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckModifyStatusCode();

private:
	ITablePtr m_ipRelAddrTable; //!< 住所紐付けテーブル
	sindy::CFieldMap m_relAddrTableFieldMap; //!< 住所紐付けテーブルフィールドマップ
};

