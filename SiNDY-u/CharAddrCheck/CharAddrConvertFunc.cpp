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
#include "CharAddrConvertFunc.h"

#include <sindy/workspace.h> 
#include <sindy/schema.h>

#include "ADOBase.h"
#include <TDC/common_check/NameYomiChk.h>

using namespace boost::spirit;
using namespace jis::x0208::spirit;

std::multimap<long,CString> CCharAddrConvert::Init( LPCTSTR lpcszCovnertTablePath )
{
	// �G���[�o�͂�˂�����
	std::multimap<long,CString> mapError;

	ITablePtr ipKanjiYomiTable, ipObjectYomiTable;
	CADOCursor cKanjiYomiTable;
	using namespace sindy;
	using namespace sindy::schema;
	if( !CString(lpcszCovnertTablePath).IsEmpty() )
	{
		INamePtr ipName(create_workspace_name(lpcszCovnertTablePath));
		if( ipName )
		{
			IWorkspacePtr ipConvertWorkspace(ipName->_Open());
			if( ipConvertWorkspace )
			{
				CString strConvertDB;
				strConvertDB.Format( _T("Provider=Microsoft.JET.OLEDB.4.0;Data source = %s"), lpcszCovnertTablePath );
				_ConnectionPtr ipConnection(__uuidof(Connection));
				if( ipConnection )
				{
					ipConnection->Open(_bstr_t(strConvertDB), "", "", adConnectUnspecified);
					// �ǂݕϊ��e�[�u�����擾����
					IFeatureWorkspacePtr ipConvertFWorkspace(ipConvertWorkspace);
					CADOBase ipConvertDB(ipConnection);
					cKanjiYomiTable = ipConvertDB.search( _T("select * from KanjiToYomi") );
					ipConvertFWorkspace->OpenTable(CComBSTR(_T("ObjectToYomi")), &ipObjectYomiTable);
					//if( cKanjiYomiTable.begin() == cKanjiYomiTable.end() ) // ����͐��������ʂł͂Ȃ����ANULL���Ă���Ǎ��߂Ȃ������Ɣ��f
					//	mapError.insert( std::pair<long,CString>( 1, _T("�e�[�u���I�[�v�����s\t�����ǂݕϊ��e�[�u��") ) );
					if( !ipObjectYomiTable )
						mapError.insert( std::pair<long,CString>( 2, _T("�e�[�u���I�[�v�����s\t�I�u�W�F�N�gID�ǂݕϊ��e�[�u��") ) );
				}
				else
					mapError.insert( std::pair<long,CString>( 1, _T("PGDB�ڑ����s") ) );
			}
			else
				mapError.insert( std::pair<long,CString>( 1, _T("�ϊ��e�[�u����IWorkspace���I�[�v���ł��܂���B") ) );
		}
		else
			mapError.insert( std::pair<long,CString>( 1, _T("PGDB�ڑ����s") ) );
	}
	else
		mapError.insert( std::pair<long,CString>( 1, _T("�e�[�u������NULL���Ă܂��B") ) );

	if( mapError.empty() )
	{
		m_cSubConvert.Init();
		// �I�u�W�F�N�gID�ǂݕϊ��e�[�u���̃��R�[�h���X�g���擾
		m_mapObjectYomiTable = GetObjectYomiTableItems( ipObjectYomiTable, mapError );
		
		// �����ǂݕϊ��e�[�u���̃��R�[�h���X�g���擾
		m_mapKanjiYomiTable = GetKanjiYomiTableItems( cKanjiYomiTable, mapError );
		
		if( mapError.empty() )
		{
			// �T�u�e�[�u���̃��R�[�h���X�g���擾����i�A���t�@�x�b�g�ǂ݁j
			m_mapSubTable = m_cSubConvert.GetSubtable();
		}
	}
	return mapError;
}

