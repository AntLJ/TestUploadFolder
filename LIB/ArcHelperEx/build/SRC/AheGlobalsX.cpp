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
* @file AheGlobalsX.cpp
*
* @brief Arc Helper �O���[�o���֐������t�@�C��
*
* @note AheGlobals�̊֐��𗘗p���A��������������A���@�\�^�֐����܂Ƃ߂Ă���
*/
#include "stdafx.h"

#include "Mesh.h"
#include "AheGlobals.h"
#include "AheGlobalsX.h"
#include "TargetFeatures.h"
#include "LoadQueueInfo.h"
#include "Map.h"
#include "GlobalFunctions.h"
#include "ErrObj.h"
#include "AheGeometryConvert.h"
#include <sindy/schema/map.h>

using namespace sindy::schema;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR EDITING_WARN                           = _T("�ҏW�x��");
	LPCTSTR EDITING_PROHIBITED                     = _T("�ҏW�֎~");
	LPCTSTR DELETE_LINKQUEUE                       = _T("�����N��̍폜");
	LPCTSTR CHECK_SHAPE                            = _T("�`��`�F�b�N");
	LPCTSTR INVALID_FEATURE                        = _T("�s���ȃt�B�[�`��");
	LPCTSTR WARN_BELONGS_TO_UNDISP                 = _T("���C�� : %s OBJECTID : %d : ��\�����C���ɑ����Ă��܂��B\n\n�ҏW���܂����H");
	LPCTSTR ERROR_EDIT_ON_FOLLOWING_RULE_LAYER_OID = _T("���C�� : %s OBJECTID : %d : �ȉ��̃��[���ɂ��ҏW�ł��܂���:\n\n%s");
	LPCTSTR WARN_EDIT_ON_FOLLOWING_RULE            = _T("���C�� : %s OBJECTID : %d : �ȉ��̃��[���ɂ�胏�[�j���O���x���ɐݒ肳��Ă��܂�:\n\n%s\n�ҏW���܂����H");
	LPCTSTR ERROR_EDIT_ON_FOLLOWING_RULE_LAYER     = _T("���C�� : %s : �ȉ��̃��[���ɂ��ҏW�ł��܂���:\n\n%s");
	LPCTSTR ERROR_CANNOT_CREATE_LAYER              = _T("�쐬�s�\�ȃ��C���ł�");
	LPCTSTR ERROR_IS_LOCKAREA                      = _T("���b�N�G���A���ł�");
	LPCTSTR ERROR_IS_OUT_OF_EDITAREA               = _T("�ҏW�G���A�O�ł�");
	LPCTSTR CONFIRM_DELETE_LINKQUEUE               = _T("�����N�Ƀ����N�񂪑��݂��܂��B\n�����N�ƃ����N����폜���Ă���낵���ł����H");
	LPCTSTR ERROR_NOT_FOUND_ROADLINK_REL_LINKQUEUE = _T("�����N��Ɋ֘A���铹�H�����N��������܂���ł����FOBJECTID=%d");
	LPCTSTR ERROR_IS_NOT_EDITABLE_AREA             = _T("�ҏW�\�G���A�ł͂Ȃ����߁A�ҏW�ł��܂���B");
	LPCTSTR ERROR_IS_LOCKMESH                      = _T("���b�N���b�V�����̂��߁A�ҏW�ł��܂���B");
	LPCTSTR ERROR_EXIST_NODE_IN_MESH_CORNER        = _T("2�����b�V����4���Ƀm�[�h�����݂��Ă��܂��B");
	LPCTSTR ERROR_LINK_LOOP                        = _T("�����N�����[�v���Ă��܂��B");
	LPCTSTR ERROR_LINK_SELF_INTERSECT              = _T("�����N�����Ȍ������Ă��܂��B");
	LPCTSTR ERROR_POLYGON_SELP_INTERSECT           = _T("���Ȍ������Ă��܂��B");
	LPCTSTR ERROR_NON_EXIST_SHAPE                  = _T("�`�󂪑��݂��܂���B");
	LPCTSTR ERROR_EIGHT_POLYGON                    = _T("���̎��|���S�����A�܂��͏d�������\���_���A�ɏ��Z�O�����g�����݂��Ă��܂��B");
	LPCTSTR ERROR_OUTSIDE_POLYGON                  = _T("�O���̃|���S����2�ȏ㑶�݂��܂��B");
	LPCTSTR ERROR_IS_NOT_CHECK_TARGET              = _T("�`��`�F�b�N�Ώۂ̃W�I���g���ł͂Ȃ����߁A�`�F�b�N�ł��܂���B");
	LPCTSTR ERROR_IS_SILVER_POLYGON                = _T("�����|���S���̉\��������܂��B");
	LPCTSTR ERROR_TWIST_POLYGON                    = _T("�`�󂪂˂���Ă��܂��B");
	LPCTSTR ERROR_POLYLINE_COMPOSITION_POINT       = _T("�|�����C���̍\���_����1�ȉ��ł��B");
	LPCTSTR ERROR_POLYGON_COMPOSITIOIN_POINT       = _T("�|���S���̍\���_����2�ȉ��ł��B");
	LPCTSTR ERROR_SILVER_SEGMENT                   = _T("�����Z�O�����g�����݂��܂��i%lf cm�j�B");
	LPCTSTR ERROR_SEGMENT_EXIST_MESH               = _T("�Z�O�����g��2�����b�V�����E����ɑ��݂��܂��B");
	LPCTSTR START_VECTOR                           = _T("�J�n�x�N�g���i%.1lfcm�j�Ƃ��̎��̃x�N�g��");
	LPCTSTR END_VECTOR                             = _T("�I���x�N�g���i%.1lfcm�j�Ƃ��̑O�̃x�N�g��");
	LPCTSTR ERROR_ANGLE_MORE_THAN_170              = _T("%s�̊p�x��%.1lf���ł����A���K�����W������50�����̏ꍇ��170���ȏ�Ȃ���΂����܂���B");
	LPCTSTR ERROR_ANGLE_MORE_THAN_150              = _T("%s�̊p�x��%.1lf���ł����A���K�����W������50�ȏ�100�����̏ꍇ��150���ȏ�Ȃ���΂����܂���B");
	LPCTSTR ERROR_ANGLE_MORE_THAN_120              = _T("%s�̊p�x��%.1lf���ł����A���K�����W������100�ȏ�250�����̏ꍇ��120���ȏ�Ȃ���΂����܂���B");
	LPCTSTR ERROR_ANGLE_MORE_THAN_90               = _T("%s�̊p�x��%.1lf���ł����A���K�����W������250�ȏ�500�����̏ꍇ��90���ȏ�Ȃ���΂����܂���B");
	LPCTSTR ERROR_ANGLE_MORE_THAN_40               = _T("%s�̊p�x��%.1lf���ł����A���K�����W������500�ȏ�1000�����̏ꍇ��40���ȏ�Ȃ���΂����܂���B");
	LPCTSTR ERROR_ANGLE_MORE_THAN_20               = _T("%s�̊p�x��%.1lf���ł����A���K�����W������1000�ȏ�̏ꍇ��20���ȏ�Ȃ���΂����܂���B");
	LPCTSTR ERROR_SHARP_TURN                       = _T("�s�p�^�[�����̓p���X�`�󂪑��݂��܂��i臒l�F%d�x�j�B");
	LPCTSTR ERROR_LIGHTNING                        = _T("��Ȍ`�󂪑��݂��܂��i臒l:%d�x�j�B");
	LPCTSTR ERROR_NON_GEOMETRY_ON_FEATURE          = _T("�t�B�[�`���̒��ɃW�I���g�������݂��܂���B");
	LPCTSTR MESSAGE_STOP_PROCESS                   = _T("%s\n\n�����𒆒f���܂��B");
	LPCTSTR CONFIRM_CONTINUE                       = _T("%s\n\n�������p�����܂����H");
	LPCTSTR CONFIRM_KEEP_ATTRIBUTE                 = _T("�m�[�h�ɐڑ����铹�H�����N��2�{�ȉ��ł����A�m�[�h�ɐM���@�t���O�y�ь����_���̂������Ă��܂��iOBJECTID=%d�j�B\n�������c���܂����H");
	LPCTSTR RECONFIGURATION_OF_NODE_TYPE           = _T("�m�[�h��ʍĐݒ�");
	LPCTSTR ERROR_NO_NETWORK_GEOMETRY              = _T("�l�b�g���[�N�W�I���g�������݂��܂���");
	LPCTSTR ERROR_NOT_JUST_2_NODES_ARE_CONNECTED   = _T("�l�b�g���[�N�����N�ɐڑ����Ă���m�[�h��2�ł͂���܂���B");
	LPCTSTR ERROR_NETWORK_LINK_LOOP                = _T("�l�b�g���[�N�����N�����[�v���Ă��܂��B");
	LPCTSTR ERROR_MORE_THAN_1_LINK_HAVE_SAME_NODE  = _T("�����m�[�h�𗼒[�ɂ������N���������݂��܂��B");
#else
	LPCTSTR EDITING_WARN                           = _T("Editing warning");
	LPCTSTR EDITING_PROHIBITED                     = _T("Editing prohibited");
	LPCTSTR DELETE_LINKQUEUE                       = _T("Delete the link queue");
	LPCTSTR CHECK_SHAPE                            = _T("Shape check");
	LPCTSTR INVALID_FEATURE                        = _T("Invalid feature");
	LPCTSTR WARN_BELONGS_TO_UNDISP                 = _T("Layer: %s OBJECTID: %d: Belongs to an undisplayed layer.\n\nEdit?");
	LPCTSTR ERROR_EDIT_ON_FOLLOWING_RULE_LAYER_OID = _T("Layer: %s OBJECTID: %d: Editing is not possible because of the following rule.\n\n%s");
	LPCTSTR WARN_EDIT_ON_FOLLOWING_RULE            = _T("Layer: %s OBJECTID: %d: Set to the warning level because of the following rule.\n\n%s\n Edit?");
	LPCTSTR ERROR_EDIT_ON_FOLLOWING_RULE_LAYER     = _T("Layer: %s: Editing is not possible because of the following rule.\n\n%s");
	LPCTSTR ERROR_CANNOT_CREATE_LAYER              = _T("Layer that cannot be created");
	LPCTSTR ERROR_IS_LOCKAREA                      = _T("This area is locked.");
	LPCTSTR ERROR_IS_OUT_OF_EDITAREA               = _T("This area is not edited.");
	LPCTSTR CONFIRM_DELETE_LINKQUEUE               = _T("There is a link queue in the link. \n Do you want to delete the link and the link queue?");
	LPCTSTR ERROR_NOT_FOUND_ROADLINK_REL_LINKQUEUE = _T("Road link related to the link queue was not found :OBJECTID=%d");
	LPCTSTR ERROR_IS_NOT_EDITABLE_AREA             = _T("Editing is not possible because the area cannot be edited.");
	LPCTSTR ERROR_IS_LOCKMESH                      = _T("Editing is not possible because of lock mesh.");
	LPCTSTR ERROR_EXIST_NODE_IN_MESH_CORNER        = _T("Nodes exist in the four corners of the secondary mesh.");
	LPCTSTR ERROR_LINK_LOOP                        = _T("The link is a loop.");
	LPCTSTR ERROR_LINK_SELF_INTERSECT              = _T("The link is self-intersecting.");
	LPCTSTR ERROR_POLYGON_SELP_INTERSECT           = _T("A shape intersects itself.");
	LPCTSTR ERROR_NON_EXIST_SHAPE                  = _T("A shape does not exist.");
	LPCTSTR ERROR_EIGHT_POLYGON                    = _T("Polygon of number eight, composition points are duplicated, or very small segments exist.");
	LPCTSTR ERROR_OUTSIDE_POLYGON                  = _T("Two or more polygons exist on the outside.");
	LPCTSTR ERROR_IS_NOT_CHECK_TARGET              = _T("The check is not possible because the geometry is not a shape check target.");
	LPCTSTR ERROR_IS_SILVER_POLYGON                = _T("A sliver polygon may exist.");
	LPCTSTR ERROR_TWIST_POLYGON                    = _T("The shape is twisted.");
	LPCTSTR ERROR_POLYLINE_COMPOSITION_POINT       = _T("There is no more than one composition point for the polyline.");
	LPCTSTR ERROR_POLYGON_COMPOSITIOIN_POINT       = _T("There are no more than two composition points for the polylgon.");
	LPCTSTR ERROR_SILVER_SEGMENT                   = _T("A sliver segment exists (%lf cm). ");
	LPCTSTR ERROR_SEGMENT_EXIST_MESH               = _T("A segment exists on the 2D mesh boundary line.");
	LPCTSTR START_VECTOR                           = _T("Start vector (%.1lfcm) and next vector");
	LPCTSTR END_VECTOR                             = _T("End vector (%.1lfcm) and previous vector");
	LPCTSTR ERROR_ANGLE_MORE_THAN_170              = _T("The angel of %s is %.1lf��, but if it is less than 50 in the normalized coordinates distance, it must be over 170��");
	LPCTSTR ERROR_ANGLE_MORE_THAN_150              = _T("The angel of %s is %.1lf��, but if it is between 50 and 100 in the normalized coordinates distance, it must be over 150��");
	LPCTSTR ERROR_ANGLE_MORE_THAN_120              = _T("The angel of %s is %.1lf��, but if it is between 100 and 250 in the normalized coordinates distance, it must be over 120��");
	LPCTSTR ERROR_ANGLE_MORE_THAN_90               = _T("The angel of %s is %.1lf��, but if it is between 250 and 500 in the normalized coordinates distance, it must be over 90��");
	LPCTSTR ERROR_ANGLE_MORE_THAN_40               = _T("The angel of %s is %.1lf��, but if it is between 500 and 1000 in the normalized coordinates distance, it must be over 40��");
	LPCTSTR ERROR_ANGLE_MORE_THAN_20               = _T("The angel of %s is %.1lf��, but if it is over 1000 in the normalized coordinates distance, it must be over 20��");
	LPCTSTR ERROR_SHARP_TURN                       = _T("A sharp turn or pulse shape exists (threshold: %d degrees).");
	LPCTSTR ERROR_LIGHTNING                        = _T("A lightning bolt shape exists (threshold: %d degrees).");
	LPCTSTR ERROR_NON_GEOMETRY_ON_FEATURE          = _T("The geometry does not exist in shape.");
	LPCTSTR MESSAGE_STOP_PROCESS                   = _T("%s\n\nStop processing.");
	LPCTSTR CONFIRM_CONTINUE                       = _T("%s\n\nContinue processing?");
	LPCTSTR CONFIRM_KEEP_ATTRIBUTE                 = _T("Although there are less than 2 Road_Links are connected to the node, there is a signal flag or intersection name(OBJECTID=%d). \nDo you want to keep the attribute?");
	LPCTSTR RECONFIGURATION_OF_NODE_TYPE           = _T("Reconfiguration of node type");
	LPCTSTR ERROR_NO_NETWORK_GEOMETRY              = _T("There is no network geometry");
	LPCTSTR ERROR_NOT_JUST_2_NODES_ARE_CONNECTED   = _T("Not just two nodes are connected to the network link.");
	LPCTSTR ERROR_NETWORK_LINK_LOOP                = _T("Network link is a loop");
	LPCTSTR ERROR_MORE_THAN_1_LINK_HAVE_SAME_NODE  = _T("There is more than one link having same nodes on both sides");
#endif // SINDY_FOR_ENGLISH
}

INT AheGetTolerance( ISiNDYRulePtr ipRule/* = NULL*/ )
{ 
	INT nTolerance = 0; 
	if( ipRule == NULL ) ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	ipRule->GetSelectTolerance( &nTolerance ); 
	return nTolerance; 
}

BOOL AheIsEditableFeature2( IMap* ipMap, _IRow* ipFeature, IGeometry* ipGeom, sindyeEditType type, ISiNDYRule* ipRule/* = NULL*/, BOOL bNotEditableIsNotShowDlg/* = FALSE*/, HWND hWnd/* = NULL*/ )
{
	// �����C���ɑ����Ă��邩�ǂ����`�F�b�N����ibug 3901�j
	if( ! AheIsVisibleFeature( ipMap, (IFeaturePtr)ipFeature ) )
	{
		long lOID = 0;
		ipFeature->get_OID( &lOID );

		CString strMsg;
		strMsg.Format(WARN_BELONGS_TO_UNDISP, AheGetFeatureClassName( ipFeature ), lOID );
		if( IDYES != AheMessageBox( strMsg, EDITING_WARN, MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH ) )
			return FALSE;
	}

	return AheIsEditableFeature( ipFeature, ipGeom, type, ipRule, bNotEditableIsNotShowDlg, hWnd );
}

