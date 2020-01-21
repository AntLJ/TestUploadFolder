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

#include <atlstr.h>
#include <atlbase.h>
#include <TDC/useful_headers/str_util.h>

// TODO: vc14�ȍ~���W���ɂȂ�����define�ł͂Ȃ�constexpr���g�p����悤�ɒ���
// ��v�ȃ��W�X�g���p�X�̐錾
#define REGPATH_IPC _T("Software\\INCREMENT P CORPORATION\\")
#define REGPATH_SINDY_E_TOOLS  REGPATH_IPC _T("SiNDY-e\\Tools\\")
#define REGPATH_SINDY_E_COMMON REGPATH_SINDY_E_TOOLS _T("COMMON\\")
#define REGPATH_SINDY_E_CHILDDLG REGPATH_SINDY_E_TOOLS _T("AttributeTool\\")
#define REGPATH_SINDY_K REGPATH_IPC _T("SiNDY-k\\")
#define REGPATH_SINDY_M REGPATH_IPC _T("SiNDY-m\\")
#define REGPATH_SINDY_U REGPATH_IPC _T("SiNDY-u\\")
#define REGPATH_SINDY_GL REGPATH_IPC _T("SiNDY-global\\")

// TODO: vc14�ȍ~���W���ɂȂ�����throw()�ł͂Ȃ�noexcept ���g�p����悤�ɒ���

namespace uh{
namespace reg_util{
	/**
	 * @brief �e�L�[�ɍ��킹�āA�����ŃA�N�Z�X����ݒ肷��
	 * @note �e�e�L�[�ɑ΂��Ď����Őݒ肳���A�N�Z�X���͉��L�̂悤�ɂȂ�B<br>
	 * <table>
	 * <tr><th>�e�L�[</th><th>�A�N�Z�X��</th></tr>
	 * <tr><td>HKEY_CLASSES_ROOT</th><th>KEY_READ</td></tr>
	 * <tr><td>HKEY_CURRENT_CONFIG</th><th>KEY_READ</td></tr>
	 * <tr><td>HKEY_CURRENT_USER</th><th>KEY_READ|KEY_WRITE</td></tr>
	 * <tr><td>HKEY_LOCAL_MACHINE</th><th>KEY_READ</td></tr>
	 * <tr><td>HKEY_USERS</th><th>KEY_READ</td></tr>
	 * </table>
	 */
	const REGSAM AUTO_SETTING_SAMDESIRED = 0UL;

	inline bool open( HKEY hkey,
		const CString& path,
		CRegKey& reg,
		REGSAM samDesired = AUTO_SETTING_SAMDESIRED
		) throw()
	{
		if(samDesired == AUTO_SETTING_SAMDESIRED)
			samDesired = (hkey == HKEY_CURRENT_USER)? KEY_READ|KEY_WRITE:KEY_READ;
		//���W�X�g���L�[���Ȃ���Ύ��s
		return ( ERROR_SUCCESS == reg.Open( hkey, path, samDesired ) );
	}

	inline bool create( HKEY hkey,
		const CString& path,
		CRegKey& reg,
		DWORD dwOptions = REG_OPTION_NON_VOLATILE
		) throw()
	{
		return ( ERROR_SUCCESS == reg.Create( hkey, path, REG_NONE, dwOptions ) );
	}

	/// REG_SZ�̒l����������
	inline bool write( const CString& val,
		const CString& key,
		const CString& path,
		HKEY hkey = HKEY_CURRENT_USER,
		DWORD dwOptions = REG_OPTION_NON_VOLATILE
		) throw()
	{
		CRegKey reg;

		if( !create(hkey, path, reg, dwOptions ) )
			return false;

		return ( ERROR_SUCCESS == reg.SetStringValue( key, val ) );
	}

	/// REG_DWORD�̒l����������
	inline bool write( long val,
		const CString& key,
		const CString& path,
		HKEY hkey = HKEY_CURRENT_USER,
		DWORD dwOptions = REG_OPTION_NON_VOLATILE
		) throw()
	{
		CRegKey reg;

		if( !create(hkey, path, reg, dwOptions) )
			return false;

		return ( ERROR_SUCCESS == reg.SetDWORDValue( key, val ) );
	}

