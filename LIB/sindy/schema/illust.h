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

#ifndef SINDY_SCHEMA_ILLUST_H_
#define SINDY_SCHEMA_ILLUST_H_

#include <iostream>
#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

/// �C���X�g�����N�t�B�[�`���N���X
namespace illust_link {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kIllustName[];            //!< �摜�t�@�C����(�t�@�C�����𕪊�����SiNDY��ɓ����̂ŁASiNDY��ɂ̓t�B�[���h�����݂��Ȃ�)
	extern const _TCHAR kIllustClass[];           //!< �C���X�g���
	extern const _TCHAR kCustomerCode[];          //!< �d������R�[�h
	extern const _TCHAR kRoadCode[];              //!< �H���ԍ�
	extern const _TCHAR kRoadName[];              //!< �H������
	extern const _TCHAR kRoadSubName[];           //!< �H��������
	extern const _TCHAR kRoadSeq[];               //!< �H�����V�[�P���X
	extern const _TCHAR kFacilName[];             //!< �{�ݖ���
	extern const _TCHAR kFacilSubName[];          //!< �{�ݕ�����
	extern const _TCHAR kIdentificationName[];    //!< ���ʗp����
	extern const _TCHAR kGuideRoute[];            //!< ����
	extern const _TCHAR kUpDownC[];               //!< �㉺���R�[�h
	extern const _TCHAR kGuideSeq[];              //!< �ē��V�[�P���X
	extern const _TCHAR kSuccessiveCode[];        //!< �A������R�[�h
	extern const _TCHAR kSingleBranchCode[];      //!< �P����R�[�h
	extern const _TCHAR kIrregularCode[];         //!< �C���M�����[����R�[�h
	extern const _TCHAR kTunnelCode[];            //!< �g���l���R�[�h
	extern const _TCHAR kStraightCode[];          //!< ���i�����R�[�h
	extern const _TCHAR kBoundaryCode[];          //!< �������E�R�[�h
	extern const _TCHAR kBranchDirectionCode[];   //!< ��������R�[�h
	extern const _TCHAR kLaneBranchPatternCode[]; //!< �Ԑ�����p�^�[���R�[�h
	extern const _TCHAR kDirguideExistCode[];     //!< ���ʈē��ŔL���R�[�h
	extern const _TCHAR kDirectionCode[];         //!< �ē������R�[�h

