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
#include "CSV2DB.h"
#include "logger.h"
#include "CsvLoader.h"
#include "globalfunc.h"
#include "EditOperationMgr.h"
#include <WinLib/ADOBase.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/GeometryBase.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_core/ErrorCode.h>
#include <TDC/sindylib_base/ErrorObject2.h>
#include <TDC/sindylib_base/GlobalRules.h>


using namespace sindy;
using namespace sindy::schema;
using namespace sindy::errorcode;


// execute()�Ŏg���񃁃��o�֐��̃v���g�^�C�v�錾
void createRows( CTableContainer& destT, CsvLoader& csv, bool testMode );
void updateRows( CTableContainer& destT, CsvLoader& csv, bool testMode );

// ���s
void CCSV2DB::execute()
{
	// �q�o�[�W�������O�̃e�[�u��
	auto parentT = GetDestTable();

	// ����CSV�ǂݍ���
	CsvLoader csv;
	csv.Load( m_args.getValue( arg::kInput ), m_args.getValue( arg::kForceOutput ), *parentT );

	// �q�o�[�W����������ĕҏW�J�n
	CSPTableContainer spDest;
	EditOperationMgr editOpe;
	bool testMode = m_args.hasOption( arg::kTestMode );
	if( testMode )
	{
		spDest = parentT;
	}
	else
	{
		auto workVer = TryCreateVersion( *parentT );
		editOpe.Init( workVer );
		editOpe.Start();
		spDest = GetDestTable( workVer );
	}

	// SiNDY�����͘M��Ȃ��I�v�V����
	if( m_args.hasOption( arg::kNoUpdateHistry ) )
		g_bSetHistory = false;
	
	// �V�K���R�[�h�ǉ�
	createRows( *spDest, csv, testMode );

	// �������R�[�h�X�V
	updateRows( *spDest, csv, testMode );

	// �ҏW�I��
	if( editOpe.IsBeingEdited() )
	{
		editOpe.Stop();
	}

	// ��������
	RUNLOGGER.WriteResult( csv );
}

// �Ώۃe�[�u�����擾
sindy::CSPTableContainer CCSV2DB::GetDestTable( const IWorkspacePtr work /*= nullptr*/ )
{
	// �t�@�C���_�����蒼��
	m_finder.ClearCollection();

	// ���[�N�X�y�[�X�w�肳��Ȃ���Έ�����DB�ɐڑ�
	IWorkspacePtr destWork = work ? work : TryConnect( m_args.getValue(arg::kTargetDB) );

	m_finder.InitCollection( 
		TryOpenTable( destWork, m_args.getValue(arg::kTargetTbl) ),
		CModel() );

	return m_finder.FindTable( m_args.getValue(arg::kTargetTbl) );
}


//////////////////////////
// �ȉ��͔񃁃��o�֐��̎���
//////////////////////////

// csv�̃��R�[�h����DB�̃��R�[�h�Ƀt�B�[���h�l���R�s�[����
void setAttribute( CRowContainer& row, RecordInfo& ri )
{
	// ����
	for( auto& fldVal : ri )
	{
		FieldInfo& fi = fldVal.first;
		const CComVariant& val = fldVal.second;

		// �I�u�W�F�N�gID��LAT,LON�͖���
		if( fi.isOidField() || fi.isLatLonField() )
			continue;

		// �l���w�肳��Ă��Ȃ���ΘM��Ȃ�
		if( uh::variant_util::isNullOrEmpty( val ) )
			continue;

		row.SetValue( fi.destIndex, val );
		fi.isChanged = true;
	}

	// �`��
	if( ri.hasShape )
	{
		auto spRef = AheGetSpatialReference( (_IRow*)row );
		IPointPtr point( CLSID_Point );
		point->putref_SpatialReference( spRef );
		point->PutCoords( ri.latlon.X, ri.latlon.Y );
		row.SetShape( point );
	}
}

void StoreTable( CTableContainer& destT, const CsvLoader& csv, bool testMode  )
{
	// Store����ƍ������Ȃ��Ȃ�̂ł����Ń��O
	ERRLOGGER.WriteLog( csv, destT );

	if( testMode )
		return;

	// �ۑ�
	CErrorObjectsBase errs;
	RUNLOGGER.Info( _T("�ۑ�... "), true );
	if( sindyErr_NoErr != destT.Store( errs ) )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("�ۑ����s: %s"), errs.ToString() ) );
	}
	RUNLOGGER.Info( _T("OK.\n"), true );
}

void createRows( CTableContainer& destT, CsvLoader& csv, bool testMode )
{
	auto recordsList = csv.GetNewRecordsList();

	RUNLOGGER.Info( _T("�V�K���R�[�h�쐬... "), true );
	long count = 0;
	boost::timer t;

	for( auto& records : recordsList )
	{
		for( auto& record : records )
		{
			auto& row = *destT.CreateDummyRow();
			row.CreateCache();

			const auto& fm = *destT.GetFieldMap();

			// OPERATOR�͔�NULL�Ȃ̂Œl�����Ă����Ȃ��ƃG���[�ɐ����Ă��܂�
			if( 0 < fm.HasField( ipc_table::kOperator )  )
				row.SetValue( fm.HasField( ipc_table::kOperator ), _T("sindy") );

			// PURPOSE_C��
			if( 0 < fm.HasField( ipc_table::kPurpose )  )
				row.SetValue( fm.HasField( ipc_table::kPurpose ), 0L );

			setAttribute( row, record );

			++count;
		}
		
		// ���̉�
		CErrorObjectsBase errs;
		destT.Materialization(nullptr, 0, errs);

		StoreTable( destT, csv, testMode );

		// ��ʂ̏ꍇ������clear���Ȃ��Ƃ��ӂ��
		destT.clear();
		
		std::cerr << count << "records. " << t.elapsed() << "[s]" <<std::endl;
	}

	RUNLOGGER.Info( _T("ok. "), true );
}

void updateRows( CTableContainer& destT, CsvLoader& csv, bool testMode )
{
	// 1000�����Ƃ�IN��ōX�V�Ώۂ̃I�u�W�F�N�g��I��
	for( const auto& ids1000 : uh::enumerate_elements( csv.GetUpdateIDs() ) )
	{
		CString where = uh::str_util::format(
			_T("%s IN (%s)"), kObjectID, CString(ids1000.c_str())
		);

		destT._Select( AheInitQueryFilter( nullptr, nullptr, where ), false );
		destT.CreateCache();


		for( auto& row : destT )
		{
			auto& destRow = *CAST_ROWC(row);
			setAttribute( destRow, const_cast<RecordInfo&>(csv.find_by_oid(destRow.GetOID()) ) );
		}
		
		// ��ʂ̏ꍇ������clear���Ȃ��Ƃ��ӂ��
		StoreTable( destT, csv, testMode );

		destT.clear();
	}
}
