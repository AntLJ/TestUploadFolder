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
#include <TDC/useful_headers/str_util.h>

// エラーレベル列挙型
namespace err_level {
	enum ECode {
		fatal,
		error,
		warning,
		info,
		notice
	};
}; // err_level


// エラーコード列挙型
namespace err_code {
	enum ECode {
		// エラーなし
		NoneError           = 0,

		// 9**:FATAL
		OpenLogFail			= 901,
		OpenMeshFileFail	= 902,
		OpenWsFail			= 903,
		OpenFcFail			= 904,
		OpenTblFail			= 905,
		GetNineMeshFail		= 906,
		GetRoadFail			= 907,
		GetWalkFail			= 908,
		GetStationFail		= 909,
		GetMeshBoundFail	= 910,
		GetWalkRelFail		= 911,
		GetPoiPointFail		= 912,
		GetUndergroundFail  = 914,
		NumErrInMeshList    = 915,
		DigitErrInMeshList  = 916,
		DataTypeError       = 917,
		DataTypeTargetError = 918,
		walkTypeError       = 919,
		GetScrambleAreaFail = 920,
		GetPedxingFail      = 921,
		PositionNearError   = 922,

		// 2**:ERROR
		FailedGeometryCulc  = 201,

		// 8**:INFO
		StrWalkDB			= 801,
		StrRoadDB			= 802,
		StrStationDB		= 803,
		StrMeshDB			= 804,
		StrLogDir			= 805,
		StrMeshList			= 806,
		StrDistThreshold	= 807,
		StrAngleThreshold	= 808,
		StrNeiThreshold		= 809,
		StrRefThreshold		= 810,
		StrStartTime		= 811,
		StrEndTime			= 812,
		StrStart			= 813,
		StrCRSThreshold		= 815,
		StrBuddyDistThreshold	= 816,
		StrBuddyAngleThreshold	= 817,
		StrCheckType		= 818,
		StrChecking			= 819,
		StrProductName		= 820,
		StrProductVer		= 821,
		StrHeightNodeINI	= 822,
		StrEnd				= 823,
		StrException		= 824,
		MaxLinkDistance		= 825,
		StrPoiDB			= 826,
		StrUnderDB          = 827,
		StrDayFilter        = 828,
		StrTimeFilter       = 829,

		// 1**:Walk Node ERROR
		NgWalkNodeClass		= 101,
		NgWalkNodeGeom		= 102,
		BorderCornerNode	= 103,
		NgBorderNodeAttr	= 104,
		NgBorderNodeGeom	= 105,
		NoConnect			= 106,
		NgOnRoadNode1		= 107,
		NgOnRoadNode2		= 108,
		NgTerminalNode1		= 109,
		NgTerminalNode2		= 110,
		TouchNgLink			= 111,
		TouchNgNode			= 112,
		BorderRoadNodeID	= 113,
		NgTerminalLink		= 115,
		NearOtherName		= 116,
		NotFindRoadNode		= 117,
		NotFindStation		= 118,
		NgStationNode		= 119,
		NoNameRoadNode		= 120,
		TooFarRoadNode		= 121,
		TooFarStation		= 122,
		NotConnectNW		= 123,
		NotConnectCross		= 124,
		NgOnRoadLink		= 125,
		ConnectCarOnly		= 126,
		NgStationGateID		= 127,
		NgUnderGateNode		= 128,
		NotFindUnderGate	= 129,
		TooFarUnderGate		= 130,
		NgUnderGateID		= 131,
		NgTicketGate1       = 132,
		NgTicketGate2       = 133,
		NgTicketGate3       = 134,
		NgOnRoadLinkException   = 135,
		NgNotOnRoadLink     = 136,
		NgOnRoadLinkCross   = 137,
		NgOnWalkLinkCross   = 138,
		ConnectWalkLink     = 139,
		NgConnectWalkLinkAngle   = 140,

