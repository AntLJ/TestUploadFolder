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
 * @brief <b>ローカルリコンサイル定義ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
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
	LPCTSTR RETRY_START_EDIT_TO_RECONCILE     = _T("リコンサイルするために再度編集を開始しています...\n");
	LPCTSTR FINISH_START_EDIT_TO_RECONCILE    = _T("リコンサイルするための編集開始が終了しました\n");
	LPCTSTR GET_PARENT_VERSION_TO_RECONCILE   = _T("リコンサイルするための親バージョン名を取得しています...\n");
	LPCTSTR PARENT_VERSION_NAME_TO_RECONCILE  = _T("リコンサイルするための親バージョン名：[%s]\n");
	LPCTSTR GIVEUP_RECONCILE_OVER_RETRY_LIMIT = _T("リコンサイルがリトライ回数[%d]を超えたため、リコンサイルをあきらめました。\n");
	LPCTSTR GIVEUP_RECONCILE_PUSHED_CANCEL    = _T("キャンセルボタンが押されましたのでリコンサイルをあきらめました。\n");
	LPCTSTR RECONCILING_TO_VERSION            = _T("バージョン[%s]へリコンサイル中（%ld）...\n");
	LPCTSTR FINISH_RECONCILING_TO_VERSION     = _T("バージョン[%s]へのリコンサイルが終了しました\n");
	LPCTSTR CONFLICT_RECONCILING_TO_VERSION   = _T("バージョン[%s]へのリコンサイルがコンフリクトしました\n");
	LPCTSTR RESTART_EDIT_EDIT_NOT_STARTED     = _T("編集開始されていないと判断されたため、再度編集を開始しています...\n");
	LPCTSTR REFROM_RECONCILE_TO_VERSION       = _T("バージョン[%s]へのリコンサイルからリトライします\n");
	LPCTSTR UNKNOWN_ERROR_ON_RECONCILE        = _T("バージョン[%s]へのリコンサイルで原因不明のエラー[%ld]がでました\n");
	LPCTSTR POSTING_TO_VERSION                = _T("バージョン[%s]へポスト中（%ld）...\n");
	LPCTSTR FINISH_POST_TO_VERSION            = _T("バージョン[%s]へのポストが終了しました\n");
	LPCTSTR CANCEL_DETECTED_PROGRESS_DIALOG   = _T("プログレスダイアログでキャンセルを検出しました\n");
	LPCTSTR UNKNOWN_ERROR_ON_POSTING          = _T("バージョン[%s]へのポスト中に原因不明のエラー[%ld]がでました\n");
	LPCTSTR FINISHING_EDIT_SUCCESS_RECONCILE  = _T("ローカルリコンサイルが正常終了したので編集を終了しています...\n");
	LPCTSTR FINISH_LOCAL_RECONCILE            = _T("ローカルリコンサイルが正常終了しました\n");
	LPCTSTR ERROR_ON_LOCAL_RECONCILE          = _T("ローカルリコンサイルでエラーが検出されました\n");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;
