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

// LoadQueueInfo.cpp: CLoadQueueInfo �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadQueueInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

std::map<CString, INT> CLoadQueueInfo::m_mapInfoTableName;
std::map<CString, INT> CLoadQueueInfo::m_mapAliasName;
std::map<CString, INT> CLoadQueueInfo::m_mapLQTableName;
std::map<CString, INT> CLoadQueueInfo::m_mapLQRFRoadFieldName;
//std::map<CString, INT> CLoadQueueInfo::m_mapRelInfoLQFieldName;
std::map<LONG, INT>    CLoadQueueInfo::m_mapDrawMode;
std::map<LONG, sindyeRoadQueueType>    CLoadQueueInfo::m_mapCheckType;
std::map<DWORD, INT>   CLoadQueueInfo::m_mapColor;

std::map< LONG, std::map< LONG, std::map<long,sindyeCheckConditionOperation> > >		CLoadQueueInfo::m_mapCheckOperation;
std::map< LONG, std::map< LONG, std::map<long,sindyeCheckConditionAction> > >		CLoadQueueInfo::m_mapCheckAction;
std::map< LONG, sindyeCheckErrorType >	CLoadQueueInfo::m_mapCheckError;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CLoadQueueInfo::CLoadQueueInfo()
{
	m_mapInfoTableName.clear();
	m_mapAliasName.clear();
	m_mapLQTableName.clear();
	m_mapLQRFRoadFieldName.clear();
	m_mapDrawMode.clear();
	m_mapColor.clear();
	m_mapCheckType.clear();

	m_mapCheckOperation.clear();
	m_mapCheckAction.clear();
	m_mapCheckError.clear();
	SetData();
}

CLoadQueueInfo::~CLoadQueueInfo()
{
}

