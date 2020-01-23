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

/**
 * @file RoadNetwork.cpp
 * @brief <b>CRoadNetwork�N���X�����t�@�C��</b>\n
 * @author 
 * @version $Id$
 */
#include "stdafx.h"
#include <boost/foreach.hpp>
#include "RoadNetwork.h"
#include "GlobalFunctions.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "GeometryRule.h"
#include "TableTypeUtil.h"
#include "AheNetwork.h"
#include <typeinfo>
#include "RuleModel.h"
#include "AheLayerFunctions.h"
#include "Mesh.h"
#include "meshutil.h"
#include "define.h"
#include "AheGeometryOp.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define foreach BOOST_FOREACH
BOOL AhePointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef/* = NULL*/, BOOL bExeptVertex/* = FALSE*/ );


namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR SHAPE_POINT_TOUCHING = _T("%d - %d �\���_�ǂ������ڐG���Ă��܂�");
	LPCTSTR SEGMENT_ARE_CLOSE    = _T("%ld - %ld ����m�[�h�ɐڑ�����������ߐڂ��Ă��܂��i���K�����W���� %.1lf�j");
	LPCTSTR ANGLE_LESS_THAN_20   = _T("%ld - %ld 2���H�̐ڑ��p��20���ȉ��ł��i%d���j");
#else
	LPCTSTR SHAPE_POINT_TOUCHING = _T("%d-%d shape points are touching");
	LPCTSTR SEGMENT_ARE_CLOSE    = _T("%ld-%ld Line segments that are connected to the same node are close together (Normalized coordinate distance %.1lf)");
	LPCTSTR ANGLE_LESS_THAN_20   = _T("%ld - %ld Connection angle of forked path is less than 20(%d degree)");
#endif // SINDY_FOR_ENGLISH
}

namespace sindy {
using namespace errorcode;

// 2�����b�V����`�Ō������邩�ǂ����̃t���O
// 
// 2�����b�V���Ńl�b�g���[�N����������ꍇ�͌�����`��
// �Ŋ֘A�t�B�[�`�����������邽�߁A�������ȗ�������܂��B
// �o�b�`��������2�����b�V���P�ʂŏ���������ۂɂ͂���
// �t���O��ON�ɂ��邱�ƂŌ����̍ۂɂ��Ȃ荂��������܂��B
bool g_bNetworkSearchBy2ndMesh = false;

CSPRow RowClassFactory( _IRow* ipRow, sindyTableType::ECode emType, const CSPFieldMap& cFields, const CSPTableNameString& cNames )
{
	CSPRow cRet; // �߂�l
	LPCTSTR lpcszTableName = cNames->GetTableName();
	if( AheIsNetworkNode( lpcszTableName ) )
		cRet.reset( new CRelNode( ipRow, emType, false, cFields, cNames ) );
	else if( AheIsNetworkLink( lpcszTableName ) )
		cRet.reset( new CRelRoad( ipRow, emType, false, cFields, cNames ) );
	else if( (IFeaturePtr)ipRow )
		cRet.reset( new CFeature( ipRow, emType, false, cFields, cNames ) );
	else
		cRet.reset( new CRow( ipRow, emType, false, cFields, cNames ) );

	return cRet;
}

// �`��Ō�������
sindyErrCode CRoadNetwork::Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/, bool bInit /* = true*/ )
{
	// �W�I���g����NULL�ł������ł��邪�A�����ł͂�����
	// �G���[�Ƃ��邱�Ƃŗ\�����Ȃ��S���������������
	RETURN_IF( ! ipGeom, sindyErr_ArgIsNull )

	sindyErrCode emErr = _Select( ipGeom, bRecycling, bCreateCache, cErrs, bInit );

	// ���ӍČ����i�����N�[�_��p���� bug 6157�j
	IGeometryCollectionPtr ipGeomColl( CLSID_GeometryBag );

	((IGeometryPtr)ipGeomColl)->putref_SpatialReference( GetSpatialReference() );

	// ���H�̗��[���擾
	sindyTableType::ECode emNetworkLinkTableType = GetTableType();
	for ( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad )
	{
		ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint(0) );
		ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 ) );
	}

	CGeometry cSearchGeom = (IGeometryPtr)ipGeomColl;

	if ( (IGeometry*)cSearchGeom && !cSearchGeom.IsEmpty() )
		emErr = _Select( cSearchGeom, bRecycling, bCreateCache, cErrs, false, false );

	return emErr;
}

// �`��Ō�������
sindyErrCode CRoadNetwork::_Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/, bool bInit /* = true*/, bool bGetRelated /* = true*/ )
{
	return _Select( AheInitSpatialFilter( NULL, ipGeom ), bRecycling, bCreateCache, cErrs, bInit, bGetRelated );
}
sindyErrCode CRoadNetwork::_Select( IQueryFilter* ipFilter, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/, bool bInit /* = true*/, bool bGetRelated /* = true*/ )
{
	// �x�[�X�I�u�W�F�N�g�̌���
	sindyErrCode emErr = CFeatureContainer::Select( ipFilter, typeid(CRelRoad&), bRecycling, bCreateCache, cErrs, bInit );
	sindyTableType::ECode emNetworkLinkTableType = GetTableType();

	unsigned int meshcode = 0; // 2�����b�V���P�ʂŌ�������Ƃ��̂ݎg�p
	if( g_bNetworkSearchBy2ndMesh )
	{
		// ���b�V���R�[�h�𒲂ׂ�
		IGeometryPtr ipGeom;
		((ISpatialFilterPtr)ipFilter)->get_Geometry( &ipGeom );
		IPointPtr ipPoint;
		((IAreaPtr)ipGeom)->get_Centroid( &ipPoint );
		WKSPoint center = {0,0};
		int x = 0, y = 0;
		ipPoint->QueryCoords( &center.X, &center.Y );
		meshcode = LL2MESH( center.X, center.Y, 2, x, y );

		for(rows_iterator i=begin(sindy::sindyTableType::road_link);i!=end(sindy::sindyTableType::road_link);i++){
			ISpatialReferencePtr srp;
			IGeometryPtr ipShape=(*i)->GetShapeCopy();
			ipShape->get_SpatialReference(&srp);
			if (!AheIsContain(ipGeom,ipShape ,srp)){
				erase(*i);
			}
		}
	}

	// �����N�����������
	for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf )
	{
		// IPCVICS�����݂��Ȃ����f��������̂ŁA����͔�΂�
		long index_lqrf = GetFieldMap()->HasField( itInf->get<2>() );
		if( 0 > index_lqrf )
			continue;

		sindyTableType::ECode emInfCode = AheTableName2TableType(itInf->get<0>());
		sindyTableType::ECode emLqCode = AheTableName2TableType(itInf->get<1>());
		//////////////////////////////////////////
		// INF ID�̎��W
		//////////////////////////////////////////
		std::set<long> listInfIDs; // �擾�Ώ�INF��ID��
		if( g_bNetworkSearchBy2ndMesh )
		{
			// 2�����b�V����`�̏ꍇ��LQ�����b�V���R�[�h�Ō�������
			// ����ɂ��A���H��ID���g�p����LQ�̌������ւł���ibug 7301�j
			CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );
			LOGASSERTEERR_IF( cLQTable != NULL, sindyErr_TableNotFound )
			{
				_ICursorPtr ipCursor( cLQTable.Search( AheInitQueryFilter( NULL, _T("*"), _T("%s=%u"), schema::link_queue::kMeshCode, meshcode ), true ) ); // INF��ID����邾���Ȃ̂Ŏ̂Ă�
				// �擾����LQ����INF��ID���擾����
				long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
				LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
				{
					_IRowPtr ipRow;
					while( S_OK == ipCursor->NextRow( &ipRow ) )
					{
						if( ! ipRow )
							break;
						CComVariant vaInfID(-1L);
						ipRow->get_Value( lInfIDIndex, &vaInfID );
						listInfIDs.insert( vaInfID.lVal );
					}
				}
			}
		}
		else {
			// �֘A���铹�HID�����W����
			std::list<long> listRoadIDs;
			for( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad )
			{
				if( (*itRoad)->GetBoolValue( index_lqrf, false ) )	// �֘A�t���O�� true ���ǂ���
					listRoadIDs.push_back( (*itRoad)->GetOID() );
			}

			if( ! listRoadIDs.empty() )
			{
				// LQ�擾
				CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );
				LOGASSERTEERR_IF( cLQTable != NULL, sindyErr_TableNotFound )
				{
					// ���W�������HID�Ɋ֘A����LQ���擾
					// �����Ō����ł���LQ�͎̂Ă�iINF��ID���X�g�쐬�݂̂Ɏg�p�j
					std::list<CString> listIDsString;
					cLQTable.IDs2Str( listRoadIDs, listIDsString );
					for( std::list<CString>::const_iterator itIDsString = listIDsString.begin(); itIDsString != listIDsString.end(); ++itIDsString )
					{
						// ����
						_ICursorPtr ipCursor( cLQTable.Search( AheInitQueryFilter( NULL, schema::link_queue::kInfID, _T("%s IN (%s)"), schema::link_queue::kLinkID, *itIDsString ), true ) );	// �Ƃ肠����INF��ID���Ƃ�̂ŁALQ�͎̂Ă�

						// �擾����LQ����INF��ID���擾����
						long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
						LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
						{
							_IRowPtr ipRow;
							while( S_OK == ipCursor->NextRow( &ipRow ) )
							{
								if( ipRow == NULL ) break;

								CComVariant vaInfID(-1L);
								ipRow->get_Value( lInfIDIndex, &vaInfID );

								listInfIDs.insert( vaInfID.lVal );
							}
						}
					}
				}
			}
		}
		// ���Ƀ��X�g�ɑ��݂���ꍇ�͂��̕�����������
		foreach( const CLinkQueue& lq, m_mapLinkQueue[emInfCode] )
			listInfIDs.erase( lq.GetOID() );
