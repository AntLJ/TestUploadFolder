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
* @file WKSHelper.h
*
* @brief WKSPoint���g�p�����W�I���g���n�֐���`�t�@�C��
*/
#if !defined( __WKSHELPER_H__)
#define __WKSHELPER_H__	//!< �d���C���N���[�h�h�~
#include <stdarg.h>
//#include "define.h"
#include <list>
using namespace std;

// �w�b�_�ł��낢���肪�������Ă���̂ŁA���[�J���Ɏ����Ă������� -------------------------------
const static DOUBLE Pai = 3.1415926535897932384626433832795;
// �W�I���g����r���Z�p
enum sindyeSpatialRel
{
	sindyeSpatialRelNone		= 0,	//!< �֘A���Ȃ�
	sindyeSpatialRelTouch		= 1,	//!< �ڐG
	sindyeSpatialRelOverlap		= 2,	//!< �I�[�o�[���b�v
	sindyeSpatialRelCross		= 4,	//!< ����
	sindyeSpatialRelWithin		= 8,	//!< �܂܂��
	sindyeSpatialRelContain		= 16,	//!< �܂�ł���
	sindyeSpatialRelDisjoint	= 32,	//!< ����
	sindyeSpatialRelEqual		= 64,	//!< ������
};

// <�����܂�> ---------------------------------------------------------------------------------


struct SIMPLERING {
	LONG INDX;	// 2003.10.02
	LONG START;
	LONG END;
};

struct RING {
	LONG NUM;
	LONG START;
	LONG END;
	LONG XMIN;
	BOOL IsClockwise;
	list<SIMPLERING> Contain;	// ���̃����O���܂�ł��郊���O�̔ԍ����X�g
	list<SIMPLERING> Within;	// ���̃����O���܂܂�Ă��郊���O�̔ԍ��̃��X�g
	list<SIMPLERING> Cross;		// ���̃����O�ƃN���X���Ă��郊���O�̔ԍ��̃��X�g
	list<SIMPLERING> Disjoint;	// ���̃����O�Ɗ֘A�̂Ȃ������O�̔ԍ��̃��X�g
};

struct SEGMENT {
	LONG p1;
	LONG p2;
	BOOL IsReverse;	//!< �_��̌�����p2->p1�̏ꍇ��TRUE
};

enum sindyeChkInOut {
	sindyeChkInOutIn,
	sindyeChkInOutOut,
	sindyeChkInOutSame,
};

/**
 * @brief 3�_�̓��ς����߂�
 *
 * @note �_�̌����� 1 -> 2 -> 3
 *
 * @return ����
 */
DOUBLE innerproduct( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE x3, DOUBLE y3 );

/**
 * @brief 2�_�̒��������߂�
 */
DOUBLE length( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2 );

/**
 * @brief 2�_����Ȃ�����̊p�x�����߂�
 *
 * @note http://www.inet-lab.org/ted/program/prog020.html#020.9 ���Q��
 *
 * @param x1	[in]	DOUBLE
 * @param y1	[in]	DOUBLE
 * @param x2	[in]	DOUBLE
 * @param y2	[in]	DOUBLE
 *
 * @retval �p�x�i0�`360�x�j
 */
double angle( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2 );

/**
 * @brief 3�_����Ȃ�����̊p�x�����߂�
 *
 * @param x1	[in]	DOUBLE
 * @param y1	[in]	DOUBLE
 * @param x2	[in]	DOUBLE
 * @param y2	[in]	DOUBLE
 * @param x3	[in]	DOUBLE
 * @param y3	[in]	DOUBLE
 *
 * @retval �p�x�i-180�`180�x�j
 */
DOUBLE angle( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE x3, DOUBLE y3 );

/**
 * @brief �_������_�𒆐S�ɉ�]������
 *
 * @note http://www.inet-lab.org/ted/program/prog016.html#016.1 ���Q��
 *
 * @param xc	[in]	��]�̒��SX���W
 * @param yc	[in]	��]�̒��SY���W
 * @param rot	[in]	��]�p�i���W�A���j
 * @param xsrc	[in]	��]������_��X���W
 * @param ysrc	[in]	��]������_��Y���W
 * @param xret	[out]	��]��̓_��X���W
 * @param yret	[out]	��]��̓_��Y���W
 */
