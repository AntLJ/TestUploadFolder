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

// EditHelper.cpp: CEditHelper �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EditHelper.h"
#include "Mesh.h"
#include "TargetFeatures.h"
#include "LoadQueueInfo.h"
#include "AheDraw.h"
#include "AheNetwork.h"
#include "RuleModel.h"
#include <set>

using namespace sindy::schema;

#ifdef _DEBUG
//#define _DEBUG_FOR_2059	// [bug 2059]�p
#ifdef _DEBUG_FOR_2059
#include "../../SiNDY-u/AnnoCheck/ShapeFileHelper.h"
#endif // ifdef _DEBUG_FOR_2059
#endif // ifdef _DEBUG

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR LINK_QUEUE_DIRGUIDE                        = _T("���ʈē�");
	LPCTSTR LINK_QUEUE_GUIDE                           = _T("�����U��");
	LPCTSTR LINK_QUEUE_VICS                            = _T("VICS");
	LPCTSTR LINK_QUEUE_BYWAY                           = _T("������");
	LPCTSTR LINK_QUEUE_TURNREG                         = _T("�E���܋֎~");
	LPCTSTR LINK_QUEUE_LANE                            = _T("���[��");
	LPCTSTR LINK_QUEUE_ROUTE                           = _T("�A�N�Z�X�^���p����");
	LPCTSTR LINK_QUEUE_UTURN                           = _T("U�^�[��");
	LPCTSTR LINK_QUEUE_INTERNAVI                       = _T("�C���^�[�i�rVICS");
	LPCTSTR LINK_QUEUE_IPCVICS                         = _T("iPC VICS");
	LPCTSTR LINK_QUEUE_INTERSECTION                    = _T("�i�������ʌ����_");
	LPCTSTR ERROR_FAILED_SPLIT                         = _T("�X�v���b�g�������s");
	LPCTSTR MESSAGE_FEATURE_IS_RANDOM                  = _T("���p���t�B�[�`�����莸�s�F\n\n���R�F%s\n\n����ID�������p���t�B�[�`���̓����_���ɂȂ�܂��B");
	LPCTSTR WARN_SPLIT                                 = _T("�X�v���b�g�����x��");
	LPCTSTR ERROR_CANNOT_CONTINUE_SPLIT                = _T("%s�̍쐬���֎~����Ă��邽�߁A�X�v���b�g�������p���ł��܂���");
	LPCTSTR ERROR_LINK_NOT_FOUND                       = _T("LQRF��t�^���邽�߂̓��H�����N��������܂���B");
	LPCTSTR ERROR_FAIL_TO_ROAD_LINK_REPLACE            = _T("���H�����N�iOBJECTID=%d�j�� FROM�ATO�̕t���ւ��Ɏ��s���܂����B�������p���ł��܂���B");
	LPCTSTR SPLIT_ROAD_LINK                            = _T("���H�����N�̃X�v���b�g����");
	LPCTSTR ERROR_FAIL_TO_RESET_NODE                   = _T("�P�ƃm�[�h���쐬���悤�Ƃ������A����2�����b�V�������2�{�ȏ�̓��H�����N��ڑ����悤�Ƃ������߂Ƀm�[�h��ʂ̍Đݒ�Ɏ��s���܂����B�������p���ł��܂���B");
	LPCTSTR ERROR_NOT_ADD_SHARED_SHAPE_POINT           = _T("���C���F%s OBJECTID=%d : �ړ��\�ȃt�B�[�`���ł͂Ȃ����߁A���L�\���_��ǉ����邱�Ƃ��ł��܂���B");
	LPCTSTR RESTRUCTURING_ATTRIBUTE_SLOPE              = _T("�����N�������E�X�΂̍\���_�č\�z");
	LPCTSTR ERROR_END_POINT_ISNOT_CONTACT_ROAD_LINK    = _T("���C���F%s OBJECTID=%d : �[�_�����H�����N�ɐڂ��Ă��Ȃ����߁A�������p�����邱�Ƃ��ł��܂���B");
	LPCTSTR SPLIT_OF_ATTRIBUTE_SLOPE                   = _T("�����N�������E�X�΂̃X�v���b�g");
	LPCTSTR ERROR_NOT_FIND_ROAD_LINK_ATTRIBUTE_SLOPE   = _T("�����N�������E�X�΂Ɗ֘A�����铹�H�����N�������邱�Ƃ��ł��܂���B");
	LPCTSTR SPLIT_OF_ATTRIBUTE_OR_SLOPE                = _T("�����N���������͌X�΂̃X�v���b�g");
	LPCTSTR ERROR_CANNOT_ACROSS_3_ROAD_LINK            = _T("�����N�������E�X�΂�3�{�ȏ�̓��H�����N���͗��̌������ׂ��ňړ����邱�Ƃ͂ł��܂���B");
	LPCTSTR ERROR_END_POINT_NOT_MATCH_ROAD_LINK        = _T("�����N�������E�X�΂̃X�v���b�g�`��̒[�_�����H�����N�ƈ�v���Ă��܂���B");
	LPCTSTR ERROR_FAIL_ADD_ROAD_LINK_ID                = _T("OBJECTID���p�����������N�������y�ьX�΂ւ̊֘A���H�����NID�t�^�Ɏ��s���܂����B");
	LPCTSTR ERROR_FAIL_TO_ADD_NEWLY_ROAD_LINK_ID       = _T("�V�K�ɍ쐬���ꂽ�����N�������y�ьX�΂ւ̊֘A���H�����NID�t�^�Ɏ��s���܂����B");
	LPCTSTR ERROR_COORD_AND_FROM_NOTE_ID_NOT_MATCH     = _T("���H�����N�F%d FROM_NODE_ID �ƃm�[�h�̍��W����v���܂���B");
	LPCTSTR ERROR_FAIL_ROAD_LINK_SPLIT                 = _T("���H�����N�X�v���b�g�������s");
	LPCTSTR ERROR_COORD_AND_TO_NOTE_ID_NOT_MATCH       = _T("���H�����N�F%d TO_NODE_ID �ƃm�[�h�̍��W����v���܂���B");
	LPCTSTR SLOPE                                      = _T("�X��");
	LPCTSTR ROAD_STRUCTURE_ATTRIBUTE                   = _T("�����N������");
	LPCTSTR ERROR_SHAPE_IS_APART_FROM_ROAD_LINK        = _T("���H�����N�F%d %s�F%d : %s�̌`�󂪓��H�����N���痣��Ă��܂��B");
	LPCTSTR ERROR_ROAD_EDIT_FAIL                       = _T("���H�n�ҏW�������s");
	LPCTSTR ERROR_FLAG_DOES_NOT_COME_WITH_ROAD_LINK    = _T("LQ�i%ld�j�Ɋ֘A���铹�H�����N�i%ld�j�Ɋ֘A�t���O�i%s�j���t���Ă��܂���");
	LPCTSTR ERROR_ROAD_LINK_NOT_EXIST_OR_SEARCH_FAIL   = _T("LQ�i%ld�j�Ɋ֘A���铹�H�����N�i%ld�j�����݂��Ȃ����A���͌����ł��Ă��܂���");
	LPCTSTR ERROR_SOMETHING_WRONG_WITH_SEQUENCE_NUMBER = _T("�����N��̃V�[�P���X�ԍ������������ł� : \n");
	LPCTSTR ERROR_START_POINT_AND_BEFORE_IS_NOT_SAME   = _T("���݂�LQ�̊J�n�_�ƂЂƂO�̏I���_���قȂ�܂�\n");
	LPCTSTR ERROR_NO_SHAPE_OF_ROAD_LINK                = _T("���H�����N�̌`�󂪑��݂��܂��� : OID[%d]");
	LPCTSTR ERROR_TERMINATION_FLAG_NOT_EXIST           = _T("�V�[�P���X�̍Ō�ɏI�[�t���O������܂���\n");
	LPCTSTR DIVISION_POLYGON                           = _T("�|���S���̕���");
	LPCTSTR ERROR_NO_TABLE                             = _T("%s�A%s�e�[�u�������݂��Ȃ����߁A�������p���ł��܂���\n");
	LPCTSTR ROAD_LINK_SPLIT                            = _T("���H�����N�̃X�v���b�g����");
	LPCTSTR ERROR_NOT_DELETE_FOLLOWING_RULE            = _T("���C�� : %s OBJECTID : %d : �ȉ��̃��[���ɂ��ҏW�ł��܂���:\n\n%s");
	LPCTSTR EDITING_PROHIBITED                         = _T("�ҏW�֎~");
	LPCTSTR CONFIRM_SET_TO_WARNING_LEVEL               = _T("���C�� : %s OBJECTID : %d : �ȉ��̃��[���ɂ�胏�[�j���O���x���ɐݒ肳��Ă��܂�:\n\n%s\n�ҏW���܂����H");
	LPCTSTR MESSAGE_WARNING                            = _T("�x��");
	LPCTSTR ERROR_SEQUENCE_NUMBER_NOT_CORRECT          = _T("%s�e�[�u�� OBJECTID=%d : �V�[�P���X�ԍ��������������Ă��Ȃ����߁A�����𑱍s�ł��܂���");
	LPCTSTR ERROR_DIRECTION_CODE_NOT_CORRECT           = _T("%s�e�[�u�� OBJECTID=%d : �����R�[�h�������������Ă��Ȃ����߁A�����𑱍s�ł��܂���");
	LPCTSTR ERROR_SPLIT_ROAD_LINK_ACROSS_MESH          = _T("�X�v���b�g��̓��H�����N�����b�V�����ׂ��ł��邽�߁A�����𑱍s�ł��܂���");
	LPCTSTR ERROR_SPLIT_ROAD_LINK_SHAPE_IS_IMPROPER    = _T("�X�v���b�g��̓��H�����N�̌`�󂪕s���Ȃ��߁A�����N��̏C�����ł��܂���");
	LPCTSTR ERROR_FAIL_COPY_EXTERNAL_RECORD            = _T("�O�����R�[�h�̃R�s�[�Ɏ��s���܂����B");
	LPCTSTR EXTERNAL_RECORD_COPY_FAILURE               = _T("�O�����R�[�h�R�s�[���s");
	LPCTSTR ERROR_FAIL_DELETE_EXTERNAL_RECORD          = _T("�O�����R�[�h�̍폜�Ɏ��s���܂����B");
	LPCTSTR EXTERNAL_RECORD_DELETE_FAILURE             = _T("�O�����R�[�h�폜���s");
	LPCTSTR CONFIRM_FILL_THIS_CLOSED_RING              = _T("���̕�Ԃ������߂��܂����H");
	LPCTSTR FILL_METHOD                                = _T("�����߃��[�`��");
	LPCTSTR CONFIRM_CHANGE_SHAPE_AND_EVADE_HEAP        = _T("�����C���ŏd�Ȃ��Ă���|���S�������݂��܂��B\n�����̃t�B�[�`���̌`���ύX���ďd�Ȃ��������܂����H");
	LPCTSTR CREATING_POLYGON                           = _T("�|���S���̍쐬");
	LPCTSTR CANNOT_ADD_SHARED_SHAPE_POINT_TO_MOVABLE   = _T("���C���F%s OBJECTID=%d : �ړ��\�ȃt�B�[�`���ł͂Ȃ����߁A���L�\���_��ǉ����邱�Ƃ��ł��܂���B");
	LPCTSTR RESTRUCTURING_OF_ATTRIBUTE_SLOPE           = _T("�����N�������E�X�΂̍\���_�č\�z");
#else
	LPCTSTR LINK_QUEUE_DIRGUIDE                        = _T("Signpost");
	LPCTSTR LINK_QUEUE_GUIDE                           = _T("Forced guiding");
	LPCTSTR LINK_QUEUE_VICS                            = _T("VICS");
	LPCTSTR LINK_QUEUE_BYWAY                           = _T("Byway");
	LPCTSTR LINK_QUEUE_TURNREG                         = _T("Turn Regulation");
	LPCTSTR LINK_QUEUE_LANE                            = _T("Lane");
	LPCTSTR LINK_QUEUE_ROUTE                           = _T("Access/connection road");
	LPCTSTR LINK_QUEUE_UTURN                           = _T("U turn");
	LPCTSTR LINK_QUEUE_INTERNAVI                       = _T("Internavi VICS");
	LPCTSTR LINK_QUEUE_IPCVICS                         = _T("iPCVICS");
	LPCTSTR LINK_QUEUE_INTERSECTION                    = _T("Direction dependent intersection");
	LPCTSTR ERROR_FAILED_SPLIT                         = _T("Failed to invoke split.");
	LPCTSTR MESSAGE_FEATURE_IS_RANDOM                  = _T("Failed to decide the inheriting feature�F\n\n Reason�F%s\n\n The feature is random when have the existing ID.");
	LPCTSTR WARN_SPLIT                                 = _T("Warning to split.");
	LPCTSTR ERROR_CANNOT_CONTINUE_SPLIT                = _T("Because creating of the %s is prohibited, I cannot continue split processing.");
	LPCTSTR ERROR_LINK_NOT_FOUND                       = _T("Road link for adding LQRF can not be found.");
	LPCTSTR ERROR_FAIL_TO_ROAD_LINK_REPLACE            = _T("Failed to replace FROM and TO of the road link (OBJECTID=%d). The processing can not be continued.");
	LPCTSTR SPLIT_ROAD_LINK                            = _T("Split processing of the road link");
	LPCTSTR ERROR_FAIL_TO_RESET_NODE                   = _T("You have failed to reset the node type because you tried to either make a single node or connect two or more road links to the boundary of the secondnary mesh line. The processing can not be continued.");
	LPCTSTR ERROR_NOT_ADD_SHARED_SHAPE_POINT           = _T("Layer: %s OBJECTID=%d : You can not add a shared shape point because it is not a movable feature.");
	LPCTSTR RESTRUCTURING_ATTRIBUTE_SLOPE              = _T("Restructuring of the shape point of the road structure attribute/slope");
	LPCTSTR ERROR_END_POINT_ISNOT_CONTACT_ROAD_LINK    = _T("Layer�F%s OBJECTID=%d : The processing can not be continued since the end point is not in contact with the road link.");
	LPCTSTR SPLIT_OF_ATTRIBUTE_SLOPE                   = _T("Splitting or road structure attribute/slope");
	LPCTSTR ERROR_NOT_FIND_ROAD_LINK_ATTRIBUTE_SLOPE   = _T("Unable to find the road link to be associated with road structure attribute/slope.");
	LPCTSTR SPLIT_OF_ATTRIBUTE_OR_SLOPE                = _T("Splitting of road structure attribute or slope");
	LPCTSTR ERROR_CANNOT_ACROSS_3_ROAD_LINK            = _T("Road structure attribute/slope can not be moved across three or more road links or grade separation..");
	LPCTSTR ERROR_END_POINT_NOT_MATCH_ROAD_LINK        = _T("End point of the split shape of road structure attribute/slope does not match the road link.");
	LPCTSTR ERROR_FAIL_ADD_ROAD_LINK_ID                = _T("Failed to add the associated road link ID to the road structure attribute/slope which inherited the OBJECTID");
	LPCTSTR ERROR_FAIL_TO_ADD_NEWLY_ROAD_LINK_ID       = _T("Failed to add the associated road link ID to the newly crerated road structure attribute and slope");
	LPCTSTR ERROR_COORD_AND_FROM_NOTE_ID_NOT_MATCH     = _T("Road link: %d Coordinates of the node and FROM_NODE_ID do not match.");
	LPCTSTR ERROR_FAIL_ROAD_LINK_SPLIT                 = _T("Road link split processing has failed");
	LPCTSTR ERROR_COORD_AND_TO_NOTE_ID_NOT_MATCH       = _T("Road link: %d Coordinates of the node and TO_NODE_ID do not match.");
	LPCTSTR SLOPE                                      = _T("slope");
	LPCTSTR ROAD_STRUCTURE_ATTRIBUTE                   = _T("road structure attribute");
	LPCTSTR ERROR_SHAPE_IS_APART_FROM_ROAD_LINK        = _T("road link:%d %s�F%d : The shape of %s is apart from the road link.");
	LPCTSTR ERROR_ROAD_EDIT_FAIL                       = _T("Road data editing process failure");
	LPCTSTR ERROR_FLAG_DOES_NOT_COME_WITH_ROAD_LINK    = _T("Relation flag (%s) does not come with a road link (%ld) related to LQ(%ld)");
	LPCTSTR ERROR_ROAD_LINK_NOT_EXIST_OR_SEARCH_FAIL   = _T("Either a road link (%ld) related to LQ(%ld) does not exist or the search has failed.");
	LPCTSTR ERROR_SOMETHING_WRONG_WITH_SEQUENCE_NUMBER = _T("Something is wrong with the sequence number of the link queue : \n");
	LPCTSTR ERROR_START_POINT_AND_BEFORE_IS_NOT_SAME   = _T("The start point of current LQ and the end point of the one before are not the same\n");
	LPCTSTR ERROR_NO_SHAPE_OF_ROAD_LINK                = _T("There is no shape of the road link : OID[%d]");
	LPCTSTR ERROR_TERMINATION_FLAG_NOT_EXIST           = _T("The termination flag does not exist at the end of the sequence\n");
	LPCTSTR DIVISION_POLYGON                           = _T("Division polygon.");
	LPCTSTR ERROR_NO_TABLE                             = _T("Processing can not be continued because there is no %s, %s table.\n");
	LPCTSTR ROAD_LINK_SPLIT                            = _T("Split processing of the road link");
	LPCTSTR ERROR_NOT_DELETE_FOLLOWING_RULE            = _T("Layer: %s OBJECTID : %d : Cannot be deleted due to the following rule:\n\n%s");
	LPCTSTR EDITING_PROHIBITED                         = _T("Editing prohibited");
	LPCTSTR CONFIRM_SET_TO_WARNING_LEVEL               = _T("Layer: %s OBJECTID : %d : It is set to warning level by the following rule:\n\n%s\n Do you want to edit?");
	LPCTSTR MESSAGE_WARNING                            = _T("Warning");
	LPCTSTR ERROR_SEQUENCE_NUMBER_NOT_CORRECT          = _T("%sTable OBJECTID=%d: Since the sequence number is not entered correctly, the processing can not be continued");
	LPCTSTR ERROR_DIRECTION_CODE_NOT_CORRECT           = _T("%sTable OBJECTID=%d: Since the direction code is not entered correctly, the processing can not be continued");
	LPCTSTR ERROR_SPLIT_ROAD_LINK_ACROSS_MESH          = _T("Since the split road link is across the mesh, the processing can not be continued.");
	LPCTSTR ERROR_SPLIT_ROAD_LINK_SHAPE_IS_IMPROPER    = _T("Since the split road link shape is improper, the link queue can not be edited");
	LPCTSTR ERROR_FAIL_COPY_EXTERNAL_RECORD            = _T("Failed to copy the external record.");
	LPCTSTR EXTERNAL_RECORD_COPY_FAILURE               = _T("External record copy failure");
	LPCTSTR ERROR_FAIL_DELETE_EXTERNAL_RECORD          = _T("Failed to delete the external record.");
	LPCTSTR EXTERNAL_RECORD_DELETE_FAILURE             = _T("External record delete failure");
	LPCTSTR CONFIRM_FILL_THIS_CLOSED_RING              = _T("Do you fill this closed ring?");
	LPCTSTR FILL_METHOD                                = _T("Fill method.");
	LPCTSTR CONFIRM_CHANGE_SHAPE_AND_EVADE_HEAP        = _T("Polygons are overlap in same layer.\nI change the shape of the existing feature, and do you evade heap?");
	LPCTSTR CREATING_POLYGON                           = _T("Creating polygon");
	LPCTSTR CANNOT_ADD_SHARED_SHAPE_POINT_TO_MOVABLE   = _T("Layer: %s OBJECTID=%d : You can not add a shared shape point because it is not a movable feature.");
	LPCTSTR RESTRUCTURING_OF_ATTRIBUTE_SLOPE           = _T("Restructuring of the shape point of the road structure attribute/slope");
#endif // SINDY_FOR_ENGLISH
}

#ifdef _DEBUG
void TargetItems_Trace( const CTargetItems& cItems, LPCTSTR lpcszHeader )
{
	for( CTargetItems::const_iterator it = cItems.begin(); it != cItems.end(); ++it )
	{
		ATLTRACE(_T("%s : TABLE[%s] OID[%d]\n"), lpcszHeader, AheGetFeatureClassName( it->GetFeature() ), it->GetOID() );
	}
}
#else
#define TargetItems_Trace(a,b) (void(0))
#endif // ifdef _DEBUG

IGeometryPtr TargetItems_MergeGeometry( const CTargetItems& cItems, bool bUnion = true )
{
	// CFeatures::GetShape()�̂قڃp�N���ł�
	// UNION��������ƁAUNION���1Unit�����������Ȃ��ꍇ�͏����Ă��܂��̂Œ��Ӂi���񍐁j�I�I
	IGeometryPtr ipRet;

	IGeometryCollectionPtr ipPointsCol, ipPolylinesCol, ipPolygonsCol;
	for( CTargetItems::const_iterator it = cItems.begin(); it != cItems.end(); ++it )
	{
		IGeometryPtr ipGeom = it->GetShapeCopy();
		if( ipGeom != NULL )
		{
			esriGeometryType emType = esriGeometryAny;
			ipGeom->get_GeometryType( &emType );
			switch( emType )
			{
				case esriGeometryBag:
				{
					IEnumGeometryPtr ipEnumGeom( ipGeom );
					ipEnumGeom->Reset();
					IGeometryPtr ipGeomPart;
					while( S_OK == ipEnumGeom->Next( &ipGeomPart ) )
					{
						esriGeometryType emTypePart = esriGeometryAny;
						ipGeomPart->get_GeometryType( &emTypePart );
						switch( emTypePart )
						{
							case esriGeometryMultipoint:
							case esriGeometryPoint:
								if( ipPointsCol == NULL )
									ipPointsCol.CreateInstance( CLSID_GeometryBag );
								ipPointsCol->AddGeometry( ipGeomPart );
								break;
							case esriGeometryPolyline:
								if( ipPolylinesCol == NULL )
									ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
								ipPolylinesCol->AddGeometry( ipGeomPart );
								break;
							case esriGeometryPolygon:
								if( ipPolygonsCol == NULL )
									ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
								ipPolygonsCol->AddGeometry( ipGeomPart );
								break;
							default:
								_ASSERTE(false);
								break;
						}
					}
					break;
				}
				case esriGeometryMultipoint:
				case esriGeometryPoint:
					if( ipPointsCol == NULL )
						ipPointsCol.CreateInstance( CLSID_GeometryBag );
					ipPointsCol->AddGeometry( ipGeom );
					break;
				case esriGeometryPolyline:
					if( ipPolylinesCol == NULL )
						ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
					ipPolylinesCol->AddGeometry( ipGeom );
					break;
				case esriGeometryPolygon:
					if( ipPolygonsCol == NULL )
						ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
					ipPolygonsCol->AddGeometry( ipGeom );
					break;
				default:
					_ASSERTE(false);
					break;
			}
		}
	}

	if( bUnion )
	{
		// ���ꂼ���Union����
		ITopologicalOperatorPtr ipPointsTopo, ipPolylinesTopo, ipPolygonsTopo;
		if( ipPointsCol )
		{
			ipPointsTopo.CreateInstance( CLSID_Multipoint );
			((IGeometryPtr)ipPointsTopo)->putref_SpatialReference( AheGetSpatialReference( ipPointsCol ) ); // ��ԎQ�ƕt�^
			ipPointsTopo->ConstructUnion( (IEnumGeometryPtr)ipPointsCol );
			ipPointsCol = ipPointsTopo;
		}
		if( ipPolylinesCol )
		{
			ipPolylinesTopo.CreateInstance( CLSID_Polyline );
			((IGeometryPtr)ipPolylinesTopo)->putref_SpatialReference( AheGetSpatialReference( ipPolylinesCol ) ); // ��ԎQ�ƕt�^
			ipPolylinesTopo->ConstructUnion( (IEnumGeometryPtr)ipPolylinesCol );
			ipPolylinesCol = ipPolylinesTopo;
		}
		if( ipPolygonsCol )
		{
			ipPolygonsTopo.CreateInstance( CLSID_Polygon );
			((IGeometryPtr)ipPolygonsTopo)->putref_SpatialReference( AheGetSpatialReference( ipPolygonsCol ) ); // ��ԎQ�ƕt�^
			ipPolygonsTopo->ConstructUnion( (IEnumGeometryPtr)ipPolygonsCol );
			ipPolygonsCol = ipPolygonsTopo;
		}
	}

	// �ǂꂩ������Ȃ���΂����Ԃ�l�Ƃ���
	bool bPoints = false, bPolylines = false, bPolygons = false;
	if( ( bPoints    = ( ipPointsCol != NULL && ipPolylinesCol == NULL && ipPolygonsCol == NULL ) ) ||
		( bPolylines = ( ipPointsCol == NULL && ipPolylinesCol != NULL && ipPolygonsCol == NULL ) ) ||
		( bPolygons  = ( ipPointsCol == NULL && ipPolylinesCol == NULL && ipPolygonsCol != NULL ) ) )
	{
		if( bPoints )
			ipRet = ipPointsCol;
		else if( bPolylines )
			ipRet = ipPolylinesCol;
		else if( bPolygons )
			ipRet = ipPolygonsCol;
	}
	else if( ipPointsCol != NULL || ipPolylinesCol != NULL || ipPolygonsCol != NULL )
	{
		IGeometryCollectionPtr ipGeomCol( CLSID_GeometryBag );
		if( ipPointsCol != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPointsCol );
		if( ipPolylinesCol != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolylinesCol );
		if( ipPolygonsCol != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolygonsCol );
		ipRet = ipGeomCol;
	}
	return ipRet;
}

bool operator < ( const LQDEF& c1, const LQDEF& c2 )
{
	return c1.seq < c2.seq;
}

bool operator == ( const LQDEF& c1, const LQDEF& c2 )
{
	return ( c1.oid == c2.oid && c1.kind == c2.kind );
}

CString GetTraceLQDEFString( const LQDEF& c )
{
	CString strName;
	switch( c.kind )
	{
		case sindyeRoadQueueDirguide  : strName = LINK_QUEUE_DIRGUIDE; break;
		case sindyeRoadQueueGuide     : strName = LINK_QUEUE_GUIDE; break;
		case sindyeRoadQueueVics      : strName = LINK_QUEUE_VICS;     break;
		case sindyeRoadQueueByway     : strName = LINK_QUEUE_BYWAY;   break;
		case sindyeRoadQueueTurnreg   : strName = LINK_QUEUE_TURNREG; break;
		case sindyeRoadQueueLane      : strName = LINK_QUEUE_LANE;       break;
		case sindyeRoadQueueRoute     : strName = LINK_QUEUE_ROUTE; break;
		case sindyeRoadQueueUTurn     : strName = LINK_QUEUE_UTURN; break;
		case sindyeRoadQueueInternavi : strName = LINK_QUEUE_INTERNAVI; break;
		case sindyeRoadQueueIPCVics   : strName = LINK_QUEUE_IPCVICS; break;
		case sindyeRoadQueueIntersection   : strName = LINK_QUEUE_INTERSECTION; break;
		default : strName = _T("UNKNOWN"); break;
	}
	CString strTrace;
	strTrace.Format(_T("LQDEF : OID[%12d] INFID[%12d] LINKID[%12d] SEQ[%3d] LAST[%5s] DIR[%s] KIND[%s]"), c.oid, c.infid, c.linkid, c.seq, ( c.last ) ? _T("TRUE") : _T("FALSE"), ( 1 == c.dir ) ? _T("��") : _T("�t"), strName );

	return strTrace;
}

CString GetTraceLQDEFString( const LQDEF& c, std::map<long, ROADDEF>& mapRoad )
{
	CString strName;
	switch( c.kind )
	{
		case sindyeRoadQueueDirguide  : strName = LINK_QUEUE_DIRGUIDE; break;
		case sindyeRoadQueueGuide     : strName = LINK_QUEUE_GUIDE; break;
		case sindyeRoadQueueVics      : strName = LINK_QUEUE_VICS;     break;
		case sindyeRoadQueueByway     : strName = LINK_QUEUE_BYWAY;   break;
		case sindyeRoadQueueTurnreg   : strName = LINK_QUEUE_TURNREG; break;
		case sindyeRoadQueueLane      : strName = LINK_QUEUE_LANE;       break;
		case sindyeRoadQueueRoute     : strName = LINK_QUEUE_ROUTE; break;
		case sindyeRoadQueueUTurn     : strName = LINK_QUEUE_UTURN; break;
		case sindyeRoadQueueInternavi : strName = LINK_QUEUE_INTERNAVI; break;
		case sindyeRoadQueueIPCVics   : strName = LINK_QUEUE_IPCVICS; break;
		case sindyeRoadQueueIntersection   : strName = LINK_QUEUE_INTERSECTION; break;
		default : strName = _T("UNKNOWN"); break;
	}

	double dx1 = -1.0, dy1 = -1.0, dx2 = -1.0, dy2 = -1.0;
	IPointPtr ipFrom, ipTo;
	std::map<long,ROADDEF>::const_iterator it = mapRoad.find( c.linkid );
	if( it != mapRoad.end() )
	{
		IPolylinePtr ipRoadGeom( it->second.shape );
		if( ipRoadGeom )
		{
			ipRoadGeom->get_FromPoint( &ipFrom );
			ipRoadGeom->get_ToPoint( &ipTo );
			if( 2 == c.dir )
			{
				IPointPtr ipTmp( ipFrom );
				ipFrom = ipTo;
				ipTo = ipTmp;
			}
			ipFrom->QueryCoords( &dx1, &dy1 );
			ipTo->QueryCoords( &dx2, &dy2 );
		}
	}

	CString strTrace;
	CString str2;
	str2.Format(_T("%.15lf, %.15lf"), dx1, dy1 );

	CString str3;
	str3.Format(_T("%.15lf, %.15lf"), dx1, dy1 );

	strTrace.Format(_T("LQDEF : OID[%12d] INFID[%12d] LINKID[%12d] SEQ[%3d] LAST[%5s] DIR[%s] KIND[%s] (%.15lf, %.15lf) - (%.15lf, %.15lf)"), c.oid, c.infid, c.linkid, c.seq, ( c.last ) ? _T("TRUE") : _T("FALSE"), ( 1 == c.dir ) ? _T("��") : _T("�t"), strName, dx1, dy1, dx2, dy2 );

	return strTrace;
}

void TraceLQDEF( const LQDEF& c )
{
	ATLTRACE(_T("%s\n"), GetTraceLQDEFString( c ) );
}

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CEditHelper::CEditHelper()
{

}

CEditHelper::~CEditHelper()
{

}