std::map<ObjectLayerPair,KanjiYomiPair> CCharAddrConvert::GetObjectYomiTableItems( ITable* ipTable, std::multimap<long,CString>& mapError )
{
	std::map<ObjectLayerPair,KanjiYomiPair> mapRet;

	// �I�u�W�F�N�gID�ǂ݃e�[�u���̃��R�[�h���X�g���擾����
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	_ICursorPtr ipCursor;
	if( S_OK == ipTable->Search( ipQFilter, VARIANT_FALSE, &ipCursor ) && ipCursor )
	{
		_IRowPtr ipRow;
		long lCount = 0;
		while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow )
		{
			lCount++;
			CComVariant vaOID = ipRow->GetValue( ipTable->_FindField( _bstr_t(_T("OBJECTID")) ) );
			CComVariant vaLayer = ipRow->GetValue( ipTable->_FindField( _bstr_t(_T("LAYER")) ) );
			CComVariant vaKanji = ipRow->GetValue( ipTable->_FindField( _bstr_t(_T("KANJI")) ) );
			CComVariant vaYomi = ipRow->GetValue( ipTable->_FindField( _bstr_t(_T("YOMI")) ) );
			long lLayerValue = vaLayer.lVal;
			if( lLayerValue == 1 || lLayerValue == 2 )
			{
				CString strKanji = vaKanji.bstrVal;
				CString strYomi = vaYomi.bstrVal;
				if( !strKanji.IsEmpty() && !strYomi.IsEmpty() )
				{
					// �l�����������R�[�h�����X�g�ɓ˂�����
					std::string lpwKanji = CT2A(strKanji);
					std::string lpwYomi = CT2A(strYomi);
					// [bug6542]�C��
					bool bName = IsCheckYomi( lpwKanji, ObjectNameType );
					bool bYomi = IsCheckYomi( lpwYomi, ObjectYomiType );

					if( !bName )
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�����s��\t") + strKanji ) );
					if( !bYomi )
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�ǂݕs��\t") + strYomi ) );

					if( bName && bYomi )
					{
						// [bug7333]�I�u�W�F�N�gID�ǂ݃e�[�u���̊����Ɠǂ݂���v���Ă��邩�`�F�b�N
						if(IsCheckKanjiYomi(strKanji, strYomi, vaOID.lVal, mapError))
						{
							if( lLayerValue == 1 )
								mapRet.insert( std::pair<ObjectLayerPair,KanjiYomiPair>( ObjectLayerPair( CityAdmin_Layer, vaOID.lVal ), KanjiYomiPair( lpwKanji, lpwYomi ) ) );
							else if( lLayerValue == 2 )
								mapRet.insert( std::pair<ObjectLayerPair,KanjiYomiPair>( ObjectLayerPair( GouPoint_Layer, vaOID.lVal ), KanjiYomiPair( lpwKanji, lpwYomi ) ) );
						}
					}
				}
				else
				{
					// �l���������Ȃ����̂̓G���[�o�̓��X�g�ɓ˂�����
					if( strKanji.IsEmpty() && !strYomi.IsEmpty() )
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t������NULL\t") + strYomi ) );
					else if( !strKanji.IsEmpty() && strYomi.IsEmpty() )
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�ǂ݂�NULL\t") + strKanji ) );
					else
						mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�ǂ����NULL") ) );
				}
			}
			else
				mapError.insert( std::pair<long,CString>( vaOID.lVal, _T("LAYER�l��0,1�ȊO���ݒ肳��Ă���") ) );
		}
	}
	return mapRet;
}

