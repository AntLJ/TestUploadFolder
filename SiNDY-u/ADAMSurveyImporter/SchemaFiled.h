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


namespace adam_v2
{
/**
 * フィールド要素
 * スキーマ向け
*/
class SchemaFiled
{
public:
	/**
	 * コンストラクタ
	 *
	 * @param[in]	cursor	ICursorポインタ
	 * @param[in]	schema	オブジェクトクラス名
	 * @deprecated	ITablePtr版を使ってください
	 * 
	 */
	SchemaFiled(const _ICursorPtr& cursor, const uh::tstring& schema) 
	{
		if(cursor)
		{
			cursor->FindField(CComBSTR(schema.c_str()), &index);
		}
		else
		{
			index = 0;
		}
		name = schema;
	}
	/**
	 * コンストラクタ
	 *
	 * @param[in]	ipTable	ITableポインタ
	 * @param[in]	schema	オブジェクトクラス名
	 * 
	 */
	SchemaFiled(const ITablePtr& ipTable, const uh::tstring& schema) 
	{
		if(ipTable)
		{
			ipTable->FindField(CComBSTR(schema.c_str()), &index);
		}
		else
		{
			index = 0;
		}
		name = schema;
	}
	/**
	 * デストラクタ
	 */
	~SchemaFiled(void){}

public:
	long index;			///< インデックス番号
	uh::tstring name;	///< オブジェクトクラス名
};

};