//		for( std::list<CLinkQueue>::const_iterator it = m_mapLinkQueue[emInfCode].begin(), itEnd = m_mapLinkQueue[emInfCode].end(); it != itEnd; ++it )
//			listInfIDs.remove( it->GetOID() );

		if( listInfIDs.empty() )
			continue;

		//////////////////////////////////////////
		// INF�̌���
		//////////////////////////////////////////
		// CLinkQueue�N���X�̍\����A�K�����INF������K�v������
		// �֘A����INF����������
		CTable cInfTable( emInfCode, OpenSameOwnerTable( emInfCode ) );
		LOGASSERTEERR_IF( cInfTable != NULL, sindyErr_TableNotFound )
		{
			// ���L�I�u�W�F�N�g�̎擾
			CSPTableNameString spInfNameString( cInfTable.GetNameString() );
			CSPFieldMap spInfFieldMap( cInfTable.GetFieldMap() );

			// INF����
			_ICursorPtr ipInfCursor( cInfTable.GetRows( listInfIDs, bRecycling ) );
			LOGASSERTEERR_IF( NULL != ipInfCursor, sindyErr_SearchFailed )
			{
				// INF�i�[
				_IRowPtr ipRow;
				while( S_OK == ipInfCursor->NextRow( &ipRow ) )
				{
					if( ipRow == NULL ) break;

					linkq_iterator itLinkQueue;
					if( g_bNetworkSearchBy2ndMesh ) // 2�����b�V���ł̌�������linkq_find���Ȃ��i��������K�v�͂Ȃ��Ǝv���j
						itLinkQueue = m_mapLinkQueue[emInfCode].insert( m_mapLinkQueue[emInfCode].end(), CLinkQueue(emInfCode,ipRow,spInfFieldMap,spInfNameString) );
					else {
						long lOID = -1;
						ipRow->get_OID( &lOID );

						itLinkQueue = linkq_find( emInfCode, lOID );
						if ( itLinkQueue == linkq_end(emInfCode) )
							itLinkQueue = m_mapLinkQueue[emInfCode].insert( m_mapLinkQueue[emInfCode].end(), CLinkQueue(emInfCode,ipRow,spInfFieldMap,spInfNameString) );
						else
							(*itLinkQueue) = ipRow;
					}
					// ���T�C�N������ꍇ�̓L���b�V���쐬
					if( bRecycling )
						itLinkQueue->CreateCache( true, spInfFieldMap, spInfNameString );
					else if( bCreateCache )
						itLinkQueue->CreateCache( false, spInfFieldMap, spInfNameString );
				}
			}
			ipInfCursor = NULL;
		}
		//////////////////////////////////////////
		// LQ �̌���
		//////////////////////////////////////////
		CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );
		LOGASSERTEERR_IF( cLQTable != NULL, sindyErr_TableNotFound )
		{
			// �擾����INFID�Ɋ֘A����LQ���擾����
			std::list<CString> listIDsString;
			cLQTable.IDs2Str( listInfIDs, listIDsString, 500 );
			// ���L�I�u�W�F�N�g�̎擾
			CSPTableNameString spLQNameString( cLQTable.GetNameString() );
			CSPFieldMap spLQFieldMap( cLQTable.GetFieldMap() );
			long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
			// LQ��push_back������INF�̒������A���̍ۂ�linkq_find������B
			// ����͑S�Ă�INF���Ȃ߂Ă���̂Ŕ��Ɍ����������B
			// �Ȃ̂ŁA�����Ŏ��O�ɑΉ��\���쐬���Ă����ibug 7301�j
			std::map<long,CLinkQueue*> mapINF;
			foreach( CLinkQueue& inf, m_mapLinkQueue[emInfCode] )
				mapINF[inf.GetOID()] = &inf;
			for( std::list<CString>::const_iterator itIDsString = listIDsString.begin(); itIDsString != listIDsString.end(); ++itIDsString )
			{
				// ����
				_ICursorPtr ipCursor( cLQTable.Search( AheInitQueryFilter( NULL, NULL, _T("%s IN (%s)"), schema::link_queue::kInfID, *itIDsString ), bRecycling ) );

				// LQ��INFID�ŐU�蕪��
				LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
				{
					_IRowPtr ipRow;
					while( S_OK == ipCursor->NextRow( &ipRow ) )
					{
						if( ipRow == NULL ) break;

						CComVariant vaInfID( -1L );
						ipRow->get_Value( lInfIDIndex, &vaInfID );

//						linkq_iterator itLinkQueue = linkq_find( emInfCode, vaInfID.lVal );
//						itLinkQueue->push_back( emLqCode, ipRow, spLQFieldMap, spLQNameString );
						CLinkQueue& inf = *mapINF[vaInfID.lVal];
						inf.push_back( emLqCode, ipRow, spLQFieldMap, spLQNameString );

						// ���T�C�N������ꍇ�̓L���b�V���쐬
						if( bRecycling || bCreateCache )
							inf.rbegin()->CreateCache( ( bRecycling ) ? true : false, spLQFieldMap, spLQNameString );
					}
					ipCursor = NULL;
				}
			}
		}
	}

	// �֘A����m�[�h����������
	CString strNetworkNodeTableName( AheGetNetworkNodeFeatureClassName( GetTableName() ) );
	sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType( strNetworkNodeTableName );
	if( g_bNetworkSearchBy2ndMesh )
	{
		// 2�����b�V����`�ł̌����Ȃ�A���������NID�ň����Ă���K�v�͂Ȃ��ibug 7301�j
		CTable cClass( OpenSameOwnerTable( strNetworkNodeTableName ) );
		_ICursorPtr ipCursor( cClass.Search( ipFilter, bRecycling ) );
		LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
		{
			Add( ipCursor, typeid(CRelNode&), emNetworkNodeTableType, cClass.GetFieldMap(), cClass.GetNameString(), bRecycling, bCreateCache, cErrs, false );
		}
	}
	else {
		if( ! empty(emNetworkLinkTableType) )
		{
			// FROM�ATO����m�[�hID��쐬
			std::list<long> listNodeIDs;
			for( const_rows_iterator itRoad = begin(emNetworkLinkTableType); itRoad != end(emNetworkLinkTableType); ++itRoad )
			{
				listNodeIDs.push_back( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) ); // TODO: FROM�ATO�t�B�[���h���̓��[���ɒ�`���Ă��Ȃ����A�ǂ��H�H
				listNodeIDs.push_back( (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) );
			}

			listNodeIDs.sort();
			listNodeIDs.unique();
			// ����
			CFeatureContainer::Select( strNetworkNodeTableName, listNodeIDs, typeid(CRelNode&), bRecycling, bCreateCache, cErrs, false ); // �������͍ŏ��ɂ��Ă���̂ōŌ��false
		}
	}
	// �֘A�I�u�W�F�N�g�̓l�b�g���[�N�����N��ID�Ō�������̂�ID�������Ă���
	// TODO: �������AID�֘A��������̂̂݁B�����`��݂̂̊֘A�̂��̂��ł����Ƃ��́A���̂Ƃ��ɍl���悤
	std::list<long> listRoadIDs;
	for( const_rows_iterator itRoad = begin(emNetworkLinkTableType); itRoad != end(emNetworkLinkTableType); ++itRoad )
		listRoadIDs.push_back( (*itRoad)->GetOID() );

	// ����I�u�W�F�N�g�̌����i������ID�֘A��������̂̂݁j
	if( g_bNetworkSearchBy2ndMesh )
	{
		for( CRuleRelationshipContain::const_value_iterator it = g_cRuleRelationshipContain.begin(GetTableName()), 
			itEnd = g_cRuleRelationshipContain.end(GetTableName()); it != itEnd; ++it )
		{
			if( it->second.IsEmpty() ) // ID�֘A��������̂̂�
				continue;

			// 2�����b�V����`�ł̌����Ȃ�A���������NID�ň����Ă���K�v�͂Ȃ��ibug 7301�j
			CTable cClass( OpenSameOwnerTable( it->first ) );
			_ICursorPtr ipCursor( cClass.Search( ipFilter, bRecycling ) );
			LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
			{
				Add( ipCursor, typeid(CFeature&), AheTableName2TableType(it->first), cClass.GetFieldMap(), cClass.GetNameString(), bRecycling, bCreateCache, cErrs, false, it->second, GetTableName() );
			}
		}
	}
	else {
		if( ! listRoadIDs.empty() )
		{
			for( CRuleRelationshipContain::const_value_iterator it = g_cRuleRelationshipContain.begin(GetTableName()), 
				itEnd = g_cRuleRelationshipContain.end(GetTableName()); it != itEnd; ++it )
			{
				if( ! it->second.IsEmpty() ) // ID�֘A��������̂̂�
				{
					// ����
					CFeatureContainer::Select( it->first, listRoadIDs, typeid(CFeature&), bRecycling, bCreateCache, cErrs, false, it->second, GetTableName() ); // �������͍ŏ��ɂ��Ă���̂ōŌ��false
				}
			}
		}
	}
	// �O���e�[�u���̌���
	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin(GetTableName()),
		itEnd = g_cRuleRelationTable.end(GetTableName()); it != itEnd; ++it )
	{
		// *_RF�t���O���t���Ă��郊���NOID�̎��W
		std::list<long> listHasRFOID;
		long index = GetFieldMap()->FindField(it->get<0>()); // *_RF
		for( const_rows_iterator itLink = begin(emNetworkLinkTableType), itLinkEnd = end(emNetworkLinkTableType); itLink != itLinkEnd; ++itLink )
		{
			// TODO : RAILWAY_LINK��*_RF�t���O�������Ă��Ȃ����߁A���ߑł��őΉ��B�i�֘A�e�[�u�����폜���邽�߁j
			// �{���͊֘A�t���O�t�B�[���h�����󂾂�����R�s�[���Ȃ��Ƃ���ׂ��B�Ή�����RAILWAL_NODE�̕����Y�ꂸ�ɑΉ��̂��ƁB
			// ���������A"�֘A�e�[�u�������t�B�[�`���N���X�͊֘A�t�B�[���h������"�Ƃ����O��Ń��C�u�����͍\������Ă���̂ŁA
			// �֘A�e�[�u�������t�B�[���h�͊֘A�t���O�����悤�Ƀ��f���ύX����ׂ�
			if( 0 == (CString(GetTableName()).CompareNoCase(sindy::schema::sj::railway_link::kTableName)) ||
				1 == (*itLink)->GetLongValue( index, -1 ) )
				listHasRFOID.push_back( (*itLink)->GetOID() );
		}
		// ����
		CFeatureContainer::Select( it->get<1>(), listHasRFOID, typeid(CRow&), bRecycling, bCreateCache, cErrs, false, it->get<2>(), GetTableName() ); // �������͍ŏ��ɂ��Ă���̂ōŌ��false
	}

	if( bGetRelated )
	{
		CGeometry cSearchGeom;

		// 2�����b�V����m�[�h�̎��͐ڑ����Ă铹�H�ōČ�����������
		CContainer cNodeFeatures; // �Č����t�B�[�`��
		for( const_rows_iterator it = begin(emNetworkNodeTableType), itEnd = end(emNetworkNodeTableType); it != itEnd; ++it )
		{
			CRelNode* pNode = static_cast<CRelNode*>(it->get());
			if ( pNode->IsBorderNode() ) {
/* ���H�����N��ID�Ńm�[�h���������Ă���̂ŁA���H�����邩�ǂ����𒲂ׂ�K�v�Ȃ�
				long lOID = (*it)->GetOID();

				for ( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad ) {
					if ( (*itRoad)->GetLongValueByFieldName( schema::road_link::kFromNodeID, -1 ) == lOID || (*itRoad)->GetLongValueByFieldName( schema::road_link::kToNodeID, -1 ) == lOID ) */
						cNodeFeatures.push_back( *it, false );
//				}
			}
		}

		// �Č����t�B�[�`��������΁A���̃t�B�[�`���̌`��Ō�����������B
		if ( !cNodeFeatures.empty() )
		{
			IGeometryCollectionPtr ipGeomColl( CLSID_GeometryBag );

			((IGeometryPtr)ipGeomColl)->putref_SpatialReference( GetSpatialReference() );

			// �Č����̑O�̓��H�̗��[���擾
			for ( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad )
			{
				ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint(0) );
				ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 ) );
			}

			// 2�����b�V����m�[�h�̌`��ōČ���
			IGeometryPtr ipNodeGeom = cNodeFeatures.GetShape();
			ipNodeGeom->putref_SpatialReference( GetSpatialReference() );
			emErr = _Select( ipNodeGeom, bRecycling, bCreateCache, cErrs, false, false );

			// �Č����Ŏ擾�������H�̌`����擾
			for ( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad )
			{
				// 2�����b�V����m�[�h�ɐڑ����Ă��铹�H�̒[�_���擾
				for ( const_rows_iterator itNode = cNodeFeatures.begin(emNetworkNodeTableType),
					itNodeEnd = cNodeFeatures.end(emNetworkNodeTableType); itNode != itNodeEnd; ++itNode )
				{
					if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) == (*itNode)->GetOID() )
						ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint(0) );

					if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) == (*itNode)->GetOID() )
						ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 ) );
				}
			}

			// �Č����ɂ����2�����b�V����m�[�h���V���ɒǉ����ꂽ��A������x�������Ȃ���΂����Ȃ��̂ŁB
			CContainer cReNodeFeatures; // �āX�����t�B�[�`��

			for( const_rows_iterator it = begin(emNetworkNodeTableType), itEnd = end(emNetworkNodeTableType); it != itEnd; ++it )
			{
				CRelNode* pNode = static_cast<CRelNode*>(it->get());

				if ( pNode->IsBorderNode() ) {
					if ( ! cNodeFeatures.find( *(*it) ) )
						cReNodeFeatures.push_back( *it );
				}
			}

			if ( !cReNodeFeatures.empty() )
				emErr = _Select( cReNodeFeatures.GetShape(), bRecycling, bCreateCache, NULL, false, true );

			cSearchGeom = (IGeometryPtr)ipGeomColl;

			// ���͂������H
			if ( (IGeometry*)cSearchGeom && !cSearchGeom.IsEmpty() )
				emErr = _Select( cSearchGeom, bRecycling, bCreateCache, NULL, false, false );	// ���ӍČ����i�����N�[�_��p���� bug 6157�j
		}

		// �l�b�g���[�N�m�[�h�ɑ΂���O���e�[�u���̌���
		for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin(strNetworkNodeTableName),
			itEnd = g_cRuleRelationTable.end(strNetworkNodeTableName); it != itEnd; ++it )
		{
			// *_RF�t���O���t���Ă���m�[�hOID�̎��W
			std::list<long> listHasRFOID;
			long index = GetFieldMap()->FindField(it->get<0>()); // *_RF
			for( const_rows_iterator itNode = begin(emNetworkNodeTableType), itNodeEnd = end(emNetworkNodeTableType); itNode != itNodeEnd; ++itNode )
			{
				// TODO : RAILWAY_NODE��*_RF�t���O�������Ă��Ȃ����߁A���ߑł��őΉ��B�i�֘A�e�[�u�����폜���邽�߁j
				// �{���͊֘A�t���O�t�B�[���h�����󂾂�����R�s�[���Ȃ��Ƃ���ׂ��B�Ή�����RAILWAL_LINK�̕����Y�ꂸ�ɑΉ��̂��ƁB
				// ���������A"�֘A�e�[�u�������t�B�[�`���N���X�͊֘A�t�B�[���h������"�Ƃ����O��Ń��C�u�����͍\������Ă���̂ŁA
				// �֘A�e�[�u�������t�B�[���h�͊֘A�t���O�����悤�Ƀ��f���ύX����ׂ�
				if( 0 == (strNetworkNodeTableName.CompareNoCase(sindy::schema::sj::railway_node::kTableName)) ||
					1 == (*itNode)->GetLongValue( index, -1 ) )
					listHasRFOID.push_back( (*itNode)->GetOID() );
			}
			// ����
			CFeatureContainer::Select( it->get<1>(), listHasRFOID, typeid(CRow&), bRecycling, bCreateCache, cErrs, false, it->get<2>(), strNetworkNodeTableName ); // �������͍ŏ��ɂ��Ă���̂ōŌ��false
		}
	}

	return emErr;
}

// �����_�C�A���O��p
sindyErrCode CRoadNetwork::SelectByObject( _IRow* ipRow, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, bool bInit /* = true*/ )
{
	return _SelectByObject( ipRow, bRecycling, bCreateCache, bInit );
}

sindyErrCode CRoadNetwork::_SelectByObject( _IRow* ipRow, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, bool bInit /* = true*/, bool bGetRelated /* = true*/ )
{
	// �����`�F�b�N
	RETURN_IF( ! ipRow, sindyErr_ArgIsNull )

	CRow cRow( ipRow );

	// �ڑ��v���p�e�B���������ǂ������m�F
	RETURN_IF( ! ( *GetNameString() == *cRow.GetNameString() ), sindyErr_ArgLimitOver )

	// �l�b�g���[�N�֘A�I�u�W�F�N�g���ǂ�������
	// TODO: CFeatureContainer�ɈڐA����ꍇ�͂����ƃ��[��������
	// �ǂ̊֘A��K�p���ׂ����𒲂ׂ�K�v������
	RETURN_IF( ! AheIsRelateWithNetwork( cRow.GetTableName() ), sindyErr_ArgLimitOver )

	// ������
	if( bInit )
		clear();

	// �֘A�����߂̃l�b�g���[�N���������ăR���e�i�Ɋi�[
	CContainer cLinkContainer;
	// TODO: LINK_ID�𒼐ڎw�肵�Ă��邪�ACFeatureContainer�ɈڐA����ꍇ��
	// �����ƃ��[�����炽�ǂ��ăt�B�[���h�����擾����K�v������
	// 
	// �l�b�g���[�N�����N�̏ꍇ�͂��̂܂܁A
	// �l�b�g���[�N�m�[�h�̏ꍇ��OBJECTID��FROM�ATO�ŕێ����Ă�����̂������A
	// �����鑤�̏ꍇ�͊O���L�[��OBJECTID�ɂȂ���̂������A�iTODO: ID�֘A���Ȃ����̂͌`��ň����K�v������j
	// TODO: �O�����R�[�h�̏ꍇ���O���L�[��OBJECTID�ɂȂ���̂������A
	// TODO: �����N��̏ꍇ�́c
	IQueryFilterPtr ipFilter;
	if( g_cRuleModelNetwork.IsNetworkLink( cRow.GetTableName() ) )
		ipFilter = AheInitQueryFilter( ipFilter, NULL, _T("OBJECTID=%d"), cRow.GetOID() );
	else if( g_cRuleModelNetwork.IsNetworkNode( cRow.GetTableName() ) )
		ipFilter = AheInitQueryFilter( ipFilter, NULL, _T("FROM_NODE_ID=%d OR TO_NODE_ID=%d") ,cRow.GetOID(), cRow.GetOID() );
	else if( g_cRuleRelationshipContain.find_value( cRow.GetTableName() ) != g_cRuleRelationshipContain.end() )
		ipFilter = AheInitQueryFilter( ipFilter, NULL, _T("OBJECTID=%d") ,cRow.GetLongValueByFieldName(_T("LINK_ID"), -1) ); // TODO: �����ƃ��[����������ׂ��B����ƁAID�֘A���Ȃ����̂ɂ��čl������Ă��Ȃ�

	return _Select( ipFilter, bRecycling, bCreateCache, NULL, bInit, bGetRelated );
}


// �����o���N���A����
void CRoadNetwork::clear()
{
	CFeatureContainer::clear();
	m_mapLinkQueue.clear();
}

// �L���b�V������Ă��邩�ǂ������`�F�b�N����
bool CRoadNetwork::IsCached() const
{
	if( CFeatureContainer::IsCached() )
	{
		for( std::map<sindyTableType::ECode, std::list<CLinkQueue> >::const_iterator it = m_mapLinkQueue.begin(); it != m_mapLinkQueue.end(); ++it )
		{
			for( std::list<CLinkQueue>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				if( ! it2->IsCached() )
					return false;
			}
		}
		return true;
	}
	return false;
}