// ���H�ɂ�铹�H�̃X�v���b�g
BOOL CEditHelper::SplitRoadByRoadAndMesh( CTargetItems& cTargetFeatures, CTargetItems& cNewFeatures, BOOL bSplitAll, IPointPtr ipTakeOverPoint/* = NULL*/, CTargetItems* pReferenceNodes/* = NULL*/, IPointCollection* ipSplitPoints/* = NULL*/ )
{
	CErrObjContainer errs;
	return SplitRoadByRoadAndMesh( cTargetFeatures, cNewFeatures, bSplitAll, errs, ipTakeOverPoint, pReferenceNodes, ipSplitPoints );
}
BOOL CEditHelper::SplitRoadByRoadAndMesh( CTargetItems& cTargetFeatures, CTargetItems& cNewFeatures, BOOL bSplitAll, CErrObjContainer& errs, IPointPtr ipTakeOverPoint/* = NULL*/, CTargetItems* pReferenceNodes/* = NULL*/, IPointCollection* ipSplitPoints/* = NULL*/ )
{
#ifdef DEBUG
        DWORD DWSTART = GetTickCount();
#endif

	BOOL bRet = TRUE;	// �Ԓl

	TargetItems_Trace( cTargetFeatures, _T("SplitRoadByRoadAndMesh : �J�n�icTargetFeatures�j") );

	// �n���ꂽ�t�B�[�`���Q�ɂ͊��ɗ��������Ă���i�͂��j�Ȃ̂ŁA����t�^���O�ΏۂƂ���
	CTargetItems cNoSetModifyDataFeatures = cTargetFeatures;

	// �ҏW�ΏیQ�̒����瓹�H�A�X�΁A�����N�������ƃm�[�h��I��
	CTargetItems cRoads, cNodes, cRoadsAndSegAndGrads;
	CString strSplitLayers;
	for( CTargetItems::const_iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
	{
		if( AheIsRelateWithNetwork( it->GetFeature() ) ) // �l�b�g���[�N�֌W�t�B�[�`����������擾�ibug 5486�j
		{
			strSplitLayers = AheGetNetworkSplitTarget( it->GetFeatureClassName() );
			break;
		}
	}
	// �X�΁A�����N�������̒P�ƈړ��ł��X�v���b�g�������ł���悤��
	cTargetFeatures.GetFeatures( cRoadsAndSegAndGrads, strSplitLayers );
	cTargetFeatures.GetNetworkLinkFeatures( cRoads );
	cTargetFeatures.GetNetworkNodeFeatures( cNodes );

	IGeometryPtr ipSearchGeometry;
	CTargetItems cDstRoadsAndSegAndGrads;
	IPointCollectionPtr ipCrossPointCol;
	// [bug 2378] �m�[�h�̂ݓn���ꂽ�Ƃ��̏������̂̂܂܂������̂œ��H�⃊���N��������
	//            �I��������ʏ�̂��̂Ɠ����ɂȂ�悤�ɏC���B�������A�����_�͌��܂���
	//            ����̂ŁA�I��������ɖ��ʂɒǉ����ꂽ�|�C���g��߂��Ă��
	BOOL bIsNodeOnly = FALSE;
	IPointCollectionPtr ipCrossPointColNodeOnly;
	if ( cNodes.size() == 1 && cRoadsAndSegAndGrads.size() <= 0 )
	{
		bIsNodeOnly = TRUE;
		ipSearchGeometry = cNodes.begin()->GetShapeCopy();
		SelectByShapeFromLayers( ipSearchGeometry, cRoadsAndSegAndGrads, strSplitLayers, esriSpatialRelIntersects, NULL, NULL, TRUE );
		ipCrossPointColNodeOnly.CreateInstance( CLSID_Multipoint );
		((IGeometryPtr)ipCrossPointColNodeOnly)->putref_SpatialReference( AheGetSpatialReference( ipSearchGeometry ) ); // ��ԎQ�ƕt�^
		ipCrossPointColNodeOnly->AddPoint( (IPointPtr)ipSearchGeometry );
		cTargetFeatures.GetNetworkLinkFeatures( cRoads );
	}
	if ( cRoadsAndSegAndGrads.size() > 0 )
	{
//		// ���H�S�̂̃G���x���[�v���擾 <- �x���I�I
//		IEnvelopePtr ipRoadEnv;
//		cRoads.GetEnvelope( &ipRoadEnv );
		// ���H�S�̂� Union ���擾
		ipSearchGeometry = cRoadsAndSegAndGrads.GetUnion();
		if( ipSearchGeometry == NULL )
		{
			ATLASSERT( ipSearchGeometry != NULL );
			return FALSE;
		}
		((ITopologicalOperatorPtr)ipSearchGeometry)->Simplify();

/*#ifdef DEBUG
		// �����O�̃��j�I���`����n�C���C�g
		AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipSearchGeometry, RGB( 20, 50, 100 ), 2, esriSLSSolid, TRUE, FALSE );
		ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �����O�̃��j�I���`����n�C���C�g�i���F�j\n") );
		IGraphicsContainerPtr ipContainer;
		m_cArcHelper.GetActiveView( FALSE )->get_GraphicsContainer( &ipContainer );
		AheAddLineElement( ipContainer, ipSearchGeometry, RGB(20,50,100), 2, esriSLSSolid );
#endif*/
		//SetDomain();

		// �����|�����C���̒[�_��TOUCH�Ȋ֌W�̃t�B�[�`�����I���ł��Ȃ��ꍇ�L��I�I <- ����
		// ���傤���Ȃ��̂ŁA�ق�̏����o�b�t�@������Č�������B
		// ����ɂ��A�]�v�Ȃ��̂��I������鋰��L��B
		IGeometryPtr ipSearchGeometry2;
		IGeoDatasetPtr ipClass( cRoadsAndSegAndGrads.GetNetworkLinkFeatureClass() );
		DOUBLE dBuffer = 0.0000005;

		if( ipClass != NULL )
		{
			ISpatialReferencePtr ipRoadRef;
			ipClass->get_SpatialReference( &ipRoadRef );
			//dBuffer = AheGetXYDomain1UnitLength( ipRoadRef ) * (DOUBLE)4.0;
			dBuffer = AheGetXYDomain1UnitLength( ipRoadRef ) * (DOUBLE)10.0;
			ATLTRACE(_T("CEditHelper::SplitRoadByRoadAndMesh : ���H�I���o�b�t�@�F%.20lf\n"), dBuffer );
		}
			
		((ITopologicalOperatorPtr)ipSearchGeometry)->Buffer( dBuffer, &ipSearchGeometry2 );
		//AheGeometry2Polygon( ipSearchGeometry, 0.0000005, &ipSearchGeometry2 );
#ifdef DEBUG
		// ���H�����G���A�n�C���C�g
		//AheAddPolygonElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipSearchGeometry2, RGB(100, 0, 100), esriSFSSolid );
#endif

		// �܂��͓��H��������
		CTargetItems cSelectedRoads;
		SelectByShapeFromFeatureClass( ipSearchGeometry2, cSelectedRoads, ipClass );
		// cSelectRoads �̒����� cRoads �ɂ�����̂����O
		cSelectedRoads -= cRoads;

#ifdef DEBUG
		for( CTargetItems::iterator itTmp = cSelectedRoads.begin(); itTmp != cSelectedRoads.end(); itTmp++ )
			ATLTRACE(_T("CEditHelper::SplitRoadByRoadAndMesh : �����ł����t�B�[�`��2 %s OID=%d\n"), itTmp->GetFeatureClassName(), itTmp->GetOID() );
#endif

		// �����|�C���g�擾�i�m�[�h�쐬���_�j
		// [bug 1889] �n���ꂽ�t�B�[�`���ɓ��H�����N�����݂��Ȃ���΂��Ȃ��B���ƁA�\���_�t�^��d���`�F�b�N���ł��Ȃ��Ȃ�
		CTargetItems cDeleteSelectedRoads;
		if( ! cRoads.empty() )
		{
			IPointCollectionPtr ipCrossPointColTmp = GetCrossOrTouchPoints( cSelectedRoads, cRoads, ( bSplitAll ? 2 : 1 ), &cDeleteSelectedRoads );

			// �����_���n����Ă���ꍇ�͂���̂ݎg�p����ibug 4882�j
			if( ipSplitPoints )
			{
				ITopologicalOperatorPtr ipTopo( ipSplitPoints );
				if( ipTopo != NULL )
				{
					IGeometryPtr ipIntersectGeom;
					ipTopo->Intersect( (IGeometryPtr)ipCrossPointColTmp, esriGeometry0Dimension, &ipIntersectGeom );
					ipCrossPointCol = ipIntersectGeom;
				}
			}
			else
				ipCrossPointCol = ipCrossPointColTmp;

			// �֌W�Ȃ��������̂� cSelectedRoads ���珜�O�i�X�s�[�h�A�b�v�ɂȂ�H�j
#ifdef DEBUG
			for( CTargetItems::iterator itDel = cDeleteSelectedRoads.begin(); itDel != cDeleteSelectedRoads.end(); itDel++ ) 
				ATLTRACE(_T("CEditHelper::SplitRoadByRoadAndMesh() : �������Ȃ������̂ŏ��O %s OBJECTID=%d\n"), itDel->GetFeatureClassName(), itDel->GetOID() );
#endif
			cSelectedRoads -= cDeleteSelectedRoads;
		}
/*#ifdef DEBUG
		//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipCrossPointCol, RGB(100, 0, 100), 8, esriSMSCircle );
#endif*/
		// ���ɁA�n���ꂽ���H�{�����ł������H�Ń����N�������E�X�΂���������iROAD_ID�̍X�V�̂��߁j
		ipSearchGeometry = ipSearchGeometry2 = NULL;
		CTargetItems cTargetAndSelectedRoad = cRoads;
		for( CTargetItems::iterator it = cSelectedRoads.begin(); it != cSelectedRoads.end(); it++ ) cTargetAndSelectedRoad.push_back( *it );
		ipSearchGeometry = cTargetAndSelectedRoad.GetUnion();
		if( ipSearchGeometry != NULL )
		{
			((ITopologicalOperatorPtr)ipSearchGeometry)->Simplify();
			((ITopologicalOperatorPtr)ipSearchGeometry)->Buffer( dBuffer, &ipSearchGeometry2 );
			CString strSegAttrAndGradLayers;
			strSegAttrAndGradLayers.Format(_T("|%s|%s|"), SEGMENTATTR_TABLE_NAME, GRAD_TABLE_NAME );
			CTargetItems cSegAttrAndGrads2;
			SelectByShapeFromLayers( ipSearchGeometry2, cSegAttrAndGrads2, strSegAttrAndGradLayers, esriSpatialRelIntersects, NULL, NULL, TRUE );
			// �]�v�ȃ����N�������E�X�΂��I������Ă���̂ŁA�n���ꂽ���H�{�����ł������H�ɒ[�_���ڂ��Ă��邩�ǂ����`�F�b�N
			// ������ Bug 1612 �̂悤�Ȃ��ƂɂȂ�̂ŁA���̒[�_���m���ڂ��Ă��邩�ǂ������`�F�b�N����K�v�A��
			// ���ł� Bug 1754 �̂悤�ȃP�[�X�͖���
			// ���������A�����̃����N��������X�΂͌X�̓��H�����N���Ɏ��܂��Ă�͂��Ȃ̂ŁA���[�͈�̓��H�����N�Ŋ�������͂�
			CTargetItems::iterator itSegAttrAndGrads2 = cSegAttrAndGrads2.begin();
			while( itSegAttrAndGrads2 != cSegAttrAndGrads2.end() )
			{
				IPointPtr ipSegAttrAndGradsFromPoint = itSegAttrAndGrads2->GetFromPoint(), ipSegAttrAndGradsToPoint = itSegAttrAndGrads2->GetToPoint();
				IGeometryPtr ipFromRoadGeom, ipToRoadGeom;

				BOOL bFind = FALSE;
				for( CTargetItems::iterator itTargetAndSelectedRoad = cTargetAndSelectedRoad.begin(); itTargetAndSelectedRoad != cTargetAndSelectedRoad.end(); itTargetAndSelectedRoad++ )
				{
					// [bug 2924] ���H�����N��P�ƕҏW����ہA�����N��������X�΂̒[�_�������ړ������Ƃ��̓����N�������E�X�΂������Ă��Ȃ�
					// �Ȃ̂ŁA�����ł����Ɗ֘A���郊���N�������ƌX�΂͕ҏW�Ώۂɂ���ׂ�
					if( itSegAttrAndGrads2->GetAttribute( _T("LINK_ID") ) == CComVariant( itTargetAndSelectedRoad->GetOID() ) )
					{ 
						itSegAttrAndGrads2++;
						bFind = TRUE;
						break;
					}

					if( ( AhePointIsOnGeometry( itTargetAndSelectedRoad->GetShape(), ipSegAttrAndGradsFromPoint, itTargetAndSelectedRoad->GetSpatialReference() ) == TRUE &&
						  AhePointIsOnGeometry( itTargetAndSelectedRoad->GetShape(), ipSegAttrAndGradsToPoint,   itTargetAndSelectedRoad->GetSpatialReference() ) == TRUE ) ||
						  AheIsEqual( itTargetAndSelectedRoad->GetShape(), itSegAttrAndGrads2->GetShape(), itTargetAndSelectedRoad->GetSpatialReference() ) == TRUE )
					{
						itSegAttrAndGrads2++;
						bFind = TRUE;
						break;
					}
				}
				if( bFind == FALSE ) itSegAttrAndGrads2 = cSegAttrAndGrads2.erase( itSegAttrAndGrads2 );
			}

			for( CTargetItems::iterator it = cSegAttrAndGrads2.begin(); it != cSegAttrAndGrads2.end(); it++ ) cDstRoadsAndSegAndGrads.push_back( *it );
			for( CTargetItems::iterator it = cSelectedRoads.begin(); it != cSelectedRoads.end(); it++ ) cDstRoadsAndSegAndGrads.push_back( *it );
		}
	} 
	// �m�[�h�݂̂̂Ƃ��́A�����Ńm�[�h�̃|�C���g�݂̂ɂ���
	if( bIsNodeOnly == TRUE ) ipCrossPointCol = ipCrossPointColNodeOnly;	// ����ւ�

	// SketchTool �ŃZ�O�����g�I�[�o�[���b�v�ō쐬�����ꍇ�ɂȂ����X���[���Ă��܂��i�����|�C���g���擾�ł��Ȃ��j�̂ŁA������
	// �Z�O�����g�I�[�o�[���b�v�`�F�b�N������
	CString strErrMsg;
	CTargetItems cTestTargets = cTargetFeatures;
	cDstRoadsAndSegAndGrads.GetFeatures( cTestTargets, strSplitLayers, TRUE );
#ifdef _DEBUG
	cTestTargets.Trace( _T("IsSafeGeometries�O") );
#endif // ifdef _DEBUG
	cTestTargets.IsSafeGeometries( errs, TRUE, GetRule() );
	if( errs.GetMaxLogLevel() == sindyeLogLevel_ERR )
	{
		AheMessageBox( errs.ToString(), ERROR_FAILED_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
		return FALSE;
	}

	// ���b�V���ł̌����|�C���g�擾
	for( CTargetItems::iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); it++ )
	{
		// ���H�����N���ǂ������`�F�b�N
		// 2�����b�V���Ő؂�t�B�[�`���͓��H�����N�݂̂ɂ��Ȃ��ƌ`��ɕs�������o��iBug 1637�j
		// �����^�]�}�b�v�̃����N�̓��b�V�����E�ŃX�v���b�g���Ȃ�
		if( AheIsNetworkLink( it->GetFeatureClassName() ) 
			&& (sindyeNetworkLane != AheGetNetWorkType(it->GetFeatureClassName()))
			&& (sindyeNetworkUpDown != AheGetNetWorkType(it->GetFeatureClassName()))
			&& (sindyeNetworkCompart != AheGetNetWorkType(it->GetFeatureClassName()))
			&& (sindyeNetworkBorder != AheGetNetWorkType(it->GetFeatureClassName())))
		{
			CMesh mesh;
			mesh.SetSpatialReference( it->GetSpatialReference() );
			IPointCollectionPtr ipPointsGeometry = mesh.GetIntersectPointsByShape( it->GetShape() );
			LONG lPointCount = -1;
			if( ipPointsGeometry != NULL )
			{
				ipPointsGeometry->get_PointCount( &lPointCount );
				for( LONG i = 0; i < lPointCount; i++ )
				{
					IPointPtr ipPoint;
					ipPointsGeometry->get_Point( i, &ipPoint );
					if( ipPoint != NULL )
					{
						if( ipCrossPointCol == NULL )
						{
							ipCrossPointCol.CreateInstance( CLSID_Multipoint );
							((IGeometryPtr)ipCrossPointCol)->putref_SpatialReference( AheGetSpatialReference( ipPoint ) ); // ��ԎQ�ƕt�^
						}

						ipCrossPointCol->AddPoint( ipPoint );
					}
				}
			}
		}
	}
#ifdef DEBUG
	// �����_�iSimplify�O�j�n�C���C�g
	AheDrawPoints( GetFocusMapScreenDisplay(), ipCrossPointCol, FALSE, 8, 20, 50, 200, esriSMSCircle, FALSE, TRUE );
	ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �X�v���b�^�n�C���C�g�i�H�F�j\n") );
//	AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipCrossPointCol, RGB(20,50,200), 8, esriSMSCircle );
#endif

	AheRemoveAdjoiningPoint( ipCrossPointCol );

	// �����N�������y�ьX�΂̓��H�����N�Ƃ̌`��̐����������̂ŁA�K���ʂ�
	// �����Ώۂ� cDstRoadsAndSegAndGrads ���܂߂�
	for( CTargetItems::iterator itTmp = cDstRoadsAndSegAndGrads.begin(); itTmp != cDstRoadsAndSegAndGrads.end(); itTmp++ )
	{
#ifdef DEBUG
		ATLTRACE(_T("CEditHelper::SplitRoadByRoadAndMesh : �����ł����t�B�[�`�� %s OID=%d\n"), itTmp->GetFeatureClassName(), itTmp->GetOID() );
#endif
		cTargetFeatures.push_back( *itTmp );
	}
#ifdef DEBUG
    ATLTRACE (_T("������ CEditHelper::SplitRoadByRoadAndMesh �X�v���b�g�O %lf ������\n"), (GetTickCount() - DWSTART)/1000.0);
#endif
#ifdef DEBUG
	// �X�v���b�^�n�C���C�g
	//IElementPtr ipElement( AheAddLineElement( ipContainer, ipSearchGeom, RGB(0,255,0), 2, esriSLSSolid ) );
	AheDrawPoints( GetFocusMapScreenDisplay(), ipCrossPointCol, FALSE, 8, 200, 50, 20, esriSMSCircle, FALSE, TRUE );
	// �����_�n�C���C�g
	ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �X�v���b�^�n�C���C�g�i�����K�F�j\n") );
	//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipCrossPointCol, RGB(200,50,20), 8, esriSMSCircle );
#endif

	bRet = SplitFeaturesByGeometry( cTargetFeatures, strSplitLayers, ipCrossPointCol, cNoSetModifyDataFeatures, cNewFeatures, ipTakeOverPoint, 2, pReferenceNodes );

#ifdef DEBUG
    ATLTRACE (_T("������ CEditHelper::SplitRoadByRoadAndMesh %lf ������\n"), (GetTickCount() - DWSTART)/1000.0);
#endif

	return bRet;
}

BOOL CEditHelper::SplitFeaturesByGeometry( CTargetItems& cTargetFeatures, LPCTSTR lpcszSplitLayers, IGeometryPtr ipGeom, CTargetItems& cNoSetModifyDataFeatures, CTargetItems& cNewFeatures,/* CTargetItems& cModifyFeatures,*/ IPointPtr ipTakeOverPoint/* = NULL*/, INT iSplitMode/* = 2*//*, BOOL bIsSplitByMesh*//* = FALSE*//*, BOOL bSearchNodes*//* = TRUE*/, CTargetItems* pReferenceNodes/* = NULL*/ )
{
	using namespace sindy::schema;
#ifdef _DEBUG
	TargetItems_Trace( cTargetFeatures, _T("cTargetFeatures") );
	for( CTargetItems::iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
		AheTraceGeometry( it->GetShape(), _T("CEditHelper::SplitFeaturesByGeometry() START") );
	TargetItems_Trace( cNoSetModifyDataFeatures, _T("cNoSetModifyDataFeatures") );
	TargetItems_Trace( cNewFeatures, _T("cNewFeatures") );
#endif // ifdef _DEBUG

	CTargetItems cTakeOveredFeatures;                    //!< �������ꂽ���ʌ���ID�������p�����t�B�[�`�����i�[
	CTargetItems cModifyNodeKindFeatures;                //!< �m�[�h��ʂ��Đݒ肵�����̂ŁA���� cNewFeatures �ɂ� cTargetFeatures �ɂ����݂��Ȃ����̂�����

	std::list<ROWDEF> listLQRows;	// �X�v���b�g�̍ۂɌ�������LQ���R�[�h�i�ŏI�`�F�b�N�p�j

	if( ipGeom )
	{
		// �Ώۃt�B�[�`���Q�𕪊�����
		if(FALSE==GetSplitResult(lpcszSplitLayers, ipGeom, cNoSetModifyDataFeatures,
			cModifyNodeKindFeatures, ipTakeOverPoint, iSplitMode, cTakeOveredFeatures,
			pReferenceNodes, cNewFeatures, cTargetFeatures, listLQRows))
			return FALSE;
	}

	// �����N�������y�ьX�΂����铹�H�����N��T���āA�֘AID�t�B�[���h�ɓ��H�����N��OBJECTID���Z�b�g
	if(FALSE==ResetCommonFeatures(cNewFeatures, cTargetFeatures, cNoSetModifyDataFeatures, cTakeOveredFeatures))
		return FALSE;

	// cNewFeatures �� cTargetFeatures �ɂ��킹����
	//cTargetFeatures += cNewFeatures;
	// cModifyNodeKindFeatures �� cTargetFeatures �ɂ��킹����
	cTargetFeatures += cModifyNodeKindFeatures;

/*#ifdef DEBUG
	for( CTargetItems::const_iterator itAll = cTargetFeatures.begin(); itAll != cTargetFeatures.end(); ++itAll )
		ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : CTargetItems�̒��g�m�F : LAYER=%s OBJECTID=%d\n"), itAll->GetFeatureClassName(), itAll->GetOID() );
#endif*/
	// 
#ifdef _DEBUG_FOR_2059
	{
		CTargetItems cLastRoads;
		cTargetFeatures.GetRoadFeatures( cLastRoads );
		cNewFeatures.GetRoadFeatures( cLastRoads, TRUE );
		// SnapTo...�O�̓��H�`��̃g���[�X���m�F
		for( CTargetItems::iterator itLastRoads = cLastRoads.begin(); itLastRoads != cLastRoads.end(); itLastRoads++ )
		{
			CString strMsg;
			strMsg.Format(_T("SnapTo...�O�̓��H�`��[%d]"), itLastRoads->GetOID() );
			AheTraceGeometry( itLastRoads->GetShape(), strMsg );
		}
	}
#endif // ifdef _DEBUG_FOR_2059
	// [bug 2059] SnapToSpatialReference������ƂȂ������H�����N�̕��������W�������H�H
	// ����A���m�ɂ�KnownSimple��VARIANT_TRUE�̏ꍇ��SnapToSpatialRefenrece������Ƃ����i���������l�ɂȂ�H�j�̂��H
	// �������ASnapTo..���͂����Ă�CTargetItems::Simplify()��ʂ��Ƃ���ς肸�ꂽ�B
	// �����ł�SimplifyNetwork������Ă���B���������Ă�����������H
	// �Ƃ肠�����A�ǂ�����O�����ŃG���[���o�Ȃ��Ȃ����̂ł����OK�Ƃ��邪�A
	// ����͂����ꂿ���ƒ������Ȃ��Ƃ����Ȃ��B
	// test/bug2059/bug2059-2.mxd�Ƀe�X�g��������Ă������̂ŁA�����ROAD_LINK[6,7]���ׂ��悤�ɓ��H�����N��
	// ����ĕ�������ƁA���̃R�[�h�𐶂�������Ԃł�2���1��̊����ŃG���[���ł��B
	// ���ƂŌ��؂��邱�� by furukawa�B
	//cTargetFeatures.SnapToSpatialReference();
	//cTargetFeatures.Simplify();
#ifdef _DEBUG_FOR_2059
	{
		CTargetItems cLastRoads;
		cTargetFeatures.GetRoadFeatures( cLastRoads );
		cNewFeatures.GetRoadFeatures( cLastRoads, TRUE );
		// SnapTo...��̓��H�`��̃g���[�X���m�F
		for( CTargetItems::iterator itLastRoads = cLastRoads.begin(); itLastRoads != cLastRoads.end(); itLastRoads++ )
		{
			CString strMsg;
			strMsg.Format(_T("SnapTo...��̓��H�`��[%d]"), itLastRoads->GetOID() );
			AheTraceGeometry( itLastRoads->GetShape(), strMsg );
		}
	}
#endif // ifdef _DEBUG_FOR_2059

	// �������ꂽ���ʂł����`��ɖ�肪�Ȃ����`�F�b�N
	if(FALSE==CheckNWStructure(cNewFeatures, cTargetFeatures, listLQRows))
		return FALSE;

	return TRUE;
}

_ISetPtr CEditHelper::SplitGeometryByGeometry( IGeometryPtr ipBaseGeometry, IGeometryPtr ipSplitterGeometry, INT iSplitMode/* = 2*/, BOOL bIsRoadFeature/* = FALSE */, IGeometryPtr ipSplittedPoints/* = NULL */, BOOL bTest/* = FALSE*/ )
{
	_ISetPtr ipSplittedGeometrySet;	// ������̌`��ێ��p

	// �W�I���g���^�C�v�擾
	esriGeometryType enumBaseGeomType, enumSplitterGeomType;
	ipBaseGeometry->get_GeometryType( &enumBaseGeomType );
	ipSplitterGeometry->get_GeometryType( &enumSplitterGeomType );

	switch( enumBaseGeomType )
	{
		case esriGeometryPolyline:
			switch( enumSplitterGeomType )
			{
				case esriGeometryPoint:
				case esriGeometryMultipoint:
					ipSplittedGeometrySet = SplitPolylineByPoints( ipBaseGeometry, ipSplitterGeometry, ( bIsRoadFeature == TRUE ) ? ipSplittedPoints : NULL );
					break;
				case esriGeometryPolyline:
					ipSplittedGeometrySet = SplitPolylineByPoints( ipBaseGeometry, AheGetCrossOrTouchPoints( ipBaseGeometry, ipSplitterGeometry, iSplitMode ), ( bIsRoadFeature == TRUE ) ? ipSplittedPoints : NULL );
					break;
				case esriGeometryPolygon:
					ipSplittedGeometrySet = SplitPolylineByPoints( ipBaseGeometry, AheGetCrossOrTouchPoints( ipBaseGeometry, ipSplitterGeometry, iSplitMode ), ( bIsRoadFeature == TRUE ) ? ipSplittedPoints : NULL );
					break;
				default:
					break;
			}
			break;
		case esriGeometryPolygon:
			switch( enumSplitterGeomType )
			{
				case esriGeometryPolyline:
					ipSplittedGeometrySet = SplitPolygonByPolyline( ipBaseGeometry, ipSplitterGeometry, bTest );
					break;
/*				case esriGeometryPolygon:
					ipSplittedGeometrySet = SplitPolygonByPolygon( ipBaseGeometry, ipSplitterGeometry, iSplitMode );
					break;*/
				default:
					break;
			}
			break;
		default:
			break;
	}

	// ��������Ă��Ȃ��Ȃ� NULL ��Ԃ�
	LONG lGeomCount = -1;
	if( ipSplittedGeometrySet != NULL )
	{
		ipSplittedGeometrySet->get_Count( &lGeomCount );
		if( lGeomCount < 1 ) return NULL;
	}

	return ipSplittedGeometrySet;
}
_ISetPtr CEditHelper::SplitPolylineByPoints( IPolylinePtr ipPolyline, IGeometryPtr ipSplitterPoints, IPointCollectionPtr ipSplittedPoints/* = NULL*/ )
{
	if( ipPolyline == NULL || ipSplitterPoints == NULL ) return NULL;

	esriGeometryType enumSplitterPointsType;
	IPointCollectionPtr ipPointCol( ipSplitterPoints );

	// ipSplitterPoints �̃W�I���g���^�C�v���|�C���g�̏ꍇ�A IPointCollection �ɂ͕ϊ��ł��Ȃ�
	ipSplitterPoints->get_GeometryType( &enumSplitterPointsType );
	if( ( ! ipPointCol ) && enumSplitterPointsType == esriGeometryPoint )
	{
		ipPointCol.CreateInstance( CLSID_Multipoint );
		((IGeometryPtr)ipPointCol)->putref_SpatialReference( AheGetSpatialReference( ipSplitterPoints ) );
		ipPointCol->AddPoint( (IPointPtr)ipSplitterPoints );
	}
	IZAwarePtr ipZAware(ipPolyline);
	VARIANT_BOOL bZAware = VARIANT_FALSE;
	if(SUCCEEDED(ipZAware->get_ZAware(&bZAware)) && bZAware){
		long pointCount = -1;
		ipPointCol->get_PointCount(&pointCount);
		for(long i=0; i<pointCount; ++i){
			IPointPtr ipOrgPoint;
			ipPointCol->get_Point(i, &ipOrgPoint);
			if(!ipOrgPoint)
				continue;

			IZAwarePtr(ipOrgPoint)->put_ZAware(VARIANT_TRUE);

			double distAlong = 0, distFrom = 0;
			VARIANT_BOOL rightSide = VARIANT_FALSE;
			IPointPtr ipSplitPoint(CLSID_Point);
			ipSplitPoint->putref_SpatialReference(AheGetSpatialReference(ipPolyline));
			if(SUCCEEDED(IPolycurve2Ptr(ipPolyline)->QueryPointAndDistance(esriNoExtension, ipOrgPoint, VARIANT_TRUE, ipSplitPoint, &distAlong, &distFrom, &rightSide))){
				double zVal = 0;
				if(SUCCEEDED(ipSplitPoint->get_Z(&zVal))){
					ipOrgPoint->put_Z(zVal);
				}
			}
		}
	}

	// ipSplitterPoints �̃|�C���g����0�̏ꍇ�̓��^�[��
	LONG lCount = 0;
	ipPointCol->get_PointCount( &lCount );
	if( lCount < 1 ) return NULL;

	// ���̌`��̒n���Q�Ǝ擾
	ISpatialReferencePtr ipSpRel;
	ipPolyline->get_SpatialReference( &ipSpRel );

	// ���̌`��R�s�[
	IPolylinePtr ipCopyPolyline( AheGetClone( ipPolyline ) );
//	AheForceSimplify( ipCopyPolyline ); <- �]�v�Ȃ��Ƃ͂��Ȃ� 2003/03/14 furukawa

	// ���̌`��� FROM�ATO �|�C���g���L���i�X�v���b�g��̌������X�v���b�g�O�ƈ�v�����邽�߁j
	IPointPtr ipFromPoint, ipToPoint;
	ipCopyPolyline->get_FromPoint( &ipFromPoint );
	ipCopyPolyline->get_ToPoint( &ipToPoint );
//	AheSnapToSpatialReference( ipFromPoint, ipSpRel );
//	AheSnapToSpatialReference( ipToPoint,   ipSpRel );

// ��񂸂X�v���b�g���Ă����ƁA�擾������_�ƃ��C��������Ă��܂��̂ł���
//	// �X�v���b�g
//	BOOL bIsSplitted = FALSE;
//	for( LONG i = 0; i < lCount; i++ )
//	{
//		IPointPtr ipPoint;
//
//		ipPointCol->get_Point( i, &ipPoint );
//		if( ipPoint == NULL )
//		{
//			ATLASSERT( ipPoint != NULL );
//			continue;
//		}
////		AheSnapToSpatialReference( ipPoint, ipSpRel );
//
//		// �|�C���g���|�����C���Ɋ܂܂�Ă��Ȃ���΁Acontinue
//		if( AheIsWithin( ipPoint, ipCopyPolyline/*, ipSpRel*/ ) == FALSE ) continue;
//
//		VARIANT_BOOL vaIsSplitted = VARIANT_FALSE;
//		LONG lNewPartIndex = -1, lNewSegmentIndex = -1, lBeforeGeomCount = -1;
//		((IGeometryCollectionPtr)ipCopyPolyline)->get_GeometryCount( &lBeforeGeomCount );
//		ipCopyPolyline->SplitAtPoint( ipPoint, VARIANT_FALSE, VARIANT_TRUE, &vaIsSplitted, &lNewPartIndex, &lNewSegmentIndex );
//		ATLTRACE(_T("CEditHelper::SplitPolylineByPoints() : �\���_�ǉ��F SPLIT=%s NewPartIndex=%d NewSegmentIndex=%d\n"), (vaIsSplitted == VARIANT_TRUE ? _T("TRUE") : _T("FALSE")), lNewPartIndex, lNewSegmentIndex );
//
//		if( vaIsSplitted == VARIANT_TRUE ) 
//		{
///*			// �\���_��0�̃p�[�g���ł���ꍇ������̂ŁA������Simplify()���ăp�[�g�����`�F�b�N
//			AheForceSimplify( ipCopyPolyline );
//
//			LONG lAfterGeomCount = -1;
//			((IGeometryCollectionPtr)ipCopyPolyline)->get_GeometryCount( &lAfterGeomCount );
//			if( lBeforeGeomCount < lAfterGeomCount )
//			{*/
//				if( ipSplittedPoints != NULL )
//					ipSplittedPoints->AddPoint( ipPoint );	// �����_���L��
//
//				bIsSplitted = TRUE;
///*			}
//			else
//				ATLTRACE(_T("CEditHelper::SplitPolylineByPoints() : �\���_�ǉ����ʁA�����p�[�g���ł����悤�ł��B�����p�[�g�� Simplify() �ɂ��폜����܂����B\n") );*/
//		}
//	}

	BOOL bIsSplitted = FALSE;

	IEnumVertexPtr ipEnumVertex;

	ipPointCol->get_EnumVertices( &ipEnumVertex );

	if ( ipEnumVertex ) {
		ipEnumVertex->Reset();

		IPolycurve2Ptr ipPolycurve2 = ipCopyPolyline;

		if ( ipPolycurve2 ) {
			// ��ԎQ�ƍŏ������擾
			DOUBLE dTorelance = AheGetXYDomain1UnitLength( ipSpRel );

			IEnumSplitPointPtr ipEnumSplit;

			// �X�v���b�g
			try {
				// ��-----��------��-------���@�Ƃ���NW������A��2�̃m�[�h�ł��݂��ɃX�i�b�v����
				// �}�[�W����悤�Ɉړ�����ƁA�����ŗ�����ibug 2413�j
				// ipPolycurve2����_�������݂��Ȃ��ꍇ�ɗ�����悤�����A���O�Ɍ`��𔻒肵�ĉ]�X
				// �Ȃ�Ėʓ|�Ȃ̂�try...catch�ŕ⑫����݂̂Ƃ���B
				HRESULT hr = ipPolycurve2->SplitAtPoints( ipEnumVertex, VARIANT_FALSE, VARIANT_TRUE, dTorelance, &ipEnumSplit );
			}
			catch( ... )
			{
				AheTraceGeometry( ipPolycurve2, _T("�X�v���b�g�Ώ�") );
				AheTraceGeometry( (IGeometryPtr)ipPointCol, _T("�X�v���b�g�|�C���g") );
			}

			if ( ipEnumSplit ) {
				ipEnumSplit->Reset();

				VARIANT_BOOL	vbIsSplit = VARIANT_FALSE;

				ipEnumSplit->get_SplitHappened( &vbIsSplit );

				if ( vbIsSplit ) {
					// �X�v���b�g�_
					IPointPtr ipPoint;
					long outPartIndex=0, vertexIndex=0;

					long lCnt = 0;
					while ( SUCCEEDED( ipEnumSplit->Next( &ipPoint, &outPartIndex, &vertexIndex ) ) ) {
						if ( !ipPoint )
							break;

						if( ipSplittedPoints != NULL )
							ipSplittedPoints->AddPoint( ipPoint );	// �����_���L��

						++lCnt;
					}

					if ( lCnt > 0 )
						bIsSplitted = TRUE;
				}
			}
		}
	}

	// �X�v���b�g�ł��Ȃ������炻�̂܂܃��^�[��
	if( bIsSplitted == FALSE ) return NULL;

	AheSetKnownSimple( ipCopyPolyline );

	// IPath -> IPolyline �ϊ�
	IGeometryCollectionPtr ipGeomCol( ipCopyPolyline );
	LONG lPartCount = -1;
	ipGeomCol->get_GeometryCount( &lPartCount );

	if( lPartCount <= 1 ) return NULL;

	_ISetPtr ipSet( CLSID_Set );
	for( LONG j = 0; j < lPartCount; j++ )
	{
		IGeometryPtr ipPartGeom;
		IGeometryCollectionPtr ipNewGeomCol( CLSID_Polyline );
		IPolylinePtr ipPoly( ipNewGeomCol );
		ipPoly->putref_SpatialReference( ipSpRel ); // ��ԎQ�ƕt�^
		IZAwarePtr(ipPoly)->put_ZAware(bZAware);

		ipGeomCol->get_Geometry( j, &ipPartGeom );
		IPathPtr ipPath( ipPartGeom );
		ATLASSERT( ipPath != NULL );

		ipNewGeomCol->AddGeometry( ipPath );
		ATLASSERT( ipPoly != NULL );

//		ipPoly->SnapToSpatialReference(); <- �]�v�Ȃ��Ƃ͂��Ȃ� 2003/03/14 furukawa
		AheSetKnownSimple( ipPoly );

		ipSet->Add( ipPoly );
	}
#if 0	// ����Ȃ��񂶂�Ȃ��́H�H���܂܂ł����[�Ƃł����ƂȂ����B
	// �X�v���b�g��̃|�����C���̌������X�v���b�g�O�ƈ�v������
	BOOL bFinished = FALSE;
	IPointPtr ipTestFromPoint( ipFromPoint );
	IPointPtr ipOldFromPoint, ipOldToPoint;
	while( bFinished == FALSE )
	{
		IUnknownPtr ipUnk;
		ipSet->Reset();
		while( SUCCEEDED( ipSet->Next( &ipUnk ) ) ) 
		{
			IPolylinePtr ipPolyline( ipUnk );
			if( ipPolyline == NULL ) break;

			// ���ɒ����ς݂̂��̂͏Ȃ�
			if( AheIsFromPoint( ipOldFromPoint, ipPolyline/*, ipSpRel*/ ) == TRUE && AheIsToPoint( ipOldToPoint, ipPolyline/*, ipSpRel*/ ) == TRUE ) continue;

			BOOL bFind = FALSE;
			if( AheIsFromPoint( ipTestFromPoint, ipPolyline/*, ipSpRel*/ ) == TRUE )
			{
				bFind = TRUE;
			}
			else if( AheIsToPoint( ipTestFromPoint, ipPolyline/*, ipSpRel*/ ) == TRUE )
			{
				bFind = TRUE;
				ATLASSERT(FALSE);
				ipPolyline->ReverseOrientation();	// �t�]���Ă�̂Ō��ɖ߂�
			}

			if( bFind == TRUE )
			{
				ipOldFromPoint = ipTestFromPoint;
				ipPolyline->get_ToPoint( &ipTestFromPoint );
				ipOldToPoint   = ipTestFromPoint;
			}

			// ipToPoit �� ipOldToPoint ����v������I��
			if( AheIsEqual( ipToPoint, ipOldToPoint/*, ipSpRel*/ ) == TRUE ) bFinished = TRUE;
		}
	}
#endif
	return ipSet;
}

_ISetPtr CEditHelper::SplitPolygonByPolyline( IPolygonPtr ipPolygon, IPolylinePtr ipSplitterPolyline, BOOL bTest/* = FALSE*/ )
{
	ITopologicalOperator4Ptr ipTopo( AheGetClone( ipPolygon ) );
	if( ! ( ipTopo && ipSplitterPolyline ) )
		return NULL;

	// �n���Q�Ƃ��킹
	ISpatialReferencePtr ipSpRef;
	ipPolygon->get_SpatialReference( &ipSpRef );
	ipSplitterPolyline->putref_SpatialReference( ipSpRef );
	ipSplitterPolyline->SnapToSpatialReference();
	// ����
	IGeometryCollectionPtr col;
	if( FAILED( ipTopo->Cut2( ipSplitterPolyline, &col ) ) )
	{
			if( !bTest )
				AheMessageBox( GetHWND(), AheGetErrorMessage(), DIVISION_POLYGON, MB_OK|MB_ICONSTOP );
			else
				ATLTRACE(_T("CEditHelper::SplitPolygonByPolyline() : %s\n"), AheGetErrorMessage() );

			return NULL;
	}
	// �i�[
	_ISetPtr ipRetSet; // �߂�l
	long count = 0;
	col->get_GeometryCount( &count );
	for( long i = 0; i < count; ++i )
	{
		IGeometryPtr geom;
		col->get_Geometry( i, &geom );
		VARIANT_BOOL isEmpty = VARIANT_TRUE;
		if( geom )
			geom->get_IsEmpty( &isEmpty );
		if( !isEmpty )
		{
			if( !ipRetSet )
				ipRetSet.CreateInstance( CLSID_Set );
			ipRetSet->Add( geom );
		}
	}
	
	return ipRetSet;
}

#ifdef _USE_SETLQATTRIBUTE
BOOL CEditHelper::SetLQAttribute( LONG lFeatureID, IGeometryPtr ipBeforeSplitGeom, CTargetItems& cNewRoads, std::list<LQRFINFO>& listLQRFInfo, std::list<ROWDEF>& listLQRows )
{
	if( lFeatureID <= 0 || cNewRoads.size() <= 0 ) return FALSE;

	USES_CONVERSION;

	// ���̓��H�t�B�[�`���擾
	CTargetItems::iterator itOldRoad;
	for ( CTargetItems::iterator itRoads = cNewRoads.begin(); itRoads != cNewRoads.end(); itRoads++ ) 
	{
		if ( itRoads->GetOID() == lFeatureID ) 
		{
			itOldRoad = itRoads;
			break;
		}
	}

	// �C���O�̌`�󂩂�FROM�ATO���擾
//	IGeometryPtr ipBeforeSplitGeom;
	IPointPtr ipBeforeSplitFromPoint, ipBeforeSplitToPoint;
//	((IFeatureChangesPtr)itOldRoad->GetFeature())->get_OriginalShape( &ipBeforeSplitGeom );
	IPolylinePtr ipBeforeSplitPolyline( ipBeforeSplitGeom );
	if( ipBeforeSplitPolyline == NULL )
	{
		// �V�K�쐬�t�B�[�`���̏ꍇ�� NULL �ɂȂ�̂ŁA���̏ꍇ�� TRUE �ŕԂ��Ȃ��Ƃ����Ȃ�
		// e.x. SketchTool �Ŏ��������œ��H�����N�ɗ��[���X�i�b�v���ē��H�����N���쐬�����ꍇ
		return TRUE;
	}
	ipBeforeSplitPolyline->get_FromPoint( &ipBeforeSplitFromPoint );
	ipBeforeSplitPolyline->get_ToPoint( &ipBeforeSplitToPoint );
	
	// �Y������ LQ ������
	for ( INT i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ ) 
	{
		CString strLQTableName        = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQTableName        );
		CString strLQRFFieldName      = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName  );
		CString strInfoTableName      = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoInfoTableName      );
		LONG    lDrawMode             = m_cLoadQueueInfo.GetLoadQueueDrawMode( (sindyeRoadQueueType)i );

		// �t���O���t���Ă��邩�ǂ����`�F�b�N
		CComVariant vaLQRF = AheGetAttribute( itOldRoad->GetFeature(), strLQRFFieldName );
		if ( vaLQRF.vt != VT_I4 || vaLQRF.lVal == 0 ) continue;

		// �����N��̃e�[�u���擾�i���H�����݂��郏�[�N�X�y�[�X������Ȃ��ƃ_���j
		ITablePtr ipLQTable( AheOpenSameOwnerTable( (ITablePtr)itOldRoad->GetFeatureClass(), strLQTableName ) );
		ITablePtr ipInfoTable( AheOpenSameOwnerTable( (ITablePtr)itOldRoad->GetFeatureClass(), strInfoTableName ) );

		// �e�[�u�������݂��Ȃ��ꍇ�͏����𒆒f
		if( ipLQTable == NULL || ipInfoTable == NULL )
		{
			// [bug 3586] AcceptNoLinkQueueTable�̃��W�X�g���̒l����񂾂����ׂ�
			static bool bCheckFlag = false;
			static bool bAcceptNoLinkQueueTable = false;
			if( ! bCheckFlag )
			{
				CRegKey key;
				if( ERROR_SUCCESS == key.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
				{
					DWORD dwAcceptNoLinkQueueTable = 0;
					if( ERROR_SUCCESS == key.QueryValue( dwAcceptNoLinkQueueTable, _T("AcceptNoLinkQueueTable") ) )
						bAcceptNoLinkQueueTable = ( 1 == dwAcceptNoLinkQueueTable ) ? true : false;
				}
				bCheckFlag = true;
			}

			// [bug 3586] AcceptNoLinkQueueTable��1�Ȃ珈�����p������i�B���I�v�V�����APGDB�p�j
			if( ! bAcceptNoLinkQueueTable )
			{
				ATLASSERT( FALSE );
				CString strMsg;
				strMsg.Format(ERROR_NO_TABLE, strInfoTableName, strLQTableName );
				AheMessageBox( strMsg, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
				return FALSE;
			}
			else
				continue;
		}

		std::list<ROWDEF> /*listLQRows, */listRoads;
		std::list<LONG> listInfoIDs;

		// �������邽�߂̓��H�����N�����X�g�ɒǉ�
		ROWDEF def;
		def.flg = lDrawMode;
		def.obj = itOldRoad->GetFeature();
		def.oid = itOldRoad->GetOID();
		def.shp = itOldRoad->GetShape();
		listRoads.push_back( def );

		// ���[��
		//ISiNDYRuleObjPtr ipRule( CLSID_SiNDYRuleObj );

		// LQ������
		AheSelectLQRowByRoadIDs( listRoads, ipLQTable, strLQRFFieldName, lDrawMode, listLQRows, listInfoIDs );

		// �֘A���� Info ���ɏC��
		for( std::list<LONG>::iterator itInfoIDs = listInfoIDs.begin(); itInfoIDs != listInfoIDs.end(); itInfoIDs++ )
		{
			// [bug 891] LQ �ɂ͗����͏������A�S�� INF �ɏ����悤�Ɏd�l�ύX
			_IRowPtr ipInfoRow;
			ipInfoTable->GetRow( *itInfoIDs, &ipInfoRow );
			// [bug 1986] �{���ɏC�������ꍇ�̂ݗ��������邱��
/*			if( AheIsEditableFeature( ipInfoRow, sindyeEditAttribute ) == FALSE ) return FALSE;	// �ҏW�\����Ȃ��Ȃ甲����
			AheSetModifyData( ipInfoRow, sindyUpdateTypeProperty );	// �K���ҏW�����H�̂ŁA�����ŗ���������
			ipInfoRow->Store();*/
			BOOL bModifyDataSetted = FALSE;	// ���ɗ���������ꂽ���ǂ���

			// INF���ړ��\���ǂ����𒲂ׂĂ���
			sindyeEditableType editableType = sindyeEditableEditable;
			CComBSTR bstrErrMsg;
			if( GetRule() != NULL )
				GetRule()->IsEditableFeature( ipInfoRow, NULL, sindyeEditMove, &editableType, &bstrErrMsg );

			// [bug 3185] ���H�����N���ړ��\�ł������N�񂪈ړ��\�ł͂Ȃ��ꍇ�͓��H�����N�̃X�v���b�g��s�Ƃ���
			//            �����ɓ����Ă���Ƃ������Ƃ́A���H�����N�̃X�v���b�g�������Ȃ��ꂽ�Ƃ������ƂȂ̂ŁA������
			//            �ړ��\�łȂ���΃L�����Z���Ƃ���
			//            �{���͓��H�����N�̃X�v���b�g�����̒i�K�Ń����N����`�F�b�N����΂悢�̂����A���݂̃A���S��
			//            �Y����X�v���b�g�������_�ł̓����N��͂܂��擾����Ă��Ȃ��̂ŁA�����Ń`�F�b�N����
			//            SiNDYLib�ł͂����Ƃ�낤�ˁB
			if( sindyeEditableEditable != editableType )
			{
				CString strMessage;
				switch( editableType )
				{
					case sindyeEditableNoEditable:
						strMessage.Format(ERROR_NOT_DELETE_FOLLOWING_RULE, strInfoTableName, *itInfoIDs, OLE2T(bstrErrMsg) );
						::AheMessageBox( strMessage, EDITING_PROHIBITED, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
						return FALSE;
						break;
					case sindyeEditableWarnning:
						strMessage.Format(CONFIRM_SET_TO_WARNING_LEVEL, strInfoTableName, *itInfoIDs, OLE2T(bstrErrMsg) );
						if( ::AheMessageBox( strMessage, MESSAGE_WARNING, MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH ) == IDNO )
							return FALSE;
						break;
				}
			}

			// lFeatureID �ɊY������ LQ ��T��
			std::list<ROWDEF> listTargetLQRows;
			LONG lTargetSeq = -1, lSeq = -1;
//			listLQRows.sort();	// �V�[�P���X�ԍ����Ƀ\�[�g���Ȃ��Ƃ����Ƃł��Ȃ��H
			for( std::list<ROWDEF>::iterator itLQRows = listLQRows.begin(); itLQRows != listLQRows.end(); itLQRows++ )
			{
				// �֘A���� INFO �� ID ��ێ�
				CComVariant vaInfoID = AheGetAttribute( itLQRows->obj, INF_ID_FIELD );
				if( vaInfoID.vt == VT_I4 && vaInfoID.lVal == *itInfoIDs )
				{
					// �֘A���铹�H�����N�� ID ��ێ�
					CComVariant vaRoadID = AheGetAttribute( itLQRows->obj, LINK_ID_FIELD );
					if( vaRoadID.vt == VT_I4 && vaRoadID.lVal == lFeatureID )
					{
						// LASTLINK_F ������
						CComVariant vaLastLinkF = AheGetAttribute( itLQRows->obj, LASTLINK_F_FIELD );
						// ���݂̃V�[�P���X�ԍ����擾
						CComVariant vaSeq = AheGetAttribute( itLQRows->obj, SEQUENCE_FIELD );
						if( vaSeq.vt == VT_I4 )
							lTargetSeq = lSeq = vaSeq.lVal;
						else
						{
							// �V�[�P���X�ԍ��������������Ă��Ȃ��i����ȃf�[�^�͂Ȃ��̂��O��j
							CString strMsg;
							strMsg.Format(ERROR_SEQUENCE_NUMBER_NOT_CORRECT, strLQTableName, itLQRows->oid );
							AheMessageBox( strMsg, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
							return FALSE;
						}
						// LINKDIR_C �����āAFROM�ATO�̂ǂ��炩�猩��΂悢�����肷��
						IPointPtr ipCheckPoint;
						BOOL bReverse;
						CComVariant vaLinkDIR = AheGetAttribute( itLQRows->obj, LINKDIR_C_FIELD );
						if( vaLinkDIR.vt == VT_I4 && vaLinkDIR.lVal == 1 )
						{
							// ������
							bReverse = FALSE;
							ipCheckPoint = ipBeforeSplitFromPoint;
						}
						else if( vaLinkDIR.vt == VT_I4 && vaLinkDIR.lVal == 2 )
						{
							// �t����
							bReverse = TRUE;
							ipCheckPoint = ipBeforeSplitToPoint;
						}
						else
						{
							// �����R�[�h�������������Ă��Ȃ��i����ȃf�[�^�͂Ȃ��̂��O��j
							CString strMsg;
							strMsg.Format(ERROR_DIRECTION_CODE_NOT_CORRECT, strLQTableName, itLQRows->oid );
							AheMessageBox( strMsg, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
							return FALSE;
						}
						// �������ꂽ���H�����N�̕��Ɋւ��ă����N��쐬���C��
						BOOL bFinished = FALSE;					// ����ȏ㏈���������Ȃ����߂̃t���O�i�����N��ŏI�����N�𕪒f���ꂽ�ꍇ�p�j
						CTargetItems cRoadItems = cNewRoads;
						while( cRoadItems.size() != 0 )
						{
							BOOL bErase = FALSE;	// // �������[�v�h�~�p
							CTargetItems::iterator itRoad = cRoadItems.begin();
							while( itRoad != cRoadItems.end() ) 
							{
								BOOL bFind = FALSE;
								if( bReverse == FALSE )
								{
									if( itRoad->IsFromPoint( ipCheckPoint ) == TRUE )
									{
										bFind = TRUE;
										ipCheckPoint = itRoad->GetToPoint();
									}
								} else {
									if( itRoad->IsToPoint( ipCheckPoint ) == TRUE )
									{
										bFind = TRUE;
										ipCheckPoint = itRoad->GetFromPoint();
									}
								}
								if( bFind == TRUE )
								{
									// ������LQRFInfo��������
									std::list<LQRFINFO>::iterator itLQRFInfo = listLQRFInfo.begin();
									while( itLQRFInfo != listLQRFInfo.end() && itLQRFInfo->oid != itRoad->GetOID() )
										++itLQRFInfo;

									if( itLQRFInfo == listLQRFInfo.end() )
									{
										// �Ȃ���ΐV�K�ǉ�
										LQRFINFO obj;
										listLQRFInfo.push_front( obj );
										itLQRFInfo = listLQRFInfo.begin();
										itLQRFInfo->oid = itRoad->GetOID();
									}

//									ATLTRACE(_T("SetLQAttribute : Find : %d\n"), itRoad->GetOID() );
									// VICS �ł͂Ȃ����C���^�[�i�rVICS �ł͂Ȃ����� SEQUENCE = 1 �ł���ꍇ�A���̓��H�����N�Ɋ֘A����LQ�͍쐬���Ȃ��E���邢�͑��݂���Ȃ�U�ւ�����i�����N��J�n�����N�𕪒f���ꂽ�ꍇ�j
									// [bug 8968] �A�N�Z�X����VICS���l�Ƃ���B
									// bFinished == TRUE �̎��͂��̓��H�����N�̏������L�����Z������K�v�A���i�����N��ŏI�����N�𕪒f���ꂽ�ꍇ�j
									if( ( m_cLoadQueueInfo.IsShortenerRoadQueue( (sindyeRoadQueueType)i ) && lTargetSeq == 1 && cRoadItems.size() > 1 ) || bFinished == TRUE )
									{
//										ATLTRACE(_T("---------------------    %d\n"), itRoad->GetOID() );
										// ���Ɋ֘A���Ă��郊���N�񂪂Ȃ��Ȃ瓹�H�����N�� LQRF_* �� 0 �ɂ���
										BOOL bFindOtherLQ = FALSE;
										for( std::list<ROWDEF>::iterator itLQRowsTmp = listLQRows.begin(); itLQRowsTmp != listLQRows.end(); itLQRowsTmp++ )
										{
											if( itLQRowsTmp == itLQRows ) continue;

											if( itRoad->GetOID() == AheGetAttribute( itLQRowsTmp->obj, LINK_ID_FIELD ).lVal )
											{
												bFindOtherLQ = TRUE;
												break;
											}
										}
										if( bFindOtherLQ == FALSE )
										{
//											ATLTRACE(_T("CEditHelper::SetLQAttribute() : �e�[�u��[ROAD_LINK] OID[%d] %s�֘A�t���O��0�ɏC��\n"), itRoad->GetOID(), strLQRFFieldName );
//											AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 0L );	// ���H�����N�� LQRF_* �� 0 �ɂ���i�����͒N�������ꂭ���j
											if( itLQRFInfo->LQRF.end() == itLQRFInfo->LQRF.find( (sindyeRoadQueueType)i ) )
											{
												itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = false;	// �܂����݂��Ȃ��ꍇ�̂�false������
											}
										}
										itRoad = cRoadItems.erase( itRoad );
										bErase = TRUE;
										continue;
									}

									// ���݂��郁�b�V���R�[�h�擾
									CMesh mesh;
									LONG lMeshCode = mesh.GetExistMeshCode( itRoad->GetShape() );
									if( lMeshCode < 0 )
									{
										AheMessageBox( ERROR_SPLIT_ROAD_LINK_ACROSS_MESH, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
										return FALSE;
									}
									_IRowPtr ipTargetRow;
									bool bCreated = false;	// LQ���V�K�ɍ��ꂽ���ǂ���
									// VICS�i�C���^�[�i�r�܂ށj�ł��邩�A���� LASTLINK_F �����Ă��Ȃ��ꍇ
									// [bug 8968] �A�N�Z�X����VICS���l�Ƃ���B
									if( !m_cLoadQueueInfo.IsShortenerRoadQueue( (sindyeRoadQueueType)i ) || ( vaLastLinkF.vt == VT_I4 && vaLastLinkF.lVal != 1L ) )
									{
										// �����̓��H�����N�Ȃ�A���� LQ �����݂���̂ł�����g�p
										if( itRoad->GetOID() == lFeatureID )
										{
											ipTargetRow = itLQRows->obj;
											//AheSetModifyData( ipTargetRow, sindyUpdateTypeProperty );			// �ύX����
//											AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 1L );			// ���H�����N�Ƀ����N��֘A�t���O��U��
//											ATLTRACE(_T("CEditHelper::SetLQAttribute() : �e�[�u��[ROAD_LINK] OID[%d] %s�֘A�t���O��1�ɏC��\n"), itRoad->GetOID(), strLQRFFieldName );
											itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = true;
										}
										// �����̓��H�����N�ł͂Ȃ��Ȃ� LQ ��V�K�ɍ쐬
										else if( m_cLoadQueueInfo.IsShortenerRoadQueue( (sindyeRoadQueueType)i ) && lTargetSeq == 1 )
										{
											ipTargetRow = itLQRows->obj;
//											AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 1L );			// ���H�����N�Ƀ����N��֘A�t���O��U��
//											ATLTRACE(_T("CEditHelper::SetLQAttribute() : �e�[�u��[ROAD_LINK] OID[%d] %s�֘A�t���O��1�ɏC��\n"), itRoad->GetOID(), strLQRFFieldName );
											itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = true;

										}
										else {
											bCreated = true;
											ipTargetRow = AheCreateRow( ipLQTable );
											//AheSetModifyData( ipTargetRow, sindyUpdateTypeCreate );				// �ύX����
//											AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 1L );		// ���H�����N�Ƀ����N��֘A�t���O��U��
//											ATLTRACE(_T("CEditHelper::SetLQAttribute() : �e�[�u��[ROAD_LINK] OID[%d] %s�֘A�t���O��1�ɏC��\n"), itRoad->GetOID(), strLQRFFieldName );
											itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = true;
										}
									}
									// VICS �ȊO�� LASTLINK_F �����Ă���ꍇ�͊����� LQ ���g�p�A������ŏI��������K�v������
									else {
										ipTargetRow = itLQRows->obj;
										//AheSetModifyData( ipTargetRow, sindyUpdateTypeProperty );				// �ύX����
//										AheSetAttribute( itRoad->GetFeature(), strLQRFFieldName, 1L );			// ���H�����N�Ƀ����N��֘A�t���O��U��
//										ATLTRACE(_T("CEditHelper::SetLQAttribute() : �e�[�u��[ROAD_LINK] OID[%d] �֘A�t���O��1�ɏC��\n"), itRoad->GetOID() );
										itLQRFInfo->LQRF[(sindyeRoadQueueType)i] = true;
										bFinished = TRUE;
									}

									if( ipTargetRow != NULL )
									{
										BOOL bEditField = FALSE;
										// ��ɕύX�����邩�ǂ����𒲂ׂ�
										// �����R�[�h
										CComVariant vaOldValue = AheGetAttribute( ipTargetRow, LINKDIR_C_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != vaLinkDIR.lVal ) bEditField = TRUE;
										// LINK_ID
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, LINK_ID_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != itRoad->GetOID() ) bEditField = TRUE;
										// SEQUENCE
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, SEQUENCE_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != lSeq ) bEditField = TRUE;
										// MESHCODE
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, MESHCODE_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != lMeshCode ) bEditField = TRUE;
										// �֘A���� INFO �� ID �t�B�[���h
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, INF_ID_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != vaInfoID.lVal ) bEditField = TRUE;
										// LASTLINK_F
										LONG lNewLastLinkFlag = ( ( vaLastLinkF.vt == VT_I4 && vaLastLinkF.lVal == 1 && ( cRoadItems.size() == 1 || bFinished == TRUE ) ) ? 1 : 0 );
										vaOldValue.Clear(); vaOldValue = AheGetAttribute( ipTargetRow, LASTLINK_F_FIELD );
										if( vaOldValue.vt != VT_I4 || vaOldValue.lVal != lNewLastLinkFlag ) bEditField = TRUE;
										
										// [bug 1986] �ύX������ꍇ��INF���u�ړ��\�v���ǂ����𒲂ׂĂ���iLQ�̑����ύX��INF�̌`��ύX�j
										if( bEditField == TRUE )
										{
											// ���ɒ����ς݂̃��[���𒲂ׂ�
											CString strMessage;
											switch( editableType )
											{
												case sindyeEditableNoEditable:
													strMessage.Format(ERROR_NOT_DELETE_FOLLOWING_RULE, strInfoTableName, *itInfoIDs, OLE2T(bstrErrMsg) );
													::AheMessageBox( strMessage, EDITING_PROHIBITED, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
													return FALSE;
													break;
												case sindyeEditableWarnning:
													strMessage.Format(CONFIRM_SET_TO_WARNING_LEVEL, strInfoTableName, *itInfoIDs, OLE2T(bstrErrMsg) );
													if( ::AheMessageBox( strMessage, MESSAGE_WARNING, MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH ) == IDNO )
														return FALSE;
													break;
											}
											ATLTRACE(_T("CEditHelper::SetLQAttribute() : �e�[�u��[%s] OID[%d] �C��\n"), strLQTableName, AheGetOID( ipTargetRow ) );
											AheSetAttribute( ipTargetRow, LINKDIR_C_FIELD, vaLinkDIR );			// �����R�[�h
											AheSetAttribute( ipTargetRow, LINK_ID_FIELD, CComVariant(itRoad->GetOID()) );	// LINK_ID �t�B�[���h��������
											AheSetAttribute( ipTargetRow, SEQUENCE_FIELD, CComVariant(lSeq) );				// SEQUENCE �t�B�[���h��������
											AheSetAttribute( ipTargetRow, MESHCODE_FIELD, CComVariant(lMeshCode) );			// MESHCODE �t�B�[���h��������
											AheSetAttribute( ipTargetRow, INF_ID_FIELD, vaInfoID );							// �֘A���� INFO �� ID �t�B�[���h��������
											AheSetAttribute( ipTargetRow, LASTLINK_F_FIELD, CComVariant(lNewLastLinkFlag) );	// LASTLINK_F ��������
											ipTargetRow->Store();		// �����ŕۑ����Ă��ǂ��H

											// LQ�ɕύX���������ꍇ��INF�Ɍ`��ύX�Ƃ��ė���������
											if( bModifyDataSetted == FALSE )
											{
												ATLTRACE(_T("CEditHelper::SetLQAttribute() : �e�[�u��[%s] OID[%d] �`��ύX�̗���ǉ�\n"), strInfoTableName, *itInfoIDs );
												AheSetModifyData( ipInfoRow, sindyUpdateTypeShape );
												ipInfoRow->Store();
												bModifyDataSetted = TRUE;	// ���x�����邱�Ƃ��Ȃ��悤�Ƀt���O�𗧂Ă�
											}
											// LQ���V�K�ɍ��ꂽ�ꍇ��listLQRows�ɒǉ����Ă���
											if( bCreated )
											{
												ROWDEF def;
												def.direction = vaLinkDIR.lVal;
												def.flg = lDrawMode;
												def.obj = ipTargetRow;
												long lOID = -1; ipTargetRow->get_OID( &lOID ); def.oid = lOID;
												def.sequence = lSeq;
												def.shp = itRoad->GetShapeCopy();
												listLQRows.push_back( def );
											}
										}
										lSeq++;
									}
									itRoad = cRoadItems.erase( itRoad );
									bErase = TRUE;	// �������[�v�h�~�p
								}
								else
									itRoad++;
							}
							// �X�v���b�g��̓��H�����N�� FROM�ATO�|�C���g���X�v���b�g�O�̓��H�����N�� FROM�ATO �ƈ�v���Ȃ�
							if( itRoad == cRoadItems.end() && bErase == FALSE )
							{
								ATLASSERT( FALSE );
								AheMessageBox(ERROR_SPLIT_ROAD_LINK_SHAPE_IS_IMPROPER, ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
								return FALSE;
							}
						}
					}
					// �����N��̕ҏW�Ώۂł͂Ȃ����̂́A�V�[�P���X�ԍ��U��Ȃ����ΏۂƂ���
					else
						listTargetLQRows.push_back( *itLQRows );
				}
			}
			// �������ꂽ���H�����N�Ɋ֘A���郊���N������̃V�[�P���X���������N��Ɋւ��ăV�[�P���X�ԍ��U��Ȃ���
			for( std::list<ROWDEF>::const_iterator itLQRows = listTargetLQRows.begin(); itLQRows != listTargetLQRows.end(); itLQRows++ )
			{
				CComVariant vaSeq = AheGetAttribute( itLQRows->obj, SEQUENCE_FIELD );
				if( vaSeq.vt == VT_I4 && vaSeq.lVal > lTargetSeq )
				{
					AheSetAttribute( itLQRows->obj, SEQUENCE_FIELD, CComVariant(vaSeq.lVal + ( (lSeq-1L) - lTargetSeq )) );
					if( AheRowIsChanged( itLQRows->obj ) == TRUE )
					{
//						AheSetModifyData( itLQRows->obj, sindyUpdateTypeProperty );			// �ύX����
						itLQRows->obj->Store();	// �����ŕۑ����Ă��ǂ��H
					}
				}
			}
		}
	}

	return TRUE;
}
#else
// �������ꂽ���H�����N�̂��߂̐V����LQ���쐬����
BOOL CEditHelper::CreateLQForSplittedRoad( CTargetItem& cRoad, CTargetItems& cNewRoads, std::list<ROWDEF>& listLQRows )
{
	bool bRet = TRUE;	// �Ԃ�l

	// cRoad�Ɋ֘A����LQ���擾
	if( ) // VICS�n�A�܂��͔������A���̓V�[�P���X��1�ȊO���ŏI�t���O��false���H
	{
		// LQ���쐬���ĐV�������H�����N�Ɗ֘A�t����
	}

	return bRet;
}

#endif // ifdef _USE_SETLQATTRIBUTE

// �V�[�P���X�̐��������C������
BOOL CEditHelper::FixLQSequence( std::list<LQDEF>& listLQ, std::map<long, ROADDEF>& mapRoad )
{
	// �֘A���H�����N��FROM�ATO���݂Đ��������ԂɃ\�[�g
	// �C���r���ŃG���[�ŋA��ꍇ�����邽�߁A���̂Ƃ��ɕ\�������_�C�A���O
	// �̃g���[�X��SetLQAttribute�ł̏������ʂł͂Ȃ�FixLQSequence�ł̏C��
	// �r���̂��̂ł��邱�Ƃɒ��ӁI�I
	BOOL bRet = false;	// �Ԃ�l

	bool bUniqued = true, bSorted = true;
	std::list<LQDEF>::iterator it;

#ifdef _DEBUG
	{
		for( std::list<LQDEF>::const_iterator it = listLQ.begin(), itEnd = listLQ.end(); it != itEnd; ++it )
			ATLTRACE(_T("�C���O�F%s\n"), GetTraceLQDEFString( *it, mapRoad ) );
		ATLTRACE(_T("\n"));
	}
#endif // ifdef _DEBUG

	// �܂���������LQ��listLQ�ɓo�^����Ă���ꍇ������̂ŁA���̏ꍇ��IRow::Delete�����Ƀ��X�g����폜
	// �����̃��[�v��IRow::Delete�̃��[�v�ƈꏏ�ɂ��Ă͂����Ȃ��I�I
	for( it = listLQ.begin(); it != listLQ.end(); ++it )
	{
		std::list<LQDEF>::iterator it2 = it; ++it2;
		while( it2 != listLQ.end() )
		{
			// �܂���������LQ��listLQ�ɓo�^����Ă���ꍇ������̂ŁA���̏ꍇ��IRow::Delete�����Ƀ��X�g����폜
			if( it->oid == it2->oid )
			{
				ATLTRACE(_T("WARNING : ����LQ���O : %s\n"), GetTraceLQDEFString(*it2) );
				it2 = listLQ.erase( it2 );
				if( listLQ.end() == it2 )
					break;
			}
			else
				++it2;
		}
	}

	// SetLQAttribute�œ������H�����N�ɕ�����LQ���쐬�����ꍇ�ꂠ��i��x��2��ȏ㕪�������ꍇ�j
	// ���XSetLQAttribute�Ɏ������̂��܂�ǂ������̂ŏd��������̂͂����ō폜���Ă��܂�
	for( it = listLQ.begin(); it != listLQ.end(); ++it )
	{
		std::list<LQDEF>::iterator it2 = it; ++it2;
		while( it2 != listLQ.end() )
		{
			// linkid�Ŕ�r
			if( it->linkid == it2->linkid )
			{
				if( SUCCEEDED( it2->obj->Delete() ) )
				{
					ATLTRACE(_T("WARNING : �d��LQ�폜 : %s\n"), GetTraceLQDEFString(*it2) );
					it2 = listLQ.erase( it2 );
					if( listLQ.end() == it2 )
						break;
				}
				else {
					_ASSERTE(false);	// �폜���s
					return FALSE;
				}
			}
			else
				++it2;
		}
	}

FIX_LQ_SEQ:
	long lSeq = 1;
	for( it = listLQ.begin(); it != listLQ.end(); ++it, ++lSeq )
	{
		// it���Ō�Ȃ�I���
		if( (ULONG)lSeq == listLQ.size() ) break;

		// FROM�ATO��������l�����Ď擾
		IPointPtr ipFrom, ipTo;
		std::map<long, ROADDEF>::const_iterator itRoad = mapRoad.find( it->linkid );
		_ASSERTE( itRoad != mapRoad.end() );
		if( itRoad != mapRoad.end() )
		{
			IPolylinePtr ipGeom( itRoad->second.shape );
			_ASSERTE( ipGeom );
			if( ipGeom )
			{
				ipGeom->get_FromPoint( ( it->dir == 1 ) ? &ipFrom : &ipTo );
				ipGeom->get_ToPoint( ( it->dir == 1 ) ? &ipTo : &ipFrom );
			}
		}
		bool bFindNext = false;

		std::list<LQDEF>::iterator it2 = it; ++it2; // it2��it�̎�����
		for( ; it2 != listLQ.end(); ++it2 )
		{
			// FROM�ATO��������l�����Ď擾
			IPointPtr ipFrom2, ipTo2;
			std::map<long, ROADDEF>::const_iterator itRoad2 = mapRoad.find( it2->linkid );
			_ASSERTE( itRoad2 != mapRoad.end() );
			if( itRoad2 != mapRoad.end() )
			{
				IPolylinePtr ipGeom( itRoad2->second.shape );
				_ASSERTE( ipGeom );
				if( ipGeom )
				{
					ipGeom->get_FromPoint( ( it2->dir == 1 ) ? &ipFrom2 : &ipTo2   );
					ipGeom->get_ToPoint(   ( it2->dir == 1 ) ? &ipTo2   : &ipFrom2 );
				}
			}
			if( lSeq == 1 )
			{
//				if( xfrom == xto2 && yfrom == yto2 )
				if( AheIsEqual( ipFrom, ipTo2 ) )
				{
					std::list<LQDEF>::iterator it2Next = it2; ++it2;	// it2�̎�
					listLQ.splice( it, listLQ, it2, it2Next );
					goto FIX_LQ_SEQ;	// �ŏ�����
				}
			}

			// it�̎���it2�̏ꍇ
//			if( xto == xfrom2 && yto == yfrom2 )
			if( AheIsEqual( ipTo, ipFrom2 ) )
			{
				bFindNext = true;
				// it2��it�̂�����Ȃ炻�̂܂ܑ�����
				// �����łȂ����it�̎��Ɉړ�
				std::list<LQDEF>::const_iterator it2Prev = it2; --it2Prev;	// it2�̑O
				if( it2Prev != it )
				{
					std::list<LQDEF>::iterator itNext = it; ++itNext;		// it�̎�
					std::list<LQDEF>::iterator it2Next = it2; ++it2Next;	// it2�̎�
					listLQ.splice( itNext, listLQ, it2, it2Next );
				}
			}

			if( bFindNext )
				break;
		}

		if( ! bFindNext )
		{
			for( std::list<LQDEF>::const_iterator it = listLQ.begin(), itEnd = listLQ.end(); it != itEnd; ++it )
				ATLTRACE(_T("�s��v�F%s\n"), GetTraceLQDEFString( *it, mapRoad ) );

			_ASSERTE(false);
			bSorted = false;
			break;
		}
	}
	// �\�[�g�ł�����V�[�P���X���C�����Ă���
	if( bSorted )
	{
		bool bFixed = true;
		long lSeq = 1, lSeqIndex = -1, lLastFIndex = -1, lSize = listLQ.size();
		for( std::list<LQDEF>::iterator it = listLQ.begin(); it != listLQ.end(); ++it, ++lSeq )
		{
			if( it->seq != lSeq )
			{
				// �C���f�b�N�X�ԍ��擾�͏���̂�
				if( 0 > lSeqIndex )
				{
					IFieldsPtr ipFields;
					it->obj->get_Fields( &ipFields );
					ipFields->FindField( CComBSTR(SEQUENCE_FIELD), &lSeqIndex );
				}
				// �C��
				// LQ�Ɋւ��Ă�LQSetAttrinbute�Ŋ���Store��������Ă���̂ł����ł�Store
				if( SUCCEEDED( it->obj->put_Value( lSeqIndex, CComVariant(lSeq) ) ) &&
					SUCCEEDED( it->obj->Store() ) )
				{
					ATLTRACE(_T("WARNING : �V�[�P���X�ԍ��C�� : %s : -> %d\n"), GetTraceLQDEFString(*it), lSeq );
					it->seq = lSeq;	// �ꉞ�������������Ă���
				}
				else {
					ATLTRACE(_T("ERROR : �V�[�P���X�ԍ��C�����s : %s : -> %d\n"), GetTraceLQDEFString(*it), lSeq );
					bFixed = false;
					_ASSERTE(false);
					break;	
				}
			}
			// �I�[�t���O
			if( it->last != ( ( lSeq == lSize ) ? true : false ) )
			{
				// �C���f�b�N�X�ԍ��擾�͏���̂�
				if( 0 > lLastFIndex )
				{
					IFieldsPtr ipFields;
					it->obj->get_Fields( &ipFields );
					ipFields->FindField( CComBSTR(LASTLINK_F_FIELD), &lLastFIndex );
				}
				// �C��
				// LQ�Ɋւ��Ă�LQSetAttrinbute�Ŋ���Store��������Ă���̂ł����ł�Store
				if( SUCCEEDED( it->obj->put_Value( lLastFIndex, CComVariant( ( lSeq == lSize ) ? 1L : 0L ) ) ) &&
					SUCCEEDED( it->obj->Store() ) )
				{
					ATLTRACE(_T("WARNING : �I�[�t���O�C�� : %s : -> %s\n"), GetTraceLQDEFString(*it), ( lSeq == lSize ) ? _T("TRUE") : _T("FALSE") );
					it->last = ( lSeq == lSize ) ? true : false;
				}
				else {
					ATLTRACE(_T("ERROR : �I�[�t���O�C�����s : %s : -> %d\n"), GetTraceLQDEFString(*it), ( lSeq == lSize ) ? _T("TRUE") : _T("FALSE") );
					bFixed = false;
					_ASSERTE(false);
					break;
				}
			}
		}
		// �C���ł��Ȃ����̂���������G���[
		bRet = ( bFixed ) ? true : false;
	}
#ifdef _DEBUG
	{
		for( std::list<LQDEF>::const_iterator it = listLQ.begin(), itEnd = listLQ.end(); it != itEnd; ++it )
			ATLTRACE(_T("�C����i%s�j�F%s\n"), ( bRet ) ? _T("����") : _T("���s"), GetTraceLQDEFString( *it, mapRoad ) );
		ATLTRACE(_T("\n"));
	}
#endif // ifdef _DEBUG

	return bRet;
}


IPointCollectionPtr CEditHelper::GetCrossOrTouchPoints( CTargetItems& cDstItems, CTargetItems& cSrcItems, INT iMode/* = 2*/, CTargetItems* pNoRelItems/* = NULL*/ )
{
//	SetDomain();
	// �����|�C���g��T��
	std::list<LongPair> listChecked;
	IPointCollectionPtr ipCrossPointCol;	// �Ԃ�l
	for( CTargetItems::iterator itDstItems = cDstItems.begin(); itDstItems != cDstItems.end(); itDstItems++ )
	{
/*		///////////////////////////
		AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), itDstItems->GetShape(), RGB(255,0,255), 2, esriSLSSolid, TRUE, FALSE );
		///////////////////////////*/
		BOOL bFindRelItems = FALSE;	// ��_�E�ړ_��������ꂽ���ǂ���
		for( CTargetItems::iterator itSrcItems = cSrcItems.begin(); itSrcItems != cSrcItems.end(); itSrcItems++ )
		{
			if( *itSrcItems == *itDstItems ) continue;	// �����t�B�[�`�����m�͂͂���
			// ���ɓ����y�A�����������ǂ����`�F�b�N
			LONG lSrcOID = itSrcItems->GetOID(), lDstOID = itDstItems->GetOID();
			for( std::list<LongPair>::iterator it = listChecked.begin(); it != listChecked.end(); it++ )
			{
				if( ( it->lID1 == lSrcOID && it->lID2 == lDstOID ) || ( it->lID1 == lDstOID && it->lID2 == lSrcOID ) )
					continue;
			}
			LongPair pair = { lSrcOID, lDstOID };
			listChecked.push_back( pair );

			ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints() : ��r�F%d AND %d\n"), itSrcItems->GetOID(), itDstItems->GetOID() );
/*			///////////////////////////
			AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), itSrcItems->GetShape(), RGB(255,255,0), 2, esriSLSSolid, TRUE, FALSE );
			continue;
			///////////////////////////*/

			// �`���IsKnownSimple��VARIANT_TRUE��ݒ�B
			// �V�K�쐬�����t�B�[�`���̌`���IsKnownSimple��ݒ肵�ĂĂ��A
			// ���Ɏ��o���Ƃ���VARIANT_FALSE�ɂȂ��Ă���̂ŁB
			IGeometryPtr ipSrcGeom = AheSetKnownSimple( itSrcItems->GetShapeCopy() );
			IGeometryPtr ipDstGeom = AheSetKnownSimple( itDstItems->GetShapeCopy() );

			BOOL bIsDisjoint = TRUE; // Disjoint���ǂ���
			IPointCollectionPtr ipIntersectPoints = AheGetCrossOrTouchPoints( ipDstGeom, ipSrcGeom, iMode, &bIsDisjoint );
			if( ! bIsDisjoint )
				bFindRelItems = TRUE; // �������[�h�ɂ�炸�ADisjoint���ǂ����Ŕ��f���Ȃ��ƍ\���_�̏d���`�F�b�N���ł��Ȃ��ibug 8463 ROADCHECK 02320�j

			if( ipIntersectPoints == NULL ) continue;

			ISpatialReferencePtr ipSpRef( AheGetSpatialReference( ipIntersectPoints ) );

			// �|�C���g�R���N�V�����ɒǉ�
			LONG lCount;
			ipIntersectPoints->get_PointCount( &lCount );
			IPointPtr ipPoint( CLSID_Point );

			ipPoint->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
			for( LONG i = 0; i < lCount; i++ )
			{
				ipPoint->PutCoords( 0.0f, 0.0f );
				ipIntersectPoints->QueryPoint( i, ipPoint );

				if( ipCrossPointCol == NULL )
				{
					ipCrossPointCol.CreateInstance( CLSID_Multipoint );
					((IGeometryPtr)ipCrossPointCol)->putref_SpatialReference( AheGetSpatialReference( ipPoint ) ); // ��ԎQ�ƕt�^
				}

				ipCrossPointCol->AddPoint( (IPointPtr)AheGetClone(ipPoint) );
/*#ifdef DEBUG
				AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipPoint, RGB(50, 255, 0 ), 8, esriSMSCircle );
#endif*/
			}
		}
		// pNoRelItems ���n����Ă���ꍇ�� DstItems �̒��Ō�_�E�ړ_�������邱�Ƃ��ł��Ȃ��������̂����Ă���
		if( pNoRelItems != NULL )
		{
			if( bFindRelItems == FALSE )
				pNoRelItems->push_back( *itDstItems );
		}
	}

	AheRemoveAdjoiningPoint( ipCrossPointCol );

	return ipCrossPointCol;
}

// �֘A�I�u�W�F�N�g���폜
bool DeleteRelation( ITablePtr ipRelTable, std::pair<CString, long>& outerKeyVal )
{
	// ����
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQueryFilter = AheInitQueryFilter( NULL, NULL, _T("%s=%ld"), outerKeyVal.first, outerKeyVal.second );
	ipRelTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	if( !ipCursor )
	{
		_ASSERTE( false );
		return false;
	}

	_IRowPtr ipRow;
	while( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		if(SUCCEEDED(ipRow->Delete()))
		{
			// �X�Ɋ֘A�Â��Ă���΍폜
			CString myTable = AheGetFeatureClassName( ipRelTable );
			CString relTable = AheGetRelationTableName( myTable, _T("") );
			// �֘A�Â����e�[�u�����Ȃ���ΏI��
			if( relTable.IsEmpty() )
				continue;

			CString field = AheGetRelationOuterKeyFieldName( myTable, _T("") );
			ITablePtr relT = AheOpenSameOwnerTable( ipRelTable, relTable );
			// TODO: OID�ȊO���L�[�ɂł��Ȃ��ȁA�A
			if(!DeleteRelation( relT, std::make_pair( field, AheGetOID(ipRow) ) ))
				return false;
		}
		else {
			AheMessageBox( ERROR_FAIL_DELETE_EXTERNAL_RECORD, EXTERNAL_RECORD_DELETE_FAILURE, MB_OK|MB_ICONSTOP );
			return false;
		}
	}
	return true;
}

// �������ꂽNW�t�B�[�`���̊֘A�e�[�u�����폜
BOOL CEditHelper::DeleteRelationTables( IFeaturePtr ipFeature ) const
{
	// �����`�F�b�N
	if( ! ipFeature ) 
	{
		_ASSERTE( ipFeature );
		return FALSE;
	}

	long lOID = AheGetOID( ipFeature ); // �֘A�e�[�u�������t�B�[�`���̃I�u�W�F�N�gID���擾
	CString strClassName( AheGetFeatureClassName( ipFeature ) ); // �t�B�[�`���N���X��

	// �O���e�[�u���֘A�t���O�̃��X�g���擾
	std::list<CString> listRelFieldName = AheGetRelationFlagFieldName( strClassName );
	for( std::list<CString>::const_iterator it = listRelFieldName.begin(); it != listRelFieldName.end(); ++it )
	{
		// [SJ�S���S�H��]�S���n�̃t�B�[�`���N���X�̂Ƃ��͊֘A�t���O���݂Ȃ�
		// [�����x�n�}] ADAM�l�b�g���[�N�̂Ƃ����֘A�t���O�݂Ȃ�
		if( 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_link::kTableName )
			&& 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_node::kTableName )
			&& !AheIsAdamNetwork(strClassName))
		{
			// �֘A�t���O�������Ă��邩�ǂ���
			CComVariant vaRelF = AheGetAttribute( ipFeature, *it );
			if( !(vaRelF.vt == VT_I4 && vaRelF.lVal == 1) )
				continue;
		}

		// �O���e�[�u�����擾
		ITablePtr ipRelTable( AheOpenRelationTable( ipFeature, *it ) );
		// �O���e�[�u���ɂ���O���L�[���擾
		CString strOuterKeyFieldName( AheGetRelationOuterKeyFieldName( strClassName, *it ) );

		// �O���e�[�u���ɕR�t���Ă�����̂��S�č폜
		if(!DeleteRelation( ipRelTable, std::make_pair( strOuterKeyFieldName, lOID ) ))
			return FALSE;
	}
	return TRUE;
}

// �w�肵���t�B�[���h���O���L�[�Ƃ��ăe�[�u���Ɋ֘A�t����
bool CopyRelation( ITablePtr ipRelTable, std::pair<CString, long>& outerKeyVal, long parentID )
{
	// ����
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQueryFilter = AheInitQueryFilter( NULL, NULL, _T("%s=%ld"), outerKeyVal.first, outerKeyVal.second );
	ipRelTable->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor );

	if( !ipCursor )
	{
		_ASSERTE( false );
		return false;
	}

	_IRowPtr ipRow;
	while( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		_IRowPtr ipNewRow = AheCreateRow( ipRelTable, NULL, TRUE );
		if( AheCopyAttribute( ipRow, ipNewRow, TRUE )                           && // [bug 4043] �������[�h�ŃR�s�[
			AheSetModifyData( ipNewRow, sindyUpdateTypeCreate )                 &&
			AheSetAttribute( ipNewRow, outerKeyVal.first, parentID )            &&
			SUCCEEDED( ipNewRow->Store() ) )
		{
			// �X�Ɋ֘A�Â��Ă���΃R�s�[�i�Ԏ�ʋK���Ή��j
			CString myTable = AheGetFeatureClassName( ipRelTable );
			CString relTable = AheGetRelationTableName( myTable, _T("") );
			// �֘A�Â����e�[�u�����Ȃ���ΏI��
			if( !relTable.IsEmpty() )
			{
				CString field = AheGetRelationOuterKeyFieldName( myTable, _T("") );
				ITablePtr relT = AheOpenSameOwnerTable( ipRelTable, relTable );
				// TODO: OID�ȊO���L�[�ɂł��Ȃ��ȁA�A
				CopyRelation( relT, std::make_pair( field, AheGetOID(ipRow) ), AheGetOID(ipNewRow) );
			}
		}
		else {
			AheMessageBox( ERROR_FAIL_COPY_EXTERNAL_RECORD, EXTERNAL_RECORD_COPY_FAILURE, MB_OK|MB_ICONSTOP );
			return false;
		}
	}
	return true;
}

