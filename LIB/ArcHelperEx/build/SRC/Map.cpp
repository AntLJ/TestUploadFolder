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
 * @file Map.h
 * @brief <b>CMap�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G\n
 * @version $Id$
 */
#include "stdafx.h"
#include "Map.h"
#include "type_convert.h"
#include "AheLayerFunctions.h"
#include "GlobalFunctions.h"
#include <TDC/useful_headers/str_util.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{

/**
 * @brief �O���[�v���C�����烌�C������������@�\�����������֐�
 * @note FindLayersFromComposite, FindLayerFromComposite �ŌĂяo�����B
 * bFirstOnly�I�v�V�������O�����J���Ȃ����߂ɒ�`�����֐��ƂȂ�B
 * @param ipTargetComposite  [in] �����Ώۂ̃O���[�v���C��
 * @param ipWorkspace        [in] ���[�N�X�y�[�X
 * @param lpcszOwnerName     [in] �I�[�i�[��
 * @param lpcszOTableName    [in] ��������e�[�u����
 * @param bOnlyDirectlyUnder [in] �O���[�v���C���̒����̂݌������邩�ǂ���
 * @param bFirstOnly         [in] �ŏ���1����������return����i�f�t�H���g�Ffalse�j
 * @retval NULL	�G���[���͌�����Ȃ�
 * @retval NULL�ȊO ILayerPtr
 */
inline std::list<CAdapt<ILayerPtr>> FindFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder, bool bFirstOnly = false )
{
	std::list<CAdapt<ILayerPtr>> layers;
	if(!ipTargetComposite) return layers;

	long layerNum = -1L;
	if(FAILED(ipTargetComposite->get_Count(&layerNum))) return layers;
	for( long i=0; i<layerNum; ++i )
	{
		ILayerPtr ipLayer;
		if( FAILED( ipTargetComposite->get_Layer(i, &ipLayer) ) || !ipLayer )
			continue;

		if( IFeatureClassPtr ipFeatureClass = AheGetFeatureClass(ipLayer) )
		{
			// �e�[�u�������r
			if( lpcszTableName )
			{
				if( 0 != AheGetFeatureClassName( ipFeatureClass ).CompareNoCase( lpcszTableName ) )
					 continue;
			}
			// �I�[�i�[�����n����Ă���ꍇ�̓I�[�i�[�����r����
			if( lpcszOwnerName )
			{
				if( 0 != AheGetFeatureClassOwnerName( ipFeatureClass ).CompareNoCase( lpcszOwnerName ) )
					continue;
			}
			// ���[�N�X�y�[�X���n����Ă���ꍇ�̓C���X�^���X���r����
			if( ipWorkspace )
			{
				IDatasetPtr ipDataset( ipFeatureClass );
				if( !ipDataset ) continue;
				IWorkspacePtr ipDatasetWorkspace;
				if( FAILED(ipDataset->get_Workspace( &ipDatasetWorkspace )) )
					continue;
				if( ipDatasetWorkspace != ipWorkspace ) continue;
			}
			layers.push_back( ipLayer );
			if( bFirstOnly ) return layers;
		}

		// ���C�����������������Ȃ��Ȃ�ȍ~�̏����͎��s���Ȃ�
		if(bOnlyDirectlyUnder) continue;

		// �O���[�v���C���Ȃ����������
		if( ICompositeLayerPtr ipCompositeLayer = ipLayer )
		{
			std::list<CAdapt<ILayerPtr>> childLayers = FindFromComposite( ipCompositeLayer, ipWorkspace, lpcszOwnerName, lpcszTableName, bOnlyDirectlyUnder, bFirstOnly );
			if(childLayers.empty()) continue;
			if( bFirstOnly ) return childLayers;
			// TODO c++11�Ή������Ȃ�splice�ɂ��邱��
			layers.insert( layers.end(), childLayers.begin(), childLayers.end() );
		}
	}
	return layers;
};

} // namespace


IEnumLayerPtr CMap::GetLayers( IUID* UID, bool recursive ) const
{
	IEnumLayerPtr ipLayers;	// �Ԃ�l

	if( ! UID )
		return NULL;

	if( FAILED( (*this)->get_Layers( UID, bool2VB(recursive), &ipLayers ) ) )
		_ASSERTE(false);

	return ipLayers;
}

