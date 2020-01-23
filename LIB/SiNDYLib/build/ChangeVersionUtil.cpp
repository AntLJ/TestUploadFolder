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
 * @file ChangeVersionUtil.cpp
 * @brief <b>�o�[�W�����؂�ւ����[�e�B���e�B�֐������t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "ChangeVersionUtil.h"
#include "util.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR NEED_TO_DELETE_EMPTY_DATA    = _T("��̃f�[�^�t���[��������ꍇ�͍폜����K�v������܂�");
	LPCTSTR DISPLAY_RELATIONSHIT_SUCCESS = _T("IDisplayRelationshipClass::DisplayRelationshipClass() : ���� [%s]-[%s]\n");
	LPCTSTR FAILED_LAYER_PUTREF_FC       = _T("%s : IFeatureLayer::putref_FeatureClass() : ���s [%s] %s\n");
	LPCTSTR SUCCEEDED_LAYER_PUTREF_FC    = _T("%s : IFeatureLayer::putref_FeatureClass() : ���� [%s]\n");
	LPCTSTR UNKNOWN_SOURCE               = _T("%s : �\�[�X�s�� [%s]\n");
#else
	LPCTSTR NEED_TO_DELETE_EMPTY_DATA    = _T("We need to delete the data frame that is empty.");
	LPCTSTR DISPLAY_RELATIONSHIT_SUCCESS = _T("IDisplayRelationshipClass::DisplayRelationshipClass() : Success [%s]-[%s]\n");
	LPCTSTR FAILED_LAYER_PUTREF_FC       = _T("%s : IFeatureLayer::putref_FeatureClass() : Failed [%s] %s\n");
	LPCTSTR SUCCEEDED_LAYER_PUTREF_FC    = _T("%s : IFeatureLayer::putref_FeatureClass() : Succeeded [%s]\n");
	LPCTSTR UNKNOWN_SOURCE               = _T("%s : an unknown source [%s]\n");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

/*// ���[�N�X�y�[�X���ҏW�����ǂ������`�F�b�N����
bool AheIsBeingEdited( IFeatureWorkspace* ipFeatWorkspace )
{
	return bRet = false;	// �Ԃ�l

	LOGASSERTE_IF( ipFeatWorkspace, sindyErr_ArgIsNull )
	{
		IWorkspaceEditPtr ipWorkspaceEdit;
		LOGASSERTE_IF( SUCCEEDED( ipFeatWorkspace->QueryInterface( IID_IWorkspaceEdit, (void**)&ipWorkspaceEdit ) ), sindyErr_COMQueryIterfaceFailed )
		{
			VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
			LOGASSERTE_IF( SUCCEEDED( ipWorkspaceEdit->IsBeingEdited( &vbIsBeingEdited ) ), sindyErr_COMFunctionFailed )
				bRet = VB2BOOL(vbIsBeingEdited);
		}
	}

	return bRet;
}*/

bool _ChangeRelQueryTable( IDataset* ipDataset, IWorkspace* ipOldWorkspace, IWorkspace* ipNewWorkspace, ICollectionTableVersionChanges* ipColVerChanges )
{
	bool bRet = false;	// �Ԃ�l

	LOGASSERTE_IF( ipDataset && ipOldWorkspace && ipNewWorkspace && ipColVerChanges, sindyErr_ArgIsNull )
	{
		IRelQueryTableManagePtr ipRelQueryTableManage;
		if( SUCCEEDED( ipDataset->QueryInterface( IID_IRelQueryTableManage, (void**)&ipRelQueryTableManage ) ) )
		{
			LOGASSERTE_IF( SUCCEEDED( ipRelQueryTableManage->VersionChanged( (IVersionPtr)ipOldWorkspace, (IVersionPtr)ipNewWorkspace, (IEnumTableVersionChangesPtr)ipColVerChanges ) ) &&
							  SUCCEEDED( ipColVerChanges->Add( (ITablePtr)ipDataset, (ITablePtr)ipDataset ) ), sindyErr_COMFunctionFailed )
				bRet = true;
		}
	}

	return bRet;
}

sindyErrCode _UpdateRelationshipClass( IDisplayRelationshipClass* ipDispRelClass, IRelQueryTable* ipRelQueryTable )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipDispRelClass && ipRelQueryTable, sindyErr_ArgIsNull )
	{
		IRelationshipClassPtr ipRelClass;
		if( SUCCEEDED( ipRelQueryTable->get_RelationshipClass( &ipRelClass ) ) && NULL != ipRelClass )
		{
			esriJoinType emJoinType;
			LOGASSERTEERR_IF( SUCCEEDED( ipDispRelClass->get_JoinType( &emJoinType ) ) &&
							  SUCCEEDED( ipDispRelClass->DisplayRelationshipClass( ipRelClass, emJoinType ) ), sindyErr_COMFunctionFailed )
			{
				CComBSTR bstrName, bstrName2;
				((IDatasetPtr)ipDispRelClass)->get_Name( &bstrName );
				((IDatasetPtr)ipRelClass)->get_Name( &bstrName2 );
				TRACEMESSAGE( DISPLAY_RELATIONSHIT_SUCCESS, CString(bstrName), CString(bstrName2) );
			}
		}	
	}

	return emErr;
}

