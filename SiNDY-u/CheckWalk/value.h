#pragma once

#include "stdafx.h"
#include "HeightInfoMgr.h"

#include <ArcHelperEx/AheBuddyGlobals.h>

namespace judge_value {

	/* [Bug12501] 1km��50m �R�̎��^�ŉ��������q�b�g���Ă��܂����ߏC���B
		�K�؂Ȓl�Ƃ͌���Ȃ����A��q�b�g�͌��錩����
	*/
	const double LINK_RELATION_DISTANCE = 50.0;

	using namespace sindy;
	using namespace sindy::schema;
	/*
	 ���O�������������ǁA�R�[�h�l�ikey�j�ƃh���C���}�b�v����
	 �R�[�h�l('�R�[�h��')
	 ����
	 �R�[�h�l('<null>')
	 �̕������Ԃ�
	 ��j
	 100('������')
	 101('<null>')
	*/
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


	inline bool IsDefineWalkable( long walkable )
	{
		using namespace road_link::walkable_class;

		switch( walkable ){
			case kNotWalk:   ///< ���s�֎~
			case kWalkable:  ///< �l�ԋ��p
			case kWalkOnly:  ///< ���s�Ґ�p
			case kRightSideWalkWalkable:  ///< �E����������i�l�ԋ��p�j
			case kRightSideWalkNonWalkable: ///< �E����������i���s�֎~�j
			case kLeftSideWalkWalkable: ///< ������������i�l�ԋ��p�j
			case kLeftSideWalkNonWalkable: ///< ������������i���s�֎~�j
			case kBothSideWalk: ///< ������������
				return true;

			default:
				return false;
		}
	};

	inline bool IsWalkOnly( long walkable )
	{
		using namespace road_link::walkable_class;

		switch( walkable ){
			case kWalkOnly:  ///< ���s�Ґ�p
				return true;
			default:
				return false;
		}
	};

