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

#include "stdafx.h"
#include "ocilibGlobal.h"

dvoid *Malloc(dvoid * ctxp, size_t size) { return malloc(size); };
dvoid *Realloc(dvoid * ctxp, dvoid *ptr, size_t size) { return realloc(ptr, size); };
void Freealloc(dvoid * ctxp, dvoid *ptr) { free(ptr); };

ocilibGlobal::ocilibGlobal()
{
	m_ociInstance = NULL;

	m_lpOCIErrorGet = NULL;
	m_lpOCIEnvCreate = NULL;
	m_lpOCIHandleAlloc = NULL;
	m_lpOCIServerAttach = NULL;
	m_lpOCIAttrGet = NULL;
	m_lpOCIAttrSet = NULL;
	m_lpOCISessionBegin = NULL;
	m_lpOCISessionEnd = NULL;
	m_lpOCIServerDetach = NULL;
	m_lpOCIHandleFree = NULL;
	m_lpOCIStmtPrepare = NULL;
	m_lpOCIStmtExecute = NULL;
	m_lpOCIParamGet = NULL;
	m_lpOCIDescriptorFree = NULL;
	m_lpOCIDefineByPos = NULL;
	m_lpOCIStmtFetch = NULL;
	m_lpOCIStmtFetch2 = NULL;
	m_lpOCINumberToReal = NULL;
	m_lpOCINumberToInt = NULL;
	m_lpOCINumberToText = NULL;
	m_lpOCINumberFromReal = NULL;
	m_lpOCINumberFromInt = NULL;
	m_lpOCINumberSetZero = NULL;
	m_lpOCIDateSysDate = NULL;
	m_lpOCIBindByName = NULL;
	m_lpOCINlsGetInfo = NULL;
	m_lpOCIWideCharToMultiByte = NULL;
	m_lpOCIMultiByteToWideChar = NULL;
	m_lpOCIStringAssignText = NULL;
	m_lpOCIThreadHndInit = NULL;
	m_lpOCIThreadInit = NULL;
	m_lpOCIThreadClose = NULL;
	m_lpOCIThreadCreate = NULL;
	m_lpOCIThreadHandleGet = NULL;
	m_lpOCIThreadHndDestroy = NULL;
	m_lpOCIThreadIdDestroy = NULL;
	m_lpOCIThreadIdGet = NULL;
	m_lpOCIThreadIdInit = NULL;
	m_lpOCIThreadIdNull = NULL;
	m_lpOCIThreadIdSame = NULL;
	m_lpOCIThreadIdSet = NULL;
	m_lpOCIThreadIdSetNull = NULL;
	m_lpOCIThreadIsMulti = NULL;
	m_lpOCIThreadJoin = NULL;
	m_lpOCIThreadKeyDestroy = NULL;
	m_lpOCIThreadKeyGet = NULL;
	m_lpOCIThreadKeyInit = NULL;
	m_lpOCIThreadKeySet = NULL;
	m_lpOCIThreadMutexAcquire = NULL;
	m_lpOCIThreadMutexDestroy = NULL;
	m_lpOCIThreadMutexInit = NULL;
	m_lpOCIThreadMutexRelease = NULL;
	m_lpOCIThreadProcessInit = NULL;
	m_lpOCIThreadTerm = NULL;
	m_lpOCIBreak = NULL;
	m_lpOCIReset = NULL;
	m_lpOCINumberIsInt = NULL;
}

ocilibGlobal::~ocilibGlobal()
{
	if ( m_ociInstance != NULL )
		FreeLibrary( m_ociInstance );
	m_ociInstance = NULL;
}

