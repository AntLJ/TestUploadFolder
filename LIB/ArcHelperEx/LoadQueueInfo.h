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

// LoadQueueInfo.h: CLoadQueueInfo �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADQUEUEINFO_H__994546F6_42EA_4D7B_8BE7_3A20A990E2BE__INCLUDED_)
#define AFX_LOADQUEUEINFO_H__994546F6_42EA_4D7B_8BE7_3A20A990E2BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <list>
#include <algorithm>
#include "define.h"

/// �����N����̃^�C�v
enum sindyeLoadQueueInfoType {
	sindyeLoadQueueInfoInfoTableName,			//!< INFO_* �e�[�u������
	sindyeLoadQueueInfoAliasName,				//!< �G�C���A�X����
	sindyeLoadQueueInfoLQTableName,				//!< LQ_* �e�[�u������
	sindyeLoadQueueInfoLQRFRoadFieldName,		//!< ���H�����N�̑����ɂ��� *_LQRF �t�B�[���h����
//	sindyeLpadQueueInfoRelInfoLQFieldName,		//!< LQ_* �e�[�u���̑����ɂ��� *_ID �t�B�[���h����
	sindyeLoadQueueInfoColor,					//!< �����N��\���F
};

/// �����N��̃^�C�v
enum sindyeRoadQueueType
{
	sindyeRoadQueueDirguide,					//!< ���ʈē�
	sindyeRoadQueueGuide,						//!< �����U��
	sindyeRoadQueueVics,						//!< VICS
	sindyeRoadQueueByway,						//!< ������
	sindyeRoadQueueTurnreg,						//!< �E���܋֎~
	sindyeRoadQueueLane,						//!< ���[��
	sindyeRoadQueueRoute,						//!< �A�N�Z�X�^���p����
	sindyeRoadQueueUTurn,						//!< U�^�[��
	sindyeRoadQueueInternavi,					//!< �C���^�[�i�rVICS
	sindyeRoadQueueIPCVics,						//!< iPCVICS(bug 6477)
	sindyeRoadQueueIntersection					//!< �i�������ʌ����_
};

/// �����N��n�C���C�g�F
const static COLORREF _COLOR_LQFLASH        = RGB( 0x00, 0xff, 0xff );
const static COLORREF _COLOR_LQFLASH_YELLOW = RGB( 0xff, 0xff, 0x00 ); // bug9405�Œǉ�

/// �t�B�[���h���́i�����N��֘A�j
const static LPCTSTR INF_ID_FIELD				= ( _T("INF_ID"));		// beta7 ���� sindyeLpadQueueInfoRelInfoLQFieldName �� INF_ID �ɓ���
const static LPCTSTR LINK_ID_FIELD				= ( _T("LINK_ID"));
const static LPCTSTR LINKDIR_C_FIELD			= ( _T("LINKDIR_C"));
const static LPCTSTR SEQUENCE_FIELD				= ( _T("SEQUENCE"));
const static LPCTSTR MESHCODE_FIELD				= ( _T("MESHCODE"));
const static LPCTSTR LASTLINK_F_FIELD			= ( _T("LASTLINK_F"));

// �����N��̊J�n�E�I��
const static INT sindyeLoadQueueStart			= sindyeRoadQueueDirguide;
const static INT sindyeLoadQueueEnd				= sindyeRoadQueueIntersection;

// �`�F�b�N�^�C�v
enum sindyeCheckType
{
	sindyeCheckOneway,			//!< ����ʍs
	sindyeCheckNoPassage,		//!< �ʍs�֎~
	sindyeCheckNoPassage_AP,	//!< �ʍs�֎~�i�֌W�҈ȊO�j
	sindyeCheckNoPassage_NG,	//!< �ʍs�֎~�i�ʍs�s�j
	sindyeDirguide,				//!< ���ʈē�
	sindyeGuide,				//!< �����U��
	sindyeVics,					//!< VICS
	sindyeByway,				//!< ������
	sindyeTurnreg,				//!< �E���܋֎~�i�����Ȃ��j
	sindyeTurnreg_CN,			//!< �E���܋֎~�i��������j
	sindyeLane,					//!< ���[��
	sindyeRoute,				//!< �A�N�Z�X�^���p�����i���p�����ȊO�j
	sindyeRoute_TR,				//!< �A�N�Z�X�^���p�����i���p�����j
	sindyeUTurn,				//!< U�^�[��
	sindyeInternavi,			//!< �C���^�[�i�rVICS
	sindyeIPCVics,				//!< iPCVICS(bug 6477)
	sindyeIntersection			//!< �i�������ʌ����_
};

