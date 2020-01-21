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
#include "AheNetwork.h"
#include "aheglobals.h"
#include "TargetFeatures.h"
#include "graph.h"
#include "RuleModel.h"
#include <iterator>

using namespace sindy::schema;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �l�b�g���[�N���f���̃��C���t�B�[�`���N���X���ǂ����𔻒肷��
bool AheIsNetworkLink( LPCTSTR lpcszTableName )
{
	for( CRuleModelNetwork::const_iterator it = g_cRuleModelNetwork.begin(); it != g_cRuleModelNetwork.end(); ++it )
	{
		if( 0 == lstrcmpi( lpcszTableName, it->first ) ) // link
			return true;
	}
	return false;
}
bool AheIsNetworkLink( ILayer* ipLayer )
{
	return AheIsNetworkLink( AheGetFeatureClassName( ipLayer ) );
}
bool AheIsNetworkLink( IFeatureClass* ipFeatureClass )
{
	return AheIsNetworkLink( AheGetFeatureClassName( ipFeatureClass ) );
}
bool AheIsNetworkLink( IFeature* ipFeature )
{
	return AheIsNetworkLink( AheGetFeatureClassName( ipFeature ) );
}

// ADAM�l�b�g���[�N���f���̃��C���t�B�[�`���N���X���ǂ����𔻒肷��
bool AheIsAdamNetworkLink(LPCTSTR lpcszTableName)
{
	using namespace adam_v2;

	return
		0 == lstrcmpi(lpcszTableName, lane_link::kTableName) ||
		0 == lstrcmpi(lpcszTableName, updown_link::kTableName) ||
		0 == lstrcmpi(lpcszTableName, compart_link::kTableName) ||
		0 == lstrcmpi(lpcszTableName, border_link::kTableName);
}
bool AheIsAdamNetworkLink(ILayer* ipLayer)
{
	return AheIsAdamNetworkLink(AheGetFeatureClassName(ipLayer));
}
bool AheIsAdamNetworkLink(IFeatureClass* ipFeatureClass)
{
	return AheIsAdamNetworkLink(AheGetFeatureClassName(ipFeatureClass));
}
bool AheIsAdamNetworkLink(IFeature* ipFeature)
{
	return AheIsAdamNetworkLink(AheGetFeatureClassName(ipFeature));
}


// �l�b�g���[�N���f���̃|�C���g�t�B�[�`���N���X���ǂ����𔻒肷��
bool AheIsNetworkNode( LPCTSTR lpcszTableName )
{
	for( CRuleModelNetwork::const_iterator it = g_cRuleModelNetwork.begin(); it != g_cRuleModelNetwork.end(); ++it )
	{
		if( 0 == lstrcmpi( lpcszTableName, it->second ) ) // point
			return true;
	}
	return false;
}
bool AheIsNetworkNode( ILayer* ipLayer )
{
	return AheIsNetworkNode( AheGetFeatureClassName( ipLayer ) );
}
bool AheIsNetworkNode( IFeatureClass* ipFeatureClass )
{
	return AheIsNetworkNode( AheGetFeatureClassName( ipFeatureClass ) );
}
bool AheIsNetworkNode( IFeature* ipFeature )
{
	return AheIsNetworkNode( AheGetFeatureClassName( ipFeature ) );
}

// ADAM�l�b�g���[�N���f���̃|�C���g�t�B�[�`���N���X���ǂ����𔻒肷��
bool AheIsAdamNetworkNode(LPCTSTR lpcszTableName)
{
	using namespace adam_v2;

	return
		0 == lstrcmpi(lpcszTableName, lane_node::kTableName) ||
		0 == lstrcmpi(lpcszTableName, updown_node::kTableName) ||
		0 == lstrcmpi(lpcszTableName, compart_node::kTableName) ||
		0 == lstrcmpi(lpcszTableName, border_node::kTableName);
}
bool AheIsAdamNetworkNode(ILayer* ipLayer)
{
	return AheIsAdamNetworkNode(AheGetFeatureClassName(ipLayer));
}
bool AheIsAdamNetworkNode(IFeatureClass* ipFeatureClass)
{
	return AheIsAdamNetworkNode(AheGetFeatureClassName(ipFeatureClass));
}
bool AheIsAdamNetworkNode(IFeature* ipFeature)
{
	return AheIsAdamNetworkNode(AheGetFeatureClassName(ipFeature));
}

// �l�b�g���[�N���f�����ǂ������肷��
bool AheIsNetwork( LPCTSTR lpcszTableName )
{
	return ( AheIsNetworkLink( lpcszTableName ) || 
		     AheIsNetworkNode( lpcszTableName ) );
}
bool AheIsNetwork( ILayer* ipLayer )
{
	return AheIsNetwork( AheGetFeatureClassName( ipLayer ) );
}
bool AheIsNetwork( IFeatureClass* ipFeatureClass )
{
	return AheIsNetwork( AheGetFeatureClassName( ipFeatureClass ) );
}
bool AheIsNetwork( IFeature* ipFeature )
{
	return AheIsNetwork( AheGetFeatureClassName( ipFeature ) );
}

// ADAM�l�b�g���[�N���f�����ǂ������肷��
bool AheIsAdamNetwork(LPCTSTR lpcszTableName)
{
	return (AheIsAdamNetworkLink(lpcszTableName) ||
		AheIsAdamNetworkNode(lpcszTableName));
}
bool AheIsAdamNetwork(ILayer* ipLayer)
{
	return AheIsAdamNetwork(AheGetFeatureClassName(ipLayer));
}
bool AheIsAdamNetwork(IFeatureClass* ipFeatureClass)
{
	return AheIsAdamNetwork(AheGetFeatureClassName(ipFeatureClass));
}
bool AheIsAdamNetwork(IFeature* ipFeature)
{
	return AheIsAdamNetwork(AheGetFeatureClassName(ipFeature));
}

///�l�b�g���[�N���f���^�C�v����
sindyeNetworkModel AheGetNetWorkType( LPCTSTR lpcszTableName )
{
	CString table = AheGetNetworkLinkFeatureClassName( lpcszTableName );
	if( 0 == table.CompareNoCase( road_link::kTableName ) )
		return sindyeNetworkRoad;
	
	if( 0 == table.CompareNoCase( walk_link::kTableName ) )
		return sindyeNetworkWalk;

	if( 0 == table.CompareNoCase( sj::subway_link::kTableName ) )
		return sindyeNetworkSubway;

	if( 0 == table.CompareNoCase( sj::railway_link::kTableName ) )
		return sindyeNetworkRailway;

	if( 0 == table.CompareNoCase( adam_v2::lane_link::kTableName ) )
		return sindyeNetworkLane;

	if( 0 == table.CompareNoCase( adam_v2::updown_link::kTableName ) )
		return sindyeNetworkUpDown;

	if( 0 == table.CompareNoCase( adam_v2::compart_link::kTableName ) )
		return sindyeNetworkCompart;

	if( 0 == table.CompareNoCase( adam_v2::border_link::kTableName ) )
		return sindyeNetworkBorder;

	return sindyeNotNetwork;
}
sindyeNetworkModel AheGetNetWorkType( ILayer* ipLayer )
{
	return AheGetNetWorkType( AheGetFeatureClassName( ipLayer ) );
}
sindyeNetworkModel AheGetNetWorkType( IFeatureClass* ipFeatureClass )
{
	return AheGetNetWorkType( AheGetFeatureClassName( ipFeatureClass ) );
}
sindyeNetworkModel AheGetNetWorkType( IFeature* ipFeature )
{
	return AheGetNetWorkType( AheGetFeatureClassName( ipFeature ) );
}

// �l�b�g���[�N���f���Ɋ֘A���邩�ǂ������肷��
bool AheIsRelateWithNetwork( LPCTSTR lpcszTableName )
{
	// �l�b�g���[�N���f�����̂��̂��ǂ���
	if( AheIsNetwork( lpcszTableName ) )
		return true;

	// �l�b�g���[�N���f���Ɋ֘A������̂��ǂ���
	for( CRuleModelNetwork::const_iterator it = g_cRuleModelNetwork.begin(); it != g_cRuleModelNetwork.end(); ++it )
	{
		for( CRuleRelationshipContain::const_value_iterator it2 = g_cRuleRelationshipContain.begin( it->first ); it2 != g_cRuleRelationshipContain.end( it->first ); ++it2 )
		{
			if( 0 == lstrcmpi( lpcszTableName, it2->first ) )
				return true;
		}
	}
	// �����N��n���ǂ���
	for( CRuleLinkQueue::const_iterator it = g_cRuleLinkQueue.begin(); it != g_cRuleLinkQueue.end(); ++it )
	{
		for( CRuleLinkQueue::TRuleValue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( 0 == it2->get<0>().CompareNoCase( lpcszTableName ) ||
				0 == it2->get<1>().CompareNoCase( lpcszTableName ) )
				return true;
		}
	}
	return false;
}
bool AheIsRelateWithNetwork( ILayer* ipLayer )
{
	return AheIsRelateWithNetwork( AheGetFeatureClassName( ipLayer ) );
}
bool AheIsRelateWithNetwork( IFeatureClass* ipFeatureClass )
{
	return AheIsRelateWithNetwork( AheGetFeatureClassName( ipFeatureClass ) );
}
bool AheIsRelateWithNetwork( IFeature* ipFeature )
{
	return AheIsRelateWithNetwork( AheGetFeatureClassName( ipFeature ) );
}

// �l�b�g���[�N�����N�E�m�[�h�t�B�[�`���N���X����Ԃ�
CString _AheGetNetworkFeatureClassName( LPCTSTR lpcszTableName, bool bIsLinkName )
{
	CString strRet; // �Ԃ�l

	if( ! lpcszTableName )
		return strRet;

	// �n���ꂽ�e�[�u�������l�b�g���[�N�����N�����ǂ���
	if( AheIsNetworkLink( lpcszTableName ) )
		strRet = ( bIsLinkName ) ? lpcszTableName : g_cRuleModelNetwork.GetPairName( lpcszTableName );
	// �n���ꂽ�e�[�u�������l�b�g���[�N�m�[�h�����ǂ���
	else if( AheIsNetworkNode( lpcszTableName ) )
		strRet = ( bIsLinkName ) ? g_cRuleModelNetwork.GetPairName( lpcszTableName ) : lpcszTableName;
	// �n���ꂽ�e�[�u�������l�b�g���[�N���f���Ɋ֘A����e�[�u�������ǂ���
	else if( AheIsRelateWithNetwork( lpcszTableName ) )
	{
		// ����֌W����T��
		for( CRuleRelationshipContain::const_iterator it = g_cRuleRelationshipContain.begin(); it != g_cRuleRelationshipContain.end(); ++it )
		{
			for( CRuleRelationshipContain::TRuleValue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				if( 0 == it2->first.Compare( lpcszTableName ) )
				{
					strRet = ( bIsLinkName ) ? it->first : g_cRuleModelNetwork.GetPairName( it->first );
					break;
				}
			}
			if( ! strRet.IsEmpty() )
				break;
		}
		if( strRet.IsEmpty() )
		{
			// �����N�񂩂�T��
			for( CRuleLinkQueue::const_iterator it = g_cRuleLinkQueue.begin(); it != g_cRuleLinkQueue.end(); ++it )
			{
				for( CRuleLinkQueue::TRuleValue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
				{
					if( 0 == it2->get<0>().CompareNoCase( lpcszTableName ) ||
						0 == it2->get<1>().CompareNoCase( lpcszTableName ) )
					{
						strRet = ( bIsLinkName ) ? it->first : g_cRuleModelNetwork.GetPairName( it->first );
						break;
					}
				}
			}
		}
	}
	
	return strRet;
}

/// �l�b�g���[�N�����N�t�B�[�`���N���X����Ԃ�
CString AheGetNetworkLinkFeatureClassName( LPCTSTR lpcszTableName )
{
	return _AheGetNetworkFeatureClassName( lpcszTableName, true );
}

/// �l�b�g���[�N�m�[�h�t�B�[�`���N���X����Ԃ�
CString AheGetNetworkNodeFeatureClassName( LPCTSTR lpcszTableName )
{
	return _AheGetNetworkFeatureClassName( lpcszTableName, false );
}

// �l�b�g���[�N���f���ł̊֘A�e�[�u�����ꗗ���擾����
CString AheGetNetworkTarget( LPCTSTR lpcszTableName )
{
	// �֘A�e�[�u�����̓X�v���b�g�����e�[�u�����ꗗ�{�l�b�g���[�N�m�[�h
	CString strSplitTarget( AheGetNetworkSplitTarget( lpcszTableName ) );
	CString strRet;
	strRet.Format(_T("%s%s|"), strSplitTarget, AheGetNetworkNodeFeatureClassName( lpcszTableName ) );
	return strRet;
}

// �l�b�g���[�N���f���ł̃X�v���b�g�����ɕK�v�ȃ��C���n�e�[�u�����ꗗ���擾����
CString AheGetNetworkSplitTarget( LPCTSTR lpcszTableName, bool bContainNW )
{
	// �n���ꂽ�e�[�u�������l�b�g���[�N���f���̂��̂Ȃ�A���̃��f���̃��C����
	// ���̃��C���Ɠ���֌W�ɂ��郉�C�������������Ă��āu|�v�ŋ�؂��ĕԂ�
	CString strRet; // �Ԃ�l

	// �܂��A�l�b�g���[�N���f���̃��C������肷��
	CString strNetworkLink( AheGetNetworkLinkFeatureClassName( lpcszTableName ) );
	if( strNetworkLink.IsEmpty() )
		return strRet; // ������Ȃ�����

	if(bContainNW)
		strRet.Format(_T("|%s"), strNetworkLink ); // �ǉ�

	// ����郉�C���t�B�[�`���N���X��ǉ�����
	for( CRuleRelationshipContain::const_iterator it = g_cRuleRelationshipContain.begin(); it != g_cRuleRelationshipContain.end(); ++it )
	{
		if( 0 == it->first.Compare( strNetworkLink ) )
		{
			for( CRuleRelationshipContain::TRuleValue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
				strRet.AppendFormat(_T("|%s"), *it2 ); // �ǉ�
		}
	}

	// ��łȂ��Ȃ� | �����
	if( !strRet.IsEmpty() ) strRet.Append(_T("|"));
	return strRet;
}

// �l�b�g���[�N�����N�t�B�[�`���N���X���̈ꗗ���擾����
CString _AheGetNetworkNames( bool bIsLink )
{
	CString strRet; // �Ԃ�l
	for( CRuleModelNetwork::const_iterator it = g_cRuleModelNetwork.begin(); it != g_cRuleModelNetwork.end(); ++it )
	{
		if( strRet.IsEmpty() )
			strRet.Format(_T("|%s|"), ( bIsLink ) ? it->first : it->second );
		else {
			CString strRetTmp( strRet );
			strRet.Format(_T("%s%s|"), strRetTmp, ( bIsLink ) ? it->first : it->second );
		}
	}
	return strRet;
}

// �l�b�g���[�N�����N�t�B�[�`���N���X���̈ꗗ���擾����
CString AheGetNetworkLinks()
{
	return _AheGetNetworkNames( true );
}

// �l�b�g���[�N�m�[�h�t�B�[�`���N���X���̈ꗗ���擾����
CString AheGetNetworkNodes()
{
	return _AheGetNetworkNames( false );
}

// �l�b�g���[�N�����N�t�B�[�`���N���X��Ԃ�
IFeatureClassPtr AheGetNetworkLinkFeatureClass( IFeature* ipFeature )
{
	return AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), AheGetNetworkLinkFeatureClassName( AheGetFeatureClassName( ipFeature ) ) );
}

// �l�b�g���[�N�m�[�h�t�B�[�`���N���X��Ԃ�
IFeatureClassPtr AheGetNetworkNodeFeatureClass( IFeature* ipFeature )
{
	return AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), AheGetNetworkNodeFeatureClassName( AheGetFeatureClassName( ipFeature ) ) );
}