void CLoadQueueInfo::SetData()
{
	m_mapInfoTableName[_T("INF_BYWAY")]			= sindyeRoadQueueByway;		// ������
	m_mapInfoTableName[_T("INF_DIRGUIDE")]		= sindyeRoadQueueDirguide;	// ���ʈē�
	m_mapInfoTableName[_T("INF_GUIDE")]			= sindyeRoadQueueGuide;		// �����U��
	m_mapInfoTableName[_T("INF_LANE")]			= sindyeRoadQueueLane;		// ���[�����
	m_mapInfoTableName[_T("INF_VICS")]			= sindyeRoadQueueVics;		// VICS
	m_mapInfoTableName[_T("INF_TURNREG")]		= sindyeRoadQueueTurnreg;	// �E���܋֎~
	m_mapInfoTableName[_T("INF_ROUTE")]			= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p����
	m_mapInfoTableName[_T("INF_UTURN")]			= sindyeRoadQueueUTurn;		// U�^�[��
	m_mapInfoTableName[_T("INF_INTERNAVI")]		= sindyeRoadQueueInternavi;	// �C���^�[�i�rVICS
	m_mapInfoTableName[_T("INF_IPCVICS")]		= sindyeRoadQueueIPCVics;	// iPCVICS�ibug 6477�j
	m_mapInfoTableName[_T("INF_INTERSECTION")]	= sindyeRoadQueueIntersection;	// �i�������ʌ����_

#ifndef SINDY_FOR_ENGLISH
	m_mapAliasName[_T("������")]				= sindyeRoadQueueByway;		// ������
	m_mapAliasName[_T("���ʈē�")]				= sindyeRoadQueueDirguide;	// ���ʈē�
	m_mapAliasName[_T("�����U��")]				= sindyeRoadQueueGuide;		// �����U��
	m_mapAliasName[_T("���[�����")]			= sindyeRoadQueueLane;		// ���[�����
	m_mapAliasName[_T("VICS")]					= sindyeRoadQueueVics;		// VICS
	m_mapAliasName[_T("�E���܋֎~")]			= sindyeRoadQueueTurnreg;	// �E���܋֎~
	m_mapAliasName[_T("�A�N�Z�X�^���p����")]	= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p����
	m_mapAliasName[_T("U�^�[��")]				= sindyeRoadQueueUTurn;		// U�^�[��
	m_mapAliasName[_T("�C���^�[�i�rVICS")]		= sindyeRoadQueueInternavi;	// �C���^�[�i�rVICS
	m_mapAliasName[_T("iPCVICS")]				= sindyeRoadQueueIPCVics;	// iPCVICS�ibug 6477�j
	m_mapAliasName[_T("�i�������ʌ����_")]		= sindyeRoadQueueIntersection;	// �i�������ʌ����_
#else
	m_mapAliasName[_T("Byway")]					= sindyeRoadQueueByway;		// ������
	m_mapAliasName[_T("Signpost")]				= sindyeRoadQueueDirguide;	// ���ʈē�
	m_mapAliasName[_T("Forced guiding")]		= sindyeRoadQueueGuide;		// �����U��
	m_mapAliasName[_T("Lane")]					= sindyeRoadQueueLane;		// ���[�����
	m_mapAliasName[_T("VICS")]					= sindyeRoadQueueVics;		// VICS
	m_mapAliasName[_T("Turn Regulation")]		= sindyeRoadQueueTurnreg;	// �E���܋֎~
	m_mapAliasName[_T("Access/connection road")]= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p����
	m_mapAliasName[_T("U turn")]				= sindyeRoadQueueUTurn;		// U�^�[��
	m_mapAliasName[_T("Internavi VICS")]		= sindyeRoadQueueInternavi;	// �C���^�[�i�rVICS
	m_mapAliasName[_T("iPCVICS")]				= sindyeRoadQueueIPCVics;	// iPCVICS�ibug 6477�j
	m_mapAliasName[_T("Direction dependent intersection")]		= sindyeRoadQueueIntersection;	// �i�������ʌ����_
#endif	// SINDY_FOR_ENGLISH

	m_mapLQTableName[_T("LQ_BYWAY")]			= sindyeRoadQueueByway;		// ������
	m_mapLQTableName[_T("LQ_DIRGUIDE")]			= sindyeRoadQueueDirguide;	// ���ʈē�
	m_mapLQTableName[_T("LQ_GUIDE")]			= sindyeRoadQueueGuide;		// �����U��
	m_mapLQTableName[_T("LQ_LANE")]				= sindyeRoadQueueLane;		// ���[�����
	m_mapLQTableName[_T("LQ_VICS")]				= sindyeRoadQueueVics;		// VICS
	m_mapLQTableName[_T("LQ_TURNREG")]			= sindyeRoadQueueTurnreg;	// �E���܋֎~
	m_mapLQTableName[_T("LQ_ROUTE")]			= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p����
	m_mapLQTableName[_T("LQ_UTURN")]			= sindyeRoadQueueUTurn;		// U�^�[��
	m_mapLQTableName[_T("LQ_INTERNAVI")]		= sindyeRoadQueueInternavi;	// �C���^�[�i�rVICS
	m_mapLQTableName[_T("LQ_IPCVICS")]			= sindyeRoadQueueIPCVics;	// iPCVICS�ibug 6477�j
	m_mapLQTableName[_T("LQ_INTERSECTION")]		= sindyeRoadQueueIntersection;	// �i�������ʌ����_

	m_mapLQRFRoadFieldName[_T("BYWAY_LQRF")]	= sindyeRoadQueueByway;		// ������
	m_mapLQRFRoadFieldName[_T("DIRGUIDE_LQRF")] = sindyeRoadQueueDirguide;	// ���ʈē�
	m_mapLQRFRoadFieldName[_T("GUIDE_LQRF")]	= sindyeRoadQueueGuide;		// �����U��
	m_mapLQRFRoadFieldName[_T("LANE_LQRF")]		= sindyeRoadQueueLane;		// ���[�����
	m_mapLQRFRoadFieldName[_T("VICS_LQRF")]		= sindyeRoadQueueVics;		// VICS
	m_mapLQRFRoadFieldName[_T("TURNREG_LQRF")]	= sindyeRoadQueueTurnreg;	// �E���܋֎~
	m_mapLQRFRoadFieldName[_T("ROUTE_LQRF")]	= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p����
	m_mapLQRFRoadFieldName[_T("UTURN_LQRF")]	= sindyeRoadQueueUTurn;		// U�^�[��
	m_mapLQRFRoadFieldName[_T("INTERNAVI_LQRF")]= sindyeRoadQueueInternavi;	// �C���^�[�i�rVICS
	m_mapLQRFRoadFieldName[_T("IPCVICS_LQRF")]	= sindyeRoadQueueIPCVics;	// iPCVICS�ibug 6477�j
	m_mapLQRFRoadFieldName[_T("INTERSECTION_LQRF")]	= sindyeRoadQueueIntersection;	// �i�������ʌ����_

/*	m_mapRelInfoLQFieldName[_T("BYWAY_ID")]		= sindyeRoadQueueByway;		// ������
	m_mapRelInfoLQFieldName[_T("DIRGUIDE_ID")]	= sindyeRoadQueueDirguide;	// ���ʈē�
	m_mapRelInfoLQFieldName[_T("GUIDE_ID")]		= sindyeRoadQueueGuide;		// �����U��
	m_mapRelInfoLQFieldName[_T("LANE_ID")]		= sindyeRoadQueueLane;		// ���[�����
	m_mapRelInfoLQFieldName[_T("VICS_ID")]		= sindyeRoadQueueVics;		// VICS
	m_mapRelInfoLQFieldName[_T("TURNREG_ID")]	= sindyeRoadQueueTurnreg;	// �E���܋֎~
	m_mapRelInfoLQFieldName[_T("ROUTE_ID")]		= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p����
	m_mapRelInfoLQFieldName[_T("UTURN_ID")]		= sindyeRoadQueueUTurn;		// U�^�[��
*/
	m_mapDrawMode[_MULTISYMBOL_BYWAY]			= sindyeRoadQueueByway;		// ������
	m_mapDrawMode[_MULTISYMBOL_DIRGUIDE]		= sindyeRoadQueueDirguide;	// ���ʈē�
	m_mapDrawMode[_MULTISYMBOL_GUIDE]			= sindyeRoadQueueGuide;		// �����U��
	m_mapDrawMode[_MULTISYMBOL_LANE]			= sindyeRoadQueueLane;		// ���[�����
	m_mapDrawMode[_MULTISYMBOL_VICS]			= sindyeRoadQueueVics;		// VICS
	m_mapDrawMode[_MULTISYMBOL_NOLRTURN]		= sindyeRoadQueueTurnreg;	// �E���܋֎~
	m_mapDrawMode[_MULTISYMBOL_ROUTE]			= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p����
	m_mapDrawMode[_MULTISYMBOL_UTURN]			= sindyeRoadQueueUTurn;		// U�^�[��
	m_mapDrawMode[_MULTISYMBOL_INTERNAVI]		= sindyeRoadQueueInternavi;	// �C���^�[�i�rVICS
	m_mapDrawMode[_MULTISYMBOL_IPCVICS]			= sindyeRoadQueueIPCVics;	// iPCVICS
	m_mapDrawMode[_MULTISYMBOL_INTERSECTION]	= sindyeRoadQueueIntersection;	// �i�������ʌ����_

	m_mapCheckType[sindyeCheckOneway]			= (sindyeRoadQueueType)-1;	// ����ʍs
	m_mapCheckType[sindyeCheckNoPassage]		= (sindyeRoadQueueType)-1;	// �ʍs�֎~
	m_mapCheckType[sindyeCheckNoPassage_AP]		= (sindyeRoadQueueType)-1;	// �ʍs�֎~�i�֌W�҈ȊO�j
	m_mapCheckType[sindyeCheckNoPassage_NG]		= (sindyeRoadQueueType)-1;	// �ʍs�֎~�i�ʍs�s�j
	m_mapCheckType[sindyeByway]					= sindyeRoadQueueByway;		// ������
	m_mapCheckType[sindyeDirguide]				= sindyeRoadQueueDirguide;	// ���ʈē�
	m_mapCheckType[sindyeGuide]					= sindyeRoadQueueGuide;		// �����U��
	m_mapCheckType[sindyeLane]					= sindyeRoadQueueLane;		// ���[�����
	m_mapCheckType[sindyeVics]					= sindyeRoadQueueVics;		// VICS
	m_mapCheckType[sindyeTurnreg]				= sindyeRoadQueueTurnreg;	// �E���܋֎~�i�����Ȃ��j
	m_mapCheckType[sindyeTurnreg_CN]			= sindyeRoadQueueTurnreg;	// �E���܋֎~�i��������j
	m_mapCheckType[sindyeRoute]					= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p�����i���p�����ȊO�j
	m_mapCheckType[sindyeRoute_TR]				= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p�����i���p�����j
	m_mapCheckType[sindyeUTurn]					= sindyeRoadQueueUTurn;		// U�^�[��
	m_mapCheckType[sindyeInternavi]				= sindyeRoadQueueInternavi;	// �C���^�[�i�rVICS
	m_mapCheckType[sindyeIPCVics]				= sindyeRoadQueueIPCVics;	// iPCVICS
	m_mapCheckType[sindyeIntersection]			= sindyeRoadQueueIntersection;	// �i�������ʌ����_

	m_mapColor[RGB( 0x00, 0xC8, 0xFA )]			= sindyeRoadQueueByway;		// �������i���F�j
	m_mapColor[RGB( 0x00, 0xFF, 0x00 )]			= sindyeRoadQueueDirguide;	// ���ʈē��i�΁j
	m_mapColor[RGB( 0xFF, 0x00, 0x00 )]			= sindyeRoadQueueGuide;		// �����U���i�ԁj
	m_mapColor[RGB( 0x00, 0x00, 0xFF )]			= sindyeRoadQueueLane;		// ���[�����i�j
	m_mapColor[RGB( 0xFF, 0xFF, 0x00 )]			= sindyeRoadQueueVics;		// VICS�i���F�j
	m_mapColor[RGB( 0xFF, 0x00, 0xFF )]			= sindyeRoadQueueTurnreg;	// �E���܋֎~�i�s���N�j
	m_mapColor[RGB( 0x7f, 0xFF, 0x7f )]			= sindyeRoadQueueRoute;		// �A�N�Z�X�^���p�����i���ΐF�j
	m_mapColor[RGB( 0x7f, 0x00, 0x7f )]			= sindyeRoadQueueUTurn;		// U�^�[���i���j
	m_mapColor[RGB( 0x00, 0xFF, 0xFF )]			= sindyeRoadQueueInternavi;	// �C���^�[�i�rVICS
	m_mapColor[RGB( 0x80, 0x40, 0x00 )]			= sindyeRoadQueueIPCVics;	// iPCVICS
	m_mapColor[RGB( 0x80, 0x40, 0x80 )]			= sindyeRoadQueueIntersection;	// �i�������ʌ����_

	m_mapCheckOperation.clear();
	m_mapCheckAction.clear();
	m_mapCheckError.clear();

	for ( LONG iCnt = sindyeLoadQueueStart; iCnt <= sindyeLoadQueueEnd; iCnt++ ) {
		// ����ʍs
		std::map<long,sindyeCheckConditionOperation>	clistCheckOneway_O;
		std::map<long,sindyeCheckConditionAction>		clistCheckOneway_A;

		for ( LONG jCnt = 0; jCnt < 4; ++jCnt ) {
			clistCheckOneway_O[jCnt].Primary = CheckOnewayOperation[iCnt].Primary[jCnt];
			clistCheckOneway_O[jCnt].Secondary = CheckOnewayOperation[iCnt].Secondary[jCnt];
			clistCheckOneway_A[jCnt].Primary = CheckOnewayAction[iCnt].Primary[jCnt];
			clistCheckOneway_A[jCnt].Secondary = CheckOnewayAction[iCnt].Secondary[jCnt];
		}

		// �ʍs�֎~
		std::map<long,sindyeCheckConditionOperation>	clistCheckNoPass_O, clistCheckNoPass_AP_O, clistCheckNoPass_NG_O;
		std::map<long,sindyeCheckConditionAction>		clistCheckNoPass_A, clistCheckNoPass_AP_A, clistCheckNoPass_NG_A;

		for ( LONG jCnt = 0; jCnt < 2; ++jCnt ) {
			// �ʍs�֎~
			clistCheckNoPass_O[jCnt].Primary = CheckNoPassOperation[iCnt].Primary[jCnt];
			clistCheckNoPass_O[jCnt].Secondary = CheckNoPassOperation[iCnt].Secondary[jCnt];
			// �ʍs�֎~�i�֌W�҈ȊO�j
			clistCheckNoPass_AP_O[jCnt].Primary = CheckNoPass_APOperation[iCnt].Primary[jCnt];
			clistCheckNoPass_AP_O[jCnt].Secondary = CheckNoPass_APOperation[iCnt].Secondary[jCnt];
			// �ʍs�֎~�i�ʍs�s�j
			clistCheckNoPass_NG_O[jCnt].Primary = CheckNoPass_NGOperation[iCnt].Primary[jCnt];
			clistCheckNoPass_NG_O[jCnt].Secondary = CheckNoPass_NGOperation[iCnt].Secondary[jCnt];

			// �ʍs�֎~
			clistCheckNoPass_A[jCnt].Primary = CheckNoPassAction[iCnt].Primary[jCnt];
			clistCheckNoPass_A[jCnt].Secondary = CheckNoPassAction[iCnt].Secondary[jCnt];
			// �ʍs�֎~�i�֌W�҈ȊO�j
			clistCheckNoPass_AP_A[jCnt].Primary = CheckNoPass_APAction[iCnt].Primary[jCnt];
			clistCheckNoPass_AP_A[jCnt].Secondary = CheckNoPass_APAction[iCnt].Secondary[jCnt];
			// �ʍs�֎~�i�ʍs�s�j
			clistCheckNoPass_NG_A[jCnt].Primary = CheckNoPass_NGAction[iCnt].Primary[jCnt];
			clistCheckNoPass_NG_A[jCnt].Secondary = CheckNoPass_NGAction[iCnt].Secondary[jCnt];
		}

		// �����N��
		std::map<long,sindyeCheckConditionOperation>	clistCheckDirguide_O, clistCheckGuide_O, clistCheckVics_O, clistCheckByway_O;
		std::map<long,sindyeCheckConditionOperation>	clistCheckNoLRTurn_O, clistCheckNoLRTurn_CN_O, clistCheckLane_O, clistCheckRoute_O;
		std::map<long,sindyeCheckConditionOperation>	clistCheckRoute_TR_O, clistCheckUTurn_O, clistCheckInternavi_O, clistCheckiPCVICS_O, clistCheckIntersection_O;

		std::map<long,sindyeCheckConditionAction>		clistCheckDirguide_A, clistCheckGuide_A, clistCheckVics_A, clistCheckByway_A;
		std::map<long,sindyeCheckConditionAction>		clistCheckNoLRTurn_A, clistCheckNoLRTurn_CN_A, clistCheckLane_A, clistCheckRoute_A;
		std::map<long,sindyeCheckConditionAction>		clistCheckRoute_TR_A, clistCheckUTurn_A, clistCheckInternavi_A, clistCheckiPCVICS_A, clistCheckIntersection_A;

		for ( LONG jCnt = 0; jCnt < 4; ++jCnt ) {
			// ���ʈē�
			clistCheckDirguide_O[jCnt].Primary = CheckDirguideOperation[iCnt].Primary[jCnt];
			clistCheckDirguide_O[jCnt].Secondary = CheckDirguideOperation[iCnt].Secondary[jCnt];
			// �����U��
			clistCheckGuide_O[jCnt].Primary = CheckGuideOperation[iCnt].Primary[jCnt];
			clistCheckGuide_O[jCnt].Secondary = CheckGuideOperation[iCnt].Secondary[jCnt];
			// VICS
			clistCheckVics_O[jCnt].Primary = CheckVicsOperation[iCnt].Primary[jCnt];
			clistCheckVics_O[jCnt].Secondary = CheckVicsOperation[iCnt].Secondary[jCnt];
			// ������
			clistCheckByway_O[jCnt].Primary = CheckBywayOperation[iCnt].Primary[jCnt];
			clistCheckByway_O[jCnt].Secondary = CheckBywayOperation[iCnt].Secondary[jCnt];
			// �E���܋֎~�Ӂi�����Ȃ��j
			clistCheckNoLRTurn_O[jCnt].Primary = CheckNoLRTurnOperation[iCnt].Primary[jCnt];
			clistCheckNoLRTurn_O[jCnt].Secondary = CheckNoLRTurnOperation[iCnt].Secondary[jCnt];
			// �E���܋֎~�i��������j
			clistCheckNoLRTurn_CN_O[jCnt].Primary = CheckNoLRTurn_CNOperation[iCnt].Primary[jCnt];
			clistCheckNoLRTurn_CN_O[jCnt].Secondary = CheckNoLRTurn_CNOperation[iCnt].Secondary[jCnt];
			// ���[��
			clistCheckLane_O[jCnt].Primary = CheckLaneOperation[iCnt].Primary[jCnt];
			clistCheckLane_O[jCnt].Secondary = CheckLaneOperation[iCnt].Secondary[jCnt];
			// �A�N�Z�X��
			clistCheckRoute_O[jCnt].Primary = CheckRouteOperation[iCnt].Primary[jCnt];
			clistCheckRoute_O[jCnt].Secondary = CheckRouteOperation[iCnt].Secondary[jCnt];
			// ���p����
			clistCheckRoute_TR_O[jCnt].Primary = CheckRoute_TROperation[iCnt].Primary[jCnt];
			clistCheckRoute_TR_O[jCnt].Secondary = CheckRoute_TROperation[iCnt].Secondary[jCnt];
			// U�^�[��
			clistCheckUTurn_O[jCnt].Primary = CheckUTurnOperation[iCnt].Primary[jCnt];
			clistCheckUTurn_O[jCnt].Secondary = CheckUTurnOperation[iCnt].Secondary[jCnt];
			// �C���^�[�i�rVICS
			clistCheckInternavi_O[jCnt].Primary = CheckInternaviOperation[iCnt].Primary[jCnt];
			clistCheckInternavi_O[jCnt].Secondary = CheckInternaviOperation[iCnt].Secondary[jCnt];
			// iPCVICS
			clistCheckiPCVICS_O[jCnt].Primary = CheckiPCVICSOperation[iCnt].Primary[jCnt];
			clistCheckiPCVICS_O[jCnt].Secondary = CheckiPCVICSOperation[iCnt].Secondary[jCnt];
			// �i�������ʌ����_
			clistCheckIntersection_O[jCnt].Primary = CheckIntersectionOperation[iCnt].Primary[jCnt];
			clistCheckIntersection_O[jCnt].Secondary = CheckIntersectionOperation[iCnt].Secondary[jCnt];

			// ���ʈē�
			clistCheckDirguide_A[jCnt].Primary = CheckDirguideAction[iCnt].Primary[jCnt];
			clistCheckDirguide_A[jCnt].Secondary = CheckDirguideAction[iCnt].Secondary[jCnt];
			// �����U��
			clistCheckGuide_A[jCnt].Primary = CheckGuideAction[iCnt].Primary[jCnt];
			clistCheckGuide_A[jCnt].Secondary = CheckGuideAction[iCnt].Secondary[jCnt];
			// VICS
			clistCheckVics_A[jCnt].Primary = CheckVicsAction[iCnt].Primary[jCnt];
			clistCheckVics_A[jCnt].Secondary = CheckVicsAction[iCnt].Secondary[jCnt];
			// ������
			clistCheckByway_A[jCnt].Primary = CheckBywayAction[iCnt].Primary[jCnt];
			clistCheckByway_A[jCnt].Secondary = CheckBywayAction[iCnt].Secondary[jCnt];
			// �E���܋֎~�Ӂi�����Ȃ��j
			clistCheckNoLRTurn_A[jCnt].Primary = CheckNoLRTurnAction[iCnt].Primary[jCnt];
			clistCheckNoLRTurn_A[jCnt].Secondary = CheckNoLRTurnAction[iCnt].Secondary[jCnt];
			// �E���܋֎~�i��������j
			clistCheckNoLRTurn_CN_A[jCnt].Primary = CheckNoLRTurn_CNAction[iCnt].Primary[jCnt];
			clistCheckNoLRTurn_CN_A[jCnt].Secondary = CheckNoLRTurn_CNAction[iCnt].Secondary[jCnt];
			// ���[��
			clistCheckLane_A[jCnt].Primary = CheckLaneAction[iCnt].Primary[jCnt];
			clistCheckLane_A[jCnt].Secondary = CheckLaneAction[iCnt].Secondary[jCnt];
			// �A�N�Z�X��
			clistCheckRoute_A[jCnt].Primary = CheckRouteAction[iCnt].Primary[jCnt];
			clistCheckRoute_A[jCnt].Secondary = CheckRouteAction[iCnt].Secondary[jCnt];
			// ���p����
			clistCheckRoute_TR_A[jCnt].Primary = CheckRoute_TRAction[iCnt].Primary[jCnt];
			clistCheckRoute_TR_A[jCnt].Secondary = CheckRoute_TRAction[iCnt].Secondary[jCnt];
			// U�^�[��
			clistCheckUTurn_A[jCnt].Primary = CheckUTurnAction[iCnt].Primary[jCnt];
			clistCheckUTurn_A[jCnt].Secondary = CheckUTurnAction[iCnt].Secondary[jCnt];
			// �C���^�[�i�rVICS
			clistCheckInternavi_A[jCnt].Primary = CheckInternaviAction[iCnt].Primary[jCnt];
			clistCheckInternavi_A[jCnt].Secondary = CheckInternaviAction[iCnt].Secondary[jCnt];
			// iPCVICS
			clistCheckiPCVICS_A[jCnt].Primary = CheckiPCVICSAction[iCnt].Primary[jCnt];
			clistCheckiPCVICS_A[jCnt].Secondary = CheckiPCVICSAction[iCnt].Secondary[jCnt];
			// �i�������ʌ����_
			clistCheckIntersection_A[jCnt].Primary = CheckIntersectionAction[iCnt].Primary[jCnt];
			clistCheckIntersection_A[jCnt].Secondary = CheckIntersectionAction[iCnt].Secondary[jCnt];
		}

		LONG DMode = GetLoadQueueDrawMode( (sindyeRoadQueueType)iCnt );
		m_mapCheckOperation[sindyeCheckOneway][DMode] = clistCheckOneway_O;
		m_mapCheckOperation[sindyeCheckNoPassage][DMode] = clistCheckNoPass_O;
		m_mapCheckOperation[sindyeCheckNoPassage_AP][DMode] = clistCheckNoPass_AP_O;
		m_mapCheckOperation[sindyeCheckNoPassage_NG][DMode] = clistCheckNoPass_NG_O;
		m_mapCheckOperation[sindyeDirguide][DMode] = clistCheckDirguide_O;
		m_mapCheckOperation[sindyeGuide][DMode] = clistCheckGuide_O;
		m_mapCheckOperation[sindyeVics][DMode] = clistCheckVics_O;
		m_mapCheckOperation[sindyeByway][DMode] = clistCheckByway_O;
		m_mapCheckOperation[sindyeTurnreg][DMode] = clistCheckNoLRTurn_O;
		m_mapCheckOperation[sindyeTurnreg_CN][DMode] = clistCheckNoLRTurn_CN_O;
		m_mapCheckOperation[sindyeLane][DMode] = clistCheckLane_O;
		m_mapCheckOperation[sindyeRoute][DMode] = clistCheckRoute_O;
		m_mapCheckOperation[sindyeRoute_TR][DMode] = clistCheckRoute_TR_O;
		m_mapCheckOperation[sindyeUTurn][DMode] = clistCheckUTurn_O;
		m_mapCheckOperation[sindyeInternavi][DMode] = clistCheckInternavi_O;
		m_mapCheckOperation[sindyeIPCVics][DMode] = clistCheckiPCVICS_O;
		m_mapCheckOperation[sindyeIntersection][DMode] = clistCheckIntersection_O;

		m_mapCheckAction[sindyeCheckOneway][DMode] = clistCheckOneway_A;
		m_mapCheckAction[sindyeCheckNoPassage][DMode] = clistCheckNoPass_A;
		m_mapCheckAction[sindyeCheckNoPassage_AP][DMode] = clistCheckNoPass_AP_A;
		m_mapCheckAction[sindyeCheckNoPassage_NG][DMode] = clistCheckNoPass_NG_A;
		m_mapCheckAction[sindyeDirguide][DMode] = clistCheckDirguide_A;
		m_mapCheckAction[sindyeGuide][DMode] = clistCheckGuide_A;
		m_mapCheckAction[sindyeVics][DMode] = clistCheckVics_A;
		m_mapCheckAction[sindyeByway][DMode] = clistCheckByway_A;
		m_mapCheckAction[sindyeTurnreg][DMode] = clistCheckNoLRTurn_A;
		m_mapCheckAction[sindyeTurnreg_CN][DMode] = clistCheckNoLRTurn_CN_A;
		m_mapCheckAction[sindyeLane][DMode] = clistCheckLane_A;
		m_mapCheckAction[sindyeRoute][DMode] = clistCheckRoute_A;
		m_mapCheckAction[sindyeRoute_TR][DMode] = clistCheckRoute_TR_A;
		m_mapCheckAction[sindyeUTurn][DMode] = clistCheckUTurn_A;
		m_mapCheckAction[sindyeInternavi][DMode] = clistCheckInternavi_A;
		m_mapCheckAction[sindyeIPCVics][DMode] = clistCheckiPCVICS_A;
		m_mapCheckAction[sindyeIntersection][DMode] = clistCheckIntersection_A;
	}

	for ( LONG iCnt = 0; iCnt <= ErrorCode_MAX; ++iCnt )
		// �����N��G���[�f�[�^
		m_mapCheckError[iCnt] = Error[iCnt].Type;
}

