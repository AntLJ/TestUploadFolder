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

#pragma once

namespace generic
{

/**
 * @brief ドッカブルウィンドウを取得する関数
 * @param clsid [in] 取得するドッカブルウィンドウのCLSID
 * @param ipApp [in] アプリケーションクラスのポインタ
 * @return IDockableWindowPtr ドッカブルウィンドウクラス
*/
IDockableWindowPtr getDockableWindow(const CLSID& clsid, IApplication* ipApp);

/**
 * @brief 指定のポインタが持つUserDataをポインタ型にキャストして取得
 * @warning targetはget_UserData(VARIANT*)関数のインターフェースをもっていなければならない
 * @param target [in] UserDataを持つデータのポインタ
 * @return UserDataをIUnknownPtrとして取得したもの
 */
template <typename HAVE_USERDATA_T>
IUnknownPtr castUserDataToIUnknownPtr( HAVE_USERDATA_T& target )
{
	if(!target) return nullptr;
	CComVariant vari;
	if(FAILED(target->get_UserData( &vari )))
		return nullptr;

	// この関数で対応しているデータ型だけ変換
	switch( vari.vt )
	{
	case VT_UNKNOWN:
		return static_cast<IUnknownPtr>(vari.punkVal);
		break;
	}
	return nullptr;
}


} // generic
