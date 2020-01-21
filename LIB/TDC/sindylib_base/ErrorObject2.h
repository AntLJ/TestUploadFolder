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
 * @file ErrorObjectBase.h
 * @brief <b>CErrorObjectBase、CErrorObjectsBase定義ファイル</b>\n
 * @author 技術開発部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _ERROROBJECTBASE_H_
#define _ERROROBJECTBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SiNDYLogFunctions.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include <boost/tuple/tuple.hpp>

namespace sindy {

/**
 * @struct stErrorInfoBase
 * @brief <b>エラー情報格納構造体</b>
 */
struct stErrorInfoBase
{
	IGeometryPtr			m_ipErrGeom;	//!< エラー形状\nフィーチャの形状の参照は危険なので、必ずコピーを作成すること
	long					m_lFieldIndex;	//!< フィールドインデックス番号
	CString					m_strMsg;		//!< エラーメッセージ
	errorcode::sindyErrCode	m_emErr;		//!< エラーコード
	long					m_lScale;		//!< エラー参照スケール
	sindyErrLevel			m_emLevel;		//!< エラーレベル
};

/**
 * @class CErrorInfoBase
 * @brief <b>エラー情報格納クラス</b>
 */
class CErrorInfoBase
{
	friend class CErrorInfoBase;
public:
	CErrorInfoBase();
	virtual ~CErrorInfoBase();

	CErrorInfoBase& operator=( const CErrorInfoBase& cInfo );
	CErrorInfoBase( const CErrorInfoBase& cInfo ){ *this = cInfo; }

	CErrorInfoBase& operator=( const stErrorInfoBase& stInfo );
	CErrorInfoBase( const stErrorInfoBase& stInfo ){ *this = stInfo; }

	CErrorInfoBase& operator=( errorcode::sindyErrCode emErr ) { init(); SetErrorCode( emErr ); return *this; }
	CErrorInfoBase( errorcode::sindyErrCode emErr ){ *this = emErr; }

	/**
	 * @brief 全ての値をセットできるコンストラクタ
	 *
	 * @note emErrがsindyErr_NoErrの場合はemLevelの値に関わらず
	 * 常にsindyErrLevel_INFOがセットされます
	 *
	 * @param emErr			[in]			エラーコード
	 * @param emLevel		[in]			エラーレベル
	 * @param ipGeom		[in,optional]	エラー時の参照形状（コピー推奨、デフォルト：NULL）
	 * @param lFieldIndex	[in,optional]	エラーの原因となったフィールドのインデックス番号（デフォルト：-1）
	 * @param lScale		[in,optional]	エラーを参照する時の推奨スケール値（デフォルト：-1）
	 * @param lpcszFormat	[in,optional]	エラーメッセージ用フォーマット（デフォルト：NULL）
	 * @param ...			[in,optional]	エラーメッセージ（可変長引数）
	 */
	CErrorInfoBase( errorcode::sindyErrCode emErr, sindyErrLevel emLevel, IGeometry* ipGeom = (IGeometry*)NULL, long lFieldIndex = -1, long lScale = -1, LPCTSTR lpcszFormat = NULL, ... );

