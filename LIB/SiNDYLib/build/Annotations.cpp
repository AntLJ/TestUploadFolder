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
 * @file Annotations.cpp
 * @brief <b>CAnnotations�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @vertion $Id$
 */
#include "stdafx.h"
#include "Annotations.h"
#include "GlobalFunctions.h"
#include "util.h"
#include "TableTypeUtil.h"
#include "AheWKSFunctions.h"
#include "AnnoGlobals.h"
#include "GeometryRule.h"
#include <algorithm>
#include "ErrorObject.h"
#include "GlobalRules.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR ANNO_START_SEARCH       = _T("CAnnotations : start a search%s...");
	LPCTSTR EFFECTIVE_CACHE         = _T("(Cache is effective)");
	LPCTSTR NON_EFFECTIVE_CACHE     = _T("(Cache is not effective)");
	LPCTSTR SEARCH_TRUE_POINT	    = _T("True position[search %d Features : %.5lfsec]...");
	LPCTSTR SEARCH_DISPPOINT_STATUS = _T("Display position[Total time:%.5lfsec, search:%.5lfsec, Fetch loop:%.5lfsec, The first half of Fetch loop:%.5lfsec, SetDispLine:%.5lf] Search Table[%ds:%.5lfsec]...");
	LPCTSTR ADD_SELECTED_DISPPOINT  = _T("Adds a selected display position [%s:%d]\n");
	LPCTSTR FINISH_SEARCH_ANNO      = _T("Finished searching[Total:%.5lfsec cache:%.5lfsec]\n");
	LPCTSTR START_CACHE_CREATION    = _T("Start creating parameter cache...");
	LPCTSTR FINISH_CREATE_SEC       = _T("Finished creating:%.5lfsec\n");
	LPCTSTR REMOVE_OUT_OF_AREA      = _T("[%s:%d] It is removed because it is out of a search area\n");
	LPCTSTR START_SPATIAL_CREATION  = _T("Start creating spatial index...");
#else
	LPCTSTR ANNO_START_SEARCH       = _T("CAnnotations : �����J�n%s�c");
	LPCTSTR EFFECTIVE_CACHE         = _T("�i�L���b�V������j");
	LPCTSTR NON_EFFECTIVE_CACHE     = _T("�i�L���b�V���Ȃ��j");
	LPCTSTR SEARCH_TRUE_POINT	    = _T("�^�ʒu[%d�����F%.5lf�b]�c");
	LPCTSTR SEARCH_DISPPOINT_STATUS = _T("�\���ʒu[�����ԁF%.5lf�b �����F%.5lf�b �t�F�b�`���[�v�F%.5lf�b �t�F�b�`���[�v�O���F%.5lf�b SetDispLine�F%.5lf] �e�[�u������[%d��F%.5lf�b]�c");
	LPCTSTR ADD_SELECTED_DISPPOINT  = _T("�\���ʒu�ǉ��擾 [%s:%d]\n");
	LPCTSTR FINISH_SEARCH_ANNO      = _T("�����I��[Total�F%.5lf�b �L���b�V���F%.5lf�b]\n");
	LPCTSTR START_CACHE_CREATION    = _T("�p�����[�^�L���b�V���쐬�J�n�c");
	LPCTSTR FINISH_CREATE_SEC       = _T("�쐬�����F%.5lf�b\n");
	LPCTSTR REMOVE_OUT_OF_AREA      = _T("[%s:%d] �����G���A�O�̂��ߏ��O���܂�\n");
	LPCTSTR START_SPATIAL_CREATION  = _T("��ԃC���f�b�N�X�쐬�J�n�c");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

/**
 * @brief �������������I�u�W�F�N�g���������Ƃ��ۂ̃X�P�[������e�[�u���^�C�v
 *
 * CAnnotations::_SelectByShape()�Ŏg�p
 * �t�B�[�`���R�s�[�c�[���ł̎g�p��O��Ƃ��Ă���̂ŁA���̕ϐ���
 * �g�p����ۂ͎Q�l�ɂ��邱�ƁB
 */
sindyTableType::ECode g_emViewTableType = sindyTableType::unknown;

// ���b�V���R�[�h�Ō�������</b>\n
sindyErrCode CAnnotations::SelectByMesh( long lMeshCode, mesh_type::ECode emMeshType, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/ )
{
	// mesh_type -> group
	sindyAnnotationGroup emGroup = (sindyAnnotationGroup)-1;
	switch( emMeshType )
	{
		case mesh_type::kCity:   emGroup = sindyAnnoGroup_city; break;
		case mesh_type::kBase:   emGroup = sindyAnnoGroup_base; break;
		case mesh_type::kMiddle: emGroup = sindyAnnoGroup_middle; break;
		case mesh_type::kTop:    emGroup = sindyAnnoGroup_top; break;
		default: break;
	}
	// ���b�V���R�[�h -> �G���A�ϊ�
	IEnvelopePtr ipEnv( AheMeshCode2Envelope( lMeshCode, emGroup, GetSpatialReference() ) );
	if( ! ipEnv )
		return sindyErr_ArgLimitOver;
	// ����
	return Select( ipEnv, bRecycling, bCreateCache, pErrs );
}

