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
 * @file PointIterator.h
 * PointIteratorクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _POINTITERATOR_H_
#define _POINTITERATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EnumVertex.h"
#include "Point.h"

namespace sindy {
/**
 * @class PointIterator
 * @brief <b>ジオメトリポイントイテレータクラス</b>\n
 * このクラスは、IEnumVertexをイテレータとして使用するためのクラスです。\n
 * 通常はCGeometryのイテレータとして使用されます。\n
 * ポイントジオメトリの場合でも動作するようになっています。\n
 */
class SINDYLIB_API PointIterator
{
	friend class PointIterator;
public:
	explicit PointIterator();
	virtual ~PointIterator();
	explicit PointIterator( IGeometry* _lp, bool _reverse = false );
public:
	PointIterator& operator++();
	const PointIterator& operator++() const { return ++(*const_cast<PointIterator*>(this)); }
	IPoint* operator->() const { return get_current_point(); }
	IPoint* operator*() const { return get_current_point(); }
	bool operator ==( const PointIterator& _it ) const;
	bool operator !=( const PointIterator& _it ) const { return !( *this == _it ); }
public:
	double get_x() const { double _x = -1; if( !is_end() ) get_current_point()->get_X( &_x ); return _x; }
	double get_y() const { double _y = -1; if( !is_end() ) get_current_point()->get_Y( &_y ); return _y; }
	std::pair<double,double> get_coords() const { std::pair<double,double> _pair_p( -1.0, -1.0 ); if( !is_end() ) get_current_point()->QueryCoords( &_pair_p.first, &_pair_p.second ); else _ASSERTE(false); return _pair_p; }
	errorcode::sindyErrCode set_x( const double& _x );
	errorcode::sindyErrCode set_y( const double& _y );
	errorcode::sindyErrCode update( IPoint* _p );
	errorcode::sindyErrCode update( const double& _x, const double& _y );
	errorcode::sindyErrCode move( const double& _dx, const double& _dy );
	errorcode::sindyErrCode move( const std::pair<double,double>& pairMove ){ return move( pairMove.first, pairMove.second ); }
	errorcode::sindyErrCode erase();
	errorcode::sindyErrCode at( long part, long index );
	errorcode::sindyErrCode advance( long i );
	long vertex_index() const { return __current_vertex_index; }
	long part_index() const { return __current_part_index; }
	long part_vertex_index() const { return __current_part_vertex_index; }
private:
	void init( IGeometry* _lp = NULL, bool _reverse = false );
	IPointPtr get_current_point(){ return __current_vertex; }
	IPointPtr get_current_point() const {  return const_cast<PointIterator*>(this)->get_current_point(); }
	bool is_end() const;
private:
#pragma warning(push)
#pragma warning(disable: 4251)
	IGeometryPtr __geometry; //!< 渡された形状（erase()に対応するために必要）
	IEnumVertexPtr __vertexes; //!< 構成点列（IPointCollectionをサポートするもの用）
	IPointPtr __current_vertex; //!< 現在の構成点
	long __current_part_index, __current_vertex_index, __current_part_vertex_index;
	bool __reverse; //!< リバースイテレータ用
#pragma warning(pop)
};

} // sindy

#endif // ifndef _POINTITERATOR_H_
