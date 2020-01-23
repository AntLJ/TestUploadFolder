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
 * @file LocalReconcile.h
 * @brief <b>���[�J�����R���T�C����`�t�@�C��</b>\n
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "LocalReconcile.h"
#include "ChangeVersionUtil.h"
#include "util.h"
#include "Progressor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR RETRY_START_EDIT_TO_RECONCILE     = _T("Retrying to start editing for reconciliation ...\n");
	LPCTSTR FINISH_START_EDIT_TO_RECONCILE    = _T("Finished starting editing for reconciliation\n");
	LPCTSTR GET_PARENT_VERSION_TO_RECONCILE   = _T("Retrieving the parent version name for reconciliation ...\n");
	LPCTSTR PARENT_VERSION_NAME_TO_RECONCILE  = _T("The parent version name for reconciliation:[%s]\n");
	LPCTSTR GIVEUP_RECONCILE_OVER_RETRY_LIMIT = _T("Give up Reconcile process because it was beyond re-try number of times [%d].\n");
	LPCTSTR GIVEUP_RECONCILE_PUSHED_CANCEL    = _T("Give up Reconcile process because cancel button is pushed.\n");
	LPCTSTR RECONCILING_TO_VERSION            = _T("Reconciling to the version [%s] (%ld) ...\n");
	LPCTSTR FINISH_RECONCILING_TO_VERSION     = _T("Finished reconciling to the version[%s]\n");
	LPCTSTR CONFLICT_RECONCILING_TO_VERSION   = _T("The reconcile to the version [%s] is conflicting\n");
	LPCTSTR RESTART_EDIT_EDIT_NOT_STARTED     = _T("Restart editing because it was judged that editing is not started...\n");
	LPCTSTR REFROM_RECONCILE_TO_VERSION       = _T("Re-from reconcile to the version [%s]\n");
	LPCTSTR UNKNOWN_ERROR_ON_RECONCILE        = _T("During reconcile to the version [%s], an unknown error[%ld] occurred\n");
	LPCTSTR POSTING_TO_VERSION                = _T("Posting to version[%s] (%ld)...\n");
	LPCTSTR FINISH_POST_TO_VERSION            = _T("Finish posting to version[%s]\n");
	LPCTSTR CANCEL_DETECTED_PROGRESS_DIALOG   = _T("Cancel has been detected on progress dialog\n");
	LPCTSTR UNKNOWN_ERROR_ON_POSTING          = _T("During the post to the version [%s], an unknown error[%ld] occurred\n");
	LPCTSTR FINISHING_EDIT_SUCCESS_RECONCILE  = _T("Finishing editing because the local reconcilehas been successfully completed...\n");
	LPCTSTR FINISH_LOCAL_RECONCILE            = _T("The local reconcilehas been successfully completed\n");
	LPCTSTR ERROR_ON_LOCAL_RECONCILE          = _T("The error has been detected between the local reconcile operation\n");
#else
	LPCTSTR RETRY_START_EDIT_TO_RECONCILE     = _T("���R���T�C�����邽�߂ɍēx�ҏW���J�n���Ă��܂�...\n");
	LPCTSTR FINISH_START_EDIT_TO_RECONCILE    = _T("���R���T�C�����邽�߂̕ҏW�J�n���I�����܂���\n");
	LPCTSTR GET_PARENT_VERSION_TO_RECONCILE   = _T("���R���T�C�����邽�߂̐e�o�[�W���������擾���Ă��܂�...\n");
	LPCTSTR PARENT_VERSION_NAME_TO_RECONCILE  = _T("���R���T�C�����邽�߂̐e�o�[�W�������F[%s]\n");
	LPCTSTR GIVEUP_RECONCILE_OVER_RETRY_LIMIT = _T("���R���T�C�������g���C��[%d]�𒴂������߁A���R���T�C����������߂܂����B\n");
	LPCTSTR GIVEUP_RECONCILE_PUSHED_CANCEL    = _T("�L�����Z���{�^����������܂����̂Ń��R���T�C����������߂܂����B\n");
	LPCTSTR RECONCILING_TO_VERSION            = _T("�o�[�W����[%s]�փ��R���T�C�����i%ld�j...\n");
	LPCTSTR FINISH_RECONCILING_TO_VERSION     = _T("�o�[�W����[%s]�ւ̃��R���T�C�����I�����܂���\n");
	LPCTSTR CONFLICT_RECONCILING_TO_VERSION   = _T("�o�[�W����[%s]�ւ̃��R���T�C�����R���t���N�g���܂���\n");
	LPCTSTR RESTART_EDIT_EDIT_NOT_STARTED     = _T("�ҏW�J�n����Ă��Ȃ��Ɣ��f���ꂽ���߁A�ēx�ҏW���J�n���Ă��܂�...\n");
	LPCTSTR REFROM_RECONCILE_TO_VERSION       = _T("�o�[�W����[%s]�ւ̃��R���T�C�����烊�g���C���܂�\n");
	LPCTSTR UNKNOWN_ERROR_ON_RECONCILE        = _T("�o�[�W����[%s]�ւ̃��R���T�C���Ō����s���̃G���[[%ld]���ł܂���\n");
	LPCTSTR POSTING_TO_VERSION                = _T("�o�[�W����[%s]�փ|�X�g���i%ld�j...\n");
	LPCTSTR FINISH_POST_TO_VERSION            = _T("�o�[�W����[%s]�ւ̃|�X�g���I�����܂���\n");
	LPCTSTR CANCEL_DETECTED_PROGRESS_DIALOG   = _T("�v���O���X�_�C�A���O�ŃL�����Z�������o���܂���\n");
	LPCTSTR UNKNOWN_ERROR_ON_POSTING          = _T("�o�[�W����[%s]�ւ̃|�X�g���Ɍ����s���̃G���[[%ld]���ł܂���\n");
	LPCTSTR FINISHING_EDIT_SUCCESS_RECONCILE  = _T("���[�J�����R���T�C��������I�������̂ŕҏW���I�����Ă��܂�...\n");
	LPCTSTR FINISH_LOCAL_RECONCILE            = _T("���[�J�����R���T�C��������I�����܂���\n");
	LPCTSTR ERROR_ON_LOCAL_RECONCILE          = _T("���[�J�����R���T�C���ŃG���[�����o����܂���\n");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;
// ���[�J�����R���T�C�������s����
sindyErrCode LocalReconcile( IWorkspace* ipWorkspace, ITrackCancel* ipTrackCancel, HRESULT& hr, int nRetryMax/* = -1*/, int nRetryWait/* = 0*/ )
{
	// �v���O���b�T�[��35-75���g�p

	CProgressor cProgress( ipTrackCancel );

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	
	FUNCTRACE(true);

	TRACEMESSAGE( cProgress.GetProgressor(), 35, true, false, _T("LocalReconciling...\n") );

	LOGASSERTEERR_IF( ipWorkspace, sindyErr_COMInterfaceIsNull )
	{
		// �ҏW�I�����Ă��邩�ǂ����m�F
		IWorkspaceEditPtr ipWorkspaceEdit;
		LOGASSERTERR_IF( SUCCEEDED( hr = ipWorkspace->QueryInterface( IID_IWorkspaceEdit, (void**)&ipWorkspaceEdit ) ), sindyErr_COMQueryInterfaceFailed )
		{
			VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
			LOGASSERTERR_IF( SUCCEEDED( hr = ipWorkspaceEdit->IsBeingEdited( &vbIsBeingEdited ) ), sindyErr_COMFunctionFailed )
			{
				LOGASSERTEERR_IF( ! vbIsBeingEdited, sindyErr_EditAlreadyStarted )
				{
					// ���R���T�C�����邽�߂ɍēx�ҏW�J�n
					TRACEMESSAGE( cProgress.GetProgressor(), 36, true, false, RETRY_START_EDIT_TO_RECONCILE );
					LOGASSERTERR_IF( SUCCEEDED( hr = ipWorkspaceEdit->StartEditing( bool2VB(true) ) ), sindyErr_EditStartFailed )
					{
						TRACEMESSAGE( cProgress.GetProgressor(), 40, true, false, FINISH_START_EDIT_TO_RECONCILE );

						// �e�o�[�W�����̃o�[�W�������擾
						TRACEMESSAGE( cProgress.GetProgressor(), 40, true, false, GET_PARENT_VERSION_TO_RECONCILE );
						BSTR bstrParentVersionName = 0;
						IVersionPtr ipVersion = AheGetParentVersion( (IVersionPtr)ipWorkspaceEdit );
						LOGASSERTEERR_IF( NULL != ipVersion, sindyErr_EditReconcileFailed )
						{
							LOGASSERTERR_IF( SUCCEEDED( hr = ipVersion->get_VersionName( &bstrParentVersionName ) ), sindyErr_COMFunctionFailed );
						}

						USES_CONVERSION;

						TRACEMESSAGE( cProgress.GetProgressor(), 41, true, false, PARENT_VERSION_NAME_TO_RECONCILE, OLE2T(bstrParentVersionName) );
						LOGASSERTEERR_IF( bstrParentVersionName, sindyErr_EditReconcileFailed )
						{
							IVersionEdit2Ptr ipVersionEdit;
							LOGASSERTERR_IF( SUCCEEDED( hr = ipWorkspaceEdit->QueryInterface( IID_IVersionEdit2, (void**)&ipVersionEdit ) ), sindyErr_COMQueryInterfaceFailed )
							{
								long lCountRetry = 0;
RECONCILE:
								// ���g���C����MAX�𒴂����炠����߂�
								// ���Ƃł����Ƃ�������
								if( 0 < nRetryMax && lCountRetry > nRetryMax )
								{
									TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, GIVEUP_RECONCILE_OVER_RETRY_LIMIT, nRetryMax );
									emErr = sindyErr_EditReconcileFailed;
									TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, _T("LocalReconciling...end[%s].\n"), (sindyErr_NoErr == emErr) ? _T("NoErr") : _T("Err") );
									FUNCTRACE(false);
									return emErr;
								}
								// �L�����Z���{�^���������ꂽ��A������߂�ibug 4480�j
								if( ! cProgress.Continue() )
								{
									TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, GIVEUP_RECONCILE_PUSHED_CANCEL );
									emErr = sindyErr_EditReconcileFailed;
									TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, _T("LocalReconciling...end[%s].\n"), (sindyErr_NoErr == emErr) ? _T("NoErr") : _T("Err") );
									FUNCTRACE(false);
									return emErr;
								}

								// ���g���C���̓E�F�C�g������
								if( 0 < nRetryWait ) Sleep(nRetryWait);

								USES_CONVERSION;
								// ���b�N�������Ȃ��񓯊����R���T�C���Ȃ̂ŁA�K���I���͂�
								VARIANT_BOOL vbConflicts = VARIANT_FALSE;
								TRACEMESSAGE( cProgress.GetProgressor(), 45, true, false, RECONCILING_TO_VERSION, OLE2T(bstrParentVersionName), ++lCountRetry );
								HRESULT hrReconcile = hr = ipVersionEdit->Reconcile2( bstrParentVersionName, VARIANT_FALSE, &vbConflicts );
								TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, FINISH_RECONCILING_TO_VERSION, OLE2T(bstrParentVersionName) );
								switch( hrReconcile )
								{
									case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE:
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, _T("LocalReconcile : [FDO_E_RECONCILE_VERSION_NOT_AVAILABLE]\n") );
										emErr = sindyErr_FDOReconcileVersionNotAvailable;
										LOGASSERTEHR(hrReconcile,emErr);
										break;
									case FDO_E_VERSION_BEING_EDITED:
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, _T("LocalReconcile : [FDO_E_VERSION_BEING_EDITED]\n") );
										emErr = sindyErr_FDOVersionBeingEdited;
										LOGASSERTEHR(hrReconcile,emErr);
										break;
									case FDO_E_VERSION_NOT_FOUND:
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, _T("LocalReconcile : [FDO_E_VERSION_NOT_FOUND]\n") );
										emErr = sindyErr_FDOVersionNotFound;
										LOGASSERTEHR(hrReconcile,emErr);
										break;
									case S_OK:
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, FINISH_RECONCILING_TO_VERSION, OLE2T(bstrParentVersionName) );
										// �R���t���N�g�����ꍇ
										if( vbConflicts )
										{
											TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, CONFLICT_RECONCILING_TO_VERSION, OLE2T(bstrParentVersionName) );
											emErr = sindyErr_EditConflictDetected;
										}
										break;
									case FDO_E_NOT_EDITING:	// [bug 3219] �Ȃ����A���R���T�C�����J��Ԃ��Ă���Ƃ��̃G���[���ł邱�Ƃ�����
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, _T("LocalReconcile : [FDO_E_NOT_EDITING]\n") );
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, RESTART_EDIT_EDIT_NOT_STARTED );
										LOGASSERTERR_IF( SUCCEEDED( ipWorkspaceEdit->StartEditing( bool2VB(true) ) ), sindyErr_EditStartFailed )
										{
											TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, FINISH_START_EDIT_TO_RECONCILE );
										}
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, REFROM_RECONCILE_TO_VERSION, OLE2T(bstrParentVersionName) );
										goto RECONCILE;
										break;
									default:
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, UNKNOWN_ERROR_ON_RECONCILE, OLE2T(bstrParentVersionName), hrReconcile );
										emErr = sindyErr_EditReconcileFailed;
										LOGASSERTEHR(hrReconcile,emErr);
										break;
								}

								if( sindyErr_NoErr == emErr )
								{
									// �|�X�g�ł��邩�ǂ������`�F�b�N
									VARIANT_BOOL vbCanPost = VARIANT_FALSE;
									LOGASSERTERR_IF( SUCCEEDED( hr = ipVersionEdit->CanPost( &vbCanPost ) )&& vbCanPost, sindyErr_EditPostFailed )
									{
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, POSTING_TO_VERSION, OLE2T(bstrParentVersionName), lCountRetry );
										HRESULT hrPost = hr = ipVersionEdit->Post( bstrParentVersionName );
										TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, FINISH_POST_TO_VERSION, OLE2T(bstrParentVersionName) );
										switch( hrPost )
										{
											case FDO_E_VERSION_IS_PROTECTED:	// �e�o�[�W�������v���e�N�g����Ă��邽�߃��R���T�C�����ł��Ȃ�
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, _T("Post : [FDO_E_VERSION_IS_PROTECTED]\n") );
												emErr = sindyErr_FDOVersionIsProtected;
												LOGASSERTEHR(hrReconcile,emErr);
												break;
											case FDO_E_VERSION_RECONCILE_LOST:	// ���R���T�C������Ă��Ȃ��̂Ƀ|�X�g���悤�Ƃ���
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, _T("Post : [FDO_E_VERSION_RECONCILE_LOST]\n") );
												emErr = sindyErr_FDOVersionReconcileLost;
												LOGASSERTEHR(hrReconcile,emErr);
												break;
											case FDO_E_VERSION_REDEFINED:		// ���R���T�C�����ɑ��̃v���Z�X�E���[�U���|�X�g���Đe�o�[�W�����̃X�e�[�g��������
											case FDO_E_SE_VERSION_HAS_MOVED:	// [bug 3179�֘A] �������͓����Ӗ���SDE���x���ł̃G���[�炵��
											{
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, _T("Post : [FDO_E_SE_VERSION_HAS_MOVED]\n") );
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, REFROM_RECONCILE_TO_VERSION, OLE2T(bstrParentVersionName) );
												// �L�����Z����������Ă��Ȃ���΃��g���C
												if( cProgress.Continue() )
													goto RECONCILE;					// ���̏ꍇ�͍ēx���R���T�C�����s��
												else
													TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, CANCEL_DETECTED_PROGRESS_DIALOG );
												break;
											}
											case FDO_E_SE_STATE_INUSE: // [bug 3211]
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, _T("Post : [FDO_E_SE_STATE_INUSE]\n") );
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, REFROM_RECONCILE_TO_VERSION, OLE2T(bstrParentVersionName) );
												goto RECONCILE;
												break;
											case S_OK:
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, FINISH_POST_TO_VERSION, OLE2T(bstrParentVersionName) );
												break;
											// [Bug3179]��L�ȊO�̏�Ԃ̏ꍇ�ɂ́A�����炭S_FALSE�����肪�Ԃ��Ă��Ă���Ǝv���邪
											// ��L4�ȊO�̎��ɁA�ۑ����������ɁA���������ۑ������������̂悤�ɕۑ������𔲂��Ă��܂��̂ŁA
											// �����s���̃G���[�Ƃ������Ƃɂ��ă��R���T�C���̎��Ɠ��l�ɕۑ����������Ȃ��悤�ɂ��Ă����܂��Bby �X
											default:
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, UNKNOWN_ERROR_ON_POSTING, OLE2T(bstrParentVersionName), hrPost );
												emErr = sindyErr_EditPostFailed;
												LOGASSERTEHR(hrReconcile,emErr);
												break;
										}
									}
								}
							}
						}
						SysFreeString(bstrParentVersionName);

						// �G���[���Ȃ���ΕҏW���I������
						if( sindyErr_NoErr == emErr )
						{
							TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, FINISHING_EDIT_SUCCESS_RECONCILE );
							LOGASSERTERR_IF( SUCCEEDED( hr = ipWorkspaceEdit->StopEditing( bool2VB(true) ) ), sindyErr_EditStopFailed )
							{
								TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, FINISH_LOCAL_RECONCILE );
							}
						}
						else {
							TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, ERROR_ON_LOCAL_RECONCILE );
						}
					}
				}
			}
		}
	}

	TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, _T("LocalReconciling...end[%s].\n"), (sindyErr_NoErr == emErr) ? _T("NoErr") : _T("Err") );
	FUNCTRACE(false);

	return emErr;
}

} // sindy
