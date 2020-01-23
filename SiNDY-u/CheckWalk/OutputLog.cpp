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

#include "stdafx.h"
#include "OutputLog.h"
#include <boost/filesystem.hpp>

namespace
{
	/**
	 * @brief ディレクトリパス内にファイルバージョンと日付を付与したファイルを生成
	 * @param fileName [in] 生成するファイル名
	 * @param dirPath  [in] ディレクトリパス
	 */
inline CString createFilepath(
	const CString& fileName
	, const CString& dirPath
	, CVersion& version
	, SYSTEMTIME& time
	)
{
	boost::filesystem::path filepath(dirPath);
	filepath /= version.GetInternalName() + _T("_") + uh::str_util::ToString(&time, _T("%04d%02d%02d%02d%02d%02d")) + _T("_") + fileName;
	return filepath.c_str();
}

} // namespace

bool COutPut::Open( const CString& strDirName )
{
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CVersion cVer;
	CStringA runLogFile = createFilepath( _T("run.log"), strDirName, cVer, aTime);
	CStringA errLogFile = createFilepath( _T("err.log"), strDirName, cVer, aTime);
	m_ofs_run.open( runLogFile.GetString() );
	m_ofs_error.open( errLogFile.GetString() );

	if( ! m_ofs_run || ! m_ofs_error )
		return false;
	return true;
}

void COutPut::Close(){
	m_ofs_run.close();
	m_ofs_error.close();
}

void COutPut::OutputHeader()
{
	if( ! m_ofs_error )
		return;
	CString logStyle = _T("# FREESTYLELOG");
	m_ofs_error << CT2CA( logStyle ) << std::endl;
	std::vector<CString> fieldList = boost::assign::list_of
		( _T( "FLAG" ) )
		( _T( "LAYER" ) )
		( _T( "OBJECTID" ) )
		( _T( "メッシュコード" ) )
		( _T( "LON" ) )
		( _T( "LAT" ) )
		( _T( "エラーレベル" ) )
		( _T( "エラーコード" ) )
		( _T( "エラーメッセージ" ) )
		( _T( "情報1" ) )
		( _T( "情報2" ) )
		( _T( "最終更新者" ) )
		( _T( "最終更新日時" ) )
		( _T( "最終更新プログラム" ) )
		( _T( "最終プログラム更新日時" ) );
	m_ofs_error << CT2CA( uh::str_util::join( fieldList, _T("\t") ) ) << std::endl;

	if( ! m_ofs_run )
		return;
	CVersion cver;
	OutputLog(err_level::info, err_code::StrProductName, cver.GetFileDescription());
	OutputLog(err_level::info, err_code::StrProductVer, cver.GetProductVersion());
}

void COutPut::SetModifyInfo( const long id, const CString& modOperator, const CString& modDate, const CString& modProgram, const CString& modProgramDate )
{
	m_id = id;
	m_modOperator = modOperator;
	m_modDate = modDate;
	m_modProgram = modProgram;
	m_modProgramDate = modProgramDate;
}

void COutPut::SetMeshCode( const CString meshcode )
{
	m_meshcode = meshcode;
}

void COutPut::OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode,
	const CString& meshcode/* = _T("")*/, const CString& modOperator/* = _T("")*/, const CString& modDate/* = _T("")*/,
	const CString& modProgram/* = _T("")*/, const CString& modProgramDate/* = _T("")*/,
	const CString& info1/* = _T("")*/, const CString& info2/* = _T("")*/, double x/* = 0.0*/, double y/* = 0.0*/, bool isGeneral/* = true*/ )
{
	if( ! m_ofs_error )
		return;

	CString errMsg = isGeneral?GetErrMsg( errCode ):GetDownConvertErrMsg( errCode );
	CString str;
	if(x == 0.0 && y == 0.0){
		// X座標、Y座標がともに0の場合、座標フィールドは空にする
		str.Format( _T("0\t%s\t%ld\t%s\t\t\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s"),
			layer, id, meshcode, GetErrLevelStr( errLevel ), errCode, errMsg,
			info1, info2, modOperator, modDate, modProgram, modProgramDate );
	}else{
		str.Format( _T("0\t%s\t%ld\t%s\t%lf\t%lf\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s"), 
			layer, id, meshcode, x, y, GetErrLevelStr( errLevel ), errCode, errMsg, 
			info1, info2, modOperator, modDate, modProgram, modProgramDate );
	}
	m_ofs_error << CT2CA( str ) << std::endl;
}