	extern const _TCHAR kBranch1PatternCode[];           //!< ����1_�p�^�[���R�[�h
	extern const _TCHAR kBranch1LaneCountMainBefore[];   //!< ����1_�{���ω��O�Ԑ���
	extern const _TCHAR kBranch1LaneCountMainAfter[];    //!< ����1_�{���ω���Ԑ���
	extern const _TCHAR kBranch1LaneCountLeftBefore[];   //!< ����1_������ω��O�Ԑ���
	extern const _TCHAR kBranch1LaneCountLeftAfter[];    //!< ����1_������ω���Ԑ���
	extern const _TCHAR kBranch1LaneCountCenterBefore[]; //!< ����1_��������ω��O�Ԑ���
	extern const _TCHAR kBranch1LaneCountCenterAfter[];  //!< ����1_��������ω���Ԑ���
	extern const _TCHAR kBranch1LaneCountRightBefore[];  //!< ����1_�E����ω��O�Ԑ���
	extern const _TCHAR kBranch1LaneCountRightAfter[];   //!< ����1_�E����ω���Ԑ���
	extern const _TCHAR kBranch2PatternCode[];           //!< ����2_�p�^�[���R�[�h
	extern const _TCHAR kBranch2LaneCountMainBefore[];   //!< ����2_�{���ω��O�Ԑ���
	extern const _TCHAR kBranch2LaneCountMainAfter[];    //!< ����2_�{���ω���Ԑ���
	extern const _TCHAR kBranch2LaneCountLeftBefore[];   //!< ����2_������ω��O�Ԑ���
	extern const _TCHAR kBranch2LaneCountLeftAfter[];    //!< ����2_������ω���Ԑ���
	extern const _TCHAR kBranch2LaneCountCenterBefore[]; //!< ����2_��������ω��O�Ԑ���
	extern const _TCHAR kBranch2LaneCountCenterAfter[];  //!< ����2_��������ω���Ԑ���
	extern const _TCHAR kBranch2LaneCountRightBefore[];  //!< ����2_�E����ω��O�Ԑ���
	extern const _TCHAR kBranch2LaneCountRightAfter[];   //!< ����2_�E����ω���Ԑ���
	extern const _TCHAR kBranch3PatternCode[];           //!< ����3_�p�^�[���R�[�h
	extern const _TCHAR kBranch3LaneCountMainBefore[];   //!< ����3_�{���ω��O�Ԑ���
	extern const _TCHAR kBranch3LaneCountMainAfter[];    //!< ����3_�{���ω���Ԑ���
	extern const _TCHAR kBranch3LaneCountLeftBefore[];   //!< ����3_������ω��O�Ԑ���
	extern const _TCHAR kBranch3LaneCountLeftAfter[];    //!< ����3_������ω���Ԑ���
	extern const _TCHAR kBranch3LaneCountCenterBefore[]; //!< ����3_��������ω��O�Ԑ���
	extern const _TCHAR kBranch3LaneCountCenterAfter[];  //!< ����3_��������ω���Ԑ���
	extern const _TCHAR kBranch3LaneCountRightBefore[];  //!< ����3_�E����ω��O�Ԑ���
	extern const _TCHAR kBranch3LaneCountRightAfter[];   //!< ����3_�E����ω���Ԑ���
	extern const _TCHAR kBranch4PatternCode[];           //!< ����4_�p�^�[���R�[�h
	extern const _TCHAR kBranch4LaneCountMainBefore[];   //!< ����4_�{���ω��O�Ԑ���
	extern const _TCHAR kBranch4LaneCountMainAfter[];    //!< ����4_�{���ω���Ԑ���
	extern const _TCHAR kBranch4LaneCountLeftBefore[];   //!< ����4_������ω��O�Ԑ���
	extern const _TCHAR kBranch4LaneCountLeftAfter[];    //!< ����4_������ω���Ԑ���
	extern const _TCHAR kBranch4LaneCountCenterBefore[]; //!< ����4_��������ω��O�Ԑ���
	extern const _TCHAR kBranch4LaneCountCenterAfter[];  //!< ����4_��������ω���Ԑ���
	extern const _TCHAR kBranch4LaneCountRightBefore[];  //!< ����4_�E����ω��O�Ԑ���
	extern const _TCHAR kBranch4LaneCountRightAfter[];   //!< ����4_�E����ω���Ԑ���

	extern const _TCHAR kCrossRoadFront_A[];             //!< �ڑ����H_��O_A
	extern const _TCHAR kCrossRoadFront_B[];             //!< �ڑ����H_��O_B
	extern const _TCHAR kCrossRoadFront_C[];             //!< �ڑ����H_��O_C
	extern const _TCHAR kCrossRoadFront_D[];             //!< �ڑ����H_��O_D
	extern const _TCHAR kCrossRoadFront_E[];             //!< �ڑ����H_��O_E
	extern const _TCHAR kCrossRoadFront_F[];             //!< �ڑ����H_��O_F
	extern const _TCHAR kCrossRoadMiddle_A[];            //!< �ڑ����H_����_A
	extern const _TCHAR kCrossRoadMiddle_B[];            //!< �ڑ����H_����_B
	extern const _TCHAR kCrossRoadMiddle_C[];            //!< �ڑ����H_����_C
	extern const _TCHAR kCrossRoadMiddle_D[];            //!< �ڑ����H_����_D
	extern const _TCHAR kCrossRoadMiddle_E[];            //!< �ڑ����H_����_E
	extern const _TCHAR kCrossRoadMiddle_F[];            //!< �ڑ����H_����_F
	extern const _TCHAR kCrossRoadAhead_A[];             //!< �ڑ����H_��_A
	extern const _TCHAR kCrossRoadAhead_B[];             //!< �ڑ����H_��_B
	extern const _TCHAR kCrossRoadAhead_C[];             //!< �ڑ����H_��_C
	extern const _TCHAR kCrossRoadAhead_D[];             //!< �ڑ����H_��_D
	extern const _TCHAR kCrossRoadAhead_E[];             //!< �ڑ����H_��_E
	extern const _TCHAR kCrossRoadAhead_F[];             //!< �ڑ����H_��_F
	extern const _TCHAR kCrossRoadAhead_G[];             //!< �ڑ����H_��_G

