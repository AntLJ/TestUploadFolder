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
#include "CheckConvert.h"
#include "CharAddrConvertFunc.h"

#include <sindy/schema.h>

CCheckConvert::CCheckConvert()
{
}

CCheckConvert::~CCheckConvert()
{
}

void  CCheckConvert::Init( IFeatureClass* ipGouPoint, IFeatureClass* ipCityAdmin, LPCTSTR lpcszConvertTablePath, LPCTSTR lpcszOutput )
{
	m_ipGouPointClass = ipGouPoint;
	m_ipCityAdminClass = ipCityAdmin;
	m_strConvertTable = lpcszConvertTablePath;
	m_strOutput = lpcszOutput;
}

bool CCheckConvert::run()
{
	bool bReturn = true;

	std::multimap<long,CString> mapError = m_cCharAddrConvert.Init( m_strConvertTable );
	if( mapError.empty() )
	{
		// ���|�C���g���當���t���Z�����X�g�擾
		std::cerr << "\n�����t�����|�C���g���擾��...\n";
		std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapGouPoint = GetItems( _T("LIKE '#%'"), sindy::schema::gou_point::kGouNo, m_ipGouPointClass, 0 );
		long lGouPointSize = mapGouPoint.size();

		// �s���E���當���t���Z�����X�g�擾
		std::cerr << "�����t���s�s�n�}�s���E���擾��...\n";
		std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapCityAdmin = GetItems( _T("IS NOT NULL"), sindy::schema::city_admin::kExtGaikuFugo, m_ipCityAdminClass, 1 );
		long lCityAdminSize = mapCityAdmin.size();

		// <long,<CString,long>> OID,�ǂ�,���v�^�C�v(0:�Ȃ�,1:�����ǂ�,2:�I�u�W�F�N�g�ǂ�)
		std::map<long,std::pair<CString,long>> mapConvertedGouPoint;
		std::map<long,std::pair<CString,long>> mapConvertedCityAdmin;

		// ���|�C���g�̃I�u�W�F�N�gID�ǂݕϊ�
		std::cerr << "���|�C���g�̃I�u�W�F�N�gID�ǂݕϊ���...\n";
		ConvertItemsFromObjectYomiTable( mapGouPoint, mapConvertedGouPoint, GouPoint_Layer );

		// �s���E�̃I�u�W�F�N�gID�ǂݕϊ�
		std::cerr << "�s�s�n�}�s���E�̂̃I�u�W�F�N�gID�ǂݕϊ���...\n";
		ConvertItemsFromObjectYomiTable( mapCityAdmin, mapConvertedCityAdmin, CityAdmin_Layer );


		// ���|�C���g�̊����ǂ݃e�[�u���ϊ�
		std::cerr << "���|�C���g�̊����ǂݕϊ���...\n";
		ConvertItemsFromKanjiYomiTable( mapGouPoint, mapConvertedGouPoint );

		// �s���E�̂̊����ǂ݃e�[�u���ϊ�
		std::cerr << "�s�s�n�}�s���E�̊����ǂݕϊ���...\n";
		ConvertItemsFromKanjiYomiTable( mapCityAdmin, mapConvertedCityAdmin );

		if( mapConvertedGouPoint.size() == lGouPointSize && mapConvertedCityAdmin.size() == lCityAdminSize )
		{
			// ���|�C���g�̕ϊ��̌��ʂ��o��
			std::cerr << "���|�C���g�̕ϊ����ʂ��o�͒�...\n";
			m_bGouNG = WriteConvertResult( mapConvertedGouPoint, _T("convertGouPoint"), m_strNGGouFilePath );

			// �s���E�̕ϊ��̌��ʂ��o��
			std::cerr << "�s�s�n�}�s���E�̕ϊ����ʂ��o�͒�...\n";
			m_bCityAdminNG = WriteConvertResult( mapConvertedCityAdmin, _T("convertCityAdmin"), m_strNGCityAdminFilePath );
		}
		else
		{
			std::cout << "#ERROR\t�����t���Z�����X�g�Ɠǂݕϊ���̃��X�g�̐�����v���܂���I\n";
			std::cerr << "���ʂ̏o�͂��ł��܂���ł���\n";
			bReturn = false;
		}
	}
	else
	{
		bReturn = false;
		std::cerr << "�ϊ��e�[�u���ɃG���[���������̂ŏ����I��\n";
		for( std::multimap<long,CString>::const_iterator it = mapError.begin(); it != mapError.end(); ++it )
			std::cout << "#ERRROR\t" << it->first << "\t" << it->second << "\n";
	}

	return bReturn;
}

