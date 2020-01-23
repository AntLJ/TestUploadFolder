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

#include "stdafx.h"
#include "ModelContainContainer.h"
#include "GlobalFunctions.h"
#include "RuleModel.h"
#include "Feature.h"
#include "TableTypeUtil.h"

using namespace sindy;
using namespace sindy::errorcode;

CModelContainContainer::CModelContainContainer(void)
{
}

CModelContainContainer::~CModelContainContainer(void)
{
}

// �`��Ō�������
sindyErrCode CModelContainContainer::Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects *pErrs/* = NULL*/, bool bInit /*= true*/  )
{
	// �W�I���g����NULL�ł������ł��邪�A�����ł͂�����
	// �G���[�Ƃ��邱�Ƃŗ\�����Ȃ��S���������������
	RETURN_IF( ! ipGeom, sindyErr_ArgIsNull )

	// �x�[�X�t�B�[�`������������
	sindyErrCode emErr = CFeatureContainer::Select( ipGeom, bRecycling, bCreateCache, pErrs, bInit );
	if( emErr != sindyErr_NoErr ) // �G���[���O��CFeatureContainer�ŏo��̂ł���Ȃ�
		return emErr;

	// �`��쐬(�܂ޑ��j
	IGeometryCollectionPtr ipGeomBag( CLSID_GeometryBag );

	((IGeometryPtr)ipGeomBag)->putref_SpatialReference( GetSpatialReference() );
	ipGeomBag->AddGeometry( ipGeom );

	for ( CContainer::const_rows_iterator itContain = CContainer::begin(GetTableType()); itContain != CContainer::end(GetTableType()); ++itContain )
		ipGeomBag->AddGeometry( (*itContain)->GetShapeCopy() );

	// ������t�B�[�`������������
	for( CRuleRelationshipContain::const_value_iterator it = g_cRuleRelationshipContain.begin(GetTableName()), itEnd = g_cRuleRelationshipContain.end(GetTableName()); it != itEnd; ++it )
	{
		IGeometryCollectionPtr ipGeomContainBag( CLSID_GeometryBag );
		sindyTableType::ECode emContainType = AheTableName2TableType( it->first );
		CFeatureClass cClass( OpenSameOwnerTable( emContainType ) );
		((IGeometryPtr)ipGeomContainBag)->putref_SpatialReference( cClass.GetSpatialReference() );

		IFeatureCursorPtr ipCursor( cClass.Search( AheInitSpatialFilter( NULL, (IGeometryPtr)ipGeomBag ), bRecycling ) );
		LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
		{
			CSPFieldMap spFieldMap = cClass.GetFieldMap();
			CSPTableNameString spNameString = cClass.GetNameString();

			IFeaturePtr ipFeature;
			while( S_OK == ipCursor->NextFeature( &ipFeature ) )
			{
				if( ipFeature == NULL ) break;

				CSPRow cFeat( new CFeature( ipFeature, emContainType, false, spFieldMap, spNameString ) );

				// �ǉ��������[�h�̏ꍇ�A�����t�B�[�`���ł�������o�^�͂��Ȃ�
				bool bFind = false;

				// ���Ɋ֌W�Â����Ă���H
				for ( const_rows_iterator itContain = CContainer::begin(GetTableType()); itContain != CContainer::end(GetTableType()); ++itContain ) {
					if ( (*itContain)->find( *cFeat.get() ) ) {
						bFind = true;
						break;
					}
				}

				if ( !bFind ) {
					// �֌W�Â����Ă��Ȃ����Ǌi�[�ς݁H
					if ( find( *cFeat.get() ) )
						bFind = true;
				}

				if( bInit || ! bFind )
				{
					// ���T�C�N������ꍇ�̓L���b�V�������
					if( bRecycling || bCreateCache )
						cFeat->CreateCache( bRecycling ? true : false );
				}

				if ( !bFind ) {
					// �֘A�t��
					for ( rows_iterator itContain = CContainer::begin(GetTableType()); itContain != CContainer::end(GetTableType()); ++itContain ) {
						if ( (*itContain)->GetShape()->Contains( cFeat->GetShapeCopy() ) || (*itContain)->GetShape()->Equals( cFeat->GetShapeCopy() ) ) {
							(*itContain)->push_back( cFeat );

							// �`��쐬(�܂܂�鑤�j
							ipGeomContainBag->AddGeometry( cFeat->GetShapeCopy() );
						}
					}
				}
			}

			long nCount = 0;

			ipGeomContainBag->get_GeometryCount( &nCount );

			if ( nCount > 0 ) {
				// IGeometryCollection�Ɋi�[����Ă���`�����ɂ܂Ƃ߂� [Arc10.3.1�ڍs�Ή�]
				// (2017/3/29)�s�s�����|���S���A�s�s�����K���|���S���Ō��؂����{�B
				// Polyline��Union�\�ł��邪�A���݂̃��f���ł͂��̊֐��𗘗p���ĕR�t�����s��Polyline�̃��C�����Ȃ����ߖ����؁B
				CGeometry unionGeom;
				for (long l = 0; l < nCount; ++l)
				{
					IGeometryPtr geom;
					ipGeomContainBag->get_Geometry(l, &geom);
					emErr = unionGeom.Union(geom);
						if (emErr != sindyErr_NoErr)
							return emErr;
				}
				// �܂ޑ��̒ǉ�����
				sindyErrCode emErr = CModelContainContainer::Select( unionGeom, bRecycling, bCreateCache, pErrs, false );
				if( emErr != sindyErr_NoErr ) // �G���[���O��CFeatureContainer�ŏo��̂ł���Ȃ�
					return emErr;
			}

			ipCursor = NULL;
		}
	}

	return emErr;
}

