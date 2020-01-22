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

#pragma once

#include "stdafx.h"

namespace judge_value {

	using namespace sindy::schema;
	using namespace sindy::schema::sj;

	inline CString IsFindObj( long key, const std::map<long, CString>& objMap )
	{
		CString retStr;
		retStr.Format( _T("%ld"), key );
		auto itr = objMap.find( key );
		if( itr != objMap.end() )
			retStr.AppendFormat( _T("('%s')"), itr->second );
		
		else
			retStr.AppendFormat( _T("('<null>')"));

		return retStr;
	};

	inline bool IsDefineRailLinkClass( long railLinkClass )
	{
		using namespace railway_link::link_class;
		
		switch( railLinkClass ){

			case kDivideByPoi: //!< �S��POI��ʂŕ���
			case kFreightLine: //!< �ݕ���
			case kSideTrack:   //!< �S��������
				return true;

			default:
				return false;
		}	
	};

	inline bool IsDefineDispScale( long dispScale )
	{
		using namespace railway_link::disp_scale;

		switch( dispScale ){

			case kS1toS4: //!< S1�`S4�ŕ\��
			case kS2toS4: //!< S2�`S4�ŕ\��
			case kS3toS4: //!< S3�`S4�ŕ\��
			case k4:      //!< S4�ŕ\��
				return true;

			default:
				return false;
		}
	};

	inline bool IsOKFlag( const CString& flag )
	{
		if( 0 == flag.CompareNoCase( _T("0") ) || 0 == flag.CompareNoCase( _T("1") ) )
			return true;
		return false;
	};

	inline bool IsSameRailLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	{
		using namespace railway_link;

		try{
			if( 0 != geoClass1.attr.at( kRailwayClass_C ).CompareNoCase( geoClass2.attr.at( kRailwayClass_C ) ) ) return false;
			if( 0 != geoClass1.attr.at( kDispScale_C    ).CompareNoCase( geoClass2.attr.at( kDispScale_C    ) ) ) return false;
			if( 0 != geoClass1.attr.at( kUnderGround_F  ).CompareNoCase( geoClass2.attr.at( kUnderGround_F  ) ) ) return false;

			return true;
		}
		catch( ... ){
			return false;
		}
	};

	inline CString GetDiffRailLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	{
		using namespace railway_link;

		try{
			if( 0 != geoClass1.attr.at( kRailwayClass_C ).CompareNoCase( geoClass2.attr.at( kRailwayClass_C ) ) ) return kRailwayClass_C;
			if( 0 != geoClass1.attr.at( kDispScale_C    ).CompareNoCase( geoClass2.attr.at( kDispScale_C    ) ) ) return kDispScale_C;
			if( 0 != geoClass1.attr.at( kUnderGround_F  ).CompareNoCase( geoClass2.attr.at( kUnderGround_F  ) ) ) return kUnderGround_F;

			return _T("");
		}
		catch( ... ){
			return _T("");
		}
	};

	inline bool IsDefineRailNodeClass( long railNodeClass )
	{
		using namespace railway_node::node_class;

		switch( railNodeClass ){
			case kNone:     //!< �@�\�Ȃ��m�[�h
			case kTerminal: //!< �[�_�m�[�h
			case kStation:  //!< �wPOI�m�[�h
			case kAlter:    //!< �����ω��_�m�[�h
			case kBranch:   //!< ����m�[�h
			case kOutline:  //!< �}�s��m�[�h
				return true;

			default:
				return false;
		}
	};

	inline bool IsOKTerminalNode( long railNodeClass )
	{
		using namespace railway_node::node_class;

		switch( railNodeClass ){
			case kTerminal: //!< �[�_�m�[�h
			case kStation:  //!< �wPOI�m�[�h
				return true;

			case kNone:     //!< �@�\�Ȃ��m�[�h
			case kAlter:    //!< �����ω��_�m�[�h
			case kBranch:   //!< ����m�[�h
			case kOutline:  //!< �}�s��m�[�h
			default:
				return false;
		}
	};

	inline bool IsOKBranchNode( long railNodeClass )
	{
		using namespace railway_node::node_class;

		switch( railNodeClass ){
			case kBranch:   //!< ����m�[�h
			case kStation:  //!< �wPOI�m�[�h
				return true;

			case kNone:     //!< �@�\�Ȃ��m�[�h
			case kAlter:    //!< �����ω��_�m�[�h
			case kTerminal: //!< �[�_�m�[�h
			case kOutline:  //!< �}�s��m�[�h
			default:
				return false;
		}
	};

