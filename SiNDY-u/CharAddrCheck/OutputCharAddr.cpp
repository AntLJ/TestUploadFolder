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
#include "OutputCharAddr.h"
#include "CharAddrConvertFunc.h"

#include <sindy/schema.h>

COutputCharAddr::COutputCharAddr()
{
}

COutputCharAddr::~COutputCharAddr()
{
}

void  COutputCharAddr::Init( IFeatureClass* ipGouPoint, IFeatureClass* ipCityAdmin, ODatabase& eOraDB, LPCTSTR lpcszOutput, ITable* ipWorkPorposeTable )
{
	m_ipGouPointClass = ipGouPoint;
	m_ipCityAdminClass = ipCityAdmin;
	m_eOraDB = eOraDB;
	m_strOutput = lpcszOutput;
	m_ipWorkPorposeTable = ipWorkPorposeTable;
}

bool COutputCharAddr::run( LPCTSTR lpcszConvertTablePath, LPCTSTR lpcszGouPoint, LPCTSTR lpcszCityAdmin )
{
	bool bRet = true;

	// �����ɗ���̂̓I�v�V�����Ɂu-i(--infoaddr)�v���w�肵���Ƃ��̂݁B
	// �����ōs�������̓T�[�o��ɂ��镶���t���Z���f�[�^�ꗗ���擾���邾���Ȃ̂�
	// �ϊ��e�[�u���iy0000.mdb�j�̊����ǂ݃e�[�u���A�I�u�W�F�N�gID�����ǂ݃e�[�u���͕K�v�Ȃ���
	// CCharAddrConvert�̏������̍ۂɃG���[������ƋC�����悭�Ȃ��̂ŏI�������܂��B
	std::multimap<long,CString> mapError = m_cCharAddrConvert.Init( lpcszConvertTablePath );
	if( mapError.empty() )
	{
		std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapGouPoint;
		std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapCityAdmin;
		if( !CString(lpcszGouPoint).IsEmpty() && !CString(lpcszCityAdmin).IsEmpty() )
		{
			// �����t�����|�C���g���擾
			std::cerr << "\n�����t�����|�C���g���擾��...\n";
			mapGouPoint = GetItems2( _T("LIKE '#%'"), sindy::schema::gou_point::kGouNo, m_ipGouPointClass, lpcszGouPoint, 0 );

			// �����t���s���E���擾
			std::cerr << "�����t���s�s�n�}�s���E���擾��...\n";
			mapCityAdmin = GetItems2( _T("IS NOT NULL"), sindy::schema::city_admin::kExtGaikuFugo, m_ipCityAdminClass, lpcszCityAdmin, 1 );
		}
		else
		{
			// �����t�����|�C���g���擾
			std::cerr << "\n�����t�����|�C���g���擾��...\n";
			mapGouPoint = GetItems( _T("LIKE '#%'"), sindy::schema::gou_point::kGouNo, m_ipGouPointClass, 0 );

			// �����t���s���E���擾
			std::cerr << "�����t���s�s�n�}�s���E���擾��...\n";
			mapCityAdmin = GetItems( _T("IS NOT NULL"), sindy::schema::city_admin::kExtGaikuFugo, m_ipCityAdminClass, 1 );
		}

		if(!mapGouPoint.empty()){
			// �Ƃ肠�������X�g���o�́i���|�C���g�j
			std::cerr << "�����t�����|�C���g���X�g�o�͒�...\n";
			WriteCharAddrList( mapGouPoint, _T("goulist"), sindy::schema::gou_point::kTableName, m_ipGouPointClass );
		}

		if(!mapCityAdmin.empty()){
			// �Ƃ肠�������X�g���o�́i�s���E�j
			std::cerr << "�����t���s�s�n�}�s���E���X�g�o�͒�...\n";
			WriteCharAddrList( mapCityAdmin, _T("cityadminlist"), sindy::schema::city_admin::kTableName, m_ipCityAdminClass );
		}

		// ���|�C���g�̕�����𕪊����ă��X�g��
		std::cerr << "�����t�����|�C���g���X�g��P�ꖈ�ɕ�����...\n";
		std::map<CString,std::map<CString,long>> mapGouCharList = getDataFromList( m_ipCityAdminClass, mapGouPoint, 0 );

		// �s���E�̕�����𕪊����ă��X�g��
		std::cerr << "�����t���s�s�n�}�s���E���X�g��P�ꖈ�ɕ�����...\n";
		std::map<CString,std::map<CString,long>> mapCityAdminCharList = getDataFromList( m_ipCityAdminClass, mapCityAdmin, 1 );

		// ���|�C���g�ƍs���E�̃��X�g�����������
		CombineCharList( mapGouCharList, mapCityAdminCharList );

		// �P�ꖈ�ɕ������������t�����X�g���o��
		std::cerr << "�P�ꖈ�ɕ������������t�����X�g���o�͒�...\n";
		WriteCharAddrList2( mapGouCharList, _T("checkcharList") );
	}
	else
	{
		bRet = false;
		std::cerr << "�ϊ��e�[�u���ɃG���[���������̂ŏ����I��\n";
		for( std::multimap<long,CString>::const_iterator it = mapError.begin(); it != mapError.end(); ++it )
			std::cout << "#ERRROR\t" << it->first << "\t" << it->second << "\n";
	}

	return bRet;
}

