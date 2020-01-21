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

#ifndef _SDE_ONDEMAND_H_
#define _SDE_ONDEMAND_H_

#include <sdetype.h>
#include <sdeerno.h>

LONG SSE_connection_create (const CHAR *server,const CHAR *instance,const CHAR *database,const CHAR *username,const CHAR *password,SE_ERROR *error,SE_CONNECTION *connection);
void SSE_connection_free (SE_CONNECTION connection);
LONG SSE_connection_get_error_string(SE_CONNECTION connection, LONG error_code, CHAR *error_string);
LONG SSE_connection_get_ext_error(SE_CONNECTION connection, SE_ERROR* error);
LONG SSE_stream_create(SE_CONNECTION connection,SE_STREAM *stream);
LONG SSE_stream_get_integer(SE_STREAM stream,SHORT column,LONG *int_val);
LONG SSE_stream_get_smallint(SE_STREAM stream,SHORT column,SHORT *short_val);
LONG SSE_stream_get_float(SE_STREAM stream,SHORT column,FLOAT *float_val);
LONG SSE_stream_get_double(SE_STREAM stream,SHORT column,LFLOAT *double_val);
LONG SSE_stream_get_date(SE_STREAM stream,SHORT column,struct tm *date_val);
LONG SSE_stream_get_string(SE_STREAM stream,SHORT column,CHAR *string_val);
LONG SSE_stream_get_blob(SE_STREAM stream,SHORT column,SE_BLOB_INFO *blob_val);
LONG SSE_stream_get_nstring(SE_STREAM stream,SHORT column,WCHAR *string_val);
LONG SSE_stream_set_integer(SE_STREAM stream,SHORT column,const LONG *int_val);
LONG SSE_stream_set_smallint(SE_STREAM stream,SHORT column,const SHORT *short_val);
LONG SSE_stream_set_float(SE_STREAM stream,SHORT column,const FLOAT *float_val);
LONG SSE_stream_set_double(SE_STREAM stream,SHORT column,const LFLOAT *double_val);
LONG SSE_stream_set_date(SE_STREAM stream,SHORT column,const struct tm *date_val);
LONG SSE_stream_set_string(SE_STREAM stream,SHORT column,const CHAR *string_val);
LONG SSE_stream_get_nstring(SE_STREAM stream,SHORT column,WCHAR *string_val);
LONG SSE_stream_set_write_mode(SE_STREAM stream,BOOL buffered);
LONG SSE_stream_describe_column(SE_STREAM stream,SHORT column,SE_COLUMN_DEF *column_def);
LONG SSE_stream_fetch (SE_STREAM stream);
LONG SSE_stream_prepare_sql(SE_STREAM stream,const CHAR *sql_stmt);
LONG SSE_stream_query(SE_STREAM stream, SHORT num_columns, const CHAR **columns, const SE_SQL_CONSTRUCT *construct);
LONG SSE_stream_execute(SE_STREAM stream);
LONG SSE_stream_set_state(SE_STREAM stream,LONG source_id,LONG differences_id,LONG difference_type);
LONG SSE_stream_query_with_info(SE_STREAM stream,const SE_QUERYINFO query_info);
LONG SSE_stream_get_ext_error(SE_STREAM stream, SE_ERROR* error);
LONG SSE_stream_free (SE_STREAM stream);
LONG SSE_stream_num_result_columns(SE_STREAM stream,SHORT *num_columns);
LONG SSE_version_change_state (SE_CONNECTION connection, const SE_VERSIONINFO version, LONG new_state_id);
void SSE_version_free_info_list (LONG count, SE_VERSIONINFO *version_list);
LONG SSE_version_get_info_list (SE_CONNECTION connection, const CHAR *where_clause, SE_VERSIONINFO **version_list, LONG *count);
LONG SSE_version_get_info (SE_CONNECTION connection, const CHAR *version_name, SE_VERSIONINFO version);
LONG SSE_versioninfo_create (SE_VERSIONINFO *version);
LONG SSE_versioninfo_get_state_id (const SE_VERSIONINFO version, LONG *state_id);
void SSE_versioninfo_free (SE_VERSIONINFO version);
void SSE_state_free_info_list (LONG count, SE_STATEINFO *state_list);
LONG SSE_state_get_info_list (SE_CONNECTION connection, const CHAR *where_clause, SE_STATEINFO **state_list, LONG *count);
LONG SSE_stateinfo_get_parent (const SE_STATEINFO state, LONG *parent_id);
LONG SSE_stateinfo_get_id (const SE_STATEINFO state, LONG *id);
LONG SSE_stateinfo_get_creation_time (const SE_STATEINFO state, struct tm *created_time);
LONG SSE_table_create_mv_view(SE_CONNECTION connection,const CHAR *table,const CHAR *mv_view);
LONG SSE_table_delete_mv_view(SE_CONNECTION connection,const CHAR *table);
LONG SSE_sql_construct_alloc(LONG num_tables,SE_SQL_CONSTRUCT **constructor);
void SSE_sql_construct_free (SE_SQL_CONSTRUCT *constructor);
LONG SSE_queryinfo_create (SE_QUERYINFO *query_info);
LONG SSE_queryinfo_set_tables(SE_QUERYINFO query_info,const LONG num_tables,const CHAR **tables,const CHAR **aliases);
LONG SSE_queryinfo_set_columns(SE_QUERYINFO query_info,const LONG num_columns,const CHAR **columns);
LONG SSE_queryinfo_set_where_clause(SE_QUERYINFO query_info,const CHAR *where_clause);
LONG SSE_queryinfo_get_where_clause(const SE_QUERYINFO query_info,CHAR *where_clause);
LONG SSE_queryinfo_set_by_clause(SE_QUERYINFO query_info,const CHAR *order_by);
void SSE_queryinfo_free (SE_QUERYINFO query_info);
LONG SSE_connection_get_release(SE_CONNECTION connection,SE_RELEASE *rls_version);

