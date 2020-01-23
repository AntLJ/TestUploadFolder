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
#include <SiNDYLib/FieldMap.h>

namespace Util
{
	/**
	 * @brief   �W�I���g������w�苗�������o�b�t�@�����N�����`����쐬����
	 * @param[in] geo    �o�b�t�@�����O���̃W�I���g��
	 * @param[in] meter  �o�b�t�@�����O����(���[�g��)
	 * @return  �o�b�t�@�����N�����`��
	 */
	IGeometryPtr getBuffer( const IGeometryPtr& geo, double meter );

	/**
	 * @brief   ��r���Ɣ�r��̍ŒZ�������擾����
	 * @param[in]	ipBase			��r��
	 * @param[in]	ipComp			��r��
	 * @param[out]	distance		�ŒZ����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getNearestDistance( const IFeaturePtr& ipBase, const IFeaturePtr& ipComp, double& nearestDistance );

	/**
	 * @brief	�����N�̒��_��XY���擾����
	 * @param[in]	ipFeature	�����N
	 * @param[out]	x			�o�x
	 * @param[out]	y			�ܓx
	 * @param[in]	ipSpRef		��ԎQ��
	 */
	void getXY( const IFeaturePtr& ipFeature, double& x, double& y, const ISpatialReferencePtr& ipSpRef = nullptr );

	/**
	 * @brief	GID�����t�B�[�`���[�Q��GID�ƕR�t���ĕԂ�
	 * @param[in]	features	�t�B�[�`���[�Q
	 * @param[in]	fieldMap	�t�B�[���h�}�b�v
	 * @param[out]	ret			GID�ƕR�t�����t�B�[�`���[�Q
	 */
	void getGid2Feature( const std::vector<IFeaturePtr>& features, const sindy::CFieldMap& fieldMap, std::map<CString, IFeaturePtr>& ret );
}