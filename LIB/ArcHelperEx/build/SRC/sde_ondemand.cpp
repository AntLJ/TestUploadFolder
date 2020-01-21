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
#include "sde_ondemand.h"
#include "../../LIB/WinLib/charutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FARPROC SSE_get_func( LPCSTR lpcszFunc )
{
	static HMODULE hSdeModule = NULL;
	if( ! hSdeModule )
	{
		// インストールフォルダ取得
		TCHAR strPath[_MAX_PATH+1] = _T("");
		CRegKey key;
		if( ERROR_SUCCESS == key.Open( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\ESRI\\ArcGIS"), KEY_READ ) )
		{
			// Arc10からレジストリのパスがかわったので、一度RealVersionをとって確認する
			DWORD dwSize = _MAX_PATH;
			TCHAR strRealVersion[_MAX_PATH+1] = _T("");
			if( ERROR_SUCCESS != key.QueryValue( strRealVersion, _T("RealVersion"), &dwSize ) )
				_ASSERTE(false);

			// かっこ悪いが、9.3かどうかで判断する
			CString strVersion = strRealVersion;
			if( strVersion.Left( 3 ) == _T("9.3") )
			{
				dwSize = _MAX_PATH;
				if( ERROR_SUCCESS != key.QueryValue( strPath, _T("InstallDir"), &dwSize ) )
					_ASSERTE(false);
			}
			else
			{
				key.Close();
				// とりあえず、10.0と10.1は以下の方法でいける。今後10.2とかでもいけるだろう。
				// Arc11とかになった場合に、インストールフォルダがDesktop11とかになった場合
				// には、これではだめになるが、そのときにまた直そう
				CString strRegKey = _T("SOFTWARE\\ESRI\\Desktop") + strVersion.Left( 4 );
				if( ERROR_SUCCESS == key.Open( HKEY_LOCAL_MACHINE, strRegKey, KEY_READ ) )
				{
					dwSize = _MAX_PATH;
					if( ERROR_SUCCESS != key.QueryValue( strPath, _T("InstallDir"), &dwSize ) )
						_ASSERTE(false);
				}
			}
		}
		// 取得したフォルダからsde[0-9][0-9].dllを探す
		CString strFilePath( strPath );
		strFilePath += _T("bin\\");
		if( 0 < lstrlen(strPath) )
		{
			if ( ! PathFileExists( strFilePath+_T("sde.dll") ) ) {
				HANDLE hFind = NULL;
				WIN32_FIND_DATA stFind = {0};
				for( hFind = FindFirstFile( strFilePath + _T("sde*.dll"), &stFind ); INVALID_HANDLE_VALUE != hFind; FindNextFile( hFind, &stFind ) )
				{
					CStringA strFileNameP(stFind.cFileName);
					if( isasciinumber(strFileNameP[3]) && isasciinumber(strFileNameP[4]) )
					{
						strFilePath += stFind.cFileName;
						break;
					}
				}
				FindClose( hFind );
			} else
				strFilePath += _T("sde.dll");
		}

		// 探したファイルをロード
		hSdeModule = ::LoadLibrary( strFilePath );
		_ASSERTE( hSdeModule );
	}

	return GetProcAddress( hSdeModule, lpcszFunc );
}
LONG SSE_connection_create (const CHAR *server,const CHAR *instance,const CHAR *database,const CHAR *username,const CHAR *password,SE_ERROR *error,SE_CONNECTION *connection)
{
	CString strInstance(instance);
	CString strPassword(password);
	strInstance.MakeUpper();
	// ダイレクトコネクションならpasswordの後ろに@serverをつける
	if(strInstance.Left(3) == _T("SDE"))
		strPassword += _T("@") + (CString)server;

	typedef LONG (SDEAPI *LPSECONNECTIONCREATEAPI)(const CHAR*,const CHAR*, const CHAR*, const CHAR*, const CHAR*, SE_ERROR*, SE_CONNECTION*);
	static LPSECONNECTIONCREATEAPI api = (LPSECONNECTIONCREATEAPI)SSE_get_func("SE_connection_create");
	return api(server,instance,database,username,CT2A(strPassword),error,connection);
}
void SSE_connection_free (SE_CONNECTION connection)
{
	typedef void (SDEAPI* LPSECONNECTIONFREEAPI)(SE_CONNECTION);
	static LPSECONNECTIONFREEAPI api = (LPSECONNECTIONFREEAPI)SSE_get_func("SE_connection_free");
	api(connection);
}
LONG SSE_connection_get_error_string(SE_CONNECTION connection, LONG error_code, CHAR *error_string)
{
	typedef LONG (SDEAPI *LPSECONNECTIONGETERROR)(SE_CONNECTION,LONG,CHAR*);
	static LPSECONNECTIONGETERROR api = (LPSECONNECTIONGETERROR)SSE_get_func("SE_connection_get_error_string");
	return api(connection,error_code,error_string);
}
LONG SSE_connection_get_ext_error(SE_CONNECTION connection, SE_ERROR* error)
{
	typedef LONG (SDEAPI *LPSECONNECTIONGETEXTERROR)(SE_CONNECTION,SE_ERROR*);
	static LPSECONNECTIONGETEXTERROR api = (LPSECONNECTIONGETEXTERROR)SSE_get_func("SE_connection_get_ext_error");
	return api(connection,error);
}
LONG SSE_stream_free (SE_STREAM stream)
{
	typedef LONG (SDEAPI *LPSESTREAMFREEAPI)(SE_STREAM);
	static LPSESTREAMFREEAPI api = (LPSESTREAMFREEAPI)SSE_get_func("SE_stream_free");
	return api(stream);
}
LONG SSE_stream_num_result_columns(SE_STREAM stream,SHORT *num_columns)
{
	typedef LONG (SDEAPI *LPSTREAMNUMRESULTCOLUMNSAPI)(SE_STREAM,SHORT*);
	static LPSTREAMNUMRESULTCOLUMNSAPI api = (LPSTREAMNUMRESULTCOLUMNSAPI)SSE_get_func("SE_stream_num_result_columns");
	return api(stream,num_columns);
}
LONG SSE_stream_get_integer(SE_STREAM stream,SHORT column,LONG *int_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETINTEGERAPI)(SE_STREAM,SHORT,LONG*);
	static LPSESTREAMGETINTEGERAPI api = (LPSESTREAMGETINTEGERAPI)SSE_get_func("SE_stream_get_integer");
	return api(stream,column,int_val);
}
LONG SSE_stream_get_smallint(SE_STREAM stream,SHORT column,SHORT *short_val)
{
	typedef LONG (SDEAPI *LPSTREAMGETSMALLINTAPI)(SE_STREAM,SHORT,SHORT*);
	static LPSTREAMGETSMALLINTAPI api = (LPSTREAMGETSMALLINTAPI)SSE_get_func("SE_stream_get_smallint");
	return api(stream,column,short_val);
}
LONG SSE_stream_get_float(SE_STREAM stream,SHORT column,FLOAT *float_val)
{
	typedef LONG (SDEAPI *LPSESTREAMGETFLOATAPI)(SE_STREAM,SHORT,FLOAT*);
	static LPSESTREAMGETFLOATAPI api = (LPSESTREAMGETFLOATAPI)SSE_get_func("SE_stream_get_float");
	return api(stream,column,float_val);
}
LONG SSE_stream_get_double(SE_STREAM stream,SHORT column,LFLOAT *double_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETDOBULE)(SE_STREAM,SHORT,LFLOAT*);
	static LPSESTREAMGETDOBULE api = (LPSESTREAMGETDOBULE)SSE_get_func("SE_stream_get_double");
	return api(stream,column,double_val);
}
LONG SSE_stream_get_date(SE_STREAM stream,SHORT column,struct tm *date_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETDATEAPI)(SE_STREAM,SHORT,struct tm*);
	static LPSESTREAMGETDATEAPI api = (LPSESTREAMGETDATEAPI)SSE_get_func("SE_stream_get_date");
	return api(stream,column,date_val);
}
LONG SSE_stream_get_string(SE_STREAM stream,SHORT column,CHAR *string_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETSTRINGAPI)(SE_STREAM,SHORT,CHAR*);
	static LPSESTREAMGETSTRINGAPI api = (LPSESTREAMGETSTRINGAPI)SSE_get_func("SE_stream_get_string");
	return api(stream,column,string_val);
}
LONG SSE_stream_get_blob(SE_STREAM stream,SHORT column,SE_BLOB_INFO *blob_val)
{
	typedef LONG(SDEAPI *LPSDEAPI)(SE_STREAM,SHORT,SE_BLOB_INFO*);
	static LPSDEAPI api = (LPSDEAPI)SSE_get_func("SE_stream_get_blob");
	return api(stream,column,blob_val);
}
LONG SSE_stream_get_nstring(SE_STREAM stream,SHORT column,WCHAR *string_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETNSTRINGAPI)(SE_STREAM,SHORT,WCHAR*);
	static LPSESTREAMGETNSTRINGAPI api = (LPSESTREAMGETNSTRINGAPI)SSE_get_func("SE_stream_get_nstring");
	return api(stream,column,string_val);
}LONG SSE_stream_set_integer(SE_STREAM stream,SHORT column,const LONG *int_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETINTEGERAPI)(SE_STREAM,SHORT,const LONG*);
	static LPSESTREAMGETINTEGERAPI api = (LPSESTREAMGETINTEGERAPI)SSE_get_func("SE_stream_set_integer");
	return api(stream,column,int_val);
}
LONG SSE_stream_set_smallint(SE_STREAM stream,SHORT column,const SHORT *short_val)
{
	typedef LONG (SDEAPI *LPSTREAMGETSMALLINTAPI)(SE_STREAM,SHORT,const SHORT*);
	static LPSTREAMGETSMALLINTAPI api = (LPSTREAMGETSMALLINTAPI)SSE_get_func("SE_stream_set_smallint");
	return api(stream,column,short_val);
}
LONG SSE_stream_set_float(SE_STREAM stream,SHORT column,const FLOAT *float_val)
{
	typedef LONG (SDEAPI *LPSESTREAMGETFLOATAPI)(SE_STREAM,SHORT,const FLOAT*);
	static LPSESTREAMGETFLOATAPI api = (LPSESTREAMGETFLOATAPI)SSE_get_func("SE_stream_set_float");
	return api(stream,column,float_val);
}
LONG SSE_stream_set_double(SE_STREAM stream,SHORT column,const LFLOAT *double_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETDOBULE)(SE_STREAM,SHORT,const LFLOAT*);
	static LPSESTREAMGETDOBULE api = (LPSESTREAMGETDOBULE)SSE_get_func("SE_stream_set_double");
	return api(stream,column,double_val);
}
LONG SSE_stream_set_date(SE_STREAM stream,SHORT column,const struct tm *date_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETDATEAPI)(SE_STREAM,SHORT,const struct tm*);
	static LPSESTREAMGETDATEAPI api = (LPSESTREAMGETDATEAPI)SSE_get_func("SE_stream_set_date");
	return api(stream,column,date_val);
}
LONG SSE_stream_set_string(SE_STREAM stream,SHORT column,const CHAR *string_val)
{
	typedef LONG(SDEAPI *LPSESTREAMGETSTRINGAPI)(SE_STREAM,SHORT,const CHAR*);
	static LPSESTREAMGETSTRINGAPI api = (LPSESTREAMGETSTRINGAPI)SSE_get_func("SE_stream_set_string");
	return api(stream,column,string_val);
}
LONG SSE_stream_set_write_mode(SE_STREAM stream,BOOL buffered)
{
	typedef LONG(SDEAPI *LPSDEAPI)(SE_STREAM,BOOL);
	static LPSDEAPI api = (LPSDEAPI)SSE_get_func("SE_stream_set_write_mode");
	return api(stream,buffered);
}
LONG SSE_stream_describe_column(SE_STREAM stream,SHORT column,SE_COLUMN_DEF *column_def)
{
	typedef LONG(SDEAPI *LPSESTREAMDESCRIBECOLUMNAPI)(SE_STREAM,SHORT,SE_COLUMN_DEF*);
	static LPSESTREAMDESCRIBECOLUMNAPI api = (LPSESTREAMDESCRIBECOLUMNAPI)SSE_get_func("SE_stream_describe_column");
	return api(stream,column,column_def);
}
LONG SSE_stream_fetch (SE_STREAM stream)
{
	typedef LONG(SDEAPI *LPSESTREAMFETCHAPI)(SE_STREAM);
	static LPSESTREAMFETCHAPI api = (LPSESTREAMFETCHAPI)SSE_get_func("SE_stream_fetch");
	return api(stream);
}
LONG SSE_stream_prepare_sql(SE_STREAM stream,const CHAR *sql_stmt)
{
	typedef LONG(SDEAPI *LPSESTREAMPREPARESQLAPI)(SE_STREAM,const CHAR*);
	static LPSESTREAMPREPARESQLAPI api = (LPSESTREAMPREPARESQLAPI)SSE_get_func("SE_stream_prepare_sql");
	return api(stream,sql_stmt);
}
LONG SSE_stream_query(SE_STREAM stream, SHORT num_columns, const CHAR **columns, const SE_SQL_CONSTRUCT *construct)
{
	typedef LONG (SDEAPI *LPSESTREAMQUERYAPI)(SE_STREAM, SHORT, const CHAR**, const SE_SQL_CONSTRUCT*);
	static LPSESTREAMQUERYAPI api = (LPSESTREAMQUERYAPI)SSE_get_func("SE_stream_query");
	return api(stream,num_columns,columns,construct);
}
LONG SSE_stream_create(SE_CONNECTION connection,SE_STREAM *stream)
{
	typedef LONG(SDEAPI *LPSESTREAMCREATEAPI)(SE_CONNECTION,SE_STREAM*);
	static LPSESTREAMCREATEAPI api = (LPSESTREAMCREATEAPI)SSE_get_func("SE_stream_create");
	return api(connection,stream);
}
LONG SSE_stream_execute(SE_STREAM stream)
{
	typedef LONG(SDEAPI *LPSESTREAMEXECUTEAPI)(SE_STREAM);
	static LPSESTREAMEXECUTEAPI api = (LPSESTREAMEXECUTEAPI)SSE_get_func("SE_stream_execute");
	return api(stream);
}
LONG SSE_stream_set_state(SE_STREAM stream,LONG source_id,LONG differences_id,LONG difference_type)
{
	typedef LONG (SDEAPI *LPSESTREAMSETSTATEAPI)(SE_STREAM,LONG,LONG,LONG);
	static LPSESTREAMSETSTATEAPI api = (LPSESTREAMSETSTATEAPI)SSE_get_func("SE_stream_set_state");
	return api(stream,source_id,differences_id,difference_type);
}
LONG SSE_stream_query_with_info(SE_STREAM stream,const SE_QUERYINFO query_info)
{
	typedef LONG (SDEAPI *LPSESTREAMQUERYWITHINFOAPI)(SE_STREAM,SE_QUERYINFO);
	static LPSESTREAMQUERYWITHINFOAPI api = (LPSESTREAMQUERYWITHINFOAPI)SSE_get_func("SE_stream_query_with_info");
	return api(stream,query_info);
}
LONG SSE_stream_get_ext_error(SE_STREAM stream, SE_ERROR* error)
{
	typedef LONG (SDEAPI *LPSESTREAMGETEXTERRORAPI)(SE_STREAM,SE_ERROR*);
	static LPSESTREAMGETEXTERRORAPI api = (LPSESTREAMGETEXTERRORAPI)SSE_get_func("SE_stream_get_ext_error");
	return api(stream,error);
}
void SSE_versioninfo_free (SE_VERSIONINFO version)
{
	typedef void(SDEAPI *LPSEVERSIONINFOFREEAPI)(SE_VERSIONINFO);
	static LPSEVERSIONINFOFREEAPI api = (LPSEVERSIONINFOFREEAPI)SSE_get_func("SE_versioninfo_free");
	api(version);
}
LONG SSE_version_change_state (SE_CONNECTION connection, const SE_VERSIONINFO version, LONG new_state_id)
{
	typedef LONG(SDEAPI *LPSEVERSIONCHANGESTATEAPI)(SE_CONNECTION,const SE_VERSIONINFO,LONG);
	static LPSEVERSIONCHANGESTATEAPI api = (LPSEVERSIONCHANGESTATEAPI)SSE_get_func("SE_version_change_state");
	return api(connection,version,new_state_id);
}
void SSE_version_free_info_list (LONG count, SE_VERSIONINFO *version_list)
{
	typedef void(SDEAPI *LPSEVERSIONFREEINFOLISTAPI)(LONG,SE_VERSIONINFO*);
	static LPSEVERSIONFREEINFOLISTAPI api = (LPSEVERSIONFREEINFOLISTAPI)SSE_get_func("SE_version_free_info_list");
	api(count,version_list);
}
void SSE_state_free_info_list (LONG count, SE_STATEINFO *state_list)
{
	typedef void(SDEAPI *LPSESTATEFREEINFOLISTAPI)(LONG,SE_STATEINFO*);
	static LPSESTATEFREEINFOLISTAPI api = (LPSESTATEFREEINFOLISTAPI)SSE_get_func("SE_state_free_info_list");
	api(count,state_list);
}
LONG SSE_version_get_info_list (SE_CONNECTION connection, const CHAR *where_clause, SE_VERSIONINFO **version_list, LONG *count)
{
	typedef LONG(SDEAPI *LPSEVERSIONGETINFOLISTAPI)(SE_CONNECTION,const CHAR*,SE_VERSIONINFO**,LONG*);
	static LPSEVERSIONGETINFOLISTAPI api = (LPSEVERSIONGETINFOLISTAPI)SSE_get_func("SE_version_get_info_list");
	return api(connection,where_clause,version_list,count);
}
LONG SSE_state_get_info_list (SE_CONNECTION connection, const CHAR *where_clause, SE_STATEINFO **state_list, LONG *count)
{
	typedef LONG(SDEAPI *LPSESTATEGETINFOLISTAPI)(SE_CONNECTION,const CHAR*,SE_STATEINFO**,LONG*);
	static LPSESTATEGETINFOLISTAPI api = (LPSESTATEGETINFOLISTAPI)SSE_get_func("SE_state_get_info_list");
	return api(connection,where_clause,state_list,count);
}
LONG SSE_versioninfo_get_state_id (const SE_VERSIONINFO version, LONG *state_id)
{
	typedef LONG(SDEAPI *LPSEVERSIONINFOGETSTATEIDAPI)(const SE_VERSIONINFO,LONG*);
	static LPSEVERSIONINFOGETSTATEIDAPI api = (LPSEVERSIONINFOGETSTATEIDAPI)SSE_get_func("SE_versioninfo_get_state_id");
	return api(version,state_id);
}
LONG SSE_version_get_info (SE_CONNECTION connection, const CHAR *version_name, SE_VERSIONINFO version)
{
	typedef LONG(SDEAPI *LPSEVERSIONGETINFOAPI)(SE_CONNECTION,const CHAR*,SE_VERSIONINFO);
	static LPSEVERSIONGETINFOAPI api = (LPSEVERSIONGETINFOAPI)SSE_get_func("SE_version_get_info");
	return api(connection,version_name,version);
}
LONG SSE_versioninfo_create (SE_VERSIONINFO *version)
{
	typedef LONG(SDEAPI *LPSEVERSIONINFOCREATEAPI)(SE_VERSIONINFO*);
	static LPSEVERSIONINFOCREATEAPI api = (LPSEVERSIONINFOCREATEAPI)SSE_get_func("SE_versioninfo_create");
	return api(version);
}
LONG SSE_stateinfo_get_parent (const SE_STATEINFO state, LONG *parent_id)
{
	typedef LONG(SDEAPI *LPSESTATEINFOGETPARENTAPI)(const SE_STATEINFO,LONG*);
	static LPSESTATEINFOGETPARENTAPI api = (LPSESTATEINFOGETPARENTAPI)SSE_get_func("SE_stateinfo_get_parent");
	return api(state,parent_id);
}
LONG SSE_stateinfo_get_id (const SE_STATEINFO state, LONG *id)
{
	typedef LONG(SDEAPI *LPSESTATEINFOGETIDAPI)(const SE_STATEINFO,LONG*);
	static LPSESTATEINFOGETIDAPI api = (LPSESTATEINFOGETIDAPI)SSE_get_func("SE_stateinfo_get_id");
	return api(state,id);
}
LONG SSE_stateinfo_get_creation_time (const SE_STATEINFO state, struct tm *created_time)
{
	typedef LONG(SDEAPI *LPSESTATEINFOGETCREATIONTIMEAPI)(const SE_STATEINFO,struct tm*);
	static LPSESTATEINFOGETCREATIONTIMEAPI api = (LPSESTATEINFOGETCREATIONTIMEAPI)SSE_get_func("SE_stateinfo_get_creation_time");
	return api(state,created_time);
}
LONG SSE_sql_construct_alloc(LONG num_tables,SE_SQL_CONSTRUCT **constructor)
{
	typedef LONG (SDEAPI* LPSESQLCONSTRUCTALLOCAPI)(LONG,SE_SQL_CONSTRUCT**);
	static LPSESQLCONSTRUCTALLOCAPI api = (LPSESQLCONSTRUCTALLOCAPI)SSE_get_func("SE_sql_construct_alloc");
	return api(num_tables,constructor);
}
void SSE_sql_construct_free (SE_SQL_CONSTRUCT *constructor)
{
	typedef void (SDEAPI* LPSESQLCONSTRUCTFREEAPI)(SE_SQL_CONSTRUCT*);
	static LPSESQLCONSTRUCTFREEAPI api = (LPSESQLCONSTRUCTFREEAPI)SSE_get_func("SE_sql_construct_free");
	api(constructor);
}
LONG SSE_queryinfo_create (SE_QUERYINFO *query_info)
{
	typedef LONG (SDEAPI* LPSEQUERYINFOCREATEAPI)(SE_QUERYINFO*);
	static LPSEQUERYINFOCREATEAPI api = (LPSEQUERYINFOCREATEAPI)SSE_get_func("SE_queryinfo_create");
	return api(query_info);
}
LONG SSE_queryinfo_set_tables(SE_QUERYINFO query_info,const LONG num_tables,const CHAR **tables,const CHAR **aliases)
{
	typedef LONG (SDEAPI* LPSEQUERYINFOSETTABLESAPI)(SE_QUERYINFO,const LONG,const CHAR**,const CHAR**);
	static LPSEQUERYINFOSETTABLESAPI api = (LPSEQUERYINFOSETTABLESAPI)SSE_get_func("SE_queryinfo_set_tables");
	return api(query_info,num_tables,tables,aliases);
}
LONG SSE_queryinfo_set_columns(SE_QUERYINFO query_info,const LONG num_columns,const CHAR **columns)
{
	typedef LONG (SDEAPI* LPSEQUERYINFOSETCOLUMNSAPI)(SE_QUERYINFO,const LONG,const CHAR**);
	static LPSEQUERYINFOSETCOLUMNSAPI api = (LPSEQUERYINFOSETCOLUMNSAPI)SSE_get_func("SE_queryinfo_set_columns");
	return api(query_info,num_columns,columns);
}
LONG SSE_queryinfo_set_where_clause(SE_QUERYINFO query_info,const CHAR *where_clause)
{
	typedef LONG (SDEAPI* LPSEQUERYINFOSETWHERECLAUSEAPI)(SE_QUERYINFO,const CHAR*);
	static LPSEQUERYINFOSETWHERECLAUSEAPI api = (LPSEQUERYINFOSETWHERECLAUSEAPI)SSE_get_func("SE_queryinfo_set_where_clause");
	return api(query_info,where_clause);
}
LONG SSE_queryinfo_get_where_clause(const SE_QUERYINFO query_info,CHAR *where_clause)
{
	typedef LONG (SDEAPI* LPSEQUERYINFOGETWHERECLAUSEAPI)(SE_QUERYINFO,CHAR*);
	static LPSEQUERYINFOGETWHERECLAUSEAPI api = (LPSEQUERYINFOGETWHERECLAUSEAPI)SSE_get_func("SE_queryinfo_get_where_clause");
	return api(query_info,where_clause);
}
LONG SSE_queryinfo_set_by_clause(SE_QUERYINFO query_info,const CHAR *order_by)
{
	typedef LONG (SDEAPI* LPSEQUERYINFOSETBYCLAUSEAPI)(SE_QUERYINFO,const CHAR*);
	static LPSEQUERYINFOSETBYCLAUSEAPI api = (LPSEQUERYINFOSETBYCLAUSEAPI)SSE_get_func("SE_queryinfo_set_by_clause");
	return api(query_info,order_by);
}
void SSE_queryinfo_free (SE_QUERYINFO query_info)
{
	typedef void (SDEAPI* LPSEQUERYINFOFREEAPI)(SE_QUERYINFO);
	static LPSEQUERYINFOFREEAPI api = (LPSEQUERYINFOFREEAPI)SSE_get_func("SE_queryinfo_free");
	api(query_info);
}
LONG SSE_table_create_mv_view(SE_CONNECTION connection,const CHAR *table,const CHAR *mv_view)
{
	typedef LONG (SDEAPI *LPSETABLECREATEMVVIEWAPI)(SE_CONNECTION,const CHAR*,const CHAR*);
	static LPSETABLECREATEMVVIEWAPI api = (LPSETABLECREATEMVVIEWAPI)SSE_get_func("SE_table_create_mv_view");
	return api(connection,table,mv_view);
}
LONG SSE_table_delete_mv_view(SE_CONNECTION connection,const CHAR *table)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_CONNECTION,const CHAR*);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_table_delete_mv_view");
	return api(connection,table);
}