sindyErrCode _FireChangeDataset( IMapAdmin2* ipMapAdmin, IEnumTableVersionChanges* ipEnumTableVerChanges )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipMapAdmin && ipEnumTableVerChanges, sindyErr_ArgIsNull )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ipEnumTableVerChanges->Reset() ), sindyErr_COMFunctionFailed )
		{
			ITablePtr ipOldTable, ipNewTable;
			while( S_OK == ipEnumTableVerChanges->Next( &ipOldTable, &ipNewTable ) )
			{
				if( NULL == ipOldTable || NULL == ipNewTable ) break;

				IFeatureClassPtr ipDummy;
				if( SUCCEEDED( ipOldTable->QueryInterface( IID_IFeatureClass, (void**)&ipDummy ) ) )
				{
					LOGASSERTEERR_IF( SUCCEEDED( ipMapAdmin->FireChangeFeatureClass( (IFeatureClassPtr)ipOldTable, (IFeatureClassPtr)ipNewTable ) ), sindyErr_COMFunctionFailed );
				}
				else {
					LOGASSERTEERR_IF( SUCCEEDED( ipMapAdmin->FireChangeTable( ipOldTable, ipNewTable ) ), sindyErr_COMFunctionFailed );
				}
			}
		}
	}

	return emErr;
}

