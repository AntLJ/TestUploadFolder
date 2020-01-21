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

#ifndef	__OCILIBGLOBAL_H_
#define	__OCILIBGLOBAL_H_

class ocilibGlobal
{
public:
	ocilibGlobal();
	virtual ~ocilibGlobal();

public:
	sword ErrorGet( dvoid *hndlp,ub4 recordno,text *sqlstate,sb4 *errcodep,text *bufp,ub4 bufsiz,ub4 type );
	sword EnvCreate( OCIEnv **envhpp,ub4 mode,CONST dvoid *ctxp, size_t xtramemsz,dvoid **usrmempp );
	sword HandleAlloc( CONST dvoid *parenth, dvoid **hndlpp, ub4 type, size_t xtramem_sz, dvoid **usrmempp );
	sword ServerAttach( OCIServer *srvhp,OCIError *errhp,CONST text *dblink,sb4 dblink_len,ub4 mode );
	sword AttrGet(CONST dvoid *trgthndlp,ub4 trghndltyp,dvoid *attributep,ub4 *sizep,ub4 attrtype,OCIError *errhp);
	sword AttrSet( dvoid *trgthndlp,ub4 trghndltyp,dvoid *attributep,ub4 size,ub4 attrtype,OCIError *errhp );
	sword SessionBegin( OCISvcCtx *svchp,OCIError *errhp,OCISession *usrhp,ub4 credt,ub4 mode );
	sword SessionEnd( OCISvcCtx *svchp,OCIError *errhp,OCISession *usrhp,ub4 mode );
	sword ServerDetach( OCIServer *srvhp,OCIError *errhp,ub4 mode );
	sword HandleFree( dvoid *hndlp,ub4 type );
	sword StmtPrepare( OCIStmt *stmtp,OCIError *errhp,CONST text *stmt,ub4 stmt_len,ub4 language,ub4 mode );
	sword StmtExecute( OCISvcCtx *svchp,OCIStmt *stmtp,OCIError *errhp,ub4 iters,ub4 rowoff,CONST OCISnapshot *snap_in,OCISnapshot *snap_out,ub4 mode );
	sword ParamGet( CONST dvoid *hndlp,ub4 htype,OCIError *errhp,dvoid **parmdpp,ub4 pos );
	sword DescriptorFree( dvoid *descp,ub4 type );
	sword DefineByPos( OCIStmt *stmtp,OCIDefine **defnpp,OCIError *errhp,ub4 position,dvoid *valuep,sb4 value_sz,ub2 dty,dvoid *indp,ub2 *rlenp,ub2 *rcodep,ub4 mode );
	sword StmtFetch( OCIStmt *stmthp,OCIError *errhp,ub4 nrows,ub2 orientation,ub4 mode );
	sword StmtFetch2( OCIStmt *stmthp,OCIError *errhp,ub4 nrows,ub2 orientation,sb4 fetchOffset,ub4 mode );
	sword NumberToReal( OCIError *err,CONST OCINumber *number,uword rsl_length,dvoid *rsl );
	sword NumberToInt( OCIError *err,CONST OCINumber *number,uword rsl_length,uword rsl_flag,dvoid *rsl );
	sword NumberToText( OCIError *err,CONST OCINumber *number,CONST OraText *fmt,ub4 fmt_length,CONST OraText *nls_params,ub4 nls_p_length,ub4 *buf_size,text *buf );
	sword NumberFromReal( OCIError *err,CONST dvoid *rnum,uword rnum_length,OCINumber *number );
	sword NumberFromInt( OCIError *err,CONST dvoid *inum,uword inum_length,uword inum_s_flag,OCINumber *number );
	void  NumberSetZero( OCIError *err,OCINumber *num );
	sword DateSysDate( OCIError *err,OCIDate *sys_date );
	sword BindByName( OCIStmt *stmtp,OCIBind **bindpp,OCIError *errhp,CONST text *placeholder,sb4 placeh_len,dvoid *valuep,sb4 value_sz,ub2 dty,dvoid *indp,ub2 *alenp,ub2 *rcodep,ub4 maxarr_len,ub4 *curelep,ub4 mode );
	sword NlsGetInfo(CONST dvoid *parenth, CONST dvoid *hndlpp,text *sqlstate,sb4 errcodep,ub2 mdoe);
	sword WideChar2MultiByte(dvoid *envhp, OraText *dst, CONST OCIWchar *src, size_t *rsize);
	sword MultiByte2WideChar(dvoid *envhp, OCIWchar *dst, CONST OraText *src, size_t *rsize);
	sword StringAssignText(OCIEnv *env, OCIError *err, CONST text *rhs, ub4 rhs_len, OCIString **lhs );
	sword ThreadHndInit(dvoid *hndl, OCIError *err, OCIThreadHandle **thnd);
	sword ThreadInit(dvoid *hndl, OCIError *err);
	sword ThreadClose( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd );
	sword ThreadCreate( dvoid *hndl, OCIError *err, dvoid *arg, OCIThreadId *tid, OCIThreadHandle *tHnd );
	sword ThreadHandleGet( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd );
	sword ThreadHndDestroy( dvoid *hndl, OCIError *err, OCIThreadHandle **thnd );
	sword ThreadIdDestroy( dvoid *hndl, OCIError *err, OCIThreadId **tid );
	sword ThreadIdGet( dvoid *hndl, OCIError *err, OCIThreadId *tid );
	sword ThreadIdInit( dvoid *hndl, OCIError *err, OCIThreadId **tid );
	sword ThreadIdNull( dvoid *hndl, OCIError *err, OCIThreadId *tid, boolean *result );
	sword ThreadIdSame( dvoid *hndl, OCIError *err, OCIThreadId *tid1, OCIThreadId *tid2, boolean *result );
	sword ThreadIdSet( dvoid *hndl, OCIError *err, OCIThreadId *tidDest, OCIThreadId *tidSrc );
	sword ThreadIdSetNull( dvoid *hndl, OCIError *err, OCIThreadId *tid );
	boolean ThreadIsMulti();
	sword ThreadJoin( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd );
	sword ThreadKeyDestroy( dvoid *hndl, OCIError *err, OCIThreadKey **key );
	sword ThreadKeyGet( dvoid *hndl, OCIError *err, OCIThreadKey *key, dvoid **pValue );
	sword ThreadKeyInit( dvoid *hndl, OCIError *err, OCIThreadKey **key, OCIThreadKeyDestFunc destFn );
	sword ThreadKeySet( dvoid *hndl, OCIError *err, OCIThreadKey *key, dvoid *value );
	sword ThreadMutexAcquire( dvoid *hndl, OCIError *err, OCIThreadMutex *mutex );
	sword ThreadMutexDestroy( dvoid *hndl, OCIError *err, OCIThreadMutex **mutex );
	sword ThreadMutexInit( dvoid *hndl, OCIError *err, OCIThreadMutex **mutex );
	sword ThreadMutexRelease( dvoid *hndl, OCIError *err, OCIThreadMutex *mutex );
	void ThreadProcessInit();
	sword ThreadTerm( dvoid *hndl, OCIError *err );
	sword Break( dvoid *hndlp, OCIError *errhp );
	sword Reset( dvoid *hndlp, OCIError *errhp );
	sword NumberIsInt( OCIError *err, CONST OCINumber *number, boolean *result );

private:
	void Method(long method);

private:
	HINSTANCE		m_ociInstance;

