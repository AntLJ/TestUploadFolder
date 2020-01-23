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
 * @file Features.cpp
 * @brief <b>CFeatures�N���X�����t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "Features.h"

#include "util.h"
#include "Map.h"
#include "TableTypeUtil.h"
#include "RoadNetwork.h"
#include "Annotations.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "FeatureContainer.h"
#include "ListString.h"
#include "Workspace.h"
#include "ModelContainContainer.h"
#include "AheNetwork.h"
#include "RuleModel.h"
#include "AnnoGlobals.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// �������邽�߂̃R���e�i���Z�b�g����
sindyErrCode CFeatures::SetFeatureContainer( IWorkspace* ipWorkspace, LPCTSTR lpcszName, std::list<CFeatureContainer*>& listContainer )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipWorkspace && lpcszName, sindyErr_ArgIsNull )
	{
		CWorkspace cWork( ipWorkspace );

		// �I�[�i�[���ƃe�[�u�����ɕ���
		CString strTableName( lpcszName );
		CString strOwnerName;
		if( _tcschr( lpcszName, _T('.') ) )
		{
			int i=0;
			strOwnerName = strTableName.Tokenize( _T("."), i );
			strTableName = strTableName.Tokenize( _T("."), i );
		}

		// �e�[�u���^�C�v���擾
		sindyTableType::ECode emTableType = AheTableName2TableType( strTableName );

		// �e�[�u���^�C�v�ŃR���e�i���Z�b�g
		CFeatureContainer* pContainer = NULL;
		CString strBaseName;

		// �l�b�g���[�N�n
		if( AheIsRelateWithNetwork( strTableName ) )
		{
			strBaseName = AheGetNetworkLinkFeatureClassName( strTableName ); // �l�b�g���[�N�����N�e�[�u����
			pContainer = new CRoadNetwork();
		}
		// ���L�n
		else if( AheIsAnnoPointTableType( emTableType ) ||
			AheIsAnnoLineTableType( emTableType ) )
		{
			strBaseName = AheTableType2TableName( AheTableType2AnnoPointTableType( emTableType ) );
			pContainer = new CAnnotations();
		}
		// ����֘A�n
		else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
		{
			if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
				strBaseName = g_cRuleRelationshipContain.find( strTableName )->first; // �u�܂ށv���̃e�[�u����
			else
				strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

			pContainer = new CModelContainContainer();
		}
		// ���̑�
		else {
			strBaseName = strTableName;
			pContainer = new CFeatureContainer();
		}

		// ���ɓǂݍ��܂�Ă��邩�ǂ����`�F�b�N
		bool bFind = false;
		for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		{
			if( typeid(*it) == typeid(pContainer) )
			{
				// �I�[�i�[���t���œn����Ă���Ƃ��̓I�[�i�[������r�Ώۂɂ���
				if( 0 == strBaseName.CompareNoCase( (*it)->GetTableName() ) &&
					( strOwnerName.IsEmpty() || 0 == strOwnerName.CompareNoCase( (*it)->GetOwnerName() ) ) )
				{
					bFind = true;
					listContainer.push_back( *it ); // ���������͕̂ԋp�p�R���e�i�ɓ���Ă���
					delete pContainer; // TODO: shared_ptr�Ǘ��ɂ���΂���Ȃ��Ȃ�
					break;
				}
			}
		}

		// �ǂݍ��܂�Ă��Ȃ��Ȃ�V�K�ɓǂݍ��݁E�o�^
		if( ! bFind )
		{
			// �I�[�i�[��.�l�b�g���[�N�����N�e�[�u�����ɂ���
			CString strName( lpcszName );
			int nPeriodIndex = strName.Find( _T('.'), 0 );
			CString strLinkName = ( 0 < nPeriodIndex ) ? strName.Left( nPeriodIndex ) + _T(".") + strBaseName : strBaseName;

			// �I�[�v���ł�����o�^�ibug 9779�j
			ITablePtr ipTable = cWork.OpenTable( strLinkName );
			if( ipTable )
			{
				pContainer->SetObject( (IFeatureClassPtr)ipTable );
				m_listFeatureContainer.push_back( pContainer );
				listContainer.push_back( pContainer ); // �V�K�̂��̂��ԋp�p�R���e�i�ɓ���Ă���
			}
		}
	}

	return emErr;
}

// �������邽�߂̃R���e�i���Z�b�g����
sindyErrCode CFeatures::SetFeatureContainer( IWorkspace* ipWorkspace, const CListString& listNames, std::list<CFeatureContainer*>& listContainer )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	for( CListString::const_iterator it = listNames.begin(), itEnd = listNames.end(); it != itEnd; ++it )
	{
		sindyErrCode emErrTmp = SetFeatureContainer( ipWorkspace, *it, listContainer );
		if( sindyErr_NoErr == emErr )
			emErr = emErrTmp;
	}
	// �d�����Ă���\��������̂Ń��j�[�N�ɂ���
	listContainer.sort();
	listContainer.unique();

	return emErr;
}
	
