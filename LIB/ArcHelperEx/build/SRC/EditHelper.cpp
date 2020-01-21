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

// EditHelper.cpp: CEditHelper クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EditHelper.h"
#include "Mesh.h"
#include "TargetFeatures.h"
#include "LoadQueueInfo.h"
#include "AheDraw.h"
#include "AheNetwork.h"
#include "RuleModel.h"
#include <set>

using namespace sindy::schema;

#ifdef _DEBUG
//#define _DEBUG_FOR_2059	// [bug 2059]用
#ifdef _DEBUG_FOR_2059
#include "../../SiNDY-u/AnnoCheck/ShapeFileHelper.h"
#endif // ifdef _DEBUG_FOR_2059
#endif // ifdef _DEBUG

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR LINK_QUEUE_DIRGUIDE                        = _T("方面案内");
	LPCTSTR LINK_QUEUE_GUIDE                           = _T("強制誘導");
	LPCTSTR LINK_QUEUE_VICS                            = _T("VICS");
	LPCTSTR LINK_QUEUE_BYWAY                           = _T("抜け道");
	LPCTSTR LINK_QUEUE_TURNREG                         = _T("右左折禁止");
	LPCTSTR LINK_QUEUE_LANE                            = _T("レーン");
	LPCTSTR LINK_QUEUE_ROUTE                           = _T("アクセス／乗り継ぎ道");
	LPCTSTR LINK_QUEUE_UTURN                           = _T("Uターン");
	LPCTSTR LINK_QUEUE_INTERNAVI                       = _T("インターナビVICS");
	LPCTSTR LINK_QUEUE_IPCVICS                         = _T("iPC VICS");
	LPCTSTR LINK_QUEUE_INTERSECTION                    = _T("進入方向別交差点");
	LPCTSTR ERROR_FAILED_SPLIT                         = _T("スプリット処理失敗");
	LPCTSTR MESSAGE_FEATURE_IS_RANDOM                  = _T("引継ぎフィーチャ決定失敗：\n\n理由：%s\n\n既存IDを引き継ぐフィーチャはランダムになります。");
	LPCTSTR WARN_SPLIT                                 = _T("スプリット処理警告");
	LPCTSTR ERROR_CANNOT_CONTINUE_SPLIT                = _T("%sの作成が禁止されているため、スプリット処理を継続できません");
	LPCTSTR ERROR_LINK_NOT_FOUND                       = _T("LQRFを付与するための道路リンクが見つかりません。");
	LPCTSTR ERROR_FAIL_TO_ROAD_LINK_REPLACE            = _T("道路リンク（OBJECTID=%d）の FROM、TOの付け替えに失敗しました。処理を継続できません。");
	LPCTSTR SPLIT_ROAD_LINK                            = _T("道路リンクのスプリット処理");
	LPCTSTR ERROR_FAIL_TO_RESET_NODE                   = _T("単独ノードを作成しようとしたか、又は2次メッシュ線上に2本以上の道路リンクを接続しようとしたためにノード種別の再設定に失敗しました。処理を継続できません。");
	LPCTSTR ERROR_NOT_ADD_SHARED_SHAPE_POINT           = _T("レイヤ：%s OBJECTID=%d : 移動可能なフィーチャではないため、共有構成点を追加することができません。");
	LPCTSTR RESTRUCTURING_ATTRIBUTE_SLOPE              = _T("リンク内属性・傾斜の構成点再構築");
	LPCTSTR ERROR_END_POINT_ISNOT_CONTACT_ROAD_LINK    = _T("レイヤ：%s OBJECTID=%d : 端点が道路リンクに接していないため、処理を継続することができません。");
	LPCTSTR SPLIT_OF_ATTRIBUTE_SLOPE                   = _T("リンク内属性・傾斜のスプリット");
	LPCTSTR ERROR_NOT_FIND_ROAD_LINK_ATTRIBUTE_SLOPE   = _T("リンク内属性・傾斜と関連させる道路リンクを見つけることができません。");
	LPCTSTR SPLIT_OF_ATTRIBUTE_OR_SLOPE                = _T("リンク内属性又は傾斜のスプリット");
	LPCTSTR ERROR_CANNOT_ACROSS_3_ROAD_LINK            = _T("リンク内属性・傾斜は3本以上の道路リンク又は立体交差を跨いで移動することはできません。");
	LPCTSTR ERROR_END_POINT_NOT_MATCH_ROAD_LINK        = _T("リンク内属性・傾斜のスプリット形状の端点が道路リンクと一致していません。");
	LPCTSTR ERROR_FAIL_ADD_ROAD_LINK_ID                = _T("OBJECTIDを継承したリンク内属性及び傾斜への関連道路リンクID付与に失敗しました。");
	LPCTSTR ERROR_FAIL_TO_ADD_NEWLY_ROAD_LINK_ID       = _T("新規に作成されたリンク内属性及び傾斜への関連道路リンクID付与に失敗しました。");
	LPCTSTR ERROR_COORD_AND_FROM_NOTE_ID_NOT_MATCH     = _T("道路リンク：%d FROM_NODE_ID とノードの座標が一致しません。");
	LPCTSTR ERROR_FAIL_ROAD_LINK_SPLIT                 = _T("道路リンクスプリット処理失敗");
	LPCTSTR ERROR_COORD_AND_TO_NOTE_ID_NOT_MATCH       = _T("道路リンク：%d TO_NODE_ID とノードの座標が一致しません。");
	LPCTSTR SLOPE                                      = _T("傾斜");
	LPCTSTR ROAD_STRUCTURE_ATTRIBUTE                   = _T("リンク内属性");
	LPCTSTR ERROR_SHAPE_IS_APART_FROM_ROAD_LINK        = _T("道路リンク：%d %s：%d : %sの形状が道路リンクから離れています。");
	LPCTSTR ERROR_ROAD_EDIT_FAIL                       = _T("道路系編集処理失敗");
	LPCTSTR ERROR_FLAG_DOES_NOT_COME_WITH_ROAD_LINK    = _T("LQ（%ld）に関連する道路リンク（%ld）に関連フラグ（%s）が付いていません");
	LPCTSTR ERROR_ROAD_LINK_NOT_EXIST_OR_SEARCH_FAIL   = _T("LQ（%ld）に関連する道路リンク（%ld）が存在しないか、又は検索できていません");
	LPCTSTR ERROR_SOMETHING_WRONG_WITH_SEQUENCE_NUMBER = _T("リンク列のシーケンス番号がおかしいです : \n");
	LPCTSTR ERROR_START_POINT_AND_BEFORE_IS_NOT_SAME   = _T("現在のLQの開始点とひとつ前の終了点が異なります\n");
	LPCTSTR ERROR_NO_SHAPE_OF_ROAD_LINK                = _T("道路リンクの形状が存在しません : OID[%d]");
	LPCTSTR ERROR_TERMINATION_FLAG_NOT_EXIST           = _T("シーケンスの最後に終端フラグがありません\n");
	LPCTSTR DIVISION_POLYGON                           = _T("ポリゴンの分割");
	LPCTSTR ERROR_NO_TABLE                             = _T("%s、%sテーブルが存在しないため、処理を継続できません\n");
	LPCTSTR ROAD_LINK_SPLIT                            = _T("道路リンクのスプリット処理");
	LPCTSTR ERROR_NOT_DELETE_FOLLOWING_RULE            = _T("レイヤ : %s OBJECTID : %d : 以下のルールにより編集できません:\n\n%s");
	LPCTSTR EDITING_PROHIBITED                         = _T("編集禁止");
	LPCTSTR CONFIRM_SET_TO_WARNING_LEVEL               = _T("レイヤ : %s OBJECTID : %d : 以下のルールによりワーニングレベルに設定されています:\n\n%s\n編集しますか？");
	LPCTSTR MESSAGE_WARNING                            = _T("警告");
	LPCTSTR ERROR_SEQUENCE_NUMBER_NOT_CORRECT          = _T("%sテーブル OBJECTID=%d : シーケンス番号が正しく入っていないため、処理を続行できません");
	LPCTSTR ERROR_DIRECTION_CODE_NOT_CORRECT           = _T("%sテーブル OBJECTID=%d : 方向コードが正しく入っていないため、処理を続行できません");
	LPCTSTR ERROR_SPLIT_ROAD_LINK_ACROSS_MESH          = _T("スプリット後の道路リンクがメッシュを跨いでいるため、処理を続行できません");
	LPCTSTR ERROR_SPLIT_ROAD_LINK_SHAPE_IS_IMPROPER    = _T("スプリット後の道路リンクの形状が不正なため、リンク列の修正ができません");
	LPCTSTR ERROR_FAIL_COPY_EXTERNAL_RECORD            = _T("外部レコードのコピーに失敗しました。");
	LPCTSTR EXTERNAL_RECORD_COPY_FAILURE               = _T("外部レコードコピー失敗");
	LPCTSTR ERROR_FAIL_DELETE_EXTERNAL_RECORD          = _T("外部レコードの削除に失敗しました。");
	LPCTSTR EXTERNAL_RECORD_DELETE_FAILURE             = _T("外部レコード削除失敗");
	LPCTSTR CONFIRM_FILL_THIS_CLOSED_RING              = _T("この閉空間を穴埋めしますか？");
	LPCTSTR FILL_METHOD                                = _T("穴埋めルーチン");
	LPCTSTR CONFIRM_CHANGE_SHAPE_AND_EVADE_HEAP        = _T("同レイヤで重なっているポリゴンが存在します。\n既存のフィーチャの形状を変更して重なりを回避しますか？");
	LPCTSTR CREATING_POLYGON                           = _T("ポリゴンの作成");
	LPCTSTR CANNOT_ADD_SHARED_SHAPE_POINT_TO_MOVABLE   = _T("レイヤ：%s OBJECTID=%d : 移動可能なフィーチャではないため、共有構成点を追加することができません。");
	LPCTSTR RESTRUCTURING_OF_ATTRIBUTE_SLOPE           = _T("リンク内属性・傾斜の構成点再構築");
#else
	LPCTSTR LINK_QUEUE_DIRGUIDE                        = _T("Signpost");
	LPCTSTR LINK_QUEUE_GUIDE                           = _T("Forced guiding");
	LPCTSTR LINK_QUEUE_VICS                            = _T("VICS");
	LPCTSTR LINK_QUEUE_BYWAY                           = _T("Byway");
	LPCTSTR LINK_QUEUE_TURNREG                         = _T("Turn Regulation");
	LPCTSTR LINK_QUEUE_LANE                            = _T("Lane");
	LPCTSTR LINK_QUEUE_ROUTE                           = _T("Access/connection road");
	LPCTSTR LINK_QUEUE_UTURN                           = _T("U turn");
	LPCTSTR LINK_QUEUE_INTERNAVI                       = _T("Internavi VICS");
	LPCTSTR LINK_QUEUE_IPCVICS                         = _T("iPCVICS");
	LPCTSTR LINK_QUEUE_INTERSECTION                    = _T("Direction dependent intersection");
	LPCTSTR ERROR_FAILED_SPLIT                         = _T("Failed to invoke split.");
	LPCTSTR MESSAGE_FEATURE_IS_RANDOM                  = _T("Failed to decide the inheriting feature：\n\n Reason：%s\n\n The feature is random when have the existing ID.");
	LPCTSTR WARN_SPLIT                                 = _T("Warning to split.");
	LPCTSTR ERROR_CANNOT_CONTINUE_SPLIT                = _T("Because creating of the %s is prohibited, I cannot continue split processing.");
	LPCTSTR ERROR_LINK_NOT_FOUND                       = _T("Road link for adding LQRF can not be found.");
	LPCTSTR ERROR_FAIL_TO_ROAD_LINK_REPLACE            = _T("Failed to replace FROM and TO of the road link (OBJECTID=%d). The processing can not be continued.");
	LPCTSTR SPLIT_ROAD_LINK                            = _T("Split processing of the road link");
	LPCTSTR ERROR_FAIL_TO_RESET_NODE                   = _T("You have failed to reset the node type because you tried to either make a single node or connect two or more road links to the boundary of the secondnary mesh line. The processing can not be continued.");
	LPCTSTR ERROR_NOT_ADD_SHARED_SHAPE_POINT           = _T("Layer: %s OBJECTID=%d : You can not add a shared shape point because it is not a movable feature.");
	LPCTSTR RESTRUCTURING_ATTRIBUTE_SLOPE              = _T("Restructuring of the shape point of the road structure attribute/slope");
	LPCTSTR ERROR_END_POINT_ISNOT_CONTACT_ROAD_LINK    = _T("Layer：%s OBJECTID=%d : The processing can not be continued since the end point is not in contact with the road link.");
	LPCTSTR SPLIT_OF_ATTRIBUTE_SLOPE                   = _T("Splitting or road structure attribute/slope");
	LPCTSTR ERROR_NOT_FIND_ROAD_LINK_ATTRIBUTE_SLOPE   = _T("Unable to find the road link to be associated with road structure attribute/slope.");
	LPCTSTR SPLIT_OF_ATTRIBUTE_OR_SLOPE                = _T("Splitting of road structure attribute or slope");
	LPCTSTR ERROR_CANNOT_ACROSS_3_ROAD_LINK            = _T("Road structure attribute/slope can not be moved across three or more road links or grade separation..");
	LPCTSTR ERROR_END_POINT_NOT_MATCH_ROAD_LINK        = _T("End point of the split shape of road structure attribute/slope does not match the road link.");
	LPCTSTR ERROR_FAIL_ADD_ROAD_LINK_ID                = _T("Failed to add the associated road link ID to the road structure attribute/slope which inherited the OBJECTID");
	LPCTSTR ERROR_FAIL_TO_ADD_NEWLY_ROAD_LINK_ID       = _T("Failed to add the associated road link ID to the newly crerated road structure attribute and slope");
	LPCTSTR ERROR_COORD_AND_FROM_NOTE_ID_NOT_MATCH     = _T("Road link: %d Coordinates of the node and FROM_NODE_ID do not match.");
	LPCTSTR ERROR_FAIL_ROAD_LINK_SPLIT                 = _T("Road link split processing has failed");
	LPCTSTR ERROR_COORD_AND_TO_NOTE_ID_NOT_MATCH       = _T("Road link: %d Coordinates of the node and TO_NODE_ID do not match.");
	LPCTSTR SLOPE                                      = _T("slope");
	LPCTSTR ROAD_STRUCTURE_ATTRIBUTE                   = _T("road structure attribute");
	LPCTSTR ERROR_SHAPE_IS_APART_FROM_ROAD_LINK        = _T("road link:%d %s：%d : The shape of %s is apart from the road link.");
	LPCTSTR ERROR_ROAD_EDIT_FAIL                       = _T("Road data editing process failure");
	LPCTSTR ERROR_FLAG_DOES_NOT_COME_WITH_ROAD_LINK    = _T("Relation flag (%s) does not come with a road link (%ld) related to LQ(%ld)");
	LPCTSTR ERROR_ROAD_LINK_NOT_EXIST_OR_SEARCH_FAIL   = _T("Either a road link (%ld) related to LQ(%ld) does not exist or the search has failed.");
	LPCTSTR ERROR_SOMETHING_WRONG_WITH_SEQUENCE_NUMBER = _T("Something is wrong with the sequence number of the link queue : \n");
	LPCTSTR ERROR_START_POINT_AND_BEFORE_IS_NOT_SAME   = _T("The start point of current LQ and the end point of the one before are not the same\n");
	LPCTSTR ERROR_NO_SHAPE_OF_ROAD_LINK                = _T("There is no shape of the road link : OID[%d]");
	LPCTSTR ERROR_TERMINATION_FLAG_NOT_EXIST           = _T("The termination flag does not exist at the end of the sequence\n");
	LPCTSTR DIVISION_POLYGON                           = _T("Division polygon.");
	LPCTSTR ERROR_NO_TABLE                             = _T("Processing can not be continued because there is no %s, %s table.\n");
	LPCTSTR ROAD_LINK_SPLIT                            = _T("Split processing of the road link");
	LPCTSTR ERROR_NOT_DELETE_FOLLOWING_RULE            = _T("Layer: %s OBJECTID : %d : Cannot be deleted due to the following rule:\n\n%s");
	LPCTSTR EDITING_PROHIBITED                         = _T("Editing prohibited");
	LPCTSTR CONFIRM_SET_TO_WARNING_LEVEL               = _T("Layer: %s OBJECTID : %d : It is set to warning level by the following rule:\n\n%s\n Do you want to edit?");
	LPCTSTR MESSAGE_WARNING                            = _T("Warning");
	LPCTSTR ERROR_SEQUENCE_NUMBER_NOT_CORRECT          = _T("%sTable OBJECTID=%d: Since the sequence number is not entered correctly, the processing can not be continued");
	LPCTSTR ERROR_DIRECTION_CODE_NOT_CORRECT           = _T("%sTable OBJECTID=%d: Since the direction code is not entered correctly, the processing can not be continued");
	LPCTSTR ERROR_SPLIT_ROAD_LINK_ACROSS_MESH          = _T("Since the split road link is across the mesh, the processing can not be continued.");
	LPCTSTR ERROR_SPLIT_ROAD_LINK_SHAPE_IS_IMPROPER    = _T("Since the split road link shape is improper, the link queue can not be edited");
	LPCTSTR ERROR_FAIL_COPY_EXTERNAL_RECORD            = _T("Failed to copy the external record.");
	LPCTSTR EXTERNAL_RECORD_COPY_FAILURE               = _T("External record copy failure");
	LPCTSTR ERROR_FAIL_DELETE_EXTERNAL_RECORD          = _T("Failed to delete the external record.");
	LPCTSTR EXTERNAL_RECORD_DELETE_FAILURE             = _T("External record delete failure");
	LPCTSTR CONFIRM_FILL_THIS_CLOSED_RING              = _T("Do you fill this closed ring?");
	LPCTSTR FILL_METHOD                                = _T("Fill method.");
	LPCTSTR CONFIRM_CHANGE_SHAPE_AND_EVADE_HEAP        = _T("Polygons are overlap in same layer.\nI change the shape of the existing feature, and do you evade heap?");
	LPCTSTR CREATING_POLYGON                           = _T("Creating polygon");
	LPCTSTR CANNOT_ADD_SHARED_SHAPE_POINT_TO_MOVABLE   = _T("Layer: %s OBJECTID=%d : You can not add a shared shape point because it is not a movable feature.");
	LPCTSTR RESTRUCTURING_OF_ATTRIBUTE_SLOPE           = _T("Restructuring of the shape point of the road structure attribute/slope");
#endif // SINDY_FOR_ENGLISH
}

#ifdef _DEBUG
void TargetItems_Trace( const CTargetItems& cItems, LPCTSTR lpcszHeader )
{
	for( CTargetItems::const_iterator it = cItems.begin(); it != cItems.end(); ++it )
	{
		ATLTRACE(_T("%s : TABLE[%s] OID[%d]\n"), lpcszHeader, AheGetFeatureClassName( it->GetFeature() ), it->GetOID() );
	}
}
#else
#define TargetItems_Trace(a,b) (void(0))
#endif // ifdef _DEBUG

IGeometryPtr TargetItems_MergeGeometry( const CTargetItems& cItems, bool bUnion = true )
{
	// CFeatures::GetShape()のほぼパクリです
	// UNIONをかけると、UNION後に1Unitしか長さがない場合は消えてしまうので注意（未報告）！！
	IGeometryPtr ipRet;

	IGeometryCollectionPtr ipPointsCol, ipPolylinesCol, ipPolygonsCol;
	for( CTargetItems::const_iterator it = cItems.begin(); it != cItems.end(); ++it )
	{
		IGeometryPtr ipGeom = it->GetShapeCopy();
		if( ipGeom != NULL )
		{
			esriGeometryType emType = esriGeometryAny;
			ipGeom->get_GeometryType( &emType );
			switch( emType )
			{
				case esriGeometryBag:
				{
					IEnumGeometryPtr ipEnumGeom( ipGeom );
					ipEnumGeom->Reset();
					IGeometryPtr ipGeomPart;
					while( S_OK == ipEnumGeom->Next( &ipGeomPart ) )
					{
						esriGeometryType emTypePart = esriGeometryAny;
						ipGeomPart->get_GeometryType( &emTypePart );
						switch( emTypePart )
						{
							case esriGeometryMultipoint:
							case esriGeometryPoint:
								if( ipPointsCol == NULL )
									ipPointsCol.CreateInstance( CLSID_GeometryBag );
								ipPointsCol->AddGeometry( ipGeomPart );
								break;
							case esriGeometryPolyline:
								if( ipPolylinesCol == NULL )
									ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
								ipPolylinesCol->AddGeometry( ipGeomPart );
								break;
							case esriGeometryPolygon:
								if( ipPolygonsCol == NULL )
									ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
								ipPolygonsCol->AddGeometry( ipGeomPart );
								break;
							default:
								_ASSERTE(false);
								break;
						}
					}
					break;
				}
				case esriGeometryMultipoint:
				case esriGeometryPoint:
					if( ipPointsCol == NULL )
						ipPointsCol.CreateInstance( CLSID_GeometryBag );
					ipPointsCol->AddGeometry( ipGeom );
					break;
				case esriGeometryPolyline:
					if( ipPolylinesCol == NULL )
						ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
					ipPolylinesCol->AddGeometry( ipGeom );
					break;
				case esriGeometryPolygon:
					if( ipPolygonsCol == NULL )
						ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
					ipPolygonsCol->AddGeometry( ipGeom );
					break;
				default:
					_ASSERTE(false);
					break;
			}
		}
	}

	if( bUnion )
	{
		// それぞれをUnionする
		ITopologicalOperatorPtr ipPointsTopo, ipPolylinesTopo, ipPolygonsTopo;
		if( ipPointsCol )
		{
			ipPointsTopo.CreateInstance( CLSID_Multipoint );
			((IGeometryPtr)ipPointsTopo)->putref_SpatialReference( AheGetSpatialReference( ipPointsCol ) ); // 空間参照付与
			ipPointsTopo->ConstructUnion( (IEnumGeometryPtr)ipPointsCol );
			ipPointsCol = ipPointsTopo;
		}
		if( ipPolylinesCol )
		{
			ipPolylinesTopo.CreateInstance( CLSID_Polyline );
			((IGeometryPtr)ipPolylinesTopo)->putref_SpatialReference( AheGetSpatialReference( ipPolylinesCol ) ); // 空間参照付与
			ipPolylinesTopo->ConstructUnion( (IEnumGeometryPtr)ipPolylinesCol );
			ipPolylinesCol = ipPolylinesTopo;
		}
		if( ipPolygonsCol )
		{
			ipPolygonsTopo.CreateInstance( CLSID_Polygon );
			((IGeometryPtr)ipPolygonsTopo)->putref_SpatialReference( AheGetSpatialReference( ipPolygonsCol ) ); // 空間参照付与
			ipPolygonsTopo->ConstructUnion( (IEnumGeometryPtr)ipPolygonsCol );
			ipPolygonsCol = ipPolygonsTopo;
		}
	}

	// どれか一つしかなければそれを返り値とする
	bool bPoints = false, bPolylines = false, bPolygons = false;
	if( ( bPoints    = ( ipPointsCol != NULL && ipPolylinesCol == NULL && ipPolygonsCol == NULL ) ) ||
		( bPolylines = ( ipPointsCol == NULL && ipPolylinesCol != NULL && ipPolygonsCol == NULL ) ) ||
		( bPolygons  = ( ipPointsCol == NULL && ipPolylinesCol == NULL && ipPolygonsCol != NULL ) ) )
	{
		if( bPoints )
			ipRet = ipPointsCol;
		else if( bPolylines )
			ipRet = ipPolylinesCol;
		else if( bPolygons )
			ipRet = ipPolygonsCol;
	}
	else if( ipPointsCol != NULL || ipPolylinesCol != NULL || ipPolygonsCol != NULL )
	{
		IGeometryCollectionPtr ipGeomCol( CLSID_GeometryBag );
		if( ipPointsCol != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPointsCol );
		if( ipPolylinesCol != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolylinesCol );
		if( ipPolygonsCol != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolygonsCol );
		ipRet = ipGeomCol;
	}
	return ipRet;
}

bool operator < ( const LQDEF& c1, const LQDEF& c2 )
{
	return c1.seq < c2.seq;
}

bool operator == ( const LQDEF& c1, const LQDEF& c2 )
{
	return ( c1.oid == c2.oid && c1.kind == c2.kind );
}

CString GetTraceLQDEFString( const LQDEF& c )
{
	CString strName;
	switch( c.kind )
	{
		case sindyeRoadQueueDirguide  : strName = LINK_QUEUE_DIRGUIDE; break;
		case sindyeRoadQueueGuide     : strName = LINK_QUEUE_GUIDE; break;
		case sindyeRoadQueueVics      : strName = LINK_QUEUE_VICS;     break;
		case sindyeRoadQueueByway     : strName = LINK_QUEUE_BYWAY;   break;
		case sindyeRoadQueueTurnreg   : strName = LINK_QUEUE_TURNREG; break;
		case sindyeRoadQueueLane      : strName = LINK_QUEUE_LANE;       break;
		case sindyeRoadQueueRoute     : strName = LINK_QUEUE_ROUTE; break;
		case sindyeRoadQueueUTurn     : strName = LINK_QUEUE_UTURN; break;
		case sindyeRoadQueueInternavi : strName = LINK_QUEUE_INTERNAVI; break;
		case sindyeRoadQueueIPCVics   : strName = LINK_QUEUE_IPCVICS; break;
		case sindyeRoadQueueIntersection   : strName = LINK_QUEUE_INTERSECTION; break;
		default : strName = _T("UNKNOWN"); break;
	}
	CString strTrace;
	strTrace.Format(_T("LQDEF : OID[%12d] INFID[%12d] LINKID[%12d] SEQ[%3d] LAST[%5s] DIR[%s] KIND[%s]"), c.oid, c.infid, c.linkid, c.seq, ( c.last ) ? _T("TRUE") : _T("FALSE"), ( 1 == c.dir ) ? _T("正") : _T("逆"), strName );

	return strTrace;
}

CString GetTraceLQDEFString( const LQDEF& c, std::map<long, ROADDEF>& mapRoad )
{
	CString strName;
	switch( c.kind )
	{
		case sindyeRoadQueueDirguide  : strName = LINK_QUEUE_DIRGUIDE; break;
		case sindyeRoadQueueGuide     : strName = LINK_QUEUE_GUIDE; break;
		case sindyeRoadQueueVics      : strName = LINK_QUEUE_VICS;     break;
		case sindyeRoadQueueByway     : strName = LINK_QUEUE_BYWAY;   break;
		case sindyeRoadQueueTurnreg   : strName = LINK_QUEUE_TURNREG; break;
		case sindyeRoadQueueLane      : strName = LINK_QUEUE_LANE;       break;
		case sindyeRoadQueueRoute     : strName = LINK_QUEUE_ROUTE; break;
		case sindyeRoadQueueUTurn     : strName = LINK_QUEUE_UTURN; break;
		case sindyeRoadQueueInternavi : strName = LINK_QUEUE_INTERNAVI; break;
		case sindyeRoadQueueIPCVics   : strName = LINK_QUEUE_IPCVICS; break;
		case sindyeRoadQueueIntersection   : strName = LINK_QUEUE_INTERSECTION; break;
		default : strName = _T("UNKNOWN"); break;
	}

	double dx1 = -1.0, dy1 = -1.0, dx2 = -1.0, dy2 = -1.0;
	IPointPtr ipFrom, ipTo;
	std::map<long,ROADDEF>::const_iterator it = mapRoad.find( c.linkid );
	if( it != mapRoad.end() )
	{
		IPolylinePtr ipRoadGeom( it->second.shape );
		if( ipRoadGeom )
		{
			ipRoadGeom->get_FromPoint( &ipFrom );
			ipRoadGeom->get_ToPoint( &ipTo );
			if( 2 == c.dir )
			{
				IPointPtr ipTmp( ipFrom );
				ipFrom = ipTo;
				ipTo = ipTmp;
			}
			ipFrom->QueryCoords( &dx1, &dy1 );
			ipTo->QueryCoords( &dx2, &dy2 );
		}
	}

	CString strTrace;
	CString str2;
	str2.Format(_T("%.15lf, %.15lf"), dx1, dy1 );

	CString str3;
	str3.Format(_T("%.15lf, %.15lf"), dx1, dy1 );

	strTrace.Format(_T("LQDEF : OID[%12d] INFID[%12d] LINKID[%12d] SEQ[%3d] LAST[%5s] DIR[%s] KIND[%s] (%.15lf, %.15lf) - (%.15lf, %.15lf)"), c.oid, c.infid, c.linkid, c.seq, ( c.last ) ? _T("TRUE") : _T("FALSE"), ( 1 == c.dir ) ? _T("正") : _T("逆"), strName, dx1, dy1, dx2, dy2 );

	return strTrace;
}

