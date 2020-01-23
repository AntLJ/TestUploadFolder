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

#include "StationLayersHelper.h"

// boost
#include <boost/assign.hpp>

// WinLib
#include <type_convert.h>

// ArcHelperEx
#include <AheLayerFunctions.h>
#include <GlobalFunctions.h>

// useful_header
#include <TDC/useful_headers/str_util.h>

using namespace sindy;
using namespace sindy::schema::sj;

namespace station_layer_helper
{
	// ���C�����ł̃��C���ԍ������p
	std::map<CString, long> LAYER_NOS = boost::assign::map_list_of
							( station_detail::kTableName,   rel_station_site::layerno_class::kStationDetail )
							( midzoom_platform::kTableName, rel_station_site::layerno_class::kMidzoomPlatform )
							( highzoom_station::kTableName, rel_station_site::layerno_class::kHighzoomStation )
							( midzoom_station::kTableName,  rel_station_site::layerno_class::kMidzoomStation )
							( lowzoom_platform::kTableName, rel_station_site::layerno_class::kLowzoomPlatform )
							;

	// ���C���ԍ��ł̃��C���������p
	std::map<long, CString> STATION_LAYERS = boost::assign::map_list_of
							( rel_station_site::layerno_class::kStationDetail,   station_detail::kTableName )
							( rel_station_site::layerno_class::kMidzoomPlatform, midzoom_platform::kTableName )
							( rel_station_site::layerno_class::kHighzoomStation, highzoom_station::kTableName )
							( rel_station_site::layerno_class::kMidzoomStation,  midzoom_station::kTableName )
							( rel_station_site::layerno_class::kLowzoomPlatform, lowzoom_platform::kTableName )
							;

	// �w�肵���w�|���S�����C���݂̂���������Ă��邩�`�F�b�N���� (bug12350)
	bool checkRelationalLayer(const sindy::CRowContainer& row, sindy::CTableContainer& table, std::bitset<6> okLayerBits )
	{
		// �`�F�b�N�Ώۂ̉wID���擾
		long stationId = getStationId(row, table);
		// �w�ɕR�t���Ă��Ȃ�
		if (stationId == -1)
			return false;

		// �w�|���S���R�t���e�[�u������`�F�b�N�Ώۂ̉wID�Ɉ�v���郌�R�[�h������
		CString where = uh::str_util::format(_T("%s=%d"), rel_station_site::kGroupID, stationId);
		
		table.clear();
		table._Select(AheInitQueryFilter(nullptr, nullptr, where), true);
		table.CreateCache();

		// �e�[�u���ɕR�t���Ă��Ȃ�
		if (table.empty())
			return false;

		// �����������R�[�h�̃��C���ԍ��Q���擾
		std::set<long> layerNumbers;
		for (const auto& target_row : table) {
			long layerNo = CAST_ROWC(target_row)->GetValue(rel_station_site::kLayerNo_C, -1L);
			if (layerNo != -1L)
				layerNumbers.insert(layerNo);
		}

		// �����������R�[�h�̃��C����ʂ�bitset�Ɋi�[
		std::bitset<6> layerBits;
		for (const auto& layerNo : layerNumbers) {
			layerBits.set(layerNo);
		}

		return okLayerBits == layerBits ? true : false;
	}

	// �Ώۃt�B�[�`���̉wID���擾
	long getStationId(const sindy::CRowContainer& row, sindy::CTableContainer& table)
	{
		using namespace sindy::schema::sj;
		using namespace station_layer_helper;

		// ���Y�t�B�[�`����OID�ƃ��C��ID���擾
		long layerNo = getLayerNo(row.GetTableName());
		long layerOid = row.GetOID();

		// �w�|���S���R�t���e�[�u��������
		CString where = uh::str_util::format(_T("%s=%d and %s=%d"),
			rel_station_site::kLayerNo_C, layerNo,
			rel_station_site::kLayerOID, layerOid);

		table.clear();
		table._Select(AheInitQueryFilter(nullptr, nullptr, where), true);
		table.CreateCache();
		if (table.empty())
			return -1;

		// �O���[�vID ���擾
		const auto& rowc = CAST_ROWC(*table.begin());
		long stationId = rowc->GetValue(rel_station_site::kGroupID, 0L);

		// �������ʃN���A
		table.clear();

		return stationId;
	}
}