// �`��Ō�������
sindyErrCode CAnnotations::Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/ )
{
	return _SelectByShape( ipGeom, bRecycling, bCreateCache, pErrs, bInit );
}
sindyErrCode CAnnotations::_SelectByShape( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/, IFeatureCursor* ipDispCursor/* = NULL*/, sindyTableType::ECode* pDispTableType/* = NULL*/, bool bCreateIndex/* = true*/ )
{
	double dStart = GetTickCount();
	TRACEMESSAGE( ANNO_START_SEARCH, ( bRecycling ) ? EFFECTIVE_CACHE : NON_EFFECTIVE_CACHE );

	// ������
	if( bInit )
		clear();

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	CGeometry cSearchGeom( ipGeom );
	LOGASSERTEERR_IF( p, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( ! ( bRecycling && ( ! bCreateCache ) ), sindyErr_ArgIsConsistency )
		{
			// ���L�I�u�W�F�N�g�擾
			CSPTableNameString spAnnoNameString( GetNameString() );
			CSPFieldMap spAnnoFieldMap( GetFieldMap() );

			// �W�I���g����NULL�ł������ł��邪�A�����ł͂�����
			// �G���[�Ƃ��邱�Ƃŗ\�����Ȃ��S���������������
			LOGASSERTEERR_IF( cSearchGeom != NULL, sindyErr_ArgIsNull )
			{
				{
					IQueryFilterPtr ipFilter = AheInitSpatialFilter( NULL, ipGeom );
					IFeatureCursorPtr ipCursor;
					
					double dAnnoFetchStart = GetTickCount();	// �^�ʒu�擾�J�n���ԁi�g���[�X�p�j

					// �^�ʒu������
					std::map<long,long> mapAnnoID;	// OID�ƃ��X�g�̃C���f�b�N�X�̑Ή��\
					//std::list<long> listExtendAnnoIDs;	// �I����`���ɂȂ��^�ʒu��ID�ێ��p
					std::map<sindyTableType::ECode,std::list<long> > maplistID; // ����Ȃ��I�u�W�F�N�g�𔻕ʂ���̂Ɏg�p

					// �L���̑傫�����l�����āA
					// �^����ꂽ�����`����ő�L��/2���o�b�t�@�����O���Č�������
					IGeometryPtr ipSearchGeom( cSearchGeom.Buffer( (double)8.0*AheCalcDispLineType21PXLen( *scale_rbegin(), true ) ) );
//					if( g_bBufferedSearch )
//						ipSearchGeom = cSearchGeom.Buffer( (double)8.0*AheCalcDispLineType21PXLen( *scale_rbegin(), true ) );
//					else
//						ipSearchGeom = ipGeom;

					LOGASSERTEERR_IF( ipSearchGeom, sindyErr_GeometryFunctionFailed )
					{
						((ISpatialFilterPtr)ipFilter)->putref_Geometry( ipSearchGeom );
						ipCursor = Search( ipFilter, bRecycling );
					}

					LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
					{
						long lCount = ( bInit ) ? 0 : size( m_emTableType );
						IFeature* ipFeature = NULL;
//						IEnvelopePtr ipAnnoRect( CLSID_Envelope ); // �^�ʒu��`��r�Ŏg�p����̂ł��炩���ߍ쐬���Ă���
//						ipAnnoRect->putref_SpatialReference( GetSpatialReference() ); // ��ԎQ�ƃZ�b�g
						while( S_OK == ipCursor->NextFeature( &ipFeature ) )
						{
							if( ! ipFeature )
								break;

							// OID�ƃ��X�g�̃C���f�b�N�X�Ή��\�쐬
							long lOID = 0;
							ipFeature->get_OID( &lOID );
							mapAnnoID[lOID] = lCount;
							// ID�͌�ő���Ȃ����̂������̂Ɏg���̂Ń��X�g�ɂ��i�[
							maplistID[GetTableType()].push_back( lOID );

							// �o�b�t�@�����O���Ȃ����[�h���ƁA�^�ʒu�̂�
							// �I�������悤�ȏ󋵂̏ꍇ�ɕ\���ʒu��I��
							// ���邱�Ƃ��ł��Ȃ��I�I�Ƃ肠�����b��I��
							// listExtendAnnoIDs�ɓ˂�����ł��ƂŌ������Ă��炤
//							if( ! g_bBufferedSearch )
//								listExtendAnnoIDs.push_back( lOID );

							CSPRelAnnotationParameter spAnno( new CRelAnnotationParameter( ipFeature, m_emTableType, false, spAnnoFieldMap, spAnnoNameString ) );

/*							// �R���e�i�ɒǉ�����O�ɁA�����G���A�ɖ{���Ɋ܂܂�邩�ǂ���
							// �����O�Ɍ������A����Ȃ����̂͂͂���
							// ����́AS1�`S4�S�Ăōs���K�v������ibug 3041�j
							bool bIsContain = false;
							IGeometryPtr ipAnnoPoint( (IPoint*)(spAnno->GetShape()->GetPoint(0)) );
							for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
							{
								// �^�ʒu���o�b�t�@�����O�O�̌����G���A�ɂ��邩�ǂ���
								if( ! cSearchGeom.Disjoint( ipAnnoPoint ) )
								{
									bIsContain = true;
									break;
								}
								// �L���\���^�ʒu���o�b�t�@�����O�O�̌����G���A�ɂȂ��ꍇ�A
								// �L���\������Ȃ��̂Ȃ炢��Ȃ�
								if( ! spAnno->CanShowMark( *itScale ) )
									continue;
								// �^�ʒu��`���o�b�t�@�����O�O�̌����G���A�Ƃ��Ԃ邩�ǂ���
								WKSEnvelope env = {0};
								spAnno->CalcSymbolBounds( spAnno->CalcMarkLength( *itScale, true ), spAnno->CalcMarkLength( *itScale, false ), env );
								ipAnnoRect->PutWKSCoords( &env );
								ipAnnoRect->SnapToSpatialReference();
								if( ! cSearchGeom.Disjoint( (IGeometryPtr)ipAnnoRect ) )
									bIsContain = true;
							}

							// �����G���A���Ɋ܂܂�Ȃ����͎̂̂Ă�
							if( ! bIsContain )
							{
								TRACEMESSAGE(_T("�^�ʒu OID[%d] �����G���A�O�̂��ߔr��\n"), lOID );
								continue;
							}*/

							push_back( spAnno );
							
							ipFeature->Release();
							ipFeature = NULL;

							if( bRecycling )
								spAnno->CreateCache( true, spAnnoFieldMap, spAnnoNameString );
							else if( bCreateCache )
								spAnno->CreateCache( false, spAnnoFieldMap, spAnnoNameString );

							++lCount;
						}
						ipCursor = NULL;
						TRACEMESSAGE(SEARCH_TRUE_POINT, lCount, ((double)GetTickCount()-dAnnoFetchStart)/(double)1000.0);
					}

					// �\���ʒu������
					double dSearchDispTime = 0.0; // �\���ʒu�������ԁi�g���[�X�p�j
					double dFetchDispTime = 0.0;  // �\���ʒu�t�F�b�`���[�v���ԁi�g���[�X�p�j
					double dFetchLoop1Time = 0.0; // �\���ʒu�t�F�b�`���[�v�O�����ԁi�g���[�X�p�j

					LPCTSTR lpcszOwnerName = GetOwnerName();	// �^�ʒu�̃I�[�i�[��

					for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
					{
						// �J�[�\�����w�肳��Ă���A�\���ʒu�e�[�u�������肳��Ă���ꍇ�͂���ȊO���΂�
						if( ipDispCursor && pDispTableType && *itScale != *pDispTableType )
							continue;

						double dStartDispFetch = GetTickCount();	// �\���ʒu�擾�J�n���ԁi�g���[�X�p�j
						double dSearchTable = 0.0;					// �֘A�^�ʒu���������ԁi�g���[�X�p�j
						long lSearchTableCount =0;					// �֘A�^�ʒu�������񐔁i�g���[�X�p�j

						// �\���ʒu���C�����擾�i�I�[�i�[��������ꍇ�̓I�[�i�[��������j
						CString strDispClassName;
						if( 0 < lstrlen(lpcszOwnerName) )
							strDispClassName.Format(_T("%s.%s"), lpcszOwnerName, AheTableType2TableName( *itScale ) );
						else
							strDispClassName = AheTableType2TableName( *itScale );					

						CFeatureClass cDispClass( *itScale, OpenTable( strDispClassName ) );
						LOGASSERTEERR_IF( cDispClass != NULL, sindyErr_TableNotFound )
						{
							double dSearchDispStart = (double)GetTickCount();

							// WARNING �����G���A���ő�X�P�[���t�H���g4���g�傷��[bug 3041]�b��΍�
							// �����A����ł����g����R�s�[���L�ł͏E���Ȃ����̂��o�Ă���͂��Ȃ̂ŁA
							// �����Ƃ��̂ł���ΐ^�ʒu��OBJECTID�Ō�������K�v������
							if( ! ipDispCursor )
							{
								IGeometryPtr ipSearchGeom( cSearchGeom.Buffer( (double)schema::disp_line::font_size::k24 * AheCalcDispLineType21PTLen( *itScale, true ) ) );
//								if( g_bBufferedSearch )
//									ipSearchGeom = cSearchGeom.Buffer( (double)schema::disp_line::font_size::k24 * AheCalcDispLineType21PTLen( *itScale, true ) * (double)4.0 );
//								else
//									ipSearchGeom = ipGeom;
								LOGASSERTEERR_IF( NULL != ipSearchGeom, sindyErr_GeometryFunctionFailed )
								{
									// ����
									((ISpatialFilterPtr)ipFilter)->putref_Geometry( ipSearchGeom );
									ipCursor = cDispClass.Search( ipFilter, bRecycling );
									dSearchDispTime += (double)GetTickCount()-dSearchDispStart; // �\���ʒu���������Ԃ��v�Z
								}
							}
							else
								ipCursor = ipDispCursor;

							// �t�F�b�`
							double dFetchDispStart = GetTickCount();	// �\���ʒu�t�F�b�`���[�v�J�n���ԁi�g���[�X�p�j
							LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
							{
								// ���L�I�u�W�F�N�g�擾
								CSPTableNameString spDispNameString( cDispClass.GetNameString() );
								CSPFieldMap spDispFields( cDispClass.GetFieldMap() );

								// �����ł����������邽�߂ɁA�^�ʒuID�̃C���f�b�N�X�ԍ����擾���Ă���
								long lAnnoIDIndex = spDispFields->FindField( schema::disp_line::kAnnoID );	// �C���f�b�N�X�ԍ�
								VARIANT vaAnnoID;												// �֘AID�ێ��p
								VariantInit( &vaAnnoID );

								SASSERT( lAnnoIDIndex >= 0 );	// �C���f�b�N�X�ԍ���0�ȏ�

								IFeature* ipDisp = NULL;
								while( S_OK == ipCursor->NextFeature( &ipDisp ) )
								{
									double dFetchLoop1Start = GetTickCount();
									if( ipDisp == NULL ) break;

//									// �v��Ȃ��\���ʒu�𗎂Ƃ�

									ipDisp->get_Value( lAnnoIDIndex, &vaAnnoID );
									SASSERT( vaAnnoID.vt == VT_I4 );	// �l�͂���͂�

									long lOID = -1;
									ipDisp->get_OID( &lOID );
									// ��ő���Ȃ����̂������̂Ɏg���̂ŋL�^���Ă���
									maplistID[*itScale].push_back( lOID );
#ifdef _DEBUG
									ATLTRACE(_T("�\���ʒu[%s] OBJECTID[%ld]\n"), spDispNameString->GetTableName(), lOID );
#endif // ifdef _DEBUG

									CRelAnnotationParameter* pAnnoParam = NULL; // �A�N�Z�X���₷�����邽��
									std::map<long,long>::const_iterator itID = mapAnnoID.find( vaAnnoID.lVal );
									if( itID != mapAnnoID.end() )
									{
										
										pAnnoParam = static_cast<CRelAnnotationParameter*>(&*find( m_emTableType )->second[itID->second]);// �����Ă���
										dFetchLoop1Time += GetTickCount()-dFetchLoop1Start;
										pAnnoParam->SetDispLine( *itScale, ipDisp, bRecycling, spDispFields, spDispNameString );
									}
									// ���������^�ʒu�̒��ɂȂ��ꍇ
									else {
										double dSearch = GetTickCount();

										// ���X�g�ɒǉ�
										IFeaturePtr ipFeature( GetFeature( vaAnnoID.lVal ) );
										if( ipFeature )
										{
											CSPRelAnnotationParameter spAnnoParam = CSPRelAnnotationParameter( new CRelAnnotationParameter( ipFeature, m_emTableType, false, spAnnoFieldMap, spAnnoNameString ) );
											pAnnoParam = static_cast<CRelAnnotationParameter*>(&*spAnnoParam);

											if( bRecycling )
												spAnnoParam->CreateCache( true, spAnnoFieldMap, spAnnoNameString );
											else if( bCreateCache )
												spAnnoParam->CreateCache( false, spAnnoFieldMap, spAnnoNameString );
											push_back( spAnnoParam );

											mapAnnoID[vaAnnoID.lVal] = size( m_emTableType ) - 1;
											// ��ő���Ȃ��I�u�W�F�N�g�������̂Ɏg�p����̂Ŋm��
											maplistID[GetTableType()].push_back( vaAnnoID.lVal );

											spAnnoParam->SetDispLine( *itScale, ipDisp, bRecycling, spDispFields, spDispNameString ); // �֘A�t��
										}
										else {
											emErr = sindyErr_RelFeatureNotFound;	// �֘A�t�B�[�`����������Ȃ� [bug 4000]
											if( pErrs )
											{
												CDispLine cDisp( ipDisp );
												pErrs->push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale() ) ) );
											}
											else
												ERRORLOG(emErr);											
										}

										// ��Ŋ֘A�\�����C���t�B�[�`���������ƌ������邽�߂�ID�ێ�
//										listExtendAnnoIDs.push_back( vaAnnoID.lVal );
										
										// �g���[�X�^�C���p
										dSearchTable += GetTickCount()-dSearch;
										++lSearchTableCount;
									}

									ipDisp->Release();
									ipDisp = NULL;

									emErr = sindyErr_NoErr;	// �֘A�t�����Ȃ��������͎̂̂Ă�Ƃ����d�l�i���O�ɂ͒ǉ����Ă���j�Ȃ̂ŁA�G���[�R�[�h�͂����ŃN���A
								}
							}
							dFetchDispTime += (double)GetTickCount()-dFetchDispStart; // �t�F�b�`���[�v���Ԃ��v�Z
						}
						TRACEMESSAGE(SEARCH_DISPPOINT_STATUS, ((double)GetTickCount()-dStartDispFetch)/(double)1000.0, dSearchDispTime/(double)1000.0, dFetchDispTime/(double)1000.0, dFetchLoop1Time/(double)1000.0, g_dTotalSetDispLineTime/(double)1000.0, lSearchTableCount, dSearchTable/(double)1000.0);
					}
					// ���̎��_�ŁA�^�ʒu���痣�ꂽ�ꏊ�ɑ��݂���\���ʒu������Ă��Ȃ��P�[�X�����肤��
					// �R�s�[���L�̏ꍇ�ȂǁA�ǂ̂��炢����Ă��邩���������Ȃ��̂ŁA���ݑI���ł��Ă���^�ʒu
					// ��ID�ŕ\���ʒuID���������A���ꂩ��擾�ς݂̂��̂����������̂�I������
					//
					// �\���ʒuID���擾
					std::list<CString> strIDs;
					IDs2Str( maplistID[GetTableType()], strIDs );
					std::map<sindyTableType::ECode, std::list<long> > maplistID_Exact; // �擾�ł��Ă��Ȃ���΂Ȃ�Ȃ�ID�̃��X�g�i�[�p
					for( std::list<CString>::const_iterator it = strIDs.begin(); it != strIDs.end(); ++it )
					{
						for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
						{
							// �������w�肳��Ă���A�\���ʒu�e�[�u�������肳��Ă���ꍇ�͂���ȊO���΂�
							if( ipDispCursor && pDispTableType && *itScale != *pDispTableType )
								continue;

							_ICursorPtr ipCursor;
							OpenSameOwnerTable( AheTableType2TableName( *itScale ) )->Search( AheInitQueryFilter( NULL, _T("OBJECTID"), _T("%s in (%s)"), schema::disp_line::kAnnoID, *it ), VARIANT_TRUE, &ipCursor );
							_IRowPtr ipRow;
							while( S_OK == ipCursor->NextRow( &ipRow ) )
							{
								long lOID = 0;
								ipRow->get_OID( &lOID );
								maplistID_Exact[*itScale].push_back( lOID );
							}
						}
					}
					// �����ł���ID���X�g������Ɍ����ς݂̃��X�g�����������̂�����Ȃ����X�g
					std::map<sindyTableType::ECode,std::list<long> > maplistID_Query; // �E���Ă��Ȃ���΂Ȃ�Ȃ����X�g
					for( std::map<sindyTableType::ECode,std::list<long> >::iterator it = maplistID_Exact.begin(); it != maplistID_Exact.end(); ++it )
					{
						std::list<long>& listID_Exact = it->second;
						std::list<long>& listID = maplistID[it->first];
						std::list<long>& listID_Query = maplistID_Query[it->first]; // �E���Ă��Ȃ���΂Ȃ�Ȃ�ID
						listID_Exact.sort();
						listID.sort();
						// listID_Exact�Ɋ܂܂�āAlistID�Ɋ܂܂�Ȃ����X�g��listID_Query�ɓ����
						std::set_difference( listID_Exact.begin(), listID_Exact.end(), listID.begin(), listID.end(), std::back_inserter(listID_Query) );
					}
					for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
					{
						// �������w�肳��Ă���A�\���ʒu�e�[�u�������肳��Ă���ꍇ�͂���ȊO���΂�
						if( ipDispCursor && pDispTableType && *itScale != *pDispTableType )
							continue;

						std::list<long>& listID = maplistID_Query[*itScale];
						if( ! listID.empty() )
						{
							CString strDispTableName( AheTableType2TableName(*itScale) );
							CFeatureClass cDispClass( *itScale, OpenSameOwnerTable( strDispTableName ) );
							
							// ���L�I�u�W�F�N�g�擾
							CSPTableNameString spDispNameString( cDispClass.GetNameString() );
							CSPFieldMap spDispFields( cDispClass.GetFieldMap() );
							
							// �����ł����������邽�߂ɁA�^�ʒuID�̃C���f�b�N�X�ԍ����擾���Ă���
							long lAnnoIDIndex = spDispFields->FindField( schema::disp_line::kAnnoID );	// �C���f�b�N�X�ԍ�
							VARIANT vaAnnoID;												// �֘AID�ێ��p

							SASSERT( lAnnoIDIndex >= 0 );	// �C���f�b�N�X�ԍ���0�ȏ�

							VariantInit( &vaAnnoID );

							_ICursorPtr ipCursor( cDispClass.GetRows( listID, bRecycling ) );
							if( ipCursor )
							{
								_IRowPtr ipDisp;
								while( S_OK == ipCursor->NextRow( &ipDisp ) )
								{
									ipDisp->get_Value( lAnnoIDIndex, &vaAnnoID );
									static_cast<CRelAnnotationParameter&>(**find(m_emTableType, vaAnnoID.lVal)).SetDispLine( *itScale, (IFeaturePtr)ipDisp, bRecycling, spDispFields, spDispNameString );
									TRACEMESSAGE(ADD_SELECTED_DISPPOINT, strDispTableName, AheGetOID( ipDisp ) );
								}
							}
						}
					}
/*
					// ���̎��_�ŁA�u������`���ɑ��݂��Ȃ��^�ʒu�v�Ɋ֘A����\���ʒu�t�B�[�`�����s�����Ă���i�s�s�n�}�ȊO�j�̂ŁA�ēx�\���ʒu����������
					if( !listExtendAnnoIDs.empty() )
					{
						for( scale_iterator itScale = scale_begin(); itScale != scale_end(); ++itScale )
						{
							// �������w�肳��Ă���A�\���ʒu�e�[�u�������肳��Ă���ꍇ�͂���ȊO���΂�
							if( ipDispCursor && pDispTableType && *itScale != *pDispTableType )
								continue;

							// �\���ʒu���C�����擾�i�I�[�i�[��������ꍇ�̓I�[�i�[��������j
							CString strDispClassName;
							if( 0 < lstrlen(lpcszOwnerName) )
								strDispClassName.Format(_T("%s.%s"), lpcszOwnerName, AheTableType2TableName( *itScale ) );
							else
								strDispClassName = AheTableType2TableName( *itScale );					
							CFeatureClass cDispClass( *itScale, OpenTable( strDispClassName ) );

							LOGASSERTEERR_IF( cDispClass != NULL, sindyErr_TableNotFound )
							{
								// ���L�I�u�W�F�N�g�擾
								CSPTableNameString spDispNameString( cDispClass.GetNameString() );
								CSPFieldMap spDispFields( cDispClass.GetFieldMap() );

								// �����ł����������邽�߂ɁA�^�ʒuID�̃C���f�b�N�X�ԍ����擾���Ă���
								long lAnnoIDIndex = spDispFields->FindField( schema::disp_line::kAnnoID );	// �C���f�b�N�X�ԍ�
								VARIANT vaAnnoID;												// �֘AID�ێ��p

								SASSERT( lAnnoIDIndex >= 0 );	// �C���f�b�N�X�ԍ���0�ȏ�

								VariantInit( &vaAnnoID );

								// WHERE���IN�̒��g���쐬
								std::list<CString> listStrIDs;
								cDispClass.IDs2Str( listExtendAnnoIDs, listStrIDs );

								for( std::list<CString>::const_iterator itStrIDs = listStrIDs.begin(); itStrIDs != listStrIDs.end(); ++itStrIDs )
								{
									CString strWhere;
									strWhere.Format(_T("%s IN (%s)"), schema::disp_line::kAnnoID ,*itStrIDs );
									IFeatureCursorPtr ipCursor = cDispClass.Search( AheInitQueryFilter( NULL, NULL, strWhere ), bRecycling );

									LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
									{
										IFeature* ipDisp = NULL;
										while( S_OK == ipCursor->NextFeature( &ipDisp ) )
										{
											if( ipDisp == NULL ) break;

											LOGASSERTEERR_IF( SUCCEEDED( ipDisp->get_Value( lAnnoIDIndex, &vaAnnoID ) ), sindyErr_AOGetValueFailed )
											{
												SASSERT( vaAnnoID.vt == VT_I4 );	// �l�͂���͂�

												std::map<long,long>::const_iterator itID = mapAnnoID.find( vaAnnoID.lVal );
												if( mapAnnoID.end() != itID )
												{
													// ���ɓo�^����Ă��Ȃ��ꍇ�̂ݓo�^
													CSPRow spDisp( find( ipDisp ) );
													if( ! spDisp.get() )
														static_cast<CRelAnnotationParameter&>(**find(m_emTableType, vaAnnoID.lVal)).SetDispLine( *itScale, ipDisp, bRecycling, spDispFields, spDispNameString );
												}
												else {
													emErr = sindyErr_RelFeatureNotFound;	// �֘A�t�B�[�`����������Ȃ� [bug 4000]
													if( pErrs )
													{
														CDispLine cDisp( ipDisp );
														pErrs->push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale() ) ) );
													}
													else
														ERRORLOG(emErr);
												}
											}
											ipDisp->Release();
											ipDisp = NULL;
										}
									}
								}
							}
						}
					}
*/
				}
			}
		}
	}

