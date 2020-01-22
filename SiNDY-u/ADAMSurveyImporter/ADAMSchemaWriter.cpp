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

#include <sindy/workspace.h>
#include <TDC/useful_headers/scope_guard.h>
#include "LogUtil.h"

//↓ユーザ名取得用
#define SECURITY_WIN32
#include <sspi.h>
#include <security.h>
//↑

#include "ADAMSchemaWriter.h"

using namespace adam_v2;

ADAMSchemaWriter::ADAMSchemaWriter(void)
{
	_schema = std::make_shared<AdamSchema>();
}


ADAMSchemaWriter::~ADAMSchemaWriter(void)
{
}

void ADAMSchemaWriter::Clear(void)
{
	//_workspace = nullptr;
	SchemaWriter::Clear();

	_schema->Clear();
	_csv->Clear();
}

void ADAMSchemaWriter::SetCSV(const SurveyCSVPtr& csv)
{
	_csv = csv;
}

bool ADAMSchemaWriter::SetSpatialReferenceId()
{
	if(!_csv->listSpatialRef.size() == 1)
		return false;

	return SetSRID(_csv->listSpatialRef.begin()->srid.GetValue());
}

bool ADAMSchemaWriter::StartEdit()
{
	if(!_workspace)
		return false;

	return SchemaWriter::StartEdit(IWorkspaceEditPtr(_workspace));
}
bool ADAMSchemaWriter::StopEdit()
{
	if(!_workspace)
		return false;

	return SchemaWriter::StopEdit(IWorkspaceEditPtr(_workspace));
}
bool ADAMSchemaWriter::AbortEdit()
{
	if(!_workspace)
		return false;

	return SchemaWriter::AbortEdit(IWorkspaceEditPtr(_workspace));
}

bool ADAMSchemaWriter::Write(const uh::tstring& tableName)
{
	LogUtil::printDoubleLine();
	LogUtil::print("DBへのデータインポートを開始します");

	bool ret = true;

	try
	{
		//インポート処理
		//ノード、リンク、ノード列、関連テーブル の順に処理

			 if( tableName == AdamSchema::LaneLinkGroup::TableName )			ret &=	Insert( &ADAMSchemaWriter::InsertLaneLinkGroup,				tableName );	// 車線リンクグループ
		else if( tableName == AdamSchema::LaneNode::TableName )					ret &=	Insert( &ADAMSchemaWriter::InsertLaneNode,					tableName );	// 車線ノード
		else if( tableName == AdamSchema::LaneLink::TableName )					ret &=	Insert( &ADAMSchemaWriter::InsertLaneLink,					tableName );	// 車線リンク
		else if( tableName == AdamSchema::NQLaneNode::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertNQLaneNode,				tableName );	// 車線ノード列
		else if( tableName == AdamSchema::RelRoadLinkLaneLink::TableName )				Insert( &ADAMSchemaWriter::InsertRelRoadLinkLaneLink,		tableName );	// 車線関連
		else if( tableName == AdamSchema::CompartNode::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertCompartNode,				tableName );	// 区画線ノード
		else if( tableName == AdamSchema::CompartLink::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertCompartLink,				tableName );	// 区画線リンク
		else if( tableName == AdamSchema::NQCompartNode::TableName )			ret &=	Insert( &ADAMSchemaWriter::InsertNQCompartNode,				tableName );	// 区画線ノード列
		else if( tableName == AdamSchema::RelCompartLinkLaneLink::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelCompartLinkLaneLink,	tableName );	// 区画線関連
		else if( tableName == AdamSchema::BorderNode::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertBorderNode,				tableName );	// 境界線ノード
		else if( tableName == AdamSchema::BorderLink::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertBorderLink,				tableName );	// 境界線リンク
		else if( tableName == AdamSchema::NQBorderNode::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertNQBorderNode,				tableName );	// 境界線ノード列
		else if( tableName == AdamSchema::RelBorderLinkLaneLink::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelBorderLinkLaneLink,		tableName );	// 境界線関連
		else if( tableName == AdamSchema::SignalPoint::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertSignalPoint,				tableName );	// 信号機
		else if( tableName == AdamSchema::RelLaneNodeSignalPoint::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelLaneNodeSignalPoint,	tableName );	// 信号機関連
		else if( tableName == AdamSchema::RoadSignPoint::TableName )			ret &=	Insert( &ADAMSchemaWriter::InsertRoadSignPoint,				tableName );	// 道路標識
		else if( tableName == AdamSchema::RelLaneNodeRoadSignPoint::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelLaneNodeRoadSignPoint,	tableName );	// 道路標識関連
		else if( tableName == AdamSchema::CrossWalkLine::TableName )			ret &=	Insert( &ADAMSchemaWriter::InsertCrossWalkLine,				tableName );	// 横断歩道
		else if( tableName == AdamSchema::RelLaneNodeCrossWalkLine::TableName )	ret &=	Insert( &ADAMSchemaWriter::InsertRelLaneNodeCrossWalkLine,	tableName );	// 横断歩道関連
		else if( tableName == AdamSchema::VirtualCompartLine::TableName )				Insert( &ADAMSchemaWriter::InsertVirtualCompartLine,		tableName );	// 仮想線リンク
		else if( tableName == AdamSchema::CoveringArea::TableName )				ret &=	Insert( &ADAMSchemaWriter::InsertCoveringArea,				tableName );	// 覆い物
		else _ASSERT( false );

		if(!ret)
		{
			LogUtil::printError("インポート処理に失敗しました。編集を破棄します");
		}
	}
	catch(const _com_error& e)
	{
		LogUtil::printError("異常終了しました");
		LogUtil::printError("例外検出\t" +  uh::toStr(atl2::stringize(e)));
		LogUtil::printError("編集を破棄します");
		LogUtil::printError("インポート処理に失敗しました。ログを確認してください");
		return false;
	}
	catch(const std::exception& e)
	{
		LogUtil::printError("異常終了しました");
		LogUtil::printError("例外検出\t" +  uh::toStr(e.what()));
		LogUtil::printError("編集を破棄します");
		LogUtil::printError("インポート処理に失敗しました。ログを確認してください");
		return false;
	}
	catch(...)
	{
		LogUtil::printError("異常終了しました");
		LogUtil::printError("例外検出");
		LogUtil::printError("編集を破棄します");
		LogUtil::printError("インポート処理に失敗しました。ログを確認してください");
		return false;
	}

	if(!ret)
	{
		LogUtil::printError("インポート処理に失敗しました。ログを確認してください");
	}	
	else
	{
		LogUtil::print("インポート処理に成功しました");
	}

	LogUtil::printDoubleLine();

	return ret;
}