std::map<std::string,std::string> CCharAddrConvert::GetKanjiYomiTableItems( CADOCursor& cKanjiYomiCursor, std::multimap<long,CString>& mapError )
{
	std::map<std::string,std::string> mapRet;

	// �����ǂ݃e�[�u���̃��R�[�h���X�g���擾����
	long lCount = 0;
	std::vector<CADOCursor::Field> vecField = cKanjiYomiCursor.fields();
	for( CADOCursor::iterator it = cKanjiYomiCursor.begin(); it != cKanjiYomiCursor.end(); ++it )
	{
		lCount++;
		std::vector<CComVariant> vecValue = it.dereference();
		std::vector<CComVariant>::iterator itVal = vecValue.begin();
		CString strKanji, strYomi;
		for( std::vector<CADOCursor::Field>::iterator itField = vecField.begin(); itField != vecField.end(); ++itField )
		{
			CString strFieldName = itField->name;
			if( _tcsicmp( strFieldName, _T("KANJI") ) == 0 )
				strKanji = *itVal;
			else if( _tcsicmp( strFieldName, _T("YOMI") ) == 0 )
				strYomi = *itVal;
			itVal++;
		}
		if( !strKanji.IsEmpty() && !strYomi.IsEmpty() )
		{
			// �l�����������R�[�h�����X�g�ɓ˂�����
			std::string lpwKanji = CT2A(strKanji);
			std::string lpwYomi = CT2A(strYomi);
			// [bug6542]�C��
			bool bName = IsCheckYomi( lpwKanji, KanjiNameType );
			bool bYomi = IsCheckYomi( lpwYomi, KanjiYomiType );
			if( !bName )
				mapError.insert( std::pair<long,CString>( lCount, _T("�����ǂ݃e�[�u��\t�����s��\t") + strKanji ) );
			if( !bYomi )
				mapError.insert( std::pair<long,CString>( lCount, _T("�����ǂ݃e�[�u��\t�ǂݕs��\t") + strYomi ) );

			if( bName && bYomi )
				mapRet.insert( std::pair<std::string,std::string>( lpwKanji, lpwYomi ) );
		}
		else
		{
			// �l���������Ȃ����̂̓G���[�o�̓��X�g�ɓ˂�����
			if( strKanji.IsEmpty() && !strYomi.IsEmpty() )
				mapError.insert( std::pair<long,CString>( lCount, _T("�����ǂ݃e�[�u��\t������NULL\t") + strYomi ) );
			else if( !strKanji.IsEmpty() && strYomi.IsEmpty() )
				mapError.insert( std::pair<long,CString>( lCount, _T("�����ǂ݃e�[�u��\t�ǂ݂�NULL\t") + strKanji ) );
			else
				mapError.insert( std::pair<long,CString>( lCount, _T("�����ǂ݃e�[�u��\t�ǂ����NULL") ) );
		}
	}
	return mapRet;
}

std::string CCharAddrConvert::GetYomiFromCharAddr_Object( GouOrCityAddrLayer LayerType, long lOID, LPCTSTR lpcszCharAddr, bool& bRet )
{
	std::string strRet;

	// �w��I�u�W�F�N�g���I�u�W�F�N�gID�ǂ݃e�[�u���ɂ��邩�`�F�b�N
	std::map<ObjectLayerPair,KanjiYomiPair>::const_iterator it = m_mapObjectYomiTable.find( ObjectLayerPair( LayerType, lOID ) );
	if( it != m_mapObjectYomiTable.end() )
	{
		// �I�u�W�F�N�gID�ǂ݃e�[�u���ɂ����Ă����R�[�h���̖��̂������t���Z�����̂ƈႤ�ꍇ������̂ł����Ń`�F�b�N
		// ��j�e�[�u���ɂ�[OID:123456, Layer:gou_point, Name:����]�Ɠ����Ă��邪�A���ۂɃ`�F�b�N������OID:123456�̍��|�C���g����́u�����P�Q�R�v�Ƃ������̂ł���ꍇ�B
		//      �{���Ȃ��[OID:123456, Layer:gou_point, Name:�����P�Q�R]�ƂȂ��Ă��Ȃ���΂Ȃ�Ȃ��B
		//      �����炭��Ǝ҂��ԈႦ�ē��͂��Ă��܂����̂������B
		std::string strTemp = CT2A(lpcszCharAddr);
		if( it->second.first.compare( strTemp ) == 0 )
		{
			strRet = it->second.second;
			bRet = true;
		}
		else
			bRet = false;
	}
	else
		bRet = false;

	return strRet;
}

std::string CCharAddrConvert::GetYomiFromCharAddr_Kanji( LPCTSTR lpcszCharAddr, bool& bRet )
{
	std::string strRet;

	// �����t���Z���̕������P�ꖈ�ɕ������ă��X�g�ɂ���
	std::list<std::pair<YomiType,std::string>> listRet = GetCharAddrList( lpcszCharAddr );

	// ���X�g���������̂������ǂݕϊ�����
	strRet = GetYomi( listRet, bRet );

	return strRet;
}

