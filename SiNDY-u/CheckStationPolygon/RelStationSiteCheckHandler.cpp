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
#include "RelStationSiteCheckHandler.h"
#include "StationLayersHelper.h"
#include "ErrDef.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// sindy
#include <schema.h>

using namespace sindy;
using namespace sindy::schema::sj;


// ������
bool RelStationSiteCheckHandler::init( const CWorkspace& workspace, SPStationChecker checker  )
{
	m_Checker = checker;
	m_TableFinder.InitCollection( workspace.OpenSameOwnersTable( rel_station_site::kTableName ) , CModel() );

	return true;
}


// �`�F�b�N���s
bool RelStationSiteCheckHandler::check()
{
	using namespace station_layer_helper;

	// �e�[�u���R���e�i�擾
	auto& table = *m_TableFinder.FindTable( rel_station_site::kTableName );
	table.clear();

	// �S���R�[�h�擾
	table._Select( IQueryFilterPtr(), true );
	table.CreateCache();

	// 1���R�[�h���`�F�b�N
	using namespace err_def;
	for( const auto& row : table )
	{
		const auto& rowc = *CAST_ROWC( row );

		// [ERR_CODE 105]: ���݂��Ȃ��w�|���S���ւ̕R�t����ێ��������R�[�h�����݂��Ȃ���
		if( ! m_Checker->checkLinkedNotExistPolygon( rowc ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrLinkedNotExistPolygon) );

		// [ERR_CODE 106]: ���݂��Ȃ��w���ւ̕R�t����ێ��������R�[�h�����݂��Ȃ���
		if( ! m_Checker->checkLinkedNotExistStation( rowc ) )
		{
			// ���O�o�͗p�ɉw�|���S���̃��C�����AOID���擾
			long layerNo  = rowc.GetValue( rel_station_site::kLayerNo_C, 0L );
			uh::tstring tableName = getLayerName( layerNo );
			long layerOid = rowc.GetValue( rel_station_site::kLayerOID, 0L );

			// ���O�o��
			ERRLOGGER.Error( tableName, layerOid, err(kErrLinkedNotExistStation) );
		}
	}

	return true;
}