// ����
enum sindyeCheckOperation {
	sindyeCheckOperation_ONE,		// �P�����N��
	sindyeCheckOperation_END,		// �m�莞
};

// ��������������̂ŁA�E���܋֎~�i�����Ȃ��j�ƉE���܋֎~�i��������j�A�A�N�Z�X���Ə��p����
// �E���܋֎~�i�����Ȃ��j�ƃA�N�Z�X����Primary���g�p
// �E���܋֎~�i��������j�Ə��p������Secondary���g�p
// ���̑���Primary�����g�p���Ȃ�
const struct sindyeCheckConditionOperation {
	sindyeCheckOperation	Primary;		// 
	sindyeCheckOperation	Secondary;		// 
};

// ����ʍs
const struct sindyeOnewayCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeOnewayCheckOperation CheckOnewayOperation[sindyeLoadQueueEnd + 1] = {
	// 					(�Ȃ�,�ʉ�)					(�Ȃ�,�t��)					(����,�ʉ�)					(����,�t��)					(�Ȃ�,�ʉ�)					(�Ȃ�,�t��)					(����,�ʉ�)					(����,�t��)
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// �ʍs�֎~
const struct sindyeNoPassCheckOperation {
	sindyeCheckOperation	Primary[2];		// 
	sindyeCheckOperation	Secondary[2];		// 
};

const struct sindyeNoPassCheckOperation CheckNoPassOperation[sindyeLoadQueueEnd + 1] = {
	// 					�Ȃ�						����						�Ȃ�						����
	/* Dirguide */		{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// �ʍs�֎~�֌W�҈ȊO
const struct sindyeNoPass_APCheckOperation {
	sindyeCheckOperation	Primary[2];		// 
	sindyeCheckOperation	Secondary[2];		// 
};

const struct sindyeNoPass_APCheckOperation CheckNoPass_APOperation[sindyeLoadQueueEnd + 1] = {
	// 					�Ȃ�						����						�Ȃ�						����
	/* Dirguide */		{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// �ʍs�֎~�ʍs�s��
const struct sindyeNoPass_NGCheckOperation {
	sindyeCheckOperation	Primary[2];		// 
	sindyeCheckOperation	Secondary[2];		// 
};

const struct sindyeNoPass_NGCheckOperation CheckNoPass_NGOperation[sindyeLoadQueueEnd + 1] = {
	// 					�Ȃ�						����						�Ȃ�						����
	/* Dirguide */		{{sindyeCheckOperation_END,	sindyeCheckOperation_END},	{sindyeCheckOperation_END,	sindyeCheckOperation_END}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_END,	sindyeCheckOperation_END},	{sindyeCheckOperation_END,	sindyeCheckOperation_END}},	
	/* NoLRTurn */		{{sindyeCheckOperation_END,	sindyeCheckOperation_END},	{sindyeCheckOperation_END,	sindyeCheckOperation_END}},	
	/* Lane */			{{sindyeCheckOperation_END,	sindyeCheckOperation_END},	{sindyeCheckOperation_END,	sindyeCheckOperation_END}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// ���ʈē�
const struct sindyeDirguideCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeDirguideCheckOperation CheckDirguideOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// �����U��
const struct sindyeGuideCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeGuideCheckOperation CheckGuideOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// VICS
const struct sindyeVicsCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeVicsCheckOperation CheckVicsOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// ������
const struct sindyeBywayCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeBywayCheckOperation CheckBywayOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// �E���܋֎~�����Ȃ�
const struct sindyeNoLRTurnCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeNoLRTurnCheckOperation CheckNoLRTurnOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// �E���܋֎~��������
const struct sindyeNoLRTurn_CNCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeNoLRTurn_CNCheckOperation CheckNoLRTurn_CNOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// ���[��
const struct sindyeLaneCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeLaneCheckOperation CheckLaneOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// �A�N�Z�X��
const struct sindyeRouteCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeRouteCheckOperation CheckRouteOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// ���p����
const struct sindyeRoute_TRCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeRoute_TRCheckOperation CheckRoute_TROperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// U�^�[��
const struct sindyeUTurnCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeUTurnCheckOperation CheckUTurnOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// �C���^�[�i�rVICS
const struct sindyeInternaviCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeInternaviCheckOperation CheckInternaviOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// iPCVICS
const struct sindyeiPCVICSCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeiPCVICSCheckOperation CheckiPCVICSOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// Intersection
const struct sindyeIntersectionCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeIntersectionCheckOperation CheckIntersectionOperation[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����			������܁E������			������܁E�t����			���S��܁E������			���S��܁E�t����
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// ����
enum sindyeCheckAction {
	sindyeCheckAction_NG,			// �s����
	sindyeCheckAction_OK,			// ����
	sindyeCheckAction_MSG,			// �m�F
};

// ��������������̂ŁA�E���܋֎~�i�����Ȃ��j�ƉE���܋֎~�i��������j�A�A�N�Z�X���Ə��p����
// �E���܋֎~�i�����Ȃ��j�ƃA�N�Z�X����Primary���g�p
// �E���܋֎~�i��������j�Ə��p������Secondary���g�p
// ���̑���Primary�����g�p���Ȃ�
const struct sindyeCheckConditionAction {
	sindyeCheckAction	Primary;		// 
	sindyeCheckAction	Secondary;		// 
};

// ����ʍs
const struct sindyeOnewayCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeOnewayCheckAction CheckOnewayAction[sindyeLoadQueueEnd + 1] = {
	// 					(�Ȃ�,�ʉ�)				(�Ȃ�,�t��)				(����,�ʉ�)				(����,�t��)				(�Ȃ�,�ʉ�)				(�Ȃ�,�t��)				(����,�ʉ�)					(����,�t��)
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG},	{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG}},
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG},	{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG}},
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG},	{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG}},
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG},	{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG}},
};