std::list<std::pair<YomiType,std::string>> CCharAddrConvert::GetCharAddrList( LPCTSTR lpcszCharAddr, const sjis_ranges& sjisSeparate )
{
	std::list<std::pair<YomiType,std::string>> listCharAddr;
	std::vector<std::string> vecCharAddr;

	std::string strCharAddr = CT2A(lpcszCharAddr);

	// ��������������ȃJ�i�A�p���A�����ɕ������鏈��
	// �֗��ȃ��C�u�����iLIB/jis�j���������̂Ŏg�p�B�B�B
	parse_info<> aInfo = 
		parse( strCharAddr.c_str(), *( 
			(+m_cSubConvert.getKanjiKana())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getNumber())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getAlphabet())[push_back_a(vecCharAddr)] |
			(+sjisSeparate)[push_back_a(vecCharAddr)] |
			+anychar_p ) );

	// ����������������X�Ɂu���ȃJ�i�A�A���t�@�x�b�g�A�����A�����ǂ݁v�ɕ�����
	for( std::vector<std::string>::const_iterator it = vecCharAddr.begin(); it != vecCharAddr.end(); ++it )
	{
		// �������ȃJ�i��
		parse_info<> aInfoKanjiKana = parse( it->c_str(), *(m_cSubConvert.getKanjiKana()) );
		// ������
		parse_info<> aInfoNumber = parse( it->c_str(), *(m_cSubConvert.getNumber()) );
		// �p����
		parse_info<> aInfoAlphabet = parse( it->c_str(), *(m_cSubConvert.getAlphabet()) );

		// �����Ƃ��ȃJ�i���`�F�b�N
		if( aInfoKanjiKana.full )
		{
			parse_info<> aInfoKana = parse( it->c_str(), *(m_cSubConvert.getKana()) );
			// ���ȃJ�i�݂̂�����ȊO��
			if( aInfoKana.full )
				listCharAddr.push_back( std::pair<YomiType,std::string>( HiraKataType, *it ) );
			else
				listCharAddr.push_back( std::pair<YomiType,std::string>( KanjiYomiTableType, *it ) );
		}
		// �����������`�F�b�N
		else if( aInfoNumber.full )
			listCharAddr.push_back( std::pair<YomiType,std::string>( NumberType, *it ) );
		// �A���t�@�x�b�g�������`�F�b�N
		else if( aInfoAlphabet.full )
		{
			std::wstring strWStr = CA2W(it->c_str());
			// �A���t�@�x�b�g��3�����ȏ�͊����ǂݕϊ��̑ΏۂƂȂ�
			if( strWStr.size() > 2 )
				listCharAddr.push_back( std::pair<YomiType,std::string>( KanjiYomiTableType, *it ) );
			else
				listCharAddr.push_back( std::pair<YomiType,std::string>( AlphabetType, *it ) );
		}
		// ��L�ȊO
		else
			listCharAddr.push_back( std::pair<YomiType,std::string>( NoneType, *it ) );
	}

	return listCharAddr;
}