void ocilibGlobal::Method(long method)
{
	if ( m_ociInstance == NULL )
		m_ociInstance = LoadLibrary( _T("oci.dll") );

	switch( method ) {
		case 0:
			if ( m_lpOCIErrorGet == NULL )
				m_lpOCIErrorGet = (sword (WINAPIV *)( dvoid *hndlp,ub4 recordno,text *sqlstate,sb4 *errcodep,text *bufp,ub4 bufsiz,ub4 type ) )GetProcAddress( m_ociInstance, "OCIErrorGet" );
			break;
		case 1:
			if ( m_lpOCIEnvCreate == NULL )
				m_lpOCIEnvCreate = (sword (WINAPIV *)( OCIEnv **envhpp,ub4 mode,CONST dvoid *ctxp,CONST dvoid *(*malocfp)(dvoid *ctxp,size_t size),CONST dvoid *(*ralocfp)(dvoid *ctxp,dvoid *memptr,size_t newsize),CONST void (*mfreefp)(dvoid *ctxp,dvoid *memptr), size_t xtramemsz,dvoid **usrmempp ) )GetProcAddress( m_ociInstance, "OCIEnvCreate" );
			break;
		case 2:
			if ( m_lpOCIHandleAlloc == NULL )
				m_lpOCIHandleAlloc = (sword (WINAPIV *)( CONST dvoid *parenth,dvoid **hndlpp,ub4 type,size_t xtramem_sz,dvoid **usrmempp ) )GetProcAddress( m_ociInstance, "OCIHandleAlloc" );
			break;
		case 3:
			if ( m_lpOCIServerAttach == NULL )
				m_lpOCIServerAttach = (sword (WINAPIV *)( OCIServer *srvhp,OCIError *errhp,CONST text *dblink,sb4 dblink_len,ub4 mode ) )GetProcAddress( m_ociInstance, "OCIServerAttach" );
			break;
		case 4:
			if ( m_lpOCIAttrGet == NULL )
				m_lpOCIAttrGet = (sword (WINAPIV *)(CONST dvoid *trgthndlp,ub4 trghndltyp,dvoid *attributep,ub4 *sizep,ub4 attrtype,OCIError *errhp) )GetProcAddress( m_ociInstance, "OCIAttrGet" );
			break;
		case 5:
			if ( m_lpOCIAttrSet == NULL )
				m_lpOCIAttrSet = (sword (WINAPIV *)( dvoid *trgthndlp,ub4 trghndltyp,dvoid *attributep,ub4 size,ub4 attrtype,OCIError *errhp ) )GetProcAddress( m_ociInstance, "OCIAttrSet" );
			break;
		case 6:
			if ( m_lpOCISessionBegin == NULL )
				m_lpOCISessionBegin = (sword (WINAPIV *)( OCISvcCtx *svchp,OCIError *errhp,OCISession *usrhp,ub4 credt,ub4 mode ) )GetProcAddress( m_ociInstance, "OCISessionBegin" );
			break;
		case 7:
			if ( m_lpOCISessionEnd == NULL )
				m_lpOCISessionEnd = (sword (WINAPIV *)( OCISvcCtx *svchp,OCIError *errhp,OCISession *usrhp,ub4 mode ) )GetProcAddress( m_ociInstance, "OCISessionEnd" );
			break;
		case 8:
			if ( m_lpOCIServerDetach == NULL )
				m_lpOCIServerDetach = (sword (WINAPIV *)( OCIServer *srvhp,OCIError *errhp,ub4 mode ) )GetProcAddress( m_ociInstance, "OCIServerDetach" );
			break;
		case 9:
			if ( m_lpOCIHandleFree == NULL )
				m_lpOCIHandleFree = (sword (WINAPIV *)( dvoid *hndlp,ub4 type ) )GetProcAddress( m_ociInstance, "OCIHandleFree" );
			break;
		case 10:
			if ( m_lpOCIStmtPrepare == NULL )
				m_lpOCIStmtPrepare = (sword (WINAPIV *)( OCIStmt *stmtp,OCIError *errhp,CONST text *stmt,ub4 stmt_len,ub4 language,ub4 mode ) )GetProcAddress( m_ociInstance, "OCIStmtPrepare" );
			break;
		case 11:
			if ( m_lpOCIStmtExecute == NULL )
				m_lpOCIStmtExecute = (sword (WINAPIV *)( OCISvcCtx *svchp,OCIStmt *stmtp,OCIError *errhp,ub4 iters,ub4 rowoff,CONST OCISnapshot *snap_in,OCISnapshot *snap_out,ub4 mode ) )GetProcAddress( m_ociInstance, "OCIStmtExecute" );
			break;
		case 12:
			if ( m_lpOCIParamGet == NULL )
				m_lpOCIParamGet = (sword (WINAPIV *)( CONST dvoid *hndlp,ub4 htype,OCIError *errhp,dvoid **parmdpp,ub4 pos ) )GetProcAddress( m_ociInstance, "OCIParamGet" );
			break;
		case 13:
			if ( m_lpOCIDescriptorFree == NULL )
				m_lpOCIDescriptorFree = (sword (WINAPIV *)( dvoid *descp,ub4 type ) )GetProcAddress( m_ociInstance, "OCIDescriptorFree" );
			break;
		case 14:
			if ( m_lpOCIDefineByPos == NULL )
				m_lpOCIDefineByPos = (sword (WINAPIV *)( OCIStmt *stmtp,OCIDefine **defnpp,OCIError *errhp,ub4 position,dvoid *valuep,sb4 value_sz,ub2 dty,dvoid *indp,ub2 *rlenp,ub2 *rcodep,ub4 mode ) )GetProcAddress( m_ociInstance, "OCIDefineByPos" );
			break;
		case 15:
			if ( m_lpOCIStmtFetch == NULL )
				m_lpOCIStmtFetch = (sword (WINAPIV *)( OCIStmt *stmthp,OCIError *errhp,ub4 nrows,ub2 orientation,ub4 mode ) )GetProcAddress( m_ociInstance, "OCIStmtFetch" );
			break;
		case 16:
			if ( m_lpOCIStmtFetch2 == NULL )
				m_lpOCIStmtFetch2 = (sword (WINAPIV *)( OCIStmt *stmthp,OCIError *errhp,ub4 nrows,ub2 orientation,sb4 fetchOffset,ub4 mode ) )GetProcAddress( m_ociInstance, "OCIStmtFetch2" );
			break;
		case 17:
			if ( m_lpOCINumberToReal == NULL )
				m_lpOCINumberToReal = (sword (WINAPIV *)( OCIError *err,CONST OCINumber *number,uword rsl_length,dvoid *rsl ) )GetProcAddress( m_ociInstance, "OCINumberToReal" );
			break;
		case 18:
			if ( m_lpOCINumberToInt == NULL )
				m_lpOCINumberToInt = (sword (WINAPIV *)( OCIError *err,CONST OCINumber *number,uword rsl_length,uword rsl_flag,dvoid *rsl ) )GetProcAddress( m_ociInstance, "OCINumberToInt" );
			break;
		case 19:
			if ( m_lpOCINumberToText == NULL )
				m_lpOCINumberToText = (sword (WINAPIV *)( OCIError *err,CONST OCINumber *number,CONST OraText *fmt,ub4 fmt_length,CONST OraText *nls_params,ub4 nls_p_length,ub4 *buf_size,text *buf ) )GetProcAddress( m_ociInstance, "OCINumberToText" );
			break;
		case 20:
			if ( m_lpOCINumberFromReal == NULL )
				m_lpOCINumberFromReal = (sword (WINAPIV *)( OCIError *err,CONST dvoid *rnum,uword rnum_length,OCINumber *number ) )GetProcAddress( m_ociInstance, "OCINumberFromReal" );
			break;
		case 21:
			if ( m_lpOCINumberFromInt == NULL )
				m_lpOCINumberFromInt = (sword (WINAPIV *)( OCIError *err,CONST dvoid *inum,uword inum_length,uword inum_s_flag,OCINumber *number ) )GetProcAddress( m_ociInstance, "OCINumberFromInt" );
			break;
		case 22:
			if ( m_lpOCINumberSetZero == NULL )
				m_lpOCINumberSetZero = (void (WINAPIV *)( OCIError *err,OCINumber *num ) )GetProcAddress( m_ociInstance, "OCINumberSetZero" );
			break;
		case 23:
			if ( m_lpOCIDateSysDate == NULL )
				m_lpOCIDateSysDate = (sword (WINAPIV *)( OCIError *err,OCIDate *sys_date ) )GetProcAddress( m_ociInstance, "OCIDateSysDate" );
			break;
		case 24:
			if ( m_lpOCIBindByName == NULL )
				m_lpOCIBindByName = (sword (WINAPIV *)( OCIStmt *stmtp,OCIBind **bindpp,OCIError *errhp,CONST text *placeholder,sb4 placeh_len,dvoid *valuep,sb4 value_sz,ub2 dty,dvoid *indp,ub2 *alenp,ub2 *rcodep,ub4 maxarr_len,ub4 *curelep,ub4 mode ) )GetProcAddress( m_ociInstance, "OCIBindByName" );
			break;
		case 25:
			if ( m_lpOCINlsGetInfo == NULL )
				m_lpOCINlsGetInfo = (sword (WINAPIV *)( CONST dvoid *parenth, CONST dvoid *hndlpp,text *sqlstate,sb4 errcodep,ub2 mdoe ) )GetProcAddress( m_ociInstance, "OCINlsGetInfo" );
			break;
		case 26:
			if ( m_lpOCIWideCharToMultiByte == NULL )
				m_lpOCIWideCharToMultiByte = (sword (WINAPIV *)(dvoid *envhp, OraText *dst, CONST OCIWchar *src, size_t *rsize) )GetProcAddress( m_ociInstance, "OCIWideCharToMultiByte" );
			break;
		case 27:
			if ( m_lpOCIMultiByteToWideChar == NULL )
				m_lpOCIMultiByteToWideChar = (sword (WINAPIV *)(dvoid *envhp, OCIWchar *dst, CONST OraText *src, size_t *rsize) )GetProcAddress( m_ociInstance, "OCIMultiByteToWideChar" );
			break;
		case 28:
			if ( m_lpOCIStringAssignText == NULL )
				m_lpOCIStringAssignText = (sword (WINAPIV *)(OCIEnv *env, OCIError *err, CONST text *rhs, ub4 rhs_len, OCIString **lhs) )GetProcAddress( m_ociInstance, "OCIStringAssignText" );
			break;
		case 29:
			if ( m_lpOCIThreadHndInit == NULL )
				m_lpOCIThreadHndInit = (sword (WINAPIV *)(dvoid *hndl, OCIError *err, OCIThreadHandle **thnd) )GetProcAddress( m_ociInstance, "OCIThreadHndInit" );
			break;
		case 30:
			if ( m_lpOCIThreadInit == NULL )
				m_lpOCIThreadInit = (sword (WINAPIV *)(dvoid *hndl, OCIError *err))GetProcAddress( m_ociInstance, "OCIThreadInit" );
			break;
		case 31:
			if ( m_lpOCIThreadClose == NULL )
				m_lpOCIThreadClose = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd ))GetProcAddress( m_ociInstance, "OCIThreadClose" );
			break;
		case 32:
			if ( m_lpOCIThreadCreate == NULL )
				m_lpOCIThreadCreate = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, dvoid *arg, OCIThreadId *tid, OCIThreadHandle *tHnd ))GetProcAddress( m_ociInstance, "OCIThreadCreate" );
			break;
		case 33:
			if ( m_lpOCIThreadHandleGet == NULL )
				m_lpOCIThreadHandleGet = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd ))GetProcAddress( m_ociInstance, "OCIThreadHandleGet" );
			break;
		case 34:
			if ( m_lpOCIThreadHndDestroy == NULL )
				m_lpOCIThreadHndDestroy = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadHandle **thnd ))GetProcAddress( m_ociInstance, "OCIThreadHndDestroy" );
			break;
		case 35:
			if ( m_lpOCIThreadIdDestroy == NULL )
				m_lpOCIThreadIdDestroy = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadId **tid ))GetProcAddress( m_ociInstance, "OCIThreadIdDestroy" );
			break;
		case 36:
			if ( m_lpOCIThreadIdGet == NULL )
				m_lpOCIThreadIdGet = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadId *tid ))GetProcAddress( m_ociInstance, "OCIThreadIdGet" );
			break;
		case 37:
			if ( m_lpOCIThreadIdInit == NULL )
				m_lpOCIThreadIdInit = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadId **tid ))GetProcAddress( m_ociInstance, "OCIThreadIdInit" );
			break;
		case 38:
			if ( m_lpOCIThreadIdNull == NULL )
				m_lpOCIThreadIdNull = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadId *tid, boolean *result ))GetProcAddress( m_ociInstance, "OCIThreadIdNull" );
			break;
		case 39:
			if ( m_lpOCIThreadIdSame == NULL )
				m_lpOCIThreadIdSame = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadId *tid1, OCIThreadId *tid2, boolean *result ))GetProcAddress( m_ociInstance, "OCIThreadIdSame" );
			break;
		case 40:
			if ( m_lpOCIThreadIdSet == NULL )
				m_lpOCIThreadIdSet = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadId *tidDest, OCIThreadId *tidSrc ))GetProcAddress( m_ociInstance, "OCIThreadIdSet" );
			break;
		case 41:
			if ( m_lpOCIThreadIdSetNull == NULL )
				m_lpOCIThreadIdSetNull = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadId *tid ))GetProcAddress( m_ociInstance, "OCIThreadIdSetNull" );
			break;
		case 42:
			if ( m_lpOCIThreadIsMulti == NULL )
				m_lpOCIThreadIsMulti = (boolean (WINAPIV *)())GetProcAddress( m_ociInstance, "OCIThreadIsMulti" );
			break;
		case 43:
			if ( m_lpOCIThreadJoin == NULL )
				m_lpOCIThreadJoin = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd ))GetProcAddress( m_ociInstance, "OCIThreadJoin" );
			break;
		case 44:
			if ( m_lpOCIThreadKeyDestroy == NULL )
				m_lpOCIThreadKeyDestroy = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadKey **key ))GetProcAddress( m_ociInstance, "OCIThreadKeyDestroy" );
			break;
		case 45:
			if ( m_lpOCIThreadKeyGet == NULL )
				m_lpOCIThreadKeyGet = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadKey *key, dvoid **pValue ))GetProcAddress( m_ociInstance, "OCIThreadKeyGet" );
			break;
		case 46:
			if ( m_lpOCIThreadKeyInit == NULL )
				m_lpOCIThreadKeyInit = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadKey **key, OCIThreadKeyDestFunc destFn ))GetProcAddress( m_ociInstance, "OCIThreadKeyInit" );
			break;
		case 47:
			if ( m_lpOCIThreadKeySet == NULL )
				m_lpOCIThreadKeySet = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadKey *key, dvoid *value ))GetProcAddress( m_ociInstance, "OCIThreadKeySet" );
			break;
		case 48:
			if ( m_lpOCIThreadMutexAcquire == NULL )
				m_lpOCIThreadMutexAcquire = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadMutex *mutex ))GetProcAddress( m_ociInstance, "OCIThreadMutexAcquire" );
			break;
		case 49:
			if ( m_lpOCIThreadMutexDestroy == NULL )
				m_lpOCIThreadMutexDestroy = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadMutex **mutex ))GetProcAddress( m_ociInstance, "OCIThreadMutexDestroy" );
			break;
		case 50:
			if ( m_lpOCIThreadMutexInit == NULL )
				m_lpOCIThreadMutexInit = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadMutex **mutex ))GetProcAddress( m_ociInstance, "OCIThreadMutexInit" );
			break;
		case 51:
			if ( m_lpOCIThreadMutexRelease == NULL )
				m_lpOCIThreadMutexRelease = (sword (WINAPIV *)( dvoid *hndl, OCIError *err, OCIThreadMutex *mutex ))GetProcAddress( m_ociInstance, "OCIThreadMutexRelease" );
			break;
		case 52:
			if ( m_lpOCIThreadProcessInit == NULL )
				m_lpOCIThreadProcessInit = (void (WINAPIV *)())GetProcAddress( m_ociInstance, "OCIThreadProcessInit" );
			break;
		case 53:
			if ( m_lpOCIThreadTerm == NULL )
				m_lpOCIThreadTerm = (sword (WINAPIV *)( dvoid *hndl, OCIError *err ))GetProcAddress( m_ociInstance, "OCIThreadTerm" );
			break;
		case 54:
			if ( m_lpOCIBreak == NULL )
				m_lpOCIBreak = (sword (WINAPIV *)(dvoid *hndlp, OCIError *errhp))GetProcAddress( m_ociInstance, "OCIBreak" );
			break;
		case 55:
			if ( m_lpOCIReset == NULL )
				m_lpOCIReset = (sword (WINAPIV *)(dvoid *hndlp, OCIError *errhp))GetProcAddress( m_ociInstance, "OCIReset" );
			break;
		case 56:
			if ( m_lpOCINumberIsInt == NULL )
				m_lpOCINumberIsInt = (sword (WINAPIV *)(OCIError *err, CONST OCINumber *number, boolean *result))GetProcAddress( m_ociInstance, "OCINumberIsInt" );
			break;
	}
}