BOOL AheIsEditableFeature( _IRowPtr ipFeature, IGeometryPtr ipGeom, sindyeEditType type, ISiNDYRulePtr ipRule/* = NULL*/, BOOL bNotEditableIsNotShowDlg/* = FALSE*/, HWND hWnd/* = NULL*/ )
{
	if( ipFeature == NULL )
	{
		_ASSERTE( ipFeature != NULL );
		return FALSE;
	}

	if( ipRule == NULL ) ipRule.CreateInstance( CLSID_SiNDYRuleObj );

	CComBSTR bstrErrMsg;
	sindyeEditableType editableType;
	BOOL bEditable = TRUE;
	CString strMessage;
	LONG lOID = -1;
	INT iMsgRet;

	ipFeature->get_OID( &lOID );

	USES_CONVERSION;

	ipRule->IsEditableFeature( ipFeature, ipGeom, type, &editableType, &bstrErrMsg );
	switch( editableType )
	{
	case sindyeEditableEditable: bEditable = TRUE; break;
	case sindyeEditableNoEditable:
		if( bNotEditableIsNotShowDlg == FALSE ) 
		{
			strMessage.Format( ERROR_EDIT_ON_FOLLOWING_RULE_LAYER_OID, AheGetFeatureClassName( ipFeature ), lOID, OLE2T(bstrErrMsg) );
			::AheMessageBox( strMessage, EDITING_PROHIBITED, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
		}
		bEditable = FALSE;
		break;
	case sindyeEditableWarnning:
		strMessage.Format(WARN_EDIT_ON_FOLLOWING_RULE, AheGetFeatureClassName( ipFeature ), lOID, OLE2T(bstrErrMsg) );
		iMsgRet = ::AheMessageBox( strMessage, EDITING_WARN, MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH );
		if( iMsgRet != IDYES )
			bEditable = FALSE;
		else
			bEditable = TRUE;
		break;
	default:
		break;
	}

	return bEditable;
}

BOOL AheIsCreatableLayer( ITablePtr ipTable, IGeometryPtr ipCreateArea, BOOL bNotEditableIsNotShowDlg/* = FALSE*/, HWND hWnd/* = NULL*/ )
{
	if( ipTable == NULL || ipCreateArea == NULL )
	{
		_ASSERTE( ipTable != NULL && ipCreateArea != NULL );
		return FALSE;
	}

	// �e�[�u�����ƃI�[�i�[�����擾
	CString strTableName( AheGetFeatureClassName( ipTable ) );
	CString strOwnerName( AheGetFeatureClassOwnerName( ipTable ) );

	ISiNDYRulePtr ipRule( CLSID_SiNDYRuleObj );
	CString strMessage;
	
	USES_CONVERSION;

	// �쐬�\���ǂ������`�F�b�N
	VARIANT_BOOL vbCreatable = VARIANT_FALSE;
	ipRule->IsCreatableLayer( CComBSTR(strTableName), &vbCreatable );
	if( vbCreatable == VARIANT_FALSE )
	{
		if( bNotEditableIsNotShowDlg == FALSE )
		{
			strMessage.Format(ERROR_EDIT_ON_FOLLOWING_RULE_LAYER, strTableName, ERROR_CANNOT_CREATE_LAYER );
			AheMessageBox( hWnd, strMessage, EDITING_PROHIBITED, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
		}

		return FALSE;
	}
	// LockArea���ǂ������`�F�b�N
	VARIANT_BOOL vbIsLockArea = VARIANT_TRUE;
	ipRule->IsLockArea( CComBSTR( strOwnerName ), CComBSTR( strTableName ), ipCreateArea, &vbIsLockArea );
	if( vbIsLockArea == VARIANT_TRUE )
	{
		if( bNotEditableIsNotShowDlg == FALSE )
		{
			strMessage.Format(ERROR_EDIT_ON_FOLLOWING_RULE_LAYER, strTableName, ERROR_IS_LOCKAREA );
			AheMessageBox( hWnd, strMessage, EDITING_PROHIBITED, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
		}

		return FALSE;
	}
	// EditArea���ǂ������`�F�b�N
	VARIANT_BOOL vbIsEditArea = VARIANT_FALSE;
	ipRule->IsEditArea( CComBSTR( strTableName ), ipCreateArea, &vbIsEditArea );
	if( vbIsEditArea == VARIANT_FALSE )
	{
		if( bNotEditableIsNotShowDlg == FALSE )
		{
			strMessage.Format(ERROR_EDIT_ON_FOLLOWING_RULE_LAYER, strTableName, ERROR_IS_OUT_OF_EDITAREA );
			AheMessageBox( hWnd, strMessage, EDITING_PROHIBITED, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
		}

		return FALSE;
	}

	return TRUE;
}

// �폜�\���C�����ǂ���
BOOL AheIsDeletableLayer( LPCTSTR lpcszLayerName, ISiNDYRulePtr ipRule/* = NULL*/ )
{ 
	_ASSERTE( _T("�g��Ȃ����ƁI�I") );
	VARIANT_BOOL vaIsDeletable = VARIANT_FALSE; 
	if( ipRule == NULL ) ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	ipRule->IsDeletableLayer( CComBSTR(lpcszLayerName), &vaIsDeletable );
	return ( vaIsDeletable == VARIANT_TRUE ) ? TRUE : FALSE;
}

// �����ҏW�\���C�����ǂ���
BOOL AheIsAttrEditableLayer( LPCTSTR lpcszLayerName, ISiNDYRulePtr ipRule/* = NULL*/ )
{ 
	_ASSERTE( _T("�g��Ȃ����ƁI�I") );
	VARIANT_BOOL vaIsAttrEditable = VARIANT_FALSE; 
	if( ipRule == NULL ) ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	ipRule->IsAttributeEditableLayer( CComBSTR(lpcszLayerName), &vaIsAttrEditable );
	return ( vaIsAttrEditable == VARIANT_TRUE ) ? TRUE : FALSE;
}

BOOL AheIsLockArea( LPCTSTR lpcszOwnerName, LPCTSTR lpcszLayerName, IGeometryPtr ipGeom, ISiNDYRulePtr ipRule = NULL )
{
	// ����J�֐�(bug 4938�΍�j
	if( ipRule == NULL )
		ipRule.CreateInstance( CLSID_SiNDYRuleObj );

	VARIANT_BOOL vaIsLock = VARIANT_FALSE;
	ipRule->IsLockArea( CComBSTR( lpcszOwnerName ), CComBSTR( lpcszLayerName ), ipGeom, &vaIsLock );
	
	return ( vaIsLock == VARIANT_TRUE ) ? TRUE : FALSE;
}

BOOL AheResetNodeAttribute( IFeaturePtr ipNodeFeature, CTargetItems& cRoadItems, BOOL* bIsChanged/* = NULL*/, BOOL bSetModifyData/* = FALSE */ )
{
	if( !ipNodeFeature ) return FALSE;

	BOOL bHasNodeAttr = FALSE;
	BOOL bDeleteNodeAttr = FALSE;
	// [�����^�]�x���}�b�v] �Ԑ��E�㉺�����f���Ή�
	BOOL bIsLaneUpDown = FALSE;

	if( bIsChanged ) *bIsChanged = FALSE;

	long nodeclass = 0;
	switch ( AheGetNetWorkType(ipNodeFeature) )
	{
	case sindyeNetworkRoad:
		nodeclass = (long)AheGetModifyRoadNodeKind( ipNodeFeature, cRoadItems, &bHasNodeAttr );
		break;
	case sindyeNetworkWalk:
		nodeclass = (long)AheGetModifyWalkNodeKind( ipNodeFeature, cRoadItems );
		break;
	case sindyeNetworkSubway:
		nodeclass = (long)AheGetModifySubwayNodeKind( ipNodeFeature, cRoadItems  );
		break;
	case sindyeNetworkRailway:
		nodeclass = (long)AheGetModifyRailwayNodeKind( ipNodeFeature, cRoadItems );
		break;
	// [�����^�]�x���}�b�v] �Ԑ��E�㉺�����f���Ή�
	case sindyeNetworkLane:
	case sindyeNetworkUpDown:
	case sindyeNetworkCompart:
	case sindyeNetworkBorder:
		bIsLaneUpDown = TRUE;
		break;
	case sindyeNotNetwork:
	default:
		break;
	}

#ifndef __SINDYE_NO_CHECK_NODE_ATTRIBUTE__
	// �m�[�h�ɐڑ����铹�H�����N��2�{�ȉ��Ȃ̂Ɍ����_���̂܂��͐M���@�t���O�����Ă�����폜���邩�ǂ����m�F����
	if( bHasNodeAttr == TRUE )
	{
		LONG lOID = -1;
		CString strMsg;

		ipNodeFeature->get_OID( &lOID );
		strMsg.Format(CONFIRM_KEEP_ATTRIBUTE, lOID );
		if( AheMessageBox( strMsg, RECONFIGURATION_OF_NODE_TYPE, MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH ) == IDNO )
		{
			CComVariant vaValue;
			vaValue.vt = VT_NULL;	// VT_NULL �� VARIANT �� CComVariant �̃R���X�g���N�^�ł͍��Ȃ� 
			AheSetAttribute( ipNodeFeature, _T("SIGNAL_C"), CComVariant(0L) );
			AheSetAttribute( ipNodeFeature, _T("NAME_KANJI"), vaValue );
			AheSetAttribute( ipNodeFeature, _T("NAME_YOMI"), vaValue );

			bDeleteNodeAttr = TRUE;

			// �m�[�h�̑������ς���Ă���̂ŁA�ēx��ʂ��擾
			nodeclass = (long)AheGetModifyNodeKind( ipNodeFeature, cRoadItems, &bHasNodeAttr );
		}
	}
#endif	// __SINDYE_NO_CHECK_NODE_ATTRIBUTE__

	// [�����^�]�x���}�b�v] �Ԑ��E�㉺�����f���Ή�
	// �Ԑ��E�㉺�����f���ɂ́A�m�[�h�������Ȃ��̂Ń`�F�b�N�͂��Ȃ�
	// TODO: �Ԑ��E�㉺�����f���Ƀm�[�h�������ǉ�����Ă���C���̂��ƁB
	if(bIsLaneUpDown)
	{
		return TRUE;
	}

	CComVariant vaValue( nodeclass );
	if( nodeclass < 0 ) 
		return FALSE;

	// �m�[�h�̎�ʂ��قȂ�ꍇ�̂ݕύX
	if( bDeleteNodeAttr == TRUE || vaValue != AheGetAttribute( ipNodeFeature, NODECLASS_FIELD ) )
	{
		AheSetAttribute( ipNodeFeature, NODECLASS_FIELD, vaValue );
		if( bIsChanged ) *bIsChanged = TRUE;	// �ύX���ꂽ

		// �����m�[�h�̏ꍇ�̂݁A�����ύX�̗�����������
		if( bSetModifyData == TRUE )
			AheSetModifyData( ipNodeFeature, sindyUpdateTypeProperty );
	}

	return TRUE;
}

sindyeNodeClass AheGetModifyRoadNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems, BOOL* pbHasNodeAttr )
{
	if( ! ( ipNodeFeature && pbHasNodeAttr ) )
		return (sindyeNodeClass)-1;

	// ������
	(*pbHasNodeAttr) = FALSE;

	// �m�[�h��ʂɂ́A��܂��ɕ�����3��ނ���B
	// 
	// �E2�����b�V��������ɂ���ꍇ�̎��
	//   �E2�����b�V��������PEC�m�[�h
	//   �E2�����b�V��������m�[�h
	// �E�ҏW�҂��Ӑ}�I�ɕt�^�������
	//   �E�������m�[�h
	//   �E�n�C�E�F�C�֌W�m�[�h
	//   �E�_�~�[�{�݃m�[�h�iTF�p�j
	//   �E���ꃊ���NID�h�~�m�[�h
	// �E�ڑ����H�����N�ƃm�[�h�̑����݂̂Ō��܂�ꍇ�̎��
	//   �E�[�_�m�[�h <- PEC�����N���ڑ����Ă����ꍇ���[�_�m�[�h�ł����񂾂����H
	//   �EPEC�����N�ڑ��m�[�h
	//   �E�����_�m�[�h
	//   �E�����ω��_�m�[�h
	//   �E�����t���m�[�h
	//   �E�Ӗ��Ȃ��m�[�h
	//   �E�m�[�h���_�m�[�h�i�g�p���ĂȂ��H�j
	//
	// ���[���F
	// 1. ��L��3�O���[�v�ł͏�̕����D�揇�ʂ�����
	// 2. ��Ԗڂ̃O���[�v���̎�ʂ͏�������O���[�v��艺�̃O���[�v�̎�ʂ�
	//    �ύX����邱�Ƃ͂Ȃ�
	// 3. ��Ԗڂ̃O���[�v���̎�ʂ͕ҏW�҂ɂ���ĉ��̃O���[�v�̎�ʂɕύX��
	//    ��Ȃ����艺�̃O���[�v�̎�ʂɎ����I�ɕύX����邱�Ƃ͂Ȃ�
	// 4. ��ԖڂƎO�Ԗڂ͎����t�^�ł��邪�A��Ԗڂ̓��[�U�t�^�ł���A���炷
	//    ��K�v������
	// 
	// �]���āA�����t�^�̍ۂɈȑO�̑������C�ɂ���K�v������͓̂�Ԗڂ̃O���[�v
	// �ɏ������鑮���݂̂ł���A����ȊO�͌��݂̑����݂̂Ō��߂Ă��܂��Ă悢�B
	// 
	// �A���S���Y���I�ɂ́A
	// �E���b�V�������ォ�ǂ����m�F
	//   �E������Ȃ�return
	// �E�O�̎�ʂ��ҏW�҂��Ӑ}�I�ɕt�^������ʂ��ǂ����m�F
	//   �E�Ӑ}�I�ɕt�^������ʂȂ�ύX�Ȃ���return
	// �E����ȊO�͐ڑ��󋵂Ƒ����Ŕ��f

	//////////////////////////////////////////
	// �O����
	//////////////////////////////////////////
	IGeometryPtr ipNodeGeom;
	ipNodeFeature->get_ShapeCopy( &ipNodeGeom );
	// �Ȃ��� ipNodeGeom �� SpatialReference �������Ă��Ȃ��ꍇ����I�I
	// e.x. SketchTool �Ŕ�����œ��H�����N�ɗ��[�_���X�i�b�v���ē��H�����N���쐬�����ꍇ
	// �d�����Ȃ��̂Ńm�[�h�̃t�B�[�`���N���X�o�R�� SpatialReference �擾
	IGeoDatasetPtr ipNodeDataset = AheGetFeatureClass( ipNodeFeature );
	ISpatialReferencePtr ipSpRef;
	if( ipNodeDataset )
		ipNodeDataset->get_SpatialReference( &ipSpRef );

	// ���ۂɐڑ�����Ă��铹�H����肷��
	CTargetItems cAllRoadItems, cRoadItems;
	cAllRoadItems.clear(), cRoadItems.clear();

	cFeatureItems.GetRoadFeatures( cAllRoadItems );
	for( CTargetItems::iterator it = cAllRoadItems.begin(); it != cAllRoadItems.end(); it++ )
	{
		//if( it->IsEndPoint( ipNodeGeom, ipSpRef ) )
		if( AheIsEndPoint( (IPointPtr)ipNodeGeom, (ICurvePtr)it->GetShapeCopy(), ipSpRef ) ) // �R�s�[�Ƃ�Ȃ��Ƃ��߁H
			cRoadItems.push_back( *it );
	}

	// �ڑ������N���`�F�b�N	
	// ���H�����N��2�{�ȉ��ŁA���m�[�h�ɐM���@�t���O�y�ь����_���̂������Ă���ꍇ�̓t���O�𗧂Ă�
	if( cRoadItems.size() <= 0 ) 
		return (sindyeNodeClass)-1;
	else if( cRoadItems.size() < 3 )
	{
		if( AheIsExistAttrNode( ipNodeFeature ) )
			(*pbHasNodeAttr) = TRUE;
	}

	//////////////////////////////////////////
	// 2�����b�V�������ォ�ǂ�������
	//////////////////////////////////////////
	CMesh mesh;
	mesh.SetSpatialReference( ipSpRef );	// �m�[�h�� SpatialReference ���Z�b�g���Ă��
	std::list<UINT> listMeshCode;
	if( mesh.IsTouches( ipNodeGeom, listMeshCode ) )
	{
		// ���b�V����؂��PGDB�ł́A�m�[�h��2�����b�V���m�[�h�ł��Е��ɂ������H�����N���Ȃ�
		// ���̂��߂ɕK���G���[�ɂȂ��Ă��܂��̂ŁA���W�X�g�������ē��H�̖{���𖳎����邩�ǂ���
		// �����߂�
		if( AheIsPGDBEditMode() )
		{
			if( 1 == cRoadItems.size() )
			{
				CTargetItems::iterator it1 = cRoadItems.begin();
				if( it1->IsPECLink() )
					return sindyeOnMeshPEC;
				else
					return sindyeOnMesh;
			}
		}
		else {
			if( 2 == cRoadItems.size() )
			{
				CTargetItems::iterator it1 = cRoadItems.begin();
				CTargetItems::iterator it2 = cRoadItems.begin(); advance( it2, 1 );
				if( AheIsSameAttributes( it1->GetFeature(), it2->GetFeature() ) )
				{
					// �����Ƃ�PEC�����N�Ȃ�2�����b�V�����ӏ�PEC�m�[�h�A�����łȂ����2�����b�V�����ӏ�m�[�h
					if( it1->IsPECLink() == TRUE && it2->IsPECLink() )
						return sindyeOnMeshPEC;
					else
						return sindyeOnMesh;
				}
				else
				{
					// 2�����b�V�����E���ɐڂ��Ă���̂ɐڑ����铹�H�����N�̎�ʂ��قȂ�I�I
					_ASSERTE( mesh.IsTouches( ipNodeGeom, listMeshCode ) && ( ! AheIsSameAttributes( it1->GetFeature(), it2->GetFeature() ) ) );
					return (sindyeNodeClass)-1;
				}
			}
			else {
				// 2�����b�V�����E���ɐڂ��Ă���̂ɐڑ����铹�H��2�{�ł͂Ȃ��I�I
				_ASSERTE( mesh.IsTouches( ipNodeGeom, listMeshCode ) && 2 != cRoadItems.size() );
				return (sindyeNodeClass)-1;
			}
		}
	}

	//////////////////////////////////////////
	// �����̎�ʂ��Ӑ}�I�ɕt�^���ꂽ��ʂ��ǂ����𔻒�
	//////////////////////////////////////////
	// ���̃m�[�h��ʂ��擾
	CComVariant vaNodeClassVal = AheGetAttribute( ipNodeFeature, NODECLASS_FIELD );
	if( VT_NULL == vaNodeClassVal.vt || VT_I4 == vaNodeClassVal.vt )
	{
		sindyeNodeClass nodeclass = ( vaNodeClassVal.vt == VT_NULL ) ? (sindyeNodeClass)-1 : (sindyeNodeClass)vaNodeClassVal.lVal;

		switch( nodeclass )
		{
			case sindyeDammyNode:
			case sindyeHWYChargePoint:
			case sindyeRelHWY:
			case sindyeNodeNQStartPoint: return nodeclass; break;
			default: break;
		}
	}
	else
		_ASSERTE( VT_NULL == vaNodeClassVal.vt || VT_I4 == vaNodeClassVal.vt );

	//////////////////////////////////////////
	// �ڑ��󋵂Ƒ����Ŕ���
	//////////////////////////////////////////
	sindyeNodeClass nodeclass = sindyeNodeNoDef;
	// ��{�̏ꍇ�͒[�_�m�[�h
	if( cRoadItems.size() == 1 )
		nodeclass = sindyeNodeEndPoint;
	// ��{�ȏ�̏ꍇ
    else
	{
		BOOL bIsNormalRoad = FALSE, bIsPECRoad = FALSE;
		for( CTargetItems::iterator it = cRoadItems.begin(); it != cRoadItems.end(); ++it )
		{
			if( it->IsPECLink() )
				bIsPECRoad = TRUE;
			else
				bIsNormalRoad = TRUE;
		}
		// �S��PEC�����N�Ȃ�PEC�m�[�h
		// ��ł�PEC�����N������Ȃ�PEC�����N�ڑ��m�[�h
		// �c���ꂽ���H�����N��3�{�ȏ��PEC�����N���Ȃ��Ȃ�����_�m�[�h
		nodeclass = ( ! bIsPECRoad ) ? sindyeNodeIntersection : ( ( bIsNormalRoad ) ? sindyeNodeConnectPecLink : sindyePECNode );

		// �O�{�ȏ�̏ꍇ�͂����ŏI��
		// ��{�̏ꍇ�ŁA����L�����sindyeNodeIntersection�̏ꍇ�͂���ɒ��ׂ�
		if( cRoadItems.size() == 2 && sindyeNodeIntersection == nodeclass )
		{
			CTargetItems::const_iterator itRoads1 = cRoadItems.begin();
			CTargetItems::const_iterator itRoads2 = itRoads1; ++itRoads2;

			// �ڑ����铹�H�����N���قȂ鑮�������̂ł���Α����ω��_
			if( ! AheIsSameAttributes( itRoads1->GetFeature(), itRoads2->GetFeature() ) )
				nodeclass = sindyeNodeAttrChanged;
			// �m�[�h�̌����_�E�����������݂āA����Α����t���m�[�h
			else if( *pbHasNodeAttr )
				nodeclass = sindyeNodeExistAttr;
			// �ڑ����铹�H�ŁA�^�[�Q�b�g�ȊO�̃m�[�hID��FROM�ETO�Ɏ����̂�
			// ����Γ��ꃊ���NID�h�~�m�[�h�ɂ���
			// �Ȃ���ΈӖ������m�[�h
			else {
				// �^�[�Q�b�g�ȊO�̃m�[�hID�����W����
				long lNodeOID = AheGetOID( ipNodeFeature ), lOtherNodeID1 = -1, lOtherNodeID2 = -1; // �^�[�Q�b�g�ȊO�̃m�[�hID
				for( CTargetItems::const_iterator it = cRoadItems.begin(); it != cRoadItems.end(); ++it )
				{
					CComVariant vaFromID = AheGetAttribute( it->GetFeature(), FROM_FIELD );
					CComVariant vaToID = AheGetAttribute( it->GetFeature(), TO_FIELD );
					_ASSERTE( VT_I4 == vaFromID.vt && VT_I4 == vaToID.vt );

					if( -1 == lOtherNodeID1 )
						lOtherNodeID1 = ( lNodeOID == vaFromID.lVal ) ? vaToID.lVal : vaFromID.lVal;
					else
						lOtherNodeID2 = ( lNodeOID == vaFromID.lVal ) ? vaToID.lVal : vaFromID.lVal;
				}
				// �^�[�Q�b�g�ȊO�̃m�[�hID�������̂��n����Ă��邩�ǂ����`�F�b�N
				bool bFind = false; // �����������ǂ����̃t���O
				for( CTargetItems::const_iterator it = cFeatureItems.begin(); it != cFeatureItems.end(); ++it )
				{
					CComVariant vaFromID = AheGetAttribute( it->GetFeature(), FROM_FIELD );
					CComVariant vaToID = AheGetAttribute( it->GetFeature(), TO_FIELD );
					_ASSERTE( VT_I4 == vaFromID.vt && VT_I4 == vaToID.vt );

					if( ( lOtherNodeID1 == vaFromID.lVal && lOtherNodeID2 == vaToID.lVal ) ||
						( lOtherNodeID2 == vaFromID.lVal && lOtherNodeID1 == vaToID.lVal ) )
					{
						bFind = true;
						break;
					}
				}
				nodeclass = ( bFind ) ? sindyeCheckSameID : sindyeNodeNoDef;
			}
		}
	}
	return nodeclass;
}

// 2�����b�V��������m�[�h����i����J�j
// �n���S�H���Ή��̍ۂ�AheGetModifyWalkNodeKind()���番��
// TODO:AheGetModifyRoadNodeKind()����{�������Ƃ�����Ă���̂ł�����g���悤�ɂ���
bool OnMeshEdgeNode( const IFeaturePtr& ipNodeFeature, CTargetItems& cFeatureItems, CTargetItems& cRoadItems, bool& hasErr )
{
	hasErr = false;

	if( ! ipNodeFeature )
	{
		hasErr = true;
		return false;
	}

	//////////////////////////////////////////
	// �O����
	//////////////////////////////////////////
	IGeometryPtr ipNodeGeom;
	ipNodeFeature->get_ShapeCopy( &ipNodeGeom );
	// �Ȃ��� ipNodeGeom �� SpatialReference �������Ă��Ȃ��ꍇ����I�I
	// e.x. SketchTool �Ŕ�����œ��H�����N�ɗ��[�_���X�i�b�v���ē��H�����N���쐬�����ꍇ
	// �d�����Ȃ��̂Ńm�[�h�̃t�B�[�`���N���X�o�R�� SpatialReference �擾
	IGeoDatasetPtr ipNodeDataset = AheGetFeatureClass( ipNodeFeature );
	ISpatialReferencePtr ipSpRef;
	if( ipNodeDataset )
		ipNodeDataset->get_SpatialReference( &ipSpRef );

	// ���ۂɐڑ�����Ă��铹�H����肷��
	CTargetItems cAllRoadItems;
	cFeatureItems.GetNetworkLinkFeatures( cAllRoadItems );
	for( CTargetItems::iterator it = cAllRoadItems.begin(); it != cAllRoadItems.end(); it++ )
	{
		if( it->IsEndPoint( ipNodeGeom, ipSpRef ) )
			cRoadItems.push_back( *it );
	}

	// �ڑ������N���`�F�b�N	
	if( cRoadItems.empty() ) 
	{
		hasErr = true;
		return false;
	}

	//////////////////////////////////////////
	// 2�����b�V�������ォ�ǂ�������
	//////////////////////////////////////////
	CMesh mesh;
	mesh.SetSpatialReference( ipSpRef );	// �m�[�h�� SpatialReference ���Z�b�g���Ă��
	std::list<UINT> listMeshCode;
	if( mesh.IsTouches( ipNodeGeom, listMeshCode ) )
	{
		// ���b�V����؂��PGDB�ł́A�m�[�h��2�����b�V���m�[�h�ł��Е��ɂ������H�����N���Ȃ�
		// ���̂��߂ɕK���G���[�ɂȂ��Ă��܂��̂ŁA���W�X�g�������ē��H�̖{���𖳎����邩�ǂ���
		// �����߂�
		if( AheIsPGDBEditMode() )
		{
			if( 1 == cRoadItems.size() )
			{
				return true;
			}
		}
		else {
			if( 2 == cRoadItems.size() )
			{
				CTargetItems::iterator it1 = cRoadItems.begin();
				CTargetItems::iterator it2 = cRoadItems.begin(); advance( it2, 1 );
				if( AheIsSameAttributes( it1->GetFeature(), it2->GetFeature() ) )
				{
					return true;
				}
				else
				{
					// 2�����b�V�����E���ɐڂ��Ă���̂ɐڑ����铹�H�����N�̎�ʂ��قȂ�I�I
					_ASSERTE( mesh.IsTouches( ipNodeGeom, listMeshCode ) && ( ! AheIsSameAttributes( it1->GetFeature(), it2->GetFeature() ) ) );
					hasErr = true;
				}
			}
			else {
				// 2�����b�V�����E���ɐڂ��Ă���̂ɐڑ����铹�H��2�{�ł͂Ȃ��I�I
				_ASSERTE( mesh.IsTouches( ipNodeGeom, listMeshCode ) && 2 != cRoadItems.size() );
				hasErr = true;
			}
		}
	}
	return false;
}

sj::subway_node::node_class::ECode AheGetModifySubwayNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems )
{
	using namespace sj::subway_node::node_class;
	bool hasErr = false;
	CTargetItems cRoadItems;
	if( OnMeshEdgeNode( ipNodeFeature, cFeatureItems, cRoadItems, hasErr ) && !hasErr )
	{
		return kOutline;
	}
	// �}�s��m�[�h�łȂ���ΑS�ĉw�m�[�h�Ƃ���
	return hasErr ? (ECode)-1 : kStation;
}

sindy::schema::sj::railway_node::node_class::ECode AheGetModifyRailwayNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems )
{
	using namespace sj::railway_node::node_class;
	bool hasErr = false;
	CTargetItems cLinkItems;
	if( OnMeshEdgeNode( ipNodeFeature, cFeatureItems, cLinkItems, hasErr ) && !hasErr )
		return kOutline;

	// �G���[���������Ȃ�G���[�p�R�[�h��Ԃ�
	if(hasErr) return (ECode)-1;

	// �wPOI�m�[�h���ݒ肳��Ă���̂ł���Α����͕ύX���Ȃ�
	CComVariant nowNodeClass = AheGetAttribute( ipNodeFeature, sj::railway_node::kNodeClass_C );
	if( nowNodeClass.lVal == kStation )
		return kStation;

	// ��{�̏ꍇ�͒[�_�m�[�h
	if( cLinkItems.size() == 1 )
		return kTerminal;

	// �O�{�ȏ�̏ꍇ�͕���m�[�h
	if( cLinkItems.size() >= 3 )
		return kBranch;

	// ��{�̏ꍇ�͐ڑ�����Ă��郊���N�̑����ɂ���Ď�ʂ�ς���
	CTargetItems::const_iterator itRoads1 = cLinkItems.begin();
	CTargetItems::const_iterator itRoads2 = itRoads1;;
	std::advance(itRoads2, 1);

	// �قȂ鑮�������̂ł���Α����ω��_
	//�i���\�[�X�⎩�����͂����悤�ȑ����͔�r�Ώۂ��珜���j
	if( !AheIsSameAttributes( itRoads1->GetFeature(), itRoads2->GetFeature() ) )
		return kAlter;

	// �������������̂ł���΋@�\�Ȃ��m�[�h
	return kNone;
}

sindy::schema::walk_node::node_class::ECode AheGetModifyWalkNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems )
{
	using namespace sindy::schema::walk_node::node_class;

	bool hasErr = false;
	CTargetItems cRoadItems;
	if( OnMeshEdgeNode( ipNodeFeature, cFeatureItems, cRoadItems, hasErr ) && !hasErr )
	{
		return kOutline;
	}

	if( hasErr )
		return (ECode)-1;

	//////////////////////////////////////////
	// �����̎�ʂ��Ӑ}�I�ɕt�^���ꂽ��ʂ��ǂ����𔻒�
	//////////////////////////////////////////
	// ���̃m�[�h��ʂ��擾
	CComVariant vaNodeClassVal = AheGetAttribute( ipNodeFeature, NODECLASS_FIELD );
	if( VT_NULL == vaNodeClassVal.vt || VT_I4 == vaNodeClassVal.vt )
	{
		ECode nodeclass = ( vaNodeClassVal.vt == VT_NULL ) ? (ECode)-1 : (ECode)vaNodeClassVal.lVal;

		switch( nodeclass )
		{
			case kRoadNW:
			case kGate:
			case kUndergroundGate:
			case kElevator:
			case kKaisatsu:
			case kCarPratform: 
			case kBusPratform:
			case kTaxiPratform:
			case kParking:
			case kDepartureGate:
			case kArrivalGate:
			case kTicketGate:
			case kConnectedRoad:// �_�E���R���o�[�g��p�A�����I�ɂȂ��Ȃ�z��
			case kUnsurveyTicketGate:// �ȈՃf�[�^�Ő�p�A�����I�ɂȂ��Ȃ�z��
				return nodeclass; break;
			default: break;
		}
	}
	else
		_ASSERTE( VT_NULL == vaNodeClassVal.vt || VT_I4 == vaNodeClassVal.vt );

	//////////////////////////////////////////
	// �ڑ��󋵂Ƒ����Ŕ���
	//////////////////////////////////////////
	ECode nodeclass = kPublic;
	// ��{�̏ꍇ�͒[�_�m�[�h
	if( cRoadItems.size() == 1 )
		nodeclass = kTerminal;
	// ��{�ȏ�̏ꍇ
    else
	{
		nodeclass = kPublic;

		// �O�{�ȏ�̏ꍇ�͂����ŏI��
		// ��{�̏ꍇ�͂���ɒ��ׂ�
		if( cRoadItems.size() == 2 )
		{
			CTargetItems::const_iterator itRoads1 = cRoadItems.begin();
			CTargetItems::const_iterator itRoads2 = itRoads1; ++itRoads2;

			// �ڑ����铹�H�����N���قȂ鑮�������̂ł���Α����ω��_
			if( ! AheIsSameAttributes( itRoads1->GetFeature(), itRoads2->GetFeature() ) )
				nodeclass = kAlter;
		}
	}
	return nodeclass;
}

BOOL AheIsExistAttrNode( IFeaturePtr ipNodeFeature, BOOL bCheckAll/* = TRUE*/ )
{
	CComVariant vaValue;

	// �����_�t���O���t���Ă��邩�ǂ���
	vaValue = AheGetAttribute( ipNodeFeature, _T("SIGNAL_C") );
	if( vaValue.vt == VT_I4 && vaValue.lVal == 1 ) return TRUE;

	USES_CONVERSION;

	// �����_���̂����邩�ǂ���
	vaValue = AheGetAttribute( ipNodeFeature, _T("NAME_KANJI") );
	if( vaValue.vt == VT_BSTR && lstrcmpi( _T(""), OLE2T( vaValue.bstrVal ) ) != 0 ) return TRUE;

	// �����_���Ȗ��̂����邩�ǂ���
	vaValue = AheGetAttribute( ipNodeFeature, _T("NAME_YOMI") );
	if( vaValue.vt == VT_BSTR && lstrcmpi( _T(""), OLE2T( vaValue.bstrVal ) ) != 0 ) return TRUE;

	if( bCheckAll == TRUE )
	{
		// �����R�[�h���t���Ă��邩�ǂ���
		vaValue = AheGetAttribute( ipNodeFeature, _T("HEIGHT_C") );
		if( vaValue.vt == VT_I4 && vaValue.lVal != 0 ) return TRUE;

		// �����f�[�^�������Ă��邩�ǂ���
		vaValue = AheGetAttribute( ipNodeFeature, _T("HEIGHT") );
		if( vaValue.vt == VT_I4 && vaValue.lVal != 0 ) return TRUE;
	}

	return FALSE;
}

