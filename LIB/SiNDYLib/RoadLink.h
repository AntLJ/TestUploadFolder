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
 * @file RoadLink.h
 * @brief <b>���H�����N�N���X��`</b>\n
 * @author 
 * @version $Id$
 */

#ifndef _ROADLINK_H_
#define _ROADLINK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"
#include "Feature.h"

namespace sindy {
namespace road_link {

/**
 * @class CRoadLink
 * @brief ���H�����N�t�B�[�`���N���X
 */
class SINDYLIB_API CRoadLink : public CFeature  
{
	ROWCLASS_CONSTRUCTOR( CRoadLink, road_link )
	//@{ @name �����擾�֐�

	/**
	 * @brief From_NodeID ��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	long GetFromNodeID() const
	{
		return GetLongValueByFieldName( schema::road_link::kFromNodeID, -1 );
	}

	/**
	 * @brief To_NodeID ��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	long GetToNodeID() const
	{
		return GetLongValueByFieldName( schema::road_link::kToNodeID, -1 );
	}

	/**
	 * @brief ���H�\����ʂ�Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::road_class::ECode GetRoadClassCode() const
	{
		return (schema::road_link::road_class::ECode)GetLongValueByFieldName( schema::road_link::kRoadClass, -1 );
	}

	/**
	 * @brief ���H�o�H��ʂ�Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::navi_class::ECode GetNaviClassCode() const
	{
		return (schema::road_link::navi_class::ECode)GetLongValueByFieldName( schema::road_link::kNaviClass, -1 );
	}

	/**
	 * @brief �僊���N��ʂ�Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::link_class::ECode GetMainLinkClassCode() const
	{
		return (schema::road_link::link_class::ECode)GetLongValueByFieldName( schema::road_link::kMainLinkClass, -1 );
	}

	/**
	 * @brief �������N��ʂ�Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::link_class::ECode GetSubLinkClassCode() const
	{
		return (schema::road_link::link_class::ECode)GetLongValueByFieldName( schema::road_link::kSubLinkClass, -1 );
	}

	/**
	 * @brief �s���R�[�h��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �s���R�[�h
	 */
	long GetAdminCode() const
	{
		return GetLongValueByFieldName( schema::road_link::kAdminCode, -1 );
	}

