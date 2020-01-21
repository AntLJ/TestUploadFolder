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

#include "CityAdmin.h"
#include "GouPoint.h"
#include <SiNDYLib/FeatureContainer.h>

namespace addr
{

using namespace sindy;

/**
 * @class	CGouPoints
 * @brief	指定行政界以下にある号ポイントリスト
 *			号ポイントと家形の対応もここで関連付けしている
 */
class CGouPoints : public CFeatureContainer
{
public:

	explicit CGouPoints()
	{
		m_emTableType = sindyTableType::city_admin;
	}
	virtual ~CGouPoints(){ clear(); }

	explicit CGouPoints( ITable* ipAdminTable )
	{
		SetObject( sindyTableType::city_admin, ipAdminTable );
	}

	explicit CGouPoints( IFeatureClass* ipAdminFC )
	{
		SetObject( sindyTableType::city_admin, ipAdminFC );
	}

	explicit CGouPoints( const CFeatureClass& cAdminFC )
	{
		SetObject( cAdminFC.GetTableType(), (ITable*)cAdminFC );
	}

	/**
	 * @brief	<b>行政界以下にある号ポイントを取得(住所コード指定Ver)</b>
	 * @param	lpcszAddrCode	[in]			住所コード(5,8,11,16桁の指定が可)
	 * @return	sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	errorcode::sindyErrCode SelectByAddrCode( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	<b>行政界以下にある号ポイントを取得(Where句指定Ver)</b>
	 * @param	lpcszWhere	[in]	Where句
	 * @return	sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	errorcode::sindyErrCode SelectByWhereClause( const _TCHAR* lpcszWhere );

	/**
	 * @brief	検索対象号ポイントテーブルセット
	 * @note	行政界と違うワークスペースの号を検索対象としたいときに明示的に指定して下さい。
	 * @param	ipTable	[in]	号ポイントテーブル
	 */
	void SetGouPointTable( ITablePtr ipTable ){ m_cGouFC.SetObject(ipTable); }

private:

	/**
	 * @brief	<b>クエリ作成</b>
	 * @param	lpcszAddrCode	[in]	住所コード(5,8,11桁の指定が可)
	 * @return	クエリ文字列
	 */
	CString GetQueryStr( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief <b>行政界を検索</b>\n
	 * CFeatureContainer::Select 使用すると、CCityAdminのインスタンスを生成するために
	 * SiNDYLib/RowClassFactoryに修正を加える必要があるので独自に定義
	 * @param ipFilter		[in]			フィルタ
	 * @param bRecycling	[in,optional]	trueにするとキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @param pErrs			[out,optional]	エラー格納コンテナ（オブジェクト同士の関連付けに失敗したもののみ入る）
	 * @return sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	errorcode::sindyErrCode Select( IQueryFilter* ipFilter, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );

private:

	CFeatureClass	m_cGouFC;
};

}	// addr
