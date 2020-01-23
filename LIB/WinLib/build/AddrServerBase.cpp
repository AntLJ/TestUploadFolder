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
#include "AddrServerBase.h"
#include <iostream>
#include <tuple>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include "crd_cnv.h"

#define foreach BOOST_FOREACH
void PrintComError(_com_error &e);
extern crd_cnv g_cnv;

/// �擪����A������ch���폜����
CString ltrim( const CString& str, TCHAR ch )
{
	long c = str.GetLength();
	long i = 0;
	for( ; i < c; ++i )
	{
		if( str.GetAt(i) != ch )
			break;
	}
	if( 0 < i )
		return str.Right( c - i );

	return str;
}

/// ������str�ɑ΂��āAfrom����to�ɕϊ�����i�ꕶ�����j
CString translate( const CString& str, const CString& from, const CString& to )
{
	_ASSERTE( from.GetLength() == to.GetLength() );

	CString ret( str );

	long c = from.GetLength();
	for( int i = 0; i < c; ++i )
		ret.Replace( from.GetAt(i), to.GetAt(i) );

	return ret;
}

bool is123( TCHAR ch ) {
	switch( ch )
	{
	case '1':
	case '2':
	case '3':
		return true;
		break;
	default: break;
	}
	return false;
}

CAddrServerBase::CAddrServerBase(void) : m_bUseProcedure(false), m_bIsOracle(false)
{
}

CAddrServerBase::~CAddrServerBase(void)
{
}

// �ڑ�����
bool CAddrServerBase::Connect( LPCTSTR lpcszConn )
{
	if( m_cConn.handle() )
		return true; // �ڑ��ς�

	_ConnectionPtr conn( __uuidof(Connection) );
	m_cConn = conn;

	// �ڑ�
	conn->ConnectionString = lpcszConn;
	try {
		conn->Open(_T(""),"","",adConnectUnspecified);
	}
	catch( _com_error& e )
	{
		conn = NULL;
		std::cerr << "ERROR: �Z���T�[�o�ւ̐ڑ����s : [" << (LPCSTR)CStringA(lpcszConn) << "]" << std::endl;
		PrintComError(e);

		return false;
	}
	// Oracle��PostgreSQL��
	if( ! _tcsstr( lpcszConn, _T("PostgreSQL") ) )
		m_bIsOracle = true;

	return true;

}

// �Z�������񂩂���W���擾����t�@���N�V����(SS-PJ�Ŏg�p)
CString CAddrServerBase::addrtocode( LPCTSTR lpcszAddr ) const
{
//	if( GetUseProcedure() )
//		return _addrtocode( lpcszAddr );

	// �Z�������� �� �Z���R�[�h��
	CString result( addr2code( lpcszAddr ) );

	// ��͏Z���R�[�h�擾
	CString strAddrCode( translate( result.Left( result.Find( _T(',') ) ), _T("x."), _T("00") ) );

	// ��͐��x�擾
	long ret_seido = _ttol( result.Mid( result.Find(_T(','))+1, 1 ) );

	std::vector<CString> vecAddrName; // �Z�����e�K�w�i�[�p
	int mesh = 0, x = 0, y = 0;
	long ap_seido = 0; // Address_Point���x
	if( 0 < ret_seido )
	{		
		if( ! ( GetAddrName( strAddrCode, vecAddrName ) && // �Z���R�[�h����Z�����̎擾
			GetAddrPos( strAddrCode, ret_seido, mesh, x, y, ap_seido ) ) ) // �Z���R�[�h������W�擾
			return CString();

		// proteus�ł͉�͐��x6�i�K�Ȃ̂ŁA4�ȏ��-1
		if( 3 < ret_seido )
			ret_seido -= 1;
	}

	// IPCODE, CHIBAN_STATUS�͕K�v�Ȃ��̂ŁANULL
	CString ret;
	ret.Format(_T("%s,%ld,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%ld,%ld,%ld,,%ld,"), 
		lpcszAddr, ret_seido, 
		strAddrCode.Mid( 0, 2 ), strAddrCode.Mid( 2, 3 ), strAddrCode.Mid( 5, 3 ), 
		strAddrCode.Mid( 8, 3 ), strAddrCode.Mid( 11, 5 ), strAddrCode.Mid( 16, 4 ),
		vecAddrName[0], vecAddrName[1], vecAddrName[2], vecAddrName[3], vecAddrName[4], vecAddrName[5],
		mesh, x, y, ap_seido );

	return ret;
}

CString CAddrServerBase::_addr2code( const CString& strAddr ) const
{
	CStringA strSQL;
	if( m_bIsOracle )
		strSQL.Format( "SELECT /*+ FIRST_ROWS */ CAST( %s.ADRS2CODE.CONV('%s') AS CHAR(512)) FROM DUAL", CStringA(m_strOwner), CStringA(strAddr) );
	else
		strSQL.Format( "SELECT CAST( ADDRTOCODE.CONV('%s') AS CHAR(512)) FROM DUAL", CStringA(strAddr) );

	CADOCursor cur( m_cConn.search( "%s", strSQL ) );
	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return _T("xxxxxxxxxxxxxxxxxxxx,0,") + strAddr;
	}

	return (*cur.begin())[0].bstrVal;
}