void rotation( DOUBLE xc, DOUBLE yc, DOUBLE rot, DOUBLE xsrc, DOUBLE ysrc, DOUBLE* xret, DOUBLE* yret );

/**
 * @brief �p�x�i0�`360�j���烉�W�A���֕ϊ�����
 *
 * @param angle		[in]	�p�x�i0�`360�j
 *
 * @return ���W�A���iDOUBLE�j
 */
DOUBLE angle2rad( DOUBLE angle );

/**
 * @brief ���W�A������p�x�i0�`360�j�֕ϊ�����
 *
 * @param rad		[in]	���W�A��
 *
 * @return �p�x�i0�`360�j�iDOUBLE�j
 */
DOUBLE rad2angle( DOUBLE rad );

/**
 * @brief �w�肳�ꂽ�����O�����v��肩�ǂ������ׂ�
 *
 * @note http://www.inet-lab.org/ted/program/prog032.html ���Q��
 *
 * @param ring		[in,out]	�����O�̊J�n�C���f�b�N�X
 * @param lEnd		[in]		�����O�̏I���C���f�b�N�X
 * @param pPoints	[in]		�|���S���_��
 *
 * @retval TRUE ���v���
 * @retval FALSE �����v���
 */
BOOL IsClockwise( RING& ring, WKSPoint*& pPoints );

/**
 * @brief �_ test �� base �̃����O�ɓ����邩
 *
 * @param base		[in]	RING
 * @param test		[in]	��������_�̃C���f�b�N�X
 * @param pPoints	[in]	�_��
 *
 * @see http://www.inet-lab.org/ted/program/prog033.html
 *
 * @retval sindyuChkInOutIn ������
 * @retval sindyuChkInOutOut �����Ȃ�
 * @retval sindyuChkInOutSame ����_������
 */
sindyeChkInOut inside( const RING& base, LONG test, const WKSPoint*& pPoints );

/**
 * @brief base��test�̃����O�ɑ΂��ăW�I���g���̊֌W�𒲂ׂ�
 *
 * @note within�Acontain�ɂ��Ē��ׂ܂�
 * @note
 * @note base��test�Ɋ܂܂��ꍇ��within��Ԃ��܂�
 * @note base��test���܂ޏꍇ��contain��Ԃ��܂�
 * @note �ǂ�����ǂ���Ɋ܂܂�Ȃ��ꍇ��none��Ԃ��܂�
 * @note 
 * @note �X�s�[�h�d���̂��߁Aoverlap���Ă��Ă�none�ƂȂ�܂����A
 * @note ���̃v���O������ł͖�肠��܂���B���̃A���S���Y���ɓK�p
 * @note ����ꍇ�͒��ӂ��ĉ������B
 *
 * @param base		[in]	RING
 * @param test		[in]	RING
 * @param pPoints	[in]	�|�C���g��
 *
 * @retval sindyeSpatialRelWithin base��test�Ɋ܂܂��
 * @retval sindyeSpatialRelContain base��test���܂�
 * @retval sindyeSpatialRelNone base��test�̊֘A���s��
 */
sindyeSpatialRel TestRelationship( const RING& base, const RING& test, WKSPoint*& pPoints, WKSPoint& errPoint );

/**
 * @brief WKSPoint -> RING �R���o�[�^
 *
 * @param lPointCount	[in]	���|�C���g��
 * @param pPoints		[in]	�|�C���g�z��
 */
void InitRing( LONG lPointCount, WKSPoint*& pPoints, list<RING>& listRings );

/**
 * @brief �����O�̕�܊֌W���`�F�b�N����
 *
 * @note InitRing ���ɌĂԕK�v������܂�
 *
 * @param pPoints		[in]	�|�C���g�z��
 * @param listRings		[in]	�������ς݂̃����O���X�g
 */
void CheckExteriorInterior( WKSPoint*& pPoints, list<RING>& listRings );

#endif // if !defined( __WKSHELPER_H__)
