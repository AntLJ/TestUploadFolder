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
 * @file GeometryIterator.h
 * GeometryIteratorクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _GEOMETRYITERATOR_H_
#define _GEOMETRYITERATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace sindy {
/**
 * @class GeometryIterator
 * @brief <b>ジオメトリコレクションイテレータクラス</b>\n
 * 通常はCGeometryのイテレータとして使用されます。\n
 * ポイントジオメトリの場合でも動作するようになっています。\n
 */
class SINDYLIB_API GeometryIterator
{
	friend class GeometryIterator;
public:
	explicit GeometryIterator(){ init(); }
	virtual ~GeometryIterator(){}
	explicit GeometryIterator( IGeometry* _lp ){ init( _lp ); }
	GeometryIterator& operator++() {
		if( __max_count <= ++__index )
			init();
		else
			__current = NULL; // operator*、operator->で必要なときに入れるのでここは毎回クリアする

		return *this;
	}
	const GeometryIterator& operator++() const {
		return const_cast<GeometryIterator*>(this)->operator++();
	}
	IGeometry* operator*(){
		if( ! is_end() && NULL != __geom_col )
			__geom_col->get_Geometry( __index, &__current );
		else if( ! is_end() && NULL != __geom )
			__current = __geom;
		return __current;
	}
	IGeometry* operator*() const { return const_cast<GeometryIterator*>(this)->operator*(); }
	IGeometry* operator->() { return operator*(); }
	bool operator ==( const GeometryIterator& _it ) const {
		return ( ( __geom.GetInterfacePtr() == _it.__geom.GetInterfacePtr()/* アドレスで比較 */ ) && ( __index == _it.__index ) ) ? true : false;
	}
	bool operator !=( const GeometryIterator& _it ) const { return !( this->operator==( _it ) ); }
	long index() const { return __index; }
private:
	bool is_end() const {
		return ( NULL == __geom && NULL == __current && NULL == __geom_col && -1 == __index && 0 == __max_count );
	}
	void init( IGeometry* _lp = NULL ){
		__geom_col = __geom = _lp;
		__current = NULL;
		__index = ( _lp ) ? 0 : -1;
		if( NULL != __geom_col )
			__geom_col->get_GeometryCount( &__max_count );
		else if( NULL != __geom )
			__max_count = 1;
		else __max_count = 0;
	}
private:
#pragma warning(push)
#pragma warning(disable: 4251)
	IGeometryPtr __geom;
	IGeometryPtr __current;
	IGeometryCollectionPtr __geom_col;
	long __index, __max_count;
#pragma warning(pop)
};

} // sindy

#endif // ifndef _GEOMETRYITERATOR_H_