sword ocilibGlobal::ErrorGet( dvoid *hndlp, ub4 recordno, text *sqlstate, sb4 *errcodep, text *bufp, ub4 bufsiz, ub4 type )
{
	Method(0);

	if ( m_lpOCIErrorGet != NULL )
		return m_lpOCIErrorGet( hndlp, recordno, sqlstate, errcodep, bufp, bufsiz, type );
	return 0;
}

sword ocilibGlobal::EnvCreate( OCIEnv **envhpp,ub4 mode,CONST dvoid *ctxp, size_t xtramemsz, dvoid **usrmempp )
{
	Method(1);

	if ( m_lpOCIEnvCreate != NULL )
		return m_lpOCIEnvCreate( envhpp, mode, ctxp, (CONST dvoid *(WINAPIV *)(dvoid *ctxp,size_t size))Malloc, (CONST dvoid *(WINAPIV *)(dvoid *ctxp,dvoid *memptr,size_t newsize))Realloc, (CONST void (WINAPIV *)(dvoid *ctxp,dvoid *memptr))Freealloc, xtramemsz, usrmempp );
	return 0;
}

sword ocilibGlobal::HandleAlloc( CONST dvoid *parenth, dvoid **hndlpp, ub4 type, size_t xtramem_sz, dvoid **usrmempp )
{
	Method(2);

	if ( m_lpOCIHandleAlloc != NULL )
		return m_lpOCIHandleAlloc( parenth, hndlpp, type, xtramem_sz, usrmempp );
	return 0;
}

