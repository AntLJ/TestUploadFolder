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

#ifndef ARCTL_COINITIALIZER_H_
#define ARCTL_COINITIALIZER_H_

#include <atl2/coinitializer.h>

namespace arctl {

/**
 * @brief CoInitializeラッピングクラス
 *
 * コンストラクタでCoInitializeEx()、デストラクタでCoUninitialize()。<br>
 * ArcGIS CQ00151553への対応も行う。
 */
class coinitializer : atl2::coinitializer
{
public:
	coinitializer()
	{
		bool bIsOK = true;
		// ライセンス認証前に、バインドする必要あり
		if( !SetEsriVersion( esriArcGISEngine ) )
		{
			if( !SetEsriVersion( esriArcGISDesktop ) )
			{
				bIsOK = false;
				::CoUninitialize();	// 例外呼ぶと基底のデストラクタ呼ばれないので
				throw std::exception("LoadVersion失敗");
			}
		}

#ifdef _BUILD_FOR_ARC10
		// Arc10向け ライセンス認証
		if( !CertifyEsriLicense(esriLicenseProductCodeEngineGeoDB) && 
			!CertifyEsriLicense(esriLicenseProductCodeArcEditor) )
#else
		// Arc10.1向け ライセンス認証
		// bug11454 Advanced（ArcInfo）でのライセンス認証を追加
		if( !CertifyEsriLicense(esriLicenseProductCodeEngineGeoDB) && 
			!CertifyEsriLicense(esriLicenseProductCodeStandard) &&
			!CertifyEsriLicense(esriLicenseProductCodeAdvanced))
#endif
		{
			bIsOK = false;
			::CoUninitialize();	// 例外呼ぶと基底のデストラクタ呼ばれないので
			throw std::exception("ライセンス認証失敗");
		}
		// Arc10からバッチ系はSetLocaleしないと2バイト文字認証しなくなった

		if( bIsOK )
		{

			IArcGISLocalePtr ipLocal(CLSID_ArcGISLocale);
			ipLocal->SetLocale();
		}
	}
	
private:

	/**
	 * @brief	ライセンス認証コード
	 * @param	esriLPC	[in]	ライセンスコード
	 * @retval	true  : 成功
	 * @retval	false : 失敗
	 */
	bool CertifyEsriLicense( esriLicenseProductCode esriLPC )
	{
		IAoInitializePtr ipInit(CLSID_AoInitialize);
		esriLicenseStatus status = esriLicenseFailure;
		ipInit->IsProductCodeAvailable(esriLPC, &status);
		if( esriLicenseAvailable == status )
		{
			ipInit->Initialize(esriLPC, &status);
			if( status == esriLicenseNotLicensed ||
				status == esriLicenseUnavailable ||
				status == esriLicenseFailure ||
				status == esriLicenseNotInitialized )
				return false;
			else
				return true;
		}
		else
			return false;
	}

	/**
	 * @brief	バージョンの呼び出し
	 * @param	iLicense	[in]	ライセンス
	 * @retval	true  : 成功
	 * @retval	false : 失敗
	 */
	bool SetEsriVersion(const int iLicense)
	{ 

		IArcGISVersionPtr ipVersion( __uuidof( VersionManager ) );
		if( !ipVersion )
			return false;

		VARIANT_BOOL vb = VARIANT_FALSE;
		HRESULT hr = ipVersion->LoadVersion( (esriVersionProductCode)iLicense, L"", &vb );
		if( !SUCCEEDED( hr ) || vb != VARIANT_TRUE )
			return false;

		return true; 
	};

};

} // namespace arctl

#endif // ARCTL_COINITIALIZER_H_