CString CLoadQueueInfo::GetLoadQueueInfo( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType, sindyeLoadQueueInfoType enumValueType )
{
	std::map<CString, INT>* pmapKey = GetInfoMap( enumKeyType );

	return GetLoadQueueInfo( (sindyeRoadQueueType)((*pmapKey)[lpcszKey]), enumValueType );
}

CString CLoadQueueInfo::GetLoadQueueInfo( sindyeRoadQueueType enumLQType, sindyeLoadQueueInfoType enumValueType )
{
	std::map<CString, INT>* pmapVal = GetInfoMap( enumValueType );
	for( std::map<CString, INT>::iterator it = pmapVal->begin(); it != pmapVal->end(); it++ )
	{
		if( it->second == enumLQType )
			return it->first;
	}
	return _T("");
}

CString CLoadQueueInfo::GetLoadQueueInfo( LONG lDrawMode, sindyeLoadQueueInfoType enumValueType )
{
	return GetLoadQueueInfo( (sindyeRoadQueueType)(m_mapDrawMode[lDrawMode]), enumValueType );
}

LONG CLoadQueueInfo::GetLoadQueueDrawMode( sindyeRoadQueueType enumLQType )
{
	for( std::map<LONG, INT>::iterator it = m_mapDrawMode.begin(); it != m_mapDrawMode.end(); it++ )
	{
		if( it->second == enumLQType )
			return it->first;
	}
	return -1;
}
LONG CLoadQueueInfo::GetLoadQueueCheckType( sindyeRoadQueueType enumLQType )
{
	for( std::map<LONG, sindyeRoadQueueType>::iterator it = m_mapCheckType.begin(); it != m_mapCheckType.end(); it++ )
	{
		if( it->second == enumLQType )
			return it->first;
	}
	return -1;
}