void CCheckConvert::ConvertItemsFromObjectYomiTable( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, std::map<long,std::pair<CString,long>>& mapConvertedItems, GouOrCityAddrLayer LayerType )
{
	// �I�u�W�F�N�gID�ǂݕϊ��e�[�u������ǂ݂��擾����
	std::map<ObjectLayerPair,KanjiYomiPair> mapObjectYomiItems = m_cCharAddrConvert.getObjectYomiItems();
	std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapRet;
	long lCount = 0, lSize = mapItems.size();
	for( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>::const_iterator it = mapItems.begin(); it != mapItems.end(); ++it )
	{
		lCount++;
		if(lCount % 100 == 0)
			std::cerr << "\t������: " << lCount << " / " << lSize << "\r";

		// �ϊ��e�[�u���Ɠ������̂�����Ώ����o���p���X�g�ɒǉ�����
		bool bRet = false;
		std::string strYomi = m_cCharAddrConvert.GetYomiFromCharAddr_Object( LayerType, it->first, it->second.first, bRet );
		if( bRet )
			mapConvertedItems.insert( std::pair<long,std::pair<CString,long>>( it->first, std::pair<CString,long>( strYomi.c_str(), 2 ) ) );
		else
			mapRet.insert( *it );
	}
	std::cerr << "\t������: " << lCount << " / " << lSize << "\n";
	std::cerr << "\t�ϊ�����: " << mapItems.size() - mapRet.size() << " ��\n";
	mapItems.swap( mapRet );

	std::cerr << std::endl;
}

void CCheckConvert::ConvertItemsFromKanjiYomiTable( const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, std::map<long,std::pair<CString,long>>& mapConvertedItems )
{
	long lCount = 0, lSize = mapItems.size();
	long lSuccess = 0;
	for( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>::const_iterator it = mapItems.begin(); it != mapItems.end(); ++it )
	{
		lCount++;
		if(lCount % 100 == 0)
			std::cerr << "\t������: " << lCount << " / " << lSize << "\r";

		bool bSuccess = true;
		CString strYomi = CA2T( m_cCharAddrConvert.GetYomiFromCharAddr_Kanji( it->second.first, bSuccess ).c_str() );
		if( bSuccess )
		{
			lSuccess++;
			mapConvertedItems.insert( std::pair<long,std::pair<CString,long>>( it->first, std::pair<CString,long>( strYomi, 1 ) ) );
		}
		else
			mapConvertedItems.insert( std::pair<long,std::pair<CString,long>>( it->first, std::pair<CString,long>( strYomi, 0 ) ) );
	}
	std::cerr << "\t������: " << lCount << " / " << lSize << "\n";
	std::cerr << "\t�ϊ�����: " << lSuccess << " ��\n";
	std::cerr << std::endl;
}

bool CCheckConvert::WriteConvertResult( const std::map<long,std::pair<CString,long>>& mapConvertedItems, LPCTSTR lpcszOutputFile, CString& strNGFilePath )
{
	bool bNg = false;
	CString strOKFilePath;
	strOKFilePath.Format( _T("%s\\%s_OK.txt"), m_strOutput, lpcszOutputFile );
	strNGFilePath.Format( _T("%s\\%s_NG.txt"), m_strOutput, lpcszOutputFile );
	if( PathFileExists( strOKFilePath ) )
		strOKFilePath = CreateLogFile( m_strOutput, CString(lpcszOutputFile) + _T("_OK"), 1 );
	if( PathFileExists( strNGFilePath ) )
		strNGFilePath = CreateLogFile( m_strOutput, CString(lpcszOutputFile) + _T("_NG"), 1 );
	std::ofstream ofs_ok(static_cast<CT2W>(strOKFilePath));
	std::ofstream ofs_ng(static_cast<CT2W>(strNGFilePath));
	if( !ofs_ok.fail() && !ofs_ng.fail() )
	{
		for( std::map<long,std::pair<CString,long>>::const_iterator it = mapConvertedItems.begin(); it != mapConvertedItems.end(); ++it )
		{
			switch( it->second.second )
			{
			case 0:
				ofs_ng << "NG\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				bNg = true;
				break;
			case 1:
				ofs_ok << "OK(�����ǂ�)\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				break;
			case 2:
				ofs_ok << "OK(OID�ǂ�)\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				break;
			default:
				ofs_ng << "���̏o�͂͂��������I\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				std::cout << "#ERROR\t���̏o�͂͂��������I\t" << it->first << "\t" << CT2CA(it->second.first) << std::endl;
				break;
			}
		}
	} else {
		std::cout << "#ERROR\t" << CT2A(strOKFilePath) << "\t���O�t�@�C���쐬���s\n";
		std::cout << "#ERROR\t" << CT2A(strNGFilePath) << "\t���O�t�@�C���쐬���s\n";
	}
	return bNg;
}

bool CCheckConvert::get_GouCityAdminNG(CString& strGouPointText, CString& strCityAdminText)
{
	strGouPointText = m_strNGGouFilePath;
	strCityAdminText = m_strNGCityAdminFilePath;

	// �ǂ�����NG������ꍇ��true, �ǂ����NG�������ꍇ��false
	if(m_bGouNG || m_bCityAdminNG)
		return true;
	else
		return false;
}