// �t�B�[�`�����C���̃o�[�W������ύX����
sindyErrCode _ChangeFeatureLayers( IMap* ipMap, IFeatureWorkspace* ipOldWorkspace, IFeatureWorkspace* ipNewWorkspace, ICollectionTableVersionChanges* ipColVerChanges )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipMap && ipOldWorkspace && ipNewWorkspace && ipColVerChanges, sindyErr_ArgIsNull )
	{
		IEnumLayerPtr ipEnumLayer;
		SUCCEEDED_IF( ipMap->get_Layers( NULL, (VARIANT_BOOL)-1, &ipEnumLayer ), emErr = sindyErr_COMFunctionFailed, NEED_TO_DELETE_EMPTY_DATA )
		{
			LOGASSERTEERR_IF( SUCCEEDED( ipEnumLayer->Reset() ), sindyErr_COMFunctionFailed )
			{
				ILayerPtr ipLayer;
				while( S_OK == ipEnumLayer->Next( &ipLayer ) )
				{
					if( NULL == ipLayer ) break;

					// �ԃr�b�N���������
					VARIANT_BOOL vbValid = VARIANT_FALSE;
					LOGASSERTEERR_IF( SUCCEEDED( ipLayer->get_Valid( &vbValid ) ), sindyErr_COMFunctionFailed )
					{
						if( vbValid )
						{
							IDisplayTablePtr ipDisplayTable;
							if( SUCCEEDED( ipLayer->QueryInterface( IID_IDisplayTable, (void**)&ipDisplayTable ) ) )
							{
								ITablePtr ipTable;
								LOGASSERTEERR_IF( SUCCEEDED( ipDisplayTable->get_DisplayTable( &ipTable ) ), sindyErr_COMFunctionFailed )
								{
									if( _ChangeRelQueryTable( (IDatasetPtr)ipTable, (IWorkspacePtr)ipOldWorkspace, (IWorkspacePtr)ipNewWorkspace, ipColVerChanges ) )
										emErr = _UpdateRelationshipClass( (IDisplayRelationshipClassPtr)ipLayer, (IRelQueryTablePtr)ipTable );
								}
							}

							IFeatureLayerPtr ipFeatLayer;
							if( SUCCEEDED( ipLayer->QueryInterface( IID_IFeatureLayer, (void**)&ipFeatLayer ) ) )
							{
								IFeatureClassPtr ipFeatClass;
								LOGASSERTEERR_IF( SUCCEEDED( ipFeatLayer->get_FeatureClass( &ipFeatClass ) ), sindyErr_COMFunctionFailed )
								{
									IDatasetPtr ipDataset;
									LOGASSERTEERR_IF( SUCCEEDED( ipFeatClass->QueryInterface( IID_IDataset, (void**)&ipDataset ) ), sindyErr_COMQueryInterfaceFailed )
									{
										_ChangeRelQueryTable( ipDataset, (IWorkspacePtr)ipOldWorkspace, (IWorkspacePtr)ipNewWorkspace, ipColVerChanges );

										IWorkspacePtr ipWorkspace;
										LOGASSERTEERR_IF( SUCCEEDED( ipDataset->get_Workspace( &ipWorkspace ) ), sindyErr_COMFunctionFailed )
										{
											if( ipWorkspace == ipOldWorkspace )
											{
												BSTR bstrDatasetName = 0;
												ITablePtr ipTable;
												LOGASSERTEERR_IF( SUCCEEDED( ipDataset->get_Name( &bstrDatasetName ) ) &&
																  SUCCEEDED( ipNewWorkspace->OpenTable( bstrDatasetName, &ipTable ) ), sindyErr_COMFunctionFailed )
												{
													IFeatureClassPtr ipNewFeatureClass;
													if( SUCCEEDED( ipTable->QueryInterface( IID_IFeatureClass, (void**)&ipNewFeatureClass ) ) )
													{
#if 0
														LOGASSERTEERR_IF( SUCCEEDED( ipFeatLayer->putref_FeatureClass( ipNewFeatureClass ) ) , sindyErr_COMFunctionFailed )
														{
#else
														HRESULT hr = 0;
														if( FAILED( hr = ipFeatLayer->putref_FeatureClass( ipNewFeatureClass ) ) )
														{
															CComBSTR bstrName;
															ipFeatLayer->get_Name( &bstrName );
															TRACEMESSAGE( FAILED_LAYER_PUTREF_FC, __func__, CString(bstrName), GETCOMERRORSTRING() );
														}
														else {
															CComBSTR bstrName;
															ipFeatLayer->get_Name( &bstrName );
															TRACEMESSAGE( SUCCEEDED_LAYER_PUTREF_FC, __func__, CString(bstrName) );
#endif // if 0
															LOGASSERTEERR_IF( SUCCEEDED( ipColVerChanges->Add( (ITablePtr)ipDataset, (ITablePtr)ipNewFeatureClass ) ), sindyErr_COMFunctionFailed );
														}
													}
												}
												SysFreeString(bstrDatasetName);
											}
										}
									}
								}
							}
						}
						else {
							BSTR bstrName = 0;
							ipLayer->get_Name( &bstrName );
							TRACEMESSAGE(UNKNOWN_SOURCE, __func__, CString(bstrName) );
							SysFreeString(bstrName);
						}
					}
				}
			}
		}
	}

	return emErr;
}

