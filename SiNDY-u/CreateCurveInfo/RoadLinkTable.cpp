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

#include <boost/assign/list_of.hpp>
#include "RoadLinkTable.h"
#include "RowContainer.h"
#include "GeometryBase.h"
#include "ClassFactory.h"

namespace sindy {

using namespace schema;

CSPTableContainer _TableClassFactory2( const CTableFinder& cFinder, ITablePtr& ipTable )
{
	CSPTableContainer cTable; // �߂�l

	CString strTableName( AheGetFeatureClassName( ipTable ) );
	strTableName.MakeUpper(); // �啶����

	if( strTableName == road_link::kTableName )
		cTable.reset( new CRoadLinkTable( ipTable, cFinder ) );
	else
		cTable.reset( new CTableContainer( ipTable, cFinder ) );

	return cTable;
}

// ���C�u����������������
void init_sindynlib()
{
	TableClassFactory = _TableClassFactory2;
}

// ���C�u�����̌�n��������
void uninit_sindynlib()
{
}

// ���H�l�b�g���[�N�p�̃��f�����쐬����
void GetSindynModel( CModel& model )
{
	model.Add( road_link::kTableName, kObjectID, NULL, NULL );
	BOOST_FOREACH( LPCTSTR lpcszLQ, linkqueuetype2lqtablename( linkqueue_type::kAll ) )
	{
		model.Add( lpcszLQ,					link_queue::kLinkID,	road_link::kTableName,	kObjectID				);
		model.Add( lq2inf(lpcszLQ),			kObjectID,				lpcszLQ,				link_queue::kInfID		);
	}
}

/** 
 * @biref linkqueue_type ���� LQ�e�[�u�����̃��X�g�ɕϊ�����
 */
std::list<LPCTSTR> linkqueuetype2lqtablename( linkqueue_type::ECode emCode )
{
	std::list<LPCTSTR> ret; // �߂�l
	for( int i = linkqueue_type::kLRTurn; i <= linkqueue_type::kAll; i = i*2 )
	{
		LPCTSTR lpcszName = NULL;
		switch( (linkqueue_type::ECode)(i & emCode) )
		{
#define LINK_QUEUE(inf,lq,type) case type: lpcszName = lq; break;
#include "define_linkqueue.h"
#define LINK_QUEUE(inf,lq,type)
		default: break;
		}
		if( lpcszName )
			ret.push_back( lpcszName );
	}
	return ret;
}

/**
 * @brief LQ�e�[�u��������INF�e�[�u�����ɕϊ�����
 */
LPCTSTR lq2inf( LPCTSTR lpcszLQ )
{
#define LINK_QUEUE(inf,lq,type) if( 0 == lstrcmpi( lpcszLQ, lq ) ) return inf;
#include "define_linkqueue.h"
#define LINK_QUEUE(inf,lq,type)
	assert( false );
	return NULL;
}

/**
 * @brief INF�e�[�u��������LQ�e�[�u�����ɕϊ�����
 */
LPCTSTR inf2lq( LPCTSTR lpcszInf )
{
#define LINK_QUEUE(inf,lq,type) if( 0 == lstrcmpi( lpcszInf, inf ) ) return lq;
#include "define_linkqueue.h"
#define LINK_QUEUE(inf,lq,type)
	assert( false );
	return NULL;
}

// sort�p�v���f�B�P�[�g�iLQ�̃V�[�P���X���Ƀ\�[�g�j
bool CRoadLinkTable::LessLQSequence::operator()( CSPContainerBase& c1, CSPContainerBase& c2 ) const
{
	if( ! ( c1 && c2 ) )
		return false;

	return CAST_ROW(c1)->GetValue( link_queue::kSequence, -1L ) 
		< CAST_ROW(c2)->GetValue( link_queue::kSequence, -1L );
}

// ���H�����N��OBJECTID�Ŋ֘A�I�u�W�F�N�g����������
void CRoadLinkTable::Select( const std::list<long>& listOID, bool detach/* = false*/, bool unique/* = false*/, linkqueue_type::ECode emType/* = linkqueue_type::kAll*/, bool/* as_start = false*/, long/* start_node_id = 0*/ )
{
	if( ! IsConnected() )
		return;

	// ���H�����N�̌���
	// ���f����q���Ƃ��Ă�����̂������ňꏏ�Ɍ��������
	__super::Select( listOID, kObjectID, detach, unique );

	// ����Ă��Ȃ�LQ�̌���
	SelectLQs( detach, unique, emType );
}

void CRoadLinkTable::SelectLQs( bool detach/* = false*/, bool unique/* = false*/, linkqueue_type::ECode emType/* = linkqueue_type::kAll*/ )
{
	BOOST_FOREACH( LPCTSTR lpcszLQTable, linkqueuetype2lqtablename( emType ) )
	{
		std::list<long> listInfID; // �擾�ς�INF OID
		LPCTSTR lpcszInfTable = lq2inf(lpcszLQTable); // INF�e�[�u����

		// �i�[�pINF�e�[�u���N���X�擾
		// �e�[�u���t�@�C���_�ɓ����Ă���R���e�i�Ɋi�[����
		// �{����INF�������ɓ���Ă��܂��Ă������̂����A�Ƃ肠����
		CTableContainer& cInfTable( *FindTable( lpcszInfTable ) ); // �g�b�v���x���ɂ���INF�p�R���e�i
		// �����ς�INF�����W�y�уg�b�v���x���̃R���e�i�Ɋi�[
		BOOST_FOREACH( CSPContainerBase& con_link, GetContainer() )
		{
			CRowContainer& link( *CAST_ROW(con_link) );
			BOOST_FOREACH( CSPContainerBase& con_lq, *link.GetItemTable( lpcszLQTable ) )
			{
				CRowContainer& lq( *CAST_ROW(con_lq) );
				BOOST_FOREACH( CSPContainerBase& con_inf, *lq.GetItemTable( lpcszInfTable ) )
				{
					cInfTable.push_back( con_inf, true ); // �d�����Ȃ��悤�Ɋi�[
					listInfID.push_back( CAST_ROW(con_inf)->GetOID() ); // OID�擾
				}
			}
		}
		listInfID.sort();
		listInfID.unique();

		// INF�Ɋ֘A����LQ������
		CSPTableContainer spLqTable( FindTable( lpcszLQTable )->CreateEmptyClone() );
		CTableContainer& cLqTable( *spLqTable );
		cLqTable._Select( listInfID, link_queue::kInfID, detach, unique );
		// ��������LQ�����ꂼ���INF�ɐU�蕪��
		BOOST_FOREACH( CSPContainerBase& con, std::make_pair( cLqTable.begin(), cLqTable.end() ) )
		{
			CRowContainer& lq( *CAST_ROW(con) );
			CContainerBase::iterator itInf = cInfTable.find_by_key( kObjectID, lq.GetValue( link_queue::kInfID ) );
			if( cInfTable.end() == itInf )
			{
				// �����ɗ���Ƃ������Ƃ́ALQ�ɑΉ�����INF��DB��ɑ��݂��Ȃ���
				// LQ�̎q���Ƃ���INF���o�^����Ă��Ȃ���
				// -> TODO: ���f����LQ�̎q���Ƃ���INF���o�^����Ă��Ȃ��ꍇ�ɂ͂����̏������΂��������ق����悢
				assert( cInfTable.end() != itInf );
				continue; // ���傤���Ȃ��̂Ŕ�΂�
			}
			CRowContainer& inf( *CAST_ROW(*itInf) );
			CSPContainerBase con_inf_lq( inf.GetItemTable( lpcszLQTable ) ); // �g�b�v���x����INF�������Ă���LQ�̃R���e�i
			// ���f����INF�̎q���Ƃ���LQ���o�^����Ă��Ȃ���GetItemTable�ł��Ȃ�
			// �{���Ȃ�ꎞ�I�Ƀ��f����ύX���Č�������΍ςޘb�Ȃ̂����A�ʓ|�Ȃ̂�
			// �����ꍇ�͂����ŃR���e�i��ǉ�����
			if( ! con_inf_lq )
			{
				inf.push_back( CSPTableContainer( FastTableClassFactory( GetFinder(), lpcszLQTable ) ) );
				con_inf_lq = *inf.rbegin();
			}
			CAST_ROW(*itInf)->GetItemTable( lpcszLQTable )->push_back( con, unique );
		}
		// �ǉ�����LQ���\�[�g
		BOOST_FOREACH( CSPContainerBase& con_inf, std::make_pair( cInfTable.begin(), cInfTable.end() ) )
			CAST_ROW(con_inf)->GetItemTable( lpcszLQTable )->sort( LessLQSequence() );
	}
}

// LQ�AINF�̌`���⊮����
void CRoadLinkTable::SelectLQShape( linkqueue_type::ECode emType/* = linkqueue_type::kAll*/ )
{
	// ���H�����N��ID�����W����
	std::list<LPCTSTR> listLQName( linkqueuetype2lqtablename(emType) ); // LQ�e�[�u����
	std::list<long> listLinkID; // ���H��OID�i�����p�j
	BOOST_FOREACH( LPCTSTR lpcszLQTable, listLQName )
	{
		LPCTSTR lpcszInfTable = lq2inf(lpcszLQTable); // INF�e�[�u����
		CTableContainer& cInfTable( *FindTable( lpcszInfTable ) ); // �g�b�v���x���ɂ���INF�p�R���e�i
		BOOST_FOREACH( CSPContainerBase& con_inf, std::make_pair( cInfTable.begin(), cInfTable.end() ) )
		{
			CRowContainer& inf( *CAST_ROW(con_inf) );
			CTableContainer& cLqTable( *inf.GetItemTable( lpcszLQTable ) );
			BOOST_FOREACH( CSPContainerBase& con_lq, std::make_pair( cLqTable.begin(), cLqTable.end() ) )
				listLinkID.push_back( CAST_ROW(con_lq)->GetValue( link_queue::kLinkID, -1L ) );
		}
	}
	listLinkID.sort();
	listLinkID.unique();

	// ���H����������
	CSPTableContainer linkT( CreateEmptyClone() );
	linkT->_Select( listLinkID, kObjectID, _T("OBJECTID,SHAPE,SHAPE.LEN"), false, false ); // �`�󂾂�
	// �`��擾
	std::map<long,IGeometryPtr> mapLinkShape; // key: ���HOID value: ���H�`��
	BOOST_FOREACH( CSPContainerBase& con_link, std::make_pair( linkT->begin(), linkT->end() ) )
	{
		CRowContainer& link( *CAST_ROW(con_link) );
		mapLinkShape[link.GetOID()] = link.CRowBase::GetShapeCopy();
	}

	// ���H�`���LQ�Ɋ��蓖��
	BOOST_FOREACH( LPCTSTR lpcszLQTable, listLQName )
	{
		LPCTSTR lpcszInfTable = lq2inf(lpcszLQTable); // INF�e�[�u����
		CTableContainer& cInfTable( *FindTable( lpcszInfTable ) ); // �g�b�v���x���ɂ���INF�p�R���e�i
		BOOST_FOREACH( CSPContainerBase& con_inf, std::make_pair( cInfTable.begin(), cInfTable.end() ) )
		{
			IGeometryPtr inf_shape; // �����N��S�̂̌`��
			CRowContainer& inf( *CAST_ROW(con_inf) );
			CTableContainer& cLqTable( *inf.GetItemTable( lpcszLQTable ) );
			BOOST_FOREACH( CSPContainerBase& con_lq, std::make_pair( cLqTable.begin(), cLqTable.end() ) )
			{
				CRowContainer& lq( *CAST_ROW(con_lq) );
				// LQ�Ɍ`��Z�b�g
				lq.SetShape( mapLinkShape[lq.GetValue( link_queue::kLinkID, -1L )] );
				// INF�p�̌`��쐬
				if( ! inf_shape )
				{
					inf_shape = lq.CRowBase::GetShapeCopy();
					// AheUnionNetwork��base���̕����ł��킹��̂ŁA
					// �������������ɂ��Ă���
					if( lq.GetValue( link_queue::kLinkDir, -1L ) == link_queue::link_dir::kReverse )
						IPolylinePtr(inf_shape)->ReverseOrientation();
				}
				else {
					CGeometryBase& geom( *lq.GetShape() );
					inf_shape = AheUnionNetwork( inf_shape, (IGeometry*)geom, geom.GetSpatialReference()  );
				}
			}
			// INF�Ɍ`��Z�b�g
			inf.SetShape( inf_shape );
		}
	}
}

} // sindy