	extern const int    kMaxNodeNumber; // �ő�m�[�h��
	extern const _TCHAR kNode0[];   ///< ���H�m�[�h0(�g��Ȃ����֋X��)
	extern const _TCHAR kNode1[];   ///< ���H�m�[�h1
	extern const _TCHAR kNode2[];   ///< ���H�m�[�h2
	extern const _TCHAR kNode3[];   ///< ���H�m�[�h3
	extern const _TCHAR kNode4[];   ///< ���H�m�[�h4
	extern const _TCHAR kNode5[];   ///< ���H�m�[�h5
	extern const _TCHAR kNode6[];   ///< ���H�m�[�h6
	extern const _TCHAR kNode7[];   ///< ���H�m�[�h7
	extern const _TCHAR kNode8[];   ///< ���H�m�[�h8
	extern const _TCHAR kNode9[];   ///< ���H�m�[�h9
	extern const _TCHAR kNode10[];  ///< ���H�m�[�h10
	extern const _TCHAR kNode11[];  ///< ���H�m�[�h11
	extern const _TCHAR kNode12[];  ///< ���H�m�[�h12
	extern const _TCHAR kNode13[];  ///< ���H�m�[�h13
	extern const _TCHAR kNode14[];  ///< ���H�m�[�h14
	extern const _TCHAR kNode15[];  ///< ���H�m�[�h15
	extern const _TCHAR kNode16[];  ///< ���H�m�[�h16
	extern const _TCHAR kNode17[];  ///< ���H�m�[�h17
	extern const _TCHAR kNode18[];  ///< ���H�m�[�h18
	extern const _TCHAR kNode19[];  ///< ���H�m�[�h19
	extern const _TCHAR kNode20[];  ///< ���H�m�[�h20

	extern const LPCTSTR kNode[21]; ///< ���H�m�[�h�ꗗ

	extern const _TCHAR kProvisional[];   //!< �b�萮���t���O

	// �C���X�g�d������R�[�h
	namespace customer {
		enum ECode {
			kUnset         = 0,   //!< ���ݒ�
			kCommon        = 1,   //!< ����
			kTF            = 2,   //!< TF
			kExternalSale  = 3,   //!< �O��
			kTR            = 4,   //!< TR
		};
	} // illust_customer

	// �C���X�g��ʃR�[�h
	namespace illust_class {
		enum ECode {
			kSAPA          = 110, //!< SAPA
			kETC           = 120, //!< ETC�i�{���j
			kETC2          = 121, //!< ETC�i�o���j
			kProperJCT     = 130, //!< �ŗLJCT
			kBranch        = 140, //!< ��������i�s�s�j
			kBranch2       = 141, //!< ��������i�s�s�ԁj
			kEntrance      = 150, //!< ������������
			kExit          = 151, //!< �����o������
			kInIC          = 210, //!< ��������
			kAirport       = 220, //!< ��`
			kRealCross     = 230, //!< ���G�����_
			kGrade         = 240, //!< ��������
		};
	} // illust_code
	
	// �C���X�g�㉺���R�[�h
	namespace updown_code {
		enum ECode {
			kUnset         = 0,   //!< ���ݒ�
			kUp            = 1,   //!< ���
			kDown          = 2,   //!< ����
			kUpDown        = 3,   //!< �㉺�����L
		};
	} // updown_code

	// �C���X�g�A������R�[�h
	namespace successive_code {
		enum ECode {
			kUnset              = 0,   //!< ���ݒ�
			kThreeWayBranch     = 3,   //!< 3����
			kFourWayBranch      = 4,   //!< 4����
			kNotExistSuccessive = 9,   //!< �A������Ȃ�
		};
	} // successive_code

	// �C���X�g�P����R�[�h
	namespace single_branch_code {
		enum ECode {
			kUnset                 = 0,   //!< ���ݒ�
			kSingleBranchDesign    = 1,   //!< �P����f�U�C��
			kNonSingleBranchDesign = 2,   //!< ��P����f�U�C��
		};
	} // single_branch_code