void COutPut::OutputRegLog( err_level::ECode errLevel, long errCode,
	const CString& info1/* = _T("")*/, const CString& info2/* = _T("")*/, double x/* = 0.0*/, double y/* = 0.0*/, bool isGeneral/* = true*/ )
{
	if ( !m_ofs_error )
		return;

	CString str, info;
	if( errCode == err_code::ExistFlag ) info = info2;
	else if( errCode == err_code::PositionNearError) info = info1;

	CString message = isGeneral? GetErrMsg(errCode) : GetDownConvertErrMsg(errCode, info);

	if ( x == 0.0 && y == 0.0 ) {
		// X座標、Y座標がともに0の場合、座標フィールドは空にする
		str.Format( _T( "0\t%s\t%ld\t%s\t\t\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s" ),
			m_layer, m_id, m_meshcode, GetErrLevelStr( errLevel ), errCode, message,
			info1, info2, m_modOperator, m_modDate, m_modProgram, m_modProgramDate );
	}
	else {
		str.Format( _T( "0\t%s\t%ld\t%s\t%lf\t%lf\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s" ),
			m_layer, m_id, m_meshcode, x, y, GetErrLevelStr( errLevel ), errCode, message,
			info1, info2, m_modOperator, m_modDate, m_modProgram, m_modProgramDate );
	}
	m_ofs_error << CT2CA( str ) << std::endl;
}

void COutPut::OutputLog( const CString& layer, const CString& meshcode, double x, double y,
	err_level::ECode errLevel, long errCode )
{
	if( ! m_ofs_error )
		return;
	CString str;
	// OBJECTIDがないため、空にする
	str.Format( _T("0\t%s\t\t%s\t%0.13lf\t%0.13lf\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s"),
			layer, meshcode, x, y, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ),
			_T(""), _T(""), _T(""), _T(""), _T(""), _T("") );
	m_ofs_error << CT2CA( str ) << std::endl;
}

void COutPut::OutputLog( err_level::ECode errLevel, long errCode, const CString& msg )
{
	if( ! m_ofs_run )
		return;
	CString str;
	str.Format( _T("# %s\t%s\t%s"), GetErrLevelStr( errLevel ), GetErrMsg( errCode ), msg );
	m_ofs_run << CT2CA( str ) << std::endl;
}


void COutPut::OutputStdErr( err_level::ECode errLevel, long errCode, const CString& msg )
{
	std::cerr << "#" << CT2CA( GetErrLevelStr( errLevel ) ) << "\t" 
		<< CT2CA( GetErrMsg( errCode ) )<< "\t" << CT2CA( msg ) << std::endl;
}


CString COutPut::GetErrLevelStr( err_level::ECode errLevel )
{
	switch (errLevel){
	case err_level::fatal:	return _T("FATAL");
	case err_level::error:	return _T("ERROR");
	case err_level::warning: return _T("WARNING");
	case err_level::info:	return _T("INFO");
	case err_level::notice: return _T("NOTICE");
	default:	return _T("");
	}
}

CString COutPut::GetDownConvertErrMsg( long errCode, const CString& info )
{
	switch( errCode ){
		using namespace err_code;
		case NotConnectNW:		return _T("道路NWにつながっていない");
		case NgWalkLinkClass1:	return _T("歩行者リンク種別コードが定義外");
		case NotConRoadWalkLink:		return _T("接続リンク・ロジカルコネクションが道路NWに接続していない");
		case MatchRoadLink:		return _T("ロジカルコネクションではない歩行者リンクが道路リンクに重なっている");
		case NgHNWalkClass:		return _T("歩行者リンク種別がHEIGHT_NODE整備対象外");
		case ExistFlag: 
		case PositionNearError: return GetErrEnumMsg(errCode, info);

		default:	return _T("");
	}
}

