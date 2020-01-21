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
#include "YomiConverter.h"
#include <WinLib/charutil.h>
#include <WinLib/Str2.h>

namespace addr
{
	// �ϊ��e�[�u���X�L�[�}
	namespace yomi_schema
	{
		namespace common
		{
			const TCHAR kKanji[]	= _T("Kanji");
			const TCHAR kYomi[]	= _T("Yomi");
		}

		namespace kanji2yomi
		{
			const TCHAR kTableName[]	= _T("KanjiToYomi");
			using namespace yomi_schema::common;		
		}

		namespace object2yomi
		{
			const TCHAR kTableName[]	= _T("ObjectToYomi");
			const TCHAR kLayer[]		= _T("Layer");
			const TCHAR kObjectID[]	= _T("ObjectID");
			using namespace yomi_schema::common;
		}
	}

using namespace yomi_schema;

// ������
bool CYomiConverter::Init( const _TCHAR* lpcszCovnertTablePath )
{
	try
	{
		CADOCursor cKanjiYomiTable, cObjectYomiTable;

		CString strConvertDB;
		strConvertDB.Format( _T("Provider=Microsoft.JET.OLEDB.4.0;Data source = %s"), lpcszCovnertTablePath );
		_ConnectionPtr ipConnection(__uuidof(Connection));
		if( SUCCEEDED(ipConnection->Open(_bstr_t(strConvertDB), "", "", adConnectUnspecified)) )
		{
			// 2�̕ϊ��e�[�u�����擾����
			CADOBase ipConvertDB(ipConnection);
			CString strSQL;
			strSQL.Format( _T("select * from %s"), kanji2yomi::kTableName );
			cKanjiYomiTable = ipConvertDB.search( CT2CA(strSQL) );
			strSQL.Format( _T("select * from %s"), object2yomi::kTableName );
			cObjectYomiTable = ipConvertDB.search( CT2CA(strSQL) );

			m_cSubConvert.Init();
			SetKanjiYomiTableItems( cKanjiYomiTable );		// �����ǂݕϊ��e�[�u���̃��R�[�h���X�g���擾
			SetObjectYomiTableItems( cObjectYomiTable );	// �I�u�W�F�N�gID�ǂݕϊ��e�[�u���̃��R�[�h���X�g���擾
			return true;
		}
		else
			return false;
	}
	catch( _com_error& e )
	{
		m_strError.Format( _T("���~�e�[�u���I�[�v������COM�G���[����, %s"), COLE2T(e.Description()) );
		return false;
	}
}

// �I�u�W�F�N�g�ǂݕϊ��e�[�u���̃��R�[�h�擾
void CYomiConverter::SetObjectYomiTableItems( CADOCursor& cObjectYomiCursor )
{
	// �I�u�W�F�N�gID�ǂ݃e�[�u���̃��R�[�h���X�g���擾����
	long lCount = 1;
	const std::vector<CADOCursor::Field>& vecField = cObjectYomiCursor.fields();
	for( CADOCursor::iterator it = cObjectYomiCursor.begin(); it != cObjectYomiCursor.end(); ++it, ++lCount )
	{
		std::vector<CComVariant> vecValue = it.dereference();
		std::vector<CComVariant>::iterator itVal = vecValue.begin();

		// �P���R�[�h�S�Ď擾
		GouOrCityAddrLayer emLayer = None_Layer;
		long lOID = 0;
		CString strKanji, strYomi;
		for( std::vector<CADOCursor::Field>::const_iterator itField = vecField.begin(); itField != vecField.end(); ++itField )
		{
			if( itField->name.CompareNoCase(object2yomi::kLayer) == 0  )
				emLayer = (VT_I2 == itVal->vt || VT_I4 == itVal->vt && (itVal->lVal == 1 || itVal->lVal == 2) )?
						  static_cast<GouOrCityAddrLayer>(itVal->lVal) : None_Layer;
			else if( itField->name.CompareNoCase(object2yomi::kObjectID) == 0 )
				lOID = (VT_I2 == itVal->vt || VT_I4 == itVal->vt)? itVal->lVal : -1;
			else if( itField->name.CompareNoCase(object2yomi::kKanji) == 0 )
				strKanji = (VT_BSTR == itVal->vt)? CString(itVal->bstrVal) : _T("");
			else if( itField->name.CompareNoCase(object2yomi::kYomi) == 0 )
				strYomi = (VT_BSTR == itVal->vt)? CString(itVal->bstrVal) : _T("");

			++itVal;
		}

		if( emLayer != None_Layer && lOID > 0 && !strKanji.IsEmpty() && !strYomi.IsEmpty() )
		{
			// �`�F�b�N�͈�؍s��Ȃ�(CharAddrCheck�Ŏ��{���Ă���̂�)
			m_mapObjectYomiTable.insert( Object2YomiRec( ObjectLayerPair( (CityAdmin_Layer == emLayer)? CityAdmin_Layer : GouPoint_Layer, lOID ), 
														 KanjiYomiPair( strKanji, strYomi ) ) );
		}
	}
}

// �����ǂݕϊ��e�[�u���̃��R�[�h�擾
void CYomiConverter::SetKanjiYomiTableItems( CADOCursor& cKanjiYomiCursor )
{
	// �����ǂ݃e�[�u���̃��R�[�h���X�g���擾����
	long lCount = 1;
	const std::vector<CADOCursor::Field>& vecField = cKanjiYomiCursor.fields();
	for( CADOCursor::iterator it = cKanjiYomiCursor.begin(); it != cKanjiYomiCursor.end(); ++it, ++lCount )
	{
		std::vector<CComVariant> vecValue = it.dereference();
		std::vector<CComVariant>::iterator itVal = vecValue.begin();
		CString strKanji, strYomi;
		for( std::vector<CADOCursor::Field>::const_iterator itField = vecField.begin(); itField != vecField.end(); ++itField )
		{
			if( itField->name.CompareNoCase(kanji2yomi::kKanji) == 0 )
				strKanji = SUCCEEDED(itVal->ChangeType(VT_BSTR))? CString(itVal->bstrVal) : _T("");	// KANJI�t�B�[���h�́A�o�C�i���^�Ŋi�[���Ă���̂�VT_BSTR�ɕϊ���
			else if( itField->name.CompareNoCase(kanji2yomi::kYomi) == 0 )
				strYomi = (VT_BSTR == itVal->vt)? CString(itVal->bstrVal) : _T("");
			itVal++;
		}

		if( !strKanji.IsEmpty() && !strYomi.IsEmpty() )
			m_mapKanjiYomiTable.insert( std::pair<CString, CString>( strKanji, strYomi ) );
	}
}

// �����t���Z�����I�u�W�F�N�gID�ǂݕϊ��e�[�u���ŕϊ����ďo��
CString CYomiConverter::GetYomiFromCharAddr_Object( GouOrCityAddrLayer LayerType, long lOID, const _TCHAR* lpcszCharAddr, bool& bRet ) const
{
	// �w��I�u�W�F�N�g���I�u�W�F�N�gID�ǂ݃e�[�u���ɂ��邩�`�F�b�N
	std::map<ObjectLayerPair,KanjiYomiPair>::const_iterator it = m_mapObjectYomiTable.find( ObjectLayerPair(LayerType, lOID) );
	if( it != m_mapObjectYomiTable.end() )
	{
		// �I�u�W�F�N�gID�ǂ݃e�[�u���ɂ����Ă����R�[�h���̖��̂������t���Z�����̂ƈႤ�ꍇ������̂ł����Ń`�F�b�N
		// ��j�e�[�u���ɂ�[OID:123456, Layer:gou_point, Name:����]�Ɠ����Ă��邪�A���ۂɃ`�F�b�N������OID:123456�̍��|�C���g����́u�����P�Q�R�v�Ƃ������̂ł���ꍇ�B
		//      �{���Ȃ��[OID:123456, Layer:gou_point, Name:�����P�Q�R]�ƂȂ��Ă��Ȃ���΂Ȃ�Ȃ��B
		//      �����炭��Ǝ҂��ԈႦ�ē��͂��Ă��܂����̂������B
		bRet = it->second.first == lpcszCharAddr;
		return bRet? it->second.second : _T("");
	}
	else
	{
		bRet = false;
		return _T("");
	}
}

// �����t���Z���������ǂݕϊ��e�[�u���ŕϊ����ďo��
CString CYomiConverter::GetYomiFromCharAddr_Kanji( const _TCHAR* lpcszCharAddr, bool& bRet )
{
	// �����t���Z���̕������P�ꖈ�ɕ������ă��X�g���������̂������ǂ݂ɕϊ�
	std::list<std::pair<YomiType, CString>> listCharAddr;
	SetCharAddrList( lpcszCharAddr, listCharAddr );
	return GetYomi(listCharAddr, bRet);
}

// �����t���Z���̕������P�ꖈ�ɕ����������X�g���擾
void CYomiConverter::SetCharAddrList( const _TCHAR* lpcszCharAddr, std::list<std::pair<YomiType, CString>>& rList )
{
	// ��������������ȃJ�i�A�p���A�����ɕ������鏈��
	// �֗��ȃ��C�u�����iLIB/jis�j���������̂Ŏg�p�B�B�B
	std::vector<std::string> vecCharAddr;
	parse_info<> aInfo = 
		parse( (const char*)(CT2A(lpcszCharAddr)), *( 
			(+m_cSubConvert.getKanjiKana())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getNumber())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getAlphabet())[push_back_a(vecCharAddr)] |
			(+m_cSubConvert.getHyphen())[push_back_a(vecCharAddr)] |
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
			rList.push_back( std::pair<YomiType, CString>( aInfoKana.full? HiraKataType : KanjiYomiTableType, CString(it->c_str()) ) );	// ���ȃJ�i�݂̂�����ȊO��
		}
		// �����������`�F�b�N
		else if( aInfoNumber.full )
			rList.push_back( std::pair<YomiType, CString>( NumberType, CString(it->c_str()) ) );
		// �A���t�@�x�b�g�������`�F�b�N
		else if( aInfoAlphabet.full )
		{
			CString str(it->c_str());
			rList.push_back( std::pair<YomiType, CString>( CString_GetLength2(str) > 2? KanjiYomiTableType : AlphabetType, str ) );	// �A���t�@�x�b�g��3�����ȏ�͊����ǂݕϊ��̑ΏۂƂȂ�
		}
		// ��L�ȊO
		else
			rList.push_back( std::pair<YomiType, CString>( NoneType, CString(it->c_str()) ) );
	}
}