bool ADAMSchemaWriter::ConnectDB()
{
	LogUtil::print("DBへの接続を確認します");

	bool ret = true;

	//ワークスペース接続
	if(!SetWorkspace())
	{
		ret = false;
	}
	if(!_schema)
	{
		ret = false;
	}
	if(!_workspace)
	{
		ret = false;
	}

	//利用予定のADAMv2に全部接続
	ret &= SetFeature(	AdamSchema::LaneLink::TableName,					&(_schema->featureLaneLink)					);
	ret &= SetFeature(	AdamSchema::LaneNode::TableName,					&(_schema->featureLaneNode)					);
	ret &= SetFeature(	AdamSchema::CompartLink::TableName,					&(_schema->featureCompartLink)				);
	ret &= SetFeature(	AdamSchema::CompartNode::TableName,					&(_schema->featureCompartNode)				);
	ret &= SetFeature(	AdamSchema::BorderLink::TableName,					&(_schema->featureBorderLink)				);
	ret &= SetFeature(	AdamSchema::BorderNode::TableName,					&(_schema->featureBorderNode)				);
	ret &= SetFeature(	AdamSchema::SignalPoint::TableName,					&(_schema->featureSignalPoint)				);
	ret &= SetFeature(	AdamSchema::CrossWalkLine::TableName,				&(_schema->featureCrossWalkLine)			);
	ret &= SetFeature(	AdamSchema::RoadSignPoint::TableName,				&(_schema->featureRoadSignPoint)			);
	ret &= SetFeature(	AdamSchema::CoveringArea::TableName,				&(_schema->featureCoveringArea)				);
	SetFeature(	AdamSchema::VirtualCompartLine::TableName,					&(_schema->featureVirtualCompartLine)		);

	ret &= SetTable(	AdamSchema::RelRoadLinkLaneLink::TableName,			&(_schema->tableRelRoadLinkLaneLink)		);
	ret &= SetTable(	AdamSchema::LaneLinkGroup::TableName,				&(_schema->tableLaneLinkGroup)				);
	ret &= SetTable(	AdamSchema::RelCompartLinkLaneLink::TableName,		&(_schema->tableRelCompartLinkLaneLink)		);
	ret &= SetTable(	AdamSchema::RelBorderLinkLaneLink::TableName,		&(_schema->tableRelBorderLinkLaneLink)		);
	ret &= SetTable(	AdamSchema::RelLaneNodeSignalPoint::TableName,		&(_schema->tableRelLaneNodeSignalPoint)		);
	ret &= SetTable(	AdamSchema::RelLaneNodeCrossWalkLine::TableName,	&(_schema->tableRelLaneNodeCrossWalkLine)	);
	ret &= SetTable(	AdamSchema::RelLaneNodeRoadSignPoint::TableName,	&(_schema->tableRelLaneNodeRoadSignPoint)	);
	ret &= SetTable(	AdamSchema::NQLaneNode::TableName,					&(_schema->tableNQLaneNode)					);
	ret &= SetTable(	AdamSchema::NQCompartNode::TableName,				&(_schema->tableNQCompartNode)				);
	ret &= SetTable(	AdamSchema::NQBorderNode::TableName,				&(_schema->tableNQBorderNode)				);
	
	if(ret)
	{
		LogUtil::print("すべてのフィーチャ接続に成功しました");
	}
	else
	{
		LogUtil::print("フィーチャ接続に失敗しました。ログを確認してください");
	}

	return ret;
}

bool ADAMSchemaWriter::Insert(
		bool (ADAMSchemaWriter::*func)(const uh::tstring&), 
		const uh::tstring& tablename
		)
{
	LogUtil::print(uh::toStr( tablename ) + " のインポートを開始します...");

	bool ret = (this->*func)(tablename);

	if(!ret)
	{
		LogUtil::printError("インポートに失敗しました。ログを確認してください\t" + uh::toStr( tablename ));
	}
	else
	{
		LogUtil::print("正常にインポートしました\t" + uh::toStr( tablename ));
	}

	LogUtil::print(uh::toStr( tablename ) + " のインポートを終了します...");

	return ret;
}

