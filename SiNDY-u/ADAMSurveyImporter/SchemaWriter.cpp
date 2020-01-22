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

#include <TDC/useful_headers/uh_guid.h>

#include <sindy/workspace.h>
#include "LogUtil.h"

#include "SchemaWriter.h"

using namespace adam_v2;

const long long SchemaWriter::SRID_TOKYO	= 4301;
const long long SchemaWriter::SRID_JGD2000	= 4612;
const long long SchemaWriter::SRID_JGD2011	= 6668;

#ifdef _USE_FLUSH_COUNT
const long long SchemaWriter::FLUSH_COUNT_LIMIT = 1000;
#endif

SchemaWriter::SchemaWriter(void) :
	_workspace(nullptr)
	,_srid(0LL)
{
}


SchemaWriter::~SchemaWriter(void)
{
}

void SchemaWriter::Clear(void)
{
	_workspace = nullptr;
	//_srid = 0L;
	//_company = _T("");
#ifdef _USE_FLUSH_COUNT
	_flush_count = 0;
#endif
}

bool SchemaWriter::SetWorkspace()
{
	_workspace = sindy::create_workspace(_path.c_str());

	bool ret = true;
	if(!_workspace)
	{
		ret = false;
	}

	LogUtil::printTableState(60, "ワークスペース\t" + uh::toStr( _path ), ret);
	
	return ret;
}

bool SchemaWriter::SetFeature(const uh::tstring& tableName, IFeatureClass** ptr)
{
	if(!_workspace)
	{
		if(!SetWorkspace())
		{
			return false;
		}
	}

	// フィーチャ接続チェック、格納
	IFeatureWorkspacePtr ipFeatureWorkspace(_workspace);

	bool ret = true;
	if( FAILED(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(tableName.c_str()), ptr) ) )
	{
		ret = false;
	}
	
	LogUtil::printTableState(61, "フィーチャ\t" + uh::toStr( tableName ), ret);

	return ret;
}

bool SchemaWriter::SetTable(const uh::tstring& tableName, ITable** ptr)
{
	if(!_workspace)
	{
		if(!SetWorkspace())
		{
			return false;
		}
	}

	// フィーチャ接続チェック、格納
	IFeatureWorkspacePtr ipFeatureWorkspace(_workspace);

	bool ret = true;
	if( FAILED(ipFeatureWorkspace->OpenTable(CComBSTR(tableName.c_str()), ptr) ) )
	{
		ret = false;
	}
	
	LogUtil::printTableState(61, "テーブル\t" + uh::toStr( tableName ), ret);

	return ret;
}

bool SchemaWriter::StartEdit(const IWorkspaceEditPtr& ip_edit)
{
	if(_unuse_version)
	{
		return true;
	}

	if(!ip_edit)
	{
		return false;
	}

	//編集開始
	if (FAILED(ip_edit->StartEditing(VARIANT_FALSE)))
	{
		LogUtil::printError(63, "編集開始に失敗しました。処理を中断します");
		return false;
	}
	if (FAILED(ip_edit->StartEditOperation()))
	{
		LogUtil::printError(63, "編集開始に失敗しました。処理を中断します");
		return false;
	}

	return true;
}

bool SchemaWriter::StopEdit(const IWorkspaceEditPtr& ip_edit)
{
	if(_unuse_version)
	{
		return true;
	}

	if(!ip_edit)
	{
		return false;
	}

	//編集終了
	if (FAILED(ip_edit->StopEditOperation()))
	{
		LogUtil::printError(64, "編集終了に失敗しました。編集を破棄します");
		return false;
	}
	if (FAILED(ip_edit->StopEditing(VARIANT_TRUE)))
	{
		return false;
	}

	return true;
}

bool SchemaWriter::AbortEdit(const IWorkspaceEditPtr& ip_edit)
{
	if(_unuse_version)
	{
		return true;
	}

	if(!ip_edit)
	{
		return false;
	}

	//編集キャンセル
	if (FAILED(ip_edit->AbortEditOperation()))
	{
		LogUtil::printError(66, "編集キャンセルに失敗しました");
		return false;
	}

	return true;
}


