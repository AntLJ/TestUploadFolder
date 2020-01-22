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
 * @file	LayerCheckInclude.h
 * @brief	�e���C���w�b�_�[�t�@�C���C���N���[�h����t�@�C��
 * �����Ɋe���C���̃w�b�_�[�t�@�C�����C���N���[�h����
 */

#pragma once

#include ".\AddrPolygonCheck\AddrPolygonCheck.h"
#include ".\CityPolygonCheck\CityPolygonCheck.h"
/**
 * @brief	�`�F�b�N���C���C���X�^���X�擾�֐�
 * @param	[in] layer			���C����
 * @return	�`�F�b�N���C��
 */
static shared_ptr<LayerCheckBase> setLayerCheckInstance( const CString& layer )
{
	try
	{
		// �R���|�[�l���g�ǉ����ꂽ��ȉ��ɂ���񂶂��ǉ�
		if( addr_polygon::kTableName == layer )         return shared_ptr<LayerCheckBase>( new AddrPolygonCheck() );
		if( city_polygon::kTableName == layer )         return shared_ptr<LayerCheckBase>( new CityPolygonCheck() );
	}
	catch( bad_alloc )
	{
		return nullptr;	// new �����s�����Ƃ��A�����Ȃ��悤��
	}

	return nullptr;
}