BOOL CEditHelper::CopyRelationAttribute( IFeaturePtr ipFeature, CTargetItems& cSplittedRoadFeatures )
{
	// �����`�F�b�N
	if( ! ipFeature ) 
	{
		_ASSERTE( ipFeature );
		return FALSE;
	}
	if( cSplittedRoadFeatures.empty() )
		return TRUE;

	long lOID = AheGetOID( ipFeature ); // �֘A�e�[�u�������t�B�[�`���̃I�u�W�F�N�gID���擾
	CString strClassName( AheGetFeatureClassName( ipFeature ) ); // �t�B�[�`���N���X��

	// �O���e�[�u���֘A�t���O�̃��X�g���擾
	std::list<CString> listRelFieldName = AheGetRelationFlagFieldName( strClassName );
	for( std::list<CString>::const_iterator it = listRelFieldName.begin(); it != listRelFieldName.end(); ++it )
	{
		// [SJ�S���S�H��]�S���n�̃t�B�[�`���N���X�̂Ƃ��͊֘A�t���O���݂Ȃ�
		// [�����x�n�}] ADAM�l�b�g���[�N�̂Ƃ����֘A�t���O�݂Ȃ�
		if( 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_link::kTableName )
			&& 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_node::kTableName ) 
			&& !AheIsAdamNetwork(strClassName))
		{
			// �֘A�t���O�������Ă��邩�ǂ���
			CComVariant vaRelF = AheGetAttribute( ipFeature, *it );
			if( !(vaRelF.vt == VT_I4 && vaRelF.lVal == 1) )
				continue;
		}

		// �O���e�[�u�����擾
		ITablePtr ipRelTable( AheOpenRelationTable( ipFeature, *it ) );
		// �O���e�[�u���ɂ���O���L�[���擾
		CString strOuterKeyFieldName( AheGetRelationOuterKeyFieldName( strClassName, *it ) );

		// �V�K�ɍ쐬���ꂽ�����N��T��
		for( CTargetItems::iterator itItem = cSplittedRoadFeatures.begin(); itItem != cSplittedRoadFeatures.end(); ++itItem )
		{
			if( lOID == itItem->GetOID() )
				continue;	// �������̂͏Ȃ�

			// �O���e�[�u���ɕR�t���Ă�����̂��S�ăR�s�[
			CopyRelation( ipRelTable, std::make_pair( strOuterKeyFieldName, lOID ), itItem->GetOID() );

			// [SJ�S���S�H��]�S���n�̃t�B�[�`���N���X�ł͊֘A�t���O��t�^���Ȃ�
			// [�����x�n�}] ADAM�l�b�g���[�N�̂Ƃ����֘A�t���O�݂Ȃ�
			if( 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_link::kTableName )
				&& 0!=strClassName.CompareNoCase(sindy::schema::sj::railway_node::kTableName )
				&& !AheIsAdamNetwork(strClassName))
			{
				// �֘A�t���O������ꍇ�̓t���O��t�^
				itItem->SetAttribute( *it, 1L );
			}
		}
	}
	return TRUE;
}