sword ocilibGlobal::ServerAttach( OCIServer *srvhp,OCIError *errhp,CONST text *dblink,sb4 dblink_len,ub4 mode )
{
	Method(3);

	if ( m_lpOCIServerAttach != NULL )
		return m_lpOCIServerAttach( srvhp, errhp,  dblink, dblink_len, mode );
	return 0;
}

sword ocilibGlobal::AttrGet(CONST dvoid *trgthndlp,ub4 trghndltyp,dvoid *attributep,ub4 *sizep,ub4 attrtype,OCIError *errhp)
{
	Method(4);

	if ( m_lpOCIAttrGet != NULL )
		return m_lpOCIAttrGet( trgthndlp, trghndltyp, attributep, sizep, attrtype, errhp);
	return 0;
}

sword ocilibGlobal::AttrSet( dvoid *trgthndlp,ub4 trghndltyp,dvoid *attributep,ub4 size,ub4 attrtype,OCIError *errhp )
{
	Method(5);

	if ( m_lpOCIAttrSet != NULL )
		return m_lpOCIAttrSet( trgthndlp, trghndltyp, attributep, size, attrtype, errhp );
	return 0;
}

sword ocilibGlobal::SessionBegin( OCISvcCtx *svchp,OCIError *errhp,OCISession *usrhp,ub4 credt,ub4 mode )
{
	Method(6);

	if ( m_lpOCISessionBegin != NULL )
		return m_lpOCISessionBegin( svchp, errhp, usrhp, credt, mode );
	return 0;
}