void TraceLQDEF( const LQDEF& c )
{
	ATLTRACE(_T("%s\n"), GetTraceLQDEFString( c ) );
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CEditHelper::CEditHelper()
{

}

CEditHelper::~CEditHelper()
{

}

// 道路による道路のスプリット
BOOL CEditHelper::SplitRoadByRoadAndMesh( CTargetItems& cTargetFeatures, CTargetItems& cNewFeatures, BOOL bSplitAll, IPointPtr ipTakeOverPoint/* = NULL*/, CTargetItems* pReferenceNodes/* = NULL*/, IPointCollection* ipSplitPoints/* = NULL*/ )
{
	CErrObjContainer errs;
	return SplitRoadByRoadAndMesh( cTargetFeatures, cNewFeatures, bSplitAll, errs, ipTakeOverPoint, pReferenceNodes, ipSplitPoints );
}
BOOL CEditHelper::SplitRoadByRoadAndMesh( CTargetItems& cTargetFeatures, CTargetItems& cNewFeatures, BOOL bSplitAll, CErrObjContainer& errs, IPointPtr ipTakeOverPoint/* = NULL*/, CTargetItems* pReferenceNodes/* = NULL*/, IPointCollection* ipSplitPoints/* = NULL*/ )
{
#ifdef DEBUG
        DWORD DWSTART = GetTickCount();
#endif

	BOOL bRet = TRUE;	// 返値

	TargetItems_Trace( cTargetFeatures, _T("SplitRoadByRoadAndMesh : 開始（cTargetFeatures）") );

	// 渡されたフィーチャ群には既に履歴がついている（はず）なので、履歴付与除外対象とする
	CTargetItems cNoSetModifyDataFeatures = cTargetFeatures;

	// 編集対象群の中から道路、傾斜、リンク内属性とノードを選択
	CTargetItems cRoads, cNodes, cRoadsAndSegAndGrads;
	CString strSplitLayers;
	for( CTargetItems::const_iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
	{
		if( AheIsRelateWithNetwork( it->GetFeature() ) ) // ネットワーク関係フィーチャから情報を取得（bug 5486）
		{
			strSplitLayers = AheGetNetworkSplitTarget( it->GetFeatureClassName() );
			break;
		}
	}
	// 傾斜、リンク内属性の単独移動でもスプリット処理ができるように
	cTargetFeatures.GetFeatures( cRoadsAndSegAndGrads, strSplitLayers );
	cTargetFeatures.GetNetworkLinkFeatures( cRoads );
	cTargetFeatures.GetNetworkNodeFeatures( cNodes );

	IGeometryPtr ipSearchGeometry;
	CTargetItems cDstRoadsAndSegAndGrads;
	IPointCollectionPtr ipCrossPointCol;
	// [bug 2378] ノードのみ渡されたときの処理が昔のままだったので道路やリンク内属性の
	//            選択処理を通常のものと同じになるように修正。ただし、分割点は決まって
	//            いるので、選択処理後に無駄に追加されたポイントを戻してやる
	BOOL bIsNodeOnly = FALSE;
	IPointCollectionPtr ipCrossPointColNodeOnly;
	if ( cNodes.size() == 1 && cRoadsAndSegAndGrads.size() <= 0 )
	{
		bIsNodeOnly = TRUE;
		ipSearchGeometry = cNodes.begin()->GetShapeCopy();
		SelectByShapeFromLayers( ipSearchGeometry, cRoadsAndSegAndGrads, strSplitLayers, esriSpatialRelIntersects, NULL, NULL, TRUE );
		ipCrossPointColNodeOnly.CreateInstance( CLSID_Multipoint );
		((IGeometryPtr)ipCrossPointColNodeOnly)->putref_SpatialReference( AheGetSpatialReference( ipSearchGeometry ) ); // 空間参照付与
		ipCrossPointColNodeOnly->AddPoint( (IPointPtr)ipSearchGeometry );
		cTargetFeatures.GetNetworkLinkFeatures( cRoads );
	}
	if ( cRoadsAndSegAndGrads.size() > 0 )
	{
//		// 道路全体のエンベロープを取得 <- 遅い！！
//		IEnvelopePtr ipRoadEnv;
//		cRoads.GetEnvelope( &ipRoadEnv );
		// 道路全体の Union を取得
		ipSearchGeometry = cRoadsAndSegAndGrads.GetUnion();
		if( ipSearchGeometry == NULL )
		{
			ATLASSERT( ipSearchGeometry != NULL );
			return FALSE;
		}
		((ITopologicalOperatorPtr)ipSearchGeometry)->Simplify();

/*#ifdef DEBUG
		// 検索前のユニオン形状をハイライト
		AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipSearchGeometry, RGB( 20, 50, 100 ), 2, esriSLSSolid, TRUE, FALSE );
		ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : 検索前のユニオン形状をハイライト（藍色）\n") );
		IGraphicsContainerPtr ipContainer;
		m_cArcHelper.GetActiveView( FALSE )->get_GraphicsContainer( &ipContainer );
		AheAddLineElement( ipContainer, ipSearchGeometry, RGB(20,50,100), 2, esriSLSSolid );
#endif*/
		//SetDomain();

		// 検索ポリラインの端点とTOUCHな関係のフィーチャが選択できない場合有り！！ <- 未報告
		// しょうがないので、ほんの少しバッファを作って検索する。
		// これにより、余計なものが選択される恐れ有り。
		IGeometryPtr ipSearchGeometry2;
		IGeoDatasetPtr ipClass( cRoadsAndSegAndGrads.GetNetworkLinkFeatureClass() );
		DOUBLE dBuffer = 0.0000005;

		if( ipClass != NULL )
		{
			ISpatialReferencePtr ipRoadRef;
			ipClass->get_SpatialReference( &ipRoadRef );
			//dBuffer = AheGetXYDomain1UnitLength( ipRoadRef ) * (DOUBLE)4.0;
			dBuffer = AheGetXYDomain1UnitLength( ipRoadRef ) * (DOUBLE)10.0;
			ATLTRACE(_T("CEditHelper::SplitRoadByRoadAndMesh : 道路選択バッファ：%.20lf\n"), dBuffer );
		}
			
		((ITopologicalOperatorPtr)ipSearchGeometry)->Buffer( dBuffer, &ipSearchGeometry2 );
		//AheGeometry2Polygon( ipSearchGeometry, 0.0000005, &ipSearchGeometry2 );
#ifdef DEBUG
		// 道路検索エリアハイライト
		//AheAddPolygonElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipSearchGeometry2, RGB(100, 0, 100), esriSFSSolid );
#endif

		// まずは道路だけ検索
		CTargetItems cSelectedRoads;
		SelectByShapeFromFeatureClass( ipSearchGeometry2, cSelectedRoads, ipClass );
		// cSelectRoads の中から cRoads にあるものを除外
		cSelectedRoads -= cRoads;

#ifdef DEBUG
		for( CTargetItems::iterator itTmp = cSelectedRoads.begin(); itTmp != cSelectedRoads.end(); itTmp++ )
			ATLTRACE(_T("CEditHelper::SplitRoadByRoadAndMesh : 検索できたフィーチャ2 %s OID=%d\n"), itTmp->GetFeatureClassName(), itTmp->GetOID() );
#endif

		// 交差ポイント取得（ノード作成候補点）
		// [bug 1889] 渡されたフィーチャに道路リンクが存在しなければやらない。やると、構成点付与や重複チェックができなくなる
		CTargetItems cDeleteSelectedRoads;
		if( ! cRoads.empty() )
		{
			IPointCollectionPtr ipCrossPointColTmp = GetCrossOrTouchPoints( cSelectedRoads, cRoads, ( bSplitAll ? 2 : 1 ), &cDeleteSelectedRoads );

			// 分割点が渡されている場合はそれのみ使用する（bug 4882）
			if( ipSplitPoints )
			{
				ITopologicalOperatorPtr ipTopo( ipSplitPoints );
				if( ipTopo != NULL )
				{
					IGeometryPtr ipIntersectGeom;
					ipTopo->Intersect( (IGeometryPtr)ipCrossPointColTmp, esriGeometry0Dimension, &ipIntersectGeom );
					ipCrossPointCol = ipIntersectGeom;
				}
			}
			else
				ipCrossPointCol = ipCrossPointColTmp;

			// 関係なかったものを cSelectedRoads から除外（スピードアップになる？）
#ifdef DEBUG
			for( CTargetItems::iterator itDel = cDeleteSelectedRoads.begin(); itDel != cDeleteSelectedRoads.end(); itDel++ ) 
				ATLTRACE(_T("CEditHelper::SplitRoadByRoadAndMesh() : 交差しなかったので除外 %s OBJECTID=%d\n"), itDel->GetFeatureClassName(), itDel->GetOID() );
#endif
			cSelectedRoads -= cDeleteSelectedRoads;
		}
/*#ifdef DEBUG
		//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipCrossPointCol, RGB(100, 0, 100), 8, esriSMSCircle );
#endif*/
		// 次に、渡された道路＋検索できた道路でリンク内属性・傾斜を検索する（ROAD_IDの更新のため）
		ipSearchGeometry = ipSearchGeometry2 = NULL;
		CTargetItems cTargetAndSelectedRoad = cRoads;
		for( CTargetItems::iterator it = cSelectedRoads.begin(); it != cSelectedRoads.end(); it++ ) cTargetAndSelectedRoad.push_back( *it );
		ipSearchGeometry = cTargetAndSelectedRoad.GetUnion();
		if( ipSearchGeometry != NULL )
		{
			((ITopologicalOperatorPtr)ipSearchGeometry)->Simplify();
			((ITopologicalOperatorPtr)ipSearchGeometry)->Buffer( dBuffer, &ipSearchGeometry2 );
			CString strSegAttrAndGradLayers;
			strSegAttrAndGradLayers.Format(_T("|%s|%s|"), SEGMENTATTR_TABLE_NAME, GRAD_TABLE_NAME );
			CTargetItems cSegAttrAndGrads2;
			SelectByShapeFromLayers( ipSearchGeometry2, cSegAttrAndGrads2, strSegAttrAndGradLayers, esriSpatialRelIntersects, NULL, NULL, TRUE );
			// 余計なリンク内属性・傾斜が選択されているので、渡された道路＋検索できた道路に端点が接しているかどうかチェック
			// ↑だと Bug 1612 のようなことになるので、その端点同士が接しているかどうかもチェックする必要アリ
			// ↑でも Bug 1754 のようなケースは無理
			// そもそも、既存のリンク内属性や傾斜は個々の道路リンク内に収まってるはずなので、両端は一つの道路リンクで完結するはず
			CTargetItems::iterator itSegAttrAndGrads2 = cSegAttrAndGrads2.begin();
			while( itSegAttrAndGrads2 != cSegAttrAndGrads2.end() )
			{
				IPointPtr ipSegAttrAndGradsFromPoint = itSegAttrAndGrads2->GetFromPoint(), ipSegAttrAndGradsToPoint = itSegAttrAndGrads2->GetToPoint();
				IGeometryPtr ipFromRoadGeom, ipToRoadGeom;

				BOOL bFind = FALSE;
				for( CTargetItems::iterator itTargetAndSelectedRoad = cTargetAndSelectedRoad.begin(); itTargetAndSelectedRoad != cTargetAndSelectedRoad.end(); itTargetAndSelectedRoad++ )
				{
					// [bug 2924] 道路リンクを単独編集する際、リンク内属性や傾斜の端点部分を移動したときはリンク内属性・傾斜が入ってこない
					// なので、ここでちゃんと関連するリンク内属性と傾斜は編集対象にするべき
					if( itSegAttrAndGrads2->GetAttribute( _T("LINK_ID") ) == CComVariant( itTargetAndSelectedRoad->GetOID() ) )
					{ 
						itSegAttrAndGrads2++;
						bFind = TRUE;
						break;
					}

					if( ( AhePointIsOnGeometry( itTargetAndSelectedRoad->GetShape(), ipSegAttrAndGradsFromPoint, itTargetAndSelectedRoad->GetSpatialReference() ) == TRUE &&
						  AhePointIsOnGeometry( itTargetAndSelectedRoad->GetShape(), ipSegAttrAndGradsToPoint,   itTargetAndSelectedRoad->GetSpatialReference() ) == TRUE ) ||
						  AheIsEqual( itTargetAndSelectedRoad->GetShape(), itSegAttrAndGrads2->GetShape(), itTargetAndSelectedRoad->GetSpatialReference() ) == TRUE )
					{
						itSegAttrAndGrads2++;
						bFind = TRUE;
						break;
					}
				}
				if( bFind == FALSE ) itSegAttrAndGrads2 = cSegAttrAndGrads2.erase( itSegAttrAndGrads2 );
			}

			for( CTargetItems::iterator it = cSegAttrAndGrads2.begin(); it != cSegAttrAndGrads2.end(); it++ ) cDstRoadsAndSegAndGrads.push_back( *it );
			for( CTargetItems::iterator it = cSelectedRoads.begin(); it != cSelectedRoads.end(); it++ ) cDstRoadsAndSegAndGrads.push_back( *it );
		}
	} 
	// ノードのみのときは、ここでノードのポイントのみにする
	if( bIsNodeOnly == TRUE ) ipCrossPointCol = ipCrossPointColNodeOnly;	// 入れ替え

	// SketchTool でセグメントオーバーラップで作成した場合になぜかスルーしてしまう（交差ポイントが取得できない）ので、ここで
	// セグメントオーバーラップチェックをする
	CString strErrMsg;
	CTargetItems cTestTargets = cTargetFeatures;
	cDstRoadsAndSegAndGrads.GetFeatures( cTestTargets, strSplitLayers, TRUE );
#ifdef _DEBUG
	cTestTargets.Trace( _T("IsSafeGeometries前") );
#endif // ifdef _DEBUG
	cTestTargets.IsSafeGeometries( errs, TRUE, GetRule() );
	if( errs.GetMaxLogLevel() == sindyeLogLevel_ERR )
	{
		AheMessageBox( errs.ToString(), ERROR_FAILED_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
		return FALSE;
	}

	// メッシュでの交差ポイント取得
	for( CTargetItems::iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); it++ )
	{
		// 道路リンクかどうかをチェック
		// 2次メッシュで切るフィーチャは道路リンクのみにしないと形状に不整合が出る（Bug 1637）
		// 自動運転マップのリンクはメッシュ境界でスプリットしない
		if( AheIsNetworkLink( it->GetFeatureClassName() ) 
			&& (sindyeNetworkLane != AheGetNetWorkType(it->GetFeatureClassName()))
			&& (sindyeNetworkUpDown != AheGetNetWorkType(it->GetFeatureClassName()))
			&& (sindyeNetworkCompart != AheGetNetWorkType(it->GetFeatureClassName()))
			&& (sindyeNetworkBorder != AheGetNetWorkType(it->GetFeatureClassName())))
		{
			CMesh mesh;
			mesh.SetSpatialReference( it->GetSpatialReference() );
			IPointCollectionPtr ipPointsGeometry = mesh.GetIntersectPointsByShape( it->GetShape() );
			LONG lPointCount = -1;
			if( ipPointsGeometry != NULL )
			{
				ipPointsGeometry->get_PointCount( &lPointCount );
				for( LONG i = 0; i < lPointCount; i++ )
				{
					IPointPtr ipPoint;
					ipPointsGeometry->get_Point( i, &ipPoint );
					if( ipPoint != NULL )
					{
						if( ipCrossPointCol == NULL )
						{
							ipCrossPointCol.CreateInstance( CLSID_Multipoint );
							((IGeometryPtr)ipCrossPointCol)->putref_SpatialReference( AheGetSpatialReference( ipPoint ) ); // 空間参照付与
						}

						ipCrossPointCol->AddPoint( ipPoint );
					}
				}
			}
		}
	}
#ifdef DEBUG
	// 分割点（Simplify前）ハイライト
	AheDrawPoints( GetFocusMapScreenDisplay(), ipCrossPointCol, FALSE, 8, 20, 50, 200, esriSMSCircle, FALSE, TRUE );
	ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : スプリッタハイライト（？色）\n") );
//	AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipCrossPointCol, RGB(20,50,200), 8, esriSMSCircle );
#endif

	AheRemoveAdjoiningPoint( ipCrossPointCol );

	// リンク内属性及び傾斜の道路リンクとの形状の整合性を取るので、必ず通す
	// 分割対象に cDstRoadsAndSegAndGrads を含める
	for( CTargetItems::iterator itTmp = cDstRoadsAndSegAndGrads.begin(); itTmp != cDstRoadsAndSegAndGrads.end(); itTmp++ )
	{
#ifdef DEBUG
		ATLTRACE(_T("CEditHelper::SplitRoadByRoadAndMesh : 検索できたフィーチャ %s OID=%d\n"), itTmp->GetFeatureClassName(), itTmp->GetOID() );
#endif
		cTargetFeatures.push_back( *itTmp );
	}
#ifdef DEBUG
    ATLTRACE (_T("●●● CEditHelper::SplitRoadByRoadAndMesh スプリット前 %lf ●●●\n"), (GetTickCount() - DWSTART)/1000.0);
#endif
#ifdef DEBUG
	// スプリッタハイライト
	//IElementPtr ipElement( AheAddLineElement( ipContainer, ipSearchGeom, RGB(0,255,0), 2, esriSLSSolid ) );
	AheDrawPoints( GetFocusMapScreenDisplay(), ipCrossPointCol, FALSE, 8, 200, 50, 20, esriSMSCircle, FALSE, TRUE );
	// 分割点ハイライト
	ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : スプリッタハイライト（レンガ色）\n") );
	//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipCrossPointCol, RGB(200,50,20), 8, esriSMSCircle );
#endif

	bRet = SplitFeaturesByGeometry( cTargetFeatures, strSplitLayers, ipCrossPointCol, cNoSetModifyDataFeatures, cNewFeatures, ipTakeOverPoint, 2, pReferenceNodes );

#ifdef DEBUG
    ATLTRACE (_T("●●● CEditHelper::SplitRoadByRoadAndMesh %lf ●●●\n"), (GetTickCount() - DWSTART)/1000.0);
#endif

	return bRet;
}

BOOL CEditHelper::SplitFeaturesByGeometry( CTargetItems& cTargetFeatures, LPCTSTR lpcszSplitLayers, IGeometryPtr ipGeom, CTargetItems& cNoSetModifyDataFeatures, CTargetItems& cNewFeatures,/* CTargetItems& cModifyFeatures,*/ IPointPtr ipTakeOverPoint/* = NULL*/, INT iSplitMode/* = 2*//*, BOOL bIsSplitByMesh*//* = FALSE*//*, BOOL bSearchNodes*//* = TRUE*/, CTargetItems* pReferenceNodes/* = NULL*/ )
{
	using namespace sindy::schema;
#ifdef _DEBUG
	TargetItems_Trace( cTargetFeatures, _T("cTargetFeatures") );
	for( CTargetItems::iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
		AheTraceGeometry( it->GetShape(), _T("CEditHelper::SplitFeaturesByGeometry() START") );
	TargetItems_Trace( cNoSetModifyDataFeatures, _T("cNoSetModifyDataFeatures") );
	TargetItems_Trace( cNewFeatures, _T("cNewFeatures") );
#endif // ifdef _DEBUG

	CTargetItems cTakeOveredFeatures;                    //!< 分割された結果元のIDを引き継いだフィーチャを格納
	CTargetItems cModifyNodeKindFeatures;                //!< ノード種別を再設定したもので、且つ cNewFeatures にも cTargetFeatures にも存在しないものが入る

	std::list<ROWDEF> listLQRows;	// スプリットの際に検索したLQレコード（最終チェック用）

	if( ipGeom )
	{
		// 対象フィーチャ群を分割する
		if(FALSE==GetSplitResult(lpcszSplitLayers, ipGeom, cNoSetModifyDataFeatures,
			cModifyNodeKindFeatures, ipTakeOverPoint, iSplitMode, cTakeOveredFeatures,
			pReferenceNodes, cNewFeatures, cTargetFeatures, listLQRows))
			return FALSE;
	}

	// リンク内属性及び傾斜を内包する道路リンクを探して、関連IDフィールドに道路リンクのOBJECTIDをセット
	if(FALSE==ResetCommonFeatures(cNewFeatures, cTargetFeatures, cNoSetModifyDataFeatures, cTakeOveredFeatures))
		return FALSE;

	// cNewFeatures を cTargetFeatures にあわせこみ
	//cTargetFeatures += cNewFeatures;
	// cModifyNodeKindFeatures を cTargetFeatures にあわせこみ
	cTargetFeatures += cModifyNodeKindFeatures;

/*#ifdef DEBUG
	for( CTargetItems::const_iterator itAll = cTargetFeatures.begin(); itAll != cTargetFeatures.end(); ++itAll )
		ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : CTargetItemsの中身確認 : LAYER=%s OBJECTID=%d\n"), itAll->GetFeatureClassName(), itAll->GetOID() );
#endif*/
	// 
#ifdef _DEBUG_FOR_2059
	{
		CTargetItems cLastRoads;
		cTargetFeatures.GetRoadFeatures( cLastRoads );
		cNewFeatures.GetRoadFeatures( cLastRoads, TRUE );
		// SnapTo...前の道路形状のトレースを確認
		for( CTargetItems::iterator itLastRoads = cLastRoads.begin(); itLastRoads != cLastRoads.end(); itLastRoads++ )
		{
			CString strMsg;
			strMsg.Format(_T("SnapTo...前の道路形状[%d]"), itLastRoads->GetOID() );
			AheTraceGeometry( itLastRoads->GetShape(), strMsg );
		}
	}
#endif // ifdef _DEBUG_FOR_2059
	// [bug 2059] SnapToSpatialReferenceをするとなぜか道路リンクの方だけ座標がずれる？？
	// いや、正確にはKnownSimpleがVARIANT_TRUEの場合にSnapToSpatialRefenreceをするとずれる（＝正しい値になる？）のか？
	// しかし、SnapTo..をはずしてもCTargetItems::Simplify()を通すとやっぱりずれた。
	// 内部ではSimplifyNetworkをやっている。もしかしてこれも原因か？
	// とりあえず、どちらも外す事でエラーが出なくなったのでそれでOKとするが、
	// これはいずれちゃんと調査しないといけない。
	// test/bug2059/bug2059-2.mxdにテスト環境を作っておいたので、それでROAD_LINK[6,7]を跨ぐように道路リンクを
	// 作って分割すると、下のコードを生かした状態では2回に1回の割合でエラーがでた。
	// あとで検証すること by furukawa。
	//cTargetFeatures.SnapToSpatialReference();
	//cTargetFeatures.Simplify();
#ifdef _DEBUG_FOR_2059
	{
		CTargetItems cLastRoads;
		cTargetFeatures.GetRoadFeatures( cLastRoads );
		cNewFeatures.GetRoadFeatures( cLastRoads, TRUE );
		// SnapTo...後の道路形状のトレースを確認
		for( CTargetItems::iterator itLastRoads = cLastRoads.begin(); itLastRoads != cLastRoads.end(); itLastRoads++ )
		{
			CString strMsg;
			strMsg.Format(_T("SnapTo...後の道路形状[%d]"), itLastRoads->GetOID() );
			AheTraceGeometry( itLastRoads->GetShape(), strMsg );
		}
	}
#endif // ifdef _DEBUG_FOR_2059

	// 分割された結果できた形状に問題がないかチェック
	if(FALSE==CheckNWStructure(cNewFeatures, cTargetFeatures, listLQRows))
		return FALSE;

	return TRUE;
}

_ISetPtr CEditHelper::SplitGeometryByGeometry( IGeometryPtr ipBaseGeometry, IGeometryPtr ipSplitterGeometry, INT iSplitMode/* = 2*/, BOOL bIsRoadFeature/* = FALSE */, IGeometryPtr ipSplittedPoints/* = NULL */, BOOL bTest/* = FALSE*/ )
{
	_ISetPtr ipSplittedGeometrySet;	// 分割後の形状保持用

	// ジオメトリタイプ取得
	esriGeometryType enumBaseGeomType, enumSplitterGeomType;
	ipBaseGeometry->get_GeometryType( &enumBaseGeomType );
	ipSplitterGeometry->get_GeometryType( &enumSplitterGeomType );

	switch( enumBaseGeomType )
	{
		case esriGeometryPolyline:
			switch( enumSplitterGeomType )
			{
				case esriGeometryPoint:
				case esriGeometryMultipoint:
					ipSplittedGeometrySet = SplitPolylineByPoints( ipBaseGeometry, ipSplitterGeometry, ( bIsRoadFeature == TRUE ) ? ipSplittedPoints : NULL );
					break;
				case esriGeometryPolyline:
					ipSplittedGeometrySet = SplitPolylineByPoints( ipBaseGeometry, AheGetCrossOrTouchPoints( ipBaseGeometry, ipSplitterGeometry, iSplitMode ), ( bIsRoadFeature == TRUE ) ? ipSplittedPoints : NULL );
					break;
				case esriGeometryPolygon:
					ipSplittedGeometrySet = SplitPolylineByPoints( ipBaseGeometry, AheGetCrossOrTouchPoints( ipBaseGeometry, ipSplitterGeometry, iSplitMode ), ( bIsRoadFeature == TRUE ) ? ipSplittedPoints : NULL );
					break;
				default:
					break;
			}
			break;
		case esriGeometryPolygon:
			switch( enumSplitterGeomType )
			{
				case esriGeometryPolyline:
					ipSplittedGeometrySet = SplitPolygonByPolyline( ipBaseGeometry, ipSplitterGeometry, bTest );
					break;
/*				case esriGeometryPolygon:
					ipSplittedGeometrySet = SplitPolygonByPolygon( ipBaseGeometry, ipSplitterGeometry, iSplitMode );
					break;*/
				default:
					break;
			}
			break;
		default:
			break;
	}

	// 一つも入っていないなら NULL を返す
	LONG lGeomCount = -1;
	if( ipSplittedGeometrySet != NULL )
	{
		ipSplittedGeometrySet->get_Count( &lGeomCount );
		if( lGeomCount < 1 ) return NULL;
	}

	return ipSplittedGeometrySet;
}
_ISetPtr CEditHelper::SplitPolylineByPoints( IPolylinePtr ipPolyline, IGeometryPtr ipSplitterPoints, IPointCollectionPtr ipSplittedPoints/* = NULL*/ )
{
	if( ipPolyline == NULL || ipSplitterPoints == NULL ) return NULL;

	esriGeometryType enumSplitterPointsType;
	IPointCollectionPtr ipPointCol( ipSplitterPoints );

	// ipSplitterPoints のジオメトリタイプがポイントの場合、 IPointCollection には変換できない
	ipSplitterPoints->get_GeometryType( &enumSplitterPointsType );
	if( ( ! ipPointCol ) && enumSplitterPointsType == esriGeometryPoint )
	{
		ipPointCol.CreateInstance( CLSID_Multipoint );
		((IGeometryPtr)ipPointCol)->putref_SpatialReference( AheGetSpatialReference( ipSplitterPoints ) );
		ipPointCol->AddPoint( (IPointPtr)ipSplitterPoints );
	}
	IZAwarePtr ipZAware(ipPolyline);
	VARIANT_BOOL bZAware = VARIANT_FALSE;
	if(SUCCEEDED(ipZAware->get_ZAware(&bZAware)) && bZAware){
		long pointCount = -1;
		ipPointCol->get_PointCount(&pointCount);
		for(long i=0; i<pointCount; ++i){
			IPointPtr ipOrgPoint;
			ipPointCol->get_Point(i, &ipOrgPoint);
			if(!ipOrgPoint)
				continue;

			IZAwarePtr(ipOrgPoint)->put_ZAware(VARIANT_TRUE);

			double distAlong = 0, distFrom = 0;
			VARIANT_BOOL rightSide = VARIANT_FALSE;
			IPointPtr ipSplitPoint(CLSID_Point);
			ipSplitPoint->putref_SpatialReference(AheGetSpatialReference(ipPolyline));
			if(SUCCEEDED(IPolycurve2Ptr(ipPolyline)->QueryPointAndDistance(esriNoExtension, ipOrgPoint, VARIANT_TRUE, ipSplitPoint, &distAlong, &distFrom, &rightSide))){
				double zVal = 0;
				if(SUCCEEDED(ipSplitPoint->get_Z(&zVal))){
					ipOrgPoint->put_Z(zVal);
				}
			}
		}
	}

	// ipSplitterPoints のポイント数が0の場合はリターン
	LONG lCount = 0;
	ipPointCol->get_PointCount( &lCount );
	if( lCount < 1 ) return NULL;

	// 元の形状の地理参照取得
	ISpatialReferencePtr ipSpRel;
	ipPolyline->get_SpatialReference( &ipSpRel );

	// 元の形状コピー
	IPolylinePtr ipCopyPolyline( AheGetClone( ipPolyline ) );
//	AheForceSimplify( ipCopyPolyline ); <- 余計なことはやらない 2003/03/14 furukawa

	// 元の形状の FROM、TO ポイントを記憶（スプリット後の向きをスプリット前と一致させるため）
	IPointPtr ipFromPoint, ipToPoint;
	ipCopyPolyline->get_FromPoint( &ipFromPoint );
	ipCopyPolyline->get_ToPoint( &ipToPoint );
//	AheSnapToSpatialReference( ipFromPoint, ipSpRel );
//	AheSnapToSpatialReference( ipToPoint,   ipSpRel );

// 一回ずつスプリットしていくと、取得した交点とラインがずれてしまうのでだめ
//	// スプリット
//	BOOL bIsSplitted = FALSE;
//	for( LONG i = 0; i < lCount; i++ )
//	{
//		IPointPtr ipPoint;
//
//		ipPointCol->get_Point( i, &ipPoint );
//		if( ipPoint == NULL )
//		{
//			ATLASSERT( ipPoint != NULL );
//			continue;
//		}
////		AheSnapToSpatialReference( ipPoint, ipSpRel );
//
//		// ポイントがポリラインに含まれていなければ、continue
//		if( AheIsWithin( ipPoint, ipCopyPolyline/*, ipSpRel*/ ) == FALSE ) continue;
//
//		VARIANT_BOOL vaIsSplitted = VARIANT_FALSE;
//		LONG lNewPartIndex = -1, lNewSegmentIndex = -1, lBeforeGeomCount = -1;
//		((IGeometryCollectionPtr)ipCopyPolyline)->get_GeometryCount( &lBeforeGeomCount );
//		ipCopyPolyline->SplitAtPoint( ipPoint, VARIANT_FALSE, VARIANT_TRUE, &vaIsSplitted, &lNewPartIndex, &lNewSegmentIndex );
//		ATLTRACE(_T("CEditHelper::SplitPolylineByPoints() : 構成点追加： SPLIT=%s NewPartIndex=%d NewSegmentIndex=%d\n"), (vaIsSplitted == VARIANT_TRUE ? _T("TRUE") : _T("FALSE")), lNewPartIndex, lNewSegmentIndex );
//
//		if( vaIsSplitted == VARIANT_TRUE ) 
//		{
///*			// 構成点数0のパートができる場合があるので、ここでSimplify()してパート数をチェック
//			AheForceSimplify( ipCopyPolyline );
//
//			LONG lAfterGeomCount = -1;
//			((IGeometryCollectionPtr)ipCopyPolyline)->get_GeometryCount( &lAfterGeomCount );
//			if( lBeforeGeomCount < lAfterGeomCount )
//			{*/
//				if( ipSplittedPoints != NULL )
//					ipSplittedPoints->AddPoint( ipPoint );	// 分割点を記憶
//
//				bIsSplitted = TRUE;
///*			}
//			else
//				ATLTRACE(_T("CEditHelper::SplitPolylineByPoints() : 構成点追加結果、微小パートができたようです。微小パートは Simplify() により削除されました。\n") );*/
//		}
//	}

	BOOL bIsSplitted = FALSE;

	IEnumVertexPtr ipEnumVertex;

	ipPointCol->get_EnumVertices( &ipEnumVertex );

	if ( ipEnumVertex ) {
		ipEnumVertex->Reset();

		IPolycurve2Ptr ipPolycurve2 = ipCopyPolyline;

		if ( ipPolycurve2 ) {
			// 空間参照最小距離取得
			DOUBLE dTorelance = AheGetXYDomain1UnitLength( ipSpRel );

			IEnumSplitPointPtr ipEnumSplit;

			// スプリット
			try {
				// ○-----○------○-------○　というNWがあり、中2つのノードでお互いにスナップして
				// マージするように移動すると、ここで落ちる（bug 2413）
				// ipPolycurve2が一点しか存在しない場合に落ちるようだが、事前に形状を判定して云々
				// なんて面倒なのでtry...catchで補足するのみとする。
				HRESULT hr = ipPolycurve2->SplitAtPoints( ipEnumVertex, VARIANT_FALSE, VARIANT_TRUE, dTorelance, &ipEnumSplit );
			}
			catch( ... )
			{
				AheTraceGeometry( ipPolycurve2, _T("スプリット対象") );
				AheTraceGeometry( (IGeometryPtr)ipPointCol, _T("スプリットポイント") );
			}

			if ( ipEnumSplit ) {
				ipEnumSplit->Reset();

				VARIANT_BOOL	vbIsSplit = VARIANT_FALSE;

				ipEnumSplit->get_SplitHappened( &vbIsSplit );

				if ( vbIsSplit ) {
					// スプリット点
					IPointPtr ipPoint;
					long outPartIndex=0, vertexIndex=0;

					long lCnt = 0;
					while ( SUCCEEDED( ipEnumSplit->Next( &ipPoint, &outPartIndex, &vertexIndex ) ) ) {
						if ( !ipPoint )
							break;

						if( ipSplittedPoints != NULL )
							ipSplittedPoints->AddPoint( ipPoint );	// 分割点を記憶

						++lCnt;
					}

					if ( lCnt > 0 )
						bIsSplitted = TRUE;
				}
			}
		}
	}

	// スプリットできなかったらそのままリターン
	if( bIsSplitted == FALSE ) return NULL;

	AheSetKnownSimple( ipCopyPolyline );

	// IPath -> IPolyline 変換
	IGeometryCollectionPtr ipGeomCol( ipCopyPolyline );
	LONG lPartCount = -1;
	ipGeomCol->get_GeometryCount( &lPartCount );

	if( lPartCount <= 1 ) return NULL;

	_ISetPtr ipSet( CLSID_Set );
	for( LONG j = 0; j < lPartCount; j++ )
	{
		IGeometryPtr ipPartGeom;
		IGeometryCollectionPtr ipNewGeomCol( CLSID_Polyline );
		IPolylinePtr ipPoly( ipNewGeomCol );
		ipPoly->putref_SpatialReference( ipSpRel ); // 空間参照付与
		IZAwarePtr(ipPoly)->put_ZAware(bZAware);

		ipGeomCol->get_Geometry( j, &ipPartGeom );
		IPathPtr ipPath( ipPartGeom );
		ATLASSERT( ipPath != NULL );

		ipNewGeomCol->AddGeometry( ipPath );
		ATLASSERT( ipPoly != NULL );

//		ipPoly->SnapToSpatialReference(); <- 余計なことはやらない 2003/03/14 furukawa
		AheSetKnownSimple( ipPoly );

		ipSet->Add( ipPoly );
	}
#if 0	// いらないんじゃないの？？いままであさーとでたことないし。
	// スプリット後のポリラインの向きをスプリット前と一致させる
	BOOL bFinished = FALSE;
	IPointPtr ipTestFromPoint( ipFromPoint );
	IPointPtr ipOldFromPoint, ipOldToPoint;
	while( bFinished == FALSE )
	{
		IUnknownPtr ipUnk;
		ipSet->Reset();
		while( SUCCEEDED( ipSet->Next( &ipUnk ) ) ) 
		{
			IPolylinePtr ipPolyline( ipUnk );
			if( ipPolyline == NULL ) break;

			// 既に調整済みのものは省く
			if( AheIsFromPoint( ipOldFromPoint, ipPolyline/*, ipSpRel*/ ) == TRUE && AheIsToPoint( ipOldToPoint, ipPolyline/*, ipSpRel*/ ) == TRUE ) continue;

			BOOL bFind = FALSE;
			if( AheIsFromPoint( ipTestFromPoint, ipPolyline/*, ipSpRel*/ ) == TRUE )
			{
				bFind = TRUE;
			}
			else if( AheIsToPoint( ipTestFromPoint, ipPolyline/*, ipSpRel*/ ) == TRUE )
			{
				bFind = TRUE;
				ATLASSERT(FALSE);
				ipPolyline->ReverseOrientation();	// 逆転してるので元に戻す
			}

			if( bFind == TRUE )
			{
				ipOldFromPoint = ipTestFromPoint;
				ipPolyline->get_ToPoint( &ipTestFromPoint );
				ipOldToPoint   = ipTestFromPoint;
			}

			// ipToPoit と ipOldToPoint が一致したら終了
			if( AheIsEqual( ipToPoint, ipOldToPoint/*, ipSpRel*/ ) == TRUE ) bFinished = TRUE;
		}
	}
#endif
	return ipSet;
}

_ISetPtr CEditHelper::SplitPolygonByPolyline( IPolygonPtr ipPolygon, IPolylinePtr ipSplitterPolyline, BOOL bTest/* = FALSE*/ )
{
	ITopologicalOperator4Ptr ipTopo( AheGetClone( ipPolygon ) );
	if( ! ( ipTopo && ipSplitterPolyline ) )
		return NULL;

	// 地理参照あわせ
	ISpatialReferencePtr ipSpRef;
	ipPolygon->get_SpatialReference( &ipSpRef );
	ipSplitterPolyline->putref_SpatialReference( ipSpRef );
	ipSplitterPolyline->SnapToSpatialReference();
	// 分割
	IGeometryCollectionPtr col;
	if( FAILED( ipTopo->Cut2( ipSplitterPolyline, &col ) ) )
	{
			if( !bTest )
				AheMessageBox( GetHWND(), AheGetErrorMessage(), DIVISION_POLYGON, MB_OK|MB_ICONSTOP );
			else
				ATLTRACE(_T("CEditHelper::SplitPolygonByPolyline() : %s\n"), AheGetErrorMessage() );

			return NULL;
	}
	// 格納
	_ISetPtr ipRetSet; // 戻り値
	long count = 0;
	col->get_GeometryCount( &count );
	for( long i = 0; i < count; ++i )
	{
		IGeometryPtr geom;
		col->get_Geometry( i, &geom );
		VARIANT_BOOL isEmpty = VARIANT_TRUE;
		if( geom )
			geom->get_IsEmpty( &isEmpty );
		if( !isEmpty )
		{
			if( !ipRetSet )
				ipRetSet.CreateInstance( CLSID_Set );
			ipRetSet->Add( geom );
		}
	}
	
	return ipRetSet;
}

#ifdef _USE_SETLQATTRIBUTE
BOOL CEditHelper::SetLQAttribute( LONG lFeatureID, IGeometryPtr ipBeforeSplitGeom, CTargetItems& cNewRoads, std::list<LQRFINFO>& listLQRFInfo, std::list<ROWDEF>& listLQRows )
{
	if( lFeatureID <= 0 || cNewRoads.size() <= 0 ) return FALSE;

	USES_CONVERSION;

	// 元の道路フィーチャ取得
	CTargetItems::iterator itOldRoad;
	for ( CTargetItems::iterator itRoads = cNewRoads.begin(); itRoads != cNewRoads.end(); itRoads++ ) 
	{
		if ( itRoads->GetOID() == lFeatureID ) 
		{
			itOldRoad = itRoads;
			break;
		}
	}

	// 修正前の形状からFROM、TOを取得
//	IGeometryPtr ipBeforeSplitGeom;
	IPointPtr ipBeforeSplitFromPoint, ipBeforeSplitToPoint;
//	((IFeatureChangesPtr)itOldRoad->GetFeature())->get_OriginalShape( &ipBeforeSplitGeom );
	IPolylinePtr ipBeforeSplitPolyline( ipBeforeSplitGeom );
	if( ipBeforeSplitPolyline == NULL )
	{
		// 新規作成フィーチャの場合は NULL になるので、その場合は TRUE で返さないといけない
		// e.x. SketchTool で自動交差で道路リンクに両端をスナップして道路リンクを作成した場合
		return TRUE;
	}
	ipBeforeSplitPolyline->get_FromPoint( &ipBeforeSplitFromPoint );
	ipBeforeSplitPolyline->get_ToPoint( &ipBeforeSplitToPoint );
	
	// 該当する LQ を検索
	for ( INT i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ ) 
	{
		CString strLQTableName        = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQTableName        );
		CString strLQRFFieldName      = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName  );
		CString strInfoTableName      = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoInfoTableName      );
		LONG    lDrawMode             = m_cLoadQueueInfo.GetLoadQueueDrawMode( (sindyeRoadQueueType)i );

		// フラグが付いているかどうかチェック
		CComVariant vaLQRF = AheGetAttribute( itOldRoad->GetFeature(), strLQRFFieldName );
		if ( vaLQRF.vt != VT_I4 || vaLQRF.lVal == 0 ) continue;

		// リンク列のテーブル取得（道路が存在するワークスペースから取らないとダメ）
		ITablePtr ipLQTable( AheOpenSameOwnerTable( (ITablePtr)itOldRoad->GetFeatureClass(), strLQTableName ) );
		ITablePtr ipInfoTable( AheOpenSameOwnerTable( (ITablePtr)itOldRoad->GetFeatureClass(), strInfoTableName ) );

		// テーブルが存在しない場合は処理を中断
		if( ipLQTable == NULL || ipInfoTable == NULL )
		{
			// [bug 3586] AcceptNoLinkQueueTableのレジストリの値を一回だけ調べる
			static bool bCheckFlag = false;
			static bool bAcceptNoLinkQueueTable = false;
			if( ! bCheckFlag )
			{
				CRegKey key;
				if( ERROR_SUCCESS == key.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
				{
					DWORD dwAcceptNoLinkQueueTable = 0;
					if( ERROR_SUCCESS == key.QueryValue( dwAcceptNoLinkQueueTable, _T("AcceptNoLinkQueueTable") ) )
						bAcceptNoLinkQueueTable = ( 1 == dwAcceptNoLinkQueueTable ) ? true : false;
				}
				bCheckFlag = true;
			}

			// [bug 3586] AcceptNoLinkQueueTableが1なら処理を継続する（隠しオプション、PGDB用）
			if( ! bAcceptNoLinkQueueTable )
			{
				ATLASSERT( FALSE );
				CString strMsg;
				strMsg.Format(ERROR_NO_TABLE, strInfoTableName, strLQTableName );
				AheMessageBox( strMsg, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
				return FALSE;
			}
			else
				continue;
		}

		std::list<ROWDEF> /*listLQRows, */listRoads;
		std::list<LONG> listInfoIDs;

		// 検索するための道路リンクをリストに追加
		ROWDEF def;
		def.flg = lDrawMode;
		def.obj = itOldRoad->GetFeature();
		def.oid = itOldRoad->GetOID();
		def.shp = itOldRoad->GetShape();
		listRoads.push_back( def );

		// ルール
		//ISiNDYRuleObjPtr ipRule( CLSID_SiNDYRuleObj );

		// LQを検索
		AheSelectLQRowByRoadIDs( listRoads, ipLQTable, strLQRFFieldName, lDrawMode, listLQRows, listInfoIDs );

		// 関連する Info 毎に修正
		for( std::list<LONG>::iterator itInfoIDs = listInfoIDs.begin(); itInfoIDs != listInfoIDs.end(); itInfoIDs++ )
		{
			// [bug 891] LQ には履歴は書かず、全て INF に書くように仕様変更
			_IRowPtr ipInfoRow;
			ipInfoTable->GetRow( *itInfoIDs, &ipInfoRow );
			// [bug 1986] 本当に修正される場合のみ履歴をつけること
/*			if( AheIsEditableFeature( ipInfoRow, sindyeEditAttribute ) == FALSE ) return FALSE;	// 編集可能じゃないなら抜ける
			AheSetModifyData( ipInfoRow, sindyUpdateTypeProperty );	// 必ず編集される？ので、ここで履歴を書く
			ipInfoRow->Store();*/
			BOOL bModifyDataSetted = FALSE;	// 既に履歴がつけられたかどうか

			// INFが移動可能かどうかを調べておく
			sindyeEditableType editableType = sindyeEditableEditable;
			CComBSTR bstrErrMsg;
			if( GetRule() != NULL )
				GetRule()->IsEditableFeature( ipInfoRow, NULL, sindyeEditMove, &editableType, &bstrErrMsg );

			// [bug 3185] 道路リンクが移動可能でもリンク列が移動可能ではない場合は道路リンクのスプリットを不可とする
			//            ここに入ってくるということは、道路リンクのスプリット処理がなされたということなので、ここで
			//            移動可能でなければキャンセルとする
			//            本来は道路リンクのスプリット処理の段階でリンク列をチェックすればよいのだが、現在のアルゴリ
			//            ズム上スプリット処理時点ではリンク列はまだ取得されていないので、ここでチェックする
			//            SiNDYLibではちゃんとやろうね。
			if( sindyeEditableEditable != editableType )
			{
				CString strMessage;
				switch( editableType )
				{
					case sindyeEditableNoEditable:
						strMessage.Format(ERROR_NOT_DELETE_FOLLOWING_RULE, strInfoTableName, *itInfoIDs, OLE2T(bstrErrMsg) );
						::AheMessageBox( strMessage, EDITING_PROHIBITED, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
						return FALSE;
						break;
					case sindyeEditableWarnning:
						strMessage.Format(CONFIRM_SET_TO_WARNING_LEVEL, strInfoTableName, *itInfoIDs, OLE2T(bstrErrMsg) );
						if( ::AheMessageBox( strMessage, MESSAGE_WARNING, MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH ) == IDNO )
							return FALSE;
						break;
				}
			}

			// lFeatureID に該当する LQ を探す
			std::list<ROWDEF> listTargetLQRows;
			LONG lTargetSeq = -1, lSeq = -1;
//			listLQRows.sort();	// シーケンス番号順にソートしないとちゃんとできない？
			for( std::list<ROWDEF>::iterator itLQRows = listLQRows.begin(); itLQRows != listLQRows.end(); itLQRows++ )
			{
				// 関連する INFO の ID を保持
				CComVariant vaInfoID = AheGetAttribute( itLQRows->obj, INF_ID_FIELD );
				if( vaInfoID.vt == VT_I4 && vaInfoID.lVal == *itInfoIDs )
				{
					// 関連する道路リンクの ID を保持
					CComVariant vaRoadID = AheGetAttribute( itLQRows->obj, LINK_ID_FIELD );
					if( vaRoadID.vt == VT_I4 && vaRoadID.lVal == lFeatureID )
					{
						// LASTLINK_F を見る
						CComVariant vaLastLinkF = AheGetAttribute( itLQRows->obj, LASTLINK_F_FIELD );
						// 現在のシーケンス番号を取得
						CComVariant vaSeq = AheGetAttribute( itLQRows->obj, SEQUENCE_FIELD );
						if( vaSeq.vt == VT_I4 )
							lTargetSeq = lSeq = vaSeq.lVal;
						else
						{
							// シーケンス番号が正しく入っていない（そんなデータはないのが前提）
							CString strMsg;
							strMsg.Format(ERROR_SEQUENCE_NUMBER_NOT_CORRECT, strLQTableName, itLQRows->oid );
							AheMessageBox( strMsg, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
							return FALSE;
						}
						// LINKDIR_C を見て、FROM、TOのどちらから見ればよいか決定する
						IPointPtr ipCheckPoint;
						BOOL bReverse;
						CComVariant vaLinkDIR = AheGetAttribute( itLQRows->obj, LINKDIR_C_FIELD );
						if( vaLinkDIR.vt == VT_I4 && vaLinkDIR.lVal == 1 )
						{
							// 順方向
							bReverse = FALSE;
							ipCheckPoint = ipBeforeSplitFromPoint;
						}
						else if( vaLinkDIR.vt == VT_I4 && vaLinkDIR.lVal == 2 )
						{
							// 逆方向
							bReverse = TRUE;
							ipCheckPoint = ipBeforeSplitToPoint;
						}
						else
						{
							// 方向コードが正しく入っていない（そんなデータはないのが前提）
							CString strMsg;
							strMsg.Format(ERROR_DIRECTION_CODE_NOT_CORRECT, strLQTableName, itLQRows->oid );
							AheMessageBox( strMsg, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
							return FALSE;
						}
						// 分割された道路リンクの分に関してリンク列作成＆修正
						BOOL bFinished = FALSE;					// これ以上処理をさせないためのフラグ（リンク列最終リンクを分断された場合用）
						CTargetItems cRoadItems = cNewRoads;
						while( cRoadItems.size() != 0 )
						{
							BOOL bErase = FALSE;	// // 無限ループ防止用
							CTargetItems::iterator itRoad = cRoadItems.begin();
							while( itRoad != cRoadItems.end() ) 
							{
								BOOL bFind = FALSE;
								if( bReverse == FALSE )
								{
									if( itRoad->IsFromPoint( ipCheckPoint ) == TRUE )
									{
										bFind = TRUE;
										ipCheckPoint = itRoad->GetToPoint();
									}
								} else {
									if( itRoad->IsToPoint( ipCheckPoint ) == TRUE )
									{
										bFind = TRUE;
										ipCheckPoint = itRoad->GetFromPoint();
									}
								}
								if( bFind == TRUE )
								{
									// 既存のLQRFInfoを見つける
									std::list<LQRFINFO>::iterator itLQRFInfo = listLQRFInfo.begin();
									while( itLQRFInfo != listLQRFInfo.end() && itLQRFInfo->oid != itRoad->GetOID() )
										++itLQRFInfo;

									if( itLQRFInfo == listLQRFInfo.end() )
									{
										// なければ新規追加
										LQRFINFO obj;
										listLQRFInfo.push_front( obj );
										itLQRFInfo = listLQRFInfo.begin();
										itLQRFInfo->oid = itRoad->GetOID();
									}

//									ATLTRACE(_T("SetLQAttribute : Find : %d\n"), itRoad->GetOID() );
									// VICS ではない且つインターナビVICS ではない且つ SEQUENCE = 1 である場合、この道路リンクに関連するLQは作成しない・あるいは存在するなら振替をする（リンク列開始リンクを分断された場合）
									// [bug 8968] アクセス道もVICS同様とする。
									// bFinished == TRUE の時はこの道路リンクの処理をキャンセルする必要アリ（リンク列最終リンクを分断された場合）
									if( ( m_cLoadQueueInfo.IsShortenerRoadQueue( (sindyeRoadQueueType)i ) && lTargetSeq == 1 && cRoadItems.size() > 1 ) || bFinished == TRUE )
									{
//										ATLTRACE(_T("---------------------    %d\n"), itRoad->GetOID() );
										// 他に関連しているリンク列がないなら道路リンクの LQRF_* を 0 にする
										BOOL bFindOtherLQ = FALSE;
										for( std::list<ROWDEF>::iterator itLQRowsTmp = listLQRows.begin(); itLQRowsTmp != listLQRows.end(); itLQRowsTmp++ )
										{
											if( itLQRowsTmp == itLQRows ) continue;

											if( itRoad->GetOID() == AheGetAttribute( itLQRowsTmp->obj, LINK_ID_FIELD ).lVal )
											{
												bFindOtherLQ = TRUE;
												break;
											}
										}
										if( bFindOtherLQ == FALSE )
										{
//											ATLTRACE(_T("CEditHelper::SetLQAttribute() : テーブル[ROAD_LINK] OID[%d] %s関連フラグを0に修正\n"), itRoad->GetOID(), strLQRFFieldName );
//											AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 0L );	// 道路リンクの LQRF_* を 0 にする（履歴は誰かがつけれくれる）
											if( itLQRFInfo->LQRF.end() == itLQRFInfo->LQRF.find( (sindyeRoadQueueType)i ) )
											{
												itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = false;	// まだ存在しない場合のみfalseをつける
											}
										}
										itRoad = cRoadItems.erase( itRoad );
										bErase = TRUE;
										continue;
									}

									// 存在するメッシュコード取得
									CMesh mesh;
									LONG lMeshCode = mesh.GetExistMeshCode( itRoad->GetShape() );
									if( lMeshCode < 0 )
									{
										AheMessageBox( ERROR_SPLIT_ROAD_LINK_ACROSS_MESH, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
										return FALSE;
									}
									_IRowPtr ipTargetRow;
									bool bCreated = false;	// LQが新規に作られたかどうか
									// VICS（インターナビ含む）であるか、又は LASTLINK_F がついていない場合
									// [bug 8968] アクセス道もVICS同様とする。
									if( !m_cLoadQueueInfo.IsShortenerRoadQueue( (sindyeRoadQueueType)i ) || ( vaLastLinkF.vt == VT_I4 && vaLastLinkF.lVal != 1L ) )
									{
										// 既存の道路リンクなら、既に LQ が存在するのでそれを使用
										if( itRoad->GetOID() == lFeatureID )
										{
											ipTargetRow = itLQRows->obj;
											//AheSetModifyData( ipTargetRow, sindyUpdateTypeProperty );			// 変更履歴
//											AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 1L );			// 道路リンクにリンク列関連フラグを振る
//											ATLTRACE(_T("CEditHelper::SetLQAttribute() : テーブル[ROAD_LINK] OID[%d] %s関連フラグを1に修正\n"), itRoad->GetOID(), strLQRFFieldName );
											itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = true;
										}
										// 既存の道路リンクではないなら LQ を新規に作成
										else if( m_cLoadQueueInfo.IsShortenerRoadQueue( (sindyeRoadQueueType)i ) && lTargetSeq == 1 )
										{
											ipTargetRow = itLQRows->obj;
//											AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 1L );			// 道路リンクにリンク列関連フラグを振る
//											ATLTRACE(_T("CEditHelper::SetLQAttribute() : テーブル[ROAD_LINK] OID[%d] %s関連フラグを1に修正\n"), itRoad->GetOID(), strLQRFFieldName );
											itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = true;

										}
										else {
											bCreated = true;
											ipTargetRow = AheCreateRow( ipLQTable );
											//AheSetModifyData( ipTargetRow, sindyUpdateTypeCreate );				// 変更履歴
//											AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 1L );		// 道路リンクにリンク列関連フラグを振る
//											ATLTRACE(_T("CEditHelper::SetLQAttribute() : テーブル[ROAD_LINK] OID[%d] %s関連フラグを1に修正\n"), itRoad->GetOID(), strLQRFFieldName );
											itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = true;
										}
									}
									// VICS 以外で LASTLINK_F がついている場合は既存の LQ を使用、且つこれで終了させる必要がある
									else {
										ipTargetRow = itLQRows->obj;
										//AheSetModifyData( ipTargetRow, sindyUpdateTypeProperty );				// 変更履歴
//										AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 1L );			// 道路リンクにリンク列関連フラグを振る
//										ATLTRACE(_T("CEditHelper::SetLQAttribute() : テーブル[ROAD_LINK] OID[%d] 関連フラグを1に修正\n"), itRoad->GetOID() );
										itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = true;
										bFinished = TRUE;
									}

									if( ipTargetRow != NULL )
									{
										BOOL bEditField = FALSE;
										// 先に変更があるかどうかを調べる
										// 方向コード
										CComVariant vaOldValue = AheGetAttribute( ipTargetRow, LINKDIR_C_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != vaLinkDIR.lVal ) bEditField = TRUE;
										// LINK_ID
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, LINK_ID_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != itRoad->GetOID() ) bEditField = TRUE;
										// SEQUENCE
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, SEQUENCE_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != lSeq ) bEditField = TRUE;
										// MESHCODE
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, MESHCODE_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != lMeshCode ) bEditField = TRUE;
										// 関連する INFO の ID フィールド
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, INF_ID_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != vaInfoID.lVal ) bEditField = TRUE;
										// LASTLINK_F
										LONG lNewLastLinkFlag = ( ( vaLastLinkF.vt == VT_I4 && vaLastLinkF.lVal == 1 && ( cRoadItems.size() == 1 || bFinished == TRUE ) ) ? 1 : 0 );
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, LASTLINK_F_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != lNewLastLinkFlag ) bEditField = TRUE;
										
										// [bug 1986] 変更がある場合はINFが「移動可能」かどうかを調べてから（LQの属性変更＝INFの形状変更）
										if( bEditField == TRUE )
										{
											// 既に調査済みのルールを調べる
											CString strMessage;
											switch( editableType )
											{
												case sindyeEditableNoEditable:
													strMessage.Format(ERROR_NOT_DELETE_FOLLOWING_RULE, strInfoTableName, *itInfoIDs, OLE2T(bstrErrMsg) );
													::AheMessageBox( strMessage, EDITING_PROHIBITED, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
													return FALSE;
													break;
												case sindyeEditableWarnning:
													strMessage.Format(CONFIRM_SET_TO_WARNING_LEVEL, strInfoTableName, *itInfoIDs, OLE2T(bstrErrMsg) );
													if( ::AheMessageBox( strMessage, MESSAGE_WARNING, MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH ) == IDNO )
														return FALSE;
													break;
											}
											ATLTRACE(_T("CEditHelper::SetLQAttribute() : テーブル[%s] OID[%d] 修正\n"), strLQTableName, AheGetOID( ipTargetRow ) );
											AheSetAttribute( ipTargetRow, LINKDIR_C_FIELD, vaLinkDIR );			// 方向コード
											AheSetAttribute( ipTargetRow, LINK_ID_FIELD, CComVariant(itRoad->GetOID()) );	// LINK_ID フィールド書き換え
											AheSetAttribute( ipTargetRow, SEQUENCE_FIELD, CComVariant(lSeq) );				// SEQUENCE フィールド書き換え
											AheSetAttribute( ipTargetRow, MESHCODE_FIELD, CComVariant(lMeshCode) );			// MESHCODE フィールド書き換え
											AheSetAttribute( ipTargetRow, INF_ID_FIELD, vaInfoID );							// 関連する INFO の ID フィールド書き換え
											AheSetAttribute( ipTargetRow, LASTLINK_F_FIELD, CComVariant(lNewLastLinkFlag) );	// LASTLINK_F 書き換え
											ipTargetRow->Store();		// ここで保存しても良い？

											// LQに変更があった場合はINFに形状変更として履歴をつける
											if( bModifyDataSetted == FALSE )
											{
												ATLTRACE(_T("CEditHelper::SetLQAttribute() : テーブル[%s] OID[%d] 形状変更の履歴追加\n"), strInfoTableName, *itInfoIDs );
												AheSetModifyData( ipInfoRow, sindyUpdateTypeShape );
												ipInfoRow->Store();
												bModifyDataSetted = TRUE;	// 何度もつけることがないようにフラグを立てる
											}
											// LQが新規に作られた場合はlistLQRowsに追加しておく
											if( bCreated )
											{
												ROWDEF def;
												def.direction = vaLinkDIR.lVal;
												def.flg = lDrawMode;
												def.obj = ipTargetRow;
												long lOID = -1; ipTargetRow->get_OID( &lOID ); def.oid = lOID;
												def.sequence = lSeq;
												def.shp = itRoad->GetShapeCopy();
												listLQRows.push_back( def );
											}
										}
										lSeq++;
									}
									itRoad = cRoadItems.erase( itRoad );
									bErase = TRUE;	// 無限ループ防止用
								}
								else
									itRoad++;
							}
							// スプリット後の道路リンクの FROM、TOポイントがスプリット前の道路リンクの FROM、TO と一致しない
							if( itRoad == cRoadItems.end() && bErase == FALSE )
							{
								ATLASSERT( FALSE );
								AheMessageBox(ERROR_SPLIT_ROAD_LINK_SHAPE_IS_IMPROPER, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
								return FALSE;
							}
						}
					}
					// リンク列の編集対象ではないものは、シーケンス番号振りなおし対象とする
					else
						listTargetLQRows.push_back( *itLQRows );
				}
			}
			// 分割された道路リンクに関連するリンク列より後ろのシーケンスを持つリンク列に関してシーケンス番号振りなおし
			for( std::list<ROWDEF>::const_iterator itLQRows = listTargetLQRows.begin(); itLQRows != listTargetLQRows.end(); itLQRows++ )
			{
				CComVariant vaSeq = AheGetAttribute( itLQRows->obj, SEQUENCE_FIELD );
				if( vaSeq.vt == VT_I4 && vaSeq.lVal > lTargetSeq )
				{
					AheSetAttribute( itLQRows->obj, SEQUENCE_FIELD, CComVariant(vaSeq.lVal + ( (lSeq-1L) - lTargetSeq )) );
					if( AheRowIsChanged( itLQRows->obj ) == TRUE )
					{
//						AheSetModifyData( itLQRows->obj, sindyUpdateTypeProperty );			// 変更履歴
						itLQRows->obj->Store();	// ここで保存しても良い？
					}
				}
			}
		}
	}

	return TRUE;
}
#else
// 分割された道路リンクのための新しいLQを作成する
BOOL CEditHelper::CreateLQForSplittedRoad( CTargetItem& cRoad, CTargetItems& cNewRoads, std::list<ROWDEF>& listLQRows )
{
	bool bRet = TRUE;	// 返り値

	// cRoadに関連するLQを取得
	if( ) // VICS系、または抜け道、又はシーケンスが1以外且つ最終フラグがfalseか？
	{
		// LQを作成して新しい道路リンクと関連付ける
	}

	return bRet;
}

#endif // ifdef _USE_SETLQATTRIBUTE

// シーケンスの整合性を修復する
BOOL CEditHelper::FixLQSequence( std::list<LQDEF>& listLQ, std::map<long, ROADDEF>& mapRoad )
{
	// 関連道路リンクのFROM、TOをみて正しい順番にソート
	// 修正途中でエラーで帰る場合があるため、そのときに表示されるダイアログ
	// のトレースはSetLQAttributeでの処理結果ではなくFixLQSequenceでの修復
	// 途中のものであることに注意！！
	BOOL bRet = false;	// 返り値

	bool bUniqued = true, bSorted = true;
	std::list<LQDEF>::iterator it;

#ifdef _DEBUG
	{
		for( std::list<LQDEF>::const_iterator it = listLQ.begin(), itEnd = listLQ.end(); it != itEnd; ++it )
			ATLTRACE(_T("修復前：%s\n"), GetTraceLQDEFString( *it, mapRoad ) );
		ATLTRACE(_T("\n"));
	}
#endif // ifdef _DEBUG

	// まったく同じLQがlistLQに登録されている場合があるので、その場合はIRow::Deleteせずにリストから削除
	// ここのループはIRow::Deleteのループと一緒にしてはいけない！！
	for( it = listLQ.begin(); it != listLQ.end(); ++it )
	{
		std::list<LQDEF>::iterator it2 = it; ++it2;
		while( it2 != listLQ.end() )
		{
			// まったく同じLQがlistLQに登録されている場合があるので、その場合はIRow::Deleteせずにリストから削除
			if( it->oid == it2->oid )
			{
				ATLTRACE(_T("WARNING : 同一LQ除外 : %s\n"), GetTraceLQDEFString(*it2) );
				it2 = listLQ.erase( it2 );
				if( listLQ.end() == it2 )
					break;
			}
			else
				++it2;
		}
	}

	// SetLQAttributeで同じ道路リンクに複数のLQが作成される場合場ある（一度に2回以上分割される場合）
	// 今更SetLQAttributeに手を入れるのもまんどくさいので重複するものはここで削除してしまう
	for( it = listLQ.begin(); it != listLQ.end(); ++it )
	{
		std::list<LQDEF>::iterator it2 = it; ++it2;
		while( it2 != listLQ.end() )
		{
			// linkidで比較
			if( it->linkid == it2->linkid )
			{
				if( SUCCEEDED( it2->obj->Delete() ) )
				{
					ATLTRACE(_T("WARNING : 重複LQ削除 : %s\n"), GetTraceLQDEFString(*it2) );
					it2 = listLQ.erase( it2 );
					if( listLQ.end() == it2 )
						break;
				}
				else {
					_ASSERTE(false);	// 削除失敗
					return FALSE;
				}
			}
			else
				++it2;
		}
	}

FIX_LQ_SEQ:
	long lSeq = 1;
	for( it = listLQ.begin(); it != listLQ.end(); ++it, ++lSeq )
	{
		// itが最後なら終わり
		if( (ULONG)lSeq == listLQ.size() ) break;

		// FROM、TOを方向を考慮して取得
		IPointPtr ipFrom, ipTo;
		std::map<long, ROADDEF>::const_iterator itRoad = mapRoad.find( it->linkid );
		_ASSERTE( itRoad != mapRoad.end() );
		if( itRoad != mapRoad.end() )
		{
			IPolylinePtr ipGeom( itRoad->second.shape );
			_ASSERTE( ipGeom );
			if( ipGeom )
			{
				ipGeom->get_FromPoint( ( it->dir == 1 ) ? &ipFrom : &ipTo );
				ipGeom->get_ToPoint( ( it->dir == 1 ) ? &ipTo : &ipFrom );
			}
		}
		bool bFindNext = false;

		std::list<LQDEF>::iterator it2 = it; ++it2; // it2はitの次から
		for( ; it2 != listLQ.end(); ++it2 )
		{
			// FROM、TOを方向を考慮して取得
			IPointPtr ipFrom2, ipTo2;
			std::map<long, ROADDEF>::const_iterator itRoad2 = mapRoad.find( it2->linkid );
			_ASSERTE( itRoad2 != mapRoad.end() );
			if( itRoad2 != mapRoad.end() )
			{
				IPolylinePtr ipGeom( itRoad2->second.shape );
				_ASSERTE( ipGeom );
				if( ipGeom )
				{
					ipGeom->get_FromPoint( ( it2->dir == 1 ) ? &ipFrom2 : &ipTo2   );
					ipGeom->get_ToPoint(   ( it2->dir == 1 ) ? &ipTo2   : &ipFrom2 );
				}
			}
			if( lSeq == 1 )
			{
//				if( xfrom == xto2 && yfrom == yto2 )
				if( AheIsEqual( ipFrom, ipTo2 ) )
				{
					std::list<LQDEF>::iterator it2Next = it2; ++it2;	// it2の次
					listLQ.splice( it, listLQ, it2, it2Next );
					goto FIX_LQ_SEQ;	// 最初から
				}
			}

			// itの次がit2の場合
//			if( xto == xfrom2 && yto == yfrom2 )
			if( AheIsEqual( ipTo, ipFrom2 ) )
			{
				bFindNext = true;
				// it2がitのすぐ後ならそのまま続ける
				// そうでなければitの次に移動
				std::list<LQDEF>::const_iterator it2Prev = it2; --it2Prev;	// it2の前
				if( it2Prev != it )
				{
					std::list<LQDEF>::iterator itNext = it; ++itNext;		// itの次
					std::list<LQDEF>::iterator it2Next = it2; ++it2Next;	// it2の次
					listLQ.splice( itNext, listLQ, it2, it2Next );
				}
			}

			if( bFindNext )
				break;
		}

		if( ! bFindNext )
		{
			for( std::list<LQDEF>::const_iterator it = listLQ.begin(), itEnd = listLQ.end(); it != itEnd; ++it )
				ATLTRACE(_T("不一致：%s\n"), GetTraceLQDEFString( *it, mapRoad ) );

			_ASSERTE(false);
			bSorted = false;
			break;
		}
	}
	// ソートできたらシーケンスを修正していく
	if( bSorted )
	{
		bool bFixed = true;
		long lSeq = 1, lSeqIndex = -1, lLastFIndex = -1, lSize = listLQ.size();
		for( std::list<LQDEF>::iterator it = listLQ.begin(); it != listLQ.end(); ++it, ++lSeq )
		{
			if( it->seq != lSeq )
			{
				// インデックス番号取得は初回のみ
				if( 0 > lSeqIndex )
				{
					IFieldsPtr ipFields;
					it->obj->get_Fields( &ipFields );
					ipFields->FindField( CComBSTR(SEQUENCE_FIELD), &lSeqIndex );
				}
				// 修正
				// LQに関してはLQSetAttrinbuteで既にStoreしちゃっているのでここでもStore
				if( SUCCEEDED( it->obj->put_Value( lSeqIndex, CComVariant(lSeq) ) ) &&
					SUCCEEDED( it->obj->Store() ) )
				{
					ATLTRACE(_T("WARNING : シーケンス番号修正 : %s : -> %d\n"), GetTraceLQDEFString(*it), lSeq );
					it->seq = lSeq;	// 一応こっちも直しておく
				}
				else {
					ATLTRACE(_T("ERROR : シーケンス番号修正失敗 : %s : -> %d\n"), GetTraceLQDEFString(*it), lSeq );
					bFixed = false;
					_ASSERTE(false);
					break;	
				}
			}
			// 終端フラグ
			if( it->last != ( ( lSeq == lSize ) ? true : false ) )
			{
				// インデックス番号取得は初回のみ
				if( 0 > lLastFIndex )
				{
					IFieldsPtr ipFields;
					it->obj->get_Fields( &ipFields );
					ipFields->FindField( CComBSTR(LASTLINK_F_FIELD), &lLastFIndex );
				}
				// 修正
				// LQに関してはLQSetAttrinbuteで既にStoreしちゃっているのでここでもStore
				if( SUCCEEDED( it->obj->put_Value( lLastFIndex, CComVariant( ( lSeq == lSize ) ? 1L : 0L ) ) ) &&
					SUCCEEDED( it->obj->Store() ) )
				{
					ATLTRACE(_T("WARNING : 終端フラグ修正 : %s : -> %s\n"), GetTraceLQDEFString(*it), ( lSeq == lSize ) ? _T("TRUE") : _T("FALSE") );
					it->last = ( lSeq == lSize ) ? true : false;
				}
				else {
					ATLTRACE(_T("ERROR : 終端フラグ修正失敗 : %s : -> %d\n"), GetTraceLQDEFString(*it), ( lSeq == lSize ) ? _T("TRUE") : _T("FALSE") );
					bFixed = false;
					_ASSERTE(false);
					break;
				}
			}
		}
		// 修復できないものがあったらエラー
		bRet = ( bFixed ) ? true : false;
	}
#ifdef _DEBUG
	{
		for( std::list<LQDEF>::const_iterator it = listLQ.begin(), itEnd = listLQ.end(); it != itEnd; ++it )
			ATLTRACE(_T("修復後（%s）：%s\n"), ( bRet ) ? _T("成功") : _T("失敗"), GetTraceLQDEFString( *it, mapRoad ) );
		ATLTRACE(_T("\n"));
	}
#endif // ifdef _DEBUG

	return bRet;
}


IPointCollectionPtr CEditHelper::GetCrossOrTouchPoints( CTargetItems& cDstItems, CTargetItems& cSrcItems, INT iMode/* = 2*/, CTargetItems* pNoRelItems/* = NULL*/ )
{
//	SetDomain();
	// 交差ポイントを探す
	std::list<LongPair> listChecked;
	IPointCollectionPtr ipCrossPointCol;	// 返り値
	for( CTargetItems::iterator itDstItems = cDstItems.begin(); itDstItems != cDstItems.end(); itDstItems++ )
	{
/*		///////////////////////////
		AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), itDstItems->GetShape(), RGB(255,0,255), 2, esriSLSSolid, TRUE, FALSE );
		///////////////////////////*/
		BOOL bFindRelItems = FALSE;	// 交点・接点を見つけられたかどうか
		for( CTargetItems::iterator itSrcItems = cSrcItems.begin(); itSrcItems != cSrcItems.end(); itSrcItems++ )
		{
			if( *itSrcItems == *itDstItems ) continue;	// 同じフィーチャ同士ははじく
			// 既に同じペアがあったかどうかチェック
			LONG lSrcOID = itSrcItems->GetOID(), lDstOID = itDstItems->GetOID();
			for( std::list<LongPair>::iterator it = listChecked.begin(); it != listChecked.end(); it++ )
			{
				if( ( it->lID1 == lSrcOID && it->lID2 == lDstOID ) || ( it->lID1 == lDstOID && it->lID2 == lSrcOID ) )
					continue;
			}
			LongPair pair = { lSrcOID, lDstOID };
			listChecked.push_back( pair );

			ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints() : 比較：%d AND %d\n"), itSrcItems->GetOID(), itDstItems->GetOID() );
/*			///////////////////////////
			AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), itSrcItems->GetShape(), RGB(255,255,0), 2, esriSLSSolid, TRUE, FALSE );
			continue;
			///////////////////////////*/

			// 形状のIsKnownSimpleにVARIANT_TRUEを設定。
			// 新規作成したフィーチャの形状にIsKnownSimpleを設定してても、
			// 次に取り出すときにVARIANT_FALSEになっているので。
			IGeometryPtr ipSrcGeom = AheSetKnownSimple( itSrcItems->GetShapeCopy() );
			IGeometryPtr ipDstGeom = AheSetKnownSimple( itDstItems->GetShapeCopy() );

			BOOL bIsDisjoint = TRUE; // Disjointかどうか
			IPointCollectionPtr ipIntersectPoints = AheGetCrossOrTouchPoints( ipDstGeom, ipSrcGeom, iMode, &bIsDisjoint );
			if( ! bIsDisjoint )
				bFindRelItems = TRUE; // 分割モードによらず、Disjointかどうかで判断しないと構成点の重複チェックができない（bug 8463 ROADCHECK 02320）

			if( ipIntersectPoints == NULL ) continue;

			ISpatialReferencePtr ipSpRef( AheGetSpatialReference( ipIntersectPoints ) );

			// ポイントコレクションに追加
			LONG lCount;
			ipIntersectPoints->get_PointCount( &lCount );
			IPointPtr ipPoint( CLSID_Point );

			ipPoint->putref_SpatialReference( ipSpRef ); // 空間参照付与
			for( LONG i = 0; i < lCount; i++ )
			{
				ipPoint->PutCoords( 0.0f, 0.0f );
				ipIntersectPoints->QueryPoint( i, ipPoint );

				if( ipCrossPointCol == NULL )
				{
					ipCrossPointCol.CreateInstance( CLSID_Multipoint );
					((IGeometryPtr)ipCrossPointCol)->putref_SpatialReference( AheGetSpatialReference( ipPoint ) ); // 空間参照付与
				}

				ipCrossPointCol->AddPoint( (IPointPtr)AheGetClone(ipPoint) );
/*#ifdef DEBUG
				AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipPoint, RGB(50, 255, 0 ), 8, esriSMSCircle );
#endif*/
			}
		}
		// pNoRelItems が渡されている場合は DstItems の中で交点・接点を見つけることができなかったものを入れておく
		if( pNoRelItems != NULL )
		{
			if( bFindRelItems == FALSE )
				pNoRelItems->push_back( *itDstItems );
		}
	}

	AheRemoveAdjoiningPoint( ipCrossPointCol );

	return ipCrossPointCol;
}

// 関連オブジェクトを削除
bool DeleteRelation( ITablePtr ipRelTable, std::pair<CString, long>& outerKeyVal )
{
	// 検索
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQueryFilter = AheInitQueryFilter( NULL, NULL, _T("%s=%ld"), outerKeyVal.first, outerKeyVal.second );
	ipRelTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	if( !ipCursor )
	{
		_ASSERTE( false );
		return false;
	}

	_IRowPtr ipRow;
	while( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		if(SUCCEEDED(ipRow->Delete()))
		{
			// 更に関連づいていれば削除
			CString myTable = AheGetFeatureClassName( ipRelTable );
			CString relTable = AheGetRelationTableName( myTable, _T("") );
			// 関連づいたテーブルがなければ終了
			if( relTable.IsEmpty() )
				continue;

			CString field = AheGetRelationOuterKeyFieldName( myTable, _T("") );
			ITablePtr relT = AheOpenSameOwnerTable( ipRelTable, relTable );
			// TODO: OID以外をキーにできないな、、
			if(!DeleteRelation( relT, std::make_pair( field, AheGetOID(ipRow) ) ))
				return false;
		}
		else {
			AheMessageBox( ERROR_FAIL_DELETE_EXTERNAL_RECORD, EXTERNAL_RECORD_DELETE_FAILURE, MB_OK|MB_ICONSTOP );
			return false;
		}
	}
	return true;
}

// 分割されたNWフィーチャの関連テーブルを削除
BOOL CEditHelper::DeleteRelationTables( IFeaturePtr ipFeature ) const
{
	// 引数チェック
	if( ! ipFeature ) 
	{
		_ASSERTE( ipFeature );
		return FALSE;
	}

	long lOID = AheGetOID( ipFeature ); // 関連テーブルを持つフィーチャのオブジェクトIDを取得
	CString strClassName( AheGetFeatureClassName( ipFeature ) ); // フィーチャクラス名

	// 外部テーブル関連フラグのリストを取得
	std::list<CString> listRelFieldName = AheGetRelationFlagFieldName( strClassName );
	for( std::list<CString>::const_iterator it = listRelFieldName.begin(); it != listRelFieldName.end(); ++it )
	{
		// [SJ鉄道全路線]鉄道系のフィーチャクラスのときは関連フラグをみない
		// [高精度地図] ADAMネットワークのときも関連フラグみない
		if( 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_link::kTableName )
			&& 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_node::kTableName )
			&& !AheIsAdamNetwork(strClassName))
		{
			// 関連フラグが立っているかどうか
			CComVariant vaRelF = AheGetAttribute( ipFeature, *it );
			if( !(vaRelF.vt == VT_I4 && vaRelF.lVal == 1) )
				continue;
		}

		// 外部テーブルを取得
		ITablePtr ipRelTable( AheOpenRelationTable( ipFeature, *it ) );
		// 外部テーブルにある外部キーを取得
		CString strOuterKeyFieldName( AheGetRelationOuterKeyFieldName( strClassName, *it ) );

		// 外部テーブルに紐付いているものも全て削除
		if(!DeleteRelation( ipRelTable, std::make_pair( strOuterKeyFieldName, lOID ) ))
			return FALSE;
	}
	return TRUE;
}

// 指定したフィールドを外部キーとしてテーブルに関連付ける
bool CopyRelation( ITablePtr ipRelTable, std::pair<CString, long>& outerKeyVal, long parentID )
{
	// 検索
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQueryFilter = AheInitQueryFilter( NULL, NULL, _T("%s=%ld"), outerKeyVal.first, outerKeyVal.second );
	ipRelTable->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor );

	if( !ipCursor )
	{
		_ASSERTE( false );
		return false;
	}

	_IRowPtr ipRow;
	while( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		_IRowPtr ipNewRow = AheCreateRow( ipRelTable, NULL, TRUE );
		if( AheCopyAttribute( ipRow, ipNewRow, TRUE )                           && // [bug 4043] 分割モードでコピー
			AheSetModifyData( ipNewRow, sindyUpdateTypeCreate )                 &&
			AheSetAttribute( ipNewRow, outerKeyVal.first, parentID )            &&
			SUCCEEDED( ipNewRow->Store() ) )
		{
			// 更に関連づいていればコピー（車種別規制対応）
			CString myTable = AheGetFeatureClassName( ipRelTable );
			CString relTable = AheGetRelationTableName( myTable, _T("") );
			// 関連づいたテーブルがなければ終了
			if( !relTable.IsEmpty() )
			{
				CString field = AheGetRelationOuterKeyFieldName( myTable, _T("") );
				ITablePtr relT = AheOpenSameOwnerTable( ipRelTable, relTable );
				// TODO: OID以外をキーにできないな、、
				CopyRelation( relT, std::make_pair( field, AheGetOID(ipRow) ), AheGetOID(ipNewRow) );
			}
		}
		else {
			AheMessageBox( ERROR_FAIL_COPY_EXTERNAL_RECORD, EXTERNAL_RECORD_COPY_FAILURE, MB_OK|MB_ICONSTOP );
			return false;
		}
	}
	return true;
}