LONG CLoadQueueInfo::GetLoadQueueDrawMode( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType )
{
	std::map<CString, INT>* pmapKey = GetInfoMap( enumKeyType );
	return GetLoadQueueDrawMode( (sindyeRoadQueueType)((*pmapKey)[lpcszKey]) );
}

DWORD CLoadQueueInfo::GetLoadQueueColor( sindyeRoadQueueType enumLQType )
{
	for( std::map<DWORD, INT>::iterator it = m_mapColor.begin(); it != m_mapColor.end(); it++ )
	{
		if( it->second == enumLQType )
			return it->first;
	}
	return 0;
}

sindyeRoadQueueType CLoadQueueInfo::GetLoadQueueType( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType )
{
	return (sindyeRoadQueueType)(*GetInfoMap( enumKeyType ))[lpcszKey];
}

sindyeRoadQueueType CLoadQueueInfo::GetLoadQueueType( LONG lDrawMode )
{
	return (sindyeRoadQueueType)m_mapDrawMode[lDrawMode];
}

sindyeRoadQueueType CLoadQueueInfo::GetLoadQueueTypeByCheckType( LONG lCheckType )
{
	return (sindyeRoadQueueType)m_mapCheckType[lCheckType];
}

sindyeRoadQueueType CLoadQueueInfo::GetLoadQueueType( DWORD color )
{
	return (sindyeRoadQueueType)m_mapColor[color];
}