/*#ifdef _DEBUG
	CString strTmp;
	for( CAnnotations::const_iterator it = begin(); it != end(); ++it )
	{
		strTmp.Format(_T("%s OID=%d"), __func__, it->GetDispLine(sindyDisp_city).GetOID() );
		AheTraceGeometry( (IGeometry*)*it->GetDispLine(sindyDisp_city).GetShape(), strTmp );
	}
#endif // _DEBUG //*/
	TRACEMESSAGE(FINISH_SEARCH_ANNO, ((double)GetTickCount()-dStart)/(double)1000.0, g_dTotalCacheTime/(double)1000.0);

	// �p�����[�^�L���b�V�����쐬
	double dStartCalcParameter = GetTickCount();
	TRACEMESSAGE(START_CACHE_CREATION);
	UpdateParameter();
	TRACEMESSAGE(FINISH_CREATE_SEC, ((double)GetTickCount()-dStartCalcParameter)/(double)1000.0 );

	// g_bBufferedSearch��false�̎��͗]�v�Ȃ��̂𗎂Ƃ�
	if( ! g_bBufferedSearch )
	{
		// �S�̂̌`���UNION���쐬���A����ƌ����`����r����
		rows_iterator it = begin(GetTableType());
		while( it != end(GetTableType()) )
		{
			bool bIsContain = false;
			for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
			{
				// �����̌������[�`���� S4�`S1�܂ł𑊎�ɂ��邽�߁A�Ⴆ��e�n��S4�ŕ\�����Ă��Ă����ۂɑ���ɂ��Ă���̂�
				// S3,S2,S1���܂܂��B���̂��߁A�\���㌟���͈͊O�ł����Ă��E���Ă���悤�Ɍ�������̂��o�Ă���ibug 9639�j�B
				// �\����^�Ƃ���ꍇ�͌��݂̕\���X�P�[���݂̂Ŕ��f���ׂ��Ȃ̂����A�����܂ł��̏������������Ă��邱�Ƃ�
				// �ł��Ȃ��̂ŁA�O���[�o���ϐ��Œ�`����Ă����炻�̃X�P�[���Ɍ��肷�邱�Ƃɂ���B
				if( g_emViewTableType != sindyTableType::unknown && g_emViewTableType != *itScale )
					continue;
				IGeometryPtr ipGeom( (*it)->GetShapeCopy( true, false, false, false, AheTableType2AnnotationScale( *itScale ) ) );
				if( ! cSearchGeom.Disjoint( ipGeom ) )
				{
					bIsContain = true;
					break;
				}
			}
			if( ! bIsContain ) {
				TRACEMESSAGE(REMOVE_OUT_OF_AREA, AheTableType2TableName(GetTableType()), (*it)->GetOID() );
				it = erase( GetTableType(), it );
			}
			else
				++it;
		}
	}

	if( bCreateIndex ) // �C���f�b�N�X�쐬�͔C�ӂɁibug �j
	{
		// ��ԃC���f�b�N�X���쐬����
		double dStartCreateIndex = GetTickCount();
		TRACEMESSAGE(START_SPATIAL_CREATION);
		WKSEnvelope cRect = {-1.0};
		cSearchGeom.GetEnvelope()->QueryCoords( &cRect.XMin, &cRect.YMin, &cRect.XMax, &cRect.YMax );

		// ��ԃC���f�b�N�X�̃O���b�h
		double dGrid = 0.0001;
		switch( GetTableType() )
		{
			case sindyTableType::city_annotation:	dGrid = 0.0005; break; 
			case sindyTableType::base_annotation:	dGrid = 0.002;  break; // city ��4�{
			case sindyTableType::middle_annotation:	dGrid = 0.012;  break; // base ��16�{
			case sindyTableType::top_annotation:	dGrid = 0.192;  break; // middle ��16�{
			default: break;
		}
		CreateSpatialIndex( cRect, dGrid );
		TRACEMESSAGE(FINISH_CREATE_SEC, ((double)GetTickCount()-dStartCreateIndex)/(double)1000.0 );
	}

	return emErr;
}

