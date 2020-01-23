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
 * @file Features.h
 * @brief <b>CFeatures�N���X��`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @vertion $Id$
 */
#ifndef _FEATURES_H_
#define _FEATURES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FeatureContainer.h"

class CListString;

namespace sindy {

//class CFeatureContainer;
//class CLogicRule;
//class CErrorObjects;
//class CRow;
//class CGeometry;

/**
 * @class CFeatures
 * @brief <b>�t�B�[�`���E���R�[�h�̓����I�ȏ������s���N���X</b>\n
 * ���̃N���X�́A�S�Ăɂ����鏈�����s�����Ƃ�ړI�Ƃ����N���X�ł��B\n
 * ���̃N���X���g�p���邱�ƂŁA�S�Ă̏����̓��C�u�����ɔC���邱�Ƃ��ł��A
 * �N���C�A���g���L�q�����Ԃ�啝�ɏȂ����Ƃ��\�ł��B\n
 * �g�p���@�F\n
 * @li �N���X�������iInit�j
 * @li �t�B�[�`���I���iSelect...�j
 * @li �ҏW�����iMove�ADelete�A...�j
 * @li �_���`�F�b�N�iCheck�j
 */
class SINDYLIB_API CFeatures
{
public:
	explicit CFeatures()
	{
	}
	virtual ~CFeatures()
	{
		clear();
	}

	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * �t�B�[�`����ҏW������ɕK���`�F�b�N���s���Ă��������B
	 * @param cRule			[in]	�_���`�F�b�N�p���[��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>���ɕҏW����Ă��Ȃ����ǂ����m�F����</b>\n
	 * �ύX�����������̂ɑ΂��Ă̂݃`�F�b�N���s���܂��B\n
	 * �L���b�V�����쐬���Ă��Ȃ��ƕύX�̃`�F�b�N�Ɉ���������Ȃ��̂Œ��ӂ��Ă��������B\n
	 * @param cErrObjects	[in,out]	�G���[�I�u�W�F�N�g�����閔�́A���g���`�F�b�N����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>�t�B�[�`����I������</b>\n
	 * @param ipGeom		[in]			�I���`��
	 * @param listNames		[in]			�I�����郌�C���E�e�[�u����
	 * @param bRecycling	[in,optional]	true�ɂ���ƃL���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @param cErrs			[out,optional]	�G���[�i�[�R���e�i�i�I�u�W�F�N�g���m�̊֘A�t���Ɏ��s�������̂̂ݓ���j
	 *
	 * @return sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	errorcode::sindyErrCode SelectByShape( IWorkspace* ipWorkspace, IGeometry* ipGeom, const CListString& listNames, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* cErrs = NULL, bool bInit = true );

	/**
	 * @brief <b>�t�B�[�`����I������</b>\n
	 * @param ipGeom		[in]			�I���`��
	 * @param strName		[in]			�I�����郌�C���E�e�[�u����
	 * @param bRecycling	[in,optional]	true�ɂ���ƃL���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByShape( IWorkspace* ipWorkspace, IGeometry* ipGeom, LPCTSTR strName, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* cErrs = NULL, bool bInit = true );

	/**
	 * @brief <b>�t�B�[�`����I������</b>\n
	 * �n���ꂽ�t�B�[�`���̕ύX�O�E�ύX��̌`��Ō������܂��B
	 * �ύX�O�̌`��擾�ɂ�IFeatureChanges::get_OriginalShape���A�ύX��̌`��擾�ɂ�IFeature::get_Shape���g�p���܂��B
	 * @param ipGeom		[in]			�I���`��
	 * @param bRecycling	[in,optional]	true�ɂ���ƃL���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByObject( IWorkspace* ipWorkspace, _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false );

	/**
	 * @brief <b>�^����ꂽ�`��Ɋ֘A������̂��R���e�i�Ɋi�[����</b>\n
	 * @param ipGeom		[in]	�N�G���`��iNULL�̏ꍇ�͑S�Ď擾�j
	 * @param listFeatures	[out]	�i�[�R���e�i
	 */
	void QueryFeatures( IGeometry* ipGeom, CContainer& cContainer ) const;

	//@{ @name �ҏW�n�֐�
	/**
	 * @brief <b>�^����ꂽ�`��͈̔͂ňړ��������s��</b>\n
	 * �ړ����[�h�͎��̒ʂ�F
	 * @li <b>sindyeMove_Segment</b>\n
	 *		�\���_��ǉ������ɃN�G���`����𕽍s�ړ����郂�[�h�ł��B\n
	 *		�N�G���`��̋��E�ɂ���Z�O�����g�͐L�k�ΏۂƂȂ�܂��B\n
	 *		�N�G���`��̋��E�ɂ���\���_�́A�ړ��ΏۂƂ͂Ȃ�܂���B
	 * @li sindyeMove_Point
	 *		�N�G���`����̒��S�Ɉ�ԋ߂��\���_�݂̂��ړ����郂�[�h�ł��B\n
	 *		�W���̈ړ������Ɉ�ԋ߂��ł����A�N�G���`����ɍ\���_�����
	 *		���݂��Ȃ��ꍇ�A�\���_���쐬���ړ����܂��B
	 * @li sindyeMove_SegmentAddPointBorderIntersect
	 *		�N�G���`��̋��E�ɍ\���_����쐬���ĕ��s�ړ����郂�[�h�ł��B\n
	 *		�N�G���`��̋��E�ɂ���Z�O�����g�ɍ\���_����ǉ�����A���̍\���_
	 *		�y�уN�G���`����̍\���_���ړ����܂��B\n
	 *		�N�G���`��̋��E�ɂ���\���_���ړ��ΏۂƂȂ�܂��B
	 * @li sindyeMove_SegmentAddPointBorderWithin
	 *		�N�G���`��̋��E�ɍ\���_����쐬���ĕ��s�ړ����郂�[�h�ł��B\n
	 *		�N�G���`��̋��E�ɂ���Z�O�����g�ɍ\���_����ǉ����ꂽ��AsindyeMove_Segment
	 *		�Ɠ��l�̏������s���܂��B�N�G���`����݂̂��ړ��ΏۂƂȂ�_���AsindyeMove_SegmentAddPointBorderIntersect
	 *		�ƈقȂ�܂��B
	 * @li sindyeMove_SegmentAddPointsBorder
	 *		�N�G���`��̋��E�ɍ\���_���쐬���ĕ��s�ړ����郂�[�h�ł��B\n
	 *		�N�G���`��̋��E�ɍ\���_���쐬���A�Е��͂��̂܂܁A�����Е���
	 *		�ړ����܂��B�N�G���`��̋��E�ɂ���ق��̍\���_�͈ړ��ΏۂƂ͂Ȃ�܂���B
	 * @param cGeom			[in]	�N�G���`��
	 * @param pairMove		[in]	�ړ�����
	 * @param emMode		[in]	�ړ����[�h
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�_->�_�̈ړ��������s��</b>\n
	 * Move�̓_->�_�ړ�����łł��B
	 * �ړ����[�`���� sindyeMove_Point ���g�p���܂��B
	 * @param ipFrom		[in]	�ړ��O�̓_
	 * @param ipTo			[in]	�ړ���̓_
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode MovePoint( IPoint* ipFrom, IPoint* ipTo, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�^����ꂽ�`��Ƃ̌��������ŕ����������s��</b>\n
	 * @param pSource		[in]	�����Ώ�
	 * @param cGeom			[in]	�����`��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�^����ꂽ�t�B�[�`���̃}�[�W���s��</b>\n
	 * �w��t�B�[�`�����i�[����Ă��邱�ƁB
	 * @param pSource		[in]	�}�[�W��
	 * @param pDest			[in]	�}�[�W��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�_�̍폜���s��</b>\n
	 * @param pRow			[in]	�폜�Ώ�
	 * @param ipGeom		[in]	�폜�`��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @param bEraseOnly	[in/optional]	CRoadNetwork�̂ݎg�p�B
	 *                                      �������g�݂̂ɂ��邩�ǂ����ipRow������֌W�̎��ɂ����g��Ȃ��j
	 *                                      �폜�c�[���̃Z�O�����g�폜�Ŏn�_�E�I�_�ǂ��炩���܂܂�Ă��鎞�ɁA
	 *                                      OID��ێ����邽�߂ɍ\���_�\���_�폜������悤�ɂ��Ă���֌W��
	 *                                      pRow�Ŏw�肵�Ă���ȊO�̓���֌W�͏������Ȃ��悤�ɂ��邽�߂����Ɏg�p
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects, bool bEraseOnly=false );

	/**
	 * @brief <b>1�폜���s��</b>\n
	 * @param pRow			[in]	�폜�Ώ�
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Delete( CSPRow pRow, CErrorObjects& cErrObjects );

	/**
	 * @brief �N���[�����쐬����
	 *
	 * �����̃I�u�W�F�N�g������ێ����A�V�K�ɃI�u�W�F�N�g���쐬����
	 * �������R�s�[���܂��B���̍ہA�����̃I�u�W�F�N�g�͊J������܂��B
	 * @param ipWorkspace [in,optional] �N���[�����쐬���郏�[�N�X�y�[�X�i�f�t�H���g�FNULL�j
	 * @note ipWorkspace���w�肵���ꍇ�͓����e�[�u�������������ăN���[�����쐬���܂��B
	 * @warning ipWorkspace���w�肷��ꍇ�A�w�胏�[�N�X�y�[�X���ɂ͊����I�u�W�F�N�g����������
	 * �e�[�u�����Ɠ���̃e�[�u�������݂���K�v������܂��B�܂��ASDE�̏ꍇ��ipWorkspace�̐ڑ�
	 * ���[�U�����I�[�i�[���Ƃ��ăe�[�u�����������܂��B
	 */
	errorcode::sindyErrCode Clone( IWorkspace* ipWorkspace = NULL );

	/**
	 * @brief <b>�ҏW���e��ۑ�����</b>\n
	 * ���̊֐��ł͈ȉ��̏����������I�ɍs���܂��F
	 * @li ModifyCheck�ŕҏW���ꂽ�I�u�W�F�N�g��ΏۂɃ`�F�b�N
	 * @li �L���b�V��������΁A�L���b�V���ւ̕ύX���ʂ𔽉f
	 * @li Store
	 * @param cErrObjects	[out]			�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @retval sindyErr_NoErr				�G���[�Ȃ�
	 * @retval ��L�ȊO						�G���[
	 */
	errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );
	//@}

	/**
	 * @brief <b>�L���b�V������Ă��邩�ǂ������`�F�b�N����</b>\n
	 * @retval true		�S�ăL���b�V������Ă���
	 * @retval false	��ł��L���b�V������Ă��Ȃ����̂�����
	 */
	bool IsCached() const;

	//@{ @name �R���e�i����n�֐�
	/**
	 * @brief <b>�����o���N���A����</b>\n
	 */
	void clear();

	/**
	 * @brief <b>�R���e�i���󂩂ǂ������`�F�b�N����</b>\n
	 * @retval false	��ł͂Ȃ�
	 * @retval true		��ł���
	 */
	bool empty() const;

	/**
	 * @brief <b>�o�^����Ă���t�B�[�`��������΂��̃|�C���^��Ԃ�</b>\n
	 * ���̂Ƃ��둮���_�C�A���O�Ŏg�p��
	 * @return CRow*	�o�^����Ă���t�B�[�`���̃|�C���^
	 */
	CRow* FindObject( _IRow* ipRow );

	//@}

	//@{ @name �R���e�i����n�֐�
	typedef std::list<CFeatureContainer*>::iterator iterator;
	typedef std::list<CFeatureContainer*>::const_iterator const_iterator;
	iterator begin(){ return m_listFeatureContainer.begin(); }
	iterator end(){ return m_listFeatureContainer.end(); }
	const_iterator begin() const{ return m_listFeatureContainer.begin(); }
	const_iterator end() const{ return m_listFeatureContainer.end(); }
	//@}
	/**
	 * @brief <b>���݊i�[����Ă���t�B�[�`���̑��݋�`��Ԃ�<b>\n
	 * @param bOriginal              [in,optional] �ύX�O�̌`���`���~�����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bChanged               [in,optional] �ύX��̌`���`���~�����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bOriginalIsArcOriginal [in,optional] IFeatureChanges::get_OriginalShape�̌`���`���~�����ꍇ�ɂ�true�i�f�t�H���g�Ftrue�j
	 * @note ���̊֐��́A�Ⴆ�ΕҏW��̍ĕ`�������ꍇ�ɕύX�O�E�ύX��̌`����܂񂾋�`���~�����ꍇ�ȂǂɎg�p���܂��B
	 * @warning Store()���Ă��܂��ƃL���b�V���͑S�Ĕj������܂��̂ŁA�ύX�O�̌`�󂪎擾�ł��Ȃ��Ȃ�܂��B
	 * �ύX�O�̌`����擾����K�v������ꍇ��Store()�O�ɂ��̊֐����ĂԕK�v������܂��B
	 */
	IEnvelopePtr GetEnvelope( bool bOriginal = true, bool bChanged = true, bool bFeatureChangedOriginal = true ) const;

	/**
	 * @brief <b>���݊i�[����Ă���t�B�[�`���̌`����}�[�W�������̂�Ԃ�</b>\n
	 * @param bLatest                [in,optional] �ŐV�̌`�󂪗~�����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bOriginal              [in,optional] �ύX�O�̌`�󂪗~�����ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @param bOriginalIsArcOriginal [in,optional] IFeatureChanges::get_OriginalShape�̌`�󂪗~�����ꍇ�ɂ�true�i�f�t�H���g�Ffalse�j
	 * @param lParam                 [in,optional] ����̓X�P�[���l��z�肵�Ă��܂����A�ύX�����\��������܂��B�i�f�t�H���g�F-1�j
	 * @note ���̊֐��́A�ύX�O�E�ύX����܂ޑS�Ă̌`���Union�������̂��擾���邱�Ƃ��ł��܂��B
	 * �܂��APoint�APolyline�APolygon���������Ă���ꍇ��GeometryBag�ŕԂ��܂��B
	 * @note ���̊֐��́A�Ⴆ�ΕҏW��̘_���`�F�b�N������ꍇ�ɕύX��̏ꏊ�ɑ��݂���t�B�[�`�����������Ȃ���΂Ȃ�Ȃ��ꍇ�ȂǂɎg�p���܂��B
	 */
	IGeometryPtr GetShape( bool bLatest = true, bool bOriginal = false, bool bFeatureChangedOriginal = false, long lParam = -1 ) const;

	/**
	 * @brief �L���b�V�����쐬����
	 * @param bDetatch [in,optional] ���R�[�h�E�e�[�u����؂藣���ꍇ��true�i�f�t�H���g�Ffalse�j
	 * 
	 * �����E�`��̃L���b�V�����쐬���s���܂��B
	 * �L���b�V�����폜�������ꍇ��DeleteCache���Ăт܂��B
	 */
	errorcode::sindyErrCode CreateCache( bool bDetatch = false ) {
		for( iterator it = begin(); it != end(); ++it )
		{
			errorcode::sindyErrCode emErr = (*it)->CreateCache( bDetatch );
			if( errorcode::sindyErr_NoErr != emErr )
				return emErr;
		}
		return errorcode::sindyErr_NoErr;
	}

	/**
	 * @brief <b>�L���b�V�����폜����</b>
	 * @param bRecursive [in,optional] CRow�������Ă���R���e�i�̃L���b�V�����폜����ꍇ��true�i�f�t�H���g�Ffalse�j
	 */
	void DeleteCache( bool bRecursive = false )	{
		for( iterator it = begin(); it != end(); ++it )
			(*it)->DeleteCache( bRecursive );
	}

	/**
	 * @brief �L���b�V���ȊO�̃p�����[�^���X�V����
	 * @param bForce [in,optional] �����I�ɍ�蒼���ꍇ��true
	 * 
	 * �����E�`��ɕt������e��p�����[�^���X�V���܂��B
	 * �f�t�H���g�ł͉������삵�܂���B���L�̋L���E������
	 * ��`��A�����N��`��Ȃǂ�z�肵�Ă��܂��B
	 * @see CContainer::UpdateParameter
	 */
	errorcode::sindyErrCode UpdateParameter( bool bForce = false ) {
		for( iterator it = begin(); it != end(); ++it )
		{
			errorcode::sindyErrCode emErr = (*it)->UpdateParameter( bForce );
			if( errorcode::sindyErr_NoErr != emErr )
				return emErr;
		}
		return errorcode::sindyErr_NoErr;
	}
	void DeleteParameter() {
		for( iterator it = begin(); it != end(); ++it )
			(*it)->DeleteParameter();
	}

	void Trace( bool bAttribute = true, bool bGeometry = true ) const;
private:
	/**
	 * @brief <b>�������邽�߂̃R���e�i���Z�b�g����</b>\n
	 * ���L�ҏW�Ώۂ͋����I�ɓo�^����܂��B\n
	 * SelectByShape�֐�����g�p����܂��B\n
	 * �����Ń��X�g�ł̓����֐����g�p���Ă��܂��B
	 * @param ipWorkspace		[in]	�����Ώۃ��[�N�X�y�[�X
	 * @param lpcszName			[in]	�e�[�u�����i�I�[�i�[���t���ł��邱�Ƃ��]�܂����j
	 * @param listContainer		[out]	listNames�ɑΉ�����R���e�i���X�g
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetFeatureContainer( IWorkspace* ipWorkspace, LPCTSTR lpcszName, std::list<CFeatureContainer*>& listContainer );

	/**
	 * @brief <b>�������邽�߂̃R���e�i���Z�b�g����</b>\n
	 * ���L�ҏW�Ώۂ͋����I�ɓo�^����܂��B\n
	 * SelectByShape�֐�����g�p����܂��B
	 * @param ipWorkspace		[in]	�����Ώۃ��[�N�X�y�[�X
	 * @param listNames			[in]	�e�[�u�����i�I�[�i�[���t���ł��邱�Ƃ��]�܂����j
	 * @param listContainer		[out]	listNames�ɑΉ�����R���e�i���X�g
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetFeatureContainer( IWorkspace* ipWorkspace, const CListString& listNames, std::list<CFeatureContainer*>& listContainer );

	/**
	 * @brief <b>�U�������N���������`�F�b�N����</b>\n
	 * �t�B�[�`����ҏW������ɕK���`�F�b�N���s���Ă��������B
	 * [Bug 5846] [���s��NW]�U�������N�̒[�_�I���ŒZ�k���Ȃ��悤�ɂ��ė~����
	 * @param pContainer	[in]	�U�������N�R���e�i
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic_GuideLink( CFeatureContainer* pContainer, CErrorObjects& cErrObjects ) const;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list<CFeatureContainer*> m_listFeatureContainer;	//!< �e�t�B�[�`���N���X��O���[�v�i���H�n�Ƃ����L�n�Ƃ��j�ʂ̃R���e�i�̃��X�g
#pragma warning(pop)
};

} // namespace sindy

#endif // ifndef _FEATURES_H_