LONG SSE_connection_get_release(SE_CONNECTION connection,SE_RELEASE *rls_version)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_CONNECTION,SE_RELEASE*);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_connection_get_release");
	return api(connection,rls_version);
}

LONG SSE_coordref_create (SE_COORDREF *coordref)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_COORDREF*);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_coordref_create");
	return api(coordref);
}

LONG SSE_coordref_set_by_id (SE_COORDREF coordref, LONG coordsys_id)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_COORDREF,LONG);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_coordref_set_by_id");
	return api(coordref,coordsys_id);
}

LONG SSE_coordref_set_xy_by_envelope (SE_COORDREF coordref, SE_ENVELOPE *extent)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_COORDREF,SE_ENVELOPE*);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_coordref_set_xy_by_envelope");
	return api(coordref,extent);
}

LONG SSE_coordref_set_m_by_range (SE_COORDREF coordref, LFLOAT min_mvalue, LFLOAT max_mvalue)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_COORDREF,LFLOAT,LFLOAT);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_coordref_set_m_by_range");
	return api(coordref,min_mvalue,max_mvalue);
}

LONG SSE_coordref_set_z_by_range (SE_COORDREF coordref, LFLOAT min_zvalue, LFLOAT max_zvalue)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_COORDREF,LFLOAT,LFLOAT);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_coordref_set_z_by_range");
	return api(coordref,min_zvalue,max_zvalue);
}