std::map<CString,std::map<CString,long>> COutputCharAddr::getDataFromList( IFeatureClass* ipCityAdminClass, const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapList, long lType )
{
	std::map<CString,std::map<CString,long>> mapRetCharAndAddr;
	long lCityCodeIndex = ipCityAdminClass->_FindField(sindy::schema::city_admin::kCityCode);
	long lCityAddrCodeIndex = ipCityAdminClass->_FindField(sindy::schema::city_admin::kAddrCode);

	long lCount = 0, lSize = mapList.size();
	for( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>::const_iterator it = mapList.begin(); it != mapList.end(); ++it )
	{
		lCount++;
		std::cerr << lCount << _T(" / ") << lSize << _T("\r");

		CString strCode11;
		// ���|�C���g���X�g
		if( lType == 0 )
		{
			ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
			ipSpFilter->putref_Geometry( it->second.second.m_T->GetShapeCopy() );
			ipSpFilter->put_SpatialRel( esriSpatialRelIntersects );
			IFeatureCursorPtr ipCityAdminCursor;
			// 11���R�[�h�擾
			if( S_OK == ipCityAdminClass->Search( ipSpFilter, VARIANT_TRUE, &ipCityAdminCursor ) && ipCityAdminCursor )
			{
				IFeaturePtr ipCityAdmin;
				while( S_OK == ipCityAdminCursor->NextFeature( &ipCityAdmin ) && ipCityAdmin )
				{
					CComVariant vaCityCode = ipCityAdmin->GetValue( lCityCodeIndex );
					CComVariant vaCityAddrCode = ipCityAdmin->GetValue( lCityAddrCodeIndex );
					strCode11 = CString(vaCityCode.bstrVal) + CString(vaCityAddrCode.bstrVal);
					break;
				}
			}
		}
		// �s�s�n�}�s���E���X�g
		else if( lType == 1 )
		{
			// 11���R�[�h�擾
			CComVariant vaCityCode = it->second.second.m_T->GetValue( lCityCodeIndex );
			CComVariant vaCityAddrCode = it->second.second.m_T->GetValue( lCityAddrCodeIndex );
			strCode11 = CString(vaCityCode.bstrVal) + CString(vaCityAddrCode.bstrVal);
		}

		if( !strCode11.IsEmpty() )
		{
			std::list<CString> listChar;
			std::list<std::pair<YomiType,std::string>> listCharAddr = m_cCharAddrConvert.GetCharAddrList( it->second.first );
			for( std::list<std::pair<YomiType,std::string>>::const_iterator itCharAddr = listCharAddr.begin(); itCharAddr != listCharAddr.end(); ++itCharAddr )
			{
				if( itCharAddr->first == KanjiYomiTableType )
					listChar.push_back( itCharAddr->second.c_str() );
			}

			for( std::list<CString>::const_iterator itChar = listChar.begin(); itChar != listChar.end(); ++itChar )
			{
				std::map<CString,std::map<CString,long>>::iterator itCharAndAddr = mapRetCharAndAddr.find( *itChar );
				if( itCharAndAddr == mapRetCharAndAddr.end() )
					mapRetCharAndAddr[*itChar].insert( std::pair<CString,long>( strCode11, 1 ) );
				else
				{
					std::map<CString,long>::iterator itAddr = itCharAndAddr->second.find( strCode11 );
					if( itAddr == itCharAndAddr->second.end() )
						mapRetCharAndAddr[itCharAndAddr->first].insert( std::pair<CString,long>( strCode11, 1 ) );
					else
						mapRetCharAndAddr[itCharAndAddr->first][strCode11] += 1;
				}
			}
		}
	}
	std::cerr << std::endl;

	return mapRetCharAndAddr;
}

