#pragma once

#include "Arguments.h"
#include "CheckBase.h"
#include "CheckAll.h"
#include "CheckTable.h"
#include "CheckTableAndMaster.h"
#include "CheckTableAndAddr.h"
#include "CheckPolygon.h"
#include "CheckPolygonAndTable.h"
#include "CheckPolygonAndAddr.h"

/**
 * @brief   メインクラス
 */
class CCheckZipCodeArea
{
public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CCheckZipCodeArea(const Arguments& args) : m_args(args) {}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CCheckZipCodeArea() {};

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

	/**
	 * @brief チェッククラスのインスタンスを用意する
	 * @param checkMode [in] チェックモード
	 */
	void SetCheckObject(const uh::tstring& checkMode);

	/**
	 * @brief チェック実行
	 */
	bool DoCheck();

private:

	Arguments m_args; //!< 引数
	std::shared_ptr<CheckBase> m_checkClassPtr; //!< チェック用クラスインスタンス
};