// �^����ꂽ�`��Ƃ̌��������ŕ����������s��
errorcode::sindyErrCode CRoadNetwork::Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

		if( pFeature )
		{
			// Split������ɕ����_��ǉ�����ƁA���ۂɕ��������_�Ƃ���Ă���\��������̂ł��������Ȃ��Ă��܂��̂ŁA
			// ��ɕ����_��ǉ����Ă����B
			CString strTableName( pSource->GetTableName() );

			if( ! AheIsNetworkNode( strTableName ) ) {
				if ( AheIsNetworkLink( strTableName ) ) {
					// ����֌W�𕪊�
					for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
						if ( (*itRoad) == pSource ) {
							for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
								itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
								itContain != itContainEnd; ++itContain )
							{
								CContainer cContains;

								for( const_rows_iterator itSeg = (*itRoad)->begin(AheTableName2TableType(itContain->first)),
									itSetEnd = (*itRoad)->end(AheTableName2TableType(itContain->first));
									itSeg != itSetEnd; ++itSeg )
									cContains.push_back( (*itSeg) );

								// ����֌W�ɕ����_��ǉ�
								for ( CGeometry::const_point_iterator itPt = cGeom.point_begin(), itPtEnd = cGeom.point_end(); itPt != itPtEnd; ++itPt ) {
									for ( CContainer::rows_iterator itSeg = cContains.begin(AheTableName2TableType(itContain->first)), itSegEnd = cContains.end(AheTableName2TableType(itContain->first)); itSeg != itSegEnd; ++itSeg ) {
										CGeometry cPtGeom = (IGeometryPtr)AheCreateClone( (IGeometry*)*itPt );
										CGeometry* pSegGeom = (*itSeg)->GetShape();

										cPtGeom.PutRefSpatialReference( pSegGeom->GetSpatialReference() );
										// �����_���Z�O�����g��ɂ��邯�Ǎ\���_���Ȃ����͒ǉ�
										if ( AhePointIsOnGeometry( (IGeometry*)(*pSegGeom), (IPointPtr)(IGeometry*)cPtGeom ) && ! pSegGeom->IntersectPoints( cPtGeom ) )
											pSegGeom->AddVertex( (IPointPtr)(IGeometry*)cPtGeom );
									}
								}

								// ����֌W�𕪊�
								for ( CContainer::rows_iterator itSeg = cContains.begin(AheTableName2TableType(itContain->first)), itSegEnd = cContains.end(AheTableName2TableType(itContain->first)); itSeg != itSegEnd; ++itSeg )
									_Split( (*itSeg), cGeom, cErrObjects );
							}

							break;
						}
					}
				} else {
					// �����_�𓹘H�Ƒ��̓���֌W�ɒǉ�
					// �������铹�H������
					CSPRow pRoad;

					for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
						// ����֌W�̂���
						for( rows_iterator itSeg = (*itRoad)->begin(pSource->GetTableType()),
							itSetEnd = (*itRoad)->end(pSource->GetTableType());
							itSeg != itSetEnd; ++itSeg )
						{
							if ( (*itSeg) == pSource ) {
								pRoad = (*itRoad);
								break;
							}
						}

						if ( pRoad )
							break;
					}

					if ( pRoad ) {
						// �����_�𓹘H�ɒǉ�
						for ( CGeometry::const_point_iterator itPt = cGeom.point_begin(), itPtEnd = cGeom.point_end(); itPt != itPtEnd; ++itPt ) {
							CGeometry cPtGeom = (IGeometryPtr)AheCreateClone( (IGeometry*)*itPt );
							CGeometry* pRoadGeom = pRoad->GetShape();

							cPtGeom.PutRefSpatialReference( pRoadGeom->GetSpatialReference() );
							// �����_���Z�O�����g��ɂ��邯�Ǎ\���_���Ȃ����͒ǉ�
							if ( AhePointIsOnGeometry( (IGeometry*)(*pRoadGeom), (IPointPtr)(IGeometry*)cPtGeom ) && ! pRoadGeom->IntersectPoints( cPtGeom ) )
								pRoadGeom->AddVertex( (IPointPtr)(IGeometry*)cPtGeom );

							// ����֌W�ɕ����_��ǉ�
							for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
								itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
								itContain != itContainEnd; ++itContain )
							{
								for( rows_iterator itSeg = pRoad->begin(AheTableName2TableType(itContain->first)),
									itSetEnd = pRoad->end(AheTableName2TableType(itContain->first));
									itSeg != itSetEnd; ++itSeg )
								{
									if ( (*itSeg) != pSource ) {
										cPtGeom = (IGeometryPtr)AheCreateClone( (IGeometry*)*itPt );
										CGeometry* pSegGeom = (*itSeg)->GetShape();

										cPtGeom.PutRefSpatialReference( pSegGeom->GetSpatialReference() );
										// �����_���Z�O�����g��ɂ��邯�Ǎ\���_���Ȃ����͒ǉ�
										if ( AhePointIsOnGeometry( (IGeometry*)(*pSegGeom), (IPointPtr)(IGeometry*)cPtGeom ) && ! pSegGeom->IntersectPoints( cPtGeom ) )
											pSegGeom->AddVertex( (IPointPtr)(IGeometry*)cPtGeom );
									}
								}
							}
						}
					}
				}
			}

			// ����
			_Split( pSource, cGeom, cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �^����ꂽ�`��Ƃ̌��������ŕ����������s��
errorcode::sindyErrCode CRoadNetwork::_Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

		if( pFeature )
		{
			CString strTableName( pSource->GetTableName() );

			if( ! AheIsNetworkNode( strTableName ) ) {
				if ( AheIsNetworkLink( strTableName ) ) {
					CGeometry cSplittedGeoms, cSplittedPoints;

					// Split
					CPoint cFromP( pFeature->GetShape()->GetPoint(0) ); // �n�_���R�s�[�ibug 7300�j
					if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
						if ( (IGeometry*)cSplittedGeoms ) {
							if ( cSplittedGeoms.GetPartCount() > 0 ) {
								CString strNetworkNodeTableName( AheGetNetworkNodeFeatureClassName( GetTableName() ) );
								sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType( strNetworkNodeTableName );

								for ( rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
									if ( (*itRoad) == pSource ) {
										CContainer cNewNodes;	// �m�[�h��ʂ̍Đݒ�Ŏg�p
										CContainer cNewRoads;	// �����N��̕����Ŏg�p
										CTable cNodeTable( (*begin(emNetworkNodeTableType))->GetTable() );

										// �����`������蓖�Ă�
										for ( CGeometry::const_part_iterator itPart = cSplittedGeoms.part_begin(), itPartEnd = cSplittedGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
											CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

											if ( (IGeometry*)cNewGeom ) {
												CSPRow pRoad;
												WKSPoint* point_wks = new WKSPoint[cNewGeom.GetPointCount()];
												((IPointCollectionPtr)(IGeometry*)cNewGeom)->QueryWKSPoints( 0, cNewGeom.GetPointCount(), point_wks );
												// �����O�̎n�_�������̂�ID���p���ΏۂƂ���ibug 7300�j
												if( cFromP.Equals( point_wks[0] ) )
												{
													// ���݂̒��g�Ɠ���ւ���
													((IPointCollectionPtr)(IGeometry*)*pFeature->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );
													pRoad = pSource;
												} 
												else {
													// �t�B�[�`���쐬
													pRoad.reset( new CRelRoad( NULL, pSource->GetTableType(), true, pSource->GetFieldMap(), pSource->GetNameString() ) );

													CTable( pSource->GetTable() ).CreateRow( *pRoad );

													// �������R�s�[
													pSource->CopyTo( *pRoad, true, false, false, false );

													// �`����Z�b�g
													((IPointCollectionPtr)(IGeometry*)*pRoad->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

													// �O���e�[�u���֌W�֘A�t��
													for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(GetTableName()),
														itOuterEnd = g_cRuleRelationTable.end(GetTableName());
														itOuter != itOuterEnd; ++itOuter )
													{
														for( rows_iterator itOneway = pSource->begin(AheTableName2TableType(itOuter->get<1>())),
															itOnewayEnd = pSource->end(AheTableName2TableType(itOuter->get<1>()));
															itOneway != itOnewayEnd; ++itOneway )
														{
															// �쐬
															CSPRow cNew;

															cNew.reset( new CRow( NULL, (*itOneway)->GetTableType(), true, (*itOneway)->GetFieldMap(), (*itOneway)->GetNameString() ) );

															CTable cTable( (*itOneway)->GetTable() );

															cTable.CreateRow( *cNew.get() );

															// �������R�s�[
															(*itOneway)->CopyTo( *cNew.get(), true, false, false, false );

															cNew->SetLongValueByFieldName( itOuter->get<2>(), pRoad->GetOID() );

															// ���H�Ɋi�[
															pRoad->push_back( cNew );
														}
													}

													// ���H���i�[
													push_back( pRoad );
													cNewRoads.push_back( pRoad );
												}
												delete [] point_wks;
												point_wks = NULL;

												// �m�[�h�����݂��Ă��邩�`�F�b�N
												bool bHasFrom = false, bHasTo = false;
												CGeometry cFront = cNewGeom.GetPoint( 0 );
												CGeometry cBack = cNewGeom.GetPoint( cNewGeom.GetPointCount() - 1 );
												CSPRow pFromNode, pToNode;

												for ( const_rows_iterator itNode = begin(emNetworkNodeTableType), itNodeEnd = end(emNetworkNodeTableType);
													itNode != itNodeEnd; ++itNode )
												{
													if ( (*itNode)->GetShape()->Equals( cFront ) ) {
														pFromNode = (*itNode);
														bHasFrom = true;
													}

													if ( (*itNode)->GetShape()->Equals( cBack ) ) {
														pToNode = (*itNode);
														bHasTo = true;
													}
												}

												if ( ! bHasFrom ) {	// FROM���̃m�[�h���Ȃ��H
													// �m�[�h�쐬
													CSPRow cNewNode;

													cNewNode.reset( new CRelNode( NULL, emNetworkNodeTableType, true ) );

													cNodeTable.CreateRow( *cNewNode.get() );
													cNewNode->SetShape( cFront );	// �`��Z�b�g

													// �L���b�V���쐬
													cNewNode->CreateCache();

													if ( pRoad->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) != cNewNode->GetOID() )
														// ���H��FROM_NODE_ID�Z�b�g
														pRoad->SetLongValueByFieldName(schema::road_link::kFromNodeID, cNewNode->GetOID() );

													// �ǉ�
													push_back( cNewNode );
													cNewNodes.push_back( cNewNode );
												} else {
													// ����ꍇ�ł��ANODE_ID�������Ă��Ȃ���������̂�
													if ( pRoad->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) != pFromNode->GetOID() )
														// ���H��FROM_NODE_ID�Z�b�g
														pRoad->SetLongValueByFieldName(schema::road_link::kFromNodeID, pFromNode->GetOID() );
												}

												if ( ! bHasTo ) {	// TO���̃m�[�h���Ȃ��H
													// �m�[�h�쐬
													CSPRow cNewNode;

													cNewNode.reset( new CRelNode( NULL, emNetworkNodeTableType, true ) );

													cNodeTable.CreateRow( *cNewNode.get() );
													cNewNode->SetShape( cBack );	// �`��Z�b�g

													// �L���b�V���쐬
													cNewNode->CreateCache();

													if ( pRoad->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) != cNewNode->GetOID() )
														// ���H��FROM_NODE_ID�Z�b�g
														pRoad->SetLongValueByFieldName(schema::road_link::kToNodeID, cNewNode->GetOID() );

													// �ǉ�
													push_back( cNewNode );
													cNewNodes.push_back( cNewNode );
												} else {
													// ����ꍇ�ł��ANODE_ID�������Ă��Ȃ���������̂�
													if ( pRoad->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) != pToNode->GetOID() )
														// ���H��TO_NODE_ID�Z�b�g
														pRoad->SetLongValueByFieldName(schema::road_link::kToNodeID, pToNode->GetOID() );
												}
											}
										}

										// �m�[�h��ʂ̍Đݒ�
										for ( CContainer::rows_iterator itNode = cNewNodes.begin(emNetworkNodeTableType), itNodeEnd = cNewNodes.end(emNetworkNodeTableType); itNode != itNodeEnd; ++itNode )
											// �m�[�h��ʂ̍Đݒ�
											ResetNodeAttribute( (*itNode) );

										// �����N��̕���
										for ( linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs ) {
											// INF��
											for ( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq ) {
												if ( itLinkq->HasRoad( *pSource.get() ) ) {	// INF��LQ�ɓ��H������H
													if ( sindyErr_NoErr != itLinkq->Split( *pSource.get(), GetTableType(), cNewRoads, cErrObjects ) )
														cErrObjects.push_back( CErrorObject( *itLinkq ) );
												}
											}
										}

										break;
									}
								}

								// ����֌W�֘A�t��
								for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
									itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
									itContain != itContainEnd; ++itContain )
								{
									CContainer cContains;

									for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
										// �ύX������������֌W�擾
										for( rows_iterator itSeg = (*itRoad)->begin(AheTableName2TableType(itContain->first)),
											itSetEnd = (*itRoad)->end(AheTableName2TableType(itContain->first));
											itSeg != itSetEnd; ++itSeg )
										{
											schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

											if ( (*itSeg)->GetLongValueByFieldName( itContain->second, -1 ) == pSource->GetOID() ||
												( (*itSeg)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) )
												cContains.push_back( (*itSeg) );
										}
									}

									// �֘A�t��
									for ( CContainer::const_rows_iterator itSeg = cContains.begin(AheTableName2TableType(itContain->first)), itSegEnd = cContains.end(AheTableName2TableType(itContain->first)); itSeg != itSegEnd; ++itSeg ) {
										// [Bug 6076]���H�����N�̃Z�O�����g�폜���Ƀ����N���������폜����Ȃ��ꍇ������
										// �폜�������̈ȊO�̊֘A�t����ύX����悤�ɂ��Ȃ��ƁA�������Ȃ����̂��łĂ��Ă��܂��B
										if ( ! (*itSeg)->IsDeleted() ) {
											for ( rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
												if ( (*itRoad)->GetShape()->Contains( (*itSeg)->GetShapeCopy() ) || (*itRoad)->GetShape()->Equals( (*itSeg)->GetShapeCopy() ) ) {
													// LINK_ID�ݒ�
													(*itSeg)->SetLongValueByFieldName( itContain->second, (*itRoad)->GetOID() );

													// ���H�Ɋi�[
													(*itRoad)->push_back( (*itSeg) );
												} else
													// �������铹�H���ς�����̂ŏ��O
													(*itRoad)->erase( (*itSeg) );
											}
										}
									}
								}
								// ����Ȃ��Ȃ����m�[�h���폜�i�[�_�m�[�h�j
								
								
								std::vector<long> listNode;
								std::set<long> listFromTo;
								for ( rows_iterator it = begin(emNetworkNodeTableType); it != end(emNetworkNodeTableType); ++it ) {
									listNode.push_back( (*it)->GetOID() );
								}
								
								for ( rows_iterator it = begin(GetTableType()); it != end(GetTableType()); ++it ) {
									listFromTo.insert( (*it)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1));
									listFromTo.insert( (*it)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1));
								}
								std::vector<long> listDelete;
								//std::set_difference(listNode.begin(), listNode.end(),listFromTo.begin(), listFromTo.end(), back_inserter(listDelete));
								for(std::vector<long>::iterator it=listNode.begin();it!=listNode.end();it++){
									if(listFromTo.find(*it)==listFromTo.end())
										listDelete.push_back(*it);
								}

								for ( rows_iterator it = begin(emNetworkNodeTableType); it != end(emNetworkNodeTableType); ++it ) {
									long oid = (*it)->GetOID();
									for( std::vector<long>::iterator itOID = listDelete.begin();itOID!=listDelete.end();++itOID){
										if(oid==*itOID){
											(*it)->Delete();
											listDelete.erase(itOID);
											break;
										}
									}
								}
								
								
								
								// ����Ȃ��Ȃ����m�[�h���폜�i�[�_�m�[�h�j
								/*
								std::cerr << size(emNetworkNodeTableType) << std::endl;
								for ( rows_iterator itNode = begin(emNetworkNodeTableType); itNode != end(emNetworkNodeTableType); ++itNode ) {
									bool bIsFind = false;

									for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
										if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) == (*itNode)->GetOID() ||
											(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) == (*itNode)->GetOID() ) {
											bIsFind = true;
											break;
										}
									}

									if ( ! bIsFind )
										(*itNode)->Delete();
								}
								
								*/
							}
						}
					}
				} else {
					// �����_�𓹘H�Ƒ��̓���֌W�̂��̂ɒǉ�
					// �������铹�H������
					CSPRow pRoad;

					for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
						// ����֌W�̂���
						for( rows_iterator itSeg = (*itRoad)->begin(pSource->GetTableType()),
							itSetEnd = (*itRoad)->end(pSource->GetTableType());
							itSeg != itSetEnd; ++itSeg )
						{
							if ( (*itSeg) == pSource ) {
								pRoad = (*itRoad);
								break;
							}
						}

						if ( pRoad )
							break;
					}

					if ( pRoad ) {
						CGeometry cSplittedGeoms, cSplittedPoints;

						// Split
						if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
							if ( (IGeometry*)cSplittedGeoms ) {
								if ( cSplittedGeoms.GetPartCount() > 0 ) {
									// �������铹�H�ɍ쐬�t�B�[�`�����i�[
									CTable cTable( pSource->GetTable() );

									// �����`������蓖�Ă�
									for ( CGeometry::const_part_iterator itPart = cSplittedGeoms.part_begin(), itPartEnd = cSplittedGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
										CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

										if ( (IGeometry*)cNewGeom ) {
											WKSPoint* point_wks = new WKSPoint[cNewGeom.GetPointCount()];

											((IPointCollectionPtr)(IGeometry*)cNewGeom)->QueryWKSPoints( 0, cNewGeom.GetPointCount(), point_wks );

											if ( itPart == cSplittedGeoms.part_begin() ) {
												// ���݂̒��g�Ɠ���ւ���
												((IPointCollectionPtr)(IGeometry*)*pFeature->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );
											} else {
												// �t�B�[�`���쐬
												CSPRow cNew;

												cNew.reset( new CFeature( NULL, pSource->GetTableType(), true, pSource->GetFieldMap(), pSource->GetNameString() ) );

												cTable.CreateRow( *cNew.get() );

												// �������R�s�[
												pSource->CopyTo( *cNew.get(), true, false, false, false );

												// �`����Z�b�g
												((IPointCollectionPtr)(IGeometry*)*cNew->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

												// �������铹�H�ɒǉ�
												pRoad->push_back( cNew );
											}
											delete [] point_wks;
											point_wks = NULL;
										}
									}
								} else {
									// �Ȃ����Ƃ͂��肦�Ȃ��͂��c
									cErrObjects.push_back( CErrorObject( *pSource.get() ) );
								}
							}
						}
					}
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �O���e�[�u���֌W���폜
void DeleteRelation( CSPRow& row, const CString& table )
{
	// itOuter <0>:�e�̃e�[�u���� <1>:�폜�Ώۂ̃e�[�u���� <2>:<1>�̃e�[�u���ŃL�[�ƂȂ�t�B�[���h��
	for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(table), 
		itOuterEnd = g_cRuleRelationTable.end(table);
		itOuter != itOuterEnd; ++itOuter )
	{
		for( CContainer::rows_iterator itOuterObj = row->begin(AheTableName2TableType(itOuter->get<1>())),
			itOuterObjEnd = row->end(AheTableName2TableType(itOuter->get<1>()));
			itOuterObj != itOuterObjEnd; ++itOuterObj )
		{
			// �ċA�ł���Ɋ֘A�Â��Ă�����̂��S�ď���
			DeleteRelation( (*itOuterObj), itOuter->get<1>() );
			(*itOuterObj)->Delete();
		}
	}
}
errorcode::sindyErrCode CRoadNetwork::Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource && pDest, sindyErr_ArgIsNull )
	{
		if( *pSource->GetNameString() == *pDest->GetNameString() && AheIsNetworkLink( pSource->GetTableName() ) )	// �����^�C�v�H
		{
			CString strNetworkNodeFC = AheGetNetworkNodeFeatureClassName(GetTableName());
			sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType(strNetworkNodeFC); // �l�b�g���[�N�m�[�h�^�C�v

			// �}�[�W�擹�H�ɂ���K�����֘A�t�B�[�`����LINK_ID��ύX
			//const_rows_iterator itRoad = find( GetTableType(), pDest->GetOID() );

			//if ( itRoad != end(GetTableType()) ) {
				// ����֌W
				for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()), 
					itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
					itContain != itContainEnd; ++itContain )
				{
					CContainer cContains;

					for( rows_iterator itContainFeat = pDest->begin(AheTableName2TableType(itContain->first)), 
						itContainFeatEnd = pDest->end(AheTableName2TableType(itContain->first));
						itContainFeat != itContainFeatEnd; ++itContainFeat )
					{
						if( sindyErr_NoErr != (*itContainFeat)->SetLongValueByFieldName( itContain->second, pSource->GetOID() ) )
							cErrObjects.push_back( CErrorObject( *itContainFeat->get() ) );

						cContains.push_back( (*itContainFeat) );
					}

					// [Bug 5949] ����������ݒ肪�Ȃ��Ȃ���
					// ������ύX���Ă��Ȃ��Ɠ���֌W�t�B�[�`���̍\���_�폜���ł��Ȃ��̂ŁB
					// �������铹�H���ς�����̂Ń}�[�W�����珜�O���Ă���
					for ( CContainer::const_rows_iterator itSeg = cContains.begin(AheTableName2TableType(itContain->first)), 
						itSegEnd = cContains.end(AheTableName2TableType(itContain->first));
						itSeg != itSegEnd; ++itSeg )
					{
						// �}�[�W��ɏ����ύX
						pSource->push_back( (*itSeg) );

						pDest->erase( (*itSeg) );
					}
				}
				// �O���e�[�u���֌W���폜
				DeleteRelation( pDest, GetTableName() );
			//}

			if ( cErrObjects.empty() ) {
				// From_Node_ID / To_Node_ID�̕ύX
				long nSourceFromID = pSource->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1);
				long nSourceToID = pSource->GetLongValueByFieldName(schema::road_link::kToNodeID, -1);
				long nDestFromID = pDest->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1);
				long nDestToID = pDest->GetLongValueByFieldName(schema::road_link::kToNodeID, -1);
				bool bIsReverse = false;	// �`��𔽓]���邩�ǂ���
				long nDelNodeID = -1;	// �폜�m�[�hID
				long nChkNodeID = -1;	// �����`�F�b�N�m�[�hID

				if ( nSourceFromID == nDestFromID ) {			// T---FF---T
					nDelNodeID = nSourceFromID;

					if ( sindyErr_NoErr != pSource->SetLongValueByFieldName(schema::road_link::kFromNodeID, nDestToID) )
						cErrObjects.push_back( CErrorObject( (*pSource) ) );

					nChkNodeID = nSourceFromID;
					bIsReverse = true;
				} else if ( nSourceFromID == nDestToID ) {	// T---FT---F
					nDelNodeID = nSourceFromID;

					if ( sindyErr_NoErr != pSource->SetLongValueByFieldName(schema::road_link::kFromNodeID, nDestFromID) )
						cErrObjects.push_back( CErrorObject( (*pSource) ) );

					nChkNodeID = nSourceFromID;
				} else if ( nSourceToID == nDestFromID ) {	// F---TF---T
					nDelNodeID = nSourceToID;

					if ( sindyErr_NoErr != pSource->SetLongValueByFieldName(schema::road_link::kToNodeID, nDestToID) )
						cErrObjects.push_back( CErrorObject( (*pSource) ) );

					nChkNodeID = nSourceToID;
				} else if ( nSourceToID == nDestToID ) {		// F---TT---F
					nDelNodeID = nSourceToID;

					if ( sindyErr_NoErr != pSource->SetLongValueByFieldName(schema::road_link::kToNodeID, nDestFromID) )
						cErrObjects.push_back( CErrorObject( (*pSource) ) );

					nChkNodeID = nSourceToID;
					bIsReverse = true;
				} else {
					cErrObjects.push_back( CErrorObject( (*pSource) ) );
					cErrObjects.push_back( CErrorObject( (*pDest) ) );
				}

				if ( cErrObjects.empty() ) {
					CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

					// �`��̃}�[�W
					if ( sindyErr_NoErr == pFeature->Merge( *pDest->GetShape(), cErrObjects ) ) {
						// �m�[�h�̍폜
						rows_iterator itNode = find( emNetworkNodeTableType, nDelNodeID );

						if ( itNode != end(emNetworkNodeTableType) )
						{
							(*itNode)->Delete();
							DeleteRelation( *itNode, strNetworkNodeFC );
						}

						if ( cErrObjects.empty() ) {
							// �����N��̃}�[�W
							for ( const_linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs ) {
								bool bEdit = false;

								// INF��
								for ( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq ) {
									if ( ! itLinkq->IsDeleted() ) {
										itLinkq->Merge( (*pSource), (*pDest), bIsReverse, GetTableType(), *this, cErrObjects );

										// �}�[�W����LQ�������LQRF�̃t���O�𗧂Ă�
										for ( CLinkQueue::iterator itLQ = (*itLinkq).begin(); itLQ != (*itLinkq).end(); ++itLQ ) {
											if ( (*itLQ).GetLinkId() == pSource->GetOID() ) {
												// �}�[�W����LQRF�̃t���O�𗧂Ă�
												for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
													if ( itLinkqs->first == AheTableName2TableType(itInf->get<0>()) ) {
														if ( sindyErr_NoErr != pSource->SetBoolValueByFieldName(itInf->get<2>(), true) )
															cErrObjects.push_back( CErrorObject( (*pSource) ) );
														break;
													}
												}

												break;
											}
										}

										bEdit = true;
									}
								}

								if ( bEdit )
									ResetRoadLQRF( itLinkqs->first, cErrObjects );	// ���H��LQRF�Đݒ�
							}

							if ( cErrObjects.empty() ) {
								// �}�[�W�擹�H�̍폜
								pDest->Delete();

								if ( cErrObjects.empty() ) {
									// �m�[�h��ʂ̍Đݒ�
									itNode = find( emNetworkNodeTableType, nChkNodeID );

									if ( itNode != end(emNetworkNodeTableType) )
										ResetNodeAttribute( (*itNode) );
								}
							}
						}
					}
				}
			}
		} else {
			cErrObjects.push_back( CErrorObject( (*pSource) ) );
			cErrObjects.push_back( CErrorObject( (*pDest) ) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects, bool bEraseOnly/*=false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

		if ( pFeature ) {
			CGeometry cIntersect = pFeature->GetShape()->IntersectPoints( cGeom );

			if( (IGeometry*)cIntersect && ! cIntersect.IsEmpty() )
			{ 
				if( AheIsNetworkLink( pRow->GetTableName() ) )
				{
					// ����֌W�̂���
					for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
						if ( (*itRoad) == pRow ) {
							for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
								itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
								itContain != itContainEnd; ++itContain )
							{
								for( rows_iterator itSeg = (*itRoad)->begin(AheTableName2TableType(itContain->first)),
									itSetEnd = (*itRoad)->end(AheTableName2TableType(itContain->first));
									itSeg != itSetEnd; ++itSeg )
								{
									if ( sindyErr_NoErr != Erase( *itSeg, cIntersect, cErrObjects ) )
										break;
								}
							}

							break;
						}
					}

					// �n�I�_�ȊO�̓_���폜
					CGeometry cFront = pFeature->GetShape()->GetPoint( 0 );
					CGeometry cBack = pFeature->GetShape()->GetPoint( pFeature->GetShape()->GetPointCount() - 1 );
					bool bHasFront = false, bHasBack = false;

					for ( CGeometry::point_iterator it = cIntersect.point_begin(); it != cIntersect.point_end(); ++it ) {
						if ( cFront.Equals( *it ) )
							bHasFront = true;
						else if ( cBack.Equals( *it ) )
							bHasBack = true;
						else
							pFeature->GetShape()->EraseVertex( *it );
					}

					if ( ! bHasFront && ! bHasBack ) {
						if ( pFeature->GetShape()->GetPointCount() <= 1 )
							Delete( pRow, cErrObjects );
					} else {
						CGeometry cBefore( (IGeometryPtr)AheCreateClone( pFeature->GetShapeCopy() ) );	// ���������O�̌`��ێ�
						CGeometry cFirst, cEnd;
						long nCnt = pFeature->GetShape()->GetPointCount() - 1, i = 1;

						for ( CGeometry::point_iterator it = pFeature->GetShape()->point_begin(); it != pFeature->GetShape()->point_end(); ++it, ++i ) {
							if ( bHasFront ) {
								if ( ! (IGeometry*)cFirst && i == 2 )
									cFirst = (IGeometryPtr)AheCreateClone( *it );	// �n�_�̎��̓_
							}

							if ( bHasBack ) {
								if ( ! (IGeometry*)cEnd && nCnt == i )
									cEnd = (IGeometryPtr)AheCreateClone( *it );	// �I�_�̑O�̓_
							}
						}

						// �폜�����n�I�_���̃m�[�h��2�����b�V���m�[�h�Ȃ�A
						// ���Α��̓��H�̎n�I�_���폜����B
						CListLong listNodeIDs;

						if ( bHasFront )
							listNodeIDs.push_back( pFeature->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) );

						if ( bHasBack )
							listNodeIDs.push_back( pFeature->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) );

						std::map< long, CContainer > mapRoads;	// map<�m�[�hID,list<���H>>

						for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
							itRoad != itRoadEnd; ++itRoad )
						{
							if( pRow != *itRoad )
							{
								mapRoads[(*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1)].push_back( *itRoad );
								mapRoads[(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1)].push_back( *itRoad );
							}
						}

						// 2�����b�V���m�[�h���o
						sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType(g_cRuleModelNetwork.GetPairName(GetTableName())); // �m�[�h�e�[�u���^�C�v
						CContainer cEdgeNodes;

						for ( CListLong::const_iterator itIDs = listNodeIDs.begin(), itIDsEnd = listNodeIDs.end(); itIDs != itIDsEnd; ++itIDs ) {
							rows_iterator itNode = end(emNetworkNodeTableType);

							if ( end(emNetworkNodeTableType) != (itNode = find( emNetworkNodeTableType, (*itIDs) )) ) {
								CRelNode* pNode = static_cast<CRelNode*>(itNode->get());

								// 2�����b�V����m�[�h�ɐڑ����Ă��铹�H�̍폜
								if ( pNode->IsBorderNode() ) {
									if ( mapRoads.find( pNode->GetOID() ) != mapRoads.end() )
										cEdgeNodes.push_back( *itNode );
								}
							}
						}

						if ( bHasFront ) {
							// �n�_�̎��̓_�ŕ���
							if ( sindyErr_NoErr == Split( pRow, cFirst, cErrObjects ) ) {
								CContainer cRoads;

								// �����_�������̂𒊏o(2�����Ȃ��͂�)
								for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
									itRoad != itRoadEnd; ++itRoad )
								{
									schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

									if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) {
										if ( (*itRoad)->GetShape()->IntersectPoints( cFirst ) )
											cRoads.push_back( (*itRoad) );
									}
								}

								// ���������O�̌`��̎n�_�������̂��I���W�i�����`�F�b�N
								// �쐬���ꂽ���Ȃ�I���W�i���Ɠ���ւ�
								bool bIsSwap = false;
								CSPRow cOrgRoad, cNewRoad;

								for( const_rows_iterator itRoad = cRoads.begin(GetTableType()), itRoadEnd = cRoads.end(GetTableType());
									itRoad != itRoadEnd; ++itRoad )
								{
									if ( (*itRoad)->GetShape()->GetPoint( 0 ).Equals( cBefore.GetPoint( 0 ) ) ) {
										schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

										if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kCreated ) {
											cOrgRoad = (*itRoad);
											bIsSwap = true;
										} else
											cNewRoad = (*itRoad);
									} else
										cNewRoad = (*itRoad);
								}

								if ( cNewRoad ) {
									if ( bIsSwap )
										// ���g�����ւ�
										swap( cOrgRoad, cNewRoad );

									// �����łł������H���폜
									CContainer listexclusions;

									listexclusions.push_back( cNewRoad );

									_Delete( cNewRoad, listexclusions, false, cErrObjects );
								} else {
									// �����łł��Ȃ��������i�\���_��2�_�����Ȃ����j�́A�I���W�i�����폜
									CContainer listexclusions;

									listexclusions.push_back( pRow );

									_Delete( pRow, listexclusions, false, cErrObjects );
								}
							}
						}

						if ( bHasBack ) {
							// �I�_�̑O�̓_�ŕ���
							if ( sindyErr_NoErr == Split( pRow, cEnd, cErrObjects ) ) {
								CContainer cRoads;

								// �����_�������̂𒊏o(2�����Ȃ��͂�)
								for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
									itRoad != itRoadEnd; ++itRoad )
								{
									schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

									if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) {
										if ( (*itRoad)->GetShape()->IntersectPoints( cEnd ) )
											cRoads.push_back( (*itRoad) );
									}
								}

								// ���������O�̌`��̎n�_�������̂��I���W�i�����`�F�b�N
								// �쐬���ꂽ���Ȃ�I���W�i���Ɠ���ւ�
								bool bIsSwap = false;
								CSPRow cOrgRoad, cNewRoad;

								for( const_rows_iterator itRoad = cRoads.begin(GetTableType()), itRoadEnd = cRoads.end(GetTableType());
									itRoad != itRoadEnd; ++itRoad )
								{
									if ( (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 ).Equals( cBefore.GetPoint( cBefore.GetPointCount() - 1 ) ) ) {
										schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

										if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kCreated ) {
											cOrgRoad = (*itRoad);
											bIsSwap = true;
										} else
											cNewRoad = (*itRoad);
									} else
										cNewRoad = (*itRoad);
								}

								if ( cNewRoad ) {
									if ( bIsSwap )
										// ���g�����ւ�
										swap( cOrgRoad, cNewRoad );

									// �����łł������H���폜
									CContainer listexclusions;

									listexclusions.push_back( cNewRoad );

									_Delete( cNewRoad, listexclusions, false, cErrObjects );
								} else {
									// �����łł��Ȃ��������i�\���_��2�_�����Ȃ����j�́A�I���W�i�����폜
									CContainer listexclusions;

									listexclusions.push_back( pRow );

									_Delete( pRow, listexclusions, false, cErrObjects );
								}
							}
						}

						// 2�����b�V���m�[�h�ɐڑ����锽�Α��̓��H�̍폜
						for ( CContainer::rows_iterator itNode = cEdgeNodes.begin(emNetworkNodeTableType); itNode != cEdgeNodes.end(emNetworkNodeTableType); ++itNode ) {
							for ( CContainer::rows_iterator itRoad = mapRoads[(*itNode)->GetOID()].begin(GetTableType()); itRoad != mapRoads[(*itNode)->GetOID()].end(GetTableType()); ++itRoad ) {
								if ( ! (*itRoad)->Changed() )
									Erase( *itRoad, cIntersect, cErrObjects );
							}
						}
					}
				}
				// AheIsNetwork()�ȊO
				else {
					// ����֌W���̂̍\���_�폜
					// �}���`�|�C���g�̏ꍇ������̂ŁA�������č폜
					for( long i=0; i<cIntersect.GetPointCount(); ++i )
						deleteContainVertex( pRow, (CGeometry)cIntersect.GetPoint(i), cErrObjects );
				}
			}
		}
		else
			cErrObjects.push_back( CErrorObject( (*pRow) ) );
	}

	return cErrObjects.GetDefaultErrCode();
}

bool CRoadNetwork::isOnNodePoint( const CGeometry& geom, bool onlyMeshBorder /*= false*/ )
{
	// �����Ă�m�[�h������
	sindyTableType::ECode nodeType = AheTableName2TableType(g_cRuleModelNetwork.GetPairName(GetTableName()));
	CSPRow onBorderNode;
	for( auto itNode = begin(nodeType), itEnd = end(nodeType);
		 itNode != itEnd; ++itNode )
	{
		CRelNode* pNode = static_cast<CRelNode*>(itNode->get());

		// �񎟃��b�V�����E����
		if( onlyMeshBorder && !pNode->IsBorderNode() )
			continue;

		// geom�́i�}���`�j�|�C���g�O��
		if( !geom.Disjoint(*pNode->GetShape()) )
			return true;
	}

	return false;
}


void CRoadNetwork::deleteContainVertex( CSPRow& containRel, const CGeometry& delVertex, CErrorObjects& cErrObjects )
{
	CFeature* pFeature = dynamic_cast<CFeature*>( containRel.get() );

	if( !pFeature || pFeature->IsDeleted() )
		return;

    // ���ɍ폜����Ă���ꍇ������
	if( delVertex.Disjoint( (IGeometry*)(*pFeature->GetShape()) ) )
		return;

	// �����̍\���_���폜
	// �\���_�폜�̌��ʁA�������g���Ȃ��Ȃ������ǂ�����CFeature::Erase()�ł��܂����Ƃ���Ă�̂ł����ł͋C�ɂ��Ȃ�
	pFeature->Erase( delVertex, cErrObjects );

	// �e�ƂȂ郊���N������폜
	deleteSharedVertex( getParentLink( containRel ), delVertex );

	// �������g�����������A2�����b�V����̍\���_�������ꂽ��A���Α�������
	if( pFeature->IsDeleted() || isOnNodePoint( delVertex, true ) )
	{
		// ���Α��͍폜�O�̌`��ŒT��
		CSPRow opposite = getContainOppositeMesh( containRel, true );
		if( opposite )
		{
			opposite->Trace();
			// ���Α��͑S�폜
			deleteContainRelation( opposite );
		}
	}
}

void CRoadNetwork::deleteContainRelation( CSPRow& containLine )
{
	if( containLine->IsDeleted() )
		return;

	// �Ƃ肠��������������
	containLine->Delete();

	// 2�����b�V�����E�������甽�Α���
	CSPRow opposite = getContainOppositeMesh( containLine );
	if( opposite )
		deleteContainRelation( opposite);
}

void CRoadNetwork::deleteSharedVertex( CSPRow& link, const CGeometry& vertices )
{
	if( !link || link->IsDeleted() )
		return;

	// overlap �̍\���_�ƈ�v���� target �̍\���_���폜���郉���_��
	auto deleteVertex = [&]( CSPRow& target, const CGeometry& overlap )
	{
		if( target->IsDeleted() )
			return;

		target->Trace();

		long pointNum = overlap.GetPointCount();
		for( long i=0; i<pointNum; ++i)
		{
			CPoint vertex = overlap.GetPoint(i);
			
			// �\���_������Ώۂɂ���̂Ńm�[�h�i�����N�̎n�I�_�j�͏��O
			// �܂��A������̂��m�[�h�Őڂ���ꍇ�A�Е��������������ꍇ������̂ő��̂͏����Ȃ�
			// �����������̍폜��2�����b�V�����E�]�X�͌Ăяo�����ideleteContainVertex�j�Ŗʓ|������
			if( isOnNodePoint( (CGeometry)vertex ) )
				continue;

			// �\���_�폜
			target->GetShape()->EraseVertex( vertex );
		}
	};

	// ���H�����N�̋��L�\���_�폜
	deleteVertex( link, vertices );

	// ����֌W�̂��̂�����S�č폜
	for( auto itContain = g_cRuleRelationshipContain.begin(GetTableName()),
		itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
		itContain != itContainEnd; ++itContain )
	{
		for( auto itSeg = link->begin(AheTableName2TableType(itContain->first)),
			itSetEnd = link->end(AheTableName2TableType(itContain->first));
			itSeg != itSetEnd; ++itSeg )
		{
			deleteVertex( *itSeg, vertices );

			// ���L�\���_�Ƃ��č폜���ꂽ���ʎ����������邱�Ƃ�����̂ŁA
			// ���̏ꍇ�͓񎟃��b�V�����l�����Ȃ���΂Ȃ�Ȃ�
			if( (*itSeg)->GetShape()->GetPointCount() == 1 )
				(*itSeg)->Delete();

			if( !(*itSeg)->IsDeleted() )
				continue;

			CSPRow opposite = getContainOppositeMesh( *itSeg, true );
			if( opposite )
				deleteContainRelation(opposite);
		}
	}
}

CSPRow CRoadNetwork::getParentLink( const CSPRow& containRel )
{
	// �e����
	CSPRow pRoad;

	for ( auto itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
		// ����֌W�̂���
		for( auto itContain = g_cRuleRelationshipContain.begin(GetTableName()),
			itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
			itContain != itContainEnd; ++itContain )
		{
			for( auto itSeg = (*itRoad)->begin(AheTableName2TableType(itContain->first)),
				itSetEnd = (*itRoad)->end(AheTableName2TableType(itContain->first));
				itSeg != itSetEnd; ++itSeg )
			{
				if ( containRel == *itSeg ) {
					pRoad = *itRoad;
					break;
				}
			}
		}
		if ( pRoad )
			break;
	}

	return pRoad;
}

CSPRow CRoadNetwork::getContainOppositeMesh( const CSPRow& row, bool useOrgShape /*= false*/  )
{
	// �擾�Ώۂ̌`��
	const CGeometry geom = useOrgShape ? CGeometry(row->GetShapeCopy( false, true )) : *row->GetShape();

	// �n�I�_���擾
	CGeometry fromP = geom.GetPoint(0);
	CGeometry toP = geom.GetPoint( geom.GetPointCount()-1 );

	// �����Ă�m�[�h������
	sindyTableType::ECode nodeType = AheTableName2TableType(g_cRuleModelNetwork.GetPairName(GetTableName()));
	CSPRow onBorderNode;
	for( auto itNode = begin(nodeType), itEnd = end(nodeType);
		 itNode != itEnd; ++itNode )
	{
		CRelNode* pNode = static_cast<CRelNode*>(itNode->get());
		if( !pNode->IsBorderNode() )
			continue;

		// �ڂ��Ă���Q�����b�V�����E�m�[�h������Ύ擾
		if( fromP.Equals(*pNode->GetShape()) || toP.Equals(*pNode->GetShape()) )
		{
			onBorderNode = *itNode;
			break;
		}
	}

	// 2�����b�V�����E�Ɩ����Ȃ�null�̂܂ܕԂ��ďI��
	if( !onBorderNode )
		return 	onBorderNode;


	// �e�̃����N����
	CSPRow parentLink = getParentLink( row );

	// ���Α��̃����N��T��
	CSPRow oppositeLink;
	for( auto itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
		 itRoad != itRoadEnd; ++itRoad )
	{
		const CGeometry* roadGeom = (*itRoad)->GetShape();
		// 2�����b�V���m�[�h�ɐڂ�����̂̂���
		if( onBorderNode->GetShape()->Disjoint( *roadGeom ) )
			continue;

		// �������g����Ȃ��̂����ړ��āi���Α��j�̑���
		if( *itRoad != parentLink )
		{
			// ����͕K��1�����̂͂��Ȃ̂ŁA�������甲����
			oppositeLink = *itRoad;
			break;
		}
	}

	// ���ɏ����Ă���ꍇ������
	if( !oppositeLink )
		return oppositeLink;

	// ������ނ̓�����̂�T��
	CSPRow retRow;
	auto tblType = row->GetTableType();
	for( auto itSeg = oppositeLink->begin(tblType), itSetEnd = oppositeLink->end(tblType);
		 itSeg != itSetEnd; ++itSeg )
	{
		// 2�����b�V���ɐڂ��Ă��Ȃ����̂��܂܂��ibug 10557�j
		if( (*itSeg)->GetShape()->Disjoint( geom ) )
			continue;

		// �����N�������͏d�Ȃ��Ă���ꍇ������̂Ŏ�ʂ܂Ō���K�v������ibug 11689�j
		if( (*itSeg)->GetTableType() == sindyTableType::segment_attr )
		{
			if( (*itSeg)->GetLongValueByFieldName( schema::segment_attr::kLinkAttr, -1L ) == 
				row->GetLongValueByFieldName( schema::segment_attr::kLinkAttr, -1L ) )
				retRow = *itSeg;
		}
		else
			retRow = *itSeg;
	}
	return retRow;
}

errorcode::sindyErrCode CRoadNetwork::Delete( CSPRow pRow, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CContainer listexclusions;

		listexclusions.push_back( pRow );

		emErr = _Delete( pRow, listexclusions, true, cErrObjects );
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::_Delete( CSPRow pRow, CContainer& listexclusions, bool bDelEdgeNode, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

		if( pFeature )
		{
			CString strTableName( pRow->GetTableName() );
			if( AheIsNetworkLink( strTableName ) )
			{
				// �폜�����N�ɂ���֘A�I�u�W�F�N�g�̍폜
				//rows_iterator itRoad = find( GetTableType(), pRow->GetOID() );

				//if ( itRoad != end(GetTableType()) )
				//{
					// ����֌W�̂���
					for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
						itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
						itContain != itContainEnd; ++itContain )
					{
						for( rows_iterator itSeg = pRow->begin(AheTableName2TableType(itContain->first)),
							itSegEnd = pRow->end(AheTableName2TableType(itContain->first));
							itSeg != itSegEnd; ++itSeg )
							(*itSeg)->Delete();
					}

					// �O���e�[�u���֌W���폜
					DeleteRelation( pRow, GetTableName() );
				//}

				// �����N��̍폜
				if ( sindyErr_NoErr != _DeleteLQ( pRow, cErrObjects ) )
					cErrObjects.push_back( (*pRow) );

				// ���H�̍폜
				pRow->Delete();

				// FROM�ATO����m�[�hID��쐬
				CListLong listNodeIDs;

				listNodeIDs.push_back( pRow->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) );
				listNodeIDs.push_back( pRow->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) );

				std::map< long, std::list<CSPRow> > mapRoads;	// map<�m�[�hID,list<���H>>

				for( rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
					itRoad != itRoadEnd; ++itRoad )
				{
					if( pRow != *itRoad )
					{
						mapRoads[(*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1)].push_back( *itRoad );
						mapRoads[(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1)].push_back( *itRoad );
					}
				}

				sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType(g_cRuleModelNetwork.GetPairName(GetTableName())); // �m�[�h�e�[�u���^�C�v

				if ( bDelEdgeNode ) {
					for ( CListLong::const_iterator itIDs = listNodeIDs.begin(), itIDsEnd = listNodeIDs.end(); itIDs != itIDsEnd; ++itIDs ) {
						rows_iterator itNode = end(emNetworkNodeTableType);

						if ( end(emNetworkNodeTableType) != (itNode = find( emNetworkNodeTableType, (*itIDs) )) ) {
							CRelNode* pNode = static_cast<CRelNode*>(itNode->get());

							// 2�����b�V����m�[�h�ɐڑ����Ă��铹�H�̍폜
							if ( pNode->IsBorderNode() ) {
								if ( mapRoads.find( pNode->GetOID() ) != mapRoads.end() ) {
									for ( std::list<CSPRow>::const_iterator itRoad = mapRoads[pNode->GetOID()].begin(); itRoad != mapRoads[pNode->GetOID()].end(); ++itRoad ) {
										if ( ! listexclusions.find( *(*itRoad).get() ) ) {
											// ���H�̍폜
											listexclusions.push_back( (*itRoad) );
											if ( sindyErr_NoErr != _Delete( (*itRoad), listexclusions, true, cErrObjects ) )
												break;
										}
									}
								}
							}
						}
					}
				}

				// �m�[�h�̍폜
				for ( rows_iterator itNode = begin(emNetworkNodeTableType), itNodeEnd = end(emNetworkNodeTableType);
					itNode != itNodeEnd; ++itNode )
				{
					long nConNum = mapRoads[(*itNode)->GetOID()].size(), nDelConNum = 0;

					// �ڑ����Ă��铹�H���P�{���Ȃ����͍폜
					for ( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
						itRoad != itRoadEnd; ++itRoad )
					{
						if( pRow != *itRoad ) {
							if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) == (*itNode)->GetOID() || 
								(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) == (*itNode)->GetOID() ) {
								schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

								if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType == schema::ipc_table::update_type::kDeleted )
									++nDelConNum;
							}
						}
					}

					if ( nDelConNum == nConNum ) {
						// �폜
						if ( sindyErr_NoErr != _Delete( *itNode, listexclusions, true, cErrObjects ) )
							break;
					} else {
						// �폜�������H�Ɋ֌W����m�[�h�̎�ʕύX
						if ( pRow->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) == (*itNode)->GetOID() || pRow->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) == (*itNode)->GetOID() )
							ResetNodeAttribute( *itNode );
					}
				}
			}
			else if( AheIsNetworkNode( strTableName ) )
			{
				// �ڑ����Ă��铹�H�폜
				for ( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
					itRoad != itRoadEnd; ++itRoad )
				{
					if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) == pRow->GetOID() || 
						(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) == pRow->GetOID() ) {
						schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

						if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
							if ( sindyErr_NoErr != _Delete( *itRoad, listexclusions, true, cErrObjects ) )
								break;
						}
					}
				}

				// �O���e�[�u���֌W���폜
				DeleteRelation( pRow, strTableName );

				pRow->Delete();
			}
			else {
				// ����֌W�̂��̂̏ꍇ
				if( AheIsRelateWithNetwork( strTableName ) )
				{
					deleteContainRelation( pRow );
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �ҏW���e��ۑ�����
errorcode::sindyErrCode CRoadNetwork::Store( CErrorObjects& cErrObjects )
{
	// �l�b�g���[�N���f���Ɠ���֌W���f��
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator itRoad = it->second.begin(); itRoad != it->second.end(); ++itRoad )
			(*itRoad)->Store( cErrObjects );
	}
	// �����N��
	for( linkqs_iterator it = linkqs_begin(); it != linkqs_end(); ++it )
	{
		for( linkq_iterator itLinkQueue = it->second.begin(); itLinkQueue != it->second.end(); ++itLinkQueue )
			itLinkQueue->Store( cErrObjects );
	}
	return cErrObjects.GetDefaultErrCode();
}