// �n���ꂽ�e�[�u�����ɕt������֘A�t���O�t�B�[���h���̃��X�g���擾����
std::list<CString> AheGetRelationFlagFieldName( LPCTSTR lpcszTableName )
{
	std::list<CString> listRet; // �Ԃ�l

	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin( lpcszTableName ); it != g_cRuleRelationTable.end( lpcszTableName ); ++it )
		listRet.push_back( it->get<0>() );

	return listRet;
}

// �֘A�t���O�t�B�[���h���ƑΉ�����O���e�[�u���̊O���L�[�t�B�[���h�����擾����
CString AheGetRelationOuterKeyFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszRelFieldName )
{
	CString strRet; // �Ԃ�l
	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin( lpcszTableName ); it != g_cRuleRelationTable.end( lpcszTableName ); ++it )
	{
		if( 0 == it->get<0>().CompareNoCase( lpcszRelFieldName ) )
		{
			strRet = it->get<2>();
			break;
		}
	}
	return strRet;
}

// �֘A�t���O�t�B�[���h���ƑΉ�����O���e�[�u�������擾����
CString AheGetRelationTableName( LPCTSTR lpcszTableName, LPCTSTR lpcszRelFieldName )
{
	CString strRet; // �Ԃ�l
	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin( lpcszTableName ); it != g_cRuleRelationTable.end( lpcszTableName ); ++it )
	{
		if( 0 == it->get<0>().CompareNoCase( lpcszRelFieldName ) )
		{
			strRet = it->get<1>();
			break;
		}
	}
	return strRet;
}