// �e�[�u���̃o�[�W������ύX����
sindyErrCode _ChangeTables( IStandaloneTableCollection* ipTableCol, IFeatureWorkspace* ipOldWorkspace, IFeatureWorkspace* ipNewWorkspace, ICollectionTableVersionChanges* ipColVerChanges )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipTableCol && ipOldWorkspace && ipNewWorkspace && ipColVerChanges, sindyErr_ArgIsNull )
	{
		long lTableCount = -1;
		LOGASSERTEERR_IF( SUCCEEDED( ipTableCol->get_StandaloneTableCount( &lTableCount ) ), sindyErr_COMFunctionFailed )
		{
			for( long i = 0; i < lTableCount; ++i )
			{
				IStandaloneTablePtr ipStandaloneTable;
				LOGASSERTEERR_IF( SUCCEEDED( ipTableCol->get_StandaloneTable( i, &ipStandaloneTable ) ), sindyErr_COMFunctionFailed )
				{
					IDisplayTablePtr ipDisplayTable;
					if( SUCCEEDED( ipStandaloneTable->QueryInterface( IID_IDisplayTable, (void**)&ipDisplayTable ) ) )
					{
						ITablePtr ipTable;
						LOGASSERTEERR_IF( SUCCEEDED( ipDisplayTable->get_DisplayTable( &ipTable ) ), sindyErr_COMFunctionFailed )
						{
							if( !ipTable )
								continue; // �ҏW�J�n�Ώۃ��[�N�X�y�[�X�ւ̐ڑ��ɁA�o�[�W������Ή��e�[�u��������Ƃ��̌㗎���Ă��܂�

							if( _ChangeRelQueryTable( (IDatasetPtr)ipTable, (IWorkspacePtr)ipOldWorkspace, (IWorkspacePtr)ipNewWorkspace, ipColVerChanges ) )
								emErr = _UpdateRelationshipClass( (IDisplayRelationshipClassPtr)ipStandaloneTable, (IRelQueryTablePtr)ipTable );
						}

						IDatasetPtr ipDataset;
						LOGASSERTEERR_IF( SUCCEEDED( ipTable->QueryInterface( IID_IDataset, (void**)&ipDataset ) ), sindyErr_COMQueryInterfaceFailed )
						{
							_ChangeRelQueryTable( ipDataset, (IWorkspacePtr)ipOldWorkspace, (IWorkspacePtr)ipNewWorkspace, ipColVerChanges );

							IWorkspacePtr ipWorkspace;
							LOGASSERTEERR_IF( SUCCEEDED( ipDataset->get_Workspace( &ipWorkspace ) ), sindyErr_COMFunctionFailed )
							{
								if( ipWorkspace == ipOldWorkspace )
								{
									BSTR bstrDatasetName = 0;
									ITablePtr ipNewTable;
									LOGASSERTEERR_IF( SUCCEEDED( ipDataset->get_Name( &bstrDatasetName ) ) &&
													  SUCCEEDED( ipNewWorkspace->OpenTable( bstrDatasetName, &ipNewTable ) ), sindyErr_COMFunctionFailed )
									{
										LOGASSERTEERR_IF( SUCCEEDED( ipStandaloneTable->putref_Table( ipNewTable ) ) &&
														  SUCCEEDED( ipColVerChanges->Add( (ITablePtr)ipDataset, ipNewTable ) ), sindyErr_COMFunctionFailed );
									}
									SysFreeString(bstrDatasetName);
								}
							}
						}
					}
				}
			}
		}
	}

	return emErr;
}

// �o�[�W������ύX����
sindyErrCode AheChangeVersionByName( IMxDocument* ipMxDoc, IFeatureWorkspace* ipOldWorkspace, LPCTSTR lpcszNewVersionName )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipMxDoc && ipOldWorkspace && lpcszNewVersionName, sindyErr_ArgIsNull )
	{
		IVersionedWorkspacePtr ipVerWorkspace;
		LOGASSERTEERR_IF( SUCCEEDED( ipOldWorkspace->QueryInterface( IID_IVersionedWorkspace, (void**)&ipVerWorkspace ) ), sindyErr_COMQueryInterfaceFailed )
		{
			IVersionPtr ipNewVersion;

			USES_CONVERSION;
			BSTR bstrNewVerName = T2BSTR(lpcszNewVersionName);
			LOGASSERTEERR_IF( SUCCEEDED( ipVerWorkspace->FindVersion( bstrNewVerName, &ipNewVersion ) ), sindyErr_VersionNotFound )
			{
				emErr = AheChangeVersion( ipMxDoc, ipOldWorkspace, (IFeatureWorkspacePtr)ipNewVersion );
			}
			SysFreeString(bstrNewVerName);
		}
	}

	return emErr;
}