		// 3**:Walk Link ERROR
		NgWalkLinkClass1	= 301,
		NgWalkLinkClass2	= 302,
		NgFloorMoveClass	= 303,
		NgOnewayCode1		= 304,
		NgNoPassCode		= 305,
		NgWalkCodeC			= 309,
		NgFlagField			= 310,
		NgFromID			= 311,
		NgToID				= 312,
		NgWalkLinkGeom		= 313,
		CrossMesh			= 314,
		NoBorderNode		= 315,
		NotFoundFromNode	= 316,
		NotFoundToNode		= 317,
		NotSameBorderAttr	= 318,
		NgBorderLink		= 319,
		NgBorderOneway		= 320,
		UnMatchStartFrom	= 321,
		UnMatchEndTo		= 322,
		UnderThresholdSeg	= 323,
		UnderThresholdAg	= 324,
		DupPoint			= 325,
		LoopLinkSingleFT	= 326,
		LoopLinkDupFT		= 327,
		NgFloorMoveAndF		= 329,
		ExistFloorMove		= 330,
		NotFoundWalkCode	= 331,
		DiffWalkCode		= 332,
		NotOnewayCode		= 334,
		UnMatchRoadLink		= 335,
		NotConOpenAreaF		= 336,
		NotConRoadWalkLink	= 337,
		ExistFlag			= 338,
		NgRoadSurfaceClass	= 339,
		ThreeMoreCross		= 340,
		NotConRoadNwNode	= 341,
		NgHumanCarAndF		= 342,
		NgNotNoPassInfo		= 343,
		NgBorderNoPassage	= 344,
		NgLinkRelationF		= 345,
		NgHCWalkCode		= 346,
		NgNoPassageRF		= 347,
		NgOnewayRF			= 348,
		NgNotOnewayInfo		= 349,
		BothRoadNW			= 350,
		NgRoadLinkWalkable	= 351,
		WithInRoadNode		= 352,
		DiffWalkClass		= 353,
		DiffFloorLevel		= 354,
		DiffFloorMoveCode	= 355,
		SameEscOnway		= 356,
		NgMovingWalkArcade	= 357,
		MatchRoadLink		= 358,
		NgUnderConnect      = 359,
		PointOnOtherLink    = 360,
		NgCrossRoadNode     = 361,
		NgInternalTicketGate= 362,
		NgWalkLinkNotOnScrambleArea = 363,
		NgWalkLinkOnScrambleArea    = 364,
		NgWalkLinkWheelChair        = 365,
		NgWalkLinkPlatformNumber    = 366,
		MicroLogicalConnection      = 367,
		NgCrossWalkLink     = 368,

		// 4**:Walk Link WARN
		ExistOnewayCode		= 401,
		CrossMultiLane		= 402,
		NgBorthCrossRoadNode= 403,


		// 5**:Road Link ERROR
		OutWalkable			= 501,
		NgWalkable			= 502,
		NotWalkSandwiched	= 503,
		NgCarOnly			= 504,
		NgPedestrianZone	= 505,
		NgPedestrianAndF	= 506,
		NgPedZoneCarOnly	= 507,
		HumanCarSandwiched  = 508,
		NgWalkableClass     = 509,
		NgOverMeshWalkableClass= 510,
		NgConnectedSideWalk = 511,


		// 6**:Walk Code List ERROR
		NgWalkCode	= 601,
		NgWalkCodeKanji1	= 602,
		NgWalkCodeKanji2	= 603,
		NotExistWalkCodeWL	= 604,
		NgWalkCodeYomi		= 605,

		// 7**:Walk No Passage ERROR
		NotExistNoPassWL	= 701,
		OverLapNoPassTime	= 702,
		NgNoPassageTime		= 703,
		SameNoPassageTime	= 704,
		NgNoPassFullYear	= 705,
		NgNoPassLinkID      = 706,

		// 10**:Height Node ERROR
		NoExistLink			= 1001,
		NgHeight			= 1002,
		NgLinkTable1		= 1003,
		NgLinkTable2		= 1004,
		NgBothRoadLink		= 1005,
		NgSelhCross			= 1006,
		NgHeightEqual		= 1007,
		NgHNWalkClass		= 1008,
		NgHeightWalk		= 1009,
		NgHeightRoad		= 1010,
		NoExistPointOnLink	= 1011,
		NearNoramlNode		= 1012,
		NearHeightNode		= 1013,
		NearSegment			= 1014,
		LargeDifference		= 1015,
		NonExistHeightNode  = 1016,
		NgHeightNodeGeometry  = 1017,
		NgHeightNodeLinkID  = 1018,

