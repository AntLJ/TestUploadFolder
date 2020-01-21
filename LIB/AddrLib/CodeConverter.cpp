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
#include "CodeConverter.h"

namespace addr
{
	// �ϊ��e�[�u���X�L�[�}
	namespace cv_schema
	{
		namespace cv_common
		{
			const TCHAR kAddrCode[]		= _T("AddrCode");
			const TCHAR kRegistDate[]	= _T("RegistDate");
		}

		namespace pnt_conv
		{
			const TCHAR kTableName[]	= _T("Pnt_CodeConv");
			const TCHAR kPntCode[]	= _T("PntCode");
			const TCHAR kPntName[]	= _T("PntName");
			using namespace cv_schema::cv_common;		
		}

		namespace gou_conv
		{
			const TCHAR kTableName[]	= _T("Gou_CodeConv");
			const TCHAR kGouCode[]	= _T("GouCode");
			const TCHAR kGouName[]	= _T("GouName");
			using namespace cv_schema::cv_common;
		}
	}

using namespace cv_schema;


bool CCodeConverter::Init( const _TCHAR* lpcszCodeConvDB )
{
	 // ADO�g�p
	try
	{
		m_cAdoHelper.disconnect();

		_ConnectionPtr ipConn( _uuidof(Connection) );

		CString strCon;
		strCon.Format( _T("Driver={Microsoft Access Driver (*.mdb)}; DBQ=%s;"), lpcszCodeConvDB );
		if( SUCCEEDED(ipConn->Open(_bstr_t(strCon), _T(""), _T(""), adConnectUnspecified)) )
		{
			m_cAdoHelper.attach( ipConn );
			return true;
		}
		else
			return false;
	}
	catch( _com_error& e )
	{
		m_strError.Format( _T("�ϊ��e�[�u���I�[�v������COM�G���[����, %s"), COLE2CT(e.Description()) );
		return false;
	}
}

// �n�ԃR�[�h�擾
CString CCodeConverter::GetPntCode( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszPntName, bool bSave /* = false */ )
{
	CString strSQL;
	strSQL.Format( _T("select count(*) from %s where %s='%s' and %s='%s'"), pnt_conv::kTableName, pnt_conv::kAddrCode, lpcszAddrCode, pnt_conv::kPntName, lpcszPntName );
	CADOCursor cCur( m_cAdoHelper.search(CT2CA(strSQL)) );
 	if( cCur.err() == S_OK )
	{
		long lCount = (*cCur.begin())[0].lVal;
		CString strPntCode;
		switch( lCount )
		{
		case 0:	// �����Ȃ�(�V�K�R�[�h���s)
			{
				// �Y��11���R�[�h�̍Ō�Ɋ��蓖�Ă�ꂽ�n�ԃR�[�h���擾
				strSQL.Format( _T("select max(%s) from %s where %s='%s'"), pnt_conv::kPntCode, pnt_conv::kTableName, pnt_conv::kAddrCode, lpcszAddrCode );
				CADOCursor cCur2( m_cAdoHelper.search(CT2CA(strSQL)) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					if( VT_BSTR == vaValue.vt )
					{
						// �擾�����R�[�h+1��V�R�[�h�Ƃ��Ċ��蓖��
						CString strMaxPntCode = CString(vaValue.bstrVal);						
						TCHAR szAlpha = strMaxPntCode.GetAt(0);
						long lSeq = _tstol(strMaxPntCode.Mid(1)) + 1;
						if( lSeq > 9999 )
						{
							++szAlpha;
							lSeq = 0;
							if( 'G' == szAlpha )
							{
								m_strError.Format( _T("����ȏ�n�ԃR�[�h�̊��蓖�Ă��ł��܂���, %s"), lpcszAddrCode );
								return _T("");
							}
						}
						strPntCode.Format( _T("%c%04d"), szAlpha, lSeq );
					}
					else
						strPntCode = _T("A0000");
					
					return (!bSave || InsertCode('p', lpcszAddrCode, lpcszPntName, strPntCode))? strPntCode : _T("");	// �ۑ�����Ȃ�e�[�u���ɒǉ�
				}
				else
				{
					m_strError.Format( _T("�n�ԃR�[�h�̎擾�Ɏ��s, %s"), cCur2.GetErrorMessage() );
					return _T("");
				}
			}
			break;
		case 1:	// �����R�[�h
			{
				strSQL.Format( _T("select %s from %s where %s='%s' and %s='%s'"), 
							   pnt_conv::kPntCode, pnt_conv::kTableName, pnt_conv::kAddrCode, lpcszAddrCode, pnt_conv::kPntName, lpcszPntName );
				CADOCursor cCur2( m_cAdoHelper.search(CT2CA(strSQL)) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					strPntCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
				}
				else
				{
					m_strError.Format( _T("�n�ԃR�[�h�̎擾�Ɏ��s, %s"), cCur2.GetErrorMessage() );
					return _T("");
				}
			}
			break;
		default:
			m_strError.Format( _T("����L�[�ɂQ�̃R�[�h�����蓖�Ă��Ă���, %s, %s"), lpcszAddrCode, lpcszPntName );
			return _T("");
		}
		return strPntCode;
	}
	else
	{
		m_strError.Format( _T("�n�ԃR�[�h�̎擾�Ɏ��s, %s"), cCur.GetErrorMessage() );
		return _T("");
	}		
}

// �Z���ԍ��R�[�h�擾
CString CCodeConverter::GetGouCode( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszGouName, bool bSave /* = false */ )
{
	CString strSQL;
	strSQL.Format( _T("select count(*) from %s where %s='%s' and %s='%s'"), gou_conv::kTableName, gou_conv::kAddrCode, lpcszAddrCode, gou_conv::kGouName, lpcszGouName );
	CADOCursor cCur( m_cAdoHelper.search(CT2CA(strSQL)) );
 	if( cCur.err() == S_OK )
	{
		long lCount = (*cCur.begin())[0].lVal;
		CString strGouCode;
		switch( lCount )
		{
		case 0:	// �����Ȃ�(�V�K�R�[�h���s)
			{
				// �Y��11���R�[�h�̍Ō�Ɋ��蓖�Ă�ꂽ�n�ԃR�[�h���擾
				strSQL.Format( _T("select max(%s) from %s where %s='%s'"), gou_conv::kGouCode, gou_conv::kTableName, gou_conv::kAddrCode, lpcszAddrCode );
				CADOCursor cCur2( m_cAdoHelper.search(CT2CA(strSQL)) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					if( VT_BSTR == vaValue.vt )
					{
						// �擾�����R�[�h+1��V�R�[�h�Ƃ��Ċ��蓖��
						CString strMaxGouCode = CString( (*cCur2.begin())[0].bstrVal );
						TCHAR szAlpha = strMaxGouCode.GetAt(0);
						long lSeq = _tstol(strMaxGouCode.Mid(1)) + 1;
						if( lSeq > 999 )
						{
							++szAlpha;
							lSeq = 0;
							if( 'G' == szAlpha )
							{
								m_strError.Format( _T("����ȏ�Z���ԍ��R�[�h�̊��蓖�Ă��ł��܂���, %s"), lpcszAddrCode );
								return _T("");
							}
						}
						strGouCode.Format( _T("%c%03d"), szAlpha, lSeq );
					}
					else
						strGouCode = _T("A000");

					return (!bSave || InsertCode('g', lpcszAddrCode, lpcszGouName, strGouCode))? strGouCode : _T("");	// �ۑ�����Ȃ�e�[�u���ɒǉ�
				}
				else
				{
					m_strError.Format( _T("�Z���ԍ��R�[�h�̎擾�Ɏ��s, %s"), cCur2.GetErrorMessage() );
					return _T("");
				}
			}
			break;
		case 1:	// �����R�[�h
			{
				strSQL.Format( _T("select %s from %s where %s='%s' and %s='%s'"), 
							   gou_conv::kGouCode, gou_conv::kTableName, gou_conv::kAddrCode, lpcszAddrCode, gou_conv::kGouName, lpcszGouName );
				CADOCursor cCur2( m_cAdoHelper.search( CT2CA(strSQL) ) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					strGouCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
				}
				else
				{
					m_strError.Format( _T("�Z���ԍ��R�[�h�̎擾�Ɏ��s, %s"), cCur2.GetErrorMessage() );
					return _T("");
				}
			}
			break;
		default:
			m_strError.Format( _T("����L�[�ɂQ�̃R�[�h�����蓖�Ă��Ă���, %s, %s"), lpcszAddrCode, lpcszGouName );
			return _T("");
		}
		return strGouCode;
	}
	else
	{
		m_strError.Format( _T("�Z���ԍ��R�[�h�̎擾�Ɏ��s, %s"), cCur.GetErrorMessage() );
		return _T("");
	}
}

// �n�Ԗ��̎擾
CString CCodeConverter::GetPntName( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszPntCode ) const
{
	CString strSQL;
	strSQL.Format( _T("select %s from %s where %s='%s' and %s='%s'"), 
				   pnt_conv::kPntName, pnt_conv::kTableName, pnt_conv::kAddrCode, lpcszAddrCode, pnt_conv::kPntCode, lpcszPntCode );
	CADOCursor cCur( m_cAdoHelper.search(CT2CA(strSQL)) );
	if( cCur.err() == S_OK && cCur.begin() != cCur.end() )
	{
		CComVariant vaValue = (*cCur.begin())[0];
		return SUCCEEDED(vaValue.ChangeType(VT_BSTR))? CString(vaValue.bstrVal) : _T("");	// �o�C�i���^�Ŋi�[���Ă���̂ŁAVT_BSTR�ɕϊ�
	}
	else
	{
		m_strError.Format( _T("�n�Ԗ��̂̎擾�Ɏ��s, %s"), cCur.GetErrorMessage() );
		return _T("");
	}
}

// �Z���ԍ����̎擾
CString CCodeConverter::GetGouName( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszGouCode ) const
{
	CString strSQL;
	strSQL.Format( _T("select %s from %s where %s='%s' and %s='%s'"), 
				   gou_conv::kGouName, gou_conv::kTableName, gou_conv::kAddrCode, lpcszAddrCode, gou_conv::kGouCode, lpcszGouCode );
	CADOCursor cCur( m_cAdoHelper.search(CT2CA(strSQL)) );
	if( cCur.err() == S_OK && cCur.begin() != cCur.end()  )
	{
		CComVariant vaValue = (*cCur.begin())[0];
		return SUCCEEDED(vaValue.ChangeType(VT_BSTR))? CString(vaValue.bstrVal) : _T("");	// �o�C�i���^�Ŋi�[���Ă���̂ŁAVT_BSTR�ɕϊ�
	}
	else
	{
		m_strError.Format( _T("�Z���ԍ����̂̎擾�Ɏ��s, %s"), cCur.GetErrorMessage() );
		return _T("");
	}
}

//////////////////////////////////////////
//	�����֐�
//////////////////////////////////////////

// �V�K�R�[�h�o�^
bool CCodeConverter::InsertCode( TCHAR szClass, const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszName, const _TCHAR* lpcszNewCode )
{
	// �o�^���擾
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strRegistDate;
	strRegistDate.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

	CString strSQL;
	strSQL.Format( _T("insert into %s values( '%s', '%s', '%s', '%s' )"),
				   ('p' == szClass)? pnt_conv::kTableName : gou_conv::kTableName, lpcszAddrCode, lpcszName, lpcszNewCode, strRegistDate );
	CADOCursor cCur = m_cAdoHelper.insert( CT2CA(strSQL) );
	if( cCur.err() == S_OK )
		return true;
	else
	{
		m_strError.Format( _T("�e�[�u���ɐV�R�[�h��ǉ����s, %s, %s, %s, %s"), cCur.GetErrorMessage(), lpcszAddrCode, lpcszName, lpcszNewCode );
		return false;
	}
}

}	// namespace addr
