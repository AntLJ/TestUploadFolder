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

#include "stdafx.h"

// エラーレベル列挙型
namespace err_level {
	enum ECode {
		fatal,
		error,
		warning,
		info
	};
}; // err_level

// エラーコード列挙型
namespace err_code {
	enum ECode {

		// 9**:FATAL
		OpenLogFail       = 901,
		OpenMeshFileFail  = 902,
		OpenWsFail        = 903,
		OpenFcFail        = 904,
		OpenTblFail       = 905,
		GetNineMeshFail   = 906,
		GetRailFail       = 907,
		GetStaPoiFail     = 908,
		GetBaseStaFail    = 909,
		GetMeshBoundFail  = 910,
		GetOIDAllFail     = 911,
//		GetRoadCodeList   = 911,

		// 8**:INFO
		StrRailNWDB       = 801,
		StrRailPoiDB      = 802,
		StrStationDB      = 803,
		StrMeshDB         = 804,
		StrLogFile        = 805,
		StrMeshList       = 806,
		StrDistThreshold  = 807,
//		StrAngleThreshold = 808,
		StrNeiThreshold   = 809,
		StrStartTime      = 810,
		StrEndTime        = 811,
		StrStart          = 812,
		StrQueryStart     = 813,

		// 1**:Rail Node ERROR
		NgRailNodeClass   = 101,
		NgRailNodeGeom    = 102,
		BorderCornerNode  = 103,
		NgBorderNodeAttr  = 104,
		NgBorderNodeGeom  = 105,
		NoConnect         = 106,
		NgTerminalNode    = 107,
		NgBranchNode      = 108,
		NgAlterNode       = 109,
		TouchNgLink       = 110,
		TouchNgNode       = 111,
		NoTouchBaseSta    = 112,
		NgRelJoinLine1    = 113,
		NgRelJoinLine2    = 114,
		NgRelJoinLine3    = 115,
		TooFarStaPoint    = 116,


		// 3**:Rail Link ERROR
		NgRailLinkClass   = 301,
		NgDispScaleClass  = 302,
		NgFlagField       = 303,
		NgFromID          = 304,
		NgToID            = 305,
		NgRailLinkGeom    = 306,
		CrossMesh         = 307,
		NoBorderNode      = 308,
		NotFoundFromNode  = 309,
		NotFoundToNode    = 310,
		NotSameBorderAttr = 311,
		NgBorderLink      = 312,
		UnMatchStartFrom  = 313,
		UnMatchEndTo      = 314,
		UnderThresholdSeg = 315,
		DupPoint          = 316,
		LoopLinkSingleFT  = 317,
		LoopLinkDupFT     = 318,
		NgRelRailwayLine1 = 319,
		NgRelRailwayLine2 = 320,

		// 5**:Query ERROR
		NonRelRailLink    = 501,
		NonRelRailNode    = 502,
		NotOnlySeqLine    = 503,
		DupStationNum     = 504,
		NoNumberStaNum    = 505,
		NgSeqStaNum       = 506,
		NonJoinLinkInfo   = 507,
		NonRailwayLine    = 508,



	};
}; // err_code

/**
 * @brief   出力クラス
 * @note    標準出力 / ログ出力
 */
class COutPut
{
public:

	/**
	 * @brief   コンストラクタ
	 */
	COutPut(){};

	/**
	 * @brief   デストラクタ
	 */
	~COutPut(){};

	/**
	 * @brief   オープン
	 * @param   strFileName [in]  ログファイル名
	 * @retval  true  オープン成功
	 * @retval  false オープン失敗
	 */
	bool Open( const CString& strFileName ){
		m_ofs.open( strFileName );
		if( ! m_ofs )
			return false;
		return true;
	};

	/**
	 * @brief   クローズ
	 */
	void Close(){
		m_ofs.close();
	};

	/**
	 * @brief   ヘッダ出力
	 */
	void OutputHeader()
	{
		if( ! m_ofs )
			return;
		CString str = _T("#SINDYSTDLOG");
		m_ofs << CT2CA( str ) << std::endl;
	};

