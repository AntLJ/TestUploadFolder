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
#include "CheckLinkLayer.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckLinkLayer::CCheckLinkLayer(void) : m_bIsError( false )
{
}

CCheckLinkLayer::~CCheckLinkLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// 道路リンクレイヤをチェック
//////////////////////////////////////////////////////////////////////
bool CCheckLinkLayer::check(CString &rFilePath, int rMeshCode, LinkAttrList& rLinkAttr, Customer eCustomer, const std::set<int>& setMesh )
{
	Z2LinkLoad aLinkLoad;
	if( !aLinkLoad.load(rFilePath, rMeshCode, 0x00300000) )
	{
		cout << rMeshCode << "\tERROR" << rLinkAttr.eError[error::link::kLayer] << endl;
		return false;
	}

	ErrorLink aError;
	aError.eError = rLinkAttr.eError;
	aError.eMesh = rMeshCode;
	for(int i = 0; i < (int)aLinkLoad.getCount(); ++i)
	{
		IN2Link* pLink = aLinkLoad[i];
		SetObjectID(aError.eFrom, pLink->eID1);
		SetObjectID(aError.eTo, pLink->eID2);

		// 道路管理者コードチェック
		set<int>::iterator iter_manage = rLinkAttr.eLinkInfo.eManager.find( pLink->eInfo.eManager );
		if( iter_manage == rLinkAttr.eLinkInfo.eManager.end() )
			PrintError( error::link::kManage, aError, pLink->eInfo.eManager );

		// リンク種別チェック
		set<int>::iterator iter_lattr = rLinkAttr.eLinkInfo.eLinkAttr.find( pLink->eInfo.eLinks );
		if( iter_lattr == rLinkAttr.eLinkInfo.eLinkAttr.end() )
			PrintError( error::link::kLinkAttr, aError, pLink->eInfo.eLinks );

		// バイパスフラグチェック
		set<int>::iterator iter_bypass = rLinkAttr.eLinkInfo.eBypass.find( pLink->eInfo.eBypass );
		if( iter_bypass == rLinkAttr.eLinkInfo.eBypass.end() )
			PrintError( error::link::kBypass, aError, pLink->eInfo.eBypass );

		// 自動車専用フラグチェック
		set<int>::iterator iter_auto = rLinkAttr.eLinkInfo.eAuto.find( pLink->eInfo.eAuto );
		if( iter_auto == rLinkAttr.eLinkInfo.eAuto.end() )
			PrintError( error::link::kAuto, aError, pLink->eInfo.eAuto );

		// 道路幅員チェック
		set<int>::iterator iter_width = rLinkAttr.eLinkInfo.eWidth.find( pLink->eInfo.eWidth );
		if( iter_width == rLinkAttr.eLinkInfo.eWidth.end() )
			PrintError( error::link::kWidth, aError, pLink->eInfo.eWidth );

		// 車線数チェック
		set<int>::iterator iter_lane = rLinkAttr.eLinkInfo.eLane.find( pLink->eInfo.eLane );
		if( iter_lane == rLinkAttr.eLinkInfo.eLane.end() )
			PrintError( error::link::kLaneCount, aError, pLink->eInfo.eRest );

		// 交通規制種別コードチェック(基準値以外が入っているかのチェック)
		set<int>::iterator iter_rest = rLinkAttr.eLinkInfo.eRest.find( pLink->eInfo.eRest );
		if( iter_rest == rLinkAttr.eLinkInfo.eRest.end() )
			PrintError( error::link::kRest, aError, pLink->eInfo.eRest );

		// 交通規制種別コードチェック(条件付き規制が入力されていないかのチェック)
		if( pLink->eInfo.eRest == in::road::rest_class::no_passage2 ||
			pLink->eInfo.eRest == in::road::rest_class::oneway_order2 ||
			pLink->eInfo.eRest == in::road::rest_class::oneway_reverse2 || 
			pLink->eInfo.eRest == in::road::rest_class::oneway_both )
			PrintError( error::link::kRest, aError, pLink->eInfo.eRest );

		// 交通規制条件種別コードチェック(条件付は未調査で入力されている)
		if( pLink->eInfo.eCond != 0 )
			PrintError( error::link::kCond, aError, pLink->eInfo.eCond );

		// 行政区域コードチェック
		CString  cstrAdmin;
		cstrAdmin.Format("%x", pLink->ePolitic );			// 10進→16進
		int iAdmin = _ttol(cstrAdmin)/1000;					// 使用しない領域の0を削除
		set<int>::iterator iter_admin = rLinkAttr.eAdminCode.find( iAdmin );
		if( iter_admin == rLinkAttr.eAdminCode.end() )
		{
			// フェリー航路で行政区域コード0のリンクはエラー対象外
			if( iAdmin != 0 || (pLink->eDispClass & 0x0000f000) != 0x0000f000)
				PrintHexError( error::link::kAdmin, aError, pLink->ePolitic );
		}

		// 表示種別コードチェック
		set<int>::iterator iter_disp = rLinkAttr.eDispClass.find( pLink->eDispClass );
		if( iter_disp == rLinkAttr.eDispClass.end() )
			PrintHexError( error::link::kDisp, aError, pLink->eDispClass );

		// 経路種別コードチェック
		int iNaviClass = pLink->eNaviClass & 0xfffffff0;
		set<int>::iterator iter_navi = rLinkAttr.eNaviClass.find( iNaviClass );
		if( iter_navi == rLinkAttr.eNaviClass.end() )
			PrintHexError( error::link::kNavi, aError, pLink->eNaviClass );

		// 広域フラグチェック
		CString cstrAreaFlagCode, cstrAreaFlag;
		cstrAreaFlagCode.Format("%x", pLink->eNaviClass );			// 10進→16進
		cstrAreaFlag = cstrAreaFlagCode.Right(1);
		int iAreaFlag = strtol(cstrAreaFlag, NULL, 16);
		set<int>::iterator iter_areaflag = rLinkAttr.eAreaFlag.find(iAreaFlag);
		if( iter_areaflag == rLinkAttr.eAreaFlag.end() )
			PrintHexError( error::link::kAreaFlag, aError, iAreaFlag );

		// 駐車場種別チェック
		set<int>::iterator iter_park = rLinkAttr.eParkClass.find( pLink->ePark.eAttr );
		if( iter_park == rLinkAttr.eParkClass.end() )
			PrintError( error::link::kParkClass, aError, pLink->ePark.eAttr );

		// 駐車場リンク種別チェック
		set<int>::iterator iter_parklink = rLinkAttr.eParkLinkClass.find( pLink->ePark.eLink );
		if( iter_parklink == rLinkAttr.eParkLinkClass.end() )
			PrintError( error::link::kParkLink, aError, pLink->ePark.eLink );

		// 道路番号チェック(フェリー航路についてはNewINtxtchkでチェック)
		if( pLink->eDispClass == in::road::road_class::kokudo || pLink->eDispClass == in::road::road_class::kokudo2 ||
			pLink->eDispClass == in::road::road_class::pref_mlocal || pLink->eDispClass == in::road::road_class::pref_mlocal2 ||
			pLink->eDispClass == in::road::road_class::city_mlocal || pLink->eDispClass == in::road::road_class::city_mlocal2 ||
			pLink->eDispClass == in::road::road_class::pref_road || pLink->eDispClass == in::road::road_class::pref_road2 )
		{
			// 国道の道路番号に矛盾が無いかをチェックする
			if( pLink->eDispClass == in::road::road_class::kokudo || pLink->eDispClass == in::road::road_class::kokudo2 )
			{
				if( pLink->eRoadNo < 0 || pLink->eRoadNo > 507 )
				{
					CString cstrDisp;
					SetObjectID(cstrDisp, pLink->eDispClass);
					cout << aError.eMesh << "\tERROR" << aError.eError[error::link::kRoadNo] << "\t";
					cout << aError.eFrom << "\t" << aError.eTo << "\t" << pLink->eRoadNo;
					cout << "\t" << static_cast<LPCSTR>(cstrDisp)<< endl;

					m_bIsError = true;
				}
			}
		}
		else if( pLink->eDispClass == in::road::road_class::ferry )
		{
			if( pLink->eRoadNo == 0 )
			{
				CString cstrDisp;
				SetObjectID(cstrDisp, pLink->eDispClass);
				cout << aError.eMesh << "\tERROR" << aError.eError[error::link::kRoadNo] << "\t";
				cout << aError.eFrom << "\t" << aError.eTo << "\t" << pLink->eRoadNo;
				cout << "\t" << static_cast<LPCSTR>(cstrDisp)<< endl;

				m_bIsError = true;
			}
		}
		else
		{
			if(pLink->eRoadNo != 0 )
			{
				CString cstrDisp;
				SetObjectID(cstrDisp, pLink->eDispClass);
				cout << aError.eMesh << "\tERROR" << aError.eError[error::link::kRoadNo] << "\t";
				cout << aError.eFrom << "\t" << aError.eTo << "\t" << pLink->eRoadNo;
				cout << "\t" << static_cast<LPCSTR>(cstrDisp)<< endl;

				m_bIsError = true;
			}
		}
		// 道路名称IDのチェックはNewINtxtchkで行なう。

		// VICSチェック
		if( pLink->eVics.eOrd < 0 || 4095 < pLink->eVics.eOrd ) // 順方向が1～4095以外
		{
			PrintError( error::link::kVicsOrd, aError, pLink->eVics.eOrd );
			CheckVics( setMesh, aError, *pLink, false );
		}
		else if( pLink->eVics.eOrd == 0 )	// 順方向が0のとき
			CheckVics( setMesh, aError, *pLink, false );
		else								// 順方向が1～4095のとき
			CheckVics( setMesh, aError, *pLink, true );

		set<NodeXY> XYSet;
		for( int iCount = 0; iCount < pLink->ePointCount; ++ iCount )
		{
			if( eCustomer == PEC )
			{
				// 属性種別コード(傾斜フラグ)チェック
				if( ( (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 ) == 0x0 || ( (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 ) == 0x3 )
				{
					// 傾斜フラグが0,3(未調査、平坦)の場合は傾斜角度も0でないとダメ
					// 属性種別コード(傾斜角度)チェック
					if( (pLink->eShapePoint[iCount].eAttr&0xff000000) != 0x0 )
					{
						PrintError( error::link::kGradFlag, aError, (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 );
						PrintHexError( error::link::kGradAngle, aError, pLink->eShapePoint[iCount].eAttr&0xff000000 );
					}
				}
				else
				{
					// 上記と同様にフラグが1,2の場合は傾斜角度が0以外でないとダメ
					if( (pLink->eShapePoint[iCount].eAttr&0xff000000) == 0x0 )
					{
						PrintError( error::link::kGradFlag, aError, (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 );
						PrintHexError( error::link::kGradAngle, aError, pLink->eShapePoint[iCount].eAttr&0xff000000 );
					}
				}
			}
			else
			{
				// 仕向け先が「PEC」以外の場合は傾斜フラグ、角度の値は0でないとダメ
				// 属性種別コード(傾斜フラグ)チェック
				if( (pLink->eShapePoint[iCount].eAttr&0x00c00000) != 0x0 )
					PrintError( error::link::kGradFlag2, aError, (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 );

				// 属性種別コード(傾斜角度)チェック
				if( (pLink->eShapePoint[iCount].eAttr&0xff000000) != 0x0 )
					PrintHexError( error::link::kGradAngle2, aError, pLink->eShapePoint[iCount].eAttr&0xff000000 );
			}

			// 属性種別コード(ReservedC)チェック
			if( (pLink->eShapePoint[iCount].eAttr&0x003f0000) != 0x0 )
				PrintHexError( error::link::kReservedC, aError, pLink->eShapePoint[iCount].eAttr&0x003f0000 );

			if( (pLink->eShapePoint[iCount].eAttr&0x0000f000) != 0x0 )
			{
#if IN_VERSION >= 590
				// 属性種別コード(ReservedD)チェック
				PrintHexError( error::link::kReservedD, aError, (pLink->eShapePoint[iCount].eAttr&0x0000f000) );
#else
				// 属性種別コード(地面からの相対高)チェック
				PrintHexError( error::link::kCrossHigh, aError, pLink->eShapePoint[iCount].eAttr&0x0000f000 );
#endif
			}

			// 属性種別コード(ReservedE)チェック（旧上下関係チェック）
			if( (pLink->eShapePoint[iCount].eAttr&0x00000f00) != 0x0 )
				PrintHexError( error::link::kReservedE, aError, pLink->eShapePoint[iCount].eAttr&0x00000f00 );

			// 属性種別コード(リンク内属性)チェック
#if IN_VERSION >= 550
			if( ((pLink->eShapePoint[iCount].eAttr&0x000000ff)&0x00000040) != 0x0 )
				PrintHexError( error::link::kAttr, aError, pLink->eShapePoint[iCount].eAttr&0x000000ff );
#endif
			// 上下関係チェック（属性種別コード(ReservedE)チェックに統合）
			/*CString cstrUp;
			cstrUp.Format("%x",(pLink->eShapePoint[iCount].eAttr&0x00000f00) >> 8 );
			int iUpDown = _ttol(cstrUp);
			set<int>::iterator iter_updown = rLinkAttr.eUpDown.find( iUpDown );
			if (iUpDown != 0 )
				PrintError( error::link::kUpDown, aError,  iUpDown );*/

			NodeXY aXY(pLink->eShapePoint[iCount].eX, pLink->eShapePoint[iCount].eY);
			set<NodeXY>::const_iterator iter_xy = XYSet.find(aXY);
			// 補完点重複チェック
			if( iCount > 0 && iter_xy != XYSet.end() )
			{
				cout << aError.eMesh << "\tERROR" << aError.eError[error::link::kPoints] << "\t";
				cout << static_cast<LPCTSTR>(aError.eFrom) << "\t";
				cout << static_cast<LPCTSTR>(aError.eTo) << "\t" << pLink->eShapePoint[iCount].eX;
				cout << "\t" << pLink->eShapePoint[iCount].eY << "\t" << iCount << endl;

				m_bIsError = true;
			}
			else
				XYSet.insert(aXY);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(コード値:16進数)
//////////////////////////////////////////////////////////////////////
void CCheckLinkLayer::PrintHexError(int rKey, ErrorLink &rError, int rCode)
{
	CString cstrCode;
	SetObjectID(cstrCode, rCode);
	cout << rError.eMesh << "\tERROR" << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eFrom) << "\t";
	cout << static_cast<LPCTSTR>(rError.eTo) << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(コード値:10進数)
//////////////////////////////////////////////////////////////////////
void CCheckLinkLayer::PrintError(int rKey, ErrorLink &rError, int rCode)
{
	cout << rError.eMesh << "\tERROR" << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eFrom) << "\t";
	cout << static_cast<LPCTSTR>(rError.eTo) << "\t" << rCode << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// VICS情報チェック
//////////////////////////////////////////////////////////////////////
void CCheckLinkLayer::CheckVics( const std::set<int>& setMesh, ErrorLink& aError, IN2Link& pLink, bool bVicsOrdRev )
{
	// 逆方向が1～4095以外
	if( pLink.eVics.eRev < 0 || 4095 < pLink.eVics.eRev )
	{
		PrintError( error::link::kVicsRev, aError, pLink.eVics.eRev );
		CheckVicsPartMesh( setMesh, aError, pLink, bVicsOrdRev );
	}
	// 逆方向が0
	else if( pLink.eVics.eRev == 0 )
		CheckVicsPartMesh( setMesh, aError, pLink, bVicsOrdRev );
	// 逆方向が1～4095
	else
		CheckVicsPartMesh( setMesh, aError, pLink, true );
}
void CCheckLinkLayer::CheckVicsPartMesh( const std::set<int>& setMesh, ErrorLink& aError, IN2Link& pLink, bool bVicsOrdRev )
{
	
	if( bVicsOrdRev )	// 順or逆VICS番号が付与されている
	{
		// 区分は1,2,3,4のみOK
		if( pLink.eVics.ePart < 0 || 4 < pLink.eVics.ePart )
			PrintError( error::link::kVicsPart, aError, pLink.eVics.ePart );
		// mnt,rdgファイルが存在するメッシュであればOKとする
		// chkRoad.exeは仕様外の値があるかをチェックするツールなのでVICSの繋がりまでは見ていない
		if( setMesh.end() == setMesh.find(pLink.eVics.eMesh) )
			PrintError( error::link::kVicsMesh, aError, pLink.eVics.eMesh );
	}
	else	// 順or逆VICS番号が付与されていない
	{
		// 区分、メッシュともに0であること
		if( pLink.eVics.ePart != 0 )
			PrintError( error::link::kVicsPart2, aError, pLink.eVics.ePart );
		if( pLink.eVics.eMesh != 0 )
			PrintError( error::link::kVicsMesh2, aError, pLink.eVics.eMesh );
	}
}
