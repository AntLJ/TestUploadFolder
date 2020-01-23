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
#include "Output.h"
#include "define.h"
#include <iostream>
#include "AnnoGlobals.h"



COutput::COutput(void)
{
	m_lPOIObjectID = 0;
	mapDispType[-1] = _T("�~");
	mapDispType[2]  = _T("�L���{������");
	mapDispType[4]  = _T("�L���̂�");

	m_mapAnnoCodeDomain[-1] = _T("-");
	m_mapAnnoCodeDomain[-2] = _T("�s���ȓX�܃R�[�h");

	m_lExistOID = 0;
	m_lOverlapOID = 0;
	m_nNewDispType = 0;
	m_nPlaceType =0;
}

COutput::~COutput(void)
{
	m_fout.close();
}

void COutput::SetResult( int n )
{
	switch(n){
		case -2:	m_strResult = _T("�s���ȓX�܃R�[�h"); break;
		case -1:	m_strResult = _T("�~");   break;
		case  0:	m_strResult = _T("����"); break;
		case  1:	m_strResult = _T("�V�K"); break;
		case  2:	m_strResult = _T("PEC"); break;
	}
}

void COutput::SetWornString( int n )
{
	switch(n){
		case 1:		m_strWorn = _T("�����ԍ�"); break;
		case 2:		m_strWorn = _T("SA�{��"); break;
	}
}

void COutput::ResetValue()
{
	m_lNewAnnoOID = m_lOverlapAnnoCode = -1; 
	m_lstOrgAnno.clear(); 
	m_strAccCode.Empty();
	m_lMeshCode = -1;
	m_strOverlapAnnoName.Empty();
	m_strAnnoName.Empty();
	m_lExistOID = 0;
	m_lOverlapOID = 0;
	m_nNewDispType = 0;
	m_nPlaceType = 0;
	m_strResult.Empty();
	m_strExistAnnoName.Empty();
	m_strPOIName.Empty();
	m_nCrossingLinks = -1;
	m_strWorn.Empty();
}

void COutput::InitFile( LPCTSTR lpszFileName )
{
	m_fout.open(lpszFileName);

	m_fout << CW2A( _T("#") ) << std::endl;
	m_fout << CW2A( _T("#FREESTYLELOG	") ) << std::endl;
	m_fout << CW2A( _T("#!r	<-- ���ǔ���Ɏg�p���邽�߁A���̍s�͏����Ȃ��ŉ������B	") ) << std::endl;
	m_fout << CW2A( _T("LAYER\tOBJECTID\t�쐬\t�V�K���LOID\t�\������\t�u�����ꏊ\t�X�R�A\t�d�Ȃ���OID\t�d�Ȃ������\t�d�Ȃ����\������\tWorn") ) << std::endl;
}

std::map<CString, long> COutput::GetPOIFieldMap( IFeature* ipFeature )
{
	if( m_mapPOIField.empty() )
	{
		IFieldsPtr ipFields;
		ipFeature->get_Fields( &ipFields );
		long lFieldnum = 0;
		ipFields->get_FieldCount( &lFieldnum );
		for(long i=0; i<lFieldnum; i++ )
		{
			IFieldPtr ipField;
			ipFields->get_Field( i, &ipField );
			CComBSTR bstrFiledName;
			ipField->get_Name( &bstrFiledName );
			m_mapPOIField[ CString(bstrFiledName)] = i;
		}
	}
	return m_mapPOIField;
}

void COutput::SetPOI( IFeature* ipPOIFeature, long Meshcode )
{
	ResetValue();

	// �I�u�W�F�N�gID
	if(ipPOIFeature)
		ipPOIFeature->get_OID( &m_lPOIObjectID );

	// �o�n�h����
	CComVariant vaVal;
	ipPOIFeature->get_Value( GetPOIFieldMap( ipPOIFeature )[ _T("NAME")], &vaVal );
	m_strPOIName = vaVal;

	// �r�`�{�݋^�f����������x�����O
	if( m_strPOIName.Find( _T("�r�`") ) != -1 || m_strPOIName.Find( _T("�T�[�r�X�G���A") ) != -1 )
		SetWornString( 2 );

	// ���b�V���R�[�h
	m_lMeshCode = Meshcode;
}

void COutput::VariantLog( CComVariant& cval)
{
	USES_CONVERSION;
	m_fout << OLE2A(cval.bstrVal) << std::endl;
}
void COutput::WriteLog()
{
	if( m_strResult != _T("�V�K") )
		m_strWorn.Empty();

	CString strLog;
	strLog.Format( _T("Common_Contents\t%ld\t%s\t%ld\t%s\t%d\t%d\t%ld\t%s\t%s\t%s"), 
					m_lPOIObjectID,	                               //
					m_strResult,
					m_lNewAnnoOID,
					(m_strResult == _T("����") ) ? m_strExistAnnoName : (m_strResult == _T("PEC") || m_strResult == _T("�s���ȓX�܃R�[�h") ) ? m_strPOIName : m_strAnnoName,		                          // 
					m_nPlaceType,
					m_nCrossingLinks,
					m_lOverlapOID,								  // 
					sindy::AheAnnoCode2CodeName( (sindy::schema::annotation::annotation_code::ECode)m_lOverlapAnnoCode ),
					m_strOverlapAnnoName,                          //
					m_strWorn
	);

	
	for( std::list< std::pair<long, int> >::const_iterator it = m_lstOrgAnno.begin(); it != m_lstOrgAnno.end(); it++ )
	{
		CString strTmp;
		strTmp.Format( _T(" ,%ld, %s"), it->first, mapDispType[ it->second ] );
		strLog += strTmp;
	}
	m_fout << CW2A( strLog ) << std::endl;

	ResetValue();
}

void COutput::SetNewAnnoPoint( long lOID, LPCTSTR lpcszName, int nPlace, int nCrossingLinks )
{
	m_lNewAnnoOID = lOID;
	m_strAnnoName = lpcszName;
	m_nPlaceType = nPlace;
	m_nCrossingLinks = nCrossingLinks;
}

void COutput::SetOrgAnnoPoint( long lOID, int type )
{
	std::pair<long, int> OrgAnno;
	OrgAnno.first = lOID;
	OrgAnno.second = type;
	m_lstOrgAnno.push_back( OrgAnno );
}
