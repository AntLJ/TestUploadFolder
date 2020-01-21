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
 * @file sindylib_base_classconstructorimpl.h
 * @brief <b>クラス定義用ユーティリティ定義ファイル</b>
 * このファイルには、CRowBaseを継承するクラス定義のための
 * ユーティリティが定義されています。
 * 特に、クラスのコンストラクタ・デストラクタや、コピーオペレータなど
 * 共有の記述が多いものに関して定義が簡単に済むようなマクロを定義して
 * あります。
 * @author コンテンツ事業本部技術開発部システム開発グループ 古川貴宏
 * @version $Id$
 */

/**
 * @brief <b>レコードベースクラス用のコンストラクタマクロ</b>\n
 * レコードのラップクラスで使用してください。
 */
#define SINDYBASE_ROWCLASS_CONSTRUCTOR_BASE( class_name, table_type ) \
protected:\
	explicit class_name() : CRowBase() \
	{ \
		Init(); \
		m_emTableType = sindyTableType::table_type; \
	} \
public: \
	virtual ~class_name(){} \
	explicit class_name( const class_name& obj ) : CRowBase( obj ), CTableFinderSupport(obj.m_cFinder) \
	{ \
		InitRelationalTables(); \
	} \
	class_name( _IRow* lp, const CTableFinder& cFinder, sindyTableType::ECode emTableType = sindyTableType::table_type, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() ) : \
		CRowBase( lp, emTableType, bCreated, spFields, spNameString ), \
		CTableFinderSupport(cFinder) \
	{ \
		InitRelationalTables(); \
	} \
	class_name& operator=( const class_name& obj ) \
	{ \
		CRowBase::operator=( obj ); \
		CContainerBase::operator=( obj ); \
		return *this; \
	} \
	operator _IRow*() const \
	{ \
		return (_IRowPtr)p; \
	} \


#define SINDYBASE_ROWCLASS_CONSTRUCTOR( class_name, parent_class_name, table_type ) \
protected:\
	explicit class_name() : parent_class_name()\
	{ \
	} \
public: \
	virtual ~class_name(){} \
	explicit class_name( const class_name& obj ) : parent_class_name( obj ) \
	{ \
	} \
	class_name( _IRow* lp, const CTableFinder& cFinder, sindyTableType::ECode emTableType = sindyTableType::table_type, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() ) : parent_class_name(lp,cFinder,emTableType,bCreated,spFields, spNameString) \
	{ \
	} \
	class_name& operator=( const class_name& obj ) \
	{ \
		parent_class_name::operator=( obj ); \
		return *this; \
	} \
	operator _IRow*() const \
	{ \
		return (_IRowPtr)p; \
	} \


#define SINDYBASE_TABLECLASS_CONSTRUCTOR_BASE( class_name ) \
protected:\
	explicit class_name() : CTableFinderSupport(){} \
public:\
	virtual ~class_name(){} \
	class_name( const class_name& obj ) : CTableFinderSupport(obj.m_cFinder) { *this = obj; } \
	class_name( ITable* lp, const CTableFinder& cFinder ) : CTableFinderSupport(cFinder), CTableBase( lp ) {} \
	class_name( IFeatureClass* lp, const CTableFinder& cFinder ) : CTableFinderSupport(cFinder), CTableBase( lp ) {} \
	class_name( sindyTableType::ECode emTableType, ITable *lp, const CTableFinder& cFinder ) : CTableFinderSupport(cFinder), CTableBase( emTableType, lp ) {} \
	class_name( sindyTableType::ECode emTableType, IFeatureClass *lp, const CTableFinder& cFinder ) : CTableFinderSupport(cFinder), CTableBase( emTableType, lp ) {} \
	class_name& operator =( ITable* lp ){ clear(); return static_cast<class_name&>(CTableBase::operator=(lp)); } \
	class_name& operator =( IFeatureClass* lp ){ clear(); return static_cast<class_name&>(CTableBase::operator=(lp)); } \
	class_name& operator =( const CTableBase& obj ){ clear(); return static_cast<class_name&>(CTableBase::operator=(obj)); } \
	class_name& operator =( const CTableContainer& obj ){ CContainerBase::operator=(static_cast<const CContainerBase&>(obj)); return static_cast<class_name&>(CTableBase::operator=(obj)); } \

#define SINDYBASE_TABLECLASS_CONSTRUCTOR( class_name, parent_class_name ) \
private:\
	explicit class_name() : parent_class_name() {}\
public:\
	virtual ~class_name(){} \
	class_name( const class_name& obj ) : parent_class_name( obj ) {} \
	class_name( ITable* lp, const CTableFinder& cFinder ) : parent_class_name( lp, cFinder ) {} \
	class_name( IFeatureClass* lp, const CTableFinder& cFinder ) : parent_class_name( lp, cFinder ) {} \
	class_name( sindyTableType::ECode emTableType, ITable *lp, const CTableFinder& cFinder ) : parent_class_name( emTableType, lp, cFinder ) {} \
	class_name( sindyTableType::ECode emTableType, IFeatureClass *lp, const CTableFinder& cFinder ) : parent_class_name( emTableType, lp, cFinder ) {} \
	class_name& operator =( ITable* lp ){ return static_cast<class_name&>(parent_class_name::operator=(lp)); } \
	class_name& operator =( IFeatureClass* lp ){ return static_cast<class_name&>(parent_class_name::operator=(lp)); } \
	class_name& operator =( const CTableBase& obj ){ return static_cast<class_name&>(parent_class_name::operator=(obj)); } \
	class_name& operator =( const class_name& obj ){ return static_cast<class_name&>(parent_class_name::operator=(obj)); } \