// ローカルリコンサイルを実行する
sindyErrCode LocalReconcile( IWorkspace* ipWorkspace, ITrackCancel* ipTrackCancel, HRESULT& hr, int nRetryMax/* = -1*/, int nRetryWait/* = 0*/ )
{
	// プログレッサーは35-75を使用

	CProgressor cProgress( ipTrackCancel );

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	
	FUNCTRACE(true);

	TRACEMESSAGE( cProgress.GetProgressor(), 35, true, false, _T("LocalReconciling...\n") );

	LOGASSERTEERR_IF( ipWorkspace, sindyErr_COMInterfaceIsNull )
	{
		// 編集終了しているかどうか確認
		IWorkspaceEditPtr ipWorkspaceEdit;
		LOGASSERTERR_IF( SUCCEEDED( hr = ipWorkspace->QueryInterface( IID_IWorkspaceEdit, (void**)&ipWorkspaceEdit ) ), sindyErr_COMQueryInterfaceFailed )
		{
			VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
			LOGASSERTERR_IF( SUCCEEDED( hr = ipWorkspaceEdit->IsBeingEdited( &vbIsBeingEdited ) ), sindyErr_COMFunctionFailed )
			{
				LOGASSERTEERR_IF( ! vbIsBeingEdited, sindyErr_EditAlreadyStarted )
				{
					// リコンサイルするために再度編集開始
					TRACEMESSAGE( cProgress.GetProgressor(), 36, true, false, RETRY_START_EDIT_TO_RECONCILE );
					LOGASSERTERR_IF( SUCCEEDED( hr = ipWorkspaceEdit->StartEditing( bool2VB(true) ) ), sindyErr_EditStartFailed )
					{
						TRACEMESSAGE( cProgress.GetProgressor(), 40, true, false, FINISH_START_EDIT_TO_RECONCILE );

						// 親バージョンのバージョン名取得
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
								// リトライ数がMAXを超えたらあきらめる
								// あとでちゃんとかこうね
								if( 0 < nRetryMax && lCountRetry > nRetryMax )
								{
									TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, GIVEUP_RECONCILE_OVER_RETRY_LIMIT, nRetryMax );
									emErr = sindyErr_EditReconcileFailed;
									TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, _T("LocalReconciling...end[%s].\n"), (sindyErr_NoErr == emErr) ? _T("NoErr") : _T("Err") );
									FUNCTRACE(false);
									return emErr;
								}
								// キャンセルボタンが押されたら、あきらめる（bug 4480）
								if( ! cProgress.Continue() )
								{
									TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, GIVEUP_RECONCILE_PUSHED_CANCEL );
									emErr = sindyErr_EditReconcileFailed;
									TRACEMESSAGE( cProgress.GetProgressor(), 75, true, false, _T("LocalReconciling...end[%s].\n"), (sindyErr_NoErr == emErr) ? _T("NoErr") : _T("Err") );
									FUNCTRACE(false);
									return emErr;
								}

								// リトライ時はウェイトを入れる
								if( 0 < nRetryWait ) Sleep(nRetryWait);

								USES_CONVERSION;
								// ロックをかけない非同期リコンサイルなので、必ず終わるはず
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
										// コンフリクトした場合
										if( vbConflicts )
										{
											TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, CONFLICT_RECONCILING_TO_VERSION, OLE2T(bstrParentVersionName) );
											emErr = sindyErr_EditConflictDetected;
										}
										break;
									case FDO_E_NOT_EDITING:	// [bug 3219] なぜか、リコンサイルを繰り返しているとこのエラーがでることがある
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
									// ポストできるかどうかをチェック
									VARIANT_BOOL vbCanPost = VARIANT_FALSE;
									LOGASSERTERR_IF( SUCCEEDED( hr = ipVersionEdit->CanPost( &vbCanPost ) )&& vbCanPost, sindyErr_EditPostFailed )
									{
										TRACEMESSAGE( cProgress.GetProgressor(), 60, true, false, POSTING_TO_VERSION, OLE2T(bstrParentVersionName), lCountRetry );
										HRESULT hrPost = hr = ipVersionEdit->Post( bstrParentVersionName );
										TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, FINISH_POST_TO_VERSION, OLE2T(bstrParentVersionName) );
										switch( hrPost )
										{
											case FDO_E_VERSION_IS_PROTECTED:	// 親バージョンがプロテクトされているためリコンサイルができない
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, _T("Post : [FDO_E_VERSION_IS_PROTECTED]\n") );
												emErr = sindyErr_FDOVersionIsProtected;
												LOGASSERTEHR(hrReconcile,emErr);
												break;
											case FDO_E_VERSION_RECONCILE_LOST:	// リコンサイルされていないのにポストしようとした
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, _T("Post : [FDO_E_VERSION_RECONCILE_LOST]\n") );
												emErr = sindyErr_FDOVersionReconcileLost;
												LOGASSERTEHR(hrReconcile,emErr);
												break;
											case FDO_E_VERSION_REDEFINED:		// リコンサイル中に他のプロセス・ユーザがポストして親バージョンのステートが動いた
											case FDO_E_SE_VERSION_HAS_MOVED:	// [bug 3179関連] こっちは同じ意味のSDEレベルでのエラーらしい
											{
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, _T("Post : [FDO_E_SE_VERSION_HAS_MOVED]\n") );
												TRACEMESSAGE( cProgress.GetProgressor(), 70, true, false, REFROM_RECONCILE_TO_VERSION, OLE2T(bstrParentVersionName) );
												// キャンセルが押されていなければリトライ
												if( cProgress.Continue() )
													goto RECONCILE;					// この場合は再度リコンサイルを行う
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
											// [Bug3179]上記以外の状態の場合には、おそらくS_FALSEあたりが返ってきていると思われるが
											// 上記4つ以外の時に、保存完了せずに、あたかも保存完了したかのように保存処理を抜けてしまうので、
											// 原因不明のエラーということにしてリコンサイルの時と同様に保存完了させないようにしておきます。by 森
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

						// エラーがなければ編集を終了する
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
