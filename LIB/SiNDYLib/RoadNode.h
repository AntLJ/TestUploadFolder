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

/**
 * @file CRoadNode.h
 * @brief <b>道路ノードクラス定義ヘッダファイル</b>\n
 * @version $Id$
 */

#ifndef _ROADNODE_H_
#define _ROADNODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"
#include "Feature.h"

namespace sindy {
namespace road_node {

/**
 * @class CRoadNode
 * @brief 道路ノードフィーチャクラス
 */

class SINDYLIB_API CRoadNode : public CFeature  
{
public:
	ROWCLASS_CONSTRUCTOR( CRoadNode, road_node )
public:
	//@{ @name 属性取得関数
	/**
	 * @brief ノード種別を返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_node::node_class::ECode GetNodeClassCode() const
	{
		return (schema::road_node::node_class::ECode)GetLongValueByFieldName( schema::road_node::kNodeClass, -1 );
	}
		
	/**
	 * @brief 交差点漢字名称を返す
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又はNULLの場合は空文字列
	 */
	LPCTSTR GetNameKanji( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::road_node::kNameKanji, nLen, lpszName );
	}	

	/**
	 * @brief 交差点カナ名称を返す
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又はNULLの場合は空文字列
	 */
	LPCTSTR GetNameYomi( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::road_node::kNameYomi, nLen, lpszName );
	}	
	
	/**
	 * @brief 相対高さコードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_node::height_code::ECode GetHeightCode() const
	{
		return (schema::road_node::height_code::ECode)GetLongValueByFieldName( schema::road_node::kHeightCode, -1 );
	}
	
	/**
	 * @brief 相対高さを返す
	 * @retval -1 エラー
	 * @retval -1以外 相対高さ
	 */
	long GetHeight() const
	{
		return GetLongValueByFieldName( schema::road_node::kHeight, -1 );
	}
	
	/**
	 * @brief ノード信号機コードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_node::signal::ECode GetSignalCode() const
	{
		return (schema::road_node::signal::ECode)GetLongValueByFieldName( schema::road_node::kSignal, -1 );
	}
	
	/**
	 * @brief 駐車場IDを返す
	 * @retval -1 エラー
	 * @retval -1以外 駐車場ID
	 */
	long GetParkingID() const
	{
		return GetLongValueByFieldName( schema::road_node::kParkingID, -1 );
	}	
	
	//@}

	//@{ @name 属性設定関数

	/**
	 * @brief ノード種別を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetNodeClassCode(schema::road_node::node_class::ECode ecode)
	{
		return SetLongValueByFieldName( schema::road_node::kNodeClass, ecode );
	}
		
	/**
	 * @brief 交差点漢字名称を設定する
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @brief NULL 許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetNameKanji( LPTSTR lpszName )
	{
		return SetStringValueByFieldName( schema::road_node::kNameKanji, lpszName );
	}	

	/**
	 * @brief 交差点カナ名称を設定する
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @brief NULL 許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetNameYomi( LPTSTR lpszName )
	{
		return SetStringValueByFieldName( schema::road_node::kNameYomi, lpszName );
	}	
	
	/**
	 * @brief 相対高さコードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetHeightCode(schema::road_node::height_code::ECode ecode)
	{
		return SetLongValueByFieldName( schema::road_node::kHeightCode, ecode );
	}
	
	/**
	 * @brief 相対高さを設定する
	 * @brief NULL 許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetHeight(long lValue)
	{
		return SetLongValueByFieldName( schema::road_node::kHeight, lValue );
	}
	
	/**
	 * @brief ノード信号機コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetSignalCode(schema::road_node::signal::ECode ecode)
	{
		return SetLongValueByFieldName( schema::road_node::kSignal, ecode );
	}
	
	/**
	 * @brief 駐車場IDを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetParkingID(long lValue)
	{
		return SetLongValueByFieldName( schema::road_node::kParkingID, lValue );
	}	
	
	//@}


	//@{ @name チェック関数
	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * ノードを編集した後に必ずチェックを行ってください。
	 * このメソッドでチェックされる項目は以下のとおりです。
	 * @li 二次メッシュ区画線上ノード・二次メッシュ区画線上PECノードは二次メッシュ区画線上にない場合はエラー
	 * @li 二次メッシュ区画線上に上記以外の種別のノードがある場合はエラー
	 * @li 二次メッシュ四隅にノードがある場合はエラー（種類問わず）
	 * @param cRule			[in]	整合性チェック用ルール
	 * @param cErrInfos		[out]	エラー情報格納用コンテナ
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const;

	/**
	 * @brief 正しい種別が設定されているか
	 * @retval	sindyErr_NoErr		成功：正しい種別
	 * @retval	それ以外のエラー	失敗：規約に違反した種別
	*/
	errorcode::sindyErrCode CheckNodeClass() const;
	//@}

	bool IsExistAttrNode( bool bCheckAll = true ) const;
	/**
	 * @brief 2次メッシュ上ノードかどうか
	 * @retval	成功：2次メッシュ上ノード
	 * @retval	失敗：上記以外
	*/
	bool IsBorderNode() const;
};

} // namespace road_node
} // namespace sindy

#endif // _ROADNODE_H_