	/// �L�[���폜����
	inline bool deletekey( const CString& key,
		const CString& path,
		HKEY hkey = HKEY_CURRENT_USER,
		REGSAM samDesired = AUTO_SETTING_SAMDESIRED
		) throw()
	{
		CRegKey reg;
		if( !open( hkey, path, reg, samDesired) )
			return false;

		return (  ERROR_SUCCESS == reg.DeleteValue( key ) );
	}

	/// ���݊m�F
	inline bool exist( const CString& key,
		const CString& path,
		HKEY hkey = HKEY_CURRENT_USER,
		REGSAM samDesired = AUTO_SETTING_SAMDESIRED
		) throw()
	{
		CRegKey reg;
		if( !open( hkey, path, reg, samDesired ) )
			return false;

		DWORD type = 0;
		return ( ERROR_SUCCESS == reg.QueryValue(key, &type, NULL, NULL) );
	}

	/// ���������ňقȂ�^�̖߂�l��Ԃ��I�[�o�[���[�h�֐����ǂ����������邽�߂̏��׍H�N���X
	class read
	{
	public:
		/**
		* @brief �R���X�g���N�^
		* @breaf �����I�Ȋ֐�
		* @param[in]          _key ���W�X�g���̃L�[
		* @param[in] _path	_hkey�����������W�X�g���L�[�܂ł̃p�X
		* @param[in,optional] _default �L�[�����������ꍇ�ɕԂ��l
		* @param[in,optional] createDefaultKey �L�[�����������ꍇ�ɍ쐬����i�f�t�H���g�Ffalse�j
		* @param[in,optional] _hkey	HKEY_XXX�̃L�[�i�f�t�H���g�FHKEY_CURRENT_USER�j
		* @param[in,optional] _dwOptions �L�[�쐬�I�v�V����(�f�t�H���g�FREG_OPTION_NON_VOLATILE)<br>
		* (�ڍׂ�RegCreateKeyEx��dwOptions�̐������Q�Ƃ̂���)
		* @param[in,optional] _samDesired �A�N�Z�X���̎w��i�f�t�H���g�FAUTO_SETTING_SAMDESIRED)<br>
		* AUTO_SETTING_SAMDESIRED���w�肷��ƁAhkey�̒l�ɍ��킹�Ď����ŃA�N�Z�X����ݒ肷��B<br>
		* �A�N�Z�X���̐ݒ�K���ɂ��ẮAAUTO_SETTING_SAMDESIRED�萔�̐������Q�Ƃ̂��ƁB
		*/
		read( LPCTSTR _key,
			const CString& _path,
			const CComVariant& _default = CComVariant(),
			bool createDefaultKey = false,
			HKEY _hkey = HKEY_CURRENT_USER,
			DWORD _dwOptions = REG_OPTION_NON_VOLATILE,
			REGSAM _samDesired = AUTO_SETTING_SAMDESIRED
			):
			key(_key),
			path(_path),
			hkey(_hkey),
			va_default(_default),
			m_createDefaultKey(createDefaultKey),
			m_dwOptions(_dwOptions),
			m_samDesired(_samDesired)
		{
		}

		/// REG_DWORD�̒l��Ԃ�
		operator long()
		{
			DWORD lVal = 0;

			bool ok = false;
			if( open( hkey, path, reg, m_samDesired ) )
			{
				ok = (ERROR_SUCCESS == reg.QueryDWORDValue(key, lVal));
				reg.Close();
			}

			if( !ok )
			{
				// �f�t�H���g�w��Ȃ��iVT_EMPTY�j�Ȃ�0
				lVal = va_default.lVal;

				if( m_createDefaultKey )
					write( lVal, key, path, hkey );
			}

			return (long)lVal;
		}

		/// REG_DWORD��0�܂��̓L�[���������false�B�����łȂ����true
		operator bool()
		{
			DWORD lVal = 0;

			bool ok = false;
			if( open( hkey, path, reg, m_samDesired ) )
			{
				ok = ( ERROR_SUCCESS == reg.QueryDWORDValue(key, lVal) );
				reg.Close();
			}

			if( !ok )
			{
				// �f�t�H���g�w��Ȃ��iVT_EMPTY�j�Ȃ�0
				lVal = va_default.boolVal ? 1 : 0;

				if( m_createDefaultKey )
					write( lVal, key, path, hkey, m_dwOptions);
			}

			return (lVal != 0);
		}