bool ADAMSchemaWriter::InsertLaneLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//車線リンク
	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureLaneLink))
	{
		assert(false);
		return false;
	}
	AdamSchema::LaneLink index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	std::map<long long, OGID> dicRegNode;
	{
		//車線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	std::map<long long, OGID> dicRegGroup;
	{
		//車線リンクグループ★
		ITablePtr ipGroupFC;
		if(!(ipGroupFC = _schema->tableLaneLinkGroup))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegGroup, ipGroupFC, _T("SOURCEID"), _T("OBJECTID"), _T(""));
	}

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	IFeatureCursorPtr ipFCursor;
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}


	long count = 0;
	long listSize = _csv->listLaneLink.size();
	//データインポート
	//入れれるものを先にいれる
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listLaneLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(100, "ユニークチェック", "重複オブジェクトのためSKIPします", "車線リンクSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与
		//FROM_NODE_* はあれば入れる
		auto fFromNode = dicRegNode.find(csv.from_node_id.GetValue());
		uh::tstring from_node_gid = (fFromNode != dicRegNode.end()) ? fFromNode->second.GLOBALID : nullStr;
		long long   from_node_oid = (fFromNode != dicRegNode.end()) ? fFromNode->second.OBJECTID : nullInt;
		if(from_node_gid != nullStr && from_node_oid != nullInt)
		{
			SetValue(buf, index.from_node_gid.index, from_node_gid, nullStr);
			SetValue(buf, index.from_node_id.index, from_node_oid, 0LL);
		}
		else
		{
			//なければエラー
			LogUtil::printError(101, "外部キーチェック", "始点ノードIDが存在しません", "車線リンクSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t始点ノードSID(from_node_id):" + std::to_string(csv.from_node_id.GetValue()));
			continue;
		}
		//TO_NODE_* はあれば入れる
		auto fToNode = dicRegNode.find(csv.to_node_id.GetValue());
		uh::tstring to_node_gid = (fToNode != dicRegNode.end()) ? fToNode->second.GLOBALID : nullStr;
		long long   to_node_oid = (fToNode != dicRegNode.end()) ? fToNode->second.OBJECTID : nullInt;
		if(to_node_gid != nullStr && to_node_oid != nullInt)
		{
			SetValue(buf, index.to_node_gid.index, to_node_gid, nullStr);
			SetValue(buf, index.to_node_id.index, to_node_oid, 0LL);
		}
		else
		{
			//なければエラー
			LogUtil::printError(102, "外部キーチェック", "終点ノードIDが存在しません", "車線リンクSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t終点ノードSID(to_node_id):" + std::to_string(csv.to_node_id.GetValue()));
			continue;
		}
		//存在しない場合はNULL(存在しない場合 GetOIDが-1で返ってくるので変換)
		auto fGroup = dicRegGroup.find(csv.lane_group_id.GetValue());
		long long gid = (fGroup != dicRegGroup.end()) ? fGroup->second.OBJECTID : nullInt;	//LANE_GROUP_ID
		if(gid == nullInt)
		{
			SetNULL(buf, index.lane_group_id.index);
		}
		else
		{
			SetValue(buf, index.lane_group_id.index, gid, 0LL);	
		}
		SetValue(buf, index.lane_number.index, csv.lane_no, 0);	//LANE_NUMBER
		SetValue(buf, index.lane_count.index, csv.lane_count, 0);	//LANE_COUNT
		SetValue(buf, index.max_legal_speed_c.index, csv.max_legal_speed_c, nullInt);	//MAX_LEGAL_SPEED_C
		SetValue(buf, index.min_legal_speed_c.index, csv.min_legal_speed_c, nullInt);	//MIN_LEGAL_SPEED_C
		SetValue(buf, index.travel_direction_c.index, csv.travel_direction_c, nullInt);	//TRAVEL_DIRECTION_C
		SetValue(buf, index.road_structure_c.index, csv.road_structure_c, nullInt);	//ROAD_STRUCTURE_C

		//ONEWAY_C
		if(csv.both_link_direction_f.GetIsNull())
		{
			//NULL不可なので 無効値を設定
			SetValue(buf, index.oneway_c.index, nullInt, nullInt);
		}
		else
		{
			//ONEWAY_Cは変換して格納
			//@NOTE	0/1以外を利用する場合は、オーサリングツールと要調整(逆向きは考慮していない)
			int convertOnewayC = -1;
			switch(csv.both_link_direction_f.GetValue())
			{
			case 0:	//条件なし一方通行（正方向）
				convertOnewayC = 0;
				break;
			case 1:	//一方通行なし
				convertOnewayC = 1;
				break;
			}
			SetValue(buf, index.oneway_c.index, convertOnewayC, nullInt);	//ONEWAY_C
		}	
		SetValue(buf, index.left_change_f.index, csv.left_change_f, 0);	//LEFT_CHANGE_F
		SetValue(buf, index.right_change_f.index, csv.right_change_f, 0);	//RIGHT_CHANGE_F
		SetValue(buf, index.left_protrusion_f.index, csv.left_protrusion_f, 0);	//LEFT_PROTRUSION_F
		SetValue(buf, index.right_protrusion_f.index, csv.right_protrusion_f, 0);	//RIGHT_PROTRUSION_F
		SetValue(buf, index.rubbing_f.index, csv.rubbing_f, 0);	//RUBBING_F
		SetValue(buf, index.tunnel_f.index, csv.tunnel_f, 0);	//TUNNEL_F
			
		//形状設定
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif
	
	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertLaneNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureLaneNode))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::LaneNode index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listLanePoint.size();

	//データインポート
	//ノードフィーチャ(重複ポイントはインポートしない)
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listLanePoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.node_id.GetValue()) != listRegSID.end())
		{
			//重複はあり
			LogUtil::printDebug("重複オブジェクトのためSKIPします\t車線ポイントSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t車線ノードSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.node_id, 0LL);	//SOURCEID ★csv.object_idではなくcsv.node_id
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		SetValue(buf, index.nodeclass_c.index, csv.point_class_c, nullInt);	//NODECLASS_C
		SetValue(buf, index.stop_point_f.index, csv.stop_point_f, 0);	//STOP_POINT_F
		SetValue(buf, index.covering_f.index, csv.cover_f, 0);	//COVERING_F

		//形状設定
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.node_id.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}
#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertNQLaneNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	//車線ノード列
	ITablePtr ipT;
	if(!(ipT = _schema->tableNQLaneNode))
	{
		assert(false);
		return false;
	}
	AdamSchema::NQLaneNode index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegNode;
	{
		//車線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	std::map<long long, OGID> dicRegLink;
	{
		//車線リンク
		IFeatureClassPtr ipLinkFC;
		if(!(ipLinkFC = _schema->featureLaneLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLink, ipLinkFC);
	}

	//インサートカーソル設定
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listLanePoint.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	//ノード列テーブル(重複ポイントもインポートする)
	for(const auto& csv: _csv->listLanePoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(100, "ユニークチェック", "重複オブジェクトのためSKIPします", "車線ポイントSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//NQ_LANE_NODE.LANE_NODE_GID
		//LANE_NODEからGID取得
		auto fNode = dicRegNode.find(csv.node_id.GetValue());
		uh::tstring node_gid = (fNode != dicRegNode.end()) ? fNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.lane_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(152, "外部キーチェック", "車線ノードIDが存在しません", "車線ポイントSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t車線ノードSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}
		//NQ_LANE_NODE.LANE_NODE_GID
		//LANE_LINKからGID取得
		auto fLink = dicRegLink.find(csv.lane_link_id.GetValue());
		uh::tstring link_gid = (fLink != dicRegLink.end()) ? fLink->second.GLOBALID : nullStr;
		if(link_gid != nullStr)
		{
			SetValue(buf, index.lane_link_gid.index, link_gid, nullStr);	
		}
		else
		{
			//なければエラー
			LogUtil::printError(141, "外部キーチェック", "車線リンクIDが存在しません", "車線ポイントSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t車線リンクSID(lane_link_id):" + std::to_string(csv.lane_link_id.GetValue()));
			continue;
		}
		
		SetValue(buf, index.sequence.index, csv.sequence, 0LL);	//NQ_LANE_NODE.SEQUENCE
		SetValue(buf, index.lastnode_f.index, csv.lastnode_f, 0);	//NQ_LANE_NODE.LASTNODE_F
		SetValue(buf, index.width.index, csv.width, 0.0);	//WIDTH
		SetValue(buf, index.linear_element_c.index, csv.linear_element_c, nullInt);	//LINEAR_ELEMENT_C
		SetValue(buf, index.curvature.index, csv.curvature, 0.0);	//CURVATURE
		SetValue(buf, index.curvature_radius.index, csv.curvature_radius, 0.0);	//CURVATURE_RADIUS
		SetValue(buf, index.curvature_change_rate.index, csv.curvature_change_rate, 0.0);	//CURVATURE_CHANGE_RATE
		SetValue(buf, index.clothoid_parameter.index, csv.clothoid_parameter, 0.0);	//CLOTHOID_PARAMETER
		SetValue(buf, index.longitudinal_slope.index, csv.longitudinal_slope, 0.0);	//LONGITUDINAL_SLOPE
		SetValue(buf, index.cross_slope.index, csv.cross_slope, 0.0);	//CROSS_SLOPE

		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif
	
	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

//インポート対象外
bool ADAMSchemaWriter::InsertRelRoadLinkLaneLink(const uh::tstring& tablename)
{
	return true;
}

bool ADAMSchemaWriter::InsertLaneLinkGroup(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	ITablePtr ipT;
	if(!(ipT = _schema->tableLaneLinkGroup))
	{
		assert(false);
		return false;
	}
	_ICursorPtr ipTCursor;
	if(!ipT)
	{
		assert(false);
		return false;
	}
	AdamSchema::LaneLinkGroup index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	//インサートカーソル設定
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listLaneLink.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listLaneLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//未設定はSKIP
		if(csv.lane_group_id.GetIsNull())
		{
			continue;
		}

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.lane_group_id.GetValue()) != listRegSID.end())
		{
			//重複はあり
			LogUtil::printDebug("重複オブジェクトのためSKIPします\tSID(lane_group_id):" + std::to_string(csv.lane_group_id.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		//SetGlobalId(buf, index.globalid.index);						//GLOBALID
		SetValue(buf, index.sourceid.index, csv.lane_group_id, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//上下線リンクグループは対象外のためNULL指定
		SetNULL(buf, index.updown_link_group_id.index);				//LANE_LINK_GROUP.UPDOWN_LINK_GROUP_ID
		SetValue(buf, index.group_c.index, csv.lane_group_c, nullInt);	//LANE_LINK_GROUP.GROUP_C

		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.lane_group_id.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertCompartLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureCompartLink))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::CompartLink index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	std::map<long long, OGID> dicRegNode;
	{
		//区画線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureCompartNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}


	long count = 0;
	long listSize = _csv->listCompartLink.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCompartLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(200, "ユニークチェック", "重複オブジェクトのためSKIPします", "区画線リンクSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//FROM_NODE_* はあれば入れる
		auto fFromNode = dicRegNode.find(csv.from_node_id.GetValue());
		uh::tstring from_node_gid = (fFromNode != dicRegNode.end()) ? fFromNode->second.GLOBALID : nullStr;
		long long   from_node_oid = (fFromNode != dicRegNode.end()) ? fFromNode->second.OBJECTID : -1LL;
		if(from_node_gid != nullStr && from_node_oid != nullInt)
		{
			SetValue(buf, index.from_node_gid.index, from_node_gid, nullStr);
			SetValue(buf, index.from_node_id.index, from_node_oid, 0LL);
		}
		else
		{
			//なければエラー
			LogUtil::printError(201, "外部キーチェック", "始点ノードIDが存在しません", "車線リンクSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t始点ノードSID(from_node_id):" + std::to_string(csv.from_node_id.GetValue()));
			continue;
		}
		//TO_NODE_* はあれば入れる
		auto fToNode = dicRegNode.find(csv.to_node_id.GetValue());
		uh::tstring to_node_gid = (fToNode != dicRegNode.end()) ? fToNode->second.GLOBALID : nullStr;
		long long   to_node_oid = (fToNode != dicRegNode.end()) ? fToNode->second.OBJECTID : -1LL;
		if(to_node_gid != nullStr && to_node_oid != nullInt)
		{
			SetValue(buf, index.to_node_gid.index, to_node_gid, nullStr);
			SetValue(buf, index.to_node_id.index, to_node_oid, 0LL);
		}
		else
		{
			//なければエラー
			LogUtil::printError(202, "外部キーチェック", "終点ノードIDが存在しません", "車線リンクSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t終点ノードSID(to_node_id):" + std::to_string(csv.to_node_id.GetValue()));
			continue;
		}
		SetValue(buf, index.compart_line_c.index, csv.compart_line_c, nullInt);	//COMPART_LINE_C
		SetValue(buf, index.compart_style_c.index, csv.compart_style_c, nullInt);	//COMPART_STYLE_C

		SetValue(buf, index.slowdown_label_c.index, csv.slowdown_label_c, nullInt);	//SLOWDOWN_LABEL_C

		SetValue(buf, index.installation_f.index, csv.installation_f, 0);	//INSTALLATION_F

		//形状設定
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertCompartNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureCompartNode))
	{
		assert(false);
		return false;
	}
	//SearchとInsert用のCursorは分ける
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::CompartNode index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listCompartPoint.size();

	//データインポート
	//ノード(重複しないポイント)が対象
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCompartPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.node_id.GetValue()) != listRegSID.end())
		{
			//重複はあり
			LogUtil::printDebug("重複オブジェクトのためSKIPします\t区画線ポイントSID(objectid):" + std::to_string(csv.objectid.GetValue()) + "\t区画線ノードSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.node_id, 0LL);	//SOURCEID	★csv.object_idではなくcsv.node_id
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		SetValue(buf, index.nodeclass_c.index, csv.point_class_c, nullInt);	//NODECLASS_C

		//形状設定
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.node_id.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertNQCompartNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	ITablePtr ipT;
	if(!(ipT = _schema->tableNQCompartNode))
	{
		assert(false);
		return false;
	}
	AdamSchema::NQCompartNode index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegNode;
	{
		//区画線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureCompartNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	std::map<long long, OGID> dicRegLink;
	{
		//区画線リンク
		IFeatureClassPtr ipLinkFC;
		if(!(ipLinkFC = _schema->featureCompartLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLink, ipLinkFC);
	}

	//インサートカーソル設定
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}



	long count = 0;
	long listSize = _csv->listCompartPoint.size();

	//データインポート
	//ポイント(重複したポイント)が対象
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCompartPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(240, "ユニークチェック", "重複オブジェクトのためSKIPします", "区画線ポイントSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//NQ_COMPART_NODE.GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//NQ_COMPART_NODE.SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//NQ_COMPART_NODE.OBJECTID はArcObject自動付与

		//NQ_COMPART_NODE.COMPART_NODE_GID
		//COMPART_NODE からGID取得
		auto fNode = dicRegNode.find(csv.node_id.GetValue());
		uh::tstring node_gid = (fNode != dicRegNode.end()) ? fNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.compart_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(247, "外部キーチェック", "区画線ノードIDが存在しません", "区画線ポイントSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t区画線ノードSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}
		//NQ_COMPART_NODE.COMPART_LINK_GID
		//COMPART_LINKからGID取得
		auto fLink = dicRegLink.find(csv.compart_link_id.GetValue());
		uh::tstring link_gid = (fLink != dicRegLink.end()) ? fLink->second.GLOBALID : nullStr;
		if(link_gid != nullStr)
		{
			SetValue(buf, index.compart_link_gid.index, link_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(241, "外部キーチェック", "区画線リンクIDが存在しません", "区画線ポイントSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t区画線リンクSID(compart_link_id):" + std::to_string(csv.compart_link_id.GetValue()));
			continue;
		}
		SetValue(buf, index.sequence.index, csv.sequence, 0LL);		//NQ_COMPART_NODE.SEQUENCE
		SetValue(buf, index.lastnode_f.index, csv.lastnode_f, 0);	//NQ_COMPART_NODE.LASTNODE_F
		SetValue(buf, index.width.index, csv.width, 0.0);	//WIDTH
		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertRelCompartLinkLaneLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelCompartLinkLaneLink))
	{
		assert(false);
		return false;
	}
	_ICursorPtr ipTCursor;
	if(!ipT)
	{
		assert(false);
		return false;
	}
	AdamSchema::RelCompartLinkLaneLink index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//車線リンク
		IFeatureClassPtr ipLaneLinkFC;
		if(!(ipLaneLinkFC = _schema->featureLaneLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipLaneLinkFC);
	}

	std::map<long long, OGID> dicRegCompart;
	{
		//区画線リンク
		IFeatureClassPtr ipCompartLinkFC;
		if(!(ipCompartLinkFC = _schema->featureCompartLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegCompart, ipCompartLinkFC);
	}

	//インサートカーソル設定
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelCompartLink.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelCompartLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(280, "ユニークチェック", "重複オブジェクトのためSKIPします", "区画線関連SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);		//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//LANE_LINK_GID
		//LANE_LINK からGID取得
		auto fLaneLink = dicRegLane.find(csv.lane_link_id.GetValue());
		uh::tstring lane_gid = (fLaneLink != dicRegLane.end()) ? fLaneLink->second.GLOBALID : nullStr;
		if(lane_gid != nullStr)
		{
			SetValue(buf, index.lane_link_gid.index, lane_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(281, "外部キーチェック", "車線リンクIDが存在しません", "区画線関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t車線リンクSID(object_id):" + std::to_string(csv.lane_link_id.GetValue()));
			continue;
		}
		//COMPART_LINK_GID
		//COMPART_LINK からGID取得
		auto fCompartLink = dicRegCompart.find(csv.compart_link_id.GetValue());
		uh::tstring comp_gid = (fCompartLink != dicRegCompart.end()) ? fCompartLink->second.GLOBALID : nullStr;
		if(comp_gid != nullStr)
		{
			SetValue(buf, index.compart_link_gid.index, comp_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(282, "外部キーチェック", "区画線リンクIDが存在しません", "区画線関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t区画線リンクSID(object_id):" + std::to_string(csv.compart_link_id.GetValue()));
			continue;
		}

		SetValue(buf, index.lr_c.index, csv.lr_f, nullInt);	//LR_C
		SetValue(buf, index.compart_link_direction_c.index, csv.compart_link_direction_c, nullInt);	//COMPART_LINK_DIRECTION_C
		SetValue(buf, index.sequence.index, csv.sequence, 0LL);	//SEQUENCE

		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}
#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}


bool ADAMSchemaWriter::InsertBorderLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureBorderLink))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::BorderLink index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	std::map<long long, OGID> dicRegNode;
	{
		//境界線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureBorderNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listBorderLink.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listBorderLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(300, "ユニークチェック", "重複オブジェクトのためSKIPします", "境界線リンクSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//FROM_NODE_* はあれば入れる
		auto fFromNode = dicRegNode.find(csv.from_node_id.GetValue());
		uh::tstring from_node_gid = (fFromNode != dicRegNode.end()) ? fFromNode->second.GLOBALID : nullStr;
		long long   from_node_oid = (fFromNode != dicRegNode.end()) ? fFromNode->second.OBJECTID : -1LL;
		if(from_node_gid != nullStr && from_node_oid != nullInt)
		{
			SetValue(buf, index.from_node_gid.index, from_node_gid, nullStr);
			SetValue(buf, index.from_node_id.index, from_node_oid, 0LL);
		}
		else
		{
			//なければエラー
			LogUtil::printError(301, "外部キーチェック", "始点ノードIDが存在しません", "境界線リンクSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t始点ノードSID(from_node_id):" + std::to_string(csv.from_node_id.GetValue()));
			continue;
		}
		//TO_NODE_* はあれば入れる
		auto fToNode = dicRegNode.find(csv.to_node_id.GetValue());
		uh::tstring to_node_gid = (fToNode != dicRegNode.end()) ? fToNode->second.GLOBALID : nullStr;
		long long   to_node_oid = (fToNode != dicRegNode.end()) ? fToNode->second.OBJECTID : -1LL;
		if(to_node_gid != nullStr && to_node_oid != nullInt)
		{
			SetValue(buf, index.to_node_gid.index, to_node_gid, nullStr);
			SetValue(buf, index.to_node_id.index, to_node_oid, 0LL);
		}
		else
		{
			//なければエラー
			LogUtil::printError(302, "外部キーチェック", "終点ノードIDが存在しません", "境界線リンクSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t終点ノードSID(to_node_id):" + std::to_string(csv.to_node_id.GetValue()));
			continue;
		}
		SetValue(buf, index.imaginary_border_f.index, csv.imaginary_boder_f, 0);//IMAGINARY_BODER_F

		//形状設定
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertBorderNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureBorderNode))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::BorderNode index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listBorderPoint.size();

	//データインポート
	//ノード(重複しないポイント)が対象
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listBorderPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.node_id.GetValue()) != listRegSID.end())
		{
			LogUtil::printDebug("重複オブジェクトのためSKIPします\t境界線ポイントSID(objectid)" + std::to_string(csv.objectid.GetValue()) + "\t境界線ノードSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.node_id, 0LL);	//SOURCEID	★csv.object_idではなくcsv.node_id
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		SetValue(buf, index.nodeclass_c.index, csv.point_class_c, nullInt);	//NODECLASS_C

		//形状設定
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.node_id.GetValue());
		
#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}
#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertNQBorderNode(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	ITablePtr ipT;
	if(!(ipT = _schema->tableNQBorderNode))
	{
		assert(false);
		return false;
	}
	AdamSchema::NQBorderNode index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegNode;
	{
		//境界線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureBorderNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegNode, ipNodeFC);
	}

	std::map<long long, OGID> dicRegLink;
	{
		//境界線リンク
		IFeatureClassPtr ipLinkFC;
		if(!(ipLinkFC = _schema->featureBorderLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLink, ipLinkFC);
	}

	//インサートカーソル設定
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listBorderPoint.size();

	//データインポート
	//ポイント(重複したポイント)が対象
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listBorderPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(340, "ユニークチェック", "重複オブジェクトのためSKIPします", "境界線ポイントSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//NQ_BORDER_NODE.GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//NQ_BORDER_NODE.SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//NQ_BORDER_NODE.SOURCE_UPDATE_C
		//NQ_BORDER_NODE.OBJECTID はArcObject自動付与

		//NQ_BORDER_NODE.BORDER_NODE_GID
		//BORDER_NODE からGID取得
		auto fNode = dicRegNode.find(csv.node_id.GetValue());
		uh::tstring node_gid = (fNode != dicRegNode.end()) ? fNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.border_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(347, "外部キーチェック", "境界線ノードIDが存在しません", "境界線ポイントSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t境界線ノードSID(node_id):" + std::to_string(csv.node_id.GetValue()));
			continue;
		}
		//NQ_BORDER_NODE.BORDER_LINK_GID
		//BORDER_LINKからGID取得
		auto fLink = dicRegLink.find(csv.border_link_id.GetValue());
		uh::tstring border_gid = (fLink != dicRegLink.end()) ? fLink->second.GLOBALID : nullStr;
		if(border_gid != nullStr)
		{
			SetValue(buf, index.border_link_gid.index, border_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(341, "外部キーチェック", "境界線リンクIDが存在しません", "境界線ポイントSID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t境界線リンクSID(border_link_id):" + std::to_string(csv.border_link_id.GetValue()));
			continue;
		}

		SetValue(buf, index.sequence.index, csv.sequence, 0LL);	//NQ_BORDER_NODE.SEQUENCE
		SetValue(buf, index.lastnode_f.index, csv.lastnode_f, 0);	//NQ_BORDER_NODE.LASTNODE_F
		
		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());
		
#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertRelBorderLinkLaneLink(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelBorderLinkLaneLink))
	{
		assert(false);
		return false;
	}
	AdamSchema::RelBorderLinkLaneLink index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//車線リンク
		IFeatureClassPtr ipLaneLinkFC;
		if(!(ipLaneLinkFC = _schema->featureLaneLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipLaneLinkFC);
	}

	std::map<long long, OGID> dicRegBorder;
	{
		//境界線リンク
		IFeatureClassPtr ipBorderLinkFC;
		if(!(ipBorderLinkFC = _schema->featureBorderLink))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegBorder, ipBorderLinkFC);
	}

	//インサートカーソル設定
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelBorderLink.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelBorderLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(380, "ユニークチェック", "重複オブジェクトのためSKIPします", "境界線関連SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//LANE_LINK_GID
		//LANE_LINK からGID取得
		auto fLaneLink = dicRegLane.find(csv.lane_link_id.GetValue());
		uh::tstring lane_gid = (fLaneLink != dicRegLane.end()) ? fLaneLink->second.GLOBALID : nullStr;
		if(lane_gid != nullStr)
		{
			SetValue(buf, index.lane_link_gid.index, lane_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(381, "外部キーチェック", "車線リンクIDが存在しません", "境界線関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t車線リンクSID(object_id):" + std::to_string(csv.lane_link_id.GetValue()));
			continue;
		}
		//BORDER_LINK_GID
		//COMPART_LINK からGID取得
		auto fBorderLink = dicRegBorder.find(csv.border_link_id.GetValue());
		uh::tstring border_gid = (fBorderLink != dicRegBorder.end()) ? fBorderLink->second.GLOBALID : nullStr;
		if(border_gid != nullStr)
		{
			SetValue(buf, index.border_link_gid.index, border_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(382, "外部キーチェック", "境界線リンクIDが存在しません", "境界線関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t境界線リンクSID(object_id):" + std::to_string(csv.border_link_id.GetValue()));
			continue;
		}

		SetValue(buf, index.lr_c.index, csv.lr_f, nullInt);	//LR_C
		SetValue(buf, index.border_link_direction_c.index, csv.border_link_direction_c, nullInt);	//BORDER_LINK_DIRECTION_C
		SetValue(buf, index.sequence.index, csv.sequence, 0LL);	//SEQUENCE

		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}


bool ADAMSchemaWriter::InsertSignalPoint(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureSignalPoint))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::SignalPoint index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listSignalPoint.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listSignalPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(400, "ユニークチェック", "重複オブジェクトのためSKIPします", "信号機ポイントSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		SetValue(buf, index.horizontal_angle.index, csv.horizontal_angle, 0.0);	//HORIZONTAL_ANGLE
		SetValue(buf, index.signal_mold_c.index, csv.signal_mold_c, nullInt);	//SIGNAL_MOLD_C
		SetValue(buf, index.lamp_count.index, csv.lamp_count, 0);	//LAMP_COUNT
		SetValue(buf, index.arrow_f.index, csv.arrow_f, 0);	//ARROW_F
		SetValue(buf, index.warning_f.index, csv.warning_f, 0);	//WARNING_F
		SetValue(buf, index.independent_f.index, csv.independent_f, 0);	//INDEPENDENT_F

		//形状設定
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertRelLaneNodeSignalPoint(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelLaneNodeSignalPoint))
	{
		assert(false);
		return false;
	}
	AdamSchema::RelLaneNodeSignalPoint index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//車線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipNodeFC);
	}

	std::map<long long, OGID> dicRegSignal;
	{
		//信号機ポイント
		IFeatureClassPtr ipSignalFC;
		if(!(ipSignalFC = _schema->featureSignalPoint))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegSignal, ipSignalFC);
	}

	//インサートカーソル設定
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelSignalPoint.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelSignalPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(440, "ユニークチェック", "重複オブジェクトのためSKIPします", "信号機関連SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//LANE_NODE_GID
		//LANE_NODE からGID取得
		auto fLaneNode = dicRegLane.find(csv.lane_node_id.GetValue());
		uh::tstring node_gid = (fLaneNode != dicRegLane.end()) ? fLaneNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.lane_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(441, "外部キーチェック", "車線ノードIDが存在しません", "信号機関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t車線ノードSID(object_id):" + std::to_string(csv.lane_node_id.GetValue()));
			continue;
		}
		//SIGNAL_POINT_GID
		//SIGNAL_POINT からGID取得
		auto fSignalPoint = dicRegSignal.find(csv.signal_point_id.GetValue());
		uh::tstring obj_gid = (fSignalPoint != dicRegSignal.end()) ? fSignalPoint->second.GLOBALID : nullStr;
		if(obj_gid != nullStr)
		{
			SetValue(buf, index.signal_point_gid.index, obj_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(442, "外部キーチェック", "信号機ポイントIDが存在しません", "信号機関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t信号機ポイントSID(object_id):" + std::to_string(csv.signal_point_id.GetValue()));
			continue;
		}

		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}


bool ADAMSchemaWriter::InsertCrossWalkLine(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureCrossWalkLine))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::CrossWalkLine index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listCrossWalkLine.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCrossWalkLine)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(500, "ユニークチェック", "重複オブジェクトのためSKIPします", "横断歩道SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		SetValue(buf, index.width.index, csv.width, 0.0);	//WIDTH

		//形状設定
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}
#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertRelLaneNodeCrossWalkLine(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelLaneNodeCrossWalkLine))
	{
		assert(false);
		return false;
	}
	AdamSchema::RelLaneNodeCrossWalkLine index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//車線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipNodeFC);
	}

	std::map<long  long, OGID> dicRegCrossWalk;
	{
		//横断歩道ポイント
		IFeatureClassPtr ipCrossWalkFC;
		if(!(ipCrossWalkFC = _schema->featureCrossWalkLine))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegCrossWalk, ipCrossWalkFC);
	}

	//インサートカーソル設定
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelCrossWalkLine.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelCrossWalkLine)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(540, "ユニークチェック", "重複オブジェクトのためSKIPします", "横断歩道関連SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//LANE_NODE_GID
		//LANE_NODE からGID取得
		auto fLaneNode = dicRegLane.find(csv.lane_node_id.GetValue());
		uh::tstring node_gid = (fLaneNode != dicRegLane.end()) ? fLaneNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.lane_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(641, "外部キーチェック", "車線ノードIDが存在しません", "横断歩道関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t車線ノードSID(object_id):" + std::to_string(csv.lane_node_id.GetValue()));
			continue;
		}
		//CROSSWALK_LINE_GID
		//CROSSWALK_LINE からGID取得
		auto fCrossWalk = dicRegCrossWalk.find(csv.crosswalk_line_id.GetValue());
		uh::tstring obj_gid = (fCrossWalk != dicRegCrossWalk.end()) ? fCrossWalk->second.GLOBALID : nullStr;
		if(obj_gid != nullStr)
		{
			SetValue(buf, index.crosswalk_line_gid.index, obj_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(642, "外部キーチェック", "横断歩道ラインIDが存在しません", "横断歩道関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t横断歩道ラインSID(object_id):" + std::to_string(csv.crosswalk_line_id.GetValue()));
			continue;
		}

		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}


bool ADAMSchemaWriter::InsertRoadSignPoint(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureRoadSignPoint))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::RoadSignPoint index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRoadSignPoint.size();

	//データインポート
	const uh::tstring nullStr = _T("-1");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRoadSignPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(600, "ユニークチェック", "重複オブジェクトのためSKIPします", "道路標識SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		SetValue(buf, index.horizontal_angle.index, csv.horizontal_angle, 0.0);	//HORIZONTAL_ANGLE
		SetValue(buf, index.roadsign_c.index, csv.roadsign_c, nullStr);	//ROADSIGN_C
		SetValue(buf, index.variable_f.index, csv.variable_f, 0);	//VARIABLE_F

		//形状設定
		IPointPtr ipPoint = CreatePointZ(csv.shape.GetValue());
		buf->putref_Shape(ipPoint);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertRelLaneNodeRoadSignPoint(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	ITablePtr ipT;
	if(!(ipT = _schema->tableRelLaneNodeRoadSignPoint))
	{
		assert(false);
		return false;
	}
	AdamSchema::RelLaneNodeRoadSignPoint index(ipT);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipT);

	std::map<long long, OGID> dicRegLane;
	{
		//車線ノード
		IFeatureClassPtr ipNodeFC;
		if(!(ipNodeFC = _schema->featureLaneNode))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegLane, ipNodeFC);
	}

	std::map<long long, OGID> dicRegRoadSign;
	{
		//路面標識ポイント
		IFeatureClassPtr ipRoadSignFC;
		if(!(ipRoadSignFC = _schema->featureRoadSignPoint))
		{
			assert(false);
			return false;
		}
		GetRegisterdOGIDList(dicRegRoadSign, ipRoadSignFC);
	}

	//インサートカーソル設定
	_ICursorPtr ipTCursor;
	ipT->Insert(VARIANT_TRUE, &ipTCursor);
	if(!ipTCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listRelRoadSignPoint.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listRelRoadSignPoint)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(640, "ユニークチェック", "重複オブジェクトのためSKIPします", "道路標識関連SID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IRowBufferPtr buf;

		ipT->CreateRowBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		//SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//LANE_NODE_GID
		//LANE_NODE からGID取得
		auto fLaneNode = dicRegLane.find(csv.lane_node_id.GetValue());
		uh::tstring node_gid = (fLaneNode != dicRegLane.end()) ? fLaneNode->second.GLOBALID : nullStr;
		if(node_gid != nullStr)
		{
			SetValue(buf, index.lane_node_gid.index, node_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(541, "外部キーチェック", "車線ノードIDが存在しません", "道路標識関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t車線ノードSID(object_id):" + std::to_string(csv.lane_node_id.GetValue()));
			continue;
		}
		//ROADSIGN_POINT_GID
		//ROADSIGN_POINT からGID取得
		auto fRoadSign = dicRegRoadSign.find(csv.roadsign_point_id.GetValue());
		uh::tstring obj_gid = (fRoadSign != dicRegRoadSign.end()) ? fRoadSign->second.GLOBALID : nullStr;
		if(obj_gid != nullStr)
		{
			SetValue(buf, index.roadsign_point_gid.index, obj_gid, nullStr);
		}
		else
		{
			//なければエラー
			LogUtil::printError(542, "外部キーチェック", "道路標識ポイントIDが存在しません", "道路標識関連SID(csv.objectid):" + std::to_string(csv.objectid.GetValue()) + "\t道路標識ポイントSID(object_id):" + std::to_string(csv.roadsign_point_id.GetValue()));
			continue;
		}

		//インサート
		if (FAILED(ipTCursor->InsertRow(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipTCursor->Flush();
			_flush_count = 0;
		}
#else
		ipTCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipTCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertVirtualCompartLine		(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureVirtualCompartLine))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::VirtualCompartLine index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listCompartLink.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listVirtualCompartLink)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(200, "ユニークチェック", "重複オブジェクトのためSKIPします", "仮想線ラインSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		//OBJECTID はArcObject自動付与

		//形状設定
		IPolylinePtr ipPolyline = CreatePolylineZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolyline);

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::InsertCoveringArea			(const uh::tstring& tablename)
{
	bool ret = true;
	long inserted_count = 0; //インサート済み件数

#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif

	//インデックス設定
	IFeatureClassPtr ipFC;
	if(!(ipFC = _schema->featureCoveringArea))
	{
		assert(false);
		return false;
	}
	IFeatureCursorPtr ipFCursor;
	if(!ipFC)
	{
		assert(false);
		return false;
	}
	AdamSchema::CoveringArea index(ipFC);

	//登録済みSIDリスト
	std::set<long long> listRegSID;
	GetRegisterdSIDList(listRegSID, ipFC);

	// スキーマロックした上でロードオンリーモードをONにし、アンロック＋OFF用にscope_guardを張っておく
	StartProcess(ITablePtr(ipFC));
	uh::scope_guard scope(std::bind(FinishProcess, ITablePtr(ipFC)));

	//インサートカーソル設定
	ipFC->Insert(VARIANT_TRUE, &ipFCursor);
	if(!ipFCursor)
	{
		assert(false);
		return false;
	}

	long count = 0;
	long listSize = _csv->listCoveringArea.size();

	//データインポート
	const uh::tstring nullStr = _T("");
	const int nullInt = -1;
	for(const auto& csv: _csv->listCoveringArea)
	{
		count++;
		LogUtil::flushLine((boost::format("%10ld / %10ld importing...") % count % listSize).str());

		//既に同一SIDがあればチェックしてSKIP
		if(listRegSID.find(csv.objectid.GetValue()) != listRegSID.end())
		{
			LogUtil::printError(200, "ユニークチェック", "重複オブジェクトのためSKIPします", "覆い物エリアSID(object_id):" + std::to_string(csv.objectid.GetValue()));
			continue;
		}

		CComVariant v;
		IFeatureBufferPtr buf;

		ipFC->CreateFeatureBuffer(&buf);
		if(!buf)
		{
			assert(false);
			return false;
		}

		//編集履歴情報設定
		SetModifyData(index, buf);

		//属性設定
		SetGlobalId(buf, index.globalid.index);					//GLOBALID
		SetValue(buf, index.sourceid.index, csv.objectid, 0LL);	//SOURCEID
		SetValue(buf, index.source_update_c.index, csv.update_c, -1);	//SOURCE_UPDATE_C
		//OBJECTID はArcObject自動付与

		//形状設定
		IPolygonPtr ipPolygon = CreatePolygonZ(csv.shape.GetValue());
		buf->putref_Shape(ipPolygon);

		//自己交差チェック
		{
			ITopologicalOperatorPtr ipTopo(ipPolygon);
			VARIANT_BOOL IsSimple;
			if (ipTopo) { ipTopo->get_IsSimple(&IsSimple); }

			if(IsSimple == VARIANT_FALSE)
			{
				LogUtil::printWarning(900, "ジオメトリチェック", "ポリゴンが自己交差しているためSKIPします", "覆い物エリアSID(object_id):" + std::to_string(csv.objectid.GetValue()));
				continue;
			}
		}

		//インサート
		if (FAILED(ipFCursor->InsertFeature(buf, &v)))
		{
			//インサート失敗
			LogUtil::printError(65, "インサート失敗\t", uh::toStr( tablename ), "OID:" + csv.objectid.GetText());
			assert(false);
			return false;
		}

		//登録済みリストに追加
		listRegSID.insert(csv.objectid.GetValue());

#ifdef _USE_FLUSH_COUNT
		_flush_count++;
		if(_flush_count >= FLUSH_COUNT_LIMIT)
		{
			ipFCursor->Flush();
			_flush_count = 0;
		}
#else
		ipFCursor->Flush();
#endif
		inserted_count++;
	}

#ifdef _USE_FLUSH_COUNT
	ipFCursor->Flush();
#endif

	LogUtil::print(std::to_string(inserted_count) + "件 インポートしました");

	return ret;
}

bool ADAMSchemaWriter::CheckAdamVersion()
{
	LogUtil::print("ADAMバージョンチェックを実施します");

	bool ret = SetTable(	AdamSchema::AdamVersion::TableName,				&(_schema->tableAdamVersion));

	if( !ret )
	{
		LogUtil::printError( 73, "スキーマバージョン不一致", "スキーマバージョンが異なります", uh::toStr( AdamSchema::AdamVersion::TableName ) + "が存在しません" );
		return false;
	}
	LogUtil::print( "ADAMバージョンチェックに成功しました" );

	LogUtil::printLine();

	return true;
}

IPointPtr ADAMSchemaWriter::CreatePointZ(const Location& loc)
{
	bool ret = true;

	//空間参照取得元は LANE_NODE
	IFeatureClassPtr ipPntFC = _schema->featureLaneNode;
	if(!ipPntFC)
	{
		assert(false);
		return nullptr;
	}

	IPointPtr ipPnt;

	//ポイントジオメトリ作成
	SetZPoint(ipPntFC, ipPnt);

	//測地系変換
	WKSPointZ wksPnt = {loc.longitude.GetValue(), loc.latitude.GetValue(), loc.height.GetValue()};
	WKSPointZ cnvPnt = wksPnt;
	if(!crdcnvTokyo2Ref(wksPnt, cnvPnt))
	{
		assert(false);
		return nullptr;
	}

	ipPnt->put_X(cnvPnt.X);
	ipPnt->put_Y(cnvPnt.Y);
	ipPnt->put_Z(cnvPnt.Z);

	return ipPnt;
}

IPointPtr ADAMSchemaWriter::CreatePointZ(const adam_v2::Point& loc)
{
	return CreatePointZ(loc.GetGeometry());
}

IPolylinePtr ADAMSchemaWriter::CreatePolylineZ(const std::vector<Location>& loc)
{
	//空間参照取得元は LANE_LINK
	IFeatureClassPtr ipLinkFC = _schema->featureLaneLink;

	if(!ipLinkFC)
	{
		assert(false);
		return nullptr;
	}

	IPolylinePtr ipPolyline;

	//ジオメトリ初期化	
	SetZPolyline(ipLinkFC, ipPolyline);

	for(const auto& pnt : loc)
	{
		//ポイントジオメトリ作成
		IPointPtr ipPoint = CreatePointZ(pnt);

		//ポリラインに追加
		((IPointCollectionPtr)ipPolyline)->AddPoint(ipPoint);
	}

	return ipPolyline;
}

IPolylinePtr ADAMSchemaWriter::CreatePolylineZ(const adam_v2::Polyline& loc)
{
	return CreatePolylineZ(loc.GetGeometry());
}

IPolygonPtr ADAMSchemaWriter::CreatePolygonZ(const std::vector<Location>& loc)
{
	//空間参照取得元は COVERING_AREA
	IFeatureClassPtr ipPolygonFC = _schema->featureCoveringArea;

	if(!ipPolygonFC)
	{
		assert(false);
		return nullptr;
	}

	IPolygonPtr ipPolygon;

	//ジオメトリ初期化	
	SetZPolygon(ipPolygonFC, ipPolygon);

	for(const auto& pnt : loc)
	{
		//ポイントジオメトリ作成
		IPointPtr ipPoint = CreatePointZ(pnt);

		//ポリラインに追加
		((IPointCollectionPtr)ipPolygon)->AddPoint(ipPoint);
	}

	return ipPolygon;
}

IPolygonPtr ADAMSchemaWriter::CreatePolygonZ(const adam_v2::Polygon& loc)
{
	return CreatePolygonZ(loc.GetGeometry());
}

void ADAMSchemaWriter::SetModifyData(const AdamSchema::AdamiPCFeature& adamSchema, IRowBufferPtr ipRow)
{
	SYSTEMTIME st = {};
	::GetLocalTime(&st);

	CString strDate;
	strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	//ユーザ名取得
	TCHAR szName[256];
	DWORD dwName = 255;
	if(!GetUserName(szName, &dwName))
	{
		_tcscpy_s(szName, 256 * sizeof(TCHAR),_T(""));	//名前取れなかったら名無し
	}

	ipRow->put_Value(adamSchema.operator_name.index,	CComVariant(szName)									);
	ipRow->put_Value(adamSchema.purpose_c.index,		CComVariant(long(0))								);
	ipRow->put_Value(adamSchema.progmodifydate.index,	CComVariant(strDate)								);
	ipRow->put_Value(adamSchema.updatetype_c.index,		CComVariant(long(1))								);	//新規作成
	ipRow->put_Value(adamSchema.progmodifydate.index,	CComVariant(strDate)								);
	ipRow->put_Value(adamSchema.modifyprogname.index,	CComVariant(CString(_T("ADAMSurveyImporter")))		);
	ipRow->put_Value(adamSchema.userclaim_f.index,		CComVariant(long(0))								);
	ipRow->put_Value(adamSchema.speculation_c.index,	CComVariant(long(1))								);

	ipRow->put_Value(adamSchema.source.index,			CComVariant(CString(_source.c_str()))				);
}

void ADAMSchemaWriter::SetModifyData(const AdamSchema::TableClass& adamSchema, IRowBufferPtr ipRow)
{
	SYSTEMTIME st = {};
	::GetLocalTime(&st);

	CString strDate;
	strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	//ユーザ名取得
	TCHAR szName[256];
	DWORD dwName = 255;
	if(!GetUserName(szName, &dwName))
	{
		_tcscpy_s(szName, 256 * sizeof(TCHAR),_T(""));	//名前取れなかったら名無し
	}

	ipRow->put_Value(adamSchema.operator_name.index,	CComVariant(szName)									);
	ipRow->put_Value(adamSchema.purpose_c.index,		CComVariant(long(0))								);
	ipRow->put_Value(adamSchema.progmodifydate.index,	CComVariant(strDate)								);
	ipRow->put_Value(adamSchema.updatetype_c.index,		CComVariant(long(1))								);	//新規作成
	ipRow->put_Value(adamSchema.progmodifydate.index,	CComVariant(strDate)								);
	ipRow->put_Value(adamSchema.modifyprogname.index,	CComVariant(CString(_T("ADAMSurveyImporter")))		);
	ipRow->put_Value(adamSchema.userclaim_f.index,		CComVariant(long(0))								);

	ipRow->put_Value(adamSchema.source.index,			CComVariant(CString(_source.c_str()))				);
}

void ADAMSchemaWriter::StartProcess(ITablePtr& ipT)
{
	ChangeSchemeLock(ipT, esriExclusiveSchemaLock);	// スキーマロック
	ChangeLoadOnlyMode(ipT, VARIANT_TRUE);			// RoadOnlyMode ON
}

void ADAMSchemaWriter::FinishProcess(ITablePtr& ipT)
{
	ChangeLoadOnlyMode(ipT, VARIANT_FALSE);			// RoadOnlyMode OFF
	ChangeSchemeLock(ipT, esriSharedSchemaLock);	// スキーマアンロック
}

void ADAMSchemaWriter::ChangeSchemeLock(ITablePtr& ipT, esriSchemaLock eStatus)
{
	ISchemaLockPtr ipSchemaLock(ipT);
	ipSchemaLock->ChangeSchemaLock(eStatus);
}

void ADAMSchemaWriter::ChangeLoadOnlyMode(ITablePtr& ipT, VARIANT_BOOL vbOnOff)
{
	IFeatureClassLoadPtr ipFcLoad(ipT);
	ipFcLoad->PutLoadOnlyMode(vbOnOff);
}