// ���������`�F�b�N����
errorcode::sindyErrCode CRoadNetwork::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType(AheGetNetworkNodeFeatureClassName(GetTableName())); // �m�[�h�e�[�u���^�C�v
	CListLong listNodeIDs;	// �m�[�h�`�F�b�N�p

	// �l�b�g���[�N�����N��̂̕����`�F�b�N
	std::map<long,std::vector<CSPRow>> mapRoadsByNodeID; // �m�[�hID���L�[�Ƃ������H���X�g�i2���H�p�x�`�F�b�N�Ŏg�p�j
	for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
		itRoad != itRoadEnd; ++itRoad )
	{
		schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

		(*itRoad)->Changed( &emUpdateType );

		// �폜����Ă��Ȃ����̂̂݃`�F�b�N
		if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
			// �l�b�g���[�N�����N�̒P�̃`�F�b�N
			boost::shared_ptr<CRelRoad> spRoad( boost::static_pointer_cast<CRelRoad>(*itRoad) );
			const CRelRoad& cRoad( *spRoad );
			cRoad.CheckLogic( cRule, cErrObjects );

			// FROM_NODE_ID��TO_NODE_ID���擾
			long nFromNodeID = cRoad.GetFromNodeID();
			long nToNodeID = cRoad.GetToNodeID();
			mapRoadsByNodeID[nFromNodeID].push_back( spRoad );
			mapRoadsByNodeID[nToNodeID].push_back( spRoad );

			// �l�b�g���[�N�����N�ƃl�b�g���[�N�m�[�h�̕����`�F�b�N
			// �E���H�����N�̒[�_�̍��W�ƁAFROM�ATO�Ɗ֘A�Â��Ă���m�[�h�̍��W����v���Ă��邩
			const_rows_iterator itFromNode = find( emNetworkNodeTableType, nFromNodeID );
			const_rows_iterator itToNode   = find( emNetworkNodeTableType, nToNodeID   );
			if( end(emNetworkNodeTableType) == itFromNode || end(emNetworkNodeTableType) == itToNode ||	// �m�[�h�̑���
				( ! ( (*itFromNode)->GetShape()->Equals( cRoad.GetFromPoint() ) &&					// FROM
					  (*itToNode)->GetShape()->Equals( cRoad.GetToPoint() ) ) ) )					// TO
			{
				cErrObjects.push_back( CErrorObject( *itRoad->get(), CErrorInfo( sindyErr_RoadFromToPointIsEqualNodePoint, sindyErrLevel_ERR ) ) );
				//obj.SetErrorMessage( _T("%s"), strMsg ); <- �ڍׂ͂��Ƃŏ�����
			}

			// �m�[�h�`�F�b�N�p�ɐڑ��m�[�h���i�[
			if ( end(emNetworkNodeTableType) != itFromNode )
				listNodeIDs.push_back( (*itFromNode)->GetOID() );
			if ( end(emNetworkNodeTableType) != itToNode )
				listNodeIDs.push_back( (*itFromNode)->GetOID() );

			// �l�b�g���[�N�����N���[�v�`�F�b�N
			// [Bug 6242]���H�����N�����[�v�`��ɂȂ�ꍇ�ł��m�[�h���폜�ł��Ă��܂�
			// ����FROM_NODE_ID/TO_NODE_ID���������N�͑��݂��Ă͂����Ȃ�
			const_rows_iterator itRoad2 = itRoad;
			++itRoad2;
			IPointCollectionPtr ipGeom1( (IGeometry*)*cRoad.GetShape() );
			for( ; itRoad2 != itRoadEnd; ++itRoad2 )
			{
				// FROM_NODE_ID��TO_NODE_ID���擾
				boost::shared_ptr<CRelRoad> spRoad2( boost::static_pointer_cast<CRelRoad>(*itRoad2) );
				const CRelRoad& cRoad2( *spRoad2 );
				long nFromNodeID2 = cRoad2.GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 );
				long nToNodeID2 = cRoad2.GetLongValueByFieldName(schema::road_link::kToNodeID, -1 );

				if ( (nFromNodeID == nFromNodeID2 && nToNodeID == nToNodeID2) ||
					 (nFromNodeID == nToNodeID2 && nToNodeID == nFromNodeID2) )
					cErrObjects.push_back( CErrorObject( cRoad, CErrorInfo( sindyErr_NetworkLinkLoop, sindyErrLevel_ERR, NULL, -1, -1, _T("%d - %d"), cRoad.GetOID(), cRoad2.GetOID() ) ) );
				// [CHECK] [ROADCHECK 02320] �\���_���m���ڐG���Ă��邩�ǂ����ibug 8463�j
				IPointPtr ipTouch;
				IPointCollectionPtr ipGeom2( (IGeometry*)*cRoad2.GetShape() );
				if( AheIsTouchVertex( ipGeom1, ipGeom2, ipTouch ) )
					cErrObjects.push_back( CErrorObject( cRoad, CErrorInfo( sindyErr_CheckReturnErr, sindyErrLevel_ERR, (IGeometryPtr)ipTouch, -1, -1, SHAPE_POINT_TOUCHING, cRoad.GetOID(), cRoad2.GetOID() ) ) );

				// [CHECK] [ROADCHECK 02322] ����m�[�h�ɐڑ�����������ߐڂ��Ă���ibug 8463�j
				// ���������N�ł̓`�F�b�N���Ȃ�(���s��Phase2�Ή�)
				if( GetTableType()==sindyTableType::walk_link || GetTableType()==sindyTableType::lane_link ) continue;

				if( nFromNodeID == nFromNodeID2 || nFromNodeID == nToNodeID2 ||
					nToNodeID   == nFromNodeID2 || nToNodeID   == nToNodeID2 )
				{
					double dLen = min( AheGetConnectSegmentCrack( ipGeom1, ipGeom2 ), AheGetConnectSegmentCrack( ipGeom1, ipGeom2 ) );
					if( 20.0 >= dLen )
					{
						IGeometryPtr ipErrGeom;
						if( nFromNodeID == nFromNodeID2 || nFromNodeID == nToNodeID2 )
							ipErrGeom = (IPoint*)cRoad.GetFromPoint();
						else
							ipErrGeom = (IPoint*)cRoad.GetToPoint();
						cErrObjects.push_back( CErrorObject( cRoad, CErrorInfo( sindyErr_CheckReturnWarning, sindyErrLevel_WARN, ipErrGeom, -1, -1, SEGMENT_ARE_CLOSE, cRoad.GetOID(), cRoad2.GetOID(), dLen ) ) );
					}
				}
			}
		}
	}
	// [CHECK] �񍳘H�p�x�`�F�b�N�ibug 8463 ROADCHECK 02195�j
	typedef std::pair<const long,std::vector<CSPRow>> PAIR_ROADS_BY_NODEID;
	// �񍳘H�p�x�`�F�b�N
	// �S�������N�ł͌x���Ƃ��A�S�������N�ȊO�ł̓G���[�Ƃ���
	// TODO �l�b�g���[�N�����N�ʂ̃`�F�b�N�Ƃ��Ď������ׂ��ł́H
	bool bRailWayChk = (GetTableType()==sindyTableType::railway_link);

	foreach( const PAIR_ROADS_BY_NODEID& _pair_roads_by_nodeid, mapRoadsByNodeID )
	{
		if( 2 != _pair_roads_by_nodeid.second.size() ) // �񍳘H�̂�
			continue;

		const_rows_iterator itNode = find( emNetworkNodeTableType, _pair_roads_by_nodeid.first );
		_ASSERT( itNode != end( emNetworkNodeTableType ) ); // �K������
		// �ύX������ꍇ�̂݃`�F�b�N����
		// �ύX���Ȃ������N�̏ꍇ�A���ׂĂ̐ڑ������N��������Ă��Ȃ��\������ibug 8463 #14�j
		const CRow& road1 = *_pair_roads_by_nodeid.second[0];
		const CRow& road2 = *_pair_roads_by_nodeid.second[1];
		if( (*itNode)->Changed() || road1.Changed() || road2.Changed() )
		{
			double dAngle = AheGetAngle( (ICurvePtr)(IGeometry*)*road1.GetShape(), (ICurvePtr)(IGeometry*)*road2.GetShape() );
			if( -20.0 <= dAngle && dAngle <= 20.0 )
				cErrObjects.push_back( CErrorObject( road1, CErrorInfo( sindyErr_CheckReturnErr,
				(bRailWayChk? sindyErrLevel_WARN:sindyErrLevel_ERR),
				NULL, -1, -1,
				ANGLE_LESS_THAN_20, road1.GetOID(), road2.GetOID(), (int)dAngle ) ) );
		}
	}
	// ���H�m�[�h��̂̕����`�F�b�N
	foreach( const boost::shared_ptr<CRow>& spNode, std::make_pair( begin(emNetworkNodeTableType), end(emNetworkNodeTableType) ) )
	{
		schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

		// �m�[�h��ʂ��K�؂��ǂ���
		// ���H�����N���m�̐ڑ��p�x�̃`�F�b�N

		// �ڑ����Ă��铹�H���ҏW���ꂽ or �ҏW�������́��폜����Ă��Ȃ����̂̂݃`�F�b�N
		if ( listNodeIDs.find( spNode->GetOID() ) != listNodeIDs.end() || (spNode->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted) )
			spNode->CheckLogic( /*g_cRule.CreateLogicRule( itNode->second, */cRule/* )*/, cErrObjects );
	}

	// �ҏW���������N��Ɋ֌W���铹�H���擾(LQRF�t���O�`�F�b�N�p)
	std::map<CString, std::list<long>> mapRoadLQRFs;

	for( const_linkqs_iterator it = linkqs_begin(); it != linkqs_end(); ++it )
	{
		// ***_LQRF�擾
		CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName());

		for ( CRuleLinkQueue::const_value_iterator itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
			if ( it->first == AheTableName2TableType(itInf->get<0>()) )
				break;
		}

		// �����N�񕪃��[�v
		for( const_linkq_iterator itLinkQueue = it->second.begin(); itLinkQueue != it->second.end(); ++itLinkQueue ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( (*itLinkQueue).Changed( &emUpdateType ) ) {
				for ( CLinkQueue::const_iterator itLQ = (*itLinkQueue).begin(); itLQ != (*itLinkQueue).end(); ++itLQ ) {
					const_rows_iterator itRoad = find( GetTableType(), (*itLQ).GetLinkId() );	// LINK_ID�̓��H���擾

					// ���H�����݂��Ă��� and LQ���폜���ꂽ�H
					if ( itRoad != end(GetTableType()) && ( (*itLQ).Changed( &emUpdateType ) && emUpdateType == schema::ipc_table::update_type::kDeleted ) ) {
						// ���H�폜����Ă��Ȃ��H
						if ( ! (*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
							if ( (*itRoad)->GetBoolValueByFieldName( itInf->get<2>(), false ) ) // �֘A�t���O�� true ���ǂ���
								mapRoadLQRFs[itInf->get<2>()].push_back( (*itRoad)->GetOID() );
						}
					}
				}
			}
		}
	}

	// �����N��̐������Ɠ��H��LQRF�̐������`�F�b�N
	for( const_linkqs_iterator it = linkqs_begin(); it != linkqs_end(); ++it )
	{
		// ***_LQRF�擾
		CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName());

		for ( CRuleLinkQueue::const_value_iterator itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
			if ( it->first == AheTableName2TableType(itInf->get<0>()) )
				break;
		}

		std::list<long> listIDs = mapRoadLQRFs[itInf->get<2>()];

		listIDs.sort();
		listIDs.unique();

		// �����N�񕪃��[�v
		for( const_linkq_iterator itLinkQueue = it->second.begin(); itLinkQueue != it->second.end(); ++itLinkQueue ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			// �ҏW���Ă�����́��폜�ȊO
			if ( (*itLinkQueue).Changed( &emUpdateType ) ) {
				if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
					itLinkQueue->CheckLogic( cRule, cErrObjects );	// �����N��̐������`�F�b�N

					CGeometry cBasePoint;	// LINKDIR_C�`�F�b�N�p

					for ( CLinkQueue::const_iterator itLQ = (*itLinkQueue).begin(); itLQ != (*itLinkQueue).end(); ++itLQ ) {
						(*itLQ).Changed( &emUpdateType );

						// �폜�ȊO
						if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
							const_rows_iterator itRoad = find( GetTableType(), (*itLQ).GetLinkId() );	// LINK_ID�̓��H���擾

							// ���H�����݂��ĂȂ� or �폜���ꂽ�H
							if ( itRoad == end(GetTableType()) || ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType == schema::ipc_table::update_type::kDeleted ) )
								cErrObjects.push_back( CErrorObject( (*itLQ), CErrorInfo( sindyErr_LinkQueueNotRoad, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kLinkID ) ) );
							else {
								// LQRF�t���O�����Ă��邩�ǂ���
								if( ! (*itRoad)->GetBoolValueByFieldName( itInf->get<2>(), false ) ) // �֘A�t���O�� true ���ǂ���
									cErrObjects.push_back( CErrorObject( *itRoad->get(), CErrorInfo( sindyErr_RoadBadLQRF, sindyErrLevel_ERR, NULL, 0, -1, itInf->get<2>() ) ) );

								// LQRF�Q����폜
								listIDs.erase( std::remove( listIDs.begin(), listIDs.end(), (*itLQ).GetLinkId() ), listIDs.end() );

								// �����`�F�b�N
								CGeometry cPoint;

								// From�|�C���g�擾
								if ( (*itLQ).GetLinkDir() == 1 )
									cPoint = (*itRoad)->GetShape()->GetPoint( 0 );
								else
									cPoint = (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 );

								if ( (IGeometry*)cBasePoint ) {
									// From/To����v���Ȃ�������LINKDIR_C����������
									if ( ! cPoint.Equals( cBasePoint ) )
										cErrObjects.push_back( CErrorObject( (*itLQ), CErrorInfo( sindyErr_LinkQueueBadDirection, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kLinkDir ) ) );
								}

								// To�|�C���g�擾
								if ( (*itLQ).GetLinkDir() == 1 )
									cBasePoint = (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 );
								else
									cBasePoint = (*itRoad)->GetShape()->GetPoint( 0 );
							}
						}
					}
				}
			} else {
				// LQRF�Q����폜
				for ( CLinkQueue::const_iterator itLQ = (*itLinkQueue).begin(); itLQ != (*itLinkQueue).end(); ++itLQ )
					listIDs.erase( std::remove( listIDs.begin(), listIDs.end(), (*itLQ).GetLinkId() ), listIDs.end() );
			}
		}

		mapRoadLQRFs[itInf->get<2>()].swap( listIDs );
	}

	// LQRF�Q�ɓ��H��OID���c���Ă��炨������
	for ( std::map<CString, std::list<long>>::const_iterator itLQRF = mapRoadLQRFs.begin(), itLQRFEnd = mapRoadLQRFs.end(); itLQRF != itLQRFEnd; ++itLQRF ) {
		for ( std::list<long>::const_iterator itID = itLQRF->second.begin(), itIDEnd = itLQRF->second.end(); itID != itIDEnd; ++itID ) {
			const_rows_iterator itRoad = find( GetTableType(), (*itID) );	// LINK_ID�̓��H���擾

			cErrObjects.push_back( CErrorObject( *itRoad->get(), CErrorInfo( sindyErr_RoadBadLQRF, sindyErrLevel_ERR, NULL, 0, -1, itLQRF->first ) ) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��
sindyErrCode CRoadNetwork::CheckModify( CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->CheckModify( cErrObjects );
	}

	// �����N��
	for( const_linkqs_iterator it = linkqs_begin(); it != linkqs_end(); ++it )
	{
		for( const_linkq_iterator itLinkQueue = it->second.begin(); itLinkQueue != it->second.end(); ++itLinkQueue )
			itLinkQueue->CheckModify( cErrObjects );
	}

	return cErrObjects.GetDefaultErrCode();
}

// �o�^����Ă���t�B�[�`��������΂��̃t�B�[�`���̃|�C���^��Ԃ��܂�
const CRow* CRoadNetwork::FindObject( _IRow* ipRow ) const
{
	if( ! ipRow )
		return NULL;

	CRow* pRet = NULL;	// �Ԃ�l

	sindyTableType::ECode emType = AheTableName2TableType(AheGetFeatureClassName( ipRow ));
	const_rows_iterator it = find( emType, AheGetOID(ipRow) );
	if( it != end(emType) )
		pRet = it->get();

	return pRet;
} 

errorcode::sindyErrCode CRoadNetwork::SelectByRoadFromLinkqs(const CListLong& listRoadIDs, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/)
{
	// �i�[����Ă��铹�H�Q
	sindyErrCode emErr  = sindyErr_NoErr;

	// [Bug 5873] �������铹�H��2�����b�V����m�[�h�ɐڑ����Ă��鎞�́A���Α��̓��H�������Ώۂɂ���B
	CListLong listRoadIDsTmp( listRoadIDs );

	_SelectByRoadFromLinkqs( listRoadIDs, listRoadIDsTmp );

	std::list<long> listSearchIDs;

	// �����N��
	for( const_linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs )
	{
		// INF��
		for( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq )
		{
			(*itLinkq).sort();

			// ���H�Ɋ֘A���郊���N��̂�
			bool bIsRoadRel = false;

			for ( std::list<long>::const_iterator itIds = listRoadIDsTmp.begin(), itIdsEnd = listRoadIDsTmp.end(); itIds != itIdsEnd; ++ itIds ) {
				if ( (*itLinkq).HasRoad( *find(GetTableType(), (*itIds) )->get() ) ) {
					bIsRoadRel = true;
					break;
				}
			}

			if ( bIsRoadRel ) {
				// �i�[����Ă��Ȃ����H������
				// LQ��
				for ( CLinkQueue::const_iterator itLQ = (*itLinkq).begin(); itLQ != (*itLinkq).end(); ++itLQ )
				{
					// LQ�̓��H���擾�ς݂łȂ���Βǉ�
					if ( find( GetTableType(), (*itLQ).GetLinkId() ) == end(GetTableType()) )
						listSearchIDs.push_back( (*itLQ).GetLinkId() );
				}
			}
		}
	}

	listSearchIDs.sort();
	listSearchIDs.unique();

	if ( ! listSearchIDs.empty() ) {
		// ���H����
		CFeatureContainer::Select( GetTableName(), listSearchIDs, typeid(CRelRoad&), bRecycling, bCreateCache, cErrs, false ); // �������͍ŏ��ɂ��Ă���̂ōŌ��false

		// CheckLogic�Ő������̃`�F�b�N������̂ŁA�֘A������̂��ǉ�����
		// �m�[�h����(�ǉ������������H�Ɋ֘A����m�[�h)
		CString strNetworkNodeTableName( AheGetNetworkNodeFeatureClassName( GetTableName() ) );
		if( ! empty(GetTableType()) )
		{
			// FROM�ATO����m�[�hID��쐬
			std::list<long> listNodeIDs;
			for( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad )
			{
				if ( std::find( listSearchIDs.begin(), listSearchIDs.end(), (*itRoad)->GetOID() ) != listSearchIDs.end() ) {
					listNodeIDs.push_back( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) ); // TODO: FROM�ATO�t�B�[���h���̓��[���ɒ�`���Ă��Ȃ����A�ǂ��H�H
					listNodeIDs.push_back( (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) );
				}
			}

			listNodeIDs.sort();
			listNodeIDs.unique();

			// ����
			CFeatureContainer::Select( strNetworkNodeTableName, listNodeIDs, typeid(CRelNode&), bRecycling, bCreateCache, cErrs, false ); // �������͍ŏ��ɂ��Ă���̂ōŌ��false
		}

		// �֘A�I�u�W�F�N�g�̓l�b�g���[�N�����N�̂�ID�Ō�������̂�ID�������Ă���
		// TODO: �������AID�֘A��������̂̂݁B�����`��݂̂̊֘A�̂��̂��ł����Ƃ��́A���̂Ƃ��ɍl���悤
		// ����I�u�W�F�N�g�̌����i������ID�֘A��������̂̂݁j
		if( ! listSearchIDs.empty() )
		{
			for( CRuleRelationshipContain::const_value_iterator it = g_cRuleRelationshipContain.begin(GetTableName()), 
				itEnd = g_cRuleRelationshipContain.end(GetTableName()); it != itEnd; ++it )
			{
				if( ! it->second.IsEmpty() ) // ID�֘A��������̂̂�
				{
					// ����
					CFeatureContainer::Select( it->first, listSearchIDs, typeid(CFeature&), bRecycling, bCreateCache, cErrs, false, it->second, GetTableName() ); // �������͍ŏ��ɂ��Ă���̂ōŌ��false
				}
			}
		}
		// �O���e�[�u���̌���
		if( ! listSearchIDs.empty() )
		{
			for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin(GetTableName()),
				itEnd = g_cRuleRelationTable.end(GetTableName()); it != itEnd; ++it )
			{
				// ����
				CFeatureContainer::Select( it->get<1>(), listSearchIDs, typeid(CRow&), bRecycling, bCreateCache, cErrs, false, it->get<2>(), GetTableName() ); // �������͍ŏ��ɂ��Ă���̂ōŌ��false
			}
		}

		// �����N�����������
		for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf )
		{
			sindyTableType::ECode emInfCode = AheTableName2TableType(itInf->get<0>());
			sindyTableType::ECode emLqCode = AheTableName2TableType(itInf->get<1>());

			// LQ�擾
			std::list<long> listInfIDs;

			CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );
			LOGASSERTEERR_IF( cLQTable != NULL, sindyErr_TableNotFound )
			{
				// ���W�������HID�Ɋ֘A����LQ���擾
				// �����Ō����ł���LQ�͎̂Ă�iINF��ID���X�g�쐬�݂̂Ɏg�p�j
				std::list<CString> listIDsString;
				cLQTable.IDs2Str( listSearchIDs, listIDsString );
				for( std::list<CString>::const_iterator itIDsString = listIDsString.begin(); itIDsString != listIDsString.end(); ++itIDsString )
				{
					// WHERE��쐬
					CString strWhere;
					strWhere.Format(_T("%s IN (%s)"), schema::link_queue::kLinkID, *itIDsString );

					// ����
					_ICursorPtr ipCursor = cLQTable.Search( AheInitQueryFilter( NULL, schema::link_queue::kInfID, strWhere ), true );	// �Ƃ肠����INF��ID���Ƃ�̂ŁALQ�͎̂Ă�

					// �擾����LQ����INF��ID���擾����
					long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
					LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
					{
						_IRowPtr ipRow;
						while( S_OK == ipCursor->NextRow( &ipRow ) )
						{
							if( ipRow == NULL ) break;

							CComVariant vaInfID(-1L);
							ipRow->get_Value( lInfIDIndex, &vaInfID );

							listInfIDs.push_back( vaInfID.lVal );
						}
						ipCursor = NULL;
					}
				}
				listInfIDs.sort();
				listInfIDs.unique();

				// ���Ƀ��X�g�ɑ��݂���ꍇ�͂��̕�����������
				for( std::list<CLinkQueue>::const_iterator it = m_mapLinkQueue[emInfCode].begin(); it != m_mapLinkQueue[emInfCode].end(); ++it )
					listInfIDs.remove( it->GetOID() );

				// CLinkQueue�N���X�̍\����A�K�����INF������K�v������
				// �֘A����INF����������
				if( ! listInfIDs.empty() )
				{
					CTable cInfTable( emInfCode, OpenSameOwnerTable( emInfCode ) );
					LOGASSERTEERR_IF( cInfTable != NULL, sindyErr_TableNotFound )
					{
						// ���L�I�u�W�F�N�g�̎擾
						CSPTableNameString spInfNameString( cInfTable.GetNameString() );
						CSPFieldMap spInfFieldMap( cInfTable.GetFieldMap() );

						// ����
						_ICursorPtr ipCursor = cInfTable.GetRows( listInfIDs, bRecycling );

						// �i�[
						LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
						{
							_IRowPtr ipRow;
							while( S_OK == ipCursor->NextRow( &ipRow ) )
							{
								if( ipRow == NULL ) break;

								long lOID = -1;
								ipRow->get_OID( &lOID );

								linkq_iterator itLinkQueue = linkq_find( emInfCode, lOID );
								if ( itLinkQueue == linkq_end(emInfCode) )
									itLinkQueue = m_mapLinkQueue[emInfCode].insert( m_mapLinkQueue[emInfCode].end(), CLinkQueue(emInfCode,ipRow) );
								else
									(*itLinkQueue) = ipRow;

								// ���T�C�N������ꍇ�̓L���b�V���쐬
								if( bRecycling )
									itLinkQueue->CreateCache( true, spInfFieldMap, spInfNameString );
								else if( bCreateCache )
									itLinkQueue->CreateCache( false, spInfFieldMap, spInfNameString );
							}
						}
						ipCursor = NULL;
					}
				}

				// �擾����INFID�Ɋ֘A����LQ���擾����
				listIDsString.clear();
				cLQTable.IDs2Str( listInfIDs, listIDsString );
				for( std::list<CString>::const_iterator itIDsString = listIDsString.begin(); itIDsString != listIDsString.end(); ++itIDsString )
				{
					// WHERE��쐬
					CString strWhere;
					strWhere.Format(_T("%s IN (%s)"), schema::link_queue::kInfID, *itIDsString );

					// ���L�I�u�W�F�N�g�̎擾
					CSPTableNameString spLQNameString( cLQTable.GetNameString() );
					CSPFieldMap spLQFieldMap( cLQTable.GetFieldMap() );

					// ����
					_ICursorPtr ipCursor = cLQTable.Search( AheInitQueryFilter( NULL, NULL, strWhere ), bRecycling );

					// LQ��INFID�ŐU�蕪��
					long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
					LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
					{
						_IRowPtr ipRow;
						while( S_OK == ipCursor->NextRow( &ipRow ) )
						{
							if( ipRow == NULL ) break;

							CComVariant vaInfID( -1L );
							ipRow->get_Value( lInfIDIndex, &vaInfID );

							linkq_iterator itLinkQueue = linkq_find( emInfCode, vaInfID.lVal );
							itLinkQueue->push_back( emLqCode, ipRow, spLQFieldMap, spLQNameString );

							// ���T�C�N������ꍇ�̓L���b�V���쐬
							if( bRecycling )
								itLinkQueue->rbegin()->CreateCache( true, spLQFieldMap, spLQNameString );
							else if( bCreateCache )
								itLinkQueue->rbegin()->CreateCache( false, spLQFieldMap, spLQNameString );
						}
						ipCursor = NULL;
					}
				}
			}
		}
	}

	return emErr;
}