CString COutPut::GetErrMsg( long errCode )
{
	switch( errCode ){
		using namespace err_code;

		// 2**:ERROR
		case FailedGeometryCulc: return _T("チェック過程で形状計算に失敗");

		// 9**:FATAL
		case OpenLogFail:		return _T("Open LogFile Fail");
		case OpenMeshFileFail:	return _T("Open meshlist Fail");
		case OpenWsFail:		return _T("Open Workspace Fail");
		case OpenFcFail:		return _T("Open Feature Class Fail");
		case OpenTblFail:		return _T("Open Table Fail");
		case GetNineMeshFail:	return _T("隣接9メッシュポリゴン取得失敗");
		case GetRoadFail:		return _T("道路系オブジェクト取得失敗");
		case GetWalkFail:		return _T("歩行者系オブジェクト取得失敗");
		case GetStationFail:	return _T("駅出入り口ポイント取得失敗");
		case GetMeshBoundFail:	return _T("メッシュの枠線取得失敗");
		case GetWalkRelFail:	return _T("歩行者関連テーブルの取得失敗");
		case GetPoiPointFail:	return _T("POIポイント取得失敗");
		case NumErrInMeshList:  return _T("メッシュリストに数字以外が存在");
		case DigitErrInMeshList:return _T("メッシュコードの桁数が誤っている");
		case DataTypeError:     return _T("オプションdata_typeに異常値(1、2以外の値)");
		case DataTypeTargetError:     return _T("オプションdata_typeと設定ファイルの[target]オプション一致しない");
		case walkTypeError:     return _T("データベース中のWALKTYPE_Cと指定オプションdata_type値が合っていない");
		case GetScrambleAreaFail:	  return _T("メッシュのスクランブルエリア取得失敗");
		case GetPedxingFail:	return _T("メッシュの横断ポイント取得失敗");
		
		// 8**:INFO
		case StrWalkDB:			return _T("Walk DB");
		case StrRoadDB:			return _T("Road DB");
		case StrStationDB:		return _T("Station DB");
		case StrMeshDB:			return _T("Mesh DB");
		case StrLogDir:			return _T("Log Dir");
		case StrMeshList:		return _T("Mesh List");
		case StrDistThreshold:	return _T("Dist Threshold");
		case StrAngleThreshold:	return _T("Angle Threshold");
		case StrNeiThreshold:	return _T("Neighbor Threshold");
		case StrRefThreshold:	return _T("ReferID Threshold");
		case StrStartTime:		return _T("Start Time");
		case StrEndTime:		return _T("End Time");
		case StrStart:			return _T("Start");
		case StrEnd:			return _T("End");
		case StrException:		return _T("Exception");
		case StrCRSThreshold:	return _T("Cross Threshold");
		case StrBuddyDistThreshold:		return _T("Buddy Dist Threshold");
		case StrBuddyAngleThreshold:	return _T("Buddy Angle Threshold");
		case StrCheckType:		return _T("Check Type");
		case StrChecking:		return _T("Checking");
		case StrProductName:	return _T("Product Name");
		case StrProductVer:		return _T("Product Version");
		case StrHeightNodeINI:	return _T("HeightNode INI");
		case MaxLinkDistance:	return _T("Max LinkRelation Distance");
		case StrPoiDB:			return _T("POI DB");
		case StrUnderDB:		return _T("UnderGroundArea DB");
		case StrDayFilter:      return _T("day");
		case StrTimeFilter:     return _T("time");

		// 1**:Walk Node ERROR
		case NgWalkNodeClass:	return _T("歩行者ノード種別コードが定義外");
		case NgWalkNodeGeom:	return _T("歩行者ノード形状が不正");
		case BorderCornerNode:	return _T("図郭の隅にノードがある");
		case NgBorderNodeAttr:	return _T("2次メッシュ境界線上にあるのに図郭上ノードになっていない");
		case NgBorderNodeGeom:	return _T("図郭上ノードが2次メッシュ境界線上に無い");
		case NoConnect:			return _T("歩行者リンクに接続していない");
		case NgOnRoadNode1:		return _T("道路ノード上にあるのに道路NW接続ノードになっていない");
		case NgOnRoadNode2:		return _T("道路NW接続ノードが道路ノード上に無い");
		case NgTerminalNode1:	return _T("端点なのに端点ノードになっていない");
		case NgTerminalNode2:	return _T("接続リンクが複数なのに、端点ノード");
		case TouchNgLink:		return _T("ノードが関係ないリンクに接触している");
		case TouchNgNode:		return _T("ノードが別のノードに接触している");
		case BorderRoadNodeID:	return _T("図郭上ノードに道路ノード対応IDが付与されている");
		case NgTerminalLink:	return _T("端点ノードに接続してはいけない歩行者リンク種別");
		case NearOtherName:		return _T("近傍に別の交差点名称を持つノードが存在する");
		case NotFindRoadNode:	return _T("道路ノード対応IDに対応する道路ノードがない");
		case NotFindStation:	return _T("駅出入口ポイントIDに対応する駅出入口POIがない");
		case NgStationNode:		return _T("駅出入口ノードなのに駅出入口ポイントIDが付与されていない");
		case NoNameRoadNode:	return _T("道路ノード対応IDに対応する道路ノードに交差点名称がない");
		case TooFarRoadNode:	return _T("道路ノード対応IDに対応する道路ノードが近くにない");
		case TooFarStation:		return _T("駅出入口ポイントIDに対応する駅出入口POIが近くにない");
		case NotConnectNW:		return _T("道路NW接続ノードが存在しない");
		case NotConnectCross:	return _T("道路ノード対応IDをもつのに横断歩道リンクに接続していない");
		case NgOnRoadLink:		return _T("歩行者ノードが道路リンクに乗っている");
		case ConnectCarOnly:	return _T("接続する道路リンクが歩行禁止のリンクのみ");
		case NgStationGateID:	return _T("駅出入口ポイントIDが付与されているのに駅出入口ノードではない");
		case NgUnderGateNode:	return _T("地下街出入口ノードなのに地下街出入口ポイントIDが付与されていない");
		case NotFindUnderGate:	return _T("地下街出入口ポイントIDに対応する地下街出入口POIがない");
		case TooFarUnderGate:	return _T("地下街出入口ポイントIDに対応する地下街出入口POIが近くにない");
		case NgUnderGateID:		return _T("地下街出入口ポイントIDが付与されているのに地下街出入口ノードではない");
		case NgTicketGate1:		return _T( "改札ノードなのに端点になっている" );
		case NgTicketGate2:		return _T( "改札ノードに改札内リンクが接続していない" );
		case NgTicketGate3:		return _T( "改札ノードから道路接続ノードまで接続されていない" );
		case NgOnRoadLinkException:		return _T("道路リンク上にあるのに道路リンク接続ノードになっていない");
		case NgNotOnRoadLink:		    return _T("道路リンク接続ノードが道路リンク上に無い");
		case NgOnRoadLinkCross:	return _T("道路リンク接続ノードが道路リンク交差箇所にある");
		case NgOnWalkLinkCross:	return _T("スクランブル交差点なのにスクランブルエリアポリゴンが無い");
		case ConnectWalkLink:	return _T("ロジカルコネクションが複数接続している");
		case NgConnectWalkLinkAngle:	return _T("ロジカルコネクションが鋭角に接続している");

		// 3**:Walk Link ERROR
		case NgWalkLinkClass1:	return _T("歩行者種別が定義外");
		case NgWalkLinkClass2:	return _T("歩行者種別コードに整備対象外の値が付与されている");
		case NgFloorMoveClass:	return _T("階層移動種別コードが定義外");
		case NgOnewayCode1:		return _T("一方通行種別コードが定義外");
		case NgNoPassCode:		return _T("歩行者通行禁止コードが定義外");
		case NgWalkCodeC:		return _T("歩行者路線コードが定義外");
		case NgFlagField:		return _T("フラグフィールドなのに、[0]と[1]以外の値");
		case NgFromID:			return _T("始点ノードIDが不正");
		case NgToID:			return _T("終点ノードIDが不正");
		case NgWalkLinkGeom:	return _T("歩行者リンク形状が不正");
		case CrossMesh:			return _T("2次メッシュを横断している");
		case NoBorderNode:		return _T("メッシュ跨ぎのリンクに図郭ノードが存在しない");
		case NotFoundFromNode:	return _T("FromNodeIDのノードが見つからない");
		case NotFoundToNode:	return _T("ToNodeIDのノードが見つからない");
		case NotSameBorderAttr:	return _T("メッシュ境界の隣接で属性が異なる");
		case NgBorderLink:		return _T("メッシュ境界で隣接リンクが不正");
		case NgBorderOneway:	return _T("一方通行が2次メッシュ境界で途切れている");
		case UnMatchStartFrom:	return _T("リンクの始点とFromノードの座標が一致しない");
		case UnMatchEndTo:		return _T("リンクの終点とToノードの座標が一致しない");
		case UnderThresholdSeg: return _T("微小セグメントが存在する");
		case UnderThresholdAg:	return _T("急激なターンが存在する");
		case DupPoint:			return _T("構成点が重複している");
		case LoopLinkSingleFT:	return _T("リンクの始終点が等しい");
		case LoopLinkDupFT:		return _T("同一From/Toを持つリンクが複数存在");
		case NgFloorMoveAndF:	return _T("階層移動種別が付与されていない");
		case ExistFloorMove:	return _T("階層移動種別が付与されている");
		case NotFoundWalkCode:	return _T("対応する歩行者路線コードデータがない");
		case DiffWalkCode:		return _T("歩行者路線コードが前後のリンクで異なる");
		case NotOnewayCode:		return _T("一方通行種別が付与されていない");
		case UnMatchRoadLink:	return _T("人車共用リンクが道路リンクに含まれていない");
		case NotConOpenAreaF:	return _T("広場が閉じていない");
		case NotConRoadWalkLink:	return _T("接続リンクが道路リンク、または人車共用リンクに接続していない");
		case ExistFlag:			return _T("フラグ整備対象外の歩行者種別に対しフラグが整備されている");
		case NgRoadSurfaceClass:	return _T("路面素材コードが定義外");
		case ThreeMoreCross:	return _T("2つ以上のリンクとの交差箇所が近い");
		case NotConRoadNwNode:	return _T("人車共用リンク群が道路NW接続ノードに接続していない");
		case NgHumanCarAndF:	return _T("人車共用リンクに規制が整備されている");
		case NgNotNoPassInfo:	return _T("通行禁止時間規制関連フラグがONだが、通行禁止規制情報が存在しない");
		case NgBorderNoPassage: return _T("通行禁止が2次メッシュ境界で変わっている");
		case NgLinkRelationF:	return _T("リンク紐づけテーブルフラグがONだが、歩行者・道路リンク紐付けテーブル情報がない");
		case NgHCWalkCode:		return _T("人車共用リンクに歩行者路線コードが整備されている");
		case NgNoPassageRF:		return _T("通行禁止種別コードと関連フラグの整合性がとれていない");
		case NgOnewayRF:		return _T("一方種別コードと関連フラグの整合性がとれていない");
		case NgNotOnewayInfo:	return _T("一方通行時間規制関連フラグがONだが、一方通行規制情報が存在しない");
		case BothRoadNW:		return _T("Fromノード,Toノード共に道路NW接続ノード");
		case NgRoadLinkWalkable:	return _T("人車共用リンクと一致する道路リンクが歩行禁止以外");
		case WithInRoadNode:	return _T("歩行者リンクに道路ノードが含まれている");
		case DiffWalkClass:		return _T("歩行者種別が前後のリンクと異なる");
		case DiffFloorLevel:	return _T("階層レベルが前後のリンクで異なる");
		case DiffFloorMoveCode:	return _T("並行リンクの階層移動種別の方向が異なる");
		case SameEscOnway:		return _T("並行リンクのエスカレータの一方通行方向が同じ");
		case NgMovingWalkArcade:	return _T("エスカレータ/オートウォークフラグがONだが、屋根付きフラグがOFF");
		case MatchRoadLink:		return _T("人車共用リンクではないのに道路リンクにマッチしている");
		case NgUnderConnect:    return _T("エレベーターも降りる階層もないのに地下に繋がっている");
		case PointOnOtherLink:  return _T("構成点か端点が他のリンクに乗っている");
		case NgCrossRoadNode:  return _T( "横断歩道の片側にしか道路ノードIDが付与されていない" );
		case NgInternalTicketGate:return _T( "改札内リンクと改札外リンク間のノードが改札ノードになっていない" );
		case NgBorthCrossRoadNode:return _T("横断歩道の両端ノードが特定できない");
		case NgWalkLinkNotOnScrambleArea: return _T("スクランブルフラグが付与されているのにスクランブルエリア内に無い");
		case NgWalkLinkOnScrambleArea:    return _T("スクランブルエリア内なのにスクランブルフラグが付与されていない");
		case NgWalkLinkWheelChair:        return _T("車椅子通行コードが不正な値");
		case NgWalkLinkPlatformNumber:    return _T("のりば番号が不正な値");
		case MicroLogicalConnection:      return _T("微小ロジカルコネクション");
		case NgCrossWalkLink:   return _T("ロジカルコネクションと交差している");

		// 4**:Walk Link WARN
		case ExistOnewayCode:	return _T("一方通行種別が付与されている");
		case CrossMultiLane:	return _T("みなし横断歩道が交差している道路リンクが2車線以上");

		// 5**:Road Link ERROR
		case OutWalkable:        return _T("歩行可能種別コードが定義外");
		case NgWalkable:         return _T("道路リンクなのに歩行可能種別コードが歩行者専用になっている");
		case NotWalkSandwiched:   return _T("人車共用の中で歩行禁止が浮いている");
		case NgCarOnly:          return _T("自動車専用道なのに歩行可能種別コードが歩道なし（歩行禁止）になっていない");
		case NgPedestrianZone:   return _T("歩行者天国フラグが定義外");
		case NgPedestrianAndF:   return _T("歩行者天国フラグがONだが、通行禁止規制情報が存在しない");
		case NgPedZoneCarOnly:   return _T("歩行者天国フラグがONだが道路リンク種別が自動車専用道");
		case HumanCarSandwiched: return _T("歩行禁止の中で人車共用が浮いている");
		case NgConnectedSideWalk:return _T("歩道がつながっていない");
		case NgWalkableClass:    return _T("歩行可能種別コードが不正な値");
		case NgOverMeshWalkableClass:    return _T("メッシュ境界で歩行可能種別が変わっている");

		// 6**:Walk Code List ERROR
		case NgWalkCode:		return _T("歩行者路線コードが不正な値");
		case NgWalkCodeKanji1:	return _T("歩行者路線漢字が不正");
		case NgWalkCodeKanji2:	return _T("歩行者路線漢字がデフォルト値");
		case NotExistWalkCodeWL:	return _T("歩行者路線コードが歩行者リンクに1件も存在しない");
		case NgWalkCodeYomi:	return _T("歩行者路線読みが不正");

		// 7**:Walk No Passage ERROR
		case NotExistNoPassWL:	return _T("通行禁止規制条件テーブルで指定した歩行者リンクが存在しない");
		case OverLapNoPassTime:	return _T("通行禁止の規制時間が重なっている");
		case NgNoPassageTime:	return _T("通行禁止の規制条件が不正な値");
		case SameNoPassageTime:	return _T("通行禁止規制条件が同じものが複数存在している");
		case NgNoPassFullYear:	return _T("通行禁止規制条件の通年が1/1～12/31になっていない");
		case NgNoPassLinkID:	return _T("リンクIDが不正な値");

		// 10**:Height Node ERROR
		case NoExistLink:		return _T("リンクIDに指定したリンクが存在しない");
		case NgHeight:			return _T("相対高さが範囲外");
		case NgLinkTable1:		return _T("リンクテーブルが定義外");
		case NgLinkTable2:		return _T("リンクテーブルが整備対象外");
		case NgBothRoadLink:	return _T("道路リンク同士の相対高さ");
		case NgSelhCross:		return _T("自己交差箇所に付与されている");
		case NgHeightEqual:		return _T("相対高さが同じ");
		case NgHNWalkClass:		return _T("歩行者種別が相対高さ付与対象外");
		case NgHeightWalk:		return _T("歩行者種別に対する相対高さが範囲外");
		case NgHeightRoad:		return _T("リンク内属性に対する相対高さが範囲外");
		case NoExistPointOnLink:	return _T("HEIGHT_NODEがリンク上にない");
		case NearNoramlNode:	return _T("HEIGHT_NODE付近にノードが存在する");
		case NearHeightNode:	return _T("HEIGHT_NODE付近に別のHEIGT_NODEが存在する");
		case NearSegment:		return _T("HEIGHT_NODE付近にリンクID1またはリンクID2の構成点が存在する");
		case LargeDifference:	return _T("相対高さの差が大きい");
		case NonExistHeightNode: return _T("HEIGHT_NODEがあるべき箇所に存在しない");
		case NgHeightNodeGeometry:  return _T("HEIGHT_NODEの形状が不正");
		case NgHeightNodeLinkID:  return _T("リンクIDが不正な値");

		// 11**:Walk Oneway ERROR
		case NotExistOnewayWL:	return _T("一方通行規制条件テーブルで指定した歩行者リンクが存在しない");
		case OverLapOnewayTime: return _T("一方通行同士、または一方通行と通行禁止の規制時間が重なっている");
		case NgOnewayTime:		return _T("一方通行の規制条件が不正な値");
		case SameOnewayTime:	return _T("一方通行または通行禁止で全く同じ規制時間が複数存在している");
		case NgOnewayFullYear:	return _T("規制条件の通年が1/1～12/31になっていない");
		case NgOnewayLinkID:	return _T("リンクIDが不正な値");
		case NgOnewayLinkDir:	return _T("リンク方向コードが不正な値");

		// 12**:LinkRealation ERROR
		case NotExistWalkLink:	return _T("基準リンクIDが歩行者リンクに存在しない");
		case NotExistRoadLink:	return _T("関連リンクIDが道路リンクに存在しない");
		case NgLRWalkClass:		return _T("基準リンクの歩行者種別が対象外");
		case NgLRRoadClass:		return _T("関連リンクの道路表示種別が対象外");
		case NgLRWalkFlag:		return _T("紐付対象外になるフラグがONになっている");
		case NgLinkDist:		return _T("リンク同士が離れている");
		case NgLinkAngle:		return _T("リンク同士の角度が大きい");
		case NgConnectRelLinks:	return _T("関連リンク群が接続していない");
		case NgBuddyRelation:	return _T("リリース変換で紐付られないBuddy関係");
		case NgLRRoadLinkWalkable: return _T("Buddyをもつのに歩行禁止以外");
		case NgDuplicatedRecord: return _T("リンクリレーションに重複レコードがある");

		// 13**:StationGatePoint ERROR
		case NotExistSameNameSGP:	return _T("付近に同一の駅出入口名称をもつ駅出入口ノードが存在しない");

		// 14**:PoiPointOrg ERROR
		case NotExistSameNamePOI:	return _T("付近に同一の地下街出入口名称をもつ地下街出入口ノードが存在しない");
		case NotAlivePOI        :   return _T("ノードに紐付く地下街出入口POIの削除コードが「生存」ではない");

		// 15**:RoadNode ERROR
		case NotExistSameNameRND:	return _T("付近に同一の交差点名をもつ歩行者ノードが存在しない");

		// 16**:WalkLink-UnderGround ERROR
		case PartOfUnderLinkIncludedArea: return _T("地下通路の一部が地下街からはみ出ている");
		case UndergroundAreaNoneOverLinks: return _T("地下通路が完全に地下街からはみ出ている");
		case UndergroundAreaNotExist: return _T("地下通路リンク群に重なる地下街がない");

		// 17**:Pedxing ERROR
		case NgPedxingGeometry:       return _T("横断ポイントの形状が不正");
		case NgPedxingOnScrambleArea: return _T("横断ポイントがスクランブルエリア内にある");
		case NgPedxingOnRoadLink:     return _T("横断ポイントが道路リンク上に無い");
		case NgPedxingCrossRoadLink:  return _T("横断ポイントが道路リンク交差箇所にある");
		case NgPedxingCrossWalkLink:  return _T("横断ポイントが歩行者リンク交差箇所にある");
		case NgPedxingWithWalkLink:   return _T("横断ポイントに関係ない歩行者リンクが存在する");
		case NgPedxingClass:          return _T("横断タイプコードが不正な値");
		case CrossRoadLink:           return _T("意味なしPEDXING");
		// 18**:ScrambleArea ERROR
		case NgScrambleAreaGeometry:       return _T("スクランブルエリアポリゴンの形状が不正");
		case NgScrambleAreaCrossWalkLink:  return _T("スクランブルエリア内にスクランブルフラグが付与された横断歩道が1つも無い");
		case NgScrambleAreaClass:          return _T("スクランブル種別コードが不正な値");

		default:	return _T("");
	}
}