BOOL CEditHelper::CopyRelationAttribute( IFeaturePtr ipFeature, CTargetItems& cSplittedRoadFeatures )
{
	// 引数チェック
	if( ! ipFeature ) 
	{
		_ASSERTE( ipFeature );
		return FALSE;
	}
	if( cSplittedRoadFeatures.empty() )
		return TRUE;

	long lOID = AheGetOID( ipFeature ); // 関連テーブルを持つフィーチャのオブジェクトIDを取得
	CString strClassName( AheGetFeatureClassName( ipFeature ) ); // フィーチャクラス名

	// 外部テーブル関連フラグのリストを取得
	std::list<CString> listRelFieldName = AheGetRelationFlagFieldName( strClassName );
	for( std::list<CString>::const_iterator it = listRelFieldName.begin(); it != listRelFieldName.end(); ++it )
	{
		// [SJ鉄道全路線]鉄道系のフィーチャクラスのときは関連フラグをみない
		// [高精度地図] ADAMネットワークのときも関連フラグみない
		if( 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_link::kTableName )
			&& 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_node::kTableName ) 
			&& !AheIsAdamNetwork(strClassName))
		{
			// 関連フラグが立っているかどうか
			CComVariant vaRelF = AheGetAttribute( ipFeature, *it );
			if( !(vaRelF.vt == VT_I4 && vaRelF.lVal == 1) )
				continue;
		}

		// 外部テーブルを取得
		ITablePtr ipRelTable( AheOpenRelationTable( ipFeature, *it ) );
		// 外部テーブルにある外部キーを取得
		CString strOuterKeyFieldName( AheGetRelationOuterKeyFieldName( strClassName, *it ) );

		// 新規に作成されたリンクを探す
		for( CTargetItems::iterator itItem = cSplittedRoadFeatures.begin(); itItem != cSplittedRoadFeatures.end(); ++itItem )
		{
			if( lOID == itItem->GetOID() )
				continue;	// 同じものは省く

			// 外部テーブルに紐付いているものも全てコピー
			CopyRelation( ipRelTable, std::make_pair( strOuterKeyFieldName, lOID ), itItem->GetOID() );

			// [SJ鉄道全路線]鉄道系のフィーチャクラスでは関連フラグを付与しない
			// [高精度地図] ADAMネットワークのときも関連フラグみない
			if( 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_link::kTableName )
				&& 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_node::kTableName )
				&& !AheIsAdamNetwork(strClassName))
			{
				// 関連フラグがある場合はフラグを付与
				itItem->SetAttribute( *it, 1L );
			}
		}
	}
	return TRUE;
}

