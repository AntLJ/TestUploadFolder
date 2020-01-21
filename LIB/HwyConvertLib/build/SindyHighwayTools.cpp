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

/**
 *	�e�탆�[�e�B���e�B
 *	@author	Fusatoshi Abe
 *	@date	2003/10/30		�V�K�쐬
 */

#include "SindyErrorInfo.h"
#include "SindyHighwayTools.h"

bool isMainLineLink(IFeature* ipLink)
{
	using namespace sindy;
	using namespace road_link;

	// MAIN_LINKCLASS_C���{���i����or�񕪗��j�ł��邩�H
	static long aMainLinkClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::MAIN_LINKCLASS_C.c_str() );
	long aMainLinkClass = ipLink->GetValue( aMainLinkClassFieldIndex );
	switch (aMainLinkClass)
	{
		case linkclass_bunri:
		case linkclass_hibunri:
			break;
		default:
			return false;
	}
	
	// �H���R�[�h�̃`�F�b�N
	static long aRoadCodeFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROAD_CODE.c_str() );
	long aRoadCode = ipLink->GetValue( aRoadCodeFieldIndex );
	if (aRoadCode == 0)								{ return false; }	// �H���R�[�h=0
	if (400000 <= aRoadCode && aRoadCode < 600000)	{ return false; }	// �ڑ��������N
	
	return true;
}


bool isMainLineTraceTarget(IFeature* ipLink)
{
#ifdef	_DEBUG
	long aOID = ipLink->GetOID();
#endif

	// UpDownClass_C��HWY�ΏۂłȂ���ΒǐՑΏۊO
	static long aUpDownClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::UPDOWNCLASS_C.c_str() );
	long aUpDownClass = ipLink->GetValue( aUpDownClassFieldIndex );
	switch (aUpDownClass)
	{
		case sindy::road_link::updownclass_nodir:
		case sindy::road_link::updownclass_up:
		case sindy::road_link::updownclass_down:
		case sindy::road_link::updownclass_common:
			break;
		default:
			return false;
	}

	// Main_LinkClass_C���u�{��(�㉺�񕪗�)�����N�v�u�{��(�㉺����)�����N�v�u�A���H(�{���Ԃ̓n���)�v�łȂ���ΒǐՑΏۊO
	static long aMainLinkClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::MAIN_LINKCLASS_C.c_str() );
	long aMainLinkClass = ipLink->GetValue( aMainLinkClassFieldIndex );
	switch (aMainLinkClass)
	{
	case sindy::road_link::linkclass_hibunri:
	case sindy::road_link::linkclass_bunri:
		break;
	case sindy::road_link::linkclass_junction:	// �{�����n����ɂȂ��Ă���̂����i�A�����b�Z�[�W���o���j
#ifdef	_DEBUG
		std::cout << "\t" << "Road_Link" << "\t" << aOID << "\tMAIN_LINKCLASS_C = �n���" << std::endl;
#endif
		break;
	default:
		return false;
	}

	// �H���R�[�h�̃`�F�b�N
	static long aRoadCodeFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROAD_CODE.c_str() );
	long aRoadCode = ipLink->GetValue( aRoadCodeFieldIndex );
	if (aRoadCode < 200000)
	{
		return false;
	}

	return true;
}

bool isJctLink(IFeature* ipLink)
{
	// MAIN_LINKCLASS_C���n����Ȃ�n����Ƃ���
	static long aMainLinkClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::MAIN_LINKCLASS_C.c_str() );
	long aMainLinkClass = ipLink->GetValue( aMainLinkClassFieldIndex );
	switch (aMainLinkClass)
	{
	case sindy::road_link::linkclass_junction:	// �n���
		return true;
	default:
		break;
	}

	return false;
}

bool isNewJctLink(IFeature* ipLink)
{
	static long aMainLinkClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::MAIN_LINKCLASS_C.c_str() );
	long aMainLinkClass = ipLink->GetValue( aMainLinkClassFieldIndex );
	return (aMainLinkClass == sindy::road_link::linkclass_junction);
}

bool isRampLink(IFeature* ipLink)
{
	static long aMainLinkClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::MAIN_LINKCLASS_C.c_str() );
	long aMainLinkClass = ipLink->GetValue( aMainLinkClassFieldIndex );
	return (aMainLinkClass == sindy::road_link::linkclass_ramp);
}

bool isSaLink(IFeature* ipLink)
{
	static long aMainLinkClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::MAIN_LINKCLASS_C.c_str() );
	long aMainLinkClass = ipLink->GetValue( aMainLinkClassFieldIndex );
	return (aMainLinkClass == sindy::road_link::linkclass_salink);
}

bool isSideLineLink(IFeature* ipLink)
{
	static long aMainLinkClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::MAIN_LINKCLASS_C.c_str() );
	long aMainLinkClass = ipLink->GetValue( aMainLinkClassFieldIndex );
	return (aMainLinkClass == sindy::road_link::linkclass_sidelink);
}

