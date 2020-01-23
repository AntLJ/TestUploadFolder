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

namespace sindy {

namespace schema {

namespace bus {
	
/// �o�X��|�C���g
namespace bus_stop_point {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kStopID[];        //!< �◯���R�[�h
	extern const _TCHAR kPointAttrCode[]; //!< �o�X�|�C���g��ʃR�[�h
	extern const _TCHAR kName[];          //!< �◯����
	extern const _TCHAR kYomi[];          //!< �◯������݂���
	extern const _TCHAR kChangedName[];   //!< �ύX��◯����
	extern const _TCHAR kChangedYomi[];   //!< �ύX��◯������݂���
	extern const _TCHAR kRemarks[];       //!< ���̕ύX���l
	extern const _TCHAR kPoleNo[];        //!< �W���ԍ�
	extern const _TCHAR kGettingOffF[];   //!< �~�Ԑ�p�t���O

	/// �o�X�|�C���g��ʃR�[�h
	namespace point_attr_code {
		enum ECode {
			kPole         = 1, //!< �◯���W���|�C���g
			kRoute        = 5, //!< �o�H���ԃ|�C���g
			kNationalLand = 9, //!< ���y���l���|�C���g
		};
	}
}
/// �o�X�⃋�[�g�T���p�|�C���g
namespace bus_route_point {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kBusStopPointID[]; //!< �o�X��|�C���gID
}
/// �o�X��ԏ��Ǘ�
namespace bus_stop_seq {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kBusLineMasterID[]; //!< �H�����ID
	extern const _TCHAR kStopSeq[];         //!< �����ԍ�
	extern const _TCHAR kBusStopPointID[];  //!< �o�X��|�C���gID
	extern const _TCHAR kPassF[];           //!< �ʉݒ◯���t���O
	extern const _TCHAR kDayCodes[];        //!< �o�H�j��
	extern const _TCHAR kTimeCodes[];       //!< �o�H���ԑ�
	extern const _TCHAR kRemarks[];         //!< �j�����Ԕ��l
}
/// �o�X�H���}�X�^
namespace bus_line_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kCompany[];      //!< ���ƎҖ�
	extern const _TCHAR kRouteNumber[];  //!< �n���ԍ�
	extern const _TCHAR kRouteName[];    //!< �n����
	extern const _TCHAR kRouteDetail[];  //!< �o�R
	extern const _TCHAR kRouteRemarks[]; //!< �o�R�n�����l
}
}
}
}