void CEditHelper::AddInterior(IPolygon2Ptr pIPolygonBase, IRingPtr pIRingExteriorBase, IGeometryCollectionPtr pIGeometryCollection)
{
	// 引数チェック
	if( ! ( pIPolygonBase && pIRingExteriorBase && pIGeometryCollection ) )
	{
		_ASSERTE( pIPolygonBase && pIRingExteriorBase && pIGeometryCollection );
		return;
	}

	ISpatialReferencePtr ipSpRef( AheGetSpatialReference( pIPolygonBase ) );

	IGeometryCollectionPtr pIGeometryCollectionSrc( CLSID_Polygon );
	((IGeometryPtr)pIGeometryCollectionSrc)->putref_SpatialReference( ipSpRef ); // 空間参照付与
	pIGeometryCollectionSrc->AddGeometry(pIRingExteriorBase);
	
	if( pIPolygonBase )
	{
		LONG lExteriorCount = 0;
		pIPolygonBase->get_ExteriorRingCount(&lExteriorCount);

		/// こちらでメモリ確保
		IRingPtr *parrIRingExterior = new IRingPtr[ lExteriorCount ];

		pIPolygonBase->QueryExteriorRingsEx(lExteriorCount, reinterpret_cast<IRing**>(parrIRingExterior));

		for ( int l = 0; l < lExteriorCount; l++ )
		{
			LONG lInteriorCount = 0;
			pIPolygonBase->get_InteriorRingCount(parrIRingExterior[l], &lInteriorCount);

			if ( 0 < lInteriorCount ) {
				/// こちらでメモリ確保
				IRingPtr* parrIRingInterior = new IRingPtr[ lInteriorCount ];

				pIPolygonBase->QueryInteriorRingsEx(parrIRingExterior[l], lInteriorCount, reinterpret_cast<IRing**>(parrIRingInterior));

				for ( int m = 0; m < lInteriorCount; m++ )
				{
					IGeometryCollectionPtr pIGeometryCollectionDest( CLSID_Polygon );
					((IGeometryPtr)pIGeometryCollectionDest)->putref_SpatialReference( ipSpRef ); // 空間参照付与
					pIGeometryCollectionDest->AddGeometry(parrIRingInterior[m]);

					if( AheIsContain3( pIGeometryCollectionSrc, pIGeometryCollectionDest ) )
						pIGeometryCollection->AddGeometry(parrIRingInterior[m]);
				}

				delete [] parrIRingInterior;
			}
		}

		delete [] parrIRingExterior;
	}

	return;
}

BOOL CEditHelper::SplitMultiPolygonToSimplePolygons(ILayerPtr pILayer, CTargetItems& cSource, CTargetItems& cDest)
{
	ITopologicalOperatorPtr pITopologicalOperator;
	IPolygon2Ptr pIPolygon, pINewPolygon;
	IFeaturePtr pIFeature;
	IGeometryPtr pIGeometry, pIGeometryPolygon, pIGeometryTmp, pIGeometryExterior, pIGeometryResult;
	IGeometryCollectionPtr pIGeometryCollectionPolygon, pIGeometryCollectionNewPolygon;
	ISpatialReferencePtr pISpatialReference;
	IRingPtr* parrIRingExterior = NULL;
	std::list< CAdapt< IGeometryPtr> > listGeometry;
	LONG lGeometryNewCount = 0, lOuterCount = 0;
	BOOL bFlg = FALSE;

	for ( CTargetItems::iterator it = cSource.begin(); it != cSource.end(); it++ ) {
		pIGeometry = it->GetShape();
		pIFeature = it->GetFeature();

#ifdef _DEBUG
		LONG OID = 0;
		OID = (*it).GetOID();
		ATLTRACE(_T("OID = %d\n"), OID);
#endif
		pITopologicalOperator = pIGeometry;
		pIGeometry->get_SpatialReference(&pISpatialReference);

		pIPolygon = pIGeometry;
		pIGeometryCollectionPolygon = pIPolygon;

		if ( pIPolygon ) {
			pIPolygon->get_ExteriorRingCount(&lOuterCount);

			if ( lOuterCount < 2 ) continue;
			
			/// こちらでメモリ確保
			parrIRingExterior = new IRingPtr[ lOuterCount - 1 ];

			ATLTRACE(_T("Ring Outer : %d\n"), lOuterCount);

			pIPolygon->QueryExteriorRingsEx(lOuterCount - 1, reinterpret_cast<IRing**>(parrIRingExterior));

			for ( long l = 0; l < lOuterCount - 1; l++ ) {
				pINewPolygon.CreateInstance(CLSID_Polygon);
				pINewPolygon->putref_SpatialReference(pISpatialReference); // 空間参照付与

				pIGeometryCollectionNewPolygon = pINewPolygon;

				pIGeometryCollectionNewPolygon->get_GeometryCount(&lGeometryNewCount);
				pIGeometryCollectionNewPolygon->RemoveGeometries(0, lGeometryNewCount);

				pIGeometryExterior = parrIRingExterior[l];
				
				if ( pIGeometryExterior ) {
					pIGeometryCollectionNewPolygon->AddGeometry(pIGeometryExterior);
#ifdef _DEBUG
					AheDrawPolygon(GetFocusMapScreenDisplay(), pIGeometryExterior, RGB(0,0,255), 3, esriSLSSolid, TRUE);
#endif
				}

				AddInterior(pIPolygon, parrIRingExterior[l], pIGeometryCollectionNewPolygon);

				IFeaturePtr pIFeatureNewPolygon;
				IFeatureClassPtr pIPolygonFeatureClass;

				AheGetFeatureClass(GetFocusMap(), pILayer, &pIPolygonFeatureClass);

				if ( pIPolygonFeatureClass ) {
					pIPolygonFeatureClass->CreateFeature(&pIFeatureNewPolygon);

					/// デフォルト値を入れる
					AheSetDefaultData(pIFeatureNewPolygon);

					AheCopyAttribute(pIFeature, pIFeatureNewPolygon, TRUE );	// [bug 4043] 分割モードでコピー

					AheSetModifyData(pIFeatureNewPolygon, sindyUpdateTypeCreate);

					pIGeometryCollectionNewPolygon->get_GeometryCount(&lGeometryNewCount);
					pINewPolygon = pIGeometryCollectionNewPolygon;

					pIGeometryPolygon = pINewPolygon;
					pIGeometryPolygon->putref_SpatialReference(pISpatialReference);
					pIGeometryPolygon->SnapToSpatialReference();

					if ( pIFeatureNewPolygon ) {
						pIFeatureNewPolygon->putref_Shape(pIGeometryPolygon);
						cDest.push_back( pIFeatureNewPolygon );
						cDest.Store();

						// 抜くための図形を作る
						listGeometry.push_back( pIGeometryPolygon );
						bFlg = TRUE;
					}
				}
			}

			delete[] parrIRingExterior;

			/// 元のポリゴンから、新規抽出分を削除
			for ( std::list< CAdapt< IGeometryPtr> >::iterator it2 = listGeometry.begin(); it2 != listGeometry.end(); it2++ ) 
			{
				pIGeometryTmp = it2->m_T;

				if ( it2 == listGeometry.begin() ) {
					pIGeometryTmp->putref_SpatialReference(pISpatialReference);
				} else {
					pIGeometryResult->putref_SpatialReference(pISpatialReference);
					pITopologicalOperator = pIGeometryResult;
				}
				
				pITopologicalOperator->Difference(pIGeometryTmp, &pIGeometryResult);
				pITopologicalOperator->Simplify();
			}

			(*it).SetShape(pIGeometryResult);
			(*it).SetModifyData(sindyUpdateTypeShape);
		}
	}

	return bFlg;
}

int CEditHelper::CreateInteriorPolygon(ILayer* pILayer, IPoint* pIPoint, IGeometry** pIGeometry)
{
	CTargetItems cNearPolygons;
	int nErr = 0;

	// 指定ポイントにポリゴンがある場合は終了
	long lNum = AheSelectByShapeFromLayer( pIPoint, cNearPolygons, pILayer, esriSpatialRelIntersects);
	if( 0 < lNum )
		return -2; 

	// 空間参照の取得
	ISpatialReferencePtr ipSpRef;
	pIPoint->get_SpatialReference( &ipSpRef );

	// 画面のサイズを取得
	IDisplayTransformationPtr ipDispTrans;
	IEnvelopePtr ipDisplayEnvelope;

	GetFocusMapScreenDisplay()->get_DisplayTransformation( &ipDispTrans );
	ipDispTrans->get_VisibleBounds( &ipDisplayEnvelope );
	ipDisplayEnvelope->putref_SpatialReference( ipSpRef );
	IGeometryPtr ipGeomSearch = AheEnvelope2Polygon( ipDisplayEnvelope );
	if( ipGeomSearch )
	{
		cNearPolygons.clear();

		/// 範囲内のポリゴンを検索
		long lCount = AheSelectByShapeFromLayer( ipGeomSearch, cNearPolygons, pILayer, esriSpatialRelIntersects);
	
		if( 0 < lCount )
		{
			/// 作成
			ATLTRACE(_T("Search Item = %d\n"), cNearPolygons.size());

			IGeometryPtr ipGeomBase;
			ITopologicalOperatorPtr ipTopo;
			for( CTargetItems::iterator it = cNearPolygons.begin(); it != cNearPolygons.end(); it++ )
			{
				IGeometryPtr ipGeomNearPolygon = it->GetShapeCopy();
#ifdef _DEBUG				
				long lOID = (*it).GetOID();
				ATLTRACE(_T("Object ID %d\n"), lOID);
#endif
				if( it == cNearPolygons.begin() )
					ipGeomBase = ipGeomNearPolygon;
				else
				{
					ipTopo = ipGeomBase;
					ipTopo->Union( ipGeomNearPolygon, &ipGeomBase );
				}
			}

			ipTopo = ipGeomSearch;
			IGeometryPtr ipGeomResult;
			try {
				// 穴が開いているところをポリゴン化させるため、画面全体のポリゴンとのDiffを取得する
				ipTopo->Difference( ipGeomBase, &ipGeomResult );
			}
			catch( _com_error e)
			{
				return -1;
			}
			IPolygon4Ptr ipPolygonRing = ipGeomResult;
			if( ipPolygonRing )
			{
				// マルチポリゴンをシングルポリゴンに変換
				long lPolyCount = 0;
				ipPolygonRing->get_ExteriorRingCount( &lPolyCount );

				IGeometryBagPtr ipGeoBag;
				ipPolygonRing->get_ConnectedComponentBag( &ipGeoBag );

				IGeometryCollectionPtr ipGeoCollection( ipGeoBag );
				if( !ipGeoCollection )
					return -1;

				long lGeoCnt = 0;
				ipGeoCollection->get_GeometryCount( &lGeoCnt );
				for( long l = 0; l < lGeoCnt; ++l )
				{
					IGeometryPtr ipTmpGeom;
					ipGeoCollection->get_Geometry( l, &ipTmpGeom );
					IPolygonPtr ipNewPolygon = ipTmpGeom;
					if( ipNewPolygon )
					{
						ipNewPolygon->putref_SpatialReference( ipSpRef );
						if( AheIsContain3( ipNewPolygon, pIPoint ) )
						{
							/// こいつが作成する対象
							AheDrawPolygon( GetFocusMapScreenDisplay(), ipNewPolygon, RGB(0,255,0) , 2, esriSLSSolid, TRUE );
							int nRet = AheMessageBox( CONFIRM_FILL_THIS_CLOSED_RING, FILL_METHOD, MB_YESNO | MB_ICONQUESTION , WARNLEVEL_HIGH );				
							if( IDYES == nRet ) 
							{
								*pIGeometry = ipNewPolygon;
								if( *pIGeometry ) (*pIGeometry)->AddRef();
								nErr = 1;
							} 
							else
							{
								*pIGeometry = NULL;
								nErr = -3;
							}
							break;
						}
					}
				}
			}
		}
	}

	return nErr;
}

BOOL CEditHelper::CutOutPolygonByPolygon( CTargetItems& cItems, CTargetItems& cModifyItems, bool bAskOverlap /*=true*/ )
{
	for( CTargetItems::iterator it = cItems.begin(); it != cItems.end(); it++ )
	{
		// 重複許可レイヤだったら飛ばす
		if( it->GetGeometryType() != esriGeometryPolygon ||
			AheIsOverlapableLayer( it->GetFeatureClassName() ) ) 
			continue;

		ISpatialReferencePtr ipSpRef = AheGetSpatialReference( it->GetFeature() );

		// 形状を少しバッファリング
		double dBuffer = AheGetXYDomain1UnitLength( ipSpRef );
		ITopologicalOperatorPtr ipTopo( it->GetShapeCopy() );
		IGeometryPtr ipBufferGeom;
		if ( ipTopo )
			ipTopo->Buffer( dBuffer, &ipBufferGeom );
		else
			ipBufferGeom = it->GetShapeCopy();

		// バッファリングした形状から同じフィーチャクラスのポリゴンを検索
		CTargetItems cSearchResult;
		AheSelectByShapeFromFeatureClass( ipBufferGeom, cSearchResult, it->GetFeatureClass() );

		CTargetItems cShapeEditItems;	// 形状変更するフィーチャのリスト
		CTargetItems cTouchItems;		// 作成形状にTouchだけしているフィーチャのリスト
		for( CTargetItems::iterator itSearchResult = cSearchResult.begin(); itSearchResult != cSearchResult.end(); itSearchResult++ )
		{
			// 同じものははじく
			bool bFind = false;
			for( CTargetItems::iterator itFind = cItems.begin(); itFind != cItems.end(); itFind++ )
			{
				if( *itSearchResult == *itFind )
				{
					bFind = true;
					break;
				}
			}
			if( bFind )	continue;

			///////
			/// ITopologicalOperator->Difference(), Union(),etcをすると出力形状は空間参照にフィットされてしまうので
			/// 上記処理の前に、Base形状、Add形状との接点交点を、構成点としてそれぞれに追加する必要がある
			/// （XOR作成処理だけでなく通常ポリゴン作成時にも必要な処理）[bug5485]
			///////
			// 検索形状と新規作成形状との交点、接点を取得
			IGeometryPtr ipGeomPointCol = AheGetCrossOrTouchPoints( itSearchResult->GetShapeCopy(), it->GetShapeCopy(), 2 );
			// 取得したポイント群を新規作成形状上に追加
			AheAddVertexesIfOnGeometry( it->GetShape(), ipGeomPointCol, ipSpRef );
			// 取得したポイント群を検索形状上に追加
			if( AheAddVertexesIfOnGeometry( itSearchResult->GetShape(), ipGeomPointCol, ipSpRef ) )
			{
				// Touchしているだけのフィーチャは重なり判定はしないので別に取っておく
				// (ここには検索形状の構成点だけにTouchしている形状は入ってこない)
				if( AheIsTouch( AheSetKnownSimple( it->GetShapeCopy() ), itSearchResult->GetShapeCopy(), ipSpRef ) )
					cTouchItems.push_back( *itSearchResult );
				else
					cShapeEditItems.push_back( *itSearchResult );
				continue;
			}
			// AheAddVertexesIfOnGeometry()でもれたフィーチャを取得(上記処理だと検索フィーチャを内包している場合と、検索フィーチャに内包されている場合の条件は取得できないため)
			// 上記処理だと検索フィーチャの構成点だけにTouchしてOverlapしている場合は取得できないため[bug5644]
			if( AheIsContain( AheSetKnownSimple( it->GetShapeCopy() ), itSearchResult->GetShapeCopy(), ipSpRef )
				|| AheIsWithin( AheSetKnownSimple( it->GetShapeCopy() ), itSearchResult->GetShapeCopy(), ipSpRef )
				|| AheIsOverlap( AheSetKnownSimple( it->GetShapeCopy() ), itSearchResult->GetShapeCopy(), ipSpRef ))	//[bug5644]overlap追加
				cShapeEditItems.push_back( *itSearchResult );
		}

		// 更履歴追加
		for( CTargetItems::iterator itTouchItems = cTouchItems.begin(); itTouchItems != cTouchItems.end(); itTouchItems++ )
		{
			// 編集可能フィーチャでなければ終了	[bug5508]
			if( AheIsEditableFeature2( GetFocusMap(), itTouchItems->GetFeature(), NULL, sindyeEditMove, m_ipRule, FALSE, GetHWND() ) )
			{
				// 履歴追加、形状適用
				itTouchItems->SetModifyData( sindyUpdateTypeShape );
				itTouchItems->SetShape( itTouchItems->GetShape() );
				cModifyItems.push_back( *itTouchItems );
			}
			else
				return FALSE;
		}

		// フィーチャが選択できた場合は重複回避するか聞く
		if( !cShapeEditItems.empty() )
		{
			// 回避するかどうか
			if( bAskOverlap )
				if( MessageBox( GetHWND(), CONFIRM_CHANGE_SHAPE_AND_EVADE_HEAP, CREATING_POLYGON, MB_YESNO|MB_ICONQUESTION ) == IDNO )
					return FALSE;

			for( CTargetItems::iterator itShapeEditItems = cShapeEditItems.begin(); itShapeEditItems != cShapeEditItems.end(); itShapeEditItems++ )
			{
				// 編集可能フィーチャでなければ終了	[bug5508]
				if( AheIsEditableFeature2( GetFocusMap(), itShapeEditItems->GetFeature(), NULL, sindyeEditMove, m_ipRule, FALSE, GetHWND() ) )
				{
					ITopologicalOperatorPtr ipBaseTopo( AheSetKnownSimple( itShapeEditItems->GetShapeCopy() ) );
					if( ipBaseTopo )
					{
						// 検索形状をベースとして新規作成形状とのDifferenceを取得
						IGeometryPtr ipDiffGeom;
						ipBaseTopo->Difference( it->GetShapeCopy(), &ipDiffGeom );
						if( ipDiffGeom )
						{
							VARIANT_BOOL vaIsEmpty = VARIANT_FALSE;
							ipDiffGeom->get_IsEmpty( &vaIsEmpty );

							// Difference形状がなければ削除
							if( vaIsEmpty )
							{
								// 重複形状がNULLなら削除するが、削除対象レイヤかチェックが必要[bug2357]
								if( AheIsEditableFeature2( GetFocusMap(), itShapeEditItems->GetFeature(), NULL, sindyeEditDelete, GetRule(), FALSE, GetHWND() )) 
									itShapeEditItems->Delete();
								else
									return FALSE;
							}
							else
							{
								// 履歴追加、形状適用
								itShapeEditItems->SetModifyData( sindyUpdateTypeShape );
								itShapeEditItems->SetShape( ipDiffGeom );
								cModifyItems.push_back( *itShapeEditItems );
							}
						}
					}
				}
				else
					return FALSE;
			}
		}
	}

	return TRUE;
}

IPointCollectionPtr CEditHelper::GetCrossOrTouchPoints2( CTargetItems& cDestItems, CTargetItems& cSrcItems, INT iMode/* = 2*/, CTargetItems* pNoRelItems/* = NULL*/ )
{
	// 交差ポイントを探す
	std::list<LongPair> 	listChecked;
	ITopologicalOperatorPtr ipTopo;
	IPointCollectionPtr 	ipCrossPointCol, ipIntersectPoints;	// 返り値
	IGeometryPtr			ipDestGeom;

	for( CTargetItems::iterator itDest = cDestItems.begin(); itDest != cDestItems.end(); itDest++ ) {
		LONG	lDestOID = itDest->GetOID();
		BOOL	bFindRelItems = FALSE;		// 交点・接点を見つけられたかどうか

		ipDestGeom = itDest->GetShape();
		for( CTargetItems::iterator itSrc = cSrcItems.begin(); itSrc != cSrcItems.end(); itSrc++ ) {
			// 同じフィーチャ同士ははじく
			if ( *itSrc == *itDest )
				continue;

			// 既に同じペアがあったかどうかチェック
			LONG	lSrcOID = itSrc->GetOID();
			BOOL	TorF = FALSE;

			for ( std::list<LongPair>::iterator it = listChecked.begin(); it != listChecked.end(); it++ ) {
				if( ( it->lID1 == lSrcOID && it->lID2 == lDestOID ) || ( it->lID1 == lDestOID && it->lID2 == lSrcOID ) ) {
					TorF = TRUE;
					break;
				}
			}
			if ( TorF == TRUE )
				continue;

			LongPair pair = { lSrcOID, lDestOID };
			listChecked.push_back( pair );

			ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints() : 比較：%d AND %d\n"), lSrcOID, lDestOID );

			ipIntersectPoints = GetCrossOrTouchPoints2( ipDestGeom, itSrc->GetShape(), iMode );
			if ( ipIntersectPoints != NULL ) {
				long		Count;

				ipIntersectPoints->get_PointCount( &Count );
				if ( Count >= 1 ) {
					WKSPoint*	wks = NULL;

					wks = new WKSPoint[Count];
					ipIntersectPoints->QueryWKSPoints( 0, Count, wks );

					if ( ipCrossPointCol == NULL )
					{
						ipCrossPointCol.CreateInstance( CLSID_Multipoint );
						((IGeometryPtr)ipCrossPointCol)->putref_SpatialReference( AheGetSpatialReference( itSrc->GetSpatialReference() ) ); // 空間参照付与
					}

					ipCrossPointCol->AddWKSPoints( Count, wks );

					delete [] wks;

					bFindRelItems = TRUE;
				}
			}
		}

		// pNoRelItems が渡されている場合は DestItems の中で交点・接点を見つけることができなかったものを入れておく
		if ( pNoRelItems != NULL ) {
			if ( bFindRelItems == FALSE )
				pNoRelItems->push_back( *itDest );
		}
	}

	ipTopo = ipCrossPointCol;
	if ( ipTopo != NULL )
		ipTopo->Simplify();	// 重複点は取り除く
	return ipCrossPointCol;
}