	inline bool IsOKAlterNode( long railNodeClass )
	{
		using namespace railway_node::node_class;

		switch( railNodeClass ){
			case kAlter:    //!< �����ω��_�m�[�h
			case kBranch:   //!< ����m�[�h
			case kStation:  //!< �wPOI�m�[�h
				return true;

			case kNone:     //!< �@�\�Ȃ��m�[�h
			case kOutline:  //!< �}�s��m�[�h
			case kTerminal: //!< �[�_�m�[�h
			default:
				return false;
		}
	}



	inline long GetLongValue( const GeoClass& geoClass, const CString& field )
	{
		try{
			auto& cStrValue = geoClass.attr.at( field );
			if( cStrValue.IsEmpty() )
				return -1;
			long retValue = _ttol( cStrValue );
			return retValue;
		}
		catch( ... ){
			return -1;
		}
	}


	//inline bool IsDefineWalkable( long walkable )
	//{
	//	using namespace road_link::walkable_class;

	//	switch( walkable ){
	//		case kNotWalk:   ///< ���s�֎~
	//		case kHumanCar:  ///< �l�ԋ��p
	//		case kWalkOnly:  ///< ���s�Ґ�p
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsWalkOnly( long walkable )
	//{
	//	using namespace road_link::walkable_class;

	//	switch( walkable ){
	//		case kWalkOnly:  ///< ���s�Ґ�p
	//			return true;
	//		case kNotWalk:   ///< ���s�֎~
	//		case kHumanCar:  ///< �l�ԋ��p
	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsSameRoadLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	//{
	//	using namespace road_link;

	//	try{
	//		if( 0 != geoClass1.attr.at( kRoadClass     ).CompareNoCase( geoClass2.attr.at( kRoadClass     ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kNaviClass     ).CompareNoCase( geoClass2.attr.at( kNaviClass     ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kMainLinkClass ).CompareNoCase( geoClass2.attr.at( kMainLinkClass ) ) ) return false;

	//		return true;
	//	}
	//	catch( ... ){
	//		return false;
	//	}
	//};
	//
	//inline CString IsSameWalkLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	//{
	//	using namespace walk_link;
	//	
	//	try{
	//		if( 0 != geoClass1.attr.at( kWalkClass      ).CompareNoCase( geoClass2.attr.at( kWalkClass      ) ) ) return kWalkClass;
	//		if( 0 != geoClass1.attr.at( kFloorMoveClass ).CompareNoCase( geoClass2.attr.at( kFloorMoveClass ) ) ) return kFloorMoveClass;
	//		if( 0 != geoClass1.attr.at( kAutoWalkFlag   ).CompareNoCase( geoClass2.attr.at( kAutoWalkFlag   ) ) ) return kAutoWalkFlag;
	//		if( 0 != geoClass1.attr.at( kStairFlag      ).CompareNoCase( geoClass2.attr.at( kStairFlag      ) ) ) return kStairFlag;
	//		if( 0 != geoClass1.attr.at( kSlopeFlag      ).CompareNoCase( geoClass2.attr.at( kSlopeFlag      ) ) ) return kSlopeFlag;
	//		if( 0 != geoClass1.attr.at( kEscalatorFlag  ).CompareNoCase( geoClass2.attr.at( kEscalatorFlag  ) ) ) return kEscalatorFlag;
	//		if( 0 != geoClass1.attr.at( kArcadeFlag     ).CompareNoCase( geoClass2.attr.at( kArcadeFlag     ) ) ) return kArcadeFlag;
	//	//	if( 0 != geoClass1.attr.at( kOneway         ).CompareNoCase( geoClass2.attr.at( kOneway         ) ) ) return kOneway;
	//		if( 0 != geoClass1.attr.at( kOnewayRF       ).CompareNoCase( geoClass2.attr.at( kOnewayRF       ) ) ) return kOnewayRF;
	//		if( 0 != geoClass1.attr.at( kNoPassage      ).CompareNoCase( geoClass2.attr.at( kNoPassage      ) ) ) return kNoPassage;
	//		if( 0 != geoClass1.attr.at( kNoPassageRF    ).CompareNoCase( geoClass2.attr.at( kNoPassageRF    ) ) ) return kNoPassageRF;
	//		if( 0 != geoClass1.attr.at( kSubWalkClass   ).CompareNoCase( geoClass2.attr.at( kSubWalkClass   ) ) ) return kSubWalkClass;
	//		if( 0 != geoClass1.attr.at( kRoadClass      ).CompareNoCase( geoClass2.attr.at( kRoadClass      ) ) ) return kRoadClass;
	//		if( 0 != geoClass1.attr.at( kRoadNo         ).CompareNoCase( geoClass2.attr.at( kRoadNo         ) ) ) return kRoadNo;
	//		if( 0 != geoClass1.attr.at( kRoadCode       ).CompareNoCase( geoClass2.attr.at( kRoadCode       ) ) ) return kRoadCode;

