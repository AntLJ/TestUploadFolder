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
* @file AheGlobalsX2.cpp
*
* @brief Arc Helper グローバル関数実装ファイル
*/
#include "stdafx.h"
#include "AheGlobalsX2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR ERROR_FAIL_ACQUIRE_LAYER = _T("レイヤが取得できませんでした");
#else
	LPCTSTR ERROR_FAIL_ACQUIRE_LAYER = _T("Failed to acquire the layer");
#endif // SINDY_FOR_ENGLISH
}

BOOL AheGetLayerFromFile( LPCTSTR lpszLayerFile, ILayer** ipRetLayer )
{
	try {
		IGxLayerPtr ipGxLayer( CLSID_GxLayer );
		IGxFilePtr ipGxFile( ipGxLayer );
		ILayerPtr ipLayer = NULL;

		USES_CONVERSION;

		if ( ipGxFile == NULL || ipGxLayer == NULL )
			return FALSE;

		ipGxFile->put_Path( CComBSTR(lpszLayerFile) );

		ipGxLayer->get_Layer( &ipLayer );	// IGxLayer -> ILayer 変換

		// 変換失敗？
		if( ipLayer != NULL ) {
			(*ipRetLayer) = ipLayer;
			(*ipRetLayer)->AddRef();
		} else {
			CString strTitle;
			strTitle.Format(_T("CArcHelperBase::GetLayerFromFile(%s)"), lpszLayerFile );
			::MessageBox( NULL, ERROR_FAIL_ACQUIRE_LAYER, strTitle, MB_OK );
			return FALSE;
		}

		return TRUE;
	} catch(...) {
	}

	return FALSE;
}

ILayerPtr AheGetLayerFromLyrFile( LPCTSTR lpcszFilePath )
{
	if ( lpcszFilePath == NULL || _tcslen(lpcszFilePath) <= 0 )
		return NULL;

	try {
		HRESULT	hr = S_OK;
		IStoragePtr ipStorage;

		if ( SUCCEEDED( hr = StgOpenStorage( CComBSTR(lpcszFilePath), NULL, STGM_DIRECT_SWMR | STGM_READ | STGM_SHARE_EXCLUSIVE, NULL, 0, &ipStorage ) ) ) {
			// 内部ストレージのリスト取得
			IEnumSTATSTGPtr ipEnumStg;

			if ( SUCCEEDED( hr = ipStorage->EnumElements( 0, NULL, 0, &ipEnumStg ) ) ) {
				STATSTG Stat;

				ZeroMemory( &Stat, sizeof(STATSTG) );

				while ( SUCCEEDED( hr = ipEnumStg->Next( 1, &Stat, NULL ) ) && hr == S_OK ) {
					switch ( Stat.type ) {
						// ストレージ
						case 1: break;
						// ストリーム
						case 2:
							{
								// レイヤファイルからロード
								IStreamPtr ipStream;

								if ( SUCCEEDED( hr = ipStorage->OpenStream( Stat.pwcsName, NULL, STGM_DIRECT_SWMR | STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &ipStream ) ) ) {
									LARGE_INTEGER zeroPos = {0, 0};
									ULARGE_INTEGER StreamSize = {0, 0};

									// 先頭にレイヤのCLSIDが入っているので、読込位置をsizeof(CLSID)分移動する。
									zeroPos.LowPart = sizeof(CLSID);
									if ( SUCCEEDED( hr = ipStream->Seek(zeroPos, STREAM_SEEK_SET, &StreamSize) ) ) {
										long n = 0;
										ULONG ul = 0;
		
										// ?
										if ( SUCCEEDED( hr = ipStream->Read( &n, sizeof(long), &ul ) ) ) {
											IObjectStreamPtr	ipObjectStream( CLSID_ObjectStream );

											// ロード
											if ( SUCCEEDED( hr = ipObjectStream->putref_Stream( ipStream ) ) ) {
												ILayerPtr			ipLayer( CLSID_FeatureLayer );
												IPersistStreamPtr	ipPersistStream( ipLayer );

												if ( SUCCEEDED( hr = ipPersistStream->Load( ipObjectStream ) ) ) {
													CoTaskMemFree(Stat.pwcsName);
													return ipLayer;
												}
											}
										}
									}
								}
							}
							break;
						default: break;
					}

					// バッファ確保しなおし
					CoTaskMemFree(Stat.pwcsName);
				}
			}
		}
	} catch(...) {
		OutputDebugString( _T("AheGetLayerFromLyrFileで例外エラー発生！！！\n") );
	}

	return NULL;
}
