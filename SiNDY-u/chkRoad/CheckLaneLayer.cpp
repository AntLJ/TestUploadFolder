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
#include "CheckLaneLayer.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckLaneLayer::CCheckLaneLayer(void) : m_bIsError( false )
{
}

CCheckLaneLayer::~CCheckLaneLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// ���[����񃌃C�����`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckLaneLayer::check(CString &rFilePath, int rMeshCode, LaneAttrList &rLaneAttr)
{
	// ���C�����J���邩
	ZLaneLoad aLaneLoad;
	if( aLaneLoad.load(rFilePath, rMeshCode) == false )
	{
		cout << rMeshCode << "\tERROR" << rLaneAttr.eError[error::lane::kLayer] << endl;
		return false;
	}

	ErrorLane aError;
	aError.eMesh = rMeshCode;
	aError.eError = rLaneAttr.eError;

	for(int i = 0; i < (int)aLaneLoad.getCount(); ++i)
	{
		INLane* pLane = aLaneLoad[i];
		aError.eID.clear();
		SetID( pLane, aError );

		// Reserved1�̃`�F�b�N
		if( pLane->eReserved1 != 0 )
			PrintError( error::lane::kReserve1, aError, false );

		// Reserved2�̃`�F�b�N
		if( pLane->eReserved2 != 0 )
			PrintError( error::lane::kReserve2, aError, false );

		// ���o�惌�[����(Reserved3)�̃`�F�b�N
		if( pLane->eToCount != 0)
			PrintError( error::lane::kReserve3, aError, false );

		// �����������[�����̃`�F�b�N
		if( pLane->eLeftDiff < -32 || 32 < pLane->eLeftDiff )
			PrintError( error::lane::kLeftDiff, aError, pLane->eLeftDiff, true ); 

		// �E���������[�����̃`�F�b�N
		if( pLane->eRightDiff < -32 || 32 < pLane->eRightDiff )
			PrintError( error::lane::kRightDiff, aError, pLane->eRightDiff, true ); 

		// �����_�i�����[�����̃`�F�b�N
		if( pLane->eFromCount <= 0 || 32 < pLane->eFromCount )
			PrintError( error::lane::kFromCount, aError, pLane->eFromCount, true );

		// ���[�����̃`�F�b�N
		unsigned int  aMask = 0xffffffff >> pLane->eFromCount;
		if((pLane->eInfo & aMask ) != 0 ) 
			PrintHexError( error::lane::kInfo, aError, pLane->eInfo, true );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��:16�i��)
//////////////////////////////////////////////////////////////////////
void CCheckLaneLayer::PrintHexError(int rKey, ErrorLane &rError, int rCode, bool bError)
{
	CString cstrCode;
	SetObjectID( cstrCode, rCode );

	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��:10�i��)
//////////////////////////////////////////////////////////////////////
void CCheckLaneLayer::PrintError(int rKey, ErrorLane &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << "\t" << rCode << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l����)
//////////////////////////////////////////////////////////////////////
void CCheckLaneLayer::PrintError(int rKey, ErrorLane &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�Ώۂ̃m�[�hID�W������͂���B
//////////////////////////////////////////////////////////////////////
void CCheckLaneLayer::SetID(INLane *pLane, ErrorLane &rError)
{
	CString cstrFrom, cstrID, cstrTo;
	if( pLane->eFrom != 0 )
	{
		SetObjectID( cstrFrom, pLane->eFrom );
		rError.eID.push_back(cstrFrom);
	}
	SetObjectID( cstrID, pLane->eID );
	rError.eID.push_back(cstrID);

	if( pLane->eCount > 0 )
	{
		for( int i = 0; i < pLane->eCount; ++i )
		{
			SetObjectID( cstrTo, pLane->eNode[i].eNode );
			rError.eID.push_back(cstrTo);
		}
	}
}