// �t�B�[�`���Ɋ֘A����O���e�[�u�����I�[�v������
ITablePtr AheOpenRelationTable( IFeature* ipFeature, LPCTSTR lpcszRelFieldName )
{
	if( ! ipFeature )
		return NULL;

	return AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), AheGetRelationTableName( AheGetFeatureClassName(ipFeature), lpcszRelFieldName ) );
}

// �l�b�g���[�N�m�[�h�ɐڑ�����Ă���l�b�g���[�N�����N���擾
LONG AheGetConnectedNetworkLink( IFeaturePtr ipFeature, CTargetItems& cTargetItems )
{
	// �l�b�g���[�N�m�[�h����
	if( ipFeature == NULL && AheGetFeatureKinds( ipFeature ) != sindyeNodeFeature )
	{
		_ASSERTE( FALSE );
		return -1;
	}

	ISpatialFilterPtr ipFilter;

	if( SUCCEEDED( ipFilter.CreateInstance( CLSID_SpatialFilter ) ) )
	{
		IQueryFilterPtr ipQueryFilter( ipFilter );
		if( SUCCEEDED( ipFilter->QueryInterface( &ipQueryFilter) ) )
		{
			// �l�b�g���[�N�����N�N���X��n���ꂽ�t�B�[�`�����猟������
			IFeatureClassPtr ipNWLinkClass( AheGetNetworkLinkFeatureClass(ipFeature) );
			if( ! ipNWLinkClass )
			{
				_ASSERTE( ipNWLinkClass );
				return NULL;
			}

			// �W�I���g���̎擾
			IGeometryPtr ipGeom;
			ipFeature->get_Shape( &ipGeom );
			if( ipGeom == NULL )
			{
				_ASSERTE( ipGeom != NULL );
				return NULL;
			}

			// �N�G���t�B���^�̍쐬
			ipFilter->putref_Geometry( ipGeom );
			CComBSTR cComBSTR;
			ipNWLinkClass->get_ShapeFieldName( &cComBSTR );
			ipFilter->put_GeometryField( cComBSTR );
			ipFilter->put_SpatialRel( esriSpatialRelIntersects );

			// ����
			IFeatureCursorPtr ipFeatureCursor;
			if( SUCCEEDED( ipNWLinkClass->Search( ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor ) ) && NULL != ipFeatureCursor )
			{
				IFeaturePtr ipFeatureTmp = NULL;
				while( S_OK == ipFeatureCursor->NextFeature( &ipFeatureTmp ) )
					cTargetItems.push_back( ipFeatureTmp );
			}
		}
	}

	return cTargetItems.size();
}

