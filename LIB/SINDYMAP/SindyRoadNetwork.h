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
 *	SindyRoadNetwork.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/30		�V�K�쐬
 */

#ifndef	___SINDY_ROAD_NETWORK_H___
#define	___SINDY_ROAD_NETWORK_H___

#include "SindyRoadLink.h"
#include "SindyRoadNode.h"
#include "SindySegmentAttr.h"
#include "SindyGrad.h"
#include "SindyTurnreg.h"
#include "SindyUturn.h"
#include "SindyVics.h"
#include "SindyInterNavi.h"
#include "SindyByway.h"
#include "SindyRoute.h"
#include "SindyDirguide.h"
#include "SindyGuide.h"
#include "SindyLane.h"
#include "SindyParkingShape.h"
#include "SindyRoadCodeList.h"
#include "SindyDupliLink.h"
#include "SindyTimeNoPassage.h"
#include "SindyTimeOneway.h"
#include "SindyHighwayText.h"
#include "SindyHighwayNode.h"
#include "SindyAccessPoint.h"
#include "SindyFacilInfoPoint.h"
#include "SindyAccessStore.h"
#include "SindyEtcTollgate.h"
#include "SindyIntersection.h"

namespace sindy
{
	namespace rnw
	{
		namespace data_type
		{
			enum road_type
			{
				road_link			= 0x00000001,		// ���H�����N
				road_node			= 0x00000002,		// ���H�m�[�h
				segment_attr		= 0x00000004,		// �����N������
				grad				= 0x00000008,		// �X��
				turnreg				= 0x00000010,		// �E���܋֎~�EU�^�[���֎~
				uturn				= 0x00000020,		// U�^�[���\�ӏ�
				vics				= 0x00000040,		// VICS
				inter_navi			= 0x00000080,		// �C���^�[�i�rVICS
				byway				= 0x00000100,		// �������E����
				route				= 0x00000200,		// ���p�����E�ό��ē���
				lane				= 0x00004000,		// ���[�����
				guide				= 0x00008000,		// �����U��
				dirguide			= 0x00010000,		// ���ʈē�
				time_nopassage		= 0x00020000,		// �ʍs�֎~����
				time_oneway			= 0x00040000,		// ����ʍs����
				dupli_link			= 0x00080000,		// �d�p�H��
				intersection		= 0x00100000		//�i�������ʌ����_��
			};

			enum highway_type
			{
				facil_info_point	= 0x01000000,		// �{�ݏ��|�C���g
				access_point		= 0x02000000,		// �A�N�Z�X�|�C���g
				highway_node		= 0x04000000,		// �n�C�E�F�C�m�[�h
				highway_text		= 0x08000000		// �n�C�E�F�C�e�L�X�g
			};

			enum other_type
			{
				road_code_list		= 0x10000000,		// �H���R�[�h���X�g
				parking_shape		= 0x20000000		// ���ԏ�O�`
			};
		};

		namespace relation
		{
			enum code
			{
				from				= 0x00000001,		// �n�_
				to					= 0x00000002,		// �I�_
				fore				= 0x00000010,		// ������
				back				= 0x00000020,		// �t����
				in					= 0x00000100,		// ��
				out					= 0x00000200,		// �O

				fromto				= (from|to),
				bothdir				= (fore|back),
				inout				= (in|out)
			};
		};

		extern long ROADDATA;
		extern long HWYDATA;
		extern long BASEROAD;
	};
};

#endif	//___SINDY_ROAD_NETWORK_H___
