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

#include "Logger.h"
#include "MidZoomPlatformCheckHandler.h"
#include "ErrDef.h"
#include "StationLayersHelper.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// sindy
#include <schema.h>

using namespace sindy;
using namespace sindy::schema::sj;


// ������
bool MidZoomPlatformCheckHandler::init( const CWorkspace& workspace, SPStationChecker checker  )
{
	m_Checker = checker;
	m_TableFinder.InitCollection( workspace.OpenSameOwnersTable( midzoom_platform::kTableName ) , CModel() );
	m_TableFinder.InitCollection( workspace.OpenSameOwnersTable( rel_station_site::kTableName ) , CModel() );

	return true;
}


// �`�F�b�N���s
bool MidZoomPlatformCheckHandler::check()
{
	// �e�[�u���R���e�i�擾
	auto& table = *m_TableFinder.FindTable( midzoom_platform::kTableName );
	table.clear();

	// �S���R�[�h�擾
	table._Select( IQueryFilterPtr(), true );
	table.CreateCache();

	// 1���R�[�h���`�F�b�N
	using namespace err_def;
	for( const auto& row : table )
	{
		const auto& rowc = *CAST_ROWC( row );

		// [ERR_CODE 101]: 1�ȏ�̉w��񂪕R�t�����Ă��邩
		if( ! m_Checker->checkLinkedStation( rowc, midzoom_platform::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrNotLinkedStation) );

		// [ERR_CODE 103]: ���g������"�v���b�g�t�H�[���S�̃|���S��"�����݂��邩
		if( ! m_Checker->checkContain( rowc, lowzoom_platform::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrOutOfLowPlatformShape) );

		// [ERR_CODE 107]: ���g������"�v���b�g�t�H�[���S�̃|���S��"�����݂��A���wID����v���Ă��邩
		if( ! m_Checker->checkContainedBySameStationPolygon( rowc, lowzoom_platform::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrOutOfSameIdLowPlatformShape) );

		// ���C��2,5��������������Ă���w�́AERR_CODE 202,204 �̃`�F�b�N���s��Ȃ�
		std::bitset<6> checkLayerBits;
		checkLayerBits.set(rel_station_site::layerno_class::kMidzoomPlatform);
		checkLayerBits.set(rel_station_site::layerno_class::kLowzoomPlatform);

		using namespace station_layer_helper;
		if( !checkRelationalLayer(rowc, *m_TableFinder.FindTable(rel_station_site::kTableName), checkLayerBits) )
		{
			// [ERR_CODE 202]: ���g������"�w�ȈՃ|���S��"�����݂��邩
			if (!m_Checker->checkContain(rowc, midzoom_station::kTableName))
				ERRLOGGER.Warn(rowc.GetTableName(), rowc.GetOID(), err(kWarnOutOfMidZoomStation));

			// [ERR_CODE 204]: ���g������"�w�ȈՃ|���S��"�����݂��A���wID����v���Ă��邩
			if (!m_Checker->checkContainedBySameStationPolygon(rowc, midzoom_station::kTableName))
				ERRLOGGER.Warn(rowc.GetTableName(), rowc.GetOID(), err(kWarnOutOfSameIdMidZoomStation));
		}
	}

	return true;
}