void SSE_coordref_free (SE_COORDREF coordref)
{
	typedef void (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_COORDREF);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_coordref_free");
	return api(coordref);
}

LONG SSE_shape_create (SE_COORDREF coordref, SE_SHAPE *shape)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_COORDREF,SE_SHAPE*);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_create");
	return api(coordref,shape);
}

LONG SSE_shape_generate_from_WKB (const CHAR *WKB_type, SE_SHAPE shape)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(const CHAR*,SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_generate_from_WKB");
	return api(WKB_type,shape);
}

LONG SSE_shape_generate_from_binary (const CHAR *binary_type, SE_SHAPE shape)
{
	typedef LONG (SDEAPI *LPSDEAPI)(const CHAR*,SE_SHAPE);
	static LPSDEAPI api = (LPSDEAPI)SSE_get_func("SE_shape_generate_from_binary");
	return api(binary_type,shape);
}

LONG SSE_shape_set_coordref (SE_SHAPE shape, SE_COORDREF coordref)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_SHAPE,SE_COORDREF);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_set_coordref");
	return api(shape,coordref);
}

LONG SSE_shape_get_coordref (SE_SHAPE shape, SE_COORDREF coordref)
{
	typedef LONG (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_SHAPE,SE_COORDREF);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_get_coordref");
	return api(shape,coordref);
}