// �t�B�[�`����I������
sindyErrCode CFeatures::SelectByShape( IWorkspace* ipWorkspace, IGeometry* ipGeom, LPCTSTR lpcszName, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit/* = true*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( lpcszName, sindyErr_ArgIsNull )
	{
		CListString listNames;
		listNames.push_back( lpcszName );

		emErr = SelectByShape( ipWorkspace, ipGeom, listNames, bRecycling, bCreateCache, pErrs, bInit );
	}

	return emErr;
}

// �t�B�[�`����I������
sindyErrCode CFeatures::SelectByShape( IWorkspace* ipWorkspace, IGeometry* ipGeom, const CListString& listNames, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/, bool bInit/* = true*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	// ���L�ҏW�Ώۂ�ǉ�����
	//// TODO: ���[�N�X�y�[�X��SDE�Ȃ�I�[�i�[����K�{�Ƃ���
	//// ��������Ȃ��Ƃǂ̃I�[�i�[�̃e�[�u�����J���ėǂ��̂��킩��Ȃ�
	CListString listNamesTmp;
	for( CListString::const_iterator it = listNames.begin(); it != listNames.end(); ++it )
	{
		// �I�[�i�[���t���ł��邱�Ƃ��l�����ăe�[�u�����̂ݎ��o��
		CString strTableName, strOwnerName;
		int nPeriodIndex = it->Find( _T('.'), 0 );
		if( 0 < nPeriodIndex )
		{
			strOwnerName = it->Left( nPeriodIndex );
			strTableName = it->Right( it->GetLength() - nPeriodIndex - 1 );
		}
		else
			strTableName = *it;

		// ���L�ҏW�Ώێ擾
		CListString listStr;
		g_cRule.GetSharedMovableLayer( strTableName, listStr );

		// �擾�ł������X�g�ɃI�[�i�[��������listNamesTmp�Ɋi�[
		for( CListString::const_iterator it = listStr.begin(); it != listStr.end(); ++it )
		{
			CString strNameTmp;
			if( 0 < nPeriodIndex )
				strNameTmp.Format(_T("%s.%s"), strOwnerName, *it );
			else
				strNameTmp = *it;

			listNamesTmp.push_back( strNameTmp );
		}
	}

	// ��������e�[�u���ǂݍ��ݖh�~
	listNamesTmp.sort();
	listNamesTmp.unique();

	// �R���e�i�o�^
	std::list<CFeatureContainer*> listContainer; // listNamesTmp�Ƀ}�b�`�����R���e�i���X�g
	LOGASSERTE_IF( sindyErr_NoErr == ( emErr = SetFeatureContainer( ipWorkspace, listNamesTmp, listContainer ) ), emErr )
	{
		// ����
		for( std::list<CFeatureContainer*>::iterator it = listContainer.begin(); it != listContainer.end(); ++it )
		{
			sindyErrCode emErrTmp = sindyErr_NoErr;
			LOGASSERTE_IF( sindyErr_NoErr == ( emErrTmp = (*it)->Select( ipGeom, bRecycling, bCreateCache, cErrs, bInit ) ), emErrTmp )
			{
			}
			if( sindyErr_NoErr == emErr ) 
				emErr = emErrTmp;	// �G���[���N�����ꍇ�͍ŏ��̃G���[�R�[�h���Ԃ�
		}
	}

// ����x�������c TODO: �x������錴���𒼂��A�K�����ɖ߂����Ɓi�VDeleteTool?�j
//#ifdef _DEBUG
//	// �L���b�V�����쐬���ꂽ���ǂ����`�F�b�N
//	if( bCreateCache )
//	{
//		if( ! empty() )
//		{
//			LOGASSERTEERR_IF( IsCached(), sindyErr_FeatureCacheNotFound );
//		}
//	}
//#endif // ifdef _DEBUG

	return emErr;
}

// �t�B�[�`����I������
sindyErrCode CFeatures::SelectByObject( IWorkspace* ipWorkspace, _IRow* ipRow, bool bRecycling/* = false*/, bool bCreateCache/* = false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipWorkspace && ipRow, sindyErr_ArgIsNull )
	{
/*		// ���L�ړ��Ώۂ��擾
		CListString listNames;*/
		CRow cRow( ipRow );
/*		g_cRule.GetSharedMovableLayer( cRow.GetTableName(), listNames );*/

		// �ύX��̌`��Ō���
		IGeometryPtr ipGeom;
		IFeaturePtr ipFeature( ipRow );
		
		if( NULL != ipFeature )
			ipFeature->get_Shape( &ipGeom );

		LOGASSERTEERR_IF( NULL != ipGeom, sindyErr_GeometryNotFound )
		{
			LOGASSERTE_IF( sindyErr_NoErr == ( emErr = SelectByShape( ipWorkspace, ipGeom, cRow.GetNameString()->GetOwnerTableName(), bRecycling, bCreateCache ) ), emErr )
			{
				// �ύX�O�̌`��Ō���
				IFeatureChangesPtr ipChanges( ipRow );
				if( NULL != ipChanges )
				{
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					ipChanges->get_ShapeChanged( &vbChanged );
					if( vbChanged )
					{
						IGeometryPtr ipOriginalGeom;
						ipChanges->get_OriginalShape( &ipOriginalGeom );
						if( NULL != ipOriginalGeom )
						{
							LOGASSERTE_IF( sindyErr_NoErr == ( emErr = SelectByShape( ipWorkspace, ipOriginalGeom, cRow.GetNameString()->GetOwnerTableName(), bRecycling, bCreateCache ) ), emErr );
						}
					}
				}
			}
		}
	}

	return emErr;
}

