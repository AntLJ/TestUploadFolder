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
 * @file EditMeshTable.cpp
 * @breif <b>CEditMeshTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "EditMeshTable.h"
#include "meshutil.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace editmesh {

// �e�[�u����S�ăL���b�V������
long CEditMeshTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CEditMeshTable::CreateTableCache() : Started.\n") );

	CEditMesh cEditMesh;
	_ICursorPtr ipCursor;
	if( 0 > lPurposeID )
		ipCursor = Search( NULL, bDetatch );
	else
		ipCursor = Search( AheInitQueryFilter( NULL, _T("*"), _T("PURPOSE_ID=%d"), lPurposeID ), bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cEditMesh.Release() ) ) )
	{
		if( cEditMesh == NULL ) break;

		// ��ƖړIID�ƃ��C����ID�ƃI�y���[�^��ID���L�[�ɂ��Ċi�[
		long lPurposeID = cEditMesh.GetPurposeID();
		long lLayerNameID = cEditMesh.GetLayerNameID();
		long lOperatorID = cEditMesh.GetOperatorID();

		// ���ɂ��邩�ǂ����`�F�b�N
		bool bFind = false;
		std::map<long, std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > > >::iterator it1 = m_mapCache.find( lPurposeID );
		if( it1 != m_mapCache.end() )
		{
			std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > >::iterator it2 = it1->second.find( lLayerNameID );
			if( it2 != it1->second.end() )
			{
				std::map<long, std::pair<CGeometry, std::list<CEditMesh> > >::iterator it3 = it2->second.find( lOperatorID );
				if( it3 != it2->second.end() )
				{
					it3->second.second.push_front( cEditMesh );
					it3->second.second.begin()->CreateCache( bDetatch, spFields, spNameString );
					it3->second.first.Union( MESH2POLY( cEditMesh.GetMeshCode(), NULL ) );

					bFind = true;
				}
			}
		}

		// �Ȃ���΁A�V�K�o�^
		if( ! bFind )
		{
			std::pair<CGeometry, std::list<CEditMesh> > pairEditMeshes;
			pairEditMeshes.first = MESH2POLY( cEditMesh.GetMeshCode(), NULL );
			m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ] = pairEditMeshes;
			m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].second.push_front( cEditMesh );
			m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].second.begin()->CreateCache( bDetatch, spFields, spNameString );
		}
/*	
		m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].second.push_front( cEditMesh );
		m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].second.begin()->CreateCache( bDetatch, ipFields );
		// ���b�V���R�[�h����`�ɂ��ă}�[�W
		m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].first.Union( MESH2POLY( cEditMesh.GetMeshCode(), NULL ) );*/
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CEditMeshTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// �L���b�V�����N���A����
void CEditMeshTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

// �ҏW�\�ȃG���A���ǂ������`�F�b�N����
bool CEditMeshTable::IsEditable( long lPurposeID, long lLayerNameID, long lOperatorID, IGeometry* ipGeom ) const
{
	// ��ƃG���A���o�^����Ă���Ȃ�
	// ������ŗD��Ō���
	// �����ʑΉ��A�s�s���L������́APGDB�͈͊O�ɂ������邽�߉��̃`�F�b�N�����Ȃ�
	// 26�Œ�ł����́H���Ɗ�Ȃ��A�A
	if( lLayerNameID != 26 )
	{
		if( (IGeometry*)m_cGlobalWorkArea )
		{
			IGeometryPtr ipGlobalWorkAreaCopy( AheCreateClone( (IGeometry*)m_cGlobalWorkArea ) );
			if( ipGlobalWorkAreaCopy != NULL )
			{
				// GlobalWorkArea�͐������ԎQ�Ƃ���œ����Ă���̂ŁA
				// �����ŋ�ԎQ�Ƃ������Ƃ��킹�Ȃ��Ɖ��̕����ŕҏW
				// �ł��Ȃ��Ȃ�
				if( ipGeom )
				{
					ISpatialReferencePtr ipSpRef;
					ipGeom->get_SpatialReference( &ipSpRef );
					ipGlobalWorkAreaCopy->putref_SpatialReference( ipSpRef );
					ipGlobalWorkAreaCopy->SnapToSpatialReference();

					// [Bug 8164][e]SiNDY-c�g�p����PGDB�ҏW�Ń��b�V�����E��̃m�[�h���ړ��ł��Ȃ�
					// ���b�V�����E��̃|�C���g�́AContain���ᔻ��ł��Ȃ��̂ŁADisjoint��
					esriGeometryType type = esriGeometryAny;
					ipGeom->get_GeometryType( &type );
					if( type == esriGeometryPoint )
					{
						CGeometry cGeom( ipGlobalWorkAreaCopy );
						if( cGeom.Disjoint( ipGeom ) )
							return false;
					}
					else
					{
						if( ! AheGeometryContains( ipGlobalWorkAreaCopy, ipGeom ) )
							return false;
					}
				}
			}
		}
	}

	// �����`�F�b�N
	if( 0 > lOperatorID )	// [bug 3094] �I�y���[�^���o�^����Ă��Ȃ���ΑS�ĕҏW�\�ł���
		return true;
	else
	{
		std::map<long, std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > > >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
		if( itByPurposeID != m_mapCache.end() )
		{
			std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > >::const_iterator itByLayerNameID = itByPurposeID->second.find( lLayerNameID );
			if( itByLayerNameID != itByPurposeID->second.end() )
			{
				std::map<long, std::pair<CGeometry, std::list<CEditMesh> > >::const_iterator itByOperatorID = itByLayerNameID->second.find( lOperatorID );
				if( itByOperatorID != itByLayerNameID->second.end() )
					return itByOperatorID->second.first.Contains( ipGeom );
			}
		}
	}

	return false;
}

// ��Ɖ\�G���A��ݒ肷��
void CEditMeshTable::SetGlobalWorkArea( IGeometry* ipGeom )
{
	if( ! ipGeom )
		m_cGlobalWorkArea = NULL;
	else
		m_cGlobalWorkArea = (IGeometryPtr)AheCreateClone( ipGeom );
}

// ��Ɖ\�G���A���擾����
const CGeometry& CEditMeshTable::GetGlobalWorkArea() const
{
	return m_cGlobalWorkArea;
}

} // editmesh

} // sindy
