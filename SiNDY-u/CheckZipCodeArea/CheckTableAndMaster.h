#pragma once

#include "CheckBase.h"
#include <AddrLib/JusyoHelper.h>
#include <TDC/sindylib_core/FieldMap.h>

class CheckTableAndMaster
	: public CheckBase
{
public:
	CheckTableAndMaster();
	virtual ~CheckTableAndMaster();

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
	 * @brief 住所紐付けテーブルの住所コードが住所マスタに存在するかどうかをチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckExistAddrCode();

	/**
	 * @brief 住所紐付けテーブルの住所コード（5桁）が住所マスタに存在するかどうかをチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckExistCityCode();

private:
	addr::CJusyoHelper m_jusyoMasterHelper; //!< 住所マスタ取得用クラス
	addr::CJusyoHelper m_kajyoMasterHelper; //!< 加除マスタ接続用クラス
	ITablePtr m_ipRelAddrTable; //!< 住所紐付けテーブル
	sindy::CFieldMap m_relAddrTableFieldMap; //!< 住所紐付けテーブルフィールドマップ
};