void CEditHelper::AddInterior(IPolygon2Ptr pIPolygonBase, IRingPtr pIRingExteriorBase, IGeometryCollectionPtr pIGeometryCollection)
{
	// �����`�F�b�N
	if( ! ( pIPolygonBase && pIRingExteriorBase && pIGeometryCollection ) )
	{
		_ASSERTE( pIPolygonBase && pIRingExteriorBase && pIGeometryCollection );
		return;
	}

	ISpatialReferencePtr ipSpRef( AheGetSpatialReference( pIPolygonBase ) );

	IGeometryCollectionPtr pIGeometryCollectionSrc( CLSID_Polygon );
	((IGeometryPtr)pIGeometryCollectionSrc)->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
	pIGeometryCollectionSrc->AddGeometry(pIRingExteriorBase);
	
	if( pIPolygonBase )
	{
		LONG lExteriorCount = 0;
		pIPolygonBase->get_ExteriorRingCount(&lExteriorCount);

		/// ������Ń������m��
		IRingPtr *parrIRingExterior = new IRingPtr[ lExteriorCount ];

		pIPolygonBase->QueryExteriorRingsEx(lExteriorCount, reinterpret_cast<IRing**>(parrIRingExterior));

		for ( int l = 0; l < lExteriorCount; l++ )
		{
			LONG lInteriorCount = 0;
			pIPolygonBase->get_InteriorRingCount(parrIRingExterior[l], &lInteriorCount);

			if ( 0 < lInteriorCount ) {
				/// ������Ń������m��
				IRingPtr* parrIRingInterior = new IRingPtr[ lInteriorCount ];

				pIPolygonBase->QueryInteriorRingsEx(parrIRingExterior[l], lInteriorCount, reinterpret_cast<IRing**>(parrIRingInterior));

				for ( int m = 0; m < lInteriorCount; m++ )
				{
					IGeometryCollectionPtr pIGeometryCollectionDest( CLSID_Polygon );
					((IGeometryPtr)pIGeometryCollectionDest)->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
					pIGeometryCollectionDest->AddGeometry(parrIRingInterior[m]);

					if( AheIsContain3( pIGeometryCollectionSrc, pIGeometryCollectionDest ) )
						pIGeometryCollection->AddGeometry(parrIRingInterior[m]);
				}

				delete [] parrIRingInterior;
			}
		}

		delete [] parrIRingExterior;
	}

	return;
}

BOOL CEditHelper::SplitMultiPolygonToSimplePolygons(ILayerPtr pILayer, CTargetItems& cSource, CTargetItems& cDest)
{
	ITopologicalOperatorPtr pITopologicalOperator;
	IPolygon2Ptr pIPolygon, pINewPolygon;
	IFeaturePtr pIFeature;
	IGeometryPtr pIGeometry, pIGeometryPolygon, pIGeometryTmp, pIGeometryExterior, pIGeometryResult;
	IGeometryCollectionPtr pIGeometryCollectionPolygon, pIGeometryCollectionNewPolygon;
	ISpatialReferencePtr pISpatialReference;
	IRingPtr* parrIRingExterior = NULL;
	std::list< CAdapt< IGeometryPtr> > listGeometry;
	LONG lGeometryNewCount = 0, lOuterCount = 0;
	BOOL bFlg = FALSE;

	for ( CTargetItems::iterator it = cSource.begin(); it != cSource.end(); it++ ) {
		pIGeometry = it->GetShape();
		pIFeature = it->GetFeature();

#ifdef _DEBUG
		LONG OID = 0;
		OID = (*it).GetOID();
		ATLTRACE(_T("OID = %d\n"), OID);
#endif
		pITopologicalOperator = pIGeometry;
		pIGeometry->get_SpatialReference(&pISpatialReference);

		pIPolygon = pIGeometry;
		pIGeometryCollectionPolygon = pIPolygon;

		if ( pIPolygon ) {
			pIPolygon->get_ExteriorRingCount(&lOuterCount);

			if ( lOuterCount < 2 ) continue;
			
			/// ������Ń������m��
			parrIRingExterior = new IRingPtr[ lOuterCount - 1 ];

			ATLTRACE(_T("Ring Outer : %d\n"), lOuterCount);

			pIPolygon->QueryExteriorRingsEx(lOuterCount - 1, reinterpret_cast<IRing**>(parrIRingExterior));

			for ( long l = 0; l < lOuterCount - 1; l++ ) {
				pINewPolygon.CreateInstance(CLSID_Polygon);
				pINewPolygon->putref_SpatialReference(pISpatialReference); // ��ԎQ�ƕt�^

				pIGeometryCollectionNewPolygon = pINewPolygon;

				pIGeometryCollectionNewPolygon->get_GeometryCount(&lGeometryNewCount);
				pIGeometryCollectionNewPolygon->RemoveGeometries(0, lGeometryNewCount);

				pIGeometryExterior = parrIRingExterior[l];
				
				if ( pIGeometryExterior ) {
					pIGeometryCollectionNewPolygon->AddGeometry(pIGeometryExterior);
#ifdef _DEBUG
					AheDrawPolygon(GetFocusMapScreenDisplay(), pIGeometryExterior, RGB(0,0,255), 3, esriSLSSolid, TRUE);
#endif
				}

				AddInterior(pIPolygon, parrIRingExterior[l], pIGeometryCollectionNewPolygon);

				IFeaturePtr pIFeatureNewPolygon;
				IFeatureClassPtr pIPolygonFeatureClass;

				AheGetFeatureClass(GetFocusMap(), pILayer, &pIPolygonFeatureClass);

				if ( pIPolygonFeatureClass ) {
					pIPolygonFeatureClass->CreateFeature(&pIFeatureNewPolygon);

					/// �f�t�H���g�l������
					AheSetDefaultData(pIFeatureNewPolygon);

					AheCopyAttribute(pIFeature, pIFeatureNewPolygon, TRUE );	// [bug 4043] �������[�h�ŃR�s�[

					AheSetModifyData(pIFeatureNewPolygon, sindyUpdateTypeCreate);

					pIGeometryCollectionNewPolygon->get_GeometryCount(&lGeometryNewCount);
					pINewPolygon = pIGeometryCollectionNewPolygon;

					pIGeometryPolygon = pINewPolygon;
					pIGeometryPolygon->putref_SpatialReference(pISpatialReference);
					pIGeometryPolygon->SnapToSpatialReference();

					if ( pIFeatureNewPolygon ) {
						pIFeatureNewPolygon->putref_Shape(pIGeometryPolygon);
						cDest.push_back( pIFeatureNewPolygon );
						cDest.Store();

						// �������߂̐}�`�����
						listGeometry.push_back( pIGeometryPolygon );
						bFlg = TRUE;
					}
				}
			}

			delete[] parrIRingExterior;

			/// ���̃|���S������A�V�K���o�����폜
			for ( std::list< CAdapt< IGeometryPtr> >::iterator it2 = listGeometry.begin(); it2 != listGeometry.end(); it2++ ) 
			{
				pIGeometryTmp = it2->m_T;

				if ( it2 == listGeometry.begin() ) {
					pIGeometryTmp->putref_SpatialReference(pISpatialReference);
				} else {
					pIGeometryResult->putref_SpatialReference(pISpatialReference);
					pITopologicalOperator = pIGeometryResult;
				}
				
				pITopologicalOperator->Difference(pIGeometryTmp, &pIGeometryResult);
				pITopologicalOperator->Simplify();
			}

			(*it).SetShape(pIGeometryResult);
			(*it).SetModifyData(sindyUpdateTypeShape);
		}
	}

	return bFlg;
}

int CEditHelper::CreateInteriorPolygon(ILayer* pILayer, IPoint* pIPoint, IGeometry** pIGeometry)
{
	CTargetItems cNearPolygons;
	int nErr = 0;

	// �w��|�C���g�Ƀ|���S��������ꍇ�͏I��
	long lNum = AheSelectByShapeFromLayer( pIPoint, cNearPolygons, pILayer, esriSpatialRelIntersects);
	if( 0 < lNum )
		return -2; 

	// ��ԎQ�Ƃ̎擾
	ISpatialReferencePtr ipSpRef;
	pIPoint->get_SpatialReference( &ipSpRef );

	// ��ʂ̃T�C�Y���擾
	IDisplayTransformationPtr ipDispTrans;
	IEnvelopePtr ipDisplayEnvelope;

	GetFocusMapScreenDisplay()->get_DisplayTransformation( &ipDispTrans );
	ipDispTrans->get_VisibleBounds( &ipDisplayEnvelope );
	ipDisplayEnvelope->putref_SpatialReference( ipSpRef );
	IGeometryPtr ipGeomSearch = AheEnvelope2Polygon( ipDisplayEnvelope );
	if( ipGeomSearch )
	{
		cNearPolygons.clear();

		/// �͈͓��̃|���S��������
		long lCount = AheSelectByShapeFromLayer( ipGeomSearch, cNearPolygons, pILayer, esriSpatialRelIntersects);
	
		if( 0 < lCount )
		{
			/// �쐬
			ATLTRACE(_T("Search Item = %d\n"), cNearPolygons.size());

			IGeometryPtr ipGeomBase;
			ITopologicalOperatorPtr ipTopo;
			for( CTargetItems::iterator it = cNearPolygons.begin(); it != cNearPolygons.end(); it++ )
			{
				IGeometryPtr ipGeomNearPolygon = it->GetShapeCopy();
#ifdef _DEBUG				
				long lOID = (*it).GetOID();
				ATLTRACE(_T("Object ID %d\n"), lOID);
#endif
				if( it == cNearPolygons.begin() )
					ipGeomBase = ipGeomNearPolygon;
				else
				{
					ipTopo = ipGeomBase;
					ipTopo->Union( ipGeomNearPolygon, &ipGeomBase );
				}
			}

			ipTopo = ipGeomSearch;
			IGeometryPtr ipGeomResult;
			try {
				// �����J���Ă���Ƃ�����|���S���������邽�߁A��ʑS�̂̃|���S���Ƃ�Diff���擾����
				ipTopo->Difference( ipGeomBase, &ipGeomResult );
			}
			catch( _com_error e)
			{
				return -1;
			}
			IPolygon4Ptr ipPolygonRing = ipGeomResult;
			if( ipPolygonRing )
			{
				// �}���`�|���S�����V���O���|���S���ɕϊ�
				long lPolyCount = 0;
				ipPolygonRing->get_ExteriorRingCount( &lPolyCount );

				IGeometryBagPtr ipGeoBag;
				ipPolygonRing->get_ConnectedComponentBag( &ipGeoBag );

				IGeometryCollectionPtr ipGeoCollection( ipGeoBag );
				if( !ipGeoCollection )
					return -1;

				long lGeoCnt = 0;
				ipGeoCollection->get_GeometryCount( &lGeoCnt );
				for( long l = 0; l < lGeoCnt; ++l )
				{
					IGeometryPtr ipTmpGeom;
					ipGeoCollection->get_Geometry( l, &ipTmpGeom );
					IPolygonPtr ipNewPolygon = ipTmpGeom;
					if( ipNewPolygon )
					{
						ipNewPolygon->putref_SpatialReference( ipSpRef );
						if( AheIsContain3( ipNewPolygon, pIPoint ) )
						{
							/// �������쐬����Ώ�
							AheDrawPolygon( GetFocusMapScreenDisplay(), ipNewPolygon, RGB(0,255,0) , 2, esriSLSSolid, TRUE );
							int nRet = AheMessageBox( CONFIRM_FILL_THIS_CLOSED_RING, FILL_METHOD, MB_YESNO | MB_ICONQUESTION , WARNLEVEL_HIGH );				
							if( IDYES == nRet ) 
							{
								*pIGeometry = ipNewPolygon;
								if( *pIGeometry ) (*pIGeometry)->AddRef();
								nErr = 1;
							} 
							else
							{
								*pIGeometry = NULL;
								nErr = -3;
							}
							break;
						}
					}
				}
			}
		}
	}

	return nErr;
}

BOOL CEditHelper::CutOutPolygonByPolygon( CTargetItems& cItems, CTargetItems& cModifyItems, bool bAskOverlap /*=true*/ )
{
	for( CTargetItems::iterator it = cItems.begin(); it != cItems.end(); it++ )
	{
		// �d�������C�����������΂�
		if( it->GetGeometryType() != esriGeometryPolygon ||
			AheIsOverlapableLayer( it->GetFeatureClassName() ) ) 
			continue;

		ISpatialReferencePtr ipSpRef = AheGetSpatialReference( it->GetFeature() );

		// �`��������o�b�t�@�����O
		double dBuffer = AheGetXYDomain1UnitLength( ipSpRef );
		ITopologicalOperatorPtr ipTopo( it->GetShapeCopy() );
		IGeometryPtr ipBufferGeom;
		if ( ipTopo )
			ipTopo->Buffer( dBuffer, &ipBufferGeom );
		else
			ipBufferGeom = it->GetShapeCopy();

		// �o�b�t�@�����O�����`�󂩂瓯���t�B�[�`���N���X�̃|���S��������
		CTargetItems cSearchResult;
		AheSelectByShapeFromFeatureClass( ipBufferGeom, cSearchResult, it->GetFeatureClass() );

		CTargetItems cShapeEditItems;	// �`��ύX����t�B�[�`���̃��X�g
		CTargetItems cTouchItems;		// �쐬�`���Touch�������Ă���t�B�[�`���̃��X�g
		for( CTargetItems::iterator itSearchResult = cSearchResult.begin(); itSearchResult != cSearchResult.end(); itSearchResult++ )
		{
			// �������̂͂͂���
			bool bFind = false;
			for( CTargetItems::iterator itFind = cItems.begin(); itFind != cItems.end(); itFind++ )
			{
				if( *itSearchResult == *itFind )
				{
					bFind = true;
					break;
				}
			}
			if( bFind )	continue;

			///////
			/// ITopologicalOperator->Difference(), Union(),etc������Əo�͌`��͋�ԎQ�ƂɃt�B�b�g����Ă��܂��̂�
			/// ��L�����̑O�ɁABase�`��AAdd�`��Ƃ̐ړ_��_���A�\���_�Ƃ��Ă��ꂼ��ɒǉ�����K�v������
			/// �iXOR�쐬���������łȂ��ʏ�|���S���쐬���ɂ��K�v�ȏ����j[bug5485]
			///////
			// �����`��ƐV�K�쐬�`��Ƃ̌�_�A�ړ_���擾
			IGeometryPtr ipGeomPointCol = AheGetCrossOrTouchPoints( itSearchResult->GetShapeCopy(), it->GetShapeCopy(), 2 );
			// �擾�����|�C���g�Q��V�K�쐬�`���ɒǉ�
			AheAddVertexesIfOnGeometry( it->GetShape(), ipGeomPointCol, ipSpRef );
			// �擾�����|�C���g�Q�������`���ɒǉ�
			if( AheAddVertexesIfOnGeometry( itSearchResult->GetShape(), ipGeomPointCol, ipSpRef ) )
			{
				// Touch���Ă��邾���̃t�B�[�`���͏d�Ȃ蔻��͂��Ȃ��̂ŕʂɎ���Ă���
				// (�����ɂ͌����`��̍\���_������Touch���Ă���`��͓����Ă��Ȃ�)
				if( AheIsTouch( AheSetKnownSimple( it->GetShapeCopy() ), itSearchResult->GetShapeCopy(), ipSpRef ) )
					cTouchItems.push_back( *itSearchResult );
				else
					cShapeEditItems.push_back( *itSearchResult );
				continue;
			}
			// AheAddVertexesIfOnGeometry()�ł��ꂽ�t�B�[�`�����擾(��L�������ƌ����t�B�[�`�������Ă���ꍇ�ƁA�����t�B�[�`���ɓ����Ă���ꍇ�̏����͎擾�ł��Ȃ�����)
			// ��L�������ƌ����t�B�[�`���̍\���_������Touch����Overlap���Ă���ꍇ�͎擾�ł��Ȃ�����[bug5644]
			if( AheIsContain( AheSetKnownSimple( it->GetShapeCopy() ), itSearchResult->GetShapeCopy(), ipSpRef )
				|| AheIsWithin( AheSetKnownSimple( it->GetShapeCopy() ), itSearchResult->GetShapeCopy(), ipSpRef )
				|| AheIsOverlap( AheSetKnownSimple( it->GetShapeCopy() ), itSearchResult->GetShapeCopy(), ipSpRef ))	//[bug5644]overlap�ǉ�
				cShapeEditItems.push_back( *itSearchResult );
		}

		// �X����ǉ�
		for( CTargetItems::iterator itTouchItems = cTouchItems.begin(); itTouchItems != cTouchItems.end(); itTouchItems++ )
		{
			// �ҏW�\�t�B�[�`���łȂ���ΏI��	[bug5508]
			if( AheIsEditableFeature2( GetFocusMap(), itTouchItems->GetFeature(), NULL, sindyeEditMove, m_ipRule, FALSE, GetHWND() ) )
			{
				// ����ǉ��A�`��K�p
				itTouchItems->SetModifyData( sindyUpdateTypeShape );
				itTouchItems->SetShape( itTouchItems->GetShape() );
				cModifyItems.push_back( *itTouchItems );
			}
			else
				return FALSE;
		}

		// �t�B�[�`�����I���ł����ꍇ�͏d��������邩����
		if( !cShapeEditItems.empty() )
		{
			// ������邩�ǂ���
			if( bAskOverlap )
				if( MessageBox( GetHWND(), CONFIRM_CHANGE_SHAPE_AND_EVADE_HEAP, CREATING_POLYGON, MB_YESNO|MB_ICONQUESTION ) == IDNO )
					return FALSE;

			for( CTargetItems::iterator itShapeEditItems = cShapeEditItems.begin(); itShapeEditItems != cShapeEditItems.end(); itShapeEditItems++ )
			{
				// �ҏW�\�t�B�[�`���łȂ���ΏI��	[bug5508]
				if( AheIsEditableFeature2( GetFocusMap(), itShapeEditItems->GetFeature(), NULL, sindyeEditMove, m_ipRule, FALSE, GetHWND() ) )
				{
					ITopologicalOperatorPtr ipBaseTopo( AheSetKnownSimple( itShapeEditItems->GetShapeCopy() ) );
					if( ipBaseTopo )
					{
						// �����`����x�[�X�Ƃ��ĐV�K�쐬�`��Ƃ�Difference���擾
						IGeometryPtr ipDiffGeom;
						ipBaseTopo->Difference( it->GetShapeCopy(), &ipDiffGeom );
						if( ipDiffGeom )
						{
							VARIANT_BOOL vaIsEmpty = VARIANT_FALSE;
							ipDiffGeom->get_IsEmpty( &vaIsEmpty );

							// Difference�`�󂪂Ȃ���΍폜
							if( vaIsEmpty )
							{
								// �d���`��NULL�Ȃ�폜���邪�A�폜�Ώۃ��C�����`�F�b�N���K�v[bug2357]
								if( AheIsEditableFeature2( GetFocusMap(), itShapeEditItems->GetFeature(), NULL, sindyeEditDelete, GetRule(), FALSE, GetHWND() )) 
									itShapeEditItems->Delete();
								else
									return FALSE;
							}
							else
							{
								// ����ǉ��A�`��K�p
								itShapeEditItems->SetModifyData( sindyUpdateTypeShape );
								itShapeEditItems->SetShape( ipDiffGeom );
								cModifyItems.push_back( *itShapeEditItems );
							}
						}
					}
				}
				else
					return FALSE;
			}
		}
	}

	return TRUE;
}

IPointCollectionPtr CEditHelper::GetCrossOrTouchPoints2( CTargetItems& cDestItems, CTargetItems& cSrcItems, INT iMode/* = 2*/, CTargetItems* pNoRelItems/* = NULL*/ )
{
	// �����|�C���g��T��
	std::list<LongPair> 	listChecked;
	ITopologicalOperatorPtr ipTopo;
	IPointCollectionPtr 	ipCrossPointCol, ipIntersectPoints;	// �Ԃ�l
	IGeometryPtr			ipDestGeom;

	for( CTargetItems::iterator itDest = cDestItems.begin(); itDest != cDestItems.end(); itDest++ ) {
		LONG	lDestOID = itDest->GetOID();
		BOOL	bFindRelItems = FALSE;		// ��_�E�ړ_��������ꂽ���ǂ���

		ipDestGeom = itDest->GetShape();
		for( CTargetItems::iterator itSrc = cSrcItems.begin(); itSrc != cSrcItems.end(); itSrc++ ) {
			// �����t�B�[�`�����m�͂͂���
			if ( *itSrc == *itDest )
				continue;

			// ���ɓ����y�A�����������ǂ����`�F�b�N
			LONG	lSrcOID = itSrc->GetOID();
			BOOL	TorF = FALSE;

			for ( std::list<LongPair>::iterator it = listChecked.begin(); it != listChecked.end(); it++ ) {
				if( ( it->lID1 == lSrcOID && it->lID2 == lDestOID ) || ( it->lID1 == lDestOID && it->lID2 == lSrcOID ) ) {
					TorF = TRUE;
					break;
				}
			}
			if ( TorF == TRUE )
				continue;

			LongPair pair = { lSrcOID, lDestOID };
			listChecked.push_back( pair );

			ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints() : ��r�F%d AND %d\n"), lSrcOID, lDestOID );

			ipIntersectPoints = GetCrossOrTouchPoints2( ipDestGeom, itSrc->GetShape(), iMode );
			if ( ipIntersectPoints != NULL ) {
				long		Count;

				ipIntersectPoints->get_PointCount( &Count );
				if ( Count >= 1 ) {
					WKSPoint*	wks = NULL;

					wks = new WKSPoint[Count];
					ipIntersectPoints->QueryWKSPoints( 0, Count, wks );

					if ( ipCrossPointCol == NULL )
					{
						ipCrossPointCol.CreateInstance( CLSID_Multipoint );
						((IGeometryPtr)ipCrossPointCol)->putref_SpatialReference( AheGetSpatialReference( itSrc->GetSpatialReference() ) ); // ��ԎQ�ƕt�^
					}

					ipCrossPointCol->AddWKSPoints( Count, wks );

					delete [] wks;

					bFindRelItems = TRUE;
				}
			}
		}

		// pNoRelItems ���n����Ă���ꍇ�� DestItems �̒��Ō�_�E�ړ_�������邱�Ƃ��ł��Ȃ��������̂����Ă���
		if ( pNoRelItems != NULL ) {
			if ( bFindRelItems == FALSE )
				pNoRelItems->push_back( *itDest );
		}
	}

	ipTopo = ipCrossPointCol;
	if ( ipTopo != NULL )
		ipTopo->Simplify();	// �d���_�͎�菜��
	return ipCrossPointCol;
}