void COutputCharAddr::CombineCharList( std::map<CString,std::map<CString,long>>& mapGouCharList, const std::map<CString,std::map<CString,long>>& mapCityAdminCharList )
{
	for( std::map<CString,std::map<CString,long>>::const_iterator it = mapCityAdminCharList.begin(); it != mapCityAdminCharList.end(); ++it )
	{
		std::map<CString,std::map<CString,long>>::const_iterator itGouChar = mapGouCharList.find( it->first );
		if( itGouChar == mapGouCharList.end() )
			mapGouCharList.insert( std::pair<CString,std::map<CString,long>>( it->first, it->second ) );
		else
		{
			for( std::map<CString,long>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				std::map<CString,long>::const_iterator itAddr = it->second.find( it2->first );
				if( itAddr == it->second.end() )
					mapGouCharList[it->first].insert( std::pair<CString,long>( it2->first, 1 ) );
				else
					mapGouCharList[it->first][it2->first] += 1;
			}
		}
	}
}

void COutputCharAddr::WriteCharAddrList( const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapFeatureList, LPCTSTR lpcszOutputFile, LPCTSTR lpcszTableName, IFeatureClass* ipFeatureClass )
{
	CString strFilePath;
	strFilePath.Format( _T("%s\\%s.txt"), m_strOutput, lpcszOutputFile );
	if( PathFileExists( strFilePath ) )
		strFilePath = CreateLogFile( m_strOutput, lpcszOutputFile, 1 );
	std::ofstream ofs(static_cast<CT2W>(strFilePath));
	if( !ofs.fail() )
	{
		std::map<long,CString> mapPurpose;
		long lSourceIndex = ipFeatureClass->GetFields()->_FindField(sindy::schema::gou_point::kSource);	// ���\�[�X���Ȃ̂łǂ�����Ƃ��Ă��Ă������ł��傤
		long lModifyDateIndex = ipFeatureClass->GetFields()->_FindField(sindy::schema::gou_point::kModifyDate);	// �ŏI�X�V���Ȃ̂łǂ�����Ƃ��Ă��Ă������ł��傤
		long lPurposeCIndex = ipFeatureClass->GetFields()->_FindField(sindy::schema::gou_point::kPurpose);	// ��ƖړI�Ȃ̂łǂ�����Ƃ��Ă��Ă������ł��傤
		long lPurposeIndex = m_ipWorkPorposeTable->GetFields()->_FindField(sindy::schema::workpurpose::kPurpose);	// ��ƖړI�Ȃ̂łǂ�����Ƃ��Ă��Ă������ł��傤
		if(lSourceIndex >= 0 && lModifyDateIndex >= 0 && lPurposeCIndex >= 0 && lPurposeIndex >= 0){
			ofs << "#FREESTYLELOG\n";
			ofs << "FLAG\tLAYER\tOBJECTID\t�����t�Z��\t��ƖړI\t�ŏI�X�V��\t���\�[�X\t\n";
			for( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>::const_iterator it = mapFeatureList.begin(); it != mapFeatureList.end(); ++it ){
				CComVariant vaSource, vaMofifyDate, vaPurpose;
				it->second.second.m_T->get_Value(lSourceIndex, &vaSource);
				it->second.second.m_T->get_Value(lModifyDateIndex, &vaMofifyDate);
				it->second.second.m_T->get_Value(lPurposeCIndex, &vaPurpose);
				CString strSource, strModifyDate, strPurpose;
				if(vaSource.vt != VT_EMPTY)
					strSource = CString(vaSource.bstrVal);
				if(vaMofifyDate.vt != VT_EMPTY){
					SYSTEMTIME aTime;
					::VariantTimeToSystemTime(vaMofifyDate.date, &aTime);
					strModifyDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),aTime.wYear, aTime.wMonth, aTime.wDay,	aTime.wHour, aTime.wMinute, aTime.wSecond);
				}
				if(vaPurpose.vt != VT_EMPTY){
					if(vaPurpose.lVal != 0){
						std::map<long,CString>::const_iterator it = mapPurpose.find(vaPurpose.lVal);
						if(it != mapPurpose.end()){
							strPurpose = it->second;
						} else {
							_IRowPtr ipRow;
							IQueryFilterPtr ipQF(CLSID_QueryFilter);
							CString strWhere;
							strWhere.Format(_T("%s = %d"), sindy::schema::workpurpose::kPurposeID, vaPurpose.lVal);
							ipQF->put_WhereClause(CComBSTR(strWhere));
							_ICursorPtr ipCur;
							m_ipWorkPorposeTable->Search(ipQF, VARIANT_FALSE, &ipCur);
							if(ipCur){
								_IRowPtr ipRow;
								ipCur->NextRow(&ipRow);
								if(ipRow){
									CComVariant vaPurposeName;
									ipRow->get_Value(lPurposeIndex, &vaPurposeName);
									if(vaPurposeName.vt != VT_EMPTY){
										strPurpose = CString(vaPurposeName.bstrVal);
										mapPurpose[vaPurpose.lVal] = strPurpose;
									}
								}
							}
						}
					}
				}

				ofs << "0\t" << CT2A(lpcszTableName) << "\t" << it->first << _T("\t") << it->second.first << "\t" << CT2A(strPurpose) << "\t" << CT2A(strModifyDate) << "\t" << CT2A(strSource) << "\n";
			}
		} else {
			// ���\�[�X�A��ƖړI�A�ŏI�X�V���̂ǂꂩ�������ꍇ��NG�ł�
			std::cout << "#ERROR\t" << CT2A(lpcszTableName) << "\t���\�[�X�t�B�[���h���Ȃ�\n";
			std::cout << "#ERROR\t" << CT2A(lpcszTableName) << "\t�ŏI�X�V���t�B�[���h���Ȃ�\n";
			std::cout << "#ERROR\t" << CT2A(lpcszTableName) << "\t��ƖړI�t�B�[���h���Ȃ�\n";
			std::cout << "#ERROR\t" << CT2A(sindy::schema::workpurpose::kTableName) << "\t��ƖړI�t�B�[���h���Ȃ�\n";
		}
	} else {
		std::cout << "#ERROR\t" << CT2A(strFilePath) << "\t���O�t�@�C���쐬���s\n";
	}
}