// �o�[�W������ύX����
sindyErrCode AheChangeVersion( IMxDocument* ipMxDoc, IFeatureWorkspace* ipOldWorkspace, IFeatureWorkspace* ipNewWorkspace )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

/*	// ������Ǝ��Ԃ�������̂ō����v�ɂ���
	CMouseCursor cCur;
	cCur.SetCursor( CComVariant(0L) ); <- UI�r��*/

	LOGASSERTEERR_IF( ipMxDoc && ipOldWorkspace && ipNewWorkspace, sindyErr_ArgIsNull )
	{
		//ipMxDoc->put_DelayUpdateContents( (VARIANT_BOOL)-1 ); <- �����Ȃ�

		ICollectionTableVersionChangesPtr ipColVerChanges;
		LOGASSERTEERR_IF( SUCCEEDED( ipColVerChanges.CreateInstance( CLSID_EnumTableVersionChanges ) ), sindyErr_COMCreateInstanceFailed )
		{
			IVersionPtr ipNewVersion;
			LOGASSERTEERR_IF( SUCCEEDED( ipNewWorkspace->QueryInterface( IID_IVersion, (void**)&ipNewVersion ) ), sindyErr_COMQueryInterfaceFailed )
			{
				// �V�����o�[�W�������ŐV�̏�ԂɍX�V
				LOGASSERTEERR_IF( SUCCEEDED( ipNewVersion->RefreshVersion() ), sindyErr_COMFunctionFailed );

				IMapsPtr ipMaps;
				long lMapCount = -1;
				LOGASSERTEERR_IF( SUCCEEDED( ipMxDoc->get_Maps( &ipMaps ) ) &&
								  SUCCEEDED( ipMaps->get_Count( &lMapCount ) ), sindyErr_COMFunctionFailed )
				{
					IMapPtr ipMap;
					for( long i = 0; i < lMapCount; ++i )
					{
						LOGASSERTEERR_IF( SUCCEEDED( ipMaps->get_Item( i, &ipMap ) ), sindyErr_COMFunctionFailed )
						{
/*							ipMap->DelayDrawing( (VARIANT_BOOL)-1 );
							ipMap->DelayEvents( (VARIANT_BOOL)-1 ); <- �����Ȃ�*/

							// �Z���N�V�����N���A
							LOGASSERTEERR_IF( SUCCEEDED( ipMap->ClearSelection() ), sindyErr_COMFunctionFailed );

							// �o�[�W�����ύX�C�x���g�ʒm
							IMapAdmin2Ptr ipMapAdmin;
							LOGASSERTEERR_IF( SUCCEEDED( ipMap->QueryInterface( IID_IMapAdmin2, (void**)&ipMapAdmin ) ), sindyErr_COMQueryInterfaceFailed )
							{
								LOGASSERTEERR_IF( SUCCEEDED( ipMapAdmin->FireChangeVersion( (IVersionPtr)ipOldWorkspace, ipNewVersion ) ), sindyErr_COMFunctionFailed );
							}

							// �t�B�[�`�����C���̃o�[�W������ύX
							emErr = _ChangeFeatureLayers( ipMap, ipOldWorkspace, ipNewWorkspace, ipColVerChanges );

							// �e�[�u���̃o�[�W������ύX
							emErr = _ChangeTables( (IStandaloneTableCollectionPtr)ipMap, ipOldWorkspace, ipNewWorkspace, ipColVerChanges );

							// �e�t�B�[�`���N���X�E�e�[�u���̃o�[�W�����ύX�C�x���g��ʒm
							emErr = _FireChangeDataset( (IMapAdmin2Ptr)ipMap, (IEnumTableVersionChangesPtr)ipColVerChanges );

							// �R���N�V�����N���A
							LOGASSERTEERR_IF( SUCCEEDED( ipColVerChanges->RemoveAll() ), sindyErr_COMFunctionFailed );

							// �ĕ`��
							IActiveViewPtr ipView;
							LOGASSERTEERR_IF( SUCCEEDED( ipMap->QueryInterface( IID_IActiveView, (void**)&ipView ) ), sindyErr_COMQueryInterfaceFailed )
							{
								LOGASSERTEERR_IF( SUCCEEDED( ipView->Refresh() ), sindyErr_COMFunctionFailed );
							}

/*							ipMap->DelayEvents( VARIANT_FALSE );
							ipMap->DelayDrawing( VARIANT_FALSE );*/
						}
					}
				}
			}
		}
		// TOC�ȂǍĕ`��
		LOGASSERTEERR_IF( SUCCEEDED( ipMxDoc->UpdateContents() ), sindyErr_COMFunctionFailed );

		//ipMxDoc->put_DelayUpdateContents( VARIANT_FALSE );
	}

	return emErr;
}

