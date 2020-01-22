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
#include "BuddyRoadMgr.h"
#include <ArcHelperEx/Map.h>

using namespace sindy;
using namespace sindy::schema;

CBuddyRoad::CBuddyRoad( const IFeaturePtr& roadLink, const _IRowPtr& linkRel /*= nullptr*/ )
	: m_roadID(AheGetOID(roadLink)), 
	  m_geom(AheGetShapeCopy(roadLink)),
	  m_roadLink( roadLink ),
	  m_linkRel( linkRel ),
	  m_addF(false),
	  m_deleteF(false),
	  m_objectID(AheGetOID(linkRel)){}

CBuddyRoad::~CBuddyRoad()
{
}

CBuddyRoadMgr::CBuddyRoadMgr()
{
}


CBuddyRoadMgr::~CBuddyRoadMgr()
{
}

void CBuddyRoadMgr::Init( std::list<CFeatureDef>* featureDefs, const CArcHelper& ah  )
{ 
	m_searchedRoads.clear();
	// ���蓾�Ȃ��͂�������
	if( featureDefs->empty() )
		return;

	// FeatureDefs�͑��̃��C�����܂܂�Ă���\��������
	auto it = boost::find_if( *featureDefs, []( CFeatureDef& def ){
		return ( _tcscmp( walk_link::kTableName, def.GetTableName() ) == 0 );
	});
	_ASSERTE( it != featureDefs->end() );

	// RINK_RELATION�͕��s�҃����N�Ɠ������[�U�̂͂�
	ITablePtr walkT = AheGetFeatureClass( it->GetFeature() );
	m_LinkRelT = AheOpenSameOwnerTable( walkT, link_relation::kTableName );
	if( !m_LinkRelT )
	{
		CString msg = uh::str_util::format( AheLoadString( IDS_NOT_FOUND_RELTBL ), AheGetFeatureClassOwnerName( walkT ) );
		MessageBox( ah.GetFocusMapHWND(), msg, AheLoadString( IDS_CAPTION ), MB_ICONWARNING );
		return;
	}

	// ROAD_LINK�͓����O���[�v���C���ɂ������
	CMap cMap( ah.GetFocusMap() );
	m_roadLinkT = AheGetFeatureClass( cMap.FindSameGroupLayer( walkT, road_link::kTableName ) );
	if( !m_roadLinkT )
	{
		// �_�E���R���o�[�g�ł̎��ɂ͓��H�����N���Ȃ��Ă��A�ȉ��̃��b�Z�[�W���o���Ȃ��B
		if( walk_type::type_class::kDownConvert != AheIsDownconvert( it->GetFeature() ) )
			MessageBox( ah.GetFocusMapHWND(), AheLoadString( IDS_NOT_FOUND_ROADLINK ), AheLoadString( IDS_CAPTION ), MB_ICONWARNING );
		return;
	}
	// ���ꂼ��̕��s�҃����N�ɕR�t�����H�����N���m��
	for( auto& def : *featureDefs )
	{
		if( _tcscmp( walk_link::kTableName, def.GetTableName() ) != 0 )
			continue;

		long walkID = AheGetOID(def.GetFeature());

		// ���ɕR�t���Ă铹�H�����N��T��
		_ICursorPtr cur;
		m_LinkRelT->Search( 
			AheInitQueryFilter( nullptr, nullptr, _T("%s=%ld"), link_relation::kBaseLinkID, walkID ),
			VARIANT_FALSE, &cur );

		// ���H�����N�̏������X�g�Ɏ擾
		_IRowPtr row;
		while( (S_OK == cur->NextRow( &row )) && row )
		{ 
			long oid = AheGetAttribute(row, link_relation::kObjectID).lVal;

			// ���Ƀ��X�g�ɂ���ꍇ�̓X���[�ibug 11409�j
			// ���Ȃ��Ɓi�c���[�őI��؂�ւ��āj�\���������x�ɑ���������
			// ���H��ID�ł͂Ȃ��ALINK_RELATION�e�[�u�����̂̃I�u�W�F�N�gID�Ń��j�[�N�ɂȂ�悤�ύX�ibug 12472�j
			const auto it = boost::find_if( m_buddyRoads[walkID], [&oid](const CBuddyRoad& buddy ){
				return buddy.m_objectID == oid;
			});
			if( it != m_buddyRoads[walkID].end() )
				continue;

			IFeaturePtr roadLink;
			m_roadLinkT->GetFeature(AheGetAttribute(row, link_relation::kRelatedLinkID).lVal, &roadLink );
			m_buddyRoads[walkID].push_back( CBuddyRoad(roadLink, row) );
		}
	}
}