// �l�b�g���[�N�����N���ڑ����Ă���m�[�h�t�B�[�`�����擾
LONG AheGetConnectedNetworkNode( IFeaturePtr ipFeature, CTargetItems& cTargetItems )
{
	// �l�b�g���[�N�����N����
	if( ipFeature == NULL && AheGetFeatureKinds( ipFeature ) != sindyeRoadFeature )
	{
		_ASSERTE( FALSE );
		return -1;
	}

	ISpatialFilterPtr ipFilter;

	if( SUCCEEDED( ipFilter.CreateInstance( CLSID_SpatialFilter ) ) )
	{
		IQueryFilterPtr ipQueryFilter( ipFilter );
		if( SUCCEEDED( ipFilter->QueryInterface( &ipQueryFilter ) ) )
		{
			// �n���ꂽ�t�B�[�`���Ɠ������[�N�X�y�[�X����m�[�h���C����������
			IFeatureClassPtr ipNWNodeClass( AheGetNetworkNodeFeatureClass(ipFeature) );
			if( ! ipNWNodeClass )
			{
				_ASSERTE( ipNWNodeClass );
				return NULL;
			}

			// �W�I���g���̎擾
			IGeometryPtr ipGeom;
			ipFeature->get_Shape( &ipGeom );
			if( ipGeom == NULL )
			{
				_ASSERTE( ipGeom != NULL );
				return NULL;
			}

			// �N�G���t�B���^�̍쐬
			ipFilter->putref_Geometry( ipGeom );
			CComBSTR cComBSTR;
			ipNWNodeClass->get_ShapeFieldName( &cComBSTR );
			ipFilter->put_GeometryField( cComBSTR );
			ipFilter->put_SpatialRel( esriSpatialRelTouches );

			// ����
			IFeatureCursorPtr ipFeatureCursor;
			if( SUCCEEDED( ipNWNodeClass->Search( ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor ) ) && NULL != ipFeatureCursor )
			{
				IFeaturePtr ipFeatureTmp;
				while( S_OK == ipFeatureCursor->NextFeature( &ipFeatureTmp ) )
					cTargetItems.push_back( ipFeatureTmp );
			}
		}
	}

	return cTargetItems.size();
}