	//		return _T("");
	//	}
	//	catch( ... ){
	//		return _T("");
	//	}
	//};

	//inline bool IsSameOneway( const GeoClass& geoClass1, const GeoClass& geoClass2, long nodeID )
	//{
	//	using namespace walk_link;
	//	using namespace walk_link::oneway;

	//	// �����t���͂Ȃ����߁A�l�����Ȃ�
	//	
	//	try{
	//		long onewayC1 = _ttol(geoClass1.attr.at( kOneway ));
	//		long onewayC2 = _ttol(geoClass2.attr.at( kOneway ));
	//		if( onewayC1 == kNoOneway && onewayC2 == kNoOneway )
	//			return true;

	//		bool bFrom1 = false, bFrom2 = false;
	//		CString strNodeID;
	//		strNodeID.Format( _T("%ld"), nodeID );
	//		if( 0 == geoClass1.attr.at( kFromNodeID ).CompareNoCase( strNodeID ) )
	//			bFrom1 = true;
	//		else if( 0 != geoClass1.attr.at( kToNodeID ).CompareNoCase( strNodeID ) )
	//			return false; // From/To�Ƃ���v���Ȃ����OUT�i�Ȃ��͂��j

	//		if( 0 == geoClass2.attr.at( kFromNodeID ).CompareNoCase( strNodeID ) )
	//			bFrom2 = true;
	//		else if( 0 != geoClass2.attr.at( kToNodeID ).CompareNoCase( strNodeID ) )
	//			return false; // From/To�Ƃ���v���Ȃ����OUT�i�Ȃ��͂��j
	//		
	//		if( ( bFrom1 && ! bFrom2 ) || ( ! bFrom1 && bFrom2 ) ){ // From�ETo�Ő����̏ꍇ�i���������Ȃ�OK�j

	//			// ���E�� or �t�E�t �Ȃ�OK
	//			if( ( onewayC1 == kFore && onewayC2 == kFore ) || ( onewayC1 == kReverse && onewayC2 == kReverse ) )
	//				return true;

	//			return false;
	//		}
	//		else{ // From�ETo�ŋt���̏ꍇ�i�t�����Ȃ�OK�j

	//			// ���E�t or �t�E�� �Ȃ�OK
	//			if( ( onewayC1 == kFore && onewayC2 == kReverse ) || ( onewayC1 == kReverse && onewayC2 == kFore ) )
	//				return true;

	//			return false;
	//		}
	//	}
	//	catch( ... ){
	//		return false;
	//	}
	//};

	//inline bool IsSimpleSameWalkLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	//{
	//	using namespace walk_link;
	//	try{
	//		if( 0 != geoClass1.attr.at( kWalkClass      ).CompareNoCase( geoClass2.attr.at( kWalkClass      ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kAutoWalkFlag   ).CompareNoCase( geoClass2.attr.at( kAutoWalkFlag   ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kStairFlag      ).CompareNoCase( geoClass2.attr.at( kStairFlag      ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kSlopeFlag      ).CompareNoCase( geoClass2.attr.at( kSlopeFlag      ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kEscalatorFlag  ).CompareNoCase( geoClass2.attr.at( kEscalatorFlag  ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kArcadeFlag     ).CompareNoCase( geoClass2.attr.at( kArcadeFlag     ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kSubWalkClass   ).CompareNoCase( geoClass2.attr.at( kSubWalkClass   ) ) ) return false;

