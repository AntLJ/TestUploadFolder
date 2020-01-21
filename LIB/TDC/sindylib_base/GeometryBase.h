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
 * @file Geometry.h
 * CGeometryBase�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _GEOMETRYBASE_H
#define _GEOMETRYBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointIterator.h"
#include "GeometryIterator.h"
#include <utility>
#include "EditDef.h"

namespace sindy {
class CEnumSegment;
class CBasicLogicRule;
class CErrorInfosBase;
/**
 * @class CGeometryBase
 * @brief <b>�W�I���g���Ǘ��N���X</b>\n
 * �S�ẴW�I���g���N���X�͂��̃N���X�����ɂȂ�܂��B\n\n
 * ���̃N���X���p�������N���X���쐬����ꍇ�A�R�s�[�R���X�g���N�^
 * �͌p������Ȃ����߁A�R�s�[�R���X�g���N�^�Ɋւ��Ă͎������Ȃ���
 * �K�v������܂��B�܂��A���̍ۂɂ͕K��Init()���Ă�œK�؂Ƀ����o
 * �����������Ă��������B
 * @see operator=
 */
#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

class CGeometryBase : public CComWrapper<IGeometry>  
{
public:
	explicit CGeometryBase()
	{
	}
	virtual ~CGeometryBase()
	{
	}

	explicit CGeometryBase( const CGeometryBase& obj )
	{
		*this = obj;
	}

	CGeometryBase( IGeometry* lp )
	{
		*this = lp;
	}
	explicit CGeometryBase( const GUID& guid )
	{
		IGeometryPtr lp;
		if( SUCCEEDED( lp.CreateInstance( guid ) ) )
			*this = lp;
	}
	CGeometryBase& operator=( const CGeometryBase& obj )
	{
		*this = obj.Clone();
		return *this;
	}
	CGeometryBase& operator=( IGeometry* lp )
	{
		AtlComPtrAssign((IUnknown**)&p, lp );
		return *this;
	}
	operator IGeometry*() const
	{
		return (IGeometry*)p;
	}
	
	operator IGeometry*()
	{
		return (IGeometry*)p;
	}
	operator IUnknown*() const { return p; }
	operator IUnknown*() { return p; }

	//@{ @name IGeometry+���C���^�[�t�F�[�X
	esriGeometryDimension GetDimension() const;
	IEnvelopePtr GetEnvelope() const;
	esriGeometryType GetGeometryType() const;
	bool IsEmpty() const;
	errorcode::sindyErrCode Project( ISpatialReference* newReferenceSystem );
	errorcode::sindyErrCode QueryEnvelope( IEnvelope* outEnvelope ) const;
	errorcode::sindyErrCode SetEmpty();
	errorcode::sindyErrCode SnapToSpatialReference();
	ISpatialReferencePtr GetSpatialReference() const;
	errorcode::sindyErrCode PutRefSpatialReference( ISpatialReference* spatialRef );
	double GetLength() const;
	//@}
	//@{ @name IPointCollection�C���^�[�t�F�[�X�{�|�C���g�C�e���[�^
	/**
	 * @brief <b>�W�I���g���̑��\���_�����擾����</b>\n
	 * �����̃p�[�g������ꍇ�A���ꂼ��̍��v��Ԃ��܂��B
	 * @return �\���_��
	 */
	long GetPointCount() const;

	/**
	 * @brief <b>�W�I���g���̍\���_���擾����</b>\n
	 * ���̊֐����Ԃ��|�C���g�͑S�ăR�s�[�ł��B���̂��߁A���̊֐��ɂ����
	 * ����ꂽ�|�C���g�ɕύX�������Ă����̃W�I���g���ɂ͕ύX�͂���܂���B\n
	 * �L���b�V��������ꍇ�ƂȂ��ꍇ�ɕ�����K�v�����邽�߁A������private��
	 * �����Ă�ł��܂��B
	 * @see GetPoint( long, IGeometryPtr&, IPointCollectionPtr& )
	 * <h3>��F</h3>
	 * @code
	 * const CGeometryBase* pGeom = cFeat.GetShape();
	 * for( long i = 0; i < pGeom->GetPointCount(); ++i )
	 * {
	 *     printf( "X=%lf Y=%lf\n"), pGeom->GetPoint(i)->GetX(), pGeom->GetPoint(i)->GetY() );
	 * }
	 * @endcode
	 * @param index	[in]	�\���_�C���f�b�N�X�ԍ�
	 * @return CPoint
	 */
	CPoint GetPoint( long index ) const;