// �l�b�g���[�N�����N���ڑ����Ă���m�[�h�t�B�[�`�����擾
bool AheGetConnectedNetworkLinkAndNode( IFeaturePtr ipSelectRoad, IFeaturePtr ipSelectNode, IFeaturePtr &ipConnectedNetworkLink, IFeaturePtr &ipOppNode )
{
	CTargetItems cNetworkLinkItems;
	// �m�[�h�ɂ��Ă��邷�ׂẴl�b�g���[�N�����N���擾
	AheGetConnectedNetworkLink( ipSelectNode, cNetworkLinkItems );
	
	for( CTargetItems::iterator iRoad = cNetworkLinkItems.begin(); iRoad != cNetworkLinkItems.end(); iRoad++ )
	{
		IFeaturePtr ipRoadFeature = iRoad->GetFeature();

		// ���ܑI�𒆂̃l�b�g���[�N�����N�łȂ���΁A�~�������H
		LONG lSelectRoadOID, lRoadOID;
		ipSelectRoad->get_OID( &lSelectRoadOID );
		ipRoadFeature->get_OID( &lRoadOID );

		if( lSelectRoadOID != lRoadOID )
		{
			// ���Α��̃l�b�g���[�N�����N���m��
			ipConnectedNetworkLink = ipRoadFeature;

			CTargetItems	cNodeItems;
			// �l�b�g���[�N�����N�ɂ��Ă���m�[�h���擾
			AheGetConnectedNetworkNode( ipRoadFeature, cNodeItems );

			for(CTargetItems::iterator iNode = cNodeItems.begin(); iNode != cNodeItems.end(); iNode++ )
			{
				IFeaturePtr ipNodeFeature = iNode->GetFeature();

				// ���I�𒆂̃m�[�h�łȂ���΁A�����Е��̃m�[�h
				LONG lSelectNodeOID, lNodeOID;
				ipSelectNode->get_OID( &lSelectNodeOID );
				ipNodeFeature->get_OID( &lNodeOID );

				if( lSelectNodeOID != lNodeOID )
				{
					// ���Α��̃m�[�h���m��
					ipOppNode = ipNodeFeature;


					// �m�[�h��ʂ��擾
					IFieldsPtr		ipNodeFields;
					CComVariant		vNodeClass;
					long			lNodeClass = 0;
					sindyeNodeClass	sNodeClass; 

					ipNodeFeature->get_Fields(&ipNodeFields);
					ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
					ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
					sNodeClass = sindyeNodeClass(vNodeClass.lVal);

					// 2�����b�V���ゾ������A���[�Ƃ�2�����b�V���m�[�h
					if( sNodeClass == sindyeOnMesh || sNodeClass == sindyeOnMeshPEC || sNodeClass == 99 )//WALK_NODE�̋��ӏ�m�[�h=99
						return true;
				}
			}
		}
	}
	return false;
}

