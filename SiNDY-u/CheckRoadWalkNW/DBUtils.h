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

#include "stdafx.h"

#include "Properties.h"

typedef std::map< CString, long> FIELD_INDEX;
typedef std::map<long, CAdapt<IFeaturePtr>> MAP_FEATURE;
typedef std::map<long, CAdapt<_IRowPtr>> MAP_ROW;

class CDBUtils
{
public:
	/**
	 * @brief	�t�B�[���h�C���f�b�N�X�̍쐬
	 * @param	ipTable	[in]	�쐬���̃e�[�u��
	 * @param	fieldIndex	[out]	�쐬�����t�B�[���h�C���f�b�N�X
	 * @return	bool
	 */
	static bool createFieldIndex(const ITablePtr ipTable, FIELD_INDEX& fieldIndex);

	/**
	 * @brief	ObjectID���L�[�Ƃ����t�B�[�`���[�}�b�v�̐���
	 * @param	ipFeatureClass	[in]	�쐬���̃t�B�[�`���[�N���X
	 * @param	fieldIndex	[in]	�쐬���̃t�B�[���h�C���f�b�N�X
	 * @param	ipSpatialFilter	[in]	�쐬���̃t�B���^�[
	 * @param	features	[out]	�쐬�����}�b�v
	 * @return	bool
	 */
	static bool createFeatureMap(const IFeatureClassPtr ipFeatureClass, FIELD_INDEX fieldIndex, const ISpatialFilterPtr ipSpatialFilter, MAP_FEATURE& features);

	/**
	 * @brief	ObjectID���L�[�Ƃ����e�[�u��ROW�}�b�v�̐���
	 * @param	ipTable	[in]	�쐬���̃e�[�u��
	 * @param	fieldIndex	[in]	�쐬���̃t�B�[���h�C���f�b�N�X
	 * @param	ipQueryFilter	[in]	�쐬���̃t�B���^�[
	 * @param	rows	[out]	�쐬�����}�b�v
	 * @return	bool
	 */
	static bool createRowMap(const ITablePtr ipTable, FIELD_INDEX fieldIndex, const IQueryFilterPtr ipQueryFilter, MAP_ROW& rows);
};
