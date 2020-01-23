#pragma once

#include "CheckBase.h"
#include "CheckTable.h"
#include "CheckTableAndMaster.h"
#include "CheckTableAndAddr.h"
#include "CheckPolygon.h"
#include "CheckPolygonAndTable.h"
#include "CheckPolygonAndAddr.h"

class CheckAll :
	public CheckBase
{
public:
	CheckAll();
	virtual ~CheckAll();

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
	CheckTable m_checkTable; //!< 住所紐付けテーブル整合チェッククラス
	CheckTableAndMaster m_checkTableAndMaster; //!< 住所紐付けテーブルと住所マスタの整合チェッククラス
	CheckTableAndAddr m_checkTableAndAddr; //!< 住所紐付けテーブルと住所データの整合チェッククラス
	CheckPolygon m_checkPolygon; //!< 郵便番号ポリゴン整合チェッククラス
	CheckPolygonAndTable m_checkPolygonAndTable; //!< 郵便番号ポリゴンと住所紐付けテーブルの整合チェッククラス
	CheckPolygonAndAddr m_checkPolygonAndAddr; //!< 郵便番号ポリゴンと住所データの整合チェッククラス
};