	inline CString IsSameWalkLinkAttr(const GeoClass& geoClass1, const GeoClass& geoClass2, const CFieldMap& fieldMap)
	{
		using namespace walk_link;

		if ( !geoClass1.CompareAt(fieldMap.FindField( kWalkClass ), geoClass2 ) ) return kWalkClass;
		if ( !geoClass1.CompareAt(fieldMap.FindField( kFloorMoveClass ), geoClass2) ) return kFloorMoveClass;
		if ( !geoClass1.CompareAt(fieldMap.FindField(kAutoWalkFlag), geoClass2) ) return kAutoWalkFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kStairFlag ), geoClass2 ) ) return kStairFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kSlopeFlag ), geoClass2 ) ) return kSlopeFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kEscalatorFlag ), geoClass2 ) ) return kEscalatorFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kArcadeFlag ), geoClass2 ) ) return kArcadeFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kOpenAreaFlag ), geoClass2 ) ) return kOpenAreaFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kTunnelFlag ), geoClass2 ) ) return kTunnelFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kRoadSurface ), geoClass2 ) ) return kRoadSurface;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kOneway ), geoClass2 ) ) return kOneway;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kOnewayRF ), geoClass2 ) ) return kOnewayRF;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kNoPassage ), geoClass2 ) ) return kNoPassage;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kNoPassageRF ), geoClass2 ) ) return kNoPassageRF;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kLinkRelationRF ), geoClass2 ) ) return kLinkRelationRF;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kWalkCode ), geoClass2 ) ) return kWalkCode;

		return _T("");
	};

	inline bool IsSameOneway( const GeoClass& geoClass1, const GeoClass& geoClass2, const long nodeID, const CFieldMap& fieldMap )
	{
		using namespace walk_link;
		using namespace walk_link::oneway;

		// �����t���͂Ȃ����߁A�l�����Ȃ�
		
		long onewayC1 = geoClass1.GetLongValue( fieldMap.FindField(kOneway ) );
		long onewayC2 =  geoClass2.GetLongValue(fieldMap.FindField( kOneway ) );
		if( onewayC1 == kNoOneway && onewayC2 == kNoOneway )
			return true;

		bool bFrom1 = false, bFrom2 = false;
		if ( nodeID == geoClass1.GetLongValue( fieldMap.FindField( kFromNodeID ) ) )
			bFrom1 = true;
		else if ( nodeID != geoClass1.GetLongValue( fieldMap.FindField( kToNodeID ) ) )
			return false; // From/To�Ƃ���v���Ȃ����OUT�i�Ȃ��͂��j

		if ( nodeID == geoClass2.GetLongValue( fieldMap.FindField( kFromNodeID ) ) )
			bFrom2 = true;
		else if ( nodeID != geoClass2.GetLongValue( fieldMap.FindField( kToNodeID ) ) )
			return false; // From/To�Ƃ���v���Ȃ����OUT�i�Ȃ��͂��j
			
		if( ( bFrom1 && ! bFrom2 ) || ( ! bFrom1 && bFrom2 ) ){ // From�ETo�Ő����̏ꍇ�i���������Ȃ�OK�j

			// ���E�� or �t�E�t �Ȃ�OK
			if( ( onewayC1 == kFore && onewayC2 == kFore ) || ( onewayC1 == kReverse && onewayC2 == kReverse ) )
				return true;

			return false;
		}
		
		// From�ETo�ŋt���̏ꍇ�i�t�����Ȃ�OK�j
		// ���E�t or �t�E�� �Ȃ�OK
		if( ( onewayC1 == kFore && onewayC2 == kReverse ) || ( onewayC1 == kReverse && onewayC2 == kFore ) )
			return true;

		return false;
	};

	inline bool IsSameWalkLinkFlagAttr( const GeoClass& geoClass1, const GeoClass& geoClass2, const CFieldMap& fieldMap )
	{
		using namespace walk_link;

		if ( !geoClass1.CompareAt( fieldMap.FindField( kAutoWalkFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kStairFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kSlopeFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kEscalatorFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kArcadeFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kOpenAreaFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kTunnelFlag ), geoClass2 ) ) return false;

		return true;
	}

	inline bool IsSimpleSameWalkLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2, const CFieldMap& fieldMap )
	{
		using namespace walk_link;

		if ( !geoClass1.CompareAt( fieldMap.FindField( kWalkClass ), geoClass2 ) ) return false;
		if ( !IsSameWalkLinkFlagAttr( geoClass1, geoClass2, fieldMap ) ) return false;

		return true;
	};

	inline bool IsDefineWalkLinkClass( long walkLinkClassCode, sindy::schema::walk_type::type_class::ECode dataType = walk_type::type_class::kGeneral )
	{
		using namespace walk_link::walk_class;
		// �ėp�����ꍇ
		if( dataType == walk_type::type_class::kGeneral )
		{
			// �X�L�[�}��`��̂��̂͑S��OK
			switch( walkLinkClassCode ){
				case kNone:               ///< �ݒ�Ȃ�
				case kWalkRoad:           ///< ����
				case kInferredCrossRoad:  ///< �݂Ȃ����f����
				case kCrossRoad:          ///< ���f����
				case kOtherWalkRoad:      ///< ���̑�����
				case kWalkableRoad:       ///< �l�ԋ��p�����N
				case kConnect:            ///< �ڑ������N
				case kWalkBridge:         ///< ������
				case kBridge:             ///< ��
				case kRailCrossing:       ///< ����
				case kOutdoor:            ///< �{�ݕ~�n���i���O�j
				case kIndoor:             ///< �{�ݕ~�n���i�����j
				case kInternalTicketGate: ///< �n����D��
				case kPlatForm:			  ///< �n��v���b�g�t�H�[��
				case kUnsurveyInternalTicketGate: ///< ���D���i���j
				case kUnderGround:        ///< �n���ʘH
				case kUnderGroundPath:    ///< ���f�n����
				case kInternalTicketGateUG:///< �n�����D��
				case kPlatFormUG:         ///< �n���v���b�g�t�H�[��
				case kWalkFerry:          ///< ���s�Ґ�p�t�F���[
					return true;

				default:
					return false;
			}
		}
		// �_�E���R���o�[�g�����ꍇ 
		if( dataType == walk_type::type_class::kDownConvert )
		{
			// �X�L�[�}��`��̂��̂͑S��OK
			switch( walkLinkClassCode ){
				case kNone:               ///< �ݒ�Ȃ�
				case kInferredCrossRoad:  ///< �݂Ȃ����f����
				case kCrossRoad:          ///< ���f����
				case kOtherWalkRoad:      ///< ���̑�����
				case kConnect:            ///< �ڑ������N
				case kLogicalConnection:  ///< ���W�J���R�l�N�V����
				case kWalkBridge:         ///< ������
				case kBridge:             ///< ��
				case kRailCrossing:       ///< ����
				case kOutdoor:            ///< �{�ݕ~�n���i���O�j
				case kIndoor:             ///< �{�ݕ~�n���i�����j
				case kUnderGround:        ///< �n���ʘH
				case kUnderGroundPath:    ///< ���f�n����
				case kWalkFerry:          ///< ���s�Ґ�p�t�F���[
					return true;

				default:
					return false;
			}			
		}
		return true;
	};

	inline bool IsOKWalkLinkClass( long walkLinkClassCode )
	{
		using namespace walk_link::walk_class;

		// �g�p������̂̂�OK
		if ( kNone != walkLinkClassCode && IsDefineWalkLinkClass( walkLinkClassCode ) ) {
			return true;
		}
		return false;
	};

	inline bool IsDefineFloorMoveClass( long floorMoveClass )
	{
		using namespace walk_link::floor_move_class;

		// �X�L�[�}��`��̂��̂͑S��OK
		switch( floorMoveClass ){
			case kNone:      ///< �㉺����
			case kForeUp:    ///< ���(������)
			case kReverseUp: ///< ���(�t����)
				return true;

			default:
				return false;
		}
	};

	inline bool IsDefineOnewayCode( long onewayCode )
	{
		using namespace walk_link::oneway;

		// �X�L�[�}��`��̂��̂͑S��OK
		switch( onewayCode ){
			case kNoOneway:           ///< ����ʍs�Ȃ�
			case kFore:               ///< �����Ȃ�����ʍs(������)
			case kReverse:            ///< �����Ȃ�����ʍs(�t����)
			case kForeWithReg:        ///< �����t������ʍs(������)
			case kReverseWithReg:     ///< �����t������ʍs(�t����)
			case kForeReverseWithReg: ///< �����t������ʍs(���t)
				return true;

			default:
				return false;
		}
	};

	inline bool IsOKOnewayCode( long onewayCode )
	{
		using namespace walk_link::oneway;

		switch( onewayCode ){
			case kNoOneway:           ///< ����ʍs�Ȃ�
			case kFore:               ///< �����Ȃ�����ʍs(������)
			case kReverse:            ///< �����Ȃ�����ʍs(�t����)
				return true;
			default:
				return false;
		}
	};



	inline bool IsDefineNoPassCode( long noPassCode )
	{
		using namespace walk_link::nopassage;

		// �X�L�[�}��`��̂��̂͑S��OK
		switch( noPassCode ){
			case kPassable:           ///< �ʍs�֎~�Ȃ�
			case kNoPassageWithReg:   ///< �����t���ʍs�֎~
				return true;

			default:
				return false;
		}
	};

	inline bool IsDefineRoadSurfaceClass( long roadSurfaceClass )
	{
		using namespace walk_link::road_surface_class;

		// �X�L�[�}��`��̂��̂͑S��OK
		switch( roadSurfaceClass ){
			case kPaved:   ///< �ܑ�
			case kUnpaved: ///< ���ܑ�
				return true;

			default:
				return false;
		}
	};

	inline bool IsDefineWalkCode( long walkCode )
	{
		// 0(�����l) or 1000�`999999��OK
		return ( walkCode == 0 || ( 1000 <= walkCode && walkCode <= 999999 ) );
	};

	inline bool IsOKFloorMoveClassAndFlag(long floorMoveClass, const long flag)
	{
		using namespace walk_link::floor_move_class;

		if (FLAG_ON != flag) return true;
		
		switch( floorMoveClass ){
			case kForeUp:    ///< ���(������)
			case kReverseUp: ///< ���(�t����)
				return true;

			case kNone:
			default:
				return false;
		}
	};

	inline bool IsOKMovingWalkwayAndArcade(long walkLinkClassCode, const long autowalkFlag, const long escalatorFlag, const long arcadeFlag)
	{
		using namespace walk_link::walk_class;

		// �`�F�b�N�ΏۊO�̕��s�Ҏ�ʃR�[�h�̏ꍇ�X�L�b�v
		switch( walkLinkClassCode ){
			case kIndoor:             ///< �{�ݕ~�n���i�����j
			case kUnderGround:        ///< �n���ʘH
			case kUnderGroundPath:    ///< ���f�n����
			case kInternalTicketGate: ///< �n����D��
			case kPlatForm:			  ///< �n��v���b�g�t�H�[��
			case kUnsurveyInternalTicketGate: ///< ���D���i���j
			case kInternalTicketGateUG:///< �n�����D��
			case kPlatFormUG:         ///< �n���v���b�g�t�H�[��
				return true;

			default:
				break;
		}

		// �G�X�J���[�^�[�t���O �܂��� �I�[�g�E�H�[�N�t���O��ON�ŉ����t���t���O��OFF�̏ꍇ
		if ((FLAG_ON == autowalkFlag || FLAG_ON == escalatorFlag) && FLAG_ON != arcadeFlag)
			return false;

		return true;
	}

	inline bool IsOKFloorMoveClassAndFlagOFF( long floorMoveClass, const long flag )
	{
		using namespace walk_link::floor_move_class;

		if( FLAG_ON == flag ) return true;

		switch( floorMoveClass ){
			case kForeUp:    ///< ���(������)
			case kReverseUp: ///< ���(�t����)
				return false;
			default:
				return true;
		}
	};

	inline bool IsOKOnewayCodeAndFlag(long oneway, const long flag)
	{
		using namespace walk_link::oneway;

		if ( FLAG_ON != flag ) return true;

		switch( oneway ){
			case kFore:					///< �����Ȃ�����ʍs(������)
			case kReverse:				///< �����Ȃ�����ʍs(�t����)
			case kForeWithReg:			///< �����t������ʍs(������)
			case kReverseWithReg:		///< �����t������ʍs(�t����)
			case kForeReverseWithReg:	///< �����t������ʍs(���t)
				return true;
			default:
				return false;
		}
	};


	inline bool IsOKOnewayCodeAndRelFlag( long oneway, const long relFlag )
	{
		using namespace walk_link::oneway;

		switch( oneway ){
			case kNoOneway:     ///< ����ʍs�Ȃ�
			case kFore:        ///< �����Ȃ�����ʍs(������)
			case kReverse:      ///< �����Ȃ�����ʍs(�t����)
				if ( FLAG_ON == relFlag ) return false;
				return true;
		
			default:
				if ( FLAG_ON != relFlag ) return false;
				return true;
		}
	};

	enum SimpleDirection
	{
		noDirect,
		force,
		reverce,
	};

	inline int GetOnwayDirection( long oneway )
	{
		using namespace walk_link::oneway;

		switch( oneway ){
		case kFore:					///< �����Ȃ�����ʍs(������)
		case kForeWithReg:			///< �����t������ʍs(������)
			return SimpleDirection::force;

		case kReverse:				///< �����Ȃ�����ʍs(�t����)
		case kReverseWithReg:		///< �����t������ʍs(�t����)
			return SimpleDirection::reverce;

		default:
			return SimpleDirection::noDirect;
		}
	}

	inline bool IsOKNoPassageCodeAndRelFlag( long noPass, const long relFlag )
	{
		using namespace walk_link::nopassage;

		switch( noPass ){
		case kPassable:     ///< �ʍs�֎~�Ȃ�
			if ( FLAG_ON == relFlag ) return false;
			return true;

		case kNoPassageWithReg:   ///< �����t���ʍs�֎~
		default:
			if ( FLAG_ON != relFlag ) return false;
			return true;
		}
	};

	inline bool IsOKWalkLinkClassAndFlag( long walkLinkClassCode, const long flag, walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;
		if(dataType == walk_type::type_class::kDownConvert)
		{
			switch( walkLinkClassCode ){
			case kOtherWalkRoad:      ///< ���̑�����
			case kWalkBridge:         ///< ������
			case kOutdoor:            ///< �{�ݕ~�n���i���O�j
			case kIndoor:             ///< �{�ݕ~�n���i�����j
			case kInternalTicketGate: ///< �n����D��
			case kPlatForm:           ///< �n��v���b�g�t�H�[��
			case kUnsurveyInternalTicketGate: ///< ���D���i���j
			case kUnderGround:        ///< �n���ʘH
			case kUnderGroundPath:    ///< ���f�n����
			case kInternalTicketGateUG:///< �n�����D��
			case kPlatFormUG:          ///< �n���v���b�g�t�H�[��
				return true;
			default:
				return false;
			}
		}
		else
		{
			switch( walkLinkClassCode ){
			case kWalkRoad:           ///< ����
			case kOtherWalkRoad:      ///< ���̑�����
			case kWalkBridge:         ///< ������
			case kOutdoor:            ///< �{�ݕ~�n���i���O�j
			case kIndoor:             ///< �{�ݕ~�n���i�����j
			case kInternalTicketGate: ///< �n����D��
			case kUnsurveyInternalTicketGate: ///< ���D���i���j
			case kUnderGround:        ///< �n���ʘH
			case kUnderGroundPath:    ///< ���f�n����
			case kInternalTicketGateUG:///< �n�����D��
				return true;
			default:
				return false;
			}
		}
	};

	inline bool IsOKWalkLinkClassAndArcadeFlag( long walkLinkClassCode, const long flag, walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;
		if(dataType == walk_type::type_class::kDownConvert)
		{
			switch( walkLinkClassCode ){
			case kInferredCrossRoad:  ///< �݂Ȃ����f����
			case kCrossRoad:          ///< ���f����
			case kOtherWalkRoad:      ///< ���̑�����
			case kWalkBridge:         ///< ������
			case kOutdoor:            ///< �{�ݕ~�n���i���O�j
			case kInternalTicketGate: ///< �n����D��
			case kPlatForm:			  ///< �n��v���b�g�t�H�[��
			case kUnsurveyInternalTicketGate:         ///< ���D���i���j
				return true;
			default:
				return false;
			}
		}
		else
		{
			switch( walkLinkClassCode ){
			case kWalkRoad:           ///< ����
			case kInferredCrossRoad:  ///< �݂Ȃ����f����
			case kCrossRoad:          ///< ���f����
			case kOtherWalkRoad:      ///< ���̑�����
			case kWalkBridge:         ///< ������
			case kOutdoor:            ///< �{�ݕ~�n���i���O�j
			case kPlatForm:			  ///< �n��v���b�g�t�H�[��
			case kPlatFormUG:         ///< �n���v���b�g�t�H�[��
				return true;
			default:
				return false;
			}
		}
	};

	inline bool IsOKWalkLinkClassAndOpenAreaFlag( long walkLinkClassCode, const long flag, walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;
		// �ėp�ł̃`�F�b�N�̂ݏꍇ�ɁA���s�ҕ\����ʃR�[�h�u�����v�̃`�F�b�N���s���B
		if(dataType == walk_type::type_class::kGeneral && walkLinkClassCode == kWalkRoad) return true;

		switch( walkLinkClassCode ){
			case kInferredCrossRoad:  ///< �݂Ȃ����f����
			case kCrossRoad:          ///< ���f����
			case kOtherWalkRoad:      ///< ���̑�����
			case kConnect:            ///< �ڑ������N
			case kWalkBridge:         ///< ������
			case kOutdoor:            ///< �{�ݕ~�n���i���O�j
			case kIndoor:             ///< �{�ݕ~�n���i�����j
			case kUnderGround:        ///< �n���ʘH
				return true;
			default:
				return false;
		}
	};

	inline bool IsOKWalkLinkClassAndTunnelFlag( long walkLinkClassCode, const long flag, walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;
		// �ėp�ł̃`�F�b�N�̂ݏꍇ�ɁA���s�ҕ\����ʃR�[�h�u�����v�̃`�F�b�N���s���B
		if(dataType == walk_type::type_class::kGeneral && walkLinkClassCode == kWalkRoad) return true;

		switch( walkLinkClassCode ){
			case kInferredCrossRoad:  ///< �݂Ȃ����f����
			case kCrossRoad:          ///< ���f����
			case kOtherWalkRoad:      ///< ���̑�����
			case kConnect:            ///< �ڑ������N
			case kWalkBridge:         ///< ������
			case kBridge:             ///< ��
			case kOutdoor:            ///< �{�ݕ~�n���i���O�j
				return true;
			default:
				return false;
		}
	};

	inline bool IsOKWalkLinkClassAndScrambleFlag( long walkLinkClassCode, const long flag )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;

		switch ( walkLinkClassCode ) {
			case kInferredCrossRoad:  ///< �݂Ȃ����f����
			case kCrossRoad:          ///< ���f����
			case kOtherWalkRoad:      ///< ���̑�����
				return true;
			default:
				return false;
		}
	};
	inline bool IsDefineWalkNodeClass( long walkNodeClass, walk_type::type_class::ECode dataType )
	{
		using namespace walk_node::node_class;

		// �ėp�����ꍇ
		if( dataType ==  walk_type::type_class::kGeneral )
		{
			// �X�L�[�}��`��̂��̂͑S��OK
			switch( walkNodeClass ){
				case kPublic:			///< ��ʃm�[�h
				case kTerminal:			///< �[�_�m�[�h
				case kAlter:			///< �����J�ړ_�m�[�h
				case kRoadNW:			///< ���H�m�v�ڑ��m�[�h
				case kGate:				///< �w�o�����m�[�h
				case kUndergroundGate:	///< �n���X�o�����m�[�h
				case kElevator:			///< �G���x�[�^�m�[�h
				case kKaisatsu:         ///< ��`���D�m�[�h
				case kCarPratform:      ///< ��ʎԏ�~��m�[�h
				case kBusPratform:      ///< �o�X��~��m�[�h
				case kTaxiPratform:     ///< �^�N�V�[��~��m�[�h
				case kParking:          ///< ���ԏ�o�����m�[�h
				case kDepartureGate:    ///< ��`�o���Q�[�g�m�[�h
				case kArrivalGate:      ///< ��`�����Q�[�g�m�[�h
				case kTicketGate:       ///< �w���D�m�[�h
				case kUnsurveyTicketGate:///< ���D�m�[�h�i���j
				case kOutline:			///< �}�s��m�[�h
					return true;

				default:
					return false;
			}
		}
		// �_�E���R���o�[�g�����ꍇ
		if( dataType ==  walk_type::type_class::kDownConvert )
		{
			// �X�L�[�}��`��̂��̂͑S��OK
			switch( walkNodeClass ){
				case kPublic:			///< ��ʃm�[�h
				case kTerminal:			///< �[�_�m�[�h
				case kAlter:			///< �����J�ړ_�m�[�h
				case kRoadNW:			///< ���H�m�[�h�ڑ��m�[�h
				case kElevator:			///< �G���x�[�^�m�[�h
				case kKaisatsu:         ///< ��`���D�m�[�h
				case kCarPratform:      ///< ��ʎԏ�~��m�[�h
				case kBusPratform:      ///< �o�X��~��m�[�h
				case kTaxiPratform:     ///< �^�N�V�[��~��m�[�h
				case kParking:          ///< ���ԏ�o�����m�[�h
				case kDepartureGate:    ///< ��`�o���Q�[�g�m�[�h
				case kArrivalGate:      ///< ��`�����Q�[�g�m�[�h
				case kConnectedRoad:    ///< ���H�����N�ڑ��m�[�h
				case kOutline:			///< �}�s��m�[�h
					return true;

				default:
					return false;
			}			
		}
		return true;
	};
	
	inline bool IsOKTerminalLink( long walkLinkClass, sindy::schema::walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;
		// �ėp�����ꍇ
		if( dataType == walk_type::type_class::kGeneral )
		{
			// 3�F���f�����A100�F�l�ԋ��p�����N�A200�F�������A202�F���؁A401:���f�n���� ��NG
			// OK�Ȃ��̂��L�q��NG�Ȃ��̂��L�q�Ƃ����B2:�݂Ȃ����f���� 500:���s�Ґ�p�t�F���[��NG��
			switch( walkLinkClass ){
				case kInferredCrossRoad:  ///< �݂Ȃ����f����
				case kCrossRoad:          ///< ���f����
				case kWalkableRoad:       ///< �l�ԋ��p�����N
				case kWalkBridge:         ///< ������
				case kRailCrossing:       ///< ����
				case kUnderGroundPath:    ///< ���f�n����
				case kWalkFerry:          ///< ���s�Ґ�p�t�F���[
					return false;
				default:
					return IsOKWalkLinkClass( walkLinkClass );
			}
		}
		// �_�E���R���o�[�g�����ꍇ
		if( dataType == walk_type::type_class::kDownConvert )
		{
			// 101�F�ڑ������N�A199�F���W�J���R�l�N�V�����A ��NG
			switch( walkLinkClass ){
				case kConnect:                   ///< �ڑ������N
				case kLogicalConnection:         ///< ���W�J���R�l�N�V����
					return false;
				default:
					return true;
			}		
		}
		return true;
	};

	// WalkNodeCheckFunction::checkConenctLink�����`���ڂ��Ċ֐���
	inline bool IsOKTerminalNode( long walkNodeClass )
	{
		using namespace walk_node::node_class;

		switch ( walkNodeClass ) {
			case kTerminal:			///< �[�_�m�[�h
			case kRoadNW:			///< ���H�m�v�ڑ��m�[�h
			case kKaisatsu:         ///< ��`���D�m�[�h
			case kCarPratform:      ///< ��ʎԏ�~��m�[�h
			case kBusPratform:      ///< �o�X��~��m�[�h
			case kTaxiPratform:     ///< �^�N�V�[��~��m�[�h
			case kParking:          ///< ���ԏ�o�����m�[�h
			case kDepartureGate:    ///< ��`�o���Q�[�g�m�[�h
			case kArrivalGate:      ///< ��`�����Q�[�g�m�[�h

			//�ȉ��A[bug 12687] �ɂ��ǉ�
			case kTicketGate:       ///< �w���D�m�[�h
			case kUnsurveyTicketGate:///< ���D�m�[�h�i���j
				return true;

			default:
				return false;
		}
	}

	inline bool IsTicketGate( long walkNodeClass )
	{
		using namespace walk_node::node_class;
		//���D�m�[�h�Ƃ��Ĉ����̂́A14�F�w���D�m�[�h�A98�F���D�m�[�h�i���j
		switch ( walkNodeClass )
		{
			case kTicketGate:			///< �w���D�m�[�h
			case kUnsurveyTicketGate:	///< ���D�m�[�h�i���j
				return true;
			default:
				return false;
		}
	}

	inline bool IsInternalTicketGateLink( long walkLinkClass )
	{
		using namespace walk_link::walk_class;
		//���D�������N�Ƃ��Ĉ����̂́A
		//302�F�n����D���A303�F�n��v���b�g�z�[���A399�F���D���i���j�A402�F�n�����D���A403�F�n���v���b�g�z�[��
		switch ( walkLinkClass ) {
			case kInternalTicketGate: ///< �n����D��
			case kPlatForm:			  ///< �n��v���b�g�t�H�[��
			case kUnsurveyInternalTicketGate: ///< ���D���i���j
			case kInternalTicketGateUG:///< �n�����D��
			case kPlatFormUG:         ///< �n���v���b�g�t�H�[��
				return true;
			default:
				return false;
		}
	}

	inline bool IsParalleCheckTargetlLink( long walkLinkClass )
	{
		using namespace walk_link::walk_class;
		//�G���[�R�[�h355,356:���s�����N�֘A�`�F�b�N�Ώ�
		//�w�{�ݕ~�n���i�����j�x�w�n����D���x�w�n��v���b�g�z�[���x�w�n���ʘH�x�w�n�����D���x�w�n���v���b�g�z�[���x
		switch ( walkLinkClass ) {
			case kIndoor:             ///< �{�ݕ~�n���i�����j
			case kInternalTicketGate: ///< �n����D��
			case kPlatForm:			  ///< �n��v���b�g�t�H�[��
			case kUnderGround:        ///< �n���ʘH
			case kInternalTicketGateUG:///< �n�����D��
			case kPlatFormUG:         ///< �n���v���b�g�t�H�[��
				return true;
			default:
				return false;
		}

	}

	inline bool IsUnderGroundLink( long walkLinkClass )
	{
		using namespace walk_link::walk_class;
		// �n�������N
		//�u�n���ʘH�v�u���f�n�����v�u�n�����D���v�u�n���v���b�g�z�[���v
		switch ( walkLinkClass ) {
			case kUnderGround:        ///< �n���ʘH
			case kUnderGroundPath:    ///< ���f�n����
			case kInternalTicketGateUG:///< �n�����D��
			case kPlatFormUG:         ///< �n���v���b�g�t�H�[��
				return true;
			default:
				return false;
		}

	}
	inline bool IsOKHumanCarAndRegFlag( long walkLinkClass, long onwayClass, long noPassageClass )
	{
		using namespace walk_link::walk_class;
		using namespace walk_link::oneway;
		using namespace walk_link::nopassage;

		if( walkLinkClass == kWalkableRoad )
			if( ! (onwayClass == kNoOneway && noPassageClass == kPassable ))
				return false;
		return true;

	};

	inline bool IsDefineHeight( long value )
	{
		return ( -2 <= value  && value <= 2 );
	};

	inline bool IsDefineLinkTable( long value )
	{
		using namespace height_node::link_table;
		switch (value)
		{
		case kNone: return true;
		case kRoadLink: return true;
		case kWalkLink: return true;
		default:
			break;
		}
		return false;
	};

	inline bool IsOKLinkTable( long value )
	{
		using namespace height_node::link_table;
		switch (value)
		{
		case kRoadLink: return true;
		case kWalkLink: return true;
		default:
			break;
		}
		return false;
	};

	inline bool IsOKBaseLinkWalkCode( long walkLinkClass)
	{
		using namespace walk_link::walk_class;

		// �����A���A���� ��OK
		switch( walkLinkClass ){
		case kWalkRoad:      ///< ����
		case kBridge:        ///< ��
		case kRailCrossing:  ///< ����
			return true;

		default:
			return false;
		}
	}

	inline bool IsOKHeightNodeWalkCode( long walkLinkClass, sindy::schema::walk_type::type_class::ECode dataType = walk_type::type_class::kGeneral)
	{
		using namespace walk_link::walk_class;
		// �ėp�����ꍇ
		if( dataType == walk_type::type_class::kGeneral )
		{
			// �ΏۊO���
			switch( walkLinkClass ){
				case kWalkableRoad:					///< �l�ԋ��p�����N
				case kUnderGround:					///< �n���ʘH
				case kConnect:						///< �ڑ������N
				case kUnderGroundPath:			    ///< ���f�n����
				case kInternalTicketGateUG:			///< �n�����D��
				case kPlatFormUG:					///< �n���v���b�g�t�H�[��
					return false;

				default:
					return true;
			}
		}
		// �_�E���R���o�[�g�����ꍇ
		if( dataType == walk_type::type_class::kDownConvert )
		{
			// �ΏۊO���
			switch( walkLinkClass ){
				case kConnect:						///< �ڑ������N
				case kLogicalConnection:            ///< ���W�J���R�l�N�V����
				case kUnderGround:                  ///< �n���ʘH
				case kUnderGroundPath:			    ///< ���f�n����
					return false;

				default:
					return true;
			}			
		}
		return true;
	}


	inline bool IsOKCompareHeightRef(long height, int refHeight)
	{
		bool result = false;
		switch (refHeight)
		{
		case Height::high:
		case Height::higher:
			result = (refHeight <= height );
			break;
		case Height::low:
		case Height::loweer:
			result = (refHeight >= height );
			break;
		default:
			result = true;
		}
		return result;
	}

	inline bool IsOKBuddyRelation( const IGeometryPtr& baseLinkObj, const std::map<long, IGeometryPtr>& relLinks, std::list<long>& errorRelLinks, double& maxDistance )
	{
		bool existOKRelation = false;
		for( const auto& relLink : relLinks )
		{
			LinkRelationPosition position = LinkRelationPosition::kNotFound;
			double minMeter = 0.0;
			
			// ������N�Ɗ֘A�����N���t�^�����Ő���ɕR�t����
			if( AheJudgeLinkRelation( (IPolylinePtr)baseLinkObj, (IPolylinePtr)relLink.second, LINK_RELATION_DISTANCE, position, minMeter )
				&& position != LinkRelationPosition::kNotFound )
			{
				existOKRelation = true;
				if( maxDistance < minMeter)
					maxDistance = minMeter;
			}
			else
				errorRelLinks.push_back(relLink.first);
		}

		return existOKRelation;
	}

	inline bool IsOKFlag( const long flag )
	{
		if( 0 == flag || 1 == flag )
			return true;
		return false;
	};


	/**
	* @brief �����[�X�Ώۂ̓��H�����N��
	* @retval true �����[�X�Ώ�
	* @retval true �����[�X�ΏۊO
	*/
	inline bool IsOKReleaseRoadLink( const GeoClass& geoClass, const CFieldMap& fieldMap )
	{
		using namespace road_link;

		// ���H��ʂ�400���� ���� �׊X�H��ƔN�x�R�[�h��9����Ȃ�
		return geoClass.GetLongValue( fieldMap.FindField( kRoadClass ) ) < 400 &&
			 geoClass.GetLongValue( fieldMap.FindField( kAlleyMainte ) ) != alley_mainte::k2005;
	}

	/**
	* �u���s�Ҏ�ʂ��O��̃����N�ƈقȂ�v�i�G���[�R�[�h353�j�̃`�F�b�N�Ώۂ�
	* @param walkLinkClass    [in] ���s�҃����N�̎�ʃR�[�h
	* @retval true  �`�F�b�N�Ώ�
	* @retval false �`�F�b�N�ΏۊO
	*/
	inline bool IsTargetOfCheck353(const long walkLinkClass)
	{
		using namespace walk_link::walk_class;

		switch( walkLinkClass ){
		case kWalkBridge:       ///< ������
		case kOutdoor:          ///< �{�ݕ~�n���i���O�j
		case kIndoor:           ///< �{�ݕ~�n���i�����j
		case kUnderGround:      ///< �n���ʘH
		case kUnderGroundPath:  ///< ���f�n����
			return true;

		default:
			return false;
		}
	}
};