bool SchemaWriter::SetSRID(long long sr)
{
	bool ret = true;

	switch(sr)
	{
	case SRID_TOKYO:
		_srid = sr;
		break;
	case SRID_JGD2000:
		_srid = sr;
		break;
	case SRID_JGD2011:
		_srid = sr;
		break;
	default:
		ret = false;
		break;
	}
	
	return ret;
}

bool SchemaWriter::SetSource(const uh::tstring& company, const uh::tstring& date, const uh::tstring& lot)
{
	_source = company + _T("_") + date + _T("_") + lot;
	_company = company;
	_date = date;
	_lot = lot;

	return true;
}

// ===================================================================
//  ArcObjectユーティティ系
// ===================================================================

void SchemaWriter::SetZPoint(const IFeatureClassPtr& ifcp_feature, IPointPtr& ip_point)
{
	//空間参照取得
	ISpatialReferencePtr ipSpRef;
	(IGeoDatasetPtr(ifcp_feature))->get_SpatialReference(&ipSpRef);
 
	//ポイントインスタンス生成
	ip_point.CreateInstance(CLSID_Point);

	//空間参照設定
	ip_point->putref_SpatialReference(ipSpRef);
	//Z値付与
	((IZAwarePtr)ip_point)->put_ZAware(VARIANT_TRUE);
}

void SchemaWriter::SetZPolyline(const IFeatureClassPtr& ifcp_feature, IPolylinePtr& ip_polyline)
{
	//空間参照取得
	ISpatialReferencePtr ipSpRef;
	(IGeoDatasetPtr(ifcp_feature))->get_SpatialReference(&ipSpRef);
 
	//ポリラインインスタンス生成
	ip_polyline.CreateInstance(CLSID_Polyline);

	//空間参照設定
	ip_polyline->putref_SpatialReference(ipSpRef);
	//Z値付与
	((IZAwarePtr)ip_polyline)->put_ZAware(VARIANT_TRUE);
}

void SchemaWriter::SetZPolygon(const IFeatureClassPtr& ifcp_feature, IPolygonPtr& ip_polygon)
{
	//空間参照取得
	ISpatialReferencePtr ipSpRef;
	(IGeoDatasetPtr(ifcp_feature))->get_SpatialReference(&ipSpRef);
 
	//ポリラインインスタンス生成
	ip_polygon.CreateInstance(CLSID_Polygon);

	//空間参照設定
	ip_polygon->putref_SpatialReference(ipSpRef);
	//Z値付与
	((IZAwarePtr)ip_polygon)->put_ZAware(VARIANT_TRUE);
}

bool SchemaWriter::crdcnvTokyo2Ref(const WKSPointZ& in, WKSPointZ& out)
{
	return crdcnvTokyo2Ref(in.X, in.Y, in.Z, out.X, out.Y, out.Z);
}

bool SchemaWriter::crdcnvTokyo2Ref(double in_x, double in_y, double in_z, double& out_x, double& out_y, double& out_z)
{
	bool ret = true;
	int retCnv = 0;

	switch(_srid)
	{
	case SRID_TOKYO:
		out_x = in_x;
		out_y = in_y;
		out_z = in_z;
		break;
	case SRID_JGD2000:
	case SRID_JGD2011:
		out_z = in_z;
		retCnv = _cnv.JGD2000toTOKYO_RP(in_x, in_y, &(out_x), &(out_y), true);
		break;
	default:
		ret = false;
		break;
	}

	if(!ret || retCnv == 0)
	{
		LogUtil::printError(67, "測地系変換に失敗しました");
	}

	return ret;
}

bool SchemaWriter::SetNULL(IRowBufferPtr ipRow, long index)
{
	CComVariant vaVal;

	//ipRow から esriFieldType型取得
	IFieldsPtr ipFields;
	IFieldPtr  ipField;
	esriFieldType type = esriFieldTypeInteger;
	ipRow->get_Fields(&ipFields);
	if (!ipFields)
	{
		LogUtil::printError(72, "フィールド取得失敗\tインデックス番号:" + std::to_string(index) + "のフィールド取得に失敗しました");
		return false;
	}
	ipFields->get_Field(index, &ipField);
		
	if (!ipField)
	{
		LogUtil::printError(72, "フィールド取得失敗\tインデックス番号:" + std::to_string(index) + "のフィールド取得に失敗しました");
		return false;
	}
	ipField->get_Type(&type);

	//NULL可フィールドか
	if(AheIsNullableField(ipField)/*ipField->GetIsNullable() == VARIANT_TRUE*/)
	{
		//NULL設定
		vaVal.vt = VT_NULL;
	}
	else
	{
		return false;
	}

	ipRow->put_Value(index, vaVal);

	return true;
}

