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
#include "Building.h"

#include "CSAddrPoint.h"
#include "GouPoint.h"

#include <SiNDYLib/FeatureClass.h>
#include <ArcHelperEx/GlobalFunctions.h>

namespace addr
{
using namespace sindy;
using namespace schema;

// ��\�Z���R�[�h�擾
CString CBuilding::GetRepAddrCode( IWorkspace* ipAddrWorkspace /* = NULL */ ) const
{
	if( m_strRepAddrCode.IsEmpty() )
	{
		// ���w�莞�́A�����Ɠ��ꃏ�[�N�X�y�[�X����擾
		if( !ipAddrWorkspace )
			ipAddrWorkspace = GetWorkspace();

		static CFeatureClass cCSFC, cGPFC, cKPFC;
		if( ipAddrWorkspace )
		{			
			if( ! cCSFC )
			{
				IFeatureClassPtr ipCSClass;
				((IFeatureWorkspacePtr)ipAddrWorkspace)->OpenFeatureClass( _bstr_t(cs_addr_point::kTableName), &ipCSClass );
				cCSFC.SetObject( ipCSClass );
			}
			if( ! cGPFC )
			{
				IFeatureClassPtr ipGPClass;
				((IFeatureWorkspacePtr)ipAddrWorkspace)->OpenFeatureClass( _bstr_t(gou_point::kTableName), &ipGPClass );
				cGPFC.SetObject( ipGPClass );
			}
			if( ! cKPFC )
			{
				IFeatureClassPtr ipKPClass;
				((IFeatureWorkspacePtr)ipAddrWorkspace)->OpenFeatureClass( _bstr_t(klink_point::kTableName), &ipKPClass );
				cKPFC.SetObject( ipKPClass );
			}
		}

		// CS -> GP -> KP�̏��Ɍ��Ă���
		std::list<CString> listCode;
		IGeometryPtr ipGeom( GetShapeCopy() );
		IFeatureCursorPtr ipCursor( cCSFC.Search(AheInitSpatialFilter(NULL, ipGeom), false) );
		if( ipCursor )
		{
			IFeaturePtr ipFeature;
			while( ipCursor->NextFeature(&ipFeature) == S_OK )
			{
				CCSAddrPoint cCSPoint( ipFeature, sindyTableType::cs_addr_point, false, cCSFC.GetFieldMap(), cCSFC.GetNameString() );
				listCode.push_back( cCSPoint.GetAddrCode() );
			}			
		}

		if( listCode.empty() )
		{
			ipCursor = cGPFC.Search(AheInitSpatialFilter(NULL, ipGeom), false);
			if( ipCursor )
			{
				IFeaturePtr ipFeature;
				while( ipCursor->NextFeature(&ipFeature) == S_OK )
				{
					CGouPoint cGPPoint( ipFeature, sindyTableType::gou_point, false, cGPFC.GetFieldMap(), cGPFC.GetNameString() );
					// GOU ���� �G���A�����Ώ�
					if( cGPPoint.GetGouType() == 1 && city_admin::area_class::kInArea == cGPPoint.GetCityAdmin()->GetAreaClass() )
						listCode.push_back( cGPPoint.GetAddrCode() );
				}
			}
		}

		// �\�[�g���Đ擪�ɂ������̂���\�Z��
		if( !listCode.empty() )
		{
			listCode.sort();
			m_strRepAddrCode = *listCode.begin();
		}
	}
	return m_strRepAddrCode;
}

// �ʐώ擾
double CBuilding::GetArea()
{
	double dArea = 0.0;
	return SUCCEEDED(((IAreaPtr)(IGeometry*)*GetShape())->get_Area(&dArea)) ? dArea : 0.0;
}

}	// namespace addr
