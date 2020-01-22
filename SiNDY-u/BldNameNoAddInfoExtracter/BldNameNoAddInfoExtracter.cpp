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
#include "BldNameNoAddInfoExtracter.h"
#include "globalfunc.h"
#include "Logger.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_base/GeometryBase.h>
#include <WinLib/TimeTrace.h>

using namespace sindy;
using namespace schema;
using namespace sj;
using namespace sindyk;

CModel getBldNameModel()
{
	CModel model;
	model.Add( buildingname_point::kTableName, buildingname_point::kSrcOID,	nullptr, nullptr );
	model.Add( buildingname_src_point::kTableName, buildingname_src_point::kObjectID, buildingname_point::kTableName, buildingname_point::kSrcOID );
	return model;
}

// ������
void BldNameNoAddInfoExtracter::init()
{
	// �o�̓t�@�C��
	m_tsvWriter.init( m_args.getValue( option::kResult ) );

	// �G���[���O
	ERRLOGGER.Initialize( m_args.getValue( option::kErrLog ).GetString() );

	// �����r�����̃t�@�C���_������
	m_bldNameFinder.InitCollection( 
		tryOpenTable( m_args.getValue( option::kBldNameDb ), buildingname_point::kTableName ), getBldNameModel() );
	
	// �s���E�t�@�C���_������
	m_adminFinder.InitCollection( 
		tryOpenTable( m_args.getValue( option::kAdminDb ), city_admin::kTableName ), CModel() );

	// POI�t�@�C���_������
	m_poiFinder.InitCollection( 
		tryOpenTable( m_args.getValue( option::kPoiDb ), sindyk::poi_point_org::kTableName ), CModel() );
	// �⊮�pPOI�̏o�T���ɐݒ肷�邽�߁A���[�U�����擾���Ă���
	m_poiUser = uh::str_util::split( m_args.getValue( option::kPoiDb ), _T("/") )[0];

	// �Z��DB
	m_addrDb.init( m_args.getValue( option::kAddrDb ) );

}

namespace static_func{

	// ���k�s���E�`��Q�擾
	std::map<CString, IGeometryPtr> getBaseAdminGeoms( CTableContainer& baseAdminT,  // BASE_ADMIN
		                                               const CString& adminList )    // �s���E�R�[�h���X�g
	{
		using namespace uh::text_util;

		// key:�s���E�R�[�h�i5���j value:�`��
		std::map<CString, IGeometryPtr> admins;

		// ���X�g�ǂݍ���
		std::set<CString> strCity;
		for( const auto& city : ToTStrList( adminList.GetString() ) )
		{
			strCity.insert( city.c_str() );
		}

		// IN��̒��g�p�ɐ��`
		std::list<CString> cityCods;
		CTableBase::IDs2Str(std::list<CString>(strCity.begin(), strCity.end()), cityCods, _T("'") );
		// ����
		for( const auto& city : cityCods )
		{
			auto query = AheInitQueryFilter( nullptr, nullptr, 
							_T("%s IN ( %s )"), base_admin::kCityCode, city);

			baseAdminT._Select( query, false );
		}

		std::set<CString> selectedCity;
		for( const auto& ba : baseAdminT )
		{
			const auto& row = *CAST_ROWC(ba);
			CString cityCode = row.GetValue(base_admin::kCityCode, CString());
			if (admins[cityCode]) {
				//���ɑ��݂��Ă�����A�|���S�����}�[�W����i�ʏ�}���`�p�[�g�ɂȂ�͂��j
				CGeometryBase basePolygon = admins[cityCode];
				basePolygon.Union(row.CRowBase::GetShapeCopy());
				admins[cityCode] = basePolygon.Clone();
			} else {
				admins[cityCode] = row.CRowBase::GetShapeCopy();
			}

			// �`�F�b�N�p
			selectedCity.insert(cityCode);
		}

		// ���X�g�Ŏw�肳�ꂽ�s���E�R�[�h�̃��R�[�h���S�đ��݂��邩�`�F�b�N
		std::set<CString> diff;
		std::set_difference(
			strCity.begin(), strCity.end(),
			selectedCity.begin(), selectedCity.end(),
			std::inserter(diff, diff.end()));

		if (!diff.empty())
		{
			auto msg = uh::str_util::formatA(_T("�w�肳�ꂽ�s���E�̃��R�[�h�����݂��Ȃ��F %s"),
				uh::str_util::join(diff, _T(" ")));

			throw std::runtime_error(msg);
		}
		
		baseAdminT.clear();

		return admins;
	}

