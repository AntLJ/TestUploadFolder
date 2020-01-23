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
#include "HighZoomStationCheckHandler.h"
#include "ErrDef.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// sindy
#include <schema.h>

using namespace sindy;
using namespace sindy::schema::sj;


// ������
bool HighZoomStationCheckHandler::init( const CWorkspace& workspace, SPStationChecker checker  )
{
	m_Checker = checker;
	m_TableFinder.InitCollection( workspace.OpenSameOwnersTable( highzoom_station::kTableName ) , CModel() );

	return true;
}


// �`�F�b�N���s
bool HighZoomStationCheckHandler::check()
{
	// �e�[�u���R���e�i�擾
	auto& table = *m_TableFinder.FindTable( highzoom_station::kTableName );
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
		if( ! m_Checker->checkLinkedStation( rowc, highzoom_station::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrNotLinkedStation) );

		// [ERR_CODE 104]: ���g������"�w�ȈՃ|���S��"�����݂��邩
		if( ! m_Checker->checkContain( rowc, midzoom_station::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrOutOfMidZoomStation) );

		// [ERR_CODE 108]: ���g������"�w�ȈՃ|���S��"�����݂��A���wID����v���Ă��邩
		if( ! m_Checker->checkContainedBySameStationPolygon( rowc, midzoom_station::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrOutOfSameIdMidZoomStation) );
	}

	return true;
}
