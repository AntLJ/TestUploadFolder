#pragma once

#include "CheckBase.h"
#include <TDC/sindylib_core/FieldMap.h>

class CheckPolygonAndTable
	: public CheckBase
{
public:
	CheckPolygonAndTable();
	virtual ~CheckPolygonAndTable();

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
	 * @brief 郵便番号ポリゴンの持つ郵便番号と住所紐付けテーブルの持つ郵便番号の整合チェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckZipCode();

private:
	ITablePtr m_ipRelAddrTable; //!< 住所紐付けテーブル
	IFeatureClassPtr m_ipZipCodeArea; //!< 郵便番号ポリゴンフィーチャクラス
	sindy::CFieldMap m_relAddrTableFieldMap; //!< 住所紐付けテーブルフィールドマップ
	sindy::CFieldMap m_zipCodeAreaFieldMap; //!< 住所紐付けテーブルフィールドマップ
};