void CRoadNetwork::_SelectByRoadFromLinkqs(const CListLong& listRoadIDs, CListLong& listSearchRoads )
{
	// [Bug 5873]
	// �������铹�H��2�����b�V����m�[�h�ɐڑ����Ă��鎞�́A���Α��̓��H�������Ώۂɂ���B
	CString strNetworkNodeTableName( AheGetNetworkNodeFeatureClassName( GetTableName() ) );
	sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType( strNetworkNodeTableName );
	CListLong listBorderNodeIDs;	// 2�����b�V����m�[�h�Q

	// �������铹�H��2�����b�V����m�[�h�ɐڑ����Ă���Ȃ�A�m�[�hID���i�[
	for ( CListLong::const_iterator itIDs = listRoadIDs.begin(), itIDsEnd = listRoadIDs.end(); itIDs != itIDsEnd; ++itIDs ) {
		CContainer::const_rows_iterator itRoad = find( GetTableType(), (*itIDs) );

		if ( itRoad != end( GetTableType() ) ) {
			CContainer::const_rows_iterator itFrom = find( emNetworkNodeTableType, (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) );
			CContainer::const_rows_iterator itTo = find( emNetworkNodeTableType, (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) );

			if ( itFrom != end(emNetworkNodeTableType) ) {
				CRelNode* pNode = static_cast<CRelNode*>(itFrom->get());

				if ( pNode->IsBorderNode() )
					listBorderNodeIDs.push_back( (*itFrom)->GetOID() );
			}

			if ( itTo != end(emNetworkNodeTableType) ) {
				CRelNode* pNode = static_cast<CRelNode*>(itTo->get());

				if ( pNode->IsBorderNode() )
					listBorderNodeIDs.push_back( (*itTo)->GetOID() );
			}
		}
	}

	listBorderNodeIDs.sort();
	listBorderNodeIDs.unique();

	// 2�����b�V����m�[�h�ɐڑ����Ă��铹�H���������X�g�ɒǉ�
	CListLong listRoadIDsTmp;

	for ( CContainer::const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); 
		itRoad != itRoadEnd; ++itRoad ) {

		long OID = (*itRoad)->GetOID();

		if ( listBorderNodeIDs.find( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) ) != listBorderNodeIDs.end() ) {
			if ( listSearchRoads.find( OID ) == listSearchRoads.end() )	// �i�[�ς݁H
				listRoadIDsTmp.push_back( (*itRoad)->GetOID() );
		}

		if ( listBorderNodeIDs.find( (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) ) != listBorderNodeIDs.end() ) {
			if ( listSearchRoads.find( OID ) == listSearchRoads.end() )	// �i�[�ς݁H
				listRoadIDsTmp.push_back( (*itRoad)->GetOID() );
		}
	}

	listRoadIDsTmp.sort();
	listRoadIDsTmp.unique();

	if ( ! listRoadIDsTmp.empty() ) {	// �ǉ��������H�Q������H
		listSearchRoads += listRoadIDsTmp;

		// �ċA����
		_SelectByRoadFromLinkqs( listRoadIDsTmp, listSearchRoads );
	}
}