bool isCrossingLink(IFeature* ipLink)
{
	static long aMainLinkClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::MAIN_LINKCLASS_C.c_str() );
	long aMainLinkClass = ipLink->GetValue( aMainLinkClassFieldIndex );
	return (aMainLinkClass == sindy::road_link::linkclass_crossing);
}

bool isTollRoad(IFeature* ipLink)
{
	long aRoadClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROADCLASS_C.c_str());
	long aRoadClass = ipLink->GetValue( aRoadClassFieldIndex );
	return (100 <= aRoadClass && aRoadClass < 200);
}

bool isRoadCode(IFeature* ipLink)
{
	static long aRoadCodeFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROAD_CODE.c_str() );
	long aRoadCode = ipLink->GetValue( aRoadCodeFieldIndex );
	return (aRoadCode != 0);
}

bool isJointZoneLink(IFeature* ipLink)
{
	static long aRoadCodeFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROAD_CODE.c_str() );
	long aRoadCode = ipLink->GetValue( aRoadCodeFieldIndex );
	return (400000 <= aRoadCode) && (aRoadCode < 600000);
}

bool isTollFreeZoneLink(IFeature* ipLink)
{
	static long aRoadCodeFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROAD_CODE.c_str() );
	long aRoadCode = ipLink->GetValue( aRoadCodeFieldIndex );
	return (600000 <= aRoadCode) && (aRoadCode < 800000);
}

bool isGeneralRoad( const IFeaturePtr cpLink )
{
	static long aUpDownClassFieldIndex = cpLink->Fields->_FindField( sindy::road_link::UPDOWNCLASS_C.c_str() );
	long aUpDownClass = cpLink->GetValue( aUpDownClassFieldIndex );
	bool aResult = true;
	switch (aUpDownClass)
	{
		using namespace sindy::road_link;
		case updownclass_nodir:
		case updownclass_up:
		case updownclass_down:
		case updownclass_common:
		case updownclass_etcramp:
			aResult = false;
			break;
		case updownclass_nothwy:
		case updownclass_etcaccess:
			break;
		default:
			std::runtime_error("�s����UpDownClass_C");
			break;
	}
	return aResult;
}

bool isSameLine(long aRoadCode1, long aRoadCode2)
{
	// ���S�ɓ��������`�F�b�N
	if (aRoadCode1 == aRoadCode2) { return true; }

	// �����Ƀ\�[�g
	std::pair<long, long> aPair;
	if (aRoadCode1 < aRoadCode2) {
		aPair = std::make_pair(aRoadCode1, aRoadCode2);
	}
	else {
		aPair = std::make_pair(aRoadCode2, aRoadCode1);
	}

	// HWY�{���̐ڑ����Ɩ�����Ԃ͓����H���ƌ��Ȃ�
	if (200000 <= aPair.first && aPair.first < 400000) {
		if (aPair.second == aPair.first + 200000 ||
			aPair.second == aPair.first + 400000) {
			return true;
		}
	}

	return false;
}

bool isZeroBranch(_IRowPtr ipHighwayTxt)
{
	CComVariant vaForeCount;
	if( FAILED(GDBGetValue(ipHighwayTxt, sindy::highway_text::FORE_COUNT, &vaForeCount))){
		throw std::runtime_error("");
	}
	//Fore_Count��0���ǂ����B0�̏ꍇ��Back_Count���`�F�b�N
	if(! vaForeCount.lVal){
		CComVariant vaBackCount;
		if( FAILED(GDBGetValue(ipHighwayTxt, sindy::highway_text::BACK_COUNT, &vaBackCount))){
			throw std::runtime_error("");
		}
		//Fore_Count,Back_Count������0�̏ꍇ��true
		if(! vaBackCount.lVal){
			return true;
		}
	}
	return false;
}

IFeaturePtr getOppositeNode( IFeaturePtr cpLink, long cNodeID, SindyRoadInfo* cpRoadInfo )
{
	static long aFromNodeIDFieldIndex = cpLink->Fields->_FindField( sindy::road_link::FROM_NODE_ID.c_str() );
	static long aToNodeIDFieldIndex = cpLink->Fields->_FindField( sindy::road_link::TO_NODE_ID.c_str() );
	long aFromNodeID = cpLink->GetValue( aFromNodeIDFieldIndex );
	long aToNodeID = cpLink->GetValue( aToNodeIDFieldIndex );
	long aOppositeNodeID = cNodeID == aFromNodeID ? aToNodeID : aFromNodeID;

	IFeaturePtr ipOppositeNode;
	if (cpRoadInfo)
	{
		cpRoadInfo->eRoadNode.SearchByObjectID( aOppositeNodeID, &ipOppositeNode );
	}
	return ipOppositeNode;
}
