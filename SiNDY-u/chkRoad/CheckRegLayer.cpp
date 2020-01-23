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
// 新交通規制レイヤのチェック
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

		// 共通部分の論理チェック
		// Reserved1のチェック
		if( pReg->eReserved1 != 0 )
			PrintError( error::reg::kReserve1, rError, iType, false);

		// Reserved2のチェック
		if( pReg->eReserved2 != 0 )
			PrintError( error::reg::kReserve2, rError, iType, false);

		// Reserved3のチェック、季節規制がある場合そちらのチェックになる
		bool bSeasonF = false;
		if(pReg->eCond.eLink == 2 && pReg->eCond.eCond == 2)	// 通行禁止で条件付の場合のみ（リンク通行規制：通行禁止、規制条件：条件付）
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


		// Reserved4のチェック
		if( pReg->eReserved4 != 0 )
			PrintError( error::reg::kReserve4, rError, iType, false);

		// 規制条件のチェック
		set<int>::const_iterator iter_cond = rRegAttr.eCond.find(pReg->eCond.eCond);
		if( iter_cond == rRegAttr.eCond.end() )
			PrintError( error::reg::kTraffic, rError, pReg->eCond.eCond, iType, true, false );

		// ノード列チェック
		if( !CheckNode(pReg, rMeshCode, iType) )
			PrintError( error::reg::kNodes, rError, iType, true );

		switch(pReg->eCond.eCond)
		{
		case 0:		// 未調査
		case 1:		// 通常規制
			// 曜日指定のチェック
			if( pReg->eCond.eWeek != 0 )
				PrintError( error::reg::kDay2, rError, iType, true );

			// 時間指定のチェック
			if( pReg->eCond.eTimeR.eFrom != 0 || pReg->eCond.eTimeR.eTo != 0)
				PrintError( error::reg::kTime2, rError, iType, true );

			// 月日指定のチェック
			if( pReg->eCond.eDateR.eFrom != 0 || pReg->eCond.eDateR.eTo != 0)
				PrintError( error::reg::kMonth2, rError, iType, true );
			break;

		case 2:		// 条件付規制
			{
				// 曜日指定のチェック
				set<int>::const_iterator iter_week = rRegAttr.eDays.find(pReg->eCond.eWeek);
				if( iter_week == rRegAttr.eDays.end() )
					PrintError( error::reg::kDay, rError, pReg->eCond.eWeek, iType, true, false );
				
				CString cstrFTime, cstrTTime, cstrFDate, cstrTDate;
				SetObjectID( cstrFTime, pReg->eCond.eTimeR.eFrom );
				SetObjectID( cstrTTime, pReg->eCond.eTimeR.eTo );
				SetObjectID( cstrFDate, pReg->eCond.eDateR.eFrom );
				SetObjectID( cstrTDate, pReg->eCond.eDateR.eTo );
				// 全日規制のチェック
				if( pReg->eCond.eWeek == 15 &&
					pReg->eCond.eTimeR.eFrom == 0x0000 && pReg->eCond.eTimeR.eTo == 0x2400 &&
					pReg->eCond.eDateR.eFrom == 0x0101 && pReg->eCond.eDateR.eTo == 0x1231)
					PrintError( error::reg::kAllDay, rError, iType, true );

				// 0規制のチェック
				if( pReg->eCond.eWeek == 0 &&
					pReg->eCond.eTimeR.eFrom == 0x0000 && pReg->eCond.eTimeR.eTo == 0x0000 &&
					pReg->eCond.eDateR.eFrom == 0x0000 && pReg->eCond.eDateR.eTo == 0x0000 )
					PrintError( error::reg::kAllNone, rError, iType, true );

				// 月日指定があって時間指定がないかのチェック
				if( pReg->eCond.eTimeR.eFrom == 0 && pReg->eCond.eTimeR.eTo == 0 &&
					(pReg->eCond.eDateR.eFrom != 0 || pReg->eCond.eDateR.eTo != 0) )
					PrintError( error::reg::kMonthTime, rError, iType, true );

				// 時間指定があって月日指定がないかのチェック
				if( pReg->eCond.eDateR.eFrom == 0 && pReg->eCond.eDateR.eTo == 0 &&
					( pReg->eCond.eTimeR.eFrom != 0 || pReg->eCond.eTimeR.eTo != 0 ) )
					PrintError( error::reg::kTimeMonth, rError, iType, true );

				// 曜日指定があって時間指定がないかのチェック
				if( pReg->eCond.eWeek != 0 && pReg->eCond.eTimeR.eFrom == 0 && pReg->eCond.eTimeR.eTo == 0 )
					PrintError( error::reg::kDayTime, rError, iType, true );

				// 曜日指定があって月日指定がないかのチェック
				if( pReg->eCond.eWeek != 0 && pReg->eCond.eDateR.eFrom == 0 && pReg->eCond.eDateR.eTo == 0 )
					PrintError( error::reg::kDayMonth, rError, iType, true );

				// 時間指定のチェック
				if(! CheckTimeCode(pReg->eCond.eTimeR.eFrom, pReg->eCond.eTimeR.eTo))
					PrintError( error::reg::kTime, rError, pReg->eCond.eTime, iType, true, true );

				// 月日指定のチェック
				if(! CheckDayCode(pReg->eCond.eDateR.eFrom, pReg->eCond.eDateR.eTo))
					PrintError( error::reg::kMonth, rError, pReg->eCond.eDate, iType, true, true );

				// 季節規制チェック
				if(bSeasonF)
				{
					// 通年規制はNG（時間が0:00～24:00はここでは見なくてOKでしょう）
					if(pReg->eCond.eDateR.eFrom == 0x0101 && pReg->eCond.eDateR.eTo == 0x1231)
						PrintError( error::reg::kSeasonYear, rError, pReg->eCond.eDate, iType, true, true );

					// 全曜日でない場合はNG
					if(pReg->eCond.eWeek != 15)
						PrintError( error::reg::kSeasonWeek, rError, pReg->eCond.eWeek, iType, true, true );

					// 0月規制である場合はNG
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

		// 各規制個別チェック
		switch(iType)
		{
		case 1: // リンク規制
			{
				// 規制種別のチェック
				if(pReg->eCond.eClass != 1) // 1 = リンク規制
					PrintError( error::reg::kClass, rError, iType, true );
				
				// 通行規制のチェック
				set<int>::const_iterator iter_link = rRegAttr.eLink.find(pReg->eCond.eLink);
				if( iter_link == rRegAttr.eLink.end() || pReg->eCond.eLink == 0 )
					PrintError( error::reg::kLink, rError, pReg->eCond.eLink, iType, true, false );
			}
			break;
		case 0: // ノード規制
			// 規制種別のチェック
			if(pReg->eCond.eClass != 2) // 2 = ノード規制
				PrintError( error::reg::kClass, rError, iType, true );

			// 通行規制のチェック
			if( pReg->eCond.eLink != 0 )
				PrintError( error::reg::kLink2, rError, iType, true );
			break;
		case 2:
			// 規制種別のチェック
			if(pReg->eCond.eClass != 3) // 3 = ノード列規制
				PrintError( error::reg::kClass, rError, iType, true );

			// 通行規制のチェック
			if( pReg->eCond.eLink != 0 )
				PrintError( error::reg::kLink2, rError, iType, true );
			break;
		default:
			cout << "Rest Type エラー" << endl;
			m_bIsError = true;
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り)
//////////////////////////////////////////////////////////////////////
void CCheckRegLayer::PrintError(int rKey, ErrorData &rError, int rCode, int rType, bool bError, bool bHex)
{
	char *aError = (bError) ? "ERROR\t" : "WARNING\t";
	CString cstrCode;

	// 16進数表示か10進数表示かの変換
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
		cout << "新交通規制のタイプが不正です。 "<< endl;
		break;
	}
	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値無し)
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
		cout << "新交通規制のタイプが不正です。 "<< endl;
		break;
	}
	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// 規制タイプを調査する
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
// ノード列の矛盾をチェック
//////////////////////////////////////////////////////////////////////
bool CCheckRegLayer::CheckNode(const INReg *rReg, int rMeshCode, int rType)
{
	typedef std::pair<int, int> NodeData;	//<2次メッシュ, ノードID>
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
		cout << "新交通規制のタイプの値が不正です。" << endl;
		m_bIsError = true;
		break;
	}
	
	return bFlag;
}

//////////////////////////////////////////////////////////////////////
// 時間指定チェック
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
// 月日指定チェック
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