	void SetErrorCode( errorcode::sindyErrCode emErr ){ m_stInfo.m_emErr = emErr; }
	void SetErrorLevel( sindyErrLevel emLevel ){ m_stInfo.m_emLevel = emLevel; }
	void SetErrorGeometry( IGeometry* ipGeom ){ m_stInfo.m_ipErrGeom = ipGeom; }
	void SetErrorField( long lFieldIndex ){ m_stInfo.m_lFieldIndex = lFieldIndex; }
	void SetErrorScale( long lScale ){ m_stInfo.m_lScale = lScale; }
	void SetErrorMessage( LPCTSTR lpcszFormat, ... );
	errorcode::sindyErrCode GetErrorCode() const { return m_stInfo.m_emErr; }
	sindyErrLevel GetErrorLevel() const { return m_stInfo.m_emLevel; }
	const IGeometryPtr GetErrorGeometry() const { return m_stInfo.m_ipErrGeom; }
	long GetErrorField() const { return m_stInfo.m_lFieldIndex; }
	long GetErrorScale() const { return m_stInfo.m_lScale; }
	LPCTSTR GetErrorMessage() const { return m_stInfo.m_strMsg; }
	/**
	* @brief エラー情報を取得する
	* @return 「エラーコードメッセージ（エラーメッセージ）」の形式の文字列
	*/
	CString ToString() const;
protected:
	void SetErrorMessageV( LPCTSTR lpcszFormat, va_list args );
private:
	void init();
private:
#pragma warning(push)
#pragma warning(disable:4251)
	stErrorInfoBase m_stInfo;
#pragma warning(pop)
};

typedef boost::shared_ptr<CErrorInfoBase> CSPErrorInfoBase;

/**
 * @class CErrorInfosBase
 * @brief <b>エラー情報格納コンテナ</b>
 */
class CErrorInfosBase
{
	friend class CErrorInfosBase;
public:
	CErrorInfosBase();
	virtual ~CErrorInfosBase();

	CErrorInfosBase( const CErrorInfosBase& cInfos ){ *this = cInfos; }
	CErrorInfosBase& operator=( const CErrorInfosBase& cInfos );
	//@{ @name コンテナ操作系
	typedef std::list<CSPErrorInfoBase>::iterator iterator;
	typedef std::list<CSPErrorInfoBase>::const_iterator const_iterator;
	typedef std::list<CSPErrorInfoBase>::reverse_iterator reverse_iterator;
	typedef std::list<CSPErrorInfoBase>::const_reverse_iterator const_reverse_iterator;
	iterator begin(){ return m_listInfo.begin(); }
	iterator end(){ return m_listInfo.end(); }
	reverse_iterator rbegin(){ return m_listInfo.rbegin(); }
	reverse_iterator rend(){ return m_listInfo.rend(); }
	const_iterator begin() const { return m_listInfo.begin(); }
	const_iterator end() const { return m_listInfo.end(); }
	const_reverse_iterator rbegin() const { return m_listInfo.rbegin(); }
	const_reverse_iterator rend() const { return m_listInfo.rend(); }
	CSPErrorInfoBase push_back( const CSPErrorInfoBase& cInfo );
	void clear();
	bool empty() const { return m_listInfo.empty(); }
	//@}
	errorcode::sindyErrCode GetDefaultErrCode( sindyErrLevel* pLevel = NULL ) const;
	/**
	* @brief エラー情報を文字列として取得する
	* @param [in] bUnique 重複するメッセージを省く場合はtrue
	* @return コンテナ内の全てのエラー情報
	*/
	CString ToString( bool bUnique = true ) const;
private:
#pragma warning(push)
#pragma warning(disable:4251)
	std::list<CSPErrorInfoBase> m_listInfo;
#pragma warning(pop)
};

/**
 * @class CErrorObjectBase
 * @brief <b>エラーオブジェクトクラス</b>
 * このクラスはフィーチャ・レコード単位でのエラーを複数格納するクラスです。\n
 * SiNDY形式のログを出力する機能を有します。その場合、エラーが複数あれば
 * 複数行エラーを出力します。
 */
class CErrorObjectBase
{
public:
	CErrorObjectBase();
	virtual ~CErrorObjectBase();

	CErrorObjectBase( const CErrorObjectBase& cErrObj ){ *this = cErrObj; }
	CErrorObjectBase( const CRowBase& cRow ){ *this = cRow; }
	CErrorObjectBase( const CRowBase& cRow, const CSPErrorInfoBase& cInfo );
	CErrorObjectBase& operator=( const CErrorObjectBase& cErrObj );
	CErrorObjectBase& operator=( const CRowBase& cRow );

