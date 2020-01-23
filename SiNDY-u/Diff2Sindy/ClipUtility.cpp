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
#include <algorithm>
#include <TDC/sindylib_core/sindymacroutil.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/GeometryBase.h>
#include <TDC/sindylib_base/ErrorObject2.h>
#include <TDC/sindylib_core/GeometryRule.h>
#include <TDC/sindylib_core/ErrorCode.h>
#include "Notifier.h"

using namespace sindy;

#include <ArcHelperEx/GlobalFunctions.h>
#include <boost/assign.hpp>

namespace sindy {
_ISetPtr MultiGeometry2Geometries( IGeometry* geom ); // GeometryBase.cpp
} // sindy

ISpatialReferencePtr AheGetSpatialReference( IUnknownPtr ipUnk ); // AheGlobals.h

// ジオメトリのラベルポイントを取得する
IPointPtr GetLabelPoint( IGeometryPtr geom )
{
	IAreaPtr area(geom);
	if( !area )
		return geom; // もしポイントならそのままポイントが返る

	IPointPtr ret;
	area->get_LabelPoint( &ret );
	return ret;
}

// ジオメトリをたす
IGeometryPtr AddGeometry( IGeometryPtr geom1, IGeometryPtr geom2 )
{
	IGeometryCollectionPtr ret( CLSID_GeometryBag );
	((IGeometryPtr)ret)->putref_SpatialReference( AheGetSpatialReference(geom1) );
	std::list<CAdapt<IGeometryPtr>> listGeom = boost::assign::list_of(geom1)(geom2);
	for( auto& geom : listGeom )
	{
		IGeometryBagPtr tmp(geom.m_T);
		if( !tmp )
			ret->AddGeometry( geom.m_T );
		else {
			IGeometryCollectionPtr col(geom.m_T);
			if( !col )
				continue;
			long count = 0;
			col->get_GeometryCount(&count);
			for( long i = 0; i < count; ++i )
			{
				IGeometryPtr part;
				col->get_Geometry( i, &part );
				ret->AddGeometry( part );
			}
		}
	}
	return ret;
}

// ジオメトリが空かどうか判定する
bool IsEmpty( IGeometryPtr geom )
{
	if( !geom )
		return true;

	VARIANT_BOOL isEmpty = VARIANT_FALSE;
	geom->get_IsEmpty( &isEmpty );
	if( isEmpty )
		return true;

	bool ret = true;
	esriGeometryType emType = esriGeometryAny;
	geom->get_GeometryType( &emType );
	switch( emType )
	{
	case esriGeometryPolyline:
	case esriGeometryPath:
	{
		IPolycurvePtr curve(geom);
		double len = 0;
		curve->get_Length( &len );
		if( len > 0.0 )
			ret = false;
		break;
	}
	case esriGeometryPolygon:
	case esriGeometryRing:
	{
		IAreaPtr area(geom);
		double dArea = 0;
		area->get_Area(&dArea);
		if( dArea > 0.0 )
			ret = false;
		break;
	}
	case esriGeometryBag:
	{
		long count = 0;
		IGeometryCollectionPtr col(geom);
		col->get_GeometryCount(&count);
		for( long i = 0; i < count; ++i )
		{
			IGeometryPtr part;
			col->get_Geometry( i, &part );
			if( !IsEmpty(part) )
				return false;
		}
		break;
	}
	default:
		ret = false;
		break;
	}
	return ret;
}

void CopyRow( CRowBase& row, IGeometryPtr geom, CTableContainer& table )
{
	CRowBase& newRow = *table.Create();
	newRow.CreateCache();
	row.CopyTo( newRow, true, false );
	newRow.CRowBase::SetShape(geom);
}

// マルチパート形状を別フィーチャとして分離する
void MultiGeometry2Geometries( CRowBase& row, CTableContainer& table, IGeometryPtr clip )
{
	_ISetPtr clipGeoms( sindy::MultiGeometry2Geometries( (IGeometry*)*row.GetShape() ) );
	long count = 0;
	clipGeoms->Reset();
	clipGeoms->get_Count( &count );
	_ASSERTE( 0 < count );
	IRelationalOperatorPtr rel(clip);
	bool orgSetted = false;
	for( long i = 0; i < count; ++i )
	{
		IUnknownPtr unk;
		clipGeoms->Next( &unk );
		if( IsEmpty(unk) )
			continue;

		// clip形状に含まれる形状かどうか判断
		IGeometryPtr geom(unk);
		((ITopologicalOperatorPtr)geom)->Simplify(); // Simpleじゃないと演算が失敗するので
		VARIANT_BOOL isContain = VARIANT_FALSE;
		rel->Contains( geom, &isContain ); // Simplifyしてるのにしっぱいするなら知らない

		// 元々のrowにまだセットしていないはそのままセットする
		// すでに元々のrowにセット済みの場合は、先にセットした形状で新しくフィーチャを作り、isContainな形状を改めてrowにセットする。
		// こうすることにより、isContainな形状を優先しつつrowにセットされないということもないようにする。
		if( !orgSetted || isContain )
		{
			if( orgSetted )
				CopyRow( row, (IGeometry*)*row.CRowBase::GetShape(), table );
			row.CRowBase::SetShape(geom);
			orgSetted = true;
		}
		else
			CopyRow( row, geom, table );
	}
}

