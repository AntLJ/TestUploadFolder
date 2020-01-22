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

#include <sindy/schema.h>

struct AKAttrHandler {
	/**
	* @brief コンストラクタ
	*/
	AKAttrHandler() {
	}
	/**
	* @brief デストラクタ
	*/
	~AKAttrHandler() {
	}
	/**
	* @brief 除外属性の判別
	* @return 対象外=1, エラーな種別=-1, 出力対象=0
	*/
	long fnIsExcluded (
	         long cBuildAttr,  ///< [in] 建物属性
			 long cNoWall,     ///< [in] 無壁舎フラグ
			 long cCorridor    ///< [in] 渡り廊下フラグ
	     );
	/**
	* @brief 属性変換
	*/
	void fnAttrConv (
	         long& cBuildAttr  ///< [in,out] 建物属性
	     );
};

// 対象外=1, エラーな種別=-1, 出力対象=0
inline long AKAttrHandler::
fnIsExcluded ( long cBuildAttr,  ///< [in] 建物属性
               long cNoWall,     ///< [in] 無壁舎フラグ
               long cCorridor )  ///< [in] 渡り廊下フラグ
{
	using namespace sindy::schema;

	if( cBuildAttr == building::bld_class::kNone && // 建物属性=未調査・作業中
		cNoWall == 1 ) { // 無壁舎フラグが1
		return 1;
	}
	switch (cBuildAttr)
	{
	// 変換対象外
	case	14: return -1;   // 元の無壁舎種別
	case	21: return -1; // 歩道橋階段(削除種別)
	case	22: return -1; // 歩道橋エスカレータ(削除種別)
	case	24: return -1; // ペデストリアンデッキエスカレータ(削除種別)
	case	building::bld_class::kSiteStairs: return 1;  // 敷地階段
	case	building::bld_class::kArcade: return 1;  // アーケード
	case	building::bld_class::kTollGate: return 1;// 料金所
	}
	return 0;
}

inline void AKAttrHandler::
fnAttrConv ( long& cBuildAttr )  ///< [in] 建物属性
{
	using namespace sindy::schema;

	switch (cBuildAttr)
	{
	// 種別を変換
	case	building::bld_class::kUniversalTrafficStation: // 新交通駅（地上）⇒地下鉄以外駅（地上）(1)
	case	building::bld_class::kMonorailStation:         // モノレール駅（地上）⇒地下鉄以外駅（地上）(1)
	case	building::bld_class::kCableCarStation:         // ケーブルカー駅（地上）⇒地下鉄以外駅（地上）(1)
		cBuildAttr = building::bld_class::kOtherStation;
		break;
	case	building::bld_class::kForce: 
		cBuildAttr = building::bld_class::kGovmentOffice; // 自衛隊・米軍⇒官公庁(5)
		break;
	case	building::bld_class::kGasStation:
		cBuildAttr = building::bld_class::kNone; //ガソリンスタンド⇒未調査・作業中(0)
		break;
	}
};
