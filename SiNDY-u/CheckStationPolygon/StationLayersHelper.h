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

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

namespace station_layer_helper
{
	/// ���C�����ƃ��C���ԍ��̑Ή��\(���C���ԍ��̌����p)
	extern std::map<CString, long> LAYER_NOS;

	/// ���C���ԍ��ƃ��C�����̑Ή��\(���C�����̌����p)
	extern std::map<long, CString> STATION_LAYERS;

	/**
	 * @brief ���C�����w�肵�ă��C���ԍ����擾����
	 * @param layer_name [in] ���C����
	 * @return ���C���ԍ�
	 */
	inline long getLayerNo( const CString& layer_name ) { return LAYER_NOS[layer_name]; }

	/**
	 * @brief ���C���ԍ��w�肵�ă��C�������擾����
	 * @param layer_no [in] ���C���ԍ�
	 * @return ���C����
	 */
	inline CString getLayerName( long layer_no ){ return STATION_LAYERS[layer_no]; }

	/**
	 * @brief ���C�����ƃ��C���ԍ��̑Ή��\��Ԃ�
	 * @return ���C�����ƃ��C���ԍ��̑Ή��\
	 */
	inline std::map<CString, long> getStationLayerInfos(){  return LAYER_NOS; }

	/**
	 * @brief �w�|���S���̃��C����
	 * @param layer_name [in] ���C����
	 * @retval true  �w�|���S�����C���ł���
	 * @retval false �w�|���S�����C���ł͂Ȃ�
	 */
	inline bool isStationPolygonLayer( const CString& layer_name ){ return LAYER_NOS.count( layer_name ) != 0; }

	/**
	* @brief  �w�肵���w�|���S�����C���݂̂���������Ă��邩�`�F�b�N���� (bug12350)
	* @note   �t���O�̈Ӗ��ɂ��Ă� "define_enum.h"���Q��
	* @param  row         [in] �Ώۃt�B�[�`��
	* @param  table       [in] �w�|���S���R�t���e�[�u��
	* @param  okLayerBits [in] ��������Ă��ėǂ����C����\��bitset
	* @retval true  ���Ȃ�
	* @retval false ��肠��
	*/
	bool checkRelationalLayer(const sindy::CRowContainer& row, sindy::CTableContainer& table, std::bitset<6> okLayerBits );

	/**
	* @brief  �Ώۃt�B�[�`���̉wID���擾
	* @param  row   [in] �Ώۃt�B�[�`��
	* @param  table [in] �w�|���S���R�t���e�[�u��
	* @return �wID
	* @retval -1 ID�擾���s
	*/
	inline long getStationId(const sindy::CRowContainer& row, sindy::CTableContainer& table);
}
