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
#include "CheckRegLayer.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckRegLayer::CCheckRegLayer(void) : m_bIsError( false )
{
}

CCheckRegLayer::~CCheckRegLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// �V��ʋK�����C���̃`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckRegLayer::check(CString &rFilePath, int rMeshCode, RegAttrList &rRegAttr)
{
	ZRegLoad aRegLoad;
	if( aRegLoad.load(rFilePath, rMeshCode, 0x00300004) == false )
	{
		cout << rMeshCode << "\tERROR\tREGLINK" << rRegAttr.eError[error::reg::kLayer] << endl;
		return false;
	}

	for(int i = 0; i < (int)aRegLoad.getCount(); ++i)
	{
		INReg* pReg = aRegLoad[i];
		ErrorData rError;
		int iType = CheckType( pReg, rError, rRegAttr, rMeshCode );

		// ���ʕ����̘_���`�F�b�N
		// Reserved1�̃`�F�b�N
		if( pReg->eReserved1 != 0 )
			PrintError( error::reg::kReserve1, rError, iType, false);

		// Reserved2�̃`�F�b�N
		if( pReg->eReserved2 != 0 )
			PrintError( error::reg::kReserve2, rError, iType, false);

		// Reserved3�̃`�F�b�N�A�G�ߋK��������ꍇ������̃`�F�b�N�ɂȂ�
		bool bSeasonF = false;
		if(pReg->eCond.eLink == 2 && pReg->eCond.eCond == 2)	// �ʍs�֎~�ŏ����t�̏ꍇ�̂݁i�����N�ʍs�K���F�ʍs�֎~�A�K�������F�����t�j
		{
			if(pReg->eExCond.eSeasonReg_f == 1)
				bSeasonF = true;
			if(pReg->eExCond.eSeasonReg_f != 0 && pReg->eExCond.eSeasonReg_f != 1)
				PrintError( error::reg::kSeasonF, rError, pReg->eExCond.eSeasonReg_f, iType, true ,true);
			if(pReg->eExCond.eReserved != 0)
				PrintError( error::reg::kSeasonReserved, rError, iType, false);
		}
		else
			if( pReg->eReserved3 != 0 )
				PrintError( error::reg::kReserve3, rError, iType, false);


		// Reserved4�̃`�F�b�N
		if( pReg->eReserved4 != 0 )
			PrintError( error::reg::kReserve4, rError, iType, false);

		// �K�������̃`�F�b�N
		set<int>::const_iterator iter_cond = rRegAttr.eCond.find(pReg->eCond.eCond);
		if( iter_cond == rRegAttr.eCond.end() )
			PrintError( error::reg::kTraffic, rError, pReg->eCond.eCond, iType, true, false );

		// �m�[�h��`�F�b�N
		if( !CheckNode(pReg, rMeshCode, iType) )
			PrintError( error::reg::kNodes, rError, iType, true );

		switch(pReg->eCond.eCond)
		{
		case 0:		// ������
		case 1:		// �ʏ�K��
			// �j���w��̃`�F�b�N
			if( pReg->eCond.eWeek != 0 )
				PrintError( error::reg::kDay2, rError, iType, true );

			// ���Ԏw��̃`�F�b�N
			if( pReg->eCond.eTimeR.eFrom != 0 || pReg->eCond.eTimeR.eTo != 0)
				PrintError( error::reg::kTime2, rError, iType, true );

			// �����w��̃`�F�b�N
			if( pReg->eCond.eDateR.eFrom != 0 || pReg->eCond.eDateR.eTo != 0)
				PrintError( error::reg::kMonth2, rError, iType, true );
			break;

		case 2:		// �����t�K��
			{
				// �j���w��̃`�F�b�N
				set<int>::const_iterator iter_week = rRegAttr.eDays.find(pReg->eCond.eWeek);
				if( iter_week == rRegAttr.eDays.end() )
					PrintError( error::reg::kDay, rError, pReg->eCond.eWeek, iType, true, false );
				
				CString cstrFTime, cstrTTime, cstrFDate, cstrTDate;
				SetObjectID( cstrFTime, pReg->eCond.eTimeR.eFrom );
				SetObjectID( cstrTTime, pReg->eCond.eTimeR.eTo );
				SetObjectID( cstrFDate, pReg->eCond.eDateR.eFrom );
				SetObjectID( cstrTDate, pReg->eCond.eDateR.eTo );
				// �S���K���̃`�F�b�N
				if( pReg->eCond.eWeek == 15 &&
					pReg->eCond.eTimeR.eFrom == 0x0000 && pReg->eCond.eTimeR.eTo == 0x2400 &&
					pReg->eCond.eDateR.eFrom == 0x0101 && pReg->eCond.eDateR.eTo == 0x1231)
					PrintError( error::reg::kAllDay, rError, iType, true );

				// 0�K���̃`�F�b�N
				if( pReg->eCond.eWeek == 0 &&
					pReg->eCond.eTimeR.eFrom == 0x0000 && pReg->eCond.eTimeR.eTo == 0x0000 &&
					pReg->eCond.eDateR.eFrom == 0x0000 && pReg->eCond.eDateR.eTo == 0x0000 )
					PrintError( error::reg::kAllNone, rError, iType, true );

				// �����w�肪�����Ď��Ԏw�肪�Ȃ����̃`�F�b�N
				if( pReg->eCond.eTimeR.eFrom == 0 && pReg->eCond.eTimeR.eTo == 0 &&
					(pReg->eCond.eDateR.eFrom != 0 || pReg->eCond.eDateR.eTo != 0) )
					PrintError( error::reg::kMonthTime, rError, iType, true );

				// ���Ԏw�肪�����Č����w�肪�Ȃ����̃`�F�b�N
				if( pReg->eCond.eDateR.eFrom == 0 && pReg->eCond.eDateR.eTo == 0 &&
					( pReg->eCond.eTimeR.eFrom != 0 || pReg->eCond.eTimeR.eTo != 0 ) )
					PrintError( error::reg::kTimeMonth, rError, iType, true );

				// �j���w�肪�����Ď��Ԏw�肪�Ȃ����̃`�F�b�N
				if( pReg->eCond.eWeek != 0 && pReg->eCond.eTimeR.eFrom == 0 && pReg->eCond.eTimeR.eTo == 0 )
					PrintError( error::reg::kDayTime, rError, iType, true );

				// �j���w�肪�����Č����w�肪�Ȃ����̃`�F�b�N
				if( pReg->eCond.eWeek != 0 && pReg->eCond.eDateR.eFrom == 0 && pReg->eCond.eDateR.eTo == 0 )
					PrintError( error::reg::kDayMonth, rError, iType, true );

				// ���Ԏw��̃`�F�b�N
				if(! CheckTimeCode(pReg->eCond.eTimeR.eFrom, pReg->eCond.eTimeR.eTo))
					PrintError( error::reg::kTime, rError, pReg->eCond.eTime, iType, true, true );

				// �����w��̃`�F�b�N
				if(! CheckDayCode(pReg->eCond.eDateR.eFrom, pReg->eCond.eDateR.eTo))
					PrintError( error::reg::kMonth, rError, pReg->eCond.eDate, iType, true, true );

				// �G�ߋK���`�F�b�N
				if(bSeasonF)
				{
					// �ʔN�K����NG�i���Ԃ�0:00�`24:00�͂����ł͌��Ȃ���OK�ł��傤�j
					if(pReg->eCond.eDateR.eFrom == 0x0101 && pReg->eCond.eDateR.eTo == 0x1231)
						PrintError( error::reg::kSeasonYear, rError, pReg->eCond.eDate, iType, true, true );

					// �S�j���łȂ��ꍇ��NG
					if(pReg->eCond.eWeek != 15)
						PrintError( error::reg::kSeasonWeek, rError, pReg->eCond.eWeek, iType, true, true );

					// 0���K���ł���ꍇ��NG
					CString strStart, strEnd;
					strStart.Format("%x", pReg->eCond.eDateR.eFrom);
					strEnd.Format("%x", pReg->eCond.eDateR.eTo);
					if(_ttol(strStart)/100 == 0 || _ttol(strEnd)/100 == 0)
						PrintError( error::reg::kSeason0Month, rError, pReg->eCond.eDate, iType, true, true );
				}
			}
			break;
		default:
			break;
		}

		// �e�K���ʃ`�F�b�N
		switch(iType)
		{
		case 1: // �����N�K��
			{
				// �K����ʂ̃`�F�b�N
				if(pReg->eCond.eClass != 1) // 1 = �����N�K��
					PrintError( error::reg::kClass, rError, iType, true );
				
				// �ʍs�K���̃`�F�b�N
				set<int>::const_iterator iter_link = rRegAttr.eLink.find(pReg->eCond.eLink);
				if( iter_link == rRegAttr.eLink.end() || pReg->eCond.eLink == 0 )
					PrintError( error::reg::kLink, rError, pReg->eCond.eLink, iType, true, false );
			}
			break;
		case 0: // �m�[�h�K��
			// �K����ʂ̃`�F�b�N
			if(pReg->eCond.eClass != 2) // 2 = �m�[�h�K��
				PrintError( error::reg::kClass, rError, iType, true );

			// �ʍs�K���̃`�F�b�N
			if( pReg->eCond.eLink != 0 )
				PrintError( error::reg::kLink2, rError, iType, true );
			break;
		case 2:
			// �K����ʂ̃`�F�b�N
			if(pReg->eCond.eClass != 3) // 3 = �m�[�h��K��
				PrintError( error::reg::kClass, rError, iType, true );

			// �ʍs�K���̃`�F�b�N
			if( pReg->eCond.eLink != 0 )
				PrintError( error::reg::kLink2, rError, iType, true );
			break;
		default:
			cout << "Rest Type �G���[" << endl;
			m_bIsError = true;
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l�L��)
//////////////////////////////////////////////////////////////////////
void CCheckRegLayer::PrintError(int rKey, ErrorData &rError, int rCode, int rType, bool bError, bool bHex)
{
	char *aError = (bError) ? "ERROR\t" : "WARNING\t";
	CString cstrCode;

	// 16�i���\����10�i���\�����̕ϊ�
	(bHex) ? SetObjectID( cstrCode, rCode ) : cstrCode.Format("%d", rCode );

	switch(rType)
	{
	case 0:
		cout << rError.eNode.eMesh << "\t" << aError << "REGTURN";
		cout << rError.eNode.eError[rKey] << "\t" << static_cast<LPCTSTR>(rError.eNode.eInID);
		cout << "\t" << static_cast<LPCTSTR>(rError.eNode.eID) << "\t";
		cout << static_cast<LPCTSTR>(rError.eNode.eOutID) << "\t" << static_cast<LPCTSTR>(cstrCode) <<endl;
		break;
	case 1:
		cout << rError.eLink.eMesh << "\t" << aError << "REGLINK";;
		cout << rError.eLink.eError[rKey] << "\t" << static_cast<LPCTSTR>(rError.eLink.eFrom);
		cout << "\t" << static_cast<LPCTSTR>(rError.eLink.eTo) << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;
		break;
	case 2:
		cout << rError.eNodes.eMesh << "\t" << aError << "REGNODES";;
		cout << rError.eNodes.eError[rKey];
		for (int i = 0; i < (int)rError.eNodes.eID.size(); ++i )
		{
			cout << "\t" << static_cast<LPCTSTR>(rError.eNodes.eID[i]);
		}
		cout << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;
		break;
	default:
		cout << "�V��ʋK���̃^�C�v���s���ł��B "<< endl;
		break;
	}
	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �G���[�o��(�����l����)
//////////////////////////////////////////////////////////////////////
void CCheckRegLayer::PrintError(int rKey, ErrorData &rError, int rType, bool bError)
{
	char *aError = (bError) ? "ERROR\t" : "WARNING\t";

	switch(rType)
	{
	case 0:
		cout << rError.eNode.eMesh << "\t" << aError << "REGTURN";
		cout << rError.eNode.eError[rKey] << "\t" << static_cast<LPCTSTR>(rError.eNode.eInID);
		cout << "\t" << static_cast<LPCTSTR>(rError.eNode.eID) << "\t";
		cout << static_cast<LPCTSTR>(rError.eNode.eOutID) << endl;
		break;
	case 1:
		cout << rError.eLink.eMesh << "\t" << aError << "REGLINK";
		cout << rError.eLink.eError[rKey] << "\t" << static_cast<LPCTSTR>(rError.eLink.eFrom);
		cout << "\t" << static_cast<LPCTSTR>(rError.eLink.eTo) << endl;
		break;
	case 2:
		cout << rError.eNodes.eMesh << "\t" << aError << "REGNODES";
		cout << rError.eNodes.eError[rKey];
		for (int i = 0; i < (int)rError.eNodes.eID.size(); ++i )
		{
			cout << "\t" << static_cast<LPCTSTR>(rError.eNodes.eID[i]);
		}
		cout << endl;
		break;
	default:
		cout << "�V��ʋK���̃^�C�v���s���ł��B "<< endl;
		break;
	}
	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// �K���^�C�v�𒲍�����
//////////////////////////////////////////////////////////////////////
int CCheckRegLayer::CheckType(const INReg *rReg, ErrorData& rError, RegAttrList &rRegAttr, int rMeshCode)
{
	int iType = 0;
	if( rReg->eID3 == 0 )
		++iType;
	if( rReg->eID2 == 0 ) 
		++iType;

	switch( iType )
	{
	case 0:
		SetObjectID( rError.eNode.eInID, rReg->eID2 );
		SetObjectID( rError.eNode.eID, rReg->eID1 );
		SetObjectID( rError.eNode.eOutID, rReg->eID3 );
		rError.eNode.eMesh	= rMeshCode;
		rError.eNode.eError	= rRegAttr.eError;
		break;
	case 1:
		SetObjectID( rError.eLink.eFrom, rReg->eID1 );
		SetObjectID( rError.eLink.eTo, rReg->eID2 );
		rError.eLink.eMesh	= rMeshCode;
		rError.eLink.eError	= rRegAttr.eError;
		break;
	case 2:
		{
			rError.eNodes.eMesh	= rMeshCode;
			rError.eNodes.eError	= rRegAttr.eError;
			CString cstrID;
			SetObjectID( cstrID, rReg->eID1 );
			rError.eNodes.eID.push_back( cstrID );
			for(int i = 0; i < rReg->eCount; ++i )
			{
				CString cstrToID;
				SetObjectID( cstrToID, rReg->eList[i].eNode );
				rError.eNodes.eID.push_back( cstrToID );
			}
		}
		break;
	default:
		break;
	}

	return iType;
}

//////////////////////////////////////////////////////////////////////
// �m�[�h��̖������`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckRegLayer::CheckNode(const INReg *rReg, int rMeshCode, int rType)
{
	typedef std::pair<int, int> NodeData;	//<2�����b�V��, �m�[�hID>
	std::set<NodeData> aNodeSet;
	bool bFlag = false;

	switch(rType)
	{	
	case 1:
		aNodeSet.insert(NodeData(rReg->eID1, rMeshCode));
		aNodeSet.insert(NodeData(rReg->eID2, rMeshCode));
		bFlag = (aNodeSet.size() == 2);
		break;
	case 0:
		aNodeSet.insert(NodeData(rReg->eID1, rMeshCode));
		aNodeSet.insert(NodeData(rReg->eID2, rMeshCode));
		aNodeSet.insert(NodeData(rReg->eID3, rMeshCode));
		bFlag = (aNodeSet.size() == 3);
		break;
	case 2:
		for(int i = 0; i < rReg->eCount; ++i)
		{
			if( i != rReg->eCount - 1 )
			{
				aNodeSet.insert(NodeData (rReg->eList[i].eNode, rReg->eList[i].eMesh));
			}
			else
			{
				if(rReg->eList[i-1].eNode == rReg->eList[i].eNode)
					return(bFlag);
				if(rReg->eList[i-1].eMesh != rReg->eList[i].eMesh)
				return(bFlag);
			} 
		}
		bFlag = ((int)aNodeSet.size()+1 == rReg->eCount );
		break;
	default:
		cout << "�V��ʋK���̃^�C�v�̒l���s���ł��B" << endl;
		m_bIsError = true;
		break;
	}
	
	return bFlag;
}

//////////////////////////////////////////////////////////////////////
// ���Ԏw��`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckRegLayer::CheckTimeCode(int rStart, int rEnd)
{
	CString cstrStart, cstrEnd;
	cstrStart.Format("%x", rStart);
	cstrEnd.Format("%x", rEnd);

	int iStartTime	= _ttol(cstrStart)/100;
	int iStartMin	= _ttol(cstrStart)%100;
	int iEndTime	= _ttol(cstrEnd)/100;
	int iEndMin		= _ttol(cstrEnd)%100;

	if(iStartTime < 0 || iStartTime > 24) return false;
	if(iEndTime < 0 || iEndTime > 24) return false;
	if(iStartMin < 0 || iStartMin > 59) return false;
	if(iEndMin < 0 || iEndMin > 59) return false;
	if(iStartTime == 24 && iStartMin != 0) return false;
	if(iEndTime == 24 && iEndMin != 0) return false;
	if(iStartTime == iEndTime && iStartMin > iEndMin) return false;

	if((iStartMin % 10) != 0 && iStartMin != 15 && iStartMin != 45) return false;
	if((iEndMin % 10) != 0 && iEndMin != 15 && iEndMin != 45) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
// �����w��`�F�b�N
//////////////////////////////////////////////////////////////////////
bool CCheckRegLayer::CheckDayCode(int rStart, int rEnd)
{
	CString cstrStart, cstrEnd;
	cstrStart.Format("%x", rStart);
	cstrEnd.Format("%x", rEnd);

	int iStartMon	= _ttol(cstrStart)/100;
	int iStartDay	= _ttol(cstrStart)%100;
	int iEndMon		= _ttol(cstrEnd)/100;
	int iEndDay		= _ttol(cstrEnd)%100;

	if(iStartDay < 1 || iStartDay > 31) return false;
	if(iEndDay < 1 || iEndDay > 31) return false;
	if( iStartMon == 0 && iEndMon == 0 )
	{
		//if(iStartDay == 1 && iEndDay == 31) return false;
	}
	else
	{
		if(iStartMon < 1 || iStartMon > 12) return false;
		if(iEndMon < 1 || iEndMon > 12) return false;
	}

	return true;
}
