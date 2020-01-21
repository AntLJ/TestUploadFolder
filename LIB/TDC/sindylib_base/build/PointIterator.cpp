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
 * @file PointIterator.cpp
 * PointIteratorクラス実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "PointIterator.h"
#include "sindymacroutil.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

PointIterator::PointIterator(){	init(); }
PointIterator::PointIterator( IGeometry* _lp, bool _reverse/* = false*/ ) { init( _lp, _reverse ); }

PointIterator::~PointIterator(){}

PointIterator& PointIterator::operator++()
{
	if( is_end() )
		return *this;
	
	// EnumVertexから次のポイントを取得
	if( __vertexes != NULL )
	{
		if( __current_vertex != NULL )
		{
			if( (     __reverse   && S_OK != __vertexes->QueryPrevious( __current_vertex, &__current_part_index, &__current_part_vertex_index ) ) ||
				( ( ! __reverse ) && S_OK != __vertexes->QueryNext(     __current_vertex, &__current_part_index, &__current_part_vertex_index ) ) )
				init(); // to end
		}
		else {
			IPointPtr _p;
			if( (     __reverse   && S_OK != __vertexes->Previous( &_p, &__current_part_index, &__current_part_vertex_index ) ) ||
				( ( ! __reverse ) && S_OK != __vertexes->Next(     &_p, &__current_part_index, &__current_part_vertex_index ) ) )
				init(); // to end
			else
				__current_vertex = AheCreateClone(_p);
		}
	}
	else
		init(); // endにする

	if( ! is_end() )
		++__current_vertex_index; // total vertex index.

	return *this;
}
bool PointIterator::operator ==( const PointIterator& _it ) const {
	if( (IUnknown*)(IUnknownPtr)__vertexes == (IUnknown*)(IUnknownPtr)_it.__vertexes &&
		__current_part_index == _it.__current_part_index &&
		__current_part_vertex_index == _it.__current_part_vertex_index )
		return true;

	return false;
}	
errorcode::sindyErrCode PointIterator::set_x( const double& _x ){
	if( !is_end() )
	{
		if( __vertexes != NULL )
		{
			if( SUCCEEDED( __vertexes->put_X( _x ) ) )
				return errorcode::sindyErr_NoErr;
		}
		else {
			if( SUCCEEDED( __current_vertex->put_X( _x ) ) ) // Point Object
				return errorcode::sindyErr_NoErr;
		}
	}
	return errorcode::sindyErr_AlgorithmFailed;
}
errorcode::sindyErrCode PointIterator::set_y( const double& _y ){
	if( !is_end() )
	{
		if( __vertexes != NULL )
		{
			if( SUCCEEDED( __vertexes->put_Y( _y ) ) )
				return errorcode::sindyErr_NoErr;
		}
		else {
			if( SUCCEEDED( __current_vertex->put_Y( _y ) ) ) // Point Object
				return errorcode::sindyErr_NoErr;
		}
	}
	return errorcode::sindyErr_AlgorithmFailed;
}
errorcode::sindyErrCode PointIterator::update( IPoint* _p ){
	if( _p )
	{
		double _x, _y;
		if( SUCCEEDED( _p->QueryCoords( &_x, &_y ) ) )
			return update( _x, _y );
	}
	else
		return errorcode::sindyErr_ArgIsNull;

	return errorcode::sindyErr_AlgorithmFailed;
}
errorcode::sindyErrCode PointIterator::update( const double& _x, const double& _y )
{
	if( !is_end() )
	{
		if( __vertexes != NULL )
		{
			if( SUCCEEDED( __vertexes->put_X( _x ) ) &&
				SUCCEEDED( __vertexes->put_Y( _y ) ) )
				return errorcode::sindyErr_NoErr;
		}
		else {
			if( SUCCEEDED( __current_vertex->PutCoords( _x, _y ) ) ) // Point Object
				return errorcode::sindyErr_NoErr;
		}
	}
	return errorcode::sindyErr_AlgorithmFailed;
}
errorcode::sindyErrCode PointIterator::move( const double& _dx, const double& _dy )
{
	if( !is_end() )
	{
		double _x, _y;
		if( SUCCEEDED( __current_vertex->QueryCoords( &_x, &_y ) ) )
			return update( _x + _dx, _y + _dy );
	}
	return errorcode::sindyErr_AlgorithmFailed;
}
errorcode::sindyErrCode PointIterator::erase() {
	if( ! is_end() )
	{
		if( __vertexes != NULL ) // if Point, can't delete.
		{
			// EnumVertexでは追加・削除できないので、保持してある形状を使用する
			if( SUCCEEDED( ((IPointCollectionPtr)__geometry)->RemovePoints( __current_vertex_index, 1 ) ) )
			{
				// 現在位置まで進むために、インデックスを保持
				long _current_part_index = __current_part_index;
				long _current_part_vertex_index = __current_part_vertex_index;

				// 作り押し
				init( __geometry, __reverse );

				if( ! is_end() )
				{
					if( SUCCEEDED( __vertexes->SetAt( _current_part_index, _current_part_vertex_index ) ) )
						return errorcode::sindyErr_NoErr;
				}
				else
					return errorcode::sindyErr_NoErr; // no point at all.
			}
		}
	}
	return errorcode::sindyErr_AlgorithmFailed;
}
errorcode::sindyErrCode PointIterator::at( long part, long index )
{
	if( ! is_end() )
	{
		if( __vertexes )
		{
			if( SUCCEEDED( __vertexes->SetAt( part, index ) ) )
			{
				++(*this);
				return errorcode::sindyErr_NoErr;
			}
			else
				init();
		}
		else {
			// for Point object.
			if( part != 0 || index != 0 ) 
				init();
		}
	}
	return errorcode::sindyErr_AlgorithmFailed;
}
errorcode::sindyErrCode PointIterator::advance( long i )
{
	if( ! is_end() )
	{
		if( 0 == i && __vertexes == NULL && __current_vertex != NULL )
		{
			// ポイントの場合は、0番目を指定された時は何もしない
			return errorcode::sindyErr_NoErr;
		}
		else if( __vertexes )
		{
			if( (     __reverse   && SUCCEEDED( __vertexes->Skip( i + 1 ) ) ) || 
				( ( ! __reverse ) && SUCCEEDED( __vertexes->Skip( i - 1 ) ) ) ) // Skip後に++するので
			{
				++(*this);
				return errorcode::sindyErr_NoErr;
			}
			else
				init();
		}
		else
			init();
	}
	return errorcode::sindyErr_AlgorithmFailed;
}
void PointIterator::init( IGeometry* _lp/* = NULL*/, bool _reverse/* = false*/ ){
	// 初期化
	__current_part_index = __current_part_vertex_index = __current_vertex_index = -1;
	__geometry = _lp;
	__current_vertex = _lp;
	__vertexes = NULL;
	__reverse = _reverse;

	IPointCollectionPtr _col( __geometry );
	if( _col != NULL )
	{
		if( SUCCEEDED( _col->get_EnumVertices( &__vertexes ) ) )
		{
			if( __reverse )
				__vertexes->ResetToEnd();
			else
				__vertexes->Reset();
			++(*this);
		}
	}
	else {
		__current_vertex = _lp;
		if( __current_vertex != NULL )
			__current_part_index = __current_part_vertex_index = __current_vertex_index = 0;
	}
}
bool PointIterator::is_end() const {
	return ( __geometry == NULL );
}

} // sindy