std::list<std::pair<YomiType,std::string>> CCharAddrConvert::GetCharAddrList2( LPCTSTR lpcszCharAddr, const sjis_ranges& sjisSeparate )
{
	std::list<std::pair<YomiType,std::string>> listCharAddr;
	std::vector<std::string> vecCharAddr;

	std::string strCharAddr = CT2A(lpcszCharAddr);

	// ��������������ȃJ�i�p���A�����ɕ������鏈��
	// �֗��ȃ��C�u�����iLIB/jis�j���������̂Ŏg�p�B�B�B
	parse_info<> aInfo = 
		parse( strCharAddr.c_str(), *( 
			(+m_cSubConvert.getKanjiKana().add(m_cSubConvert.getAlphabet()))[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getNumber())[push_back_a(vecCharAddr)] |
			(+sjisSeparate)[push_back_a(vecCharAddr)] |
			+anychar_p ) );

	// ����������������X�Ɂu���ȃJ�i�A�A���t�@�x�b�g�A�����A�����ǂ݁v�ɕ�����
	for( std::vector<std::string>::const_iterator it = vecCharAddr.begin(); it != vecCharAddr.end(); ++it )
	{
		// �������ȃJ�i�p����
		parse_info<> aInfoKanjiKanaAlpha = parse( it->c_str(), *(m_cSubConvert.getKanjiKana().add(m_cSubConvert.getAlphabet())) );
		// ������
		parse_info<> aInfoNumber = parse( it->c_str(), *(m_cSubConvert.getNumber()) );

		// �p����
		parse_info<> aInfoAlphabet = parse( it->c_str(), *(m_cSubConvert.getAlphabet()) );

		// �����Ƃ��ȃJ�i�p�����`�F�b�N
		if( aInfoKanjiKanaAlpha.full )
		{
			parse_info<> aInfoKana = parse( it->c_str(), *(m_cSubConvert.getKana()) );
			// ���ȃJ�i�݂̂�����ȊO��
			if( aInfoKana.full )
				listCharAddr.push_back( std::pair<YomiType,std::string>( HiraKataType, *it ) );
			else
			{
				// �p��������
				if(aInfoAlphabet.full)
				{
					std::wstring strWStr = CA2W(it->c_str());
					// �A���t�@�x�b�g��3�����ȏ�͊����ǂݕϊ��̑ΏۂƂȂ�
					if( strWStr.size() >= 3 )
						listCharAddr.push_back( std::pair<YomiType,std::string>( KanjiYomiTableType, *it ) );
					else
						listCharAddr.push_back( std::pair<YomiType,std::string>( AlphabetType, *it ) );
				}
				else
					listCharAddr.push_back( std::pair<YomiType,std::string>( KanjiYomiTableType, *it ) );
			}
		}
		// �����������`�F�b�N
		else if( aInfoNumber.full )
			listCharAddr.push_back( std::pair<YomiType,std::string>( NumberType, *it ) );
		// ��L�ȊO
		else
			listCharAddr.push_back( std::pair<YomiType,std::string>( NoneType, *it ) );
	}

	return listCharAddr;
}

std::string CCharAddrConvert::GetYomiFromKanjiYomiTable( const std::string& strCharAddr, bool& bRet )
{
	std::string strRet;
	// �����ǂ݃e�[�u������ǂ݂��擾
	std::map<std::string,std::string>::const_iterator it = m_mapKanjiYomiTable.find( strCharAddr );
	if( it == m_mapKanjiYomiTable.end() )
	{
		strRet = strCharAddr;
		bRet = false;
	}
	else
		strRet = it->second;
	return strRet;
}

std::string CCharAddrConvert::GetYomiFromSubTable( const std::wstring& strCharAddr, bool& bRet )
{
	std::string strRet;
	// �T�u�e�[�u������ǂ݂��擾
	std::map<std::wstring,std::wstring>::const_iterator it = m_mapSubTable.find( strCharAddr );
	if( it == m_mapSubTable.end() )
	{
		strRet = CW2A(strCharAddr.c_str());
		bRet = false;
	}
	else
		strRet = CW2A(it->second.c_str());
	return strRet;
}

std::string CCharAddrConvert::GetYomi( const std::list<std::pair<YomiType,std::string>>& listCharAddr, bool& bRet )
{
	// ���X�g�ɂ͂���Ȋ����œ����Ă܂�
	// ��)���f�[�^�@�@�@�@�@�FAB������-12C����EFG34
	// �@ ���f�[�^�i���X�g�j�FAB(�A���t�@�x�b�g),������(���ȃJ�i),-(�Ȃ�),12(����),C(�A���t�@�x�b�g),����(�����ǂ�),EFG(�����ǂ�),34(����)
	// �@ �o�̓f�[�^�@�@�@�@�F�G�[�r�[�A�C�E�|�P�Q�V�[�^�J�n�V�C�[�G�t�W�[�R�S
	std::string strRet;
	bool bRetYomi = true;
	for( std::list<std::pair<YomiType,std::string>>::const_iterator it = listCharAddr.begin(); it != listCharAddr.end(); ++it )
	{
		switch( it->first )
		{
		// ����
		case NumberType:
			{
				// �S������
				// ����������S�p�ɕϊ�
				strRet.append(ConvertHanNumToZenNum(it->second));
			}
			break;
		// �A���t�@�x�b�g
		case AlphabetType:
			{
				std::wstring strWStr = CA2W(it->second.c_str());
				// �A���t�@�x�b�g2�����܂ł̓T�u�e�[�u���œǂ݂�t�^
				for( int i = 0; i < int(strWStr.size()); ++i )
					strRet.append( GetYomiFromSubTable( strWStr.substr( i, i+1 ), bRetYomi ) );
				break;
			}
		// ���ȃJ�i
		case HiraKataType:
			{
				// �S�����ȃJ�i
				// ���Ȃ��J�i�֕ϊ�
				strRet.append(ConvertHiraToKata(it->second));
			}
			break;
		// �����ǂ݃e�[�u��
		case KanjiYomiTableType:
			strRet.append( GetYomiFromKanjiYomiTable( it->second, bRetYomi ) );
			break;
		// �Y���Ȃ�
		case NoneType:
			if( it->second == "-" )
				strRet.append( "�|" );
			else
				bRetYomi = false;
			break;
		default:
			bRetYomi = false;
			break;
		}
	}
	bRet = bRetYomi;

	return strRet;
}