CString CAddrServerBase::addr2code( const CString& strAddr ) const
{
	if( GetUseProcedure() )
		return _addr2code( strAddr );

	// ���K��
	CString strAddr2( translate( strAddr, _T("0123456789ABCDEF"), _T("�O�P�Q�R�S�T�U�V�W�X�`�a�b�c�d�e") ) ); // �S�p��
	strAddr2.Replace(_T("�@"),_T("")); // �X�y�[�X�̏���
	strAddr2 = CommonExpr(strAddr2); // ���K��

	int nRetFlag = 0;

	// �s���{���R�[�h�̎擾
	CString strKenCode(_T("xx"));
	GetKenCode( strAddr2, strKenCode );
	if( strKenCode != _T("xx") )
		nRetFlag = 1;

	// �s�撬���R�[�h�̎擾
	CString strShiCode(_T("xxx"));
	if( 1 == nRetFlag && ( ! strAddr2.IsEmpty() ) )
	{
		GetShiCode( strAddr2, strKenCode, strShiCode );
		if( strShiCode != _T("xxx") )
			nRetFlag = 2;
	}

	// ���n�ԁH
	CString strOazaCode(_T("xxx")), strAzaCode(_T("xxx"));
	if( 2 == nRetFlag && 0 == strAddr2.FindOneOf( _T("�O�P�Q�R�S�T�U�V�W�X") ) )
	{
		// [Bug 8817] 20���Ή��Z���ɕs���Ȑ��������͂����
		// �ڐA�~�X�iinto�͎g���Ȃ��j
		CADOCursor cur( m_cConn.search( 
			"select Oaza_Code, Aza_Code from %s.AreaCode_Master "
				"where Ken_Code  = '%s' and "
				"      Shi_Code  = '%s' and "
				"     Oaza_Code != '000' and "
				"      Aza_Code  = '000' and "
				"    Oaza_Kanji is null and "
				"    Haishi_Date = '000000'", CStringA(m_strOwner), CStringA(strKenCode), CStringA(strShiCode) ) );

		if( S_OK != cur.err() )
		{
			ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
			return _T("xxxxxxxxxxxxxxxxxxxx,0,") + strAddr2;
		}

		foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		{
			strOazaCode = vals[0].bstrVal;
			strAzaCode  = vals[1].bstrVal;
			nRetFlag = 4;
			break;
		}
	}

	// �厚�E���R�[�h�̎擾
	if( 2 == nRetFlag && ( ! strAddr2.IsEmpty() ) )
	{
		GetOazaAzaCode( strAddr2, strKenCode, strShiCode, strOazaCode, strAzaCode );
		if( _T("xxx") != strOazaCode )
			nRetFlag = ( _T("xxx") == strAzaCode ) ? 3 : 4;
		else {
			// �厚�E���E�������擪�ɂ���΂���������čă`�F�b�N
			if( 0 == strAddr2.Find( _T("�厚") ) ) strAddr2.Delete( 0, 2 );
			if( 0 == strAddr2.Find( _T("����") ) ) strAddr2.Delete( 0, 2 );
			if( 0 == strAddr2.Find(   _T("��") ) ) strAddr2.Delete( 0, 1 );
			GetOazaAzaCode( strAddr2, strKenCode, strShiCode, strOazaCode, strAzaCode );
			if( _T("xxx") != strOazaCode )
				nRetFlag = ( _T("xxx") == strAzaCode ) ? 3 : 4;
		}
	}

	// �n�ԃR�[�h�̎擾
	CString strChibanCode(_T("xxxxx"));
	if( 3 <= nRetFlag && ( ! strAddr2.IsEmpty() ) )
	{
		GetChibanCode( strAddr2, nRetFlag, strKenCode, strShiCode, strOazaCode, strAzaCode, strChibanCode );
		if( _T("xxxxx") != strChibanCode )
			nRetFlag = 5;
		else if( _T("xxx") == strAzaCode )
			nRetFlag = 3;
	}

	// �ԍ��R�[�h�̎擾
	CString strBangoCode(_T("xxxx"));
	if( 5 == nRetFlag )
	{
		GetBangoCode( strAddr2, strKenCode, strShiCode, strOazaCode, strAzaCode, strChibanCode, strBangoCode );
		if( _T("xxxx") != strBangoCode )
			nRetFlag = 6;
		else
			strBangoCode = _T("0000"); // ��͕����񂪒n�Ԃ܂ł����Ȃ��Ƃ�
	}

	if( strAddr2.IsEmpty() )
	{
		if( 5 > nRetFlag )
		{
			strBangoCode  = _T("....");
			strChibanCode = _T(".....");
		}
		if( 4 > nRetFlag )
			strAzaCode = _T("...");
		if( 3 > nRetFlag )
			strOazaCode = _T("...");
		if( 2 > nRetFlag )
			strShiCode = _T("...");
	}

	CString strRet;
	strRet.Format(_T("%s%s%s%s%s%s,%d,%s"), strKenCode, strShiCode, strOazaCode, strAzaCode, strChibanCode, strBangoCode, nRetFlag, strAddr2 );

	return strRet;
}

