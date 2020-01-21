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

#ifndef	__OCILIBCONNECTION_H_
#define	__OCILIBCONNECTION_H_

#include "ocilibError.h"
#include "ocilibGlobal.h"
#include "ocilibStatement.h"
#include "ocilibRecordset.h"
//#include "ocilibThreadMain.h"

class ocilibError;
class ocilibGlobal;
class ocilibStatement;
class ocilibRecordset;
//class ocilibThreadMain;

class ocilibConnection {
public:
	ocilibConnection();
	virtual ~ocilibConnection();

	ocilibConnection( LPCTSTR server, LPCTSTR username, LPCTSTR password );

public:
	void Init();
	void Clear();
	bool Connect( LPCTSTR server, LPCTSTR username, LPCTSTR password );
	void Disconnect();

	LPCTSTR GetError() { return m_ocilibError->GetError(); };

	inline bool IsConnect() const { return m_IsConnect; };

	inline LPCTSTR Server() { return m_ServerName; };
	inline LPCTSTR User() { return m_UserName; };
	inline LPCTSTR Password() { return m_Password; };

	ocilibStatement* Prepare( LPCTSTR sql );
	ocilibRecordset* Select( LPCTSTR sql );
	bool Break()
	{
		if ( IsConnect() ) {
			sword		result;

			result = m_ocilibGlobal->Break( m_ociEnvironment, m_ociError );
			result = m_ocilibGlobal->Reset( m_ociEnvironment, m_ociError );
		}

		return true;
	}
	void Execute( LPCTSTR sql );

private:
	void Initialize();

public:
	ocilibError*	m_ocilibError;
	ocilibGlobal*	m_ocilibGlobal;

	OCIError*		m_ociError;
	OCIEnv*			m_ociEnvironment;
	OCISvcCtx*		m_ociSvcCtx;

	TCHAR*			m_ptszData;

private:
	OCIServer*		m_ociServer;
	OCISession*		m_ociSession;

	bool			m_IsConnect;

	_CSTRING_NS::CString			m_ServerName;
	_CSTRING_NS::CString			m_UserName;
	_CSTRING_NS::CString			m_Password;
};

#endif	// __OCILIBCONNECTION_H_
