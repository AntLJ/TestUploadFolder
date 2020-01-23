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

#include "FeatureContainer.h"
#include "ErrorObject.h"

namespace sindy {

class SINDYLIB_API CModelContainContainer :
	public CFeatureContainer
{
public:
	CModelContainContainer(void);
	CModelContainContainer( sindyTableType::ECode emType ) : CFeatureContainer(emType) {}
	CModelContainContainer( IFeatureClass* lp ) : CFeatureContainer( lp ){}
	CModelContainContainer( ITable* lp ) : CFeatureContainer( lp ){}
	virtual ~CModelContainContainer(void);

	/**
	 * @brief <b>形状で検索する</b>\n
	 * 内部でバッファリングなどはしないので、ポイントで検索する場合は
	 * バッファリングした形状を渡してください。\n
	 * @param ipGeom		[in]			検索形状
	 * @param bRecycling	[in,optional]	trueにするとキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @param pErrs			[out,optional]	エラー格納コンテナ（オブジェクト同士の関連付けに失敗したもののみ入る）
	 *
	 * @return sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	virtual errorcode::sindyErrCode Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );

	/**
	 * @brief <b>与えられた形状との交差部分で分割処理を行う</b>\n
	 * @param pSource		[in]	分割対象
	 * @param cGeom			[in]	クエリ形状
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );
	errorcode::sindyErrCode _Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>与えられたフィーチャのマージを行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param pSource		[in]	マージ元
	 * @param pDest			[in]	マージ先
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>与えられた形状の範囲にある点の削除を行う</b>\n
	 * @param pRow			[in]	削除対象
	 * @param cGeom			[in]	削除形状
	 * @param cErrObjects	[out]	エラーオブジェクト格納コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>フィーチャの削除を行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param pRow			[in]	削除対象
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Delete( CSPRow pRow, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>編集内容を保存する</b>\n
	 * この関数では整合性のチェックを行いません。\n
	 * 必ずCheck関数を読んだ後にこの関数を呼んでください。\n
	 */
	errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );

	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * フィーチャを編集した後に必ずチェックを行ってください。
	 * @param cRule			[in]	論理チェック用ルール
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>コンフリクト回避のための既編集チェックを行う</b>\n
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;

};

} // sindy