bool SchemaWriter::SetGlobalId(IRowBufferPtr ipRow, long index)
{
	CComVariant vaVal((BSTR)(uh::GetGUID().c_str()));
	
	//TODO GID重複チェックが必要か
	ipRow->put_Value(index, vaVal);

	return true;
}


IRowBufferPtr SchemaWriter::GetRow(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid/* = _T("SOURCEID")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	if(!ipFC)
	{
		return nullptr;
	}

	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%lld'")) % field_name_source % _company % field_name_sid % source_id).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%lld'")) % field_name_sid % source_id).str();
	}
	IQueryFilterPtr ipFilterQuery;
	ipFilterQuery = IQueryFilterPtr(CLSID_QueryFilter);
	ipFilterQuery->put_WhereClause(CComBSTR(whereQuery.c_str()));
	
	_ICursorPtr ipCursor;
	ipFC->Search(ipFilterQuery, VARIANT_TRUE, &ipCursor);
	
	_IRowPtr ipF;
	while(S_OK == ipCursor->NextRow( &ipF ) && ipF)
	{
		return ipF;
	}

	return ipF;
}

IFeaturePtr SchemaWriter::GetFeature(const IFeatureClassPtr& ipFC, long long source_id, uh::tstring field_name_sid/* = _T("SOURCEID")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	if(!ipFC)
	{
		return nullptr;
	}

	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%lld'")) % field_name_source % _company % field_name_sid % source_id).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%lld'")) % field_name_sid % source_id).str();
	}
	IQueryFilterPtr ipFilterQuery;
	ipFilterQuery = IQueryFilterPtr(CLSID_QueryFilter);
	ipFilterQuery->put_WhereClause(CComBSTR(whereQuery.c_str()));
	
	IFeatureCursorPtr ipCursor;
	ipFC->Search(ipFilterQuery, VARIANT_TRUE, &ipCursor);
	
	IFeaturePtr ipF;
	while(S_OK == ipCursor->NextFeature( &ipF ) && ipF)
	{
		return ipF;
	}

	return ipF;
}

IRowBufferPtr SchemaWriter::GetRow(const ITablePtr& ipFC, uh::tstring source_id, uh::tstring field_name_sid/* = _T("SOURCEID")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	if(!ipFC)
	{
		return nullptr;
	}

	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s'")) % field_name_source % _company % field_name_sid % source_id).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%s'")) % field_name_sid % source_id).str();
	}
	IQueryFilterPtr ipFilterQuery;
	ipFilterQuery = IQueryFilterPtr(CLSID_QueryFilter);
	ipFilterQuery->put_WhereClause(CComBSTR(whereQuery.c_str()));
	
	_ICursorPtr ipCursor;
	ipFC->Search(ipFilterQuery, VARIANT_TRUE, &ipCursor);
	
	_IRowPtr ipF;
	while(S_OK == ipCursor->NextRow( &ipF ) && ipF)
	{
		return ipF;
	}

	return ipF;
}

IFeaturePtr SchemaWriter::GetFeature(const IFeatureClassPtr& ipFC, uh::tstring source_id, uh::tstring field_name_sid/* = _T("SOURCEID")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	if(!ipFC)
	{
		return nullptr;
	}

	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s'")) % field_name_source % _company % field_name_sid % source_id).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%s'")) % field_name_sid % source_id).str();
	}
	IQueryFilterPtr ipFilterQuery;
	ipFilterQuery = IQueryFilterPtr(CLSID_QueryFilter);
	ipFilterQuery->put_WhereClause(CComBSTR(whereQuery.c_str()));
	
	IFeatureCursorPtr ipCursor;
	ipFC->Search(ipFilterQuery, VARIANT_TRUE, &ipCursor);
	
	IFeaturePtr ipF;
	while(S_OK == ipCursor->NextFeature( &ipF ) && ipF)
	{
		return ipF;
	}

	return ipF;
}

bool SchemaWriter::ExistRow(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid/* = _T("SOURCEID")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	IRowBufferPtr ipF = GetRow(ipFC, source_id, field_name_sid, field_name_source, same_company);

	if(!ipF)
	{
		return false;
	}

	return true;
}

uh::tstring SchemaWriter::GetGID(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid/* = _T("SOURCEID")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, uh::tstring field_name_guid/* = _T("GLOBALID")*/, bool same_company/* = true*/)
{
	IRowBufferPtr ipF = GetRow(ipFC, source_id, field_name_sid, field_name_source, same_company);

	uh::tstring gid = _T("");

	if(!ipF)
	{
		return gid;
	}	

	gid = SchemaWriter::GetValue<uh::tstring>(AheGetAttribute(ipF, field_name_guid.c_str()));

	return gid;
}