// �`��Ō�������
sindyErrCode CAnnotations::SelectByObject( _IRow* /*ipRow*/, bool/* bRecycling = false*/, bool/* bCreateCache = false*/, bool bInit /*= true*/ )
{
	// ������
	if( bInit == true )
		clear();

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	ERR_RETURN( emErr );
}

// �n���ꂽ�I�u�W�F�N�g�Z�b�g�Ɋ֘A������̂���������
sindyErrCode CAnnotations::Select( _ISet* ipRowSet, bool bCreateCache/* = false*/, CErrorObjects* /* pErrs = NULL*/, bool bInit /*= true*/, bool/* bCreateIndex = true*/ )
{
	// SiNDYLib�Ή��̑����_�C�A���O�ł����g�p����Ă��Ȃ����̂̂͂��Ȃ̂ŁA�ЂƂ܂�
	// ���������Ă����B�����n�֐��̖������S�𖾊m�����Ȃ���������Ȃ����K�v���� by furukawa.
	return sindyErr_NoImplimentation;
#if 0

	// ������
	if( bInit == true )
		clear();

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	// �����`�F�b�N
	if( ! ipRowSet )
	{
		emErr = sindyErr_ArgIsNull;
		return emErr;
	}

	std::list<CRow> listDispLine; // �\���ʒu�ꎟ�i�[�p
	std::map<long,long> mapAnnoID; // �^�ʒu��OBJECTID�ƃR���e�i�ʒu�̑Ή��\

	// ���L�I�u�W�F�N�g�擾
	CSPTableNameString spAnnoNameString( GetNameString() );
	CSPFieldMap spAnnoFieldMap( GetFieldMap() );

	long lCount = 0;
	ipRowSet->get_Count( &lCount );
	ipRowSet->Reset();
	for( long i = 0; i < lCount; ++i )
	{
		// �����̓s����^�ʒu����o�^�����ق����ǂ��̂ŁA�\���ʒu�͕ʂɎ���Ă���
		IUnknownPtr ipUnk;
		ipRowSet->Next( &ipUnk );
		CRow cRow( (_IRowPtr)ipUnk );
		sindyTableType::ECode emTableType = cRow.GetTableType();
		if( AheIsAnnoPointTableType( emTableType ) )
		{
			// �^�ʒu���R���e�i�ɒǉ�
			// �����̂��̂�����ꍇ���l������K�v������̂����A�Ƃ肠�����u���Ă���
			CSPRelAnnotationParameter spAnnoParam( new CRelAnnotationParameter( (_IRow*)cRow, emTableType, false, spAnnoFieldMap, spAnnoNameString ) );
			push_back( spAnnoParam, true ); // ���ɒǉ�����Ă���ꍇ���l��
			if( bCreateCache )
				spAnnoParam->CreateCache( false, spAnnoFieldMap, spAnnoNameString );

			// OID�ƃ��X�g�̃C���f�b�N�X�Ή��\�쐬
			mapAnnoID[cRow.GetOID()] = m_listRelAnno.size() - 1;
		}
		else if( AheIsAnnoLineTableType( cRow.GetTableType() ) )
			listDispLine.push_back( cRow );
	}
	// �\���ʒu�Ɋ֘A����^�ʒu����������
	for( std::list<CRow>::const_iterator it = listDispLine.begin(); it != listDispLine.end(); ++it )
	{
		long lAnnoID = it->GetLongValueByFieldName( schema::disp_line::kAnnoID, -1 );
		// �Ή��\�ɑ��݂��Ȃ���Έ����Ă���
		std::map<long,long>::const_iterator itID = mapAnnoID.find( lAnnoID );
		if( mapAnnoID.end() == itID )
		{
			m_listRelAnno.push_back( CRelAnnotationParameter( GetTableType(), GetFeature(lAnnoID) ) );
			if( bCreateCache )
				m_listRelAnno.rbegin()->CreateCache( false, spAnnoFieldMap, spAnnoNameString );

			// OID�ƃ��X�g�̃C���f�b�N�X�Ή��\�쐬
			mapAnnoID[lAnnoID] = m_listRelAnno.size() - 1;

			itID = mapAnnoID.find( lAnnoID );
		}
		// �\���ʒu�ǉ�
		// ���ɓo�^����Ă��Ȃ��ꍇ�̂ݓo�^ <- �n���ꂽ���̂�D�悷��K�v������񂶂�Ȃ��H�H�i��Œ����āj
		if( ! m_listRelAnno[itID->second].exist( it->GetTableType(), it->GetOID() ) )
			m_listRelAnno[itID->second].SetDispLine( it->GetTableType(), (IFeaturePtr)(_IRow*)*it, false/*, &spDispFields, &spDispNameString*/ );
	}
	// ���ɓo�^����Ă���^�ʒu�Ɋ֘A����\���ʒu���S�ēn����Ă���Ƃ͌���Ȃ����߁A
	// �Ή��\�ɓo�^����ID�Ɋ֘A����\���ʒu����������
	std::list<long> listAnnoID;
	for( std::map<long,long>::const_iterator it = mapAnnoID.begin(); it != mapAnnoID.end(); ++it ) // ID���X�g�쐬
		listAnnoID.push_back( it->first );
	for( scale_iterator itScale = scale_begin(); itScale != scale_end(); ++itScale )
	{
		CFeatureClass cDispClass( *itScale, OpenSameOwnerTable( AheTableType2TableName( *itScale ) ) );
		// ���L�I�u�W�F�N�g�擾
		CSPTableNameString spDispNameString( cDispClass.GetNameString() );
		CSPFieldMap spDispFields( cDispClass.GetFieldMap() );
		LOGASSERTEERR_IF( cDispClass != NULL, sindyErr_TableNotFound )
		{
			_ICursorPtr ipCursor = cDispClass.GetRows( listAnnoID, false, schema::disp_line::kAnnoID );
			if( ipCursor )
			{
				LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
				{
					CRow cRow;
					while( S_OK == ipCursor->NextRow( cRow.Release() ) )
					{
						long lAnnoID = cRow.GetLongValueByFieldName( schema::disp_line::kAnnoID, -1 );
						if( ! m_listRelAnno[mapAnnoID[lAnnoID]].exist( cRow.GetTableType(), cRow.GetOID() ) )
							m_listRelAnno[mapAnnoID[lAnnoID]].SetDispLine( cRow.GetTableType(), (IFeaturePtr)(_IRow*)cRow, false, spDispFields, spDispNameString );
					}
				}
			}
		}
	}
	return emErr;
#endif // if 0
}