//�^����ꂽ�`��Ɋ֘A������̂��R���e�i�Ɋi�[����
void CFeatures::QueryFeatures( IGeometry* ipGeom, CContainer& cContainer ) const
{
	CGeometry cGeom( ipGeom );
	for( const_iterator it = begin(); it != end(); ++it ) 
		(*it)->Query( sindyCompOpeNotEqual, cGeom, sindyeSpatialRelDisjoint, cContainer );
}

// �o�^����Ă���t�B�[�`��������΁A���̃t�B�[�`���̃|�C���^��Ԃ��܂�
CRow* CFeatures::FindObject( _IRow* ipRow )
{
	if( ipRow == NULL )
		return NULL;

	CRow* pRet = NULL;	// �Ԃ�l

	for( iterator it = begin(); it != end(); it++ )
	{
		pRet = (*it)->find( ipRow ).get();
		if( pRet ) break;
	}

	return pRet;
}

bool CFeatures::IsCached() const
{
	bool bIsCached = false;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		bool bIsCachedTmp = (*it)->IsCached();
		if( ! bIsCached ) bIsCached = bIsCachedTmp;
	}

	return bIsCached;
}

// �^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��
sindyErrCode CFeatures::Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

#if 0
	// �����`�󂪃|�C���g�ŁA�ړ����[�h��sindyeMove_Point�̏ꍇ�́A
	// ��ԋ߂��\���_���ړ�����B
	// ���̂��߂ɁA�܂��|�C���g�����ԋ߂��\���_��T��
	if( cGeom.GetGeometryType() == esriGeometryPoint && sindyeMove_Point == emMode )
	{
		IPointPtr ipNearestVertex;
		double dNearestDistance = -1;
		for( iterator it = begin(); it != end(); ++it )
		{
			IPointPtr ipVertex = (*it)->ReturnNearestVertex( (IPointPtr)(IGeometry*)cGeom ); <- ���ꂶ����ۂ̌����`��i�����~�ɂȂ�j�����O�̂��̂�����\��������̂ŁA
			double dDistance = cGeom.ReturnDistance( ipVertex );                                ����ς�_�ł̈ړ��̏ꍇ��臒l��o�b�t�@�����O��̌`�󂪂Ȃ��Ƙb�ɂȂ�Ȃ�
			if( 0 > dNearestDistance || dNearestDistance > dDistance )                          ��Ԍ����I�Ȃ̂́AsindyeMove_Point�̎��͌����`��̒��S����߂����̂��ړ�����
			{                                                                                   �悤�ɂ��邱�Ƃ��H�H
				ipNearestVertex = ipVertex;                                                     �������A���̏ꍇ�������̎��_�œ_���m�肷��K�v������B
				dNearestDistance = dDistance;                                                   ����Ƃ��A�������̂��Ɠ_���ꏏ�ɓn�����H�H�H
			}
		}
		if( ipNearestVertex != NULL )
			cGeom = ipNearestVertex;
	}
#endif // if 0

	for( iterator it = begin(); it != end(); ++it )
	{
		if( sindyErr_NoErr != ( emErr = (*it)->Move( cGeom, pairMove, emMode, cErrObjects ) ) )
			break;	// �G���[
	}

	return emErr;
}

// �_->�_�̈ړ��������s��
sindyErrCode CFeatures::MovePoint( IPoint* ipFrom, IPoint* ipTo, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipFrom && ipTo, sindyErr_ArgIsNull )
	{
		WKSPoint pFrom = {-1, -1}, pTo = {-1, -1};
		ipTo->QueryCoords( &pTo.X, &pTo.Y );
		ipFrom->QueryCoords( &pFrom.X, &pFrom.Y );

		emErr = Move( CGeometry( ipFrom ), std::pair<double,double>( pTo.X - pFrom.X, pTo.Y - pFrom.Y ), sindyeMove_Point, cErrObjects );
	}

	return emErr;
}

// �^����ꂽ�`��Ƃ̌��������ŕ����������s��
errorcode::sindyErrCode CFeatures::Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	// TODO: �����̃R�[�h��CModelContainContainer::_Aplit�ACRoadNetWork::_Split�Ƌ��ʉ����ׂ�
	sindyErrCode emErr = sindyErr_NoErr; // �Ԃ�l

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CString strTableName( CString_LastPeriodRight( pSource->GetTableName() ) );

		if( AheIsRelateWithNetwork( strTableName ) )
		{
			for ( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CRoadNetwork*>( *it ) ) {	// ���H�l�b�g���[�N�H
					CRoadNetwork* pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );

					if( 0 == lstrcmpi( pRoadNetwork->GetTableName(), AheGetNetworkLinkFeatureClassName( strTableName ) ) )
						pRoadNetwork->Split( pSource, cGeom, cErrObjects );
				}
			}
		}
		// ����֘A�n
		else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
		{
			for ( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CModelContainContainer*>( *it ) ) {	// ��܃��f���H
					CModelContainContainer* pModelContain = static_cast<CModelContainContainer*>( (*it) );

					CString strBaseName;

					if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
						strBaseName = g_cRuleRelationshipContain.find( strTableName )->first;
					else
						strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

					if( 0 == lstrcmpi( pModelContain->GetTableName(), strBaseName ) )
						pModelContain->Split( pSource, cGeom, cErrObjects );
				}
			}
		}
		else
		{
			CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

			if ( pFeature ) {
				if ( pFeature->GetShape() ) {
					esriGeometryType emType = pFeature->GetShape()->GetGeometryType();

					switch ( emType ) {
						case esriGeometryPolyline:
						case esriGeometryPolygon:
							{
								CGeometry cSplittedGeoms, cSplittedPoints;

								// Split
								if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
									if ( (IGeometry*)cSplittedGeoms ) {
										if ( cSplittedGeoms.GetPartCount() ) {
											// �V�����ł����I�u�W�F�N�g���N���X�Q�Ɋi�[���邽�߂ɏ�������R���e�i�擾
											CFeatureContainer* pContainer = NULL;

											for ( iterator it = begin(); it != end(); ++it ) {
												if ( (*it)->find( *pSource.get() ) ) {
													pContainer = (*it);
													break;
												}
											}

											if ( pContainer ) {
												CTable cTable( *pContainer );

												// �����`������蓖�Ă�
												// TODO: CRoadNetWork::Split�ł͎n�_�����`��ID���p���ΏۂƂȂ��Ă���̂ŁA���ʉ�����ۂɂ͂��̕ӂɋC�����邱�ƁI�I
												for ( CGeometry::const_part_iterator itPart = cSplittedGeoms.part_begin(), itPartEnd = cSplittedGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
													CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

													if ( (IGeometry*)cNewGeom ) {
														WKSPoint* point_wks = new WKSPoint[cNewGeom.GetPointCount()];

														((IPointCollectionPtr)(IGeometry*)cNewGeom)->QueryWKSPoints( 0, cNewGeom.GetPointCount(), point_wks );

														if ( itPart == cSplittedGeoms.part_begin() ) {
															// ���݂̒��g�Ɠ���ւ���
															((IPointCollectionPtr)(IGeometry*)*pFeature->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );
														} else {
															// �t�B�[�`���쐬
															CSPRow cNew;

															cNew.reset( new CFeature( NULL, pContainer->GetTableType(), true, pContainer->GetFieldMap(), pContainer->GetNameString() ) );

															cTable.CreateRow( *cNew.get() );

															// �������R�s�[
															pSource->CopyTo( *cNew.get(), true, false, false );

															// �`����Z�b�g
															((IPointCollectionPtr)(IGeometry*)*cNew->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

															// �R���e�i�ɒǉ�
															pContainer->push_back( cNew );
														}
														delete [] point_wks;
														point_wks = NULL;
													}
												}
											}
										}
									}
								}
							}
							break;
						default:
							_ASSERTE(false); break;
					}
				} else {
					cErrObjects.push_back( CErrorObject( (*pSource) ) );
					_ASSERTE(false);
				}
			} else {
				cErrObjects.push_back( CErrorObject( (*pSource) ) );
				_ASSERTE(false);
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CFeatures::Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr; // �Ԃ�l

	LOGASSERTEERR_IF( pSource && pDest, sindyErr_ArgIsNull )
	{
		sindyTableType::ECode emSourceTableType = pSource->GetTableType();
		sindyTableType::ECode emDestTableType = pDest->GetTableType();

		if ( *pSource->GetNameString() == *pDest->GetNameString() && emSourceTableType == emDestTableType ) {	// �����t�B�[�`���N���X�H
			CString strTableName( CString_LastPeriodRight( pSource->GetTableName() ) );

			if( AheIsRelateWithNetwork( strTableName ) )
			{
				for ( const_iterator it = begin(); it != end(); ++it ) {
					if ( dynamic_cast<CRoadNetwork*>( *it ) ) {	// ���H�l�b�g���[�N
						CRoadNetwork* pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );

						if( 0 == lstrcmpi( pRoadNetwork->GetTableName(), AheGetNetworkLinkFeatureClassName( strTableName ) ) )
							pRoadNetwork->Merge( pSource, pDest, cErrObjects );
					}
				}
			}
			// ����֘A�n
			else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
			{
				for ( const_iterator it = begin(); it != end(); ++it ) {
					if ( dynamic_cast<CModelContainContainer*>( *it ) ) {	// ���H�l�b�g���[�N
						CModelContainContainer* pModelContain = static_cast<CModelContainContainer*>( (*it) );

						CString strBaseName;

						if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
							strBaseName = g_cRuleRelationshipContain.find( strTableName )->first;
						else
							strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

						if( 0 == lstrcmpi( pModelContain->GetTableName(), strBaseName ) )
							pModelContain->Merge( pSource, pDest, cErrObjects );
					}
				}
			}
			else
			{
				CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

				if ( pFeature ) {
					if ( pFeature->GetShape() ) {
						esriGeometryType emType = pFeature->GetShape()->GetGeometryType();

						switch ( emType ) {
							case esriGeometryPolyline:
							case esriGeometryPolygon:
								{
									if( sindyErr_NoErr == pFeature->Merge( CGeometry(pDest->GetShapeCopy()), cErrObjects ) )
									{
										// �}�[�W����폜
										pDest->Delete();
									} else
										cErrObjects.push_back( CErrorObject( (*pSource) ) );
								}
								break;
							default:
								_ASSERTE(false); break;
						}
					} else {
						cErrObjects.push_back( CErrorObject( (*pSource) ) );
						_ASSERTE(false);
					}
				} else {
					cErrObjects.push_back( CErrorObject( (*pSource) ) );
					_ASSERTE(false);
				}
			}
		} else {
			cErrObjects.push_back( CErrorObject( (*pSource) ) );
			cErrObjects.push_back( CErrorObject( (*pDest) ) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CFeatures::Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects, bool bEraseOnly/*=false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr; // �Ԃ�l

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CString strTableName( CString_LastPeriodRight( pRow->GetTableName() ) );

		if( AheIsRelateWithNetwork( strTableName ) )
		{
			for( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CRoadNetwork*>( *it ) ) {	// ���H�l�b�g���[�N
					CRoadNetwork* pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );

					if( 0 == lstrcmpi( pRoadNetwork->GetTableName(), AheGetNetworkLinkFeatureClassName( strTableName ) ) )
						pRoadNetwork->Erase( pRow, cGeom, cErrObjects, bEraseOnly );
				}
			}
		}
		// ����֘A�n
		else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
		{
			for( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CModelContainContainer*>( *it ) ) {	// ���H�l�b�g���[�N
					CModelContainContainer* pModelContain = static_cast<CModelContainContainer*>( (*it) );

					CString strBaseName;

					if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
						strBaseName = g_cRuleRelationshipContain.find( strTableName )->first;
					else
						strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

					if( 0 == lstrcmpi( pModelContain->GetTableName(), strBaseName ) )
						pModelContain->Erase( pRow, cGeom, cErrObjects );
				}
			}
		}
		else
		{
			CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

			if ( pFeature ) {
				if ( pFeature->GetShape() ) {
					esriGeometryType emType = pFeature->GetShape()->GetGeometryType();

					switch ( emType ) {
						case esriGeometryPoint:
						case esriGeometryPolygon:
						case esriGeometryPolyline:
							pFeature->Erase( cGeom, cErrObjects );
							break;
						default:
							_ASSERTE(false);
							break;
					}
				} else {
					cErrObjects.push_back( CErrorObject( (*pRow) ) );
					_ASSERTE(false);
				}
			} else {
				cErrObjects.push_back( CErrorObject( (*pRow) ) );
				_ASSERTE(false);
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CFeatures::Delete( CSPRow pRow, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr; // �Ԃ�l

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CString strTableName( CString_LastPeriodRight( pRow->GetTableName() ) );

		if( AheIsRelateWithNetwork( strTableName ) )
		{
			for( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CRoadNetwork*>( *it ) ) {	// ���H�l�b�g���[�N
					CRoadNetwork* pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );

					if( 0 == lstrcmpi( pRoadNetwork->GetTableName(), AheGetNetworkLinkFeatureClassName( strTableName ) ) )
						pRoadNetwork->Delete( pRow, cErrObjects );
				}
			}
		}
		// ����֘A�n
		else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
		{
			for( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CModelContainContainer*>( *it ) ) {	// ���H�l�b�g���[�N
					CModelContainContainer* pModelContain = static_cast<CModelContainContainer*>( (*it) );

					CString strBaseName;

					if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
						strBaseName = g_cRuleRelationshipContain.find( strTableName )->first;
					else
						strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

					if( 0 == lstrcmpi( pModelContain->GetTableName(), strBaseName ) )
						pModelContain->Delete( pRow, cErrObjects );
				}
			}
		}
		else
		{
			CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

			if ( pFeature ) {
				CErrorObject cErrObj( (*pFeature) ); // �G���[���i�[�p

				pFeature->Delete();
			} else {
				cErrObjects.push_back( CErrorObject( (*pRow) ) );
				_ASSERTE(false);
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �N���[�����쐬����
sindyErrCode CFeatures::Clone( IWorkspace* ipWorkspace/* = NULL*/ )
{
	sindyErrCode emErr = sindyErr_NoErr; // �߂�l

	// ipWorkspace���w�肳��Ă���ꍇ�͓����̃e�[�u�����I�[�v������
	// SiNDY-b�̃��[���Ƃ��āA�ڑ����[�U�Ɠ����I�[�i�[�̃e�[�u�������ҏW���Ă͂Ȃ�Ȃ�
	// �Ƃ����Öق̃��[��������̂ŁA�ڑ����[�U���I�[�i�[���Ƃ��Č�������
	CWorkspace cWork( ipWorkspace );
	CString strOwnerName;
	if( cWork )
		strOwnerName = cWork.GetNameString()->GetUserName();

	for( iterator it = begin(); it != end(); ++it )
	{
		if( sindyErr_NoErr != ( emErr = (*it)->CreateCache() ) ) // �L���b�V�����쐬����
			break;

		// 
		CTable cTable;
		if( cWork )
		{
			cTable = cWork.OpenTable( ( ! strOwnerName.IsEmpty() ) ? strOwnerName + _T(".") + CString((*it)->GetTableName()) : (*it)->GetTableName() );
			if( ! cTable )
			{
				emErr = sindyErr_AOGetFeatureClassFailed; // �e�[�u�����Ȃ��ꍇ�͌p�������Ȃ�
				break;
			}
			else {
				// �e�[�u����}���ւ���
				static_cast<CTable&>(**it).SetObject( (ITable*)cTable );
				cTable = static_cast<CTable&>(**it);
			}
		}
		for( CContainer::iterator it2 = (*it)->begin(); it2 != (*it)->end(); ++it2 )
		{
			for( CContainer::rows_iterator itRow = it2->second.begin(); itRow != it2->second.end(); ++itRow )
			{
				if( sindyErr_NoErr != ( emErr = (*itRow)->ReplaceNewRow( &cTable, true ) ) ) // �K�w�I�ɑ}���ւ�
					break;
			}
			if( sindyErr_NoErr != emErr )
				break;
		}
		if( sindyErr_NoErr != emErr )
			break;
	}
	return emErr;
}

// �ҏW���e��ۑ�����
sindyErrCode CFeatures::Store( CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	// ModifyCheck�ŕҏW���ꂽ�I�u�W�F�N�g��ΏۂɃ`�F�b�N
	LOGASSERTE_IF( sindyErr_NoErr == ( emErr = CheckModify( cErrObjects ) ), sindyErr_AlreadyModified ) // �{���̃G���[�R�[�h��emErr�ɓ��邩�炱���̃G���[�R�[�h�͓K��
	{
		// �L���b�V��������΁A�L���b�V���ւ̕ύX���ʂ𔽉f���Ȃ���Store
		// �����͂����Ŏ����I�ɕt�^
		for( const_iterator it = begin(); it != end(); ++it )
		{
			if( sindyErr_NoErr != ( emErr = (*it)->Store( cErrObjects ) ) )
				break;	// �G���[���N������break
		}
	}
	return emErr;
}

// ���������`�F�b�N����
sindyErrCode CFeatures::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it ) {
		if ( (*it)->GetShapeType() == esriGeometryPolygon ) {
			// �d���`�F�b�N�p�ɕҏW�̂������I�u�W�F�N�g�̌`��Œǉ��������Ă���`�F�b�N�B
			IGeometryPtr ipEditArea = (*it)->GetEditArea();

			if ( ipEditArea )
				(*it)->Select( ipEditArea, false, true, NULL, false );
		}

		(*it)->CheckLogic( cRule, cErrObjects );

		// [Bug 5846]�U�������N�̒[�_�͕��s�҃m�[�h��ɂ��邩�`�F�b�N
		if ( (*it)->GetTableType() == sindyTableType::guide_link )	// �U�������N�H
			CheckLogic_GuideLink( (*it), cErrObjects );
	}


	return cErrObjects.GetDefaultErrCode();
}

sindyErrCode CFeatures::CheckModify( CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
		(*it)->CheckModify( cErrObjects );
	
	return cErrObjects.GetDefaultErrCode();
}

void CFeatures::clear()
{
	for( iterator it = begin(); it != end(); ++it )
		delete *it;
	m_listFeatureContainer.clear();
}

bool CFeatures::empty() const
{
	bool bRet = true;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( ! (*it)->empty() )
		{
			bRet = false;
			break;
		}
	}

	return bRet;
}