errorcode::sindyErrCode CRoadNetwork::swap( CSPRow pSource, CSPRow pDest )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pSource && pDest, sindyErr_ArgIsNull )
	{
		std::map<sindyTableType::ECode,bool> mapContains;

		// ����֌W�̕ύX�̑O����
		for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
			itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
			itContain != itContainEnd; ++itContain )
		{
			bool bOrg = pSource->empty(AheTableName2TableType(itContain->first));
			bool bNew = pDest->empty(AheTableName2TableType(itContain->first));

			if ( ! bOrg && bNew )
				mapContains[AheTableName2TableType(itContain->first)] = true;
			else 
				mapContains[AheTableName2TableType(itContain->first)] = false;
		}

		// �O���e�[�u���̕ύX�̑O����
		for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(GetTableName()),
			itOuterEnd = g_cRuleRelationTable.end(GetTableName());
			itOuter != itOuterEnd; ++itOuter )
		{
			bool bOrg = pSource->empty(AheTableName2TableType(itOuter->get<1>()));
			bool bNew = pDest->empty(AheTableName2TableType(itOuter->get<1>()));

			if ( ! bOrg && bNew )
				mapContains[AheTableName2TableType(itOuter->get<1>())] = true;
			else 
				mapContains[AheTableName2TableType(itOuter->get<1>())] = false;
		}

		// ����ւ�
		pSource->swap( *pDest.get(), true, true, false, true );

		// ����֌W�̕ύX
		for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
			itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
			itContain != itContainEnd; ++itContain )
		{
			// �I���W�i�����ɂ����ĐV�K���ɂȂ����Aswap�œ���ւ��Ă��܂��Ă���̂ŁA���꒼��
			if ( mapContains[AheTableName2TableType(itContain->first)] ) {
				for( const_rows_iterator itSeg = pDest->begin(AheTableName2TableType(itContain->first)),
					itSetEnd = pDest->end(AheTableName2TableType(itContain->first));
					itSeg != itSetEnd; ++itSeg )
				{
					(*itSeg)->SetLongValueByFieldName(itContain->second, pSource->GetOID());
					pSource->push_back( *itSeg );
				}

				for( const_rows_iterator itSeg = pSource->begin(AheTableName2TableType(itContain->first)),
					itSetEnd = pSource->end(AheTableName2TableType(itContain->first));
					itSeg != itSetEnd; ++itSeg )
				{
					pDest->erase( *itSeg );
				}
			}
		}

		// �O���e�[�u���̕ύX
		for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(GetTableName()),
			itOuterEnd = g_cRuleRelationTable.end(GetTableName());
			itOuter != itOuterEnd; ++itOuter )
		{
			// �I���W�i�����ɂ����ĐV�K���ɂȂ����Aswap�œ���ւ��Ă��܂��Ă���̂ŁA���꒼��
			if ( mapContains[AheTableName2TableType(itOuter->get<1>())] ) {
				for( const_rows_iterator itOneway = pDest->begin(AheTableName2TableType(itOuter->get<1>())),
					itOnewayEnd = pDest->end(AheTableName2TableType(itOuter->get<1>()));
					itOneway != itOnewayEnd; ++itOneway )
				{
					(*itOneway)->SetLongValueByFieldName(itOuter->get<2>(), pSource->GetOID());
					pSource->push_back( *itOneway );
				}

				for( const_rows_iterator itOneway = pSource->begin(AheTableName2TableType(itOuter->get<1>())),
					itOnewayEnd = pSource->end(AheTableName2TableType(itOuter->get<1>()));
					itOneway != itOnewayEnd; ++itOneway )
				{
					pDest->erase( *itOneway );
				}
			}
		}

		// �����N���LQ��ID��ύX
		for ( linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs ) {
			// INF��
			for ( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq ) {
				if ( itLinkq->HasRoad( *pSource.get() ) || itLinkq->HasRoad( *pDest.get() ) ) {	// INF��LQ�ɓ��H������H
					link_queue::CLQ cOrgLQ, cNewLQ;
					bool bHasOrg = false, bHasNew = false;

					for ( CLinkQueue::iterator itLQ = itLinkq->begin(); itLQ != itLinkq->end(); ++itLQ ) {
						if ( pSource->GetOID() == (*itLQ).GetLinkId() ) {
							cOrgLQ = (*itLQ);
							bHasOrg = true;
						}

						if ( pDest->GetOID() == (*itLQ).GetLinkId() ) {
							cNewLQ = (*itLQ);
							bHasNew = true;
						}
					}

					if ( bHasOrg )
						cOrgLQ.SetLinkId( pDest->GetOID() );

					if ( bHasNew )
						cNewLQ.SetLinkId( pSource->GetOID() );
				}
			}
		}
	}

	return sindyErr_NoErr;
}

