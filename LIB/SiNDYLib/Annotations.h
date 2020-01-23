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
 * @file Annotations.h
 * @brief <b>CAnnotations�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _ANNOTATIONS_H_
#define _ANNOTATIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RelAnnotationParameter.h"
#include "FeatureContainer.h"
#include "SpatialIndex.h"
#include "SiNDYLogFunctions.h"
#include "meshutil.h"

namespace sindy {

/**
 * @class CAnnotations
 * @brief <b>�s�s�n�}���L�Ɋւ��鑍���������s���N���X</b>\n
 * ���̃N���X�ł́A�^�ʒu�ƕ\���ʒu�̊֘A�t���A�����A�������ʂ�
 * �֘A�t���ƊǗ����s���܂��B
 */
class SINDYLIB_API CAnnotations :
	public CFeatureContainer
{
public:
	explicit CAnnotations(){}
	virtual ~CAnnotations(){ clear(); }

	explicit CAnnotations( sindyTableType::ECode emTableType, IFeatureClass* lp )
	{
		SetObject( emTableType, lp );
	}

	explicit CAnnotations( const CFeatureClass& obj )
	{
		CTable::SetObject( obj.GetTableType(), (ITable*)obj );
	}

	void SetObject( sindyTableType::ECode emTableType, IFeatureClass* lp )
	{
		CTable::SetObject( emTableType, lp );
		clear();
	}
	sindyTableType::ECode GetCurrentTableType() const {return m_emTableType;}

	/**
	 * @brief <b>���b�V���R�[�h�Ō�������</b>\n
	 * �^�ʒu�A�\���ʒu�������������܂��B�������ʂ͎����I��
	 * �֘A�t����ꃊ�X�g���ɕێ�����܂��B\n
	 * �֘A�t���邱�Ƃ��ł��Ȃ��������͎̂̂Ă��܂��i�G���[�I�u�W�F�N�g�͍쐬���܂��j�B
	 * �֘A�t�������鎖���ł��Ȃ������t�B�[�`��
	 * ���������ꍇ�ɂ�sindyErr_RelFeatureNotFound���Ԃ�܂��B
	 * @param lMeshCode		[in]			���b�V���R�[�h
	 * @param emMeshType	[in]			���b�V���^�C�v�i�~�h�����b�V���Ή��̂��� [bug 4054]�j
	 * @param bRecycling	[in,optional]	true�ɂ���ƁA�\���ʒu�̂݃��T�C�N�������B���̌�^�ʒu�A�\���ʒu�̃L���b�V�����쐬���Đ؂藣���̂ŁAbCreateCache��true�łȂ���΂Ȃ�Ȃ��i�f�t�H���g�Ffalse�j
	 * @param bCreateCache	[in,optional]	true�ɂ���ƃL���b�V�����쐬����BbRecycling��true�̎��͕K��true�ɂ��Ȃ���΂Ȃ�Ȃ��i�f�t�H���g�Ffalse�j
	 * @param pErrs			[out,optional]	�G���[�i�[�R���e�i�i�I�u�W�F�N�g���m�̊֘A�t���Ɏ��s�������̂̂ݓ���j
	 *
	 * @return sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	errorcode::sindyErrCode SelectByMesh( long lMeshCode, mesh_type::ECode emMeshType, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL );
	/**
	 * @brief <b>�`��Ō�������</b>\n
	 * �^�ʒu�A�\���ʒu�������������܂��B�������ʂ͎����I��
	 * �֘A�t����ꃊ�X�g���ɕێ�����܂��B\n
	 * �����Ńo�b�t�@�����O�Ȃǂ͂��Ȃ��̂ŁA�|�C���g�Ō�������ꍇ��
	 * �o�b�t�@�����O�����`���n���Ă��������B\n
	 * �����`����ɐ^�ʒu�A�\���ʒu�̂�
	 * ���炩�����Ȃ��ꍇ�Ɋ֘A�t�B�[�`�����e�[�u�����猟�����܂��B
	 * �֘A�t���邱�Ƃ��ł��Ȃ��������͎̂̂Ă��܂��B
	 * ���������ꍇ�ɂ�sindyErr_RelFeatureNotFound���Ԃ�܂��B
	 * @param ipGeom		[in]			�����`��
	 * @param bRecycling	[in,optional]	true�ɂ���ƁA�\���ʒu�̂݃��T�C�N�������B���̌�^�ʒu�A�\���ʒu�̃L���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @param pErrs			[out,optional]	�G���[�i�[�R���e�i�i�I�u�W�F�N�g���m�̊֘A�t���Ɏ��s�������̂̂ݓ���j
	 *
	 * @return sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	errorcode::sindyErrCode Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );

	/**
	 * @brief <b>�`��Ō�������i�����_���Ή��j</b>\n
	 * ��{�I�ɂ�SelectByShape�Ɠ����ł����A�����_���p�Ɉ�����ǉ����Ă��܂��B
	 * ���[�`�������ʉ����邽�߁ASelectByShape�����̊֐����Ă�ł��܂��B
	 * @param ipDispCursor		[in,optional]	�\���ʒu�e�[�u���ł̃J�[�\���i�f�t�H���g�FNULL�j
	 * @param pDispTableType	[in,optional]	�J�[�\���̃e�[�u���^�C�v�|�C���^�i�f�t�H���g�FNULL�j
	 * @param bCreateIndex		[in,optional]	��ԃC���f�b�N�X���쐬���邩�ǂ����i�f�t�H���g�Ftrue�j
	 **/
	errorcode::sindyErrCode _SelectByShape( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, IFeatureCursor* ipDispCursor = NULL, sindyTableType::ECode* pDispTableType = NULL,bool bCreateIndex = true  );

	/**
	 * @brief <b>�`��Ō�������</b>\n
	 * �^�ʒu�A�\���ʒu�������������܂��B�������ʂ͎����I��
	 * �֘A�t����ꃊ�X�g���ɕێ�����܂��B\n
	 * �����Ńo�b�t�@�����O�Ȃǂ͂��Ȃ��̂ŁA�|�C���g�Ō�������ꍇ��
	 * �o�b�t�@�����O�����`���n���Ă��������B\n
	 * �����`����ɐ^�ʒu�A�\���ʒu�̂�
	 * ���炩�����Ȃ��ꍇ�Ɋ֘A�t�B�[�`�����e�[�u�����猟�����܂��B
	 * �֘A�t���邱�Ƃ��ł��Ȃ��������͎̂̂Ă��܂��B
	 * ���������ꍇ�ɂ�sindyErr_RelFeatureNotFound���Ԃ�܂��B
	 * @param ipGeom		[in]			�����`��
	 * @param bRecycling	[in,optional]	true�ɂ���ƁA�\���ʒu�̂݃��T�C�N�������B���̌�^�ʒu�A�\���ʒu�̃L���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByObject( _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false, bool bInit = true );

	/**
	 * @brief <b>�n���ꂽ�I�u�W�F�N�g�Z�b�g�Ɋ֘A������̂���������</b>\n
	 * �^�ʒu�A�\���ʒu�����������܂��B�������ʂ͎����I��
	 * �֘A�t����ꃊ�X�g���ɕێ�����܂��B\n
	 * �n���ꂽ�I�u�W�F�N�g�Z�b�g���ŕ\���ʒu�A�^�ʒu����
	 * �S�ɃZ�b�g�ɂȂ��Ă��Ȃ��Ă��֘A�I�u�W�F�N�g��S��
	 * �������i�[���܂��B\n
	 * �n���ꂽ�I�u�W�F�N�g�Əd�������I�u�W�F�N�g��������
	 * �ꂽ�ꍇ�A�n���ꂽ�I�u�W�F�N�g����ɗD�悳��܂��B
	 * @param ipRowSet		[in]			�I�u�W�F�N�g�Z�b�g
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����i�f�t�H���g�Ffalse�j
	 * @param pErrs			[in,optional]	�G���[�i�[�R���e�i�i�I�u�W�F�N�g���m�̊֘A�t���Ɏ��s�������̂̂ݓ��� �f�t�H���g�FNULL�j
	 * @param bInit			[in,optional]	�����̃I�u�W�F�N�g���N���A����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bCreateIndex	[in,optional]	���L��p�̋[���C���f�b�N�X���쐬����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CAnnotations::Select( _ISet* ipRowSet, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/, bool bCreateIndex/* = true*/ );


