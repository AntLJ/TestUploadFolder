#pragma once

#include "CheckBase.h"

class CheckTableAndAddr
	: public CheckBase
{
public:
	CheckTableAndAddr();
	virtual ~CheckTableAndAddr();

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
	 * @brief 紐付けテーブルの住所コードに該当するデータがSiNDY上に存在しているかどうかをチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckTableToAddr();

	/**
	 * @brief 行政界の11桁コードが紐付けテーブルに存在しているかどうかをチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckCityAdminToTable();

private:
	addr::AddrRecHelper m_addrRecHelper; //!< 住所レコード生成補助クラス
	sindy::CTableFinder m_addrFinder; //!< 住所データ(行政界、号、CS)取得用
	sindy::CTableFinder m_relAddrFinder; //!< 紐付けテーブル取得用
};
