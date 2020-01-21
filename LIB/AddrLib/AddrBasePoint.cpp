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

#include "StdAfx.h"
#include "AddrBasePoint.h"
#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include "AreaCodeHelper.h"
#include "AddrGlobals.h"

#include <ArcHelperEx/GlobalFunctions.h>

namespace addr
{

// �Z�����̎擾
CString CAddrBasePoint::GetAddrName( bool bChar /* = false */ )
{
	if( m_strAddrName.IsEmpty() )
	{
		if( GetAddrCode().GetLength() >= 20 )
		{
			m_strAddrName = g_cAreaCodeHelper.IsConnected()? g_cAreaCodeHelper.GetAddrKanji(GetAddrCode()) : _T("");
			if( !m_strAddrName.IsEmpty() )
			{
				long lChiban = _ttol(GetAddrCode().Mid(11, 5));
				long lGou = _ttol(GetAddrCode().Mid(16, 4));
				if( lChiban > 0 && lGou > 0 )
					m_strAddrName.AppendFormat( _T("%d�|%d"), lChiban, lGou );
				else if( lChiban > 0 && lGou == 0 )
					m_strAddrName.AppendFormat( _T("%d"), lChiban );

				m_strAddrName = GetZenkakuData( m_strAddrName );
			}
		}
	}
	return m_strAddrName;
}

// �|�C���g��������s���E���擾
const CCityAdmin* CAddrBasePoint::GetCityAdmin()
{
	iterator itr = find(sindyTableType::city_admin);
	if( itr == end() )
	{
		// �s�s�n�}�s���E�e�[�u�����w�莞�́A����I�[�i�[�E���[�N�X�y�[�X����擾
		// �I�[�i�[�����擾�Ȃ��Ɏ擾
		CString strTableName;
		esriWorkspaceType esriType;
		GetWorkspace()->get_Type( &esriType );
		if( esriRemoteDatabaseWorkspace == esriType )
			strTableName.Format( _T("%s.%s"), GetNameString()->GetOwnerName(), city_admin::kTableName );		
		else
			strTableName = city_admin::kTableName;

		IFeatureClassPtr ipCityAdminClass;
		((IFeatureWorkspacePtr)GetWorkspace())->OpenFeatureClass( _bstr_t(strTableName), &ipCityAdminClass );
		
		if( !SetCityAdmin(CFeatureClass(ipCityAdminClass)) )
			return NULL;

		itr = find(sindyTableType::city_admin);	// �Ď擾
	}

	return itr != end()? static_cast<CCityAdmin*>(&**itr->second.begin()) : NULL;
}

// �|�C���g��������s���E���擾
const CCityAdmin* CAddrBasePoint::GetCityAdmin( const CFeatureClass& cCityAdminClass )
{
	iterator itr = find(sindyTableType::city_admin);
	if( itr == end() )
	{
		if( !SetCityAdmin(cCityAdminClass) )
			return NULL;

		itr = find(sindyTableType::city_admin);	// �Ď擾
	}

	return itr != end()? static_cast<CCityAdmin*>(&**itr->second.begin()) : NULL;
}

// �|�C���g��������s���E���Z�b�g
bool CAddrBasePoint::SetCityAdmin( const CFeatureClass& cAdminTable )
{
	if( cAdminTable )
	{
		ISpatialFilterPtr ipSpFilter( AheInitSpatialFilter(NULL, GetShapeCopy()) );
		if( ipSpFilter )
		{
			// ��ɍi��Ȃ��Ƃ��́ANULL�ɂ���
			if( cAdminTable.RowCount(ipSpFilter) == 1 )
			{
				IFeatureCursorPtr ipCursor( cAdminTable.Search(ipSpFilter, VARIANT_FALSE) );
				IFeaturePtr ipFeature;
				if( ipCursor && ipCursor->NextFeature(&ipFeature) == S_OK )
				{
					// ���ɍs���E���I������Ă�����A�����ւ�
					// �K����Ƃ����R�t���Ȃ���Ԃɂ���
					if( find(sindyTableType::city_admin) != end() )
						empty( sindyTableType::city_admin );
					CSPCityAdmin cFeat( new CCityAdmin(ipFeature, sindyTableType::city_admin, false, cAdminTable.GetFieldMap(), cAdminTable.GetNameString()) );
					push_back( cFeat );
					return true;
				}
			}
		}
	}

	return false;
}

// �|�C���g��������s���E���Z�b�g(�s���E���ڎw��Ver)
void CAddrBasePoint::SetCityAdmin( const CSPCityAdmin& cAdmin )
{
	// ���ɍs���E���I������Ă�����A�����ւ�
	// �K����Ƃ����R�t���Ȃ���Ԃɂ���
	if( find(sindyTableType::city_admin) != end() )
		empty( sindyTableType::city_admin );
	CSPCityAdmin pAdmin( new CCityAdmin((IFeature*)(*cAdmin), sindyTableType::city_admin, false, cAdmin->GetFieldMap(), cAdmin->GetNameString()) );
	push_back( pAdmin );
}

}	// namespace addr
