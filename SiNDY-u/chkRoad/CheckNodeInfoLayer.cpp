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
#include "CheckNodeInfoLayer.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckNodeInfoLayer::CCheckNodeInfoLayer(void) : m_bIsError( false )
{
}

CCheckNodeInfoLayer::~CCheckNodeInfoLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// �m�[�h��񃌃C�����`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckNodeInfoLayer::check(CString &rFilePath, int rMeshCode, NInfoAttrList &rNInfoAttr)
{
	// ���C�����J���邩
	ZNInfoLoad aNInfoLoad;
	if( aNInfoLoad.load(rFilePath, rMeshCode ) == false )
	{
		cout << rMeshCode << "\tERROR" << rNInfoAttr.eError[error::node_info::kLayer] << endl;
		return false;
	}

	ErrorNInfo aError;
	aError.eMesh = rMeshCode;
	aError.eError = rNInfoAttr.eError;

	for(int i = 0; i < (int)aNInfoLoad.getCount(); ++i)
	{
		INNInfo* pNInfo = aNInfoLoad[i];
		aError.eID.clear();
		SetID( pNInfo, aError );

		if( rMeshCode == 534041)
			int a=0;

		// Reserved1�̃`�F�b�N
		if( pNInfo->eReserved1 != 0 )
			PrintError( error::node_info::kReserve1, aError, false );

		// Reserved2�̃`�F�b�N
		if( pNInfo->eReserved2 != 0 )
			PrintError( error::node_info::kReserve2, aError, false );

		// Reserved3�̃`�F�b�N
		if( pNInfo->eReserved3 != 0 )
			PrintError( error::node_info::kReserve3, aError, false );

		// Reserved4�̃`�F�b�N
		if( pNInfo->eReserved4 != 0 )
			PrintError( error::node_info::kReserve4, aError, false );

		// Reserved5�̃`�F�b�N
		if( pNInfo->eReserved5 != 0 )
			PrintError( error::node_info::kReserve5, aError, false );

		// Reserved6�̃`�F�b�N
		if( pNInfo->eReserved6 != 0 )
			PrintError( error::node_info::kReserve6, aError, false );

		// �m�[�h��̃`�F�b�N
		if( !CheckNode( pNInfo, aError))
			PrintError( error::node_info::kNodes, aError, true );

		// �m�[�h����ʃR�[�h�̃`�F�b�N
		set<int>::const_iterator iter_class = rNInfoAttr.eClass.find(pNInfo->eClass);
		if( iter_class == rNInfoAttr.eClass.end() )
			PrintHexError(error::node_info::kInfoClass, aError, pNInfo->eClass, true );

		// �n���ʂ̃`�F�b�N
		if(pNInfo->eClass == 0x01000070 || pNInfo->eClass == 0x01000060) // ���ʈē�(��ʓ����ʈē�) �܂��́A�������H���ʈē�
		{
			CString cstrCode, cstrGuideClass;
			cstrCode.Format("%x", pNInfo->eCode);
			cstrGuideClass = cstrCode.Left(cstrCode.GetLength() -4);
			int iGuideClass = strtol(cstrGuideClass, NULL, 16);
			set<int>::const_iterator iter_dirclass = rNInfoAttr.eGuideClass.find(iGuideClass);
			if( iter_dirclass == rNInfoAttr.eGuideClass.end() )
				PrintHexError(error::node_info::kGuideClass, aError, iGuideClass, true );
		}
		else
		{
			// ���ʈē��ȊO�Œn���ʂ������Ă��Ȃ������`�F�b�N
			CString cstrCode, cstrGuideClass;
			cstrCode.Format("%x", pNInfo->eCode);
			cstrGuideClass = cstrCode.Left(cstrCode.GetLength() -4);
			int iGuideClass = strtol(cstrGuideClass, NULL, 16);
			if( iGuideClass != 0 )
				PrintHexError(error::node_info::kBothClass, aError, iGuideClass, true );
		}

		// �ē��R�[�h�̃`�F�b�N
		if(pNInfo->eClass == 0x01000090 || pNInfo->eClass == 0x01000091 ) // �����U��
		{
			CString cstrCode, cstrGuideCode;
			cstrCode.Format("%x", pNInfo->eCode);
			cstrGuideCode = cstrCode.Right(4);
			int iGuideCode = strtol(cstrGuideCode, NULL, 16);
			set<int>::const_iterator iter_guideclass = rNInfoAttr.eGuideCode.find(iGuideCode);
			if( iter_guideclass == rNInfoAttr.eGuideCode.end() )
				PrintHexError(error::node_info::kGuideCode, aError, iGuideCode, true );
		}
		else
		{
			// �����U���ȊO�ňē��R�[�h�������Ă��Ȃ������`�F�b�N
			CString cstrCode, cstrGuideCode;
			cstrCode.Format("%x", pNInfo->eCode);
			cstrGuideCode = cstrCode.Right(4);
			int iGuideCode = strtol(cstrGuideCode, NULL, 16);
			if( iGuideCode != 0 )
				PrintHexError(error::node_info::kBothCode, aError, iGuideCode, true );
		}

		// ������`�F�b�N
		int iNameCount = aNInfoLoad.getNameCount(i);
		CString cstrName(aNInfoLoad.getName(i), iNameCount);

		for(int iCntKanji = 0; iCntKanji < iNameCount; iCntKanji += 2)
		{
			int aTmp = ((int)cstrName[iCntKanji] << 8) & 0x0000ffff;
			int aTmp2 = (int)(cstrName[iCntKanji + 1]) & 0x00ff;
			int aCode = aTmp | aTmp2;

			// �g�pOK�����`�F�b�N
			if(!(( 0x824f <= aCode && aCode <=0x8258 ) ||	//�i�S�p�����j
				( 0x8260 <= aCode && aCode <=0x8279 ) ||	//�i�S�p�啶���p���j
				( 0x8281 <= aCode && aCode <=0x829a ) ||	//�i�S�p�������p���j
				( 0x829f <= aCode && aCode <=0x82f1 ) ||	//�i�S�p���ȁj
				( 0x8340 <= aCode && aCode <=0x8394 ) ||	//�i�S�p�J�i�j
				( 0x889f <= aCode && aCode <=0x9872 ) ||	//�i������1�����j
				( 0x989f <= aCode && aCode <=0xeaa4 )		//�i������2�����j
				)){
					// �g�pOK���ꕶ���`�F�b�N
					if(0x01000060 == pNInfo->eClass || 0x01000070 == pNInfo->eClass){	// ���ʈē��A�ē��Ŕ̏ꍇ
						if(rNInfoAttr.eUseOKCharDirCode.end() == rNInfoAttr.eUseOKCharDirCode.find(aCode)){
							PrintStringError( error::node_info::kKanjiNone, aError, cstrName, true);
							break;
						}
					} else {	// ���ʈē��ȊO�̏ꍇ�i���݂͌����_���̂̂݁j����K�v�ł���Ώ����𕪂���
						if(rNInfoAttr.eUseOKCharNodeCode.end() == rNInfoAttr.eUseOKCharNodeCode.find(aCode)){
							PrintStringError( error::node_info::kKanjiNoneN, aError, cstrName, true);
							break;
						}
					}
			}
		}

		// �J�i������`�F�b�N
		int iYomiCount = aNInfoLoad.getYomiCount(i);
		CString aYomiName(aNInfoLoad.getYomi(i), iYomiCount);

		for(int iCntYomi = 0; iCntYomi < iYomiCount; ++iCntYomi )
		{
			int aChar = (int)aYomiName[iCntYomi] & 0x00ff;
			if(!(0x00a6 <= aChar && aChar <= 0x00df)){
				// �g�pOK���p�����`�F�b�N
				if(!(aChar == 0x00a5 ||	// '�'
					aChar == 0x002e ||	// '.'
					aChar == 0x007e ||	// '~'
					aChar == 0x0028 ||	// '('
					aChar == 0x0029		// ')'
					)){
						if(pNInfo->eClass == 0x01000060 || pNInfo->eClass == 0x01000070 ) { // ���ʈē��A�ē��Ŕ̏ꍇ
							if(!(	aChar == 0x00a4 || // '�'
									aChar == 0x002d    // '-'
								)) {
									PrintStringError( error::node_info::kKanaNone, aError, aYomiName, true );
									break;
							}
						} else {
							PrintStringError( error::node_info::kKanaNone, aError, aYomiName, true );
							break;
						}
				}
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�Ώۂ̃m�[�hID�W������͂���B
//////////////////////////////////////////////////////////////////////
void CCheckNodeInfoLayer::SetID(INNInfo *pNInfo, ErrorNInfo &rError)
{
	CString cstrFrom, cstrID, cstrTo;
	if( pNInfo->eFrom != 0 )
	{
		SetObjectID( cstrFrom, pNInfo->eFrom );
		rError.eID.push_back(cstrFrom);
	}
	SetObjectID( cstrID, pNInfo->eID );
	rError.eID.push_back( cstrID );

	if( pNInfo->eCount > 0 )
	{
		for( int i = 0; i < pNInfo->eCount; ++i )
		{
			SetObjectID( cstrTo, pNInfo->eList[i].eNode );
			rError.eID.push_back(cstrTo);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��:������)
//////////////////////////////////////////////////////////////////////
void CCheckNodeInfoLayer::PrintStringError(int rKey, ErrorNInfo &rError, CString& rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << "\t" << static_cast<LPCTSTR>(rCode) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��:16�i��)
//////////////////////////////////////////////////////////////////////
void CCheckNodeInfoLayer::PrintHexError(int rKey, ErrorNInfo &rError, int rCode, bool bError)
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
void CCheckNodeInfoLayer::PrintError(int rKey, ErrorNInfo &rError, int rCode, bool bError)
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
void CCheckNodeInfoLayer::PrintError(int rKey, ErrorNInfo &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �m�[�h��̏d���`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckNodeInfoLayer::CheckNode(INNInfo *pNInfo, ErrorNInfo& rError)
{
	if( pNInfo->eFrom == pNInfo->eID )
		return false;

	typedef pair<int,int>	NodeData;

	// NodeSet		��� �m�[�h����r�p
	// NodeSetCheck	��� �ŏI�m�[�h�d������p
	set<NodeData>			NodeSet, NodeSetCheck;

	if( pNInfo->eFrom > 0 )
	{
		NodeSet.insert(NodeData(pNInfo->eFrom, rError.eMesh));
		NodeSetCheck.insert(NodeData(pNInfo->eFrom, rError.eMesh));
	}
	NodeSet.insert(NodeData(pNInfo->eID, rError.eMesh));
	NodeSetCheck.insert(NodeData(pNInfo->eID, rError.eMesh));

	for(int i = 0; i < pNInfo->eCount; ++i)
	{
		// �ŏI�_�ȊO�ŏd�������������`�F�b�N����B
		if( i!= pNInfo->eCount - 1 )
		{
			NodeData aNode(pNInfo->eList[i].eNode, pNInfo->eList[i].eMesh);
			set<NodeData>::iterator iter_set = NodeSet.find(aNode);
			if( iter_set != NodeSet.end() )
				return false;
			NodeSetCheck.insert(aNode);
		}

		// ���b�V�����E�m�[�h��ID���אڃ��b�V���œ��ꂩ���`�F�b�N
		if( i> 0 && pNInfo->eList[i-1].eNode != pNInfo->eList[i].eNode &&
			pNInfo->eList[i-1].eMesh != pNInfo->eList[i].eMesh )
			return false;

		// ���b�V�����E�m�[�h�Ԃōs�������Ă��Ȃ������`�F�b�N
		if( i>1 && pNInfo->eList[i-2].eNode == pNInfo->eList[i].eNode )
			return false;

		// �I�_�ƏI�_��O�̓_��ID������łȂ������`�F�b�N
		if( i>0 && i == pNInfo->eCount - 1 && pNInfo->eList[i-1].eNode == pNInfo->eList[i].eNode )
			return false;

		// �����ςݗ��o�m�[�h�����i�[
		NodeSet.insert(NodeData(pNInfo->eList[i].eNode, pNInfo->eList[i].eMesh));
	}

	// �i�[������񐔂��I�_���������m�[�h���ƈ�v���邩���`�F�b�N
	if( pNInfo->eFrom > 0 && (int)NodeSet.size() == pNInfo->eCount + 2 )
		return true;
	else if( pNInfo->eFrom == 0 && (int)NodeSet.size() == pNInfo->eCount + 1)
		return true;
	else
	{
		NodeData aNode(pNInfo->eList[pNInfo->eCount-1].eNode, pNInfo->eList[pNInfo->eCount-1].eMesh);
		set<NodeData>::iterator iter_check = NodeSetCheck.find(aNode);
		if( iter_check == NodeSetCheck.end() )
			return false;
	}

	return true;
}