IGeometryPtr CEditHelper::GetCrossOrTouchPoints2( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, INT iMode/* = 2*//*, BOOL bTouchSideIsCross*//* = FALSE*/ )
{
	if( ipBaseGeometry == NULL || ipTestGeometry == NULL )
		return NULL;

	IGeometryPtr 				ipBaseGeomCopy, ipTestGeomCopy;
	ISegmentCollectionPtr 		ipBaseSegColCopy, ipTestSegColCopy;
	IPointCollectionPtr 		ipIntersectPoints;
	IEnumSegmentPtr				ipEnumSegment, ipTestEnumSegment;
	_ISegmentPtr 				ipBaseSegTmp, ipBaseSeg, ipTestSegTmp, ipTestSeg, ipPrevBaseSeg, ipNextBaseSeg, ipPrevTestSeg, ipNextTestSeg;
	IPointPtr					ipPrevBaseSegFromPoint, ipPrevBaseSegToPoint, ipNextBaseSegFromPoint, ipNextBaseSegToPoint;
	IPointPtr 					ipPrevTestSegFromPoint, ipPrevTestSegToPoint, ipNextTestSegFromPoint, ipNextTestSegToPoint;
	sindyeSpatialRel 			enumSpRel;
	DOUBLE 						dTorelanceMin, dTorelanceMax, dTorelanceMaxMin, dTorelanceMinMax, dTorelance, dBaseFromX = 0.0f, dBaseFromY = 0.0f, dBaseToX = 0.0f, dBaseToY = 0.0f, dTestFromX = 0.0f, dTestFromY = 0.0f, dTestToX = 0.0f, dTestToY = 0.0f;
	LONG 						lBaseGeomCount = -1, lTestGeomCount = -1, lBaseSegColCount, lTestSegColCount, lPartIndex, lVertexIndex, lBaseIndex, lTestIndex;

	switch( iMode ) {
		case 0:	 enumSpRel = sindyeSpatialRelCross; break;												// Cross
		case 1:  enumSpRel = sindyeSpatialRelTouch; break;												// Touch
		case 2:  enumSpRel = (sindyeSpatialRel)(sindyeSpatialRelCross|sindyeSpatialRelTouch); break;	// Cross and Touch
		default: enumSpRel = sindyeSpatialRelNone;	break;
	}

	// 空間参照取得
	ISpatialReferencePtr ipBaseSpRef, ipTestSpRef;
	ipBaseGeometry->get_SpatialReference( &ipBaseSpRef );
	ipTestGeometry->get_SpatialReference( &ipTestSpRef );

	IPolylinePtr ipBaseSegPolyline( CLSID_Polyline ), ipTestSegPolyline( CLSID_Polyline );
	ipBaseSegPolyline->putref_SpatialReference( ipBaseSpRef ); // 空間参照付与
	ipTestSegPolyline->putref_SpatialReference( ipTestSpRef ); // 空間参照付与
	IPointPtr ipBaseFromPoint( CLSID_Point ), ipBaseToPoint( CLSID_Point ), ipTestFromPoint( CLSID_Point ), ipTestToPoint( CLSID_Point );
	ipBaseFromPoint->putref_SpatialReference( ipBaseSpRef ); // 空間参照付与
	ipBaseToPoint->putref_SpatialReference( ipBaseSpRef ); // 空間参照付与
	ipTestFromPoint->putref_SpatialReference( ipTestSpRef ); // 空間参照付与
	ipTestToPoint->putref_SpatialReference( ipTestSpRef ); // 空間参照付与
	// 両方の空間参照の細かいほうからしきい値を割り出す
	ISpatialReferencePtr ipMinimumSpRef ( AheGetMinimumSpatialReference( ipBaseSpRef, ipTestSpRef ) );
	dTorelance = AheGetXYDomain1UnitLength( ipMinimumSpRef );
	dTorelanceMin = -dTorelance;
	dTorelanceMax = 1.0f + dTorelance;
	dTorelanceMinMax = dTorelance;
	dTorelanceMaxMin = 1.0f - dTorelance;

	// 一度ジオメトリコレクションに落とす（）
	IGeometryCollectionPtr ipBaseGeomColCopy( AheGetClone( ipBaseGeometry ) );
	IGeometryCollectionPtr ipTestGeomColCopy( AheGetClone( ipTestGeometry ) );

	if( ipBaseGeomColCopy == NULL || ipTestGeomColCopy == NULL ) {
		_ASSERTE( ipBaseGeomColCopy != NULL && ipTestGeomColCopy != NULL );
		return NULL;
	}

	// 交差ポイントを探す
	ipIntersectPoints.CreateInstance( CLSID_Multipoint );
	((IGeometryPtr)ipIntersectPoints)->putref_SpatialReference( ipBaseSpRef ); // ベースの空間参照付与
	ipBaseGeomColCopy->get_GeometryCount( &lBaseGeomCount );
	ipTestGeomColCopy->get_GeometryCount( &lTestGeomCount );

	// Base のジオメトリごとのループ
	ipBaseSegColCopy = AheGetClone( ipBaseGeometry );
	if ( ipBaseSegColCopy == NULL ) {
		_ASSERTE( ipBaseSegColCopy != NULL );
		return NULL;
	}

	// Base のセグメントごとのループ
	lBaseIndex = 0;
	ipBaseSegColCopy->get_SegmentCount( &lBaseSegColCount );
	ipBaseSegColCopy->get_EnumSegments( &ipEnumSegment );
	ipEnumSegment->Reset();
	while( S_OK == ipEnumSegment->Next( &ipBaseSegTmp, &lPartIndex, &lVertexIndex ) ) {
		dBaseFromX = dBaseFromY = dBaseToX = dBaseToY = 0.0f;

		ipBaseSeg = AheGetClone( (_ISegmentPtr)ipBaseSegTmp );
		if ( ipBaseSeg == NULL ) {
			lBaseIndex++;
			continue;
		}

		// ポイント初期化
		ipBaseFromPoint->PutCoords( 0.0f, 0.0f );
		ipBaseToPoint->PutCoords( 0.0f, 0.0f );

		if( FAILED( ipBaseSeg->QueryFromPoint( ipBaseFromPoint ) ) || FAILED( ipBaseSeg->QueryToPoint( ipBaseToPoint ) ) ) {
			ATLASSERT( FALSE );
			AheTraceErrorMessage( _T("GetCrossOrTouchPoints2()") );
			lBaseIndex++;
			continue;
		}

		ipBaseFromPoint->QueryCoords( &dBaseFromX, &dBaseFromY );
		ipBaseToPoint->QueryCoords( &dBaseToX, &dBaseToY );

		// セグメント -> ポリライン変換
		ipBaseSegPolyline->SetEmpty();
		((IPointCollectionPtr)ipBaseSegPolyline)->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
		((IPointCollectionPtr)ipBaseSegPolyline)->AddPoint( (IPointPtr)AheGetClone( ipBaseToPoint ) );

		DOUBLE Ax = (DOUBLE)( dBaseToX - dBaseFromX );
		DOUBLE Ay = (DOUBLE)( dBaseToY - dBaseFromY );

		// Test のジオメトリごとのループ
		ipTestSegColCopy = AheGetClone( ipTestGeometry );
		if ( ipTestSegColCopy == NULL ) {
			_ASSERTE( ipTestSegColCopy != NULL );
			continue;
		}

		// Test のセグメントごとのループ
		lTestIndex = 0;
		ipTestSegColCopy->get_SegmentCount( &lTestSegColCount );
		ipTestSegColCopy->get_EnumSegments( &ipTestEnumSegment );
		ipTestEnumSegment->Reset();
		while( S_OK == ipTestEnumSegment->Next( &ipTestSegTmp, &lPartIndex, &lVertexIndex ) ) {
			dTestFromX = dTestFromY = dTestToX = dTestToY = 0.0f;

			// コピーを取る
			ipTestSeg = AheGetClone( (_ISegmentPtr)ipTestSegTmp );
			if( ipTestSeg == NULL ) {
				lTestIndex++;
				continue;
			}

			// ポイント初期化
			ipTestFromPoint->PutCoords( 0.0f, 0.0f );
			ipTestToPoint->PutCoords( 0.0f, 0.0f );

			if( FAILED( ipTestSeg->QueryFromPoint( ipTestFromPoint ) ) || FAILED( ipTestSeg->QueryToPoint( ipTestToPoint ) ) ) {
				ATLASSERT( FALSE );
				AheTraceErrorMessage( _T("GetCrossOrTouchPoints2()") );
				lTestIndex++;
				continue;
			}

			ipTestFromPoint->QueryCoords( &dTestFromX, &dTestFromY );
			ipTestToPoint->QueryCoords( &dTestToX, &dTestToY );

			ipTestSegPolyline->SetEmpty();
			((IPointCollectionPtr)ipTestSegPolyline)->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
			((IPointCollectionPtr)ipTestSegPolyline)->AddPoint( (IPointPtr)AheGetClone( ipTestToPoint ) );

			// セグメント単位で平行、接触、又は完全に離れているかどうかのチェック
			// Base を Test の FromTo で、Test を Base の FromTo でスプリットしてみて、できたら平行、接触ではない又は離れていない
			if( ipBaseSegPolyline != NULL && ipTestSegPolyline != NULL ) {
				// 端点同士が接しているかどうか
				BOOL bBaseFromIsTestFrom = AheIsEqual( ipBaseFromPoint, ipTestFromPoint );
				BOOL bBaseFromIsTestTo   = AheIsEqual( ipBaseFromPoint, ipTestToPoint );
				BOOL bBaseToIsTestFrom   = AheIsEqual( ipBaseToPoint, ipTestFromPoint );
				BOOL bBaseToIsTestTo     = AheIsEqual( ipBaseToPoint, ipTestToPoint );
				// 完全一致かどうか（完全一致の場合は Touch とみなす）
				if( bBaseFromIsTestFrom == TRUE && bBaseFromIsTestTo == TRUE && bBaseToIsTestFrom == TRUE && bBaseToIsTestTo == TRUE ) {
					if( enumSpRel & sindyeSpatialRelTouch ) {
						ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
						ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
						ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）一致：                         : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）一致：                         : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseToX, dBaseToY );
					}
					lTestIndex++;
					continue;	// 完全一致ならここで終わり
				}

				// 端点がセグメント上に存在しているかどうか
				VARIANT_BOOL vbBaseIsSplittedByTestFrom = ( AhePointIsOnSegment( ipBaseSeg, ipTestFromPoint, ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
				VARIANT_BOOL vbBaseIsSplittedByTestTo   = ( AhePointIsOnSegment( ipBaseSeg, ipTestToPoint,   ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
				VARIANT_BOOL vbTestIsSplittedByBaseFrom = ( AhePointIsOnSegment( ipTestSeg, ipBaseFromPoint, ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
				VARIANT_BOOL vbTestIsSplittedByBaseTo   = ( AhePointIsOnSegment( ipTestSeg, ipBaseToPoint,   ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

				// 平行かどうか（平行の場合は Touch とみなす）
				if( ( bBaseFromIsTestFrom == TRUE && ( vbBaseIsSplittedByTestTo   == VARIANT_TRUE || vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) ) ||
					( bBaseFromIsTestTo   == TRUE && ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) ) ||
					( bBaseToIsTestFrom   == TRUE && ( vbBaseIsSplittedByTestTo   == VARIANT_TRUE || vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) ) ||
					( bBaseToIsTestTo     == TRUE && ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) ) ||
					( bBaseFromIsTestFrom == FALSE && bBaseFromIsTestTo == FALSE && bBaseToIsTestFrom == FALSE && bBaseToIsTestTo == FALSE && 
					  ( ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE && vbBaseIsSplittedByTestTo == VARIANT_TRUE ) ||
					    ( vbTestIsSplittedByBaseFrom == VARIANT_TRUE && vbTestIsSplittedByBaseTo == VARIANT_TRUE ) ||
					    ( ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbBaseIsSplittedByTestTo == VARIANT_TRUE ) &&
					      ( vbTestIsSplittedByBaseFrom == VARIANT_TRUE || vbTestIsSplittedByBaseTo == VARIANT_TRUE ) ) ) ) ) {
					if( enumSpRel & sindyeSpatialRelTouch ) {
						// 平行の場合
						if( vbBaseIsSplittedByTestFrom == VARIANT_TRUE ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）平行：BaseIsSplittedByTestFrom : X=%.15lf Y=%.15lf S=-- T=--\n"), dTestFromX, dTestFromY );
						}

						if( vbBaseIsSplittedByTestTo == VARIANT_TRUE ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）平行：BaseIsSplittedByTestTo   : X=%.15lf Y=%.15lf S=-- T=--\n"), dTestToX, dTestToY );
						}

						if( vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）平行：TestIsSplittedByBaseFrom : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						}

						if( vbTestIsSplittedByBaseTo == VARIANT_TRUE ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）平行：TestIsSplittedByBaseTo   : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseToX, dBaseToY );
						}
					}
					lTestIndex++;
					continue;	// 平行ならここで終わり
				}

				// 一個前及び一個先の Base、Test のセグメントと From、To を取得
				if ( lBaseIndex > 0 ) {
					ipBaseSegColCopy->get_Segment( lBaseIndex + 1, &ipPrevBaseSeg );
					if ( ipPrevBaseSeg != NULL ) {
						ipPrevBaseSeg->get_FromPoint( &ipPrevBaseSegFromPoint );
						ipPrevBaseSeg->get_ToPoint( &ipPrevBaseSegToPoint );
					}
				}

				if( lBaseIndex < lBaseSegColCount - 1 ) {
					ipBaseSegColCopy->get_Segment( lBaseIndex - 1, &ipNextBaseSeg );
					if ( ipNextBaseSeg != NULL ) {
						ipNextBaseSeg->get_FromPoint( &ipNextBaseSegFromPoint );
						ipNextBaseSeg->get_ToPoint( &ipNextBaseSegToPoint );
					}
				}

				if( lTestIndex > 0 ) {
					ipTestSegColCopy->get_Segment( lTestIndex + 1, &ipPrevTestSeg );
					if ( ipPrevTestSeg != NULL ) {
						ipPrevTestSeg->get_FromPoint( &ipPrevTestSegFromPoint );
						ipPrevTestSeg->get_ToPoint( &ipPrevTestSegToPoint );
					}
				}

				if( lTestIndex < lTestSegColCount - 1 ) {
					ipTestSegColCopy->get_Segment( lTestIndex - 1, &ipNextTestSeg );
					if ( ipNextTestSeg != NULL ) {
						ipNextTestSeg->get_FromPoint( &ipNextTestSegFromPoint );
						ipNextTestSeg->get_ToPoint( &ipNextTestSegToPoint );
					}
				}

				// 端点同士が接しているが、一致でも平行でもない場合
				if( bBaseFromIsTestFrom == TRUE ) {
					// Base の From が Test の From と一致しているとき
					//
					// 1. Base の To           ... ipBaseToPoint
					// 2. Base の From         ... ipBaseFromPoint
					// 3. Base の一個前の From  ... ipPrevBaseSegFromPoint
					// 4. Test の To           ... ipTestToPoint
					// 5. Test の一個前の From  ... ipPrevTestSegFromPoint
					//
					// 123 のなす角の間に 124、125 の角度が両方あるまたは両方ない場合は Touch、片方があってもう片方がない場合は Cross
					if( ipPrevBaseSeg != NULL && ipPrevTestSeg != NULL ) {
						DOUBLE dAngle123 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipTestToPoint          ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevTestSegFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) {
							// クロスした場合
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						} else {
							// クロスしなかった場合は Touch とみなす（前のセグメントが接していた場合も Touch となる）
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						}
					} else {
						// Base 又は Test が開始又は最後のセグメントを指していた場合は Touch とする（問答無用）
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( bBaseFromIsTestTo == TRUE ) {
					// Base の From が Test の To と一致しているとき
					//
					// 1. Base の To           ... ipBaseToPoint
					// 2. Base の From         ... ipBaseFromPoint
					// 3. Base の一個前の From  ... ipPrevBaseSegFromPoint
					// 4. Test の From         ... ipTestFromPoint
					// 5. Test の一個先の To    ... ipNextTestSegToPoint
					//
					// 123 のなす角の間に 124、125 の角度が両方あるまたは両方ない場合は Touch、片方があってもう片方がない場合は Cross
					if( ipPrevBaseSeg != NULL && ipNextTestSeg != NULL ) {
						DOUBLE dAngle123 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipTestFromPoint        ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipNextTestSegToPoint   ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) {
							// クロスした場合
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						} else {
							// クロスしなかった場合は Touch とみなす（先のセグメントが接していた場合も Touch となる）
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						}
					} else {
						// Base 又は Test が開始又は最後のセグメントを指していた場合は Touch とする（問答無用）
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( bBaseToIsTestFrom == TRUE ) {
					// Base の To が Test の From と一致しているとき
					//
					// 1. Base の From         ... ipBaseFromPoint
					// 2. Base の To           ... ipBaseToPoint
					// 3. Base の一個先の To    ... ipNextBaseSegToPoint
					// 4. Test の To           ... ipTestToPoint
					// 5. Test の一個前の From  ... ipPrevTestSegFromPoint
					//
					// 123 のなす角の間に 124、125 の角度が両方あるまたは両方ない場合は Touch、片方があってもう片方がない場合は Cross
					if( ipNextBaseSeg != NULL && ipPrevTestSeg != NULL ) {
						DOUBLE dAngle123 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextBaseSegToPoint   ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipTestToPoint          ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipPrevTestSegFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) {
							// クロスした場合
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						} else {
							// クロスしなかった場合は Touch とみなす（前のセグメントが接していた場合も Touch となる）
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						}
					} else {
						// Base 又は Test が開始又は最後のセグメントを指していた場合は Touch とする（問答無用）
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( bBaseToIsTestTo == TRUE ) {
					// Base の To が Test の To と一致しているとき
					//
					// 1. Base の From         ... ipBaseFromPoint
					// 2. Base の To           ... ipBaseToPoint
					// 3. Base の一個先の To    ... ipNextBaseSegToPoint
					// 4. Test の From         ... ipTestFromPoint
					// 5. Test の一個先の To    ... ipNextTestSegToPoint
					//
					// 123 のなす角の間に 124、125 の角度が両方あるまたは両方ない場合は Touch、片方があってもう片方がない場合は Cross
					if( ipNextBaseSeg != NULL && ipNextTestSeg != NULL ) {
						DOUBLE dAngle123 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextBaseSegToPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipTestFromPoint      ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextTestSegToPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) {
							// クロスした場合
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						} else {
							// クロスしなかった場合は Touch とみなす（先のセグメントが接していた場合も Touch となる）
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						}
					} else {
						// Base 又は Test が開始又は最後のセグメントを指していた場合は Touch とする（問答無用）
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
						}
					}
					lTestIndex++;
					continue;
				}

				// 一致でも平行でもなく端点も接していない場合で、片方ののセグメント上にもう片方の From 又は To が存在するとき
				if( vbBaseIsSplittedByTestFrom == VARIANT_TRUE ) {
					// Base が Test の From でスプリットされたとき
					//
					// 1. Base の From か To （どちらでも良い）... ipBaseFromPoint
					// 2. Test の From                        ... ipTestFromPoint
					// 3. Test の To                          ... ipTestToPoint
					// 4. Test の一個前の From                ... ipPrevTestSegFromPoint
					//
					// 123 のなす角と 124 のなす角を取得し、どちらも正又は負の角度であれば Touch、どちらかが正でどちらかが負であれば Cross 
					// 一個前の Test を取得
					if( ipPrevTestSeg != NULL ) {
						DOUBLE dAngle1 = AheGetAngle( ipBaseFromPoint, ipTestFromPoint, ipTestToPoint );
						DOUBLE dAngle2 = AheGetAngle( ipBaseFromPoint, ipTestFromPoint, ipPrevTestSegFromPoint );
						if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) {
							// クロスした場合
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）端接：X=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
							}
						} else {
							// クロスしなかった場合は Touch とみなす（前のセグメントが接していた場合も Touch となる）
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
							}
						}
					} else {
						// Test が開始セグメントなので、 Touch とする（問答無用）
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( vbBaseIsSplittedByTestTo   == VARIANT_TRUE ) {
					// Base が Test の To でスプリットされたとき
					//
					// 1. Base の From か To （どちらでも良い）... ipBaseFromPoint
					// 2. Test の To                          ... ipTestToPoint
					// 3. Test の From                        ... ipTestFromPoint
					// 4. Test の一個先の To                  ... ipNextTestSegToPoint
					//
					// 123 のなす角と 124 のなす角を取得し、どちらも正又は負の角度であれば Touch、どちらかが正でどちらかが負であれば Cross 
					if( ipNextTestSeg != NULL ) {
						DOUBLE dAngle1 = AheGetAngle( ipBaseFromPoint, ipTestToPoint, ipTestFromPoint );
						DOUBLE dAngle2 = AheGetAngle( ipBaseFromPoint, ipTestToPoint, ipNextTestSegToPoint );
						if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) {
							// クロスした場合
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）端接：X=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
							}
						} else {
							// クロスしなかった場合は Touch とみなす（先のセグメントが接していた場合も Touch となる）
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
							}
						}
					} else {
						// Test の最後のセグメントの場合は Touch とする（問答無用）
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( vbTestIsSplittedByBaseFrom == VARIANT_TRUE )  {
					// Test が Base の From でスプリットされたとき
					//
					// 1. Test の From か To （どちらでも良い）... ipTestFromPoint
					// 2. Base の From                        ... ipBaseFromPoint
					// 3. Base の To                          ... ipBaseToPoint
					// 4. Base の一個前の From                ... ipPrevBaseSegFromPoint
					//
					// 123 のなす角と 124 のなす角を取得し、どちらも正又は負の角度であれば Touch、どちらかが正でどちらかが負であれば Cross 
					// 一個前の Base を取得
					if( ipPrevBaseSeg != NULL ) {
						DOUBLE dAngle1 = AheGetAngle( ipTestFromPoint, ipBaseFromPoint, ipBaseToPoint );
						DOUBLE dAngle2 = AheGetAngle( ipTestFromPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint );
						if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) {
							// クロスした場合
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						} else {
							// クロスしなかった場合は Touch とみなす（前のセグメントが接していた場合も Touch となる）
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						}
					} else {
						// j が 0 の場合は開始セグメントなので、 Touch とする（問答無用）
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( vbTestIsSplittedByBaseTo   == VARIANT_TRUE )  {
					// Test が Base の To でスプリットされたとき
					//
					// 1. Test の From か To （どちらでも良い）... ipTestFromPoint
					// 2. Base の To                          ... ipBaseToPoint
					// 3. Base の From                        ... ipBaseFromPoint
					// 4. Base の一個先の To                  ... ipNextBaseSegToPoint
					//
					// 123 のなす角と 124 のなす角を取得し、どちらも正又は負の角度であれば Touch、どちらかが正でどちらかが負であれば Cross 
					// 一つ先の Base を取得
					if( ipNextBaseSeg != NULL ) {
						DOUBLE dAngle1 = AheGetAngle( ipTestFromPoint, ipBaseToPoint, ipBaseFromPoint );
						DOUBLE dAngle2 = AheGetAngle( ipTestFromPoint, ipBaseToPoint, ipNextBaseSegToPoint );
						if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) {
							// クロスした場合
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						} else {
							// クロスしなかった場合は Touch とみなす（先のセグメントが接していた場合も Touch となる）
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						}
					} else {
						// i が Base の最後のセグメントを指していた場合は Touch とする（問答無用）
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
						}
					}
					lTestIndex++;
					continue;
				}
			}

			DOUBLE Bx = (DOUBLE)( dTestToX - dTestFromX );
			DOUBLE By = (DOUBLE)( dTestToY - dTestFromY );
			DOUBLE DivideBase = (DOUBLE)( (DOUBLE)( Bx * Ay ) - (DOUBLE)( By * Ax ) );
			if( DivideBase == 0.0f )  {
				ATLTRACE(_T("GetCrossOrTouchPoints2() : 平行なので飛ばします。") );
				lTestIndex++;
				continue;	// 平行
			}

			DOUBLE t = (DOUBLE)( ( (DOUBLE)(Ax * (DOUBLE)( dTestFromY - dBaseFromY )) - (DOUBLE)(Ay * (DOUBLE)( dTestFromX - dBaseFromX )) ) / DivideBase );
			if( 0.0f < t && t < 1.0f ) {
				DOUBLE s = (DOUBLE)( ( (DOUBLE)(Bx * (DOUBLE)( dBaseFromY - dTestFromY )) - (DOUBLE)(By * (DOUBLE)( dBaseFromX - dTestFromX )) ) / (-DivideBase) );
				if( 0.0f < s && s < 1.0f ) {
					BOOL bAdd = FALSE;
					DOUBLE Px = (DOUBLE)( dBaseFromX + (DOUBLE)(s * Ax) );
					DOUBLE Py = (DOUBLE)( dBaseFromY + (DOUBLE)(s * Ay) );
					if( enumSpRel & sindyeSpatialRelCross ) {
						ATLTRACE(_T("GetCrossOrTouchPoints2() : 交点（CROSS）発見：X=%lf Y=%lf S=%lf T=%lf D=%.20lf\n"), Px, Py, s, t, DivideBase );
						bAdd = TRUE;
					}
					if( bAdd )
					{
						if ( ! ipIntersectPoints )
						{
							ipIntersectPoints.CreateInstance( CLSID_Multipoint );
							((IGeometryPtr)ipIntersectPoints)->putref_SpatialReference( ipBaseSpRef ); // ベースの空間参照付与
						}
						IPointPtr ipPoint( CLSID_Point );
						((IGeometryPtr)ipIntersectPoints)->putref_SpatialReference( ipBaseSpRef ); // ベースの空間参照付与
						ipPoint->PutCoords( Px, Py );
						ipIntersectPoints->AddPoint( ipPoint );
					}
				}
			}
			lTestIndex++;
		}
		lBaseIndex++;
	}

	// 一つもなければ NULL を返す
	if ( ipIntersectPoints != NULL ) {
		LONG 		lIntersectCount = -1;

		ipIntersectPoints->get_PointCount( &lIntersectCount );
		if ( lIntersectCount < 1 )
			ipIntersectPoints = NULL;
	}

	return ipIntersectPoints;
}

BOOL CEditHelper::PointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef/* = NULL*/, BOOL bExeptVertex/* = FALSE*/ )
{
	if( ipBaseGeometry == NULL || ipTestPoint == NULL )
		return FALSE;

	if( ipSpRef == NULL )
		ipBaseGeometry->get_SpatialReference( &ipSpRef );

	esriGeometryType geomType;

	ipBaseGeometry->get_GeometryType( &geomType );
	switch( geomType ) {
		case esriGeometryPoint:
			return AheIsEqual( ipBaseGeometry, ipTestPoint );
			break;
		default:
		{
			IGeometryCollectionPtr 	ipGeomCol( ipBaseGeometry );
			ISegmentCollectionPtr 	ipSegCol;
			_ISegmentPtr			ipSegment;
			IEnumSegmentPtr			ipEnumSegment;
			IGeometryPtr 			ipPartGeom;
			LONG 					lGeomCount, lPartIndex, lVertexIndex;

			if( ipGeomCol == NULL )
				return FALSE;

			ipGeomCol->get_GeometryCount( &lGeomCount );
			for ( LONG i = 0; i < lGeomCount; i++ ) {
				ipGeomCol->get_Geometry( i, &ipPartGeom );
				ipSegCol = ipPartGeom;
				if( ipSegCol == NULL ) {
					ATLASSERT( ipSegCol != NULL );
					continue;
				}

				ipSegCol->get_EnumSegments( &ipEnumSegment );
				while( S_OK == ipEnumSegment->Next( &ipSegment, &lPartIndex, &lVertexIndex ) ) {
					if( AhePointIsOnSegment( ipSegment, ipTestPoint, ipSpRef, bExeptVertex ) == TRUE )
						return TRUE;
				}
			}
		}
		break;
	}

	return FALSE;
}