sword ocilibGlobal::SessionEnd( OCISvcCtx *svchp,OCIError *errhp,OCISession *usrhp,ub4 mode )
{
	Method(7);

	if ( m_lpOCISessionEnd != NULL )
		return m_lpOCISessionEnd( svchp, errhp, usrhp, mode );
	return 0;
}

sword ocilibGlobal::ServerDetach( OCIServer *srvhp,OCIError *errhp,ub4 mode )
{
	Method(8);

	if ( m_lpOCIServerDetach != NULL )
		return m_lpOCIServerDetach( srvhp, errhp, mode );
	return 0;
}

sword ocilibGlobal::HandleFree( dvoid *hndlp,ub4 type )
{
	Method(9);

	if ( m_lpOCIHandleFree != NULL )
		return m_lpOCIHandleFree( hndlp, type );
	return 0;
}

sword ocilibGlobal::StmtPrepare( OCIStmt *stmtp,OCIError *errhp,CONST text *stmt,ub4 stmt_len,ub4 language,ub4 mode )
{
	Method(10);

	if ( m_lpOCIStmtPrepare != NULL )
		return m_lpOCIStmtPrepare( stmtp, errhp,  stmt, stmt_len, language, mode );
	return 0;
}

sword ocilibGlobal::StmtExecute( OCISvcCtx *svchp,OCIStmt *stmtp,OCIError *errhp,ub4 iters,ub4 rowoff,CONST OCISnapshot *snap_in,OCISnapshot *snap_out,ub4 mode )
{
	Method(11);

	if ( m_lpOCIStmtExecute != NULL )
		return m_lpOCIStmtExecute( svchp, stmtp, errhp, iters, rowoff,  snap_in, snap_out, mode );
	return 0;
}