// �^����ꂽ�`��Ƃ̌��������ŕ����������s��
errorcode::sindyErrCode CModelContainContainer::Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

		if( pFeature )
		{
			_Split( pSource, cGeom, cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �^����ꂽ�`��Ƃ̌��������ŕ����������s��
errorcode::sindyErrCode CModelContainContainer::_Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	// TODO: CRoadNetwork::_Split�ACFeatures::Split�Ǝ����悤�ȃR�[�h����B���ꂷ�ׂ��I�I
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

		if( pFeature )
		{
			if( GetTableType() == pSource->GetTableType() )
			{
				// �܂܂�鑤��Split
				for( CRuleRelationshipContain::const_value_iterator itRule = g_cRuleRelationshipContain.begin(GetTableName()),
					itRuleEnd = g_cRuleRelationshipContain.end(GetTableName());
					itRule != itRuleEnd; ++itRule )
				{
					CContainer cContains;
					sindyTableType::ECode emContainType = AheTableName2TableType( itRuleEnd->first );

					for( const_rows_iterator itCon = pSource->begin(emContainType),
						itConEnd = pSource->end(emContainType);
						itCon != itConEnd; ++itCon )
						cContains.push_back( (*itCon) );

					for ( CContainer::rows_iterator itContain = cContains.begin(emContainType), itContainEnd = cContains.end(emContainType); itContain != itContainEnd; ++itContain )
						_Split( (*itContain), cGeom, cErrObjects );
				}

				CGeometry cSplittedGeoms, cSplittedPoints;

				// Split
				if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
					if ( (IGeometry*)cSplittedGeoms ) {
						if ( cSplittedGeoms.GetPartCount() > 0 ) {
							// �����`������蓖�Ă�
							for ( CGeometry::const_part_iterator itPart = cSplittedGeoms.part_begin(), itPartEnd = cSplittedGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
								CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

								if ( (IGeometry*)cNewGeom ) {
									CSPRow pCon;
									WKSPoint* point_wks = new WKSPoint[cNewGeom.GetPointCount()];

									((IPointCollectionPtr)(IGeometry*)cNewGeom)->QueryWKSPoints( 0, cNewGeom.GetPointCount(), point_wks );

									if ( itPart == cSplittedGeoms.part_begin() ) {
										// ���݂̒��g�Ɠ���ւ���
										((IPointCollectionPtr)(IGeometry*)*pFeature->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );
										pCon = pSource;
									} else {
										// �t�B�[�`���쐬
										pCon.reset( new CFeature( NULL, pSource->GetTableType(), true, pSource->GetFieldMap(), pSource->GetNameString() ) );

										CTable( pSource->GetTable() ).CreateRow( *pCon );

										// �������R�s�[
										pSource->CopyTo( *pCon, true, false, false, false );

										// �`����Z�b�g
										((IPointCollectionPtr)(IGeometry*)*pCon->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

										// �܂ޑ����i�[
										push_back( pCon );
									}
									delete [] point_wks;
									point_wks = NULL;
								}
							}

							// �܂܂�鑤�֘A�t��
							for( CRuleRelationshipContain::const_value_iterator itRule = g_cRuleRelationshipContain.begin(GetTableName()),
								itRuleEnd = g_cRuleRelationshipContain.end(GetTableName());
								itRule != itRuleEnd; ++itRule )
							{
								CContainer cContains;
								sindyTableType::ECode emContainType = AheTableName2TableType( itRule->first );

								// �ύX���ꂽ���̂��擾
								for ( const_rows_iterator itContain = begin(GetTableType()); itContain != end(GetTableType()); ++itContain ) {
									for( const_rows_iterator itCon = (*itContain)->begin(emContainType),
										itConEnd = (*itContain)->end(emContainType);
										itCon != itConEnd; ++itCon ) {
											schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

											if ( (*itCon)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted )
												cContains.push_back( (*itCon) );
									}
								}

								// �֘A�t��
								for ( CContainer::const_rows_iterator itCon = cContains.begin(AheTableName2TableType(itRule->first)), itConEnd = cContains.end(AheTableName2TableType(itRule->first)); itCon != itConEnd; ++itCon ) {
									for ( rows_iterator itContain = CContainer::begin(GetTableType()); itContain != CContainer::end(GetTableType()); ++itContain ) {
										if ( (*itContain)->GetShape()->Contains( (*itCon)->GetShapeCopy() ) || (*itContain)->GetShape()->Equals( (*itCon)->GetShapeCopy() ) ) {
											// �i�[
											(*itContain)->push_back( (*itCon) );
										} else
											// �܂ޑ����ς�����̂ŏ��O
											(*itContain)->erase( (*itCon) );
									}
								}
							}
						}
					}
				}
			}
			else {
				// TODO: ��̃R�[�h�ɂ������ӏ�������B�Ȃ�Ƃ����ꂷ�邱�ƁI�I
				CGeometry cSplittedGeoms, cSplittedPoints;

				// Split
				if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
					if ( (IGeometry*)cSplittedGeoms ) {
						if ( cSplittedGeoms.GetPartCount() > 0 ) {
							CSPRow pContain;

							for( rows_iterator it = begin(GetTableType()),
								itEnd = end(GetTableType());
								it != itEnd; ++it )
							{
								// ��������܂ޑ����擾
								for ( const_rows_iterator itContain = begin(GetTableType()); itContain != end(GetTableType()); ++itContain ) {
									for( CRuleRelationshipContain::const_value_iterator itRule = g_cRuleRelationshipContain.begin(GetTableName()),
										itRuleEnd = g_cRuleRelationshipContain.end(GetTableName());
										itRule != itRuleEnd; ++itRule )
									{
										sindyTableType::ECode emContainType = AheTableName2TableType( itRule->first );

										for( const_rows_iterator itCon = (*itContain)->begin(emContainType),
											itConEnd = (*itContain)->end(emContainType);
											itCon != itConEnd; ++itCon )
										{
											if ( pSource == (*itCon) ) {
												pContain = *itContain;
												break;
											}
										}
									}

									if ( pContain )
										break;
								}

								if ( pContain )
									break;
							}

							if ( pContain ) {
								// �܂ޑ��ɍ쐬�t�B�[�`�����i�[
								CTable cTable( pSource->GetTable() );

								// �����`������蓖�Ă�
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

											cNew.reset( new CFeature( NULL, pSource->GetTableType(), true, pSource->GetFieldMap(), pSource->GetNameString() ) );

											cTable.CreateRow( *cNew.get() );

											// �������R�s�[
											pSource->CopyTo( *cNew.get(), true, false, false, false );

											// �`����Z�b�g
											((IPointCollectionPtr)(IGeometry*)*cNew->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

											// �܂ޑ��ɒǉ�
											pContain->push_back( cNew );
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
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CModelContainContainer::Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource && pDest, sindyErr_ArgIsNull )
	{
		if( *pSource->GetNameString() == *pDest->GetNameString() )	// �����^�C�v�H
		{
			CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

			if ( pFeature ) {
				// [Bug 5798]�܂ޑ��̃}�[�W�̎��́A�܂܂�鑤�̏�����ύX����B
				// ��܊֌W�ł͂Ȃ��}�[�W�̎��́A�܂܂�鑤�̏�����ύX����̂��A�܂܂�鑤���}�[�W����̂��A
				// ��X�I���ł��Ȃ��Ⴂ���Ȃ��C�����邯�ǁA�g���Ă��DeleteTool���������珊����ύX����ق��ɂ��Ƃ�
				// �܂܂�鑤�̃}�[�W���́A�܂ޑ��͂Ȃɂ����Ȃ�
				if( GetTableType() == pSource->GetTableType() )	// �܂ޑ��H
				{
					// �폜�����܂ޑ��ɏ�������܂܂�鑤��ύX
					for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
						itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
						itContain != itContainEnd; ++itContain )
					{
						sindyTableType::ECode emContainType = AheTableName2TableType( itContain->first );
						CContainer cContainContainer;	// �ꎞ�p

						// TODO : CContainer::push_back()/CContainer::erase()���Aiterator��Ԃ��Ă���Ȃ�����
						//        �ꎞ�p�Ɋi�[���Ă���ǉ�/�폜���Ă�B
						// �ꎞ�p�Ɋi�[
						for( rows_iterator itCon = pDest->begin(emContainType),
							itConEnd = pDest->end(emContainType);
							itCon != itConEnd; ++itCon )
						{
							cContainContainer.push_back( (*itCon) );
						}

						// ������ύX
						for( rows_iterator itCon = cContainContainer.begin(emContainType),
							itConEnd = cContainContainer.end(emContainType);
							itCon != itConEnd; ++itCon )
						{
							pSource->push_back( (*itCon) );
							pDest->erase( (*itCon) );	// �폜����Ȃ��悤�ɏ��O
						}
					}

					// �`��̃}�[�W
					if ( sindyErr_NoErr == pFeature->Merge( *pDest->GetShape(), cErrObjects ) ) {
						// �}�[�W��̍폜
						pDest->Delete();
					}
				}
				else
				{
					// �`��̃}�[�W
					if ( sindyErr_NoErr == pFeature->Merge( *pDest->GetShape(), cErrObjects ) ) {
						// �}�[�W��̍폜
						pDest->Delete();
					}
				}
			}
		} else {
			cErrObjects.push_back( CErrorObject( (*pSource) ) );
			cErrObjects.push_back( CErrorObject( (*pDest) ) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CModelContainContainer::Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

		if ( pFeature ) {
			CGeometry cIntersect = pFeature->GetShape()->IntersectPoints( cGeom );

			if( (IGeometry*)cIntersect && ! cIntersect.IsEmpty() )
			{ 
				if( GetTableType() == pRow->GetTableType() )
				{
					// �֘A�I�u�W�F�N�g�̍폜
					if ( sindyErr_NoErr != pFeature->GetShape()->Erase( cIntersect ) )
						cErrObjects.push_back( (*pRow) );

					for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
						itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
						itContain != itContainEnd; ++itContain )
					{
						sindyTableType::ECode emContainType = AheTableName2TableType( itContain->first );

						for( rows_iterator itCon = pRow->begin(emContainType),
							itConEnd = pRow->end(emContainType);
							itCon != itConEnd; ++itCon )
						{
							if ( sindyErr_NoErr != Erase( *itCon, cIntersect, cErrObjects ) )
								break;
						}
					}
				}
				else {
					// �폜
					if ( sindyErr_NoErr != pFeature->GetShape()->Erase( cIntersect ) )
						cErrObjects.push_back( (*pRow) );

					// �܂ޑ��̍폜
					for( rows_iterator it = begin(GetTableType()),
						itEnd = end(GetTableType());
						it != itEnd; ++it )
					{
						bool bHasCon = false;

						// ����֌W�̂���
						for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
							itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
							itContain != itContainEnd; ++itContain )
						{
							sindyTableType::ECode emContainType = AheTableName2TableType( itContain->first );

							for( const_rows_iterator itCon = (*it)->begin(emContainType),
								itConEnd = (*it)->end(emContainType);
								itCon != itConEnd; ++itCon )
							{
								if ( pRow == (*itCon) ) {
									bHasCon = true;
									break;
								}
							}

							if ( bHasCon )
								break;
						}

						if ( bHasCon )
							// �폜
								Erase( *it, cIntersect, cErrObjects );
					}
				}
			}
		}
		else
			cErrObjects.push_back( CErrorObject( (*pRow) ) );
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CModelContainContainer::Delete( CSPRow pRow, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

		if( pFeature )
		{
			if( GetTableType() == pRow->GetTableType() )
			{
				// �֘A�I�u�W�F�N�g�̍폜��CRow::Delete()��
				// �s���悤�ɂ���
				pRow->Delete(true);
			}
			else {
				// TODO: pRow���܂܂�鑤�̏ꍇ�A������폜���邱�Ƃɂ����
				// �܂܂����̂�0�ɂȂ����ꍇ�͊܂ޑ����폜����̂����݂̎d�l
				// �{���́A��܊֌W�� 1...n�A0...n�̂ǂ��炩��������Ȃ��Ƃ���
				// �Ȃ̂ł��̂悤�ȃ��[����ǉ�����K�v����B
				// �ŁA�폜��������0�ɂȂ邩�ǂ����́ACContainer::size()��
				// �e�[�u���^�C�v�̑��ɍ폜�������̂��܂ނ��ǂ����̃t���O������
				// �ɒǉ��ł���悤�ɂ���΃R�[�h�팸�ƂȂ�͂��Ȃ̂Ō��
				// ��������

				// �폜
				pRow->Delete();

				// �܂ޑ��̍폜
				for( rows_iterator it = begin(GetTableType()),
					itEnd = end(GetTableType());
					it != itEnd; ++it )
				{
					bool bHasCon = false;

					// ����֌W�̂���
					for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
						itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
						itContain != itContainEnd; ++itContain )
					{
						sindyTableType::ECode emContainType = AheTableName2TableType( itContain->first );

						for( const_rows_iterator itCon = (*it)->begin(emContainType),
							itConEnd = (*it)->end(emContainType);
							itCon != itConEnd; ++itCon )
						{
							if ( pRow == (*itCon) ) {
								bHasCon = true;
								break;
							}
						}

						if ( bHasCon )
							break;
					}

					if ( bHasCon )
						// �폜
							Delete( (*it), cErrObjects );
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �ҏW���e��ۑ�����
errorcode::sindyErrCode CModelContainContainer::Store( CErrorObjects& cErrObjects )
{
	// ���f���Ɠ���֌W���f��
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->Store( cErrObjects );
	}

	return cErrObjects.GetDefaultErrCode();
}

// ���������`�F�b�N����
errorcode::sindyErrCode CModelContainContainer::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	// �d���`�F�b�N��
	CContainer::CheckLogic( cRule, cErrObjects );

	// TODO : �N�����������邱��
	for ( const_rows_iterator it = CContainer::begin(GetTableType()); it != CContainer::end(GetTableType()); ++it ) {
		schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

		(*it)->Changed( &emUpdateType );
		if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
			(*it)->CheckLogic( cRule, cErrObjects );

			// �܂܂�鑤�̏d���`�F�b�N��
			static_cast<CContainer>(*(*it)).CheckLogic( cRule, cErrObjects );

			// �����`�F�b�N
			CGeometry cContainGeom;

			for( CRuleRelationshipContain::const_value_iterator itRule = g_cRuleRelationshipContain.begin(GetTableName()), itRuleEnd = g_cRuleRelationshipContain.end(GetTableName()); itRule != itRuleEnd; ++itRule )
			{
				sindyTableType::ECode emContainType = AheTableName2TableType( itRule->first );

				for ( const_rows_iterator itContain = (*it)->begin(emContainType); itContain != (*it)->end(emContainType); ++itContain ) {
					emUpdateType = schema::ipc_table::update_type::kDefault;

					(*itContain)->Changed( &emUpdateType );
					if ( emUpdateType != schema::ipc_table::update_type::kDeleted )
						cContainGeom.Union( (*itContain)->GetShapeCopy() );
				}
			}

			// �܂܂�鑤���Ȃ��̂͂�������
			if ( !(IGeometry*)cContainGeom )
				cErrObjects.push_back( CErrorObject( *it->get(), CErrorInfo( sindyErr_BadModelContainContain, sindyErrLevel_ERR ) ) );
			else {
				// Equals���ƁA���߂ۂ��Ă����鎖�i�_��̏��Ԃ��Ⴄ�Ƃ��Łj�������̂�SymmetricDifference���āA
				// �`�󂪂Ȃ����OK�ł����NG�B
				IGeometryPtr ipSymGeomBase = CGeometry::SymmetricDifference( (*it)->GetShapeCopy(), cContainGeom );	// �܂ޑ��ɑ΂��Ċ܂܂�鑤��SymmetricDifference
				IGeometryPtr ipSymGeomContain = CGeometry::SymmetricDifference( cContainGeom, (*it)->GetShapeCopy() );	// �܂܂�鑤�ɑ΂��Ċ܂ޑ���SymmetricDifference

				if ( ipSymGeomBase || ipSymGeomContain ) {
					VARIANT_BOOL vbIsEmptyBase = VARIANT_TRUE;

					if ( ipSymGeomBase )
						ipSymGeomBase->get_IsEmpty( &vbIsEmptyBase );

					VARIANT_BOOL vbIsEmptyContain = VARIANT_TRUE;

					if ( ipSymGeomContain )
						ipSymGeomContain->get_IsEmpty( &vbIsEmptyContain );

					// �܂ޑ��̌`��Ɗ܂܂�鑤�̌`��ō���������̂͂��������̂ŃG���[
					if ( ! vbIsEmptyBase || ! vbIsEmptyContain )
						cErrObjects.push_back( CErrorObject( *it->get(), CErrorInfo( sindyErr_BadModelContain, sindyErrLevel_ERR ) ) );
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��
sindyErrCode CModelContainContainer::CheckModify( CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->CheckModify( cErrObjects );
	}

	return cErrObjects.GetDefaultErrCode();
}