void COutputCharAddr::WriteCharAddrList2( const std::map<CString,std::map<CString,long>>& mapCharAddr, LPCTSTR lpcszOutputFile )
{
	CString strFilePath;
	strFilePath.Format( _T("%s\\%s.txt"), m_strOutput, lpcszOutputFile );
	if( PathFileExists( strFilePath ) )
		strFilePath = CreateLogFile( m_strOutput, lpcszOutputFile, 1 );
	std::ofstream ofs(static_cast<CT2W>(strFilePath));
	if( !ofs.fail() )
	{
		for( std::map<CString,std::map<CString,long>>::const_iterator it = mapCharAddr.begin(); it != mapCharAddr.end(); ++it )
		{
			for( std::map<CString,long>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				char lpAddr[100] = {0};
				if( CodeToKanji( it2->first, lpAddr, m_eOraDB ) )
				{
					CString strAddr;
					strAddr.Format( _T("%s"), lpAddr );
					if( strAddr.IsEmpty() )
					{
						if( CodeToKanji( it2->first.Left(5), lpAddr, m_eOraDB ) )
						{
							strAddr.Format( _T("%s"), lpAddr );
							if( strAddr.IsEmpty() )
								ofs << CT2CA(it->first) << "\t" << CT2CA(it2->first) << "\t\t" << it2->second << "��\n";
							else
								ofs << CT2CA(it->first) << "\t" << CT2CA(it2->first) << "\t" << CT2CA(strAddr) << "\t" << it2->second << "��\n";
						}
					}
					else
						ofs << CT2CA(it->first) << "\t" << CT2CA(it2->first) << "\t" << CT2CA(strAddr) << "\t" << it2->second << "��\n";
				}
			}
		}
	} else {
		std::cout << "#ERROR\t" << CT2A(strFilePath) << "\t���O�t�@�C���쐬���s\n";
	}
}