		// 11**:Walk Oneway ERROR
		NotExistOnewayWL	= 1101,
		OverLapOnewayTime = 1102,
		NgOnewayTime	= 1103,
		SameOnewayTime	= 1104,
		NgOnewayFullYear	= 1105,
		NgOnewayLinkID	= 1106,
		NgOnewayLinkDir	= 1107,

		// 12**:LinkRealation ERROR
		NotExistWalkLink	= 1201,
		NotExistRoadLink	= 1202,
		NgLRWalkClass		= 1203,
		NgLRRoadClass		= 1204,
		NgLRWalkFlag		= 1205,
		NgLinkDist			= 1206,
		NgLinkAngle			= 1207,
		NgConnectRelLinks	= 1208,
		NgBuddyRelation		= 1209,
		NgLRRoadLinkWalkable= 1210,
		NgDuplicatedRecord  = 1211,

		// 13**:StationGatePoint ERROR
		NotExistSameNameSGP = 1300,

		// 14**:PoiPointOrg ERROR
		NotExistSameNamePOI = 1400,
		NotAlivePOI         = 1401,

		// 15**:RoadNode ERROR
		NotExistSameNameRND = 1500,

		// 16**:WalkLink-UndergroundArea ERROR
		PartOfUnderLinkIncludedArea  = 1600,
		UndergroundAreaNoneOverLinks = 1601,
		UndergroundAreaNotExist      = 1602,

		// 17**:横断ポイント
		NgPedxingGeometry       = 1700,			
		NgPedxingOnScrambleArea = 1701,
		NgPedxingOnRoadLink     = 1702,
		NgPedxingCrossRoadLink  = 1703,
		NgPedxingCrossWalkLink  = 1704,
		NgPedxingWithWalkLink   = 1705,
		NgPedxingClass          = 1706,	
		CrossRoadLink           = 1707,	

		// 18**:スクランブルエリア
		NgScrambleAreaGeometry       = 1800,
		NgScrambleAreaCrossWalkLink  = 1801,
		NgScrambleAreaClass          = 1802,

	};
}; // err_code

/**
 * @brief	出力クラス
 * @note	標準出力 / UTF-8のログ出力
 */
class COutPut
{
public:

	/**
	 * @brief	コンストラクタ
	 */
	COutPut(){}

	/**
	 * @brief	デストラクタ
	 */
	virtual ~COutPut() { Close(); }

	/**
	 * @brief	オープン
	 * @param	strFileName [in]	ログファイル名
	 * @retval	true	オープン成功
	 * @retval	false オープン失敗
	 */
	bool Open( const CString& strDirName );

	/**
	 * @brief	クローズ
	 */
	void Close();

	/**
	 * @brief	ヘッダ出力
	 */
	void OutputHeader();