sword ocilibGlobal::ParamGet( CONST dvoid *hndlp,ub4 htype,OCIError *errhp,dvoid **parmdpp,ub4 pos )
{
	Method(12);

	if ( m_lpOCIParamGet != NULL )
		return m_lpOCIParamGet( hndlp, htype, errhp, parmdpp, pos );
	return 0;
}

sword ocilibGlobal::DescriptorFree( dvoid *descp,ub4 type )
{
	Method(13);

	if ( m_lpOCIDescriptorFree != NULL )
		return m_lpOCIDescriptorFree( descp, type );
	return 0;
}

sword ocilibGlobal::DefineByPos( OCIStmt *stmtp,OCIDefine **defnpp,OCIError *errhp,ub4 position,dvoid *valuep,sb4 value_sz,ub2 dty,dvoid *indp,ub2 *rlenp,ub2 *rcodep,ub4 mode )
{
	Method(14);

	if ( m_lpOCIDefineByPos != NULL )
		return m_lpOCIDefineByPos( stmtp, defnpp, errhp, position, valuep, value_sz, dty, indp, rlenp, rcodep, mode );
	return 0;
}

sword ocilibGlobal::StmtFetch( OCIStmt *stmthp,OCIError *errhp,ub4 nrows,ub2 orientation,ub4 mode )
{
	Method(15);
	Method(16);

	if ( m_lpOCIStmtFetch2 != NULL )
		return m_lpOCIStmtFetch2( stmthp, errhp, nrows, orientation, OCI_FETCH_CURRENT, mode );
	else {
		if ( m_lpOCIStmtFetch != NULL )
			return m_lpOCIStmtFetch( stmthp, errhp, nrows, orientation, mode );
	}

	return 0;
}

sword ocilibGlobal::StmtFetch2( OCIStmt *stmthp,OCIError *errhp,ub4 nrows,ub2 orientation,sb4 fetchOffset,ub4 mode )
{
	Method(15);
	Method(16);

	if ( m_lpOCIStmtFetch2 != NULL )
		return m_lpOCIStmtFetch2( stmthp, errhp, nrows, orientation, fetchOffset, mode );
	else {
		if ( m_lpOCIStmtFetch != NULL )
			return m_lpOCIStmtFetch( stmthp, errhp, nrows, orientation, mode );
	}

	return 0;
}

sword ocilibGlobal::NumberToReal( OCIError *err,CONST OCINumber *number,uword rsl_length,dvoid *rsl )
{
	Method(17);

	if ( m_lpOCINumberToReal != NULL )
		return m_lpOCINumberToReal( err, number, rsl_length, rsl );
	return 0;
}

sword ocilibGlobal::NumberToInt( OCIError *err,CONST OCINumber *number,uword rsl_length,uword rsl_flag,dvoid *rsl )
{
	Method(18);

	if ( m_lpOCINumberToInt != NULL )
		return m_lpOCINumberToInt( err, number, rsl_length, rsl_flag, rsl );
	return 0;
}

sword ocilibGlobal::NumberToText( OCIError *err,CONST OCINumber *number,CONST OraText *fmt,ub4 fmt_length,CONST OraText *nls_params,ub4 nls_p_length,ub4 *buf_size,text *buf )
{
	Method(19);

	if ( m_lpOCINumberToText != NULL )
		return m_lpOCINumberToText( err, number, fmt, fmt_length, nls_params, nls_p_length, buf_size, buf );
	return 0;
}

sword ocilibGlobal::NumberFromReal( OCIError *err,CONST dvoid *rnum,uword rnum_length,OCINumber *number )
{
	Method(20);

	if ( m_lpOCINumberFromReal != NULL )
		return m_lpOCINumberFromReal( err, rnum, rnum_length, number );
	return 0;
}

sword ocilibGlobal::NumberFromInt( OCIError *err,CONST dvoid *inum,uword inum_length,uword inum_s_flag,OCINumber *number )
{
	Method(21);

	if ( m_lpOCINumberFromInt != NULL )
		return m_lpOCINumberFromInt( err, inum, inum_length, inum_s_flag, number );
	return 0;
}

void ocilibGlobal::NumberSetZero( OCIError *err,OCINumber *num )
{
	Method(22);

	if ( m_lpOCINumberSetZero != NULL )
		m_lpOCINumberSetZero( err, num );
	return;
}

sword ocilibGlobal::DateSysDate( OCIError *err,OCIDate *sys_date )
{
	Method(23);

	if ( m_lpOCIDateSysDate != NULL )
		return m_lpOCIDateSysDate( err, sys_date );
	return 0;
}