	//		return true;
	//	}
	//	catch( ... ){
	//		return false;
	//	}
	//};

	//inline bool IsDefineWalkLinkClass( long walkLinkClassCode )
	//{
	//	using namespace walk_link::walk_class;

	//	// �X�L�[�}��`��̂��̂͑S��OK
	//	switch( walkLinkClassCode ){
	//		case kNone:          ///< �ݒ�Ȃ�
	//		case kWalkRoad:      ///< ����
	//		case kOtherWalkRoad: ///< ���̑�����	 
	//		case kCrossRoad:     ///< ���f����
	//		case kWalkBridge:    ///< ������
	//		case kPedestrian:    ///< �؃f�X�g���A���f�b�L
	//		case kConnect:       ///< �ڑ������N
	//		case kHumanCar:      ///< �l�ԋ��p�����N
	//		case kStation:       ///< �w�\��
	//		case kIndoor:        ///< �{�ݓ��i�����j
	//		case kUnderGround:   ///< �n���ʘH
	//		case kOutdoor:       ///< �{�ݕ~�n���i���O�j
	//		case kPark:          ///< ����
	//		case kOpenspace:     ///< �L��
	//		case kRailCrossing:  ///< ����
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsOKWalkLinkClass( long walkLinkClassCode )
	//{
	//	using namespace walk_link::walk_class;

	//	// �g�p������̂̂�OK
	//	switch( walkLinkClassCode ){
	//		case kWalkRoad:      ///< ����	 
	//		case kCrossRoad:     ///< ���f����
	//		case kWalkBridge:    ///< ������
	//		case kHumanCar:      ///< �l�ԋ��p�����N
	//		case kIndoor:        ///< �{�ݓ��i�����j
	//		case kUnderGround:   ///< �n���ʘH
	//		case kOutdoor:       ///< �{�ݕ~�n���i���O�j
	//		case kRailCrossing:  ///< ����
	//			return true;

	//		case kNone:          ///< �ݒ�Ȃ�
	//		case kOtherWalkRoad: ///< ���̑�����	 
	//		case kPedestrian:    ///< �؃f�X�g���A���f�b�L
	//		case kConnect:       ///< �ڑ������N
	//		case kStation:       ///< �w�\��
	//		case kPark:          ///< ����
	//		case kOpenspace:     ///< �L��
	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineFloorMoveClass( long floorMoveClass )
	//{
	//	using namespace walk_link::floor_move_class;

	//	// �X�L�[�}��`��̂��̂͑S��OK
	//	switch( floorMoveClass ){
	//		case kNone:      ///< �㉺����
	//		case kForeUp:    ///< ���(������)
	//		case kReverseUp: ///< ���(�t����)
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineOnewayCode( long onewayCode )
	//{
	//	using namespace walk_link::oneway;

	//	// �X�L�[�}��`��̂��̂͑S��OK
	//	switch( onewayCode ){
	//		case kNoOneway:           ///< ����ʍs�Ȃ�
	//		case kFore:               ///< �����Ȃ�����ʍs(������)
	//		case kReverse:            ///< �����Ȃ�����ʍs(�t����)
	//		case kForeWithReg:        ///< �����t������ʍs(������)
	//		case kReverseWithReg:     ///< �����t������ʍs(�t����)
	//		case kForeReverseWithReg: ///< �����t������ʍs(���t)
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineNoPassCode( long noPassCode )
	//{
	//	using namespace walk_link::nopassage;

	//	// �X�L�[�}��`��̂��̂͑S��OK
	//	switch( noPassCode ){
	//		case kPassable:           ///< �ʍs�֎~�Ȃ�
	//		case kNoPassageWithReg:   ///< �����t���ʍs�֎~
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineSubWalkClass( long subWalkClass )
	//{
	//	using namespace walk_link::subwalk_class;

	//	// �X�L�[�}��`��̂��̂͑S��OK
	//	switch( subWalkClass ){
	//		case kNone:   ///< �Ȃ�
	//		case kWatari: ///< �n�胊���N
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineWalkRoadClass( long walkRoadClass )
	//{
	//	using namespace walk_link::walk_road_class;