	/**
	 * @brief   ログ出力
	 * @note    SINDYSTDLOG形式
	 * @param   layer    [in]  レイヤ名
	 * @param   id       [in]  ObjectID
	 * @param   errLevel [in]  エラーレベル
	 * @param   errCode  [in]  エラーコード
	 * @param   msg      [in]  フリーメッセージ
	 */
	//void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode )
	//{
	//	if( ! m_ofs )
	//		return;
	//	CString str;
	//	str.Format( _T("0\t%s\t%ld\t\t\t%s\t%ld\t%s\t\t\t\t"), layer, id, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ) );
	//	m_ofs << CT2CA( str ) << std::endl;
	//};
	//void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode, const CString& info )
	//{
	//	if( ! m_ofs )
	//		return;
	//	CString str;
	//	str.Format( _T("0\t%s\t%ld\t\t\t%s\t%ld\t%s\t%s\t\t\t"), layer, id, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ), info );
	//	m_ofs << CT2CA( str ) << std::endl;
	//};
	//void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode, const CString& info1, const CString& info2 )
	//{
	//	if( ! m_ofs )
	//		return;
	//	CString str;
	//	str.Format( _T("0\t%s\t%ld\t\t\t%s\t%ld\t%s\t%s\t%s\t\t"), layer, id, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ), info1, info2 );
	//	m_ofs << CT2CA( str ) << std::endl;
	//};
	void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode, const CString& modOperator = _T(""), const CString& modDate = _T(""), const CString& info1 = _T(""), const CString& info2 = _T("") )
	{
		if( ! m_ofs )
			return;
		CString str;
		str.Format( _T("0\t%s\t%ld\t\t\t%s\t%ld\t%s\t%s\t%s\t%s\t%s"), layer, id, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ), info1, info2, modOperator, modDate );
		m_ofs << CT2CA( str ) << std::endl;
	};

	/**
	 * @brief   ログ出力
	 * @note    コメント形式
	 * @param   errLevel [in]  エラーレベル
	 * @param   errCode  [in]  エラーコード
	 * @param   msg      [in]  フリーメッセージ
	 */
	void OutputLog( err_level::ECode errLevel, long errCode, const CString& msg )
	{
		if( ! m_ofs )
			return;
		CString str;
		str.Format( _T("# %s\t%s\t%s"), GetErrLevelStr( errLevel ), GetErrMsg( errCode ), msg );
		m_ofs << CT2CA( str ) << std::endl;
	};

	/**
	 * @brief   標準出力
	 * @param   errLevel [in]  エラーレベル
	 * @param   errCode  [in]  エラーコード
	 * @param   msg      [in]  フリーメッセージ
	 */
	void OutputStdErr( err_level::ECode errLevel, long errCode, const CString& msg )
	{
		std::cerr << "#" << CT2CA( GetErrLevelStr( errLevel ) ) << "\t" 
			<< CT2CA( GetErrMsg( errCode ) )<< "\t" << CT2CA( msg ) << std::endl;
	};

