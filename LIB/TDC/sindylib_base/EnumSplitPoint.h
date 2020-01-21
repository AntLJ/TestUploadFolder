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
 * @file EnumSplitPoint.h
 * CEnumSplitPointクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _ENUMSPLITPOINT_H_
#define _ENUMSPLITPOINT_H_

#include "EnumVertex.h"
#include "Point.h"

namespace sindy {

class SplitPointIterator;
/**
 * @class CEnumSplitPoint
 * @brief <b>CEnumSplitPointクラス</b>\n
 * このクラスは、IEnumSplitPointのラッパークラスです。
 * ポイント列を扱いやすいように、イテレータを用意してあります。
 * @warning Release でインスタンスを受け取った場合には、使用する前に必ず
 * Reset を呼んでください。\n
 * コンストラクタ・コピーコンストラクタを使用した場合は自動的にResetされます
 * ので自分で呼ぶ必要はありません。
 * @warning イテレータは一度に一つしか使用できません。複数使用すると、
 * 後に定義した方が優先されますので注意してください。\n\n
 * このクラスは、主にCGeometryクラス内部で使用されます。ので通常は単独で
 * 使用することはありません。
 */
class CEnumSplitPoint : public CEnumVertex
{
public:
	explicit CEnumSplitPoint(){}
	virtual ~CEnumSplitPoint(){}
	explicit CEnumSplitPoint( IEnumSplitPoint* lp ){ *this = lp; }
	CEnumSplitPoint& operator=( IEnumSplitPoint* lp ){ AtlComPtrAssign((IUnknown**)&p, lp ); if( p ) Reset(); return *this; }

	double GetSplitDistance() const;
	bool SplitHappened() const;
	IPointPtr NextInSequence( long* Part = NULL, long* Vertex = NULL ) const;
	IPointPtr PreviousInSequence( long* Part = NULL, long* Vertex = NULL ) const;
	typedef SplitPointIterator iterator;
	typedef const SplitPointIterator const_iterator;
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
};

class SplitPointIterator
{
	friend class SplitPointIterator;
public:
	explicit SplitPointIterator(){ init(); }
	virtual ~SplitPointIterator(){}
	explicit SplitPointIterator( IEnumSplitPoint* _lp ){ init(); __enum_ver = _lp; operator++(); }
	const SplitPointIterator& operator++() const {
		return ++(*const_cast<SplitPointIterator*>(this));
	}
	SplitPointIterator& operator++() {
		if( __enum_ver != NULL )
		{
			__current = __enum_ver.Next( &__part, &__index );
			if( __current == NULL )
				init();	// 終わりまで行った
		}
		else
			init();

		return *this;
	}
	CPoint* operator->() { return &__current; }
	bool operator !=( const SplitPointIterator& _it ) const { return !( *this == _it ); }
	bool operator ==( const SplitPointIterator& _it ) const { return is_end() && _it.is_end() || ( __current.Equals( _it.__current ) && ( __part == _it.__part ) && ( __index == _it.__index ) ); }
	IPointPtr operator*() { return (IPoint*)__current; }
	IPointPtr operator*() const { return (IPoint*)__current; }
	long part() const { return __part; }
	long index() const { return __index; }
private:
	bool is_end() const { return  __enum_ver == NULL && __current == NULL && -1 == __part && -1 == __index; }
	void init(){ __enum_ver = NULL; __current = NULL; __part = __index = -1; }
private:
	CEnumSplitPoint __enum_ver;
	CPoint __current;
	long __part, __index;
};

inline CEnumSplitPoint::iterator CEnumSplitPoint::begin(){ return SplitPointIterator( (IEnumSplitPointPtr)p ); }
inline CEnumSplitPoint::iterator CEnumSplitPoint::end(){ return SplitPointIterator( NULL ); }
inline CEnumSplitPoint::const_iterator CEnumSplitPoint::begin() const { return SplitPointIterator( (IEnumSplitPointPtr)p ); }
inline CEnumSplitPoint::const_iterator CEnumSplitPoint::end() const { return SplitPointIterator( NULL ); }

} // sindy

#endif // ifndef _ENUMSPLITPOINT_H_