	//	// �X�L�[�}��`��̂��̂͑S��OK
	//	switch( walkRoadClass ){
	//		case kNone:                 ///< �\����ʂȂ�
	//		case kCountry:              ///< ��ʍ���
	//		case kPrefectureMain:       ///< ��v�n�����i�s�{���j
	//		case kCityMain:             ///< ��v�n�����i�w��s�j
	//		case kPrefectureNormal:     ///< ��ʓs���{����
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineRoadNo( long roadNo )
	//{
	//	// 0�`9999��OK
	//	if( 0 <= roadNo && roadNo <= 9999 )
	//		return true;
	//	
	//	return false;
	//};

	//inline bool IsDefineRoadCode( long roadCode )
	//{
	//	// 0(�����l) or 1000�`999999��OK
	//	if( roadCode == 0 || ( 1000 <= roadCode && roadCode <= 999999 ) )
	//		return true;

	//	return false;
	//};

	//inline bool IsOKWalkRoadClassAndRoadNo( long walkRoadClass, long roadNo )
	//{
	//	using namespace walk_link::walk_road_class;
	//	
	//	switch( walkRoadClass ){
	//		case kCountry:              ///< ��ʍ���
	//		case kPrefectureMain:       ///< ��v�n�����i�s�{���j
	//		case kCityMain:             ///< ��v�n�����i�w��s�j
	//		case kPrefectureNormal:     ///< ��ʓs���{����
	//			if( 0 < roadNo && roadNo <= 9999 )
	//				return true;
	//			
	//			return false;
	//			
	//		case kNone:                 ///< �\����ʂȂ�
	//		default:
	//			if( 0 < roadNo && roadNo <= 9999 )
	//				return false;

	//			return true;
	//	}
	//};

	//inline bool IsOKFloorMoveClassAndFlag( long floorMoveClass, const CString& flag )
	//{
	//	using namespace walk_link::floor_move_class;

	//	if( 0 == flag.CompareNoCase( _T("1") ) ){
	//		switch( floorMoveClass ){
	//		case kForeUp:    ///< ���(������)
	//		case kReverseUp: ///< ���(�t����)
	//			return true;

	//		case kNone:
	//		default:
	//			return false;
	//		}
	//	}
	//	return true;
	//};



	//inline bool IsDefineWalkNodeClass( long walkNodeClass )
	//{
	//	using namespace walk_node::node_class;

	//	// �X�L�[�}��`��̂��̂͑S��OK
	//	switch( walkNodeClass ){
	//		case kPublic:    ///< ��ʃm�[�h
	//		case kTerminal:  ///< �[�_�m�[�h
	//		case kAlter:     ///< �����J�ړ_�m�[�h
	//		case kRoadNW:    ///< ���H�m�v�ڑ��m�[�h
	//		case kGate:      ///< �w�o�����ڑ��m�[�h
	//		case kUnderGate: ///< �n���ʘH�o�����m�[�h
	//		case kElevator:  ///< �G���x�[�^�m�[�h
	//		case kOutline:   ///< �}�s��m�[�h
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsOKTerminalLink( long walkLinkClass )
	//{
	//	using namespace walk_link::walk_class;

	//	// 2�F���f�����A3�F�������A6�F�l�ԋ��p�����N�A23�F���� ��NG
	//	switch( walkLinkClass ){
	//		case kWalkRoad:      ///< ����
	//		case kOtherWalkRoad: ///< ���̑�����
	//		case kPedestrian:    ///< �؃f�X�g���A���f�b�L
	//		case kConnect:       ///< �ڑ������N
	//		case kStation:       ///< �w�\��
	//		case kIndoor:        ///< �{�ݓ��i�����j
	//		case kUnderGround:   ///< �n���ʘH
	//		case kOutdoor:       ///< �{�ݕ~�n���i���O�j
	//		case kPark:          ///< ����
	//		case kOpenspace:     ///< �L��
	//			return true;

	//		case kCrossRoad:     ///< ���f����
	//		case kWalkBridge:    ///< ������
	//		case kHumanCar:      ///< �l�ԋ��p�����N
	//		case kRailCrossing:  ///< ����
	//		default:
	//			return false;
	//	}
	//};



	////inline bool IsZeroNull( const CString& value )
	////{
	////	if( value.IsEmpty() || 0 == value.CompareNoCase( _T("0") ) )
	////		return true;
	////	return false;
	////};




};