	/**
	 * @brief ���H�Ǘ��҂�Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::road_manager::ECode GetRoadManagerCode() const
	{
		return (schema::road_link::road_manager::ECode)GetLongValueByFieldName( schema::road_link::kRoadManager, -1 );
	}

	/**
	 * @brief ���H�����敪��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::road_width::ECode GetRoadWidthCode() const
	{
		return (schema::road_link::road_width::ECode)GetLongValueByFieldName( schema::road_link::kRoadWidth, -1 );
	}

	/**
	 * @brief �㉺����ʃR�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::up_down_class::ECode GetUpDownClassCode() const
	{
		return (schema::road_link::up_down_class::ECode)GetLongValueByFieldName( schema::road_link::kUpDownClass, -1 );
	}

	/**
	 * @brief �o�C�p�X�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �o�C�p�X�t���O
	 */
	long GetBypassFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kBypass, false );
	}

	/**
	 * @brief �����Ԑ�p���t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �����Ԑ�p���t���O
	 */
	long GetCarOnlyFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kCarOnly, false );
	}

	/**
	 * @brief �����t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �����t���O
	 */
	long GetIslandFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kIsland, false );
	}

	/**
	 * @brief �����E�����ԍ���Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �����E�����ԍ�
	 */
	long GetRoadNo() const
	{
		return GetLongValueByFieldName( schema::road_link::kRoadNo, -1 );
	}

	/**
	 * @brief �H���R�[�h��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �H���R�[�h
	 */
	long GetRoadCode() const
	{
		return GetLongValueByFieldName( schema::road_link::kRoadCode, -1 );
	}

	/**
	 * @brief �ʍs�֎~��ʃR�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::no_passage::ECode GetNoPassageCode() const
	{
		return (schema::road_link::no_passage::ECode)GetLongValueByFieldName( schema::road_link::kNoPassage, -1 );
	}

	/**
	 * @brief �ʍs�֎~���ԋK���֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �ʍs�֎~���ԋK���֘A�t���O
	 */
	long GetNoPassageRelFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kNoPassageRF, false );
	}

	/**
	 * @brief ����ʍs��ʃR�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::oneway::ECode GetOnewayCode() const
	{
		return (schema::road_link::oneway::ECode)GetLongValueByFieldName( schema::road_link::kOneway, -1 );
	}

	/**
	 * @brief ����ʍs���ԋK���֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ����ʍs���ԋK���֘A�t���O
	 */
	long GetOnewayRelFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kOnewayRF, false );
	}

	/**
	 * @brief DRMA���s���x��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO DRMA���s���x
	 */
	long GetTripSpeed() const
	{
		return GetLongValueByFieldName( schema::road_link::kTripSpeed, -1 );
	}

	/**
	 * @brief DRMA�K�����x��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::limit_speed::ECode GetLimitSpeedCode() const
	{
		return (schema::road_link::limit_speed::ECode)GetLongValueByFieldName( schema::road_link::kLimitSpeed, -1 );
	}

	/**
	 * @brief ��]���H�敪�R�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::kubun::ECode GetKubunCode() const
	{
		return (schema::road_link::kubun::ECode)GetLongValueByFieldName( schema::road_link::kKubun, -1 );
	}

	/**
	 * @brief �����N�Ԑ�����Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �����N�Ԑ���
	 */
	long GetLaneCount() const
	{
		return GetLongValueByFieldName( schema::road_link::kLaneCount, -1 );
	}

	/**
	 * @brief �E�������Ԑ�����Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �E�������Ԑ���
	 */
	long GetRightChange() const
	{
		return GetLongValueByFieldName( schema::road_link::kRightChange, -1 );
	}

	/**
	 * @brief ���������Ԑ�����Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���������Ԑ���
	 */
	long GetLeftChange() const
	{
		return GetLongValueByFieldName( schema::road_link::kLeftChange, -1 );
	}

	/**
	 * @brief �Ԑ��������e�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �Ԑ��������e�t���O
	 */
	long GetLaneValidFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kLaneValid, false );
	}

	/**
	 * @brief �׊X�H��ƔN�x�R�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::alley_mainte::ECode GetAlleyMainteCode() const
	{
		return (schema::road_link::alley_mainte::ECode)GetLongValueByFieldName( schema::road_link::kAlleyMainte, -1 );
	}

	/**
	 * @brief �d�p�H���֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �d�p�H���֘A�t���O
	 */
	long GetDupLinkRelFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kDupliLinkRF, false );
	}

	/**
	 * @brief �E���܋֎~�����N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �E���܋֎~�����N��֘A�t���O
	 */
	long GetTurnRegLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kTurnRegLQRF, false );
	}

	/**
	 * @brief ���ʈē������N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���ʈē������N��֘A�t���O
	 */
	long GetDirGuideLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kDirGuideLQRF, false );
	}

	/**
	 * @brief �����U�������N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �����U�������N��֘A�t���O
	 */
	long GetGuideLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kGuideLQRF, false );
	}

	/**
	 * @brief ���[����񃊃��N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���[����񃊃��N��֘A�t���O
	 */
	long GetLaneLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kLaneLQRF, false );
	}

	/**
	 * @brief VICS�����N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO VICS�����N��֘A�t���O
	 */
	long GetVICSLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kVICSLQRF, false );
	}

	/**
	 * @brief ���������������N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���������������N��֘A�t���O
	 */
	long GetBywayLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kBywayLQRF, false );
	}

	/**
	 * @brief �A�N�Z�X�����p���������N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �A�N�Z�X�����p���������N��֘A�t���O
	 */
	long GetRouteLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kRouteLQRF, false );
	}

	/**
	 * @brief U�^�[���\�����N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO U�^�[���\�����N��֘A�t���O
	 */
	long GetUTurnLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kUturnLQRF, false );
	}

	/**
	 * @brief �C���^�[�i�r�����N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �C���^�[�i�r�����N��֘A�t���O
	 */
	long GetInterNaviLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kInternaviLQRF, false );
	}

	/**
	 * @brief iPCVICS�����N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO iPCVICS�����N��֘A�t���O
	 */
	long GetiPCVICSLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kIpcVicsLQRF, false );
	}

	/**
	 * @brief �i�������ʌ����_�����N��֘A�t���O��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO �i���ʌ����_�����N��֘A�t���O
	 */
	long GetIntersectionLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kIntersectionLQRF, false );
	}

	/**
	 * @brief ���ԏ��ʃR�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::parking_class::ECode GetParkingClassCode() const
	{
		return (schema::road_link::parking_class::ECode)GetLongValueByFieldName( schema::road_link::kParkingClass, -1 );
	}

	/**
	 * @brief ���ԏꃊ���N��ʃR�[�h��Ԃ�
	 * @retval -1 �G���[����NULL
	 */
	schema::road_link::parking_link_class::ECode GetParkingLinkClassCode() const
	{
		return (schema::road_link::parking_link_class::ECode)GetLongValueByFieldName( schema::road_link::kParkingLinkClass, -1 );
	}

	/**
	 * @brief ���ԏ�K�w��Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���ԏ�K�w
	 */
	long GetFloorLevel() const
	{
		return GetLongValueByFieldName( schema::road_link::kFloorLevel, -1 );
	}

	/**
	 * @brief ���ԏ�K�w�ԃ��x����Ԃ�
	 * @retval -1 �G���[
	 * @retval -1�ȊO ���ԏ�K�w�ԃ��x��
	 */
	long GetMiddleFloorLevel() const
	{
		return GetLongValueByFieldName( schema::road_link::kMiddleFloorLevel, -1 );
	}
	
	//@}

	//@{ @name �����ݒ�֐�

	/**
	 * @brief From_NodeID ��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetFromNodeID(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kFromNodeID, lValue );
	}

	/**
	 * @brief ���H�\����ʂ�ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetToNodeID(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kToNodeID, lValue );
	}

	/**
	 * @brief ���H�\����ʂ�ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadClassCode(schema::road_link::road_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadClass, emCode );
	}

	/**
	 * @brief ���H�o�H��ʂ�ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetNaviClassCode(schema::road_link::navi_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kNaviClass, emCode );
	}

	/**
	 * @brief �僊���N��ʂ�ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetMainLinkClassCode(schema::road_link::link_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kMainLinkClass, emCode );
	}

	/**
	 * @brief �������N��ʂ�ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetSubLinkClassCode(schema::road_link::link_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kSubLinkClass, emCode );
	}

	/**
	 * @brief �s���R�[�h��ݒ肷��
	 * @brief NULL ����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetAdminCode(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kAdminCode, lValue );
	}

	/**
	 * @brief ���H�Ǘ��҂�ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadManagerCode(schema::road_link::road_manager::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadManager, emCode );
	}

	/**
	 * @brief ���H�����敪��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadWidthCode(schema::road_link::road_width::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadWidth, emCode );
	}

	/**
	 * @brief �㉺����ʃR�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetUpDownClassCode(schema::road_link::up_down_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kUpDownClass, emCode );
	}

	/**
	 * @brief �o�C�p�X�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetBypassFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kBypass, bFlag );
	}

	/**
	 * @brief �����Ԑ�p���t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetCarOnlyFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kCarOnly, bFlag );
	}

	/**
	 * @brief �����t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetIslandFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kIsland, bFlag );
	}

	/**
	 * @brief �����E�����ԍ���ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadNo(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadNo, lValue );
	}

	/**
	 * @brief �H���R�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadCode(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadCode, lValue );
	}

	/**
	 * @brief �ʍs�֎~��ʃR�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetNoPassageCode(schema::road_link::no_passage::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kNoPassage, emCode );
	}

	/**
	 * @brief �ʍs�֎~���ԋK���֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetNoPassageRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kNoPassageRF, bFlag );
	}

	/**
	 * @brief ����ʍs��ʃR�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetOnewayCode(schema::road_link::oneway::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kOneway, emCode );
	}

	/**
	 * @brief ����ʍs���ԋK���֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetOnewayRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kOnewayRF, bFlag );
	}

	/**
	 * @brief DRMA���s���x��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetTripSpeed(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kTripSpeed, lValue );
	}

	/**
	 * @brief DRMA�K�����x��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetLimitSpeedCode(schema::road_link::limit_speed::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kLimitSpeed, emCode );
	}

	/**
	 * @brief ��]���H�敪�R�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetKubunCode(schema::road_link::kubun::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kKubun, emCode );
	}

	/**
	 * @brief �����N�Ԑ�����ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetLaneCount(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kLaneCount, lValue );
	}

	/**
	 * @brief �E�������Ԑ�����ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetRightChange(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kRightChange, lValue );
	}

	/**
	 * @brief ���������Ԑ�����ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetLeftChange(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kLeftChange, lValue );
	}

	/**
	 * @brief �Ԑ��������e�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetLaneValidFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kLaneValid, bFlag );
	}

	/**
	 * @brief �׊X�H��ƔN�x�R�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetAlleyMainteCode(schema::road_link::alley_mainte::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kAlleyMainte, emCode );
	}

	/**
	 * @brief �d�p�H���֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetDupLinkRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kDupliLinkRF, bFlag );
	}

	/**
	 * @brief �E���܋֎~�����N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetTurnRegLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kTurnRegLQRF, bFlag );
	}

	/**
	 * @brief ���ʈē������N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetDirGuideLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kDirGuideLQRF, bFlag );
	}

	/**
	 * @brief �����U�������N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetGuideLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kGuideLQRF, bFlag );
	}

	/**
	 * @brief ���[����񃊃��N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetLaneLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kLaneLQRF, bFlag );
	}

	/**
	 * @brief VICS�����N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetVICSLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kVICSLQRF, bFlag );
	}

	/**
	 * @brief ���������������N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetBywayLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kBywayLQRF, bFlag );
	}

	/**
	 * @brief �A�N�Z�X�����p���������N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetRouteLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kRouteLQRF, bFlag );
	}

	/**
	 * @brief U�^�[���\�����N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetUTurnLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kUturnLQRF, bFlag );
	}

	/**
	 * @brief �C���^�[�i�r�����N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetInterNaviLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kInternaviLQRF, bFlag );
	}

	/**
	 * @brief iPCVICS�����N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetiPCVICSLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kIpcVicsLQRF, bFlag );
	}

	/**
	 * @brief �i�������ʌ����_�����N��֘A�t���O��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetIntersectionLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kIntersectionLQRF, bFlag );
	}

	/**
	 * @brief ���ԏ��ʃR�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetParkingClassCode(schema::road_link::parking_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kParkingClass, emCode );
	}

	/**
	 * @brief ���ԏꃊ���N��ʃR�[�h��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetParkingLinkClassCode(schema::road_link::parking_link_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kParkingLinkClass, emCode );
	}

	/**
	 * @brief ���ԏ�K�w��ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetFloorLevel(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kFloorLevel, lValue );
	}

	/**
	 * @brief ���ԏ�K�w�ԃ��x����ݒ肷��
	 * @brief NULL �s����
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�� sindyErrCode
	 */
	errorcode::sindyErrCode SetMiddleFloorLevel(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kMiddleFloorLevel, lValue );
	}
	
	//@}

	//@{ @name �`�F�b�N�R�[�h
	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * ���H�����N��ҏW������ɕK���`�F�b�N���s���Ă��������B
	 * ���̃��\�b�h�Ń`�F�b�N����鍀�ڂ͈ȉ��̂Ƃ���ł��B
	 * @li 
	 * @li 
	 * @li 
	 * @param cRule			[in]	�������`�F�b�N�p���[��
	 * @param cErrs			[out]	�G���[���i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const;

	/*
	 * @brief ���[�ɓ��H�m�[�h�����݂��Ă��邩
	 * @retval �����@sindyErr_NoErr
	 * @retval ���s�@����ȊO�̃G���[
	 */
	errorcode::sindyErrCode CheckNodeID() const;
	//@}
	/**
	 * @brief <b>FROM���̒[�_���擾����</b>\n
	 * return CPoint
	 */
	CPoint GetFromPoint() const;

	/**
	 * @brief <b>TO���̒[�_���擾����</b>\n
	 * return CPoint
	 */
	CPoint GetToPoint() const;

	bool IsPECLink() const;
};

} // road_link

} // sindy

#endif // _ROADLINK_H_