BOOL AheSelectDeleteLQ( CTargetItems cSelectItems )
{
	CLoadQueueInfo	cLQInfo;
	BOOL			MsgCnt = FALSE;
	BOOL			bRoadIsAttrEditable = FALSE;
	BOOL			bCheckedRoadLayer	= FALSE;

	for( CTargetItems::iterator iCnt = cSelectItems.begin(); iCnt != cSelectItems.end(); iCnt++ ) 
	{
		if ( iCnt->GetFeatureKinds() != sindyeRoadFeature )
			continue;

		// ���H�����N�������ҏW�\���ǂ���
		if( bCheckedRoadLayer == FALSE )
		{
			bCheckedRoadLayer = TRUE;
			bRoadIsAttrEditable = AheIsAttrEditableLayer( ROAD_TABLE_NAME );
		}

		for( INT i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ )
		{
			CString strInfoTableName	= cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoInfoTableName );
			CString strLQTableName		= cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQTableName );
			CString strRoadRelFieldName	= cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName );
			LONG    lDrawMode			= cLQInfo.GetLoadQueueDrawMode( (sindyeRoadQueueType)i );
			CComVariant vaValue = iCnt->GetAttribute( strRoadRelFieldName );

			if ( vaValue.vt == VT_I4 && vaValue.lVal == 1 )
			{
				if ( MsgCnt == FALSE ) {
					if( AheMessageBox( CONFIRM_DELETE_LINKQUEUE, DELETE_LINKQUEUE, MB_ICONQUESTION|MB_OKCANCEL|MB_DEFBUTTON1, WARNLEVEL_MIDDLE ) == IDCANCEL )
						return FALSE;
					MsgCnt = TRUE;
				}
			}

			if( AheIsDeletableLayer( strInfoTableName ) == FALSE ||	bRoadIsAttrEditable == FALSE )
				return FALSE;

			if( AheDeleteLQ( (*iCnt), strInfoTableName, strLQTableName, strRoadRelFieldName, lDrawMode ) == FALSE )
				return FALSE;
		}
	}
	return TRUE;
}

BOOL AheIsEndPoints( IGeometryPtr ipPointGeom, CTargetItems& cTargetItems)
{	
	if( ipPointGeom == NULL ) return FALSE;

	for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++)
	{
		IFeaturePtr ipFeature = it->GetFeature();
		if( AheIsEndPoint( ipPointGeom, ipFeature ) == TRUE )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// �����Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
LONG AheSelectByShape( IMapPtr ipMap, IGeometryPtr ipGeom, CTargetItems& cTargetItems, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bVisibleOnly/* = FALSE*/, BOOL bEditableOnly/* = FALSE*/ )
{
	if( ipMap == NULL )
		return -1;

	CMap cLayers( ipMap );
	for( CMap::const_iterator it = cLayers.begin( LayerTypeFeatureLayer, bEditableOnly ? true : false, bVisibleOnly ? true : false ); it != cLayers.end(); ++it )
	{
		IFeatureClassPtr ipClass( AheGetFeatureClass( *it ) );
		if( ipClass == NULL )
			continue;

		AheSelectByShapeFromFeatureClass( ipGeom, cTargetItems, ipClass, spatialRel, lpcszSubFields, lpcszWhereClause );
	}

	return cTargetItems.size();
}


LONG AheSelectByShapeFromLayer( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/ )
{
	return AheSelectByShapeFromFeatureClass( ipGeometry, cTargetItems, AheGetFeatureClass( ipLayer ), spatialRel, lpcszSubFields, lpcszWhereClause );
}

LONG AheSelectByShapeFromFeatureClass( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/ )
{
	if( ipGeometry == NULL || ipFeatureClass == NULL )
		return -1;
	
	IFeatureCursorPtr ipFeatureCursor = AheSelectByShape( ipGeometry, ipFeatureClass, spatialRel, lpcszSubFields, lpcszWhereClause );
	if( ipFeatureCursor == NULL )
		return 0;

	IGeoDatasetPtr ipDataset( ipFeatureClass );
	ISpatialReferencePtr ipDatasetSpRef;
	
	ipDataset->get_SpatialReference( &ipDatasetSpRef );
	
	IFeaturePtr ipFeature;
	while( SUCCEEDED( ipFeatureCursor->NextFeature( &ipFeature ) ) )
	{
		if( ipFeature == NULL )
			break;

		IGeometryPtr ipGeom;
		ISpatialReferencePtr ipSpRef;
		ipFeature->get_Shape( &ipGeom );
		ipGeom->get_SpatialReference( &ipSpRef );
		//AheTraceSpatialReference( ipSpRef, _T("AheSelectByShapeFromLayer") );
		
		VARIANT_BOOL vaIsSameSpRef = VARIANT_FALSE;
		ipSpRef->IsPrecisionEqual( ipDatasetSpRef, &vaIsSameSpRef );
		if( ! vaIsSameSpRef )
		{
			ATLTRACE(_T("AheSelectByShapeFromLayer : �t�B�[�`���N���X�̋�ԎQ�Ƃƃt�B�[�`�������W�I���g���̋�ԎQ�Ƃ��قȂ�܂��B�t�B�[�`���N���X�̋�ԎQ�ƂɈ�v�����܂��B\n") );
			AheSnapToSpatialReference( ipGeom, ipDatasetSpRef );
		}
		cTargetItems.push_back( ipFeature );
	}

	return cTargetItems.size();
}

LONG AheSelectByShapeFromLayers( IMapPtr piMap, IGeometryPtr ipGeom, CTargetItems& cTargetItems, LPCTSTR lpcszLayerNames, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bEditableOnly/* = FALSE*/ )
{
	// �u|�v�Z�p���[�^�ŕ���������X�g�ɓ˂�����
	std::list<CString> listLayers;
	AheSepStr2listString( lpcszLayerNames, _T("|"), listLayers );

	for( std::list<CString>::iterator it = listLayers.begin(); it != listLayers.end(); it++ )
		AheSelectByShapeFromLayer( ipGeom, cTargetItems, AheGetLayerByTableName( piMap, *it, bEditableOnly ), spatialRel, lpcszSubFields, lpcszWhereClause );

	return cTargetItems.size();
}
/////////////////////////////////////////////////////////////////////////////
//
// �H�H�Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
LONG AheGetConnectedRoad( IFeaturePtr ipFeature, CTargetItems& cTargetItems)
{
	ISpatialFilterPtr ipFilter;
	if( SUCCEEDED( ipFilter.CreateInstance( CLSID_SpatialFilter )))
	{
		IQueryFilterPtr ipQueryFilter( ipFilter );
		if( SUCCEEDED( ipFilter->QueryInterface( &ipQueryFilter)))
		{
			// �m�[�h�t�B�[�`������
			if( AheGetFeatureKinds( ipFeature ) != sindyeNodeFeature )
			{
				_ASSERTE( FALSE );
				return NULL;
			}

			// ���H�N���X��n���ꂽ�t�B�[�`�����猟������
			IFeatureClassPtr ipRoadClass( AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), ROAD_TABLE_NAME ) );
			if( ! ipRoadClass )
			{
				_ASSERTE( ipRoadClass );
				return NULL;
			}

			// �W�I���g���̎擾
			IGeometryPtr ipGeom;
			ipFeature->get_Shape( &ipGeom );
			if( ipGeom == NULL )
			{
				_ASSERTE( ipGeom != NULL );
				return NULL;
			}

			// �N�G���t�B���^�̍쐬
			ipFilter->putref_Geometry( ipGeom );
			CComBSTR cComBSTR;
			ipRoadClass->get_ShapeFieldName( &cComBSTR );
			ipFilter->put_GeometryField( cComBSTR );
			ipFilter->put_SpatialRel( esriSpatialRelIntersects );

			// ����
			IFeatureCursorPtr ipFeatureCursor;
			if( SUCCEEDED( ipRoadClass->Search( ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor)) && NULL != ipFeatureCursor)
			{
				IFeaturePtr ipFeatureTmp = NULL;
				while( S_OK == ipFeatureCursor->NextFeature( &ipFeatureTmp ))
					cTargetItems.push_back( ipFeatureTmp );
			}
		}
	}

#ifdef DEBUG
	LONG lFeatureCount = 0;
	ATLTRACE("AheGetConnectedRoad() : �����������H�̐��F%d\n", cTargetItems.size() );
#endif

	return cTargetItems.size();
}
/////////////////////////////////////////////////////////////////////////////
//
// �����N��Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
LONG AheSelectInfoRowByShape( IFeatureClassPtr ipRoadClass, ITablePtr ipInfoTable, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, IGeometryPtr ipGeom, LONG lDrawMode, std::list<ROWDEF>& listInfoRows, std::list<ROWDEF>& listRoads, LONG lSequence/* = -1*/, bool bLastLink )
{
	if( ipRoadClass == NULL || ipInfoTable == NULL || ipLQTable == NULL || ipGeom == NULL ) return 0;

	// ���H�����N�t�B�[�`����I��
	IFeatureCursorPtr ipFeatureCursor = AheSelectByShape( ipGeom, ipRoadClass, esriSpatialRelIntersects );
	if( ipFeatureCursor == NULL ) return 0;

	// LQ �e�[�u�����������邽�߂̃N�G���t�B���^�쐬
	CString strWhereClause;
	IFeaturePtr ipFeature;
	LONG lCount = 0;
	while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
	{
		if( ipFeature == NULL ) continue;
		LONG lFlag = -1;

		CComVariant vaFlag = AheGetAttribute( ipFeature, lpcszRoadRelFieldName );
		if( !( vaFlag.vt == VT_I4 && vaFlag.lVal == 1 ) ) continue;

		LONG lOID = -1;
		IGeometryPtr ipGeom;

		ipFeature->get_OID( &lOID );
		ipFeature->get_ShapeCopy( &ipGeom );

		ROWDEF rowDef;
		rowDef.oid = lOID;
		rowDef.shp = ipGeom;
		rowDef.obj = ipFeature;
		rowDef.flg = lDrawMode;
		if( lSequence != -1 )
			rowDef.sequence = lSequence;

		listRoads.push_back( rowDef );
	}

	return AheSelectInfoRowByRoadIDs( listRoads, ipInfoTable, ipLQTable, lDrawMode, listInfoRows, bLastLink );
}

LONG AheSelectInfoRowByRoadIDs( const std::list<ROWDEF>& listRoads, ITablePtr ipInfoTable, ITablePtr ipLQTable, LONG lDrawMode, std::list<ROWDEF>& listInfoRows, bool bLastLink )
{
	std::list<LONG> listRelIDs;

	if( listRoads.size() < 1 ) return 0;

	if ( ! ipInfoTable || ! ipLQTable )	// [Bug 6551]�����N��̃e�[�u�������݂��Ȃ����Ƃ����e���邽��NULL�`�F�b�N����
		 return -1;

	// �n���ꂽ���H��ID�Ɋ֘A���� INFO �� ID ���擾
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_SubFields( CComBSTR(_T("*")) );
	for( std::list<ROWDEF>::const_iterator it = listRoads.begin(); it != listRoads.end(); it++ )
	{
		// �Ώۂ� LQ �ȊO�͔�΂�
		if( !( it->flg & lDrawMode ) ) continue;

		// �N�G���t�B���^���쐬���Č���
		CString strWhereClause;
		strWhereClause.Format(_T("%s = %d"), LINK_ID_FIELD, it->oid );
		if( it->direction != -1 )
		{
			CString strWhereClauseTmp;
			strWhereClauseTmp.Format(_T(" AND %s = %d"), LINKDIR_C_FIELD, it->direction );
			strWhereClause += strWhereClauseTmp;
		}
		if( it->sequence != -1 && bLastLink == false)
		{
			CString strWhereClauseTmp;
			strWhereClauseTmp.Format(_T(" AND %s = %d"), SEQUENCE_FIELD, it->sequence );
			strWhereClause += strWhereClauseTmp;
		}

		// [Bug3106]�Ή��ŁA���o�����N�����������N��������Ă���K�v�����邽��
		if( bLastLink == true )
		{
			CString strWhereClauseTmp;
			strWhereClauseTmp.Format(_T(" AND %s = %d"), LASTLINK_F_FIELD, 1 );
			strWhereClause += strWhereClauseTmp;
		}

		ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
		_ICursorPtr ipCursor;
		ipLQTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
		ATLASSERT( ipCursor != NULL );

		_IRowPtr ipRow;

		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			CComVariant vaValue = AheGetAttribute( ipRow, INF_ID_FIELD );
			ATLASSERT( vaValue.vt == VT_I4 );

			listRelIDs.push_back( vaValue.lVal );
		}
	}
	
	// ���j�[�N�ɂ���
	listRelIDs.sort();
	listRelIDs.unique();	

	CString strInfoTableName = AheGetFeatureClassName( ipInfoTable );

	// [Bug3565]���[���̏ꍇ�ɂ́AShift�����Ȃ���̏ꍇ�ɂ́A�I���ł��Ȃ��悤�ɂ��邽��
	if( !( bLastLink == true && strInfoTableName == _T("INF_LANE") ) )
	{
		// INFO �e�[�u��������
		for( std::list<LONG>::iterator itIDs = listRelIDs.begin(); itIDs != listRelIDs.end(); itIDs++ )
		{
			_IRowPtr ipRow;
			ipInfoTable->GetRow( *itIDs, &ipRow );

			ROWDEF rowDef;
			rowDef.oid = *itIDs;
			rowDef.obj = ipRow;
			rowDef.flg = lDrawMode;
			rowDef.name = strInfoTableName;
			listInfoRows.push_back( rowDef );
		}
	}
	return listInfoRows.size();
}

LONG AheSelectLQRowByInfoRows( const std::list<ROWDEF>& listInfoRows, ITablePtr ipLQTable, std::list<ROWDEF>& listRQRows )
{
	if( listInfoRows.size() < 1 || ipLQTable == NULL ) return 0;

	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_SubFields(CComBSTR(_T("*")));
	for( std::list<ROWDEF>::const_iterator itInfoRows = listInfoRows.begin(); itInfoRows != listInfoRows.end(); itInfoRows++ )
	{
		_IRowPtr ipRow( itInfoRows->obj );
		LONG lOID = itInfoRows->oid;

		if( ipRow == NULL ) continue;

		CString strLQTableName = AheGetFeatureClassName( ipLQTable );
		// �N�G���t�B���^�쐬
		CString strWhereClause;
		strWhereClause.Format(_T("%s = %d"), INF_ID_FIELD, lOID );
		_ICursorPtr ipCursor;
		ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );

		ipLQTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
		
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			ROWDEF rowDef;

			rowDef.obj = ipRow;
			rowDef.oid = AheGetOID( ipRow );
			rowDef.flg = itInfoRows->flg;
			rowDef.name = strLQTableName;
			rowDef.sequence = AheGetAttribute( ipRow, SEQUENCE_FIELD ).lVal;

			listRQRows.push_back( rowDef );
		}
	}

	return listRQRows.size();
}

LONG AheSelectLQRowByRoadIDs( const std::list<ROWDEF>& listRoads, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, LONG lDrawMode, std::list<ROWDEF>& listRQRows, std::list<LONG>& listInfoIDs )
{
	CString strWhereClause;

	if( NULL == ipLQTable ) return -1;
	if( listRoads.size() < 1 ) return 0;

	// �n���ꂽ���H��ID�Ɋ֘A���� INFO �� ID ���擾
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_SubFields( CComBSTR(_T("*")) );
	for( std::list<ROWDEF>::const_iterator it = listRoads.begin(); it != listRoads.end(); it++ )
	{
		// �Ώۂ� LQ �ȊO�͔�΂�
		if( !( it->flg & lDrawMode ) ) continue;

		// �N�G���t�B���^���쐬���Č���
		CString strWhereClause;
		strWhereClause.Format(_T("%s = %d"), LINK_ID_FIELD, it->oid );
		if( it->direction != -1 )
		{
			CString strWhereClauseTmp;
			strWhereClauseTmp.Format(_T(" AND %s = %d"), LINKDIR_C_FIELD, it->direction );
			strWhereClause += strWhereClauseTmp;
		}
		if( it->sequence != -1 )
		{
			CString strWhereClauseTmp;
			strWhereClauseTmp.Format(_T(" AND %s = %d"), SEQUENCE_FIELD, it->sequence );
			strWhereClause += strWhereClauseTmp;
		}
		ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );

		_ICursorPtr ipCursor;
		ipLQTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
		ATLASSERT( ipCursor != NULL );

		_IRowPtr ipRow;

		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			CComVariant vaValue = AheGetAttribute( ipRow, INF_ID_FIELD );
			ATLASSERT( vaValue.vt == VT_I4 );

			listInfoIDs.push_back( vaValue.lVal );
		}
	}
	
	// ���j�[�N�ɂ���
	listInfoIDs.sort();
	listInfoIDs.unique();	

	for( std::list<LONG>::iterator itInfoIDs = listInfoIDs.begin(); itInfoIDs != listInfoIDs.end(); itInfoIDs++ )
	{
		// �N�G���t�B���^�쐬
		strWhereClause.Format(_T("%s = %d"), INF_ID_FIELD, *itInfoIDs );
		_ICursorPtr ipCursor;
		ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
		// ����
		ipLQTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
		_IRowPtr ipRow;
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			ROWDEF rowDef;

			LONG lOID = -1;
			ipRow->get_OID( &lOID );
			rowDef.obj = ipRow;
			rowDef.oid = lOID;
			rowDef.flg = lDrawMode;
			rowDef.sequence = AheGetAttribute( ipRow, SEQUENCE_FIELD ).lVal;

			listRQRows.push_back( rowDef );
		}
	}

	return listRQRows.size();
}

LONG AheSelectLQRowByRoadIDs2( const std::list<ROWDEF>& listRoads, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, LONG lDrawMode, std::list<ROWDEF>& listRQRows, std::list<LONG>& listInfoIDs, CString& strWhereClause )
{
	if( listRoads.size() < 1 ) return 0;
	if ( ! ipLQTable )	// [Bug 6551]�����N��̃e�[�u�������݂��Ȃ����Ƃ����e���邽��NULL�`�F�b�N����
		 return -1;

	// �n���ꂽ���H��ID�Ɋ֘A���� INFO �� ID ���擾
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_SubFields( CComBSTR(INF_ID_FIELD) );

	// �N�G���t�B���^���쐬���Č���
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );

	_ICursorPtr ipCursor;
	ipLQTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	ATLASSERT( ipCursor != NULL );

	_IRowPtr ipRow;

	while( ipCursor->NextRow( &ipRow ) == S_OK ) {
		CComVariant vaValue = AheGetAttribute( ipRow, INF_ID_FIELD );
		ATLASSERT( vaValue.vt == VT_I4 );

		listInfoIDs.push_back( vaValue.lVal );
	}

	// ���j�[�N�ɂ���
	listInfoIDs.sort();
	listInfoIDs.unique();

	CString strWhereClause2, strTmp, strTmp2;

	for( std::list<LONG>::iterator itInfoIDs = listInfoIDs.begin(); itInfoIDs != listInfoIDs.end(); itInfoIDs++ ) {
		if ( strTmp.IsEmpty() ) {
			strTmp.Format(_T("%d"), *itInfoIDs);
		} else {
			strTmp2.Format(_T(",%d"), *itInfoIDs);
			strTmp += strTmp2;
		}
	}
	
	if ( strTmp.IsEmpty() ) {
		return listRQRows.size();
	}

	// �N�G���t�B���^�쐬
	ipQueryFilter->put_SubFields( CComBSTR(_T("OBJECTID,INF_ID,LINK_ID,LINKDIR_C,SEQUENCE")) );
	strWhereClause2.Format(_T("%s in (%s)"), INF_ID_FIELD, strTmp );
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause2) );

	// ����
	ipLQTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	while( ipCursor->NextRow( &ipRow ) == S_OK )
	{
		ROWDEF rowDef;

		LONG lOID = -1;
		ipRow->get_OID( &lOID );
		rowDef.obj = ipRow;
		rowDef.oid = lOID;

		listRQRows.push_back( rowDef );
	}

	return listRQRows.size();
}

