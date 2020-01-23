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
 * @file
 * @note SindyRoadConvert����ړ��BSindyHighwayConvert������Q�Ƃ��Ă���̂Ń��C�u���������ׂ��Ɣ��f�B
 * @todo �uSindyRoadConvert��SindyHighwayConvert�̋��ʕ����v�Ƃ����̂�HwyConvertLib�Ƃ̓R���Z�v�g���قȂ�̂ŁA
 *        �ʂ̃��C�u�����Ƃ��ׂ��B
 */

#ifndef	___SINDY2INCODE_H___
#define	___SINDY2INCODE_H___

#include <atlbase.h>
#include "Z2INHeader.h"
#include "Z2LinkWrite.h"
#include "Z2NodeWrite.h"
#include "Z2RestWrite.h"
#include "Z2FacilWrite.h"
#include "Z2RegWrite.h"
#include "ZGuideWrite.h"
#include "ZNInfoWrite.h"
#include "ZLaneWrite.h"
#include "ZLextWrite.h"
#include "INAttrDef.h"

namespace SINDY2INCODE
{
	enum customer
	{
		pioneer			= 1,			// �p�C�I�j�A
		clarion			= 2,			// �N�����I��(TA)
		mitsubishi		= 3,			// �O�H�d�@
		kiwi			= 4,			// �N�����I��(Kiwi)
		xanavi			= 5,			// �U�i�r
		other			= 6				// Sony
	};

	struct Code
	{
		long			sindy_code;
		long			in_code;
	};

	extern Code road_manager[];			// ���H�Ǘ���
	extern Code link_class[];			// �����N���
	extern Code bypass_flag[];			// �o�C�p�X�t���O
	extern Code caronly_flag[];			// �����Ԑ�p���H�t���O
	extern Code road_width[];			// ���H�����敪
	extern Code road_class[];			// ���H��ʃR�[�h
	extern Code navi_class[];			// �o�H��ʃR�[�h
	extern Code wide_area[];			// �L��t���O
	extern Code parking_class[];		// ���ԏ���
	extern Code parkinglink_class[];	// ���ԏꃊ���N���
	extern Code vics_class[];			// VICS�敪
	extern Code segment_attr[];			// �����N������
	extern Code kubun_code[];			// ��]�敪
	extern Code limit_speed[];			// �K�����x
	extern Code area_flag[];			// �׊X�H�G���A�t���O

	extern Code signal_flag[];			// �M���@�t���O
	extern Code height_flag[];			// ���H�����t���O

	extern Code dayofweek[];			// �j���w�����

	extern Code dirguide_class[];		// �n����
	extern Code guide_class[];			// �����U���ē��R�[�h

	extern HRESULT Convert(Code *cTable, long cSindyCode, long *cInCode);
	extern HRESULT Convert2(Code *cTable, long cInCode, long *cSindyCode);
	extern void wcs2mbs(BSTR bstrVal, std::string *cName);
	extern std::string SJIS2EUC(const char *cSjisString);
	extern std::string EUC2SJIS(const char *cEucString);
};


#endif	//___SINDY2INCODE_H___