// 2�����b�V�����ׂ��ł���l�b�g���[�N�����N���擾����
bool AheIsNetworkLinkCrossMesh( IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cNetworkLinkCrossMesh )
{
	// PGDB�ł������ꍇ�ɂ́A�ŏ��łӂ邢���Ƃ�
	IDatasetPtr ipDataset( AheGetFeatureClass( ipFeature ) );
	if( ipDataset == NULL )
		return false;

	IFeatureClassPtr ipFeatureClass = NULL;

	// �t�B�[�`���N���X�擾
	if( AheGetFeatureClass( ipMap, ipFeature, &ipFeatureClass ) )
	{
		// ���H�����N���ǂ����H
		if( AheIsNetworkLink( ipFeatureClass ) )
		{
			CTargetItems cNodeItems;

			// ���H�����N�ɂ��Ă���m�[�h���擾
			LONG lNodeNum = AheGetConnectedNetworkNode( ipFeature, cNodeItems );

			// �ڑ����Ă���m�[�h�̌���2�łȂ�������A���������̂ŁA�����ŏI��
			// [Bug2606]�֘A�ŁA�܂����b�V�����E�Ƀm�[�h��2�d�Ȃ��Ă���ꍇ������̂ŁA����ւ̑Ή�
			// ��{�̓��H�����N�ɐڑ����Ă���m�[�h�͕K��2�ł���B
			if( lNodeNum != 2 )
				return false;

			for( CTargetItems::iterator iCnt = cNodeItems.begin(); iCnt != cNodeItems.end(); iCnt++ )
			{
				IFeaturePtr ipNodeFeature = iCnt->GetFeature();
				CTargetItems cRoadItems;

				// �m�[�h��ʂ��擾
				IFieldsPtr		ipNodeFields;
				CComVariant		vNodeClass;
				long			lNodeClass = 0;
				sindyeNodeClass	sNodeClass; 

				ipNodeFeature->get_Fields(&ipNodeFields);
				ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
				ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
				sNodeClass = sindyeNodeClass(vNodeClass.lVal);


				// 2�����b�V���ゾ������A���Α��̓��H���擾
				if( sNodeClass == sindyeOnMesh || sNodeClass == sindyeOnMeshPEC || sNodeClass == 99 )//WALK_NODE�̋��ӏ�m�[�h=99
				{
					CTargetItems cConnectedRoadOnMesh;
					IFeaturePtr ipOppNode;
					IFeaturePtr ipRoadsOnMesh;
					cConnectedRoadOnMesh.clear();

					IFeaturePtr ipTempFeature = ipFeature;

					bool bIsBothNodeOnMesh = false;

					do{
						bIsBothNodeOnMesh = AheGetConnectedNetworkLinkAndNode( ipTempFeature, ipNodeFeature, ipRoadsOnMesh, ipOppNode );

						// ���Α��̓��H���m�ۂ��Ă���
						cConnectedRoadOnMesh.push_back( ipRoadsOnMesh );

						// �I�����ꂽ���Α��̓��H�ɂ܂�2�����b�V���m�[�h�����Ă���ꍇ�A���H�����N�ƃm�[�h��
						// �u�������āA�����Ē��ׂ�
						if( bIsBothNodeOnMesh )
						{
							ipTempFeature = ipRoadsOnMesh;
							ipNodeFeature = ipOppNode; 
						}
					}while( bIsBothNodeOnMesh );

					// �I�����ꂽ�A2�����b�V���֘A�̓��H��o�^����
					for( CTargetItems::iterator it = cConnectedRoadOnMesh.begin(); it != cConnectedRoadOnMesh.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						cNetworkLinkCrossMesh.push_back( ipFeatureOnMesh );
					}
				}
			}
		}
	}
	if( cNetworkLinkCrossMesh.size() > 0 )
		return true;
	return false;
}

// ���ʂ̃m�[�hID���X�g���擾����iAheGetRelationshipContainFeature�Ŏg�p�j
std::list<long> _AheGetCommonNodeID( const CTargetItems& cItems )
{
	std::list<long> listRet; // �߂�l

	// ���ʂ̃m�[�hID���X�g���擾����
	// �������AcItems����̏ꍇ�͉����ł��Ȃ��̂ł��̏ꍇ�Ɍ���
	// ���[��Ԃ�
	long lNodeID = -1, lFromDummy = -1, lToDummy = -1;
	for( CTargetItems::const_iterator it = cItems.begin(); it != cItems.end(); ++it )
	{
		if( 0 > lFromDummy && 0 > lToDummy )
		{
			lFromDummy = it->GetAttribute( _T("FROM_NODE_ID") ).lVal;
			lToDummy   = it->GetAttribute( _T("TO_NODE_ID") ).lVal;
		}
		else {
			long lFromDummy2 = it->GetAttribute(_T("FROM_NODE_ID")).lVal;
			long lToDummy2   = it->GetAttribute(_T("TO_NODE_ID")).lVal;
			if( lFromDummy == lFromDummy2 || lFromDummy == lToDummy2 )
				lNodeID = lFromDummy;
			else if( lToDummy == lFromDummy2 || lToDummy == lToDummy2 )
				lNodeID = lToDummy;

			break; // 2�񂵂��ʂ��Ȃ��i���Ԃ̖��ʁj
		}
	}
	if( 0 > lNodeID ) // �Z�O�����g�ɃX�i�b�v�����Ĉ������ꍇ
	{
		listRet.push_back( lFromDummy );
		listRet.push_back( lToDummy );
	}
	else
		listRet.push_back( lNodeID );

	return listRet;
}

