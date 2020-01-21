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
 * @brief CoInitialize���b�s���O�N���X
 *
 * �R���X�g���N�^��CoInitializeEx()�A�f�X�g���N�^��CoUninitialize()�B<br>
 * ArcGIS CQ00151553�ւ̑Ή����s���B
 */
class coinitializer : atl2::coinitializer
{
public:
	coinitializer()
	{
		bool bIsOK = true;
		// ���C�Z���X�F�ؑO�ɁA�o�C���h����K�v����
		if( !SetEsriVersion( esriArcGISEngine ) )
		{
			if( !SetEsriVersion( esriArcGISDesktop ) )
			{
				bIsOK = false;
				::CoUninitialize();	// ��O�ĂԂƊ��̃f�X�g���N�^�Ă΂�Ȃ��̂�
				throw std::exception("LoadVersion���s");
			}
		}

#ifdef _BUILD_FOR_ARC10
		// Arc10���� ���C�Z���X�F��
		if( !CertifyEsriLicense(esriLicenseProductCodeEngineGeoDB) && 
			!CertifyEsriLicense(esriLicenseProductCodeArcEditor) )
#else
		// Arc10.1���� ���C�Z���X�F��
		// bug11454 Advanced�iArcInfo�j�ł̃��C�Z���X�F�؂�ǉ�
		if( !CertifyEsriLicense(esriLicenseProductCodeEngineGeoDB) && 
			!CertifyEsriLicense(esriLicenseProductCodeStandard) &&
			!CertifyEsriLicense(esriLicenseProductCodeAdvanced))
#endif
		{
			bIsOK = false;
			::CoUninitialize();	// ��O�ĂԂƊ��̃f�X�g���N�^�Ă΂�Ȃ��̂�
			throw std::exception("���C�Z���X�F�؎��s");
		}
		// Arc10����o�b�`�n��SetLocale���Ȃ���2�o�C�g�����F�؂��Ȃ��Ȃ���

		if( bIsOK )
		{

			IArcGISLocalePtr ipLocal(CLSID_ArcGISLocale);
			ipLocal->SetLocale();
		}
	}
	
private:

	/**
	 * @brief	���C�Z���X�F�؃R�[�h
	 * @param	esriLPC	[in]	���C�Z���X�R�[�h
	 * @retval	true  : ����
	 * @retval	false : ���s
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
	 * @brief	�o�[�W�����̌Ăяo��
	 * @param	iLicense	[in]	���C�Z���X
	 * @retval	true  : ����
	 * @retval	false : ���s
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
