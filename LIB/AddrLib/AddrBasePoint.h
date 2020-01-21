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

#include <SiNDYLib/Feature.h>
#include <SiNDYLib/FeatureClass.h>
#include "CityAdmin.h"

namespace addr
{
using namespace sindy;
using namespace sindy::errorcode;
using namespace sindy::schema;

/**
 * @class	CAddrBasePoint
 * @brief	�e��Z���|�C���g�N���X�̐e�N���X(����)
 *          �����ŋ��L�����Ȃǂ��܂Ƃ߂Ă���
 */
class CAddrBasePoint : public CFeature
{
public:
	CAddrBasePoint(){}
	virtual ~CAddrBasePoint(){ DeleteCache(); }

	/**
	 * @brief	<b>�Z���R�[�h�擾</b>
 	 * @param	bChar			[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
	 * @param	bCodeDBSaved	[in, optional]	�����t���Z���R�[�h�ϊ�DB�ɑ��݂��Ȗ��̂̂Ƃ��ۑ�������ŃR�[�h��V�K���s����H(true�ɂ���ƁA�R�[�h�ϊ�DB�ɍX�V����������܂�)
	 * @return	�Z���R�[�h
	 */
	virtual CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false ) = 0;

	/**
	 * @brief	<b>�Z�����̎擾</b>
 	 * @param	bChar			[in, optional]	�����t���Z���l������H(true�ɂ���ƁA�����t���Z���l���������ʂ��Ԃ�܂�)
 	 * @note	���g���������ꍇ�́A�p����Ŏ������Ă��������B
	 *          ��Initializer.h �� Init_AreaCodeHelper�֐����Ăяo���Ă���g�p���邱�ƁI
	 * @return	�Z������
	 */
	virtual CString GetAddrName( bool bChar = false );

	/**
	 * @brief	<b>�|�C���g��������s���E���擾</b>
	 * @note	NULL���́A���|�C���g�Ɠ��ꃏ�[�N�X�y�[�X���猟��
	 * @return	�s�s�n�}�s���E
	 */
	const CCityAdmin* GetCityAdmin();

	/**
	 * @brief	<b>�|�C���g��������s���E���擾</b>
 	 * @param	cCityAdminClass	[in]	�s�s�s���E�e�[�u��
	 * @return	�s�s�n�}�s���E
	 */
	const CCityAdmin* GetCityAdmin( const CFeatureClass& cCityAdminClass );

	/**
	 * @brief	<b>�|�C���g��������s���E���Z�b�g</b>
	 * @param	cAdminTable	[in]	�s���E�e�[�u��
	 * @retval	true  : �擾����
	 * @retval	false : �擾�ł��Ȃ�
	 */
	bool SetCityAdmin( const CFeatureClass& cAdminTable );

	/**
	 * @brief	<b>�|�C���g��������s���E���Z�b�g(�s���E���ڎw��Ver)</b>
	 * @param	cAdmin	[in]	�s���E
	 */
	void SetCityAdmin( const CSPCityAdmin& cAdmin );

	/**
	 * @brief <b>�L���b�V�����폜����</b>\n
	 * CRow::DeleteCache���I�[�o���[�h
	 */
	virtual void DeleteCache( bool bDeleteContainerCache = false )
	{
		CRow::DeleteCache( bDeleteContainerCache );
		m_strAddrName = _T("");
	}

protected:

	mutable CString m_strAddrName;	//!< �Z����(�L���b�V���p)

};

}	// namespace addr