// ���݊i�[����Ă���t�B�[�`���̑��݋�`��Ԃ�
IEnvelopePtr CFeatures::GetEnvelope( bool bOriginal/* = true*/, bool bChanged/* = true*/, bool bFeatureChangedOriginal/* = true*/ ) const
{
	IEnvelopePtr ipRet;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		IEnvelopePtr ipEnv = (*it)->GetEnvelope( bOriginal, bChanged, bFeatureChangedOriginal );
		if( ipRet != NULL )
		{
			if( ipEnv != NULL )
			{
				LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed )
				{
				}
			}
		}
		else
			ipRet = ipEnv;
	}
	return ipRet;
}

// ���݊i�[����Ă���t�B�[�`���̌`���Ԃ�
IGeometryPtr CFeatures::GetShape( bool bLatest/* = true*/, bool bOriginal/* = false*/, bool bFeatureChangedOriginal/* = false*/, long lParam/* = -1*/ ) const
{
	IGeometryPtr ipRet;

	IGeometryCollectionPtr ipPointsCol, ipPolylinesCol, ipPolygonsCol;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		IGeometryPtr ipGeom = (*it)->GetShape( bLatest, bOriginal, bFeatureChangedOriginal, lParam );
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
	// ���ꂼ���Union����
	ITopologicalOperatorPtr ipPointsTopo, ipPolylinesTopo, ipPolygonsTopo;
	if( ipPointsCol != NULL )
	{
		ipPointsTopo.CreateInstance( CLSID_Multipoint );
		ipPointsTopo->ConstructUnion( (IEnumGeometryPtr)ipPointsCol );
	}
	if( ipPolylinesCol != NULL )
	{
		ipPolylinesTopo.CreateInstance( CLSID_Polyline );
		ipPolylinesTopo->ConstructUnion( (IEnumGeometryPtr)ipPolylinesCol );
	}
	if( ipPolygonsCol != NULL )
	{
		ipPolygonsTopo.CreateInstance( CLSID_Polygon );
		ipPolygonsTopo->ConstructUnion( (IEnumGeometryPtr)ipPolygonsCol );
	}
	// �ǂꂩ������Ȃ���΂����Ԃ�l�Ƃ���
	bool bPoints    = ( ipPointsTopo != NULL && ipPolylinesTopo == NULL && ipPolygonsTopo == NULL );
	bool bPolylines = ( ipPointsTopo == NULL && ipPolylinesTopo != NULL && ipPolygonsTopo == NULL );
	bool bPolygons  = ( ipPointsTopo == NULL && ipPolylinesTopo == NULL && ipPolygonsTopo != NULL );
	if( bPoints || bPolylines || bPolygons )
	{
		if( bPoints )
			ipRet = ipPointsTopo;
		else if( bPolylines )
			ipRet = ipPolylinesTopo;
		else if( bPolygons )
			ipRet = ipPolygonsTopo;
	}
	else if( ipPointsTopo != NULL || ipPolylinesTopo != NULL || ipPolygonsTopo != NULL )
	{
		IGeometryCollectionPtr ipGeomCol( CLSID_GeometryBag );
		if( ipPointsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPointsTopo );
		if( ipPolylinesTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolylinesTopo );
		if( ipPolygonsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolygonsTopo );
		ipRet = ipGeomCol;
	}
	return ipRet;
}

