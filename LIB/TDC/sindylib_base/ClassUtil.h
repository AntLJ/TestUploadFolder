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

/**
 * @file ClassUtil.h
 * @brief <b>クラス定義用ユーティリティ定義ファイル</b>
 * このファイルには、CRowを継承するクラス定義のための
 * ユーティリティが定義されています。
 * 特に、クラスのコンストラクタ・デストラクタや、コピーオペレータなど
 * 共有の記述が多いものに関して定義が簡単に済むようなマクロを定義して
 * あります。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */

/**
 * @brief <b>レコードベースクラス用のコンストラクタマクロ</b>\n
 * レコードのラップクラスで使用してください。
 */
#define ROWCLASS_CONSTRUCTOR( class_name, table_type ) \
public: \
	class_name() \
	{ \
		Init(); \
		m_emTableType = sindyTableType::table_type; \
	} \
	virtual ~class_name(){ /*DeleteCache();*/ } \
	class_name( const class_name& obj ) \
	{ \
		Init(); \
		*this = obj; \
	} \
	class_name( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::table_type, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() ) \
	{ \
		Init(); \
		SetObject( lp, emTableType, bCreated, spFields, spNameString ); \
	} \
	class_name& operator=( const class_name& obj ) \
	{ \
		obj.CopyTo( *this ); \
		return *this; \
	} \
	class_name& operator=( _IRow* lp ) \
	{ \
		SetObject( lp, sindyTableType::table_type ); \
		return *this; \
	}
