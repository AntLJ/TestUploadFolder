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
#include "GP2WorkTxt.h"

#include <AddrLib/GouPoints.h>


// ������
bool CGP2WorkTxt::Init()
{
	// ���s�Ώێs�撬���R�[�h����
	m_cAddrCodeList.SetObject( m_rParam.m_cCityAdminFC );

	if( m_rParam.m_pairArea.first == _T("-ca")  )
	{
		if( !m_cAddrCodeList.MakeCodeListAll(5) )
		{
			std::wcerr << L"#Error �S�����̎s�撬���R�[�h���X�g�����Ɏ��s" << std::endl;
			return false;
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cl")  )
	{
		if( !m_cAddrCodeList.AddFromFile(m_rParam.m_pairArea.second, 5) )
		{
			std::wcerr << L"#Error �s�撬�����X�g����̎s�撬���R�[�h���X�g�����Ɏ��s : " << m_rParam.m_pairArea.second << std::endl;
			return false;
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cr") )
	{
		CString strRange( m_rParam.m_pairArea.second );
		int nPos = strRange.Find( _T('-') ) ;
		if( nPos > 0 )
		{
			if( !m_cAddrCodeList.MakeCodeListFromRange(strRange.Left(nPos), strRange.Mid(nPos+1)) )
			{
				std::wcerr << L"#Error �s�撬�����X�g����̎s�撬���R�[�h���X�g�����Ɏ��s : " << m_rParam.m_pairArea.second << std::endl;
				return false;
			}
		}
		else
		{
			std::wcerr << L"#Error �s�撬���R�[�h�w��t�H�[�}�b�g���s���ł��B(start-end) : " << m_rParam.m_pairArea.second << std::endl;
			return false;
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cs") )
	{
		m_cAddrCodeList.Add( m_rParam.m_pairArea.second );
	}
	else
	{
		std::wcerr << L"#Error ���s�G���A������ł��܂��� : " << m_rParam.m_pairArea.first << std::endl;
		return false;
	}

	return true;
}

// �f�[�^�o��
bool CGP2WorkTxt::WriteData()
{
	CGouPoints cGous( m_rParam.m_cCityAdminFC );
	for( CAddrCodeList::const_iterator itCode = m_cAddrCodeList.begin(); itCode != m_cAddrCodeList.end(); ++itCode )
	{
		std::cout << "#" << CT2CA(*itCode) << std::endl;

		cGous.SelectByAddrCode( *itCode );
		if(! cGous.empty(cGous.GetTableType()) )
		{
			CString strFile;
			strFile.Format( _T("%s\\gpw%s.txt"), m_rParam.m_strOutDir, *itCode );
			std::ofstream ofs( strFile, std::ios::binary );
			if( ofs )
			{
				long lCount = 1;
				for( CCityAdmin::const_rows_iterator itAdmin = cGous.begin(cGous.GetTableType()); itAdmin != cGous.end(cGous.GetTableType()); ++itAdmin, lCount++ )
				{
					// �s���E���ɂ��鍆�|�C���g�擾
					const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);

					// �Z���\�������ςŊX�敄����NULL�ɂȂ��Ă���G���A�̓f�[�^�����Ȃ�
					if( pAdmin->GetGaikuFugo() == 0 
						&& (pAdmin->GetAddrClass() == city_admin::addr_class::kChibanMaintenance || pAdmin->GetAddrClass() == city_admin::addr_class::kResidenceIndicationMaintenance) )
					{
						PrintError( pAdmin.get(), _T("NOTICE"), _T("�Z���\�������ςŊX�敄����NULL�Ȃ̂Ńf�[�^�����܂���"), pAdmin->GetAddrCode() );
						continue;
					}

					for( CGouPoint::const_rows_iterator itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou )
					{
						const boost::shared_ptr<CGouPoint> pGou = boost::static_pointer_cast<CGouPoint>(*itGou);

						// �Z���R�[�h�m�F
						//   �E20�����ł��Ȃ����̂́A����(�n�ԂȂ��Z����)
						//   �E�厚�E���R�[�h[999999]�̂��̂́A����(bug7975)
						CString strAddrCode = pGou->GetAddrCode();
						if( strAddrCode.IsEmpty() )
						{
							CString strInfo;
							strInfo.Format( _T("�s���E�R�[�h:%s �{ ���ԍ�:%s"), pAdmin->GetAddrCode(), pGou->GetGouNo() );
							PrintError( pGou.get(), _T("WARNING"), _T("20���R�[�h�����s"), strInfo );
							continue;
						}
						else if( strAddrCode.Mid(5, 6) == _T("999999") )
						{
							PrintError( pGou.get(), _T("NOTICE"), _T("�厚�E���R�[�h[999999]�̍s���E�G���A�ׁ̈A�f�[�^�����܂���"), strAddrCode );
							continue;
						}

						// ���b�V���R�[�h�E�o�x�E�ܓx�擾
						WKSPoint point;
						IGeometryPtr ipGeo;
						((IFeature*)*pGou)->get_Shape( &ipGeo );
						((IPointPtr)ipGeo)->QueryCoords( &point.X, &point.Y );

						// �f�[�^�����o��
						// [ObjectID],[�Z���R�[�h],[���^�C�v],[�s���E�G���A���],[�D��t���O],[���b�V���R�[�h],[10�i�o�x],[10�i�ܓx]
						CString strData;
						strData.Format( _T("%d,%s,%c,%d,%d,%d,%.12f,%.12f"), 
										pGou->GetOID(),								// OID
										strAddrCode,								// �Z���R�[�h
										pGou->GetGouType() == 1? _T('g') : _T('t'),	// GouType
										pAdmin->GetAreaClass(),						// �s���E�G���A���
										pGou->IsPrior()? 1 : 0,						// �D�捆
										GetMeshCode(point.X, point.Y), point.X, point.Y	);				// ���b�V���R�[�h���W
						ofs << CT2CA(strData) << "\n";

					}
					ofs.flush();
					std::cout << lCount << " / " << cGous.size(cGous.GetTableType()) << " �s���E\r";
				}
				std::cout << std::endl;
			}
			else
				std::cerr << "#Error �o�̓t�@�C���̍쐬�Ɏ��s : " << CT2CA(strFile) << std::endl;
		}
	}
	return true;
}