LONG AheSelectLQRoad( IUnknownPtr ipUnk, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listRoads, std::list<ROWDEF>& listLQRoads, BOOL bComplete )
{
	std::list<LONG> listRoadID;
	CString strWhereClause;

	for( std::list<ROWDEF>::const_iterator itLQRows = listLQRows.begin(); itLQRows != listLQRows.end(); itLQRows++ )
	{
		_IRowPtr ipRow( itLQRows->obj );
		if( ipRow == NULL ) continue;

		CComVariant vaValue = AheGetAttribute( ipRow, LINK_ID_FIELD );
		ATLASSERT( vaValue.vt == VT_I4 );

		listRoadID.push_back( vaValue.lVal );
	}

	// ���j�[�N�ɂ���
	listRoadID.sort();
	listRoadID.unique();

	IFeatureClassPtr ipRoadClass = AheGetFeatureClass( ipUnk );
	for( std::list<LONG>::iterator it = listRoadID.begin(); it != listRoadID.end(); it++ )
	{
		// �n���ꂽ�Z�b�g�̒��ɂ���Ȃ炻����g�p�i�X�s�[�h�A�b�v�ׁ̈j
		BOOL bFind = FALSE;
		for( std::list<ROWDEF>::const_iterator itRowDef = listRoads.begin(); itRowDef != listRoads.end(); itRowDef++ )
		{
			if( *it == itRowDef->oid )
			{
				bFind = TRUE;
				ATLTRACE(_T("*"));

				listLQRoads.push_back( *itRowDef );
				break;
			}
		}
		if( bFind == TRUE || bComplete == FALSE ) continue;

		// �n���ꂽ�Z�b�g�̒��ɑ��݂��Ȃ������ꍇ�͂��傤���Ȃ��̂ň����Ă���
		IFeaturePtr ipFeature;
		IGeometryPtr ipCopyShape;
		ipRoadClass->GetFeature( *it, &ipFeature );
		if( ipFeature == NULL )
		{
			ATLASSERT( ipFeature != NULL );
			CString strMsg;
			strMsg.Format(ERROR_NOT_FOUND_ROADLINK_REL_LINKQUEUE, *it );
			AheMessageBox( strMsg, _T("AheSelectLQRoad"), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
			continue;
		}
		ipFeature->get_ShapeCopy( &ipCopyShape );

		ROWDEF rowDef;
		rowDef.oid = *it;
		rowDef.obj = ipFeature;
		rowDef.shp = ipCopyShape;
		listLQRoads.push_back( rowDef );
	}

	return listLQRoads.size();
}

IGeometryPtr AheGetLQRoadGeometry( LONG lInfoID, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listLQRoads, BOOL* bFixFlow )
{
	if( bFixFlow == NULL || lInfoID < 1 || listLQRows.size() < 1 || listLQRoads.size() < 1 ) return NULL;

	*bFixFlow = FALSE;

	IPolylinePtr ipPoly;
	IPointPtr ipStartPoint, ipEndPoint;
	ISpatialReferencePtr ipSpRef;

	// listLQRows�͌Ăяo�����ŃV�[�P���X�Ń\�[�g���Ă����I�I

	LONG lMinSequence = -1;	// Union �̌��ʂ���_��̌������C������̂ɕK�v
	for( std::list<ROWDEF>::const_iterator itLQRows = listLQRows.begin(); itLQRows != listLQRows.end(); itLQRows++ )
	{
		_IRowPtr ipRow( itLQRows->obj );

		if( ipRow == NULL ) continue;

		CComVariant vaValue = AheGetAttribute( ipRow, INF_ID_FIELD );
		ATLASSERT( vaValue.vt == VT_I4 );

		if( vaValue.lVal == lInfoID )
		{
			CComVariant vaRoadID = AheGetAttribute( ipRow, LINK_ID_FIELD );
			ATLASSERT( vaRoadID.vt == VT_I4 );

			IGeometryPtr ipGeom;
			for( std::list<ROWDEF>::const_iterator itLQRoads = listLQRoads.begin(); itLQRoads != listLQRoads.end(); itLQRoads++ )
			{
				if( vaRoadID.lVal == itLQRoads->oid )
				{
					IClonePtr ipClone;

					((IClonePtr)itLQRoads->shp)->Clone( &ipClone );
					ipGeom = (IGeometryPtr)ipClone;

					// ��ԎQ�Ƃ�ێ����Ă���
					if( ipSpRef == NULL ) itLQRoads->shp->get_SpatialReference( &ipSpRef );

					break;
				}
			}
			if( ipGeom == NULL ) continue;

			CComVariant vaValueDir = AheGetAttribute( ipRow, LINKDIR_C_FIELD );
			ATLASSERT( vaValueDir.vt == VT_I4 );

			if( vaValueDir.lVal == 2 )
				((IPolylinePtr)ipGeom)->ReverseOrientation();	// ������Ȃ��ꍇ������̂ŕK�v

			CComVariant vaValue2 = AheGetAttribute( ipRow, SEQUENCE_FIELD );
			ATLASSERT( vaValue2.vt == VT_I4 );

			// ��ԎႢSEQUENCE�̊J�n�E�I���_���L��
			if( lMinSequence < 0 || lMinSequence > vaValue2.lVal )
			{

				lMinSequence = vaValue2.lVal;
				((IPolylinePtr)ipGeom)->get_FromPoint( &ipStartPoint );
				((IPolylinePtr)ipGeom)->get_ToPoint( &ipEndPoint );
				// ��ԎQ�Ƃ���v������
				ipStartPoint->putref_SpatialReference( ipSpRef );
				ipEndPoint->putref_SpatialReference( ipSpRef );
				ipStartPoint->SnapToSpatialReference();
				ipEndPoint->SnapToSpatialReference();
			}

			if( ipPoly == NULL )
				ipPoly = ipGeom;
			else
			{
				IGeometryPtr ipRetGeom = AheUnionNetwork( ipPoly, ipGeom, ipSpRef );
				if( ipRetGeom != NULL )
					ipPoly = ipRetGeom;
				else
					ATLASSERT( FALSE );
//#ifdef DEBUG
////				LONG lUnionedPointCount;
////				((IPointCollectionPtr)ipPoly)->get_PointCount( &lUnionedPointCount );
////				ATLTRACE(_T("AheGetLQRoadGeometry() : ���j�I����̃|�C���g���F%d\n"), lUnionedPointCount );
//#endif 
			}
		}
	}

	// ���j�I���̌��ʂƋL�����Ă������X�^�[�g�|�C���g���r���āA�t�̏ꍇ�͓_����Ђ�����Ԃ�
	if( ipPoly != NULL )
	{
#ifdef DEBUG
		// �e�X�g
		LONG lCount = -1;
		((IGeometryCollectionPtr)ipPoly)->get_GeometryCount( &lCount );
		ATLTRACE(_T("�W�I���g���̐��F%d\n"), lCount );
		AheTraceGeometry( ipPoly, _T("AheGetLQRoadGeometry() : ") );
#endif

		IPointPtr ipFromPoint, ipToPoint;
		ipPoly->get_FromPoint( &ipFromPoint );
		ipPoly->get_ToPoint( &ipToPoint );
		// ��ԎQ�Ƃ���v������
		ipFromPoint->putref_SpatialReference( ipSpRef );
		ipToPoint->putref_SpatialReference( ipSpRef );
		ipFromPoint->SnapToSpatialReference();
		ipToPoint->SnapToSpatialReference();

		IRelationalOperatorPtr ipRelOpeStart( ipStartPoint );
		IRelationalOperatorPtr ipRelOpeEnd( ipEndPoint );

		if( ipRelOpeStart == NULL || ipRelOpeEnd == NULL ) return ipPoly;

		VARIANT_BOOL vaStartAndFromIsEqual = VARIANT_TRUE;
		VARIANT_BOOL vaEndAndFromIsEqual = VARIANT_TRUE;

		BOOL bOK1 = SUCCEEDED(ipRelOpeStart->Equals( ipFromPoint, &vaStartAndFromIsEqual ));
		BOOL bOK2 = SUCCEEDED(ipRelOpeEnd->Equals( ipFromPoint, &vaEndAndFromIsEqual ));
		if( ( bOK1 == TRUE && bOK2 == TRUE ) && ( vaStartAndFromIsEqual == VARIANT_FALSE && vaEndAndFromIsEqual == VARIANT_FALSE ) )
		{
			VARIANT_BOOL vaStartAndToIsEqual = VARIANT_FALSE;
			VARIANT_BOOL vaEndAndToIsEqual = VARIANT_FALSE;

			BOOL bOK3 = SUCCEEDED(ipRelOpeStart->Equals( ipToPoint, &vaStartAndToIsEqual ));
			BOOL bOK4 = SUCCEEDED(ipRelOpeEnd->Equals( ipToPoint, &vaEndAndToIsEqual ));
			if( ( bOK3 == TRUE && bOK4 == TRUE ) && ( vaStartAndToIsEqual == VARIANT_TRUE || vaEndAndToIsEqual == VARIANT_TRUE ) )
			{
				ipPoly->ReverseOrientation();
				*bFixFlow = TRUE;
			}
		}
		// ���]����K�v�Ȃ�
		else if( ( bOK1 == TRUE && bOK2 == TRUE ) && ( vaStartAndFromIsEqual == VARIANT_TRUE || vaEndAndFromIsEqual == VARIANT_TRUE ) )
		{
			*bFixFlow = TRUE;
		}
	}

	return ipPoly;
}

// �}���`�����_���p
IGeometryPtr AheGetLQRoadGeometry2( LONG lInfoID, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listLQRoads, BOOL* bFixFlow )
{
	if( bFixFlow == NULL || lInfoID < 1 || listLQRows.size() < 1 || listLQRoads.size() < 1 ) return NULL;

	*bFixFlow = FALSE;

	IPolylinePtr ipPoly;
	IPointPtr ipStartPoint, ipEndPoint;
	ISpatialReferencePtr ipSpRef;

	LONG lMinSequence = -1;	// Union �̌��ʂ���_��̌������C������̂ɕK�v
	for( std::list<ROWDEF>::const_iterator itLQRows = listLQRows.begin(); itLQRows != listLQRows.end(); itLQRows++ )
	{
		_IRowPtr ipRow( itLQRows->obj );

		if( ipRow == NULL ) continue;

		CComVariant vaValue = AheGetAttribute( ipRow, INF_ID_FIELD );
		ATLASSERT( vaValue.vt == VT_I4 );

		if( vaValue.lVal == lInfoID )
		{
			CComVariant vaRoadID = AheGetAttribute( ipRow, LINK_ID_FIELD );
			ATLASSERT( vaRoadID.vt == VT_I4 );

			IGeometryPtr ipGeom;
			for( std::list<ROWDEF>::const_iterator itLQRoads = listLQRoads.begin(); itLQRoads != listLQRoads.end(); itLQRoads++ )
			{
				if( vaRoadID.lVal == itLQRoads->oid )
				{
					IClonePtr ipClone;

					((IClonePtr)itLQRoads->shp)->Clone( &ipClone );
					ipGeom = (IGeometryPtr)ipClone;

					// ��ԎQ�Ƃ�ێ����Ă���
					if( ipSpRef == NULL ) itLQRoads->shp->get_SpatialReference( &ipSpRef );

					break;
				}
			}
			if( ipGeom == NULL ) continue;

			CComVariant vaValueDir = AheGetAttribute( ipRow, LINKDIR_C_FIELD );
			ATLASSERT( vaValueDir.vt == VT_I4 );

			if( vaValueDir.lVal == 2 )
				((IPolylinePtr)ipGeom)->ReverseOrientation();	// ������Ȃ��ꍇ������̂ŕK�v

			CComVariant vaValue2 = AheGetAttribute( ipRow, SEQUENCE_FIELD );
			ATLASSERT( vaValue2.vt == VT_I4 );

			// ��ԎႢSEQUENCE�̊J�n�E�I���_���L��
			if( lMinSequence < 0 || lMinSequence > vaValue2.lVal )
			{

				lMinSequence = vaValue2.lVal;
				((IPolylinePtr)ipGeom)->get_FromPoint( &ipStartPoint );
				((IPolylinePtr)ipGeom)->get_ToPoint( &ipEndPoint );
				// ��ԎQ�Ƃ���v������
				ipStartPoint->putref_SpatialReference( ipSpRef );
				ipEndPoint->putref_SpatialReference( ipSpRef );
				ipStartPoint->SnapToSpatialReference();
				ipEndPoint->SnapToSpatialReference();
			}

			if( ipPoly == NULL )
				ipPoly = ipGeom;
			else
			{
				// AheUnion()�AAheUnionNetwork()���g�킸�W�I���g���ǂ������Z�O�����g�Ƃ��Ă�������
				IGeometryPtr ipRetGeom = AheUnionSegments( ipPoly, ipGeom, ipSpRef );
				ipPoly = ipRetGeom;
//#ifdef DEBUG
////				LONG lUnionedPointCount;
////				((IPointCollectionPtr)ipPoly)->get_PointCount( &lUnionedPointCount );
////				ATLTRACE(_T("AheGetLQRoadGeometry() : ���j�I����̃|�C���g���F%d\n"), lUnionedPointCount );
//#endif 
			}
		}
	}

	// ���j�I���̌��ʂƋL�����Ă������X�^�[�g�|�C���g���r���āA�t�̏ꍇ�͓_����Ђ�����Ԃ�
	if( ipPoly != NULL )
	{
#ifdef DEBUG
		// �e�X�g
		LONG lCount = -1;
		((IGeometryCollectionPtr)ipPoly)->get_GeometryCount( &lCount );
		ATLTRACE(_T("�W�I���g���̐��F%d\n"), lCount );
		AheTraceGeometry( ipPoly, _T("AheGetLQRoadGeometry() : ") );
#endif

		IPointPtr ipFromPoint, ipToPoint;
		ipPoly->get_FromPoint( &ipFromPoint );
		ipPoly->get_ToPoint( &ipToPoint );
		// ��ԎQ�Ƃ���v������
		ipFromPoint->putref_SpatialReference( ipSpRef );
		ipToPoint->putref_SpatialReference( ipSpRef );
		ipFromPoint->SnapToSpatialReference();
		ipToPoint->SnapToSpatialReference();

		IRelationalOperatorPtr ipRelOpeStart( ipStartPoint );
		IRelationalOperatorPtr ipRelOpeEnd( ipEndPoint );

		if( ipRelOpeStart == NULL || ipRelOpeEnd == NULL ) return ipPoly;

		VARIANT_BOOL vaStartAndFromIsEqual = VARIANT_TRUE;
		VARIANT_BOOL vaEndAndFromIsEqual = VARIANT_TRUE;

		BOOL bOK1 = SUCCEEDED(ipRelOpeStart->Equals( ipFromPoint, &vaStartAndFromIsEqual ));
		BOOL bOK2 = SUCCEEDED(ipRelOpeEnd->Equals( ipFromPoint, &vaEndAndFromIsEqual ));
		if( ( bOK1 == TRUE && bOK2 == TRUE ) && ( vaStartAndFromIsEqual == VARIANT_FALSE && vaEndAndFromIsEqual == VARIANT_FALSE ) )
		{
			VARIANT_BOOL vaStartAndToIsEqual = VARIANT_FALSE;
			VARIANT_BOOL vaEndAndToIsEqual = VARIANT_FALSE;

			BOOL bOK3 = SUCCEEDED(ipRelOpeStart->Equals( ipToPoint, &vaStartAndToIsEqual ));
			BOOL bOK4 = SUCCEEDED(ipRelOpeEnd->Equals( ipToPoint, &vaEndAndToIsEqual ));
			if( ( bOK3 == TRUE && bOK4 == TRUE ) && ( vaStartAndToIsEqual == VARIANT_TRUE || vaEndAndToIsEqual == VARIANT_TRUE ) )
			{
				ipPoly->ReverseOrientation();
				*bFixFlow = TRUE;
			}
		}
		// ���]����K�v�Ȃ�
		else if( ( bOK1 == TRUE && bOK2 == TRUE ) && ( vaStartAndFromIsEqual == VARIANT_TRUE || vaEndAndFromIsEqual == VARIANT_TRUE ) )
		{
			*bFixFlow = TRUE;
		}
	}

	return ipPoly;
}
/////////////////////////////////////////////////////////////////////////////
//
// �H�H�Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
//!< �n���ꂽ�t�B�[�`���̍\���_��ɕʂ̃t�B�[�`�����������邩���ׂ�


BOOL AheGetClosestVertexPointOrPartIndex( 
		CTargetItems&		cTargetItems,
		IPointPtr			ipPoint,
		DOUBLE				dTorelance,
		IPoint**			ipRetPoint,
		ILayer**			ipRetLayer,
		IFeature**			ipRetFeature,
		LONG*				lPartIndex,
		LONG*				lVertexIndex,
		BOOL*				bIsHitVertex )
{
//	ILayerPtr ipLayer = NULL, ipClosestpointLayer = NULL;
	DOUBLE dHitDist = 0.0, dClosestDist = -1.0, dClosestpointDist = 0.0;
	IFeaturePtr ipHasClosestpointFeature = NULL, ipTmpFeature = NULL;
	LONG lClosestVertexIndex = 0, lClosestPartIndex = 0;
	IPointPtr ipClosestPoint = NULL;
	VARIANT_BOOL vbIsVisible = VARIANT_FALSE;

	dClosestpointDist = dTorelance + 1.0;	// ������
	// �Z���N�V�����ɓ����Ă���t�B�[�`���ɑ΂��ăq�b�g�e�X�g
	for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++)
	{
		IFeaturePtr ipFeature = it->GetFeature();

/*		// ���C�����\������Ă��邩�ǂ����`�F�b�N
		ipLayer->get_Visible( &vbIsVisible );
		if( vbIsVisible == VARIANT_FALSE ) {
			ATLTRACE("AheGetClosestVertexPointOrPartIndex()�F��\�����C���Ȃ̂Ŕ�΂��܂�");
			continue;
		}*/
		// �q�b�g�e�X�g
		if( esriGeometryPartNone != AheTestFeatureGeometryHit( ipFeature, ipPoint, dTorelance, ipRetPoint, &dHitDist, lPartIndex, lVertexIndex))
		{
			// ��������ԋ߂����̂ōX�V
			if( dClosestpointDist > dHitDist )
			{
				ipClosestPoint = (*ipRetPoint);
				dClosestpointDist = dHitDist;
				ipHasClosestpointFeature = ipFeature;
				lClosestVertexIndex = *lVertexIndex;
				lClosestPartIndex = *lPartIndex;
//				ipClosestpointLayer = ipLayer;
			}
		}
	}

	if( dClosestpointDist > dTorelance ) {	// ������Ȃ�����
		(*ipRetPoint)   = NULL;
		(*ipRetFeature) = NULL;
		(*lPartIndex)   = 0;
		(*lVertexIndex) = 0;
		(*bIsHitVertex) = FALSE;
		return FALSE;
	} else {
		(*ipRetPoint)   = ipClosestPoint;
		(*ipRetFeature) = ipHasClosestpointFeature;
//		(*ipRetLayer)	= ipClosestpointLayer;
		(*lVertexIndex) = lClosestVertexIndex;
		(*lPartIndex)   = lClosestPartIndex;
		(*ipRetPoint)->AddRef();
		(*ipRetFeature)->AddRef();
//		(*ipRetLayer)->AddRef();
	}
		return TRUE;
}