long long SchemaWriter::GetOID(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid/* = _T("SOURCEID")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, uh::tstring field_name_oid/* = _T("OBJECTID")*/, bool same_company/* = true*/)
{
	IRowBufferPtr ipF = GetRow(ipFC, source_id, field_name_sid, field_name_source, same_company);

	long long oid = -1LL;

	if(!ipF)
	{
		return oid;
	}	

	oid = SchemaWriter::GetValue<long long>(AheGetAttribute(ipF, field_name_oid.c_str()));

	return oid;
}

long SchemaWriter::GetPointCount4Link(const IFeaturePtr& ipF)
{
	LONG pointCnt = 0;
	if(!ipF)
	{
		return pointCnt;
	}

	IGeometryPtr ipShape;
	ipF->get_ShapeCopy(&ipShape);

	IPointCollectionPtr ipPC_LaneLink(ipShape);
	ipPC_LaneLink->get_PointCount(&pointCnt);

	return (long)pointCnt;
}
long SchemaWriter::GetPointCount4Point(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNodeT, const uh::tstring& fieldNama_link, long long linkSid, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	LONG pointCnt = 0;

	if(!_workspace || !ipLinkFC || !ipNodeT)
	{
		return pointCnt;
	}

	//リンクGID
	uh::tstring linkGID = GetGID(ipLinkFC, linkSid);

	//リンクID取得
	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s'")) % field_name_source % _company % fieldNama_link % linkGID).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%s'")) % fieldNama_link % linkGID).str();
	}
	uh::tstring subField = _T("COUNT(OBJECTID)"); 

	IQueryDefPtr ipQueryDef;
	IFeatureWorkspacePtr(_workspace)->CreateQueryDef(&ipQueryDef);
	ipQueryDef->put_Tables(CComBSTR(AheGetFeatureClassName(ipNodeT)));
	ipQueryDef->put_WhereClause(CComBSTR(whereQuery.c_str()));
	ipQueryDef->put_SubFields(CComBSTR(subField.c_str()));
	
	_ICursorPtr ipCursor;
	ipQueryDef->Evaluate(&ipCursor);
	
	_IRowPtr ipRow;
	if(ipCursor && S_OK == ipCursor->NextRow( &ipRow ) && ipRow)
	{
		CComVariant vaVal;

		ipRow->get_Value(0, &vaVal);
		vaVal.ChangeType(VT_I4);

		pointCnt = vaVal.lVal;
	}

	return (long)pointCnt;
}