// �ʍs�֎~
const struct sindyeNoPassCheckAction {
	sindyeCheckAction	Primary[2];		// 
	sindyeCheckAction	Secondary[2];		// 
};

const struct sindyeNoPassCheckAction CheckNoPassAction[sindyeLoadQueueEnd + 1] = {
	// 					�Ȃ�						����					�Ȃ�					����
	/* Dirguide */		{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* UTurn */			{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_NG,	sindyeCheckAction_MSG}},	
};

// �ʍs�֎~�֌W�҈ȊO
const struct sindyeNoPass_APCheckAction {
	sindyeCheckAction	Primary[2];		// 
	sindyeCheckAction	Secondary[2];		// 
};

const struct sindyeNoPass_APCheckAction CheckNoPass_APAction[sindyeLoadQueueEnd + 1] = {
	// 					�Ȃ�						����					�Ȃ�					����
	/* Dirguide */		{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* UTurn */			{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_NG,	sindyeCheckAction_MSG}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_NG,	sindyeCheckAction_MSG}},	
};

// �ʍs�֎~�s��
const struct sindyeNoPass_NGCheckAction {
	sindyeCheckAction	Primary[2];		// 
	sindyeCheckAction	Secondary[2];		// 
};

const struct sindyeNoPass_NGCheckAction CheckNoPass_NGAction[sindyeLoadQueueEnd + 1] = {
	// 					�Ȃ�						����					�Ȃ�					����
	/* Dirguide */		{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Guide */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* NoLRTurn */		{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Lane */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Route */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* UTurn */			{{sindyeCheckAction_NG,		sindyeCheckAction_NG},	{sindyeCheckAction_NG,	sindyeCheckAction_NG}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_NG,	sindyeCheckAction_MSG}},	
};

// ���ʈē�
const struct sindyeDirguideCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeDirguideCheckAction CheckDirguideAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// �����U��
const struct sindyeGuideCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeGuideCheckAction CheckGuideAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// VICS
const struct sindyeVicsCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeVicsCheckAction CheckVicsAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// ������
const struct sindyeBywayCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeBywayCheckAction CheckBywayAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// �E���܋֎~�����Ȃ�
const struct sindyeNoLRTurnCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeNoLRTurnCheckAction CheckNoLRTurnAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// �E���܋֎~��������
const struct sindyeNoLRTurn_CNCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeNoLRTurn_CNCheckAction CheckNoLRTurn_CNAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������			������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// ���[��
const struct sindyeLaneCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeLaneCheckAction CheckLaneAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// �A�N�Z�X��
const struct sindyeRouteCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeRouteCheckAction CheckRouteAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_NG,	sindyeCheckAction_NG,	sindyeCheckAction_NG,	sindyeCheckAction_NG},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// ���p����
const struct sindyeRoute_TRCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeRoute_TRCheckAction CheckRoute_TRAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_NG,	sindyeCheckAction_NG,	sindyeCheckAction_NG}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// U�^�[��
const struct sindyeUTurnCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeUTurnCheckAction CheckUTurnAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// �C���^�[�i�rVICS
const struct sindyeInternaviCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeInternaviCheckAction CheckInternaviAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},		// [bug7113]Internavi VICS��IPC VICS�̋���OK
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// iPCVICS
const struct sindyeiPCVICSCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeiPCVICSCheckAction CheckiPCVICSAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},		// [bug7113]Internavi VICS��IPC VICS�̋���OK
	/* iPCVICS */		{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// Intersection
const struct sindyeIntersectionCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeIntersectionCheckAction CheckIntersectionAction[sindyeLoadQueueEnd + 1] = {
	// 					������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����		������܁E������		������܁E�t����		���S��܁E������		���S��܁E�t����
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
};

// ----------------------------------------------------------------------------
//!< �����N��K�����[���G���[�R�[�h
// ----------------------------------------------------------------------------
enum sindyeCheckErrorCode {
	sindyeCheckErrorCode_Duplicate,				// �����N�񂪏d�����Ă��܂��܂��I
	sindyeCheckErrorCode_Set_Dirguide,				// ���� ���ʈē� ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Guide,				// ���� �����U�� ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Vics,					// ���� VICS ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Byway,				// ���� ������ ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Turnreg,				// ���� �E���܋֎~ ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Lane,					// ���� ���[����� ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Route,				// ���� �A�N�Z�X�^���p���� ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_UTurn,				// ���� U�^�[�� ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Internavi,			// ���� �C���^�[�i�rVICS ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_ShapeDirection,		// ���� %s [����`�� / �������] ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Shape,				// ���� %s [����`��] ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_Set_Direction,			// ���� %s [�������] ���ݒ肳��Ă��܂��I
	sindyeCheckErrorCode_OutCourse1,				// VICS ���ݒ肳��Ă��� ���H�����N[OBJECTID:%d]�� �o�H�O �ɐݒ肷�邱�Ƃ͂ł��܂���I
	sindyeCheckErrorCode_OutCourse2,				// �C���^�[�i�rVICS ���ݒ肳��Ă��� ���H�����N[OBJECTID:%d]�� �o�H�O �ɐݒ肷�邱�Ƃ͂ł��܂���I
	sindyeCheckErrorCode_OutCourse3,				// VICS/�C���^�[�i�rVICS ���ݒ肳��Ă��� ���H�����N[OBJECTID:%d]�� �o�H�O �ɐݒ肷�邱�Ƃ͂ł��܂���I
	sindyeCheckErrorCode_OutCourse_Vics,			// �o�H�O �̓��H�� VICS ��ݒ肷�邱�Ƃ͂ł��܂���I
	sindyeCheckErrorCode_OutCourse_Internavi,		// �o�H�O �̓��H�� �C���^�[�i�rVICS ��ݒ肷�邱�Ƃ͂ł��܂���I

	sindyeCheckErrorCode_Oneway,			// %s �� �����N��K���G���[[����ʍs] �ł��I\n�E���H�����N[OBJECTID:%d] �� ����ʍs �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_Oneway_Msg,		// %s �� �����N��K���G���[[����ʍs] �ł��I\n�E���H�����N[OBJECTID:%d] �� ����ʍs �ɑ΂��� �ʉ� �ł��B

	sindyeCheckErrorCode_NoPassage,				// %s �� �����N��K���G���[[�ʍs�֎~] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~ �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_AP,				// %s �� �����N��K���G���[[�ʍs�֎~�i�֌W�҈ȊO�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�֌W�҈ȊO�j �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_NG,				// %s �� �����N��K���G���[[�ʍs�֎~�i�ʍs�s�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�ʍs�s�j �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_Msg,			// %s �� �����N��K���G���[[�ʍs�֎~] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~ �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_AP_Msg,			// %s �� �����N��K���G���[[�ʍs�֎~�i�֌W�҈ȊO�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�֌W�҈ȊO�j �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_NG_Msg,			// %s �� �����N��K���G���[[�ʍs�֎~�i�ʍs�s�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�ʍs�s�j �ɑ΂��� �ʉ� �ł��B

	sindyeCheckErrorCode_Oneway_LQ,		// %s[INF_ID:%d] �� �����N��K���G���[[����ʍs] �ł��I\n�E���H�����N[OBJECTID:%d] �� ����ʍs �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_Oneway_LQ_Msg,	// %s[INF_ID:%d] �� �����N��K���G���[[����ʍs] �ł��I\n�E���H�����N[OBJECTID:%d] �� ����ʍs �ɑ΂��� �t�� �ł��B

	sindyeCheckErrorCode_NoPassageLQ,				// %s �� �����N��K���G���[[�ʍs�֎~] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~ �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_AP_LQ,			// %s �� �����N��K���G���[[�ʍs�֎~�i�֌W�҈ȊO�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�֌W�҈ȊO�j �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_NG_LQ,			// %s �� �����N��K���G���[[�ʍs�֎~�i�ʍs�s�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�ʍs�s�j �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassageLQ_Msg,			// %s �� �����N��K���G���[[�ʍs�֎~] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~ �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_AP_LQ_Msg,		// %s �� �����N��K���G���[[�ʍs�֎~�i�֌W�҈ȊO�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�֌W�҈ȊO�j �ɑ΂��� �ʉ� �ł��B
	sindyeCheckErrorCode_NoPassage_NG_LQ_Msg,		// %s �� �����N��K���G���[[�ʍs�֎~�i�ʍs�s�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�ʍs�s�j �ɑ΂��� �ʉ� �ł��B

	sindyeCheckErrorCode_LQ,					// %s �� �����N��K���G���[[%s] �ł��I\n�E���H�����N[OBJECTID:%d] �� %s[INF_ID:%d] �ɑ΂��� %s �ł��B
	sindyeCheckErrorCode_LQ_Msg,				// %s �� �����N��K���G���[[%s] �ł��I\n�E���H�����N[OBJECTID:%d] �� %s[INF_ID:%d] �ɑ΂��� %s �ł��B

	sindyeCheckErrorCode_Duplicate_VICSID,			// %s[INF_ID:%d] �� ���b�V���R�[�h�A�����N�ԍ��A��ʂ����� %s �����݂��܂��I
	sindyeCheckErrorCode_AdjoinDisconnect_Node,	// �Ώۃm�[�h�����b�V�����E�ɂ���܂��I
	sindyeCheckErrorCode_NoChange_VICS,			// %s �� �����_�ŕω����Ȃ���΂����܂���!
	sindyeCheckErrorCode_Other,
};

const static LONG ErrorCode_MAX = sindyeCheckErrorCode_Other;

//!< �����N��K�����[���G���[�f�[�^
const struct sindyeCheckErrorType {
	INT				MsgType;
	UINT			Type;
	UINT			Level;
	TCHAR			Message[256];
};

const struct sindyeCheckError {
	sindyeCheckErrorType		Type;
};

const struct sindyeCheckError Error[ErrorCode_MAX + 1] = {
#ifndef SINDY_FOR_ENGLISH
	// 	 (�^�C�v),						(���x��),		(���b�Z�[�W)
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("�����N�񂪏d�����Ă��܂��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� ���ʈē� ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� �����U�� ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� VICS ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� ������ ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� �E���܋֎~ ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� ���[�� ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� �A�N�Z�X�^���p���� ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� U�^�[�� ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� �C���^�[�i�rVICS ���ݒ肳��Ă��܂��I") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� %s [����`�� / �������] ���ݒ肳��Ă��܂��I") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� %s [����`��] ���ݒ肳��Ă��܂��I") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("���� %s [�������] ���ݒ肳��Ă��܂��I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("VICS ���ݒ肳��Ă��铹�H�����N[OBJECTID:%d]�� �o�H�O �ɐݒ肷�邱�Ƃ͂ł��܂���I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("�C���^�[�i�rVICS ���ݒ肳��Ă��铹�H�����N[OBJECTID:%d]�� �o�H�O �ɐݒ肷�邱�Ƃ͂ł��܂���I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("VICS/�C���^�[�i�rVICS ���ݒ肳��Ă��铹�H�����N[OBJECTID:%d]�� �o�H�O �ɐݒ肷�邱�Ƃ͂ł��܂���I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("�o�H�O �̓��H�� VICS ��ݒ肷�邱�Ƃ͂ł��܂���I") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("�o�H�O �̓��H�� �C���^�[�i�rVICS ��ݒ肷�邱�Ƃ͂ł��܂���I") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s �� �����N��K���G���[[����ʍs] �ł��I\n�E���H�����N[OBJECTID:%d] �� ����ʍs �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s �� �����N��K���G���[[����ʍs] �ł��I\n�E���H�����N[OBJECTID:%d] �� ����ʍs �ɑ΂��� %s �ł��B") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s �� �����N��K���G���[[�ʍs�֎~] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~ �ɑ΂��� %s �ł��B") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s �� �����N��K���G���[[�ʍs�֎~�i�֌W�҈ȊO�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�֌W�҈ȊO�j �ɑ΂��� %s �ł��B") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s �� �����N��K���G���[[�ʍs�֎~�i�ʍs�s�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�ʍs�s�j �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s �� �����N��K���G���[[�ʍs�֎~] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~ �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s �� �����N��K���G���[[�ʍs�֎~�i�֌W�҈ȊO�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�֌W�҈ȊO�j �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s �� �����N��K���G���[[�ʍs�֎~�i�ʍs�s�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�ʍs�s�j �ɑ΂��� %s �ł��B") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s[INF_ID:%d] �� �����N��K���G���[[����ʍs] �ł��I\n�E���H�����N[OBJECTID:%d] �� ����ʍs �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s[INF_ID:%d] �� �����N��K���G���[[����ʍs] �ł��I\n�E���H�����N[OBJECTID:%d] �� ����ʍs �ɑ΂��� %s �ł��B") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s �� �����N��K���G���[[�ʍs�֎~] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~ �ɑ΂��� %s �ł��B") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s �� �����N��K���G���[[�ʍs�֎~�i�֌W�҈ȊO�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�֌W�҈ȊO�j �ɑ΂��� %s �ł��B") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s �� �����N��K���G���[[�ʍs�֎~�i�ʍs�s�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�ʍs�s�j �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s �� �����N��K���G���[[�ʍs�֎~] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~ �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s �� �����N��K���G���[[�ʍs�֎~�i�֌W�҈ȊO�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�֌W�҈ȊO�j �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s �� �����N��K���G���[[�ʍs�֎~�i�ʍs�s�j] �ł��I\n�E���H�����N[OBJECTID:%d] �� �ʍs�֎~�i�ʍs�s�j �ɑ΂��� %s �ł��B") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s �� �����N��K���G���[[%s] �ł��I\n�E���H�����N[OBJECTID:%d] �� %s[INF_ID:%d] �ɑ΂��� %s �ł��B") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s �� �����N��K���G���[[%s] �ł��I\n�E���H�����N[OBJECTID:%d] �� %s[INF_ID:%d] �ɑ΂��� %s �ł��B") },

	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%s[INF_ID:%d]�@�� ���b�V���R�[�h�A�����N�ԍ��A��ʂ����� %s �����݂��܂��I") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%s �� �Ώۃm�[�h �����b�V�����E�ɂ���܂��I") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%s �� �����_�ŕω����Ȃ���΂����܂���!") },
	{ 0, MB_ICONWARNING | MB_OK,		0,	_T("�z��O�G���[�ł��B") },
#else
	// 	 (�^�C�v),						(���x��),		(���b�Z�[�W)
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("The link queue will duplicate!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Signpost has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Forced guidance has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("VICS has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Byway has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Turn restriction has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Lane has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Access/connection road has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("U-turn has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Internavi VICS has already been set!") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("[Same shape/Same direction] has already been set!") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("[Same shape] has already been set!") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("[Same direction] has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the road link with VICS [OBJECTID:%d] to the link excluded from the route calculation!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the road link with Internavi VICS [OBJECTID:%d] to the link excluded from the route calculation!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the road link with VICS/Internavi VICS [OBJECTID:%d] to the link excluded from the route calculation!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the VICS to the link excluded from the route calculation!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the Internavi VICS to the link excluded from the route calculation!") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("It is %s to the one-way of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error to [one-way] of the road link [OBJECTID:%d]!\n It is %s to the one-way.") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error to [Close-to-traffic regulation] of the road link [OBJECTID:%d]! \n It is %s to the close-to-traffic restriction.") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error to [Close-to-traffic (Authorizsed personnel only)] of the road link [OBJECTID:%d]! \n It is %s to the Close-to-traffic (Authorizsed personnel only).") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error to [Close-to-traffic regulation (impassable)] of the road link [OBJECTID:%d]! \n It is %s to the close-to-traffic restriction (Impassable).") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error to [Close-to-traffic regulation] of the road link [OBJECTID:%d]! \n It is %s to the close-to-traffic restriction.") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error to [Close-to-traffic (Authorizsed personnel only)] of the road link [OBJECTID:%d]! \n It is %s to the Close-to-traffic (Authorizsed personnel only).") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error to [Close-to-traffic regulation (impassable)] of the road link [OBJECTID:%d]! \n It is %s to the close-to-traffic restriction (Impassable).") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s[INF_ID:%d] is a link queue restriction error [one-way]! \n�EIt is %s to the one-way of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s[INF_ID:%d] is a link queue restriction error [one-way]! \n�EIt is %s to the one-way of the road link [OBJECTID:%d].") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error [Close-to-traffic regulation]! \n�EIt is %s to the close-to-traffic restriction of the road link [OBJECTID:%d].") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error [Close-to-traffic (Authorizsed personnel only)]! \n�EIt is %s to the Close-to-traffic (Authorizsed personnel only) of the road link [OBJECTID:%d].") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error [Close-to-traffic regulation (impassable)]! \n�EIt is %s to the close-to-traffic restriction (Impassable) of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error [Close-to-traffic regulation]! \n�EIt is %s to the close-to-traffic restriction of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error [Close-to-traffic (Authorizsed personnel only)]! \n�EIt is %s to the Close-to-traffic (Authorizsed personnel only) of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error [Close-to-traffic regulation (impassable)]! \n�EIt is %s to the close-to-traffic restriction (Impassable) of the road link [OBJECTID:%d].") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error [%s]! \n�ERoad_Link[OBJECTID:%d]\n�E%s[INF_ID:%d]\n It is %s.") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error [%s]! \n�ERoad_Link[OBJECTID:%d]\n�E%s[INF_ID:%d]\n It is %s.") },

	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%�� of the same mesh code, link number and type exists in %s[INF_ID:%d].") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Target node of %s is on the mesh boundary line.") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%s must be changed at the intersection.") },
	{ 0, MB_ICONWARNING | MB_OK,		0,	_T("Unexpected error") },
#endif
};

class CLoadQueueInfo
{
public:
	CLoadQueueInfo();
	virtual ~CLoadQueueInfo();
	void SetData();
	static CString GetLoadQueueInfo( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType, sindyeLoadQueueInfoType enumValueType );
	static CString GetLoadQueueInfo( sindyeRoadQueueType enumLQType, sindyeLoadQueueInfoType enumValueType );
	static CString GetLoadQueueInfo( LONG lDrawMode, sindyeLoadQueueInfoType enumValueType );
	static LONG GetLoadQueueDrawMode( sindyeRoadQueueType enumLQType );
	static LONG GetLoadQueueDrawMode( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType );
	static sindyeRoadQueueType GetLoadQueueType( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType );
	static sindyeRoadQueueType GetLoadQueueType( LONG lDrawMode );
	static sindyeRoadQueueType GetLoadQueueType( DWORD color );
	static LONG GetLoadQueueCheckType( sindyeRoadQueueType enumLQType );
	static sindyeRoadQueueType GetLoadQueueTypeByCheckType( LONG lCheckType );
	static DWORD GetLoadQueueColor( sindyeRoadQueueType enumLQType );

	// �`�F�b�N������擾
	// SourceChkType:0=����ʍs
	//               1=�ʍs�֎~
	//               2=�ʍs�֎~�i�֌W�҈ȊO�j
	//               3=�ʍs�֎~�i�ʍs�s�j
	//               4=���ʈē�
	//               5=�����U��
	//               6=VICS
	//               7=������
	//               8=�E���܋֎~�i�����Ȃ��j
	//               9=�E���܋֎~�i��������j
	//               10=���[��
	//               11=�A�N�Z�X��
	//               12=���p����
	//               13=U�^�[��
	//               14=�C���^�[�i�rVICS
	//               15=iPC VICS
	//               16=�i�������ʌ����_
	// DestChkType:0=�����Ȃ��E�ʉ�,�����Ȃ�,������܁E������
	//             1=�����Ȃ��E�t��,��������,������܁E�t����
	//             2=��������E�ʉ�,���S��܁E������
	//             3=��������E�t��,���S��܁E�t����
	static sindyeCheckConditionOperation GetCheckOperation(LONG lDrawMode, sindyeCheckType SourceChkType, long DestChkType);

	// �`�F�b�N������擾
	// SourceChkType:0=����ʍs
	//               1=�ʍs�֎~
	//               2=�ʍs�֎~�i�֌W�҈ȊO�j
	//               3=�ʍs�֎~�i�ʍs�s�j
	//               4=���ʈē�
	//               5=�����U��
	//               6=VICS
	//               7=������
	//               8=�E���܋֎~�i�����Ȃ��j
	//               9=�E���܋֎~�i��������j
	//               10=���[��
	//               11=�A�N�Z�X��
	//               12=���p����
	//               13=U�^�[��
	//               14=�C���^�[�i�rVICS
	//               15=iPC VICS
	//               16=�i�������ʌ����_
	// DestChkType:0=�����Ȃ��E�ʉ�,�����Ȃ�,������܁E������
	//             1=�����Ȃ��E�t��,��������,������܁E�t����
	//             2=��������E�ʉ�,���S��܁E������
	//             3=��������E�t��,���S��܁E�t����
	static sindyeCheckConditionAction GetCheckAction(LONG lDrawMode, sindyeCheckType SourceChkType, long DestChkType);

	// �G���[�^�C�v�擾
	static sindyeCheckErrorType GetError(sindyeCheckErrorCode ErrorCode);

	// [Bug8968] �ǉ�
	// �����N�񂪒Z�k�\���ǂ������擾�i���L�ł����true�j
	// �Z�k�\�F���ʈē�,�����U��,�E���܋֎~,���[��,U�^�[��,�i�������ʌ����_
	static bool IsShortenerRoadQueue(sindyeRoadQueueType enumLQType);

private:
	static std::map<CString, INT>* GetInfoMap( sindyeLoadQueueInfoType enumKeyType );
	static std::map<CString, INT> m_mapInfoTableName;
	static std::map<CString, INT> m_mapAliasName;
	static std::map<CString, INT> m_mapLQTableName;
	static std::map<CString, INT> m_mapLQRFRoadFieldName;
//	static std::map<CString, INT> m_mapRelInfoLQFieldName;
	static std::map<LONG, INT>    m_mapDrawMode;
	static std::map<LONG, sindyeRoadQueueType>    m_mapCheckType;			// sindyeCheckType��sindyeLoadQueueInfoType�̕ϊ��̂���
	static std::map<DWORD, INT>   m_mapColor;

	static std::map< LONG, std::map< LONG, std::map<long,sindyeCheckConditionOperation> > >	m_mapCheckOperation;
	static std::map< LONG, std::map< LONG, std::map<long,sindyeCheckConditionAction> > >	m_mapCheckAction;
	static std::map< LONG, sindyeCheckErrorType >	m_mapCheckError;
};

#endif // !defined(AFX_LOADQUEUEINFO_H__994546F6_42EA_4D7B_8BE7_3A20A990E2BE__INCLUDED_)