#ifdef _DEBUG
void CFeatures::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
{
	for( const_iterator it = begin(); it != end(); ++it ) (*it)->Trace( bAttribute, bGeometry );
}
#else
void CFeatures::Trace( bool/* bAttribute = true*/, bool/* bGeometry = true*/ ) const
{
}
#endif // ifdef _DEBUG

// ���������`�F�b�N����
sindyErrCode CFeatures::CheckLogic_GuideLink( CFeatureContainer* pContainer, CErrorObjects& cErrObjects ) const
{
	if ( pContainer ) {
		// [Bug 5846] [���s��NW]�U�������N�̒[�_�I���ŒZ�k���Ȃ��悤�ɂ��ė~����
		// �U�������N�̒[�_�͕��s�҃m�[�h��ɂȂ���΂����Ȃ��݂����Ȃ̂ŁA�����Ő������`�F�b�N
		CRoadNetwork* pRoadNetwork = NULL;

		// �l�b�g���[�N���f�������邩�`�F�b�N
		for( const_iterator it = begin(); it != end(); ++it ) {
			if( AheIsRelateWithNetwork( (*it)->GetTableName() ) )
			{
				if ( AheTableName2TableType( AheGetNetworkNodeFeatureClassName( (*it)->GetTableName() ) ) == sindyTableType::walk_node ) {
					pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );
					break;
				}
			}
		}

		for ( CContainer::const_rows_iterator itGuide = pContainer->begin(pContainer->GetTableType()), itGuideEnd = pContainer->end(pContainer->GetTableType()); 
			itGuide != itGuideEnd; ++itGuide ) {

			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			// �ҏW�������� and �폜����Ă��Ȃ����̂̂݃`�F�b�N
			if ( (*itGuide)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) {
				// �ҏW�O�̌`��ƕҏW��̌`��ŁA�n�I�_�ɕύX�����邩�`�F�b�N
				CGeometry cLastestFrom = (*itGuide)->GetShape()->GetPoint( 0 );
				CGeometry cLastestTo = (*itGuide)->GetShape()->GetPoint( (*itGuide)->GetShape()->GetPointCount() - 1 );

				// [Bug 6142] Guide_Link�𕪒f����悤�ɃZ�O�����g�폜����Ɨ�����
				// �ҏW���ꂽ�n�I�_�̂݃m�[�h��ɂ��邩�ǂ������`�F�b�N���邽�߂ɁA
				// �`��̍���������Ďn�I�_���ҏW���ꂽ���m�F�����Ă����̂ŁA
				// Split�������ɐV�������ꂽ�I�u�W�F�N�g�̃`�F�b�N�ŁA
				// ���̌`�󂪂Ȃ����߂ɗ����Ă����B
				// �ҏW���ꂽ�n�I�_�݂̂ł͂Ȃ��A�n�I�_���m�[�h��ɂ��邩�ǂ������`�F�b�N����悤�ɏC���B
				// Bug 6142 CGeometry cOrginalGeom = (*itGuide)->GetShapeCopy( false, true );
				// Bug 6142 CGeometry cOrginalFrom = cOrginalGeom.GetPoint( 0 );
				// Bug 6142 CGeometry cOrginalTo = cOrginalGeom.GetPoint( cOrginalGeom.GetPointCount() - 1 );

				// Bug 6142 bool bEditFrom = false, bEditTo = false;

				// Bug 6142 if ( ! cLastestFrom.Equals( cOrginalFrom ) )	// �n�_���Ⴄ�H
				// Bug 6142 	bEditFrom = true;

				// Bug 6142 if ( ! cLastestTo.Equals( cOrginalTo ) )	// �I�_���Ⴄ�H
				// Bug 6142 	bEditTo = true;

				// Bug 6142 if ( bEditFrom || bEditTo ) {	// �n�I�_�ɕύX������H
					bool bHasFrom = false, bHasTo = false;

					if ( pRoadNetwork ) {	// ���s��NW���Ȃ���Όx������
						// �ύX���������n�I�_�Ɉ�v������s�҃m�[�h�����邩�`�F�b�N
						sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType( AheGetNetworkNodeFeatureClassName( pRoadNetwork->GetTableName() ) ); // �m�[�h�e�[�u���^�C�v

						for ( CContainer::const_rows_iterator itNode = pRoadNetwork->begin(emNetworkNodeTableType), itNodeEnd = pRoadNetwork->end(emNetworkNodeTableType); 
							itNode != itNodeEnd; ++itNode ) {

							emUpdateType = schema::ipc_table::update_type::kDefault;

							// �ҏW�������� or �폜����Ă��Ȃ����̂̂݃`�F�b�N
							if ( (*itNode)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
								// Bug 6142 if ( bEditFrom ) {
									if ( cLastestFrom.Equals( (IGeometry*)*(*itNode)->GetShape() ) )
										bHasFrom = true;
								// Bug 6142 }

								// Bug 6142 if ( bEditTo ) {
									if ( cLastestTo.Equals( (IGeometry*)*(*itNode)->GetShape() ) )
										bHasTo = true;
								// Bug 6142 }
							}
						}
					}

					if ( (/* Bug 6142 bEditFrom &&*/ ! bHasFrom) || (/* Bug 6142 bEditTo &&*/ ! bHasTo) )	// �n�I�_�Ɉ�v������s�҃m�[�h���Ȃ�
						cErrObjects.push_back( CErrorObject( *itGuide->get(), CErrorInfo( sindyErr_RuleRelationshipContainGeometryNotOnWalkNode, sindyErrLevel_WARN ) ) );
				// Bug 6142 }
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

} // sindy