bool CCharAddrConvert::IsCheckYomi( const std::string& strStr, CharAddrType nType )
{
	bool bRet = false;

	switch( nType )
	{
	case ObjectNameType:
		// �܂��͎g�pOK�̕����������g���Ă��邩�`�F�b�N
		// sjis���A��񐅏������{�S�p���ȃJ�i�{�S�p�p���{���p�����{[�[�|-�X]
		if(parse( strStr.c_str(), *(m_cSubConvert.getObjectName()) ).full)
		{
			// �P�ꖈ�ɕ�������
			std::list<std::pair<YomiType,std::string>> listStr = GetCharAddrList( strStr.c_str() );
			// �ŏ��ƍŌオ�u-�v��������NG
			if(_tcscmp(CA2T(listStr.begin()->second.c_str()), _T("-")) == 0 || _tcscmp(CA2T(listStr.rbegin()->second.c_str()), _T("-")) == 0)
				break;

			bool bKanji = false, bOther = false;
			for(std::list<std::pair<YomiType,std::string>>::iterator it = listStr.begin(); it != listStr.end(); ++it)
			{
				switch(it->first)
				{
				case AlphabetType:
					if(std::wstring(CA2W(it->second.c_str())).length() >= 3)	// 3�����ȏ�Ȃ犿���Ɠ�������
						bKanji = true;
					break;
				case KanjiYomiTableType:
					bKanji = true;
					break;
				case NoneType:
					if(_tcscmp(it->second.c_str(), _T("-")) != 0)	// �u-�v�łȂ��ꍇ��NG
						bOther = true;
				case NumberType:
				case HiraKataType:
					break;
				default:
					bOther = true;
					break;
				}
			}
			// �������g���Ă����OK�A�Ђ�J�^�����A���t�@�x�b�g�u-�v�݂̂�NG
			if(!bOther && bKanji)
				bRet = true;
		}
		break;
	case ObjectYomiType:
		// �܂��͎g�pOK�̕����������g���Ă��邩�`�F�b�N
		// �S�p�����J�i�i�����ȊO�j�{[�[�|]
		if(parse( strStr.c_str(), *(m_cSubConvert.getObjectYomi()) ).full)
		{
			// �P�ꖈ�ɕ�������
			std::list<std::pair<YomiType,std::string>> listStr = GetCharAddrList( strStr.c_str(), sjis_ranges().add(sjis_range(0x817C,0x817C)) );
			// �ŏ��ƍŌオ�u�|�v��������NG
			if(_tcscmp(listStr.begin()->second.c_str(), _T("�|")) == 0 || _tcscmp(listStr.rbegin()->second.c_str(), _T("�|")) == 0)
				break;

			bool bHiraKata = false, bOther = false;
			for(std::list<std::pair<YomiType,std::string>>::iterator it = listStr.begin(); it != listStr.end(); ++it)
			{
				switch(it->first)
				{
				case NoneType:
					if(_tcscmp(it->second.c_str(), _T("�|")) != 0)	// �u�|�v�łȂ��ꍇ��NG
						bOther = true;
					break;
				case NumberType:
					break;
				case HiraKataType:
					// �J�^�J�i�����̕��т��K���ȂƂ��̂݁A�J�^�J�i���g���Ă���Ɣ��肷��
					if( CheckFirstLargeKana( it->second.c_str() ) && CheckYomiCharacter( it->second.c_str() ) ){
						bHiraKata = true;
					}
					break;
				default:
					bOther = true;
					break;
				}
			}
			// �J�^�J�i���g���Ă����OK�A�����u�|�v�݂̂�NG
			if(!bOther && bHiraKata)
				bRet = true;
		}
		break;
	case KanjiNameType:
		// �܂��͎g�pOK�̕����������g���Ă��邩�`�F�b�N
		// sjis���A��񐅏������{�S�p�p���{�S�p���ȃJ�i�{[�[�X]
		if( parse( strStr.c_str(), *(m_cSubConvert.getKanjiName()) ).full )
		{
			// �P�ꖈ�ɕ�������
			std::list<std::pair<YomiType,std::string>> listStr = GetCharAddrList( strStr.c_str() );
			// �����ł���悤�Ȃ�NG
			if(listStr.size() == 1)
			{
				bool bKanji = false, bOther = false;
				for(std::list<std::pair<YomiType,std::string>>::iterator it = listStr.begin(); it != listStr.end(); ++it)
				{
					switch(it->first)
					{
					case AlphabetType:
						if(std::wstring(CA2W(it->second.c_str())).length() >= 3)	// 3�����ȏ�Ȃ犿���Ɠ�������
							bKanji = true;
						break;
					case KanjiYomiTableType:
						bKanji = true;
						break;
					default:
						bOther = true;
						break;
					}
				}
				// �J�^�J�i���g���Ă����OK�A�����݂̂�NG
				if(!bOther && bKanji)
					bRet = true;
			}
		}
		break;
	case KanjiYomiType:
		// �P�����ڂ��K���ȑ啶���J�^�J�i�ł���A�S�̂��K���ȃJ�i�ł��邩�`�F�b�N (Bug 9249)
		bRet = ( CheckFirstLargeKana( strStr.c_str() ) && CheckYomiCharacter( strStr.c_str() ) );
		break;
	}

	return bRet;
}

