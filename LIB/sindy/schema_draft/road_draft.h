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

#ifndef SINDY_SCHEMA_DRAFT_ROAD_H_
#define SINDY_SCHEMA_DRAFT_ROAD_H_

#include <iostream>
#include <boost/preprocessor.hpp>
#include <sindy/schema/common.h>   // �Ԏ؂�
#include <sindy/schema/category.h> // �Ԏ؂�

namespace sindy {

namespace schema {

namespace road_link {
	namespace walkable_class {
		enum ECode_draft {
			kBothSideWalk             = 3, //!< ������������
			kRightSideWalkWalkable    = 4, //!< �E����������i�l�ԋ��p�j
			kRightSideWalkNonWalkable = 5, //!< �E����������i���s�֎~�j
			kLeftSideWalkWalkable     = 6, //!< ������������i�l�ԋ��p�j
			kLeftSideWalkNonWalkable  = 7, //!< ������������i���s�֎~�j
		};
	} // walkable_class
} // road_link

namespace walk_link {

	// ���s�ҕ\����ʃR�[�h
	namespace walk_class { // �����I�ɂȂ��Ȃ�R�[�h�l
		enum ECode_draft {
			kLogicalConnection          = 199,  //!< ���W�J���R�l�N�V����
			kUnsurveyInternalTicketGate = 399,  //!< ���D���i���j
		};
	} // walk_class
} // walk_link

namespace walk_node {

	namespace node_class {
		enum ECode_draft {
			kConnectedRoad = 15,       //!< ���H�����N�ڑ��m�[�h
			kUnsurveyTicketGate = 98,  //!< ���D�m�[�h�i���j
		};
	} // node_class
} // walk_node

namespace pedxing {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	extern const _TCHAR kPedxingClass[];  //!< ���s�҃m�[�h���

	namespace pedxing_class {
		enum ECode_draft {
			kCrossRoad         = 1, //!< ���f����
			kInferredCrossRoad = 2, //!< �݂Ȃ����f����
		};
	} // pedxing_class
} // pedxing

namespace scramble_area {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	extern const _TCHAR kScrambleClass[]; //!< �X�N�����u����ʃR�[�h

	// �X�N�����u����ʃR�[�h
	namespace scramble_class {
		enum ECode_draft {
			kManual      = 1,   //!< �蓮
			kAuto        = 2,   //!< �@�B
		};
	} // scramble_class
} // scramble_area
}
}
#endif // SINDY_SCHEMA_ROAD_H_