	sword (WINAPIV *m_lpOCIErrorGet)( dvoid *hndlp,ub4 recordno,text *sqlstate,sb4 *errcodep,text *bufp,ub4 bufsiz,ub4 type );
	sword (WINAPIV *m_lpOCIEnvCreate)( OCIEnv **envhpp,ub4 mode,CONST dvoid *ctxp,CONST dvoid *(*malocfp)(dvoid *ctxp,size_t size),CONST dvoid *(*ralocfp)(dvoid *ctxp,dvoid *memptr,size_t newsize),CONST void (*mfreefp)(dvoid *ctxp,dvoid *memptr), size_t xtramemsz,dvoid **usrmempp );
	sword (WINAPIV *m_lpOCIHandleAlloc)( CONST dvoid *parenth,dvoid **hndlpp,ub4 type,size_t xtramem_sz,dvoid **usrmempp );
	sword (WINAPIV *m_lpOCIServerAttach)( OCIServer *srvhp,OCIError *errhp,CONST text *dblink,sb4 dblink_len,ub4 mode );
	sword (WINAPIV *m_lpOCIAttrGet)(CONST dvoid *trgthndlp,ub4 trghndltyp,dvoid *attributep,ub4 *sizep,ub4 attrtype,OCIError *errhp);
	sword (WINAPIV *m_lpOCIAttrSet)( dvoid *trgthndlp,ub4 trghndltyp,dvoid *attributep,ub4 size,ub4 attrtype,OCIError *errhp );
	sword (WINAPIV *m_lpOCISessionBegin)( OCISvcCtx *svchp,OCIError *errhp,OCISession *usrhp,ub4 credt,ub4 mode );
	sword (WINAPIV *m_lpOCISessionEnd)( OCISvcCtx *svchp,OCIError *errhp,OCISession *usrhp,ub4 mode );
	sword (WINAPIV *m_lpOCIServerDetach)( OCIServer *srvhp,OCIError *errhp,ub4 mode );
	sword (WINAPIV *m_lpOCIHandleFree)( dvoid *hndlp,ub4 type );
	sword (WINAPIV *m_lpOCIStmtPrepare)( OCIStmt *stmtp,OCIError *errhp,CONST text *stmt,ub4 stmt_len,ub4 language,ub4 mode );
	sword (WINAPIV *m_lpOCIStmtExecute)( OCISvcCtx *svchp,OCIStmt *stmtp,OCIError *errhp,ub4 iters,ub4 rowoff,CONST OCISnapshot *snap_in,OCISnapshot *snap_out,ub4 mode );
	sword (WINAPIV *m_lpOCIParamGet)( CONST dvoid *hndlp,ub4 htype,OCIError *errhp,dvoid **parmdpp,ub4 pos );
	sword (WINAPIV *m_lpOCIDescriptorFree)( dvoid *descp,ub4 type );
	sword (WINAPIV *m_lpOCIDefineByPos)( OCIStmt *stmtp,OCIDefine **defnpp,OCIError *errhp,ub4 position,dvoid *valuep,sb4 value_sz,ub2 dty,dvoid *indp,ub2 *rlenp,ub2 *rcodep,ub4 mode );
	sword (WINAPIV *m_lpOCIStmtFetch)( OCIStmt *stmthp,OCIError *errhp,ub4 nrows,ub2 orientation,ub4 mode );
	sword (WINAPIV *m_lpOCIStmtFetch2)( OCIStmt *stmthp,OCIError *errhp,ub4 nrows,ub2 orientation,sb4 fetchOffset,ub4 mode );
	sword (WINAPIV *m_lpOCINumberToReal)( OCIError *err,CONST OCINumber *number,uword rsl_length,dvoid *rsl );
	sword (WINAPIV *m_lpOCINumberToInt)( OCIError *err,CONST OCINumber *number,uword rsl_length,uword rsl_flag,dvoid *rsl );
	sword (WINAPIV *m_lpOCINumberToText)( OCIError *err,CONST OCINumber *number,CONST OraText *fmt,ub4 fmt_length,CONST OraText *nls_params,ub4 nls_p_length,ub4 *buf_size,text *buf );
	sword (WINAPIV *m_lpOCINumberFromReal)( OCIError *err,CONST dvoid *rnum,uword rnum_length,OCINumber *number );
	sword (WINAPIV *m_lpOCINumberFromInt)( OCIError *err,CONST dvoid *inum,uword inum_length,uword inum_s_flag,OCINumber *number );
	void (WINAPIV *m_lpOCINumberSetZero)( OCIError *err,OCINumber *num );
	sword (WINAPIV *m_lpOCIDateSysDate)( OCIError *err,OCIDate *sys_date );
	sword (WINAPIV *m_lpOCIBindByName)( OCIStmt *stmtp,OCIBind **bindpp,OCIError *errhp,CONST text *placeholder,sb4 placeh_len,dvoid *valuep,sb4 value_sz,ub2 dty,dvoid *indp,ub2 *alenp,ub2 *rcodep,ub4 maxarr_len,ub4 *curelep,ub4 mode );
	sword (WINAPIV *m_lpOCINlsGetInfo)( CONST dvoid *parenth, CONST dvoid *hndlpp,text *sqlstate,sb4 errcodep,ub2 mdoe);
	sword (WINAPIV *m_lpOCIWideCharToMultiByte)(dvoid *envhp, OraText *dst, CONST OCIWchar *src, size_t *rsize);
	sword (WINAPIV *m_lpOCIMultiByteToWideChar)(dvoid *envhp, OCIWchar *dst, CONST OraText *src, size_t *rsize);
	sword (WINAPIV *m_lpOCIStringAssignText)(OCIEnv *env, OCIError *err, CONST text *rhs, ub4 rhs_len, OCIString **lhs );
	sword (WINAPIV *m_lpOCIThreadHndInit)(dvoid *hndl, OCIError *err, OCIThreadHandle **thnd);
	sword (WINAPIV *m_lpOCIThreadInit)(dvoid *hndl, OCIError *err);
	sword (WINAPIV *m_lpOCIThreadClose)( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd );
	sword (WINAPIV *m_lpOCIThreadCreate)( dvoid *hndl, OCIError *err, dvoid *arg, OCIThreadId *tid, OCIThreadHandle *tHnd );
	sword (WINAPIV *m_lpOCIThreadHandleGet)( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd );
	sword (WINAPIV *m_lpOCIThreadHndDestroy)( dvoid *hndl, OCIError *err, OCIThreadHandle **thnd );
	sword (WINAPIV *m_lpOCIThreadIdDestroy)( dvoid *hndl, OCIError *err, OCIThreadId **tid );
	sword (WINAPIV *m_lpOCIThreadIdGet)( dvoid *hndl, OCIError *err, OCIThreadId *tid );
	sword (WINAPIV *m_lpOCIThreadIdInit)( dvoid *hndl, OCIError *err, OCIThreadId **tid );
	sword (WINAPIV *m_lpOCIThreadIdNull)( dvoid *hndl, OCIError *err, OCIThreadId *tid, boolean *result );
	sword (WINAPIV *m_lpOCIThreadIdSame)( dvoid *hndl, OCIError *err, OCIThreadId *tid1, OCIThreadId *tid2, boolean *result );
	sword (WINAPIV *m_lpOCIThreadIdSet)( dvoid *hndl, OCIError *err, OCIThreadId *tidDest, OCIThreadId *tidSrc );
	sword (WINAPIV *m_lpOCIThreadIdSetNull)( dvoid *hndl, OCIError *err, OCIThreadId *tid );
	boolean (WINAPIV *m_lpOCIThreadIsMulti)();
	sword (WINAPIV *m_lpOCIThreadJoin)( dvoid *hndl, OCIError *err, OCIThreadHandle *tHnd );
	sword (WINAPIV *m_lpOCIThreadKeyDestroy)( dvoid *hndl, OCIError *err, OCIThreadKey **key );
	sword (WINAPIV *m_lpOCIThreadKeyGet)( dvoid *hndl, OCIError *err, OCIThreadKey *key, dvoid **pValue );
	sword (WINAPIV *m_lpOCIThreadKeyInit)( dvoid *hndl, OCIError *err, OCIThreadKey **key, OCIThreadKeyDestFunc destFn );
	sword (WINAPIV *m_lpOCIThreadKeySet)( dvoid *hndl, OCIError *err, OCIThreadKey *key, dvoid *value );
	sword (WINAPIV *m_lpOCIThreadMutexAcquire)( dvoid *hndl, OCIError *err, OCIThreadMutex *mutex );
	sword (WINAPIV *m_lpOCIThreadMutexDestroy)( dvoid *hndl, OCIError *err, OCIThreadMutex **mutex );
	sword (WINAPIV *m_lpOCIThreadMutexInit)( dvoid *hndl, OCIError *err, OCIThreadMutex **mutex );
	sword (WINAPIV *m_lpOCIThreadMutexRelease)( dvoid *hndl, OCIError *err, OCIThreadMutex *mutex );
	void (WINAPIV *m_lpOCIThreadProcessInit)();
	sword (WINAPIV *m_lpOCIThreadTerm)( dvoid *hndl, OCIError *err );
	sword (WINAPIV *m_lpOCIBreak)(dvoid *hndlp, OCIError *errhp);
	sword (WINAPIV *m_lpOCIReset)(dvoid *hndlp, OCIError *errhp);
	sword (WINAPIV *m_lpOCINumberIsInt)( OCIError *err, CONST OCINumber *number, boolean *result );
};

#endif	// __OCILIBGLOBAL_H_