BOOL SSE_shape_is_containing (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape)
{
	typedef BOOL (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(const SE_SHAPE,const SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_is_containing");
	return api(primary_shape,secondary_shape);
}

BOOL SSE_shape_is_crossing (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape)
{
	typedef BOOL (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(const SE_SHAPE,const SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_is_crossing");
	return api(primary_shape,secondary_shape);
}

BOOL SSE_shape_is_disjoint (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape)
{
	typedef BOOL (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(const SE_SHAPE,const SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_is_disjoint");
	return api(primary_shape,secondary_shape);
}

BOOL SSE_shape_is_equal (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape)
{
	typedef BOOL (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(const SE_SHAPE,const SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_is_equal");
	return api(primary_shape,secondary_shape);
}

BOOL SSE_shape_is_overlapping (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape)
{
	typedef BOOL (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(const SE_SHAPE,const SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_is_overlapping");
	return api(primary_shape,secondary_shape);
}

BOOL SSE_shape_is_touching (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape)
{
	typedef BOOL (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(const SE_SHAPE,const SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_is_touching");
	return api(primary_shape,secondary_shape);
}

BOOL SSE_shape_is_within (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape)
{
	typedef BOOL (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(const SE_SHAPE,const SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_is_within");
	return api(primary_shape,secondary_shape);
}

void SSE_shape_free (SE_SHAPE shape)
{
	typedef void (SDEAPI *LPSETABLEDELETEMVVIEWAPI)(SE_SHAPE);
	static LPSETABLEDELETEMVVIEWAPI api = (LPSETABLEDELETEMVVIEWAPI)SSE_get_func("SE_shape_free");
	return api(shape);
}
LONG SSE_stream_set_spatial_constraints(SE_STREAM stream, SHORT search_order, BOOL calc_masks, SHORT num_filters, const SE_FILTER *filters)
{
	typedef LONG (SDEAPI *LPSTREAMSETSPATIALCONSTRAINTSAPI)(SE_STREAM, SHORT, BOOL, SHORT, const SE_FILTER*);
	static LPSTREAMSETSPATIALCONSTRAINTSAPI api = (LPSTREAMSETSPATIALCONSTRAINTSAPI)SSE_get_func("SE_stream_set_spatial_constraints");
	return api(stream, search_order, calc_masks, num_filters, filters);
}