	// �C���X�g�C���M�����[����R�[�h
	namespace irregular_code {
		enum ECode {
			kUnset           = 0,   //!< ���ݒ�
			kIrregularBranch = 1,   //!< �C���M�����[����
			kCommonBranch    = 2,   //!< �ʏ핪��
		};
	} // irregular_code

	// �C���X�g�g���l���R�[�h
	namespace tunnel_code {
		enum ECode {
			kUnset            = 0,   //!< ���ݒ�
			kExistTunnel      = 1,   //!< �g���l������
			kNotExistTunnel   = 2,   //!< �g���l���Ȃ�
		};
	} // tunnel_code

	// �C���X�g���i�����R�[�h
	namespace straight_code {
		enum ECode {
			kUnset            = 0,   //!< ���ݒ�
			kStraight         = 1,   //!< ���i����
			kBranch           = 2,   //!< �������
		};
	} // straight_code

	// �C���X�g�������E�Ȃ��R�[�h
	namespace boundary_code {
		enum ECode {
			kUnset            = 0,   //!< ���ݒ�
			kExistBoundary    = 1,   //!< �������E����
			kNotExistBoundary = 2,   //!< �������E�Ȃ�
		};
	} // boundary_code

	// �C���X�g��������R�[�h
	namespace branch_dir_code {
		enum ECode {
			kUnset         = 0,   //!< ���ݒ�
			kLeft          = 1,   //!< ��
			kRight         = 2,   //!< �E
		};
	} // branch_dir_code

	// �C���X�g�Ԑ�����p�^�[���R�[�h
	namespace lane_brn_pattern_code {
		enum ECode {
			kUnset             = 0,   //!< ���ݒ�
			kExistLaneBranch   = 1,   //!< �Ԑ����򂠂�
			kExistFrontageJoin = 2,   //!< ������������
		};
	} // lane_brn_pattern_code

	// �C���X�g���ʈē��ŔL���R�[�h
	namespace dir_guide_exist_code {
		enum ECode {
			kUnset         = 0,   //!< ���ݒ�
			kLX            = 1,   //!< ��
			kXR            = 2,   //!< �E
			kLR            = 3,   //!< ���E
			kXX            = 9,   //!< �Ȃ�
		};
	} // dir_guide_exist_code

	// �C���X�g�ē������R�[�h
	namespace direction_code {
		enum ECode {
			kUnset         = 0,   //!< ���ݒ�
			kA             = 1,   //!< A
			kB             = 2,   //!< B
			kC             = 3,   //!< C
			kD             = 4,   //!< D
			kE             = 5,   //!< E
			kF             = 6,   //!< F
			kG             = 7,   //!< G
			kH             = 8,   //!< H
			kI             = 9,   //!< I
			kJ             = 10,  //!< J
			kK             = 11,  //!< K
			kL             = 12,  //!< L
			kM             = 13,  //!< M
			kN             = 14,  //!< N
			kO             = 15,  //!< O
		};
	} // direction_code

	// �C���X�g����p�^�[���R�[�h
	namespace branch_pattern_code {
		enum ECode {
			kUnset         = 0,   //!< ���ݒ�
			kS             = 101, //!< ���E����
			kY             = 102, //!< Y������
			kN             = 103, //!< ����Y������
			kC             = 104, //!< �A������
			kO1            = 201, //!< �{���I�[�o�[�p�X�E��������
			kU1            = 202, //!< �{���A���_�[�p�X�E��������
			kO2            = 203, //!< �{�������E�����A���_�[�p�X
			kU2            = 204, //!< �{�������E�����I�[�o�[�p�X
			kF1            = 205, //!< �{�������E��������
		};
	} // branch_pattern_code

	// �C���X�g�ڑ����H�L���R�[�h
	namespace cross_road_code {
		enum ECode {
			kUnset             = 0,   //!< ���ݒ�
			kExistCrossRoad    = 1,   //!< �ڑ����H����
			kNotExistCrossRoad = 2,   //!< �ڑ����H�Ȃ�
		};
	} // cross_road_code

} // illust_link

} // schema

} // sindy

#endif // SINDY_SCHEMA_ILLUST_H_