	//@{ @name CFeaturesContainer �C���^�[�t�F�[�X
	/**
	 * @brief <b>�n���ꂽ�W�I���g���Ɋ֘A����t�B�[�`���������ς݃t�B�[�`���̒�����I������</b>\n
	 * ���̊֐��́A���ۂ̏����̂��߂̑O�����i���o�[�o���h�쐬�Ȃǁj�Ɏg�p���邱�Ƃ�O��ɍ쐬���Ă��邽�߁A
	 * �e�[�u���I�u�W�F�N�g�͑Ώۂɓ���܂���B\n
	 * �܂��A���̊֐��͓n���ꂽ�R���e�i�Ɍ��ʂ��i�[���邽�߁A�C�����C���ɂȂ��Ă��܂��B\n
	 * @param ipGeom		[in]	�t�B���^�W�I���g���iNULL�̏ꍇ�͑S�Ď擾�j
	 * @param emSpRel		[in]	�֘A�t�B���^�i���̏��g�p����܂���c�j
	 * @param cContainer	[out]	�֘A����t�B�[�`�����i�[�����
	 * @return sindyErrCode
	 */
	void QueryFeatures( IGeometry* ipGeom, esriSpatialRelEnum/* emSpRel*/, CContainer& cContainer ) const
	{
		bool bAll = ( ipGeom ) ? false : true;
		CGeometry cGeom( ipGeom );

		// ���ۂ̌`��ł͂Ȃ��A�L���Ȃ�L���̑傫���A������Ȃ當����̕\���̈�Ō�������K�v������B
		// �Ȃ̂ɁA���̊֐��ł̓X�P�[���̊T�O���������ߑS�ẴX�P�[���ɂ����ă`�F�b�N������𓾂Ȃ��B
		// ����Ȃ��ƂȂ�A�����Ƀo�E���f�B���O�{�b�N�X�Ōv�Z���悤�Ƃ����ɂ��̂܂܂̌`��ň����|���Ă�
		// �ς��Ȃ���ˁH�H
		// �Ƃ������ƂŁA���̂܂܂̌`��Ń`�F�b�N���邱�Ƃɂ���Bwritten by furukawa 2005/11/17
		for( const_iterator it = begin(); it != end(); ++it )
		{
			for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				// �^�ʒu
				if( bAll || ( ! cGeom.Disjoint( (IGeometry*)((*it2)->GetShape()) ) ) )
					cContainer.push_back( *it2 );

				for( CRelAnnotation::const_iterator itDisp = (*it2)->begin(); itDisp != (*it2)->end(); ++itDisp )
				{
					for( CRelAnnotation::const_rows_iterator itDisp2 = itDisp->second.begin(); itDisp2 != itDisp->second.end(); ++itDisp2 )
					{
						// �\���ʒu
						if( bAll || ( ! cGeom.Disjoint( (IGeometry*)((*itDisp2)->GetShape()) ) ) )
							cContainer.push_back( *itDisp2 );
					}
				}
			}
		}
	}

	/**
	 * @brief <b>�^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��</b>\n
	 * @param cGeom			[in]	�N�G���`��
	 * @param pairMove		[in]	�ړ�����
	 * @param emMode		[in]	�ړ����[�h�i���݂̂Ƃ��늮�S���s�ړ��̂݁j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( const CGeometry&/* cGeom*/, const std::pair<double,double>& pairMove, sindyeMoveMode/* emMode*/, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�^����ꂽ�`��Ƃ̌��������ŕ����������s��</b>\n
	 * @param cGeom			[in]	�N�G���`��
	 * @param cSplitTarget	[in]	�����Ώ�
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
//	errorcode::sindyErrCode Split( const CGeometry& cGeom, const CSplitTarget& cSplitTarget, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * �I���ς݂̒��L�Ɋւ��ă`�F�b�N���s���܂��B\n
	 * �t�B�[�`����ҏW������ɕK���`�F�b�N���s���Ă��������B
	 * ���̃��\�b�h�Ń`�F�b�N����鍀�ڂ͈ȉ��̂Ƃ���ł��B
	 * @li ���L������`�F�b�N
	 * @li ���L��ʃ`�F�b�N
	 * @li �\���^�C�v�`�F�b�N
	 * @li ���L������Ɛ^�ʒu�̈ʒu�֌W�`�F�b�N
	 * @li �\���ʒu���m�̏d�Ȃ�`�F�b�N
	 * @li �^�ʒu���m�̏d�Ȃ�`�F�b�N
	 * @li �^�ʒu�ƕ\���ʒu���m�̏d�Ȃ�`�F�b�N
	 * @param cRule			[in]	�_���`�F�b�N�p���[��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;
	//@}

	/**
	 * @brief <b>��ԃC���f�b�N�X���쐬����</b>\n
	 * �쐬���ꂽ�����E�`��L���b�V���A�y�уp�����[�^�L���b�V�������ɁA��ԃC���f�b�N�X
	 * ���쐬���܂��B�L���b�V�������݂���K�v������̂ŁA�K���L���b�V�����쐬���Ă���
	 * ���s���Ă��������B\n\n
	 * ��ԃC���f�b�N�X���쐬����O�ɁA�K�؂ȃC���f�b�N�X�쐬��`���X�V���܂��B\n
	 * cIndexRect�ɂ́A�K�؂ȋ�`��^���Ă��������B�ʏ�́ASelectByShpe�ɓn�����`���
	 * ��`���K�؂ł��B
	 * @param cIndexRect	[in]	��ԃC���f�b�N�X�쐬�p������`
	 * @param nGrid			[in]	�O���b�h�̒P�ʋ���
	 */
	void CAnnotations::CreateSpatialIndex( const WKSEnvelope& cIndexRect, double dGrid );

	/**
	 * @brief <b>��ԃC���f�b�N�X���쐬����</b>\n
	 * �쐬���ꂽ�����E�`��L���b�V���A�y�уp�����[�^�L���b�V�������ɁA��ԃC���f�b�N�X
	 * ���쐬���܂��B�L���b�V�������݂���K�v������̂ŁA�K���L���b�V�����쐬���Ă���
	 * ���s���Ă��������B\n\n
	 * ��ԃC���f�b�N�X���쐬����O�ɁA�K�؂ȃC���f�b�N�X�쐬��`���X�V���܂��B\n
	 * cIndexRect�ɂ́A�K�؂ȋ�`��^���Ă��������B�ʏ�́ASelectByShpe�ɓn�����`���
	 * ��`���K�؂ł��B
	 * @param cIndexRect	[in]	��ԃC���f�b�N�X�쐬�p������`
	 * @param nDivideX		[in]	��������
	 * @param nDivideY		[in]	�c������
	 */
	void CreateSpatialIndex( const WKSEnvelope& cIndexRect, int nDivideX, int nDivideY );

	//@{ @name ���L�X�P�[���C�e���[�^
	typedef DispIterator scale_iterator;
	typedef DispIterator scale_reverse_iterator;
	scale_iterator scale_begin() const { scale_iterator sc = DispIterator(GetTableType()); return sc; }
	scale_iterator scale_end() const { scale_iterator sc = DispIterator(sindyTableType::unknown); return sc; }
	scale_reverse_iterator scale_rbegin() const { scale_reverse_iterator sc = DispIterator(GetTableType(),true); return sc; }
	scale_reverse_iterator scale_rend() const { scale_reverse_iterator sc = DispIterator(sindyTableType::unknown,true); return sc; }
	//@}
private:
	CSpatialIndex m_cAnnoSpatialIndex[4];	//!< ���L�^�ʒu�p�[����ԃC���f�b�N�X
	CSpatialIndex m_cDispSpatialIndex[4];	//!< ���L�\���ʒu�p�[����ԃC���f�b�N�X
};

} // sindy

#endif // _ANNOTATIONS_H_