	/**
	 * @brief <b>�W�I���g���̍\���_���擾����</b>\n
	 * �p�[�g���w�肵�č\���_���擾����ȊO�� GetPoint( long ) �ƈꏏ�ł��B
	 * �������Aindex �͂��̃p�[�g���ł̃C���f�b�N�X�ɂȂ�܂��B
	 * @param part	[in]	�p�[�g�C���f�b�N�X�ԍ�
	 * @param index	[in]	�p�[�g���\���_�C���f�b�N�X�ԍ�
	 * @return CPoint
	 */
	CPoint GetPoint( long part, long index ) const;

	/**
	 * @brief <b>�\���_�̃C�e���[�^���擾����</b>\n
	 * IPointCollection::get_EnumVertices���g�p���܂��B
	 * IPointCollection���T�|�[�g���Ă��Ȃ��W�I���g���̏ꍇ�́ANULL���Ԃ�܂��B����
	 * �ꍇ�A�����ŃG���[���������܂��̂ŁAIPointCollection���T�|�[�g���Ă��Ȃ��W�I���g��
	 * �ł͎g�p���Ȃ��ł��������B
	 * @param queryGeometry	[in,optional]	�N�G���`��i�f�t�H���g�FNULL�j
	 * @return IEnumVertexPtr
	 */
	IEnumVertexPtr GetEnumVertex() const;

	errorcode::sindyErrCode UpdatePoint( long i, IPoint* p );

	typedef PointIterator point_iterator;
	typedef const PointIterator const_point_iterator;
	point_iterator point_begin(){ return PointIterator( (IGeometry*)*this ); }
	point_iterator point_end(){ return PointIterator(); }
	const_point_iterator point_begin() const { return PointIterator( (IGeometry*)*this ); }
	const_point_iterator point_end() const { return PointIterator(); }
	errorcode::sindyErrCode point_unique();
	//@}
	//@{ @name ISegmentCollection�C���^�[�t�F�[�X�{�Z�O�����g�C�e���[�^
	/**
	 * @brief <b>�Z�O�����g����Ԃ�</b>\n
	 * �|�C���g�n�̃W�I���g���̏ꍇ�́A���-1���Ԃ�܂��B
	 * �܂��A���̏ꍇ�͓����ŃG���[���������܂��̂ŁA�|�C���g�n
	 * �̃W�I���g���ɂ��Ă͂��̃��\�b�h�͎g�p���Ȃ��ł��������B
	 * @retval 0����	�G���[
	 * @retval 0�ȏ�	�Z�O�����g��
	 */
	long GetSegmentCount() const;

	/**
	 * @brief <b>�Z�O�����g���擾����</b>\n
	 * �|�C���g�n�̃W�I���g���̏ꍇ�́A���-1���Ԃ�܂��B
	 * �܂��A���̏ꍇ�͓����ŃG���[���������܂��̂ŁA�|�C���g�n
	 * �̃W�I���g���ɂ��Ă͂��̃��\�b�h�͎g�p���Ȃ��ł��������B
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	�Z�O�����g
	 */
	_ISegmentPtr GetSegment( long index ) const;

