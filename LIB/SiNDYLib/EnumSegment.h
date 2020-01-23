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
 * CEnumSegment�N���X��`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
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
 * @brief <b>�Z�O�����g�C�e���[�^�N���X</b>\n
 * ���̃N���X��IEnumSegment�C���^�[�t�F�[�X�̃��b�v�N���X�ł��B\n
 * C++�W�����C�u�����̃C�e���[�^�Ɠ��l�̎g�������ł���悤�ɂ��Ă���܂��B\n
 * �R�s�[�R���X�g���N�^�y�уI�y���[�^��IEnumSegment�����L����`�ɂȂ�܂��̂ŁA
 * ���Ƃ���CGeometry::segment_ieterator���R�s�[���Ĉ�O�̂��̂�ێ��Ƃ������Ƃ�
 * �ł��Ȃ����Ƃɒ��ӂ��Ă��������B
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

	//@{ @name IEnumSegment�C���^�[�t�F�[�X + ��
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
	 * @brief <b>���݂̃Z�O�����g�Ɨ^����ꂽ�Z�O�����g�̂Ȃ��p�����߂�</b>\n
	 * �C�e���[�^�����ݎw���Ă���Z�O�����g��FROM�Ƃ��A�����v���̊p�x�����߂܂��B\n
	 * �v�Z���́A��-��1+��2�ł��i�������A��1�̓C�e���[�^���w���Ă��錻�݂̃Z�O�����g�̌��_����̊p�x�j
	 * @param ipToLine	[in]	To���̃��C��
	 * @retval 0�`360�x	����I��
	 * @retval 0����	�G���[
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