	// 11���R�[�h�擾
	CString getCityAddrCode( const CRowContainer& row,  // �Ώۂ̃r������
		                 CTableContainer& cityAdminT )  // CITY_ADMIN
	{
		cityAdminT._Select( (IQueryFilterPtr)AheInitSpatialFilter( nullptr, row.CRowBase::GetShapeCopy() ), true );
		if( cityAdminT.empty() )
		{
			ERRLOGGER.WriteLog( row, _T("CITY ADMIN �擾���s�B") );
			return CString();
		}

		CString cityCode = CAST_ROWC(*cityAdminT.begin())->GetValue( city_admin::kCityCode );
		CString addrCode = CAST_ROWC(*cityAdminT.begin())->GetValue( city_admin::kAddrCode );

		// begin()��1�������E���Ă���̂Ŗ�������Ȃ��ƃ_��
		cityAdminT.clear();
		return cityCode + addrCode;
	}

} // namespace static_func


// ���s
void BldNameNoAddInfoExtracter::execute()
{
	using namespace static_func;

	// ������
	init();
	
	// �e��e�[�u��
	auto& bldNameT = *m_bldNameFinder.FindTable( buildingname_point::kTableName );
	auto& cityAdminT = *m_adminFinder.FindTable( city_admin::kTableName );
	auto& srcPointT = *m_bldNameFinder.FindTable( buildingname_src_point::kTableName );
	auto& poiT = *m_poiFinder.FindTable( poi_point_org::kTableName );

	// �s���E�i5���j�`��擾
	auto baseAdmins = getBaseAdminGeoms( 
		*m_adminFinder.FindTable( base_admin::kTableName), 
		m_args.getValue( option::kBaseAdminList ) );

	// �s���E�P�ʂŏ���
	for( const auto& ba : baseAdmins )
	{
		if( !ba.second)
			continue;

		// �s�����P�ʂŏZ�����L���b�V��
		m_addrDb.cacheAddrInCity( ba.first );

		// �s���E������Ԍ���
		auto query = AheInitSpatialFilter( nullptr, ba.second );
		if( bldNameT.FindField(buildingname_point::kYomiSeparated) > -1 )
		{
			query = AheInitQueryFilter( (IQueryFilterPtr)AheGetClone(query), nullptr,
				_T("%s IS NULL"), buildingname_point::kYomiSeparated );
		}


		// �����m�F
		RUNLOGGER.Log( uh::str_util::format( _T("%s�F %ld�� "), ba.first, bldNameT.RowCount( query )), true );
		boost::timer timer;
		uh::scope_guard sg( [&timer]{
			RUNLOGGER.Log( uh::str_util::format( _T("[����] %lf[s]\n"), timer.elapsed() ), true );
		});

		// �������s
		std::map<CString, IQueryFilterPtr> querys = boost::assign::map_list_of
			( buildingname_point::kTableName, query );
		bldNameT.Select( querys, true );

		// BUIDNAME_POINT���Ƃ̏���
		for( const auto& bn : bldNameT )
		{
			const auto& bldNameRow = *CAST_ROWC( bn );

			// 1�s�����߂���
			m_tsvWriter.addRecord(
				bldNameRow,
				getSrcPoint( bldNameRow, poiT ),
				m_addrDb.getAddrInfo( getCityAddrCode( bldNameRow, cityAdminT ) ),
				getPoiYomi( bldNameRow, poiT )
			);
		}

		// ���ʏ�������
		m_tsvWriter.flush();

		bldNameT.clear();
		cityAdminT.clear();
		srcPointT.clear();
		poiT.clear();
	}
}


