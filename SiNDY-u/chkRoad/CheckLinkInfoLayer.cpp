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
#include "CheckLinkInfoLayer.h"
#include "ZRInfoLoad.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckLinkInfoLayer::CCheckLinkInfoLayer(void)
{
}

CCheckLinkInfoLayer::~CCheckLinkInfoLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// �����N��񃌃C�����`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckLinkInfoLayer::check(CString &rFilePath, int rMeshCode, LInfoAttrList &rLInfoAttr)
{
	// ���C�����쐬����Ă��Ȃ������`�F�b�N����B�쐬����Ă�����x����\������B
	// ���̃��C����2001�N�t�f�[�^���g�p���Ă��Ȃ��B
	ZRInfoLoad aLInfoLoad;
	if( aLInfoLoad.load(rFilePath, rMeshCode) == true )
	{
		cout << rMeshCode << "\tWARNING\t" << rLInfoAttr.eError[error::link_info::kLayer] << endl;
		return false;
	}
	else
		return true;

	ErrorLInfo aError;
	aError.eMesh = rMeshCode;
	aError.eError = rLInfoAttr.eError;

	for(int i = 0; i < (int)aLInfoLoad.getCount(); ++i)
	{
		INRInfo* pLInfo = aLInfoLoad[i];
		SetObjectID(aError.eFrom, pLInfo->eID1);
		SetObjectID(aError.eTo, pLInfo->eID2);

		// ��H��::�\����ʂ��`�F�b�N
		set<int>::const_iterator iter_maindisp = rLInfoAttr.eDisp.find(pLInfo->eMainInfo.eClass);
		if( iter_maindisp == rLInfoAttr.eDisp.end() )
			PrintHexError( error::link_info::kMainDisp, aError, pLInfo->eMainInfo.eClass, true ); 

		// ��H��::Reserve ���`�F�b�N
		if( pLInfo->eMainInfo.eReserved != 0 )
			PrintError( error::link_info::kMainReserve, aError, false );

		// ��H��::��]�敪�R�[�h���`�F�b�N
		set<int>::const_iterator iter_mainsection = rLInfoAttr.eSection.find(pLInfo->eMainInfo.eCode);
		if( iter_mainsection == rLInfoAttr.eSection.end() )
			PrintError( error::link_info::kMainSection, aError, pLInfo->eMainInfo.eCode, true ); 

		// �����N�����`�F�b�N
		if( pLInfo->eLength == 0) 
			PrintError( error::link_info::kLength, aError, false );

		// �K�����x���`�F�b�N
		set<int>::const_iterator iter_speed = rLInfoAttr.eSpeed.find(pLInfo->eLimitSpeed);
		if( iter_speed == rLInfoAttr.eSection.end() )
			PrintError( error::link_info::kSpeed, aError, pLInfo->eLimitSpeed, true );

		// �S�Ă̒l��0�Ŗ��������`�F�b�N
		if( pLInfo->eMainInfo.eRoadNo && pLInfo->eMainInfo.eCode == 0 &&
			pLInfo->eWidth == 0 && pLInfo->eCenterArea == 0 &&
			pLInfo->eTripSpeed == 0 && pLInfo->eLimitSpeed == 0 )
			PrintError( error::link_info::kAllNone, aError, true);

		for( int iCount = 0; iCount < pLInfo->eCount; ++iCount )
		{
			// �d�v�H��::�\����ʃR�[�h�̃`�F�b�N
			set<int>::const_iterator iter_subdisp = rLInfoAttr.eDisp.find(pLInfo->eInfo[iCount].eClass);
			if( iter_subdisp == rLInfoAttr.eDisp.end() )
				PrintHexError( error::link_info::kSubDisp, aError, pLInfo->eInfo[iCount].eClass, true ); 

			// �d�v�H��::Reserve�̃`�F�b�N
			if( pLInfo->eInfo[iCount].eReserved != 0 )
				PrintError( error::link_info::kSubReserve, aError, false );

			// �d�v�H��::��]�敪�̃`�F�b�N
			set<int>::const_iterator iter_subsection = rLInfoAttr.eSection.find(pLInfo->eInfo[iCount].eCode);
			if( iter_subsection != rLInfoAttr.eSection.end() )
				PrintError( error::link_info::kSubSection, aError, pLInfo->eInfo[iCount].eCode, true );
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��:16�i��)
//////////////////////////////////////////////////////////////////////
void CCheckLinkInfoLayer::PrintHexError(int rKey, ErrorLInfo &rError, int rCode, bool bError)
{
	CString cstrCode;
	SetObjectID(cstrCode, rCode);

	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo);
	cout << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��:10�i��)
//////////////////////////////////////////////////////////////////////
void CCheckLinkInfoLayer::PrintError(int rKey, ErrorLInfo &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo);
	cout << "\t" << rCode << endl;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l����)
//////////////////////////////////////////////////////////////////////
void CCheckLinkInfoLayer::PrintError(int rKey, ErrorLInfo &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo) << endl;
}
