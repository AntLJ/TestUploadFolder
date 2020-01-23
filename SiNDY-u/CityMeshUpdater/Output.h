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

#include <atlfile.h>

class COutput
{
public:
	COutput(void);
	~COutput(void);
	void OutputMsg( LPCTSTR msg, bool bIsWriteLog = true );
	bool SetDBInfo( IWorkspace* ipSrcWS, IWorkspace* ipDstWS, IWorkspace* ipDstEditWS );

private:
	CString GetWSProperty( IWorkspace * ipWorkspace, LPCTSTR lpszPropName );
	void InitFile( LPCTSTR lpszFileName );
	BOOL WriteFile( CString strWriteBuff );

private:
	CAtlFile m_cFile;
	bool m_bIsInitFile;
};