// �ύX�`�F�b�N
bool CBuddyRoadMgr::IsChanged( long walkID )
{
	// 1�ł��ύX������ΕύX����Ƃ���
	const auto& roadLinks = GetBuddyRoads(walkID);
	auto it = boost::find_if( roadLinks, []( const CBuddyRoad& road ){
		return road.IsChanged();
	});

	return ( it != roadLinks.end() );
}

// ���݃`�F�b�N
bool CBuddyRoadMgr::HasBuddy( long walkID )
{
	// �폜����Ă���͖̂���
	long count = boost::count_if( GetBuddyRoads( walkID ), []( const CBuddyRoad& road ){
		return !road.m_deleteF;
	});

	return ( count > 0 );
}


RoadList& CBuddyRoadMgr::GetBuddyRoads( long walkID )
{ 
	// �����ĂȂ���΃_�~�[��Ԃ�
	auto it = m_buddyRoads.find( walkID );
	if( m_buddyRoads.end() == it )
	{
		static RoadList dummy;
		return dummy;
	}

	return it->second;
}

// �����ςݓ��H�����N����o�f�B��ǉ�
void CBuddyRoadMgr::AddFromSearchedRoads( long walkID )
{
	// �����ς݂ō폜�t���O�������Ă�����̂��Ώ�
	for( const auto& searched : m_searchedRoads )
	{
		if( !searched.m_deleteF )
			continue;

		// ���ɂ���o�f�B����T��
		auto& buddys = GetBuddyRoads(walkID);

		if( !buddys.empty() )
		{
			auto it = boost::find( buddys, searched );
			if( buddys.end() != it )
			{
				// ���s�҃����N�����Ƀo�f�B�������Ă��āA���������̒��Ɋ܂܂����
				// �ŏ����炠�����̂���U�����āA�������Ȃ����āA�܂��ǉ��Ƃ������ꍇ�����肤��
				it->m_deleteF = false;
				continue;
			}
		}

		// ���s�҃����N�ɑ΂���o�f�B�ȓ��H��1���Ȃ��ꍇ�AGetBuddyRoads()��
		// �_�~�[��Ԃ��̂ŁA������push_back���Ă͂����Ȃ��B�K�������̃R���e�i�ɓ���邱�ƁB
		m_buddyRoads[walkID].push_back(searched);

		// �ǉ��t���O�͂����ł̂ݒǉ������O��
		m_buddyRoads[walkID].back().m_addF = true;
		m_buddyRoads[walkID].back().m_deleteF = false;
	}
	
	// �����ς݃����N����͍폜
	boost::range::remove_erase_if( m_searchedRoads, [](const CBuddyRoad& road){
		return road.m_deleteF;
	});
}

// �o�f�B�֌W�ɂ��邩
bool CBuddyRoadMgr::IsBuddy( long walkID, long roadID )
{
	const auto& roads = GetBuddyRoads( walkID );

	// �ړ��Ă�ID��T�������_��
	auto finder = [&roadID](const CBuddyRoad& road){
		return (road.m_roadID == roadID) && 
			   !road.m_deleteF;
	};

	return roads.end() != boost::find_if( roads, finder );
}

// ���H�����N��Buddy�Ώێ�ʁibug 11283�j
// TODO: �����Ђ��t���ł��������Ƃ���Ă�̂ŁA���C�u�����ɕ�����
bool isBuddyRoadClass( const _IRowPtr& roadRow )
{
	using namespace road_link::road_class;

	// �ΏۊO��ʁi�t�F���[�ȍ~�����A�뉀�H��OK�j
	switch ( AheGetAttribute( roadRow, road_link::kRoadClass ).lVal )
	{
	case kNone:                // �\����ʂȂ�
	case kFerryNoDisp:         // �t�F���[�q�H�i��\���j
	case kFerryS2:             // �t�F���[�q�H�iS2�j���j
	case kFerryS3:             // �t�F���[�q�H�iS3�j���j
	case kFerryS4:             // �t�F���[�q�H�iS4�j���j
	case kVirtual:             // ���z�����N
	case kBridge:              // �u���b�W�����N
	case kFacilityEntrance:    // �{�ݏo����������N
	case kParkingEntrance:     // �{�ݓ������N
	case kParking:             // ���ԏ�\�����H�iPEC�j
	case kMatching:            // �}�b�`���O�����N�iPEC�j
	case kMatchingBT:          // �}�b�`���O�p�����N(BT)
		return false;
		break;

	default:
		break;
	};

	// �����Ԑ�p
	if( 0 != AheGetAttribute( roadRow, road_link::kCarOnly ).lVal )
		return false;

	return true;
}