// �t�B�[�`�����C���݂̂̃��C���Z�b�g��Ԃ�
IEnumLayerPtr CMap::GetFeatureLayers( bool recursive ) const
{
	IEnumLayerPtr ipLayers;	// �Ԃ�l

	IUIDPtr ipUID( CLSID_UID );

	if( SUCCEEDED( ipUID->put_Value( _variant_t(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ) ) )
		ipLayers = GetLayers( ipUID, recursive );
	else
		_ASSERTE(false);

	return ipLayers;
}

IEnumLayerPtr CMap::GetGroupLayers( bool recursive ) const
{
	IEnumLayerPtr ipLayers;	// �Ԃ�l

	IUIDPtr ipUID( CLSID_UID );

	if( SUCCEEDED( ipUID->put_Value( _variant_t(L"{EDAD6644-1810-11D1-86AE-0000F8751720}") ) ) )
		ipLayers = GetLayers( ipUID, recursive );
	else
		_ASSERTE(false);

	return ipLayers;
}

ICompositeLayerPtr CMap::GetParentCompositeLayer( ILayerPtr ipLayer ) const
{
	// �O���[�v���C���[�Q���擾����
	IEnumLayerPtr ipEnumLayer = GetGroupLayers( true );
	ipEnumLayer->Reset();

	ILayerPtr ipGroupLayer;
	while( ipEnumLayer->Next( &ipGroupLayer ) == S_OK && ipGroupLayer )
	{
		ICompositeLayerPtr ipCompositeLayer = ipGroupLayer;
		long lLayerCount = 0;
		ipCompositeLayer->get_Count( &lLayerCount );
		for( int i = 0; i < lLayerCount; ++i )
		{
			ILayerPtr ipCheckLayer;
			ipCompositeLayer->get_Layer( i, &ipCheckLayer );
			if( ipLayer == ipCheckLayer )
				return ipCompositeLayer;
		}
	}

	return NULL;
}

#if 0
// ���C������������
ILayerPtr CMap::FindLayer( LPCTSTR/* lpcszServerName*/, LPCTSTR/* lpcszVersionName*/, LPCTSTR/* lpcszOwnerName*/, LPCTSTR/* lpcszTableName*/ ) const
{
	ILayerPtr ipLayer;	// �Ԃ�l

	LOGASSERTE(false,sindyErr_NoImplimentation );

	return ipLayer;
}

// ���C������������
ILayerPtr CMap::FindLayer( LPCTSTR/* lpcszServerName*/, LPCTSTR/* lpcszVersionName*/, LPCTSTR/* lpcszOwnerTableName*/ ) const
{
	ILayerPtr ipLayer;	// �Ԃ�l

	LOGASSERTE(false,sindyErr_NoImplimentation );

	return ipLayer;
}
#endif // if 0

// ���C������������
std::list<CAdapt<ILayerPtr>> CMap::FindLayers( const IWorkspacePtr& ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool first_only /*= false*/ ) const
{
	std::list<CAdapt<ILayerPtr>> layers;

	if( !lpcszTableName )
		return layers;

	for( const_iterator it = begin(); it != end(); ++it )
	{
		ITablePtr ipTable( it.getTable() );
		if( !ipTable )
			continue;

		// �e�[�u�������r
		if( 0 != AheGetFeatureClassName( ipTable ).CompareNoCase( lpcszTableName ) )
			continue;

		// �I�[�i�[�����n����Ă���ꍇ�̓I�[�i�[�����r����
		if( lpcszOwnerName )
		{
			if( 0 != AheGetFeatureClassOwnerName( ipTable ).CompareNoCase( lpcszOwnerName ) )
				continue;
		}

		// ���[�N�X�y�[�X���n����Ă���ꍇ�̓v���p�e�B�Z�b�g���r����
		if( ipWorkspace )
		{
			IDatasetPtr ipDataset( ipTable );
			if( ipDataset )
			{
				IWorkspacePtr ipDatasetWorkspace;
				IPropertySetPtr ipWSProp, ipDSProp;
				if( SUCCEEDED( ipDataset->get_Workspace( &ipDatasetWorkspace ) ) &&
					SUCCEEDED( ipDatasetWorkspace->get_ConnectionProperties( &ipDSProp ) ) &&
					SUCCEEDED( ipWorkspace->get_ConnectionProperties( &ipWSProp ) ) )
				{
					if( AheIsSamePropertySet( ipWSProp, ipDSProp ) ) 
						layers.push_back(*it);
				}
				else
					_ASSERTE(false);
			}
			else
				_ASSERTE(false);
		}
		else
			layers.push_back(*it);

		// �ŏ���1����������I��
		if( first_only && !layers.empty() )
			return layers;
	}
	return layers;
}

// ���C������������
ILayerPtr CMap::FindLayer( IWorkspace* ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName ) const
{
	std::list<CAdapt<ILayerPtr>> layers = FindLayers( ipWorkspace, lpcszOwnerName, lpcszTableName, true );
	return layers.empty() ? NULL : layers.begin()->m_T;
}

// ���C������������
ILayerPtr CMap::FindLayer( IWorkspace* ipWorkspace, LPCTSTR lpcszOwnerTableName ) const
{
	ILayerPtr ipLayer;	// �Ԃ�l

	if( !lpcszOwnerTableName )
		return NULL;

	const std::vector<CString> owner_table = uh::str_util::split( lpcszOwnerTableName, _T(".") );

	if( 2 != owner_table.size() )
		return NULL;

	return FindLayer( ipWorkspace, owner_table[0], owner_table[1] );
}

// �t�B�[�`���N���X�ɑΉ����郌�C������������
ILayerPtr CMap::FindLayer( const IFeatureClassPtr ipFeatureClass ) const
{
	// �t�B�[�`�����C���Q���擾����
	IEnumLayerPtr ipEnumLayer = GetFeatureLayers( true );
	ipEnumLayer->Reset();

	ILayerPtr ipLayer;
	while( ipEnumLayer->Next( &ipLayer ) == S_OK && ipLayer )
	{
		IFeatureLayerPtr ipFeatureLayer = ipLayer;
		IFeatureClassPtr ipCheckedFeatureClass;
		if( FAILED( ipFeatureLayer->get_FeatureClass(&ipCheckedFeatureClass) ) )
			continue;
		if( ipCheckedFeatureClass == ipFeatureClass )
			return ipLayer;
	}
	return NULL;
}

// ���C���Ɠ����O���[�v�ɏ�������S�Ẵ��C������������
std::list<CAdapt<ILayerPtr>> CMap::FindSameGroupLayers( const IUnknownPtr ipUnknown, LPCTSTR lpcszTableName ) const
{
	ILayerPtr ipLayer = ipUnknown;
	// ipUnknown �����C���łȂ��Ȃ�A�Ή����郌�C������������
	if( !ipLayer )
	{
		ipLayer = FindLayer(ipUnknown);
		if( !ipLayer ) return std::list<CAdapt<ILayerPtr>>();
	}

	// �O���[�v���C���ɏ�������Ȃ�A�O���[�v���C����������
	if( ICompositeLayerPtr ipCompositeLayer = GetParentCompositeLayer( ipLayer ) )
		return FindLayersFromComposite( ipCompositeLayer, NULL, NULL, lpcszTableName, true );

	// �O���[�v���C���ɏ������Ȃ��Ȃ�ATOC�̒����ɂ���t�B�[�`���N���X������
	std::list<CAdapt<ILayerPtr>> layers;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( ICompositeLayerPtr(*it) ) continue;
		if( 0!=AheGetFeatureClassName( *it ).CompareNoCase( lpcszTableName ) )
			continue;

		// �O���[�v���C�����̃��C���Ȃ�ΏۊO
		if( GetParentCompositeLayer( *it ) ) continue;
		layers.push_back(*it);
	}
	return layers;
}