LONG SSE_coordref_create (SE_COORDREF *coordref);
LONG SSE_coordref_set_by_id (SE_COORDREF coordref, LONG coordsys_id);
LONG SSE_coordref_set_xy_by_envelope (SE_COORDREF coordref, SE_ENVELOPE *extent);
LONG SSE_coordref_set_m_by_range (SE_COORDREF coordref, LFLOAT min_mvalue, LFLOAT max_mvalue);
LONG SSE_coordref_set_z_by_range (SE_COORDREF coordref, LFLOAT min_zvalue, LFLOAT max_zvalue);
void SSE_coordref_free (SE_COORDREF coordref);

LONG SSE_shape_create (SE_COORDREF coordref, SE_SHAPE *shape);
LONG SSE_shape_generate_from_WKB (const CHAR *WKB_type, SE_SHAPE shape);
LONG SSE_shape_generate_from_binary (const CHAR *binary_type, SE_SHAPE shape);
LONG SSE_shape_set_coordref (SE_SHAPE shape, SE_COORDREF coordref);
LONG SSE_shape_get_coordref (SE_SHAPE shape, SE_COORDREF coordref);
BOOL SSE_shape_is_containing (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape);
BOOL SSE_shape_is_crossing (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape);
BOOL SSE_shape_is_disjoint (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape);
BOOL SSE_shape_is_equal (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape);
BOOL SSE_shape_is_overlapping (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape);
BOOL SSE_shape_is_touching (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape);
BOOL SSE_shape_is_within (const SE_SHAPE primary_shape, const SE_SHAPE secondary_shape);
void SSE_shape_free (SE_SHAPE shape);

LONG SSE_stream_set_spatial_constraints(SE_STREAM stream, SHORT search_order, BOOL calc_masks, SHORT num_filters, const SE_FILTER *filters);

#endif // ifndef _SDE_ONDEMAND_H_
