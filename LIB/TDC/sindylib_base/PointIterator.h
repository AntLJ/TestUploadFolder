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
 * PointIterator�N���X��`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
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
 * @brief <b>�W�I���g���|�C���g�C�e���[�^�N���X</b>\n
 * ���̃N���X�́AIEnumVertex���C�e���[�^�Ƃ��Ďg�p���邽�߂̃N���X�ł��B\n
 * �ʏ��CGeometry�̃C�e���[�^�Ƃ��Ďg�p����܂��B\n
 * �|�C���g�W�I���g���̏ꍇ�ł����삷��悤�ɂȂ��Ă��܂��B\n
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
	IGeometryPtr __geometry; //!< �n���ꂽ�`��ierase()�ɑΉ����邽�߂ɕK�v�j
	IEnumVertexPtr __vertexes; //!< �\���_��iIPointCollection���T�|�[�g������̗p�j
	IPointPtr __current_vertex; //!< ���݂̍\���_
	long __current_part_index, __current_vertex_index, __current_part_vertex_index;
	bool __reverse; //!< ���o�[�X�C�e���[�^�p
#pragma warning(pop)
};

} // sindy

#endif // ifndef _POINTITERATOR_H_
