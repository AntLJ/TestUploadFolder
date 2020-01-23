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
#include "CheckNodeLayer.h"
#include "Z2NodeLoad.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"
#include "SetRoadAttrList.h"

CCheckNodeLayer::CCheckNodeLayer(void) : m_bIsError( false )
{
}

CCheckNodeLayer::~CCheckNodeLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// ���H�m�[�h���C�����`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckNodeLayer::check(CString &rFilePath, int rMeshCode, NodeAttrList &rNodeAttr)
{
	Z2NodeLoad aNodeLoad;
	if( aNodeLoad.load(rFilePath, rMeshCode) == false )
	{
		cout << rMeshCode << "\tERROR" << rNodeAttr.eError[error::node::kLayer] << endl;
		return false;
	}

	ErrorNode aError;
	aError.eError = rNodeAttr.eError;
	aError.eMesh = rMeshCode;

	for(int i = 0; i < (int)aNodeLoad.getCount(); ++i)
	{
		IN2Node* pNode = aNodeLoad[i];
		SetObjectID(aError.eID, pNode->eID);

		// 28bit(2^28(268,435,456))�ȏ�̃f�[�^�̏ꍇ��NG
		if(pNode->eID & 0xf0000000)
			PrintHexError( error::node::kNodeID, aError, pNode->eID, true );

		// �M���@�t���O�̃`�F�b�N
		set<int>::iterator iter_signal = rNodeAttr.eSignal.find( pNode->eSignal );
		if( iter_signal == rNodeAttr.eSignal.end() )
			PrintError( error::node::kSignal, aError, pNode->eSignal, true);

		// ��ʃR�[�h�̃`�F�b�N
		set<int>::iterator iter_class = rNodeAttr.eNodeClass.find( pNode->eClass );
		if( iter_class == rNodeAttr.eNodeClass.end() )
			PrintHexError( error::node::kClass, aError, pNode->eClass, true );

		// ���K��X���W
		if(pNode->eX < 0 || pNode->eX > 1000000)
			PrintError( error::node::kXY, aError, pNode->eX, true );

		// ���K��Y���W
		if(pNode->eY < 0 || pNode->eY > 1000000)
			PrintError( error::node::kXY, aError, pNode->eY, true );

		// ���H�����L���t���O
		set<int>::iterator iter_height = rNodeAttr.eHeight.find( pNode->eRoadAvail );
		if( iter_height == rNodeAttr.eHeight.end() )
			PrintError( error::node::kHeight1, aError, pNode->eRoadAvail, true );

		// ���H���΍�
		if( pNode->eRoadAvail == 0 && pNode->eRoadLevel != 0 )
			PrintError( error::node::kHeight2, aError, pNode->eRoadLevel, true );

		// Reserved1�̃`�F�b�N
		if( pNode->eReserved1 != 0 )
			PrintError( error::node::kReserve1, aError, false );

		// Reserved2�̃`�F�b�N
		if( pNode->eReserved2 != 0 )
			PrintError( error::node::kReserve2, aError, false );

		// Reserved4�̃`�F�b�N
		if( pNode->eReserved4 != 0 )
			PrintError( error::node::kReserve4, aError, false );

		// Reserved5�̃`�F�b�N
		if( pNode->eReserved5 != 0 )
			PrintError( error::node::kReserve5, aError, false );

		// �ڑ������N�{���̃`�F�b�N
		if( pNode->eJointCount < 1 || pNode->eJointCount > 8 )
		{
			PrintError( error::node::kConnectCount, aError, pNode->eJointCount, true );
			continue;
		}

		for( int iCount = 0; iCount < pNode->eJointCount; ++ iCount )
		{
			// �ڑ��m�[�h�`�F�b�N
			IN2Node* aTemp = aNodeLoad.search( pNode->eJoint[iCount].eID );
			if( !aTemp )
				PrintHexError( error::node::kConnectNode, aError, pNode->eJoint[iCount].eID, true );

			// �m�[�hID(24->28bit)�Ή�	2010/5/12
			// ���4bit��Reserved�ɂȂ�̂Œl�������Ă���ꍇ��NG
			long aReserved = pNode->eJoint[iCount].eReserved;
			if(aReserved != 0)
				PrintHexError( error::node::kConnectNodeIDReserved, aError, pNode->eJoint[iCount].eReserved, true );
			// 28bit(2^28(268,435,456))�ȏ�̃f�[�^�̏ꍇ��NG
			if(pNode->eJoint[iCount].eID & 0xf0000000)
				PrintHexError( error::node::kConnectNodeID, aError, pNode->eJoint[iCount].eID, true );
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��:16�i��)
//////////////////////////////////////////////////////////////////////
void CCheckNodeLayer::PrintHexError(int rKey, ErrorNode &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	CString cstrCode;
	SetObjectID(cstrCode, rCode);
	cout << rError.eMesh << aError << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eID) << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��:10�i��)
//////////////////////////////////////////////////////////////////////
void CCheckNodeLayer::PrintError(int rKey, ErrorNode &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eID) << "\t" << rCode << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�Ȃ�)
//////////////////////////////////////////////////////////////////////
void CCheckNodeLayer::PrintError(int rKey, ErrorNode &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eID) << endl;

	m_bIsError = true;
}
