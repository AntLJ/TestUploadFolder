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
#include "CS2WorkTxt.h"

#include <AddrLib/CSAddrPoint.h>

using namespace addr;

// �f�[�^�o��
bool CCS2WorkTxt::WriteData()
{
	long lAllCount = m_rParam.m_cPointFC.FeatureCount( NULL );
	IFeatureCursorPtr ipCursor( m_rParam.m_cPointFC.Search(NULL, VARIANT_FALSE) );
	if( lAllCount > 0 && ipCursor )
	{
		// �o�̓t�@�C���I�[�v��
		std::ofstream ofs( m_rParam.m_strOutFile, std::ios::out | std::ios::binary );
		if( !ofs )
		{
			std::cerr << "#Error �o�̓t�@�C���̃I�[�v�����s : " << CT2CA(m_rParam.m_strOutFile) << std::endl;
			return false;
		}


		CSPFieldMap cFields( m_rParam.m_cPointFC.GetFieldMap() );
		CSPTableNameString cNames( m_rParam.m_cPointFC.GetNameString() );

		long lCount = 1;
		IFeaturePtr ipFeature;
		while( ipCursor->NextFeature(&ipFeature) == S_OK )
		{
			CCSAddrPoint cCSPoint( ipFeature, sindyTableType::cs_addr_point, false, cFields, cNames );

			const CCityAdmin* pAdmin = cCSPoint.GetCityAdmin( (ITable*)m_rParam.m_cCityAdminFC );
			if( pAdmin )
			{
				city_admin::area_class::ECode eAreaClass = pAdmin->GetAreaClass();
				if( eAreaClass == city_admin::area_class::kSea )
				{
					// �C�Ȃ̂ŃG���[
					continue;
				}

				// ���b�V���R�[�h�E�o�x�E�ܓx�擾
				WKSPoint point;
				IGeometryPtr ipGeo;
				((IFeature*)cCSPoint)->get_Shape( &ipGeo );
				((IPointPtr)ipGeo)->QueryCoords( &point.X, &point.Y );

				CString strData;
				strData.Format( _T("%d,%s,%d,%d,%.12f,%.12f"), 
								cCSPoint.GetOID(),			// ObjectID
								cCSPoint.GetAddrCode(),		// 20���Z���R�[�h
								eAreaClass,					// �s���E�G���A���
								GetMeshCode(point.X, point.Y), point.X, point.Y );	// ���b�V���R�[�h���W

				ofs << CT2CA(strData) << "\n";
				ofs.flush();
			}
			else
				PrintError( &cCSPoint, _T("ERROR"), _T("�s���E���擾�ł��Ȃ� or ���E��ɂ���") );

			std::cout << "#" << lCount++ << " / " << lAllCount << " ������\r";
		}		

		return true;
	}
	else
		return false;
}