std::map<CString, INT>* CLoadQueueInfo::GetInfoMap( sindyeLoadQueueInfoType enumKeyType )
{
	switch( enumKeyType )
	{
	case sindyeLoadQueueInfoInfoTableName:		return &m_mapInfoTableName;			break;
	case sindyeLoadQueueInfoAliasName:			return &m_mapAliasName;				break;
	case sindyeLoadQueueInfoLQTableName:		return &m_mapLQTableName;			break;
	case sindyeLoadQueueInfoLQRFRoadFieldName:	return &m_mapLQRFRoadFieldName;		break;
//	case sindyeLpadQueueInfoRelInfoLQFieldName:	return &m_mapRelInfoLQFieldName;	break;
	default:break;
	}

	return NULL;
}


sindyeCheckConditionOperation CLoadQueueInfo::GetCheckOperation(LONG lDrawMode, sindyeCheckType SourceChkType, long DestChkType)
{
	return m_mapCheckOperation[SourceChkType][lDrawMode][DestChkType];
}

sindyeCheckConditionAction CLoadQueueInfo::GetCheckAction(LONG lDrawMode, sindyeCheckType SourceChkType, long DestChkType)
{
	return m_mapCheckAction[SourceChkType][lDrawMode][DestChkType];
}

sindyeCheckErrorType CLoadQueueInfo::GetError(sindyeCheckErrorCode ErrorCode)
{
	return m_mapCheckError[ErrorCode];
}

bool CLoadQueueInfo::IsShortenerRoadQueue(sindyeRoadQueueType enumLQType)
{
	if( enumLQType != sindyeRoadQueueVics && enumLQType != sindyeRoadQueueInternavi  && enumLQType != sindyeRoadQueueIPCVics && enumLQType != sindyeRoadQueueByway && enumLQType != sindyeRoadQueueRoute )
		return true;
	else
		return false;
}