// Contain���[���֌W�ɂ�����̂̂����֘A������ׂ����́i�p�X�j���擾����
CTargetItems AheGetRelationshipContainFeature( IFeaturePtr ipWithinFeature, const CTargetItems& cContainFeatures )
{
	CTargetItems cRet; // �߂�l

	if( ! ipWithinFeature ) // �����`�F�b�N
		return cRet;

	// ipWithinFeature�̗��[�_���ڂ�����̂�cContainFeatures���猟��
	IGeometryPtr ipGeom;
	ipWithinFeature->get_Shape( &ipGeom );
	IPolylinePtr ipPolyline( ipGeom ); // ���̂Ƃ��냉�C�������z�肵�Ă��Ȃ�
	if( ipPolyline )
	{
		IPointPtr ipFrom, ipTo;
		ipPolyline->get_FromPoint( &ipFrom );
		ipPolyline->get_ToPoint( &ipTo );

		CTargetItems cFromFeatures; // FROM���ɐڂ���t�B�[�`���Q
		CTargetItems cToFeatures; // TO���ɐڂ���t�B�[�`���Q

		// FROM�ATO�̂ǂ���ɐڂ��邩������
		for( CTargetItems::const_iterator it = cContainFeatures.begin(); it != cContainFeatures.end(); ++it )
		{
			IGeometryPtr ipContainGeom( it->GetShape() ); // �u�܂ށv���t�B�[�`���̃W�I���g��
			if( AheTestGeometryRelationship( ipFrom, ipContainGeom, (sindyeSpatialRel)(sindyeSpatialRelTouch|sindyeSpatialRelWithin) ) )
				cFromFeatures.push_back( *it );
			if( AheTestGeometryRelationship( ipTo, ipContainGeom, (sindyeSpatialRel)(sindyeSpatialRelTouch|sindyeSpatialRelWithin) ) )
				cToFeatures.push_back( *it );
		}
		// ��O�Ƃ��āAFROM�ATO�ǂ���ɂ��܂܂��ꍇ�͂���ȊO�ɂ��肦�Ȃ��̂ŁA���̃P�[�X���͂����Ă���
		cFromFeatures.sort();
		cToFeatures.sort();
		std::set_intersection( cFromFeatures.begin(), cFromFeatures.end(), cToFeatures.begin(), cToFeatures.end(), std::back_inserter(cRet) );

		_ASSERTE( cRet.empty() || 1 == cRet.size() ); // �󂩁A��ł���͂�

		if( cRet.empty() )
		{
			using namespace path_analyzer;

			// FROM�ATO�����ׂ�p�X�����ׂĒ��ׂ�
			graph<CTargetItems,edge<CTargetItem> > cGraph( cContainFeatures );
			std::list<path<edge<CTargetItem> > > listPath;
			cGraph.create_path( _AheGetCommonNodeID( cFromFeatures ), _AheGetCommonNodeID( cToFeatures ), listPath );
#ifdef _DEBUG
			cGraph.trace_path(listPath);
#endif // ifdef _DEBUG
			// �ǂ̃p�X���ӂ��킵���̂�����
			// �D�揇�ʂ͈ȉ��̂Ƃ���F
			// 1. ipWithinFeature��end_point���ڂ��Ă��邱�Ɓi=FROM�ATO���X�g�̂�����������Ă��邱�Ɓj
			// 2. ipWithinFeature��2�������x���ŋ��L���Ă��邱�Ɓi��Ԓ������́j
			// 3. ipWithinFeature��1�������x���ŋ��L���Ă��邱�Ɓi��ԑ������́j
			if( 1 < listPath.size() )
			{
				// 1. �̌���
				for( std::list<path<edge<CTargetItem> > >::iterator itPath = listPath.begin(); itPath != listPath.end(); )
				{
					bool bFindFrom = false, bFindTo = false;
					for( path<edge<CTargetItem> >::const_iterator it = itPath->begin(); it != itPath->end(); ++it )
					{
						if( ! bFindFrom ) bFindFrom = ( std::find( cFromFeatures.begin(), cFromFeatures.end(), (*it)->__edge_native ) == cFromFeatures.end() ) ? false : true;
						if( ! bFindTo   ) bFindTo   = ( std::find( cToFeatures.begin(),   cToFeatures.end(),   (*it)->__edge_native ) == cToFeatures.end()   ) ? false : true;

						if( bFindFrom && bFindTo )
							break;
					}

					if( ! ( bFindFrom && bFindTo ) )
						itPath = listPath.erase( itPath );
					else
						++itPath;
				}
			}
			if( 1 < listPath.size() )
			{
				// 2. �̌���
				ITopologicalOperatorPtr ipTopo( ipGeom );
				std::map<double, path<edge<CTargetItem> > > mapPath; // ���ʃZ�O�����g���Ń\�[�g
				for( std::list<path<edge<CTargetItem> > >::const_iterator itPath = listPath.begin(); itPath != listPath.end(); ++itPath )
				{
					double dLen = 0.0f; // ���ʃZ�O�����g��
					for( path<edge<CTargetItem> >::const_iterator it = itPath->begin(); it != itPath->end(); ++it )
					{
						IGeometryPtr ipResultGeom;
						if( SUCCEEDED( ipTopo->Intersect( (*it)->__edge_native.GetShape(), esriGeometry1Dimension, &ipResultGeom ) ) )
						{
							double dLenTmp = 0.0f;
							((ICurvePtr)ipResultGeom)->get_Length( &dLenTmp );
							dLen += dLenTmp;
						}
						else
							_ASSERTE( false ); // Intersect�Ɏ��s
					}
					if( 0 < dLen )
						mapPath[dLen] = *itPath;
				}
				if( ! mapPath.empty() )
				{
					listPath.clear();
					listPath.push_back( mapPath.rbegin()->second );
				}
			}
			if( 1 < listPath.size() )
			{
				// 3. �̌���
				ITopologicalOperatorPtr ipTopo( ipGeom );
				std::map<int, path<edge<CTargetItem> > > mapPath; // ��������_�̐��Ń\�[�g
				for( std::list<path<edge<CTargetItem> > >::const_iterator itPath = listPath.begin(); itPath != listPath.end(); ++itPath )
				{
					int lCount = 0; // ��������_�̐�
					for( path<edge<CTargetItem> >::const_iterator it = itPath->begin(); it != itPath->end(); ++it )
					{
						IGeometryPtr ipResultGeom;
						if( SUCCEEDED( ipTopo->Intersect( (*it)->__edge_native.GetShape(), esriGeometry0Dimension, &ipResultGeom ) ) )
						{
							long lCountTmp = 0;
							((IPointCollectionPtr)ipResultGeom)->get_PointCount( &lCountTmp );
							if( 0 < lCountTmp )
								lCount += lCountTmp;
						}
						else
							_ASSERTE( false ); // Intersect�Ɏ��s
					}
					if( 0 < lCount )
						mapPath[lCount] = *itPath;
				}
				if( ! mapPath.empty() )
				{
					listPath.clear();
					listPath.push_back( mapPath.rbegin()->second );
				}
			}
			if( ! listPath.empty() )
			{
				for( path<edge<CTargetItem> >::const_iterator it = listPath.begin()->begin(); it != listPath.begin()->end(); ++it )
					cRet.push_back( (*it)->__edge_native );
			}
		}
	}

	return cRet;
}