/**
 * @brief 指定された矩形に近い構成点を矩形にスナップさせる
 *
 * */
void AdjustVertex( CGeometryBase& shape, IEnvelopePtr ipEnv, double threshold )
{
	WKSEnvelope env = {};
	ipEnv->QueryWKSCoords(&env);
	long geomCount = shape.GetPartCount();
	for( long i = 0; i < geomCount; ++i )
	{
		CGeometryBase part( shape.GetPart(i) );
		long pointCount = part.GetPointCount();
		for( long j = 0; j < pointCount; ++j )
		{
			IPointPtr point;
			(((IPointCollectionPtr)(IGeometry*)part)->get_Point(j, &point));
			WKSPoint p = {}, p2 = {};
			point->QueryCoords( &p.X, &p.Y );
			p2 = p;
			if( threshold > abs(env.XMax - p.X) )
				p2.X = env.XMax;
			else if( threshold > abs(env.XMin - p.X) )
				p2.X = env.XMin;
			if( threshold > abs(env.YMax - p.Y) )
				p2.Y = env.YMax;
			else if( threshold > abs(env.YMin - p.Y) )
				p2.Y = env.YMin;
			// 更新
			if( p.X != p2.X || p.Y != p2.Y )
			{
				IPointPtr orgPoint( AheCreateClone(point) );
				point->PutCoords( p2.X, p2.Y );
				part.UpdatePoint(j, point);
			}
		}
	}
}

void ErrorReport( const CRowBase& row, const uh::tstring& msg )
{
	IPointPtr p = GetLabelPoint((IGeometry*)*row.GetShape());
	WKSPoint pp = {};
	p->QueryCoords( &pp.X, &pp.Y );
	std::wstringstream ss;
	ss << msg << _T("：理由[" << (LPCTSTR)GETCOMERRORSTRING() << "]");
	notifier::reportObjInfo( row.GetOwnerTableName(), row.GetOID(), pp.X, pp.Y, notifier::error_level::error, ss.str(), std::wcerr );
}

/**
 * @brief ポリゴンでクリッピングする
 * @param con [in] rowが所属するコンテナ
 * @param row [in] クリッピング対象
 * @param ipEnv [in] クリッピング形状
 */
void Clip( CTableContainer& con, CRowBase& row, IGeometryPtr ipClip )
{
	CGeometryBase& shape = *row.GetShape();
	// クリップ形状にものすごく近い構成点があると、どうしてもそっちに引っ張られる
	// 為に意図した場所でクリップできない（bug 10595, bug 10496）
	// なので、事前に削除してしまう
	// クリップ後に無理やり移動する処理でも、どうしても解決できないケースがあった
	// ため、こちらに処理変更（bug 10496）
	//
	// クリップ前の形状とメッシュ矩形とで交点を求め、その交点と一定のしきい値内に
	// 存在する構成点を強制的に移動する。
//	AdjustVertex( shape, ipEnv, threshold );

	IGeometryPtr clipGeom;

	// メッシュ境界で切りつつも、処理対象メッシュの向こう側も残す
	// 最初Cut2でやろうとしたが、やはり切れなかったりエラーがでたりしてうまく処理できない
	// Inersect+Differenceで代用するが、この際Differenceでメッシュポリゴンを使用するとやはり
	// なぜかうまく処理できないことが多い。。。
	// 試しにIntersectした結果でDifference取ってみたら全くエラーがでなくなったので、このやり
	// 方でやる。ロジック変更する際は注意すること！！
	ITopologicalOperator2Ptr target((IGeometry*)(shape));
	if( FAILED( target->Intersect( ipClip, shape.GetDimension(), &clipGeom ) ) )
	{
		ErrorReport( row, _T("intersectに失敗、Clipできません") );
		return;
	}

	IGeometryPtr diffGeom;
	if( FAILED( target->Difference( clipGeom, &diffGeom ) ) )
	{
		ErrorReport( row, _T("Differenceに失敗、Clipできません") );
		return;
	}

	if( !IsEmpty(diffGeom) )
		clipGeom = AddGeometry( clipGeom, diffGeom );

	if( IsEmpty(clipGeom) )
	{
		ErrorReport( row, _T("Clipに失敗、Clipできません") );
		return;
	}
	// マルチパートの可能性があるので、分解する
	row.SetShape( clipGeom );
	MultiGeometry2Geometries( row, con, ipClip );
}