sword ocilibGlobal::BindByName( OCIStmt *stmtp,OCIBind **bindpp,OCIError *errhp,CONST text *placeholder,sb4 placeh_len,dvoid *valuep,sb4 value_sz,ub2 dty,dvoid *indp,ub2 *alenp,ub2 *rcodep,ub4 maxarr_len,ub4 *curelep,ub4 mode )
{
	Method(24);

	if ( m_lpOCIBindByName != NULL )
		return m_lpOCIBindByName( stmtp, bindpp, errhp,  placeholder, placeh_len, valuep, value_sz, dty, indp, alenp, rcodep, maxarr_len, curelep, mode );
	return 0;
}

sword ocilibGlobal::NlsGetInfo( CONST dvoid *parenth, CONST dvoid *hndlpp,text *sqlstate,sb4 errcodep,ub2 mdoe)
{
	Method(25);

	if ( m_lpOCINlsGetInfo != NULL )
		return m_lpOCINlsGetInfo( parenth, hndlpp, sqlstate, errcodep, mdoe );
	return 0;
}

sword ocilibGlobal::WideChar2MultiByte( dvoid *envhp, OraText *dst, CONST OCIWchar *src, size_t *rsize)
{
	Method(26);

	if ( m_lpOCIWideCharToMultiByte != NULL )
		return m_lpOCIWideCharToMultiByte( envhp, dst, src, rsize );
	return 0;
}

sword ocilibGlobal::MultiByte2WideChar( dvoid *envhp, OCIWchar *dst, CONST OraText *src, size_t *rsize)
{
	Method(27);

	if ( m_lpOCIMultiByteToWideChar != NULL )
		return m_lpOCIMultiByteToWideChar( envhp, dst, src, rsize );
	return 0;
}

sword ocilibGlobal::StringAssignText( OCIEnv *env, OCIError *err, CONST text *rhs, ub4 rhs_len, OCIString **lhs)
{
	Method(28);

	if ( m_lpOCIStringAssignText != NULL )
		return m_lpOCIStringAssignText( env, err, rhs, rhs_len, lhs );
	return 0;
}

sword ocilibGlobal::ThreadHndInit( dvoid *hndl, OCIError *err, OCIThreadHandle **thnd)
{
	Method(29);

	if ( m_lpOCIThreadHndInit != NULL )
		return m_lpOCIThreadHndInit( hndl, err, thnd );
	return 0;
}

sword ocilibGlobal::ThreadInit( dvoid *hndl, OCIError *err)
{
	Method(30);

	if ( m_lpOCIThreadInit != NULL )
		return m_lpOCIThreadInit( hndl, err );
	return 0;
}

sword ocilibGlobal::ThreadClose( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd )
{
	Method(31);

	if ( m_lpOCIThreadClose != NULL )
		return m_lpOCIThreadClose( hndl, err, tHnd );
	return 0;
}

sword ocilibGlobal::ThreadCreate( dvoid *hndl, OCIError *err, dvoid *arg, OCIThreadId *tid, OCIThreadHandle *tHnd )
{
	Method(32);

	if ( m_lpOCIThreadCreate != NULL )
		return m_lpOCIThreadCreate( hndl, err, arg, tid, tHnd );
	return 0;
}

sword ocilibGlobal::ThreadHandleGet( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd )
{
	Method(33);

	if ( m_lpOCIThreadHandleGet != NULL )
		return m_lpOCIThreadHandleGet( hndl, err, tHnd );
	return 0;
}

sword ocilibGlobal::ThreadHndDestroy( dvoid *hndl, OCIError *err, OCIThreadHandle **thnd )
{
	Method(34);

	if ( m_lpOCIThreadHndDestroy != NULL )
		return m_lpOCIThreadHndDestroy( hndl, err, thnd );
	return 0;
}

sword ocilibGlobal::ThreadIdDestroy( dvoid *hndl, OCIError *err, OCIThreadId **tid )
{
	Method(35);

	if ( m_lpOCIThreadIdDestroy != NULL )
		return m_lpOCIThreadIdDestroy( hndl, err, tid);
	return 0;
}

sword ocilibGlobal::ThreadIdGet( dvoid *hndl, OCIError *err, OCIThreadId *tid )
{
	Method(36);

	if ( m_lpOCIThreadIdGet != NULL )
		return m_lpOCIThreadIdGet( hndl, err, tid );
	return 0;
}

sword ocilibGlobal::ThreadIdInit( dvoid *hndl, OCIError *err, OCIThreadId **tid )
{
	Method(37);

	if ( m_lpOCIThreadIdInit != NULL )
		return m_lpOCIThreadIdInit( hndl, err, tid );
	return 0;
}

sword ocilibGlobal::ThreadIdNull( dvoid *hndl, OCIError *err, OCIThreadId *tid, boolean *result )
{
	Method(38);

	if ( m_lpOCIThreadIdNull != NULL )
		return m_lpOCIThreadIdNull( hndl, err, tid, result );
	return 0;
}

