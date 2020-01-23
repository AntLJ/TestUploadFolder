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

namespace gf
{
	/**
	 * @brief	�h���C�����擾
	 * @param	ipRow		[in]	IRow
	 * @param	lFID		[in]	�t�B�[���h�h�c
	 * @param	lCode		[in]	�R�[�h�l
	 * @param	strDomain	[out]	�h���C��
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool getDomain( const _IRowPtr& ipRow, const LONG& lFID, const LONG& lCode, CString& strDomain );

	/**
	 * @brief	�ܓx�o�x�擾
	 * @param	ipGeo		[in]	�W�I���g��
	 * @param	point		[out]	�ܓx�o�x�i�[�p
	 * @param	bLabelPoint	[in]	�|���S���ɑ΂���ܓx�o�x���A�d�S�ł͂Ȃ����x���|�C���g�Ŏ擾���邩�ǂ���
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool getLonLat( const IGeometryPtr& ipGeo, WKSPoint& point, bool bLabelPoint = false );

	/**
	 * @brief	�\���_���擾
	 * @param	ipGeo	[in]	�W�I���g��
	 * @param	lCount	[out]	�\���_��
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool getPointCount( const IGeometryPtr& ipGeo, LONG& lCount );

	/**
	 * @brief	���`�I�u�W�F�N�g�H
	 * @param	ipGeo	[in]	�W�I���g��
	 ` @param	bResult	[out]	bool
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool isLinearObject( const IGeometryPtr& ipGeo, bool& bResult );

	/**
	 * @brief	�����擾
	 * @param	ipGeo	[in]	�W�I���g��
	 * @param	dLen	[out]	����(10�i�x�P��)
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool getLength( const IGeometryPtr& ipGeo, double& dLen );

	/**
	 * @brief	�ʐώ擾
	 * @param	ipGeo	[in]	�W�I���g��
	 * @param	dArea	[out]	�ʐ�(10�i�x�P��)
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool getArea( const IGeometryPtr& ipGeo, double& dArea );
}