	/**
	 * @brief <b>�Z�O�����g��ǉ�����</b>\n
	 * �C���f�b�N�X�ԍ���-1�ɂ���ƁAbAddAfter�Ɋ֌W�Ȃ���Ɉ�ԍŌ�ɑ}������܂��B\n
	 * bAddAfter��true�ɂ���ƁA�w�肵���C���f�b�N�X�ԍ��̃Z�O�����g�͒ǉ������Z�O�����g�ɂȂ�܂��B\n
	 * �|�C���g�n�̃W�I���g���ɂ��Ă͂��̃��\�b�h�͎g�p���Ȃ��ł��������B
	 * @param inSegment		[in]			�ǉ�����Z�O�����g
	 * @param lInsertIndex	[in,optional]	�}������C���f�b�N�X�ԍ��i�f�t�H���g�F-1�j
	 * @param bAddAfter		[in,optional]	�C���f�b�N�X�ԍ��̌�ɑ}�����邩�ǂ����i�f�t�H���g�Ftrue�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode AddSegment( _ISegment* inSegment, long lInsertIndex = -1, bool bAddAfter = true );

	typedef CEnumSegment const_segment_iterator;
	const_segment_iterator segment_begin( IEnvelope* ipQuery = NULL ) const;
	const_segment_iterator segment_end() const;
	/**
	 * @brief <b>�d���Z�O�����g���폜����</b>
	 * Simplify�n�͎g�p�����ɏd���Z�O�����g���폜���܂��B\n
	 * segment_iterator���g�p���Ă���ꍇ�A���̊֐��ɂ����
	 * �C�e���[�^�����������Ȃ邱�Ƃ�����̂ł��̊֐��g�p�O��
	 * �擾�����C�e���[�^�͂��̊֐��g�p��Ɏg�p���Ȃ��ł��������B\n
	 * �܂��A�Z�O�����g���폜���邱�Ƃɂ��Z�O�����g���m�̂Ȃ��肪
	 * ���f����܂����A�����ł͂��̕��f�ꏊ���C�����܂���B\n
	 * ���f�������ƏC������ɂ�Simplify�n���g�p����K�v������܂����A
	 * �ꍇ�ɂ���Ă̓}���`�p�X�Ⓡ�|���S�����ł��Ă��܂��ꍇ������̂�
	 * �\���ɒ��ӂ��Ă��������B
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode segment_unique();
	//@}
	//@{ @name IGeometryCollection�C���^�[�t�F�[�X�{�W�I���g���C�e���[�^�i�C�e���[�^�͑S�ĎQ�Ƃ�Ԃ����Ƃɒ��Ӂj
	/**
	 * @brief <b>�p�[�g�W�I���g����Ԃ�</b>\n
	 * ���̊֐��ŕԂ�W�I���g���̓R�s�[�ł͂Ȃ��Q�ƂȂ̂ŁA
	 * ���̊֐��ɂ���Ď擾�����W�I���g���ɑ΂��ĕύX���������
	 * �匳�̃W�I���g���ɑ΂��ĕύX�����������ƂɂȂ�܂��̂�
	 * ���ӂ��Ă��������B
	 * @param Index		[in]	�p�[�g�C���f�b�N�X�ԍ�
	 * @return IGeometryPtr	�p�[�g�W�I���g���i�Q�Ƃł��邱�Ƃɒ��Ӂj
	 */
	IGeometryPtr GetPart( long Index ) const;