// ��ԃC���f�b�N�X���쐬����
void CAnnotations::CreateSpatialIndex( const WKSEnvelope& cIndexRect, double dGrid )
{
	int nDivideX = (int)((cIndexRect.XMax-cIndexRect.XMin)/dGrid);
	nDivideX = ( nDivideX > 0 ) ? nDivideX : 1;
	int nDivideY = (int)((cIndexRect.YMax-cIndexRect.YMin)/dGrid);
	nDivideY = ( nDivideY > 0 ) ? nDivideY : 1;

	CreateSpatialIndex( cIndexRect, nDivideX, nDivideY );
}

// ��ԃC���f�b�N�X���쐬����
void CAnnotations::CreateSpatialIndex( const WKSEnvelope& cIndexRect, int nDivideX, int nDivideY )
{
	// ��ԃC���f�b�N�X��`�쐬
	//
	// �n���ꂽ��`�͒ʏ��SelectByShape�ŗ^����ꂽ��`�ł���A
	// �\��������o�E���f�B���O�{�b�N�X��^�ʒu�L���o�E���f�B���O�{�b�N�X�A
	// �R�s�[���L�Ȃǂ��l������ƁA���R�n���ꂽ��`�ɑ΂��đ傫���Ȃ邱�Ƃ�
	// ����B���̂܂܂ŃC���f�b�N�X���쐬����Ɗi�[�ł��Ȃ����̂������邽�߁A
	// �璷�ł͂��邪�����ōēx��`���`�F�b�N������ŃC���f�b�N�X���쐬����B
	WKSEnvelope cAnnoRect[4], cDispRect[4];
	int nCount = ( GetTableType() == sindyTableType::city_annotation ) ? 1 : 4;
	for( int i = 0; i < nCount; ++i ){ cAnnoRect[i] = cIndexRect; cDispRect[i] = cIndexRect; }
	
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator itAnno = it->second.begin(); itAnno != it->second.end(); ++itAnno )
		{
			// �^�ʒu��`
			const CRelAnnotationParameter& cAnnoParam = static_cast<CRelAnnotationParameter&>(**itAnno);
			int nCount = 0;
			for( CRelAnnotationParameter::annorect_const_iterator itAnnoRect = cAnnoParam.annorect_begin(); itAnnoRect != cAnnoParam.annorect_end(); ++itAnnoRect, ++nCount )
			{
				cAnnoRect[nCount].XMin = min( cAnnoRect[nCount].XMin, itAnnoRect->XMin );
				cAnnoRect[nCount].YMin = min( cAnnoRect[nCount].YMin, itAnnoRect->YMin );
				cAnnoRect[nCount].XMax = max( cAnnoRect[nCount].XMax, itAnnoRect->XMax );
				cAnnoRect[nCount].YMax = max( cAnnoRect[nCount].YMax, itAnnoRect->YMax );
			}

			// �\���ʒu��`
			for( CRelAnnotationParameter::const_iterator it2 = cAnnoParam.begin(); it2 != cAnnoParam.end(); ++it2 )
			{
				for( CRelAnnotationParameter::const_rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
				{
					const CDispLine& cDisp = static_cast<CDispLine&>(**itDisp);
					WKSEnvelope* pDispRectTmp = NULL;
					switch( cDisp.GetTableType() )
					{
						case sindyTableType::city_disp_line:
						case sindyTableType::c_sc1disp_line:
						case sindyTableType::b_sc4disp_line: 
						case sindyTableType::m_sc4disp_line: 
						case sindyTableType::t_sc4disp_line: pDispRectTmp = &cDispRect[0]; break;
						case sindyTableType::b_sc3disp_line: 
						case sindyTableType::m_sc3disp_line: 
						case sindyTableType::t_sc3disp_line: pDispRectTmp = &cDispRect[1]; break;
						case sindyTableType::b_sc2disp_line: 
						case sindyTableType::m_sc2disp_line: 
						case sindyTableType::t_sc2disp_line: pDispRectTmp = &cDispRect[2]; break;
						case sindyTableType::b_sc1disp_line: 
						case sindyTableType::m_sc1disp_line: 
						case sindyTableType::t_sc1disp_line: pDispRectTmp = &cDispRect[3]; break;
						default: ERRORLOG(sindyErr_UnknownTableType); SASSERT(false); break;
					}
					if( pDispRectTmp )
					{
						pDispRectTmp->XMin = min( cDisp.m_cRect.XMin, pDispRectTmp->XMin );
						pDispRectTmp->YMin = min( cDisp.m_cRect.YMin, pDispRectTmp->YMin );
						pDispRectTmp->XMax = max( cDisp.m_cRect.XMax, pDispRectTmp->XMax );
						pDispRectTmp->YMax = max( cDisp.m_cRect.YMax, pDispRectTmp->YMax );
					}
				}
			}
		}
	}

	// ��ԃC���f�b�N�X������
	for( int i = 0; i < nCount; ++i )
	{
		m_cAnnoSpatialIndex[i].Init( cAnnoRect[i], nDivideX, nDivideY );
		m_cDispSpatialIndex[i].Init( cDispRect[i], nDivideX, nDivideY );
	}

	// ��ԃC���f�b�N�X�쐬
	long lAnnoCount = 0;	// ���X�g�̏��Ԃ�����ID�Ƃ���
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator itAnno = it->second.begin(); itAnno != it->second.end(); ++itAnno, ++lAnnoCount )
		{
			const CRelAnnotationParameter& cAnnoParam = static_cast<CRelAnnotationParameter&>(**itAnno);
			// ���L�^�ʒu��ԃC���f�b�N�X�쐬
			int nIndex = 0;
			for( CRelAnnotationParameter::annorect_const_iterator itAnnoRect = cAnnoParam.annorect_begin(); itAnnoRect != cAnnoParam.annorect_end(); ++itAnnoRect, ++nIndex )
				m_cAnnoSpatialIndex[nIndex].Add( *itAnnoRect, lAnnoCount );

			// ���L�\���ʒu��ԃC���f�b�N�X�쐬
			for( CRelAnnotationParameter::const_iterator it2 = (*itAnno)->begin(); it2 != (*itAnno)->end(); ++it2 )
			{
				for( CRelAnnotationParameter::const_rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
				{
					const CDispLine& cDisp = static_cast<CDispLine&>(**itDisp);
					int nIndex = -1;
					switch( cDisp.GetTableType() )
					{
						case sindyTableType::city_disp_line:
						case sindyTableType::c_sc1disp_line:
						case sindyTableType::b_sc4disp_line: 
						case sindyTableType::m_sc4disp_line: 
						case sindyTableType::t_sc4disp_line: nIndex = 0; break;
						case sindyTableType::b_sc3disp_line: 
						case sindyTableType::m_sc3disp_line: 
						case sindyTableType::t_sc3disp_line: nIndex = 1; break;
						case sindyTableType::b_sc2disp_line: 
						case sindyTableType::m_sc2disp_line: 
						case sindyTableType::t_sc2disp_line: nIndex = 2; break;
						case sindyTableType::b_sc1disp_line: 
						case sindyTableType::m_sc1disp_line: 
						case sindyTableType::t_sc1disp_line: nIndex = 3; break;
						default: ERRORLOG(sindyErr_UnknownTableType); SASSERT(false); break;
					}
					m_cDispSpatialIndex[nIndex].Add( cDisp.m_cRect, lAnnoCount );
				}
			}
		}
	}