errorcode::sindyErrCode CRoadNetwork::_DeleteLQ( CSPRow pRow, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		std::map<sindyTableType::ECode, std::list<CLinkQueue> > mapLinkqs;

		// �폜���铹�H�Ɋ֘A���郊���N����擾
		for( const_linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs )
		{
			// INF��
			for( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq )
			{
				if ( itLinkq->HasRoad(*pRow) ) {	// INF��LQ�ɓ��H������H
					CLinkQueue cNewLinkq;

					cNewLinkq.SetObject( NULL, itLinkqs->first, true, (*itLinkq).GetFieldMap(), (*itLinkq).GetNameString() );

					// LQ�폜
					itLinkq->DeleteLQ( *pRow, cNewLinkq, cErrObjects );

					if ( !cNewLinkq.empty_lq() )	// �������ꂽLQ������H
						mapLinkqs[itLinkqs->first].insert( mapLinkqs[itLinkqs->first].end(), cNewLinkq );
				}
			}
		}

		for ( const_linkqs_iterator itLinkqs = mapLinkqs.begin(); itLinkqs != mapLinkqs.end(); ++itLinkqs ) {
			for ( const_linkq_iterator itLinkq = itLinkqs->second.begin(); itLinkq != itLinkqs->second.end(); ++itLinkq )
				m_mapLinkQueue[itLinkqs->first].insert( m_mapLinkQueue[itLinkqs->first].end(), (*itLinkq) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::DeleteLinkq( CLinkQueue& cDelLinkq, CErrorObjects& cErrObjects )
{
	if ( sindyErr_NoErr == cDelLinkq.Delete( cErrObjects ) )	// INF�폜
		ResetRoadLQRF( cDelLinkq.GetTableType(), cErrObjects );	// ���H��LQRF�Đݒ�

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::ResetRoadLQRF(sindyTableType::ECode emLinkqType, CErrorObjects& cErrObjects)
{
	for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
		if ( emLinkqType == AheTableName2TableType(itInf->get<0>()) ) {	// �폜���郊���N��Ɠ�����ʁH
			// ���H��LQRF��0��
			for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); itRoad != itRoadEnd; ++itRoad )
			{
				schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

				if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {	// �폜���Ă��Ȃ����H�H
					if ( (*itRoad)->GetBoolValueByFieldName(itInf->get<2>(), false) ) {	// �t���O�������Ă���H
						// ���Ƀ����N�񂪂��邩�`�F�b�N
						bool bHasLinkq = false;

						// INF��
						for ( const_linkq_iterator itLinkq = linkq_begin(emLinkqType); itLinkq != linkq_end(emLinkqType); ++itLinkq ) {
							schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

							if( !(*itLinkq).Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {	// �폜���Ă��Ȃ������N��H
								// LQ��
								for ( CLinkQueue::const_iterator itLQ = (*itLinkq).begin(); itLQ != (*itLinkq).end(); ++itLQ ) {
									schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

									if( !(*itLQ).Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
										if ( (*itRoad)->GetOID() == (*itLQ).GetLinkId() ) {
											bHasLinkq = true;
											break;
										}
									}
								}

								if ( bHasLinkq )
									break;
							}
						}

						if ( ! bHasLinkq ) {	// ����LQ���Ȃ��H
							// LQRF���O��
							if ( sindyErr_NoErr != (*itRoad)->SetBoolValueByFieldName(itInf->get<2>(), false) )
								cErrObjects.push_back( CErrorObject( *itRoad->get() ) );
						}
					}
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::DeleteLQ( CLinkQueue* pLinkq, const link_queue::CLQ& cLQ, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pLinkq, sindyErr_ArgIsNull )
	{
		long OID = cLQ.GetLinkId();

		for( rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); itRoad != itRoadEnd; ++itRoad )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( ! (*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {	// �폜���Ă��Ȃ����H�H
				if ( (*itRoad)->GetOID() == OID ) {
					CLinkQueue cNewLinkq;
					std::map<sindyTableType::ECode, std::list<CLinkQueue> > mapLinkqs;

					cNewLinkq.SetObject( NULL, pLinkq->GetTableType(), true, pLinkq->GetFieldMap(), pLinkq->GetNameString() );

					// LQ�폜
					pLinkq->DeleteLQ( *(*itRoad).get(), cNewLinkq, cErrObjects );

					if ( !cNewLinkq.empty_lq() )	// �������ꂽLQ������H
						mapLinkqs[pLinkq->GetTableType()].insert( mapLinkqs[pLinkq->GetTableType()].end(), cNewLinkq );

					for ( const_linkqs_iterator itLinkqs = mapLinkqs.begin(); itLinkqs != mapLinkqs.end(); ++itLinkqs ) {
						for ( const_linkq_iterator itLinkq = itLinkqs->second.begin(); itLinkq != itLinkqs->second.end(); ++itLinkq )
							m_mapLinkQueue[itLinkqs->first].insert( m_mapLinkQueue[itLinkqs->first].end(), (*itLinkq) );
					}

					break;
				}
			}
		}

		ResetRoadLQRF( pLinkq->GetTableType(), cErrObjects );	// ���H��LQRF�Đݒ�
	}

	return cErrObjects.GetDefaultErrCode();
}

void CRoadNetwork::MaterializationLinkq()
{
	// ���̉�
	for ( CRoadNetwork::const_linkqs_iterator itLinkqs = linkqs_begin(), itLinkqsEnd = linkqs_end(); itLinkqs != itLinkqsEnd; ++itLinkqs ) {
		for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
			sindyTableType::ECode emInfCode = AheTableName2TableType(itInf->get<0>());
			sindyTableType::ECode emLqCode = AheTableName2TableType(itInf->get<1>());

			if ( itLinkqs->first == emInfCode ) {
				CTable cInfTable( emInfCode, OpenSameOwnerTable( emInfCode ) );
				CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );

				for( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq ) {
					if ( ! (_IRow*)(*itLinkq) && ! (*itLinkq).IsDeleted() )	// ���̂��Ȃ��č폜����Ȃ�����
						itLinkq->Materialization( cInfTable, cLQTable );
				}

				break;
			}
		}
	}
}

bool CRoadNetwork::ResetNodeAttribute( CSPRow cRow, bool* bIsChanged/* = NULL*/ ) // TODO: CRow��CContainer�������ɂƂ�O���֐��ɂ��邱��
{
	bool bHasNodeAttr = false, bDeleteNodeAttr = false;

	if ( bIsChanged )
		*bIsChanged = false;

	sindyTableType::ECode emNodeType( AheTableName2TableType( AheGetNetworkNodeFeatureClassName( GetTableName() ) ) );

	long sNodeclass = -1;
	switch ( AheGetNetWorkType( cRow->GetTableName() ) )
	{
	case sindyeNetworkRoad:
		sNodeclass = (long)GetModifyRoadNodeKind( cRow, &bHasNodeAttr );
		break;
	case sindyeNetworkWalk:
		sNodeclass = (long)GetModifyWalkNodeKind( cRow );
		break;
	case sindyeNetworkSubway:
		sNodeclass = (long)GetModifySubwayNodeKind( cRow );
		break;
	case sindyeNetworkRailway:
		sNodeclass = (long)GetModifyRailwayNodeKind( cRow );
		break;
	case sindyeNotNetwork:
	default:
		return false;
		break;
	}

	// �m�[�h�̎�ʂ��قȂ�ꍇ�̂ݕύX
	if ( bDeleteNodeAttr || sNodeclass != cRow->GetLongValueByFieldName( schema::road_node::kNodeClass, -1 ) )
	{
		cRow->SetLongValueByFieldName( schema::road_node::kNodeClass, sNodeclass );
		if ( bIsChanged )
			*bIsChanged = true;	// �ύX���ꂽ

		// �����m�[�h�̏ꍇ�̂݁A�����ύX�̗�����������
		//if( bSetModifyData )
		//	AheSetModifyData( ipNodeFeature, sindyUpdateTypeProperty );
	}

	return true;
}

schema::road_node::node_class::ECode CRoadNetwork::GetModifyRoadNodeKind( const CSPRow& cRow, bool* pbHasNodeAttr ) const // TODO: CRow��Container�������Ɏ��O���֐��ɂ��邱��
{
	// TODO : �قڂ��҂؂Ȃ̂ŁA�����Ə����������Ɓ�

	// ������
	if ( pbHasNodeAttr )
		(*pbHasNodeAttr) = false;

	// �m�[�h��ʂɂ́A��܂��ɕ�����3��ނ���B
	// 
	// �E2�����b�V��������ɂ���ꍇ�̎��
	//   �E2�����b�V��������PEC�m�[�h
	//   �E2�����b�V��������m�[�h
	// �E�ҏW�҂��Ӑ}�I�ɕt�^�������
	//   �E�������m�[�h
	//   �E�n�C�E�F�C�֌W�m�[�h
	//   �E�_�~�[�{�݃m�[�h�iTF�p�j
	//   �E���ꃊ���NID�h�~�m�[�h
	// �E�ڑ����H�����N�ƃm�[�h�̑����݂̂Ō��܂�ꍇ�̎��
	//   �E�[�_�m�[�h <- PEC�����N���ڑ����Ă����ꍇ���[�_�m�[�h�ł����񂾂����H
	//   �EPEC�����N�ڑ��m�[�h
	//   �E�����_�m�[�h
	//   �E�����ω��_�m�[�h
	//   �E�����t���m�[�h
	//   �E�Ӗ��Ȃ��m�[�h
	//   �E�m�[�h���_�m�[�h�i�g�p���ĂȂ��H�j
	//
	// ���[���F
	// 1. ��L��3�O���[�v�ł͏�̕����D�揇�ʂ�����
	// 2. ��Ԗڂ̃O���[�v���̎�ʂ͏�������O���[�v��艺�̃O���[�v�̎�ʂ�
	//    �ύX����邱�Ƃ͂Ȃ�
	// 3. ��Ԗڂ̃O���[�v���̎�ʂ͕ҏW�҂ɂ���ĉ��̃O���[�v�̎�ʂɕύX��
	//    ��Ȃ����艺�̃O���[�v�̎�ʂɎ����I�ɕύX����邱�Ƃ͂Ȃ�
	// 4. ��ԖڂƎO�Ԗڂ͎����t�^�ł��邪�A��Ԗڂ̓��[�U�t�^�ł���A���炷
	//    ��K�v������
	// 
	// �]���āA�����t�^�̍ۂɈȑO�̑������C�ɂ���K�v������͓̂�Ԗڂ̃O���[�v
	// �ɏ������鑮���݂̂ł���A����ȊO�͌��݂̑����݂̂Ō��߂Ă��܂��Ă悢�B
	// 
	// �A���S���Y���I�ɂ́A
	// �E���b�V�������ォ�ǂ����m�F
	//   �E������Ȃ�return
	// �E�O�̎�ʂ��ҏW�҂��Ӑ}�I�ɕt�^������ʂ��ǂ����m�F
	//   �E�Ӑ}�I�ɕt�^������ʂȂ�ύX�Ȃ���return
	// �E����ȊO�͐ڑ��󋵂Ƒ����Ŕ��f

	//////////////////////////////////////////
	// �O����
	//////////////////////////////////////////
	// �ڑ����Ă��铹�H���擾
	std::list<road_link::CRoadLink*> listConnectRoads;

	for ( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); itRoad != itRoadEnd; ++itRoad ) {
		if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) == cRow->GetOID() ||
			(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) == cRow->GetOID() ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
				listConnectRoads.push_back( dynamic_cast<road_link::CRoadLink*>(itRoad->get()) );
		}
	}

	// �ڑ������N���`�F�b�N	
	// ���H�����N��2�{�ȉ��ŁA���m�[�h�ɐM���@�t���O�y�ь����_���̂������Ă���ꍇ�̓t���O�𗧂Ă�
	if ( listConnectRoads.size() <= 0 ) 
		return (schema::road_node::node_class::ECode)-1;
	else if ( listConnectRoads.size() < 3 ) {
		const road_node::CRoadNode* pRoadNode( dynamic_cast<const road_node::CRoadNode*>(cRow.get()) );

		if ( pRoadNode->IsExistAttrNode() ) {
			if ( pbHasNodeAttr )
				(*pbHasNodeAttr) = true;
		}
	}

	//////////////////////////////////////////
	// 2�����b�V�������ォ�ǂ�������
	//////////////////////////////////////////
	CMesh mesh;

	mesh.SetSpatialReference( cRow->GetShape()->GetSpatialReference() );	// �m�[�h�� SpatialReference ���Z�b�g���Ă��

	std::list<UINT> listMeshCode;

	if ( mesh.IsTouches( cRow->GetShapeCopy(), listMeshCode ) ) {
		// ���b�V����؂��PGDB�ł́A�m�[�h��2�����b�V���m�[�h�ł��Е��ɂ������H�����N���Ȃ�
		// ���̂��߂ɕK���G���[�ɂȂ��Ă��܂��̂ŁA���W�X�g�������ē��H�̖{���𖳎����邩�ǂ���
		// �����߂�
		if ( AheIsPGDBEditMode() ) {
			if ( 1 == listConnectRoads.size() ) {
				if ( listConnectRoads.front()->IsPECLink() )
					return schema::road_node::node_class::kPecEdge;
				else
					return schema::road_node::node_class::kEdge;
			}
		} else {
			if ( 2 == listConnectRoads.size() ) {
				road_link::CRoadLink* pRoad = listConnectRoads.front();
				std::list<road_link::CRoadLink*>::iterator it2 = listConnectRoads.begin(); ++it2;

				if ( pRoad->IsSameAttribute( *(*it2) ) ) {
					// �����Ƃ�PEC�����N�Ȃ�2�����b�V�����ӏ�PEC�m�[�h�A�����łȂ����2�����b�V�����ӏ�m�[�h
					if ( pRoad->IsPECLink() && (*it2)->IsPECLink() )
						return schema::road_node::node_class::kPecEdge;
					else
						return schema::road_node::node_class::kEdge;
				} else {
					// 2�����b�V�����E���ɐڂ��Ă���̂ɐڑ����铹�H�����N�̎�ʂ��قȂ�I�I
					_ASSERTE( false );
					return (schema::road_node::node_class::ECode)-1;
				}
			} else {
				// 2�����b�V�����E���ɐڂ��Ă���̂ɐڑ����铹�H��2�{�ł͂Ȃ��I�I
				_ASSERTE( false );
				return (schema::road_node::node_class::ECode)-1;
			}
		}
	}

	//////////////////////////////////////////
	// �����̎�ʂ��Ӑ}�I�ɕt�^���ꂽ��ʂ��ǂ����𔻒�
	//////////////////////////////////////////
	// ���̃m�[�h��ʂ��擾
	schema::road_node::node_class::ECode sNodeClass = (schema::road_node::node_class::ECode)cRow->GetLongValueByFieldName( schema::road_node::kNodeClass, -1 );

	switch ( sNodeClass ) {
		case schema::road_node::node_class::kDummyFacil:
		case schema::road_node::node_class::kTollbooth:
		case schema::road_node::node_class::kHighway:
		case schema::road_node::node_class::kLQBase: return sNodeClass; break;
		default: break;
	}

	//////////////////////////////////////////
	// �ڑ��󋵂Ƒ����Ŕ���
	//////////////////////////////////////////
	sNodeClass = (schema::road_node::node_class::ECode)0;	// �Ӗ��Ȃ��m�[�h

	if ( listConnectRoads.size() == 1 )	// ��{�̏ꍇ�͒[�_�m�[�h
		sNodeClass = schema::road_node::node_class::kTerminal;
    else {	// ��{�ȏ�̏ꍇ
		bool bIsNormalRoad = false, bIsPECRoad = false;

		for ( std::list<road_link::CRoadLink*>::const_iterator it = listConnectRoads.begin(), itEnd = listConnectRoads.end(); it != itEnd; ++it ) {
			if ( (*it)->IsPECLink() )
				bIsPECRoad = true;
			else
				bIsNormalRoad = true;
		}

		// �S��PEC�����N�Ȃ�PEC�m�[�h
		// ��ł�PEC�����N������Ȃ�PEC�����N�ڑ��m�[�h
		// �c���ꂽ���H�����N��3�{�ȏ��PEC�����N���Ȃ��Ȃ�����_�m�[�h
		sNodeClass = ( ! bIsPECRoad ) ? schema::road_node::node_class::kCross : ( ( bIsNormalRoad ) ? schema::road_node::node_class::kParking : schema::road_node::node_class::kPec );

		// �O�{�ȏ�̏ꍇ�͂����ŏI��
		// ��{�̏ꍇ�ŁA����L�����sindyeNodeIntersection�̏ꍇ�͂���ɒ��ׂ�
		if ( listConnectRoads.size() == 2 && schema::road_node::node_class::kCross == sNodeClass ) {
			road_link::CRoadLink* pRoad = listConnectRoads.front();
			std::list<road_link::CRoadLink*>::iterator it2 = listConnectRoads.begin(); ++it2;

			// �ڑ����铹�H�����N���قȂ鑮�������̂ł���Α����ω��_
			if ( !pRoad->IsSameAttribute( *(*it2) ) )
				sNodeClass = schema::road_node::node_class::kAlter;
			// �m�[�h�̌����_�E�����������݂āA����Α����t���m�[�h
			else if ( pbHasNodeAttr && *pbHasNodeAttr )
				sNodeClass = schema::road_node::node_class::kAttr;
			// �ڑ����铹�H�ŁA�^�[�Q�b�g�ȊO�̃m�[�hID��FROM�ETO�Ɏ����̂�
			// ����Γ��ꃊ���NID�h�~�m�[�h�ɂ���
			// �Ȃ���ΈӖ������m�[�h
			else {
				// �^�[�Q�b�g�ȊO�̃m�[�hID�����W����
				long lNodeOID = cRow->GetOID(), lOtherNodeID1 = -1, lOtherNodeID2 = -1; // �^�[�Q�b�g�ȊO�̃m�[�hID

				for ( std::list<road_link::CRoadLink*>::const_iterator it = listConnectRoads.begin(), itEnd = listConnectRoads.end(); it != itEnd; ++it ) {
					long lFromID = (*it)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 );
					long lToID = (*it)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 );

					if ( -1 == lOtherNodeID1 )
						lOtherNodeID1 = ( lNodeOID == lFromID ) ? lToID : lFromID;
					else
						lOtherNodeID2 = ( lNodeOID == lFromID ) ? lToID : lFromID;
				}

				// �^�[�Q�b�g�ȊO�̃m�[�hID�������̂��n����Ă��邩�ǂ����`�F�b�N
				bool bFind = false; // �����������ǂ����̃t���O

				for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
					long lFromID = (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 );
					long lToID = (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 );

					schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

					if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
						if( ( lOtherNodeID1 == lFromID && lOtherNodeID2 == lToID ) ||
							( lOtherNodeID2 == lFromID && lOtherNodeID1 == lToID ) )
						{
							bFind = true;
							break;
						}
					}
				}

				sNodeClass = ( bFind ) ? schema::road_node::node_class::kEvasion : (schema::road_node::node_class::ECode)0;
			}
		}
	}

	return sNodeClass;
}