CString CYomiConverter::GetYomiFromKanjiYomiTable( const CString& strCharAddr, bool& bRet ) const
{
	// �����ǂ݃e�[�u������ǂ݂��擾
	std::map<CString, CString>::const_iterator it = m_mapKanjiYomiTable.find( strCharAddr );
	if( it == m_mapKanjiYomiTable.end() )
	{
		bRet = false;
		return strCharAddr;
	}
	else
		return it->second;
}

// �T�u�e�[�u������ǂ݂��擾
CString CYomiConverter::GetYomiFromSubTable( const std::wstring& strCharAddr, bool& bRet ) const
{
	// �T�u�e�[�u������ǂ݂��擾
	std::map<std::wstring, std::wstring>::const_iterator it = m_cSubConvert.GetSubtable().find( strCharAddr );
	if( it == m_cSubConvert.GetSubtable().end() )
	{
		bRet = false;
		return CString(strCharAddr.c_str());
	}
	else
		return CString(it->second.c_str());
}

// �����t���Z���������ǂݕϊ��e�[�u���ŕϊ����ďo��
CString CYomiConverter::GetYomi( const std::list<std::pair<YomiType, CString>>& listCharAddr, bool& bRet ) const
{
	// ���X�g�ɂ͂���Ȋ����œ����Ă܂�
	// ��)���f�[�^�@�@�@�@�@�FAB������-12C����EFG34
	// �@ ���f�[�^�i���X�g�j�FAB(�A���t�@�x�b�g),������(���ȃJ�i),-(�Ȃ�),12(����),C(�A���t�@�x�b�g),����(�����ǂ�),EFG(�����ǂ�),34(����)
	// �@ �o�̓f�[�^�@�@�@�@�F�G�[�r�[�A�C�E�|�P�Q�V�[�^�J�n�V�C�[�G�t�W�[�R�S
	CString strRet;
	bool bRetYomi = true;
	for( std::list<std::pair<YomiType, CString>>::const_iterator it = listCharAddr.begin(); it != listCharAddr.end(); ++it )
	{
		switch( it->first )
		{
		// ����
		case NumberType:
			{
				// �S������
#ifdef _UNICODE
				long lSize = it->second.GetLength()*2+1;
#else	// MBCS
				long lSize = it->second.GetLength()+1;
#endif
				TCHAR *lpzBuf = new TCHAR[lSize];
				ZeroMemory(lpzBuf, lSize);
				// ����������S�p�ɕϊ�
				LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, it->second, lSize, lpzBuf, lSize );
				strRet.Format( _T("%s"), lpzBuf );
				delete[] lpzBuf;
			}
			break;
		// �A���t�@�x�b�g
		case AlphabetType:
			{
				// �A���t�@�x�b�g2�����܂ł̓T�u�e�[�u���œǂ݂�t�^
				int nLen = CString_GetLength2( it->second );
				for( int i = 0; i < nLen; ++i )
					strRet+= GetYomiFromSubTable( std::wstring(CT2CW(CString_GetAt2(it->second, i))), bRetYomi );
				break;
			}
		// ���ȃJ�i
		case HiraKataType:
			{
				// �S������
#ifdef _UNICODE
				long lSize = it->second.GetLength()*2+1;
#else	// MBCS
				long lSize = it->second.GetLength()+1;
#endif
				TCHAR *lpzBuf = new TCHAR[lSize];
				ZeroMemory(lpzBuf, lSize);
				// �S�p���Ȃ�S�p�J�i�ɕϊ�
				LCMapString( GetUserDefaultLCID(), LCMAP_KATAKANA, it->second, lSize, lpzBuf, lSize );
				strRet.Format( _T("%s"), lpzBuf );
				delete[] lpzBuf;
				// [bug6542]�C��
				strRet.Replace(_T("��"),_T("�C"));
				strRet.Replace(_T("��"),_T("�G"));
			}
			break;
		// �����ǂ݃e�[�u��
		case KanjiYomiTableType:
			strRet += GetYomiFromKanjiYomiTable( it->second, bRetYomi );
			break;
		// �Y���Ȃ�
		case NoneType:
			if( it->second == _T("-") )
				strRet += _T("�|");
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

}	// namespace addr
