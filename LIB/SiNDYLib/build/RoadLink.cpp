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

// RoadLink.cpp: CRoadLink �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoadLink.h"
#include "meshutil.h"
#include "ErrorObject.h"
#include "RuleModel.h"
#include "AheGeometryOp.h"
#include "GlobalFunctions.h"
#include "EnumSegment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR START_VECTOR_AND_NEXT             = _T("�J�n�x�N�g���i%.1lfcm�j�Ƃ��̎��̃x�N�g��");
	LPCTSTR END_VECTOR_AND_PRE                = _T("�I���x�N�g���i%.1lfcm�j�Ƃ��̑O�̃x�N�g��");
	LPCTSTR DISTANCE_LESS_THAN_50_MSG         = _T("%s�̊p�x��%.1lf���ł����A���K�����W������50�����̏ꍇ��170���ȏ�Ȃ���΂����܂���B");
	LPCTSTR DISTANCE_BETWEEN_50_AND_100_MSG   = _T("%s�̊p�x��%.1lf���ł����A���K�����W������50�ȏ�100�����̏ꍇ��150���ȏ�Ȃ���΂����܂���B");
	LPCTSTR DISTANCE_BETWEEN_100_AND_250_MSG  = _T("%s�̊p�x��%.1lf���ł����A���K�����W������100�ȏ�250�����̏ꍇ��120���ȏ�Ȃ���΂����܂���B");
	LPCTSTR DISTANCE_BETWEEN_250_AND_500_MSG  = _T("%s�̊p�x��%.1lf���ł����A���K�����W������250�ȏ�500�����̏ꍇ��90���ȏ�Ȃ���΂����܂���B");
	LPCTSTR DISTANCE_BETWEEN_500_AND_1000_MSG = _T("%s�̊p�x��%.1lf���ł����A���K�����W������500�ȏ�1000�����̏ꍇ��40���ȏ�Ȃ���΂����܂���B");
	LPCTSTR DISTANCE_OVER_1000_MSG            = _T("%s�̊p�x��%.1lf���ł����A���K�����W������1000�ȏ�̏ꍇ��20���ȏ�Ȃ���΂����܂���B");
#else
	LPCTSTR START_VECTOR_AND_NEXT             = _T("Start vector (%.1lfcm) and next vector");
	LPCTSTR END_VECTOR_AND_PRE                = _T("End vector (%.1lfcm) and previous vector");
	LPCTSTR DISTANCE_LESS_THAN_50_MSG         = _T("The angel of %s is %.1lf degree, but if it is less than 50 in the normalized coordinates distance, it must be over 170 degree");
	LPCTSTR DISTANCE_BETWEEN_50_AND_100_MSG   = _T("The angel of %s is %.1lf degree, but if it is between 50 and 100 in the normalized coordinates distance, it must be over 150 degree");
	LPCTSTR DISTANCE_BETWEEN_100_AND_250_MSG  = _T("The angel of %s is %.1lf degree, but if it is between 100 and 250 in the normalized coordinates distance, it must be over 120 degree");
	LPCTSTR DISTANCE_BETWEEN_250_AND_500_MSG  = _T("The angel of %s is %.1lf degree, but if it is between 250 and 500 in the normalized coordinates distance, it must be over 90 degree");
	LPCTSTR DISTANCE_BETWEEN_500_AND_1000_MSG = _T("The angel of %s is %.1lf degree, but if it is between 500 and 1000 in the normalized coordinates distance, it must be over 150 degree");
	LPCTSTR DISTANCE_OVER_1000_MSG            = _T("The angel of %s is %.1lf degree, but if it is over 1000 in the normalized coordinates distance, it must be over 20 degree");
#endif // SINDY_FOR_ENGLISH
}

