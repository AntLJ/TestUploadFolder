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
#include "AddrAnalyzer.h"
#include "Logger.h"
#include "globalfunc.h"
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::sj;

void AddrAnalyzer::init( const CString& addrSvr, const CString& xyUsr, const CString& workUsr, const CString& buildingDB )
{
	m_addr2XY.init( addrSvr, xyUsr );
	connectAdo( addrSvr, workUsr, m_adoWorkUser );
	
	// �ƌ`�p
	auto tbl = TryOpenTable( buildingDB, building::kTableName );
	m_finder.InitCollection( tbl, CModel() );

	// �����X�V�Ɏg�p����̂łƂ��Ă���
	m_xyUser = xyUsr;
}

bool AddrAnalyzer::updateRow( CRowContainer& row )
{
	using namespace sindy::schema::sj::buildingname_src_point;

	// XY�t�^����
	if( !m_addr2XY.execFunc( row.GetValue( kAddr, _T("") ) ) )
		return false;

	// ADDRCODE
	CString addrCode = m_addr2XY.getAddrCode();
	row.SetValue( kAddrCode, addrCode );

	// ADDR_LON,LAT
	auto lonLat = m_addr2XY.getLonLat();
	row.SetValue( kAddrLon, lonLat.X );
	row.SetValue( kAddrLat, lonLat.Y );

	// ADDR_VER
	row.SetValue( kAddrVer, m_xyUser );

	// REPTYPE_C�i�ꗥ�u0�v�j
	row.SetValue( kRepType, (long)chiban_fukugou::kIndefinite );

	// ����ȍ~�͖h�ΑΏە����iSOURCE_TYPE=2�j�̂ݍX�V�Ώ�
	if( row.GetValue( kSourceType, -1L ) == source_type::kFirePreventive )
	{
		// ADDRLEVEL_C
		row.SetValue( kAddrLevel, m_addr2XY.getAddrLevel() );
		
		// PINPOINT_F
		row.SetValue( kPinPoint, (long)m_addr2XY.isPinpoint() );

		// SAME_ADDR_COUNT�ASAME_ADDR_F
		long sameAddrs = countSameAddr( addrCode );
		row.SetValue( kSameAddrCount, sameAddrs );
		row.SetValue( kSameAddr, (sameAddrs >= 2) ? 1L : 0L );

		// �`��
		IPointPtr point = createPoint( lonLat, AheGetSpatialReference( row.GetTable() ) );
		row.SetShape( point );

		// MULTISAMEBLDG_F
		row.SetValue( kMultiSameBldg, (countBuildingAddrs(point) >= 2) ? 1L : 0L );
	}
	else
	{
		row.SetValue( kAddrLevel, (long)addr_level::kUnconverted );
		row.SetValue( kPinPoint, 0L);
		row.SetValue( kSameAddrCount, 0L );
		row.SetValue( kSameAddr, 0L );
		row.SetValue( kMultiSameBldg, 0L );
	}

	return true;
}

long AddrAnalyzer::countSameAddr( const CString& addrCode )
{
	static const CString count_point_per_addr =
		_T("select count(*) as addr_count")
		_T(" from (")
		_T("  select 'g' as point_type, point_id, addr_code from gp_work where addr_code = '%s'")
		_T("   union ")
		_T("  select 'c' as point_type, point_id, addr_code from cs_work where addr_code = '%s'")
		_T(" )")
		_T(" group by addr_code");

	long count = 0;
	try{
		count = getCountResult( uh::str_util::format( count_point_per_addr, addrCode, addrCode ) );
	}
	catch( const std::runtime_error& e){
		uh::str_util::formatA( _T("����Z�����R�[�h�̌����Ɏ��s�F %s"), CA2T( e.what() ) );
	}

	return count;
}

long AddrAnalyzer::countBuildingAddrs( IPointPtr bldgSrcP )
{
	// �|�C���g������Ă���ƌ`���擾
	auto& buildingT = *m_finder.FindTable( building::kTableName );
	buildingT.clear();
	buildingT._Select( AheInitQueryFilter( AheInitSpatialFilter( nullptr, bldgSrcP ) ), true );

	// �ƌ`�O�̃|�C���g
	if( buildingT.empty() )
		return 0;

	// 1�ԍŏ��Ɍ����������́iBNAnalyzer.exe�̋����𓥏P�j
	long buildingId = CAST_ROWC( *buildingT.begin() )->GetOID();

	static const CString extract_build_id_with_plural_points =
		_T("select count(*) as point_count")
		_T(" from (")
		_T("  select 'g' as point_type, point_id, polygon_id from gp_polygon where polygon_id = %d")
		_T("   union")
		_T("  select 'c' as point_type, point_id, polygon_id from cs_polygon where polygon_id = %d")
		_T(" )")
		_T(" group by polygon_id");
	
	long count = 0;
	try{
		count = getCountResult( uh::str_util::format( extract_build_id_with_plural_points, buildingId, buildingId ) );
	}
	catch( const std::runtime_error& e){
		uh::str_util::formatA( _T("����ƌn���Z���|�C���g���̌����Ɏ��s�F %s"), CA2T( e.what() ) );
	}

	return count;
}

long AddrAnalyzer::getCountResult( const CString& query )
{
	auto cur = m_adoWorkUser.search( CT2A(query) );

	if( FAILED(cur.err()) )
	{
		throw std::runtime_error( CT2A(cur.GetErrorMessage()) );
	}

	long count = 0;
	for( const auto& row : cur )
	{
		count = row[0].lVal;  // 1�s1�񂾂��̂͂�
	}

	return count;
}