bool CRoadNetwork::OnMeshEdgeNode( const CSPRow& cRow, std::list<road_link::CRoadLink*>& listConnectRoads, bool& hasErr ) const
{
	hasErr = false;
	if ( ! cRow )
	{
		hasErr = true;
		return false;
	}

	// ���ۂɐڑ�����Ă��铹�H����肷��
	for ( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); itRoad != itRoadEnd; ++itRoad ) {
		if ( (*itRoad)->GetLongValueByFieldName(schema::walk_link::kFromNodeID, -1 ) == cRow->GetOID() ||
			(*itRoad)->GetLongValueByFieldName(schema::walk_link::kToNodeID, -1 ) == cRow->GetOID() ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
				listConnectRoads.push_back( dynamic_cast<road_link::CRoadLink*>(itRoad->get()) );
		}
	}

	// �ڑ������N���`�F�b�N	
	if ( listConnectRoads.empty() ) 
	{
		hasErr = true;
		return false;
	}

	//////////////////////////////////////////
	// 2�����b�V�������ォ�ǂ�������
	//////////////////////////////////////////
	CMesh mesh;

	mesh.SetSpatialReference( cRow->GetShape()->GetSpatialReference() );	// �m�[�h�� SpatialReference ���Z�b�g���Ă��

	std::list<UINT> listMeshCode;

	if ( mesh.IsTouches( cRow->GetShapeCopy(), listMeshCode ) ) {
		// ���b�V����؂��PGDB�ł́A�m�[�h��2�����b�V���m�[�h�ł��Е��ɂ������H�����N���Ȃ�
		// ���̂��߂ɕK���G���[�ɂȂ��Ă��܂��̂ŁA���W�X�g�������ē��H�̖{���𖳎����邩�ǂ���
		// �����߂�
		if ( AheIsPGDBEditMode() ) {
			if ( 1 == listConnectRoads.size() )
			{
				return true;
			}
		} else {
			if ( 2 == listConnectRoads.size() ) {
				road_link::CRoadLink* pRoad = listConnectRoads.front();
				std::list<road_link::CRoadLink*>::iterator it2 = listConnectRoads.begin(); ++it2;

				if ( pRoad->IsSameAttribute( *(*it2) ) ) {
					return true;
				} else {
					// 2�����b�V�����E���ɐڂ��Ă���̂ɐڑ����铹�H�����N�̎�ʂ��قȂ�I�I
					_ASSERTE( false );
					hasErr = true;
				}
			} else {
				// 2�����b�V�����E���ɐڂ��Ă���̂ɐڑ����铹�H��2�{�ł͂Ȃ��I�I
				_ASSERTE( false );
				hasErr = true;
			}
		}
	}
	return false;
}

schema::walk_node::node_class::ECode CRoadNetwork::GetModifyWalkNodeKind( const CSPRow& cRow ) const
{
	// TODO : �قڂ��҂؂Ȃ̂ŁA�����Ə����������Ɓ�

	if ( ! cRow )
		return (schema::walk_node::node_class::ECode)-1;
	
	
	//////////////////////////////////////////
	// 2�����b�V�������ォ�ǂ�������
	//////////////////////////////////////////
	bool hasErr = false;
	std::list<road_link::CRoadLink*> listConnectRoads; // ���ۂɐڑ�����Ă��郊���N
	if( OnMeshEdgeNode( cRow, listConnectRoads, hasErr ) )
	{
		return hasErr ? (schema::walk_node::node_class::ECode)-1 : schema::walk_node::node_class::kOutline;
	}

	//////////////////////////////////////////
	// �����̎�ʂ��Ӑ}�I�ɕt�^���ꂽ��ʂ��ǂ����𔻒�
	//////////////////////////////////////////
	// ���̃m�[�h��ʂ��擾
	schema::walk_node::node_class::ECode sNodeClass = (schema::walk_node::node_class::ECode)cRow->GetLongValueByFieldName( schema::walk_node::kNodeClass, -1 );

	switch ( sNodeClass ) {
		case schema::walk_node::node_class::kRoadNW:
		case schema::walk_node::node_class::kGate:
		case schema::walk_node::node_class::kUndergroundGate:
		case schema::walk_node::node_class::kElevator:
			return sNodeClass;
			break;
		default: break;
	}

	//////////////////////////////////////////
	// �ڑ��󋵂Ƒ����Ŕ���
	//////////////////////////////////////////
	sNodeClass = schema::walk_node::node_class::kPublic;
	// ��{�̏ꍇ�͒[�_�m�[�h
	if( listConnectRoads.size() == 1 )
		sNodeClass = schema::walk_node::node_class::kTerminal;
	// ��{�ȏ�̏ꍇ
    else
	{
		// �O�{�ȏ�̏ꍇ�͂����ŏI��
		// ��{�̏ꍇ�͂���ɒ��ׂ�
		if( listConnectRoads.size() == 2 )
		{
			road_link::CRoadLink* pRoad = listConnectRoads.front();
			std::list<road_link::CRoadLink*>::iterator it2 = listConnectRoads.begin(); ++it2;

			// �ڑ����铹�H�����N���قȂ鑮�������̂ł���Α����ω��_
			if ( !pRoad->IsSameAttribute( *(*it2) ) )
				sNodeClass = schema::walk_node::node_class::kAlter;
		}
	}

	return sNodeClass;
}

schema::sj::subway_node::node_class::ECode CRoadNetwork::GetModifySubwayNodeKind( const CSPRow& cRow ) const
{
	using namespace schema::sj::subway_node::node_class;
	bool hasErr = false;
	std::list<road_link::CRoadLink*> listConnectRoads;
	if( OnMeshEdgeNode( cRow, listConnectRoads, hasErr ) && !hasErr)
	{
		return kOutline;
	}
	return hasErr ? (ECode)-1 : kStation;
}

sindy::schema::sj::railway_node::node_class::ECode CRoadNetwork::GetModifyRailwayNodeKind( const CSPRow& cRow ) const
{
	using namespace schema::sj::railway_node::node_class;
	bool hasErr = false;
	std::list<road_link::CRoadLink*> listConnectRoads;
	if( OnMeshEdgeNode( cRow, listConnectRoads, hasErr ) && !hasErr )
		return kOutline;

	// �G���[���������Ȃ�G���[�p�R�[�h��Ԃ�
	if(hasErr) return (ECode)-1;

	// �wPOI�m�[�h���ݒ肳��Ă���̂ł���Α����͕ύX���Ȃ�
	// ���̃m�[�h��ʂ��擾
	ECode sNodeClass = (ECode)cRow->GetLongValueByFieldName( schema::sj::railway_node::kNodeClass_C, -1 );
	if( sNodeClass == kStation )
		return kStation;

	// ��{�̏ꍇ�͒[�_�m�[�h
	if( listConnectRoads.size() == 1 )
		return kTerminal;

	// �O�{�ȏ�̏ꍇ�͕���m�[�h
	if( listConnectRoads.size() >= 3 )
		return kBranch;

	// ��{�̏ꍇ�͐ڑ�����Ă��郊���N�̑����ɂ���Ď�ʂ�ς���
	road_link::CRoadLink* pRoad = listConnectRoads.front();
	std::list<road_link::CRoadLink*>::iterator itRoad2 = listConnectRoads.begin();
	std::advance(itRoad2, 1);

	// �قȂ鑮�������̂ł���Α����ω��_
	//�i���\�[�X�⎩�����͂����悤�ȑ����͔�r�Ώۂ��珜���j
	if( !pRoad->IsSameAttribute( *(*itRoad2) ) )
		return kAlter;

	// �������������̂ł���΋@�\�Ȃ��m�[�h
	return kNone;
}

}// sindy
