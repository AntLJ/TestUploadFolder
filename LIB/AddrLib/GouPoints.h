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

#include "CityAdmin.h"
#include "GouPoint.h"
#include <SiNDYLib/FeatureContainer.h>

namespace addr
{

using namespace sindy;

/**
 * @class	CGouPoints
 * @brief	�w��s���E�ȉ��ɂ��鍆�|�C���g���X�g
 *			���|�C���g�Ɖƌ`�̑Ή��������Ŋ֘A�t�����Ă���
 */
class CGouPoints : public CFeatureContainer
{
public:

	explicit CGouPoints()
	{
		m_emTableType = sindyTableType::city_admin;
	}
	virtual ~CGouPoints(){ clear(); }

	explicit CGouPoints( ITable* ipAdminTable )
	{
		SetObject( sindyTableType::city_admin, ipAdminTable );
	}

	explicit CGouPoints( IFeatureClass* ipAdminFC )
	{
		SetObject( sindyTableType::city_admin, ipAdminFC );
	}

	explicit CGouPoints( const CFeatureClass& cAdminFC )
	{
		SetObject( cAdminFC.GetTableType(), (ITable*)cAdminFC );
	}

	/**
	 * @brief	<b>�s���E�ȉ��ɂ��鍆�|�C���g���擾(�Z���R�[�h�w��Ver)</b>
	 * @param	lpcszAddrCode	[in]			�Z���R�[�h(5,8,11,16���̎w�肪��)
	 * @return	sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	errorcode::sindyErrCode SelectByAddrCode( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	<b>�s���E�ȉ��ɂ��鍆�|�C���g���擾(Where��w��Ver)</b>
	 * @param	lpcszWhere	[in]	Where��
	 * @return	sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	errorcode::sindyErrCode SelectByWhereClause( const _TCHAR* lpcszWhere );

	/**
	 * @brief	�����Ώۍ��|�C���g�e�[�u���Z�b�g
	 * @note	�s���E�ƈႤ���[�N�X�y�[�X�̍��������ΏۂƂ������Ƃ��ɖ����I�Ɏw�肵�ĉ������B
	 * @param	ipTable	[in]	���|�C���g�e�[�u��
	 */
	void SetGouPointTable( ITablePtr ipTable ){ m_cGouFC.SetObject(ipTable); }

private:

	/**
	 * @brief	<b>�N�G���쐬</b>
	 * @param	lpcszAddrCode	[in]	�Z���R�[�h(5,8,11���̎w�肪��)
	 * @return	�N�G��������
	 */
	CString GetQueryStr( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief <b>�s���E������</b>\n
	 * CFeatureContainer::Select �g�p����ƁACCityAdmin�̃C���X�^���X�𐶐����邽�߂�
	 * SiNDYLib/RowClassFactory�ɏC����������K�v������̂œƎ��ɒ�`
	 * @param ipFilter		[in]			�t�B���^
	 * @param bRecycling	[in,optional]	true�ɂ���ƃL���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @param pErrs			[out,optional]	�G���[�i�[�R���e�i�i�I�u�W�F�N�g���m�̊֘A�t���Ɏ��s�������̂̂ݓ���j
	 * @return sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	errorcode::sindyErrCode Select( IQueryFilter* ipFilter, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );

private:

	CFeatureClass	m_cGouFC;
};

}	// addr