CSPRowContainer BldNameNoAddInfoExtracter::getSrcPoint( const CRowContainer& bldName, CTableContainer& poiT )
{
	CSPRowContainer srcP;
	switch (bldName.GetValue( buildingname_point::kSourceType, -1L ))
	{
	case buildingname_point::source_type::kCompAnnoPOI:
		{
			// �⊮�pPOI�͑f�ނɂ��Ȃ��̂ŁA�_�~�[������Ăł����グ��
			srcP = bldName.FindTable( buildingname_src_point::kTableName )->CreateDummyRow();
			
			// POI_POINT_ORG�̃e�[�u������T��
			auto it = poiT.find_by_key( kObjectID, bldName.GetValue( buildingname_point::kSrcOID, -1L ) );
			if( it == poiT.end() )
				return CSPRowContainer();

			// �o�T����ID��POI�V�[�P���X
			auto poi = CAST_ROWC(*it);
			long conCode = poi->GetValue( poi_point_org::kContentsCode, -1L );
			std::vector<CString> poiSeq = boost::assign::list_of
				( uh::str_util::ToString( conCode / 100000 ) )
				( uh::str_util::ToString( conCode % 100000 ) )
				( poi->GetValue( poi_point_org::kContentsSeq, _T("") ) );
			srcP->SetValue( buildingname_src_point::kSourceID, uh::str_util::join( poiSeq, _T("-") ) );

			// �o�T�R�[�h��102�Œ�
			srcP->SetValue( buildingname_src_point::kSourceType, (long)buildingname_src_point::source_type::kCompAnnoPOI );
			// �o�T���͐ڑ���̃��[�U��
			srcP->SetValue( buildingname_src_point::kSourceName, m_poiUser );
		}
		break;

	default:
		// �⊮�pPOI�ȊO�̓��f���̑Ή��t���ɂ��R�t���Ă���͂�
		auto& srcPointT = *bldName.GetItemTable( buildingname_src_point::kTableName );
		if( srcPointT.empty() )
		{
			ERRLOGGER.WriteLog( bldName, _T("�R�t���f�ނ�������Ȃ��B") );
			return srcP;
		}
		srcP = CAST_ROWC(*srcPointT.begin());
		break;
	}

	return srcP;
}

CString BldNameNoAddInfoExtracter::getPoiYomi( const CRowContainer& bldName, CTableContainer& poiT )
{
	// NULL�\���p������i�󕶎��j
	static const CString NULL_STRING = _T("");

	poiT.SetAutoCache(sindyContainerAutoOnDemandCacheDetach);

	// xxxx-x-xxx �� �R���e���c�R�[�h�ƃV�[�P���X�ɂ��郉���_��
	auto toCodeAndSeq = [&bldName](const CString& seqStr )
	{
		auto seq = uh::str_util::split( seqStr, _T("-") );
		if( seq.size() != 3 )
		{
			ERRLOGGER.WriteLog( bldName, uh::str_util::format( 
				_T("POI�̃V�[�P���X���s��[ %s ]"), seqStr ) );
			return std::make_pair(0L, CString() );
		}
		long conCode = _ttol(seq[0])*100000 + _ttol(seq[1]);
		return std::make_pair( conCode, seq[2] );
	};

	CSPRowContainer poi;
	switch (bldName.GetValue( buildingname_point::kSourceType, -1L ))
	{
	// �⊮�pPOI��SrcOID�ɒ���POI��OID�������Ă���
	case buildingname_point::source_type::kCompAnnoPOI:
		{
			long poiId = bldName.GetValue( buildingname_point::kSrcOID ).lVal;
			auto it = poiT.find_by_key( kObjectID, poiId);
			if( it != poiT.end() )
				poi = CAST_ROWC(*it);
			else
			{
				ERRLOGGER.WriteLog( bldName, uh::str_util::format( 
					_T("����POI�i�⊮�p�j��������Ȃ��iOID�F%ld�j"), poiId ) );
			}
		}
		break;

	// ���LPOI�͑f��P��OID�������Ă��āA���̑f��P��POI�̃V�[�P���X�������Ă���B�B�B
	case buildingname_point::source_type::kAnnoPOI:
		{
			// �f��OID����R���e���c�R�[�h�ƃV�[�P���X���擾����
			const auto& srcP = *getSrcPoint( bldName, poiT );
			CString strSeq = srcP.GetValue( buildingname_src_point::kSourceID, _T("") );
			auto codeAndSeq = toCodeAndSeq( strSeq );

			// �R���e���c�R�[�h�ƃR���e���c���V�[�P���X���^�O�ɂ��Č���
			static const auto tags = std::make_pair( 
				poi_point_org::kContentsCode, poi_point_org::kContentsSeq );

			auto keys = std::make_pair( codeAndSeq.first, codeAndSeq.second );
			auto it = poiT.find_by_key( tags, keys );
			if( it != poiT.end() )
				poi = CAST_ROWC(*it);
			else
			{
				ERRLOGGER.WriteLog( bldName, uh::str_util::format( 
					_T("����POI��������Ȃ��iPOI�F%s�j"), strSeq ) );
			}
		}
		break;

	// POI�ȊO��NULL
	default:
		return NULL_STRING;
		break;
	}

	// ������Ȃ�������NULL
	if( !poi )
		return NULL_STRING;

	// �u���~�k���v�Ȃ�NULL
	CString yomi = poi->GetValue( poi_point_org::kYomi, _T("") );
	if( yomi.Compare( _T("���~�k��") ) == 0 )
		return NULL_STRING;

	// �b�p�C�v�͍폜
	return uh::str_util::replace(yomi, _T("�b"));
}
