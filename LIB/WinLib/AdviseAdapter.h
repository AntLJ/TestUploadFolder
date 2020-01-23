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

#ifndef _ADVISEADAPTER_H_
#define _ADVISEADAPTER_H_

/**
 * @class CAdviseAdapter
 * @brief COMイベントクラス接続ヘルパークラス
 *
 * このクラスは、COMのイベントクラスへの接続管理を
 * 行うクラスです。\n
 * このクラスを使用することで、イベントへの接続・切断
 * を楽に行う事ができます。
 * また、インスタンス破棄時に必ず接続を解除しますので、
 * 切断し忘れということがなくなります。
 *
 * 使い方：\n
 * 宣言：\n
 * CAdviseAdapter<IActiveView,&IID_IActiveViewEvents> m_cActiveViewEvents;\n
 * 初期化：\n
 * m_cActiveViewEvents = ipActiveView;\n
 * 接続：\n
 * m_cActiveViewEvents.Advise( GetUnknown() );\n
 * 切断：\n
 * m_cActiveViewEvents.Unadvise();\n
 */
template <class Interface, const IID* pdiid>
class CAdviseAdapter : public CComPtr<Interface>
{
public:
	CAdviseAdapter() : m_dwCookie(0)
	{
		p = NULL;
	}

	CAdviseAdapter( IUnknown* _p ) : m_dwCookie(0)
	{
		p = NULL;
		*this = _p;
	}

	virtual ~CAdviseAdapter()
	{
		if( IsAdvised() )
			Unadvise();
	}

	/**
	 * @breif コピーコンストラクタ
	 *
	 * 接続される側のオブジェクトを渡してください。
	 * @note 既存の接続がある場合は切断されます。
	 * @param _p	[in]	接続されるオブジェクト
	 */
	CAdviseAdapter& operator=( IUnknown* _p )
	{
		if( p )
		{
			if( IsAdvised() )
				Unadvise();

			p->Release();
			p = NULL;
		}

		if( _p )
			_p->QueryInterface( __uuidof(Interface), (void**)&p );

		return *this;
	}

/*	bool operator!=( const int& n ) const
	{
		return ! ( p == n );
	}*/

	/**
	 * @brief イベントに接続する
	 *
	 * 接続する側のオブジェクトを渡してください。
	 * @param punk	[in]	接続する側のオブジェクト
	 */
	HRESULT Advise( IUnknown* punk )
	{
		if( ! IsAdvised() )
			return AtlAdvise( p, punk, *pdiid, &m_dwCookie );
		else if( m_dwCookie )
			ATLTRACE(_T("CAdviseAdapter::Advise() : Cookie is not 0. Cookie : %ld\n"), m_dwCookie );
		else if( ! p )
			ATLTRACE(_T("CAdviseAdapter::Advise() : Interface is NULL.\n") );

		return 0;
	}
	/**
	 * @brief イベント接続を解除する
	 */
	HRESULT Unadvise()
	{
		HRESULT hr = S_OK;

		if( IsAdvised() )
		{
			hr = AtlUnadvise( p, *pdiid, m_dwCookie );
			m_dwCookie = 0;

			if( FAILED(hr) )
				ATLTRACE(_T("CAdviseAdapter::Unadvise() : Unadvise failed.\n") );
		}
		else if( 0 == m_dwCookie )
			ATLTRACE(_T("CAdviseAdapter::Unadvise() : Cookie is 0.\n") );
		else if( ! p )
			ATLTRACE(_T("CAdviseAdapter::Unadvise() : Interface is NULL.\n") );

		return hr;
	}

	HRESULT Readvise( IUnknown* punk )
	{
		HRESULT hr = S_OK;
		if( IsAdvised() )
		{
			if( SUCCEEDED( hr = Unadvise() ) )
				hr = Advise( punk );
		}
		else
			hr = Advise( punk );

		return hr;
	}

	bool IsAdvised(){ return ( m_dwCookie && p ); }
protected:
	DWORD m_dwCookie;
};

#endif // ifndef _ADVISEADAPTER_H_