namespace sindy {
namespace road_link {
using namespace errorcode;
using namespace sindy::schema;
/*
 * @brief ���[�ɓ��H�m�[�h�����݂��Ă��邩
 * @retval �����@sindyErr_NoErr
 * @retval ���s�@����ȊO�̃G���[
 */
sindyErrCode CRoadLink::CheckNodeID() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
/*	int nFromID = GetFrom_NodeID();

	if ( -1 == nFromID ) {
		/// ���������ԍ��������ĂȂ�
		return errorcode::sindyErr_NoFromNodeId;
	}

	/// From Node ������		
	if ( FALSE  ) {
		/// �m�[�h�����������猩����Ȃ�
		return errorcode::sindyErr_NoFromNodeId;
	}

	if ( FALSE ) {
		/// �m�[�h�Ɠ��H���ڂ��Ă��Ȃ�
		return errorcode::sindyErr_NoFromNodeId;
	}

	int nToID = GetTo_NodeID();

	if ( -1 == nToID ) {
		/// ���������ԍ��������ĂȂ�
		return errorcode::sindyErr_NoToNodeId;
	}

	/// To Node ������		
	if ( FALSE  ) {
		/// �m�[�h�����������猩����Ȃ�
		return errorcode::sindyErr_NoToNodeId;
	}

	if ( FALSE ) {
		/// �m�[�h�Ɠ��H���ڂ��Ă��Ȃ�
		return errorcode::sindyErr_NoToNodeId;
	}
	
	return errorcode::sindyErr_NoErr;*/
	emErr = sindyErr_NoImplimentation;
	return emErr;
}

// FROM���̒[�_���擾����
CPoint CRoadLink::GetFromPoint() const
{
	return GetShape()->GetPoint(0);
}

/**
 * @brief <b>TO���̒[�_���擾����</b>\n
 * return CPoint
 */
CPoint CRoadLink::GetToPoint() const
{
	return GetShape()->GetPoint(GetShape()->GetPointCount()-1);
}

// x, y�����b�V��������̏ꍇ�ɂ�����̃��b�V���R�[�h��Ԃ�
unsigned int ANOTHERMESH( unsigned int meshcode, int ix, int iy )
{
	if( 0 == ix )
		meshcode = DECMESH_X(meshcode);
	else if( 100000 == ix )
		meshcode = INCMESH_X(meshcode);
	if( 0 == iy )
		meshcode = DECMESH_Y(meshcode);
	else if( 1000000 == iy )
		meshcode = INCMESH_Y(meshcode);

	return meshcode;
}

// ���������`�F�b�N����
errorcode::sindyErrCode CRoadLink::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const
{
	// ��{�`�F�b�N
	CFeature::CheckLogic( cRule, cErrs );

	// �`��`�F�b�N
	CErrorInfos cErrInfos;
	const CGeometry* pGeom = GetShape();

	// [ADAM�o�O 41]2�����b�V�����E�t�߂̍\���_���������Ƃ���ƃG���[���b�Z�[�W���o�ĕҏW�ł��Ȃ�
	// ADAM�����N�͓񎟃��b�V���`�F�b�N���Ȃ�
	if((0!=_tcsicmp(GetTableName(), adam_v2::lane_link::kTableName)) &&
	   (0!=_tcsicmp(GetTableName(), adam_v2::updown_link::kTableName)) &&
	   (0!=_tcsicmp(GetTableName(), adam_v2::compart_link::kTableName)) && 
	   (0!=_tcsicmp(GetTableName(), adam_v2::border_link::kTableName)))
	{
		long lCount = 0;
		const long lPointCount = pGeom->GetPointCount();
		unsigned int nMeshCode = 0;
		unsigned int nMeshCodeEdge1 = 0, nMeshCodeEdge2 = 0; // 0�Ԗڂ��񎟃��b�V��������̏ꍇ�͈ꎞ�I�ɂ������Ɋi�[����
		for( CGeometry::const_point_iterator it = pGeom->point_begin(); it != pGeom->point_end(); ++it, ++lCount )
		{
			// �\���_�����݂��郁�b�V���R�[�h���擾
			int ix, iy;
			unsigned int nMeshCodeTmp = LL2MESH( it.get_x(), it.get_y(), 2, ix, iy );

			// 0�Ԗڂ̏ꍇ
			if( lCount == 0 )
			{
				// 0�Ԗڂ��񎟃��b�V��������̏ꍇ�͈ꎞ�I�ɕʂ̂��̂ɓ���A1�Ԗڂ̎���
				// ��r���Ă���nMeshCode�Ɋi�[����
				if( (0 == ix || 1000000 == ix) || (0 == iy || 1000000 == iy) )
				{
					// [CHECK] 0�Ԗڂ����b�V��4���ɑ��݂��Ȃ����ǂ���
					if( (0 == ix || 1000000 == ix) && (0 == iy || 1000000 == iy) )
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadEndPointExist4MeshEdge, sindyErrLevel_ERR ) ); // 0�Ԗڂ����b�V��4���ɑ��݂���

					nMeshCodeEdge1 = nMeshCodeTmp;
					nMeshCodeEdge2 = ANOTHERMESH( nMeshCodeEdge1, ix, iy );
				}
				else
					nMeshCode = nMeshCodeTmp;
			}
			// �ŏI�_�̏ꍇ
			else if( lCount == lPointCount - 1 )
			{
				// �ŏI�_���񎟃��b�V��������̏ꍇ
				if( (0 == ix || 1000000 == ix) || (0 == iy || 1000000 == iy) )
				{
					// [CHECK] �ŏI�_�����b�V��4���ɑ��݂��Ȃ����ǂ���
					if( (0 == ix || 1000000 == ix) && (0 == iy || 1000000 == ix) )
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadEndPointExist4MeshEdge, sindyErrLevel_ERR ) ); // �ŏI�_�����b�V��4���ɑ��݂���

					// [CHECK] �Z�O�����g���񎟃��b�V���������ׂ��ł��Ȃ����i�ŏI�_���񎟃��b�V��������ɂ���ꍇ�j
					if( nMeshCode != nMeshCodeTmp && nMeshCode != ANOTHERMESH( nMeshCodeTmp, ix, iy ) )
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // �ŏI�_�����b�V���ɐڂ��Ă���A���ꂪ�ʂ̃��b�V���̒��ɂ���ꍇ
				}
				// [CHECK] �Z�O�����g���񎟃��b�V���������ׂ��ł��Ȃ����i�ŏI�_���񎟃��b�V��������ɂȂ��ꍇ�j
				else if( nMeshCode != nMeshCodeTmp )
				{
					if( 0 == nMeshCode )
					{
						if( nMeshCodeEdge1 == nMeshCodeTmp || nMeshCodeEdge2 == nMeshCodeTmp )
							nMeshCode = nMeshCodeTmp;
						else
							cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // 0�Ԗڂ����b�V���ɐڂ��Ă���A2�Ԗڂ�����ɕʂ̃��b�V���̒��ɂ���ꍇ
					}
					else
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // �ŏI�_�����b�V���ɐڂ��Ă��炸�A�ʂ̃��b�V���̒��ɂ���ꍇ
				}
			}
			// ���ԓ_�̏ꍇ
			else {
				// [CHECK] �[�_�ȊO���񎟃��b�V�������Ɛڂ��Ă��Ȃ���
				if( (0 == ix || 1000000 == ix) || (0 == iy || 1000000 == iy)  )
					cErrInfos.push_back( CErrorInfo( sindyErr_RoadMiddlePointExistMeshEdge, sindyErrLevel_ERR ) );	// ���ԓ_�����b�V���ɐڂ��Ă���ꍇ
				else {
					// [CHECK] �Z�O�����g���񎟃��b�V���������ׂ��ł��Ȃ���
					if( 0 == nMeshCode )
					{
						if( nMeshCodeEdge1 == nMeshCodeTmp || nMeshCodeEdge2 == nMeshCodeTmp )
							nMeshCode = nMeshCodeTmp;
						else
							cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // 0�Ԗڂ����b�V���ɐڂ��Ă���A2�Ԗڂ�����ɕʂ̃��b�V���̒��ɂ���ꍇ
					}
					else if( nMeshCode != nMeshCodeTmp )
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // ���ԓ_���ʂ̃��b�V���ɂ���ꍇ
				}
			}
		}
	}
	// �Z�O�����g�`�F�b�N�ibug 8463�j
	// ���������N�ł̓`�F�b�N���Ȃ�(SJ���s��Phase2�Ή�)
	// ADAM�����N(�Ԑ������N�A�㉺�������N)�͓񎟃��b�V���`�F�b�N���Ȃ�
	// TODO RoadLink�̃`�F�b�N��ŕ��������N�̃`�F�b�N������Ƃ����������A�����炭�A�݌v�v�z�ɍ����ĂȂ�
	// ���H�����N�Ƃ̋��ʕ����𒊏o���ĕ�������ׂ��ł͂Ȃ���
	if(0!=_tcsicmp(GetTableName(), walk_link::kTableName) &&
	  (0!=_tcsicmp(GetTableName(), adam_v2::lane_link::kTableName)) &&
	  (0!=_tcsicmp(GetTableName(), adam_v2::updown_link::kTableName)) &&
	  (0!=_tcsicmp(GetTableName(), adam_v2::compart_link::kTableName)) &&
	  (0!=_tcsicmp(GetTableName(), adam_v2::border_link::kTableName)))
	{
		long lSegmentCount = pGeom->GetSegmentCount(), cnt = 0;
		ISegmentCollectionPtr ipSegCol( (IGeometry*)*pGeom );
		for( CGeometry::const_segment_iterator it = pGeom->segment_begin(); it != pGeom->segment_end(); ++it, ++cnt )
		{
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
			if( lSegmentCount > 1 && ( cnt == 0 || cnt == lSegmentCount - 1 ) )
			{
				IPointPtr ipTmpFromPoint(it.from()), ipTmpToPoint(it.to());
				double dLenMeter = AheGetMeterLength( ipTmpFromPoint, ipTmpToPoint );
				CString strTmp;
				IPointPtr ip1, ip2, ip3;
				if( cnt == 0 )
				{
					_ISegmentPtr ipSegNext;
					ipSegCol->get_Segment( 1, &ipSegNext );
					ip1 = ipTmpFromPoint;
					ip2 = ipTmpToPoint;
					ipSegNext->get_ToPoint( &ip3 );
					strTmp.Format(START_VECTOR_AND_NEXT, (DOUBLE)dLenMeter * 100.0 );
				}
				else {
					_ISegmentPtr ipSegPrev;
					ipSegCol->get_Segment( lSegmentCount - 2, &ipSegPrev );
					ipSegPrev->get_FromPoint( &ip1 );
					ip2 = ipTmpFromPoint;
					ip3 = ipTmpToPoint;
					strTmp.Format(END_VECTOR_AND_PRE, (DOUBLE)dLenMeter * 100.0 );
				}
				DOUBLE dAngle = AheGetAngle( ip1, ip2, ip3 );
				double dMeshXYLen = AheGetMeshXYLength( ipTmpFromPoint, ipTmpToPoint ); // MESH XY�����Ŕ�r�ibug 8463�j
				CString strMsg;
				if( dMeshXYLen < 50.0                                && ! ( -170.0 >= dAngle || 170.0 <= dAngle ) ) strMsg.Format(DISTANCE_LESS_THAN_50_MSG, strTmp, fabs(dAngle) );
				else if(   50.0 <= dMeshXYLen && dMeshXYLen <  100.0 && ! ( -150.0 >= dAngle || 150.0 <= dAngle ) ) strMsg.Format(DISTANCE_BETWEEN_50_AND_100_MSG, strTmp, fabs(dAngle) );
				else if(  100.0 <= dMeshXYLen && dMeshXYLen <  250.0 && ! ( -120.0 >= dAngle || 120.0 <= dAngle ) ) strMsg.Format(DISTANCE_BETWEEN_100_AND_250_MSG, strTmp, fabs(dAngle) );
				else if(  250.0 <= dMeshXYLen && dMeshXYLen <  500.0 && ! (  -90.0 >= dAngle ||  90.0 <= dAngle ) ) strMsg.Format(DISTANCE_BETWEEN_250_AND_500_MSG, strTmp, fabs(dAngle) );
				else if(  500.0 <= dMeshXYLen && dMeshXYLen < 1000.0 && ! (  -40.0 >= dAngle ||  40.0 <= dAngle ) ) strMsg.Format(DISTANCE_BETWEEN_500_AND_1000_MSG, strTmp, fabs(dAngle) );
				else if( 1000.0 <= dMeshXYLen                        && ! (  -20.0 >= dAngle ||  20.0 <= dAngle ) ) strMsg.Format(DISTANCE_OVER_1000_MSG, strTmp, fabs(dAngle) );
				if( ! strMsg.IsEmpty() )
					cErrInfos.push_back( CErrorInfo( sindyErr_CheckReturnWarning, sindyErrLevel_WARN, (IGeometryPtr)AheCreateClone(ip2), -1, -1, _T("%s"), strMsg ) );
			}
		}
	}
	// ����֌W�t�B�[�`���̒P�̃`�F�b�N
	// �l�b�g���[�N�����N�Ɠ���֌W�t�B�[�`���̕����`�F�b�N
	for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()), 
		itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
		itContain != itContainEnd; ++itContain )
	{
		for( const_rows_iterator itContainFeat = begin(AheTableName2TableType(itContain->first)), 
			itContainFeatEnd = end(AheTableName2TableType(itContain->first));
			itContainFeat != itContainFeatEnd; ++itContainFeat )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			(*itContainFeat)->Changed( &emUpdateType );

			// �폜����Ă��Ȃ����̂̂݃`�F�b�N
			if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
				// LINK_ID�����������ǂ����̃`�F�b�N
				if ( GetOID() != (*itContainFeat)->GetLongValueByFieldName(itContain->second, -1 ) )
					cErrs.push_back( CErrorObject( *itContainFeat->get(), CErrorInfo( sindyErr_RelationIDInvalid, sindyErrLevel_ERR ) ) );

				GetShape()->Trace( _T("���H") );
				(*itContainFeat)->Trace( _T("�����N������") );

				// ���H�ɂ����Ɠ����Ă��邩�`�F�b�N
				if ( ! GetShape()->Equals( (*itContainFeat)->GetShapeCopy() ) && ! GetShape()->Contains( (*itContainFeat)->GetShapeCopy() ) )
				{
					this->Trace( _T("���H"));
					(*itContainFeat)->Trace(_T("�����N������"));
					cErrs.push_back( CErrorObject( *itContainFeat->get(), CErrorInfo( sindyErr_RuleRelationshipContainGeometryNotWithinRoad, sindyErrLevel_ERR ) ) );
				}
			}
		}
	}

	// �O���e�[�u��
	for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(GetTableName()), 
		itOuterEnd = g_cRuleRelationTable.end(GetTableName());
		itOuter != itOuterEnd; ++itOuter )
	{
		for( const_rows_iterator itOuterObj = begin(AheTableName2TableType(itOuter->get<1>())),
			itOuterObjEnd = end(AheTableName2TableType(itOuter->get<1>()));
			itOuterObj != itOuterObjEnd; ++itOuterObj )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			(*itOuterObj)->Changed( &emUpdateType );

			// �폜����Ă��Ȃ����̂̂݃`�F�b�N
			if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
				// LINK_ID�����������ǂ����̃`�F�b�N
				if ( GetOID() != (*itOuterObj)->GetLongValueByFieldName(itOuter->get<2>(), -1 ) )
					cErrs.push_back( CErrorObject( *itOuterObj->get(), CErrorInfo( sindyErr_RelationIDInvalid, sindyErrLevel_ERR ) ) );
			}
		}
	}

	// �����`�F�b�N

	if( sindyErr_NoErr != cErrInfos.GetDefaultErrCode() )
	{
		CErrorObject cErr( *this );
		cErr.push_back( cErrInfos );
		cErrs.push_back( cErr );
	}
	return cErrs.GetDefaultErrCode();
}

bool CRoadLink::IsPECLink() const
{
	schema::road_link::road_class::ECode sRoadClass = GetRoadClassCode();

	if ( sRoadClass != -1 && sRoadClass == schema::road_link::road_class::kParking/*���ԏ�\�����H�����N�iPEC�j*/ || sRoadClass == schema::road_link::road_class::kMatching/* �}�b�`���O�p�����N�iPEC�j*/ )
		return true;

	return false;
}

} // road_link

} // sindy