	long GetPartCount() const;
	typedef GeometryIterator part_iterator;
	typedef const GeometryIterator const_part_iterator;
	part_iterator part_begin(){ return GeometryIterator( (IGeometry*)*this ); }
	part_iterator part_end(){ return GeometryIterator(); }
	const_part_iterator part_begin() const { return GeometryIterator( (IGeometry*)*this ); }
	const_part_iterator part_end() const { return GeometryIterator(); }
	errorcode::sindyErrCode part_unique();
	//@}
	//@{ @name IHitTest�C���^�[�t�F�[�X�{��
	bool HitTest( IPoint* QueryPoint, const double& searchRadius, esriGeometryHitPartType geometryPart, IPoint* hitPoint = NULL, double* hitDistance = NULL, long* hitPartIndex = NULL, long* hitSegmentIndex = NULL, bool* bRightSide = NULL ) const;
	bool HitTestSegment( IPoint* QueryPoint, const double& searchRadius, IPoint* hitPoint = NULL, double* hitDistance = NULL, long* hitPartIndex = NULL, long* hitSegmentIndex = NULL, bool* bRightSide = NULL ) const
	{ return HitTest( QueryPoint, searchRadius, esriGeometryPartBoundary, hitPoint, hitDistance, hitPartIndex, hitSegmentIndex, bRightSide ); }
	bool HitTestPoint( IPoint* QueryPoint, const double& searchRadius, IPoint* hitPoint = NULL, double* hitDistance = NULL, long* hitPartIndex = NULL, long* hitSegmentIndex = NULL, bool* bRightSide = NULL ) const
	{ return HitTest( QueryPoint, searchRadius, esriGeometryPartVertex, hitPoint, hitDistance, hitPartIndex, hitSegmentIndex, bRightSide ); }
	//@}
	//@{ @name IRelationalOperator�C���^�[�t�F�[�X
	bool Contains( IGeometry* ipGeom ) const;
	bool Crosses( IGeometry* ipGeom ) const;
	bool Disjoint( IGeometry* ipGeom ) const;
	bool Equals( IGeometry* ipGeom, bool bIgnoreOrient = false ) const;
	/**
	 * @brief �_���r�œ������ǂ������肷��
	 * 
	 * �_���r�ɂ�蓯���`�󂩂ǂ����𔻒肵�܂��B\n
	 * ���W�l�Œ��ɔ�������邽�߁AStore��ɓ���ƂȂ�ꍇ��
	 * �_��̌����i���C���j�A�n�I�_�̈Ⴂ�i�|���S���j���قȂ�
	 * �v���Ƃ��Ĕ��肳��邱�Ƃɒ��ӂ��Ă��������B\n
	 * �܂��A��ԎQ�Ƃ͖������邽�߁A�قȂ��ԎQ�Ƃ̃W�I���g����
	 * �قڕK���قȂ�`��Ƃ��Ĕ��肳��邱�Ƃɒ��ӂ��Ă��������B\n
	 * @warning �T�|�[�g����`��� IPoint�AIPointCollection�̃C���^�[�t�F�[�X
	 * �����`��݂̂ł��邱�Ƃɒ��ӂ��Ă��������B
	 */
	bool EqualsVertex( IGeometry* ipGeom ) const;
	bool Overlaps( IGeometry* ipGeom ) const;
	bool Touches( IGeometry* ipGeom ) const;
	bool Within( IGeometry* ipGeom ) const;
	//@}
	//@{ @name IProximityOperator�C���^�[�t�F�[�X
	/**
	 * @brief <b>�^����ꂽ�W�I���g���Ƃ̋������v�Z����</b>\n
	 * @retval 0���� �G���[
	 * @retval 0�ȏ� ����
	 */
	double ReturnDistance( IGeometry* ipGeom ) const;

	/**
	 * @brief <b>�^����ꂽ�_�����ԋ߂��_��Ԃ�</b>\n
	 * �\���_�Ɍ��炸�A�ӏ�ł��|�C���g�Ƃ��ĕԂ��܂��B
	 * ���̂��߁A�����ɂ͂��̃W�I���g���Ƌɋ͂��ɂ����ꍇ������
	 * �����l�����Ă��������B
	 * 
	 * ipRetPoint ���g�p������́A���ɂ���C���X�^���X�ɓ���ė~�����ꍇ
	 * �ɗL���ł��B�܂��A�������������ꍇ�ɂ��L���ł��B
	 * @param ipPoint		[in]	��r��_�|�C���g
	 * @param ipRetPoint	[out]	��ԋ߂��|�C���g�iNOT NULL�j
	 * @return ��ԋ߂��|�C���g
	 */
	IPointPtr ReturnNearestPoint( IPoint* ipPoint ) const;
	IPointPtr QueryNearestPoint( IPoint* ipPoint, IPoint* ipRetPoint ) const;

	//@{ @name ITopoligicalOprator�C���^�[�t�F�[�X
	IGeometryPtr Intersect( IGeometry* ipGeom, esriGeometryDimension emGeomDim ) const;
	IGeometryPtr Buffer( const double& Distance ) const;
	bool Simplify( bool force = false );

