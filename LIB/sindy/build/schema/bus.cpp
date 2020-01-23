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

#include <stdafx.h>
#include <sindy/schema/bus.h>

namespace sindy {

namespace schema {

namespace bus {
	
/// �o�X��|�C���g
namespace bus_stop_point {
	const _TCHAR kTableName[] = _T("BUS_STOP_POINT"); //!< �e�[�u����

	const _TCHAR kStopID[] = _T("STOP_ID");        //!< �◯���R�[�h
	const _TCHAR kPointAttrCode[] = _T("POINT_ATTR_CODE"); //!< �o�X�|�C���g��ʃR�[�h
	const _TCHAR kName[] = _T("STOP_NAME_KANJI");          //!< �◯����
	const _TCHAR kYomi[] = _T("STOP_NAME_YOMI");          //!< �◯������݂���
	const _TCHAR kChangedName[] = _T("CHANGED_STOP_NAME_KANJI");   //!< �ύX��◯����
	const _TCHAR kChangedYomi[] = _T("CHANGED_STOP_NAME_YOMI");   //!< �ύX��◯������݂���
	const _TCHAR kRemarks[] = _T("REMARKS");       //!< ���̕ύX���l
	const _TCHAR kPoleNo[] = _T("POLE_NO");        //!< �W���ԍ�
	const _TCHAR kGettingOffF[] = _T("GETTING_OFF_ONLY_F");   //!< �~�Ԑ�p�t���O
}
/// �o�X�⃋�[�g�T���p�|�C���g
namespace bus_route_point {
	const _TCHAR kTableName[] = _T("BUS_ROUTE_POINT"); //!< �e�[�u����

	const _TCHAR kBusStopPointID[] = _T("BUS_STOP_POINT_ID"); //!< �o�X��|�C���gID
}
/// �o�X��ԏ��Ǘ�
namespace bus_stop_seq {
	const _TCHAR kTableName[] = _T("BUS_STOP_SEQ"); //!< �e�[�u����

	const _TCHAR kBusLineMasterID[] = _T("BUS_LINE_MASTER_ID"); //!< �H�����ID
	const _TCHAR kStopSeq[] = _T("STOP_SEQ");         //!< �����ԍ�
	const _TCHAR kBusStopPointID[] = _T("BUS_STOP_POINT_ID");  //!< �o�X��|�C���gID
	const _TCHAR kPassF[] = _T("PASS_F");           //!< �ʉݒ◯���t���O
	const _TCHAR kDayCodes[] = _T("DAY_CODES");        //!< �o�H�j��
	const _TCHAR kTimeCodes[] = _T("TIME_CODES");       //!< �o�H���ԑ�
	const _TCHAR kRemarks[] = _T("REMARKS");         //!< �j�����Ԕ��l
}
/// �o�X�H���}�X�^
namespace bus_line_master {
	const _TCHAR kTableName[] = _T("BUS_LINE_MASTER"); //!< �e�[�u����

	const _TCHAR kCompany[] = _T("COMPANY");            //!< ���ƎҖ�
	const _TCHAR kRouteNumber[] = _T("ROUTE_NUMBER");   //!< �n���ԍ�
	const _TCHAR kRouteName[] = _T("ROUTE_NAME");       //!< �n����
	const _TCHAR kRouteDetail[] = _T("ROUTE_DETAIL");   //!< �o�R
	const _TCHAR kRouteRemarks[] = _T("ROUTE_REMARKS"); //!< �o�R�n�����l
}
}
}
}