std::map<int, uh::tstring> SchemaWriter::GetPointIDs(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNodeT, const uh::tstring& fieldNama_link, long long linkSid, uh::tstring field_name_target, uh::tstring field_name_sequence/* = _T("SEQUENCE")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	std::map<int, uh::tstring> mapNode;

	if(!_workspace || !ipLinkFC || !ipNodeT)
	{
		return mapNode;
	}

	//リンクGID
	uh::tstring linkGID = GetGID(ipLinkFC, linkSid);

	//リンクID取得
	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s'")) % field_name_source % _company % fieldNama_link % linkGID).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%s'")) % fieldNama_link % linkGID).str();
	}
	
	IQueryFilterPtr ipFilterQuery;
	ipFilterQuery = IQueryFilterPtr(CLSID_QueryFilter);
	ipFilterQuery->put_WhereClause(CComBSTR(whereQuery.c_str()));
	
	_ICursorPtr ipCursor;
	ipNodeT->Search(ipFilterQuery, VARIANT_TRUE, &ipCursor);
	
	_IRowPtr ipF;
	while(S_OK == ipCursor->NextRow( &ipF ) && ipF)
	{
		CComVariant vaVal;

		vaVal = AheGetAttribute(ipF, field_name_target.c_str());
		uh::tstring value = GetValue<uh::tstring>(vaVal);

		vaVal = AheGetAttribute(ipF, field_name_sequence.c_str());
		int seqence = GetValue<int>(vaVal);

		mapNode.insert(std::map<int, uh::tstring>::value_type(seqence, value));
	}

	return mapNode;
}


uh::tstring SchemaWriter::GetMinSequenceNodeGID(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNqT, long long link_id, uh::tstring field_name_link_gid, uh::tstring field_name_node_gid, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	uh::tstring nodeGID = _T("");

	if(!ipLinkFC || !ipNqT)
	{
		return nodeGID;
	}

	//リンクGID
	uh::tstring linkGID = GetGID(ipLinkFC, link_id);

	//リンクID取得
	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s'")) % field_name_source % _company % field_name_link_gid % linkGID).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%s'")) % field_name_link_gid % linkGID).str();
	}

	//最小シーケンス 取得
	uh::tstring subField = _T("MIN(SEQUENCE)"); 
	IQueryDefPtr ipQueryDef;
	IFeatureWorkspacePtr(_workspace)->CreateQueryDef(&ipQueryDef);
	ipQueryDef->put_Tables(CComBSTR(AheGetFeatureClassName(ipNqT)));
	ipQueryDef->put_WhereClause(CComBSTR(whereQuery.c_str()));
	ipQueryDef->put_SubFields(CComBSTR(subField.c_str()));
	
	_ICursorPtr ipCursor;
	ipQueryDef->Evaluate(&ipCursor);
	
	long sequence;
	_IRowPtr ipRow;
	if(ipCursor && S_OK == ipCursor->NextRow( &ipRow ) && ipRow)
	{
		CComVariant v;

		ipRow->get_Value(0, &v);
		v.ChangeType(VT_I4);
		sequence = v.lVal;
	}

	//ノードGID取得
	uh::tstring whereNodeQuery;
	if(same_company)
	{
		whereNodeQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s' AND SEQUENCE = %ld")) % field_name_source % _company % field_name_link_gid % linkGID % sequence).str();
	}
	else
	{
		whereNodeQuery = (uh::tformat(_T("%s like '%s' AND SEQUENCE = %ld")) % field_name_link_gid % linkGID % sequence).str();
	}
	IQueryFilterPtr ipFilterNodeQuery;
	ipFilterNodeQuery = IQueryFilterPtr(CLSID_QueryFilter);
	ipFilterNodeQuery->put_WhereClause(CComBSTR(whereNodeQuery.c_str()));
	
	_ICursorPtr ipNodeCursor;
	ipNqT->Search(ipFilterNodeQuery, VARIANT_TRUE, &ipNodeCursor);
	
	_IRowPtr ipNodeRow;
	while(S_OK == ipNodeCursor->NextRow( &ipNodeRow ) && ipNodeRow)
	{
		CComVariant vaVal;
		vaVal = AheGetAttribute(ipNodeRow, field_name_node_gid.c_str());
		nodeGID = GetValue<uh::tstring>(vaVal);
		return nodeGID;
	}

	return nodeGID;
}

uh::tstring SchemaWriter::GetMaxSequenceNodeGID(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNqT, long long link_id, uh::tstring field_name_link_gid, uh::tstring field_name_node_gid, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	uh::tstring nodeGID = _T("");

	if(!ipLinkFC || !ipNqT)
	{
		return nodeGID;
	}

	//リンクGID
	uh::tstring linkGID = GetGID(ipLinkFC, link_id);

	//リンクID取得
	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s'")) % field_name_source % _company % field_name_link_gid % linkGID).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%s'")) % field_name_link_gid % linkGID).str();
	}

	//最小シーケンス 取得
	uh::tstring subField = _T("MAX(SEQUENCE)"); 
	IQueryDefPtr ipQueryDef;
	IFeatureWorkspacePtr(_workspace)->CreateQueryDef(&ipQueryDef);
	ipQueryDef->put_Tables(CComBSTR(AheGetFeatureClassName(ipNqT)));
	ipQueryDef->put_WhereClause(CComBSTR(whereQuery.c_str()));
	ipQueryDef->put_SubFields(CComBSTR(subField.c_str()));
	
	_ICursorPtr ipCursor;
	ipQueryDef->Evaluate(&ipCursor);
	
	long sequence;
	_IRowPtr ipRow;
	if(ipCursor && S_OK == ipCursor->NextRow( &ipRow ) && ipRow)
	{
		CComVariant v;

		ipRow->get_Value(0, &v);
		v.ChangeType(VT_I4);
		sequence = v.lVal;
	}

	//ノードGID取得
	uh::tstring whereNodeQuery;
	if(same_company)
	{
		whereNodeQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s' AND SEQUENCE = %ld")) % field_name_source % _company % field_name_link_gid % linkGID % sequence).str();
	}
	else
	{
		whereNodeQuery = (uh::tformat(_T("%s like '%s' AND SEQUENCE = %ld")) % field_name_link_gid % linkGID % sequence).str();
	}
	IQueryFilterPtr ipFilterNodeQuery;
	ipFilterNodeQuery = IQueryFilterPtr(CLSID_QueryFilter);
	ipFilterNodeQuery->put_WhereClause(CComBSTR(whereNodeQuery.c_str()));
	
	_ICursorPtr ipNodeCursor;
	ipNqT->Search(ipFilterNodeQuery, VARIANT_TRUE, &ipNodeCursor);
	
	_IRowPtr ipNodeRow;
	while(S_OK == ipNodeCursor->NextRow( &ipNodeRow ) && ipNodeRow)
	{
		CComVariant vaVal;
		vaVal = AheGetAttribute(ipNodeRow, field_name_node_gid.c_str());
		nodeGID = GetValue<uh::tstring>(vaVal);
		return nodeGID;
	}

	return nodeGID;
}

std::vector<uh::tstring> SchemaWriter::GetFeature4Rel(const ITablePtr& ipRelT, const IFeatureClassPtr& ipObjFC, const IFeatureClassPtr& ipTargetFC, long long targetSID, uh::tstring field_name_obj_gid, uh::tstring field_name_target_gid, uh::tstring query/* = _T("")*/, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	std::vector<uh::tstring> vecGID;	

	if(!ipRelT || !ipObjFC || !ipTargetFC)
	{
		return vecGID;
	}

	//SIDからGID取得
	uh::tstring gid = GetGID(ipTargetFC, targetSID);

	//GIDをキーに関連テーブルからGID取得
	uh::tstring whereQuery;
	if(same_company)
	{
		whereQuery = (uh::tformat(_T("%s like '%s%%' AND %s like '%s' %s")) % field_name_source % _company % field_name_target_gid % gid % query).str();
	}
	else
	{
		whereQuery = (uh::tformat(_T("%s like '%s' %s")) % field_name_target_gid % gid % query).str();
	}
	IQueryFilterPtr ipFilterQuery;
	ipFilterQuery = IQueryFilterPtr(CLSID_QueryFilter);
	ipFilterQuery->put_WhereClause(CComBSTR(whereQuery.c_str()));
	
	_ICursorPtr ipCursor;
	ipRelT->Search(ipFilterQuery, VARIANT_TRUE, &ipCursor);
	
	_IRowPtr ipRow;
	while(S_OK == ipCursor->NextRow( &ipRow ) && ipRow)
	{
		CComVariant vaVal;
		vaVal = AheGetAttribute(ipRow, field_name_obj_gid.c_str());
		vecGID.push_back(GetValue<uh::tstring>(vaVal));
	}

	return vecGID;
}