// �n���ꂽ�o�[�W�����̐e�o�[�W�������擾����
IVersionPtr AheGetParentVersion( IVersion* ipVersion )
{
	IVersionPtr ipParent;	// �Ԃ�l

	LOGASSERTE_IF( ipVersion, sindyErr_COMInterfaceIsNull )
	{
		VARIANT_BOOL vbHasParent = VARIANT_FALSE;
		LOGASSERT_IF( SUCCEEDED( ipVersion->HasParent( &vbHasParent ) ), sindyErr_COMFunctionFailed )
		{
			IVersionInfoPtr ipVersionInfo;
			LOGASSERT_IF( SUCCEEDED( ipVersion->get_VersionInfo( &ipVersionInfo ) ), sindyErr_COMFunctionFailed )
			{
				IVersionInfoPtr ipParentVersionInfo;
				LOGASSERT_IF( SUCCEEDED( ipVersionInfo->get_Parent( &ipParentVersionInfo ) ), sindyErr_COMFunctionFailed )
				{
					// �e�o�[�W���������擾
					BSTR bstrName = 0;
					LOGASSERT_IF( SUCCEEDED( ipParentVersionInfo->get_VersionName( &bstrName ) ), sindyErr_COMFunctionFailed )
					{
						// ���o�[�W�����̐ڑ��v���p�e�B�擾
						IWorkspacePtr ipWorkspace;
						LOGASSERT_IF( SUCCEEDED( ipVersion->QueryInterface( IID_IWorkspace, (void**)&ipWorkspace ) ), sindyErr_COMQueryInterfaceFailed )
						{
							IPropertySetPtr ipProp;
							LOGASSERT_IF( SUCCEEDED( ipWorkspace->get_ConnectionProperties( &ipProp ) ), sindyErr_COMFunctionFailed )
							{
								VARIANT va;
								VariantInit(&va);
								va.vt = VT_BSTR;
								va.bstrVal = bstrName;
								bstrName = NULL;
								LOGASSERT_IF( SUCCEEDED( ipProp->SetProperty( L"VERSION", va ) ), sindyErr_COMFunctionFailed )
								{
									IWorkspaceFactoryPtr ipFact;
//									LOGASSERT_IF( SUCCEEDED( ipWorkspace->get_WorkspaceFactory( &ipFact ) ), sindyErr_COMFunctionFailed )
									LOGASSERT_IF( SUCCEEDED( ipFact.CreateInstance( CLSID_SdeWorkspaceFactory ) ), sindyErr_COMCreateInstanceFailed )
									{
										IWorkspacePtr ipParentWorkspace;
										LOGASSERT_IF( SUCCEEDED( ipFact->Open( ipProp, NULL, &ipParentWorkspace ) ), sindyErr_COMFunctionFailed )
										{
											LOGASSERT_IF( SUCCEEDED( ipParentWorkspace->QueryInterface( IID_IVersion, (void**)&ipParent ) ), sindyErr_COMQueryInterfaceFailed );
										}
									}
								}
								VariantClear(&va);
							}
						}
						SysFreeString(bstrName);
					}
				}
			}
		}
	}

	return ipParent;
}

} // sindy		