BOOL CEditHelper::GetSplitResult(
	LPCTSTR lpcszSplitLayers,
	const IGeometryPtr& ipGeom,
	CTargetItems& cNoSetModifyDataFeatures,
	CTargetItems& cModifyNodeKindFeatures,
	const IPointPtr& ipTakeOverPoint,
	INT iSplitMode,
	CTargetItems& cTakeOveredFeatures,
	CTargetItems* pReferenceNodes,
	CTargetItems& cNewFeatures,
	CTargetItems& cTargetFeatures,
	std::list<ROWDEF>& listLQRows)
{
	/// ネットワーク系以外のフィーチャクラスを表す識別子(構成点を作るべきポイントを保持するクラスの識別用)
	const _TCHAR OTHER_FEATURECLASS[] = _T("__OTHER__");

	// ネットワークモデルごとのノードを作るべきポイントの集合
	// <リンクのフィーチャクラス名, ノードを作るべきポイント>
	std::map<const CString, IPointCollectionPtr> createNodePoints;

	{
		// ネットワークモデル以外のフィーチャ（何でも良い）を保持しておく
		IFeaturePtr nwOtherFeature;

		// ノードの空間参照をセット
		// セットしないと既存ノードとのEqual判定に失敗する
		for( CTargetItems::const_iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
		{
			if( !AheIsNetwork(it->GetFeature()) )
			{
				// 空間参照をセットするのに使用するために、ネットワークモデル以外のフィーチャを1つだけ保持しておく
				if(!nwOtherFeature)
					nwOtherFeature = it->GetFeature();
				continue;
			}
			CString nwFCName = AheGetNetworkLinkFeatureClassName(it->GetFeatureClassName());
			if( createNodePoints.find(nwFCName)!=createNodePoints.end() ) continue;
			createNodePoints[nwFCName] = IPointCollectionPtr( CLSID_Multipoint );
			((IGeometryPtr)createNodePoints[nwFCName])->putref_SpatialReference( AheGetSpatialReference( it->GetFeature() ) );
		}

		if(nwOtherFeature)
		{
			// ネットワーク系以外は全て同様に扱う
			createNodePoints[OTHER_FEATURECLASS] = IPointCollectionPtr( CLSID_Multipoint );
			((IGeometryPtr)createNodePoints[OTHER_FEATURECLASS])->putref_SpatialReference( AheGetSpatialReference( nwOtherFeature ) );
		}
	}

	esriGeometryType enumSplitterGeomType;
	ipGeom->get_GeometryType( &enumSplitterGeomType );

#ifdef _USE_SETLQATTRIBUTE
	std::list<LQRFINFO> listLQRFInfo;	// [bug 2641] 道路リンクのLQRFは最後にまとめて付与しないと現在のアルゴリズムでは整合性が取れない
#endif // ifdef _USE_SETLQATTRIBUTE

	if(FALSE==SplitFeatures(lpcszSplitLayers,	OTHER_FEATURECLASS, ipGeom, cNoSetModifyDataFeatures, ipTakeOverPoint, iSplitMode,
		listLQRFInfo, listLQRows, cTakeOveredFeatures, cNewFeatures, cTargetFeatures, createNodePoints))
		return FALSE;

	if(FALSE==SplitNetworkLink(ipGeom, cNewFeatures, cTargetFeatures, cNoSetModifyDataFeatures, cModifyNodeKindFeatures,
		listLQRFInfo, createNodePoints, cTakeOveredFeatures, pReferenceNodes))
		return FALSE;

	return TRUE;
}

BOOL CEditHelper::SplitFeatures(
	LPCTSTR lpcszSplitLayers,
	LPCTSTR strOthersName,
	const IGeometryPtr& ipGeom,
	const CTargetItems& cNoSetModifyDataFeatures,
	const IPointPtr& ipTakeOverPoint,
	INT iSplitMode,
	std::list<LQRFINFO>& listLQRFInfo,
	std::list<ROWDEF>& listLQRows,
	CTargetItems& cTakeOveredFeatures,
	CTargetItems& cNewFeatures,
	const CTargetItems& cTargetFeatures,
	std::map<const CString, IPointCollectionPtr>& createNodePoints)
{
	CString strSplitLayers( lpcszSplitLayers );
	for( CTargetItems::const_iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
	{
		IFeaturePtr ipFeature( it->GetFeature() );

		// 分割対象フィーチャかどうかをチェック
		CString strClassName, strLayerName;
		strLayerName = it->GetFeatureClassName();
		strClassName.Format(_T("|%s|"), strLayerName );
		ILayerPtr ipLayer( GetLayerByTableName( strLayerName ) );

		// 分割対象でないなら何もしない
		if( strSplitLayers.Find( strClassName ) < 0  ) continue;

		// どのNWモデルか
		const sindyeNetworkModel eNWModel = AheGetNetWorkType(ipFeature);
		// ネットワークであればフィーチャクラス名をキーに、そうでなければOTHER_FEATURECLASSをキーに
		// ノード、あるいは、構成点を追加すべきポイントクラスの参照をとる
		IPointCollectionPtr& NodePoints = ( AheIsNetworkLink(ipFeature) )? createNodePoints[strLayerName] : createNodePoints[strOthersName];

		IGeometryPtr ipTargetGeom = AheSetKnownSimple( it->GetShapeCopy() );

		// 分割後の形状取得
		_ISetPtr ipSplittedGeometrySet = SplitGeometryByGeometry( ipTargetGeom, ipGeom, iSplitMode, AheIsNetworkLink( strLayerName ) ? TRUE : FALSE, NodePoints );
		if( !ipSplittedGeometrySet ) continue;

		// 分割できなかったら continue
		LONG lSplittedGeometryCount = -1;	// 分割後の形状の数保持用
		ipSplittedGeometrySet->get_Count( &lSplittedGeometryCount );
		if( lSplittedGeometryCount < 1 ) continue;

		// どのジオメトリが元のフィーチャを引き継ぐか決める
		LONG lTakeOverGeomNum = 0;
		if( ipTakeOverPoint )
		{
			ISpatialReferencePtr ipSpRef( AheGetSpatialReference( ipTakeOverPoint ) );	// 空間参照をあわせるために取得（[bug 3253]）

			ipSplittedGeometrySet->Reset();
			DOUBLE dMinDistance = -1.0;
			IProximityOperatorPtr ipProxOpe( ipTakeOverPoint );
			for( LONG i = 0; i < lSplittedGeometryCount; i++ )
			{
				IUnknownPtr ipUnk;
				ipSplittedGeometrySet->Next( &ipUnk );
				if( !ipUnk ) break;
				IGeometryPtr ipTakeOverGeom( ipUnk );
				if( !ipTakeOverGeom ) continue;
/*#ifdef DEBUG
				AheAddPolygonElement( m_cArcHelper.GetGraphicsContainer(), ipGeom, RGB(0,255,0), esriSFSSolid );
#endif*/
				// 距離取得
				DOUBLE dMinDistanceTmp = -1.0;
				ipTakeOverGeom->putref_SpatialReference( ipSpRef );	// [bug 3253] 比較対照と比較元の空間参照が一致していないとエラーになる！！
				if( SUCCEEDED( ipProxOpe->ReturnDistance( ipTakeOverGeom, &dMinDistanceTmp ) ) )
				{
					if( dMinDistance < 0 || ( dMinDistance > dMinDistanceTmp && dMinDistance > 0 ) )
					{
						// 最小の距離を持つものを引き継ぎ先とする
						dMinDistance = dMinDistanceTmp;
						lTakeOverGeomNum = i;
					}
				}
				else {
					CString strMsg;
					strMsg.Format(MESSAGE_FEATURE_IS_RANDOM, AheGetErrorMessage() );
					AheMessageBox( strMsg, WARN_SPLIT, MB_OK|MB_ICONWARNING );
				}
			}
		}
		CTargetItems cSplittedRoadFeatures;	// 道路リンクに関連するリンク列の処理のため

		// 分割開始
		IGeometryPtr ipBeforeSplitGeomCopy;	// スプリット前の形状を記憶（そうしないとリンク列の処理で進まなくなる）
		ipSplittedGeometrySet->Reset();
		for( LONG i = 0; i < lSplittedGeometryCount; ++i )
		{
			IUnknownPtr ipUnk;
			ipSplittedGeometrySet->Next( &ipUnk );
			if( !ipUnk ) break;
			IGeometryPtr ipGeom( ipUnk );
			if( !ipGeom ) continue;

			if( i == lTakeOverGeomNum )
			{
				// 形状確定
				if( AheIsEditableFeature2( m_piMap, ipFeature, NULL, sindyeEditMove, GetRule(), FALSE, GetHWND() ) == FALSE ) 
					return FALSE;

				// スプリット前の形状記憶
				if( sindyeNetworkRoad == eNWModel)
					ipFeature->get_ShapeCopy( &ipBeforeSplitGeomCopy );	

#ifdef _DEBUG_FOR_2059	// [bug 2059] 調査用
				{
					CString strMsg;
					strMsg.Format(_T("分割形状のトレース[%s:%d:%s]"), it->GetFeatureClassName(), it->GetOID(), _T("既存") );
					AheTraceGeometry( ipGeom, strMsg );
				}
#endif // ifdef _DEBUG_FOR_2059

				//ipFeature->putref_Shape( ipGeom );
				AheSetShape( ipFeature, ipGeom );

#ifdef _DEBUG_FOR_2059	// [bug 2059] 調査用
				{
					CString strMsg;
					IGeometryPtr ipGeomTmp;
					ipFeature->get_ShapeCopy( &ipGeomTmp );
					strMsg.Format(_T("分割形状のトレース（セット後）[%s:%d:%s]"), it->GetFeatureClassName(), it->GetOID(), _T("既存") );
					AheTraceGeometry( ipGeomTmp, strMsg );

					//さらに、Simplifyした場合の変化をみてほしー
					IClonePtr ipClone( ipGeomTmp ), ipCopy;
					ipClone->Clone( &ipCopy );
					ITopologicalOperatorPtr ipTopoTmp( ipCopy );
					ipTopoTmp->Simplify();
					strMsg.Format(_T("分割形状のトレース（セット後、Simplify）[%s:%d:%s]"), it->GetFeatureClassName(), it->GetOID(), _T("既存") );
					AheTraceGeometry( (IGeometryPtr)ipTopoTmp, strMsg );
				}
#endif // ifdef _DEBUG_FOR_2059
				if( cNoSetModifyDataFeatures.IsAlreadyExist( *it ) == FALSE )
					AheSetModifyData( ipFeature, sindyUpdateTypeShape );	// cNoSetModifyDataFeatures に入っていないもののみ形状修正の履歴をつける
				cTakeOveredFeatures.push_back( ipFeature );	// IDを引き継いだものを保持

				// スプリット直後に関連テーブル等の修正をする必要があるので一時格納
				cSplittedRoadFeatures.push_back( ipFeature );
			}
			// それ以外は新規に作成
			else 
			{
				if( AheIsCreatableLayer( AheGetFeatureClass( ipFeature ), ipGeom, TRUE ) == FALSE )
				{
					CString strMsg;
					strMsg.Format(ERROR_CANNOT_CONTINUE_SPLIT, strLayerName );
					AheMessageBox( strMsg, ERROR_FAILED_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
					return FALSE;	// 作成禁止
				}
/*						//////////////////////////////////////////////
				// フィーチャ作成（高速化のために Insert を使用）
				//////////////////////////////////////////////
//					IFeatureCursorPtr ipInsertCursor;
				IFeatureClassPtr ipCreateClass( AheGetFeatureClass( ipFeature ) );
//						ipCreateClass->Insert( FALSE, &ipInsertCursor );
				IFeaturePtr ipNewFeature = AheCreateRowBuffer( ipCreateClass, ipGeom );*/
#ifdef _DEBUG_FOR_2059	// [bug 2059] 調査用
				{
					CString strMsg;
					strMsg.Format(_T("分割形状のトレース[%s:%d:%s]"), it->GetFeatureClassName(), -1, _T("新規") );
					AheTraceGeometry( ipGeom, strMsg );
				}
#endif // ifdef _DEBUG_FOR_2059
				IFeaturePtr ipNewFeature = AheCreateRow( ipFeature, ipGeom );
#ifdef _DEBUG_FOR_2059	// [bug 2059] 調査用
				{
					CString strMsg;
					long lOIDTmp;
					ipNewFeature->get_OID( &lOIDTmp );
					strMsg.Format(_T("分割形状のトレース（セット後）[%s:%d:%s]"), it->GetFeatureClassName(), lOIDTmp, _T("新規") );
					AheTraceGeometry( ipGeom, strMsg );

					//さらに、Simplifyした場合の変化をみてほしー
					IClonePtr ipClone( ipGeom ), ipCopy;
					ipClone->Clone( &ipCopy );
					ITopologicalOperatorPtr ipTopoTmp( ipCopy );
					ipTopoTmp->Simplify();
					strMsg.Format(_T("分割形状のトレース（セット後、Simplify）[%s:%d:%s]"), it->GetFeatureClassName(), lOIDTmp, _T("新規") );
					AheTraceGeometry( (IGeometryPtr)ipTopoTmp, strMsg );
				}
#endif // ifdef _DEBUG_FOR_2059
				// 属性コピー
				AheCopyAttribute( ipFeature, ipNewFeature, TRUE );	// [bug 4043] 分割モードでコピー

				////////////////////////////////////////////////////
				// [bug4497]LINK_IDだけが変わる時に更新履歴が付かない対策
				// 新規作成と属性変更を区別するためリンク内属性と傾斜の場合は「LINK_ID=0」にする
				// 「LINK_ID」は後で振り直すので大丈夫、、、のはず…
				if( AheGetFeatureClassName( ipFeature ) == _T("SEGMENT_ATTR") || AheGetFeatureClassName( ipFeature ) == _T("GRAD") ){
					AheSetAttribute( ipNewFeature, _T("LINK_ID"), CComVariant(0L) );
				}
				////////////////////////////////////////////////////

				// 履歴
				AheSetModifyData( ipNewFeature, sindyUpdateTypeCreate );
/*					// インサート
				CComVariant vaID;
				ipInsertCursor->InsertFeature( (IFeatureBufferPtr)ipNewFeature, &vaID ); <- 後で構成点の追加をするのでまだインサートしない*/
/*						// 作成されたフィーチャには OBJECTID がついてないので、引きなおし（トホホ）
				ipCreateClass->GetFeature( vaID.lVal, &ipNewFeature );*/
				/////////////////////////////////////////////// ここまで
//						cNewFeatures.push_back( ipNewFeature, -1 );
				cNewFeatures.push_back( ipNewFeature );

				// スプリット直後に関連テーブル等の修正をする必要があるので一時格納
				cSplittedRoadFeatures.push_back( ipNewFeature );
			}
		}

		// TODO 特定のネットワークモデルに対して特別な処理を行っている
		// この方法には以下の問題点がある
		// ・関連テーブルを持ったネットワークモデルが新たに定義される度に、ここに条件を追加しなければならない
		// ・リンク列の処理の中でg_cRuleLinkQueueを参照していない
		// ・ネットワークモデル以外に関連テーブルをもつものができた場合に、分割時にレコードがコピーされない
		switch(eNWModel)
		{
		// 道路リンクをスプリットした場合
		case sindyeNetworkRoad:
#ifdef _USE_SETLQATTRIBUTE
			// リンク列の修正をここでする
			if( SetLQAttribute( it->GetOID(), ipBeforeSplitGeomCopy, cSplittedRoadFeatures, listLQRFInfo, listLQRows ) == FALSE )
				return FALSE;
#else
			if( ! CreateLQForSplittedRoad( *it, cPlittedRoadFeatures, listLQRows ) )
				return FALSE;
#endif // ifdef _USE_SETLQATTRIBUTE
			// 時間規制があったかどうか調べ、あるのなら時間規制を新しく作成
			if( CopyRelationAttribute( it->GetFeature(), cSplittedRoadFeatures ) == FALSE )
				return FALSE;
			break;
		// 鉄道・歩道をスプリットした場合
		case sindyeNetworkRailway:
		case sindyeNetworkWalk:
			// 関連テーブルを複製
			if( CopyRelationAttribute( it->GetFeature(), cSplittedRoadFeatures ) == FALSE )
				return FALSE;
			break;
		default:
			break;
		}
	}
	return TRUE;
}

BOOL CEditHelper::SplitNetworkLink(
	const IGeometryPtr& ipGeom,
	CTargetItems& cNewFeatures,
	CTargetItems& cTargetFeatures,
	CTargetItems& cNoSetModifyDataFeatures,
	CTargetItems& cModifyNodeKindFeatures,
	const std::list<LQRFINFO>& listLQRFInfo,
	const std::map<const CString, IPointCollectionPtr>& createNodePoints,
	CTargetItems& cTakeOveredFeatures,
	CTargetItems* pReferenceNodes) const
{
	// ネットワーク系(道路NWなど、リンクとノードが同時に生成しなければならない類のフィーチャクラス)専用処理（スプリット処理が走った場合のみならず）
	if( cTargetFeatures.IsIncludedNetworkItems() == FALSE ) return TRUE;

	for( std::map<const CString, IPointCollectionPtr>::const_iterator nodePointIte = createNodePoints.begin(); nodePointIte!=createNodePoints.end(); ++nodePointIte )
	{
		// 対象の全てのネットワーク格納用
		// 全てのネットワーク ＝ cTargetFeatures のネットワーク ＋ cNewFeatures のネットワーク
		CTargetItems targetItems;
		cTargetFeatures.GetFeature( targetItems, nodePointIte->first );
		cNewFeatures.GetFeature( targetItems, nodePointIte->first, TRUE );

		// 処理中のリンクに対応付くノードのフィーチャクラス名を取得
		CString nodeFCName = AheGetNetworkNodeFeatureClassName(nodePointIte->first);

		if( !targetItems.empty() )
		{
#ifdef _USE_SETLQATTRIBUTE
			// ネットワーク系の中でLQテーブルと関係があるのは道路リンクだけなので道路リンク以外は処理しない
			if( sindyeNetworkRoad==targetItems.GetNetworkType() )
			{
				// [bug 2641] LQRFはここで付与
				for( std::list<LQRFINFO>::const_iterator itLQRF = listLQRFInfo.begin(); itLQRF != listLQRFInfo.end(); ++itLQRF )
				{
					CTargetItems::iterator itRoad = targetItems.begin();
					for(; itRoad != targetItems.end(); ++itRoad)
					{
						// 処理中のLQテーブルのリンクIDとオブジェクトIDが一致しないなら飛ばす
						if(	itRoad->GetOID() != itLQRF->oid ) continue;

						// 上記の条件を全て満たすものを見つけたら リンク列の情報を設定して break する
						for( std::map<sindyeRoadQueueType,bool>::const_iterator itLQRFInfo = itLQRF->LQRF.begin(); itLQRFInfo != itLQRF->LQRF.end(); ++itLQRFInfo )
						{
							itRoad->SetAttribute( m_cLoadQueueInfo.GetLoadQueueInfo( itLQRFInfo->first, sindyeLoadQueueInfoLQRFRoadFieldName ), ( itLQRFInfo->second ) ? 1L : 0L );
							ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : テーブル[ROAD_LINK] OID[%d] [%s]=%d\n"),
								itRoad->GetOID(), m_cLoadQueueInfo.GetLoadQueueInfo( itLQRFInfo->first, sindyeLoadQueueInfoLQRFRoadFieldName ), ( itLQRFInfo->second ) ? 1 : 0 );
						}
						break;
					}
					if( itRoad == targetItems.end() )
					{
						// ここに入ってくる場合は、cAllRoadFeaturesに追加する道路リンクが足りていないことを意味する
						ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : LQRFを付与するための道路リンクが見つからない OID[%d]\n"), itLQRF->oid );
						_ASSERTE( itRoad != targetItems.end() );
						AheMessageBox( ERROR_LINK_NOT_FOUND, _T("CEditHelper::SplitFeaturesByGeometry()"), MB_OK|MB_ICONSTOP );
						return FALSE;
					}
				}
			}
#endif // ifdef _USE_SETLQATTRIBUTE
		}

		IPointCollectionPtr ipCreateNodePoints(nodePointIte->second);
		CTargetItems cExistNodes; // 全ネットワークのエンベロープで検索したノードが入る（FROM、TO はこれ＋ cExistOrNewNodes をみて書き換える）
		cExistNodes.clear();

		// 関連するノードを取得（FROM、TO付け替え及びノード作成の判断に使用）
		IFeatureClassPtr ipNodeClass( targetItems.GetNetworkNodeFeatureClass() );

#ifdef DEBUG
		// UNION する前の形状をハイライト
		ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ユニオンする前の道路リンクハイライト（黄色）\n") );
/*			// エレメント作成（再描画しても消えないように）
		IGraphicsContainerPtr ipContainer;
		m_cArcHelper.GetActiveView(FALSE)->get_GraphicsContainer( &ipContainer );*/

		for( CTargetItems::iterator itTmp = targetItems.begin(); itTmp != targetItems.end(); ++itTmp )
			//IElementPtr ipElement( AheAddLineElement( ipContainer, itTmp->GetShape(), RGB(255,255,0), 2, esriSLSSolid ) );
			AheDrawPolyline( GetFocusMapScreenDisplay(), itTmp->GetShape(), RGB(255,255,0), 2, esriSLSSolid, TRUE );
#endif
		// エンベロープではでかすぎる（無駄が多い）ので、ユニオンで検索
//				IGeometryPtr ipSearchGeom( cAllRoadFeatures.GetUnion() ); <- UNIONすると、1Unitしかないパートは消えてしまうので注意（未報告）！！
		IGeometryPtr ipSearchGeom( TargetItems_MergeGeometry( targetItems, false ) );
		if( ipSearchGeom )
		{
#ifdef DEBUG
			TargetItems_Trace( targetItems, _T("ノード検索の為にUNIONする道路リンク") );
			// 既存ノードを選択するための道路リンクのユニオン形状ハイライト
			//IElementPtr ipElement( AheAddLineElement( ipContainer, ipSearchGeom, RGB(0,255,0), 2, esriSLSSolid ) );
			AheDrawPolyline( GetFocusMapScreenDisplay(), ipSearchGeom, RGB(0,255,0), 2, esriSLSSolid, TRUE );
			ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : 道路リンクのUNION完了、ハイライト（緑色）\n") );
			//AheTraceGeometry( ipSearchGeom, _T("CEditHelper::SplitFeaturesByGeometry()") );
#endif
			((ITopologicalOperatorPtr)ipSearchGeom)->Simplify();
			AheSelectByShapeFromFeatureClass( ipSearchGeom, cExistNodes, ipNodeClass, esriSpatialRelIntersects );
#ifdef DEBUG
			// 選択できた既存ノードハイライト
			ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : 道路リンクのUNION形状でのノード選択終了、ハイライト（水色）\n") );
			for( CTargetItems::iterator itTmp = cExistNodes.begin(); itTmp != cExistNodes.end(); ++itTmp )
			{
				//ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : CExistNodes : OID=%d\n"), itTmp->GetOID() );
				AheDrawPoint( GetFocusMapScreenDisplay(), itTmp->GetShape(), FALSE, 8, 128, 255, 255, esriSMSCircle, FALSE, TRUE );
			}
			TargetItems_Trace( cExistNodes, _T("選択できたノード") );
			IElementPtr ipElement( CLSID_LineElement );
			ipElement->put_Geometry( ipSearchGeom );
			((IGraphicsContainerPtr)GetMap())->AddElement( ipElement, 0 );
#endif
			// [bug 1965] 形状チェック時の2叉路角度判定で使用する
			if( pReferenceNodes ) *pReferenceNodes = cExistNodes;
		}
//#if 0
		if ( ipNodeClass )	// [Bug 7848]リンク内属性を分割しようとすると落ちる 
		{
			std::list<long> listConnectNodeIds;

			for( CTargetItems::iterator itRoads1 = targetItems.begin(); itRoads1 != targetItems.end(); ++itRoads1 )
			{
				CComVariant vaFromNodeID = itRoads1->GetAttribute( FROM_FIELD );
				CComVariant vaToNodeID   = itRoads1->GetAttribute( TO_FIELD );

				listConnectNodeIds.push_back( vaFromNodeID.lVal );
				listConnectNodeIds.push_back( vaToNodeID.lVal );
			}

			listConnectNodeIds.sort();
			listConnectNodeIds.unique();

			CString strIds;
			for( std::list<long>::const_iterator itId = listConnectNodeIds.begin(); itId != listConnectNodeIds.end(); ++itId )
			{
				CString strTmp;
				strTmp.Format( _T("%d"), *itId );

				if ( ! strIds.IsEmpty() )
					strIds += _T(",");

				strIds += strTmp;
			}

			IQueryFilterPtr ipQuery( CLSID_QueryFilter );
			ipQuery->put_SubFields( CComBSTR( _T("*") ) );

			CString strWhere;
			strWhere.Format( _T("OBJECTID IN (%s)"), strIds );
			ipQuery->put_WhereClause( CComBSTR( strWhere ) );

			IFeatureCursorPtr ipFeatureCursor;
			ipNodeClass->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );

			if( ipFeatureCursor )
			{
				IGeoDatasetPtr ipDataset( ipNodeClass );
				ISpatialReferencePtr ipDatasetSpRef;

				ipDataset->get_SpatialReference( &ipDatasetSpRef );

				IFeaturePtr ipFeature;
				while( SUCCEEDED( ipFeatureCursor->NextFeature( &ipFeature ) ) && ipFeature )
				{
					IGeometryPtr ipGeom;
					ISpatialReferencePtr ipSpRef;
					ipFeature->get_ShapeCopy( &ipGeom );
					ipGeom->get_SpatialReference( &ipSpRef );

					VARIANT_BOOL vaIsSameSpRef = VARIANT_FALSE;
					ipSpRef->IsPrecisionEqual( ipDatasetSpRef, &vaIsSameSpRef );
					if( ! vaIsSameSpRef )
					{
						ATLTRACE(_T("AheSelectByShapeFromLayer : フィーチャクラスの空間参照とフィーチャが持つジオメトリの空間参照が異なります。フィーチャクラスの空間参照に一致させます。\n") );
						AheSnapToSpatialReference( ipGeom, ipDatasetSpRef );
					}

					cExistNodes.push_back( ipFeature );
				}

#ifdef DEBUG
				// 選択できた既存ノードハイライト
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : 道路リンクのUNION形状でのノード選択終了、ハイライト（水色）\n") );
				for( CTargetItems::iterator itTmp = cExistNodes.begin(); itTmp != cExistNodes.end(); ++itTmp )
				{
					//ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : CExistNodes : OID=%d\n"), itTmp->GetOID() );
					AheDrawPoint( GetFocusMapScreenDisplay(), itTmp->GetShape(), FALSE, 8, 128, 255, 255, esriSMSCircle, FALSE, TRUE );
				}
				TargetItems_Trace( cExistNodes, _T("選択できたノード") );
				IElementPtr ipElement( CLSID_LineElement );
				ipElement->put_Geometry( ipSearchGeom );
				((IGraphicsContainerPtr)GetMap())->AddElement( ipElement, 0 );
#endif
				// [bug 1965] 形状チェック時の2叉路角度判定で使用する
				if( pReferenceNodes ) *pReferenceNodes = cExistNodes;
			}
		}
//#endif	// if 0

		// ノードを作らなければいけない場合はノード作成
		LONG lCreateNodePointCount = -1;
		ipCreateNodePoints->get_PointCount( &lCreateNodePointCount );
		if( lCreateNodePointCount > 0 )
		{
			// 比較対象ノードをまとめる
			// ノードを作成したらcAllNodesにもpush_backしなければならないことに注意！！
			CTargetItems cAllNodes( cExistNodes );
			cNewFeatures.GetFeature( cAllNodes, nodeFCName, TRUE );
			cTargetFeatures.GetFeature( cAllNodes, nodeFCName, TRUE );
			for( LONG i = 0; i < lCreateNodePointCount; i++ )
			{
				IPointPtr ipCreateNodePoint;
				ipCreateNodePoints->get_Point( i, &ipCreateNodePoint );

				BOOL bFindExistNode = FALSE;
				// 既存ノードに既に存在するかどうかチェック
				for( CTargetItems::iterator it = cAllNodes.begin(); it != cAllNodes.end(); ++it )
				{
					// ipCreateNodePointはまだ空間参照にあっていない形状なので、
					// ここでは1Unitなら許容してくれるIClone::Equalsを使用したほうが良い（bug 5807）
					if( AheIsEqual( it->GetShape(), ipCreateNodePoint ) )
					{
						bFindExistNode = TRUE;
						break;
					}
				}
				if( bFindExistNode ) continue;

				// ノード作成
				if( ! AheIsCreatableLayer( ipNodeClass, ipCreateNodePoint, TRUE ) )
				{
					CString strMsg;
					strMsg.Format(ERROR_CANNOT_CONTINUE_SPLIT, nodeFCName );
					AheMessageBox( strMsg, ERROR_FAILED_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
					return FALSE;	// 作成禁止
				}
				IFeaturePtr ipNewFeature = AheCreateRow( ipNodeClass, ipCreateNodePoint, TRUE );
				// 履歴
				AheSetModifyData( ipNewFeature, sindyUpdateTypeCreate );
				// ノード種別設定
				AheResetNodeAttribute( ipNewFeature, targetItems );
#ifdef DEBUG
				LONG lNodeID = -1;
				ipNewFeature->get_OID( &lNodeID );
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ノード作成 OBJECTID=%d\n"), lNodeID );
				//AheTraceGeometry( ipCreateNodePoint, _T("CEditHelper::SplitFeaturesByGeometry()") );
				AheDrawPoint( GetFocusMapScreenDisplay(), ipCreateNodePoint, FALSE, 8, 0, 0, 255, esriSMSCircle, FALSE, TRUE );
				AheTraceRow( ipNewFeature, _T("CEditHelper::SplitFeaturesByGeometry() : 新規作成ノード") );
				AheTraceGeometry( AheGetShape(ipNewFeature), _T("CEditHelper::SplitFeaturesByGeometry() : 新規作成ノード") );
#endif
				cNewFeatures.push_back( ipNewFeature );
				cAllNodes.push_back( ipNewFeature );
			}
		}

		// 重複するノードはマージする
		CTargetItems cAllNodeItems;				// 全ノード = cExistNodes + cNewFeatures のノード + cTargetFeatures のノード
		CTargetItems cDeleteNodeFeatures;		// 削除対象となるフィーチャ

		cExistNodes.GetFeature( cAllNodeItems, nodeFCName );

		cNewFeatures.GetFeature( cAllNodeItems, nodeFCName, TRUE );
		cTargetFeatures.GetFeatures( cAllNodeItems, nodeFCName, TRUE );
		for( CTargetItems::iterator itNodes1 = cAllNodeItems.begin(); itNodes1 != cAllNodeItems.end(); ++itNodes1 )
		{
			if( cDeleteNodeFeatures.IsAlreadyExist( *itNodes1 ) ) continue;

			for( CTargetItems::iterator itNodes2 = cAllNodeItems.begin(); itNodes2 != cAllNodeItems.end(); ++itNodes2 )
			{
				if( cDeleteNodeFeatures.IsAlreadyExist( *itNodes2 ) ) continue;

				if( itNodes2 == itNodes1 ) continue;

				if( AheIsDBEqual( itNodes2->GetShape(), itNodes1->GetShape() ) )
				{
					IAttributeSelectDlgObjPtr ipAttrSelectDlg( CLSID_AttributeSelectDlgObj );
					CComBSTR bstrEnableField;
					// TODO: 暫定対応
					// 鉄道ノード同士のマージのときはノード種別も選択対象に入れる
					// それ以外のネットワークノードは道路ノードに準拠する
					// 今回は直打ちとするが、本来、どこかに定義しておくべきであり
					// フィールド名はsindy_schemaのものを使用するべき
					if(0==itNodes1->GetFeatureClassName().CompareNoCase(sindy::schema::sj::railway_node::kTableName))
						bstrEnableField = _T("OBJECTID|SOURCE|WORK_YEAR|NODECLASS_C");
					else
						bstrEnableField = _T("OBJECTID|SOURCE|WORK_YEAR|NAME_KANJI|NAME_YOMI|HEIGHT_C|HEIGHT|SIGNAL_C|PARKING_ID");

					if( ipAttrSelectDlg->SetFeature( itNodes1->GetFeature() ) != S_OK ||
						ipAttrSelectDlg->SetFeature( itNodes2->GetFeature() ) != S_OK ||
						ipAttrSelectDlg->SetEnableFields( bstrEnableField )   != S_OK )
						return FALSE;

					if( ( ipAttrSelectDlg->DoModal( (OLE_HANDLE)GetHWND() ) ) == S_OK )
					{
						IUnknownPtr ipUnk;
						ipAttrSelectDlg->GetMeargedFeature( &ipUnk );

						if( itNodes1->GetFeature() == (IFeaturePtr)ipUnk )
							cDeleteNodeFeatures.push_back( *itNodes2 );
						else if( itNodes2->GetFeature() == (IFeaturePtr)ipUnk )
							cDeleteNodeFeatures.push_back( *itNodes1 );
						else 
							return FALSE;
					}
					else
						return FALSE;
				}
			}
		}
		if( cDeleteNodeFeatures.size() > 0 )
		{
			// ノードを格納しているリストから削除対象ノードを削除
			for( CTargetItems::iterator itDeleteNodes = cDeleteNodeFeatures.begin(); itDeleteNodes != cDeleteNodeFeatures.end(); ++itDeleteNodes )
			{
				// 全ノード
				CTargetItems::iterator itAllNodes = cAllNodeItems.begin();
				while( itAllNodes != cAllNodeItems.end() )
				{
					if( *itDeleteNodes == *itAllNodes )
						itAllNodes = cAllNodeItems.erase( itAllNodes );
					else
						++itAllNodes;
				}
				// cNoSetModifyDataFeatures
				CTargetItems::iterator itNoSetModify = cNoSetModifyDataFeatures.begin();
				while( itNoSetModify != cNoSetModifyDataFeatures.end() )
				{
					if( *itDeleteNodes == *itNoSetModify )
						itNoSetModify = cNoSetModifyDataFeatures.erase( itNoSetModify );
					else
						++itNoSetModify;
				}
				// 渡されたフィーチャ
				CTargetItems::iterator itTargets = cTargetFeatures.begin();
				while( itTargets != cTargetFeatures.end() )
				{
					if( *itDeleteNodes == *itTargets )
						itTargets = cTargetFeatures.erase( itTargets );
					else
						++itTargets;
				}
				// pReferenceNodes
				if( pReferenceNodes )
				{
					CTargetItems::iterator itReference = pReferenceNodes->begin();
					while( itReference != pReferenceNodes->end() )
					{
						if( *itDeleteNodes == *itReference )
							itReference = pReferenceNodes->erase( itReference );
						else
							++itReference;
					}
				}
				// 削除
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ノード OBJECTID=%d 削除\n"), itDeleteNodes->GetOID() );
				
				// 関連するフィーチャも削除
				if( !DeleteRelationTables(itDeleteNodes->GetFeature()) )
					return false;
				// ノード削除
				if( itDeleteNodes->Delete() == FALSE ) _ASSERTE( FALSE );
			}
		}

		// FROM、TO 書き換え対象と思われる道路リンクの FROM、TO 書き換え
		for( CTargetItems::iterator itRoads = targetItems.begin(); itRoads != targetItems.end(); ++itRoads )
		{
			LONG lFromNodeFeatureID = -1, lToNodeFeatureID = -1;
			for( CTargetItems::iterator itNodes = cAllNodeItems.begin(); itNodes != cAllNodeItems.end(); ++itNodes )
			{
				if( itRoads->IsFromPoint( itNodes->GetShape() ) )
					lFromNodeFeatureID = itNodes->GetOID();
				else if( itRoads->IsToPoint( itNodes->GetShape() ) )
					lToNodeFeatureID = itNodes->GetOID();

				if( lFromNodeFeatureID >= 0 && lToNodeFeatureID >= 0 ) break;
			}
			if( lFromNodeFeatureID < 0 || lToNodeFeatureID < 0 )
			{
				// FROM、TOに接続するノードが見つからなかった
				CString strMsg;
#ifdef DEBUG
				for( CTargetItems::iterator itNodes = cAllNodeItems.begin(); itNodes != cAllNodeItems.end(); ++itNodes )
					ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : 調査対象ノード：OBJECTID=%d\n"), itNodes->GetOID() );
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() 付け替え失敗道路リンクのハイライト（赤）\n") );
				AheDrawPolyline( GetFocusMapScreenDisplay(), itRoads->GetShape(), RGB(255,0,0), 2, esriSLSSolid, TRUE );
#endif
				strMsg.Format(ERROR_FAIL_TO_ROAD_LINK_REPLACE, itRoads->GetOID() );
				AheMessageBox( strMsg, SPLIT_ROAD_LINK, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
				return FALSE;
			}
			CComVariant vaFromID = itRoads->GetAttribute( FROM_FIELD );
			CComVariant vaToID   = itRoads->GetAttribute( TO_FIELD   );
			if( vaFromID.vt != VT_I4 || vaFromID.lVal != lFromNodeFeatureID ||
				vaToID.vt   != VT_I4 || vaToID.lVal   != lToNodeFeatureID )
			{
				// 書き換え
				itRoads->SetAttribute( FROM_FIELD, lFromNodeFeatureID );
				itRoads->SetAttribute( TO_FIELD,   lToNodeFeatureID );
				// ID を引き継いだものでもなく、新規に作成されたものでもなく、cTargetFeaturesにもない場合はここで属性編集の履歴を書き込む（あるのか？）
				if( cTakeOveredFeatures.IsAlreadyExist( *itRoads ) == FALSE &&
					cNewFeatures.IsAlreadyExist( *itRoads ) == FALSE &&
					cTargetFeatures.IsAlreadyExist( *itRoads ) == FALSE )
					itRoads->SetModifyData( sindyUpdateTypeProperty );
			}
		}
		// ノード種別の再設定（必要なもののみ）
		IPointCollectionPtr ipPointCol( AheGetClone( ipGeom ) );
		if( !ipPointCol )
		{
			ipPointCol.CreateInstance( CLSID_Multipoint );
			((IGeometryPtr)ipPointCol)->putref_SpatialReference( AheGetSpatialReference( ipGeom ) ); // 空間参照付与
			IPointPtr ipPoint( ipGeom );
			if( ipPoint )
				ipPointCol->AddPoint( ipPoint );
		}
		// cTargetFeatures の中のノードのポイントも種別設定対象として追加
		CTargetItems cTargetNodes;
		cTargetFeatures.GetNodeFeatures( cTargetNodes, TRUE );	// 2叉路から道路とノードを一つずつ分離した場合など、これをしないと種別が再設定されない
		for( CTargetItems::iterator itTargetNodes = cTargetNodes.begin(); itTargetNodes != cTargetNodes.end(); ++itTargetNodes )
			ipPointCol->AddPoint( (IPointPtr)itTargetNodes->GetShapeCopy() );	// <- ポイントの場合に GetShape() だと StopEditOperation() で落ちる！！なんでー？？
		// 重複点は取り除く
		ITopologicalOperatorPtr ipTopo( ipPointCol );
		if( ipTopo )
		{
			ipTopo->Simplify();
			ipPointCol = ipTopo;
		}

		LONG lPointCount = -1;
		ipPointCol->get_PointCount( &lPointCount );
		for( LONG i = 0; i < lPointCount; ++i )
		{
			IPointPtr ipPoint;
			ipPointCol->get_Point( i, &ipPoint );
			for( CTargetItems::iterator itAllNodes = cAllNodeItems.begin(); itAllNodes != cAllNodeItems.end(); ++itAllNodes )
			{
				if( itAllNodes->IsEquals( ipPoint ) == TRUE )
				{
					BOOL bIsChanged = FALSE;
					BOOL bSetModifyData = ( cNewFeatures.IsAlreadyExist( *itAllNodes ) == FALSE
						&& cTargetFeatures.IsAlreadyExist( *itAllNodes ) == FALSE );
					if( AheResetNodeAttribute( itAllNodes->GetFeature(), targetItems, &bIsChanged, bSetModifyData ) )
					{
						if( bIsChanged == TRUE )
							cModifyNodeKindFeatures.push_back( *itAllNodes );	// 後で cTargetFeatures にあわせこみ
					}
					else {
						// ノード種別再設定失敗！！
						AheMessageBox( ERROR_FAIL_TO_RESET_NODE, SPLIT_ROAD_LINK, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
						return FALSE;
					}
				}
			}
		}
	}
	return TRUE;
}

// 関連フィーチャを内包するネットワークリンクを探して、関連IDフィールドにネットワークリンクのOBJECTIDをセット
BOOL CEditHelper::ResetCommonFeatures(
	CTargetItems& cNewFeatures,
	CTargetItems& cTargetFeatures,
	const CTargetItems& cNoSetModifyDataFeatures,
	const CTargetItems& cTakeOveredFeatures)
{
	// 分割されたフィーチャをみて、ネットワークリンクに関連するフィーチャクラスがあるなら、
	// そのネットワークリンクを格納する（例えば、SEGMENT_ATTRに対するROAD_LINKなど）
	// 後々の処理でネットワークリンクのIFeatureClassPtrを使うので、敢えて、その形で取得する
	// （後で検索し直す場合、編集開始されているフィーチャを探し直すのが少し手間になる。
	//   ここの処理に入るということは編集開始されているということなので、ポインタを保持しておく）
	// <フィーチャクラス名, フィーチャクラス>
	std::map< CString, IFeatureClassPtr > ipNWClassMap;
	for( CTargetItems::const_iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
	{
		if( AheIsRelateWithNetwork( it->GetFeature() ) ) // ネットワーク関係フィーチャから情報を取得（bug 5486）
		{
			// ネットワーク関係フィーチャなら、それを内包するフィーチャクラスを取得
			// 例えば、GRADだったらROAD_LINKのIFeatureClassPtrを取得
			IFeatureClassPtr ipFC = AheGetNetworkLinkFeatureClass( it->GetFeature() );
			ipNWClassMap.insert( std::make_pair(AheGetFeatureClassName(ipFC), ipFC) );
		}
	}

	// ネットワークリンクごとに関連するフィーチャを探して処理
	for(std::map< CString, IFeatureClassPtr >::iterator itNWClass=ipNWClassMap.begin(); itNWClass!=ipNWClassMap.end(); ++itNWClass)
	{
		/// フィーチャクラス名を格納
		CString strNWClassName( itNWClass->first );

		/// フィーチャクラスに内包される可能性のある関連フィーチャ名を格納
		CString strRelClassName( AheGetNetworkSplitTarget( strNWClassName, false ) );
		// 内包されるものがないなら以下の処理は必要ない
		if( strRelClassName.IsEmpty() ) continue;

		CTargetItems cRelFeatures, cTargetNWFeatures;
		// 全ての関連フィーチャ = cTargetFeatures の関連フィーチャ ＋ cNewFeatures の関連フィーチャ
		cTargetFeatures.GetFeatures( cRelFeatures, strRelClassName );
		cNewFeatures.GetFeatures( cRelFeatures, strRelClassName, TRUE );
		// 対象のネットワークリンク ＝ cTargetFeatures のネットワークリンク ＋ cNewFeatures のネットワークリンク
		cTargetFeatures.GetFeatures( cTargetNWFeatures, strNWClassName );
		cNewFeatures.GetFeatures( cTargetNWFeatures, strNWClassName, TRUE );

		// 対象のネットワークリンクが属するデータセットの SpatialReference を取る
		// 道路リンクのリンク内属性・傾斜処理でスプリット後の形状比較でどうしても合わなくなる（2つのリンク内属性が重なっている場合？）
		// ので、しょうがない
		ISpatialReferencePtr ipNWSpRef = AheGetSpatialReference( itNWClass->second );

		// 全ての関連フィーチャの EndPoint を接するネットワークリンクに追加
		if(!AddEndPointToNetworkLink(cNewFeatures, cRelFeatures, cTargetNWFeatures, ipNWSpRef))
			return FALSE;

		///	// 新規に作成された道路リンクへの属性・形状の反映はここで終わりのはずなので、 Insert する
		///	CTargetItems cNewRoadFeatures;
		///	cNewFeatures.GetRoadFeatures( cNewRoadFeatures );
		///	if( cNewRoadFeatures.size() > 0 )
		///	{
		///		CComVariant vaID;
		///		IFeatureCursorPtr ipFeatureCursor;
		///		IFeatureClassPtr ipRoadClass = GetRoadClass();
		///		ipRoadClass->Insert( VARIANT_FALSE, &ipFeatureCursor );
		///		for( CTargetItems::iterator it = cNewRoadFeatures.begin(); it != cNewRoadFeatures.end(); it++ )
		///		{
		///			ipFeatureCursor->InsertFeature( (IFeatureBufferPtr)it->GetFeature(), &vaID );
		///			it->Attach( it->GetFeature(), vaID.lVal );
		///		}
		///	}

		// 全ての関連フィーチャの構成点再構築及び関連リンクIDの再設定
		if(!ReconstructCommonVertexAndResetRelField( cNewFeatures, cTargetFeatures, cNoSetModifyDataFeatures, cTakeOveredFeatures,
			cRelFeatures, cTargetNWFeatures, itNWClass->second, ipNWSpRef))
			return FALSE;
	}
	return TRUE;
}

// 全てのリンク内属性及び傾斜の EndPoint を接するネットワークリンクに追加する
BOOL CEditHelper::AddEndPointToNetworkLink(
	const CTargetItems& cNewFeatures,
	CTargetItems& cSegAttrAndGradFeatures,
	const CTargetItems& cAllRoadFeatures,
	const ISpatialReferencePtr& ipRoadSpRef) const
{
	if(cSegAttrAndGradFeatures.empty()) return TRUE;

	// 全てのリンク内属性及び傾斜の EndPoint を取得
	std::list< CAdapt< IPointPtr > > listSegAttrAndGradsEndPoint;
	for( CTargetItems::iterator itSegAttrAndGrads = cSegAttrAndGradFeatures.begin(); itSegAttrAndGrads != cSegAttrAndGradFeatures.end(); ++itSegAttrAndGrads )
	{
		IPointPtr ipSegAttrAndGradsFromPoint = itSegAttrAndGrads->GetFromPoint();
		IPointPtr ipSegAttrAndGradsToPoint   = itSegAttrAndGrads->GetToPoint();
		// 同じ点に既にあるものは追加しない
		BOOL bFindSameFromPoint = FALSE, bFindSameToPoint = FALSE;
		for( std::list< CAdapt< IPointPtr > >::iterator it = listSegAttrAndGradsEndPoint.begin(); it != listSegAttrAndGradsEndPoint.end(); ++it )
		{
			if( bFindSameFromPoint == FALSE && AheIsEqual( it->m_T, ipSegAttrAndGradsFromPoint ) == TRUE )
				bFindSameFromPoint = TRUE;
			if( bFindSameToPoint   == FALSE && AheIsEqual( it->m_T, ipSegAttrAndGradsToPoint   ) == TRUE )
				bFindSameToPoint   = TRUE;

			if( bFindSameFromPoint == TRUE && bFindSameToPoint == TRUE ) break;
		}
		if( bFindSameFromPoint == FALSE ) listSegAttrAndGradsEndPoint.push_back( ipSegAttrAndGradsFromPoint );
		if( bFindSameToPoint   == FALSE ) listSegAttrAndGradsEndPoint.push_back( ipSegAttrAndGradsToPoint   );
	}
	// 道路リンクに構成点を追加
	for( CTargetItems::const_iterator itAllRoads = cAllRoadFeatures.begin(); itAllRoads != cAllRoadFeatures.end(); ++itAllRoads )
	{
		IPolylinePtr ipRoadGeometry = itAllRoads->GetShape();
		for( std::list< CAdapt< IPointPtr > >::iterator it = listSegAttrAndGradsEndPoint.begin(); it != listSegAttrAndGradsEndPoint.end(); ++it )
		{
			if (!AheIsVertexPoint(it->m_T, ipRoadGeometry, ipRoadSpRef) &&
				AhePointIsOnGeometry(ipRoadGeometry, it->m_T, ipRoadSpRef))
			{
				// 編集可能？
				if( AheIsEditableFeature2( m_piMap, itAllRoads->GetFeature(), NULL, sindyeEditMove, GetRule(), FALSE, GetHWND() ) != TRUE )
				{
					CString strMsg;
					strMsg.Format(CANNOT_ADD_SHARED_SHAPE_POINT_TO_MOVABLE, itAllRoads->GetFeatureClassName(), itAllRoads->GetOID() );
					AheMessageBox( strMsg, RESTRUCTURING_OF_ATTRIBUTE_SLOPE, MB_OK|MB_ICONSTOP );
					return FALSE;
				}

				VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
				LONG lNewPartIndex = -1, lNewSegmentIndex = -1;
				ipRoadGeometry->SplitAtPoint( it->m_T, VARIANT_FALSE, VARIANT_FALSE, &vaIsSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

				AheSetShape( itAllRoads->GetFeature(), ipRoadGeometry );
				// 新規に作成されたフィーチャ群に存在しない場合のみ形状変更の履歴追加
				if( cNewFeatures.IsAlreadyExist( *itAllRoads ) == FALSE )
					AheSetModifyData( itAllRoads->GetFeature(), sindyUpdateTypeShape );
			}
		}
	}
	return TRUE;
}

BOOL CEditHelper::ReconstructCommonVertexAndResetRelField(
	CTargetItems& cNewFeatures,
	CTargetItems& cTargetFeatures,
	const CTargetItems& cNoSetModifyDataFeatures,
	const CTargetItems& cTakeOveredFeatures,
	CTargetItems& cSegAttrAndGradFeatures,
	const CTargetItems& cAllRoadFeatures,
	const IFeatureClassPtr& ipRoadClass,
	const ISpatialReferencePtr& ipRoadSpRef)
{
	for( CTargetItems::iterator itSegAttrAndGrads = cSegAttrAndGradFeatures.begin(); itSegAttrAndGrads != cSegAttrAndGradFeatures.end(); ++itSegAttrAndGrads )
	{
		IPointPtr ipSGFromPoint, ipSGToPoint;
		IGeometryPtr ipSGGeom = itSegAttrAndGrads->GetShapeCopy();
		((IPolylinePtr)ipSGGeom)->get_FromPoint( &ipSGFromPoint );
		((IPolylinePtr)ipSGGeom)->get_ToPoint( &ipSGToPoint );
		
		// リンク内属性・傾斜に包含される道路を検索（bug 5564）
		// 無ければ、2 つの端点で道路を検索
		CTargetItems cFromRoadItems, cToRoadItems;
		for( CTargetItems::const_iterator itRoads = cAllRoadFeatures.begin(); itRoads != cAllRoadFeatures.end(); ++itRoads )
		{
//			if( ! AheIsContain( ipSGGeom, itRoads->GetShape() ) )
//			{
#ifdef DEBUG
			sindyeSpatialRel sprelFrom = AheTestGeometryRelationship( itRoads->GetShape(), ipSGFromPoint, (sindyeSpatialRel)127, ipRoadSpRef );
			sindyeSpatialRel sprelTo   = AheTestGeometryRelationship( itRoads->GetShape(), ipSGToPoint,   (sindyeSpatialRel)127, ipRoadSpRef );
			ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : 傾斜・リンク内属性（%d）と道路リンク（%d）のジオメトリ関係： FROM:%d TO:%d\n"), itSegAttrAndGrads->GetOID(), itRoads->GetOID(), sprelFrom, sprelTo );
#endif
			if( AheTestGeometryRelationship( itRoads->GetShape(), ipSGFromPoint, (sindyeSpatialRel)(sindyeSpatialRelTouch|sindyeSpatialRelContain), ipRoadSpRef ) > 0 )
				cFromRoadItems.push_back( *itRoads );
			if( AheTestGeometryRelationship( itRoads->GetShape(), ipSGToPoint,   (sindyeSpatialRel)(sindyeSpatialRelTouch|sindyeSpatialRelContain), ipRoadSpRef ) > 0 )
				cToRoadItems.push_back(   *itRoads );
/*#ifdef DEBUG
			// 検索対象の道路リンクエレメント追加
			AheAddLineElement( m_cArcHelper.GetGraphicsContainer( FALSE ), itRoads->GetShape(), RGB(255,0,0), 2, esriSLSSolid );
#endif*/
		}
		if( cFromRoadItems.size() < 1 || cToRoadItems.size() < 1 )
		{
			// コピーツールでリンク内属性・傾斜のみ渡された場合は道路がないので、ここで検索してやる
			SelectByShapeFromFeatureClass( ipSGFromPoint, cFromRoadItems, ipRoadClass );
			SelectByShapeFromFeatureClass( ipSGToPoint, cToRoadItems, ipRoadClass );
			// 検索できた道路にはリンク内属性・傾斜の端点が追加されていないので、ここで追加
			CTargetItems cSGFromAndToRoads = cFromRoadItems;
			cSGFromAndToRoads += cToRoadItems;
			for( CTargetItems::iterator itSGFromAndToRoads = cSGFromAndToRoads.begin(); itSGFromAndToRoads != cSGFromAndToRoads.end(); ++itSGFromAndToRoads )
			{
				IPolylinePtr ipRoadGeometry = itSGFromAndToRoads->GetShape();
				for( LONG k = 0; k < 2; k++ )
				{
					IPointPtr ipSGFromToPoint;
					switch( k )
					{
						case 0: ipSGFromToPoint = ipSGFromPoint; break;
						case 1: ipSGFromToPoint = ipSGToPoint; break;
					}
					// 既存の構成点ではない場合に構成点を追加する
					if( AheIsVertexPoint( ipSGFromToPoint, ipRoadGeometry, ipRoadSpRef ) == FALSE && 
						AhePointIsOnGeometry( ipRoadGeometry, ipSGFromToPoint, ipRoadSpRef ) == TRUE )
					{
						// 編集可能？
						if( AheIsEditableFeature2( m_piMap, itSGFromAndToRoads->GetFeature(), NULL, sindyeEditMove, GetRule(), FALSE, GetHWND() ) == TRUE )
						{
							VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
							LONG lNewPartIndex = -1, lNewSegmentIndex = -1;
							ipRoadGeometry->SplitAtPoint( ipSGFromToPoint, VARIANT_FALSE, VARIANT_FALSE, &vaIsSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

							AheSetShape( itSGFromAndToRoads->GetFeature(), ipRoadGeometry );
							// 新規に作成されたフィーチャ群に存在しない場合のみ形状変更の履歴追加
							if( cNewFeatures.IsAlreadyExist( *itSGFromAndToRoads ) == FALSE )
							{
								AheSetModifyData( itSGFromAndToRoads->GetFeature(), sindyUpdateTypeShape );
								cTargetFeatures.push_back( *itSGFromAndToRoads );
							}
						}
						else {
							CString strMsg;
							strMsg.Format(ERROR_NOT_ADD_SHARED_SHAPE_POINT, itSGFromAndToRoads->GetFeatureClassName(), itSGFromAndToRoads->GetOID() );
							AheMessageBox( strMsg, RESTRUCTURING_ATTRIBUTE_SLOPE, MB_OK|MB_ICONSTOP );
							return FALSE;
						}
					}
				}	
			}

			if( cFromRoadItems.size() < 1 || cToRoadItems.size() < 1 )
			{
	/*#ifdef DEBUG
				// 接続する道路リンクが見つからなかった傾斜・リンク内属性エレメント追加
				AheAddLineElement( m_cArcHelper.GetGraphicsContainer( FALSE ), itSegAttrAndGrads->GetShape(), RGB(0,255,0), 2, esriSLSSolid );
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : FROM方向で見つかった道路リンクの数：%d TO方向で見つかった道路リンクの数：%d\n"), cFromRoadItems.size(), cToRoadItems.size() );
	#endif*/
				CString strMsg;
				strMsg.Format(ERROR_END_POINT_ISNOT_CONTACT_ROAD_LINK, itSegAttrAndGrads->GetFeatureClassName(), itSegAttrAndGrads->GetOID() );
				AheMessageBox( strMsg, SPLIT_OF_ATTRIBUTE_SLOPE, MB_OK|MB_ICONSTOP );
				return FALSE;
			}
		}
#if 0
		CTargetItems cSegRoad;	// 絞込み後の道路リンク格納用
		BOOL bFinish = FALSE, bFinish2 = FALSE;

		// 選択できた道路リンクの絞込み
		for( CTargetItems::iterator itFromRoads = cFromRoadItems.begin(); itFromRoads != cFromRoadItems.end(); itFromRoads++ )
		{
			for( CTargetItems::iterator itToRoads = cToRoadItems.begin(); itToRoads != cToRoadItems.end(); itToRoads++ )
			{
				// それぞれの検索結果の中に同じ道路リンクがあった場合は関連する道路リンクはひとつである
				if( itFromRoads->GetOID() == itToRoads->GetOID() )
				{
					cSegRoad.clear();
					cSegRoad.push_back( *itToRoads );
					bFinish = TRUE;
					break;
				}
				else if( bFinish2 == FALSE )
				{
					ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : レイヤ名=%s OID=%d FROM に接する道路 : OBJECTID=%d TO に接する道路 : OBJECTID=%d\n"), itSegAttrAndGrads->GetFeatureClassName(), itSegAttrAndGrads->GetOID(), itFromRoads->GetOID(), itToRoads->GetOID() );
					AheTraceGeometry( itFromRoads->GetShape(), _T("CEditHelper::SplitFeaturesByGeometry() : FROMに接する道路") );
					AheTraceGeometry( itToRoads->GetShape(),   _T("CEditHelper::SplitFeaturesByGeometry() : TO  に接する道路") );
					if( AheIsTouchEndPoint( itFromRoads->GetShape(), itToRoads->GetShape(), ipRoadSpRef ) == TRUE )
					{
						// それぞれの検索結果のなかで端点が接しているものがあればそれを関連道路リンクとする
						cSegRoad.push_back( *itFromRoads );
						cSegRoad.push_back( *itToRoads );
						bFinish2 = TRUE;
					}
				}
			}
			if( bFinish == TRUE ) break;
		}
#else
		// リンク内属性・傾斜の構成点は、既に道路リンクに共有構成点として
		// 追加されている。しかし、大本のループで道路リンクを検索しながら
		// 処理している関係上どうしてもループ前にリンク内属性・傾斜の空間
		// 参照を合わせる事ができない。
		// そこで、不本意ながらここで空間参照を合わせる。
		// ちなみに、合わせないとリンク内属性・傾斜の端点を道路リンクの辺
		// にスナップした場合にFROM、TO判定ができなくなる。
		//itSegAttrAndGrads->Simplify(); これが原因でスプリット処理していないリンク内属性の警告が出る（bug 4763）。
		// リンク内属性を単独編集で道路リンク辺にスナップしても大丈夫なことから、コメントにする。
		CTargetItems cSegRoad( AheGetRelationshipContainFeature( itSegAttrAndGrads->GetFeature(), cFromRoadItems + cToRoadItems ) ); // 絞込み後の道路リンク
#endif // if 0
		if( ! ( 1 <= cSegRoad.size() && cSegRoad.size() <= 2 ) )
		{
			AheMessageBox( ERROR_NOT_FIND_ROAD_LINK_ATTRIBUTE_SLOPE, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
			return FALSE;
		}

		IPointPtr ipTouchPoint;	// 道路リンク同士の端点が接している場所（cSegRoad.size() == 2 の場合のみ）
		if( cSegRoad.size() == 2 )
		{
			// リンク内属性あるいは傾斜のスプリットポイントを道路リンク同士の端点の接点から求める
			CTargetItems::iterator it = cSegRoad.begin();
			IGeometryPtr ipGeom1 = it->GetShapeCopy();
			advance( it, 1 );
			IGeometryPtr ipGeom2 = it->GetShapeCopy();
			IGeometryPtr ipPoints = AheGetCrossOrTouchPoints( ipGeom1, ipGeom2, 1 );
			if( ipPoints == NULL ) 
			{
				AheMessageBox( ERROR_CANNOT_ACROSS_3_ROAD_LINK, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
				return FALSE;
			}
			((IPointCollectionPtr)ipPoints)->get_Point( 0, &ipTouchPoint );
		}
		IPointCollectionPtr ipSplitterPoints( CLSID_Multipoint );
		((IGeometryPtr)ipSplitterPoints)->putref_SpatialReference( AheGetSpatialReference( ipSGFromPoint ) ); // 空間参照付与
		ipSplitterPoints->AddPoint( ipSGFromPoint );
		ipSplitterPoints->AddPoint( ipSGToPoint );
/*#ifdef DEBUG
		AheTraceGeometry( ipTouchPoint, _T("CEditHelper::SplitFeaturesByGeometry() : ipTouchPoint :") );
		AheTraceGeometry( ipSGFromPoint, _T("CEditHelper::SplitFeaturesByGeometry() : ipSGFromPoint :") );
		AheTraceGeometry( ipSGToPoint, _T("CEditHelper::SplitFeaturesByGeometry() : ipSGToPoint :") );
#endif */
		
		std::list< CAdapt< IGeometryPtr > > listGeometry;
		for( CTargetItems::iterator it = cSegRoad.begin(); it != cSegRoad.end(); ++it )
		{
/*			// リンク内属性・傾斜が接する道路リンクを分割して、それを新しいリンク内属性・傾斜の形状とする
			// のだが、分割点群にipSplitterPointsを使用するのは間違い
			// 分割点群はリンク内属性・傾斜のEndPointを足す必要がある
			IPointCollectionPtr ipSplitterPoints2( AheGetClone( ipSplitterPoints ) );
			ipSplitterPoints2->AddPoint( ipSGFromPoint, &vtMissing, &vtMissing );
			ipSplitterPoints2->AddPoint( ipSGToPoint, &vtMissing, &vtMissing );*/
			_ISetPtr ipSplittedGeomSet = SplitPolylineByPoints( it->GetShapeCopy(), ipSplitterPoints );
			// リンク内属性・傾斜がノード to ノードの場合はipSplittedGeomSetはNULLになるので、
			// その場合は道路の形状をそのまま突っ込む（bug 5194,5181）
			if( ! ipSplittedGeomSet )
			{
				ipSplittedGeomSet.CreateInstance( CLSID_Set );
				ipSplittedGeomSet->Add( it->GetShapeCopy() );
			}

			if( ipSplittedGeomSet )
			{
				ipSplittedGeomSet->Reset();

				LONG lCount = -1;
				ipSplittedGeomSet->get_Count( &lCount );

				IUnknownPtr ipUnk;
				while( SUCCEEDED( ipSplittedGeomSet->Next( &ipUnk ) ) )
				{
					IGeometryPtr ipSplittedGeom( ipUnk );
					if( !ipSplittedGeom ) break;

/*#ifdef DEBUG
					AheTraceGeometry( ipSplittedGeom, _T("CEditHelper::SplitFeaturesByGeometry() : ipSplittedGeom :") );
#endif*/
					if( ipTouchPoint )
					{
						// ipTouchPoint を持つもののみ
						if( AheIsEndPoint( ipTouchPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
						{
							// スプリット後の形状が道路の向きに合わせられてしまうので、元の向きにあわせる
							if( AheIsFromPoint( ipSGToPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE ||
								AheIsToPoint( ipSGFromPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
								((IPolylinePtr)ipSplittedGeom)->ReverseOrientation();

							listGeometry.push_back( ipSplittedGeom );
						}
					}
					else {
						// ipFromPoint と ipToPoint を両端点に持つもののみ
						if( AheIsEndPoint( ipSGFromPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE && AheIsEndPoint( ipSGToPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
						{
							// スプリット後の形状が道路の向きに合わせられてしまうので、元の向きにあわせる
							if( AheIsFromPoint( ipSGToPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE ||
								AheIsToPoint( ipSGFromPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
								((IPolylinePtr)ipSplittedGeom)->ReverseOrientation();

							listGeometry.push_back( ipSplittedGeom );
						}
					}
				}
			}
			else {
				// EditToolで
				//  A         B             C
				// ○----×--------○   道路リンク、ノード
				//         --------     リンク内属性
				// というような形状で、BをCTRLを掴んで道路リンクのみを
				// 移動する
				// 
				//         
				//         --------     リンク内属性
				// ○              ○   ノード
				//   ＼          ／
				//     ＼      ／       道路リンク
				//       ＼  ／
				//         ×       
				// ここの時点では、こんな感じになっている。
				// で、ipSplittedGeomSetはリンク内属性の形状で
				// 今の道路リンクの形状を分割したものが入っている。
				// 当然分割形状はなく、ipSplittedSetはNULLとなるが、
				// だからといってリンク内属性が道路リンク全体に
				// 付いているわけではないのは一目瞭然と思う。
				// なので、ここで本当に全体についているのか（全体につけても良いのか）
				// どうかをチェックする必要がある
				// -> それには、エンドポイントが一致しているかどうかをチェックすればよい
				if( //AheIsEqual( it->GetShape(), itSegAttrAndGrads->GetShape() ) ||											// 形状が一致しているか
					( AheIsEqual( it->GetFromPoint(), ipSGToPoint ) && AheIsEqual( it->GetToPoint(), ipSGFromPoint ) ||		// エンドポイントが一致しているか
					  AheIsEqual( it->GetFromPoint(), ipSGFromPoint ) && AheIsEqual( it->GetToPoint(), ipSGToPoint ) ) )
				{
					// リンク全体についている場合
					IGeometryPtr ipSplittedGeom( it->GetShapeCopy() );
					// スプリット後の形状が道路の向きに合わせられてしまうので、元の向きにあわせる
					if( AheIsFromPoint( ipSGToPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE ||
						AheIsToPoint( ipSGFromPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
						((IPolylinePtr)ipSplittedGeom)->ReverseOrientation();

					listGeometry.push_back( ipSplittedGeom );
				}
			}
		}
		// 形状確定
		BOOL bFlg = FALSE;
		if( listGeometry.size() < 1 )
		{
			_ASSERTE( FALSE );	// ここに来てはいけない！！
			AheMessageBox( ERROR_END_POINT_NOT_MATCH_ROAD_LINK, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
			return FALSE;
		}
		IFeaturePtr ipTargetFeat;
		for( std::list< CAdapt< IGeometryPtr > >::iterator itListGeom = listGeometry.begin(); itListGeom != listGeometry.end(); ++itListGeom )
		{
			BOOL bSettedModifyData = FALSE;	// 形状変更及び新規作成の履歴がつけられたかどうか
			if( bFlg == FALSE )
			{
				// 形状が変更されている場合はアップデート＆履歴をつける
#ifdef _DEBUG
				AheTraceGeometry( itSegAttrAndGrads->GetShape(), _T("itSegAttrAndGrads->GetShape()") );
				AheTraceGeometry( itListGeom->m_T, _T("itListGeom->m_T") );
				IGeometryPtr ipSegGrashapeCopyTmp( itSegAttrAndGrads->GetShapeCopy() );
				IGeometryPtr ipListGeomTmp( (IGeometryPtr)AheCreateClone( itListGeom->m_T ) );
				AheSnapToSpatialReferenceAndForceSimplify( ipSegGrashapeCopyTmp, itSegAttrAndGrads->GetSpatialReference() );
				AheSnapToSpatialReferenceAndForceSimplify( ipListGeomTmp, itSegAttrAndGrads->GetSpatialReference() );
				AheTraceGeometry( ipSegGrashapeCopyTmp, _T("S:itSegAttrAndGrads->GetShape()") );
				AheTraceGeometry( ipListGeomTmp, _T("S:itListGeom->m_T") );
#endif // ifdef _DEBUG
				if( AheIsEqual( itSegAttrAndGrads->GetShape(), itListGeom->m_T ) == FALSE )
				{
					// 道路リンク内にリンク内属性などが存在した場合に道路リンクを単独移動した場合、
					// ここでリンク内属性の形状が変更されるが、この時まで該当リンク内属性の編集可不可は
					// チェックされていないようだ…
					// なので、ここで移動可能かどうかチェックする
					if( AheIsEditableFeature2( m_piMap, itSegAttrAndGrads->GetFeature(), itSegAttrAndGrads->GetShape(), sindyeEditMove ) )
					{
						AheTraceGeometry( itListGeom->m_T, _T("既存リンク内属性・傾斜の形状引継ぎ（AheSetShape前）") );
						AheSetShape( itSegAttrAndGrads->GetFeature(), itListGeom->m_T );
						// 履歴付与対象のみ
						if( cNoSetModifyDataFeatures.IsAlreadyExist( *itSegAttrAndGrads ) == FALSE )
						{
							itSegAttrAndGrads->SetModifyData( sindyUpdateTypeShape );
							bSettedModifyData = TRUE;
						}
					}
					else {
						return FALSE;
					}
				}
				// 関連リンク ID 設定
				BOOL bFind = FALSE;
				for( CTargetItems::iterator itRoads = cSegRoad.begin(); itRoads != cSegRoad.end(); ++itRoads )
				{
/*					ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : 関連リンクID設定： ROAD:OBJECTID=%d\n"), itRoads->GetOID() );
					((ITopologicalOperator2Ptr)itListGeom->m_T)->Simplify();
					((ITopologicalOperator2Ptr)itRoads->GetShape())->Simplify();
					((ITopologicalOperator2Ptr)itListGeom->m_T)->put_IsKnownSimple( VARIANT_TRUE );
					((ITopologicalOperator2Ptr)itRoads->GetShape())->put_IsKnownSimple( VARIANT_TRUE );
					AheTraceGeometry( itListGeom->m_T, _T("関連リンクID設定：分割形状") );
					AheTraceGeometry( itRoads->GetShape(), _T("関連リンクID設定：道路リンク") );
					((ITopologicalOperator2Ptr)itListGeom->m_T)->put_IsKnownSimple( VARIANT_TRUE );*/

					// [bug 1936] Simplify() をかけていないと、自己交差ラインのジオメトリ演算は失敗する！！
					// ただし、この部分に限定して言えばリンク内属性及び傾斜は構成点にスナップしているためコピーさえとっておけば
					// Simplify() をかけて処理を行ってもよいと思われる
					IGeometryPtr ipRoadGeomCopy( itRoads->GetShapeCopy() );
					IGeometryPtr ipGeomCopy( AheGetClone( itListGeom->m_T ) );
					((ITopologicalOperator2Ptr)ipRoadGeomCopy)->put_IsKnownSimple( VARIANT_FALSE );
					((ITopologicalOperatorPtr)ipRoadGeomCopy)->Simplify();
					((ITopologicalOperator2Ptr)ipGeomCopy)->put_IsKnownSimple( VARIANT_FALSE );
					((ITopologicalOperatorPtr)ipGeomCopy)->Simplify();
/*					AheForceSimplify( ipRoadGeomCopy );
					AheForceSimplify( ipGeomCopy );	     <- このやり方ではジオメトリ演算がやっぱり失敗する*/
					IRelationalOperatorPtr ipRelOpe( ipRoadGeomCopy );
					VARIANT_BOOL vbContain = VARIANT_FALSE, vbEqual = VARIANT_FALSE;
					ipRelOpe->Contains( ipGeomCopy, &vbContain );
					ipRelOpe->Equals( ipGeomCopy, &vbEqual );
					if( vbContain == VARIANT_TRUE || vbEqual == VARIANT_TRUE )
					//if( itRoads->IsContains2( itListGeom->m_T ) == TRUE )
					{
						// [bug 2883] 関連IDが異なる場合のみセットするようにする（じゃないと沢山編集される）
						if( itRoads->GetOID() != itSegAttrAndGrads->GetAttribute( _T("LINK_ID") ).lVal )
						{
							ATLTRACE(_T("EditHelper::SplitFeaturesByGeometry() : リンク内属性OR傾斜 OBJECTID=%d %d %d\n"), itSegAttrAndGrads->GetOID(), itSegAttrAndGrads->GetAttribute( _T("LINK_ID") ).lVal, itRoads->GetOID() );
							
							/////////////////////////////////////////////////////////////////////////////////
							// [bug4497]LINK_IDだけが変わる時に更新履歴が付かない対策
							// 「形状変更」は「属性変更」も包括するので「属性変更」としない
							BOOL bUpdateShape = TRUE;
							LONG OID, OIDTmp = itSegAttrAndGrads->GetOID();
							for( CTargetItems::const_iterator it_over = cTakeOveredFeatures.begin(); it_over != cTakeOveredFeatures.end(); it_over++ ){
								OID = it_over->GetOID();
								if( OID == OIDTmp )
									bUpdateShape = FALSE;
							}
							if( itSegAttrAndGrads->GetAttribute( _T("LINK_ID") ).lVal != 0  && bUpdateShape == TRUE )
								itSegAttrAndGrads->SetModifyData( sindyUpdateTypeProperty );
							/////////////////////////////////////////////////////////////////////////////////

							itSegAttrAndGrads->SetAttribute( _T("LINK_ID"), itRoads->GetOID() );
						}
						bFind = TRUE;
						break;
					}
				}
				if( bFind == FALSE )
				{
					_ASSERTE( bFind == TRUE );	// ここへきてはいけない
					AheMessageBox( ERROR_FAIL_ADD_ROAD_LINK_ID, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
					return FALSE;
				}

				ipTargetFeat = itSegAttrAndGrads->GetFeature();
				bFlg = TRUE;
			}
			else
			{
///				CComVariant vaID;
///				IFeatureClassPtr ipSegAttrOrGradClass = AheGetFeatureClass( itSegAttrAndGrads->GetFeature() );
///				IFeatureCursorPtr ipFeatureCursor;
///				ipSegAttrOrGradClass->Insert( VARIANT_FALSE, &ipFeatureCursor );
///				ipTargetFeat = AheCreateRowBuffer( itSegAttrAndGrads->GetFeature(), itListGeom->m_T );
				AheTraceGeometry( itListGeom->m_T, _T("新規リンク内属性・傾斜（AheCreateRow前）") );
				ipTargetFeat = AheCreateRow( itSegAttrAndGrads->GetFeature(), itListGeom->m_T );
				AheSetModifyData( ipTargetFeat, sindyUpdateTypeCreate );
				AheCopyAttribute( itSegAttrAndGrads->GetFeature(), ipTargetFeat, TRUE );	// [bug 4043] 分割モードでコピー
				// 関連リンク ID 設定
				BOOL bFind = FALSE;
				for( CTargetItems::iterator itRoads = cSegRoad.begin(); itRoads != cSegRoad.end(); ++itRoads )
				{
					// [bug 2059] 上の方で引継ぎリンク内属性・傾斜への形状変更時には道路リンクのコピー
					// をSimplifyしたもので形状判定をしているのに、新規のもののとき（つまりここのこと）
					// はやってなかった。そのため、作成したリンク内属性や傾斜がノードで分割される場合に
					// 弾かれてしまっていた。
					// しかし、自己交差の演算何とかならんかね…
					IGeometryPtr ipRoadGeomCopy( itRoads->GetShapeCopy() );
					IGeometryPtr ipGeomCopy( AheGetClone( itListGeom->m_T ) );
					((ITopologicalOperator2Ptr)ipRoadGeomCopy)->put_IsKnownSimple( VARIANT_FALSE );
					((ITopologicalOperatorPtr)ipRoadGeomCopy)->Simplify();
					((ITopologicalOperator2Ptr)ipGeomCopy)->put_IsKnownSimple( VARIANT_FALSE );
					((ITopologicalOperatorPtr)ipGeomCopy)->Simplify();
/*					AheForceSimplify( ipRoadGeomCopy );
					AheForceSimplify( ipGeomCopy );	     <- このやり方ではジオメトリ演算がやっぱり失敗する*/
					IRelationalOperatorPtr ipRelOpe( ipRoadGeomCopy );
					VARIANT_BOOL vbContain = VARIANT_FALSE, vbEqual = VARIANT_FALSE;
					ipRelOpe->Contains( ipGeomCopy, &vbContain );
					ipRelOpe->Equals( ipGeomCopy, &vbEqual );
					if( vbContain == VARIANT_TRUE || vbEqual == VARIANT_TRUE )
					//if( itRoads->IsContains2( itListGeom->m_T ) == TRUE )
					{
						// 既についている関連リンク ID と違う場合のみ属性＆履歴セット
						if( AheGetAttribute( ipTargetFeat, _T("LINK_ID") ) != CComVariant( itRoads->GetOID() ) )
						{
							ATLTRACE(_T("EditHelper::SplitFeaturesByGeometry() : リンク内属性OR傾斜 OBJECTID=%d %d %d\n"), itSegAttrAndGrads->GetOID(), AheGetAttribute( ipTargetFeat, _T("LINK_ID") ).lVal, itRoads->GetOID() );

							if( itSegAttrAndGrads->GetAttribute( _T("LINK_ID") ).lVal != 0 )
								itSegAttrAndGrads->SetModifyData( sindyUpdateTypeProperty );

							AheSetAttribute( ipTargetFeat, _T("LINK_ID"), CComVariant(itRoads->GetOID()) );
							bFind = TRUE;
							break;
						}
					}
				}
				if( bFind == FALSE )
				{
					_ASSERTE( bFind == TRUE );	// ここへきてはいけない
					AheMessageBox( ERROR_FAIL_TO_ADD_NEWLY_ROAD_LINK_ID, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
///				// DB に書き込み
///				ipFeatureCursor->InsertFeature( (IFeatureBufferPtr)ipTargetFeat, &vaID );
///				cNewFeatures.push_back( ipTargetFeat, vaID.lVal );
				cNewFeatures.push_back( ipTargetFeat );
			}
		}
	}
	return TRUE;
}

BOOL CEditHelper::CheckNWStructure(CTargetItems& cNewFeatures, CTargetItems& cTargetFeatures, const std::list<ROWDEF>& listLQRows) const
{
	// NWフィーチャを含んでいないなら処理をする必要がないのでTRUEを返す
	if( cTargetFeatures.IsIncludedNetworkItems() == FALSE ) return TRUE;

	// スプリット処理後にノードとリンクが本当に接続されているかどうかをNWフィーチャごとにチェック
	for(CRuleModelNetwork::const_iterator itNWModel=g_cRuleModelNetwork.begin(); itNWModel!=g_cRuleModelNetwork.end(); ++itNWModel)
	{
		CTargetItems cLastNodes, cLastLinks;
		cTargetFeatures.GetFeature( cLastNodes, itNWModel->second );
		cNewFeatures.GetFeature( cLastNodes, itNWModel->second, TRUE );
		cTargetFeatures.GetFeature( cLastLinks, itNWModel->first );
		cNewFeatures.GetFeature( cLastLinks, itNWModel->first, TRUE );

		// NWリンクの端点にNWノードが接続されているかチェック
		if(FALSE==CheckNWConnect(cLastLinks, cLastNodes))
		{
			return FALSE;
		}

		/// チェック用にネットワークリンクに関連するフィーチャを格納
		CString strRelFeatureas( AheGetNetworkSplitTarget( itNWModel->first, false ) );
		CTargetItems cLastRelFeatures;
		cTargetFeatures.GetFeatures( cLastRelFeatures, strRelFeatureas );
		cNewFeatures.GetFeatures( cLastRelFeatures, strRelFeatureas, TRUE );

		// [bug 2924] 道路リンクを単独編集した場合にリンク内属性や傾斜が離れる場合あり（リンク内属性や傾斜の端点部分の道路リンク構成点を編集した場合）
		// この部分にチェックを入れるのが一番影響が少なそうなのでここでチェックする
		if(FALSE==CheckCommonFeaturesConnect(cLastLinks, cLastRelFeatures))
		{
			return FALSE;
		}

		// 以下のチェックはROAD_LINKだけが行う
		if( 0!=itNWModel->first.CompareNoCase( road_link::kTableName) )
			continue;

		// リンク列の整合が取れているかチェック
		if(FALSE==CheckLQConnect(cNewFeatures, cLastLinks, listLQRows))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CEditHelper::CheckNWConnect(const CTargetItems& cLastRoads, const CTargetItems& cLastNodes) const
{
	for( CTargetItems::const_iterator itLastRoads = cLastRoads.begin(); itLastRoads != cLastRoads.end(); ++itLastRoads )
	{
		CComVariant vaFromNodeID = itLastRoads->GetAttribute( FROM_FIELD );
		CComVariant vaToNodeID   = itLastRoads->GetAttribute( TO_FIELD );
		BOOL bFindFromNode = FALSE, bFindToNode = FALSE, bIsFromNode = FALSE, bIsToNode = FALSE;

		for( CTargetItems::const_iterator itLastNodes = cLastNodes.begin(); itLastNodes != cLastNodes.end(); ++itLastNodes )
		{
			if( bFindFromNode == FALSE && vaFromNodeID.lVal == itLastNodes->GetOID() )
			{
				bFindFromNode = TRUE;
				if( AheIsFromPoint( itLastNodes->GetShape(), itLastRoads->GetFeature() ) == TRUE )
					bIsFromNode = TRUE;
				else
				{
					// TODO 道路リンク専用のコメントになっているので、コメントをする必要有り
					CString strMsg;
					strMsg.Format( _T("CEditHelper::SplitFeaturesByGeometry() : 道路リンク（%d）"), itLastRoads->GetOID() );
					AheTraceGeometry( itLastRoads->GetShape(), strMsg );
					strMsg.Format( _T("CEditHelper::SplitFeaturesByGeometry() :  FROMノード（%d）"), itLastNodes->GetOID() );
					AheTraceGeometry( itLastNodes->GetShape(), strMsg );

					strMsg.Format(ERROR_COORD_AND_FROM_NOTE_ID_NOT_MATCH, itLastRoads->GetOID(), vaFromNodeID.lVal );
					AheMessageBox( strMsg, ERROR_FAIL_ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
			}
			else if( bFindToNode == FALSE && vaToNodeID.lVal == itLastNodes->GetOID() )
			{
				bFindToNode = TRUE;
				if( AheIsToPoint( itLastNodes->GetShape(), itLastRoads->GetFeature() ) == TRUE )
					bIsToNode = TRUE;
				else
				{
					// TODO 道路リンク専用のコメントになっているので、コメントをする必要有り
					CString strMsg;
					strMsg.Format( _T("CEditHelper::SplitFeaturesByGeometry() : 道路リンク（%d）"), itLastRoads->GetOID() );
					AheTraceGeometry( itLastRoads->GetShape(), strMsg );
					strMsg.Format( _T("CEditHelper::SplitFeaturesByGeometry() :  TOノード（%d）"), itLastNodes->GetOID() );
					AheTraceGeometry( itLastNodes->GetShape(), strMsg );

					strMsg.Format(ERROR_COORD_AND_TO_NOTE_ID_NOT_MATCH, itLastRoads->GetOID(), vaToNodeID.lVal );
					AheMessageBox( strMsg, ERROR_FAIL_ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
			}
		}
		if( bFindFromNode == FALSE || bFindToNode == FALSE )
			ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : 道路リンク（%d）FROM、TO のノードが見つかりませんでしたので処理を飛ばします。\n"), itLastRoads->GetOID() );
	}
	return TRUE;
}

BOOL CEditHelper::CheckCommonFeaturesConnect(const CTargetItems& cLastRoads, const CTargetItems& cLastSegGrad) const
{
	// 新規作成・更新された全ての道路リンクに関して、対応するリンク内属性・傾斜が含まれるか確認
	for( CTargetItems::const_iterator itLastRoads = cLastRoads.begin(); itLastRoads != cLastRoads.end(); ++itLastRoads )
	{
		for( CTargetItems::const_iterator itLastSegGrad = cLastSegGrad.begin(); itLastSegGrad != cLastSegGrad.end(); ++itLastSegGrad )
		{
			if( itLastSegGrad->GetAttribute( _T("LINK_ID") ) != CComVariant(itLastRoads->GetOID()) )
				continue;
			// 道路の形状の中にリンク内属性・傾斜の形状が完全に含まれているなら問題ないので次を見る
			if( AheIsContain( itLastRoads->GetShape(), itLastSegGrad->GetShape() ) ) continue;

			// [bug 1755] 自己交差している道路リンクでは必ず失敗するので、SimplifyGeometryをかけて再挑戦
			IRelationalOperatorPtr ipRoadTmp = itLastRoads->GetShapeCopy();
			IGeometryPtr ipSegGradTmp = itLastSegGrad->GetShapeCopy();
			VARIANT_BOOL vb = VARIANT_FALSE;
			if( ipRoadTmp && ipSegGradTmp )
			{
				ITopologicalOperator2Ptr ipTopo1 = ipRoadTmp, ipTopo2 = ipSegGradTmp;
				if( ipTopo1 && ipTopo2 )
				{
					ipTopo1->put_IsKnownSimple( VARIANT_FALSE );
					ipTopo2->put_IsKnownSimple( VARIANT_FALSE );
					ipTopo1->Simplify();
					ipTopo2->Simplify();
				}
				ipRoadTmp->Contains( ipSegGradTmp, &vb );
				if( vb ) 
					ATLTRACE(_T("CEditHelper::SplitFeaturesByGeomtry() : 道路リンク[%d] リンク内属性or傾斜[%d] : Simplify後の再挑戦でContains成功\n"),
						itLastRoads->GetOID(), itLastSegGrad->GetOID() );
				else {
#ifdef _DEBUG
					CString strMsg;
					CString strSegGrad = ( itLastSegGrad->GetFeatureClassName().CompareNoCase(_T("GRAD") ) == 0 ) ? _T("傾斜") : _T("リンク内属性");
					strMsg.Format(_T("道路リンク：%d %s：%d : %sの形状が道路リンクから離れています。"),
						itLastRoads->GetOID(), strSegGrad, itLastSegGrad->GetOID(), strSegGrad );
					ATLTRACE(_T("\n"), strMsg );

					strMsg.Format(_T("形状不一致（Simplify前）[%s:%d]"), itLastRoads->GetFeatureClassName(), itLastRoads->GetOID() );
					AheTraceGeometry( itLastRoads->GetShape(), strMsg );
					strMsg.Format(_T("形状不一致（Simplify前）[%s:%d]"), itLastSegGrad->GetFeatureClassName(), itLastSegGrad->GetOID() );
					AheTraceGeometry( itLastSegGrad->GetShape(), strMsg );

					strMsg.Format(_T("形状不一致（Simplify後）[%s:%d]"), itLastRoads->GetFeatureClassName(), itLastRoads->GetOID() );
					AheTraceGeometry( (IGeometryPtr)ipRoadTmp, strMsg );
					strMsg.Format(_T("形状不一致（Simplify後）[%s:%d]"), itLastSegGrad->GetFeatureClassName(), itLastSegGrad->GetOID() );
					AheTraceGeometry( (IGeometryPtr)ipSegGradTmp, strMsg );
#ifdef _DEBUG_FOR_2059
					AddGeometryToShapeFile( _T("C:\\Home\\furu2\\work\\src-svn\\SiNDY-e\\SketchTool"), _T("SPLITTED"), _T("道路リンク"), itLastRoads->GetShapeCopy(), false, false, false );
					AddGeometryToShapeFile( _T("C:\\Home\\furu2\\work\\src-svn\\SiNDY-e\\SketchTool"), _T("SPLITTED"), _T("傾斜・リンク内属性"), itLastSegGrad->GetShapeCopy(), false, false, false );
#endif // ifdef _DEBUG_FOR_2059
#endif // ifdef _DEBUG
				}
			}
			if( ! vb )
			{
				CString strMsg;
				CString strSegGrad = ( itLastSegGrad->GetFeatureClassName().CompareNoCase(_T("GRAD") ) == 0 ) ? SLOPE : ROAD_STRUCTURE_ATTRIBUTE;
				strMsg.Format(ERROR_SHAPE_IS_APART_FROM_ROAD_LINK, itLastRoads->GetOID(), strSegGrad, itLastSegGrad->GetOID(), strSegGrad );
				MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
#ifndef _DEBUG
				return FALSE;
#endif // ifdef _DEBUG
			}
		}
	}
	return TRUE;
}

BOOL CEditHelper::CheckLQConnect(const CTargetItems& cNewFeatures, CTargetItems& cLastRoads, const std::list<ROWDEF>& listLQRows) const
{
	// cNewFeaturesはここでしか使っていない
	if( cNewFeatures.empty() ) return TRUE;

#ifdef _DEBUG
	ATLTRACE(_T("###########################################################\n"));
	ATLTRACE(_T("リンク列のチェックを開始します\n"));
	DWORD dwSTART = GetTickCount();
#endif // ifdef _DEBUG
	VARIANT_BOOL vbRecycle = VARIANT_FALSE;	// LQの修復が入る場合があるため
	// [bug 3521] リンク列のスプリット処理がおかしい問題のためのチェック
	CTargetItems::iterator itLastRoad = cLastRoads.begin();
	if( itLastRoad == cLastRoads.end() ) return TRUE;

	IDatasetPtr ipDataset( itLastRoad->GetFeatureClass() );
	IWorkspacePtr ipWorkspace;
	GetEditor()->get_EditWorkspace( &ipWorkspace );

	// 道路リンクのオーナー名を取得
	CString strOwnerName = AheGetFeatureClassOwnerName( itLastRoad->GetFeatureClass() );

	std::list<long> listRoadID;
	///////////////////////////////////////////////////////////////////////////////////
	// 該当する LQ を検索
	///////////////////////////////////////////////////////////////////////////////////
	std::map<long, std::map<long, std::list<LQDEF> > > mapLQ;	// first key : kind of INF, second key : INF ID

	// LQを検索するためのクエリフィルタ作成
	IQueryFilterPtr ipQuery( CLSID_QueryFilter ), ipQueryLQ( CLSID_QueryFilter );
	ipQuery->AddField( CComBSTR(_T("INF_ID")) );
	ipQueryLQ->AddField( CComBSTR(_T("INF_ID")) );
	ipQueryLQ->AddField( CComBSTR(_T("LINK_ID")) );
	ipQueryLQ->AddField( CComBSTR(_T("OBJECTID")) );
	ipQueryLQ->AddField( CComBSTR(_T("SEQUENCE")) );
	ipQueryLQ->AddField( CComBSTR(_T("LASTLINK_F")) );
	ipQueryLQ->AddField( CComBSTR(_T("LINKDIR_C")) );

	// 道路リンクのIDで検索するためのWhere句を作成
	std::list<CString> listWhere;
	static const long MAX_OID_LEN = 27;			// OIDは27桁が最大って白石君が言ってた
	static const long MAX_IN_OID_NUM = 1000;	// INのID数は1000が最大って秦君が言ってた
	static const long MAX_IN_LEN = MAX_OID_LEN * MAX_IN_OID_NUM;
	static const size_t IN_BUFFER_SIZE = sizeof(TCHAR) * MAX_IN_LEN;
	static const long MAX_IN_LEN_CANADD = MAX_IN_LEN - ( MAX_OID_LEN + 1 );
	TCHAR strWhere[MAX_IN_LEN];
	LPTSTR lpszWhereEnd = strWhere;
	FillMemory( strWhere, IN_BUFFER_SIZE, _T('\0') );
	for( CTargetItems::iterator itRoad = cLastRoads.begin(), itRoadEnd = cLastRoads.end(); itRoad != itRoadEnd; ++itRoad )
	{
		lstrcat( lpszWhereEnd, ( 0 < lstrlen( strWhere ) ) ? _T(",") : _T("LINK_ID IN (") );
		lpszWhereEnd = &strWhere[lstrlen( strWhere )];

		_ltot( itRoad->GetOID(), lpszWhereEnd, 10 );
		lpszWhereEnd = &strWhere[lstrlen( strWhere )];
		if( lstrlen( strWhere ) > MAX_IN_LEN_CANADD/* ",OID一つ"の分*/ )
		{
			lstrcat( lpszWhereEnd, _T(")") );
			listWhere.push_back( strWhere );
			FillMemory( (LPTSTR)strWhere, IN_BUFFER_SIZE, _T('\0') );
			lpszWhereEnd = strWhere;
		}
	}
	if( 0 < lstrlen( strWhere ) )
	{
		lstrcat( lpszWhereEnd, _T(")") );
		listWhere.push_back( strWhere );
	}
#ifdef _DEBUG
	DWORD dwSTART2 = GetTickCount();
	ATLTRACE(_T("INFO : 前準備時間 : %.3lf\n"), ((double)(dwSTART2 - dwSTART))/1000.0 );
#endif // ifdef _DEBUG
	for ( INT i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ ) 
	{
#ifdef _DEBUG
		DWORD dwLQSTART = GetTickCount();
#endif // ifdef _DEBUG
		CString strLQTableName        = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQTableName        );
		CString strLQRFFieldName      = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName  );

		// リンク列のテーブル取得（道路が存在するワークスペースから取らないとダメ）
		ITablePtr ipLQTable = AheOpenTableByTableName( ipWorkspace, ( strOwnerName.IsEmpty() ) ? strLQTableName : strOwnerName + _T(".") + strLQTableName );

		if( NULL != ipLQTable )
		{
			// INFのIDを取得
			std::list<long> listInfIDs;	// INFのID格納用
			for( std::list<CString>::const_iterator it = listWhere.begin(), itEnd = listWhere.end(); it != itEnd; ++it )
			{
				_ICursorPtr ipCursor;
				ipQuery->put_WhereClause( CComBSTR( *it ) );
				ipLQTable->Search( ipQuery, vbRecycle, &ipCursor );

				if( NULL != ipCursor )
				{
					// 高速化のためインデックス番号を取得しておく
					long lIdx_InfID = -1;
					ipCursor->FindField( CComBSTR(_T("INF_ID")), &lIdx_InfID );

					_IRowPtr ipRow;
					CComVariant va;

					while( S_OK == ipCursor->NextRow( &ipRow ) )
					{
						if( NULL == ipRow ) break;

						ipRow->get_Value( lIdx_InfID, &va );
						listInfIDs.push_back( va.lVal );
					}
				}
				ipCursor = NULL;
			}
			listInfIDs.sort();		// 重複防止
			listInfIDs.unique();	// 重複防止

			// 取得したINFのIDで検索するためのWhere句を作成
			std::list<CString> listWhereINFID;
			lpszWhereEnd = strWhere;
			FillMemory( strWhere, IN_BUFFER_SIZE, _T('\0') );
			for( std::list<long>::iterator itInfID = listInfIDs.begin(), itInfIDEnd = listInfIDs.end(); itInfID != itInfIDEnd; ++itInfID )
			{
				lstrcat( lpszWhereEnd, ( 0 < lstrlen( strWhere ) ) ? _T(",") : _T("INF_ID IN (") );
				lpszWhereEnd = &strWhere[lstrlen( strWhere )];

				_ltot( *itInfID, lpszWhereEnd, 10 );
				lpszWhereEnd = &strWhere[lstrlen( strWhere )];
				if( lstrlen( strWhere ) > MAX_IN_LEN_CANADD )
				{
					lstrcat( lpszWhereEnd, _T(")") );
					listWhereINFID.push_back( strWhere );
					FillMemory( (LPTSTR)strWhere, IN_BUFFER_SIZE, _T('\0') );
					lpszWhereEnd = strWhere;
				}
			}
			if( 0 < lstrlen( strWhere ) )
			{
				lstrcat( lpszWhereEnd, _T(")") );
				listWhereINFID.push_back( strWhere );
			}

			// 取得したINFのIDでLQを検索する
			for( std::list<CString>::const_iterator itWhere = listWhereINFID.begin(), itWhereEnd = listWhereINFID.end(); itWhere != itWhereEnd; ++itWhere )
			{
				_ICursorPtr ipCursor;
				ipQueryLQ->put_WhereClause( CComBSTR( *itWhere ) );
				ipLQTable->Search( ipQueryLQ, vbRecycle, &ipCursor );

				if( NULL != ipCursor )
				{						
					long lIdx_LinkID = -1, lIdx_Seq = -1, lIdx_LastLinkF = -1, lIdx_LinkDirC = -1, lIdx_InfID = -1;
					ipCursor->FindField( CComBSTR(_T("LINK_ID")), &lIdx_LinkID );
					ipCursor->FindField( CComBSTR(_T("SEQUENCE")), &lIdx_Seq );
					ipCursor->FindField( CComBSTR(_T("LASTLINK_F")), &lIdx_LastLinkF );
					ipCursor->FindField( CComBSTR(_T("LINKDIR_C")), &lIdx_LinkDirC );
					ipCursor->FindField( CComBSTR(_T("INF_ID")), &lIdx_InfID );

					_IRowPtr ipRow;
					CComVariant va;
					LQDEF def;

					// 初期化
					def.oid = def.linkid = def.seq = def.dir = def.infid = -1;
					def.last = false;
					def.kind = (sindyeRoadQueueType)i;

					// 先に、スプリット処理で使用したlistLQRowsを格納する
					// そうしないと、まだ編集セッションが終了していないため
					// 検索しても変更結果が反映されていない
					for( std::list<ROWDEF>::const_iterator it = listLQRows.begin(), itEnd = listLQRows.end(); it != itEnd; ++it )
					{
						if( i == m_cLoadQueueInfo.GetLoadQueueType( it->flg ) )
						{
							if( i == m_cLoadQueueInfo.GetLoadQueueType( it->flg ) )
							{
								ipRow = it->obj;

								ipRow->get_OID( &def.oid );
								ipRow->get_Value( lIdx_LinkID,    &va ); def.linkid = va.lVal;
								ipRow->get_Value( lIdx_Seq,       &va ); def.seq    = va.lVal;
								ipRow->get_Value( lIdx_LinkDirC,  &va ); def.dir    = va.lVal;
								ipRow->get_Value( lIdx_LastLinkF, &va ); def.last   = ( 0 == va.lVal ) ? false : true;
								ipRow->get_Value( lIdx_InfID,     &va ); def.infid  = va.lVal;
								def.obj = ipRow;
#ifdef _LQCHECK_TRACE_LQ
								ATLTRACE(_T("INFO : LQ登録（検索済） : ") );
								TraceLQDEF( def );
#endif // ifdef _LQCHECK_TRACE_LQ

								mapLQ[def.kind][def.infid].push_back( def );

								listRoadID.push_back( def.linkid );	// リンクのIDはここで追加しておく
							}
						}
					}

					// 検索したものを取得
					while( S_OK == ipCursor->NextRow( &ipRow ) )
					{
						if( NULL == ipRow ) break;

						ipRow->get_OID( &def.oid );
						ipRow->get_Value( lIdx_LinkID,    &va ); def.linkid = va.lVal;
						ipRow->get_Value( lIdx_Seq,       &va ); def.seq    = va.lVal;
						ipRow->get_Value( lIdx_LinkDirC,  &va ); def.dir    = va.lVal;
						ipRow->get_Value( lIdx_LastLinkF, &va ); def.last   = ( 0 == va.lVal ) ? false : true;
						ipRow->get_Value( lIdx_InfID,     &va ); def.infid  = va.lVal;
						def.obj = ipRow;

						// 同じ物がある場合は捨てる
						std::list<LQDEF>& listLQDefTmp = mapLQ[def.kind][def.infid];
						if( listLQDefTmp.end() == find( listLQDefTmp.begin(), listLQDefTmp.end(), def ) )
						{
#ifdef _LQCHECK_TRACE_LQ
							ATLTRACE(_T("INFO : LQ登録（新検索） : ") );
							TraceLQDEF( def );
#endif // ifdef _LQCHECK_TRACE_LQ

							listLQDefTmp.push_back( def );

							listRoadID.push_back( def.linkid );	// リンクのIDはここで追加しておく
						}
#ifdef _LQCHECK_TRACE_LQ 
						else {
							ATLTRACE(_T("INFO : 同じLQがあったので登録しません : ") );
							TraceLQDEF( def );
						}
#endif // ifdef _LQCHECK_TRACE_LQ
					}
					ipCursor = NULL;
				}
			}
#ifdef _DEBUG
			ATLTRACE(_T("INFO :   LQ検索（%d） : %.3lf\n"), i, ((double)(GetTickCount()-dwLQSTART))/1000.0 );
#endif // ifdef _DEBUG
		}
	}
#ifdef _DEBUG
	DWORD dwSTART3 = GetTickCount();
	ATLTRACE(_T("INFO : LQ検索 : %.3lf\n"), ((double)(dwSTART3-dwSTART2))/1000.0 );
	dwSTART2 = dwSTART3;
#endif // ifdef _DEBUG
	///////////////////////////////////////////////////////////////////////////////////
	// 検索したLQに関連する道路リンクを選択
	///////////////////////////////////////////////////////////////////////////////////
	listRoadID.sort();
	listRoadID.unique();

	// 高速化の為にインデックス番号を取っておく
	std::map<sindyeRoadQueueType,long> mapLQRFIndex;
	ITablePtr ipTable = cLastRoads.begin()->GetFeatureClass();
	IFieldsPtr ipFields;
	ipTable->get_Fields( &ipFields );
	for( int i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; ++i )
	{
		if( FAILED( ipFields->FindField( CComBSTR( m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName ) ), &mapLQRFIndex[(sindyeRoadQueueType)i] ) ) )
			mapLQRFIndex[(sindyeRoadQueueType)i] = -1; // リンク列の存在は任意（iPC VICS対応）
	}

	// 既に選択されているものに関しては先に取っておく
	ROADDEF def;
	std::map<long, ROADDEF> mapRoad;
	IFeaturePtr ipFeat;
	CComVariant va;
	for( CTargetItems::const_iterator itRoad = cLastRoads.begin(), itRoadEnd = cLastRoads.end(); itRoad != itRoadEnd; ++itRoad )
	{
		ipFeat = itRoad->GetFeature();
		ipFeat->get_OID( &def.oid );
		ipFeat->get_ShapeCopy( &def.shape );

		def.rf = 0;
		for( std::map<sindyeRoadQueueType,long>::const_iterator itIndex = mapLQRFIndex.begin(); itIndex != mapLQRFIndex.end(); ++itIndex )
		{
			if( 0 < itIndex->second ) // リンク列の存在は任意（iPC VICS対応）
			{
				ipFeat->get_Value( itIndex->second, &va );
				def.rf = ( 0 == va.lVal ) ? def.rf : def.rf | m_cLoadQueueInfo.GetLoadQueueDrawMode(itIndex->first);
			}
		}
		mapRoad[def.oid] = def;

		// リストに登録されていたら削除
		listRoadID.remove( def.oid );
	}

	// 取得したINFのIDで検索するためのWhere句を作成
	std::list<CString> listWhereRoadID;
	lpszWhereEnd = strWhere;
	FillMemory( strWhere, IN_BUFFER_SIZE, _T('\0') );
	for( std::list<long>::iterator itRoadID = listRoadID.begin(), itRoadIDEnd = listRoadID.end(); itRoadID != itRoadIDEnd; ++itRoadID )
	{
		lstrcat( lpszWhereEnd, ( 0 < lstrlen( strWhere ) ) ? _T(",") : _T("OBJECTID IN (") );
		lpszWhereEnd = &strWhere[lstrlen( strWhere )];

		_ltot( *itRoadID, lpszWhereEnd, 10 );
		lpszWhereEnd = &strWhere[lstrlen( strWhere )];
		if( lstrlen( strWhere ) > MAX_IN_LEN_CANADD )
		{
			lstrcat( lpszWhereEnd, _T(")") );
			listWhereRoadID.push_back( strWhere );
			FillMemory( (LPTSTR)strWhere, IN_BUFFER_SIZE, _T('\0') );
			lpszWhereEnd = strWhere;
		}
	}
	if( 0 < lstrlen( strWhere ) )
	{
		lstrcat( lpszWhereEnd, _T(")") );
		listWhereRoadID.push_back( strWhere );
	}
	// クエリフィルタ作成
	CString strSubFields(_T("OBJECTID,SHAPE,SHAPE.LEN"));
	for( std::map<sindyeRoadQueueType,long>::const_iterator itIndex = mapLQRFIndex.begin(); itIndex != mapLQRFIndex.end(); ++itIndex )
	{
		if( 0 < itIndex->second ) // LQRFフィールドが存在しない場合を考慮
			strSubFields += _T(",") + m_cLoadQueueInfo.GetLoadQueueInfo( itIndex->first, sindyeLoadQueueInfoLQRFRoadFieldName );
	}
	ipQuery->put_SubFields( CComBSTR(strSubFields) );

	// 検索
	for( std::list<CString>::const_iterator itWhereRoadID = listWhereRoadID.begin(), itWhereRoadIDEnd = listWhereRoadID.end(); itWhereRoadID != itWhereRoadIDEnd; ++itWhereRoadID )
	{
		ipQuery->put_WhereClause( CComBSTR(*itWhereRoadID) );

		_ICursorPtr ipCursor;
		ipTable->Search( ipQuery, vbRecycle, &ipCursor );

		if( NULL != ipCursor )
		{
			_IRowPtr ipRow;
			while( S_OK == ipCursor->NextRow( &ipRow ) ) 
			{
				ipFeat = ipRow;

				if( NULL == ipFeat ) break;

				ipFeat->get_OID( &def.oid );
				ipFeat->get_ShapeCopy( &def.shape );

				def.rf = 0;
				for( std::map<sindyeRoadQueueType,long>::const_iterator itIndex = mapLQRFIndex.begin(); itIndex != mapLQRFIndex.end(); ++itIndex )
				{
					if( 0 < itIndex->second ) // LQRFフィールドが存在しない場合を考慮
					{
						ipFeat->get_Value( itIndex->second, &va );
						def.rf = ( 0 == va.lVal ) ? def.rf : def.rf | m_cLoadQueueInfo.GetLoadQueueDrawMode(itIndex->first);
					}
				}
				mapRoad[def.oid] = def;
			}
		}
	}
#ifdef _DEBUG
	dwSTART3 = GetTickCount();
	ATLTRACE(_T("INFO : 道路リンク検索 : %.3lf\n"), ((double)(dwSTART3-dwSTART2))/1000.0 );
	dwSTART2 = dwSTART3;
#endif // ifdef _DEBUG
LQ_CHECK:
	///////////////////////////////////////////////////////////////////////////////////
	// チェック開始
	///////////////////////////////////////////////////////////////////////////////////
	// 関連フラグとLQの整合が取れているか
	//   ・CTargetItemsの道路リンクに関連フラグが付いていないのにLQがあるならおかしい
	//   ・CTargetItemsの道路リンクに関連フラグが付いているのにLQが無いのはおかしい
	//
	// LQの形状が一本のラインになるか
	//   ・シーケンスの順番及び方向コードのとおりにつながらなければおかしい
	//
	// シーケンスの並びの整合が取れているか
	//   ・0から始まっていなければおかしい
	//   ・番号が飛んでいるのはおかしい
	//   ・最後のLQは終端フラグが無ければおかしい
	for( std::map<long, std::map<long, std::list<LQDEF> > >::iterator it = mapLQ.begin(), itEnd = mapLQ.end(); it != itEnd; ++it )
	{
		for( std::map<long, std::list<LQDEF> >::iterator it2 = it->second.begin(), it2End = it->second.end(); it2 != it2End; ++it2 )
		{
			it2->second.sort();								// シーケンス順にソート
			it2->second.unique();

			long lSeq = 1;
			IPointPtr ipPrevEndPoint;
			for( std::list<LQDEF>::const_iterator itLQ = it2->second.begin(), itLQEnd = it2->second.end(); itLQ != itLQEnd; ++itLQ, ++lSeq )
			{
				// 関連フラグが付いているかどうかのチェック
				std::map<long,ROADDEF>::const_iterator it = mapRoad.find(itLQ->linkid);
				if( mapRoad.end() != it )
				{
					if( ! ( it->second.rf & m_cLoadQueueInfo.GetLoadQueueDrawMode(itLQ->kind) ) )
					{
						// 修復
						if( FixLQSequence( it2->second, mapRoad ) )
						{
							// もう一度最初からチェック
							goto LQ_CHECK;
						}
						else {
							// エラー
							CString strMsg;
							strMsg.Format(ERROR_FLAG_DOES_NOT_COME_WITH_ROAD_LINK, itLQ->oid, itLQ->linkid, m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)itLQ->kind, sindyeLoadQueueInfoLQRFRoadFieldName ) );
							ATLTRACE(_T("ERROR : %s\n"), strMsg );
							MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
							return FALSE;
						}
					}
				}
				else {
					// 修復
					if( FixLQSequence( it2->second, mapRoad ) )
					{
						// もう一度最初からチェック
						goto LQ_CHECK;
					}
					else {
						// エラー
						CString strMsg;
						strMsg.Format(ERROR_ROAD_LINK_NOT_EXIST_OR_SEARCH_FAIL, itLQ->oid, itLQ->linkid );
						ATLTRACE(_T("ERROR : %s\n"), strMsg );
						MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
						return FALSE;
					}
				}

				// シーケンス番号の整合チェック
				if( itLQ->seq != lSeq )
				{
					// 修復
					if( FixLQSequence( it2->second, mapRoad ) )
					{
						// もう一度最初からチェック
						goto LQ_CHECK;
					}
					else {
						// エラー
						CString strMsg;
						strMsg.Format(ERROR_SOMETHING_WRONG_WITH_SEQUENCE_NUMBER );
						ATLTRACE(_T("ERROR : %s\n"), strMsg );
						for_each( it2->second.begin(), it2->second.end(), TraceLQDEF );

						for( std::list<LQDEF>::iterator it3 = it2->second.begin(), it3End = it2->second.end(); it3 != it3End; ++it3 )
							strMsg += GetTraceLQDEFString( *it3 ) + _T("\n");
						MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
						return FALSE;
					}
				}
				// LQと関連づいている道路リンクの端点同士が接しているかどうかチェック
				// 現在のLQの開始点を取得
				IPolylinePtr ipRoadLine( mapRoad[itLQ->linkid].shape );
				if( NULL != ipRoadLine )
				{
					IPointPtr ipFirst, ipEnd;
					ipRoadLine->get_FromPoint( ( 1 == itLQ->dir ) ? &ipFirst : &ipEnd   );
					ipRoadLine->get_ToPoint(   ( 1 == itLQ->dir ) ? &ipEnd   : &ipFirst );
					if( NULL != ipPrevEndPoint )
					{
/*								// [bug 3569 後半] 空間参照に一致させないと、スナップしないでスプリットした場合に
						// どうしても合わなくなる
						ipPrevEndPoint->SnapToSpatialReference();
						ipFirst->SnapToSpatialReference();

						double dx1, dx2, dy1, dy2;
						ipPrevEndPoint->QueryCoords( &dx1, &dy1 );
						ipFirst->QueryCoords( &dx2, &dy2 );
						// 現在のLQの開始点と一つ前の終了点が同じかどうか
						if( dx1 != dx2 || dy1 != dy2 )*/
						if( ! AheIsEqual( ipPrevEndPoint, ipFirst ) )
						{
							// 修復
							if( FixLQSequence( it2->second, mapRoad ) )
							{
								// もう一度最初からチェック
								goto LQ_CHECK;
							}
							else {
								// エラー
								CString strMsg;
								strMsg.Format(ERROR_START_POINT_AND_BEFORE_IS_NOT_SAME );
								ATLTRACE(_T("ERROR : %s"), strMsg );
								for_each( it2->second.begin(), it2->second.end(), TraceLQDEF );

								for( std::list<LQDEF>::iterator it3 = it2->second.begin(), it3End = it2->second.end(); it3 != it3End; ++it3 )
									strMsg += GetTraceLQDEFString( *it3 ) + _T("\n");
								MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
								return FALSE;
							}
						}
					}
					ipPrevEndPoint = ipEnd;
				}
				else {
					// アルゴリズムエラー
					CString strMsg;
					strMsg.Format(ERROR_NO_SHAPE_OF_ROAD_LINK, mapRoad[itLQ->linkid].oid );
					ATLTRACE(_T("ALGORITHM ERROR : %s\n"), strMsg );
					MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
			}
			// シーケンスの最後に終端フラグがあるかどうかのチェック
			if( ! it2->second.rbegin()->last )
			{
				// 修復
				if( FixLQSequence( it2->second, mapRoad ) )
				{
					// もう一度最初からチェック
					goto LQ_CHECK;
				}
				else {
					// エラー
					CString strMsg;
					strMsg.Format(ERROR_TERMINATION_FLAG_NOT_EXIST );
					ATLTRACE(_T("ERROR : %s"), strMsg );
					for_each( it2->second.begin(), it2->second.end(), TraceLQDEF );

					for( std::list<LQDEF>::iterator it3 = it2->second.begin(), it3End = it2->second.end(); it3 != it3End; ++it3 )
						strMsg += GetTraceLQDEFString( *it3 ) + _T("\n");
					MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
			}
		}
	}
	// 道路リンクに不要なLQRFが付いていないかどうかチェック
	for( CTargetItems::iterator itRoad = cLastRoads.begin(), itRoadEnd = cLastRoads.end(); itRoad != itRoadEnd; ++itRoad )
	{
		const ROADDEF& def = mapRoad[itRoad->GetOID()];

		for ( int i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ ) 
		{
			long lDrawMode = m_cLoadQueueInfo.GetLoadQueueDrawMode( (sindyeRoadQueueType)i );
			if( def.rf & lDrawMode )
			{
				bool bFindLQ = false;
				for( std::map<long, std::list<LQDEF> >::iterator itLQs = mapLQ[i].begin(), itLQsEnd = mapLQ[i].end(); itLQs != itLQsEnd; ++itLQs )
				{
					for( std::list<LQDEF>::iterator itLQ = itLQs->second.begin(), itLQEnd = itLQs->second.end(); itLQ != itLQEnd; ++itLQ )
					{
						if( def.oid == itLQ->linkid )
						{
							bFindLQ = true;
							break;
						}
					}
					if( bFindLQ ) break;
				}
				if( ! bFindLQ )
				{
					// 警告
					// [bug 3521] リンク列処理に手を加えるのは面倒なので、ここで属性を削除する
					ATLTRACE(_T("WARNING : 関連するLQが存在しないのに道路リンク（%ld）に関連フラグ（%d）がついていますのでフラグを削除します\n"), def.oid, i );
					itRoad->SetAttribute( m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName ), 0L );
				}
			}
		}
	}
#ifdef _DEBUG
	DWORD dwEND = GetTickCount();
	ATLTRACE(_T("INFO : チェック時間 : %.3lf\n"), ((double)(dwEND-dwSTART2))/1000.0 );
	ATLTRACE(_T("INFO : リンク列のチェック完了：時間[%.3lf秒]\n"), ((double)( dwEND - dwSTART ))/1000.0 );
	ATLTRACE(_T("###########################################################\n"));
#endif // ifdef _DEBUG
	return TRUE;
}