long SchemaWriter::GetCount( const ITablePtr& ipT, const uh::tstring& whereQuery, uh::tstring field_name_source/* = _T("SOURCE")*/, bool same_company/* = true*/)
{
	LONG pointCnt = 0;

	if(!_workspace || !ipT)
	{
		return pointCnt;
	}

	//リンクID取得
	uh::tstring whereQueryStr = whereQuery;
	if(same_company)
	{
		whereQueryStr += (uh::tformat(_T(" AND %s like '%s%%'")) % field_name_source % _company).str();
	}
	uh::tstring subField = _T("COUNT(OBJECTID)"); 

	IQueryDefPtr ipQueryDef;
	IFeatureWorkspacePtr(_workspace)->CreateQueryDef(&ipQueryDef);
	ipQueryDef->put_Tables(CComBSTR(AheGetFeatureClassName(ipT)));
	ipQueryDef->put_WhereClause(CComBSTR(whereQueryStr.c_str()));
	ipQueryDef->put_SubFields(CComBSTR(subField.c_str()));
	
	_ICursorPtr ipCursor;
	ipQueryDef->Evaluate(&ipCursor);
	
	_IRowPtr ipRow;
	if(ipCursor && S_OK == ipCursor->NextRow( &ipRow ) && ipRow)
	{
		CComVariant vaVal;

		ipRow->get_Value(0, &vaVal);
		vaVal.ChangeType(VT_I4);

		pointCnt = vaVal.lVal;
	}

	return (long)pointCnt;
}