	//@}
	//@{ @name ���̑����Z�n
	/**
	 * @brief <b>�W�I���g���̈ړ��������s��</b>\n
	 * �^����W�I���g���ɂ���Č��ʂ��ς��܂��F
	 * @li Polygon�AEnvelope�ARing : �����t���s�ړ��i�^����ꂽ�`��̒��̂ݕ��s�ړ��A���ڂ͂Ȃ������ڂ̃Z�O�����g���ړ��j
	 * @li Point                   : �\���_�̈ړ��iMoveVertex�������I�ɌĂ΂��j
	 * @li NULL                    : ���s�ړ�
	 * @param ipQueryGeom		[in]	�N�G���`��
	 * @param pairMove			[in]	�ړ������i�}�b�v�P�ʁj
	 * @param emMode			[in]	�ړ����[�h
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( IGeometry* ipQueryGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode );

	/**
	 * @brief <b>�\���_�̈ړ��������s��</b>\n
	 * �N�G���`����ɑ��݂���\���_���ړ����܂��B
	 * �|�C���g�t�B�[�`���̏ꍇ�͒P���ɕ��s�ړ��ƂȂ�܂��B
	 * @param ipQueryGeometry	[in]	�N�G���`��
	 * @param pairMove			[in]	�ړ������i�}�b�v�P�ʁj
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode MoveVertex( IGeometry* ipQueryGeometry, const std::pair<double,double>& pairMove );

	/**
	 * @brief <b>�\���_��ǉ�����</b>\n
	 * IPolycurve::SplitAtPoint���g�p���āA�\���_��ǉ����܂��B\n
	 * ����̍\���_�����݂���ꍇ�ȊO�́A�K���\���_���ǉ�����܂��B���̂��߁A
	 * �n���ꂽ�|�C���g�Ƃ͊��S�ɂ͈�v���܂���B
	 * @param ipPoint			[in]			�ǉ��|�C���g
	 * @param pAddPart			[out,optional]	�ǉ����ꂽ�p�[�g�i�f�t�H���g�FNULL�j
	 * @param pAddIndex			[out,optional]	�ǉ����ꂽ�C���f�b�N�X�i�f�t�H���g�FNULL�j
	 * @return true		�ǉ����ꂽ
	 * @return false	�ǉ�����Ȃ�����
	 */
	bool AddVertex( IPoint* ipPoint, long* pAddPart = NULL, long* pAddIndex = NULL );

	/**
	 * @brief <b>�\���_��ǉ�����</b>\n
	 * ���̊֐��́AIPolycurve2::SplitAtPoints���g�p���āA�\���_��ǉ����܂��B\n
	 * ����̍\���_�����݂���ꍇ�ȊO�́A�K���\���_���ǉ�����܂��B���̂��߁A
	 * �n���ꂽ�|�C���g�Ƃ͊��S�ɂ͈�v���܂���B
	 * @param splitPoints		[in]			�ǉ��|�C���g��
	 * @param cutoffDistance	[in]			�ǉ����e����
	 * @param splitInfo			[out,optional]	�ǉ��_���i�f�t�H���g�FNULL�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode AddVertex( IEnumVertex* splitPoints, const double& cutoffDistance, IEnumSplitPoint** splitInfo = NULL );

	/**
	 * @brief <b>�\���_��ǉ�����</b>\n
	 * IPointCollection::AddPoint���g�p���āA�\���_��ǉ����܂��B\n
	 * ����̍\���_������ꍇ�ł��K���ǉ�����܂��B
	 * @param ipPoint			[in]			�ǉ��|�C���g
	 * @param lPart				[in]			�ǉ�����p�[�g
	 * @param lIndex			[in]			�ǉ�����C���f�b�N�X
	 * @param bAddBefore		[in,optional]	true�̏ꍇ�͎w�肵���C���f�b�N�X�̑O�ɒǉ��i�f�t�H���g�Ffalse�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode AddVertex( IPoint* ipPoint, long lPart, long lIndex, bool bAddBefore = false );

	/**
	 * @brief <b>�W�I���g���̍폜�������s��</b>\n
	 * �^����W�I���g���ɂ���Č��ʂ��ς��܂��F
	 * @li Polygon, Envelope, Ring : �^����ꂽ�`��ɓ��镔�������������i�S���̌`�󂪏�����ꍇ�̓G���[�j
	 * @li Point                   : �\���_�̍폜�iEraseVertex���Ă΂��j
	 * @param ipQueryGeom		[in]	�N�G���`��
	 * @retval sindyErr_NoErr					����
	 * @retval sindyErr_GeometryChangeToNull	�W�I���g���`�󂪖����Ȃ鑀��͕s�i���̏ꍇ�̓t�B�[�`�����̂������K�v������j
	 * @retval ��L�ȊO							�G���[
	 */
	errorcode::sindyErrCode Erase( IGeometry* ipQueryGeom );