// エラーコードとメッセージからエラーメッセージ取得
CString COutPut::GetErrEnumMsg( long errCode, const CString& info)
{
	using namespace err_code;
	using namespace sindy::schema;
	using namespace sindy::schema::walk_link;

	switch( errCode ){

	case ExistFlag:
		if(info == kAutoWalkFlag)
			return _T("オートウォークフラグ整備対象外の歩行者種別に対しフラグが整備されている");
		else if(info == kStairFlag)
			return _T("階段フラグ整備対象外の歩行者種別に対しフラグが整備されている");
		else if(info == kSlopeFlag)
			return _T("スロープフラグ整備対象外の歩行者種別に対しフラグが整備されている");
		else if(info == kEscalatorFlag)
			return _T("エスカレータフラグ整備対象外の歩行者種別に対しフラグが整備されている");
		else if(info == kArcadeFlag)
			return _T("屋根つきフラグ整備対象外の歩行者種別に対しフラグが整備されている");
		else if(info == kOpenAreaFlag)
			return _T("広場フラグ整備対象外の歩行者種別に対しフラグが整備されている");
		else if(info == kTunnelFlag)
			return _T("トンネルフラグ整備対象外の歩行者種別に対しフラグが整備されている");
		else if(info == kScrambleFlag)
			return _T("スクランブルフラグ整備対象外の歩行者種別に対しフラグが整備されている");
		else
			return _T("フラグ整備対象外の歩行者種別に対しフラグが整備されている");
	case PositionNearError:
		if(info.Find(walk_node::kTableName) >= 0)
			return _T("ポイント同士WALK_NODEの距離が近い");
		else if(info.Find(pedxing::kTableName) >= 0)
			return _T("ポイント同士PEDXINGの距離が近い");
		else if(info.Find( height_node::kTableName) >= 0)
			return _T("ポイント同士HEIGHT_NODEの距離が近い");
	default:	return _T("");
	}
}
