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


namespace sindy{

// リンク相対位置
enum LinkRelationPosition
{
	kNotFound = 0,	// 見つからない
	kRight = 1,		// 右
	kLeft  = 2		// 左
};


/**
 * @brief  基準のポリラインから見て、ターゲットのポリラインが左右どちらに位置するかを判定する
 * @param [in]    ipBasePolyline     左右を判定する側(ベース側)のポリライン
 * @param [in]    ipTargetPolyline   左右を判定される側(ターゲット側)のポリライン
 * @param [in]    dSearchMeter       検索距離(メートル)
 * @param [out]   eRelationInfo      左右どちらに位置するか(1:右, 2:左, 0:見つからない)
 * @param [out]	  dMinMeter	 判定距離(dgree) 左右判定に使用した垂線の長さ(最小距離)
 * @retval true   正常に検索成功(見つからなかった場合も trueを 返す)
 * @retval false  検索が不正に失敗
 * @detail アルゴリズムは以下を参照とする
 *         http://preon.mr.ipc.pioneer.co.jp/svn/source/trunk/SiNDY-u/mkLinkRelation/doc/ソフトウェア開発文書(mkLinkRelation).xlsx
 */
bool AheJudgeLinkRelation( const IPolylinePtr& ipBasePolyline,
						   const IPolylinePtr& ipTargetPolyline, 
						   double dSearchMeter,
						   LinkRelationPosition& eRelationInfo,
						   double& dMinMeter );

/**
 * @brief  基準のポリラインから見て、ターゲットのポリラインが左右どちらに位置するかを判定する
 * @note   左右判定に使用した垂線の距離が必要ない場合に使用
 * @param [in]    ipBasePolyline     左右を判定する側(ベース側)のポリライン
 * @param [in]    ipTargetPolyline   左右を判定される側(ターゲット側)のポリライン
 * @param [in]    dSearchMeter       検索距離(メートル)
 * @param [out]   eRelationInfo      左右どちらに位置するか(1:右, 2:左, 0:見つからない)
 * @retval true   正常に検索成功(見つからなかった場合も trueを 返す)
 * @retval false  検索が不正に失敗
 */
bool AheJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchMeter, LinkRelationPosition& eRelationInfo );


/**
 * @brief AheJudgeLinkRelationの重なり判定版
 * @param [note] ベースポリラインに検索距離分バッファを掛け、ターゲットポリラインとの共通部分を取得し、<br>
 *               取得した距離がdRoundOffMeter未満であれば、左右が判定できなかったこととする。<br>
 *				 ※ 但し、リンク総距離がdNoCheckMeter未満の場合は、AheJudgeLinkRelationの結果をそのまま返す。
 * @param [in]    ipBasePolyline     左右を判定する側(ベース側)のポリライン
 * @param [in]    ipTargetPolyline   左右を判定される側(ターゲット側)のポリライン
 * @param [in]    dSearchMeter       検索距離(メートル)
 * @parmm [in]    dRoundOffMeter     重なり判定距離(メートル)
 * @param [in]    dNoCheckMeter      重なり判定除外距離(メートル)
 * @param [out]   eRelationInfo      左右どちらに位置するか(1:右, 2:左, 0:見つからない)
 * @retval true   正常に検索成功(見つからなかった場合も trueを 返す)
 * @retval false  検索が不正に失敗
 */
bool AheJudgeLinkRelationBuf( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchMeter, double dRoundOffMeter, double dNoCheckMeter, LinkRelationPosition& eRelationInfo );


/**
 * @brief  基準のポリラインから見て、ターゲットのポリラインが左右どちらに位置するかを判定する
 * @param [in]    ipBaseGeo          左右を判定する側(ベース側)のポリライン
 * @param [in]    ipTargetGeo        左右を判定される側(ターゲット側)のポリライン
 * @param [out]   nRelationInfo      左右どちらに位置するか(1:右, 2:左)
 * @retval true   正常に検索成功
 * @retval false  検索が不正に失敗
 * @detail AheJudgeLinkRelationに比べ簡易的な手法であり、必ず左右どちらかに判定されるメリットがあるが、判定精度が落ちる
 *         アルゴリズムは以下を参照とする
 *         http://preon.mr.ipc.pioneer.co.jp/svn/source/trunk/SiNDY-u/mkLinkRelation/doc/ソフトウェア開発文書(mkLinkRelation).xlsx
 */
bool AheSimpleJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, unsigned int& nRelationInfo );

} // sindy