	/**
	 * @brief <b>�\���_�̍폜�������s��</b>\n
	 * �\���_��������Ή������܂���B\n
	 * �|�C���g�t�B�[�`���̏ꍇ�̓G���[���Ԃ�܂��B
	 * @param ipQueryPoint		[in]	�N�G���|�C���g
	 * @retval sindyErr_NoErr					����
	 * @retval sindyErr_GeometryChangeToNull	�W�I���g���`�󂪖����Ȃ鑀��͕s�i���̏ꍇ�̓t�B�[�`�����̂������K�v������j
	 * @retval ��L�ȊO							�G���[
	 */
	errorcode::sindyErrCode EraseVertex( IPoint* ipQueryPoint );

	/**
	 * @brief <b>�W�I���g���̕����������s��</b>\n
	 * @param ipQueryGeom		[in]			�N�G���`��
	 * @param ipOtherGeometries	[out,optional]	NULL�̏ꍇ�͍\���_�̂ݒǉ��A�����łȂ��ꍇ�͕����`�󂪓���i�f�t�H���g�FNULL�j
	 * @param ipTakeOverPoint	[in,optional]	��������ꍇ�Ɉ����p���`������߂邽�߂̃|�C���g�i�f�t�H���g�FNULL�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( IGeometry* ipQueryGeom, _ISet** ipOtherGeometries = NULL, IPoint* ipTakeOverPoint = NULL );

	/**
	 * @brief <b>�|�����C����_�ŕ�������</b>\n
	 * ���̊֐��́AIPolycurve::SplitAtPoint���g�p���܂��B\n
	 * ������ɂ̓}���`�p�[�g�ɂȂ�܂����ASiNDY�f�[�^�x�[�X���ł�
	 * �|�����C���̃}���`�p�[�g�`��̓T�|�[�g���Ă��܂���̂Œ��ӂ��Ă��������B
	 * �T�|�[�g�����W�I���g���͈ȉ��̂Ƃ���F\n
	 * @li Polyline
	 * @param splitPoint		[in]			��������_
	 * @param newPartIndex		[out,optional]	��������Ăł����V�����p�[�g�C���f�b�N�X�ԍ��i�G���[�̏ꍇ��-1 �f�t�H���g�FNULL�j
	 * @param hitIndex			[out,optional]	�����ł��Ȃ������ꍇ�Ƀq�b�g�����Z�O�����g���͍\���_�ԍ���Ԃ��i�����Ԃ�̂����܂����킩���B�G���[�̏ꍇ��-1 �f�t�H���g�FNULL�j
	 * @retval false	�G���[�A���͕����ł��Ȃ�����
	 * @retval true		�����ł���
	 */
	bool CGeometryBase::Split( IPoint* splitPoint, long* newPartIndex = NULL, long* hitIndex = NULL );

	/**
	 * @brief <b>�|�����C����_��ŕ�������</b>\n
	 * ���̊֐��́AIPolycurve2::SplitAtPoints���g�p���܂��B\n
	 * ������ɂ̓}���`�p�[�g�ɂȂ�܂����ASiNDY�f�[�^�x�[�X���ł�
	 * �|�����C���̃}���`�p�[�g�`��̓T�|�[�g���Ă��܂���̂Œ��ӂ��Ă��������B
	 * �T�|�[�g�����W�I���g���͈ȉ��̂Ƃ���F\n
	 * @li Polyline
	 * @param splitPoints		[in]			��������_��
	 * @param cutoffDistance	[in]			�������e����
	 * @param splitInfo			[out,optional]	�����_���i�f�t�H���g�FNULL�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( IEnumVertex* splitPoints, const double& cutoffDistance, IEnumSplitPoint** splitInfo = NULL );

	/**
	 * @brief <b>�W�I���g���̃}�[�W�������s��</b>\n
	 * �����I��static�֐��̕����Ă�ŏ������܂��B
	 * ����IGeometry*��NULL�̏ꍇ�́A�n���ꂽ�`����R�s�[����
	 * ���Ƃ��܂��B
	 * @warning ���̊֐������s����ƕK�����W�I���g��������ւ��܂��̂Œ��ӂ��Ă��������B
	 * @param ipGeom	[in]	UNION�`��
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Union( IGeometry* ipGeom );

	/**
	 * @brief <b>�W�I���g���̃}�[�W�������s��</b>\n
	 * ���̊֐��́A������ITopologicalOperator::ConstructUnion���g�p����
	 * �}�[�W�������s�����߁AITopologicalOperator::Union�Ƃ͈قȂ�A
	 * ��̌`�󂪕����I�ɗ���Ă��Ă����v�ł��B
	 * @param ipGeom1	[in]	UNION�`��
	 * @param ipGeom2	[in]	UNION�`��
	 */
	static IGeometryPtr Union( IGeometry* ipGeom1, IGeometry* ipGeom2, errorcode::sindyErrCode* pErr = NULL );