	/**
	 * @brief	ログ出力
	 * @note	SINDYSTDLOG形式
	 * @param	layer	[in]	レイヤ名
	 * @param	id	[in]	ObjectID
	 * @param	errLevel	[in]	エラーレベル
	 * @param	errCode	[in]	エラーコード
	 * @param	meshcode	[in]	メッシュコード
	 * @param	modOperator	[in]	最終更新者
	 * @param	modDate	[in]	最終更新日
	 * @param	modProgram	[in]	最終更新プログラム名
	 * @param	modProgramDate	[in]	最終プログラム更新日
	 * @param	info1	[in]	メッセージ1
	 * @param	info2	[in]	メッセージ2
	 * @param	x	[in]	X座標
	 * @param	y	[in]	Y座標
	 * @param	isGeneral	[in]	汎用向け場合フラグ
	 */
	void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode,
		const CString& meshcode = _T(""), const CString& modOperator = _T(""), const CString& modDate = _T(""),
		const CString& modProgram = _T(""), const CString& modProgramDate = _T(""),
		const CString& info1 = _T(""), const CString& info2 = _T(""), double x = 0.0, double y = 0.0, bool isGeneral = true );

	/**
	* @brief	ログ出力
	* @note	SINDYSTDLOG形式
	*       オブジェクト固有情報は、予め設定されたものを使用する版
	* @param	errLevel	[in]	エラーレベル
	* @param	errCode	[in]	エラーコード
	* @param	info1	[in]	メッセージ1
	* @param	info2	[in]	メッセージ2
	* @param	x	[in]	X座標
	* @param	y	[in]	Y座標
	* @param	isGeneral	[in]	汎用向け場合フラグ
	*/
	void OutputRegLog( err_level::ECode errLevel, long errCode,
		const CString& info1 = _T( "" ), const CString& info2 = _T( "" ), double x = 0.0, double y = 0.0, bool isGeneral = true );

	/**
	 * @brief	ログ出力
	 * @note	経緯度のみ出力する形式
	 * @param	layer	[in]	レイヤ名
	 * @param	x	[in]	経度
	 * @param	y	[in]	緯度
	 * @param	meshcode	[in]	メッシュコード
	 * @param	errLevel [in]	エラーレベル
	 * @param	errCode	[in]	エラーコード
	 */
	void OutputLog(	const CString& layer, const CString& meshcode, double x, double y,
		err_level::ECode errLevel, long errCode );

	/**
	 * @brief	ログ出力
	 * @note	コメント形式
	 * @param	errLevel [in]	エラーレベル
	 * @param	errCode	[in]	エラーコード
	 * @param	msg	[in]	フリーメッセージ
	 */
	void OutputLog( err_level::ECode errLevel, long errCode, const CString& msg );

	/**
	 * @brief	標準出力
	 * @param	errLevel [in]	エラーレベル
	 * @param	errCode	[in]	エラーコード
	 * @param	msg	[in]	フリーメッセージ
	 */
	void OutputStdErr( err_level::ECode errLevel, long errCode, const CString& msg );

	/**
	* @brief	エラー出力のレイヤ名設定
	* @param	layer [in]	レイヤ名
	*/
	void SetLayer( const CString& layer ) { m_layer = layer; }

	/**
	* @brief	エラー出力の共通部を設定
	* @param	id [in]	オブジェクトID
	* @param	modOperator	[in]	最終更新者
	* @param	modDate	[in]	最終更新日
	* @param	modProgram	[in]	最終更新プログラム名
	* @param	modProgramDate	[in]	最新プログラム更新日
	*/
	void SetModifyInfo( const long id, const CString& modOperator, const CString& modDate, const CString& modProgram, const CString& modProgramDate );

	/**
	* @brief	エラー出力のメッシュコード設定
	* @param	meshcode [in]	メッシュコード
	*/
	void SetMeshCode( const CString meshcode );

	/**
	* @brief	エラー出力のオブジェクトID設定
	* @note	本来、単体で使用する場面はないはずだが、一部互換を保つため（多分バグ）に用意
	* @param	id [in]	オブジェクトID
	*/
	void SetID( const long id ) { m_id = id; }
private:

	/** 
	 * @brief	エラーレベルの文字列取得
	 * @param	errLevel [in]	エラーレベル
	 * @retval	CString	エラーレベル文字列
	 */
	CString GetErrLevelStr( err_level::ECode errLevel );

	/**
	 * @brief	エラーコードからエラーメッセージ取得
	 * @param	errCode [in]	エラーコード
	 * @retval	CString エラーメッセージ
	 */
	CString GetErrMsg( long errCode );

	/**
	 * @brief	エラーコードからダウンコンバート版エラーメッセージ取得
	 * @param	[in] errCode エラーコード
	 * @param	[in] info	 メッセージ
	 * @retval	CString エラーメッセージ
	 */
	CString GetDownConvertErrMsg( long errCode,  const CString& info = _T("") );

	/**
	 * @brief	エラーコードとメッセージからエラーメッセージ取得
	 * @param	errCode [in]    エラーコード
	 * @param	info    [in]    メッセージ
	 * @return  CString エラーメッセージ
	 */
	CString GetErrEnumMsg( long errCode, const CString& info);

private:

	std::ofstream m_ofs_run;	//!< 実行ログ出力ストリーム
	std::ofstream m_ofs_error;	//!< エラーログ出力ストリーム
	CString m_layer;			//!< プリセットレイヤ名
	long m_id;					//!< プリセットレイヤ
	CString m_meshcode;			//!< プリセットメッシュコード
	CString m_modOperator;		//!< プリセット最終編集者
	CString m_modDate;			//!< プリセット最終更新日付
	CString m_modProgram;		//!< プリセット最終更新プログラム名
	CString m_modProgramDate;	//!< プリセットプログラム最終更新日付

};