private:

	/** 
	 * @brief   エラーレベルの文字列取得
	 * @param   errLevel [in]  エラーレベル
	 * @retval  CString  エラーレベル文字列
	 */
	CString GetErrLevelStr( err_level::ECode errLevel )
	{
		switch (errLevel){
			using namespace err_level;
			case fatal:   return _T("FATAL");
			case error:   return _T("ERROR");
			case warning: return _T("WARNING");
			case info:    return _T("INFO");
			default:      return _T("");
		}
	};

	/**
	 * @brief   エラーコードからエラーメッセージ取得
	 * @param   errCode [in]  エラーコード
	 * @retval  CString エラーメッセージ
	 */
	CString GetErrMsg( long errCode )
	{
		switch( errCode ){
			using namespace err_code;

			// 9**:FATAL
			case OpenLogFail:      return _T("Open LogFile Fail");
			case OpenMeshFileFail: return _T("Open meshlist Fail");
			case OpenWsFail:       return _T("Open Workspace Fail");
			case OpenFcFail:       return _T("Open Feature Class Fail");
			case OpenTblFail:      return _T("Open Table Fail");
			case GetNineMeshFail:  return _T("隣接9メッシュポリゴン取得失敗");
			case GetRailFail:      return _T("鉄道NW系オブジェクト取得失敗");
			case GetStaPoiFail:    return _T("駅ポイント取得失敗");
			case GetBaseStaFail:   return _T("中縮駅舎ポリゴン取得失敗");
			case GetMeshBoundFail: return _T("メッシュの枠線取得失敗");
			case GetOIDAllFail:    return _T("全国分のObjectID取得失敗");
			//case GetRoadCodeList:  return _T("RoadCodeListの取得失敗");

			// 8**:INFO
			case StrRailNWDB:       return _T("Rail NW DB");
			case StrRailPoiDB:      return _T("Rail POI DB");
			case StrStationDB:      return _T("Station DB");
			case StrMeshDB:         return _T("Mesh DB");
			case StrLogFile:        return _T("Log File");
			case StrMeshList:       return _T("Mesh List");
			case StrDistThreshold:  return _T("Dist Threshold");
			case StrNeiThreshold:   return _T("Neighbor Threshold");
			case StrStartTime:      return _T("Start Time");
			case StrEndTime:        return _T("End Time");
			case StrStart:          return _T("Start");
			case StrQueryStart:     return _T("QueryChk Start");

				// 1**:Rail Node ERROR
			case NgRailNodeClass:   return _T("ノード種別コードが定義外");
			case NgRailNodeGeom:    return _T("形状が不正");
			case BorderCornerNode:  return _T("図郭の隅にノードがある");
			case NgBorderNodeAttr:  return _T("2次メッシュ境界線上にあるのに図郭上ノードになっていない");
			case NgBorderNodeGeom:  return _T("図郭上ノードが2次メッシュ境界線上に無い");
			case NoConnect:         return _T("鉄道リンクに接続していない");
			case NgTerminalNode:    return _T("端点なのに端点ノードになっていない");
			case NgBranchNode:      return _T("分岐なのに分岐ノードになっていない");
			case NgAlterNode:       return _T("接続するリンクの属性が異なるのに属性変化点ノードになっていない");
			case TouchNgLink:       return _T("ノードが関係ないリンクに接触している");
			case TouchNgNode:       return _T("ノードが別のノードに接触している");
			case NoTouchBaseSta:    return _T("中縮駅舎ポリゴン上に存在しない駅POIノードが存在する");
			case NgRelJoinLine1:    return _T("駅POIノードなのに路線リストの値がnull");
			case NgRelJoinLine2:    return _T("駅POIノード以外のノードで路線リストの値がnull以外");
			case NgRelJoinLine3:    return _T("接続しているリンクに存在しない路線IDがノードに存在している");
			case TooFarStaPoint:    return _T("駅POIノードの近傍に対応する駅POIが存在しない");


			// 3**:Rail Link ERROR
			case NgRailLinkClass:   return _T("鉄道種別コードが定義外");
			case NgDispScaleClass:  return _T("スケール表示コードが定義外");
			case NgFlagField:       return _T("フラグフィールドなのに、[0]と[1]以外の値");
			case NgFromID:          return _T("始点ノードIDが不正");
			case NgToID:            return _T("終点ノードIDが不正");
			case NgRailLinkGeom:    return _T("鉄道リンク形状が不正");
			case CrossMesh:         return _T("2次メッシュを横断している");
			case NoBorderNode:      return _T("メッシュ跨ぎのリンクに図郭ノードが存在しない");
			case NotFoundFromNode:  return _T("FromNodeIDのノードが見つからない");
			case NotFoundToNode:    return _T("ToNodeIDのノードが見つからない");
			case NotSameBorderAttr: return _T("2次メッシュ境界の隣接でリンクの属性が異なる");
			case NgBorderLink:      return _T("メッシュ境界で隣接リンクが不正");
			case UnMatchStartFrom:  return _T("リンクの始点とFromノードの座標が一致しない");
			case UnMatchEndTo:      return _T("リンクの終点とToノードの座標が一致しない");
			case UnderThresholdSeg: return _T("微小セグメントが存在する");
			case DupPoint:          return _T("構成点が重複している");
			case LoopLinkSingleFT:  return _T("リンクの始終点が等しい");
			case LoopLinkDupFT:     return _T("From/Toの組み合わせが同じリンクが複数存在する");
			case NgRelRailwayLine1: return _T("鉄道種別コードが0(鉄道POI種別で分類)で路線リストの値がnull");
			case NgRelRailwayLine2: return _T("鉄道種別コードが1or2(貨物線・鉄道側線部)で路線リストの値がnull以外");

			// 5**:Query ERROR
			case NonRelRailLink:    return _T("紐づく鉄道リンクが存在しない");
			case NonRelRailNode:    return _T("紐づく鉄道ノードが存在しない");
			case NotOnlySeqLine:    return _T("同じ路線名称で連続しているリンク群が複数存在している");
			case DupStationNum:     return _T("駅番号が重複している");
			case NoNumberStaNum:    return _T("駅番号に数字が存在しない");
			case NgSeqStaNum:       return _T("駅番号の並び順が不正");
			case NonJoinLinkInfo:   return _T("紐付く乗り入れ路線が存在しない");
			case NonRailwayLine:    return _T("紐付く鉄道路線が存在しない");

			default:                return _T("");
		}
	};

private:

	std::ofstream m_ofs; //!< ログ出力ストリーム
};