IGeometryPtr CEditHelper::GetCrossOrTouchPoints2( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, INT iMode/* = 2*//*, BOOL bTouchSideIsCross*//* = FALSE*/ )
{
	if( ipBaseGeometry == NULL || ipTestGeometry == NULL )
		return NULL;

	IGeometryPtr 				ipBaseGeomCopy, ipTestGeomCopy;
	ISegmentCollectionPtr 		ipBaseSegColCopy, ipTestSegColCopy;
	IPointCollectionPtr 		ipIntersectPoints;
	IEnumSegmentPtr				ipEnumSegment, ipTestEnumSegment;
	_ISegmentPtr 				ipBaseSegTmp, ipBaseSeg, ipTestSegTmp, ipTestSeg, ipPrevBaseSeg, ipNextBaseSeg, ipPrevTestSeg, ipNextTestSeg;
	IPointPtr					ipPrevBaseSegFromPoint, ipPrevBaseSegToPoint, ipNextBaseSegFromPoint, ipNextBaseSegToPoint;
	IPointPtr 					ipPrevTestSegFromPoint, ipPrevTestSegToPoint, ipNextTestSegFromPoint, ipNextTestSegToPoint;
	sindyeSpatialRel 			enumSpRel;
	DOUBLE 						dTorelanceMin, dTorelanceMax, dTorelanceMaxMin, dTorelanceMinMax, dTorelance, dBaseFromX = 0.0f, dBaseFromY = 0.0f, dBaseToX = 0.0f, dBaseToY = 0.0f, dTestFromX = 0.0f, dTestFromY = 0.0f, dTestToX = 0.0f, dTestToY = 0.0f;
	LONG 						lBaseGeomCount = -1, lTestGeomCount = -1, lBaseSegColCount, lTestSegColCount, lPartIndex, lVertexIndex, lBaseIndex, lTestIndex;

	switch( iMode ) {
		case 0:	 enumSpRel = sindyeSpatialRelCross; break;												// Cross
		case 1:  enumSpRel = sindyeSpatialRelTouch; break;												// Touch
		case 2:  enumSpRel = (sindyeSpatialRel)(sindyeSpatialRelCross|sindyeSpatialRelTouch); break;	// Cross and Touch
		default: enumSpRel = sindyeSpatialRelNone;	break;
	}

	// ��ԎQ�Ǝ擾
	ISpatialReferencePtr ipBaseSpRef, ipTestSpRef;
	ipBaseGeometry->get_SpatialReference( &ipBaseSpRef );
	ipTestGeometry->get_SpatialReference( &ipTestSpRef );

	IPolylinePtr ipBaseSegPolyline( CLSID_Polyline ), ipTestSegPolyline( CLSID_Polyline );
	ipBaseSegPolyline->putref_SpatialReference( ipBaseSpRef ); // ��ԎQ�ƕt�^
	ipTestSegPolyline->putref_SpatialReference( ipTestSpRef ); // ��ԎQ�ƕt�^
	IPointPtr ipBaseFromPoint( CLSID_Point ), ipBaseToPoint( CLSID_Point ), ipTestFromPoint( CLSID_Point ), ipTestToPoint( CLSID_Point );
	ipBaseFromPoint->putref_SpatialReference( ipBaseSpRef ); // ��ԎQ�ƕt�^
	ipBaseToPoint->putref_SpatialReference( ipBaseSpRef ); // ��ԎQ�ƕt�^
	ipTestFromPoint->putref_SpatialReference( ipTestSpRef ); // ��ԎQ�ƕt�^
	ipTestToPoint->putref_SpatialReference( ipTestSpRef ); // ��ԎQ�ƕt�^
	// �����̋�ԎQ�Ƃׂ̍����ق����炵�����l������o��
	ISpatialReferencePtr ipMinimumSpRef ( AheGetMinimumSpatialReference( ipBaseSpRef, ipTestSpRef ) );
	dTorelance = AheGetXYDomain1UnitLength( ipMinimumSpRef );
	dTorelanceMin = -dTorelance;
	dTorelanceMax = 1.0f + dTorelance;
	dTorelanceMinMax = dTorelance;
	dTorelanceMaxMin = 1.0f - dTorelance;

	// ��x�W�I���g���R���N�V�����ɗ��Ƃ��i�j
	IGeometryCollectionPtr ipBaseGeomColCopy( AheGetClone( ipBaseGeometry ) );
	IGeometryCollectionPtr ipTestGeomColCopy( AheGetClone( ipTestGeometry ) );

	if( ipBaseGeomColCopy == NULL || ipTestGeomColCopy == NULL ) {
		_ASSERTE( ipBaseGeomColCopy != NULL && ipTestGeomColCopy != NULL );
		return NULL;
	}

	// �����|�C���g��T��
	ipIntersectPoints.CreateInstance( CLSID_Multipoint );
	((IGeometryPtr)ipIntersectPoints)->putref_SpatialReference( ipBaseSpRef ); // �x�[�X�̋�ԎQ�ƕt�^
	ipBaseGeomColCopy->get_GeometryCount( &lBaseGeomCount );
	ipTestGeomColCopy->get_GeometryCount( &lTestGeomCount );

	// Base �̃W�I���g�����Ƃ̃��[�v
	ipBaseSegColCopy = AheGetClone( ipBaseGeometry );
	if ( ipBaseSegColCopy == NULL ) {
		_ASSERTE( ipBaseSegColCopy != NULL );
		return NULL;
	}

	// Base �̃Z�O�����g���Ƃ̃��[�v
	lBaseIndex = 0;
	ipBaseSegColCopy->get_SegmentCount( &lBaseSegColCount );
	ipBaseSegColCopy->get_EnumSegments( &ipEnumSegment );
	ipEnumSegment->Reset();
	while( S_OK == ipEnumSegment->Next( &ipBaseSegTmp, &lPartIndex, &lVertexIndex ) ) {
		dBaseFromX = dBaseFromY = dBaseToX = dBaseToY = 0.0f;

		ipBaseSeg = AheGetClone( (_ISegmentPtr)ipBaseSegTmp );
		if ( ipBaseSeg == NULL ) {
			lBaseIndex++;
			continue;
		}

		// �|�C���g������
		ipBaseFromPoint->PutCoords( 0.0f, 0.0f );
		ipBaseToPoint->PutCoords( 0.0f, 0.0f );

		if( FAILED( ipBaseSeg->QueryFromPoint( ipBaseFromPoint ) ) || FAILED( ipBaseSeg->QueryToPoint( ipBaseToPoint ) ) ) {
			ATLASSERT( FALSE );
			AheTraceErrorMessage( _T("GetCrossOrTouchPoints2()") );
			lBaseIndex++;
			continue;
		}

		ipBaseFromPoint->QueryCoords( &dBaseFromX, &dBaseFromY );
		ipBaseToPoint->QueryCoords( &dBaseToX, &dBaseToY );

		// �Z�O�����g -> �|�����C���ϊ�
		ipBaseSegPolyline->SetEmpty();
		((IPointCollectionPtr)ipBaseSegPolyline)->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
		((IPointCollectionPtr)ipBaseSegPolyline)->AddPoint( (IPointPtr)AheGetClone( ipBaseToPoint ) );

		DOUBLE Ax = (DOUBLE)( dBaseToX - dBaseFromX );
		DOUBLE Ay = (DOUBLE)( dBaseToY - dBaseFromY );

		// Test �̃W�I���g�����Ƃ̃��[�v
		ipTestSegColCopy = AheGetClone( ipTestGeometry );
		if ( ipTestSegColCopy == NULL ) {
			_ASSERTE( ipTestSegColCopy != NULL );
			continue;
		}

		// Test �̃Z�O�����g���Ƃ̃��[�v
		lTestIndex = 0;
		ipTestSegColCopy->get_SegmentCount( &lTestSegColCount );
		ipTestSegColCopy->get_EnumSegments( &ipTestEnumSegment );
		ipTestEnumSegment->Reset();
		while( S_OK == ipTestEnumSegment->Next( &ipTestSegTmp, &lPartIndex, &lVertexIndex ) ) {
			dTestFromX = dTestFromY = dTestToX = dTestToY = 0.0f;

			// �R�s�[�����
			ipTestSeg = AheGetClone( (_ISegmentPtr)ipTestSegTmp );
			if( ipTestSeg == NULL ) {
				lTestIndex++;
				continue;
			}

			// �|�C���g������
			ipTestFromPoint->PutCoords( 0.0f, 0.0f );
			ipTestToPoint->PutCoords( 0.0f, 0.0f );

			if( FAILED( ipTestSeg->QueryFromPoint( ipTestFromPoint ) ) || FAILED( ipTestSeg->QueryToPoint( ipTestToPoint ) ) ) {
				ATLASSERT( FALSE );
				AheTraceErrorMessage( _T("GetCrossOrTouchPoints2()") );
				lTestIndex++;
				continue;
			}

			ipTestFromPoint->QueryCoords( &dTestFromX, &dTestFromY );
			ipTestToPoint->QueryCoords( &dTestToX, &dTestToY );

			ipTestSegPolyline->SetEmpty();
			((IPointCollectionPtr)ipTestSegPolyline)->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
			((IPointCollectionPtr)ipTestSegPolyline)->AddPoint( (IPointPtr)AheGetClone( ipTestToPoint ) );

			// �Z�O�����g�P�ʂŕ��s�A�ڐG�A���͊��S�ɗ���Ă��邩�ǂ����̃`�F�b�N
			// Base �� Test �� FromTo �ŁATest �� Base �� FromTo �ŃX�v���b�g���Ă݂āA�ł����畽�s�A�ڐG�ł͂Ȃ����͗���Ă��Ȃ�
			if( ipBaseSegPolyline != NULL && ipTestSegPolyline != NULL ) {
				// �[�_���m���ڂ��Ă��邩�ǂ���
				BOOL bBaseFromIsTestFrom = AheIsEqual( ipBaseFromPoint, ipTestFromPoint );
				BOOL bBaseFromIsTestTo   = AheIsEqual( ipBaseFromPoint, ipTestToPoint );
				BOOL bBaseToIsTestFrom   = AheIsEqual( ipBaseToPoint, ipTestFromPoint );
				BOOL bBaseToIsTestTo     = AheIsEqual( ipBaseToPoint, ipTestToPoint );
				// ���S��v���ǂ����i���S��v�̏ꍇ�� Touch �Ƃ݂Ȃ��j
				if( bBaseFromIsTestFrom == TRUE && bBaseFromIsTestTo == TRUE && bBaseToIsTestFrom == TRUE && bBaseToIsTestTo == TRUE ) {
					if( enumSpRel & sindyeSpatialRelTouch ) {
						ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
						ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
						ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j��v�F                         : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j��v�F                         : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseToX, dBaseToY );
					}
					lTestIndex++;
					continue;	// ���S��v�Ȃ炱���ŏI���
				}

				// �[�_���Z�O�����g��ɑ��݂��Ă��邩�ǂ���
				VARIANT_BOOL vbBaseIsSplittedByTestFrom = ( AhePointIsOnSegment( ipBaseSeg, ipTestFromPoint, ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
				VARIANT_BOOL vbBaseIsSplittedByTestTo   = ( AhePointIsOnSegment( ipBaseSeg, ipTestToPoint,   ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
				VARIANT_BOOL vbTestIsSplittedByBaseFrom = ( AhePointIsOnSegment( ipTestSeg, ipBaseFromPoint, ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
				VARIANT_BOOL vbTestIsSplittedByBaseTo   = ( AhePointIsOnSegment( ipTestSeg, ipBaseToPoint,   ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

				// ���s���ǂ����i���s�̏ꍇ�� Touch �Ƃ݂Ȃ��j
				if( ( bBaseFromIsTestFrom == TRUE && ( vbBaseIsSplittedByTestTo   == VARIANT_TRUE || vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) ) ||
					( bBaseFromIsTestTo   == TRUE && ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) ) ||
					( bBaseToIsTestFrom   == TRUE && ( vbBaseIsSplittedByTestTo   == VARIANT_TRUE || vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) ) ||
					( bBaseToIsTestTo     == TRUE && ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) ) ||
					( bBaseFromIsTestFrom == FALSE && bBaseFromIsTestTo == FALSE && bBaseToIsTestFrom == FALSE && bBaseToIsTestTo == FALSE && 
					  ( ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE && vbBaseIsSplittedByTestTo == VARIANT_TRUE ) ||
					    ( vbTestIsSplittedByBaseFrom == VARIANT_TRUE && vbTestIsSplittedByBaseTo == VARIANT_TRUE ) ||
					    ( ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbBaseIsSplittedByTestTo == VARIANT_TRUE ) &&
					      ( vbTestIsSplittedByBaseFrom == VARIANT_TRUE || vbTestIsSplittedByBaseTo == VARIANT_TRUE ) ) ) ) ) {
					if( enumSpRel & sindyeSpatialRelTouch ) {
						// ���s�̏ꍇ
						if( vbBaseIsSplittedByTestFrom == VARIANT_TRUE ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j���s�FBaseIsSplittedByTestFrom : X=%.15lf Y=%.15lf S=-- T=--\n"), dTestFromX, dTestFromY );
						}

						if( vbBaseIsSplittedByTestTo == VARIANT_TRUE ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j���s�FBaseIsSplittedByTestTo   : X=%.15lf Y=%.15lf S=-- T=--\n"), dTestToX, dTestToY );
						}

						if( vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j���s�FTestIsSplittedByBaseFrom : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						}

						if( vbTestIsSplittedByBaseTo == VARIANT_TRUE ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j���s�FTestIsSplittedByBaseTo   : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseToX, dBaseToY );
						}
					}
					lTestIndex++;
					continue;	// ���s�Ȃ炱���ŏI���
				}

				// ��O�y�ш��� Base�ATest �̃Z�O�����g�� From�ATo ���擾
				if ( lBaseIndex > 0 ) {
					ipBaseSegColCopy->get_Segment( lBaseIndex + 1, &ipPrevBaseSeg );
					if ( ipPrevBaseSeg != NULL ) {
						ipPrevBaseSeg->get_FromPoint( &ipPrevBaseSegFromPoint );
						ipPrevBaseSeg->get_ToPoint( &ipPrevBaseSegToPoint );
					}
				}

				if( lBaseIndex < lBaseSegColCount - 1 ) {
					ipBaseSegColCopy->get_Segment( lBaseIndex - 1, &ipNextBaseSeg );
					if ( ipNextBaseSeg != NULL ) {
						ipNextBaseSeg->get_FromPoint( &ipNextBaseSegFromPoint );
						ipNextBaseSeg->get_ToPoint( &ipNextBaseSegToPoint );
					}
				}

				if( lTestIndex > 0 ) {
					ipTestSegColCopy->get_Segment( lTestIndex + 1, &ipPrevTestSeg );
					if ( ipPrevTestSeg != NULL ) {
						ipPrevTestSeg->get_FromPoint( &ipPrevTestSegFromPoint );
						ipPrevTestSeg->get_ToPoint( &ipPrevTestSegToPoint );
					}
				}

				if( lTestIndex < lTestSegColCount - 1 ) {
					ipTestSegColCopy->get_Segment( lTestIndex - 1, &ipNextTestSeg );
					if ( ipNextTestSeg != NULL ) {
						ipNextTestSeg->get_FromPoint( &ipNextTestSegFromPoint );
						ipNextTestSeg->get_ToPoint( &ipNextTestSegToPoint );
					}
				}

				// �[�_���m���ڂ��Ă��邪�A��v�ł����s�ł��Ȃ��ꍇ
				if( bBaseFromIsTestFrom == TRUE ) {
					// Base �� From �� Test �� From �ƈ�v���Ă���Ƃ�
					//
					// 1. Base �� To           ... ipBaseToPoint
					// 2. Base �� From         ... ipBaseFromPoint
					// 3. Base �̈�O�� From  ... ipPrevBaseSegFromPoint
					// 4. Test �� To           ... ipTestToPoint
					// 5. Test �̈�O�� From  ... ipPrevTestSegFromPoint
					//
					// 123 �̂Ȃ��p�̊Ԃ� 124�A125 �̊p�x����������܂��͗����Ȃ��ꍇ�� Touch�A�Е��������Ă����Е����Ȃ��ꍇ�� Cross
					if( ipPrevBaseSeg != NULL && ipPrevTestSeg != NULL ) {
						DOUBLE dAngle123 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipTestToPoint          ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevTestSegFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) {
							// �N���X�����ꍇ
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						} else {
							// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i�O�̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						}
					} else {
						// Base ���� Test ���J�n���͍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( bBaseFromIsTestTo == TRUE ) {
					// Base �� From �� Test �� To �ƈ�v���Ă���Ƃ�
					//
					// 1. Base �� To           ... ipBaseToPoint
					// 2. Base �� From         ... ipBaseFromPoint
					// 3. Base �̈�O�� From  ... ipPrevBaseSegFromPoint
					// 4. Test �� From         ... ipTestFromPoint
					// 5. Test �̈��� To    ... ipNextTestSegToPoint
					//
					// 123 �̂Ȃ��p�̊Ԃ� 124�A125 �̊p�x����������܂��͗����Ȃ��ꍇ�� Touch�A�Е��������Ă����Е����Ȃ��ꍇ�� Cross
					if( ipPrevBaseSeg != NULL && ipNextTestSeg != NULL ) {
						DOUBLE dAngle123 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipTestFromPoint        ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipNextTestSegToPoint   ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) {
							// �N���X�����ꍇ
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						} else {
							// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i��̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						}
					} else {
						// Base ���� Test ���J�n���͍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( bBaseToIsTestFrom == TRUE ) {
					// Base �� To �� Test �� From �ƈ�v���Ă���Ƃ�
					//
					// 1. Base �� From         ... ipBaseFromPoint
					// 2. Base �� To           ... ipBaseToPoint
					// 3. Base �̈��� To    ... ipNextBaseSegToPoint
					// 4. Test �� To           ... ipTestToPoint
					// 5. Test �̈�O�� From  ... ipPrevTestSegFromPoint
					//
					// 123 �̂Ȃ��p�̊Ԃ� 124�A125 �̊p�x����������܂��͗����Ȃ��ꍇ�� Touch�A�Е��������Ă����Е����Ȃ��ꍇ�� Cross
					if( ipNextBaseSeg != NULL && ipPrevTestSeg != NULL ) {
						DOUBLE dAngle123 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextBaseSegToPoint   ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipTestToPoint          ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipPrevTestSegFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) {
							// �N���X�����ꍇ
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						} else {
							// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i�O�̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						}
					} else {
						// Base ���� Test ���J�n���͍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( bBaseToIsTestTo == TRUE ) {
					// Base �� To �� Test �� To �ƈ�v���Ă���Ƃ�
					//
					// 1. Base �� From         ... ipBaseFromPoint
					// 2. Base �� To           ... ipBaseToPoint
					// 3. Base �̈��� To    ... ipNextBaseSegToPoint
					// 4. Test �� From         ... ipTestFromPoint
					// 5. Test �̈��� To    ... ipNextTestSegToPoint
					//
					// 123 �̂Ȃ��p�̊Ԃ� 124�A125 �̊p�x����������܂��͗����Ȃ��ꍇ�� Touch�A�Е��������Ă����Е����Ȃ��ꍇ�� Cross
					if( ipNextBaseSeg != NULL && ipNextTestSeg != NULL ) {
						DOUBLE dAngle123 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextBaseSegToPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipTestFromPoint      ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextTestSegToPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) {
							// �N���X�����ꍇ
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						} else {
							// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i��̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						}
					} else {
						// Base ���� Test ���J�n���͍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
						}
					}
					lTestIndex++;
					continue;
				}

				// ��v�ł����s�ł��Ȃ��[�_���ڂ��Ă��Ȃ��ꍇ�ŁA�Е��̂̃Z�O�����g��ɂ����Е��� From ���� To �����݂���Ƃ�
				if( vbBaseIsSplittedByTestFrom == VARIANT_TRUE ) {
					// Base �� Test �� From �ŃX�v���b�g���ꂽ�Ƃ�
					//
					// 1. Base �� From �� To �i�ǂ���ł��ǂ��j... ipBaseFromPoint
					// 2. Test �� From                        ... ipTestFromPoint
					// 3. Test �� To                          ... ipTestToPoint
					// 4. Test �̈�O�� From                ... ipPrevTestSegFromPoint
					//
					// 123 �̂Ȃ��p�� 124 �̂Ȃ��p���擾���A�ǂ���������͕��̊p�x�ł���� Touch�A�ǂ��炩�����łǂ��炩�����ł���� Cross 
					// ��O�� Test ���擾
					if( ipPrevTestSeg != NULL ) {
						DOUBLE dAngle1 = AheGetAngle( ipBaseFromPoint, ipTestFromPoint, ipTestToPoint );
						DOUBLE dAngle2 = AheGetAngle( ipBaseFromPoint, ipTestFromPoint, ipPrevTestSegFromPoint );
						if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) {
							// �N���X�����ꍇ
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
							}
						} else {
							// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i�O�̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
							}
						}
					} else {
						// Test ���J�n�Z�O�����g�Ȃ̂ŁA Touch �Ƃ���i�ⓚ���p�j
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( vbBaseIsSplittedByTestTo   == VARIANT_TRUE ) {
					// Base �� Test �� To �ŃX�v���b�g���ꂽ�Ƃ�
					//
					// 1. Base �� From �� To �i�ǂ���ł��ǂ��j... ipBaseFromPoint
					// 2. Test �� To                          ... ipTestToPoint
					// 3. Test �� From                        ... ipTestFromPoint
					// 4. Test �̈��� To                  ... ipNextTestSegToPoint
					//
					// 123 �̂Ȃ��p�� 124 �̂Ȃ��p���擾���A�ǂ���������͕��̊p�x�ł���� Touch�A�ǂ��炩�����łǂ��炩�����ł���� Cross 
					if( ipNextTestSeg != NULL ) {
						DOUBLE dAngle1 = AheGetAngle( ipBaseFromPoint, ipTestToPoint, ipTestFromPoint );
						DOUBLE dAngle2 = AheGetAngle( ipBaseFromPoint, ipTestToPoint, ipNextTestSegToPoint );
						if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) {
							// �N���X�����ꍇ
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
							}
						} else {
							// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i��̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
							}
						}
					} else {
						// Test �̍Ō�̃Z�O�����g�̏ꍇ�� Touch �Ƃ���i�ⓚ���p�j
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( vbTestIsSplittedByBaseFrom == VARIANT_TRUE )  {
					// Test �� Base �� From �ŃX�v���b�g���ꂽ�Ƃ�
					//
					// 1. Test �� From �� To �i�ǂ���ł��ǂ��j... ipTestFromPoint
					// 2. Base �� From                        ... ipBaseFromPoint
					// 3. Base �� To                          ... ipBaseToPoint
					// 4. Base �̈�O�� From                ... ipPrevBaseSegFromPoint
					//
					// 123 �̂Ȃ��p�� 124 �̂Ȃ��p���擾���A�ǂ���������͕��̊p�x�ł���� Touch�A�ǂ��炩�����łǂ��炩�����ł���� Cross 
					// ��O�� Base ���擾
					if( ipPrevBaseSeg != NULL ) {
						DOUBLE dAngle1 = AheGetAngle( ipTestFromPoint, ipBaseFromPoint, ipBaseToPoint );
						DOUBLE dAngle2 = AheGetAngle( ipTestFromPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint );
						if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) {
							// �N���X�����ꍇ
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						} else {
							// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i�O�̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
							}
						}
					} else {
						// j �� 0 �̏ꍇ�͊J�n�Z�O�����g�Ȃ̂ŁA Touch �Ƃ���i�ⓚ���p�j
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
						}
					}
					lTestIndex++;
					continue;
				}

				if( vbTestIsSplittedByBaseTo   == VARIANT_TRUE )  {
					// Test �� Base �� To �ŃX�v���b�g���ꂽ�Ƃ�
					//
					// 1. Test �� From �� To �i�ǂ���ł��ǂ��j... ipTestFromPoint
					// 2. Base �� To                          ... ipBaseToPoint
					// 3. Base �� From                        ... ipBaseFromPoint
					// 4. Base �̈��� To                  ... ipNextBaseSegToPoint
					//
					// 123 �̂Ȃ��p�� 124 �̂Ȃ��p���擾���A�ǂ���������͕��̊p�x�ł���� Touch�A�ǂ��炩�����łǂ��炩�����ł���� Cross 
					// ���� Base ���擾
					if( ipNextBaseSeg != NULL ) {
						DOUBLE dAngle1 = AheGetAngle( ipTestFromPoint, ipBaseToPoint, ipBaseFromPoint );
						DOUBLE dAngle2 = AheGetAngle( ipTestFromPoint, ipBaseToPoint, ipNextBaseSegToPoint );
						if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) {
							// �N���X�����ꍇ
							if( enumSpRel & sindyeSpatialRelCross ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						} else {
							// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i��̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
							if( enumSpRel & sindyeSpatialRelTouch ) {
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
						}
					} else {
						// i �� Base �̍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
						if( enumSpRel & sindyeSpatialRelTouch ) {
							ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
							ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
						}
					}
					lTestIndex++;
					continue;
				}
			}

			DOUBLE Bx = (DOUBLE)( dTestToX - dTestFromX );
			DOUBLE By = (DOUBLE)( dTestToY - dTestFromY );
			DOUBLE DivideBase = (DOUBLE)( (DOUBLE)( Bx * Ay ) - (DOUBLE)( By * Ax ) );
			if( DivideBase == 0.0f )  {
				ATLTRACE(_T("GetCrossOrTouchPoints2() : ���s�Ȃ̂Ŕ�΂��܂��B") );
				lTestIndex++;
				continue;	// ���s
			}

			DOUBLE t = (DOUBLE)( ( (DOUBLE)(Ax * (DOUBLE)( dTestFromY - dBaseFromY )) - (DOUBLE)(Ay * (DOUBLE)( dTestFromX - dBaseFromX )) ) / DivideBase );
			if( 0.0f < t && t < 1.0f ) {
				DOUBLE s = (DOUBLE)( ( (DOUBLE)(Bx * (DOUBLE)( dBaseFromY - dTestFromY )) - (DOUBLE)(By * (DOUBLE)( dBaseFromX - dTestFromX )) ) / (-DivideBase) );
				if( 0.0f < s && s < 1.0f ) {
					BOOL bAdd = FALSE;
					DOUBLE Px = (DOUBLE)( dBaseFromX + (DOUBLE)(s * Ax) );
					DOUBLE Py = (DOUBLE)( dBaseFromY + (DOUBLE)(s * Ay) );
					if( enumSpRel & sindyeSpatialRelCross ) {
						ATLTRACE(_T("GetCrossOrTouchPoints2() : ��_�iCROSS�j�����FX=%lf Y=%lf S=%lf T=%lf D=%.20lf\n"), Px, Py, s, t, DivideBase );
						bAdd = TRUE;
					}
					if( bAdd )
					{
						if ( ! ipIntersectPoints )
						{
							ipIntersectPoints.CreateInstance( CLSID_Multipoint );
							((IGeometryPtr)ipIntersectPoints)->putref_SpatialReference( ipBaseSpRef ); // �x�[�X�̋�ԎQ�ƕt�^
						}
						IPointPtr ipPoint( CLSID_Point );
						((IGeometryPtr)ipIntersectPoints)->putref_SpatialReference( ipBaseSpRef ); // �x�[�X�̋�ԎQ�ƕt�^
						ipPoint->PutCoords( Px, Py );
						ipIntersectPoints->AddPoint( ipPoint );
					}
				}
			}
			lTestIndex++;
		}
		lBaseIndex++;
	}

	// ����Ȃ���� NULL ��Ԃ�
	if ( ipIntersectPoints != NULL ) {
		LONG 		lIntersectCount = -1;

		ipIntersectPoints->get_PointCount( &lIntersectCount );
		if ( lIntersectCount < 1 )
			ipIntersectPoints = NULL;
	}

	return ipIntersectPoints;
}

BOOL CEditHelper::PointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef/* = NULL*/, BOOL bExeptVertex/* = FALSE*/ )
{
	if( ipBaseGeometry == NULL || ipTestPoint == NULL )
		return FALSE;

	if( ipSpRef == NULL )
		ipBaseGeometry->get_SpatialReference( &ipSpRef );

	esriGeometryType geomType;

	ipBaseGeometry->get_GeometryType( &geomType );
	switch( geomType ) {
		case esriGeometryPoint:
			return AheIsEqual( ipBaseGeometry, ipTestPoint );
			break;
		default:
		{
			IGeometryCollectionPtr 	ipGeomCol( ipBaseGeometry );
			ISegmentCollectionPtr 	ipSegCol;
			_ISegmentPtr			ipSegment;
			IEnumSegmentPtr			ipEnumSegment;
			IGeometryPtr 			ipPartGeom;
			LONG 					lGeomCount, lPartIndex, lVertexIndex;

			if( ipGeomCol == NULL )
				return FALSE;

			ipGeomCol->get_GeometryCount( &lGeomCount );
			for ( LONG i = 0; i < lGeomCount; i++ ) {
				ipGeomCol->get_Geometry( i, &ipPartGeom );
				ipSegCol = ipPartGeom;
				if( ipSegCol == NULL ) {
					ATLASSERT( ipSegCol != NULL );
					continue;
				}

				ipSegCol->get_EnumSegments( &ipEnumSegment );
				while( S_OK == ipEnumSegment->Next( &ipSegment, &lPartIndex, &lVertexIndex ) ) {
					if( AhePointIsOnSegment( ipSegment, ipTestPoint, ipSpRef, bExeptVertex ) == TRUE )
						return TRUE;
				}
			}
		}
		break;
	}

	return FALSE;
}