	/**
	 * @brief <b>�W�I���g���̍������o�������s��</b>\n
	 * �����I��static�֐��̕����Ă�ŏ������܂��B
	 * ����IGeometry*��NULL�̏ꍇ�́A�n���ꂽ�`����R�s�[����
	 * ���Ƃ��܂��B
	 * @warning ���̊֐������s����ƕK�����W�I���g��������ւ��܂��̂Œ��ӂ��Ă��������B
	 * @param ipGeom	[in]	SymmetricDifference�`��
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SymmetricDifference( IGeometry* ipGeom );

	/**
	 * @brief <b>�W�I���g���̍������o�������s��</b>\n
	 * @param ipGeom1	[in]	Base�`��
	 * @param ipGeom2	[in]	Difference�`��
	 */
	static IGeometryPtr SymmetricDifference( IGeometry* ipGeom1, IGeometry* ipGeom2, errorcode::sindyErrCode* pErr = NULL );

	IGeometryPtr IntersectPoints( IGeometry* ipGeom ) const;

	//@}

	/**
	 * @brief <b>�W�I���g���̋�`���擾����</b>\n
	 * @param dXMin	[out]	�ŏ�X
	 * @param dYMin	[out]	�ŏ�Y
	 * @param dXMax	[out]	�ő�X
	 * @param dYMax	[out]	�ő�Y
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetEnvelope( double& dXMin, double& dYMin, double& dXMax, double& dYMax ) const;

	IGeometryPtr Clone() const;

	//@{ @name �W�I���g���ϊ��n
	/**
	 * @brief <b>�W�I���g������^����ꂽ�����̃W�I���g���𐶐�����</b>\n
	 * ���L�ϊ��ȊO�Ŏ����������ꍇ�̓R�s�[�𐶐����܂��B\n
	 * �T�|�[�g���Ă���ϊ��͎��̒ʂ�F\n
	 * @li Ring     -> Polygon
	 * @li Envelope -> Polygon
	 * @li Polygon  -> Polyline
	 * @li Ring     -> Polyline
	 * @li Envelope -> Polyline
	 * @li Path     -> Polyline
	 * @li Polygon  -> Multipoint
	 * @li Ring     -> Multipoint
	 * @li Envelope -> Multipoint
	 * @li Polyline -> Multipoint
	 * @li Path     -> Multipoint
	 * @li Point    -> Multipoint
	 * @param ipGeom	[in]	�W�I���g��
	 * @param emGeomDim	[in]	����
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	�ϊ���̃W�I���g��
	 */
	static IGeometryPtr Convert( IGeometry* ipGeom, esriGeometryDimension emGeomDim );

	/**
	 * @brief <b>�W�I���g������}���`�|�C���g�𐶐�����</b>\n
	 * �^����ꂽ�W�I���g�����}���`�|�C���g�̏ꍇ�̓R�s�[�𐶐����܂��B\n
	 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
	 * @li Polygon
	 * @li Ring
	 * @li Envelope
	 * @li Polyline
	 * @li Path
	 * @li Point
	 * @param ipGeom	[in]	�W�I���g��
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	�ϊ���̃W�I���g��
	 */
	static IGeometryPtr ToMultipoint( IGeometry* ipGeom );

	/**
	 * @brief <b>�W�I���g������|�����C���𐶐�����</b>\n
	 * �^����ꂽ�W�I���g�����|�����C���̏ꍇ�̓R�s�[�𐶐����܂��B\n
	 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
	 * @li Polygon
	 * @li Ring
	 * @li Envelope
	 * @param ipGeom	[in]	�W�I���g��
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	�ϊ���̃W�I���g��
	 */
	static IGeometryPtr ToPolyline( IGeometry* ipGeom );

