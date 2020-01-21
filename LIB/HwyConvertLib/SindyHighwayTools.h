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
 *  @file HwyConvertLib�J���ɔ����ASindyHighwayCheck�̃\�[�X��SindyHighwayConvert�ł��g���Ă����̂��������邽�߁A
 *        ���̋��L������SindyHighwayCheck����HwyConvertLib�Ɉڊǂ����B
 */
#ifndef	___SINDY_HIGHWAY_TOOLS_H___
#define	___SINDY_HIGHWAY_TOOLS_H___

#include "SindyHighwayInfo.h"

namespace sindy_highway_tools
{
	const long CONNECTION_LINK_ADDITIONAL_ROADCODE = 200000;	///< �u�{���ڑ��������N�v�̘H���R�[�h�́A�{�������N�̘H���R�[�h���200000�傫��
	const long FREEZONE_ADDITIONAL_ROADCODE = 400000;			///< �u������ԁv�̃����N�̘H���R�[�h�́A�{�������N�̘H���R�[�h���400000�傫��
} // namespace sindy_highway_tools

// TODO: �֐��R�����g�������ƑS���������B
/**
 * @brief �Ώۃ����N��HWY�̖{�������N�ł��邩�ۂ��𔻒肷��
 * @note	Main_LinkClass_C���u�{��(�㉺����)�����N�v�u�{��(�㉺�񕪗�)�����N�v�̂����ꂩ�ŁA
 *			Road_Code���u0�܂���400000�ȏ�600000�����v�ł͂Ȃ������N���uHWY�̖{�������N�v�ƌ��Ȃ�
 * @param[in]	ipLink	�Ώۃ����N
 * @return	���茋��
 **/
bool isMainLineLink(IFeature* ipLink);

/**
 *	@brief	�{���ǐՃ`�F�b�N�̑ΏۂƂ��邩�ۂ��̔���
 *	@param	ipLink	[in]	����Ώۃ����N
 *	@return	�^����ꂽ�����N���{���ǐՃ`�F�b�N�̑Ώۂł��邩�ۂ�
 **/
bool isMainLineTraceTarget(IFeature* ipLink);

/**
 * @brief JCT�̓n��������N�ł��邩�ۂ��𔻒肷��
 * @param[in]	ipLink	�Ώۃ����N
 * @return	���茋��
 **/
bool isJctLink(IFeature* ipLink);

// MAIN_LINKCLASS_C���u�A���H(�{���Ԃ̓n���)�v�ł��邩�`�F�b�N
bool isNewJctLink(IFeature* ipLink);

// MAIN_LINKCLASS_C���u�A���H(�����v)�����N�v�ł��邩�`�F�b�N
bool isRampLink(IFeature* ipLink);

// MAIN_LINKCLASS_C���uSA�����������N�v�ł��邩�`�F�b�N
bool isSaLink(IFeature* ipLink);

// MAIN_LINKCLASS_C���u�{���Ɠ���H���̑��������N�v�ł��邩�`�F�b�N
bool isSideLineLink(IFeature* ipLink);

// MAIN_LINKCLASS_C���u�����_�������N�v�ł��邩�`�F�b�N
bool isCrossingLink(IFeature* ipLink);

// ROADCLASS_C���L�����H�ł��邩�`�F�b�N
bool isTollRoad(IFeature* ipLink);

// 0�łȂ�ROAD_CODE�������Ă��邩�`�F�b�N
bool isRoadCode(IFeature* ipLink);

// ROAD_CODE��HWY�{�����m�̐ڑ��������N�ł��邩�`�F�b�N
bool isJointZoneLink(IFeature* ipLink);

// ROAD_CODE��������Ԃ̖{�������N�ł��邩�`�F�b�N
bool isTollFreeZoneLink(IFeature* ipLink);

/**
 * @brief �Ώۃ����N����ʓ��ł��邩�ۂ��𔻒肷��i[Bug7176]�Œǉ��j
 * @param[in]	cpLink	�Ώۃ����N
 * @return	���茋��
 */
bool isGeneralRoad( const IFeaturePtr cpLink );

/**
 *	@brief	�H���R�[�h���r���āA�����H���ł��邩���肷��
 *	@note	�{���̐ڑ��������N�Ɩ�����Ԃ́A�ʏ�̖{���Ɠ����H���ƌ��Ȃ�
 */
bool isSameLine(long aRoadCode1, long aRoadCode2);

/**
 *	@brief	�O��̕��򐔂�0���ǂ����𔻒肷��
 *	@retval	true	����Ȃ�
 *	@retval	false	1�ȏ�̕��򂠂�
 */
bool isZeroBranch(_IRowPtr ipHighwayTxt);

/**
 * @brief �Ώۃ����N�̑Ώۃm�[�h�Ƃ͔��Α��̃m�[�h���擾����i[Bug7176]�Œǉ��j
 * @param[in]	cpLink	�Ώۃ����N
 * @param[in]	cNodeID	�Ώۃm�[�h
 * @param[in]	cpRoadInfo	���H���
 * @return	�擾�����m�[�h
 */
IFeaturePtr getOppositeNode( IFeaturePtr cpLink, long cNodeID, SindyRoadInfo* cpRoadInfo );

#endif	//___SINDY_HIGHWAY_TOOLS_H___