/*#ifdef _DEBUG
	// �C���f�b�N�X�̒��g���ǂꂮ�炢���邩���ׂ�
	for( int i = 0; i < nCount; ++i )
	{
		m_cAnnoSpatialIndex[i].Trace();
		m_cDispSpatialIndex[i].Trace();
	}
#endif // ifdef _DEBUG*/
}

sindyErrCode CAnnotations::Move( const CGeometry&/* cGeom*/, const std::pair<double,double>&/* pairMove*/, sindyeMoveMode/* emMode*/, CErrorObjects&/* cErrObjects*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
#if 0
	for( const_iterator it = begin(); it != end(); ++it )
	{
		// �^�ʒu
		it->GetShape()->Move( (IGeometry*)cGeom, pairMove, emMode, cErrObjects );

		for( CRelAnnotation::const_iterator itDisp = it->begin(); itDisp != it->end(); ++itDisp )
		{
			// �\���ʒu
			itDisp->GetShape()->
			if( ! cGeom.Disjoint( (IGeometry*)(*itDisp->GetShape()) ) )
				listFeatures.push_back( dynamic_cast<CRow*>(const_cast<CDispLine*>(&*itDisp)) );
		}
	}
#endif // if 0
	return emErr;
}

// �^����ꂽ�`��Ƃ̌��������ŕ����������s��
/*errorcode::sindyErrCode CAnnotations::Split( const CGeometry& cGeom, const CSplitTarget& cSplitTarget, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = NoErr; // �Ԃ�l

	return emErr;
}*/

IGeometryPtr _WKSPoints2Polygon( long lPointCount, WKSPoint* pPoints, ISpatialReference* ipSpRef )
{
	sindyErrCode emErr = sindyErr_NoErr;
	IGeometryCollectionPtr ipRet; // �Ԃ�l

	LOGASSERTERR_IF( SUCCEEDED( ipRet.CreateInstance( CLSID_Polygon ) ), sindyErr_COMCreateInstanceFailed )
	{
		((IGeometryPtr)ipRet)->putref_SpatialReference( ipSpRef );

		for( long i = 0; i < lPointCount/5; ++i )
		{
			IPointCollectionPtr ipCol;
			LOGASSERTERR_IF( SUCCEEDED( ipCol.CreateInstance( CLSID_Ring ) ), sindyErr_COMCreateInstanceFailed )
			{
				((IGeometryPtr)ipCol)->putref_SpatialReference( ipSpRef );
				LOGASSERTERR_IF( SUCCEEDED( ipCol->SetWKSPoints( 5, &pPoints[i*5] ) ), sindyErr_GeometryFunctionFailed )
				{
					LOGASSERTEERR_IF( SUCCEEDED( ipRet->AddGeometry( (IGeometryPtr)ipCol, &vtMissing, &vtMissing ) ), sindyErr_GeometryFunctionFailed )
					{
					}
				}
			}
		}
		LOGASSERTEERR_IF( SUCCEEDED( ((IPolygonPtr)ipRet)->SimplifyPreserveFromTo() ), sindyErr_GeometryFunctionFailed )
		{
		}
	}
	return ipRet;
}

// �������`�F�b�N���s��
errorcode::sindyErrCode CAnnotations::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	sindyErrCode emErr = sindyErr_NoErr;
	long lAnnoCount = 0;						// �Ǘ����X�g�J�E���g

	// �L���ƕ�����|���S���̋����`�F�b�N����臒l�i�s�N�Z���j
	// �����Ӂ�
	// �{���́A���̂悤�ȕ��@�ł͂Ȃ��ACRelAnnotationParameter::CheckLogic��cRule��n���āA�����ł����ƃ��C��������肷�ׂ�
	std::pair<double,double> pairThresholdPx = cRule.GetRelThresholdPx( schema::city_annotation::kTableName, schema::city_disp_line::kTableName );

	IRelationalOperatorPtr ipAnnoRectRelOpe;	// �L����`�Ɗ֘A���Ȃ����L������|���S���Ƃ̏d���`�F�b�N�p
	IEnvelopePtr ipAnnoRect;
	// ��ŉ�����g�p����̂Ő�ɃC���X�^���X������Ă���
	LOGASSERT_IF( SUCCEEDED( ipAnnoRectRelOpe.CreateInstance( CLSID_Envelope ) ), sindyErr_COMCreateInstanceFailed )
	{
		LOGASSERT_IF( SUCCEEDED( ipAnnoRectRelOpe->QueryInterface( IID_IEnvelope, (void**)&ipAnnoRect ) ), sindyErr_COMQueryInterfaceFailed )
		{
			LOGASSERT_IF( SUCCEEDED( ipAnnoRect->putref_SpatialReference( GetSpatialReference() ) ), sindyErr_COMFunctionFailed );
		}
	}

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator itRelAnno = it->second.begin(); itRelAnno != it->second.end(); ++itRelAnno, ++lAnnoCount )
		{
			const CRelAnnotationParameter& cRelAnno = static_cast<const CRelAnnotationParameter&>(**itRelAnno);
			bool bCanOverlap = cRelAnno.CanOverlap();		// �d������������ʂ��ǂ���

			// �^�ʒu�P�̃`�F�b�N�E�\���ʒu�P�̃`�F�b�N�E�^�ʒu���\���ʒu�����`�F�b�N
			emErr = cRelAnno.CheckLogic( cErrObjects, true, true, pairThresholdPx.first, pairThresholdPx.second );

			// ���L�L���d���`�F�b�N
			if( ! bCanOverlap )
			{
				// �X�P�[�����ƂɃ`�F�b�N
				CRelAnnotationParameter::annorect_const_iterator itAnnoRect = cRelAnno.annorect_begin();
				for( int nIndex = 0; nIndex < cRelAnno.GetDispLayerCount(); ++nIndex )
				{
					if( cRelAnno.CanShowMark( cRelAnno.GetDispLayer(nIndex) ) )
					{
						// ��ԃC���f�b�N�X����Y��������̂�I��
						std::list<long> listIDs;
						m_cAnnoSpatialIndex[nIndex].Search( *itAnnoRect, listIDs );
						for( std::list<long>::const_iterator itID = listIDs.begin(); itID != listIDs.end(); ++itID )
						{
							if( lAnnoCount == *itID ) continue;	// �������͔̂�΂�

							const_rows_iterator itRelAnno2(it->second.begin() + *itID);

							if( 0 < (std::list<CRelAnnotationParameter>::difference_type)(itRelAnno2-itRelAnno) )	// itRelAnno ���� itRelAnno2 �����ɂ���ꍇ�ɂ̂݃`�F�b�N
							{
								const CRelAnnotationParameter& cRelAnno2 = static_cast<const CRelAnnotationParameter&>(**itRelAnno2);

								// �L�����m�̏d���`�F�b�N�i�d���s�y�ыL���\���̂��̂̂݁j
								if( ! cRelAnno2.CanOverlap() && cRelAnno2.CanShowMark( cRelAnno2.GetDispLayer(nIndex) ) )
								{
									CRelAnnotationParameter::annorect_const_iterator itAnnoRect2 = cRelAnno2.annorect_begin();
									std::advance( itAnnoRect2, nIndex );
									if( WKSIsCross( *itAnnoRect, *itAnnoRect2 ) )
										cErrObjects.push_back( CErrorObject( cRelAnno, CErrorInfo( sindyErr_AnnoMarkAndMarkIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cRelAnno.GetDispScale(nIndex), _T("%s"), AheTableType2ScaleTypeString( cRelAnno2.GetDispLayer(nIndex) ) ) ) );	// �L�����m���d�Ȃ��Ă���
								}
							}
						}
						// �֘A���Ȃ����L�\��������|���S���Ƃ̏d���`�F�b�N
						LOGASSERTE_IF( NULL != ipAnnoRect, sindyErr_AlgorithmFailed )
						{
							// �L����`���쐬
							LOGASSERT_IF( SUCCEEDED( ipAnnoRect->PutCoords( itAnnoRect->XMin, itAnnoRect->YMin, itAnnoRect->XMax, itAnnoRect->YMax ) ), sindyErr_GeometryFunctionFailed );
						}

						listIDs.clear();
						m_cDispSpatialIndex[nIndex].Search( *itAnnoRect, listIDs );
						for( std::list<long>::const_iterator itID = listIDs.begin(); itID != listIDs.end(); ++itID )
						{
							if( lAnnoCount == *itID ) continue;	// �������͔̂�΂�

							const_rows_iterator itRelAnno2(it->second.begin() + *itID);
							const CRelAnnotationParameter& cRelAnno2 = static_cast<const CRelAnnotationParameter&>(**itRelAnno2);
	//						if( 0 < (std::list<CRelAnnotationParameter>::difference_type)(itRelAnno2-itRelAnno) )	// itRelAnno ���� itRelAnno2 �����ɂ���ꍇ�ɂ̂݃`�F�b�N <- �L����`�ƕ������`�̃e�X�g�Ȃ̂ŁA�d�����邱�Ƃ��Ȃ����߁u���ɂ���ꍇ�v�݂̂ł͂܂����ibug 3767�j
							{
								if( ! cRelAnno2.CanOverlap() )
								{
									for( CRelAnnotationParameter::const_iterator it2 = cRelAnno2.begin(); it2 != cRelAnno2.end(); ++it2 )
									{
										for( CRelAnnotationParameter::const_rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
										{
											const CDispLine& cDisp = static_cast<const CDispLine&>(**itDisp);
											if( cRelAnno.GetDispLayer(nIndex) == cDisp.GetTableType() )	// �����e�[�u���^�C�v���m�̂݃`�F�b�N
											{
												VARIANT_BOOL vbDisjoint = VARIANT_FALSE;
												LOGASSERT_IF( SUCCEEDED( ipAnnoRectRelOpe->Disjoint( cDisp.m_ipGeom, &vbDisjoint ) ), sindyErr_GeometryFunctionFailed )
												{
													if( ! vbDisjoint )
														cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( sindyErr_AnnoMarkAndDispBoundsIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cDisp.GetMaxScale() ) ) );
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

			// �\���ʒu�`�F�b�N
			for( CRelAnnotationParameter::const_iterator it2 = cRelAnno.begin(); it2 != cRelAnno.end(); ++it2 )
			{
				for( CRelAnnotationParameter::const_rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
				{
					const CDispLine& cDisp = static_cast<const CDispLine&>(**itDisp);
					// �\���ʒu�d���`�F�b�N
					if( ! bCanOverlap )
					{
						IRelationalOperatorPtr ipRelOpe( cDisp.m_ipGeom );
						VARIANT_BOOL vbDisjoint = VARIANT_FALSE;			// �d���`�F�b�N�p

						// ��ԃC���f�b�N�X����Y��������̂�I��
						std::list<long> listIDs;
						long lIndex = -1;	// ��ԃC���f�b�N�X�̃C���f�b�N�X�ԍ�
						switch( cDisp.GetTableType() )
						{
							case sindyTableType::city_disp_line:
							case sindyTableType::c_sc1disp_line:
							case sindyTableType::b_sc4disp_line:
							case sindyTableType::m_sc4disp_line:
							case sindyTableType::t_sc4disp_line: lIndex = 0; break;
							case sindyTableType::b_sc3disp_line:
							case sindyTableType::m_sc3disp_line:
							case sindyTableType::t_sc3disp_line: lIndex = 1; break;
							case sindyTableType::b_sc2disp_line:
							case sindyTableType::m_sc2disp_line:
							case sindyTableType::t_sc2disp_line: lIndex = 2; break;
							case sindyTableType::b_sc1disp_line:
							case sindyTableType::m_sc1disp_line:
							case sindyTableType::t_sc1disp_line: lIndex = 3; break;
							default: ERRORLOG(sindyErr_UnknownTableType); SASSERT(false); break;	// ����킯�Ȃ�
						}
						m_cDispSpatialIndex[lIndex].Search( cDisp.m_cRect, listIDs );
						for( std::list<long>::const_iterator itID = listIDs.begin(); itID != listIDs.end(); ++itID )
						{
							if( lAnnoCount == *itID ) continue;	// �������͔̂�΂�

							const_rows_iterator itRelAnno2(it->second.begin() + *itID);
							
							if( 0 < (std::list<CRelAnnotationParameter>::difference_type)(itRelAnno2-itRelAnno) )	// itRelAnno ���� itRelAnno2 �����ɂ���ꍇ�ɂ̂݃`�F�b�N
							{
								const CRelAnnotationParameter& cRelAnno2 = static_cast<const CRelAnnotationParameter&>(**itRelAnno2);
								if( ! cRelAnno2.CanOverlap() )
								{
									for( CRelAnnotationParameter::const_iterator it3 = (*itRelAnno2)->begin(); it3 != (*itRelAnno2)->end(); ++it3 )
									{
										for( CRelAnnotationParameter::const_rows_iterator itDisp2 = it3->second.begin(); itDisp2 != it3->second.end(); ++itDisp2 )
										{
											const CDispLine& cDisp2 = static_cast<const CDispLine&>(**itDisp2);
											if( cDisp.GetTableType() == cDisp2.GetTableType() )	// �����e�[�u���^�C�v���m�̂݃`�F�b�N
											{
												LOGASSERT_IF( SUCCEEDED( ipRelOpe->Disjoint( cDisp2.m_ipGeom, &vbDisjoint ) ), sindyErr_GeometryFunctionFailed )
												{
													if( ! vbDisjoint )
													{
														// ���󒍋L�̕\��������o�E���f�B���O�{�b�N�X�͍\����傫�߂ɂȂ邪�A���̂��߂ɃG���[����ʂɏo�Ă��܂��B�����ŁA
														// �\���^�C�v�����󒍋L�̂��̂̂݌x���Ƃ��A���̏ꍇ�ɌX�̕������`�Ɣ�r���Ė{���ɏd�����Ă��邩�ǂ������`�F�b�N����
														if( ( cDisp.m_emLineType & sindyAnnoLine_Linear ) || ( cDisp2.m_emLineType & sindyAnnoLine_Linear ) )
														{
															// ��r�����쐬
															IRelationalOperatorPtr ipRelOpe2 = ( cDisp.m_emLineType & sindyAnnoLine_Linear  ) ? _WKSPoints2Polygon( cDisp.m_nStringRectsSize  * 5, cDisp.m_pStringRects,  GetSpatialReference() ) : ipRelOpe;
															// ��r����쐬
															IGeometryPtr ipGeomTmp           = ( cDisp2.m_emLineType & sindyAnnoLine_Linear ) ? _WKSPoints2Polygon( cDisp2.m_nStringRectsSize * 5, cDisp2.m_pStringRects, GetSpatialReference() ) : cDisp2.m_ipGeom;

															// ��r
															LOGASSERT_IF( SUCCEEDED( ipRelOpe2->Disjoint( ipGeomTmp, &vbDisjoint ) ), sindyErr_GeometryFunctionFailed )
															{
																if( ! vbDisjoint )
																	cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( sindyErr_AnnoDispBoundsIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cDisp.GetMaxScale() ) ) );
																else
																	cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( sindyErr_AnnoDispBoundsIsOverlaped, sindyErrLevel_WARN, NULL, NULL, cDisp.GetMaxScale() ) ) );
															}
														}
														else
															cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( sindyErr_AnnoDispBoundsIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cDisp.GetMaxScale() ) ) );
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

			// �^�ʒu���\���ʒu�`�F�b�N
			if( sindyErr_NoErr != ( emErr = cRelAnno.CheckPosition() ) )
				cErrObjects.push_back( CErrorObject( cRelAnno, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, NULL, cRelAnno.GetDispScale(0) ) ) );
		}
	}
	return cErrObjects.GetDefaultErrCode();
}

} // sindy