sword ocilibGlobal::ThreadIdSame( dvoid *hndl, OCIError *err, OCIThreadId *tid1, OCIThreadId *tid2, boolean *result )
{
	Method(39);

	if ( m_lpOCIThreadIdSame != NULL )
		return m_lpOCIThreadIdSame( hndl, err, tid1, tid2, result );
	return 0;
}

sword ocilibGlobal::ThreadIdSet( dvoid *hndl, OCIError *err, OCIThreadId *tidDest, OCIThreadId *tidSrc )
{
	Method(40);

	if ( m_lpOCIThreadIdSet != NULL )
		return m_lpOCIThreadIdSet( hndl, err, tidDest, tidSrc );
	return 0;
}

sword ocilibGlobal::ThreadIdSetNull( dvoid *hndl, OCIError *err, OCIThreadId *tid )
{
	Method(41);

	if ( m_lpOCIThreadIdSetNull != NULL )
		return m_lpOCIThreadIdSetNull( hndl, err, tid );
	return 0;
}

boolean ocilibGlobal::ThreadIsMulti()
{
	Method(42);

	if ( m_lpOCIThreadIsMulti != NULL )
		return m_lpOCIThreadIsMulti();
	return 0;
}

sword ocilibGlobal::ThreadJoin( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd )
{
	Method(43);

	if ( m_lpOCIThreadJoin != NULL )
		return m_lpOCIThreadJoin( hndl, err, tHnd );
	return 0;
}

sword ocilibGlobal::ThreadKeyDestroy( dvoid *hndl, OCIError *err, OCIThreadKey **key )
{
	Method(44);

	if ( m_lpOCIThreadKeyDestroy != NULL )
		return m_lpOCIThreadKeyDestroy( hndl, err, key );
	return 0;
}

sword ocilibGlobal::ThreadKeyGet( dvoid *hndl, OCIError *err, OCIThreadKey *key, dvoid **pValue )
{
	Method(45);

	if ( m_lpOCIThreadKeyGet != NULL )
		return m_lpOCIThreadKeyGet( hndl, err, key, pValue );
	return 0;
}

sword ocilibGlobal::ThreadKeyInit( dvoid *hndl, OCIError *err, OCIThreadKey **key, OCIThreadKeyDestFunc destFn )
{
	Method(46);

	if ( m_lpOCIThreadKeyInit != NULL )
		return m_lpOCIThreadKeyInit( hndl, err, key, destFn );
	return 0;
}

sword ocilibGlobal::ThreadKeySet( dvoid *hndl, OCIError *err, OCIThreadKey *key, dvoid *value )
{
	Method(47);

	if ( m_lpOCIThreadKeySet != NULL )
		return m_lpOCIThreadKeySet( hndl, err, key, value );
	return 0;
}

sword ocilibGlobal::ThreadMutexAcquire( dvoid *hndl, OCIError *err, OCIThreadMutex *mutex )
{
	Method(48);

	if ( m_lpOCIThreadMutexAcquire != NULL )
		return m_lpOCIThreadMutexAcquire( hndl, err, mutex );
	return 0;
}

sword ocilibGlobal::ThreadMutexDestroy( dvoid *hndl, OCIError *err, OCIThreadMutex **mutex )
{
	Method(49);

	if ( m_lpOCIThreadMutexDestroy != NULL )
		return m_lpOCIThreadMutexDestroy( hndl, err, mutex );
	return 0;
}

sword ocilibGlobal::ThreadMutexInit( dvoid *hndl, OCIError *err, OCIThreadMutex **mutex )
{
	Method(50);

	if ( m_lpOCIThreadMutexInit != NULL )
		return m_lpOCIThreadMutexInit( hndl, err, mutex );
	return 0;
}

sword ocilibGlobal::ThreadMutexRelease( dvoid *hndl, OCIError *err, OCIThreadMutex *mutex )
{
	Method(51);

	if ( m_lpOCIThreadMutexRelease != NULL )
		return m_lpOCIThreadMutexRelease( hndl, err, mutex );
	return 0;
}

void ocilibGlobal::ThreadProcessInit()
{
	Method(52);

	if ( m_lpOCIThreadProcessInit != NULL )
		m_lpOCIThreadProcessInit();
	return;
}

sword ocilibGlobal::ThreadTerm( dvoid *hndl, OCIError *err )
{
	Method(53);

	if ( m_lpOCIThreadTerm != NULL )
		return m_lpOCIThreadTerm( hndl, err );
	return 0;
}

sword ocilibGlobal::Break( dvoid *hndl, OCIError *err )
{
	Method(54);

	if ( m_lpOCIBreak != NULL )
		return m_lpOCIBreak( hndl, err );
	return 0;
}

sword ocilibGlobal::Reset( dvoid *hndl, OCIError *err )
{
	Method(55);

	if ( m_lpOCIReset != NULL )
		return m_lpOCIReset( hndl, err );
	return 0;
}

sword ocilibGlobal::NumberIsInt( OCIError *err, CONST OCINumber *number, boolean *result )
{
	Method(56);

	if ( m_lpOCINumberIsInt != NULL )
		return m_lpOCINumberIsInt( err, number, result );
	return 0;
}

