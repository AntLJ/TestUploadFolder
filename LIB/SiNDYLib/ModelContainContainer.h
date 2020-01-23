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

#pragma once

#include "FeatureContainer.h"
#include "ErrorObject.h"

namespace sindy {

class SINDYLIB_API CModelContainContainer :
	public CFeatureContainer
{
public:
	CModelContainContainer(void);
	CModelContainContainer( sindyTableType::ECode emType ) : CFeatureContainer(emType) {}
	CModelContainContainer( IFeatureClass* lp ) : CFeatureContainer( lp ){}
	CModelContainContainer( ITable* lp ) : CFeatureContainer( lp ){}
	virtual ~CModelContainContainer(void);

	/**
	 * @brief <b>�`��Ō�������</b>\n
	 * �����Ńo�b�t�@�����O�Ȃǂ͂��Ȃ��̂ŁA�|�C���g�Ō�������ꍇ��
	 * �o�b�t�@�����O�����`���n���Ă��������B\n
	 * @param ipGeom		[in]			�����`��
	 * @param bRecycling	[in,optional]	true�ɂ���ƃL���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @param pErrs			[out,optional]	�G���[�i�[�R���e�i�i�I�u�W�F�N�g���m�̊֘A�t���Ɏ��s�������̂̂ݓ���j
	 *
	 * @return sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	virtual errorcode::sindyErrCode Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );

	/**
	 * @brief <b>�^����ꂽ�`��Ƃ̌��������ŕ����������s��</b>\n
	 * @param pSource		[in]	�����Ώ�
	 * @param cGeom			[in]	�N�G���`��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );
	errorcode::sindyErrCode _Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );

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
	 * @brief <b>�^����ꂽ�`��͈̔͂ɂ���_�̍폜���s��</b>\n
	 * @param pRow			[in]	�폜�Ώ�
	 * @param cGeom			[in]	�폜�`��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�t�B�[�`���̍폜���s��</b>\n
	 * �w��t�B�[�`�����i�[����Ă��邱�ƁB
	 * @param pRow			[in]	�폜�Ώ�
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Delete( CSPRow pRow, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�ҏW���e��ۑ�����</b>\n
	 * ���̊֐��ł͐������̃`�F�b�N���s���܂���B\n
	 * �K��Check�֐���ǂ񂾌�ɂ��̊֐����Ă�ł��������B\n
	 */
	errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );

	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * �t�B�[�`����ҏW������ɕK���`�F�b�N���s���Ă��������B
	 * @param cRule			[in]	�_���`�F�b�N�p���[��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>�R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��</b>\n
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;

};

} // sindy