// �Z���R�[�h�擾�֐��Q(�s�撬���R�[�h�w���)
CString CAddrServerBase::addr2code( const CString& strCityCode, const CString& strAddr ) const
{
	if( strCityCode.GetLength() != 5 )
		return _T("xxxxxxxxxxxxxxxxxxxx,0,") + strAddr;

	// �s�撬���R�[�h���疼�̎擾
	CString strCityKanji;
	CADOCursor cur( m_cConn.search(
		"select distinct replace(ken_kanji || shi_kanji, '�@', '') from %s.areacode_master "
		"where ken_code = substr('%s', 1, 2) "
		"and shi_code = substr('%s', 3, 3) "
		"and oaza_code = '000' "
		"and aza_code = '000'", CStringA(m_strOwner), CStringA(strCityCode), CStringA(strCityCode) ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return _T("xxxxxxxxxxxxxxxxxxxx,0,") + strAddr;
	}


	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		strCityKanji = vals[0].bstrVal;

	return addr2code( strCityKanji + strAddr );
}

// �Z���R�[�h����Z�����擾
bool CAddrServerBase::GetAddrName( LPCTSTR lpcszAddrCode, std::vector<CString>& listName ) const
{
	CString ken_name, shi_name, oaza_name, aza_name, tmp_name;
	CStringA strAddrCode( lpcszAddrCode );
	CStringA strPrefCode(   strAddrCode.Mid(  0, 2 ) );
	CStringA strCityCode(   strAddrCode.Mid(  2, 3 ) );
	CStringA strOazaCode(   strAddrCode.Mid(  5, 3 ) );
	CStringA strAzaCode(    strAddrCode.Mid(  8, 3 ) );
	CStringA strChibanCode( strAddrCode.Mid( 11, 5 ) );
	CStringA strBangoCode(  strAddrCode.Mid( 16, 4 ) );

	listName.assign( 6, CString() );

	// �s���{���`���E���ڎ擾
	{
		CADOCursor cur( m_cConn.search( 
			"select ken_kanji, shi_kanji, oaza_kanji, aza_kanji "
			"from %s.areacode_master "
			"where ken_code = '%s' and "
			"  shi_code = '%s' and "
			" oaza_code = '%s' and "
			"  aza_code = '%s'",
			CStringA(m_strOwner), strPrefCode, strCityCode, strOazaCode, strAzaCode ) );

		if( S_OK != cur.err() )
		{
			ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
			return false;
		}

		foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		{
			for( int i = 0; i < 4; ++i )
				listName[i] = ( VT_BSTR == vals[i].vt ) ? CString(vals[i].bstrVal) : _T("");
		}
	}

	// �n�Ԏ擾
	if( strChibanCode != "00000" )
	{
		// �����t���H
		if( 0 == strChibanCode.Left(1).FindOneOf("ABCDEF") )
		{
			CADOCursor cur( m_cConn.search( 
				"select chiban_name from %s.address_point "
				"where ken_code = '%s' and "
				"	   shi_code = '%s' and "
				"     oaza_code = '%s' and "
				"      aza_code = '%s' and "
				"   chiban_code = '%s' and "
				"    bango_code = '0000'",
				CStringA(m_strOwner), strPrefCode, strCityCode, strOazaCode, strAzaCode, strChibanCode ) );

			if( S_OK != cur.err() )
			{
				ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
				return false;
			}

			foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
				listName[4] = vals[0].bstrVal;
		}
		else {
			listName[4] = translate( ltrim( CString(strChibanCode), _T('0') ), _T("0123456789ABCDEF"), _T("�O�P�Q�R�S�T�U�V�W�X�`�a�b�c�d�e") );
		}
	}

	// �Z���ԍ��擾
	if( strBangoCode != "0000" )
	{
		// �����t���H
		if( 0 == strBangoCode.Left(1).FindOneOf("ABCDEF") )
		{
			CADOCursor cur( m_cConn.search( 
				"select bango_name from %s.address_point "
				"where ken_code = '%s' and "
				"	   shi_code = '%s' and "
				"     oaza_code = '%s' and "
				"      aza_code = '%s' and "
				"   chiban_code = '%s' and "
				"    bango_code = '%s'",
				CStringA(m_strOwner), strPrefCode, strCityCode, strOazaCode, strAzaCode, strChibanCode, strBangoCode ) );

			if( S_OK != cur.err() )
			{
				ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
				return false;
			}

			foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
			{
				listName[5] = vals[0].bstrVal;
				break;
			}
		}
		else {
			listName[5] = translate( ltrim( CString(strBangoCode), _T('0') ), _T("0123456789ABCDEF"), _T("�O�P�Q�R�S�T�U�V�W�X�`�a�b�c�d�e") );
		}
		listName[5] = _T("�|") + listName[5];
	}

	return true;
}

// �Z���R�[�h������W�擾
bool CAddrServerBase::GetAddrPos( LPCTSTR lpcszAddrCode, long seido, int& mesh, int& x, int& y, long& seido_ap ) const
{
	double lon = -1.0f, lat = -1.0f;
	seido_ap = -1;
	long tmp_seido = seido;

	CStringA strAddrCode(lpcszAddrCode);
	CStringA strPrefCode( strAddrCode.Mid( 0, 2 ) );
	CStringA strCityCode( strAddrCode.Mid( 2, 3 ) );
	CStringA strOazaCode;
	CStringA strAzaCode;
	CStringA strChibanCode;
	CStringA strBangoCode;

	// �Z���R�[�h�����ɑS���Z��������W�擾(�厚��\�_�̐��x��2�Ȃ̂�3�ɂ���)
	while( tmp_seido > 0 )
	{
		strOazaCode = (   tmp_seido > 2 ? strAddrCode.Mid(  5, 3 ) : "000" );
		strAzaCode  = (   tmp_seido > 3 ? strAddrCode.Mid(  8, 3 ) : "000" );
		strChibanCode = ( tmp_seido > 4 ? strAddrCode.Mid( 11, 5 ) : "00000" );
		strBangoCode = (  tmp_seido > 5 ? strAddrCode.Mid( 16, 4 ) : "0000" );
		CADOCursor cur( m_cConn.search(
			"select longitude,latitude, "
				"case "
					"when oaza_code != '000' and aza_code = '000' and chiban_code = '00000' then '3' "
					"else seido_status "
				"end "
			"from %s.address_point "
			"where  ken_code = '%s' "
			"and    shi_code = '%s' "
			"and   oaza_code = '%s' "
			"and    aza_code = '%s' "
			"and chiban_code = '%s' "
			"and  bango_code = '%s'",
			CStringA(m_strOwner), strPrefCode, strCityCode, strOazaCode, strAzaCode, strChibanCode, strBangoCode ) );

		if( S_OK != cur.err() )
		{
			ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
			return false;
		}
		
		foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		{
			CComVariant vaLon(vals[0]), vaLat(vals[1]), vaSeido(vals[2]);
			vaLon.ChangeType(VT_R8);
			vaLat.ChangeType(VT_R8);
			vaSeido.ChangeType(VT_I4);
			lon = vaLon.dblVal;
			lat = vaLat.dblVal;
			seido_ap = vaSeido.lVal;
		}
		if( 0 < lon )
			break; // ���W�l�擾�ł����烋�[�v������
		else
			--tmp_seido; // �Ȃ���ΐ��x�����čČ���
	}
	// 10�i�ܓx�o�x����A2�����b�V��XY��
	g_cnv.LLtoMesh( lon, lat, 2, &mesh, &x, &y, 1 ); // XY���W��1000000�ɂȂ邱�Ƃ����e���Ȃ�

	return true; // �������ʂ�0�ł�true�ŕԂ邱�Ƃɒ���
}

// �Z��������𐳋K������i�h�炬�̋z���j
CString CAddrServerBase::CommonExpr( CString& strAdrs )
{
	CString strNewAdrs( translate( strAdrs, _T("�]�\�[�m�V�P���c�b�ڟ��Ş��V�w�Mꠃ����ğ��U�G�藴��y�\���"), _T("�|�|�|�̂̂����ٕ������O�����G���U�k���b������x�]����") ) );
	strNewAdrs.Replace( _T("�ʂ�"),   _T("��") );
	strNewAdrs.Replace( _T("�ʃ�"),   _T("��") );
	strNewAdrs.Replace( _T("�オ��"), _T("��") );
	strNewAdrs.Replace( _T("���"),   _T("��") );
	strNewAdrs.Replace( _T("�ニ"),   _T("��") );
	strNewAdrs.Replace( _T("������"), _T("��") );
	strNewAdrs.Replace( _T("����"),   _T("��") );
	strNewAdrs.Replace( _T("����"),   _T("��") );
	strNewAdrs.Replace( _T("����"),   _T("��") );
	strNewAdrs.Replace( _T("����"),   _T("��") );

	return strNewAdrs;
}

// ���������񂪐擪���瑶�݂����ꍇ�ɂ��̕�������J�b�g����
CString CAddrServerBase::ReplaceHead( const CString& src, const CString& rep )
{
	if( 0 == src.Find( rep ) )
		return src.Right( src.GetLength() - rep.GetLength() );
	else
		return src;
}

// �u�厚�v�u���v�u�����v�̏���
CString CAddrServerBase::RemoveAzaHead( const CString& src )
{
	CString strNew( ReplaceHead( src, _T("�厚") ) );
	strNew = ReplaceHead( strNew, _T("����") );
	return ReplaceHead( strNew, _T("��") );
}

// ���������A���r�A�����ϊ��֐��i�u��v�`�u��\��v�� �u�P�v�`�u�X�X�v�j
CString CAddrServerBase::Kanji2Num( const CString& src )
{
	if( ! src )
		return CString();

	CString ret; // �߂�l

	long count = src.GetLength();
	TCHAR ch = _T('\0');
	int nTenFlag = 0;
	for( long i = 0; i < count; ++i )
	{
		ch = src.GetAt(i);
		switch( ch )
		{
		// �����������ꂽ�Ƃ�
		case _T('��'):
		case _T('��'):
		case _T('�O'):
		case _T('�l'):
		case _T('��'):
		case _T('�Z'):
		case _T('��'):
		case _T('��'):
		case _T('��'):
		case _T('�Z'):
			nTenFlag = 1;
			ret += translate( CString(ch), _T("���O�l�ܘZ������Z"), _T("�P�Q�R�S�T�U�V�W�X�O") );
			break;
		// �u�\�v�����ꂽ�Ƃ�
		case _T('�\'):
			switch( nTenFlag )
			{
				case 0:	ret += _T("�P"); break;    // ���O���������łȂ��Ƃ�
				case 2: ret += _T("�O�P"); break;  // ���O���u�\�v�̂Ƃ�
				default: break;
			}
			nTenFlag = 2;
			break;
		// �������łȂ��Ƃ�
		default:
			if( 2 == nTenFlag ) // ���O���u�\�v�̂Ƃ�
				ret += _T("�O");
			nTenFlag = 0;
			ret += ch;
			break;
		}
	}
	return ret;
}

// ���������A���r�A�����ϊ��֐��i�u��v�`�u��\��v�� �u�P�v�`�u�X�X�v�j
CString CAddrServerBase::Kanji2NumEx( const CString& src )
{
	static boost::wregex g_reg_exp(L"([���O�l�ܘZ������\]+)(��|�n��|��|��)");
	static std::list<std::tr1::tuple<CString,int,int>> g_container; // �u���Ώۈʒu�ƒ���
	typedef std::tr1::tuple<CString,int,int> TYPE_PAIR;

	// regex_grep�̃R�[���o�b�N�֐�
	// g_container�Ɍ������ʂ�~�ς���
	struct callback {
		// ��납��u����������̂ŁApush_front�œ˂�����
		bool operator()( const boost::match_results<std::wstring::iterator>& mr ) {
			g_container.push_front( std::tr1::make_tuple( CString((mr.str(1)+mr.str(2)).c_str()), (int)mr.position(1), (int)mr.length(1) + (int)mr.length(2) ) ); // ���l�����̈ʒu�ƒ������L���ibug 8588�j
			return true;
		}
	};

	// �����͂�������
	std::wstring wsrc( (LPCWSTR)CStringW(src) );
	g_container.clear();
	boost::regex_grep( callback(), wsrc.begin(), wsrc.end(), g_reg_exp );
	foreach( const TYPE_PAIR& _pair, g_container )
		wsrc.replace( std::tr1::get<1>(_pair), std::tr1::get<2>(_pair), std::wstring( (LPCWSTR)CStringW( Kanji2Num( std::tr1::get<0>(_pair) ) ) ) );

	return wsrc.c_str();
}

// �s���{���R�[�h�擾�֐�
bool CAddrServerBase::GetKenCode( CString& strAddr, CString& strRetKenCode, bool doCache/* = true*/ ) const
{
	strRetKenCode = _T("xx");

	// �L���b�V�����őO����v����
	static std::map<CString,CString> g_mapPrefNameCode; //!< key:���� val:���R�[�h
	typedef std::pair<CString,CString> TYPE_PAIR;
	foreach( const TYPE_PAIR& _pair, g_mapPrefNameCode ) // TODO: ���񃋁[�v�܂킷���́Amap��index���g�����ق����͂₻��
	{
		if( strAddr.Left(_pair.first.GetLength()) == _pair.first )
		{
			strAddr.Delete( 0, _pair.first.GetLength() );
			strRetKenCode = _pair.second;
			return true;
		}
	}

	if( ! doCache )
		return false;

	// �����ꍇ�̓L���b�V������
	CADOCursor cur( m_cConn.search(
		"select distinct ken_code, ken_kanji from %s.areacode_master "
		"where shi_code = '000' and "
		"     oaza_code = '000' and "
		"      aza_code = '000'" , CStringA(m_strOwner) ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		g_mapPrefNameCode[vals[1].bstrVal] = vals[0].bstrVal;

	return GetKenCode( strAddr, strRetKenCode, false ); // �������[�v�ɂȂ�Ȃ��悤�A�L���b�V���͂����Ȃ�
}

// �s�撬���R�[�h�擾�֐�
bool CAddrServerBase::GetShiCode( CString& strAddr, const CString& strKenCode, CString& strRetShiCode, bool doCache/* = true*/ ) const
{
	strRetShiCode = _T("xxx");

	static std::map<CString,std::map<CString,CString>> g_mapCityNameCode; //!< key:���R�[�h val{ key:�s�撬���� val:�s�撬���R�[�h }
	static std::map<CString,std::map<CString,bool>> g_mapCityCodeHaishi; //!< key:���R�[�h val{ key:�s�撬���R�[�h val:�p�~�t���O }
	std::map<CString,std::map<CString,CString>>::const_iterator it = g_mapCityNameCode.find(strKenCode);
	if( g_mapCityNameCode.end() != it )
	{
		// �L���b�V�����őO����v����
		typedef std::pair<CString,CString> TYPE_PAIR;
		const TYPE_PAIR* _pret = NULL;
		foreach( const TYPE_PAIR& _pair, it->second ) // TODO: ���񃋁[�v�܂킷���́Amap��index���g�����ق����͂₻��
		{
			if( strAddr.Left(_pair.first.GetLength()) == _pair.first )
			{
				// �p�~�R�[�h�ł��ꉞ�Ԃ����A�����Ă���ق���D�悷��̂ŕۗ�
				if( g_mapCityCodeHaishi[strKenCode][_pair.second] )
					_pret = &_pair;
				else {
					strAddr.Delete( 0, _pair.first.GetLength() );
					strRetShiCode = _pair.second;
					return true;
				}
			}
		}
		// �p�~�R�[�h�����Ȃ��ꍇ�͂����
		if( _pret )
		{
			strAddr.Delete( 0, _pret->first.GetLength() );
			strRetShiCode = _pret->second;
			return true;
		}

		// it������Ƃ������Ƃ́A��x�̓L���b�V�����쐬�����Ƃ������ƂȂ̂ŁA
		// �ēx�L���b�V������邱�Ƃ͂��Ȃ�
		doCache = false;
	}

	if( ! doCache )
		return false;

	// �����ꍇ�̓L���b�V������
	CADOCursor cur( m_cConn.search( 
		"select Shi_Code,Shi_Kanji, "
			"case Haishi_Date "
				"when '000000' then 0 "
				"else 1 "
			"end as Haishi_F "
		"from %s.AreaCode_Master "
		"where Ken_Code = '%s' "
		"and   Shi_Code != '000' "
		"and   Oaza_Code = '000' "
		"and    Aza_Code = '000' "
		"and Shi_Kanji not like '%s' "
		"and Shi_Kanji not like '%s' "
		"and not (Shi_Code like '1__' and Shi_Kanji like '%s')", CStringA(m_strOwner), CStringA(strKenCode), "%�S", "%�x��", "%�s" ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	std::map<CString,CString>& mapCityNameCode = g_mapCityNameCode[strKenCode];
	std::map<CString,bool>& mapCityCodeHaishi = g_mapCityCodeHaishi[strKenCode];
	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
	{
		CString strCityName(vals[1].bstrVal);
		strCityName.Replace(_T("�@"), _T("")); // �S�p�󔒂͏������Ă����ibug 8570�j
		mapCityNameCode[CommonExpr(strCityName)] = vals[0].bstrVal;
		CComVariant vaHaishiF( vals[2] );
		vaHaishiF.ChangeType( VT_I4 );
		mapCityCodeHaishi[vals[0].bstrVal] = ( 0 == vaHaishiF.lVal ) ? false : true;
	}

	return GetShiCode( strAddr, strKenCode, strRetShiCode, false ); // �������[�v�ɂȂ�Ȃ��悤�A�L���b�V���͂����Ȃ�
}

// �厚�E���R�[�h�擾�֐�
bool CAddrServerBase::GetOazaAzaCode( CString& strAddr, const CString& strKenCode, const CString& strShiCode, CString& strRetOazaCode, CString& strRetAzaCode ) const
{
	strAddr = Kanji2NumEx(strAddr); // ���������A���r�A�����ɕϊ�

	CADOCursor cur( m_cConn.search( 
		"select oaza_code, oaza_kanji, oaza_flag, "
			"aza_code, aza_kanji, aza_flag, haishi_date "
		"from %s.areacode_master "
		"where ken_code = '%s' and "
			"shi_code = '%s' and "
			"oaza_code != '000' and "
			"aza_code not between '851' and '899'", CStringA(m_strOwner), CStringA(strKenCode), CStringA(strShiCode) ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	int nMatchLen = 0; // �}�b�`���O����������(�������̂قǗD�悷��)
	CString strMatchOazaCode(_T("xxx")); // �}�b�`���O�����厚�R�[�h�ꎞ�ێ��p
	CString strMatchAzaCode(_T("xxx")); // �}�b�`���O�������R�[�h�ꎞ�ێ��p
	CString strMatchHDate(_T("000000")); // �}�b�`���O�������R�[�h�̔p�~��

	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
	{
		bool bMatch = false; // �e���R�[�h���Ń}�b�`���O���������w�������t���O

		// �厚�A���̐��K���y�ы󔒏������Ă�������
		CString strOazaKanji;
		if( VT_BSTR == vals[1].vt )
			strOazaKanji = Kanji2NumEx(CommonExpr(CString(vals[1].bstrVal))); // TODO: oaza_kanji��NULL�̂��̂͂͂����Ă��������ȁH�H���Ă��A�͂����Ȃ��Ɨ�����
		CString strAzaKanji;
		if( VT_BSTR == vals[4].vt )
			strAzaKanji = Kanji2NumEx(CommonExpr(CString(vals[4].bstrVal)));
		CString strOazaKanjiT(strOazaKanji); // �󔒃g��������
		CString strAzaKanjiT(strAzaKanji); // �󔒃g��������
		strOazaKanjiT.Trim();
		strAzaKanjiT.Trim();
		CString strTargetAdrs( strOazaKanjiT + strAzaKanjiT ); // �厚�{��

		// ���ɕ\�L�h��l�������Ɍ���
		CString strHaishiDate( vals[6].bstrVal );
		CString strOazaCode( vals[0].bstrVal );
		CString strAzaCode( vals[3].bstrVal );
		if( 0 == strAddr.Find( strTargetAdrs ) ) // �O����v����Ȃ�
		{
			if( nMatchLen < strTargetAdrs.GetLength() ||
				( strMatchHDate != _T("000000") && strHaishiDate == _T("000000") ) )
			{
				nMatchLen = strTargetAdrs.GetLength();
				strMatchOazaCode = strOazaCode;
				strMatchAzaCode = strAzaCode;
				strMatchHDate = strHaishiDate;
				bMatch = true;
			}
		}

		// �u���ځv�������Č���
		long lAzaCode = _ttol(strAzaCode);
		if( ( ! bMatch ) && 1 <= lAzaCode && lAzaCode <= 99 )
		{
			strTargetAdrs = strAzaKanjiT;
			// ���ڂ̏ꍇ�C�u���ځv���n�C�t���ɒu������
			strTargetAdrs.Replace( _T("���ړ�"), _T("��") );	// 01 104 085 0xx �Ȃ�
			strTargetAdrs.Replace( _T("���ږk"), _T("�k") );	// 01 104 086 0xx �Ȃ�
			strTargetAdrs.Replace( _T("����"), _T("") );
			strTargetAdrs.Replace( _T("��"), _T("") );			// 20 201 xxx 0xx �Ȃ�
			strTargetAdrs = strOazaKanjiT + strTargetAdrs;

			if( 0 == strAddr.Find( strTargetAdrs ) )
			{
				// �}�b�`���O���Ă����̎��̕����������Ȃ�m�f
				int len = strTargetAdrs.GetLength();
				if( strAddr.GetLength() == len ||
					0 != strAddr.Mid( len, 1 ).FindOneOf( _T("�P�Q�R�S�T�U�V�W�X�O") ) )
				{
					if( nMatchLen < strTargetAdrs.GetLength() ||
						( strMatchHDate != _T("000000") && strHaishiDate == _T("000000") ) )
					{
						nMatchLen = strTargetAdrs.GetLength();
						strMatchOazaCode = strOazaCode;
						strMatchAzaCode = strAzaCode;
						strMatchHDate = strHaishiDate;
						bMatch = true;
					}
				}
			}
		}

		// �u�厚�v�u���v�u�����v�t���O�ɂ��\�L�h��Ή�����
		TCHAR oazaFlag = CString(vals[2].bstrVal).GetAt(0);
		TCHAR azaFlag  = CString(vals[5].bstrVal).GetAt(0);
		bool oaza_is123 = is123(oazaFlag);
		bool  aza_is123 = is123(azaFlag);
		if( ( ! bMatch ) && ( oaza_is123 || aza_is123 ) )
		{
			strTargetAdrs = ( oaza_is123 ? RemoveAzaHead(strOazaKanji) : strOazaKanji ) + ( aza_is123 ? RemoveAzaHead(strAzaKanji) : strAzaKanji );
			strTargetAdrs.Replace(_T("�@"), _T(""));
			if( 0 == strAddr.Find( strTargetAdrs ) )
			{
				if( nMatchLen < strTargetAdrs.GetLength() ||
					( strMatchHDate != _T("000000") && strHaishiDate == _T("000000") ) )
				{
					nMatchLen = strTargetAdrs.GetLength();
					strMatchOazaCode = strOazaCode;
					strMatchAzaCode = strAzaCode;
					strMatchHDate = strHaishiDate;
					bMatch = true;
				};
			}
		}

		// �P���Ɂu�厚�v�u���v�u�����v������(�Ƃ肠�����������̂���)
		if( ( ! bMatch ) && ( 0 < strOazaKanji.Find(_T("�@�厚")) || 0 < strOazaKanji.Find(_T("�@��")) ) )
		{
			strTargetAdrs = strOazaKanji;
			strTargetAdrs.Replace(_T("�@�厚"), _T(""));
			strTargetAdrs.Replace(_T("�@��"), _T(""));
			strTargetAdrs.Replace(_T("�@"), _T(""));
			if( 0 == strAddr.Find( strTargetAdrs ) )
			{
				if( nMatchLen < strTargetAdrs.GetLength() ||
					( strMatchHDate != _T("000000") && strHaishiDate == _T("000000") ) )
				{
					nMatchLen = strTargetAdrs.GetLength();
					strMatchOazaCode = strOazaCode;
					strMatchAzaCode = strAzaCode;
					strMatchHDate = strHaishiDate;
					bMatch = true;
				};
			}
		}
	}
	strRetOazaCode = strMatchOazaCode;
	strRetAzaCode  = strMatchAzaCode;

	if( strRetOazaCode != _T("xxx") )
		strAddr.Delete( 0, nMatchLen );

	return true;
}

// �n�ԃR�[�h�擾�֐�
bool CAddrServerBase::GetChibanCode( CString& strAddr, int nRetF, const CString& strKenCode, const CString& strShiCode, const CString& strOazaCode, CString& strAzaCode, CString& strRetChibanCode ) const
{
	strRetChibanCode = _T("xxxxx");

	// strAddr�̈ꕶ���ڂ�like�������邪�A���ꂪ���̃P�[�X����
	// ���������F������s��e���s��쎚�{���W�W
	// �{���̃R�[�h�ɂ͂Ȃ��̂����A�����Ŏ�����菜���̂���Ԃ悳����
//	if( strAddr.Left(1) == _T("��") )
//		strAddr = strAddr.Right(strAddr.GetLength()-1);

	// �擪���n�C�t���Ȃ珜������
	if( strAddr.GetAt(0) == _T('�|') )
		strAddr.Delete( 0, 1 );
	if( strAddr.IsEmpty() )
		return false;

	CADOCursor cur( m_cConn.search(
		"select chiban_code, chiban_name from %s.address_point "
		"where ken_code = '%s' and "
			"  shi_code = '%s' and "
			" oaza_code = '%s' and "
			"  aza_code = '%s' and "
			"addr_type in ('1', '3') and "
			"chiban_name like '%s' "
			"order by length(chiban_name) desc", 
			CStringA(m_strOwner), CStringA(strKenCode), CStringA(strShiCode), 
			CStringA(strOazaCode), CStringA(strAzaCode), CStringA(strAddr.Left(1)) + "%" ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	// �厚�܂ł�����͂ł��Ă��Ȃ��ꍇ�́A�b��I�Ɏ��R�[�h[000]�ɂ���
	if( 3 == nRetF )
		strAzaCode = _T("000");

	// �����t���n�ԂɈ�v������̂��邩�m�F
	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
	{
		CString strChibanCode( vals[0].bstrVal );
		CString strChibanKanji( vals[1].bstrVal );
		// �ŏ��Ɋ��S��v�������̂��̗p
		if( 0 == strAddr.Find( strChibanKanji ) )
		{
			strRetChibanCode = strChibanCode;
			strAddr.Delete( 0, strChibanKanji.GetLength() );
			break;
		}
	}

	// �����t���n�Ԃƈ�v���Ȃ���΁A�ʏ�n�ԂƂ��ĉ��
	static CString g_strWideNmber(_T("�O�P�Q�R�S�T�U�V�W�X"));
	if( strRetChibanCode == _T("xxxxx") )
	{
		// �n�ԕ�����̎擾
		int finded = strAddr.FindOneOf(g_strWideNmber);
		if( -1 == finded )
			return true;
		// ��ԏ��߂Ɍ����u���l�Z�b�g�v���擾����
		// e.x. ���l�c�˂Q�O�U�|�P -> �Q�O�U
		CString strChibanStr;
		{
			int nPos = finded;
			int nLen = strAddr.GetLength();
			for(; nLen > nPos && 0 <= g_strWideNmber.Find(strAddr.GetAt(nPos)); ++nPos );
			strChibanStr = strAddr.Mid( finded, nPos - finded );
		}
		if( strChibanStr.GetLength() > 5 )
			return true;
		strRetChibanCode.Format(_T("%05ld"), _ttol(translate( strChibanStr, _T("�O�P�Q�R�S�T�U�V�W�X"), _T("0123456789"))) ); // TODO: �v���V�[�W���ł�A�`F���ΏۂƂȂ��Ă��邪�A�����ł͕s�v�Ȃ͂��Ȃ̂ŏ���
		strAddr.Delete( 0, finded + strChibanStr.GetLength() );
	}

	// ��͌�A�擪���Ԓn�E�ԁE�́E�m�E�n�C�t���Ȃ珜������
	if( 0 == strAddr.Find( _T("�Ԓn") ) )
		strAddr.Delete( 0, 2 );
	if( 0 == strAddr.FindOneOf( _T("�Ԃ̃m�|") ) )
		strAddr.Delete( 0, 1 );

	return true;
}

// �Z���ԍ��R�[�h�擾�֐�
bool CAddrServerBase::GetBangoCode( CString& strAddr, const CString& strKenCode, const CString& strShiCode, const CString& strOazaCode, const CString& strAzaCode, const CString& strChibanCode, CString& strRetBangoCode ) const
{
	CADOCursor cur( m_cConn.search( 
		"select bango_code, bango_name from %s.address_point "
		"where  ken_code = '%s' and "
			"   shi_code = '%s' and "
			"  oaza_code = '%s' and "
			"   aza_code = '%s' and "
			"chiban_code = '%s' and "
			"addr_type in ('2', '3') "
		"order by length(bango_name) desc",
			CStringA(m_strOwner), CStringA(strKenCode), CStringA(strShiCode), 
			CStringA(strOazaCode), CStringA(strAzaCode), CStringA(strChibanCode) ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	strRetBangoCode = _T("xxxx");

	// �����t�����Ɉ�v������̂��邩�m�F
	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
	{
		CString strBangoCode( vals[0].bstrVal );
		CString strBangoKanji( vals[1].bstrVal );
		// �ŏ��Ɋ��S��v�������̂��̗p
		// �����ɗ]�v�Ȉ�s����A�폜�ibug 8570�j
		if( 0 == strAddr.Find(strBangoKanji) )
		{
			strRetBangoCode = strBangoCode;
			strAddr.Delete( 0, strBangoKanji.GetLength() );
			break;
		}
	}

	// �����t�����ƈ�v���Ȃ���΁A�ʏ퍆�Ƃ��ĉ��
	if( strRetBangoCode == _T("xxxx") )
	{
		if( 0 != strAddr.FindOneOf( _T("�O�P�Q�R�S�T�U�V�W�X") ) )
			return true;

		CString strBangoStr( strAddr.SpanIncluding(_T("�O�P�Q�R�S�T�U�V�W�X")) );
		if( strBangoStr.GetLength() > 4 )
			return true;
		strRetBangoCode.Format(_T("%04ld"), _ttol(translate( strBangoStr, _T("�O�P�Q�R�S�T�U�V�W�X"), _T("0123456789"))) ); // TODO: �v���V�[�W���ł�A�`F���ΏۂƂȂ��Ă��邪�A�����ł͕s�v�Ȃ͂��Ȃ̂ŏ���
		strAddr.Delete( 0, strRetBangoCode.GetLength() );
	}

	return true;
}
