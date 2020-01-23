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
#include "ImportData.h"
#include "globalfunc.h"
#include "atlfile.h"
#include "WinLib/ADOBase.h"

#include <boost/regex.hpp>

CImportData::CImportData(void){};
CImportData::~CImportData(void){};

void CImportData::ReadExcelFile( LPCTSTR lpcszFilepath, LPCTSTR SeetName, IFields* ipFields )
{
	using namespace gf;

	CADOBase cDB( lpcszFilepath );
	CStringA strQuery;
	//strQuery.Format( "SELECT * FROM %s", CStringA(SeetName) ); 
	strQuery.Format( "SELECT * FROM [%s$]", CStringA(SeetName) ); 
	CADOCursor cur( cDB.search(strQuery) );
	const std::vector<CADOCursor::Field>& fields = cur.fields();

	// �G�C���A�X->SDE�̃C���f�b�N�X
	std::map< CString, long > mapFields;
	foreach( const CADOCursor::Field& field, fields)
	{
		long lIndex = -1;
		CComBSTR strName = ( field.name == L"�Ǐ�ڍ�" ) ? L"�ڍ׏��" : CComBSTR(field.name);
		ipFields->FindFieldByAliasName( strName, &lIndex );
		if( lIndex > -1 )
			mapFields[field.name] = lIndex;
	}

	// Excel�ɂ��Ȃ����⑫
	long lTDC_F=-1, lWorkF=-1;
	ipFields->FindFieldByAliasName( L"TDC�S���t���O", &lTDC_F );
	mapFields[_T("TDC�S���t���O")] = lTDC_F;
	//ipFields->FindFieldByAliasName( L"����H���t���O", &lWorkF );
	//mapFields[_T("����H���t���O")] = lWorkF;

	for( CADOCursor::iterator it = cur.begin(); it != cur.end(); ++it )
	{
		CString strID;
		std::map< long, CComVariant > mapVal;
		std::map<CString, WKSPoint> mapGeom;
		for( int i=0; i < fields.size(); ++i )
		{
			if( _T("�s������H��") == fields[i].name )
			{
				CString strVal = Var2Str((*it)[i]);
				if( strVal == _T("�f�[�^�����iTDC�j") )
					mapVal[ mapFields[_T( "TDC�S���t���O" )] ] = 1L;
				//else if( strVal == _T("�f�[�^����") )
				//	mapVal[ mapFields[_T( "����H���t���O" ) ]] = 1L;
				continue;
			}

			double dDec = -1;
			if( fields[i].name ==_T("�Ǘ��ԍ�") ){
				strID = (*it)[i];
			}
			else if( fields[i].name == _T("�s������n�__�ܓx") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("�s������n�_")].Y = dDec;
				}else{
					mapVal[ mapFields[fields[i].name] ] = 0.0;
					continue;
				}
			}else if( fields[i].name == _T("�s������n�__�o�x") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("�s������n�_")].X = dDec;
				}else{
					mapVal[ mapFields[fields[i].name] ] = 0.0;
					continue;
				}
			}else if( fields[i].name == _T("�o���n_�ܓx") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("�o���n")].Y = dDec;
				}
			}else if( fields[i].name == _T("�o���n_�o�x") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("�o���n")].X = dDec;
				}
			}else if( fields[i].name == _T("�ړI�n_�ܓx") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("�ړI�n")].Y = dDec;
				}
			}else if( fields[i].name == _T("�ړI�n_�o�x") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("�ړI�n")].X = dDec;
				}
			}

			// double���̂�double�ŁA���̑��̂��̂͂��̂܂܁i�e�L�X�g�j�œ˂�����
			mapVal[ mapFields[fields[i].name] ] = (dDec > -1) ? dDec : (*it)[i];
#ifdef _DEBUG
if(strID == _T("10-396-1-00005") && fields[i].name == _T("No1:�����̐���"))
{
	printf(CStringA((*it)[i]));
}
#endif
		}
		m_lstIDs.push_back(strID);
		m_mapRecords[strID] = mapVal;
		m_mapGeoms[strID] = mapGeom;
	}
}

double CImportData::DMS2DEC( CString& strDMS )
{
	if( strDMS.IsEmpty() )
		return -1;

	int pos = 0;  

	dms _dms;
	_dms.deg = _ttol( strDMS.Tokenize( _T("."), pos ) );
	_dms.min = _ttol( strDMS.Tokenize( _T("."), pos ) );
	_dms.sec = _ttol( strDMS.Tokenize( _T("."), pos ) );
	_dms.sec += _ttol( strDMS.Tokenize( _T("."), pos ) ) / 10.0;

	return m_cnv.DMStoDEC( _dms );
}