BOOL AheGetNearestVertexInSelection( CTargetItems& cTargetItems, IPointPtr ipPoint, double dTorelance, IPoint** ipRetPoint)
{
	IFeaturePtr ipRetFeature;
	ILayerPtr	ipLayer;
	LONG        lPartIndex, lVertexIndex;
	BOOL		bIsHitVertex;

	// ���ݑI������Ă���t�B�[�`���̐�
	if( cTargetItems.size() < 1 ) return FALSE;
	// ��ԋ߂����_��T��
	if( AheGetClosestVertexPointOrPartIndex( cTargetItems, ipPoint, dTorelance, ipRetPoint, &ipLayer, 
		&ipRetFeature, &lPartIndex, &lVertexIndex, &bIsHitVertex ) == TRUE ) {
		// ���_�Ƀq�b�g���Ȃ�����
		if( bIsHitVertex == FALSE ) {
			return FALSE;
		}
		// ���_�Ƀq�b�g����
		else {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL AheIsSafeFeature( IFeaturePtr ipFeature, CErrObjContainer& errs, BOOL bCheckRoad/* = TRUE*/, BOOL bIsCheckAngle/* = TRUE*/, ISiNDYRule3Ptr ipRule/* = NULL*/ )
{
	BOOL bGeometryCheck = FALSE;
	LONG lOID = -1;
	CString strClassName = AheGetFeatureClassName( ipFeature );
	CString strOwnerName = AheGetFeatureClassOwnerName( ipFeature );

	if( ipRule == NULL )
		ipRule.CreateInstance( CLSID_SiNDYRuleObj );

	ipFeature->get_OID( &lOID );
	switch( AheGetFeatureKinds( ipFeature ) )
	{
	case sindyeRoadFeature:
		bGeometryCheck = TRUE;
		if( bCheckRoad )
			AheIsSafeRoadForms( ipFeature, errs );
		break;
	case sindyeLineFeature:
	case sindyePolygonFeature:
	case sindyeNodeFeature:
	case sindyePointFeature:	// �|�C���g���`��`�F�b�N�ɒǉ����Ȃ��ƃ��b�N�G���A�`�F�b�N�ł��Ȃ��ibug 4932�j
	{
		bGeometryCheck = TRUE;
		break;
	}
	default:
		break;
	}

	// �`��`�F�b�N
	if( bGeometryCheck )
	{
		// �W�I���g���̎擾
		IGeometryPtr ipGeometry( AheGetShape( ipFeature ) );
		if( ! ipGeometry )
			errs.add( strOwnerName + _T(".") + strClassName, lOID, ipGeometry, -1, MakeWKSPoint( -1.0, -1.0 ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_NO_NETWORK_GEOMETRY );
		else
			AheIsSafeGeometry( ipGeometry, strOwnerName, strClassName, lOID, errs, bIsCheckAngle, ipRule, AheShapeIsChanged( ipFeature ) ); 
	}

	return ( sindyeLogLevel_WARN <= errs.GetMaxLogLevel() ) ? FALSE : TRUE;
}

BOOL AheIsSafeRoadForms( IFeaturePtr ipRoadFeature, CErrObjContainer& errs )
{
	if( ! ipRoadFeature )
		return FALSE;

	// �ڑ��m�[�h���m�F�E�擾����
	CTargetItems cNodeItems;
	long lFindNodeNum = AheGetConnectedNode( ipRoadFeature, cNodeItems );
	if ( lFindNodeNum > 0 ) 
	{
		IFeaturePtr ipFeatureNode1, ipFeatureNode2;
		for( CTargetItems::const_iterator it = cNodeItems.begin(); it != cNodeItems.end(); ++it )
		{
			if( ! ipFeatureNode1 )
				ipFeatureNode1 = it->GetFeature();
			else
				ipFeatureNode2 = it->GetFeature();
		}

		// �ڑ����Ă���m�[�h��2�ł͂Ȃ�������G���[
		if( ! ( ipFeatureNode1 && ipFeatureNode2 ) )
			errs.add( AheGetOwnerDotClassName( ipRoadFeature ), AheGetOID( ipRoadFeature ), AheGetShapeCopy( ipRoadFeature ), -1, MakeWKSPoint ( AheGetFromPoint( AheGetShape( ipRoadFeature ) ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_NOT_JUST_2_NODES_ARE_CONNECTED );
		// ���[�v�`�F�b�N
		if ( ! AheIsSafeRoadLoop( ipFeatureNode1, ipFeatureNode2, ipRoadFeature, errs ) )
			errs.add( AheGetOwnerDotClassName( ipRoadFeature ), AheGetOID( ipRoadFeature ), AheGetShapeCopy( ipRoadFeature ), -1, MakeWKSPoint ( AheGetFromPoint( AheGetShape( ipRoadFeature ) ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_NETWORK_LINK_LOOP );
		// ���[�v�`�F�b�N
		if ( ! AheIsSafeRoadLoop( ipFeatureNode2, ipFeatureNode1, ipRoadFeature, errs ) )
			errs.add( AheGetOwnerDotClassName( ipRoadFeature ), AheGetOID( ipRoadFeature ), AheGetShapeCopy( ipRoadFeature ), -1, MakeWKSPoint ( AheGetFromPoint( AheGetShape( ipRoadFeature ) ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_NETWORK_LINK_LOOP );
	}

	return ( sindyeLogLevel_WARN <= errs.GetMaxLogLevel() ) ? FALSE : TRUE;
}

// �����m�[�h�𗼒[�ɂ������N���������݃`�F�b�N
BOOL AheIsSafeRoadLoop( IFeaturePtr ipFeatureNode1, IFeaturePtr ipFeatureNode2, IFeaturePtr ipRoadFeature, CErrObjContainer& errs )
{
	if( ! ( ipFeatureNode1 && ipFeatureNode2 && ipRoadFeature ) )
		return FALSE;

	// �ڑ������N���m�F�E�擾����
	CTargetItems cRoadItems;
	AheGetConnectedRoad( ipFeatureNode1, cRoadItems );
	for( CTargetItems::const_iterator it = cRoadItems.begin(); it != cRoadItems.end(); ++it )
	{
		IFeaturePtr ipRoadFeatureTmp( it->GetFeature() );

		if ( ipRoadFeature == ipRoadFeatureTmp )
			continue;

		// �ڑ��m�[�h���m�F�E�擾����
		CTargetItems cNodeItems2;
		AheGetConnectedNode( ipRoadFeatureTmp, cNodeItems2 );
		for( CTargetItems::const_iterator it2 = cNodeItems2.begin(); it2 != cNodeItems2.end(); ++it2 )
		{
			IFeaturePtr ipFeatureNodeTmp( it2->GetFeature() );

			if ( ipFeatureNode1 == ipFeatureNodeTmp )
				continue;
			else if ( ipFeatureNode2 == ipFeatureNodeTmp )
				errs.add( AheGetOwnerDotClassName( ipRoadFeature ), AheGetOID( ipRoadFeature ), AheGetShapeCopy( ipRoadFeature ), -1, 
				MakeWKSPoint( AheGetFromPoint( AheGetShape( ipRoadFeature ) ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_MORE_THAN_1_LINK_HAVE_SAME_NODE );
		}
	}
	return TRUE;
}

// ����Ȍ`�󂩂ǂ����`�F�b�N
BOOL AheIsSafeGeometry( IGeometryPtr ipGeom, LPCTSTR lpcszOwnerName, LPCTSTR lpcszClassName, LONG lOID, sindyeGeometryCheck* check, CString& strErrMessage, BOOL bIsCheckAngle/* = TRUE*/, ISiNDYRule3Ptr ipRule/* = NULL*/, BOOL bDoSimplify/* = TRUE*/, BOOL bCheckCross /* = FALSE*/ )
{
	if( ! check )
		return FALSE;

	*check = sindyeGeometryOK;

	CErrObjContainer errs;
	BOOL bRet = AheIsSafeGeometry( ipGeom, lpcszOwnerName, lpcszClassName, lOID, errs, bIsCheckAngle, ipRule, bDoSimplify, bCheckCross );
	if( ! bRet )
	{
		*check = ( sindyeLogLevel_ERR == errs.begin()->begin()->m_emLevel ) ? sindyeGeometryError : sindyeGeometryWarnning;
		strErrMessage = errs.begin()->begin()->m_strMsg;
	}
	return bRet;
}

BOOL AheIsSafeGeometry( IGeometryPtr ipGeom, LPCTSTR lpcszOwnerName, LPCTSTR lpcszClassName, LONG lOID, CErrObjContainer& errs, BOOL bIsCheckAngle/* = TRUE*/, ISiNDYRule3Ptr ipRule/* = NULL*/, BOOL bDoSimplify/* = TRUE*/, BOOL bCheckCross/* = FALSE*/ )
{
	if( ! ipGeom )
		return FALSE;

	if( ! lpcszClassName )
		return FALSE;

	CString strOwnerTableName; // ���O�o�͗p�e�[�u����
	if( lpcszOwnerName )
		strOwnerTableName.Format(_T("%s.%s"), lpcszOwnerName, lpcszClassName );
	else
		strOwnerTableName = lpcszClassName;

	ISpatialReferencePtr ipSpRel;
	ipGeom->get_SpatialReference( &ipSpRel );

	IGeometryPtr ipGeomCopy( AheCreateClone( ipGeom ) ); // �R�s�[�쐬

	// PGDBEditMode�Ȃ�G���A�`�F�b�N
	if( AheIsPGDBEditMode() )
	{
		if( ! ipRule )
			ipRule.CreateInstance( CLSID_SiNDYRuleObj );
		IGeometryPtr ipGlobalWorkArea;
		ipRule->GetGlobalWorkArea( &ipGlobalWorkArea );
		IGeometryPtr ipGlobalWorkAreaCopy;
		if( ipGlobalWorkArea )
			ipGlobalWorkAreaCopy = AheCreateClone( ipGlobalWorkArea );

		// AheIs**��SDE�ڑ�������Ƌ�ԎQ�Ƃ����킹�Ȃ��d�l�Ȃ̂ŁA������
		// �����Ƃ��킹��ibug 5463�j
		AheSnapToSpatialReference( ipGlobalWorkAreaCopy, ipSpRel );

		// [Bug 8164][e]SiNDY-c�g�p����PGDB�ҏW�Ń��b�V�����E��̃m�[�h���ړ��ł��Ȃ�
		// ���b�V�����E��̃|�C���g�́AContain���ᔻ��ł��Ȃ��̂ŁADisjoint��
		esriGeometryType type = esriGeometryAny;
		ipGeomCopy->get_GeometryType( &type );
		if( type == esriGeometryPoint )
		{
			if( AheIsDisjoint( ipGlobalWorkAreaCopy, ipGeomCopy ) )
				errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_IS_NOT_EDITABLE_AREA );
		}
		else
		{
			if( ! ( AheIsContain( ipGlobalWorkAreaCopy, ipGeomCopy ) ||
				    AheIsWithin( ipGeomCopy, ipGlobalWorkAreaCopy ) ) )
				errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_IS_NOT_EDITABLE_AREA );
		}
	}

	// ���b�N���b�V�����ł͂Ȃ����ǂ����̃`�F�b�N
	if( AheIsLockArea( lpcszOwnerName, lpcszClassName, ipGeomCopy ) )
		errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_IS_LOCKMESH );

	// �W�I���g���`�F�b�N�Ώۂ��ǂ����̃`�F�b�N
	esriGeometryType geomType = esriGeometryAny;
	ipGeomCopy->get_GeometryType( &geomType );
	switch( geomType )
	{
		case esriGeometryPoint:
			if( AheIsNetworkNode( lpcszClassName ) )
			{
				// �m�[�h��2�����b�V���|���S����4���ɂ����Ă͂Ȃ�Ȃ�
				int ix = 0, iy = 0;
				double dx = 0, dy = 0;

				((IPointPtr)ipGeomCopy)->QueryCoords( &dx, &dy );
				unsigned int iMeshCode = AheLL2MESH( dx, dy, 2, ix, iy, 1 ); // bug 8919
				if( ix == 0 && iy == 0 )
					errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_EXIST_NODE_IN_MESH_CORNER );

			}
			break;
		case esriGeometryPolyline:
			if( AheIsNetworkLink( lpcszClassName ) )
			{
				// ���H���[�v�`�F�b�N�E�|�����C�����Ȍ����`�F�b�N
				// ���b�V�����܂�Ԃ��`�F�b�N�iCTargetItems::IsSafeGeometries������z���j

				// ���[�v�`�F�b�N
				VARIANT_BOOL vaIsClosed = VARIANT_FALSE;
				((IPolylinePtr)ipGeomCopy)->get_IsClosed( &vaIsClosed );
				if( vaIsClosed )
					errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetFromPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_LINK_LOOP );

				// ���Ȍ����`�F�b�N
				if( AheIsOneselfCross( ipGeomCopy ) )
					errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_LINK_SELF_INTERSECT); // TODO: �ڐG�_���o���ɂ�AheIsOneselfCross���C������K�v����
			}
			else
			{
				// ���H�n�ȊO
				// �|�����C���̃N���X�`�F�b�N
				if( bCheckCross )
				{
					if( AheIsOneselfCross( ipGeomCopy, FALSE ) )
						errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, ERROR_POLYGON_SELP_INTERSECT );
				}
			}
			break;
		case esriGeometryPolygon:
		{
			// Simplify��̌`��ŐF�X���f����
			IPolygonPtr ipPolygon( AheCreateClone( ipGeomCopy ) );
			// [bug 1894 �֘A] �����ŗ�����̂ŁASimplify() �� try & catch
			try {
				((ITopologicalOperatorPtr)ipPolygon)->Simplify();
			} 
			catch( ... )
			{
				// SimplifyPreserveFromTo�Ȃ�ǂ���H
				ipPolygon->SimplifyPreserveFromTo();
			}
				
			// �\���_�����ω����Ă��邩�ǂ����`�F�b�N
			LONG lOriginalPointCount = -1, lSymplifyPointCount = -1;
			((IPointCollectionPtr)ipGeomCopy)->get_PointCount( &lOriginalPointCount );
			((IPointCollectionPtr)ipPolygon)->get_PointCount( &lSymplifyPointCount );

			// �\���_�� 0 �̃`�F�b�N
			if( lSymplifyPointCount < 1 )
				errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_NON_EXIST_SHAPE );
			// �˂���E�d���E�����Z�O�����g�`�F�b�N
			if( lOriginalPointCount != lSymplifyPointCount )
				errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_EIGHT_POLYGON );

			// Exterior Ring �͈�ł��邱�Ƃ���Ώ����i�|���S���j
			LONG lExteriorRingCount = -1;
			ipPolygon->get_ExteriorRingCount( &lExteriorRingCount );
			if( lExteriorRingCount > 1 )
				errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_OUTSIDE_POLYGON );

			break;
		}
		default:
			// ���̃W�I���g���^�C�v��f�ʂ肳���Ȃ�
			errs.add( strOwnerTableName, lOID, ipGeomCopy, -1, MakeWKSPoint( AheGetAreaPoint( ipGeomCopy ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_IS_NOT_CHECK_TARGET );
			return FALSE;
	}

	// �}���`�p�[�g�i�������|���S���j��z�肵�āA�W�I���g�����e�p�[�g�ɕ������Ĕ���
	IPointPtr ipTmpFromPoint( CLSID_Point ), ipTmpToPoint( CLSID_Point );	// ���H�����N��2�����b�V�����d���`�F�b�N�Ɏg�p
	ipTmpFromPoint->putref_SpatialReference( ipSpRel ); // ��ԎQ�ƕt�^
	ipTmpToPoint->putref_SpatialReference( ipSpRel ); // ��ԎQ�ƕt�^
	IGeometryCollectionPtr ipGeomCol( ipGeomCopy );
	esriGeometryDimension emDim = esriGeometry0Dimension;
	ipGeomCopy->get_Dimension( &emDim );
	if( ipGeomCol )
	{
		LONG lGeometryCount = -1;
		ipGeomCol->get_GeometryCount( &lGeometryCount );
		for( LONG i = 0; i < lGeometryCount; ++i )
		{
			IGeometryPtr ipPartGeom;
			ipGeomCol->get_Geometry( i, &ipPartGeom );

			if( ! ipPartGeom )
				continue;

			// �p�[�g�𐳎��ȁH�`��ɕϊ�
			IGeometryPtr ipPartGeomTmp( Convert( ipPartGeom, emDim ) );
			esriGeometryType geometryType = esriGeometryAny;
			ipPartGeomTmp->get_GeometryType( &geometryType );

			// �˂���`�F�b�N
			VARIANT_BOOL vaIsSimple = VARIANT_TRUE;
			ITopologicalOperatorPtr ipTopo( ipPartGeomTmp );
			if( ! ipTopo )
			{
				_ASSERTE( ipTopo );
				continue;
			}
			// [bug 2228] ������̂ŉ��
			try {
				if( geometryType == esriGeometryPolygon )
				{
					ipTopo->get_IsSimple( &vaIsSimple );

					VARIANT_BOOL vbIsEx = VARIANT_FALSE;
					((IRingPtr)ipPartGeom)->get_IsExterior( &vbIsEx );
					if( ! vbIsEx )
					{
						ITopologicalOperator3Ptr ipTopo3( ipPartGeomTmp );
						VARIANT_BOOL vaIsSimpleEx = VARIANT_FALSE;
						esriNonSimpleReasonEnum esriReason = esriNonSimpleOK;
	
						ipTopo3->get_IsSimpleEx( &esriReason, &vaIsSimpleEx );
						if ( esriReason == esriNonSimpleRingOrientation && ( ! vaIsSimpleEx ) )
							vaIsSimple = VARIANT_TRUE;
					}
				}
			}
			catch(...) {
				errs.add( strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint( AheGetAreaPoint( ipPartGeomTmp ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_IS_SILVER_POLYGON );
			}
			if( ! vaIsSimple )
				errs.add( strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint( AheGetAreaPoint( ipPartGeomTmp ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_TWIST_POLYGON );

			IPointCollectionPtr	ipPointCol( ipPartGeomTmp );
			if( ! ipPointCol )
				return TRUE;				// Point �̏ꍇ�� NULL �������悤��

			LONG lPointCount = -1;
			ipPointCol->get_PointCount( &lPointCount );

			// �\���_���̃`�F�b�N
			switch( geometryType )
			{
			case esriGeometryPolyline:
			case esriGeometryPath:
				if( lPointCount < 2 )
					errs.add( strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint( AheGetAreaPoint( ipPartGeomTmp ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_POLYLINE_COMPOSITION_POINT );
				break;
			case esriGeometryPolygon:
			case esriGeometryRing:
				if( lPointCount < 3 )
					errs.add( strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint( AheGetAreaPoint( ipPartGeomTmp ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_POLYGON_COMPOSITIOIN_POINT );
				break;
			default:
				break;
			}

			// ���C���n�Z�O�����g�`�F�b�N
			// �����`�F�b�N�͑S�Ẵ��C���n�ōs���ibug 1292�j
			switch( geometryType )
			{
				case esriGeometryPath:
				case esriGeometryPolyline:
				{
					ISegmentCollectionPtr ipSegCol( ipPartGeomTmp );
					if( ipSegCol != NULL )
					{
						LONG lSegCount = -1;
						ipSegCol->get_SegmentCount( &lSegCount );
						for( LONG j = 0; j < lSegCount; ++j )
						{
							_ISegmentPtr ipSeg;
							ipSegCol->get_Segment( j, &ipSeg );
							ITopologicalSegmentPtr ipTopoSeg( ipSeg );
							if( ipTopoSeg )
							{
								ipTopoSeg->QueryFromPoint( ipTmpFromPoint );
								ipTopoSeg->QueryToPoint( ipTmpToPoint );

								DOUBLE dFromX = 0.0, dFromY = 0.0, dToX = 0.0, dToY = 0.0;
								ipTmpFromPoint->QueryCoords( &dFromX, &dFromY );
								ipTmpToPoint->QueryCoords( &dToX, &dToY );

								// �����`�F�b�N��ADAM�����N�ȊO�̑S�Ẵ��C���n�ōs���ibug 1292�j
								double dLenMeter = AheGETDIST(dFromX, dFromY, dToX, dToY);
								if (lstrcmpi(adam_v2::lane_link::kTableName, lpcszClassName) != 0 &&
									lstrcmpi(adam_v2::updown_link::kTableName, lpcszClassName) != 0 &&
									lstrcmpi(adam_v2::compart_link::kTableName, lpcszClassName) != 0 &&
									lstrcmpi(adam_v2::border_link::kTableName, lpcszClassName) != 0
									)
								{
									if (0.2 > dLenMeter)
										errs.add(strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint(dFromX + (dToX - dFromX) / 2.0, dFromY + (dToY - dFromY) / 2.0), -1, sindyeLogLevel_ERR, ERROR_SILVER_SEGMENT, dLenMeter * 100.0);
								}

								// ���H�����N��
								// �E���b�V�����E�Z�O�����g���݃`�F�b�N�ibug 1614�j
								// �E�m�[�h�ڑ��Z�O�����g�̊p�x�������`�F�b�N�ibug 1870�j
								// ���K�v�B���s�҃l�b�g���[�N�͂���Ȃ��ibug 5005�A5280�A5281�j
								if( 0 == lstrcmpi( ROAD_TABLE_NAME, lpcszClassName ) )
								{
									// [bug 1614] ���H�����N�̏ꍇ��2�����b�V�����E���C���ɃZ�O�����g�����݂��Ă̓_��
									if( dFromX == dToX || dFromY == dToY )
									{
										CMesh mesh;
										std::list<UINT> listMesh;
										if( mesh.IsTouches( ipTmpFromPoint, listMesh ) && mesh.IsTouches( ipTmpToPoint, listMesh ) )
											errs.add( strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint( dFromX, dFromY ), -1, sindyeLogLevel_ERR, ERROR_SEGMENT_EXIST_MESH );
									}
									// [CHECK] [ROADCHECK 02197] �ڑ����Ă���m�[�h�i�m�[�hID:'node_id'�j�Ƃ̐ڑ��`�󂪕s��
									// [bug 1870] �m�[�h�ɐڑ����镔���̊p�x���ȉ��̏����̏ꍇ
									//            �m�[�h�ɐڑ�����x�N�g���̒������k�A�m�[�h�ɐڑ�����x�N�g����
									//				���̃x�N�g���Ԃ̊p�x���q�Ƃ����Ƃ��A
									//				 1. �k�����K�����W���� 50�����ŁA�q��170�x�����B
									//				 2. �k�����K�����W���� 50�ȏ�@100�����ŁA�q��150�x�����B
									//				 3. �k�����K�����W���� 100�ȏ� 250�����ŁA�q��120�x�����B
									//				 4. �k�����K�����W���� 250�ȏ� 500�����ŁA�q�� 90�x�����B
									//				 5. �k�����K�����W���� 500�ȏ� 1000�����ŁA�q�� 40�x�����B
									//				 6. �k�����K�����W���� 1000�ȏ�ŁA�q�� 20�x�����B
									if( lSegCount > 1 && ( j == 0 || j == lSegCount - 1 ) )
									{
										CString strTmp;
										IPointPtr ip1, ip2, ip3;
										if( j == 0 )
										{
											_ISegmentPtr ipSegNext;
											ipSegCol->get_Segment( 1, &ipSegNext );
											ip1 = ipTmpFromPoint;
											ip2 = ipTmpToPoint;
											ipSegNext->get_ToPoint( &ip3 );
											strTmp.Format(START_VECTOR, (DOUBLE)dLenMeter * 100.0 );
										}
										else {
											_ISegmentPtr ipSegPrev;
											ipSegCol->get_Segment( lSegCount - 2, &ipSegPrev );
											ipSegPrev->get_FromPoint( &ip1 );
											ip2 = ipTmpFromPoint;
											ip3 = ipTmpToPoint;
											strTmp.Format(END_VECTOR, (DOUBLE)dLenMeter * 100.0 );
										}
										DOUBLE dAngle = AheGetAngle( ip1, ip2, ip3 );
										double dMeshXYLen = AheGetMeshXYLength( ipTmpFromPoint, ipTmpToPoint ); // MESH XY�����Ŕ�r�ibug 8463�j
										CString strMsg;
										if( dMeshXYLen < 50.0                                && ! ( -170.0 >= dAngle || 170.0 <= dAngle ) )strMsg.Format(ERROR_ANGLE_MORE_THAN_170, strTmp, fabs(dAngle) );
										else if(   50.0 <= dMeshXYLen && dMeshXYLen <  100.0 && ! ( -150.0 >= dAngle || 150.0 <= dAngle ) ) strMsg.Format(ERROR_ANGLE_MORE_THAN_150, strTmp, fabs(dAngle) );
										else if(  100.0 <= dMeshXYLen && dMeshXYLen <  250.0 && ! ( -120.0 >= dAngle || 120.0 <= dAngle ) ) strMsg.Format(ERROR_ANGLE_MORE_THAN_120, strTmp, fabs(dAngle) );
										else if(  250.0 <= dMeshXYLen && dMeshXYLen <  500.0 && ! (  -90.0 >= dAngle ||  90.0 <= dAngle ) ) strMsg.Format(ERROR_ANGLE_MORE_THAN_90, strTmp, fabs(dAngle) );
										else if(  500.0 <= dMeshXYLen && dMeshXYLen < 1000.0 && ! (  -40.0 >= dAngle ||  40.0 <= dAngle ) ) strMsg.Format(ERROR_ANGLE_MORE_THAN_40, strTmp, fabs(dAngle) );
										else if( 1000.0 <= dMeshXYLen                        && ! (  -20.0 >= dAngle ||  20.0 <= dAngle ) ) strMsg.Format(ERROR_ANGLE_MORE_THAN_20, strTmp, fabs(dAngle) );
										if( ! strMsg.IsEmpty() )
											errs.add( strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint( ip2 ), -1, sindyeLogLevel_WARN, _T("%s"), strMsg );
									}
								}
							}
						}
					}
					break;
				}
				// �p���X/���/�s�p�^�[���`�F�b�N
				case esriGeometryPolygon:
					// �|���S���̊J�n�E�I���_�̂Ƃ�����`�F�b�N�����邽�ߊJ�n�_�̎��i�J�n�E�I���_�͓���j�Ƃ��̎��̓_���Ō�ɒǉ����Ă���
					for( long j = 1; j < 3; ++j )
					{
						IPointPtr ipPoint;
						ipPointCol->get_Point( j, &ipPoint );
						ipPointCol->AddPoint( (IPointPtr)AheCreateClone( ipPoint ) );	
					}
					ipPointCol->get_PointCount( &lPointCount );
					break;
				default:
					break;
			}

			if ( bIsCheckAngle )
			{
				INT nCheckAngle = 20;

				// ����̃��C���͊p�x�𒲐�
				if(      lstrcmpi( lpcszClassName, CITYSITE_TABLE_NAME                      ) == 0 ) nCheckAngle = 3;
				else if( lstrcmpi( lpcszClassName, sindy::schema::city_road::kTableName     ) == 0 ) nCheckAngle = 3;
				else if( lstrcmpi( lpcszClassName, sindy::schema::city_tunnel::kTableName   ) == 0 ) nCheckAngle = 3;
				else if( lstrcmpi( lpcszClassName, ADMINPOLY_TABLE_NAME                     ) == 0 ) nCheckAngle = 10;
				else if( lstrcmpi( lpcszClassName, CITYSTATION_TABLE_NAME                   ) == 0 ) nCheckAngle = 20;
				else if( lstrcmpi( lpcszClassName, BUILDING_TABLE_NAME						) == 0 ) nCheckAngle = 5;	// �Â��ق�
				else if( lstrcmpi( lpcszClassName, BASESITE_TABLE_NAME						) == 0 ) nCheckAngle = 10;
				else if( lstrcmpi( lpcszClassName, BASESTATION_TABLE_NAME					) == 0 ) nCheckAngle = 20;
				else if( lstrcmpi( lpcszClassName, BASE_CONTOUR_TABLE_NAME					) == 0 ) nCheckAngle = 20;
				else																				 nCheckAngle = 20;

				for( LONG j = 0; j < lPointCount -2; j++ )
				{
					IPointPtr ipPointFirst, ipPointMiddle, ipPointEnd;
					DOUBLE dwAngle = 0.0;

		 			ipPointCol->get_Point( j, &ipPointFirst );
					ipPointCol->get_Point( j + 1, &ipPointMiddle );
					ipPointCol->get_Point( j + 2, &ipPointEnd );
					dwAngle = AheGetAngle( ipPointFirst, ipPointMiddle, ipPointEnd );
					if( dwAngle < 0 )
						dwAngle = -dwAngle;	// ���΂̓}�C�i�X�ŕԂ��Ă���̂�

					// �p���X/�s�p�^�[��
					if ( dwAngle <= nCheckAngle )
						errs.add( strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint( ipPointMiddle ), -1, sindyeLogLevel_WARN, ERROR_SHARP_TURN, nCheckAngle ); // bug 8463 #11�Ōx���ɕύX
					// ��ȃ`�F�b�N�͓S�������N�ȊO�̃|�����C���̂�
					if( geometryType == esriGeometryPolyline && lstrcmpi( sj::railway_link::kTableName, lpcszClassName ) != 0)
					{
						if ( j + 3 < lPointCount ) 
						{
							IPointPtr ipPointTmp;

							ipPointCol->get_Point( j + 3, &ipPointTmp );

							if ( AheGetAngle( ipPointMiddle, ipPointEnd, ipPointTmp ) <= 30 && dwAngle <= 30 )
								errs.add( strOwnerTableName, lOID, ipPartGeomTmp, -1, MakeWKSPoint( ipPointEnd ), -1, sindyeLogLevel_ERR, ERROR_LIGHTNING, 30 );
						}
					}
				}
			}
		}
	}


	// �����܂ŗ��Ă��d���_�͏����Ȃ��݂����Ȃ̂� Simplify() ������
	if( bDoSimplify )
		AheForceSimplify( ipGeom );

	return ( sindyeLogLevel_WARN <= errs.GetMaxLogLevel() ) ? FALSE : TRUE;
}

// ���H�t�B�[�`������ڑ�����Ă���m�[�h�t�B�[�`�����擾
LONG AheGetConnectedNode( IFeaturePtr ipFeature, CTargetItems& cTargetItems)
{
	// �����`�F�b�N
	if( ipFeature == NULL )
	{
		_ASSERTE( ipFeature != NULL );
		return -1;
	}

	ISpatialFilterPtr ipFilter;

	if( SUCCEEDED( ipFilter.CreateInstance( CLSID_SpatialFilter ) ) )
	{
		IQueryFilterPtr 			ipQueryFilter( ipFilter );
		if( SUCCEEDED( ipFilter->QueryInterface( &ipQueryFilter ) ) )
		{
			// �n���ꂽ�t�B�[�`���Ɠ������[�N�X�y�[�X����m�[�h���C����������
			IFeatureClassPtr ipNodeClass( AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), NODE_TABLE_NAME ) );
			if( ! ipNodeClass )
			{
				_ASSERTE( ipNodeClass );
				return NULL;
			}

			// �W�I���g���̎擾
			IGeometryPtr ipGeom;
			ipFeature->get_Shape( &ipGeom );
			if( ipGeom == NULL ) 
			{
				MessageBox( ::GetActiveWindow(), ERROR_NON_GEOMETRY_ON_FEATURE, INVALID_FEATURE, MB_ICONEXCLAMATION );
				return NULL;
			}

			// �N�G���t�B���^�̍쐬
			CComBSTR cComBSTR;
			ipFilter->putref_Geometry( ipGeom );

			ipNodeClass->get_ShapeFieldName( &cComBSTR );
			ipFilter->put_GeometryField( cComBSTR );
			ipFilter->put_SpatialRel( esriSpatialRelTouches );

			// ����
			IFeatureCursorPtr ipFeatureCursor;
			if( SUCCEEDED( ipNodeClass->Search( ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor ) ) && NULL != ipFeatureCursor )
			{
				IFeaturePtr ipFeatureTmp;
				while( ipFeatureCursor->NextFeature( &ipFeatureTmp ) == S_OK )
					cTargetItems.push_back( ipFeatureTmp );
			}
		}
	}

	return cTargetItems.size();
}

//////////////////////////////////////////////////////////////////////////////////////

LONG AheSelectLQRowByInfoRow( const ROWDEF& listInfoRow, ITablePtr ipLQTable, std::list<ROWDEF>& listRQRows )
{
	if( ipLQTable == NULL ) return 0;

	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_SubFields(CComBSTR(_T("*")));
	_IRowPtr ipRow( listInfoRow.obj );
	LONG lOID = listInfoRow.oid;

	if( ipRow == NULL ) return listRQRows.size();

	// �N�G���t�B���^�쐬
	CString strWhereClause;
	strWhereClause.Format(_T("%s = %d"), INF_ID_FIELD, lOID );
	_ICursorPtr ipCursor;
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );

	ipLQTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	
	while( ipCursor->NextRow( &ipRow ) == S_OK )
	{
		ROWDEF rowDef;

		LONG lOID = -1;
		ipRow->get_OID( &lOID );
		rowDef.obj = ipRow;
		rowDef.oid = lOID;

		listRQRows.push_back( rowDef );
	}

	return listRQRows.size();
}

/**
 * @brief �����N�񑶍݃`�F�b�N
*/
BOOL AheCheckLQ(CTargetItem cSelectItem, ITablePtr ipInfoTable, ITablePtr ipLQTable, CString strRoadRelFieldName, LONG lDrawMode)
{
	if ( ipLQTable )	// [Bug 6551]�����N��̃e�[�u�������݂��Ȃ����Ƃ����e���邽��NULL�`�F�b�N����
	{
		std::list<ROWDEF>		listInfoRows;
		std::list<ROWDEF>		listRoads;
		ROWDEF					rowDef;

		listInfoRows.clear();
		listRoads.clear();

		IFeaturePtr	ipFeature = cSelectItem.GetFeature();
		IGeometryPtr ipGeometry = cSelectItem.GetShapeCopy();

		rowDef.oid = cSelectItem.GetOID();
		rowDef.shp = ipGeometry;
		rowDef.obj = ipFeature;
		rowDef.flg = lDrawMode;
		listRoads.push_back( rowDef );

		// ���H�ɑ��݂��郍�[�h�L���[��INFO���R�[�h���擾
		AheSelectInfoRowByRoadIDs( listRoads, ipInfoTable, ipLQTable, lDrawMode, listInfoRows );
		if( listInfoRows.size() <= 0 ) {
			CComVariant 	vaValue;

			cSelectItem.GetAttribute( strRoadRelFieldName, vaValue );
			if ( vaValue.vt == VT_I4 && vaValue.lVal == 1 ) {
				IQueryFilterPtr 	ipQueryFilter( CLSID_QueryFilter );
				_ICursorPtr 			ipCursor;
				_IRowPtr 			ipRow;
				CString 			strWhereClause;
				BOOL 				bFind = FALSE;

				strWhereClause.Format( _T("%s = %d"), LINK_ID_FIELD, cSelectItem.GetOID() );
				ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
				ipLQTable->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor );

				while( ipCursor->NextRow( &ipRow ) == S_OK ) {
					if( ipRow != NULL ) {
						bFind = TRUE;
						break;
					}
				}
				if( bFind != TRUE ) {
					if ( cSelectItem.IsEditableItem( sindyeEditAttribute ) == FALSE )
						return FALSE;
					cSelectItem.SetModifyData( sindyUpdateTypeProperty );
					cSelectItem.SetAttribute( strRoadRelFieldName, CComVariant( 0L ) );
					if( cSelectItem.Store() != TRUE )
						return FALSE;
				}
			}
		}
	}

	return TRUE;
}
BOOL AheDeleteLQ( CTargetItem& cSelectItem, LPCTSTR strInfoTableName, LPCTSTR strLQTableName, LPCTSTR strRoadRelFieldName, LONG lDrawMode )
{
	IGeometryPtr			ipGeometry = cSelectItem.GetShapeCopy();
	std::list<ROWDEF>		listInfoRows;
	std::list<ROWDEF>		listLQRows;
	std::list<ROWDEF>		listLQRoads;
	std::list<ROWDEF> 		listRoads;
	std::list<ROWDEF>::iterator 	it;

	listInfoRows.clear();
	listLQRows.clear();
	listLQRoads.clear();
	listRoads.clear();

	// ���H�����N�����݂��郏�[�N�X�y�[�X���烊���N��e�[�u�����I�[�v��
	ITablePtr ipInfoTable( AheOpenSameOwnerTable( (ITablePtr)cSelectItem.GetFeatureClass(), strInfoTableName ) );
	ITablePtr ipLQTable( AheOpenSameOwnerTable( (ITablePtr)cSelectItem.GetFeatureClass(), strLQTableName ) );

	if ( ipInfoTable && ipLQTable )	// [Bug 6551]�����N��̃e�[�u�������݂��Ȃ����Ƃ����e���邽��NULL�`�F�b�N����
	{
		// �w��̃W�I���g���ɑ��݂��郍�[�h�L���[��INFO���R�[�h���擾
		if( AheSelectInfoRowByShape( cSelectItem.GetFeatureClass(), ipInfoTable, ipLQTable, strRoadRelFieldName, ipGeometry, lDrawMode, listInfoRows, listRoads ) < 1 ) {
			if ( AheCheckLQ( cSelectItem, ipInfoTable, ipLQTable, strRoadRelFieldName, lDrawMode ) == FALSE )
				return FALSE;
			return TRUE;
		}
		// �w���INFO���R�[�h�񂩂烍�[�h�L���[��LQ���R�[�h���擾
		if( AheSelectLQRowByInfoRows( listInfoRows, ipLQTable, listLQRows ) < 1 ) {
			if ( AheCheckLQ( cSelectItem, ipInfoTable, ipLQTable, strRoadRelFieldName, lDrawMode ) == FALSE )
				return FALSE;
			return TRUE;
		}
		// �w���LQ���R�[�h�񂩂瓹�H�����N���擾
		if( AheSelectLQRoad( cSelectItem.GetFeatureClass(), listLQRows, listRoads, listLQRoads ) < 1 ) {
			if ( AheCheckLQ( cSelectItem, ipInfoTable, ipLQTable, strRoadRelFieldName, lDrawMode ) == FALSE )
				return FALSE;
			return TRUE;
		}
		if( listInfoRows.size() <= 0 )
			return FALSE;

		it = listInfoRows.begin();
		while( it != listInfoRows.end() ) {
			if( !( it->obj->Delete() == S_OK && it->obj->Store() == S_OK ) )
				return FALSE;
			else
				it = listInfoRows.erase( it );
		}

		it = listLQRows.begin();
		while( it != listLQRows.end() ) {
			if( !( it->obj->Delete() == S_OK && it->obj->Store() == S_OK ) )
				return FALSE;
			else
				it = listLQRows.erase( it );
		}

		// ���H�̊֘A�t���O��0�ɂ���
		AheSelectInfoRowByRoadIDs( listRoads, ipInfoTable, ipLQTable, lDrawMode, listInfoRows );
		if( listInfoRows.size() <= 0 ) {
			CComVariant 	vaValue;

			cSelectItem.GetAttribute( strRoadRelFieldName, vaValue );
			if ( vaValue.vt == VT_I4 && vaValue.lVal == 1 ) {
				IQueryFilterPtr 	ipQueryFilter( CLSID_QueryFilter );
				_ICursorPtr 			ipCursor;
				_IRowPtr 			ipRow;
				CString 			strWhereClause;
				BOOL 				bFind = FALSE;

				strWhereClause.Format( _T("%s = %d"), LINK_ID_FIELD, cSelectItem.GetOID() );
				ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
				ipLQTable->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor );

				while( ipCursor->NextRow( &ipRow ) == S_OK ) {
					if( ipRow != NULL ) {
						bFind = TRUE;
						break;
					}
				}
				if( bFind != TRUE ) {
					if ( cSelectItem.IsEditableItem( sindyeEditAttribute ) == FALSE )
						return FALSE;
					cSelectItem.SetModifyData( sindyUpdateTypeProperty );
					cSelectItem.SetAttribute( strRoadRelFieldName, CComVariant( 0L ) );
					if( cSelectItem.Store() != TRUE )
						return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/**
 * @brief �����N��폜
*/
INT AheDeleteLQ( const CTargetItems& cRoadItems, CTargetItem& cSelectItem, CTargetItem& cCompItem, LPCTSTR strInfoTableName, LPCTSTR strLQTableName, LPCTSTR strRoadRelFieldName, LONG lDrawMode, INT nType, INT nDelMode )
{
	std::list<ROWDEF>			listInfoRows, listLQRows, listRoads, listTmpRoads;
	std::list<ROWDEF>::iterator 	listInfoRows_iterator;
	std::list<ROWDEF>::iterator 	listLQRows_iterator;
	ROWDEF					rowDef;
	BOOL					TorF = FALSE, TorF2 = FALSE;

//	cSelectItem.GetLayer( &ipLayer );
	IFeaturePtr	ipFeature = cSelectItem.GetFeature();
	IGeometryPtr ipGeometry = cSelectItem.GetShapeCopy();

	IFeaturePtr ipCompFeature = cCompItem.GetFeature();

	// ���H�����N�����݂��郏�[�N�X�y�[�X���烊���N��e�[�u�����I�[�v��
	ITablePtr ipInfoTable( AheOpenSameOwnerTable( (ITablePtr)cSelectItem.GetFeatureClass(), strInfoTableName ) );
	ITablePtr ipLQTable( AheOpenSameOwnerTable( (ITablePtr)cSelectItem.GetFeatureClass(), strLQTableName ) );

	listInfoRows.clear();
	listLQRows.clear();
	listRoads.clear();
	listTmpRoads.clear();

	rowDef.oid = cSelectItem.GetOID();
	rowDef.shp = ipGeometry;
	rowDef.obj = ipFeature;
	rowDef.flg = lDrawMode;
	listTmpRoads.push_back( rowDef );

	// ���H�ɑ��݂��郍�[�h�L���[��INFO���R�[�h���擾
	if ( AheSelectInfoRowByRoadIDs( listTmpRoads, ipInfoTable, ipLQTable, lDrawMode, listInfoRows ) < 1 ) {
		if ( AheCheckLQ( cSelectItem, ipInfoTable, ipLQTable, strRoadRelFieldName, lDrawMode ) == FALSE )
			return IDNO;
		if ( AheCheckLQ( cCompItem, ipInfoTable, ipLQTable, strRoadRelFieldName, lDrawMode ) == FALSE )
			return IDNO;
		return IDYES;
	}

	listInfoRows_iterator = listInfoRows.begin();
	while( listInfoRows_iterator != listInfoRows.end() ) {
		TorF2 = FALSE;

		listLQRows.clear();
		if ( AheSelectLQRowByInfoRow( (*listInfoRows_iterator), ipLQTable, listLQRows ) < 1 )
			return IDYES;

		listRoads.clear();
		if( AheSelectLQRoad( cSelectItem.GetFeatureClass(), listLQRows, listTmpRoads, listRoads ) < 1 )
			return IDYES;

		for( std::list<ROWDEF>::iterator listRoads_iterator = listRoads.begin(); listRoads_iterator != listRoads.end(); listRoads_iterator++ ) {
			if ( listRoads_iterator->oid == cCompItem.GetOID() ) {
				listLQRows_iterator = listLQRows.begin();
				while( listLQRows_iterator != listLQRows.end() ) {
					CComVariant 		vaValueLink;
					INT					nLQType;

					vaValueLink = AheGetAttribute( listLQRows_iterator->obj, LINK_ID_FIELD );
					if ( vaValueLink.lVal == cSelectItem.GetOID() ) {
						CComVariant 		vaValueLast, vaValueSeq;
						CComVariant 		vaValueSeq3, vaValueSeq4;

						for( std::list<ROWDEF>::iterator listLQRows_iterator2 = listLQRows.begin(); listLQRows_iterator2 != listLQRows.end(); listLQRows_iterator2++ ) {
							CComVariant 		vaValueLink2;

							vaValueLink2 = AheGetAttribute( listLQRows_iterator2->obj, LINK_ID_FIELD );
							if ( vaValueLink2.lVal == cSelectItem.GetOID() ) {
								vaValueSeq3 = AheGetAttribute( listLQRows_iterator2->obj, SEQUENCE_FIELD );
							} else if ( vaValueLink2.lVal == cCompItem.GetOID() ) {
								vaValueSeq4 = AheGetAttribute( listLQRows_iterator2->obj, SEQUENCE_FIELD );
							}
						}
						if ( vaValueSeq3.lVal > vaValueSeq4.lVal ) {
							nLQType = 1;
						} else {
							nLQType = 0;
						}

						vaValueLast = AheGetAttribute( listLQRows_iterator->obj, LASTLINK_F_FIELD );
						vaValueSeq = AheGetAttribute( listLQRows_iterator->obj, SEQUENCE_FIELD );

						if( !( listLQRows_iterator->obj->Delete() == S_OK && listLQRows_iterator->obj->Store() == S_OK ) )
							return IDNO;
						else
							listLQRows_iterator = listLQRows.erase( listLQRows_iterator );

						for( std::list<ROWDEF>::iterator listLQRows_iterator2 = listLQRows.begin(); listLQRows_iterator2 != listLQRows.end(); listLQRows_iterator2++ ) {
							CComVariant 		vaValueLink2, vaValueLinkDir, vaValueSeq2;

							vaValueLink2 = AheGetAttribute( listLQRows_iterator2->obj, LINK_ID_FIELD );
							vaValueSeq2 = AheGetAttribute( listLQRows_iterator2->obj, SEQUENCE_FIELD );
							if ( vaValueLink2.lVal == cCompItem.GetOID() ) {
								vaValueLinkDir = AheGetAttribute( listLQRows_iterator2->obj, LINKDIR_C_FIELD );
								if ( nLQType == nType ) {
									vaValueLinkDir.lVal = 1;
									AheSetAttribute( listLQRows_iterator2->obj, LINKDIR_C_FIELD, vaValueLinkDir );
								} else {
									vaValueLinkDir.lVal = 2;
									AheSetAttribute( listLQRows_iterator2->obj, LINKDIR_C_FIELD, vaValueLinkDir );
								}
							}

							if ( vaValueLast.lVal == 1 ) {
								if ( vaValueSeq.lVal - 1 == vaValueSeq2.lVal ) {
									AheSetAttribute( listLQRows_iterator2->obj, LASTLINK_F_FIELD, vaValueLast );
									listLQRows_iterator2->obj->Store();
								}
							} else {
								if ( vaValueSeq.lVal < vaValueSeq2.lVal ) {
									AheSetAttribute( listLQRows_iterator2->obj, SEQUENCE_FIELD, CComVariant( vaValueSeq2.lVal - 1L ) );
									listLQRows_iterator2->obj->Store();
								}
							}

						}
					}
					listLQRows_iterator++;
				}
				TorF2 = TRUE;
			}
		}

		if ( TorF2 == FALSE ) {
			for( CTargetItems::const_iterator RoadItems_iterator = cRoadItems.begin(); RoadItems_iterator != cRoadItems.end(); RoadItems_iterator++ ) {
				for( std::list<ROWDEF>::iterator listRoads_iterator2 = listRoads.begin(); listRoads_iterator2 != listRoads.end(); listRoads_iterator2++ ) {
					if ( cSelectItem.GetOID() != listRoads_iterator2->oid  && cCompItem.GetOID() != listRoads_iterator2->oid ) {
						if ( listRoads_iterator2->oid == (*RoadItems_iterator).GetOID() )
							TorF = TRUE;
					}
				}
			}

			if ( TorF == TRUE ) {
				std::list<ROWDEF>::iterator	listLQRowsDel_iterator = listLQRows.begin();
				while( listLQRowsDel_iterator != listLQRows.end() ) {
					if( !( listLQRowsDel_iterator->obj->Delete() == S_OK && listLQRowsDel_iterator->obj->Store() == S_OK ) )
						return IDNO;
					else
						listLQRowsDel_iterator = listLQRows.erase( listLQRowsDel_iterator );
				}
				if( !( listInfoRows_iterator->obj->Delete() == S_OK && listInfoRows_iterator->obj->Store() == S_OK ) )
					return IDNO;
				else
					listInfoRows_iterator = listInfoRows.erase( listInfoRows_iterator );
			} else {
				BOOL		bTorF = FALSE;

				listLQRows_iterator = listLQRows.begin();
				while( listLQRows_iterator != listLQRows.end() ) {
					CComVariant 		vaValueLink;

					vaValueLink = AheGetAttribute( listLQRows_iterator->obj, LINK_ID_FIELD );
					if ( vaValueLink.lVal != cSelectItem.GetOID() )
						bTorF = TRUE;
					listLQRows_iterator++;
				}
				if ( bTorF == TRUE ) {
					listLQRows_iterator = listLQRows.begin();
					while( listLQRows_iterator != listLQRows.end() ) {
						if( !( listLQRows_iterator->obj->Delete() == S_OK && listLQRows_iterator->obj->Store() == S_OK ) )
							return IDNO;
						else
							listLQRows_iterator = listLQRows.erase( listLQRows_iterator );
					}
					if( !( listInfoRows_iterator->obj->Delete() == S_OK && listInfoRows_iterator->obj->Store() == S_OK ) )
						return IDNO;
					else
						listInfoRows_iterator = listInfoRows.erase( listInfoRows_iterator );
				} else 
					listInfoRows_iterator++;
			}
		} else {
			AheSetModifyData(listInfoRows_iterator->obj,sindyUpdateTypeShape);
			listInfoRows_iterator++;
			if ( listInfoRows_iterator != listInfoRows.end() )
				continue;
			else 
				return IDCANCEL;
		}
	}
	if ( nDelMode == 0 ) {
		if ( AheCheckLQ( cSelectItem, ipInfoTable, ipLQTable, strRoadRelFieldName, lDrawMode ) == FALSE )
			return IDNO;
	}
	return IDYES;
}


/**
 * @brief �����N�񌟍�
*/
BOOL AheSelectDeleteLQ( const CTargetItems& cRoadItems, CTargetItem& cSelectItem, CTargetItem& cCompItem, INT nType )
{
	CLoadQueueInfo	cLQInfo;
	BOOL			bRoadIsAttrEditable = FALSE;
	BOOL			bCheckedRoadLayer	= FALSE;

	if ( cSelectItem.GetFeatureKinds() != sindyeRoadFeature || cCompItem.GetFeatureKinds() != sindyeRoadFeature )
		return TRUE;
	
	for( INT i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ )
	{
		CString strInfoTableName	= cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoInfoTableName );
		CString strLQTableName		= cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQTableName );
		CString strRoadRelFieldName	= cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName );
		LONG    lDrawMode			= cLQInfo.GetLoadQueueDrawMode( (sindyeRoadQueueType)i );
		CComVariant vaValue      = cSelectItem.GetAttribute( strRoadRelFieldName );
		CComVariant vaValue_Comp = cCompItem.GetAttribute( strRoadRelFieldName );

		if( vaValue.vt == VT_I4 && vaValue.lVal == 1 )
		{
			if( AheIsDeletableLayer( strInfoTableName ) == FALSE )
				return FALSE;

			// ���H�����N�������ҏW�\���ǂ���
			if( bCheckedRoadLayer == FALSE )
			{
				bCheckedRoadLayer = TRUE;
				bRoadIsAttrEditable = AheIsAttrEditableLayer( ROAD_TABLE_NAME );
			}
			if( bRoadIsAttrEditable == FALSE )
				return TRUE;

			INT nRet = AheDeleteLQ( cRoadItems, cSelectItem, cCompItem, strInfoTableName, strLQTableName, strRoadRelFieldName, lDrawMode, nType, 0);
			if( nRet == IDNO )
				return FALSE;
			else if( nRet == IDYES )
			{
				if( AheDeleteLQ( cRoadItems, cCompItem, cSelectItem, strInfoTableName, strLQTableName, strRoadRelFieldName, lDrawMode, nType, 1) == IDNO )
					return FALSE;
			}
		}
	}
	return TRUE;
}

/**
 * @brief �����N��폜���b�Z�[�W
*/
BOOL AheDeleteLQMsg( const CTargetItems& cRoadItems )
{
	CLoadQueueInfo	cLQInfo;
	BOOL			bMsg = FALSE;

	for( CTargetItems::const_iterator iCnt = cRoadItems.begin(); iCnt != cRoadItems.end(); iCnt++ ) 
	{
		if ( iCnt->GetFeatureKinds() != sindyeRoadFeature )
			continue;

		for( INT i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ )
		{
			CComVariant vaValue = iCnt->GetAttribute( cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName ) );
			if( vaValue.vt == VT_I4 && vaValue.lVal == 1 )
			{
				if ( bMsg == FALSE ) {
					if( AheMessageBox( CONFIRM_DELETE_LINKQUEUE, DELETE_LINKQUEUE, MB_ICONQUESTION|MB_OKCANCEL|MB_DEFBUTTON1, WARNLEVEL_MIDDLE ) == IDCANCEL )
						return FALSE;
					bMsg = TRUE;
				}
			}
		}
	}
	return TRUE;
}

struct tagCLASS {
	CTargetItem	cItem;
	LONG		lVal;
};

/**
 * @brief �����_�𗧑̌����ɂ���(���H���������Ȃ牽�{�ł��}�[�W�ł���&�}�[�W����𓹘H��ʂƊp�x�Ŕ��ʂ���)
*/
BOOL AheCrossingToTwoLevelCrossing2(IMapPtr ipMap, IScreenDisplayPtr ipScreen, CTargetItem cNodeItem, IPointPtr ipPoint, INT nMode, CTargetItems& cRetDelItems)
{
	ISpatialReferencePtr		ipSpRef;
	CTargetItems				RoadItems, TmpRoads,TmpRoads1;
	std::list<DOUBLE>				listAngle;
	std::list<tagCLASS>				listClass_C;
	std::list<LONG>					listComp;
	INT							nComp = 0;
	CComVariant					vaValue;
	BOOL						bDel = TRUE, TorF = FALSE;

	IFeaturePtr	ipFeature = cNodeItem.GetFeature();
	IGeometryPtr ipSelectGeometry = cNodeItem.GetShapeCopy();

	RoadItems.clear();
	AheGetConnectedRoad( ipFeature, RoadItems );

	if ( AheDeleteLQMsg( RoadItems ) == FALSE )
		return FALSE;

	if( RoadItems.size() == 2 ) {
		bDel = nMode;
	} else if( RoadItems.size() >= 2 ) {
		cNodeItem.GetAttribute( NODECLASS_FIELD, vaValue );
		if ( vaValue.lVal == sindyeNodeIntersection ) {
			if( nMode == 1 )
				bDel = TRUE;
			else
				bDel = FALSE;
		} else {
			if ( nMode == 1 ) 
				bDel = TRUE;
		}
	}

	TmpRoads.clear();
	for( CTargetItems::iterator it = RoadItems.begin(); it != RoadItems.end(); it++ ) {
		CComVariant		vaValue1, vaValue2;
		BOOL			TorF = FALSE;

		it->GetAttribute( _T("ROADCLASS_C"), vaValue1 );
		for( CTargetItems::iterator it2 = RoadItems.begin(); it2 != RoadItems.end(); it2++ ) {
			if ( it->GetOID() == it2->GetOID() )
				continue;
			it2->GetAttribute( _T("ROADCLASS_C"), vaValue2 );
			if ( vaValue1.lVal == vaValue2.lVal ) {
				TorF = TRUE;
				break;
			}
		}
		if ( TorF == TRUE )
			TmpRoads.push_back( (*it) );
	}

	TmpRoads1.clear();
	for( CTargetItems::iterator it3 = RoadItems.begin(); it3 != RoadItems.end(); it3++ ) {
		BOOL			TorF = FALSE;

		for( CTargetItems::iterator it = TmpRoads.begin(); it != TmpRoads.end(); it++ ) {
			if ( it3->GetOID() == it->GetOID() )
				TorF = TRUE;
		}
		if ( TorF == FALSE )
			TmpRoads1.push_back( (*it3) );
	}

	for( CTargetItems::iterator it = TmpRoads1.begin(); it != TmpRoads1.end(); it++ ) {
		TmpRoads.push_back( (*it) );
	}

	RoadItems.clear();
	RoadItems = TmpRoads;

	TmpRoads.clear();
	CTargetItems::iterator it = RoadItems.begin();
	while ( it != RoadItems.end() ) {
		BOOL		TorF2 = FALSE;

		for( CTargetItems::iterator it3 = TmpRoads.begin(); it3 != TmpRoads.end(); it3++ ) {
			if ( (*it).GetOID() == (*it3).GetOID() )
				TorF2 = TRUE;
		}

		if ( TorF2 == TRUE ) {
			it++;
			continue;
		}
		TmpRoads.push_back( (*it) );

		listClass_C.clear();

		for( CTargetItems::iterator it2 = RoadItems.begin(); it2 != RoadItems.end(); it2++ ) {
			tagCLASS		tagTemp;

			TorF2 = FALSE;
			for( CTargetItems::iterator  it3 = TmpRoads.begin(); it3 != TmpRoads.end(); it3++ ) {
				if ( (*it2).GetOID() == (*it3).GetOID() )
					TorF2 = TRUE;
			}
			if ( TorF2 == TRUE )
				continue;

			it2->GetAttribute( _T("ROADCLASS_C"), vaValue );
			tagTemp.lVal = vaValue.lVal;
			tagTemp.cItem = (*it2);
			listClass_C.push_back( tagTemp );
		}

		LONG	ComCnt = 0;

		it->GetAttribute( _T("ROADCLASS_C"), vaValue );
		for ( std::list<tagCLASS>::iterator itc = listClass_C.begin(); itc != listClass_C.end(); itc++ ) {
			tagCLASS		tagTemp;

			tagTemp = (*itc);
			if ( tagTemp.lVal == vaValue.lVal ) 
				ComCnt++;
		}

		if ( ComCnt == 1 ) {
			for ( std::list<tagCLASS>::iterator itc = listClass_C.begin(); itc != listClass_C.end(); itc++ ) {
				tagCLASS		tagTemp;

				tagTemp = (*itc);
				if ( tagTemp.lVal == vaValue.lVal ) {
					nComp = AheCompRoad( (*it), tagTemp.cItem );
					if ( nComp == TRUE ) {
						TorF = AheSelectMeargeRoad( ipMap, ipScreen, RoadItems, ipPoint, (*it), tagTemp.cItem, bDel );
						cRetDelItems.push_back( tagTemp.cItem );
					} else {
						TorF = AheSelectMeargeRoad( ipMap, ipScreen, RoadItems, ipPoint, tagTemp.cItem, (*it), bDel );
						cRetDelItems.push_back( (*it) );
					}
					TmpRoads.push_back( tagTemp.cItem );
					break;
				}
			}
		} else {
			listAngle.clear();

			for( CTargetItems::iterator it2 = RoadItems.begin(); it2 != RoadItems.end(); it2++ ) {
				TorF2 = FALSE;
				for( CTargetItems::iterator  it3 = TmpRoads.begin(); it3 != TmpRoads.end(); it3++ ) {
					if ( (*it2).GetOID() == (*it3).GetOID() )
						TorF2 = TRUE;
				}
				if ( TorF2 == TRUE )
					continue;

				DOUBLE dw = AheGetRoadCrossAngle( ipScreen, (*it), (*it2), ipSelectGeometry );
				if ( dw < 0 )
					dw = dw * -1;
				DOUBLE nTmp = 180.0f- dw;
				if ( nTmp < 0 )
					nTmp = dw - 180.0f;
				dw = nTmp;
				listAngle.push_back( dw );
			}

			listAngle.sort();
			listAngle.unique();

			std::list<DOUBLE>::iterator ite = listAngle.begin();
			for( CTargetItems::const_iterator it2 = RoadItems.begin(); it2 != RoadItems.end(); it2++ ) {
				TorF2 = FALSE;
				for( CTargetItems::iterator it3 = TmpRoads.begin(); it3 != TmpRoads.end(); it3++ ) {
					if ( (*it2).GetOID() == (*it3).GetOID() )
						TorF2 = TRUE;
				}
				if ( TorF2 == TRUE )
					continue;

				DOUBLE dw = AheGetRoadCrossAngle( ipScreen, (*it), (*it2), ipSelectGeometry );
				if ( dw < 0 )
					dw = dw * -1;
				DOUBLE nTmp = 180.0f- dw;
				if ( nTmp < 0 )
					nTmp = dw - 180.0f;

				if ( (*ite) == nTmp ) {
					nComp = AheCompRoad( (*it), (*it2) );
					if ( nComp == TRUE ) {
						TorF = AheSelectMeargeRoad( ipMap, ipScreen, RoadItems, ipPoint, (*it), (*it2), bDel );
						cRetDelItems.push_back( (*it2) );
					} else {
						TorF = AheSelectMeargeRoad( ipMap, ipScreen, RoadItems, ipPoint, (*it2), (*it), bDel );
						cRetDelItems.push_back( (*it) );
					}
					TmpRoads.push_back( (*it2) );
					break;
				}
			}
		}
		it++;
	}

	return TorF;
}

/**
 * @brief ���H���}�[�W
*/
BOOL AheSelectMeargeRoad(IMapPtr ipMap, IScreenDisplayPtr ipScreen, CTargetItems cRoadItems, IPointPtr ipPoint, CTargetItem cRoadItem1, CTargetItem cRoadItem2, BOOL bDel)
{
	BOOL			TorF = TRUE;
	INT				nType;

	TorF = AheMeargeRoad( ipMap, ipScreen, ipPoint, cRoadItem1, cRoadItem2, nType, bDel);
	if ( TorF == TRUE ) {
		if ( AheSelectDeleteLQ( cRoadItems, cRoadItem2, cRoadItem1, nType) == FALSE )
			return FALSE;
	}
	return TorF;
}

/**
 * @brief ���H���}�[�W
*/
BOOL AheMeargeRoad(IMapPtr ipMap, IScreenDisplayPtr ipScreen, IPointPtr ipPoint, CTargetItem cRoadItem1, CTargetItem cRoadItem2, INT& nRetType, BOOL bDel)
{
	ITopologicalOperatorPtr		ipTopo;
	ISpatialReferencePtr		ipSpatialReference;
	IGeometryPtr				ipUnionGeom;
	IPolylinePtr				ipPolyline;
	IPointPtr					ipFromPoint1, ipToPoint1, ipFromPoint2, ipToPoint2, ipHitPoint;
	CComVariant					vaFrom1, vaTo1, vaFrom2, vaTo2, vaFrom, vaTo;
	BOOL						TorF = TRUE;

	IFeaturePtr ipFeature1 = cRoadItem1.GetFeature();
	IFeaturePtr ipFeature2 = cRoadItem2.GetFeature();
	IGeometryPtr ipGeom1 = cRoadItem1.GetShape();
	IGeometryPtr ipGeom2 = cRoadItem2.GetShape();

	ipGeom1->get_SpatialReference( &ipSpatialReference );
	ipFromPoint1 = cRoadItem1.GetFromPoint();
	ipFromPoint1->putref_SpatialReference( ipSpatialReference );
	ipFromPoint1->SnapToSpatialReference();
	ipToPoint1 = cRoadItem1.GetToPoint();
	ipToPoint1->putref_SpatialReference( ipSpatialReference );
	ipToPoint1->SnapToSpatialReference();

	AheSnapToSpatialReferenceAndForceSimplify( ipGeom2, ipSpatialReference );
	ipFromPoint2 = cRoadItem2.GetFromPoint();
	ipFromPoint2->putref_SpatialReference( ipSpatialReference );
	ipFromPoint2->SnapToSpatialReference();
	ipToPoint2 = cRoadItem2.GetToPoint();
	ipToPoint2->putref_SpatialReference( ipSpatialReference );
	ipToPoint2->SnapToSpatialReference();

	// FROM_NODEID �� TO_NODEID ���� From�ATo �̃|�C���g���擾
	cRoadItem1.GetAttribute( FROM_FIELD, vaFrom1 );
	cRoadItem1.GetAttribute( TO_FIELD, vaTo1 );
	cRoadItem2.GetAttribute( FROM_FIELD, vaFrom2 );
	cRoadItem2.GetAttribute( TO_FIELD, vaTo2 );

	ipTopo = ipGeom1;
	ipTopo->Union( ipGeom2, &ipUnionGeom );
	AheSnapToSpatialReferenceAndForceSimplify( ipUnionGeom, ipSpatialReference );

	ipPolyline = ipUnionGeom;
	ipPolyline->putref_SpatialReference( ipSpatialReference );
	ipPolyline->SnapToSpatialReference();
	if ( AheIsEqual( ipFromPoint1, ipFromPoint2 ) ) {		// �Z���\�\�Z�\�\���Z >> �Z���\�\�\�\�\�Z
		ipPolyline->ReverseOrientation();
		nRetType = 0;
	} else if ( AheIsEqual( ipFromPoint1, ipToPoint2 ) ) {	// �Z���\�\�Z���\�\�Z >> �Z���\�\�\�\�\�Z
		ipPolyline->put_FromPoint( ipFromPoint2 );
		ipPolyline->put_ToPoint( ipToPoint1 );
		nRetType = 0;
	} else if ( AheIsEqual( ipToPoint1, ipFromPoint2 ) ) {	// �Z�\�\���Z�\�\���Z >> �Z�\�\�\�\�\���Z
		ipPolyline->put_FromPoint( ipFromPoint1 );
		ipPolyline->put_ToPoint( ipToPoint2 );
		nRetType = 1;
	} else if ( AheIsEqual( ipToPoint1, ipToPoint2 ) ) {	// �Z�\�\���Z���\�\�Z >> �Z�\�\�\�\�\���Z
		ipPolyline->put_FromPoint( ipFromPoint1 );
		ipPolyline->put_ToPoint( ipFromPoint2 );
		nRetType = 1;
	}
	ipPolyline->putref_SpatialReference( ipSpatialReference );
	ipPolyline->SnapToSpatialReference();
	ipUnionGeom = ipPolyline;
	AheSnapToSpatialReferenceAndForceSimplify( ipUnionGeom, ipSpatialReference );

	if ( vaFrom1 == vaFrom2 ) {		// �Z���\�\�Z�\�\���Z >> �Z���\�\�\�\�\�Z
		vaFrom = vaTo2;
		vaTo = vaTo1;
		nRetType = 0;
	} else if( vaFrom1 == vaTo2 ) {	// �Z���\�\�Z���\�\�Z >> �Z���\�\�\�\�\�Z
		vaFrom = vaFrom2;
		vaTo = vaTo1;
		nRetType = 0;
	} else if( vaTo1 == vaFrom2 ) {	// �Z�\�\���Z�\�\���Z >> �Z�\�\�\�\�\���Z
		vaFrom = vaFrom1;
		vaTo = vaTo2;
		nRetType = 1;
	} else if( vaTo1 == vaTo2 ) {	// �Z�\�\���Z���\�\�Z >> �Z�\�\�\�\�\���Z
		vaFrom = vaFrom1;
		vaTo = vaFrom2;
		nRetType = 1;
	}

	cRoadItem1.SetAttribute( FROM_FIELD, vaFrom );
	cRoadItem1.SetAttribute( TO_FIELD, vaTo );
	cRoadItem1.SetShape( ipUnionGeom );
	cRoadItem1.SnapToSpatialReference();
	if ( ! cRoadItem1.IsEditableItem( sindyeEditAttribute ) )
		return FALSE;

	CTargetItems		cChkItems, cChkItems2, cChkItems3;

	AheGetConnectedNode( cRoadItem1.GetFeature(), cChkItems2 );
	cChkItems = cChkItems2;
	for( CTargetItems::iterator it = cChkItems2.begin(); it != cChkItems2.end(); it++ ) {
		cChkItems3.clear();
		AheGetConnectedRoad( it->GetFeature(), cChkItems3 );
		for( CTargetItems::iterator it2 = cChkItems3.begin(); it2 != cChkItems3.end(); it2++ ) {
			if ( cRoadItem2.GetOID() != it2->GetOID() )
				cChkItems.push_back( (*it2) );
		}
	}

	CErrObjContainer errs;
	cChkItems.IsSafeGeometries( errs );
	CString strTmp2;
	switch( errs.GetMaxLogLevel() )
	{
		case sindyeLogLevel_ERR:
			strTmp2.Format( MESSAGE_STOP_PROCESS, errs.ToString() );
			AheMessageBox( strTmp2, CHECK_SHAPE, MB_OK | MB_DEFBUTTON2 | MB_ICONEXCLAMATION, WARNLEVEL_NONE );
			return FALSE;
			break;
		case sindyeLogLevel_WARN:
			strTmp2.Format( CONFIRM_CONTINUE, errs.ToString() );
			if ( AheMessageBox( strTmp2, CHECK_SHAPE, MB_YESNO | MB_DEFBUTTON2 | MB_ICONEXCLAMATION, WARNLEVEL_NONE ) == IDNO )
				return FALSE;
			break;
		default:
			break;
	}
	cRoadItem1.SetModifyData( sindyUpdateTypeProperty );
	cRoadItem1.Store();

	if ( ! bDel )
	{
		CString strLayers;
		strLayers.Format( _T("|%s|%s|"), SEGMENTATTR_TABLE_NAME, GRAD_TABLE_NAME );

		// �����N���������͌X�΂����邩
		CTargetItems cSearchItems;
		if ( AheSelectRoad_SegAttr_GradFromVertex( ipMap, ipScreen, ipPoint, strLayers, cSearchItems ) ) {
			// �����N���������͌X�΂̍\���_���폜
			if ( ! AheDeleteSegAttr_GradVertexWithRoad( ipScreen, cSearchItems, ipPoint) )
				return FALSE;
		}
		TorF = AheDeleteRoadVertex( ipScreen, cRoadItem1, ipPoint );
	}

	return TorF;
}

/**
 * @brief ���H�\���_�폜
*/
BOOL AheDeleteRoadVertex( IScreenDisplayPtr ipScreen, CTargetItem& cSelectItem, IPointPtr ipHitPoint)
{
	ISpatialReferencePtr	ipSpatialReference;
	IPointPtr				ipPointTmp, ipPoint;
	LONG					SelectIndex = 0;

	IGeometryPtr ipSelectGeometry = cSelectItem.GetShape();
	ipSelectGeometry->get_SpatialReference( &ipSpatialReference );

	SelectIndex = cSelectItem.HitTestVertexIndex( ipHitPoint, AheConvertPixelsToMapUnits( ipScreen, AheGetTolerance() ) );
	ipPoint = cSelectItem.GetPoint( SelectIndex );
	ipPoint->putref_SpatialReference( ipSpatialReference );
	ipPoint->SnapToSpatialReference();

	ipPointTmp = cSelectItem.GetFromPoint();
	ipPointTmp->putref_SpatialReference( ipSpatialReference );
	ipPointTmp->SnapToSpatialReference();
	if ( AheIsEqual( ipPoint, ipPointTmp, ipSpatialReference ) )
		SelectIndex++;

	ipPointTmp = cSelectItem.GetToPoint();
	ipPointTmp->putref_SpatialReference( ipSpatialReference );
	ipPointTmp->SnapToSpatialReference();
	if ( AheIsEqual( ipPoint, ipPointTmp, ipSpatialReference ) )
		SelectIndex--;

	ipSelectGeometry = cSelectItem.RemovePoint( SelectIndex );
	AheSnapToSpatialReferenceAndForceSimplify( ipSelectGeometry, ipSpatialReference );
	cSelectItem.SetShape( ipSelectGeometry );

	// �`��`�F�b�N
	CErrObjContainer errs;
	cSelectItem.IsSafeGeometry( errs );
	CString strTmp;
	switch( errs.GetMaxLogLevel() )
	{
		case sindyeLogLevel_ERR:
			strTmp.Format( MESSAGE_STOP_PROCESS, errs.ToString() );
			AheMessageBox( strTmp, CHECK_SHAPE, MB_OK | MB_DEFBUTTON2 | MB_ICONEXCLAMATION, WARNLEVEL_NONE );
			return FALSE;
			break;
		case sindyeLogLevel_WARN:
			strTmp.Format( CONFIRM_CONTINUE, errs.ToString() );
			if ( AheMessageBox( strTmp, CHECK_SHAPE, MB_YESNO | MB_DEFBUTTON2 | MB_ICONEXCLAMATION, WARNLEVEL_NONE ) == IDNO )
				return FALSE;
			break;
		default:
			break;
	}
	cSelectItem.SetModifyData( sindyUpdateTypeShape );
	cSelectItem.Store();

	return TRUE;
}

/**
 * @brief �����N���������͌X�΂̌���
*/
BOOL AheSelectRoad_SegAttr_GradFromVertex(IMapPtr ipMap, IScreenDisplayPtr ipScreen, IPointPtr ipPoint, LPCTSTR lpszLayer, CTargetItems& cRetItems)
{
	ITopologicalOperatorPtr		ipTopo;
	ISpatialReferencePtr	iSpatialReference;	// <- �ǂ��ł��Z�b�g����Ă��Ȃ���H�����́H 2003/07/15 by furukawa.
	IEnvelopePtr			ipEnvelope;
	IPointPtr				ipHitPoint;
	IGeometryPtr			ipSearchGeom;
	CTargetItems 			cSerchItems;
	CString					strLayers = lpszLayer;
	DOUBLE 					dSearchRadius;
	UINT					unTorelance = AheGetTolerance();

	unTorelance = unTorelance + ( unTorelance / 3 );
	dSearchRadius = AheConvertPixelsToMapUnits( ipScreen, unTorelance );
	ipPoint->get_Envelope( &ipEnvelope );
	ipEnvelope->put_Height( dSearchRadius );
	ipEnvelope->put_Width( dSearchRadius );
	ipEnvelope->CenterAt( ipPoint );
	ipSearchGeom = ipEnvelope;
	ipTopo = ipPoint;

	cSerchItems.clear();
	if ( SUCCEEDED( ipTopo->Buffer( dSearchRadius, &ipSearchGeom ) ) && ipSearchGeom != NULL ) {
		if( AheSelectByShapeFromLayers( ipMap, ipSearchGeom, cSerchItems, strLayers, esriSpatialRelIntersects, NULL, NULL, TRUE ) < 1 )
			return FALSE;
	} else {
		if( AheSelectByShapeFromLayers( ipMap, ipPoint, cSerchItems, strLayers, esriSpatialRelIntersects, NULL, NULL, TRUE ) < 1 )
			return FALSE;
	}

	CTargetItems::iterator itItem = cSerchItems.begin();
	while( itItem != cSerchItems.end() ) {
		IPointPtr				ipPointB;
		LONG					VertexCount = 0;
		BOOL					TorF = FALSE;

		VertexCount = itItem->GetVertexCount();
		TorF = TRUE;
		for ( LONG lCnt = 0;lCnt < VertexCount;lCnt++ ) {
			ipPointB = itItem->GetPoint( lCnt );
			ipPointB->putref_SpatialReference( iSpatialReference );
			ipPointB->SnapToSpatialReference();
			if ( AheIsEqual( ipPointB, ipPoint, iSpatialReference ) == TRUE ) {
				TorF = TRUE;
				break;
			}
		}
		if ( TorF == FALSE )
			itItem = cSerchItems.erase( itItem );
		else
			itItem++;
	}

//	cSerchItems.SelectByPoint( ipPoint, dSearchRadius, &ipHitPoint );
	ipPoint->putref_SpatialReference( iSpatialReference );
	ipPoint->SnapToSpatialReference();

	cRetItems.clear();
	cRetItems = cSerchItems;
	return TRUE;
}

/**
 * @brief ���H�ɂ��郊���N�������ƌX�΂̍\���_���폜
*/
BOOL AheDeleteSegAttr_GradVertexWithRoad( IScreenDisplayPtr ipScreen, CTargetItems& cSearchItems, IPointPtr ipPoint)
{
	CTargetItems::iterator 	iCnt;

	if ( cSearchItems.size() <= 0 )
		return TRUE;

	// ���������N�������������[�v
	for( iCnt = cSearchItems.begin(); iCnt != cSearchItems.end(); iCnt++ ) {
		if ( AheDeletePolyLineVertex( ipScreen, (*iCnt), ipPoint ) == FALSE )
			return FALSE;
	}
	return TRUE;
}

/**
 * @brief ���C���\���_�폜
*/
BOOL AheDeletePolyLineVertex( IScreenDisplayPtr ipScreen, CTargetItem& cSelectItem, IPointPtr ipPoint)
{
	ISpatialReferencePtr		iSpatialReference;
	IGeometryPtr				ipSelectGeometry;
	LONG						SelectIndex = 0;
	UINT						unTorelance = AheGetTolerance();

	// ��ԎQ�Ƃ�n���ꂽ�t�B�[�`���̃t�B�[�`���N���X����擾
	IGeoDatasetPtr ipGeoDataset( cSelectItem.GetFeatureClass() );
	if( ipGeoDataset != NULL ) 
		ipGeoDataset->get_SpatialReference( &iSpatialReference );
	else
		_ASSERTE( ipGeoDataset != NULL );

	unTorelance = unTorelance + ( unTorelance / 3 );
	ipSelectGeometry = cSelectItem.GetShape();

	SelectIndex = cSelectItem.HitTestVertexIndex( ipPoint, AheConvertPixelsToMapUnits( ipScreen, unTorelance ) );
	ipSelectGeometry = 	cSelectItem.RemovePoint( SelectIndex );
	AheSnapToSpatialReferenceAndForceSimplify( ipSelectGeometry, iSpatialReference );

	cSelectItem.SetShape( ipSelectGeometry );

	// �`��`�F�b�N
	CErrObjContainer errs;
	cSelectItem.IsSafeGeometry( errs );
	CString strTmp;
	switch( errs.GetMaxLogLevel() )
	{
		case sindyeGeometryError:
			strTmp.Format( MESSAGE_STOP_PROCESS, errs.ToString() );
			AheMessageBox( strTmp, CHECK_SHAPE, MB_OK | MB_DEFBUTTON2 | MB_ICONEXCLAMATION, WARNLEVEL_NONE );
			return FALSE;
			break;
		case sindyeGeometryWarnning:
			strTmp.Format( CONFIRM_CONTINUE, errs.ToString() );
			if ( AheMessageBox( strTmp, CHECK_SHAPE, MB_YESNO | MB_DEFBUTTON2 | MB_ICONEXCLAMATION, WARNLEVEL_NONE ) == IDNO )
				return FALSE;
			break;
		default:
			break;
	}
	cSelectItem.SetModifyData( sindyUpdateTypeShape );
	cSelectItem.Store();

	return TRUE;
}

/**
 * @brief �R�_�̊p�x�����߂�
*/
DOUBLE AheGetRoadCrossAngle( IScreenDisplayPtr ipScreen, CTargetItem cRoadItem1, CTargetItem cRoadItem2, IPointPtr ipPoint)
{
	LONG					lRoadVertexNum;
	ISpatialReferencePtr	ipSpatialReference;

	cRoadItem1.GetShapeCopy()->get_SpatialReference( &ipSpatialReference );
	// ���ꂼ��̃t�B�[�`���̒��_�̃C���f�b�N�X�ԍ��擾
	lRoadVertexNum = cRoadItem1.HitTestVertexIndex( ipPoint, AheConvertPixelsToMapUnits( ipScreen, AheGetTolerance() ) );

	// ���_�̂ЂƂO�̃|�C���g���擾
	IPointPtr ipPoint1 = ( lRoadVertexNum == 0 ) ? cRoadItem1.GetPoint( 1 ) : cRoadItem1.GetPoint( lRoadVertexNum - 1 );
	ipPoint1->putref_SpatialReference( ipSpatialReference );
	ipPoint1->SnapToSpatialReference();

	cRoadItem2.GetShapeCopy()->get_SpatialReference( &ipSpatialReference );
	lRoadVertexNum = cRoadItem2.HitTestVertexIndex( ipPoint, AheConvertPixelsToMapUnits( ipScreen, AheGetTolerance() ) );
	IPointPtr ipPoint2 = ( lRoadVertexNum == 0 ) ? cRoadItem2.GetPoint( 1 ) : cRoadItem2.GetPoint( lRoadVertexNum - 1 );
	ipPoint2->putref_SpatialReference( ipSpatialReference );
	ipPoint2->SnapToSpatialReference();
	// 3�_�̊p�x�𒲂ׂ�i�Ƃ肠���� VEDIT �̎d�l�Łj
	return AheGetAngle( ipPoint1, ipPoint, ipPoint2 );
}

/**
 * @brief ���H��r
*/
BOOL AheCompRoad(CTargetItem cRoadItem1, CTargetItem cRoadItem2)
{
	if ( cRoadItem1.GetOID() > cRoadItem2.GetOID() )
		return TRUE;
	else
		return FALSE;
}

BOOL AheCheckOneWay(CTargetItems cItems)
{
	ILayerPtr pILayer;
	IFeatureLayerPtr ipFeatureLayer;
	IDatasetPtr pIDataset;
	IWorkspacePtr pIWorkspace;
	IFeatureWorkspacePtr ipFeatureWorkspace;
	ITablePtr pITable;
	IFeaturePtr ipFeatureNext;
	CComVariant vTmp, vFrom, vFromNext, vTo, vToNext, vDir(-1L);
	BOOL bFromTo = FALSE, bToFrom = FALSE;
	long lOID = -1;

	for ( CTargetItems::iterator it = cItems.begin(); it != cItems.end(); it++ ) 
	{
		IFeaturePtr ipFeature = it->GetFeature();

		/// ���݂̓��H�����N�Ɉ���ʍs���������邩�H
		vTmp = AheGetAttribute(ipFeature, ONEWAY_C_FIELD);

		if ( it == cItems.begin()) {
			vFrom = AheGetAttribute( ipFeature, FROM_FIELD );
			vTo = AheGetAttribute( ipFeature, TO_FIELD );
		}

		if ( 5 == vTmp.lVal) return FALSE;		/// �������Ɉ���ʍs������Ƃ��͏I��

		if ( 0 != vTmp.lVal ) {
			/// �����̊m�F
			ATLTRACE(_T("����ʍs����\n"));

			switch ( vTmp.lVal ) {
			case 1:
			case 3:
				bFromTo = TRUE;
				break;
			case 2:
			case 4:
				bToFrom = TRUE;
				break;
			}				

			ipFeature->get_OID(&lOID);

			/// ���̗v�f�� From To �𒲍����Ď������g�̕���������
			it++;

			if ( (it) != cItems.end() ) {
				ipFeatureNext = it->GetFeature();

				if ( ipFeatureNext ) {
					vFromNext = AheGetAttribute( ipFeatureNext, FROM_FIELD );
					vToNext = AheGetAttribute( ipFeatureNext, TO_FIELD );

					ATLTRACE(_T(" FROM_NEXT NODE %d \n "), vFromNext.intVal);
					ATLTRACE(_T(" TO_NEXT NODE %d \n "), vToNext.intVal);

					/// ��������ɂ������� �P�F�������@�Q�F�t����
					if ( vFrom.intVal == vFromNext.intVal ) {
						vTmp.lVal = 2;
					} else if ( vFrom.intVal == vToNext.intVal ) {
						vTmp.lVal = 2;
					} else if ( vTo.intVal == vFromNext.intVal ) {
						vTmp.lVal = 1;
					} else if ( vTo.intVal == vToNext.intVal ) {
						vTmp.lVal = 1;
					}

					vFrom = vFromNext;
					vTo = vToNext;
					vDir = vTmp;

					if ( (TRUE == bFromTo) & (2 == vTmp.lVal) ) {
						/// �t�����Ɉ���ʍs�i�������j����
						return FALSE;
					}

					if ( (TRUE == bToFrom) & (1 == vTmp.lVal) ) {
						/// �t�����Ɉ���ʍs�i�t�����j����
						return FALSE;
					}
				}
			} else {
				/// �ЂƂO�̃����N�� TO �̂ݎ擾
				it--;
				it--;
				ipFeature = it->GetFeature();

				/// FromNext �� From ���t�ɂȂ��Ă���
				vFromNext = AheGetAttribute( ipFeature, FROM_FIELD );
				vToNext = AheGetAttribute( ipFeature, TO_FIELD );				
				
				it = cItems.end();

				switch ( vDir.lVal ) {
				case 1:
					/// 1�O��������
					if ( TRUE == bFromTo ) {
						/// �����̈���ʍs�͏�����
						if ( vToNext.intVal == vTo.intVal ) {
							return FALSE;
						}
					} else {
						///�@�����̈���ʍs�͋t����
						if ( vToNext.intVal == vFrom.intVal ) {
							return FALSE;
						}
					}

					break;
				case 2:
					/// 1�O���t����
					if ( TRUE == bFromTo ) {
						/// �����̈���ʍs�͏�����
						if ( vFromNext.intVal == vTo.intVal) {
							return FALSE;
						}
					} else {
						///�@�����̈���ʍs�͋t����
						if ( vFromNext.intVal == vFrom.intVal ) {
							return FALSE;
						}
					}

					break;
				}
			}

			it--;
		} else {
			/// ���̗v�f�� From To �𒲍����Ď������g�̕���������
			it++;

			if ( (it) != cItems.end() ) {
				ipFeatureNext = it->GetFeature();

				if ( ipFeatureNext ) {
					vFromNext = AheGetAttribute( ipFeatureNext, FROM_FIELD );
					vToNext = AheGetAttribute( ipFeatureNext, TO_FIELD );

					ATLTRACE(_T(" FROM_NEXT NODE %d \n "), vFromNext.intVal);
					ATLTRACE(_T(" TO_NEXT NODE %d \n "), vToNext.intVal);

					/// ��������ɂ������� �P�F�������@�Q�F�t����
					if ( vFrom.intVal == vFromNext.intVal ) {
						vTmp.lVal = 2;
					} else if ( vFrom.intVal == vToNext.intVal ) {
						vTmp.lVal = 2;
					} else if ( vTo.intVal == vFromNext.intVal ) {
						vTmp.lVal = 1;
					} else if ( vTo.intVal == vToNext.intVal ) {
						vTmp.lVal = 1;
					}

					vFrom = vFromNext;
					vTo = vToNext;
					vDir = vTmp;
				}
			}

			it--;
		} 
	}

	return TRUE;

}

BOOL AheCheckVics(CTargetItems cItems)
{
	VARIANT vRoute;
	
	for ( CTargetItems::iterator it = cItems.begin(); it != cItems.end(); it++ ) {
		IFeaturePtr ipFeature = it->GetFeature();
		
		vRoute = AheGetAttribute( ipFeature, _T("NAVICLASS_C"));

		if ( 0 == vRoute.intVal ) return FALSE;
	}

	return TRUE;
}

BOOL AheCheckLQOverlap(IMapPtr pIMap, CTargetItems cItems, sindyeRoadQueueType LQType, INT nFromNodeID)
{
	CLoadQueueInfo cLQInfo;
	IFeaturePtr ipFeature, ipFeatureNext;
	CComVariant vTmp, vFrom, vFromNext, vTo, vToNext, vDir( 0L );
	BOOL bFromTo = FALSE, bToFrom = FALSE;
	long lOID = -1;

	for( CTargetItems::iterator it = cItems.begin(); it != cItems.end(); it++ )
	{
		ipFeature = it->GetFeature();

		/// ���݂̓��H�����N�� VICS ���������邩�H
		vTmp = AheGetAttribute(ipFeature, cLQInfo.GetLoadQueueInfo(	LQType, sindyeLoadQueueInfoLQRFRoadFieldName ) );

		if( it == cItems.begin() )
		{
			vFrom = AheGetAttribute( ipFeature, FROM_FIELD );
			vTo = AheGetAttribute( ipFeature, TO_FIELD );
		}

		if( 1 == vTmp.lVal )
		{
			/// �����̊m�F
			ATLTRACE(_T("���� VICS ��������\n"));

			ipFeature->get_OID(&lOID);

			ITablePtr pITable = AheOpenTableByTableName( AheGetWorkspace( ipFeature ), cLQInfo.GetLoadQueueInfo( LQType, sindyeLoadQueueInfoLQTableName ) );							
			if( pITable != NULL )
			{
				/// �N�G���t�B���^�̍쐬
				IQueryFilterPtr pIQueryFilter( CLSID_QueryFilter );
				CString strWhere;
				strWhere.Format( _T("LINK_ID = %d"), lOID );				
				pIQueryFilter->put_WhereClause( CComBSTR(strWhere) );

				_ICursorPtr pICursor; 
				if( S_OK == pITable->Search( pIQueryFilter, VARIANT_FALSE, &pICursor ) )
				{
					bFromTo = FALSE;
					bToFrom = FALSE;

					_IRowPtr pIRow;
					while( S_OK == pICursor->NextRow(&pIRow) )
					{		
						/// ������ LQ �̃����N�̌������擾
						vTmp = AheGetAttribute( pIRow, LINKDIR_C_FIELD);

						switch (vTmp.lVal)
						{
						case 1:	bFromTo = TRUE;	break;
						case 2:	bToFrom = TRUE;	break;
						default: _ASSERTE( vTmp.lVal == 1 || vTmp.lVal == 2 ); break;
						}
					}

					/// �������Ƀ����N���ݒ肳��Ă���Α��I��
					if( bFromTo && bToFrom )
						return FALSE;

					/// ���̗v�f�� From To �𒲍����Ď������g�̕���������
					if ( 1 < cItems.size() )
					{
						++it;

						if( (it) != cItems.end() )
						{
							ipFeatureNext = it->GetFeature();

							if( ipFeatureNext != NULL )
							{
								vFromNext = AheGetAttribute( ipFeatureNext, FROM_FIELD );
								vToNext = AheGetAttribute( ipFeatureNext, TO_FIELD );

								ATLTRACE(_T(" FROM_NEXT NODE %d \n "), vFromNext.intVal);
								ATLTRACE(_T(" TO_NEXT NODE %d \n "), vToNext.intVal);

								/// ��������ɂ������� �P�F�������@�Q�F�t����
								if ( vFrom.intVal == vFromNext.intVal )
									vDir.lVal = 2;
								else if ( vFrom.intVal == vToNext.intVal )
									vDir.lVal = 2;
								else if ( vTo.intVal == vFromNext.intVal )
									vDir.lVal = 1;
								else if ( vTo.intVal == vToNext.intVal )
									vDir.lVal = 1;

								if ( bFromTo && (1 == vDir.lVal) )
									return FALSE;

								if ( bToFrom && (2 == vDir.lVal) )
									return FALSE;

								vFrom = vFromNext;
								vTo = vToNext;
							}
						}
						else {
							--it;
							--it;
							ipFeature = it->GetFeature();

							/// FromNext �� From ���t�ɂȂ��Ă���
							vFromNext = AheGetAttribute( ipFeature, FROM_FIELD );
							vToNext = AheGetAttribute( ipFeature, TO_FIELD );
							it = cItems.end();

							switch ( vDir.lVal )
							{
							case 1:
								/// 1�O��������
								if( bFromTo )
								{
									/// ������ VICS �͏�����
									if( vToNext.intVal == vFrom.intVal )
										return FALSE;
								}
								else {
									///�@������ VICS �͋t����
									if( vToNext.intVal == vTo.intVal )
										return FALSE;
								}
								break;
							case 2:
								/// 1�O���t����
								if( bFromTo )
								{
									/// ������ VICS �͏�����
									if ( vFromNext.intVal == vFrom.intVal)
										return FALSE;
								}
								else {
									///�@������ VICS �͋t����
									if( vFromNext.intVal == vTo.intVal )
										return FALSE;
								}
								break;
							default:
								break;
							}
						}

						--it;
					}
					else {
						/// �����N���ЂƂ̂Ƃ�
						if ( bFromTo )
						{
							if ( vFrom.intVal == nFromNodeID )
								return FALSE;
						}
						else {
							if ( vTo.intVal == nFromNodeID )
								return FALSE;
						}
					}			
				}
			}
			else {
				/// �Ȃ����Ă��Ƃ͂Ȃ��͂�
				ATLASSERT(0);

				return FALSE;
			}
		}
		else {
			/// ���̗v�f�� From To �𒲍����Ď������g�̕���������
			it++;

			if ( (it) != cItems.end() )
			{
				ipFeatureNext = it->GetFeature();

				if ( ipFeatureNext )
				{
					vFromNext = AheGetAttribute( ipFeatureNext, FROM_FIELD );
					vToNext = AheGetAttribute( ipFeatureNext, TO_FIELD );

					ATLTRACE(_T(" FROM_NEXT NODE %d \n "), vFromNext.intVal);
					ATLTRACE(_T(" TO_NEXT NODE %d \n "), vToNext.intVal);

					/// ��������ɂ������� �P�F�������@�Q�F�t����
					if ( vFrom.intVal == vFromNext.intVal )
						vDir.lVal = 2;
					else if ( vFrom.intVal == vToNext.intVal )
						vDir.lVal = 2;
					else if ( vTo.intVal == vFromNext.intVal )
						vDir.lVal = 1;
					else if ( vTo.intVal == vToNext.intVal )
						vDir.lVal = 1;

					vFrom = vFromNext;
					vTo = vToNext;
				}
			}

			it--;
		} 
	}

	return TRUE;
}

#if 0 // �����̌��ʁA���g�p
// �^����ꂽ�W�I���g���͈͓̔��ŁA���S�����ԋ߂��t�B�[�`����I������
// �c���āA���ꂶ�� SelectByShape �ň�����I�������ꍇ�ƈꏏ�W����
BOOL AheGetNearestFeature( IMapPtr piMap, IEnvelopePtr piEnv, IFeature** ppiFeatuer, BOOL bEditableOnly/* = FALSE*/ )
{
	CTargetItems cTargetItems;
	IGeometryPtr ipGeom( piEnv );
	IFeaturePtr ipTempFeature = NULL;
	DOUBLE dXMin = 0, dYMin = 0, dWidth = 0, dHeight = 0;

	// ������
	(*ppiFeatuer) = NULL;

	// �I�u�W�F�N�g�I���i�����j
	cTargetItems.clear();
	AheSelectByShape( piMap, ipGeom, cTargetItems, esriSpatialRelIntersects, NULL, NULL, FALSE, bEditableOnly );
	
	if( cTargetItems.size() > 0 ) {
		IPointPtr ipPoint( CLSID_Point );
		piEnv->get_XMin( &dXMin );
		piEnv->get_YMin( &dYMin );
		piEnv->get_Width( &dWidth );
		piEnv->get_Height( &dHeight );
		ipPoint->put_X( dXMin + dWidth / 2.0 );
		ipPoint->put_Y( dYMin + dHeight / 2.0 );
		// �i����
		return AheGetClosestFeatureInFeatureSelection( cTargetItems, min( dWidth, dHeight), ipPoint, ppiFeatuer);
	} else
		return FALSE;
}
#endif // if 0

LONG AheCheckVertexPoint( IMapPtr piMap, double dTol, IFeaturePtr ipFeature, IFeaturePtr ipOwnFeature, BOOL bDeleteEndNode, IFeature** ipEndNode, LPCTSTR lpcszLayerNames )
{
	IPointCollectionPtr			ipCol = NULL;
	IGeometryPtr				ipGeom = NULL;
	CTargetItems				cTargetFeature;
	LONG						lStart, lEnd;
	LONG						lPartIndex = 0, lVertexIndex = 0, lRet = 0;
	DOUBLE						dHitDist = 0.0;
	BOOL						bVertexHit = FALSE;

	// �t�B�[�`���̓_��擾
	AheGetFeatureVertex( ipFeature, &ipCol );

	// �_��S�Ăɑ΂��ă`�F�b�N��������
	ipCol->get_PointCount( &lEnd );
	lStart = 0;
	// ���H�̏ꍇ�̓X�^�[�g�|�C���g�ƃG���h�|�C���g�͏����i�K���m�[�h���ڑ�����Ă��邽�߁j
	if( AheGetFeatureKinds( ipFeature ) == sindyeRoadFeature && bDeleteEndNode == FALSE ) {
		lStart++;
		lEnd--;
	}

	for( LONG lIndex = lStart; lIndex < lEnd; lIndex++ )
	{
		IPointPtr					ipPoint = NULL;

		ipCol->get_Point( lIndex, &ipPoint );
		ipGeom = ipPoint;

		// �t�B�[�`���I��
		cTargetFeature.clear();
		AheSelectByShapeFromLayers( piMap, ipGeom, cTargetFeature, lpcszLayerNames );
		// ����I������Ă��Ȃ�������߂�
		if( cTargetFeature.size() < 1 )
			return 0;

		// �I�����ꂽ�t�B�[�`���̒�����i����
		for( CTargetItems::iterator it = cTargetFeature.begin(); it != cTargetFeature.end(); it++ )
		{
			IFeaturePtr ipSelectedFeature = it->GetFeature();

			// �������g�͏Ȃ�
			if( AheIsSameFeature( ipSelectedFeature, ipFeature ) == TRUE ) {
				it = cTargetFeature.erase( it);
				continue;
			}
			// ���O�t�B�[�`���͏���
			if( AheIsSameFeature( ipSelectedFeature, ipOwnFeature ) == TRUE ) {
				it = cTargetFeature.erase( it);
				continue;
			}
			if( bDeleteEndNode == TRUE ) {
				// �I�[�m�[�h��������
				if( AheGetFeatureKinds( ipSelectedFeature ) == sindyeNodeFeature && AheIsEndNode( ipSelectedFeature ) ) {
					(*ipEndNode) = ipSelectedFeature;
					(*ipEndNode)->AddRef();
					it = cTargetFeature.erase( it);
					continue;
				}
			}

			IPointPtr					ipHitPoint = NULL;
			esriGeometryHitPartType		emHitPart;

			emHitPart = AheTestFeatureGeometryHit( ipSelectedFeature, ipPoint, dTol, &ipHitPoint, &dHitDist, &lPartIndex, &lVertexIndex);
			switch( emHitPart)
			{
			case esriGeometryPartNone:		// ���_�A�ӂ̂ǂ���ɂ��q�b�g��
			case esriGeometryPartBoundary:	// ���_�Ƀq�b�g���Ȃ�����
				// �Z���N�V��������폜
				it = cTargetFeature.erase( it);
				continue;
			case esriGeometryPartVertex:
				break;
			}
		}
		lRet += cTargetFeature.size();
	}
	return lRet;
}


BOOL AheGetPointsCrossedMesh( IMapPtr/* piMap*/, IFeaturePtr piLineFeature, IPointCollection** ppiPointCollection)
{
	IGeometryPtr piGeomLileFeature;
	if( FAILED( piLineFeature->get_Shape( &piGeomLileFeature)))
		return FALSE;

	// 2�����b�V���|���S�����v�Z�ɂ��Z�o
	CMesh mesh;
	IGeometryPtr piGeometry = mesh.GetIntersectPointsByShape( piGeomLileFeature );

	// ���ʕۑ��C���X�^���X
	IPointCollectionPtr piPointCollection;
	if( FAILED( piPointCollection.CreateInstance( CLSID_Multipoint)))
		return FALSE;
	((IGeometryPtr)piPointCollection)->putref_SpatialReference( AheGetSpatialReference( piGeometry ) ); // ��ԎQ�Ƃ�t�^

	// �Z�o���ꂽ�����_�����ʂɕۑ�
	IGeometryCollectionPtr piGeomColl;
	esriGeometryType emGeomType;
	piGeometry->get_GeometryType( &emGeomType);
	if( esriGeometryPoint == emGeomType)
	{
		if( !AheIsEndPoint( piGeometry, piLineFeature))
		{
			IPointPtr piPoint = piGeometry;
			piPointCollection->AddPoint( piPoint);
		}
	}
	else
	{
		if( esriGeometryMultipoint == emGeomType)
		{
			IPointPtr piPoint;
			IPointCollectionPtr piResultPointColl = piGeometry;
			long lCount;
			piResultPointColl->get_PointCount( &lCount);
			for( long lIndex = 0; lIndex < lCount; lIndex++)
			{
				piResultPointColl->get_Point( lIndex, &piPoint);
				piGeometry = piPoint;
				if( !AheIsEndPoint( piGeometry, piLineFeature))
				{
					piPointCollection->AddPoint( piPoint);
				}
			}
		}
	}

	if( NULL != ppiPointCollection)
	{
		(*ppiPointCollection) = piPointCollection;
		(*ppiPointCollection)->AddRef();
	}

	return ( NULL != piPointCollection);
}


BOOL AheGetConnectedRoadAndNode( IFeaturePtr ipSelectRoad, IFeaturePtr ipSelectNode, IFeaturePtr &ipConnectedRoad, IFeaturePtr &ipOppNode )
{
	CTargetItems	cRoadItems;
	// �m�[�h�ɂ��Ă��邷�ׂĂ̓��H���擾
	AheGetConnectedRoad( ipSelectNode, cRoadItems );
	
	for(CTargetItems::iterator iRoad = cRoadItems.begin(); iRoad != cRoadItems.end(); iRoad++ )
	{
		IFeaturePtr ipRoadFeature = iRoad->GetFeature();

		// ���ܑI�𒆂̓��H�łȂ���΁A�~�������H
		LONG lSelectRoadOID, lRoadOID;
		ipSelectRoad->get_OID( &lSelectRoadOID );
		ipRoadFeature->get_OID( &lRoadOID );

		if( lSelectRoadOID != lRoadOID )
		{
			// ���Α��̓��H���m��
			ipConnectedRoad = ipRoadFeature;

			CTargetItems	cNodeItems;
			// ���H�����N�ɂ��Ă���m�[�h���擾
			AheGetConnectedNode( ipRoadFeature, cNodeItems );

			for(CTargetItems::iterator iNode = cNodeItems.begin(); iNode != cNodeItems.end(); iNode++ )
			{
				IFeaturePtr ipNodeFeature = iNode->GetFeature();

				// ���I�𒆂̃m�[�h�łȂ���΁A�����Е��̃m�[�h
				LONG lSelectNodeOID, lNodeOID;
				ipSelectNode->get_OID( &lSelectNodeOID );
				ipNodeFeature->get_OID( &lNodeOID );

				if( lSelectNodeOID != lNodeOID )
				{
					// ���Α��̃m�[�h���m��
					ipOppNode = ipNodeFeature;


					// �m�[�h��ʂ��擾
					IFieldsPtr		ipNodeFields;
					CComVariant		vNodeClass;
					long			lNodeClass = 0;
					sindyeNodeClass	sNodeClass; 

					ipNodeFeature->get_Fields(&ipNodeFields);
					ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
					ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
					sNodeClass = sindyeNodeClass(vNodeClass.lVal);

					// 2�����b�V���ゾ������A���[�Ƃ�2�����b�V���m�[�h
					if( sNodeClass == sindyeOnMesh || sNodeClass == sindyeOnMeshPEC)
						return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL AheIsRoadCrossMesh(IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cRoadCrossMesh)
{
	// PGDB�ł������ꍇ�ɂ́A�ŏ��łӂ邢���Ƃ�
	IDatasetPtr ipDataset( AheGetFeatureClass( ipFeature ) );
	if( ipDataset == NULL )
		return FALSE;

	IFeatureClassPtr ipFeatureClass = NULL;

	// �t�B�[�`���N���X�擾
	if( AheGetFeatureClass( ipMap, ipFeature, &ipFeatureClass))
	{
		// ���H�����N���ǂ����H
		if( AheCompareFeatureName( ipFeatureClass, ROAD_TABLE_NAME) == TRUE )
		{
			CTargetItems	cNodeItems;

			// ���H�����N�ɂ��Ă���m�[�h���擾
			LONG lNodeNum = AheGetConnectedNode( ipFeature, cNodeItems );

			// �ڑ����Ă���m�[�h�̌���2�łȂ�������A���������̂ŁA�����ŏI��
			// [Bug2606]�֘A�ŁA�܂����b�V�����E�Ƀm�[�h��2�d�Ȃ��Ă���ꍇ������̂ŁA����ւ̑Ή�
			// ��{�̓��H�����N�ɐڑ����Ă���m�[�h�͕K��2�ł���B
			if( lNodeNum != 2 )
				return FALSE;

			for(CTargetItems::iterator iCnt = cNodeItems.begin(); iCnt != cNodeItems.end(); iCnt++ )
			{
				IFeaturePtr ipNodeFeature = iCnt->GetFeature();
				CTargetItems	cRoadItems;

				// �m�[�h��ʂ��擾
				IFieldsPtr		ipNodeFields;
				CComVariant		vNodeClass;
				long			lNodeClass = 0;
				sindyeNodeClass	sNodeClass; 

				ipNodeFeature->get_Fields(&ipNodeFields);
				ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
				ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
				sNodeClass = sindyeNodeClass(vNodeClass.lVal);


				// 2�����b�V���ゾ������A���Α��̓��H���擾
				if( sNodeClass == sindyeOnMesh || sNodeClass == sindyeOnMeshPEC)
				{
					CTargetItems cConnectedRoadOnMesh;
					IFeaturePtr ipOppNode;
					IFeaturePtr ipRoadsOnMesh;
					cConnectedRoadOnMesh.clear();

					IFeaturePtr ipTempFeature = ipFeature;

					BOOL bIsBothNodeOnMesh = FALSE;

					do{
						bIsBothNodeOnMesh = AheGetConnectedRoadAndNode( ipTempFeature, ipNodeFeature, ipRoadsOnMesh, ipOppNode );

						// ���Α��̓��H���m�ۂ��Ă���
						cConnectedRoadOnMesh.push_back( ipRoadsOnMesh );

						// �I�����ꂽ���Α��̓��H�ɂ܂�2�����b�V���m�[�h�����Ă���ꍇ�A���H�����N�ƃm�[�h��
						// �u�������āA�����Ē��ׂ�
						if( bIsBothNodeOnMesh == TRUE )
						{
							ipTempFeature = ipRoadsOnMesh;
							ipNodeFeature = ipOppNode; 
						}
					}while( bIsBothNodeOnMesh == TRUE );

					// �I�����ꂽ�A2�����b�V���֘A�̓��H��o�^����
					for( CTargetItems::iterator it = cConnectedRoadOnMesh.begin(); it != cConnectedRoadOnMesh.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						cRoadCrossMesh.push_back( ipFeatureOnMesh );
					}
				}
			}
		}
	}
	if( cRoadCrossMesh.size() > 0 )
		return TRUE;
	return FALSE;
}

BOOL AheIsFeatureCrossMesh(IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cFeatureCrossMesh)
{
	// PGDB�ł������ꍇ�ɂ́A�ŏ��łӂ邢���Ƃ�
	if( ! AheIsRemoteWorkspace( AheGetWorkspace(ipFeature) ) )
		return FALSE;
	
	// �t�B�[�`���̃e�[�u�����擾
	 CString strTableName = AheGetFeatureClassName( ipFeature );

	 if( ( lstrcmpi(strTableName, GRAD_TABLE_NAME) == 0 ) ||
		 ( lstrcmpi(strTableName, SEGMENTATTR_TABLE_NAME ) == 0 ) )
	 {

		// �����NID���擾
		IFieldsPtr	ipFields;
		LONG lIndex = -1;
		CComVariant		vaValue;
		ipFeature->get_Fields( &ipFields );
		ipFields->FindField( CComBSTR(_T("LINK_ID")), &lIndex );
		ipFeature->get_Value( lIndex, &vaValue );

		// �n���ꂽ�X�΁A�����N���������������郏�[�N�X�y�[�X���擾
		// �����N���������t���Ă��铹�H�����N���擾
		IFeatureClassPtr ipRoadClass( AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass(ipFeature), ROAD_TABLE_NAME ) );

		if( ipRoadClass == NULL )
		{
			_ASSERTE( ipRoadClass != NULL );
			return FALSE;
		}
		IFeaturePtr ipRoadFeature;	// ���H�t�B�[�`��
		ipRoadClass->GetFeature( vaValue.lVal, &ipRoadFeature );

		if( ipRoadFeature != NULL )
		{
			CTargetItems	cNodeItems;

			// ���H�����N�ɂ��Ă���m�[�h���擾
			LONG lNodeNum = AheGetConnectedNode( ipRoadFeature, cNodeItems );

			// �ڑ����Ă���m�[�h�̌���2�łȂ�������A���������̂ŁA�����ŏI��
			// [Bug2606]�֘A�ŁA�܂����b�V�����E�Ƀm�[�h��2�d�Ȃ��Ă���ꍇ������̂ŁA����ւ̑Ή�
			// ��{�̓��H�����N�ɐڑ����Ă���m�[�h�͕K��2�ł���B
			if( lNodeNum != 2 )
				return FALSE;

			for(CTargetItems::iterator iCnt = cNodeItems.begin(); iCnt != cNodeItems.end(); iCnt++ )
			{

				IFeaturePtr	ipTempSegmentAttrFeature = ipFeature;

				IFeaturePtr ipNodeFeature = iCnt->GetFeature();
				CTargetItems	cRoadItems;

				// �m�[�h��ʂ��擾
				IFieldsPtr		ipNodeFields;
				CComVariant		vNodeClass;
				long			lNodeClass = 0;
				sindyeNodeClass	sNodeClass; 

				ipNodeFeature->get_Fields(&ipNodeFields);
				ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
				ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
				sNodeClass = sindyeNodeClass(vNodeClass.lVal);


				// 2�����b�V���ゾ������A���Α��̓��H���擾
				if( sNodeClass == sindyeOnMesh || sNodeClass == sindyeOnMeshPEC)
				{
					CTargetItems cConnectedRoadOnMesh;
					IFeaturePtr ipOppNode;
					IFeaturePtr ipRoadsOnMesh;
					cConnectedRoadOnMesh.clear();

					IFeaturePtr ipTempFeature = ipRoadFeature;

					BOOL bIsBothNodeOnMesh = FALSE;

					do{
						bIsBothNodeOnMesh = AheGetConnectedRoadAndNode( ipTempFeature, ipNodeFeature, ipRoadsOnMesh, ipOppNode );

						// �擾����2�����b�V�����E���ׂ������H�����N�Ƀ����N�������A�܂��͌X�΂����Ă��邩�ǂ������ׂ�

						// 2�����b�V�����ׂ��ł��铹�H�����N�̃I�u�W�F�N�gID�擾
						LONG lOIDOnMesh;
						ipRoadsOnMesh->get_OID( &lOIDOnMesh );
						// SEGMENTATTR_TABLE_NAME�擾
						ITablePtr ipTable( AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass(ipFeature), strTableName ) );

						// �e�[�u�����猟��
						IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
						_ICursorPtr ipCursor;
						CString strWhereClause, strSubFields;
						strWhereClause.Format(_T("LINK_ID = %ld"), lOIDOnMesh );
//						strSubFields.Format(_T("OBJECTID,Shape"));
						strSubFields.Format(_T("*"));
						ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
						ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
						ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

						if( ipCursor == NULL )
							break;

						// Geometry�擾
						IGeometryPtr ipCurrentGeom;
						IPolylinePtr ipCurrentPolyline;
						IPointPtr	ipCurrentToPoint, ipCurrentFromPoint;

						ipTempSegmentAttrFeature->get_ShapeCopy( &ipCurrentGeom );
						ipCurrentPolyline = ipCurrentGeom;
						ipCurrentPolyline->get_ToPoint( &ipCurrentToPoint );
						ipCurrentPolyline->get_FromPoint( &ipCurrentFromPoint );

						LONG lCurrentOID;
						ipTempSegmentAttrFeature->get_OID( &lCurrentOID );

						IFeaturePtr ipFeatureOnSameLink;
						_IRowPtr ipRow;
						while( ipCursor->NextRow( &ipRow ) == S_OK )
						{
							if( ipRow == NULL ) 
									break;

							ipFeatureOnSameLink = ipRow;
							LONG lOIDOnSameLink;
							ipFeatureOnSameLink->get_OID( &lOIDOnSameLink );

							if( lCurrentOID == lOIDOnSameLink )
								continue;

							IGeometryPtr ipGeomOnSameLink;
							IPolylinePtr ipPolylineOnSameLink;
							IPointPtr	ipToPointOnSameLink, ipFromPointOnSameLink;

							ipFeatureOnSameLink->get_ShapeCopy( &ipGeomOnSameLink );
							ipPolylineOnSameLink = ipGeomOnSameLink;
							ipPolylineOnSameLink->get_ToPoint( &ipToPointOnSameLink );
							ipPolylineOnSameLink->get_FromPoint( &ipFromPointOnSameLink );
							

							// �ڐG���Ă���ꍇ�ɂ͒ǉ�
							if( AheIsEqual( ipCurrentFromPoint, ipFromPointOnSameLink ) == TRUE ||
								AheIsEqual( ipCurrentFromPoint, ipToPointOnSameLink ) == TRUE ||
								AheIsEqual( ipCurrentToPoint, ipFromPointOnSameLink ) == TRUE ||
								AheIsEqual( ipCurrentToPoint, ipToPointOnSameLink ) == TRUE )
							{
								cFeatureCrossMesh.push_back( ipFeatureOnSameLink );
								ipTempSegmentAttrFeature = ipFeatureOnSameLink;
							}

						}
						if( ipFeatureOnSameLink == NULL )
							break;

						// ���Α��̓��H���m�ۂ��Ă���
						cConnectedRoadOnMesh.push_back( ipRoadsOnMesh );

						// �I�����ꂽ���Α��̓��H�ɂ܂�2�����b�V���m�[�h�����Ă���ꍇ�A���H�����N�ƃm�[�h��
						// �u�������āA�����Ē��ׂ�
						if( bIsBothNodeOnMesh == TRUE )
						{
							ipTempFeature = ipRoadsOnMesh;
							ipNodeFeature = ipOppNode;
						}
					}while( bIsBothNodeOnMesh == TRUE );
				}
			}
		}
	}
	if( cFeatureCrossMesh.size() > 0 )
		return TRUE;
	return FALSE;
}
