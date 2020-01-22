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
#include "BldNameAddrAnalyzer.h"
#include "AddrAnalyzer.h"
#include "ConditionMgr.h"
#include "globalfunc.h"
#include "logger.h"
#include "EditOperationMgr.h"

#include <WinLib/ADOBase.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_core/ErrorCode.h>
#include <TDC/sindylib_base/ErrorObject2.h>
#include <TDC/sindylib_base/GlobalRules.h>

using namespace sindy;
using namespace sindy::schema;
using namespace sindy::errorcode;

// ������
void BldNameAddrAnalyzer::init()
{
	auto tbl = TryOpenTable( 
		m_args.getValue(option::kTargetDb),
		m_args.getValue(option::kTargetTable) );

	m_finder.InitCollection( tbl, CModel() );

	ERRLOGGER.Initialize( m_args.getValue( option::kErrLog ).GetString() );
}

// �񃁃��o�֐��O���錾
void store( CTableContainer& table );
// �X�V�Ώۃe�[�u���`�F�b�N
void checkTable( const CTableContainer& table );

// ���s
void BldNameAddrAnalyzer::execute()
{
	init();
	
	// �f�ރe�[�u���̃L���b�V���ݒ肵�Ă���
	auto bldgSrcT = *m_finder.FindTable( m_args.getValue(option::kTargetTable) );
	bldgSrcT.SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );

	// �Ώۏ����̃��X�g��ǂݍ���
	ConditionMgr conditions;
	conditions.init( m_args.getValue( option::kTargetList ) );

	// �Z���T�[�o����
	AddrAnalyzer analyzer;
	analyzer.init( m_args.getValue( option::kAddrDB ),
		           m_args.getValue( option::kXyUser ),
				   m_args.getValue( option::kReleaseUser ),
				   m_args.getValue( option::kBuildingDB ) );

	RUNLOGGER.Info( _T("�Z����͊J�n...\n"), true );
	for( const auto& condition : conditions )
	{
		// �������ƂɌ������ď���
		CString query = condition.getQuery( bldgSrcT.GetWorkspace() );
		auto filter = AheInitQueryFilter( nullptr, nullptr, _T("%s"), query );
		RUNLOGGER.Log( uh::str_util::format( _T(" �ΏہF%s (%d��)\n"), query, bldgSrcT.RowCount(filter)), true );

		bldgSrcT._Select( filter, false );
		bldgSrcT.CreateCache();
		long failed(0), update(0);
		for( auto& row : bldgSrcT )
		{
			auto& bldgSrcRow = *CAST_ROWC( row );
			if( !analyzer.updateRow( bldgSrcRow ) )
			{
				ERRLOGGER.WriteLog( bldgSrcRow, _T("�Z����͂Ɏ��s���܂����B") );
				bldgSrcRow.Reset();
				++failed;
				continue;
			}

			if( bldgSrcRow.Changed() )
				++update;
		}
		RUNLOGGER.Log( uh::str_util::format( _T(" �X�V�F%d��\n"), update ), true );
		RUNLOGGER.Log( uh::str_util::format( _T(" ��͎��s�F%d��\n\n"), failed ), true );

		store( bldgSrcT );
		bldgSrcT.clear();
	}
	RUNLOGGER.Info( _T("�Z����͊���\n"), true );
}


void store( CTableContainer& table )
{
	// BUILDINGNAME_SOURCE_POINT ��SiNDY-c�Ƃ͖��֌W��UPDATE_C�t�B�[���h��
	// �����Ă��邽�߁Asindylib_base�ŏ���ɍX�V�����̂�h���B
	// ipc�e�[�u���̃t�B�[���h�͌p�����Ă��Ȃ��̂ŁA����Ŗ��͂Ȃ��͂������A�����̂��A����ŁA�A�A
	g_bSetHistory = false;

	// �ҏW�J�n
	EditOperationMgr ope( table.GetWorkspace() );
	ope.Start();

	// �ۑ�
	RUNLOGGER.Info( _T("�ۑ�... "), true );
	CErrorObjectsBase errs;
	if( sindyErr_NoErr != table.Store( errs ) )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("�ۑ����s: %s"), errs.ToString() ) );
	}
	RUNLOGGER.Info( _T("OK.\n"), true );

	ope.Stop();
}

void checkTable( const CTableContainer& table )
{
	// BUILDINGNAME_SRC_POINT�Ɠ����X�L�[�}�ł��邱�Ƃ��O��B
	// ���Ȃ��Ƃ��A�X�V�Ώۂ̃t�B�[���h�͑��݂���K�v������B
	using namespace sindy::schema::sj::buildingname_src_point;

	std::vector<CString> fields = boost::assign::list_of
		( kAddrCode )          // ADDRCODE
		( kAddrLon )           // ADDR_LON
		( kAddrLat )           // ADDR_LAT
		( kAddrVer )           // ADDR_VER
		( kRepType )           // REPTYPE_C
		( kAddrLevel )         // ADDRLEVEL_C
		( kPinPoint )          // PINPOINT_F
		( kSameAddrCount )     // SAMEADDR_COUNT
		( kSameAddr )          // SAMEADDR_F
		( kMultiSameBldg );    // MULTISAMEBLDG_F

	const auto& fm = *table.GetFieldMap();
	for( const CString& field : fields )
	{
		if( 0 > fm.HasField( field ) )
		{
			throw std::runtime_error( uh::str_util::formatA( 
				_T("�t�B�[���h�����݂��܂���B: %s.%s"), table.GetTableName(), field) );
		}
	}
}
