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
 * @file EnumSegment.h
 * CEnumSegmentクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _ENUMSEGMENT_H_
#define _ENUMSEGMENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LIB/WinLib/ComWrapper.h"
#include "ErrorCode.h"

//#include <algorithm>
namespace sindy {

#pragma warning(push)
#pragma warning(disable: 4275)
/*
class SegmentIterator : std::iterator<std::bidirectional_iterator_tag, CEnumSegment*>
{
	typedef std::iterator<std::bidirectional_iterator_tag, CEnumSegment*> iterator_base;
public:
	SegmentIterator(){}
	SegmentIterator( const iteratorbase& in ) : m_Iterator(in) {}
	virtual ~SegmentIterator(){}
	SegmentIterator& operator*() { return *(reinterpret_cast<CEnumSegment*>(*m_Iterator)); }
	SegmentIterator* operator->() { return reinterpret_cast<CEnumSegment*>(*m_Iterator); }

	SegmentIterator& operator++() { return SegmentIterator(++m_Iterator); }
	SegmentIteartor& operator--() { return SegmentIterator(--m_Iterator); }
};
*/
/**
 * @class CEnumSegment
 * @brief <b>セグメントイテレータクラス</b>\n
 * このクラスはIEnumSegmentインターフェースのラップクラスです。\n
 * C++標準ライブラリのイテレータと同様の使い方ができるようにしてあります。\n
 * コピーコンストラクタ及びオペレータはIEnumSegmentを共有する形になりますので、
 * たとえばCGeometry::segment_ieteratorをコピーして一つ前のものを保持ということは
 * できないことに注意してください。
 */
class SINDYLIB_API CEnumSegment : public CComWrapper<IEnumSegment>
{
	friend class CEnumSegment;
public:
	explicit CEnumSegment() : m_lPart(-1), m_lIndex(-1){}
	virtual ~CEnumSegment(){}
	explicit CEnumSegment( IEnumSegment* lp ) : m_lPart(-1), m_lIndex(-1){ *this = lp; }
	/*explicit */CEnumSegment( const CEnumSegment& obj ) : m_lPart(-1), m_lIndex(-1){ *this = obj; }
	CEnumSegment& operator=( IEnumSegment* lp ){ AtlComPtrAssign((IUnknown**)&p, lp ); if( p ){ Reset(); ++(*this); } return *this; }
	CEnumSegment& operator=( const CEnumSegment& obj ){
		if( obj.p != NULL ){
			AtlComPtrAssign((IUnknown**)&p, obj.p );
			m_lPart = obj.m_lPart;
			m_lIndex = obj.m_lIndex;
			m_ipSeg = obj.m_ipSeg;
		}
		return *this;
	}
	operator IEnumSegment*() const{ return p; }
	operator _ISegment*() const { return m_ipSeg; }
	
	bool operator==( const CEnumSegment& obj ) const;
	bool operator!=( const CEnumSegment& obj ) const { return ! operator==( obj ); }
	CEnumSegment& operator++();
	CEnumSegment& operator--();
	_ISegment* operator->(){ return (_ISegment*)*this; }
	_ISegment* operator*(){ return (_ISegment*)*this; }
/*	const CEnumSegment& operator+( long n ) const;
	const CEnumSegment& operator-( long n ) const;*/

	//@{ @name IEnumSegmentインターフェース + α
	IEnumSegmentPtr Clone() const;
	bool IsFirstInPart() const { return ( 0 == m_lIndex ) ? true : false; }
	bool IsLastInPart() const;
	_ISegmentPtr Next( long* outPartIndex = NULL, long* SegmentIndex = NULL ) const;
	_ISegmentPtr NextInPart( long* OutVertexIndex = NULL ) const;
	_ISegmentPtr Previous( long* outPartIndex = NULL, long* SegmentIndex = NULL ) const;
	errorcode::sindyErrCode Reset() const;
	errorcode::sindyErrCode ResetToEnd() const;
	errorcode::sindyErrCode SetAt( long iPart, long Segment ) const;
	errorcode::sindyErrCode Skip( long numSegments ) const;
	//@}
	/**
	 * @brief <b>現在のセグメントと与えられたセグメントのなす角を求める</b>\n
	 * イテレータが現在指しているセグメントをFROMとし、半時計回りの角度を求めます。\n
	 * 計算式は、π-θ1+θ2です（ただし、θ1はイテレータが指している現在のセグメントの原点からの角度）
	 * @param ipToLine	[in]	To側のライン
	 * @retval 0～360度	正常終了
	 * @retval 0未満	エラー
	 */
	double degree( ILine* ipFromLine ) const;
	long part() const { return m_lPart; }
	long index() const { return m_lIndex; }
	IPointPtr from() const { IPointPtr ipPoint; if( NULL != m_ipSeg ) m_ipSeg->get_FromPoint( &ipPoint ); return ipPoint; }
	IPointPtr to() const { IPointPtr ipPoint; if( NULL != m_ipSeg ) m_ipSeg->get_ToPoint( &ipPoint ); return ipPoint; }
private:
	long m_lPart, m_lIndex;
#pragma warning(push)
#pragma warning(disable: 4251)
	_ISegmentPtr m_ipSeg;
#pragma warning(pop)
};
#pragma warning(pop)
} // sindy

#endif // ifndef _ENUMSEGMENT_H_