	/**
	 * @brief <b>�W�I���g������p�X�𐶐�����</b>\n
	 * �^����ꂽ�W�I���g�����p�X�̏ꍇ�̓R�s�[�𐶐����܂��B\n
	 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
	 * @li Ring
	 * @li Envelope
	 * @param ipGeom	[in]	�W�I���g��
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	�ϊ���̃W�I���g��
	 */
	static IGeometryPtr ToPath( IGeometry* ipGeom );

	/**
	 * @brief <b>�W�I���g������|���S���𐶐�����</b>\n
	 * �^����ꂽ�W�I���g�����|���S���̏ꍇ�̓R�s�[�𐶐����܂��B\n
	 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
	 * @li Ring
	 * @li Envelope
	 * @param ipGeom	[in]	�W�I���g��
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	�ϊ���̃W�I���g��
	 */
	static IGeometryPtr ToPolygon( IGeometry* ipGeom );

	/**
	 * @brief <b>�W�I���g�����烊���O�𐶐�����</b>\n
	 * �^����ꂽ�W�I���g���������O�̏ꍇ�̓R�s�[�𐶐����܂��B\n
	 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
	 * @li Envelope
	 * @param ipGeom	[in]	�W�I���g��
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	�ϊ���̃W�I���g��
	 */
	static IGeometryPtr ToRing( IGeometry* ipGeom );
	//@}
	//@{ @name �`�F�b�N�֐�
	/**
	 * @brief <b>�S�Ẵ`�F�b�N���s��</b>
	 * @param cRule		[in]	���W�b�N�`�F�b�N�p���[��
	 * @param cErrInfos	[out]	�G���[�i�[�p�R���e�i
	 * @param ipChkGeom	[in/optional]	�`�F�b�N�̈�
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CBasicLogicRule& cRule, CErrorInfosBase& cErrInfos, IEnvelope* ipQuery = NULL ) const;

	/**
	 * @brief <b>�Z�O�����g�n�`�F�b�N</b>\n
	 * ���̊֐��͈ȉ��̃`�F�b�N�����s���܂��F
	 * @li �Œ�Z�O�����g���`�F�b�N
	 * @li �����Z�O�����g�`�F�b�N
	 * @li �Z�O�����g���Ȃ��p�x�`�F�b�N
	 * @li �J�n�E�I���̂Ȃ��p�ƃZ�O�����g�̋����`�F�b�N
	 * @li ��Ȍ`��`�F�b�N
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckSegments( const CBasicLogicRule& cRule, CErrorInfosBase& cErrInfos, IEnvelope* ipQuery = NULL ) const;

	/**
	 * @brief <b>�|�C���g�n�`�F�b�N</b>\n
	 * ���̊֐��͈ȉ��̃`�F�b�N�����s���܂��F
	 * @li �Œ�|�C���g���`�F�b�N�i�K�{�j
	 * @li �d���\���_�`�F�b�N�i�K�{�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckPoints( const CBasicLogicRule& cRule, CErrorInfosBase& cErrInfos, IEnvelope* ipQuery = NULL ) const;
	//@}
#ifdef _DEBUG
	/**
	 * @brief <b>�_����g���[�X����</b>\n
	 * �g���[�X���ʂ̓f�o�b�O�E�B���h�E�ɂ̂ݕ\������܂��B
	 * �����[�X�łł͉������܂���B
	 */
	static void Trace( IGeometry* ipGeom, LPCTSTR lpcszHeader = NULL );
	void Trace( LPCTSTR lpcszHeader = NULL ) const { Trace( (IGeometry*)(*this), lpcszHeader );	}
#else
	static void Trace( IGeometry* ){}
	static void Trace( IGeometry* /*ipGeom*/, LPCTSTR/* lpcszHeader = NULL*/ ){}
	void Trace( LPCTSTR/* lpcszHeader = NULL*/ ) const {}
	void Trace() const {}
#endif // ifdef _DEBUG
private:
};
#pragma warning(pop)

/// IRelationalOperator::Contains�o�O����
bool AheGeometryContains( IGeometry* ipBase, IGeometry* ipTest );

} // sindy

#endif // _GEOMETRYBASE_H