		/// REG_SZ�̒l��Ԃ�
		operator CString()
		{
			CString strVal;

			bool ok = false;
			if( open( hkey, path, reg, m_samDesired ) )
			{
				ULONG uLen = 0;
				if( ERROR_SUCCESS == reg.QueryStringValue(key, NULL, &uLen) )
				{
					ok = ( ERROR_SUCCESS == reg.QueryStringValue(key, strVal.GetBuffer(uLen), &uLen));
					strVal.ReleaseBuffer();
				}
				reg.Close();
			}

			// �L�[���Ȃ��ꍇ
			if( !ok )
			{
				strVal = va_default;

				if( m_createDefaultKey )
					write( strVal, key, path, hkey, m_dwOptions );
			}

			return strVal;
		}

	private:
		HKEY hkey;              //!< �I�[�v������HKEY
		CString path;           //!< �I�[�v������T�u�L�[
		CString key;            //!< �I�[�v������L�[��
		CComVariant va_default; //!< �L�[���Ȃ������ꍇ�ɕԂ��l
		CRegKey reg;            //!< ���W�X�g������p
		bool m_createDefaultKey; //!< �L�[���Ȃ���������
		DWORD m_dwOptions;       //!< �L�[�쐬�I�v�V����(�ڍׂ�RegCreateKeyEx��dwOptions�̐������Q�Ƃ̂���)
		REGSAM m_samDesired;     //!< �A�N�Z�X���̎w��i�f�t�H���g�FAUTO_SETTING_SAMDESIRED(hkey�ɍ��킹�Ď����ō��))
	};

	/**
	* @brief RGB�l�Ƃ��ēǂݍ���
	* @breaf REG_SZ �� R,G,B �ŏ����ꂽ�l��RGB�l�Ƃ��Ď擾����
	* @param[in]          key ���W�X�g���̃L�[
	* @param[in]          path	hkey�����������W�X�g���L�[�܂ł̃p�X
	* @param[in,optional] defColor �L�[�����������ꍇ�ɕԂ��l
	* @param[in,optional] createDefaultKey	�L�[���Ȃ������ꍇ�̓f�t�H���g�l�ō쐬����i�f�t�H���g�Ffalse�j
	* @param[in,optional] hkey	HKEY_XXX�̃L�[�i�f�t�H���g�FHKEY_CURRENT_USER�j
	* @param[in,optional] _dwOptions �L�[�쐬�I�v�V����(�f�t�H���g�FREG_OPTION_NON_VOLATILE)<br>
	* (�ڍׂ�RegCreateKeyEx��dwOptions�̐������Q�Ƃ̂���)
	* @param[in,optional] _samDesired �A�N�Z�X���̎w��i�f�t�H���g�FAUTO_SETTING_SAMDESIRED)<br>
	* AUTO_SETTING_SAMDESIRED���w�肷��ƁAhkey�̒l�ɍ��킹�Ď����ŃA�N�Z�X����ݒ肷��B<br>
	* �A�N�Z�X���̐ݒ�K���ɂ��ẮAAUTO_SETTING_SAMDESIRED�萔�̐������Q�Ƃ̂��ƁB
	*/
	inline COLORREF readRGB( LPCTSTR key,
		                     const CString& path,
							 const COLORREF& defColor = RGB(0,0,0),
							 bool createDefaultKey = false,
							 HKEY hkey = HKEY_CURRENT_USER,
							 DWORD dwOptions = REG_OPTION_NON_VOLATILE,
							 REGSAM samDesired = AUTO_SETTING_SAMDESIRED)
	{
		CString defRGB = str_util::format( _T("%ld,%ld,%ld"),
			(long)GetRValue( defColor ),
			(long)GetGValue( defColor ),
			(long)GetBValue( defColor ) );

		// r,g,b �ŏ�����Ă���z��
		CString strRGB = read( key, path, CComVariant( defRGB ), createDefaultKey, hkey, dwOptions, samDesired );
		std::vector<CString> rgbLst = str_util::split( strRGB, _T(",") );

		// ���������m�F
		if( rgbLst.size() != 3 )
			return defColor;

		// ���Ƃ� _tstol() �C���B�����ƕϊ��ł��Ȃ��Ă��m��Ȃ��B
		return RGB( _tstol( rgbLst[0] ), _tstol( rgbLst[1] ), _tstol( rgbLst[2] ) );
	}

} // reg_util
} // uh