bool CCharAddrConvert::IsCheckKanjiYomi(LPCTSTR lpcszKanji, LPCTSTR lpcszYomi, long lOID, std::multimap<long,CString>& mapError)
{
	bool bRet = true;

	// GetCharAddrList()���Ɗ������ȃJ�i�A�p���A�����A�u-�v�ŕ������̂�
	// GetCharAddrList2()���w��B����Ŋ������ȃJ�i�p���A�����A�u-�v�ŕ������
	std::list<std::pair<YomiType,std::string>> listKanji = GetCharAddrList2(lpcszKanji);
	std::list<std::pair<YomiType,std::string>> listYomi = GetCharAddrList2(lpcszYomi, sjis_ranges().add(sjis_range(0x817C,0x817C)));

	// ���������������G���[
	if(listKanji.size() == listYomi.size())	// list��size()�͗]��g�������Ȃ����ǁA��ΐ������Ȃ�����C�ɂ��Ȃ�
	{
		std::list<std::pair<YomiType,std::string>>::const_iterator itYomi = listYomi.begin();
		for(std::list<std::pair<YomiType,std::string>>::const_iterator itKanji = listKanji.begin(); itKanji != listKanji.end(); ++itKanji)
		{
			// [����:����<->�ǂ�:�Ђ�J�^] [����:�A���t�@�x�b�g<->�ǂ�:�Ђ�J�^]
			// ��L�̑g�����̏ꍇ�̓X���[
			if((itKanji->first == KanjiYomiTableType && itYomi->first == HiraKataType) ||
				(itKanji->first == AlphabetType && itYomi->first == HiraKataType))
			{
			}
			// �Ђ�J�^���m�Ȃ犿�����J�^�J�^�ɕϊ����ē������`�F�b�N����
			else if(itKanji->first == HiraKataType && itYomi->first == HiraKataType)
			{
				std::string strKanjiKata = ConvertHiraToKata(itKanji->second);
				if(_tcscmp(strKanjiKata.c_str(), itYomi->second.c_str()) != 0)
				{
					mapError.insert( std::pair<long,CString>( lOID, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�����Ɠǂ݂��s��v\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
					bRet = false;
					break;
				}
			}
			// �������m�̑g�����̏ꍇ���`�F�b�N
			else if(itKanji->first == NumberType && itYomi->first == NumberType)
			{
				// ������������_���ł��傤�i����->�����͔��p�F�ǂ�->�����͑S�p�j
				// ���̏����̑O�Ń`�F�b�N���Ă���̂ł����ň��������邱�Ƃ͂Ȃ��Ǝv�����ǁA�A�A
				if( _tcscmp(itKanji->second.c_str(), itYomi->second.c_str()) != 0)
				{
					// ����->������S�p�ɕϊ�
					CString strKanji(ConvertHanNumToZenNum(itKanji->second).c_str());
					// �S�p�����ϊ��������̂Ŋ����Ɠǂ݂��r�A�������G���[
					if( _tcscmp(strKanji, itYomi->second.c_str()) != 0 )
					{
						mapError.insert( std::pair<long,CString>( lOID, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�����Ɠǂ݂��s��v\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
						bRet = false;
						break;
					}
				}
				else
				{
					mapError.insert( std::pair<long,CString>( lOID, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�����Ɠǂ݂��s��v\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
					bRet = false;
					break;
				}
			}
			// ���̑����m�̑g�����̏ꍇ���`�F�b�N
			else if(itKanji->first == NoneType && itYomi->first == NoneType)
			{
				// KANJI��NoneType�́u-�v�̂݁B�u--�v�̂悤�ɂȂ�ꍇ�̓G���[
				// YOMI��NoneType�́u�|�v�̂݁B�u�|�|�v�̂悤�ɂȂ�ꍇ�̓G���[
				bool bNoneType = true;
				if(_tcscmp(itKanji->second.c_str(), _T("-")) != 0)
				{
					mapError.insert( std::pair<long,CString>( lOID, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�����s��\t") + CString(lpcszKanji) ) );
					bNoneType = false;
				}
				if(_tcscmp(itYomi->second.c_str(), _T("�|")) != 0)
				{
					mapError.insert( std::pair<long,CString>( lOID, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�ǂݕs��\t") + CString(lpcszYomi) ) );
					bNoneType = false;
				}
				if(!bNoneType)
					break;
			}
			// ����ȊO�̑g�����̏ꍇ�̓G���[
			else
			{
				mapError.insert( std::pair<long,CString>( lOID, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�����Ɠǂ݂��s��v\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
				bRet = false;
				break;
			}
			itYomi++;
		}
	}
	else
	{
		mapError.insert( std::pair<long,CString>( lOID, _T("�I�u�W�F�N�gID�ǂ݃e�[�u��\t�����Ɠǂ݂��s��v\t") + CString(lpcszKanji) + _T("\t") + CString(lpcszYomi) ) );
		bRet = false;
	}

	return bRet;
}

std::string CCharAddrConvert::ConvertHiraToKata(std::string strHira)
{
	std::string strRet;
	int iSize = int(strHira.size())*2+1;
	TCHAR *lpzBuf = new TCHAR[iSize];
	ZeroMemory(lpzBuf, iSize);
	// �S�p���Ȃ�S�p�J�i�ɕϊ�
	LCMapString( GetUserDefaultLCID(), LCMAP_KATAKANA, CA2CT(strHira.c_str()), iSize, lpzBuf, iSize );
	CString strBuf;
	strBuf.Format( _T("%s"), lpzBuf );
	delete [] lpzBuf;
	// [bug6542]�C��
	strBuf.Replace(_T("��"),_T("�C"));
	strBuf.Replace(_T("��"),_T("�G"));
	strRet.append( CT2A(strBuf) );
	return strRet;
}

std::string CCharAddrConvert::ConvertHanNumToZenNum(std::string strHanNum)
{
	std::string strRet;
	int iSize = int(strHanNum.size())*2+1;
	TCHAR *lpzBuf = new TCHAR[iSize];
	ZeroMemory(lpzBuf, iSize);
	// ����������S�p�ɕϊ�
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, CA2CT(strHanNum.c_str()), iSize, lpzBuf, iSize );
	CString strBuf;
	strBuf.Format( _T("%s"), lpzBuf );
	delete [] lpzBuf;
	strRet.append( CT2A(strBuf) );
	return strRet;
}
