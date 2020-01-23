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
 * @brief COM�C�x���g�N���X�ڑ��w���p�[�N���X
 *
 * ���̃N���X�́ACOM�̃C�x���g�N���X�ւ̐ڑ��Ǘ���
 * �s���N���X�ł��B\n
 * ���̃N���X���g�p���邱�ƂŁA�C�x���g�ւ̐ڑ��E�ؒf
 * ���y�ɍs�������ł��܂��B
 * �܂��A�C���X�^���X�j�����ɕK���ڑ����������܂��̂ŁA
 * �ؒf���Y��Ƃ������Ƃ��Ȃ��Ȃ�܂��B
 *
 * �g�����F\n
 * �錾�F\n
 * CAdviseAdapter<IActiveView,&IID_IActiveViewEvents> m_cActiveViewEvents;\n
 * �������F\n
 * m_cActiveViewEvents = ipActiveView;\n
 * �ڑ��F\n
 * m_cActiveViewEvents.Advise( GetUnknown() );\n
 * �ؒf�F\n
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
	 * @breif �R�s�[�R���X�g���N�^
	 *
	 * �ڑ�����鑤�̃I�u�W�F�N�g��n���Ă��������B
	 * @note �����̐ڑ�������ꍇ�͐ؒf����܂��B
	 * @param _p	[in]	�ڑ������I�u�W�F�N�g
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
	 * @brief �C�x���g�ɐڑ�����
	 *
	 * �ڑ����鑤�̃I�u�W�F�N�g��n���Ă��������B
	 * @param punk	[in]	�ڑ����鑤�̃I�u�W�F�N�g
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
	 * @brief �C�x���g�ڑ�����������
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