// ���s��NW�̔ėp���f�����_�E���R���o�[�g���f�����𔻒肷��֐�
sindy::schema::walk_type::type_class::ECode AheIsDownconvert( IFeaturePtr ipFeature )
{
	using namespace sindy::schema::walk_type::type_class;
	// �f�t�H���g�ԋp�l�͕�������Łi�ėp�Łj
	ECode walktype = kGeneral;

	// �n���ꂽ�t�B�[�`���̃f�[�^�Z�b�g���擾��
	IFeatureClassPtr ipFeatureClass = AheGetFeatureClass( ipFeature );
	CString strFeatureClassName = AheGetFeatureClassName( ipFeatureClass );
	// ���s�҃����N�����s�҃m�[�h����Ȃ��֐�����Ă΂ꂽ��A�{�������ŗ������ق�������
	if( !(strFeatureClassName == walk_link::kTableName || strFeatureClassName == walk_node::kTableName) )
		_ASSERTE( false );
	// �擾�����t�B�[�`���N���X�Ɠ��f�[�^�Z�b�g����WALK_TYPE�����邩�ǂ����𒲂ׂ�B
	ITablePtr ipWalkTypeTable = AheOpenSameOwnerTable( (ITablePtr)ipFeatureClass, walk_type::kTableName );
	// WALK_TYPE�e�[�u�����Ȃ��ꍇ�ɂ͕�������łƂ������ƂɂȂ�
	if( !ipWalkTypeTable )
		return walktype;

	// �ȉ�WALK_TYPE������ꍇ�ɕ�������łȂ̂��A�_�E���R���o�[�g�łȂ̂��̔���
	// �S������
	_ICursorPtr ipCursor;
	ipWalkTypeTable->Search( AheInitQueryFilter(), VARIANT_FALSE, &ipCursor );

	// 1���̂݃f�[�^�������Ă���O�񂾂��A�������������Ă�����A�Ō�̃f�[�^�̒l�����
	_IRowPtr ipRow;
	int i = 0;
	while( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		CComVariant vaValue = AheGetAttribute( ipRow, walk_type::kWalkTypeClass );
		walktype = (ECode)vaValue.lVal;
		++i;
		// 1���ȏ�i�[����悤�Ȃ��Ƃ���������A�����ŗ�����ׂ�
		if( i > 1 )
			_ASSERTE( false );
	}

	return walktype;
};