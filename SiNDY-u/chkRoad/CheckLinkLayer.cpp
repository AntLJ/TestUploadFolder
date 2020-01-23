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
// ���H�����N���C�����`�F�b�N
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

		// ���H�Ǘ��҃R�[�h�`�F�b�N
		set<int>::iterator iter_manage = rLinkAttr.eLinkInfo.eManager.find( pLink->eInfo.eManager );
		if( iter_manage == rLinkAttr.eLinkInfo.eManager.end() )
			PrintError( error::link::kManage, aError, pLink->eInfo.eManager );

		// �����N��ʃ`�F�b�N
		set<int>::iterator iter_lattr = rLinkAttr.eLinkInfo.eLinkAttr.find( pLink->eInfo.eLinks );
		if( iter_lattr == rLinkAttr.eLinkInfo.eLinkAttr.end() )
			PrintError( error::link::kLinkAttr, aError, pLink->eInfo.eLinks );

		// �o�C�p�X�t���O�`�F�b�N
		set<int>::iterator iter_bypass = rLinkAttr.eLinkInfo.eBypass.find( pLink->eInfo.eBypass );
		if( iter_bypass == rLinkAttr.eLinkInfo.eBypass.end() )
			PrintError( error::link::kBypass, aError, pLink->eInfo.eBypass );

		// �����Ԑ�p�t���O�`�F�b�N
		set<int>::iterator iter_auto = rLinkAttr.eLinkInfo.eAuto.find( pLink->eInfo.eAuto );
		if( iter_auto == rLinkAttr.eLinkInfo.eAuto.end() )
			PrintError( error::link::kAuto, aError, pLink->eInfo.eAuto );

		// ���H�����`�F�b�N
		set<int>::iterator iter_width = rLinkAttr.eLinkInfo.eWidth.find( pLink->eInfo.eWidth );
		if( iter_width == rLinkAttr.eLinkInfo.eWidth.end() )
			PrintError( error::link::kWidth, aError, pLink->eInfo.eWidth );

		// �Ԑ����`�F�b�N
		set<int>::iterator iter_lane = rLinkAttr.eLinkInfo.eLane.find( pLink->eInfo.eLane );
		if( iter_lane == rLinkAttr.eLinkInfo.eLane.end() )
			PrintError( error::link::kLaneCount, aError, pLink->eInfo.eRest );

		// ��ʋK����ʃR�[�h�`�F�b�N(��l�ȊO�������Ă��邩�̃`�F�b�N)
		set<int>::iterator iter_rest = rLinkAttr.eLinkInfo.eRest.find( pLink->eInfo.eRest );
		if( iter_rest == rLinkAttr.eLinkInfo.eRest.end() )
			PrintError( error::link::kRest, aError, pLink->eInfo.eRest );

		// ��ʋK����ʃR�[�h�`�F�b�N(�����t���K�������͂���Ă��Ȃ����̃`�F�b�N)
		if( pLink->eInfo.eRest == in::road::rest_class::no_passage2 ||
			pLink->eInfo.eRest == in::road::rest_class::oneway_order2 ||
			pLink->eInfo.eRest == in::road::rest_class::oneway_reverse2 || 
			pLink->eInfo.eRest == in::road::rest_class::oneway_both )
			PrintError( error::link::kRest, aError, pLink->eInfo.eRest );

		// ��ʋK��������ʃR�[�h�`�F�b�N(�����t�͖������œ��͂���Ă���)
		if( pLink->eInfo.eCond != 0 )
			PrintError( error::link::kCond, aError, pLink->eInfo.eCond );

		// �s�����R�[�h�`�F�b�N
		CString  cstrAdmin;
		cstrAdmin.Format("%x", pLink->ePolitic );			// 10�i��16�i
		int iAdmin = _ttol(cstrAdmin)/1000;					// �g�p���Ȃ��̈��0���폜
		set<int>::iterator iter_admin = rLinkAttr.eAdminCode.find( iAdmin );
		if( iter_admin == rLinkAttr.eAdminCode.end() )
		{
			// �t�F���[�q�H�ōs�����R�[�h0�̃����N�̓G���[�ΏۊO
			if( iAdmin != 0 || (pLink->eDispClass & 0x0000f000) != 0x0000f000)
				PrintHexError( error::link::kAdmin, aError, pLink->ePolitic );
		}

		// �\����ʃR�[�h�`�F�b�N
		set<int>::iterator iter_disp = rLinkAttr.eDispClass.find( pLink->eDispClass );
		if( iter_disp == rLinkAttr.eDispClass.end() )
			PrintHexError( error::link::kDisp, aError, pLink->eDispClass );

		// �o�H��ʃR�[�h�`�F�b�N
		int iNaviClass = pLink->eNaviClass & 0xfffffff0;
		set<int>::iterator iter_navi = rLinkAttr.eNaviClass.find( iNaviClass );
		if( iter_navi == rLinkAttr.eNaviClass.end() )
			PrintHexError( error::link::kNavi, aError, pLink->eNaviClass );

		// �L��t���O�`�F�b�N
		CString cstrAreaFlagCode, cstrAreaFlag;
		cstrAreaFlagCode.Format("%x", pLink->eNaviClass );			// 10�i��16�i
		cstrAreaFlag = cstrAreaFlagCode.Right(1);
		int iAreaFlag = strtol(cstrAreaFlag, NULL, 16);
		set<int>::iterator iter_areaflag = rLinkAttr.eAreaFlag.find(iAreaFlag);
		if( iter_areaflag == rLinkAttr.eAreaFlag.end() )
			PrintHexError( error::link::kAreaFlag, aError, iAreaFlag );

		// ���ԏ��ʃ`�F�b�N
		set<int>::iterator iter_park = rLinkAttr.eParkClass.find( pLink->ePark.eAttr );
		if( iter_park == rLinkAttr.eParkClass.end() )
			PrintError( error::link::kParkClass, aError, pLink->ePark.eAttr );

		// ���ԏꃊ���N��ʃ`�F�b�N
		set<int>::iterator iter_parklink = rLinkAttr.eParkLinkClass.find( pLink->ePark.eLink );
		if( iter_parklink == rLinkAttr.eParkLinkClass.end() )
			PrintError( error::link::kParkLink, aError, pLink->ePark.eLink );

		// ���H�ԍ��`�F�b�N(�t�F���[�q�H�ɂ��Ă�NewINtxtchk�Ń`�F�b�N)
		if( pLink->eDispClass == in::road::road_class::kokudo || pLink->eDispClass == in::road::road_class::kokudo2 ||
			pLink->eDispClass == in::road::road_class::pref_mlocal || pLink->eDispClass == in::road::road_class::pref_mlocal2 ||
			pLink->eDispClass == in::road::road_class::city_mlocal || pLink->eDispClass == in::road::road_class::city_mlocal2 ||
			pLink->eDispClass == in::road::road_class::pref_road || pLink->eDispClass == in::road::road_class::pref_road2 )
		{
			// �����̓��H�ԍ��ɖ��������������`�F�b�N����
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
		// ���H����ID�̃`�F�b�N��NewINtxtchk�ōs�Ȃ��B

		// VICS�`�F�b�N
		if( pLink->eVics.eOrd < 0 || 4095 < pLink->eVics.eOrd ) // ��������1�`4095�ȊO
		{
			PrintError( error::link::kVicsOrd, aError, pLink->eVics.eOrd );
			CheckVics( setMesh, aError, *pLink, false );
		}
		else if( pLink->eVics.eOrd == 0 )	// ��������0�̂Ƃ�
			CheckVics( setMesh, aError, *pLink, false );
		else								// ��������1�`4095�̂Ƃ�
			CheckVics( setMesh, aError, *pLink, true );

		set<NodeXY> XYSet;
		for( int iCount = 0; iCount < pLink->ePointCount; ++ iCount )
		{
			if( eCustomer == PEC )
			{
				// ������ʃR�[�h(�X�΃t���O)�`�F�b�N
				if( ( (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 ) == 0x0 || ( (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 ) == 0x3 )
				{
					// �X�΃t���O��0,3(�������A���R)�̏ꍇ�͌X�Ίp�x��0�łȂ��ƃ_��
					// ������ʃR�[�h(�X�Ίp�x)�`�F�b�N
					if( (pLink->eShapePoint[iCount].eAttr&0xff000000) != 0x0 )
					{
						PrintError( error::link::kGradFlag, aError, (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 );
						PrintHexError( error::link::kGradAngle, aError, pLink->eShapePoint[iCount].eAttr&0xff000000 );
					}
				}
				else
				{
					// ��L�Ɠ��l�Ƀt���O��1,2�̏ꍇ�͌X�Ίp�x��0�ȊO�łȂ��ƃ_��
					if( (pLink->eShapePoint[iCount].eAttr&0xff000000) == 0x0 )
					{
						PrintError( error::link::kGradFlag, aError, (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 );
						PrintHexError( error::link::kGradAngle, aError, pLink->eShapePoint[iCount].eAttr&0xff000000 );
					}
				}
			}
			else
			{
				// �d�����悪�uPEC�v�ȊO�̏ꍇ�͌X�΃t���O�A�p�x�̒l��0�łȂ��ƃ_��
				// ������ʃR�[�h(�X�΃t���O)�`�F�b�N
				if( (pLink->eShapePoint[iCount].eAttr&0x00c00000) != 0x0 )
					PrintError( error::link::kGradFlag2, aError, (pLink->eShapePoint[iCount].eAttr&0x00c00000) >> 22 );

				// ������ʃR�[�h(�X�Ίp�x)�`�F�b�N
				if( (pLink->eShapePoint[iCount].eAttr&0xff000000) != 0x0 )
					PrintHexError( error::link::kGradAngle2, aError, pLink->eShapePoint[iCount].eAttr&0xff000000 );
			}

			// ������ʃR�[�h(ReservedC)�`�F�b�N
			if( (pLink->eShapePoint[iCount].eAttr&0x003f0000) != 0x0 )
				PrintHexError( error::link::kReservedC, aError, pLink->eShapePoint[iCount].eAttr&0x003f0000 );

			if( (pLink->eShapePoint[iCount].eAttr&0x0000f000) != 0x0 )
			{
#if IN_VERSION >= 590
				// ������ʃR�[�h(ReservedD)�`�F�b�N
				PrintHexError( error::link::kReservedD, aError, (pLink->eShapePoint[iCount].eAttr&0x0000f000) );
#else
				// ������ʃR�[�h(�n�ʂ���̑��΍�)�`�F�b�N
				PrintHexError( error::link::kCrossHigh, aError, pLink->eShapePoint[iCount].eAttr&0x0000f000 );
#endif
			}

			// ������ʃR�[�h(ReservedE)�`�F�b�N�i���㉺�֌W�`�F�b�N�j
			if( (pLink->eShapePoint[iCount].eAttr&0x00000f00) != 0x0 )
				PrintHexError( error::link::kReservedE, aError, pLink->eShapePoint[iCount].eAttr&0x00000f00 );

			// ������ʃR�[�h(�����N������)�`�F�b�N
#if IN_VERSION >= 550
			if( ((pLink->eShapePoint[iCount].eAttr&0x000000ff)&0x00000040) != 0x0 )
				PrintHexError( error::link::kAttr, aError, pLink->eShapePoint[iCount].eAttr&0x000000ff );
#endif
			// �㉺�֌W�`�F�b�N�i������ʃR�[�h(ReservedE)�`�F�b�N�ɓ����j
			/*CString cstrUp;
			cstrUp.Format("%x",(pLink->eShapePoint[iCount].eAttr&0x00000f00) >> 8 );
			int iUpDown = _ttol(cstrUp);
			set<int>::iterator iter_updown = rLinkAttr.eUpDown.find( iUpDown );
			if (iUpDown != 0 )
				PrintError( error::link::kUpDown, aError,  iUpDown );*/

			NodeXY aXY(pLink->eShapePoint[iCount].eX, pLink->eShapePoint[iCount].eY);
			set<NodeXY>::const_iterator iter_xy = XYSet.find(aXY);
			// �⊮�_�d���`�F�b�N
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
// �G���[�o��(�R�[�h�l:16�i��)
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
// �G���[�o��(�R�[�h�l:10�i��)
//////////////////////////////////////////////////////////////////////
void CCheckLinkLayer::PrintError(int rKey, ErrorLink &rError, int rCode)
{
	cout << rError.eMesh << "\tERROR" << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eFrom) << "\t";
	cout << static_cast<LPCTSTR>(rError.eTo) << "\t" << rCode << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// VICS���`�F�b�N
//////////////////////////////////////////////////////////////////////
void CCheckLinkLayer::CheckVics( const std::set<int>& setMesh, ErrorLink& aError, IN2Link& pLink, bool bVicsOrdRev )
{
	// �t������1�`4095�ȊO
	if( pLink.eVics.eRev < 0 || 4095 < pLink.eVics.eRev )
	{
		PrintError( error::link::kVicsRev, aError, pLink.eVics.eRev );
		CheckVicsPartMesh( setMesh, aError, pLink, bVicsOrdRev );
	}
	// �t������0
	else if( pLink.eVics.eRev == 0 )
		CheckVicsPartMesh( setMesh, aError, pLink, bVicsOrdRev );
	// �t������1�`4095
	else
		CheckVicsPartMesh( setMesh, aError, pLink, true );
}
void CCheckLinkLayer::CheckVicsPartMesh( const std::set<int>& setMesh, ErrorLink& aError, IN2Link& pLink, bool bVicsOrdRev )
{
	
	if( bVicsOrdRev )	// ��or�tVICS�ԍ����t�^����Ă���
	{
		// �敪��1,2,3,4�̂�OK
		if( pLink.eVics.ePart < 0 || 4 < pLink.eVics.ePart )
			PrintError( error::link::kVicsPart, aError, pLink.eVics.ePart );
		// mnt,rdg�t�@�C�������݂��郁�b�V���ł����OK�Ƃ���
		// chkRoad.exe�͎d�l�O�̒l�����邩���`�F�b�N����c�[���Ȃ̂�VICS�̌q����܂ł͌��Ă��Ȃ�
		if( setMesh.end() == setMesh.find(pLink.eVics.eMesh) )
			PrintError( error::link::kVicsMesh, aError, pLink.eVics.eMesh );
	}
	else	// ��or�tVICS�ԍ����t�^����Ă��Ȃ�
	{
		// �敪�A���b�V���Ƃ���0�ł��邱��
		if( pLink.eVics.ePart != 0 )
			PrintError( error::link::kVicsPart2, aError, pLink.eVics.ePart );
		if( pLink.eVics.eMesh != 0 )
			PrintError( error::link::kVicsMesh2, aError, pLink.eVics.eMesh );
	}
}