	void SetErrorObject( const CRowBase& cRow );
	void SetErrorObject( const boost::tuple<CString, long, CGeometryBase>& obj );
	const boost::tuple<CString, long, CGeometryBase>* GetErrorObject() const { return &m_ObjData; }
	virtual void Print( FILE* out, sindyLogFormat emFormat = sindyLogFormat_Common, sindyLogLevel emLevel = sindyLogLevel_ERR ) const;
	//@{ @name コンテナ操作系
	typedef CErrorInfosBase::iterator iterator;
	typedef CErrorInfosBase::const_iterator const_iterator;
	iterator begin(){ return m_cInfos.begin(); }
	iterator end(){ return m_cInfos.end(); }
	const_iterator begin() const { return m_cInfos.begin(); }
	const_iterator end() const { return m_cInfos.end(); }
	CSPErrorInfoBase& push_back( const CSPErrorInfoBase& cInfo );
	CErrorInfosBase& push_back( const CErrorInfosBase& cInfos );
	//@}
	errorcode::sindyErrCode GetDefaultErrCode( sindyErrLevel* pLevel = NULL ) const { return m_cInfos.GetDefaultErrCode( pLevel ); }
protected:
#pragma warning(push)
#pragma warning(disable:4251)
	boost::tuple<CString, long, CGeometryBase> m_ObjData;	//!< boost::tuple<テーブル名,オブジェクトID,形状>
	CErrorInfosBase	m_cInfos;
#pragma warning(pop)
};

typedef boost::shared_ptr<CErrorObjectBase> CSPErrorObjectBase;

/**
 * @class CErrorObjects
 * @brief <b>エラーオブジェクト格納コンテナクラス</b>
 */
class CErrorObjectsBase
{
public:
	explicit CErrorObjectsBase();
	virtual ~CErrorObjectsBase();

	//@{ @name コンテナ操作系
	typedef std::list<CSPErrorObjectBase>::iterator iterator;
	typedef std::list<CSPErrorObjectBase>::const_iterator const_iterator;
	typedef std::list<CSPErrorObjectBase>::reverse_iterator reverse_iterator;
	typedef std::list<CSPErrorObjectBase>::const_reverse_iterator const_reverse_iterator;
	iterator begin(){ return m_listErrorObject.begin(); }
	iterator end(){ return m_listErrorObject.end(); }
	reverse_iterator rbegin(){ return m_listErrorObject.rbegin(); }
	reverse_iterator rend(){ return m_listErrorObject.rend(); }
	const_iterator begin() const { return m_listErrorObject.begin(); }
	const_iterator end() const { return m_listErrorObject.end(); }
	const_reverse_iterator rbegin() const { return m_listErrorObject.rbegin(); }
	const_reverse_iterator rend() const { return m_listErrorObject.rend(); }
	virtual CSPErrorObjectBase& push_back( const CSPErrorObjectBase& obj );
	void clear();
	bool empty() const { return m_listErrorObject.empty(); }
	//@} 
	errorcode::sindyErrCode GetDefaultErrCode( sindyErrLevel* pLevel = NULL ) const;
	LPCTSTR ToString();
	void SetFileHandle( FILE* out, sindyLogFormat emFormat = sindyLogFormat_Common, sindyLogLevel emLevel = sindyLogLevel_INFO, bool bInstantly = true ){
		m_hFile = out;
		m_emFormat = emFormat;
		m_emLevel = emLevel;
		m_bOutputInstantly = bInstantly;
	}
private:
	/// メンバ変数を全て初期化する
	void init( bool bIsConstructor = false );
protected:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list<CSPErrorObjectBase>	m_listErrorObject;	//!< エラーオブジェクト格納コンテナ
	CString					m_strErr;			//!< 文字列変換用
	bool					m_bOutputInstantly;	//!< リアルタイムログ出力をするかどうかのフラグ
	sindyLogFormat			m_emFormat;			//!< ログ出力フォーマット
	sindyLogLevel			m_emLevel;			//!< ログ出力レベル
	FILE*					m_hFile;			//!< ログ出力ファイルハンドル
#pragma warning(pop)
};

} // sindy

#endif // ifndef _ERROROBJECTBASE_H_
