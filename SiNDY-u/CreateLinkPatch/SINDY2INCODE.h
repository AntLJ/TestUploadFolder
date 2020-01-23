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
 *	SINDY2INCODE.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/30		�V�K�쐬
 */

#ifndef	___SINDY2INCODE_H___
#define	___SINDY2INCODE_H___

#include "SindyAttrdef.h"
//#include "SINDY2INRNWINFO.h"
#include "INAttrdef.h"

namespace SINDY2INCODE
{
	enum customer
	{
		pioneer			= 1,			// �p�C�I�j�A
		clarion			= 2,			// �N�����I��(TA)
		mitsubishi		= 3,			// �O�H�d�@
		xanavi			= 4,			// �U�i�r
		other			= 5				// �N�����I��(Kiwi)�ASony
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
