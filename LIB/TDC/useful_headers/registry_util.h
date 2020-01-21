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

// TODO: vc14以降が標準になったらdefineではなくconstexprを使用するように直す
// 主要なレジストリパスの宣言
#define REGPATH_IPC _T("Software\\INCREMENT P CORPORATION\\")
#define REGPATH_SINDY_E_TOOLS  REGPATH_IPC _T("SiNDY-e\\Tools\\")
#define REGPATH_SINDY_E_COMMON REGPATH_SINDY_E_TOOLS _T("COMMON\\")
#define REGPATH_SINDY_E_CHILDDLG REGPATH_SINDY_E_TOOLS _T("AttributeTool\\")
#define REGPATH_SINDY_K REGPATH_IPC _T("SiNDY-k\\")
#define REGPATH_SINDY_M REGPATH_IPC _T("SiNDY-m\\")
#define REGPATH_SINDY_U REGPATH_IPC _T("SiNDY-u\\")
#define REGPATH_SINDY_GL REGPATH_IPC _T("SiNDY-global\\")

// TODO: vc14以降が標準になったらthrow()ではなくnoexcept を使用するように直す

namespace uh{
namespace reg_util{
	/**
	 * @brief 親キーに合わせて、自動でアクセス権を設定する
	 * @note 各親キーに対して自動で設定されるアクセス権は下記のようになる。<br>
	 * <table>
	 * <tr><th>親キー</th><th>アクセス権</th></tr>
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
		//レジストリキーがなければ失敗
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

	/// REG_SZの値を書き込む
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

	/// REG_DWORDの値を書き込む
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

	/// キーを削除する
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

	/// 存在確認
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

	/// 同じ引数で異なる型の戻り値を返すオーバーロード関数もどきを実現するための小細工クラス
	class read
	{
	public:
		/**
		* @brief コンストラクタ
		* @breaf 実質的な関数
		* @param[in]          _key レジストリのキー
		* @param[in] _path	_hkeyを除いたレジストリキーまでのパス
		* @param[in,optional] _default キーが無かった場合に返す値
		* @param[in,optional] createDefaultKey キーが無かった場合に作成する（デフォルト：false）
		* @param[in,optional] _hkey	HKEY_XXXのキー（デフォルト：HKEY_CURRENT_USER）
		* @param[in,optional] _dwOptions キー作成オプション(デフォルト：REG_OPTION_NON_VOLATILE)<br>
		* (詳細はRegCreateKeyExのdwOptionsの説明を参照のこと)
		* @param[in,optional] _samDesired アクセス権の指定（デフォルト：AUTO_SETTING_SAMDESIRED)<br>
		* AUTO_SETTING_SAMDESIREDを指定すると、hkeyの値に合わせて自動でアクセス権を設定する。<br>
		* アクセス権の設定規則については、AUTO_SETTING_SAMDESIRED定数の説明を参照のこと。
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

		/// REG_DWORDの値を返す
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
				// デフォルト指定なし（VT_EMPTY）なら0
				lVal = va_default.lVal;

				if( m_createDefaultKey )
					write( lVal, key, path, hkey );
			}

			return (long)lVal;
		}

		/// REG_DWORDが0またはキーが無ければfalse。そうでなければtrue
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
				// デフォルト指定なし（VT_EMPTY）なら0
				lVal = va_default.boolVal ? 1 : 0;

				if( m_createDefaultKey )
					write( lVal, key, path, hkey, m_dwOptions);
			}

			return (lVal != 0);
		}

		/// REG_SZの値を返す
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

			// キーがない場合
			if( !ok )
			{
				strVal = va_default;

				if( m_createDefaultKey )
					write( strVal, key, path, hkey, m_dwOptions );
			}

			return strVal;
		}

	private:
		HKEY hkey;              //!< オープンするHKEY
		CString path;           //!< オープンするサブキー
		CString key;            //!< オープンするキー名
		CComVariant va_default; //!< キーがなかった場合に返す値
		CRegKey reg;            //!< レジストリ操作用
		bool m_createDefaultKey; //!< キーがなかったら作る
		DWORD m_dwOptions;       //!< キー作成オプション(詳細はRegCreateKeyExのdwOptionsの説明を参照のこと)
		REGSAM m_samDesired;     //!< アクセス権の指定（デフォルト：AUTO_SETTING_SAMDESIRED(hkeyに合わせて自動で作る))
	};

	/**
	* @brief RGB値として読み込む
	* @breaf REG_SZ で R,G,B で書かれた値をRGB値として取得する
	* @param[in]          key レジストリのキー
	* @param[in]          path	hkeyを除いたレジストリキーまでのパス
	* @param[in,optional] defColor キーが無かった場合に返す値
	* @param[in,optional] createDefaultKey	キーがなかった場合はデフォルト値で作成する（デフォルト：false）
	* @param[in,optional] hkey	HKEY_XXXのキー（デフォルト：HKEY_CURRENT_USER）
	* @param[in,optional] _dwOptions キー作成オプション(デフォルト：REG_OPTION_NON_VOLATILE)<br>
	* (詳細はRegCreateKeyExのdwOptionsの説明を参照のこと)
	* @param[in,optional] _samDesired アクセス権の指定（デフォルト：AUTO_SETTING_SAMDESIRED)<br>
	* AUTO_SETTING_SAMDESIREDを指定すると、hkeyの値に合わせて自動でアクセス権を設定する。<br>
	* アクセス権の設定規則については、AUTO_SETTING_SAMDESIRED定数の説明を参照のこと。
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

		// r,g,b で書かれている想定
		CString strRGB = read( key, path, CComVariant( defRGB ), createDefaultKey, hkey, dwOptions, samDesired );
		std::vector<CString> rgbLst = str_util::split( strRGB, _T(",") );

		// 書式だけ確認
		if( rgbLst.size() != 3 )
			return defColor;

		// あとは _tstol() 任せ。ちゃんと変換できなくても知らない。
		return RGB( _tstol( rgbLst[0] ), _tstol( rgbLst[1] ), _tstol( rgbLst[2] ) );
	}

} // reg_util
} // uh
