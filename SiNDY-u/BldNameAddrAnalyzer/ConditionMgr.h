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

#include <TDC/sindylib_base/IndexedContainer.h>

/// 処理対象条件管理用クラス
class Condition
{
public:
	/// コンストラクタ（封印）
	Condition(){}
	
	/// 追加
	void add( const CString& key, const CComVariant& val );
	/// キー群取得
	const sindy::KeyList keys() const { return keys_; } 
	/// 値群取得
	const sindy::ValueList values() const { return values_; }
	/// 表示用
	CString getQuery( IWorkspacePtr work ) const ;
private:
	/// 条件チェック
	void check( const CString& key, const CComVariant& val );

	sindy::KeyList keys_;      //!< キー群
	sindy::ValueList values_;  //!< 値群
};

/// 処理対象条件群管理用クラス
class ConditionMgr
{
public:
	ConditionMgr(){}
	virtual ~ConditionMgr(void){}

	void init( const CString& filePath );

	std::vector<Condition>::const_iterator begin() const { return conditions.cbegin(); }
	std::vector<Condition>::const_iterator end() const { return conditions.cend(); }

private:
	void add( const Condition& con ){ conditions.push_back(con); }

	std::vector<Condition> conditions;
};