bool SchemaWriter::GetRegisterdSIDList(
	std::set<long long>& list,
	const ITablePtr& ipTable,
	uh::tstring field_name_sid/* = _T("SOURCEID")*/,
	uh::tstring field_name_source/* = _T("SOURCE")*/,
	bool same_source/* = true*/)
{
	uh::tstring whereQuery;
	IQueryFilterPtr ipFilterQuery = nullptr;

	if(same_source)
	{
		whereQuery = (uh::tformat(_T("%s = '%s'")) % field_name_source % _source).str();
		ipFilterQuery = IQueryFilterPtr(CLSID_QueryFilter);
		ipFilterQuery->put_WhereClause(CComBSTR(whereQuery.c_str()));
	}
	
	_ICursorPtr ipCursor;
	ipTable->Search(ipFilterQuery, VARIANT_TRUE, &ipCursor);
	
	_IRowPtr ipRow;
	while(ipCursor && S_OK == ipCursor->NextRow( &ipRow ) && ipRow)
	{
		long long sid = GetValue<long long>(AheGetAttribute(ipRow, field_name_sid.c_str()));
		list.insert(sid);
	}

	if(list.size() == 0)
	{
		return false;
	}

	return true;
}

bool SchemaWriter::GetRegisterdOGIDList(
	std::map<long long, OGID>& dic, 
	const ITablePtr& ipTable,
	uh::tstring field_name_sid/* = _T("SOURCEID")*/,
	uh::tstring field_name_oid/* = _T("OBJECTID")*/,
	uh::tstring field_name_gid/* = _T("GLOBALID")*/,
	uh::tstring field_name_source/* = _T("SOURCE")*/,
	bool same_source/* = true*/)
{
	uh::tstring whereQuery;
	IQueryFilterPtr ipFilterQuery = nullptr;

	if(same_source)
	{
		whereQuery = (uh::tformat(_T("%s = '%s'")) % field_name_source % _source).str();
		ipFilterQuery = IQueryFilterPtr(CLSID_QueryFilter);
		ipFilterQuery->put_WhereClause(CComBSTR(whereQuery.c_str()));
	}
	
	_ICursorPtr ipCursor;
	ipTable->Search(ipFilterQuery, VARIANT_TRUE, &ipCursor);
	
	_IRowPtr ipRow;
	while(ipCursor && S_OK == ipCursor->NextRow( &ipRow ) && ipRow)
	{
		long long   sid = GetValue<long long>  (AheGetAttribute(ipRow, field_name_sid.c_str()));
		long long   oid = (field_name_oid.empty()) ? -1LL   : GetValue<long long>  (AheGetAttribute(ipRow, field_name_oid.c_str()));
		uh::tstring gid = (field_name_gid.empty()) ? _T("") : GetValue<uh::tstring>(AheGetAttribute(ipRow, field_name_gid.c_str()));

		OGID obj = {oid, gid};
		auto f = dic.insert(std::map<long long, OGID>::value_type(sid, obj));
	}

	if(dic.size() == 0)
	{
		return false;
	}

	return true;
}