BOOL CEditHelper::GetSplitResult(
	LPCTSTR lpcszSplitLayers,
	const IGeometryPtr& ipGeom,
	CTargetItems& cNoSetModifyDataFeatures,
	CTargetItems& cModifyNodeKindFeatures,
	const IPointPtr& ipTakeOverPoint,
	INT iSplitMode,
	CTargetItems& cTakeOveredFeatures,
	CTargetItems* pReferenceNodes,
	CTargetItems& cNewFeatures,
	CTargetItems& cTargetFeatures,
	std::list<ROWDEF>& listLQRows)
{
	/// �l�b�g���[�N�n�ȊO�̃t�B�[�`���N���X��\�����ʎq(�\���_�����ׂ��|�C���g��ێ�����N���X�̎��ʗp)
	const _TCHAR OTHER_FEATURECLASS[] = _T("__OTHER__");

	// �l�b�g���[�N���f�����Ƃ̃m�[�h�����ׂ��|�C���g�̏W��
	// <�����N�̃t�B�[�`���N���X��, �m�[�h�����ׂ��|�C���g>
	std::map<const CString, IPointCollectionPtr> createNodePoints;

	{
		// �l�b�g���[�N���f���ȊO�̃t�B�[�`���i���ł��ǂ��j��ێ����Ă���
		IFeaturePtr nwOtherFeature;

		// �m�[�h�̋�ԎQ�Ƃ��Z�b�g
		// �Z�b�g���Ȃ��Ɗ����m�[�h�Ƃ�Equal����Ɏ��s����
		for( CTargetItems::const_iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
		{
			if( !AheIsNetwork(it->GetFeature()) )
			{
				// ��ԎQ�Ƃ��Z�b�g����̂Ɏg�p���邽�߂ɁA�l�b�g���[�N���f���ȊO�̃t�B�[�`����1�����ێ����Ă���
				if(!nwOtherFeature)
					nwOtherFeature = it->GetFeature();
				continue;
			}
			CString nwFCName = AheGetNetworkLinkFeatureClassName(it->GetFeatureClassName());
			if( createNodePoints.find(nwFCName)!=createNodePoints.end() ) continue;
			createNodePoints[nwFCName] = IPointCollectionPtr( CLSID_Multipoint );
			((IGeometryPtr)createNodePoints[nwFCName])->putref_SpatialReference( AheGetSpatialReference( it->GetFeature() ) );
		}

		if(nwOtherFeature)
		{
			// �l�b�g���[�N�n�ȊO�͑S�ē��l�Ɉ���
			createNodePoints[OTHER_FEATURECLASS] = IPointCollectionPtr( CLSID_Multipoint );
			((IGeometryPtr)createNodePoints[OTHER_FEATURECLASS])->putref_SpatialReference( AheGetSpatialReference( nwOtherFeature ) );
		}
	}

	esriGeometryType enumSplitterGeomType;
	ipGeom->get_GeometryType( &enumSplitterGeomType );

#ifdef _USE_SETLQATTRIBUTE
	std::list<LQRFINFO> listLQRFInfo;	// [bug 2641] ���H�����N��LQRF�͍Ō�ɂ܂Ƃ߂ĕt�^���Ȃ��ƌ��݂̃A���S���Y���ł͐����������Ȃ�
#endif // ifdef _USE_SETLQATTRIBUTE

	if(FALSE==SplitFeatures(lpcszSplitLayers,	OTHER_FEATURECLASS, ipGeom, cNoSetModifyDataFeatures, ipTakeOverPoint, iSplitMode,
		listLQRFInfo, listLQRows, cTakeOveredFeatures, cNewFeatures, cTargetFeatures, createNodePoints))
		return FALSE;

	if(FALSE==SplitNetworkLink(ipGeom, cNewFeatures, cTargetFeatures, cNoSetModifyDataFeatures, cModifyNodeKindFeatures,
		listLQRFInfo, createNodePoints, cTakeOveredFeatures, pReferenceNodes))
		return FALSE;

	return TRUE;
}

BOOL CEditHelper::SplitFeatures(
	LPCTSTR lpcszSplitLayers,
	LPCTSTR strOthersName,
	const IGeometryPtr& ipGeom,
	const CTargetItems& cNoSetModifyDataFeatures,
	const IPointPtr& ipTakeOverPoint,
	INT iSplitMode,
	std::list<LQRFINFO>& listLQRFInfo,
	std::list<ROWDEF>& listLQRows,
	CTargetItems& cTakeOveredFeatures,
	CTargetItems& cNewFeatures,
	const CTargetItems& cTargetFeatures,
	std::map<const CString, IPointCollectionPtr>& createNodePoints)
{
	CString strSplitLayers( lpcszSplitLayers );
	for( CTargetItems::const_iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
	{
		IFeaturePtr ipFeature( it->GetFeature() );

		// �����Ώۃt�B�[�`�����ǂ������`�F�b�N
		CString strClassName, strLayerName;
		strLayerName = it->GetFeatureClassName();
		strClassName.Format(_T("|%s|"), strLayerName );
		ILayerPtr ipLayer( GetLayerByTableName( strLayerName ) );

		// �����ΏۂłȂ��Ȃ牽�����Ȃ�
		if( strSplitLayers.Find( strClassName ) < 0  ) continue;

		// �ǂ�NW���f����
		const sindyeNetworkModel eNWModel = AheGetNetWorkType(ipFeature);
		// �l�b�g���[�N�ł���΃t�B�[�`���N���X�����L�[�ɁA�����łȂ����OTHER_FEATURECLASS���L�[��
		// �m�[�h�A���邢�́A�\���_��ǉ����ׂ��|�C���g�N���X�̎Q�Ƃ��Ƃ�
		IPointCollectionPtr& NodePoints = ( AheIsNetworkLink(ipFeature) )? createNodePoints[strLayerName] : createNodePoints[strOthersName];

		IGeometryPtr ipTargetGeom = AheSetKnownSimple( it->GetShapeCopy() );

		// ������̌`��擾
		_ISetPtr ipSplittedGeometrySet = SplitGeometryByGeometry( ipTargetGeom, ipGeom, iSplitMode, AheIsNetworkLink( strLayerName ) ? TRUE : FALSE, NodePoints );
		if( !ipSplittedGeometrySet ) continue;

		// �����ł��Ȃ������� continue
		LONG lSplittedGeometryCount = -1;	// ������̌`��̐��ێ��p
		ipSplittedGeometrySet->get_Count( &lSplittedGeometryCount );
		if( lSplittedGeometryCount < 1 ) continue;

		// �ǂ̃W�I���g�������̃t�B�[�`���������p�������߂�
		LONG lTakeOverGeomNum = 0;
		if( ipTakeOverPoint )
		{
			ISpatialReferencePtr ipSpRef( AheGetSpatialReference( ipTakeOverPoint ) );	// ��ԎQ�Ƃ����킹�邽�߂Ɏ擾�i[bug 3253]�j

			ipSplittedGeometrySet->Reset();
			DOUBLE dMinDistance = -1.0;
			IProximityOperatorPtr ipProxOpe( ipTakeOverPoint );
			for( LONG i = 0; i < lSplittedGeometryCount; i++ )
			{
				IUnknownPtr ipUnk;
				ipSplittedGeometrySet->Next( &ipUnk );
				if( !ipUnk ) break;
				IGeometryPtr ipTakeOverGeom( ipUnk );
				if( !ipTakeOverGeom ) continue;
/*#ifdef DEBUG
				AheAddPolygonElement( m_cArcHelper.GetGraphicsContainer(), ipGeom, RGB(0,255,0), esriSFSSolid );
#endif*/
				// �����擾
				DOUBLE dMinDistanceTmp = -1.0;
				ipTakeOverGeom->putref_SpatialReference( ipSpRef );	// [bug 3253] ��r�ΏƂƔ�r���̋�ԎQ�Ƃ���v���Ă��Ȃ��ƃG���[�ɂȂ�I�I
				if( SUCCEEDED( ipProxOpe->ReturnDistance( ipTakeOverGeom, &dMinDistanceTmp ) ) )
				{
					if( dMinDistance < 0 || ( dMinDistance > dMinDistanceTmp && dMinDistance > 0 ) )
					{
						// �ŏ��̋����������̂������p����Ƃ���
						dMinDistance = dMinDistanceTmp;
						lTakeOverGeomNum = i;
					}
				}
				else {
					CString strMsg;
					strMsg.Format(MESSAGE_FEATURE_IS_RANDOM, AheGetErrorMessage() );
					AheMessageBox( strMsg, WARN_SPLIT, MB_OK|MB_ICONWARNING );
				}
			}
		}
		CTargetItems cSplittedRoadFeatures;	// ���H�����N�Ɋ֘A���郊���N��̏����̂���

		// �����J�n
		IGeometryPtr ipBeforeSplitGeomCopy;	// �X�v���b�g�O�̌`����L���i�������Ȃ��ƃ����N��̏����Ői�܂Ȃ��Ȃ�j
		ipSplittedGeometrySet->Reset();
		for( LONG i = 0; i < lSplittedGeometryCount; ++i )
		{
			IUnknownPtr ipUnk;
			ipSplittedGeometrySet->Next( &ipUnk );
			if( !ipUnk ) break;
			IGeometryPtr ipGeom( ipUnk );
			if( !ipGeom ) continue;

			if( i == lTakeOverGeomNum )
			{
				// �`��m��
				if( AheIsEditableFeature2( m_piMap, ipFeature, NULL, sindyeEditMove, GetRule(), FALSE, GetHWND() ) == FALSE ) 
					return FALSE;

				// �X�v���b�g�O�̌`��L��
				if( sindyeNetworkRoad == eNWModel)
					ipFeature->get_ShapeCopy( &ipBeforeSplitGeomCopy );	

#ifdef _DEBUG_FOR_2059	// [bug 2059] �����p
				{
					CString strMsg;
					strMsg.Format(_T("�����`��̃g���[�X[%s:%d:%s]"), it->GetFeatureClassName(), it->GetOID(), _T("����") );
					AheTraceGeometry( ipGeom, strMsg );
				}
#endif // ifdef _DEBUG_FOR_2059

				//ipFeature->putref_Shape( ipGeom );
				AheSetShape( ipFeature, ipGeom );

#ifdef _DEBUG_FOR_2059	// [bug 2059] �����p
				{
					CString strMsg;
					IGeometryPtr ipGeomTmp;
					ipFeature->get_ShapeCopy( &ipGeomTmp );
					strMsg.Format(_T("�����`��̃g���[�X�i�Z�b�g��j[%s:%d:%s]"), it->GetFeatureClassName(), it->GetOID(), _T("����") );
					AheTraceGeometry( ipGeomTmp, strMsg );

					//����ɁASimplify�����ꍇ�̕ω����݂Ăق��[
					IClonePtr ipClone( ipGeomTmp ), ipCopy;
					ipClone->Clone( &ipCopy );
					ITopologicalOperatorPtr ipTopoTmp( ipCopy );
					ipTopoTmp->Simplify();
					strMsg.Format(_T("�����`��̃g���[�X�i�Z�b�g��ASimplify�j[%s:%d:%s]"), it->GetFeatureClassName(), it->GetOID(), _T("����") );
					AheTraceGeometry( (IGeometryPtr)ipTopoTmp, strMsg );
				}
#endif // ifdef _DEBUG_FOR_2059
				if( cNoSetModifyDataFeatures.IsAlreadyExist( *it ) == FALSE )
					AheSetModifyData( ipFeature, sindyUpdateTypeShape );	// cNoSetModifyDataFeatures �ɓ����Ă��Ȃ����̂̂݌`��C���̗���������
				cTakeOveredFeatures.push_back( ipFeature );	// ID�������p�������̂�ێ�

				// �X�v���b�g����Ɋ֘A�e�[�u�����̏C��������K�v������̂ňꎞ�i�[
				cSplittedRoadFeatures.push_back( ipFeature );
			}
			// ����ȊO�͐V�K�ɍ쐬
			else 
			{
				if( AheIsCreatableLayer( AheGetFeatureClass( ipFeature ), ipGeom, TRUE ) == FALSE )
				{
					CString strMsg;
					strMsg.Format(ERROR_CANNOT_CONTINUE_SPLIT, strLayerName );
					AheMessageBox( strMsg, ERROR_FAILED_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
					return FALSE;	// �쐬�֎~
				}
/*						//////////////////////////////////////////////
				// �t�B�[�`���쐬�i�������̂��߂� Insert ���g�p�j
				//////////////////////////////////////////////
//					IFeatureCursorPtr ipInsertCursor;
				IFeatureClassPtr ipCreateClass( AheGetFeatureClass( ipFeature ) );
//						ipCreateClass->Insert( FALSE, &ipInsertCursor );
				IFeaturePtr ipNewFeature = AheCreateRowBuffer( ipCreateClass, ipGeom );*/
#ifdef _DEBUG_FOR_2059	// [bug 2059] �����p
				{
					CString strMsg;
					strMsg.Format(_T("�����`��̃g���[�X[%s:%d:%s]"), it->GetFeatureClassName(), -1, _T("�V�K") );
					AheTraceGeometry( ipGeom, strMsg );
				}
#endif // ifdef _DEBUG_FOR_2059
				IFeaturePtr ipNewFeature = AheCreateRow( ipFeature, ipGeom );
#ifdef _DEBUG_FOR_2059	// [bug 2059] �����p
				{
					CString strMsg;
					long lOIDTmp;
					ipNewFeature->get_OID( &lOIDTmp );
					strMsg.Format(_T("�����`��̃g���[�X�i�Z�b�g��j[%s:%d:%s]"), it->GetFeatureClassName(), lOIDTmp, _T("�V�K") );
					AheTraceGeometry( ipGeom, strMsg );

					//����ɁASimplify�����ꍇ�̕ω����݂Ăق��[
					IClonePtr ipClone( ipGeom ), ipCopy;
					ipClone->Clone( &ipCopy );
					ITopologicalOperatorPtr ipTopoTmp( ipCopy );
					ipTopoTmp->Simplify();
					strMsg.Format(_T("�����`��̃g���[�X�i�Z�b�g��ASimplify�j[%s:%d:%s]"), it->GetFeatureClassName(), lOIDTmp, _T("�V�K") );
					AheTraceGeometry( (IGeometryPtr)ipTopoTmp, strMsg );
				}
#endif // ifdef _DEBUG_FOR_2059
				// �����R�s�[
				AheCopyAttribute( ipFeature, ipNewFeature, TRUE );	// [bug 4043] �������[�h�ŃR�s�[

				////////////////////////////////////////////////////
				// [bug4497]LINK_ID�������ς�鎞�ɍX�V�������t���Ȃ��΍�
				// �V�K�쐬�Ƒ����ύX����ʂ��邽�߃����N�������ƌX�΂̏ꍇ�́uLINK_ID=0�v�ɂ���
				// �uLINK_ID�v�͌�ŐU�蒼���̂ő��v�A�A�A�̂͂��c
				if( AheGetFeatureClassName( ipFeature ) == _T("SEGMENT_ATTR") || AheGetFeatureClassName( ipFeature ) == _T("GRAD") ){
					AheSetAttribute( ipNewFeature, _T("LINK_ID"), CComVariant(0L) );
				}
				////////////////////////////////////////////////////

				// ����
				AheSetModifyData( ipNewFeature, sindyUpdateTypeCreate );
/*					// �C���T�[�g
				CComVariant vaID;
				ipInsertCursor->InsertFeature( (IFeatureBufferPtr)ipNewFeature, &vaID ); <- ��ō\���_�̒ǉ�������̂ł܂��C���T�[�g���Ȃ�*/
/*						// �쐬���ꂽ�t�B�[�`���ɂ� OBJECTID �����ĂȂ��̂ŁA�����Ȃ����i�g�z�z�j
				ipCreateClass->GetFeature( vaID.lVal, &ipNewFeature );*/
				/////////////////////////////////////////////// �����܂�
//						cNewFeatures.push_back( ipNewFeature, -1 );
				cNewFeatures.push_back( ipNewFeature );

				// �X�v���b�g����Ɋ֘A�e�[�u�����̏C��������K�v������̂ňꎞ�i�[
				cSplittedRoadFeatures.push_back( ipNewFeature );
			}
		}

		// TODO ����̃l�b�g���[�N���f���ɑ΂��ē��ʂȏ������s���Ă���
		// ���̕��@�ɂ͈ȉ��̖��_������
		// �E�֘A�e�[�u�����������l�b�g���[�N���f�����V���ɒ�`�����x�ɁA�����ɏ�����ǉ����Ȃ���΂Ȃ�Ȃ�
		// �E�����N��̏����̒���g_cRuleLinkQueue���Q�Ƃ��Ă��Ȃ�
		// �E�l�b�g���[�N���f���ȊO�Ɋ֘A�e�[�u���������̂��ł����ꍇ�ɁA�������Ƀ��R�[�h���R�s�[����Ȃ�
		switch(eNWModel)
		{
		// ���H�����N���X�v���b�g�����ꍇ
		case sindyeNetworkRoad:
#ifdef _USE_SETLQATTRIBUTE
			// �����N��̏C���������ł���
			if( SetLQAttribute( it->GetOID(), ipBeforeSplitGeomCopy, cSplittedRoadFeatures, listLQRFInfo, listLQRows ) == FALSE )
				return FALSE;
#else
			if( ! CreateLQForSplittedRoad( *it, cPlittedRoadFeatures, listLQRows ) )
				return FALSE;
#endif // ifdef _USE_SETLQATTRIBUTE
			// ���ԋK�������������ǂ������ׁA����̂Ȃ玞�ԋK����V�����쐬
			if( CopyRelationAttribute( it->GetFeature(), cSplittedRoadFeatures ) == FALSE )
				return FALSE;
			break;
		// �S���E�������X�v���b�g�����ꍇ
		case sindyeNetworkRailway:
		case sindyeNetworkWalk:
			// �֘A�e�[�u���𕡐�
			if( CopyRelationAttribute( it->GetFeature(), cSplittedRoadFeatures ) == FALSE )
				return FALSE;
			break;
		default:
			break;
		}
	}
	return TRUE;
}

BOOL CEditHelper::SplitNetworkLink(
	const IGeometryPtr& ipGeom,
	CTargetItems& cNewFeatures,
	CTargetItems& cTargetFeatures,
	CTargetItems& cNoSetModifyDataFeatures,
	CTargetItems& cModifyNodeKindFeatures,
	const std::list<LQRFINFO>& listLQRFInfo,
	const std::map<const CString, IPointCollectionPtr>& createNodePoints,
	CTargetItems& cTakeOveredFeatures,
	CTargetItems* pReferenceNodes) const
{
	// �l�b�g���[�N�n(���HNW�ȂǁA�����N�ƃm�[�h�������ɐ������Ȃ���΂Ȃ�Ȃ��ނ̃t�B�[�`���N���X)��p�����i�X�v���b�g�������������ꍇ�݂̂Ȃ炸�j
	if( cTargetFeatures.IsIncludedNetworkItems() == FALSE ) return TRUE;

	for( std::map<const CString, IPointCollectionPtr>::const_iterator nodePointIte = createNodePoints.begin(); nodePointIte!=createNodePoints.end(); ++nodePointIte )
	{
		// �Ώۂ̑S�Ẵl�b�g���[�N�i�[�p
		// �S�Ẵl�b�g���[�N �� cTargetFeatures �̃l�b�g���[�N �{ cNewFeatures �̃l�b�g���[�N
		CTargetItems targetItems;
		cTargetFeatures.GetFeature( targetItems, nodePointIte->first );
		cNewFeatures.GetFeature( targetItems, nodePointIte->first, TRUE );

		// �������̃����N�ɑΉ��t���m�[�h�̃t�B�[�`���N���X�����擾
		CString nodeFCName = AheGetNetworkNodeFeatureClassName(nodePointIte->first);

		if( !targetItems.empty() )
		{
#ifdef _USE_SETLQATTRIBUTE
			// �l�b�g���[�N�n�̒���LQ�e�[�u���Ɗ֌W������͓̂��H�����N�����Ȃ̂œ��H�����N�ȊO�͏������Ȃ�
			if( sindyeNetworkRoad==targetItems.GetNetworkType() )
			{
				// [bug 2641] LQRF�͂����ŕt�^
				for( std::list<LQRFINFO>::const_iterator itLQRF = listLQRFInfo.begin(); itLQRF != listLQRFInfo.end(); ++itLQRF )
				{
					CTargetItems::iterator itRoad = targetItems.begin();
					for(; itRoad != targetItems.end(); ++itRoad)
					{
						// ��������LQ�e�[�u���̃����NID�ƃI�u�W�F�N�gID����v���Ȃ��Ȃ��΂�
						if(	itRoad->GetOID() != itLQRF->oid ) continue;

						// ��L�̏�����S�Ė��������̂��������� �����N��̏���ݒ肵�� break ����
						for( std::map<sindyeRoadQueueType,bool>::const_iterator itLQRFInfo = itLQRF->LQRF.begin(); itLQRFInfo != itLQRF->LQRF.end(); ++itLQRFInfo )
						{
							itRoad->SetAttribute( m_cLoadQueueInfo.GetLoadQueueInfo( itLQRFInfo->first, sindyeLoadQueueInfoLQRFRoadFieldName ), ( itLQRFInfo->second ) ? 1L : 0L );
							ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �e�[�u��[ROAD_LINK] OID[%d] [%s]=%d\n"),
								itRoad->GetOID(), m_cLoadQueueInfo.GetLoadQueueInfo( itLQRFInfo->first, sindyeLoadQueueInfoLQRFRoadFieldName ), ( itLQRFInfo->second ) ? 1 : 0 );
						}
						break;
					}
					if( itRoad == targetItems.end() )
					{
						// �����ɓ����Ă���ꍇ�́AcAllRoadFeatures�ɒǉ����铹�H�����N������Ă��Ȃ����Ƃ��Ӗ�����
						ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : LQRF��t�^���邽�߂̓��H�����N��������Ȃ� OID[%d]\n"), itLQRF->oid );
						_ASSERTE( itRoad != targetItems.end() );
						AheMessageBox( ERROR_LINK_NOT_FOUND, _T("CEditHelper::SplitFeaturesByGeometry()"), MB_OK|MB_ICONSTOP );
						return FALSE;
					}
				}
			}
#endif // ifdef _USE_SETLQATTRIBUTE
		}

		IPointCollectionPtr ipCreateNodePoints(nodePointIte->second);
		CTargetItems cExistNodes; // �S�l�b�g���[�N�̃G���x���[�v�Ō��������m�[�h������iFROM�ATO �͂���{ cExistOrNewNodes ���݂ď���������j
		cExistNodes.clear();

		// �֘A����m�[�h���擾�iFROM�ATO�t���ւ��y�уm�[�h�쐬�̔��f�Ɏg�p�j
		IFeatureClassPtr ipNodeClass( targetItems.GetNetworkNodeFeatureClass() );

#ifdef DEBUG
		// UNION ����O�̌`����n�C���C�g
		ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ���j�I������O�̓��H�����N�n�C���C�g�i���F�j\n") );
/*			// �G�������g�쐬�i�ĕ`�悵�Ă������Ȃ��悤�Ɂj
		IGraphicsContainerPtr ipContainer;
		m_cArcHelper.GetActiveView(FALSE)->get_GraphicsContainer( &ipContainer );*/

		for( CTargetItems::iterator itTmp = targetItems.begin(); itTmp != targetItems.end(); ++itTmp )
			//IElementPtr ipElement( AheAddLineElement( ipContainer, itTmp->GetShape(), RGB(255,255,0), 2, esriSLSSolid ) );
			AheDrawPolyline( GetFocusMapScreenDisplay(), itTmp->GetShape(), RGB(255,255,0), 2, esriSLSSolid, TRUE );
#endif
		// �G���x���[�v�ł͂ł�������i���ʂ������j�̂ŁA���j�I���Ō���
//				IGeometryPtr ipSearchGeom( cAllRoadFeatures.GetUnion() ); <- UNION����ƁA1Unit�����Ȃ��p�[�g�͏����Ă��܂��̂Œ��Ӂi���񍐁j�I�I
		IGeometryPtr ipSearchGeom( TargetItems_MergeGeometry( targetItems, false ) );
		if( ipSearchGeom )
		{
#ifdef DEBUG
			TargetItems_Trace( targetItems, _T("�m�[�h�����ׂ̈�UNION���铹�H�����N") );
			// �����m�[�h��I�����邽�߂̓��H�����N�̃��j�I���`��n�C���C�g
			//IElementPtr ipElement( AheAddLineElement( ipContainer, ipSearchGeom, RGB(0,255,0), 2, esriSLSSolid ) );
			AheDrawPolyline( GetFocusMapScreenDisplay(), ipSearchGeom, RGB(0,255,0), 2, esriSLSSolid, TRUE );
			ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ���H�����N��UNION�����A�n�C���C�g�i�ΐF�j\n") );
			//AheTraceGeometry( ipSearchGeom, _T("CEditHelper::SplitFeaturesByGeometry()") );
#endif
			((ITopologicalOperatorPtr)ipSearchGeom)->Simplify();
			AheSelectByShapeFromFeatureClass( ipSearchGeom, cExistNodes, ipNodeClass, esriSpatialRelIntersects );
#ifdef DEBUG
			// �I���ł��������m�[�h�n�C���C�g
			ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ���H�����N��UNION�`��ł̃m�[�h�I���I���A�n�C���C�g�i���F�j\n") );
			for( CTargetItems::iterator itTmp = cExistNodes.begin(); itTmp != cExistNodes.end(); ++itTmp )
			{
				//ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : CExistNodes : OID=%d\n"), itTmp->GetOID() );
				AheDrawPoint( GetFocusMapScreenDisplay(), itTmp->GetShape(), FALSE, 8, 128, 255, 255, esriSMSCircle, FALSE, TRUE );
			}
			TargetItems_Trace( cExistNodes, _T("�I���ł����m�[�h") );
			IElementPtr ipElement( CLSID_LineElement );
			ipElement->put_Geometry( ipSearchGeom );
			((IGraphicsContainerPtr)GetMap())->AddElement( ipElement, 0 );
#endif
			// [bug 1965] �`��`�F�b�N����2���H�p�x����Ŏg�p����
			if( pReferenceNodes ) *pReferenceNodes = cExistNodes;
		}
//#if 0
		if ( ipNodeClass )	// [Bug 7848]�����N�������𕪊����悤�Ƃ���Ɨ����� 
		{
			std::list<long> listConnectNodeIds;

			for( CTargetItems::iterator itRoads1 = targetItems.begin(); itRoads1 != targetItems.end(); ++itRoads1 )
			{
				CComVariant vaFromNodeID = itRoads1->GetAttribute( FROM_FIELD );
				CComVariant vaToNodeID   = itRoads1->GetAttribute( TO_FIELD );

				listConnectNodeIds.push_back( vaFromNodeID.lVal );
				listConnectNodeIds.push_back( vaToNodeID.lVal );
			}

			listConnectNodeIds.sort();
			listConnectNodeIds.unique();

			CString strIds;
			for( std::list<long>::const_iterator itId = listConnectNodeIds.begin(); itId != listConnectNodeIds.end(); ++itId )
			{
				CString strTmp;
				strTmp.Format( _T("%d"), *itId );

				if ( ! strIds.IsEmpty() )
					strIds += _T(",");

				strIds += strTmp;
			}

			IQueryFilterPtr ipQuery( CLSID_QueryFilter );
			ipQuery->put_SubFields( CComBSTR( _T("*") ) );

			CString strWhere;
			strWhere.Format( _T("OBJECTID IN (%s)"), strIds );
			ipQuery->put_WhereClause( CComBSTR( strWhere ) );

			IFeatureCursorPtr ipFeatureCursor;
			ipNodeClass->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );

			if( ipFeatureCursor )
			{
				IGeoDatasetPtr ipDataset( ipNodeClass );
				ISpatialReferencePtr ipDatasetSpRef;

				ipDataset->get_SpatialReference( &ipDatasetSpRef );

				IFeaturePtr ipFeature;
				while( SUCCEEDED( ipFeatureCursor->NextFeature( &ipFeature ) ) && ipFeature )
				{
					IGeometryPtr ipGeom;
					ISpatialReferencePtr ipSpRef;
					ipFeature->get_ShapeCopy( &ipGeom );
					ipGeom->get_SpatialReference( &ipSpRef );

					VARIANT_BOOL vaIsSameSpRef = VARIANT_FALSE;
					ipSpRef->IsPrecisionEqual( ipDatasetSpRef, &vaIsSameSpRef );
					if( ! vaIsSameSpRef )
					{
						ATLTRACE(_T("AheSelectByShapeFromLayer : �t�B�[�`���N���X�̋�ԎQ�Ƃƃt�B�[�`�������W�I���g���̋�ԎQ�Ƃ��قȂ�܂��B�t�B�[�`���N���X�̋�ԎQ�ƂɈ�v�����܂��B\n") );
						AheSnapToSpatialReference( ipGeom, ipDatasetSpRef );
					}

					cExistNodes.push_back( ipFeature );
				}

#ifdef DEBUG
				// �I���ł��������m�[�h�n�C���C�g
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ���H�����N��UNION�`��ł̃m�[�h�I���I���A�n�C���C�g�i���F�j\n") );
				for( CTargetItems::iterator itTmp = cExistNodes.begin(); itTmp != cExistNodes.end(); ++itTmp )
				{
					//ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : CExistNodes : OID=%d\n"), itTmp->GetOID() );
					AheDrawPoint( GetFocusMapScreenDisplay(), itTmp->GetShape(), FALSE, 8, 128, 255, 255, esriSMSCircle, FALSE, TRUE );
				}
				TargetItems_Trace( cExistNodes, _T("�I���ł����m�[�h") );
				IElementPtr ipElement( CLSID_LineElement );
				ipElement->put_Geometry( ipSearchGeom );
				((IGraphicsContainerPtr)GetMap())->AddElement( ipElement, 0 );
#endif
				// [bug 1965] �`��`�F�b�N����2���H�p�x����Ŏg�p����
				if( pReferenceNodes ) *pReferenceNodes = cExistNodes;
			}
		}
//#endif	// if 0

		// �m�[�h�����Ȃ���΂����Ȃ��ꍇ�̓m�[�h�쐬
		LONG lCreateNodePointCount = -1;
		ipCreateNodePoints->get_PointCount( &lCreateNodePointCount );
		if( lCreateNodePointCount > 0 )
		{
			// ��r�Ώۃm�[�h���܂Ƃ߂�
			// �m�[�h���쐬������cAllNodes�ɂ�push_back���Ȃ���΂Ȃ�Ȃ����Ƃɒ��ӁI�I
			CTargetItems cAllNodes( cExistNodes );
			cNewFeatures.GetFeature( cAllNodes, nodeFCName, TRUE );
			cTargetFeatures.GetFeature( cAllNodes, nodeFCName, TRUE );
			for( LONG i = 0; i < lCreateNodePointCount; i++ )
			{
				IPointPtr ipCreateNodePoint;
				ipCreateNodePoints->get_Point( i, &ipCreateNodePoint );

				BOOL bFindExistNode = FALSE;
				// �����m�[�h�Ɋ��ɑ��݂��邩�ǂ����`�F�b�N
				for( CTargetItems::iterator it = cAllNodes.begin(); it != cAllNodes.end(); ++it )
				{
					// ipCreateNodePoint�͂܂���ԎQ�Ƃɂ����Ă��Ȃ��`��Ȃ̂ŁA
					// �����ł�1Unit�Ȃ狖�e���Ă����IClone::Equals���g�p�����ق����ǂ��ibug 5807�j
					if( AheIsEqual( it->GetShape(), ipCreateNodePoint ) )
					{
						bFindExistNode = TRUE;
						break;
					}
				}
				if( bFindExistNode ) continue;

				// �m�[�h�쐬
				if( ! AheIsCreatableLayer( ipNodeClass, ipCreateNodePoint, TRUE ) )
				{
					CString strMsg;
					strMsg.Format(ERROR_CANNOT_CONTINUE_SPLIT, nodeFCName );
					AheMessageBox( strMsg, ERROR_FAILED_SPLIT, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
					return FALSE;	// �쐬�֎~
				}
				IFeaturePtr ipNewFeature = AheCreateRow( ipNodeClass, ipCreateNodePoint, TRUE );
				// ����
				AheSetModifyData( ipNewFeature, sindyUpdateTypeCreate );
				// �m�[�h��ʐݒ�
				AheResetNodeAttribute( ipNewFeature, targetItems );
#ifdef DEBUG
				LONG lNodeID = -1;
				ipNewFeature->get_OID( &lNodeID );
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �m�[�h�쐬 OBJECTID=%d\n"), lNodeID );
				//AheTraceGeometry( ipCreateNodePoint, _T("CEditHelper::SplitFeaturesByGeometry()") );
				AheDrawPoint( GetFocusMapScreenDisplay(), ipCreateNodePoint, FALSE, 8, 0, 0, 255, esriSMSCircle, FALSE, TRUE );
				AheTraceRow( ipNewFeature, _T("CEditHelper::SplitFeaturesByGeometry() : �V�K�쐬�m�[�h") );
				AheTraceGeometry( AheGetShape(ipNewFeature), _T("CEditHelper::SplitFeaturesByGeometry() : �V�K�쐬�m�[�h") );
#endif
				cNewFeatures.push_back( ipNewFeature );
				cAllNodes.push_back( ipNewFeature );
			}
		}

		// �d������m�[�h�̓}�[�W����
		CTargetItems cAllNodeItems;				// �S�m�[�h = cExistNodes + cNewFeatures �̃m�[�h + cTargetFeatures �̃m�[�h
		CTargetItems cDeleteNodeFeatures;		// �폜�ΏۂƂȂ�t�B�[�`��

		cExistNodes.GetFeature( cAllNodeItems, nodeFCName );

		cNewFeatures.GetFeature( cAllNodeItems, nodeFCName, TRUE );
		cTargetFeatures.GetFeatures( cAllNodeItems, nodeFCName, TRUE );
		for( CTargetItems::iterator itNodes1 = cAllNodeItems.begin(); itNodes1 != cAllNodeItems.end(); ++itNodes1 )
		{
			if( cDeleteNodeFeatures.IsAlreadyExist( *itNodes1 ) ) continue;

			for( CTargetItems::iterator itNodes2 = cAllNodeItems.begin(); itNodes2 != cAllNodeItems.end(); ++itNodes2 )
			{
				if( cDeleteNodeFeatures.IsAlreadyExist( *itNodes2 ) ) continue;

				if( itNodes2 == itNodes1 ) continue;

				if( AheIsDBEqual( itNodes2->GetShape(), itNodes1->GetShape() ) )
				{
					IAttributeSelectDlgObjPtr ipAttrSelectDlg( CLSID_AttributeSelectDlgObj );
					CComBSTR bstrEnableField;
					// TODO: �b��Ή�
					// �S���m�[�h���m�̃}�[�W�̂Ƃ��̓m�[�h��ʂ��I��Ώۂɓ����
					// ����ȊO�̃l�b�g���[�N�m�[�h�͓��H�m�[�h�ɏ�������
					// ����͒��ł��Ƃ��邪�A�{���A�ǂ����ɒ�`���Ă����ׂ��ł���
					// �t�B�[���h����sindy_schema�̂��̂��g�p����ׂ�
					if(0==itNodes1->GetFeatureClassName().CompareNoCase(sindy::schema::sj::railway_node::kTableName))
						bstrEnableField = _T("OBJECTID|SOURCE|WORK_YEAR|NODECLASS_C");
					else
						bstrEnableField = _T("OBJECTID|SOURCE|WORK_YEAR|NAME_KANJI|NAME_YOMI|HEIGHT_C|HEIGHT|SIGNAL_C|PARKING_ID");

					if( ipAttrSelectDlg->SetFeature( itNodes1->GetFeature() ) != S_OK ||
						ipAttrSelectDlg->SetFeature( itNodes2->GetFeature() ) != S_OK ||
						ipAttrSelectDlg->SetEnableFields( bstrEnableField )   != S_OK )
						return FALSE;

					if( ( ipAttrSelectDlg->DoModal( (OLE_HANDLE)GetHWND() ) ) == S_OK )
					{
						IUnknownPtr ipUnk;
						ipAttrSelectDlg->GetMeargedFeature( &ipUnk );

						if( itNodes1->GetFeature() == (IFeaturePtr)ipUnk )
							cDeleteNodeFeatures.push_back( *itNodes2 );
						else if( itNodes2->GetFeature() == (IFeaturePtr)ipUnk )
							cDeleteNodeFeatures.push_back( *itNodes1 );
						else 
							return FALSE;
					}
					else
						return FALSE;
				}
			}
		}
		if( cDeleteNodeFeatures.size() > 0 )
		{
			// �m�[�h���i�[���Ă��郊�X�g����폜�Ώۃm�[�h���폜
			for( CTargetItems::iterator itDeleteNodes = cDeleteNodeFeatures.begin(); itDeleteNodes != cDeleteNodeFeatures.end(); ++itDeleteNodes )
			{
				// �S�m�[�h
				CTargetItems::iterator itAllNodes = cAllNodeItems.begin();
				while( itAllNodes != cAllNodeItems.end() )
				{
					if( *itDeleteNodes == *itAllNodes )
						itAllNodes = cAllNodeItems.erase( itAllNodes );
					else
						++itAllNodes;
				}
				// cNoSetModifyDataFeatures
				CTargetItems::iterator itNoSetModify = cNoSetModifyDataFeatures.begin();
				while( itNoSetModify != cNoSetModifyDataFeatures.end() )
				{
					if( *itDeleteNodes == *itNoSetModify )
						itNoSetModify = cNoSetModifyDataFeatures.erase( itNoSetModify );
					else
						++itNoSetModify;
				}
				// �n���ꂽ�t�B�[�`��
				CTargetItems::iterator itTargets = cTargetFeatures.begin();
				while( itTargets != cTargetFeatures.end() )
				{
					if( *itDeleteNodes == *itTargets )
						itTargets = cTargetFeatures.erase( itTargets );
					else
						++itTargets;
				}
				// pReferenceNodes
				if( pReferenceNodes )
				{
					CTargetItems::iterator itReference = pReferenceNodes->begin();
					while( itReference != pReferenceNodes->end() )
					{
						if( *itDeleteNodes == *itReference )
							itReference = pReferenceNodes->erase( itReference );
						else
							++itReference;
					}
				}
				// �폜
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �m�[�h OBJECTID=%d �폜\n"), itDeleteNodes->GetOID() );
				
				// �֘A����t�B�[�`�����폜
				if( !DeleteRelationTables(itDeleteNodes->GetFeature()) )
					return false;
				// �m�[�h�폜
				if( itDeleteNodes->Delete() == FALSE ) _ASSERTE( FALSE );
			}
		}

		// FROM�ATO ���������ΏۂƎv���铹�H�����N�� FROM�ATO ��������
		for( CTargetItems::iterator itRoads = targetItems.begin(); itRoads != targetItems.end(); ++itRoads )
		{
			LONG lFromNodeFeatureID = -1, lToNodeFeatureID = -1;
			for( CTargetItems::iterator itNodes = cAllNodeItems.begin(); itNodes != cAllNodeItems.end(); ++itNodes )
			{
				if( itRoads->IsFromPoint( itNodes->GetShape() ) )
					lFromNodeFeatureID = itNodes->GetOID();
				else if( itRoads->IsToPoint( itNodes->GetShape() ) )
					lToNodeFeatureID = itNodes->GetOID();

				if( lFromNodeFeatureID >= 0 && lToNodeFeatureID >= 0 ) break;
			}
			if( lFromNodeFeatureID < 0 || lToNodeFeatureID < 0 )
			{
				// FROM�ATO�ɐڑ�����m�[�h��������Ȃ�����
				CString strMsg;
#ifdef DEBUG
				for( CTargetItems::iterator itNodes = cAllNodeItems.begin(); itNodes != cAllNodeItems.end(); ++itNodes )
					ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �����Ώۃm�[�h�FOBJECTID=%d\n"), itNodes->GetOID() );
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() �t���ւ����s���H�����N�̃n�C���C�g�i�ԁj\n") );
				AheDrawPolyline( GetFocusMapScreenDisplay(), itRoads->GetShape(), RGB(255,0,0), 2, esriSLSSolid, TRUE );
#endif
				strMsg.Format(ERROR_FAIL_TO_ROAD_LINK_REPLACE, itRoads->GetOID() );
				AheMessageBox( strMsg, SPLIT_ROAD_LINK, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
				return FALSE;
			}
			CComVariant vaFromID = itRoads->GetAttribute( FROM_FIELD );
			CComVariant vaToID   = itRoads->GetAttribute( TO_FIELD   );
			if( vaFromID.vt != VT_I4 || vaFromID.lVal != lFromNodeFeatureID ||
				vaToID.vt   != VT_I4 || vaToID.lVal   != lToNodeFeatureID )
			{
				// ��������
				itRoads->SetAttribute( FROM_FIELD, lFromNodeFeatureID );
				itRoads->SetAttribute( TO_FIELD,   lToNodeFeatureID );
				// ID �������p�������̂ł��Ȃ��A�V�K�ɍ쐬���ꂽ���̂ł��Ȃ��AcTargetFeatures�ɂ��Ȃ��ꍇ�͂����ő����ҏW�̗������������ށi����̂��H�j
				if( cTakeOveredFeatures.IsAlreadyExist( *itRoads ) == FALSE &&
					cNewFeatures.IsAlreadyExist( *itRoads ) == FALSE &&
					cTargetFeatures.IsAlreadyExist( *itRoads ) == FALSE )
					itRoads->SetModifyData( sindyUpdateTypeProperty );
			}
		}
		// �m�[�h��ʂ̍Đݒ�i�K�v�Ȃ��̂̂݁j
		IPointCollectionPtr ipPointCol( AheGetClone( ipGeom ) );
		if( !ipPointCol )
		{
			ipPointCol.CreateInstance( CLSID_Multipoint );
			((IGeometryPtr)ipPointCol)->putref_SpatialReference( AheGetSpatialReference( ipGeom ) ); // ��ԎQ�ƕt�^
			IPointPtr ipPoint( ipGeom );
			if( ipPoint )
				ipPointCol->AddPoint( ipPoint );
		}
		// cTargetFeatures �̒��̃m�[�h�̃|�C���g����ʐݒ�ΏۂƂ��Ēǉ�
		CTargetItems cTargetNodes;
		cTargetFeatures.GetNodeFeatures( cTargetNodes, TRUE );	// 2���H���瓹�H�ƃm�[�h��������������ꍇ�ȂǁA��������Ȃ��Ǝ�ʂ��Đݒ肳��Ȃ�
		for( CTargetItems::iterator itTargetNodes = cTargetNodes.begin(); itTargetNodes != cTargetNodes.end(); ++itTargetNodes )
			ipPointCol->AddPoint( (IPointPtr)itTargetNodes->GetShapeCopy() );	// <- �|�C���g�̏ꍇ�� GetShape() ���� StopEditOperation() �ŗ�����I�I�Ȃ�Ł[�H�H
		// �d���_�͎�菜��
		ITopologicalOperatorPtr ipTopo( ipPointCol );
		if( ipTopo )
		{
			ipTopo->Simplify();
			ipPointCol = ipTopo;
		}

		LONG lPointCount = -1;
		ipPointCol->get_PointCount( &lPointCount );
		for( LONG i = 0; i < lPointCount; ++i )
		{
			IPointPtr ipPoint;
			ipPointCol->get_Point( i, &ipPoint );
			for( CTargetItems::iterator itAllNodes = cAllNodeItems.begin(); itAllNodes != cAllNodeItems.end(); ++itAllNodes )
			{
				if( itAllNodes->IsEquals( ipPoint ) == TRUE )
				{
					BOOL bIsChanged = FALSE;
					BOOL bSetModifyData = ( cNewFeatures.IsAlreadyExist( *itAllNodes ) == FALSE
						&& cTargetFeatures.IsAlreadyExist( *itAllNodes ) == FALSE );
					if( AheResetNodeAttribute( itAllNodes->GetFeature(), targetItems, &bIsChanged, bSetModifyData ) )
					{
						if( bIsChanged == TRUE )
							cModifyNodeKindFeatures.push_back( *itAllNodes );	// ��� cTargetFeatures �ɂ��킹����
					}
					else {
						// �m�[�h��ʍĐݒ莸�s�I�I
						AheMessageBox( ERROR_FAIL_TO_RESET_NODE, SPLIT_ROAD_LINK, MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
						return FALSE;
					}
				}
			}
		}
	}
	return TRUE;
}

// �֘A�t�B�[�`��������l�b�g���[�N�����N��T���āA�֘AID�t�B�[���h�Ƀl�b�g���[�N�����N��OBJECTID���Z�b�g
BOOL CEditHelper::ResetCommonFeatures(
	CTargetItems& cNewFeatures,
	CTargetItems& cTargetFeatures,
	const CTargetItems& cNoSetModifyDataFeatures,
	const CTargetItems& cTakeOveredFeatures)
{
	// �������ꂽ�t�B�[�`�����݂āA�l�b�g���[�N�����N�Ɋ֘A����t�B�[�`���N���X������Ȃ�A
	// ���̃l�b�g���[�N�����N���i�[����i�Ⴆ�΁ASEGMENT_ATTR�ɑ΂���ROAD_LINK�Ȃǁj
	// ��X�̏����Ńl�b�g���[�N�����N��IFeatureClassPtr���g���̂ŁA�����āA���̌`�Ŏ擾����
	// �i��Ō����������ꍇ�A�ҏW�J�n����Ă���t�B�[�`����T�������̂�������ԂɂȂ�B
	//   �����̏����ɓ���Ƃ������Ƃ͕ҏW�J�n����Ă���Ƃ������ƂȂ̂ŁA�|�C���^��ێ����Ă����j
	// <�t�B�[�`���N���X��, �t�B�[�`���N���X>
	std::map< CString, IFeatureClassPtr > ipNWClassMap;
	for( CTargetItems::const_iterator it = cTargetFeatures.begin(); it != cTargetFeatures.end(); ++it )
	{
		if( AheIsRelateWithNetwork( it->GetFeature() ) ) // �l�b�g���[�N�֌W�t�B�[�`����������擾�ibug 5486�j
		{
			// �l�b�g���[�N�֌W�t�B�[�`���Ȃ�A���������t�B�[�`���N���X���擾
			// �Ⴆ�΁AGRAD��������ROAD_LINK��IFeatureClassPtr���擾
			IFeatureClassPtr ipFC = AheGetNetworkLinkFeatureClass( it->GetFeature() );
			ipNWClassMap.insert( std::make_pair(AheGetFeatureClassName(ipFC), ipFC) );
		}
	}

	// �l�b�g���[�N�����N���ƂɊ֘A����t�B�[�`����T���ď���
	for(std::map< CString, IFeatureClassPtr >::iterator itNWClass=ipNWClassMap.begin(); itNWClass!=ipNWClassMap.end(); ++itNWClass)
	{
		/// �t�B�[�`���N���X�����i�[
		CString strNWClassName( itNWClass->first );

		/// �t�B�[�`���N���X�ɓ�����\���̂���֘A�t�B�[�`�������i�[
		CString strRelClassName( AheGetNetworkSplitTarget( strNWClassName, false ) );
		// ��������̂��Ȃ��Ȃ�ȉ��̏����͕K�v�Ȃ�
		if( strRelClassName.IsEmpty() ) continue;

		CTargetItems cRelFeatures, cTargetNWFeatures;
		// �S�Ă̊֘A�t�B�[�`�� = cTargetFeatures �̊֘A�t�B�[�`�� �{ cNewFeatures �̊֘A�t�B�[�`��
		cTargetFeatures.GetFeatures( cRelFeatures, strRelClassName );
		cNewFeatures.GetFeatures( cRelFeatures, strRelClassName, TRUE );
		// �Ώۂ̃l�b�g���[�N�����N �� cTargetFeatures �̃l�b�g���[�N�����N �{ cNewFeatures �̃l�b�g���[�N�����N
		cTargetFeatures.GetFeatures( cTargetNWFeatures, strNWClassName );
		cNewFeatures.GetFeatures( cTargetNWFeatures, strNWClassName, TRUE );

		// �Ώۂ̃l�b�g���[�N�����N��������f�[�^�Z�b�g�� SpatialReference �����
		// ���H�����N�̃����N�������E�X�Ώ����ŃX�v���b�g��̌`���r�łǂ����Ă�����Ȃ��Ȃ�i2�̃����N���������d�Ȃ��Ă���ꍇ�H�j
		// �̂ŁA���傤���Ȃ�
		ISpatialReferencePtr ipNWSpRef = AheGetSpatialReference( itNWClass->second );

		// �S�Ă̊֘A�t�B�[�`���� EndPoint ��ڂ���l�b�g���[�N�����N�ɒǉ�
		if(!AddEndPointToNetworkLink(cNewFeatures, cRelFeatures, cTargetNWFeatures, ipNWSpRef))
			return FALSE;

		///	// �V�K�ɍ쐬���ꂽ���H�����N�ւ̑����E�`��̔��f�͂����ŏI���̂͂��Ȃ̂ŁA Insert ����
		///	CTargetItems cNewRoadFeatures;
		///	cNewFeatures.GetRoadFeatures( cNewRoadFeatures );
		///	if( cNewRoadFeatures.size() > 0 )
		///	{
		///		CComVariant vaID;
		///		IFeatureCursorPtr ipFeatureCursor;
		///		IFeatureClassPtr ipRoadClass = GetRoadClass();
		///		ipRoadClass->Insert( VARIANT_FALSE, &ipFeatureCursor );
		///		for( CTargetItems::iterator it = cNewRoadFeatures.begin(); it != cNewRoadFeatures.end(); it++ )
		///		{
		///			ipFeatureCursor->InsertFeature( (IFeatureBufferPtr)it->GetFeature(), &vaID );
		///			it->Attach( it->GetFeature(), vaID.lVal );
		///		}
		///	}

		// �S�Ă̊֘A�t�B�[�`���̍\���_�č\�z�y�ъ֘A�����NID�̍Đݒ�
		if(!ReconstructCommonVertexAndResetRelField( cNewFeatures, cTargetFeatures, cNoSetModifyDataFeatures, cTakeOveredFeatures,
			cRelFeatures, cTargetNWFeatures, itNWClass->second, ipNWSpRef))
			return FALSE;
	}
	return TRUE;
}

// �S�Ẵ����N�������y�ьX�΂� EndPoint ��ڂ���l�b�g���[�N�����N�ɒǉ�����
BOOL CEditHelper::AddEndPointToNetworkLink(
	const CTargetItems& cNewFeatures,
	CTargetItems& cSegAttrAndGradFeatures,
	const CTargetItems& cAllRoadFeatures,
	const ISpatialReferencePtr& ipRoadSpRef) const
{
	if(cSegAttrAndGradFeatures.empty()) return TRUE;

	// �S�Ẵ����N�������y�ьX�΂� EndPoint ���擾
	std::list< CAdapt< IPointPtr > > listSegAttrAndGradsEndPoint;
	for( CTargetItems::iterator itSegAttrAndGrads = cSegAttrAndGradFeatures.begin(); itSegAttrAndGrads != cSegAttrAndGradFeatures.end(); ++itSegAttrAndGrads )
	{
		IPointPtr ipSegAttrAndGradsFromPoint = itSegAttrAndGrads->GetFromPoint();
		IPointPtr ipSegAttrAndGradsToPoint   = itSegAttrAndGrads->GetToPoint();
		// �����_�Ɋ��ɂ�����̂͒ǉ����Ȃ�
		BOOL bFindSameFromPoint = FALSE, bFindSameToPoint = FALSE;
		for( std::list< CAdapt< IPointPtr > >::iterator it = listSegAttrAndGradsEndPoint.begin(); it != listSegAttrAndGradsEndPoint.end(); ++it )
		{
			if( bFindSameFromPoint == FALSE && AheIsEqual( it->m_T, ipSegAttrAndGradsFromPoint ) == TRUE )
				bFindSameFromPoint = TRUE;
			if( bFindSameToPoint   == FALSE && AheIsEqual( it->m_T, ipSegAttrAndGradsToPoint   ) == TRUE )
				bFindSameToPoint   = TRUE;

			if( bFindSameFromPoint == TRUE && bFindSameToPoint == TRUE ) break;
		}
		if( bFindSameFromPoint == FALSE ) listSegAttrAndGradsEndPoint.push_back( ipSegAttrAndGradsFromPoint );
		if( bFindSameToPoint   == FALSE ) listSegAttrAndGradsEndPoint.push_back( ipSegAttrAndGradsToPoint   );
	}
	// ���H�����N�ɍ\���_��ǉ�
	for( CTargetItems::const_iterator itAllRoads = cAllRoadFeatures.begin(); itAllRoads != cAllRoadFeatures.end(); ++itAllRoads )
	{
		IPolylinePtr ipRoadGeometry = itAllRoads->GetShape();
		for( std::list< CAdapt< IPointPtr > >::iterator it = listSegAttrAndGradsEndPoint.begin(); it != listSegAttrAndGradsEndPoint.end(); ++it )
		{
			if (!AheIsVertexPoint(it->m_T, ipRoadGeometry, ipRoadSpRef) &&
				AhePointIsOnGeometry(ipRoadGeometry, it->m_T, ipRoadSpRef))
			{
				// �ҏW�\�H
				if( AheIsEditableFeature2( m_piMap, itAllRoads->GetFeature(), NULL, sindyeEditMove, GetRule(), FALSE, GetHWND() ) != TRUE )
				{
					CString strMsg;
					strMsg.Format(CANNOT_ADD_SHARED_SHAPE_POINT_TO_MOVABLE, itAllRoads->GetFeatureClassName(), itAllRoads->GetOID() );
					AheMessageBox( strMsg, RESTRUCTURING_OF_ATTRIBUTE_SLOPE, MB_OK|MB_ICONSTOP );
					return FALSE;
				}

				VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
				LONG lNewPartIndex = -1, lNewSegmentIndex = -1;
				ipRoadGeometry->SplitAtPoint( it->m_T, VARIANT_FALSE, VARIANT_FALSE, &vaIsSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

				AheSetShape( itAllRoads->GetFeature(), ipRoadGeometry );
				// �V�K�ɍ쐬���ꂽ�t�B�[�`���Q�ɑ��݂��Ȃ��ꍇ�̂݌`��ύX�̗���ǉ�
				if( cNewFeatures.IsAlreadyExist( *itAllRoads ) == FALSE )
					AheSetModifyData( itAllRoads->GetFeature(), sindyUpdateTypeShape );
			}
		}
	}
	return TRUE;
}

BOOL CEditHelper::ReconstructCommonVertexAndResetRelField(
	CTargetItems& cNewFeatures,
	CTargetItems& cTargetFeatures,
	const CTargetItems& cNoSetModifyDataFeatures,
	const CTargetItems& cTakeOveredFeatures,
	CTargetItems& cSegAttrAndGradFeatures,
	const CTargetItems& cAllRoadFeatures,
	const IFeatureClassPtr& ipRoadClass,
	const ISpatialReferencePtr& ipRoadSpRef)
{
	for( CTargetItems::iterator itSegAttrAndGrads = cSegAttrAndGradFeatures.begin(); itSegAttrAndGrads != cSegAttrAndGradFeatures.end(); ++itSegAttrAndGrads )
	{
		IPointPtr ipSGFromPoint, ipSGToPoint;
		IGeometryPtr ipSGGeom = itSegAttrAndGrads->GetShapeCopy();
		((IPolylinePtr)ipSGGeom)->get_FromPoint( &ipSGFromPoint );
		((IPolylinePtr)ipSGGeom)->get_ToPoint( &ipSGToPoint );
		
		// �����N�������E�X�΂ɕ�܂���铹�H�������ibug 5564�j
		// ������΁A2 �̒[�_�œ��H������
		CTargetItems cFromRoadItems, cToRoadItems;
		for( CTargetItems::const_iterator itRoads = cAllRoadFeatures.begin(); itRoads != cAllRoadFeatures.end(); ++itRoads )
		{
//			if( ! AheIsContain( ipSGGeom, itRoads->GetShape() ) )
//			{
#ifdef DEBUG
			sindyeSpatialRel sprelFrom = AheTestGeometryRelationship( itRoads->GetShape(), ipSGFromPoint, (sindyeSpatialRel)127, ipRoadSpRef );
			sindyeSpatialRel sprelTo   = AheTestGeometryRelationship( itRoads->GetShape(), ipSGToPoint,   (sindyeSpatialRel)127, ipRoadSpRef );
			ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �X�΁E�����N�������i%d�j�Ɠ��H�����N�i%d�j�̃W�I���g���֌W�F FROM:%d TO:%d\n"), itSegAttrAndGrads->GetOID(), itRoads->GetOID(), sprelFrom, sprelTo );
#endif
			if( AheTestGeometryRelationship( itRoads->GetShape(), ipSGFromPoint, (sindyeSpatialRel)(sindyeSpatialRelTouch|sindyeSpatialRelContain), ipRoadSpRef ) > 0 )
				cFromRoadItems.push_back( *itRoads );
			if( AheTestGeometryRelationship( itRoads->GetShape(), ipSGToPoint,   (sindyeSpatialRel)(sindyeSpatialRelTouch|sindyeSpatialRelContain), ipRoadSpRef ) > 0 )
				cToRoadItems.push_back(   *itRoads );
/*#ifdef DEBUG
			// �����Ώۂ̓��H�����N�G�������g�ǉ�
			AheAddLineElement( m_cArcHelper.GetGraphicsContainer( FALSE ), itRoads->GetShape(), RGB(255,0,0), 2, esriSLSSolid );
#endif*/
		}
		if( cFromRoadItems.size() < 1 || cToRoadItems.size() < 1 )
		{
			// �R�s�[�c�[���Ń����N�������E�X�΂̂ݓn���ꂽ�ꍇ�͓��H���Ȃ��̂ŁA�����Ō������Ă��
			SelectByShapeFromFeatureClass( ipSGFromPoint, cFromRoadItems, ipRoadClass );
			SelectByShapeFromFeatureClass( ipSGToPoint, cToRoadItems, ipRoadClass );
			// �����ł������H�ɂ̓����N�������E�X�΂̒[�_���ǉ�����Ă��Ȃ��̂ŁA�����Œǉ�
			CTargetItems cSGFromAndToRoads = cFromRoadItems;
			cSGFromAndToRoads += cToRoadItems;
			for( CTargetItems::iterator itSGFromAndToRoads = cSGFromAndToRoads.begin(); itSGFromAndToRoads != cSGFromAndToRoads.end(); ++itSGFromAndToRoads )
			{
				IPolylinePtr ipRoadGeometry = itSGFromAndToRoads->GetShape();
				for( LONG k = 0; k < 2; k++ )
				{
					IPointPtr ipSGFromToPoint;
					switch( k )
					{
						case 0: ipSGFromToPoint = ipSGFromPoint; break;
						case 1: ipSGFromToPoint = ipSGToPoint; break;
					}
					// �����̍\���_�ł͂Ȃ��ꍇ�ɍ\���_��ǉ�����
					if( AheIsVertexPoint( ipSGFromToPoint, ipRoadGeometry, ipRoadSpRef ) == FALSE && 
						AhePointIsOnGeometry( ipRoadGeometry, ipSGFromToPoint, ipRoadSpRef ) == TRUE )
					{
						// �ҏW�\�H
						if( AheIsEditableFeature2( m_piMap, itSGFromAndToRoads->GetFeature(), NULL, sindyeEditMove, GetRule(), FALSE, GetHWND() ) == TRUE )
						{
							VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
							LONG lNewPartIndex = -1, lNewSegmentIndex = -1;
							ipRoadGeometry->SplitAtPoint( ipSGFromToPoint, VARIANT_FALSE, VARIANT_FALSE, &vaIsSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

							AheSetShape( itSGFromAndToRoads->GetFeature(), ipRoadGeometry );
							// �V�K�ɍ쐬���ꂽ�t�B�[�`���Q�ɑ��݂��Ȃ��ꍇ�̂݌`��ύX�̗���ǉ�
							if( cNewFeatures.IsAlreadyExist( *itSGFromAndToRoads ) == FALSE )
							{
								AheSetModifyData( itSGFromAndToRoads->GetFeature(), sindyUpdateTypeShape );
								cTargetFeatures.push_back( *itSGFromAndToRoads );
							}
						}
						else {
							CString strMsg;
							strMsg.Format(ERROR_NOT_ADD_SHARED_SHAPE_POINT, itSGFromAndToRoads->GetFeatureClassName(), itSGFromAndToRoads->GetOID() );
							AheMessageBox( strMsg, RESTRUCTURING_ATTRIBUTE_SLOPE, MB_OK|MB_ICONSTOP );
							return FALSE;
						}
					}
				}	
			}

			if( cFromRoadItems.size() < 1 || cToRoadItems.size() < 1 )
			{
	/*#ifdef DEBUG
				// �ڑ����铹�H�����N��������Ȃ������X�΁E�����N�������G�������g�ǉ�
				AheAddLineElement( m_cArcHelper.GetGraphicsContainer( FALSE ), itSegAttrAndGrads->GetShape(), RGB(0,255,0), 2, esriSLSSolid );
				ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : FROM�����Ō����������H�����N�̐��F%d TO�����Ō����������H�����N�̐��F%d\n"), cFromRoadItems.size(), cToRoadItems.size() );
	#endif*/
				CString strMsg;
				strMsg.Format(ERROR_END_POINT_ISNOT_CONTACT_ROAD_LINK, itSegAttrAndGrads->GetFeatureClassName(), itSegAttrAndGrads->GetOID() );
				AheMessageBox( strMsg, SPLIT_OF_ATTRIBUTE_SLOPE, MB_OK|MB_ICONSTOP );
				return FALSE;
			}
		}
#if 0
		CTargetItems cSegRoad;	// �i���݌�̓��H�����N�i�[�p
		BOOL bFinish = FALSE, bFinish2 = FALSE;

		// �I���ł������H�����N�̍i����
		for( CTargetItems::iterator itFromRoads = cFromRoadItems.begin(); itFromRoads != cFromRoadItems.end(); itFromRoads++ )
		{
			for( CTargetItems::iterator itToRoads = cToRoadItems.begin(); itToRoads != cToRoadItems.end(); itToRoads++ )
			{
				// ���ꂼ��̌������ʂ̒��ɓ������H�����N���������ꍇ�͊֘A���铹�H�����N�͂ЂƂł���
				if( itFromRoads->GetOID() == itToRoads->GetOID() )
				{
					cSegRoad.clear();
					cSegRoad.push_back( *itToRoads );
					bFinish = TRUE;
					break;
				}
				else if( bFinish2 == FALSE )
				{
					ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ���C����=%s OID=%d FROM �ɐڂ��铹�H : OBJECTID=%d TO �ɐڂ��铹�H : OBJECTID=%d\n"), itSegAttrAndGrads->GetFeatureClassName(), itSegAttrAndGrads->GetOID(), itFromRoads->GetOID(), itToRoads->GetOID() );
					AheTraceGeometry( itFromRoads->GetShape(), _T("CEditHelper::SplitFeaturesByGeometry() : FROM�ɐڂ��铹�H") );
					AheTraceGeometry( itToRoads->GetShape(),   _T("CEditHelper::SplitFeaturesByGeometry() : TO  �ɐڂ��铹�H") );
					if( AheIsTouchEndPoint( itFromRoads->GetShape(), itToRoads->GetShape(), ipRoadSpRef ) == TRUE )
					{
						// ���ꂼ��̌������ʂ̂Ȃ��Œ[�_���ڂ��Ă�����̂�����΂�����֘A���H�����N�Ƃ���
						cSegRoad.push_back( *itFromRoads );
						cSegRoad.push_back( *itToRoads );
						bFinish2 = TRUE;
					}
				}
			}
			if( bFinish == TRUE ) break;
		}
#else
		// �����N�������E�X�΂̍\���_�́A���ɓ��H�����N�ɋ��L�\���_�Ƃ���
		// �ǉ�����Ă���B�������A��{�̃��[�v�œ��H�����N���������Ȃ���
		// �������Ă���֌W��ǂ����Ă����[�v�O�Ƀ����N�������E�X�΂̋��
		// �Q�Ƃ����킹�鎖���ł��Ȃ��B
		// �����ŁA�s�{�ӂȂ��炱���ŋ�ԎQ�Ƃ����킹��B
		// ���Ȃ݂ɁA���킹�Ȃ��ƃ����N�������E�X�΂̒[�_�𓹘H�����N�̕�
		// �ɃX�i�b�v�����ꍇ��FROM�ATO���肪�ł��Ȃ��Ȃ�B
		//itSegAttrAndGrads->Simplify(); ���ꂪ�����ŃX�v���b�g�������Ă��Ȃ������N�������̌x�����o��ibug 4763�j�B
		// �����N��������P�ƕҏW�œ��H�����N�ӂɃX�i�b�v���Ă����v�Ȃ��Ƃ���A�R�����g�ɂ���B
		CTargetItems cSegRoad( AheGetRelationshipContainFeature( itSegAttrAndGrads->GetFeature(), cFromRoadItems + cToRoadItems ) ); // �i���݌�̓��H�����N
#endif // if 0
		if( ! ( 1 <= cSegRoad.size() && cSegRoad.size() <= 2 ) )
		{
			AheMessageBox( ERROR_NOT_FIND_ROAD_LINK_ATTRIBUTE_SLOPE, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
			return FALSE;
		}

		IPointPtr ipTouchPoint;	// ���H�����N���m�̒[�_���ڂ��Ă���ꏊ�icSegRoad.size() == 2 �̏ꍇ�̂݁j
		if( cSegRoad.size() == 2 )
		{
			// �����N���������邢�͌X�΂̃X�v���b�g�|�C���g�𓹘H�����N���m�̒[�_�̐ړ_���狁�߂�
			CTargetItems::iterator it = cSegRoad.begin();
			IGeometryPtr ipGeom1 = it->GetShapeCopy();
			advance( it, 1 );
			IGeometryPtr ipGeom2 = it->GetShapeCopy();
			IGeometryPtr ipPoints = AheGetCrossOrTouchPoints( ipGeom1, ipGeom2, 1 );
			if( ipPoints == NULL ) 
			{
				AheMessageBox( ERROR_CANNOT_ACROSS_3_ROAD_LINK, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
				return FALSE;
			}
			((IPointCollectionPtr)ipPoints)->get_Point( 0, &ipTouchPoint );
		}
		IPointCollectionPtr ipSplitterPoints( CLSID_Multipoint );
		((IGeometryPtr)ipSplitterPoints)->putref_SpatialReference( AheGetSpatialReference( ipSGFromPoint ) ); // ��ԎQ�ƕt�^
		ipSplitterPoints->AddPoint( ipSGFromPoint );
		ipSplitterPoints->AddPoint( ipSGToPoint );
/*#ifdef DEBUG
		AheTraceGeometry( ipTouchPoint, _T("CEditHelper::SplitFeaturesByGeometry() : ipTouchPoint :") );
		AheTraceGeometry( ipSGFromPoint, _T("CEditHelper::SplitFeaturesByGeometry() : ipSGFromPoint :") );
		AheTraceGeometry( ipSGToPoint, _T("CEditHelper::SplitFeaturesByGeometry() : ipSGToPoint :") );
#endif */
		
		std::list< CAdapt< IGeometryPtr > > listGeometry;
		for( CTargetItems::iterator it = cSegRoad.begin(); it != cSegRoad.end(); ++it )
		{
/*			// �����N�������E�X�΂��ڂ��铹�H�����N�𕪊����āA�����V���������N�������E�X�΂̌`��Ƃ���
			// �̂����A�����_�Q��ipSplitterPoints���g�p����̂͊ԈႢ
			// �����_�Q�̓����N�������E�X�΂�EndPoint�𑫂��K�v������
			IPointCollectionPtr ipSplitterPoints2( AheGetClone( ipSplitterPoints ) );
			ipSplitterPoints2->AddPoint( ipSGFromPoint, &vtMissing, &vtMissing );
			ipSplitterPoints2->AddPoint( ipSGToPoint, &vtMissing, &vtMissing );*/
			_ISetPtr ipSplittedGeomSet = SplitPolylineByPoints( it->GetShapeCopy(), ipSplitterPoints );
			// �����N�������E�X�΂��m�[�h to �m�[�h�̏ꍇ��ipSplittedGeomSet��NULL�ɂȂ�̂ŁA
			// ���̏ꍇ�͓��H�̌`������̂܂ܓ˂����ށibug 5194,5181�j
			if( ! ipSplittedGeomSet )
			{
				ipSplittedGeomSet.CreateInstance( CLSID_Set );
				ipSplittedGeomSet->Add( it->GetShapeCopy() );
			}

			if( ipSplittedGeomSet )
			{
				ipSplittedGeomSet->Reset();

				LONG lCount = -1;
				ipSplittedGeomSet->get_Count( &lCount );

				IUnknownPtr ipUnk;
				while( SUCCEEDED( ipSplittedGeomSet->Next( &ipUnk ) ) )
				{
					IGeometryPtr ipSplittedGeom( ipUnk );
					if( !ipSplittedGeom ) break;

/*#ifdef DEBUG
					AheTraceGeometry( ipSplittedGeom, _T("CEditHelper::SplitFeaturesByGeometry() : ipSplittedGeom :") );
#endif*/
					if( ipTouchPoint )
					{
						// ipTouchPoint �������̂̂�
						if( AheIsEndPoint( ipTouchPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
						{
							// �X�v���b�g��̌`�󂪓��H�̌����ɍ��킹���Ă��܂��̂ŁA���̌����ɂ��킹��
							if( AheIsFromPoint( ipSGToPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE ||
								AheIsToPoint( ipSGFromPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
								((IPolylinePtr)ipSplittedGeom)->ReverseOrientation();

							listGeometry.push_back( ipSplittedGeom );
						}
					}
					else {
						// ipFromPoint �� ipToPoint �𗼒[�_�Ɏ����̂̂�
						if( AheIsEndPoint( ipSGFromPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE && AheIsEndPoint( ipSGToPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
						{
							// �X�v���b�g��̌`�󂪓��H�̌����ɍ��킹���Ă��܂��̂ŁA���̌����ɂ��킹��
							if( AheIsFromPoint( ipSGToPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE ||
								AheIsToPoint( ipSGFromPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
								((IPolylinePtr)ipSplittedGeom)->ReverseOrientation();

							listGeometry.push_back( ipSplittedGeom );
						}
					}
				}
			}
			else {
				// EditTool��
				//  A         B             C
				// ��----�~--------��   ���H�����N�A�m�[�h
				//         --------     �����N������
				// �Ƃ����悤�Ȍ`��ŁAB��CTRL��͂�œ��H�����N�݂̂�
				// �ړ�����
				// 
				//         
				//         --------     �����N������
				// ��              ��   �m�[�h
				//   �_          �^
				//     �_      �^       ���H�����N
				//       �_  �^
				//         �~       
				// �����̎��_�ł́A����Ȋ����ɂȂ��Ă���B
				// �ŁAipSplittedGeomSet�̓����N�������̌`���
				// ���̓��H�����N�̌`��𕪊��������̂������Ă���B
				// ���R�����`��͂Ȃ��AipSplittedSet��NULL�ƂȂ邪�A
				// ������Ƃ����ă����N�����������H�����N�S�̂�
				// �t���Ă���킯�ł͂Ȃ��͈̂�ڗđR�Ǝv���B
				// �Ȃ̂ŁA�����Ŗ{���ɑS�̂ɂ��Ă���̂��i�S�̂ɂ��Ă��ǂ��̂��j
				// �ǂ������`�F�b�N����K�v������
				// -> ����ɂ́A�G���h�|�C���g����v���Ă��邩�ǂ������`�F�b�N����΂悢
				if( //AheIsEqual( it->GetShape(), itSegAttrAndGrads->GetShape() ) ||											// �`�󂪈�v���Ă��邩
					( AheIsEqual( it->GetFromPoint(), ipSGToPoint ) && AheIsEqual( it->GetToPoint(), ipSGFromPoint ) ||		// �G���h�|�C���g����v���Ă��邩
					  AheIsEqual( it->GetFromPoint(), ipSGFromPoint ) && AheIsEqual( it->GetToPoint(), ipSGToPoint ) ) )
				{
					// �����N�S�̂ɂ��Ă���ꍇ
					IGeometryPtr ipSplittedGeom( it->GetShapeCopy() );
					// �X�v���b�g��̌`�󂪓��H�̌����ɍ��킹���Ă��܂��̂ŁA���̌����ɂ��킹��
					if( AheIsFromPoint( ipSGToPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE ||
						AheIsToPoint( ipSGFromPoint, ipSplittedGeom, ipRoadSpRef ) == TRUE )
						((IPolylinePtr)ipSplittedGeom)->ReverseOrientation();

					listGeometry.push_back( ipSplittedGeom );
				}
			}
		}
		// �`��m��
		BOOL bFlg = FALSE;
		if( listGeometry.size() < 1 )
		{
			_ASSERTE( FALSE );	// �����ɗ��Ă͂����Ȃ��I�I
			AheMessageBox( ERROR_END_POINT_NOT_MATCH_ROAD_LINK, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
			return FALSE;
		}
		IFeaturePtr ipTargetFeat;
		for( std::list< CAdapt< IGeometryPtr > >::iterator itListGeom = listGeometry.begin(); itListGeom != listGeometry.end(); ++itListGeom )
		{
			BOOL bSettedModifyData = FALSE;	// �`��ύX�y�ѐV�K�쐬�̗���������ꂽ���ǂ���
			if( bFlg == FALSE )
			{
				// �`�󂪕ύX����Ă���ꍇ�̓A�b�v�f�[�g������������
#ifdef _DEBUG
				AheTraceGeometry( itSegAttrAndGrads->GetShape(), _T("itSegAttrAndGrads->GetShape()") );
				AheTraceGeometry( itListGeom->m_T, _T("itListGeom->m_T") );
				IGeometryPtr ipSegGrashapeCopyTmp( itSegAttrAndGrads->GetShapeCopy() );
				IGeometryPtr ipListGeomTmp( (IGeometryPtr)AheCreateClone( itListGeom->m_T ) );
				AheSnapToSpatialReferenceAndForceSimplify( ipSegGrashapeCopyTmp, itSegAttrAndGrads->GetSpatialReference() );
				AheSnapToSpatialReferenceAndForceSimplify( ipListGeomTmp, itSegAttrAndGrads->GetSpatialReference() );
				AheTraceGeometry( ipSegGrashapeCopyTmp, _T("S:itSegAttrAndGrads->GetShape()") );
				AheTraceGeometry( ipListGeomTmp, _T("S:itListGeom->m_T") );
#endif // ifdef _DEBUG
				if( AheIsEqual( itSegAttrAndGrads->GetShape(), itListGeom->m_T ) == FALSE )
				{
					// ���H�����N���Ƀ����N�������Ȃǂ����݂����ꍇ�ɓ��H�����N��P�ƈړ������ꍇ�A
					// �����Ń����N�������̌`�󂪕ύX����邪�A���̎��܂ŊY�������N�������̕ҏW�s��
					// �`�F�b�N����Ă��Ȃ��悤���c
					// �Ȃ̂ŁA�����ňړ��\���ǂ����`�F�b�N����
					if( AheIsEditableFeature2( m_piMap, itSegAttrAndGrads->GetFeature(), itSegAttrAndGrads->GetShape(), sindyeEditMove ) )
					{
						AheTraceGeometry( itListGeom->m_T, _T("���������N�������E�X�΂̌`����p���iAheSetShape�O�j") );
						AheSetShape( itSegAttrAndGrads->GetFeature(), itListGeom->m_T );
						// ����t�^�Ώۂ̂�
						if( cNoSetModifyDataFeatures.IsAlreadyExist( *itSegAttrAndGrads ) == FALSE )
						{
							itSegAttrAndGrads->SetModifyData( sindyUpdateTypeShape );
							bSettedModifyData = TRUE;
						}
					}
					else {
						return FALSE;
					}
				}
				// �֘A�����N ID �ݒ�
				BOOL bFind = FALSE;
				for( CTargetItems::iterator itRoads = cSegRoad.begin(); itRoads != cSegRoad.end(); ++itRoads )
				{
/*					ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �֘A�����NID�ݒ�F ROAD:OBJECTID=%d\n"), itRoads->GetOID() );
					((ITopologicalOperator2Ptr)itListGeom->m_T)->Simplify();
					((ITopologicalOperator2Ptr)itRoads->GetShape())->Simplify();
					((ITopologicalOperator2Ptr)itListGeom->m_T)->put_IsKnownSimple( VARIANT_TRUE );
					((ITopologicalOperator2Ptr)itRoads->GetShape())->put_IsKnownSimple( VARIANT_TRUE );
					AheTraceGeometry( itListGeom->m_T, _T("�֘A�����NID�ݒ�F�����`��") );
					AheTraceGeometry( itRoads->GetShape(), _T("�֘A�����NID�ݒ�F���H�����N") );
					((ITopologicalOperator2Ptr)itListGeom->m_T)->put_IsKnownSimple( VARIANT_TRUE );*/

					// [bug 1936] Simplify() �������Ă��Ȃ��ƁA���Ȍ������C���̃W�I���g�����Z�͎��s����I�I
					// �������A���̕����Ɍ��肵�Č����΃����N�������y�ьX�΂͍\���_�ɃX�i�b�v���Ă��邽�߃R�s�[�����Ƃ��Ă�����
					// Simplify() �������ď������s���Ă��悢�Ǝv����
					IGeometryPtr ipRoadGeomCopy( itRoads->GetShapeCopy() );
					IGeometryPtr ipGeomCopy( AheGetClone( itListGeom->m_T ) );
					((ITopologicalOperator2Ptr)ipRoadGeomCopy)->put_IsKnownSimple( VARIANT_FALSE );
					((ITopologicalOperatorPtr)ipRoadGeomCopy)->Simplify();
					((ITopologicalOperator2Ptr)ipGeomCopy)->put_IsKnownSimple( VARIANT_FALSE );
					((ITopologicalOperatorPtr)ipGeomCopy)->Simplify();
/*					AheForceSimplify( ipRoadGeomCopy );
					AheForceSimplify( ipGeomCopy );	     <- ���̂����ł̓W�I���g�����Z������ς莸�s����*/
					IRelationalOperatorPtr ipRelOpe( ipRoadGeomCopy );
					VARIANT_BOOL vbContain = VARIANT_FALSE, vbEqual = VARIANT_FALSE;
					ipRelOpe->Contains( ipGeomCopy, &vbContain );
					ipRelOpe->Equals( ipGeomCopy, &vbEqual );
					if( vbContain == VARIANT_TRUE || vbEqual == VARIANT_TRUE )
					//if( itRoads->IsContains2( itListGeom->m_T ) == TRUE )
					{
						// [bug 2883] �֘AID���قȂ�ꍇ�̂݃Z�b�g����悤�ɂ���i����Ȃ��Ƒ�R�ҏW�����j
						if( itRoads->GetOID() != itSegAttrAndGrads->GetAttribute( _T("LINK_ID") ).lVal )
						{
							ATLTRACE(_T("EditHelper::SplitFeaturesByGeometry() : �����N������OR�X�� OBJECTID=%d %d %d\n"), itSegAttrAndGrads->GetOID(), itSegAttrAndGrads->GetAttribute( _T("LINK_ID") ).lVal, itRoads->GetOID() );
							
							/////////////////////////////////////////////////////////////////////////////////
							// [bug4497]LINK_ID�������ς�鎞�ɍX�V�������t���Ȃ��΍�
							// �u�`��ύX�v�́u�����ύX�v�������̂Łu�����ύX�v�Ƃ��Ȃ�
							BOOL bUpdateShape = TRUE;
							LONG OID, OIDTmp = itSegAttrAndGrads->GetOID();
							for( CTargetItems::const_iterator it_over = cTakeOveredFeatures.begin(); it_over != cTakeOveredFeatures.end(); it_over++ ){
								OID = it_over->GetOID();
								if( OID == OIDTmp )
									bUpdateShape = FALSE;
							}
							if( itSegAttrAndGrads->GetAttribute( _T("LINK_ID") ).lVal != 0  && bUpdateShape == TRUE )
								itSegAttrAndGrads->SetModifyData( sindyUpdateTypeProperty );
							/////////////////////////////////////////////////////////////////////////////////

							itSegAttrAndGrads->SetAttribute( _T("LINK_ID"), itRoads->GetOID() );
						}
						bFind = TRUE;
						break;
					}
				}
				if( bFind == FALSE )
				{
					_ASSERTE( bFind == TRUE );	// �����ւ��Ă͂����Ȃ�
					AheMessageBox( ERROR_FAIL_ADD_ROAD_LINK_ID, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
					return FALSE;
				}

				ipTargetFeat = itSegAttrAndGrads->GetFeature();
				bFlg = TRUE;
			}
			else
			{
///				CComVariant vaID;
///				IFeatureClassPtr ipSegAttrOrGradClass = AheGetFeatureClass( itSegAttrAndGrads->GetFeature() );
///				IFeatureCursorPtr ipFeatureCursor;
///				ipSegAttrOrGradClass->Insert( VARIANT_FALSE, &ipFeatureCursor );
///				ipTargetFeat = AheCreateRowBuffer( itSegAttrAndGrads->GetFeature(), itListGeom->m_T );
				AheTraceGeometry( itListGeom->m_T, _T("�V�K�����N�������E�X�΁iAheCreateRow�O�j") );
				ipTargetFeat = AheCreateRow( itSegAttrAndGrads->GetFeature(), itListGeom->m_T );
				AheSetModifyData( ipTargetFeat, sindyUpdateTypeCreate );
				AheCopyAttribute( itSegAttrAndGrads->GetFeature(), ipTargetFeat, TRUE );	// [bug 4043] �������[�h�ŃR�s�[
				// �֘A�����N ID �ݒ�
				BOOL bFind = FALSE;
				for( CTargetItems::iterator itRoads = cSegRoad.begin(); itRoads != cSegRoad.end(); ++itRoads )
				{
					// [bug 2059] ��̕��ň��p�������N�������E�X�΂ւ̌`��ύX���ɂ͓��H�����N�̃R�s�[
					// ��Simplify�������̂Ō`�󔻒�����Ă���̂ɁA�V�K�̂��̂̂Ƃ��i�܂肱���̂��Ɓj
					// �͂���ĂȂ������B���̂��߁A�쐬���������N��������X�΂��m�[�h�ŕ��������ꍇ��
					// �e����Ă��܂��Ă����B
					// �������A���Ȍ����̉��Z���Ƃ��Ȃ�񂩂ˁc
					IGeometryPtr ipRoadGeomCopy( itRoads->GetShapeCopy() );
					IGeometryPtr ipGeomCopy( AheGetClone( itListGeom->m_T ) );
					((ITopologicalOperator2Ptr)ipRoadGeomCopy)->put_IsKnownSimple( VARIANT_FALSE );
					((ITopologicalOperatorPtr)ipRoadGeomCopy)->Simplify();
					((ITopologicalOperator2Ptr)ipGeomCopy)->put_IsKnownSimple( VARIANT_FALSE );
					((ITopologicalOperatorPtr)ipGeomCopy)->Simplify();
/*					AheForceSimplify( ipRoadGeomCopy );
					AheForceSimplify( ipGeomCopy );	     <- ���̂����ł̓W�I���g�����Z������ς莸�s����*/
					IRelationalOperatorPtr ipRelOpe( ipRoadGeomCopy );
					VARIANT_BOOL vbContain = VARIANT_FALSE, vbEqual = VARIANT_FALSE;
					ipRelOpe->Contains( ipGeomCopy, &vbContain );
					ipRelOpe->Equals( ipGeomCopy, &vbEqual );
					if( vbContain == VARIANT_TRUE || vbEqual == VARIANT_TRUE )
					//if( itRoads->IsContains2( itListGeom->m_T ) == TRUE )
					{
						// ���ɂ��Ă���֘A�����N ID �ƈႤ�ꍇ�̂ݑ����������Z�b�g
						if( AheGetAttribute( ipTargetFeat, _T("LINK_ID") ) != CComVariant( itRoads->GetOID() ) )
						{
							ATLTRACE(_T("EditHelper::SplitFeaturesByGeometry() : �����N������OR�X�� OBJECTID=%d %d %d\n"), itSegAttrAndGrads->GetOID(), AheGetAttribute( ipTargetFeat, _T("LINK_ID") ).lVal, itRoads->GetOID() );

							if( itSegAttrAndGrads->GetAttribute( _T("LINK_ID") ).lVal != 0 )
								itSegAttrAndGrads->SetModifyData( sindyUpdateTypeProperty );

							AheSetAttribute( ipTargetFeat, _T("LINK_ID"), CComVariant(itRoads->GetOID()) );
							bFind = TRUE;
							break;
						}
					}
				}
				if( bFind == FALSE )
				{
					_ASSERTE( bFind == TRUE );	// �����ւ��Ă͂����Ȃ�
					AheMessageBox( ERROR_FAIL_TO_ADD_NEWLY_ROAD_LINK_ID, SPLIT_OF_ATTRIBUTE_OR_SLOPE, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
///				// DB �ɏ�������
///				ipFeatureCursor->InsertFeature( (IFeatureBufferPtr)ipTargetFeat, &vaID );
///				cNewFeatures.push_back( ipTargetFeat, vaID.lVal );
				cNewFeatures.push_back( ipTargetFeat );
			}
		}
	}
	return TRUE;
}

BOOL CEditHelper::CheckNWStructure(CTargetItems& cNewFeatures, CTargetItems& cTargetFeatures, const std::list<ROWDEF>& listLQRows) const
{
	// NW�t�B�[�`�����܂�ł��Ȃ��Ȃ珈��������K�v���Ȃ��̂�TRUE��Ԃ�
	if( cTargetFeatures.IsIncludedNetworkItems() == FALSE ) return TRUE;

	// �X�v���b�g������Ƀm�[�h�ƃ����N���{���ɐڑ�����Ă��邩�ǂ�����NW�t�B�[�`�����ƂɃ`�F�b�N
	for(CRuleModelNetwork::const_iterator itNWModel=g_cRuleModelNetwork.begin(); itNWModel!=g_cRuleModelNetwork.end(); ++itNWModel)
	{
		CTargetItems cLastNodes, cLastLinks;
		cTargetFeatures.GetFeature( cLastNodes, itNWModel->second );
		cNewFeatures.GetFeature( cLastNodes, itNWModel->second, TRUE );
		cTargetFeatures.GetFeature( cLastLinks, itNWModel->first );
		cNewFeatures.GetFeature( cLastLinks, itNWModel->first, TRUE );

		// NW�����N�̒[�_��NW�m�[�h���ڑ�����Ă��邩�`�F�b�N
		if(FALSE==CheckNWConnect(cLastLinks, cLastNodes))
		{
			return FALSE;
		}

		/// �`�F�b�N�p�Ƀl�b�g���[�N�����N�Ɋ֘A����t�B�[�`�����i�[
		CString strRelFeatureas( AheGetNetworkSplitTarget( itNWModel->first, false ) );
		CTargetItems cLastRelFeatures;
		cTargetFeatures.GetFeatures( cLastRelFeatures, strRelFeatureas );
		cNewFeatures.GetFeatures( cLastRelFeatures, strRelFeatureas, TRUE );

		// [bug 2924] ���H�����N��P�ƕҏW�����ꍇ�Ƀ����N��������X�΂������ꍇ����i�����N��������X�΂̒[�_�����̓��H�����N�\���_��ҏW�����ꍇ�j
		// ���̕����Ƀ`�F�b�N������̂���ԉe�������Ȃ����Ȃ̂ł����Ń`�F�b�N����
		if(FALSE==CheckCommonFeaturesConnect(cLastLinks, cLastRelFeatures))
		{
			return FALSE;
		}

		// �ȉ��̃`�F�b�N��ROAD_LINK�������s��
		if( 0!=itNWModel->first.CompareNoCase( road_link::kTableName) )
			continue;

		// �����N��̐��������Ă��邩�`�F�b�N
		if(FALSE==CheckLQConnect(cNewFeatures, cLastLinks, listLQRows))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CEditHelper::CheckNWConnect(const CTargetItems& cLastRoads, const CTargetItems& cLastNodes) const
{
	for( CTargetItems::const_iterator itLastRoads = cLastRoads.begin(); itLastRoads != cLastRoads.end(); ++itLastRoads )
	{
		CComVariant vaFromNodeID = itLastRoads->GetAttribute( FROM_FIELD );
		CComVariant vaToNodeID   = itLastRoads->GetAttribute( TO_FIELD );
		BOOL bFindFromNode = FALSE, bFindToNode = FALSE, bIsFromNode = FALSE, bIsToNode = FALSE;

		for( CTargetItems::const_iterator itLastNodes = cLastNodes.begin(); itLastNodes != cLastNodes.end(); ++itLastNodes )
		{
			if( bFindFromNode == FALSE && vaFromNodeID.lVal == itLastNodes->GetOID() )
			{
				bFindFromNode = TRUE;
				if( AheIsFromPoint( itLastNodes->GetShape(), itLastRoads->GetFeature() ) == TRUE )
					bIsFromNode = TRUE;
				else
				{
					// TODO ���H�����N��p�̃R�����g�ɂȂ��Ă���̂ŁA�R�����g������K�v�L��
					CString strMsg;
					strMsg.Format( _T("CEditHelper::SplitFeaturesByGeometry() : ���H�����N�i%d�j"), itLastRoads->GetOID() );
					AheTraceGeometry( itLastRoads->GetShape(), strMsg );
					strMsg.Format( _T("CEditHelper::SplitFeaturesByGeometry() :  FROM�m�[�h�i%d�j"), itLastNodes->GetOID() );
					AheTraceGeometry( itLastNodes->GetShape(), strMsg );

					strMsg.Format(ERROR_COORD_AND_FROM_NOTE_ID_NOT_MATCH, itLastRoads->GetOID(), vaFromNodeID.lVal );
					AheMessageBox( strMsg, ERROR_FAIL_ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
			}
			else if( bFindToNode == FALSE && vaToNodeID.lVal == itLastNodes->GetOID() )
			{
				bFindToNode = TRUE;
				if( AheIsToPoint( itLastNodes->GetShape(), itLastRoads->GetFeature() ) == TRUE )
					bIsToNode = TRUE;
				else
				{
					// TODO ���H�����N��p�̃R�����g�ɂȂ��Ă���̂ŁA�R�����g������K�v�L��
					CString strMsg;
					strMsg.Format( _T("CEditHelper::SplitFeaturesByGeometry() : ���H�����N�i%d�j"), itLastRoads->GetOID() );
					AheTraceGeometry( itLastRoads->GetShape(), strMsg );
					strMsg.Format( _T("CEditHelper::SplitFeaturesByGeometry() :  TO�m�[�h�i%d�j"), itLastNodes->GetOID() );
					AheTraceGeometry( itLastNodes->GetShape(), strMsg );

					strMsg.Format(ERROR_COORD_AND_TO_NOTE_ID_NOT_MATCH, itLastRoads->GetOID(), vaToNodeID.lVal );
					AheMessageBox( strMsg, ERROR_FAIL_ROAD_LINK_SPLIT, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
			}
		}
		if( bFindFromNode == FALSE || bFindToNode == FALSE )
			ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : ���H�����N�i%d�jFROM�ATO �̃m�[�h��������܂���ł����̂ŏ������΂��܂��B\n"), itLastRoads->GetOID() );
	}
	return TRUE;
}

BOOL CEditHelper::CheckCommonFeaturesConnect(const CTargetItems& cLastRoads, const CTargetItems& cLastSegGrad) const
{
	// �V�K�쐬�E�X�V���ꂽ�S�Ă̓��H�����N�Ɋւ��āA�Ή����郊���N�������E�X�΂��܂܂�邩�m�F
	for( CTargetItems::const_iterator itLastRoads = cLastRoads.begin(); itLastRoads != cLastRoads.end(); ++itLastRoads )
	{
		for( CTargetItems::const_iterator itLastSegGrad = cLastSegGrad.begin(); itLastSegGrad != cLastSegGrad.end(); ++itLastSegGrad )
		{
			if( itLastSegGrad->GetAttribute( _T("LINK_ID") ) != CComVariant(itLastRoads->GetOID()) )
				continue;
			// ���H�̌`��̒��Ƀ����N�������E�X�΂̌`�󂪊��S�Ɋ܂܂�Ă���Ȃ���Ȃ��̂Ŏ�������
			if( AheIsContain( itLastRoads->GetShape(), itLastSegGrad->GetShape() ) ) continue;

			// [bug 1755] ���Ȍ������Ă��铹�H�����N�ł͕K�����s����̂ŁASimplifyGeometry�������čĒ���
			IRelationalOperatorPtr ipRoadTmp = itLastRoads->GetShapeCopy();
			IGeometryPtr ipSegGradTmp = itLastSegGrad->GetShapeCopy();
			VARIANT_BOOL vb = VARIANT_FALSE;
			if( ipRoadTmp && ipSegGradTmp )
			{
				ITopologicalOperator2Ptr ipTopo1 = ipRoadTmp, ipTopo2 = ipSegGradTmp;
				if( ipTopo1 && ipTopo2 )
				{
					ipTopo1->put_IsKnownSimple( VARIANT_FALSE );
					ipTopo2->put_IsKnownSimple( VARIANT_FALSE );
					ipTopo1->Simplify();
					ipTopo2->Simplify();
				}
				ipRoadTmp->Contains( ipSegGradTmp, &vb );
				if( vb ) 
					ATLTRACE(_T("CEditHelper::SplitFeaturesByGeomtry() : ���H�����N[%d] �����N������or�X��[%d] : Simplify��̍Ē����Contains����\n"),
						itLastRoads->GetOID(), itLastSegGrad->GetOID() );
				else {
#ifdef _DEBUG
					CString strMsg;
					CString strSegGrad = ( itLastSegGrad->GetFeatureClassName().CompareNoCase(_T("GRAD") ) == 0 ) ? _T("�X��") : _T("�����N������");
					strMsg.Format(_T("���H�����N�F%d %s�F%d : %s�̌`�󂪓��H�����N���痣��Ă��܂��B"),
						itLastRoads->GetOID(), strSegGrad, itLastSegGrad->GetOID(), strSegGrad );
					ATLTRACE(_T("\n"), strMsg );

					strMsg.Format(_T("�`��s��v�iSimplify�O�j[%s:%d]"), itLastRoads->GetFeatureClassName(), itLastRoads->GetOID() );
					AheTraceGeometry( itLastRoads->GetShape(), strMsg );
					strMsg.Format(_T("�`��s��v�iSimplify�O�j[%s:%d]"), itLastSegGrad->GetFeatureClassName(), itLastSegGrad->GetOID() );
					AheTraceGeometry( itLastSegGrad->GetShape(), strMsg );

					strMsg.Format(_T("�`��s��v�iSimplify��j[%s:%d]"), itLastRoads->GetFeatureClassName(), itLastRoads->GetOID() );
					AheTraceGeometry( (IGeometryPtr)ipRoadTmp, strMsg );
					strMsg.Format(_T("�`��s��v�iSimplify��j[%s:%d]"), itLastSegGrad->GetFeatureClassName(), itLastSegGrad->GetOID() );
					AheTraceGeometry( (IGeometryPtr)ipSegGradTmp, strMsg );
#ifdef _DEBUG_FOR_2059
					AddGeometryToShapeFile( _T("C:\\Home\\furu2\\work\\src-svn\\SiNDY-e\\SketchTool"), _T("SPLITTED"), _T("���H�����N"), itLastRoads->GetShapeCopy(), false, false, false );
					AddGeometryToShapeFile( _T("C:\\Home\\furu2\\work\\src-svn\\SiNDY-e\\SketchTool"), _T("SPLITTED"), _T("�X�΁E�����N������"), itLastSegGrad->GetShapeCopy(), false, false, false );
#endif // ifdef _DEBUG_FOR_2059
#endif // ifdef _DEBUG
				}
			}
			if( ! vb )
			{
				CString strMsg;
				CString strSegGrad = ( itLastSegGrad->GetFeatureClassName().CompareNoCase(_T("GRAD") ) == 0 ) ? SLOPE : ROAD_STRUCTURE_ATTRIBUTE;
				strMsg.Format(ERROR_SHAPE_IS_APART_FROM_ROAD_LINK, itLastRoads->GetOID(), strSegGrad, itLastSegGrad->GetOID(), strSegGrad );
				MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
#ifndef _DEBUG
				return FALSE;
#endif // ifdef _DEBUG
			}
		}
	}
	return TRUE;
}

BOOL CEditHelper::CheckLQConnect(const CTargetItems& cNewFeatures, CTargetItems& cLastRoads, const std::list<ROWDEF>& listLQRows) const
{
	// cNewFeatures�͂����ł����g���Ă��Ȃ�
	if( cNewFeatures.empty() ) return TRUE;

#ifdef _DEBUG
	ATLTRACE(_T("###########################################################\n"));
	ATLTRACE(_T("�����N��̃`�F�b�N���J�n���܂�\n"));
	DWORD dwSTART = GetTickCount();
#endif // ifdef _DEBUG
	VARIANT_BOOL vbRecycle = VARIANT_FALSE;	// LQ�̏C��������ꍇ�����邽��
	// [bug 3521] �����N��̃X�v���b�g�����������������̂��߂̃`�F�b�N
	CTargetItems::iterator itLastRoad = cLastRoads.begin();
	if( itLastRoad == cLastRoads.end() ) return TRUE;

	IDatasetPtr ipDataset( itLastRoad->GetFeatureClass() );
	IWorkspacePtr ipWorkspace;
	GetEditor()->get_EditWorkspace( &ipWorkspace );

	// ���H�����N�̃I�[�i�[�����擾
	CString strOwnerName = AheGetFeatureClassOwnerName( itLastRoad->GetFeatureClass() );

	std::list<long> listRoadID;
	///////////////////////////////////////////////////////////////////////////////////
	// �Y������ LQ ������
	///////////////////////////////////////////////////////////////////////////////////
	std::map<long, std::map<long, std::list<LQDEF> > > mapLQ;	// first key : kind of INF, second key : INF ID

	// LQ���������邽�߂̃N�G���t�B���^�쐬
	IQueryFilterPtr ipQuery( CLSID_QueryFilter ), ipQueryLQ( CLSID_QueryFilter );
	ipQuery->AddField( CComBSTR(_T("INF_ID")) );
	ipQueryLQ->AddField( CComBSTR(_T("INF_ID")) );
	ipQueryLQ->AddField( CComBSTR(_T("LINK_ID")) );
	ipQueryLQ->AddField( CComBSTR(_T("OBJECTID")) );
	ipQueryLQ->AddField( CComBSTR(_T("SEQUENCE")) );
	ipQueryLQ->AddField( CComBSTR(_T("LASTLINK_F")) );
	ipQueryLQ->AddField( CComBSTR(_T("LINKDIR_C")) );

	// ���H�����N��ID�Ō������邽�߂�Where����쐬
	std::list<CString> listWhere;
	static const long MAX_OID_LEN = 27;			// OID��27�����ő���Ĕ��ΌN�������Ă�
	static const long MAX_IN_OID_NUM = 1000;	// IN��ID����1000���ő���Đ`�N�������Ă�
	static const long MAX_IN_LEN = MAX_OID_LEN * MAX_IN_OID_NUM;
	static const size_t IN_BUFFER_SIZE = sizeof(TCHAR) * MAX_IN_LEN;
	static const long MAX_IN_LEN_CANADD = MAX_IN_LEN - ( MAX_OID_LEN + 1 );
	TCHAR strWhere[MAX_IN_LEN];
	LPTSTR lpszWhereEnd = strWhere;
	FillMemory( strWhere, IN_BUFFER_SIZE, _T('\0') );
	for( CTargetItems::iterator itRoad = cLastRoads.begin(), itRoadEnd = cLastRoads.end(); itRoad != itRoadEnd; ++itRoad )
	{
		lstrcat( lpszWhereEnd, ( 0 < lstrlen( strWhere ) ) ? _T(",") : _T("LINK_ID IN (") );
		lpszWhereEnd = &strWhere[lstrlen( strWhere )];

		_ltot( itRoad->GetOID(), lpszWhereEnd, 10 );
		lpszWhereEnd = &strWhere[lstrlen( strWhere )];
		if( lstrlen( strWhere ) > MAX_IN_LEN_CANADD/* ",OID���"�̕�*/ )
		{
			lstrcat( lpszWhereEnd, _T(")") );
			listWhere.push_back( strWhere );
			FillMemory( (LPTSTR)strWhere, IN_BUFFER_SIZE, _T('\0') );
			lpszWhereEnd = strWhere;
		}
	}
	if( 0 < lstrlen( strWhere ) )
	{
		lstrcat( lpszWhereEnd, _T(")") );
		listWhere.push_back( strWhere );
	}
#ifdef _DEBUG
	DWORD dwSTART2 = GetTickCount();
	ATLTRACE(_T("INFO : �O�������� : %.3lf\n"), ((double)(dwSTART2 - dwSTART))/1000.0 );
#endif // ifdef _DEBUG
	for ( INT i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ ) 
	{
#ifdef _DEBUG
		DWORD dwLQSTART = GetTickCount();
#endif // ifdef _DEBUG
		CString strLQTableName        = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQTableName        );
		CString strLQRFFieldName      = m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName  );

		// �����N��̃e�[�u���擾�i���H�����݂��郏�[�N�X�y�[�X������Ȃ��ƃ_���j
		ITablePtr ipLQTable = AheOpenTableByTableName( ipWorkspace, ( strOwnerName.IsEmpty() ) ? strLQTableName : strOwnerName + _T(".") + strLQTableName );

		if( NULL != ipLQTable )
		{
			// INF��ID���擾
			std::list<long> listInfIDs;	// INF��ID�i�[�p
			for( std::list<CString>::const_iterator it = listWhere.begin(), itEnd = listWhere.end(); it != itEnd; ++it )
			{
				_ICursorPtr ipCursor;
				ipQuery->put_WhereClause( CComBSTR( *it ) );
				ipLQTable->Search( ipQuery, vbRecycle, &ipCursor );

				if( NULL != ipCursor )
				{
					// �������̂��߃C���f�b�N�X�ԍ����擾���Ă���
					long lIdx_InfID = -1;
					ipCursor->FindField( CComBSTR(_T("INF_ID")), &lIdx_InfID );

					_IRowPtr ipRow;
					CComVariant va;

					while( S_OK == ipCursor->NextRow( &ipRow ) )
					{
						if( NULL == ipRow ) break;

						ipRow->get_Value( lIdx_InfID, &va );
						listInfIDs.push_back( va.lVal );
					}
				}
				ipCursor = NULL;
			}
			listInfIDs.sort();		// �d���h�~
			listInfIDs.unique();	// �d���h�~

			// �擾����INF��ID�Ō������邽�߂�Where����쐬
			std::list<CString> listWhereINFID;
			lpszWhereEnd = strWhere;
			FillMemory( strWhere, IN_BUFFER_SIZE, _T('\0') );
			for( std::list<long>::iterator itInfID = listInfIDs.begin(), itInfIDEnd = listInfIDs.end(); itInfID != itInfIDEnd; ++itInfID )
			{
				lstrcat( lpszWhereEnd, ( 0 < lstrlen( strWhere ) ) ? _T(",") : _T("INF_ID IN (") );
				lpszWhereEnd = &strWhere[lstrlen( strWhere )];

				_ltot( *itInfID, lpszWhereEnd, 10 );
				lpszWhereEnd = &strWhere[lstrlen( strWhere )];
				if( lstrlen( strWhere ) > MAX_IN_LEN_CANADD )
				{
					lstrcat( lpszWhereEnd, _T(")") );
					listWhereINFID.push_back( strWhere );
					FillMemory( (LPTSTR)strWhere, IN_BUFFER_SIZE, _T('\0') );
					lpszWhereEnd = strWhere;
				}
			}
			if( 0 < lstrlen( strWhere ) )
			{
				lstrcat( lpszWhereEnd, _T(")") );
				listWhereINFID.push_back( strWhere );
			}

			// �擾����INF��ID��LQ����������
			for( std::list<CString>::const_iterator itWhere = listWhereINFID.begin(), itWhereEnd = listWhereINFID.end(); itWhere != itWhereEnd; ++itWhere )
			{
				_ICursorPtr ipCursor;
				ipQueryLQ->put_WhereClause( CComBSTR( *itWhere ) );
				ipLQTable->Search( ipQueryLQ, vbRecycle, &ipCursor );

				if( NULL != ipCursor )
				{						
					long lIdx_LinkID = -1, lIdx_Seq = -1, lIdx_LastLinkF = -1, lIdx_LinkDirC = -1, lIdx_InfID = -1;
					ipCursor->FindField( CComBSTR(_T("LINK_ID")), &lIdx_LinkID );
					ipCursor->FindField( CComBSTR(_T("SEQUENCE")), &lIdx_Seq );
					ipCursor->FindField( CComBSTR(_T("LASTLINK_F")), &lIdx_LastLinkF );
					ipCursor->FindField( CComBSTR(_T("LINKDIR_C")), &lIdx_LinkDirC );
					ipCursor->FindField( CComBSTR(_T("INF_ID")), &lIdx_InfID );

					_IRowPtr ipRow;
					CComVariant va;
					LQDEF def;

					// ������
					def.oid = def.linkid = def.seq = def.dir = def.infid = -1;
					def.last = false;
					def.kind = (sindyeRoadQueueType)i;

					// ��ɁA�X�v���b�g�����Ŏg�p����listLQRows���i�[����
					// �������Ȃ��ƁA�܂��ҏW�Z�b�V�������I�����Ă��Ȃ�����
					// �������Ă��ύX���ʂ����f����Ă��Ȃ�
					for( std::list<ROWDEF>::const_iterator it = listLQRows.begin(), itEnd = listLQRows.end(); it != itEnd; ++it )
					{
						if( i == m_cLoadQueueInfo.GetLoadQueueType( it->flg ) )
						{
							if( i == m_cLoadQueueInfo.GetLoadQueueType( it->flg ) )
							{
								ipRow = it->obj;

								ipRow->get_OID( &def.oid );
								ipRow->get_Value( lIdx_LinkID,    &va ); def.linkid = va.lVal;
								ipRow->get_Value( lIdx_Seq,       &va ); def.seq    = va.lVal;
								ipRow->get_Value( lIdx_LinkDirC,  &va ); def.dir    = va.lVal;
								ipRow->get_Value( lIdx_LastLinkF, &va ); def.last   = ( 0 == va.lVal ) ? false : true;
								ipRow->get_Value( lIdx_InfID,     &va ); def.infid  = va.lVal;
								def.obj = ipRow;
#ifdef _LQCHECK_TRACE_LQ
								ATLTRACE(_T("INFO : LQ�o�^�i�����ρj : ") );
								TraceLQDEF( def );
#endif // ifdef _LQCHECK_TRACE_LQ

								mapLQ[def.kind][def.infid].push_back( def );

								listRoadID.push_back( def.linkid );	// �����N��ID�͂����Œǉ����Ă���
							}
						}
					}

					// �����������̂��擾
					while( S_OK == ipCursor->NextRow( &ipRow ) )
					{
						if( NULL == ipRow ) break;

						ipRow->get_OID( &def.oid );
						ipRow->get_Value( lIdx_LinkID,    &va ); def.linkid = va.lVal;
						ipRow->get_Value( lIdx_Seq,       &va ); def.seq    = va.lVal;
						ipRow->get_Value( lIdx_LinkDirC,  &va ); def.dir    = va.lVal;
						ipRow->get_Value( lIdx_LastLinkF, &va ); def.last   = ( 0 == va.lVal ) ? false : true;
						ipRow->get_Value( lIdx_InfID,     &va ); def.infid  = va.lVal;
						def.obj = ipRow;

						// ������������ꍇ�͎̂Ă�
						std::list<LQDEF>& listLQDefTmp = mapLQ[def.kind][def.infid];
						if( listLQDefTmp.end() == find( listLQDefTmp.begin(), listLQDefTmp.end(), def ) )
						{
#ifdef _LQCHECK_TRACE_LQ
							ATLTRACE(_T("INFO : LQ�o�^�i�V�����j : ") );
							TraceLQDEF( def );
#endif // ifdef _LQCHECK_TRACE_LQ

							listLQDefTmp.push_back( def );

							listRoadID.push_back( def.linkid );	// �����N��ID�͂����Œǉ����Ă���
						}
#ifdef _LQCHECK_TRACE_LQ 
						else {
							ATLTRACE(_T("INFO : ����LQ���������̂œo�^���܂��� : ") );
							TraceLQDEF( def );
						}
#endif // ifdef _LQCHECK_TRACE_LQ
					}
					ipCursor = NULL;
				}
			}
#ifdef _DEBUG
			ATLTRACE(_T("INFO :   LQ�����i%d�j : %.3lf\n"), i, ((double)(GetTickCount()-dwLQSTART))/1000.0 );
#endif // ifdef _DEBUG
		}
	}
#ifdef _DEBUG
	DWORD dwSTART3 = GetTickCount();
	ATLTRACE(_T("INFO : LQ���� : %.3lf\n"), ((double)(dwSTART3-dwSTART2))/1000.0 );
	dwSTART2 = dwSTART3;
#endif // ifdef _DEBUG
	///////////////////////////////////////////////////////////////////////////////////
	// ��������LQ�Ɋ֘A���铹�H�����N��I��
	///////////////////////////////////////////////////////////////////////////////////
	listRoadID.sort();
	listRoadID.unique();

	// �������ׂ̈ɃC���f�b�N�X�ԍ�������Ă���
	std::map<sindyeRoadQueueType,long> mapLQRFIndex;
	ITablePtr ipTable = cLastRoads.begin()->GetFeatureClass();
	IFieldsPtr ipFields;
	ipTable->get_Fields( &ipFields );
	for( int i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; ++i )
	{
		if( FAILED( ipFields->FindField( CComBSTR( m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName ) ), &mapLQRFIndex[(sindyeRoadQueueType)i] ) ) )
			mapLQRFIndex[(sindyeRoadQueueType)i] = -1; // �����N��̑��݂͔C�ӁiiPC VICS�Ή��j
	}

	// ���ɑI������Ă�����̂Ɋւ��Ă͐�Ɏ���Ă���
	ROADDEF def;
	std::map<long, ROADDEF> mapRoad;
	IFeaturePtr ipFeat;
	CComVariant va;
	for( CTargetItems::const_iterator itRoad = cLastRoads.begin(), itRoadEnd = cLastRoads.end(); itRoad != itRoadEnd; ++itRoad )
	{
		ipFeat = itRoad->GetFeature();
		ipFeat->get_OID( &def.oid );
		ipFeat->get_ShapeCopy( &def.shape );

		def.rf = 0;
		for( std::map<sindyeRoadQueueType,long>::const_iterator itIndex = mapLQRFIndex.begin(); itIndex != mapLQRFIndex.end(); ++itIndex )
		{
			if( 0 < itIndex->second ) // �����N��̑��݂͔C�ӁiiPC VICS�Ή��j
			{
				ipFeat->get_Value( itIndex->second, &va );
				def.rf = ( 0 == va.lVal ) ? def.rf : def.rf | m_cLoadQueueInfo.GetLoadQueueDrawMode(itIndex->first);
			}
		}
		mapRoad[def.oid] = def;

		// ���X�g�ɓo�^����Ă�����폜
		listRoadID.remove( def.oid );
	}

	// �擾����INF��ID�Ō������邽�߂�Where����쐬
	std::list<CString> listWhereRoadID;
	lpszWhereEnd = strWhere;
	FillMemory( strWhere, IN_BUFFER_SIZE, _T('\0') );
	for( std::list<long>::iterator itRoadID = listRoadID.begin(), itRoadIDEnd = listRoadID.end(); itRoadID != itRoadIDEnd; ++itRoadID )
	{
		lstrcat( lpszWhereEnd, ( 0 < lstrlen( strWhere ) ) ? _T(",") : _T("OBJECTID IN (") );
		lpszWhereEnd = &strWhere[lstrlen( strWhere )];

		_ltot( *itRoadID, lpszWhereEnd, 10 );
		lpszWhereEnd = &strWhere[lstrlen( strWhere )];
		if( lstrlen( strWhere ) > MAX_IN_LEN_CANADD )
		{
			lstrcat( lpszWhereEnd, _T(")") );
			listWhereRoadID.push_back( strWhere );
			FillMemory( (LPTSTR)strWhere, IN_BUFFER_SIZE, _T('\0') );
			lpszWhereEnd = strWhere;
		}
	}
	if( 0 < lstrlen( strWhere ) )
	{
		lstrcat( lpszWhereEnd, _T(")") );
		listWhereRoadID.push_back( strWhere );
	}
	// �N�G���t�B���^�쐬
	CString strSubFields(_T("OBJECTID,SHAPE,SHAPE.LEN"));
	for( std::map<sindyeRoadQueueType,long>::const_iterator itIndex = mapLQRFIndex.begin(); itIndex != mapLQRFIndex.end(); ++itIndex )
	{
		if( 0 < itIndex->second ) // LQRF�t�B�[���h�����݂��Ȃ��ꍇ���l��
			strSubFields += _T(",") + m_cLoadQueueInfo.GetLoadQueueInfo( itIndex->first, sindyeLoadQueueInfoLQRFRoadFieldName );
	}
	ipQuery->put_SubFields( CComBSTR(strSubFields) );

	// ����
	for( std::list<CString>::const_iterator itWhereRoadID = listWhereRoadID.begin(), itWhereRoadIDEnd = listWhereRoadID.end(); itWhereRoadID != itWhereRoadIDEnd; ++itWhereRoadID )
	{
		ipQuery->put_WhereClause( CComBSTR(*itWhereRoadID) );

		_ICursorPtr ipCursor;
		ipTable->Search( ipQuery, vbRecycle, &ipCursor );

		if( NULL != ipCursor )
		{
			_IRowPtr ipRow;
			while( S_OK == ipCursor->NextRow( &ipRow ) ) 
			{
				ipFeat = ipRow;

				if( NULL == ipFeat ) break;

				ipFeat->get_OID( &def.oid );
				ipFeat->get_ShapeCopy( &def.shape );

				def.rf = 0;
				for( std::map<sindyeRoadQueueType,long>::const_iterator itIndex = mapLQRFIndex.begin(); itIndex != mapLQRFIndex.end(); ++itIndex )
				{
					if( 0 < itIndex->second ) // LQRF�t�B�[���h�����݂��Ȃ��ꍇ���l��
					{
						ipFeat->get_Value( itIndex->second, &va );
						def.rf = ( 0 == va.lVal ) ? def.rf : def.rf | m_cLoadQueueInfo.GetLoadQueueDrawMode(itIndex->first);
					}
				}
				mapRoad[def.oid] = def;
			}
		}
	}
#ifdef _DEBUG
	dwSTART3 = GetTickCount();
	ATLTRACE(_T("INFO : ���H�����N���� : %.3lf\n"), ((double)(dwSTART3-dwSTART2))/1000.0 );
	dwSTART2 = dwSTART3;
#endif // ifdef _DEBUG
LQ_CHECK:
	///////////////////////////////////////////////////////////////////////////////////
	// �`�F�b�N�J�n
	///////////////////////////////////////////////////////////////////////////////////
	// �֘A�t���O��LQ�̐��������Ă��邩
	//   �ECTargetItems�̓��H�����N�Ɋ֘A�t���O���t���Ă��Ȃ��̂�LQ������Ȃ炨������
	//   �ECTargetItems�̓��H�����N�Ɋ֘A�t���O���t���Ă���̂�LQ�������̂͂�������
	//
	// LQ�̌`�󂪈�{�̃��C���ɂȂ邩
	//   �E�V�[�P���X�̏��ԋy�ѕ����R�[�h�̂Ƃ���ɂȂ���Ȃ���΂�������
	//
	// �V�[�P���X�̕��т̐��������Ă��邩
	//   �E0����n�܂��Ă��Ȃ���΂�������
	//   �E�ԍ������ł���̂͂�������
	//   �E�Ō��LQ�͏I�[�t���O��������΂�������
	for( std::map<long, std::map<long, std::list<LQDEF> > >::iterator it = mapLQ.begin(), itEnd = mapLQ.end(); it != itEnd; ++it )
	{
		for( std::map<long, std::list<LQDEF> >::iterator it2 = it->second.begin(), it2End = it->second.end(); it2 != it2End; ++it2 )
		{
			it2->second.sort();								// �V�[�P���X���Ƀ\�[�g
			it2->second.unique();

			long lSeq = 1;
			IPointPtr ipPrevEndPoint;
			for( std::list<LQDEF>::const_iterator itLQ = it2->second.begin(), itLQEnd = it2->second.end(); itLQ != itLQEnd; ++itLQ, ++lSeq )
			{
				// �֘A�t���O���t���Ă��邩�ǂ����̃`�F�b�N
				std::map<long,ROADDEF>::const_iterator it = mapRoad.find(itLQ->linkid);
				if( mapRoad.end() != it )
				{
					if( ! ( it->second.rf & m_cLoadQueueInfo.GetLoadQueueDrawMode(itLQ->kind) ) )
					{
						// �C��
						if( FixLQSequence( it2->second, mapRoad ) )
						{
							// ������x�ŏ�����`�F�b�N
							goto LQ_CHECK;
						}
						else {
							// �G���[
							CString strMsg;
							strMsg.Format(ERROR_FLAG_DOES_NOT_COME_WITH_ROAD_LINK, itLQ->oid, itLQ->linkid, m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)itLQ->kind, sindyeLoadQueueInfoLQRFRoadFieldName ) );
							ATLTRACE(_T("ERROR : %s\n"), strMsg );
							MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
							return FALSE;
						}
					}
				}
				else {
					// �C��
					if( FixLQSequence( it2->second, mapRoad ) )
					{
						// ������x�ŏ�����`�F�b�N
						goto LQ_CHECK;
					}
					else {
						// �G���[
						CString strMsg;
						strMsg.Format(ERROR_ROAD_LINK_NOT_EXIST_OR_SEARCH_FAIL, itLQ->oid, itLQ->linkid );
						ATLTRACE(_T("ERROR : %s\n"), strMsg );
						MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
						return FALSE;
					}
				}

				// �V�[�P���X�ԍ��̐����`�F�b�N
				if( itLQ->seq != lSeq )
				{
					// �C��
					if( FixLQSequence( it2->second, mapRoad ) )
					{
						// ������x�ŏ�����`�F�b�N
						goto LQ_CHECK;
					}
					else {
						// �G���[
						CString strMsg;
						strMsg.Format(ERROR_SOMETHING_WRONG_WITH_SEQUENCE_NUMBER );
						ATLTRACE(_T("ERROR : %s\n"), strMsg );
						for_each( it2->second.begin(), it2->second.end(), TraceLQDEF );

						for( std::list<LQDEF>::iterator it3 = it2->second.begin(), it3End = it2->second.end(); it3 != it3End; ++it3 )
							strMsg += GetTraceLQDEFString( *it3 ) + _T("\n");
						MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
						return FALSE;
					}
				}
				// LQ�Ɗ֘A�Â��Ă��铹�H�����N�̒[�_���m���ڂ��Ă��邩�ǂ����`�F�b�N
				// ���݂�LQ�̊J�n�_���擾
				IPolylinePtr ipRoadLine( mapRoad[itLQ->linkid].shape );
				if( NULL != ipRoadLine )
				{
					IPointPtr ipFirst, ipEnd;
					ipRoadLine->get_FromPoint( ( 1 == itLQ->dir ) ? &ipFirst : &ipEnd   );
					ipRoadLine->get_ToPoint(   ( 1 == itLQ->dir ) ? &ipEnd   : &ipFirst );
					if( NULL != ipPrevEndPoint )
					{
/*								// [bug 3569 �㔼] ��ԎQ�ƂɈ�v�����Ȃ��ƁA�X�i�b�v���Ȃ��ŃX�v���b�g�����ꍇ��
						// �ǂ����Ă�����Ȃ��Ȃ�
						ipPrevEndPoint->SnapToSpatialReference();
						ipFirst->SnapToSpatialReference();

						double dx1, dx2, dy1, dy2;
						ipPrevEndPoint->QueryCoords( &dx1, &dy1 );
						ipFirst->QueryCoords( &dx2, &dy2 );
						// ���݂�LQ�̊J�n�_�ƈ�O�̏I���_���������ǂ���
						if( dx1 != dx2 || dy1 != dy2 )*/
						if( ! AheIsEqual( ipPrevEndPoint, ipFirst ) )
						{
							// �C��
							if( FixLQSequence( it2->second, mapRoad ) )
							{
								// ������x�ŏ�����`�F�b�N
								goto LQ_CHECK;
							}
							else {
								// �G���[
								CString strMsg;
								strMsg.Format(ERROR_START_POINT_AND_BEFORE_IS_NOT_SAME );
								ATLTRACE(_T("ERROR : %s"), strMsg );
								for_each( it2->second.begin(), it2->second.end(), TraceLQDEF );

								for( std::list<LQDEF>::iterator it3 = it2->second.begin(), it3End = it2->second.end(); it3 != it3End; ++it3 )
									strMsg += GetTraceLQDEFString( *it3 ) + _T("\n");
								MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
								return FALSE;
							}
						}
					}
					ipPrevEndPoint = ipEnd;
				}
				else {
					// �A���S���Y���G���[
					CString strMsg;
					strMsg.Format(ERROR_NO_SHAPE_OF_ROAD_LINK, mapRoad[itLQ->linkid].oid );
					ATLTRACE(_T("ALGORITHM ERROR : %s\n"), strMsg );
					MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
			}
			// �V�[�P���X�̍Ō�ɏI�[�t���O�����邩�ǂ����̃`�F�b�N
			if( ! it2->second.rbegin()->last )
			{
				// �C��
				if( FixLQSequence( it2->second, mapRoad ) )
				{
					// ������x�ŏ�����`�F�b�N
					goto LQ_CHECK;
				}
				else {
					// �G���[
					CString strMsg;
					strMsg.Format(ERROR_TERMINATION_FLAG_NOT_EXIST );
					ATLTRACE(_T("ERROR : %s"), strMsg );
					for_each( it2->second.begin(), it2->second.end(), TraceLQDEF );

					for( std::list<LQDEF>::iterator it3 = it2->second.begin(), it3End = it2->second.end(); it3 != it3End; ++it3 )
						strMsg += GetTraceLQDEFString( *it3 ) + _T("\n");
					MessageBox( GetHWND(), strMsg, ERROR_ROAD_EDIT_FAIL, MB_OK|MB_ICONSTOP );
					return FALSE;
				}
			}
		}
	}
	// ���H�����N�ɕs�v��LQRF���t���Ă��Ȃ����ǂ����`�F�b�N
	for( CTargetItems::iterator itRoad = cLastRoads.begin(), itRoadEnd = cLastRoads.end(); itRoad != itRoadEnd; ++itRoad )
	{
		const ROADDEF& def = mapRoad[itRoad->GetOID()];

		for ( int i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ ) 
		{
			long lDrawMode = m_cLoadQueueInfo.GetLoadQueueDrawMode( (sindyeRoadQueueType)i );
			if( def.rf & lDrawMode )
			{
				bool bFindLQ = false;
				for( std::map<long, std::list<LQDEF> >::iterator itLQs = mapLQ[i].begin(), itLQsEnd = mapLQ[i].end(); itLQs != itLQsEnd; ++itLQs )
				{
					for( std::list<LQDEF>::iterator itLQ = itLQs->second.begin(), itLQEnd = itLQs->second.end(); itLQ != itLQEnd; ++itLQ )
					{
						if( def.oid == itLQ->linkid )
						{
							bFindLQ = true;
							break;
						}
					}
					if( bFindLQ ) break;
				}
				if( ! bFindLQ )
				{
					// �x��
					// [bug 3521] �����N�񏈗��Ɏ��������͖̂ʓ|�Ȃ̂ŁA�����ő������폜����
					ATLTRACE(_T("WARNING : �֘A����LQ�����݂��Ȃ��̂ɓ��H�����N�i%ld�j�Ɋ֘A�t���O�i%d�j�����Ă��܂��̂Ńt���O���폜���܂�\n"), def.oid, i );
					itRoad->SetAttribute( m_cLoadQueueInfo.GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoLQRFRoadFieldName ), 0L );
				}
			}
		}
	}
#ifdef _DEBUG
	DWORD dwEND = GetTickCount();
	ATLTRACE(_T("INFO : �`�F�b�N���� : %.3lf\n"), ((double)(dwEND-dwSTART2))/1000.0 );
	ATLTRACE(_T("INFO : �����N��̃`�F�b�N�����F����[%.3lf�b]\n"), ((double)( dwEND - dwSTART ))/1000.0 );
	ATLTRACE(_T("###########################################################\n"));
#endif // ifdef _DEBUG
	return TRUE;
}
