#pragma once

#include "CheckBase.h"
#include <TDC/sindylib_core/FieldMap.h>
#include "TDC/RepPointCalculator/RepPointCalculator.h"

class CheckPolygonAndAddr
	: public CheckBase
{
public:
	CheckPolygonAndAddr();
	virtual ~CheckPolygonAndAddr();

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
	 * @brief 行政界が郵便番号に包含されているかチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckContainCityAdmin();

	/**
	 * @brief 住所データ(号、CS)が郵便番号が包含されているかチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckContainAddrPoint();

	/**
	 * @brief 郵便番号ポリゴンが住所データを包含しているかチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool ZipToAddrCheck(
		const sindy::CTableContainer& zipCodeAreaTbl,
		const addr::AddrRecs& existRecs,
		const CString& zipCode,
		const CString& addrCode);

	/**
	 * @brief 住所データが郵便番号ポリゴンに包含されているかチェック
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool AddrToZipCheck(
		const sindy::CTableContainer& zipCodeAreaTbl,
		const addr::AddrRecs& existRecs,
		const CString& zipCode,
		const CString& addrCode);

private:
	addr::AddrRecHelper m_addrRecHelper; //!< 住所レコード生成補助クラス
	sindy::CTableFinder m_addrFinder; //!< 住所データ(行政界、号、CS)取得用
	sindy::CTableFinder m_relAddrFinder; //!< 紐付けテーブル取得用
	RepPointCalculator m_repPointCalculator; //!< 代表点算出クラス

};