// ���ӌ���
RoadList& CBuddyRoadMgr::ArroundSearch( IFeaturePtr walkLink, double dRange)
{
	if( !m_roadLinkT )
	{
		MessageBox( nullptr, AheLoadString( IDS_NOT_FOUND_ROADLINK ), AheLoadString( IDS_CAPTION ), MB_ICONWARNING );
		return m_searchedRoads;
	}

	// LINK_RELATION�e�[�u������
	auto getLinkRel = []( const ITablePtr& rekT, long walkID, long roadID )
	{
		static auto query = uh::str_util::format( _T("%s=%ld AND %s=%ld"),
			link_relation::kBaseLinkID, walkID,
			link_relation::kRelatedLinkID, roadID );

		_ICursorPtr cur;
		rekT->Search( AheInitQueryFilter( nullptr, nullptr, query ), VARIANT_FALSE, &cur );

		// �����Ă�1�����̂͂�
		_IRowPtr row;
		if( cur->NextRow( &row ) != S_OK )
			row = nullptr;

		return row;
	};

	m_searchedRoads.clear();

	ITopologicalOperatorPtr ipTopo( AheGetShapeCopy( walkLink ) );

	// m �� �x�ϊ�
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits( dRange, esriMeters, esriDecimalDegrees, &dConvRange );

	// �`����o�b�t�@�����O
	IGeometryPtr ipGeoBuffer;
	ipTopo->Buffer( dConvRange, &ipGeoBuffer );

	// ��Ԍ���
	IFeatureCursorPtr cur;
	m_roadLinkT->Search( AheInitSpatialFilter(nullptr, ipGeoBuffer), VARIANT_FALSE, &cur );

	long walkID = AheGetOID( walkLink );
	IFeaturePtr row;
	while( (S_OK == cur->NextFeature( &row )) && row )
	{
		// ���ɕR�t���Ă�����͖̂���
		long roadID = AheGetOID(row);
		if( IsBuddy( walkID, roadID ) )
			continue;

		// �Ώۂ̓��H��ʂ���
		if( !isBuddyRoadClass( row ) )
			continue;

		m_searchedRoads.push_back( CBuddyRoad( row, getLinkRel( m_LinkRelT, walkID, roadID ) ) );
	}

	return m_searchedRoads;
}

// �_���`�F�b�N
bool CBuddyRoadMgr::CheckLogic( UINT& stringID )
{
	// TODO: �`�F�b�N���邱�ƂȂ��Ȃ����B�B�ibug 11283�j
	//       �ꉞ�A�N�`�͎c���Ă����B
	return true;
}

// �X�g�A����
bool CBuddyRoadMgr::UpdateLinkRelTable()
{
	for( auto& roads : m_buddyRoads )
	{
		for( auto& road : roads.second )
		{
			// �ύX���Ȃ����͖̂���
			if( !road.IsChanged() )
				continue;

			// �ǉ��i�쐬�j
			if( road.m_addF )
			{
				_IRowPtr row = AheCreateRow( m_LinkRelT );
				if( !row )
					return false;

				// �����t�^
				if( !AheSetModifyData( row, sindyUpdateTypeCreate ) ||
					!AheSetAttribute( row, link_relation::kBaseLinkID, roads.first ) ||
					!AheSetAttribute( row, link_relation::kRelatedLinkID, road.m_roadID ) )
					return false;
				
				road.m_linkRel = row;
			}

			// �폜
			else if( road.m_deleteF )
			{
				road.m_linkRel->Delete();
			}

			// �X�g�A
			if( FAILED(road.m_linkRel->Store() ) )
				return false;
		}
	}
	return true;
}

// �ێ����Ă���e�[�u���ނ̌�n��
void CBuddyRoadMgr::ClearLinkMap()
{
	m_buddyRoads.clear();
	m_searchedRoads.clear();
}