// �w�背�C���Ɠ����O���[�v�ɏ������郌�C������������
ILayerPtr CMap::FindSameGroupLayer( const IUnknownPtr ipUnknown, LPCTSTR lpcszTableName ) const
{
	ILayerPtr ipLayer = ipUnknown;
	// ipUnknown �����C���łȂ��Ȃ�A�Ή����郌�C������������
	if( !ipLayer )
	{
		ipLayer = FindLayer(ipUnknown);
		if( !ipLayer ) return NULL;
	}

	// �O���[�v���C���ɏ�������Ȃ�A�O���[�v���C����������
	if( ICompositeLayerPtr ipCompositeLayer = GetParentCompositeLayer( ipLayer ) )
		return FindLayerFromComposite( ipCompositeLayer, NULL, NULL, lpcszTableName, true );

	// �O���[�v���C���ɏ������Ȃ��Ȃ�ATOC�̒����ɂ���t�B�[�`���N���X������
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( ICompositeLayerPtr(*it) ) continue;
		if( 0!=AheGetFeatureClassName( *it ).CompareNoCase( lpcszTableName ) )
			continue;

		// �O���[�v���C�����̃��C���Ȃ�ΏۊO
		if( GetParentCompositeLayer( *it ) ) continue;
		return *it;
	}
	return NULL;
}

// �O���[�v���C����������𖞂������C����S�Č�������
std::list<CAdapt<ILayerPtr>> CMap::FindLayersFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder/*= false*/ )
{
	return FindFromComposite( ipTargetComposite, ipWorkspace, lpcszOwnerName, lpcszTableName, bOnlyDirectlyUnder );
}

// �O���[�v���C�����烌�C������������
ILayerPtr CMap::FindLayerFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder /*= false*/ )
{
	std::list<CAdapt<ILayerPtr>> layers = FindFromComposite( ipTargetComposite, ipWorkspace, lpcszOwnerName, lpcszTableName, bOnlyDirectlyUnder, true );
	return layers.empty() ? NULL : layers.begin()->m_T;
}
