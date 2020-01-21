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
 * @file Workspace.h
 * @brief CWorkspaceクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinLib/ComWrapper.h"
#include "ErrorCode.h"
#include "NameString.h"

namespace sindy {
/**
 * @class CWorkspace
 * @brief ワークスペース接続管理クラス
 */
#pragma warning(push)
#pragma warning(disable:4275)
#pragma warning(disable:4251)
class SINDYLIB_API CWorkspace : public CComWrapper<IWorkspace>
{
public:
	explicit CWorkspace();
	virtual ~CWorkspace();

	explicit CWorkspace( IWorkspace* lp ) : m_bTrace(true)
	{
		*this = lp;
	}
	explicit CWorkspace( LPCTSTR lpcszConnectString ) : m_bTrace(true)
	{
		Connect( lpcszConnectString );
	}
	CWorkspace& operator=( IWorkspace* lp )
	{
		AtlComPtrAssign( (IUnknown**)&p, lp );

		// 接続文字列クリア
		m_spNameString.reset( (CWorkspaceNameString*)NULL );

		return *this;
	}

	CWorkspace( LPCTSTR lpcszUserName, 
		LPCTSTR lpcszPassword, 
		LPCTSTR lpcszVersion, 
		LPCTSTR lpcszInstance, 
		LPCTSTR lpcszServerName ) : m_bTrace(true)
	{
		ConnectToSDE( lpcszUserName, lpcszPassword, lpcszVersion, lpcszInstance, lpcszServerName );
	}

	bool operator ==( const CWorkspace& cWorkspace ) const;

	// TODO: VC9を考慮して、NULL使用。C++11以降に上げたらexplicitも追加してください。
	bool operator ! () const { return (p == NULL); }
	operator bool () const { return ( p != NULL); }

	operator IWorkspace* () const { return p; }

	//@{ @name IWorkspaceインターフェース
	IPropertySetPtr GetConnectionProperies() const;
	IEnumDatasetNamePtr GetDatasetNames( esriDatasetType DatasetType ) const;
	IEnumDatasetPtr GetDatasets( esriDatasetType DatasetType ) const;
	errorcode::sindyErrCode ExecuteSQL( LPCTSTR lpcszSqlStatement ) const;
	bool Exists() const;
	bool IsDirectory() const;
	LPCTSTR GetPathName( LPTSTR lpszPathName, int nLen ) const;
	esriWorkspaceType GetType() const;
	IWorkspaceFactoryPtr GetWorkspaceFactory() const;
	//@}

	/**
	 * @brief <b>データベースに接続する</b>
	 *
	 * 一度接続すると、接続を内部で保持します。<br>
	 * 複数の接続を保持することができます。<br>
	 * 対応する接続先は以下のとおり：
	 *
	 * @li ArcSDE
	 * @li PostGIS（PostGISExtension.dll経由）
	 * @li Personal GeoDatabase
	 * @li File GeoDatabse
	 *
	 * FGDBとPGDBはファイルパスを指定しますが、SDEとPostGISは接続文字列を指定します。<br>
	 * SDEとPostGISの接続文字列を区別するには、以下のようにします。
	 *
	 * SDEの場合<br>
	 * SDE/ronly/ronly/SDE.DEFAULT/5151/onyx or<br>
	 * ronly/ronly/SDE.DEFAULT/5151/onyx
	 *
	 * PostGIS（PostGISExtension.dllを使う）の場合<br>
	 * PGIS/postgres/postgres/postgis/5432/localhost
	 *
	 * RDBへのダイレクトコネクトの場合<br>
	 * SDE/postgres/postgres/postgis/sde:postgresql:localhost,5432/localhost
	 *
	 * なお、PostGISの場合、SDEではバージョン名のところがDB名になります。<br>
	 * また、先頭のSDEやPGISを付けない場合は以前と同様SDEへの接続とみなします。
	 * 
	 * @see ConnectToSDE
	 * @see ConnectToPGDB
	 * @see ConnectToPGIS
	 * @see ConnectToRDB
	 *
	 * @param lpcszConnectString	[in]	接続文字列（<em>type</em>/<em>username</em>/<em>password</em>/<em>version</em>/<em>instance</em>/<em>servername</em>又はPGDBファイル名）
	 * @return 接続できたらtrue
	 */
	bool Connect( LPCTSTR lpcszConnectString );

	/**
	 * @brief <b>SDEデータベースに接続する</b>\n
	 * 既に接続されていたら何もしません。
	 * @param lpcszUserName		[in]	接続ユーザ名
	 * @param lpcszPassword		[in]	接続ユーザパスワード
	 * @param lpcszVersion		[in]	接続バージョン
	 * @param lpcszInstance		[in]	接続インスタンス
	 * @param lpcszServerName	[in]	接続サーバ名
	 * @return 接続できたらtrue
	 */
	bool ConnectToSDE( LPCTSTR lpcszUserName, 
		LPCTSTR lpcszPassword, 
		LPCTSTR lpcszVersion, 
		LPCTSTR lpcszInstance, 
		LPCTSTR lpcszServerName );
	/**
	 * @brief パーソナル・ジオデータベースに接続する
	 * 既に接続されていたら何もしません。
	 * @param lpcszPGDBFileName
	 * @return 接続できたらtrue
	 */
	bool ConnectToPGDB( LPCTSTR lpcszPGDBFileName );

	/**
	 * @biref OLEDBに接続する
	 * 既に接続されていたら何もしません。
	 * @param lpcszConnectString [in] 接続文字列
	 * @return 接続できたらtrue
	 */
	bool ConnectToOLEDB( LPCTSTR lpcszConnectString );

	/**
	 * @brief Shapeに接続する
	 * 既に接続されていたら何もしません。
	 * @param lpcszShapeFileName [in] 接続文字列(フォルダ)
	 * @return 接続できたらtrue
	 */
	bool ConnectToShape( LPCTSTR lpcszShapeFileName );

	/**
	 * @brief PostGISに接続する
	 *
	 * 既に接続されていたら何もしません。
	 * @param lpcszUserName		[in]	接続ユーザ名
	 * @param lpcszPassword		[in]	接続ユーザパスワード
	 * @param lpcszDB			[in]	接続DB名
	 * @param lpcszInstance		[in]	接続インスタンス
	 * @param lpcszServerName	[in]	接続サーバ名
	 * @return 接続できたらtrue
	 */
	bool ConnectToPGIS( LPCTSTR lpcszUserName, 
		LPCTSTR lpcszPassword, 
		LPCTSTR lpcszDB, 
		LPCTSTR lpcszInstance, 
		LPCTSTR lpcszServerName );

	/**
	 * @brief テーブルをオープンする
	 * @param lpcszTableName	[in]	テーブル名
	 * @return ITablePtr
	 */
	ITablePtr OpenTable( LPCTSTR lpcszTableName ) const;

	/**
	 * @brief テーブルをオープンする
	 * 
	 * 接続オーナー名の所有するテーブルをオープンします。<br>
	 * ファイルベースの場合は、OpenTableと同じ動作です。
	 * return ITablePtr
	 */
	ITablePtr OpenSameOwnersTable( LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>サーバ名などを管理するメンバの共有コピーを返す</b>\n
	 * m_spNameStringの実体のshared_ptrを返します。
	 * @return boost::shared_ptr<CWorkspaceNameString>
	 */
	CSPWorkspaceNameString GetNameString() const;

	//@{ @name IWorkspaceEditインターフェース（一部）
	/**
	 * @brief <b>編集開始する</b>\n
	 * @param bWithUndoRedo	[in]	Undo/Redoを有効にするならtrue
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode StartEditing( bool bWithUndoRedo ) const;

	/**
	 * @brief <b>編集を終了する</b>\n
	 * sindyErr_EditMustBeReconcileが返った場合はリコンサイル後に
	 * 再度保存する必要があります。
	 * @param bSaveEdits	[in]	編集を保存するならtrue
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode StopEditing( bool bSaveEdits ) const;

	/**
	 * @brief <b>編集が既に開始されているかどうか</b>\n
	 * @retval true 編集が開始されている
	 * @retval false 編集が開始されていない
	 */
	bool IsBeingEdited() const;

	/**
	 * @brief <b>オペレーションの開始</b>\n
	 */
	errorcode::sindyErrCode StartEditOperation() const;

	/**
	 * @brief <b>オペレーションの破棄</b>\n
	 */
	errorcode::sindyErrCode AbortEditOperation() const;

	/**
	 * @brief <b>オペレートの終了</b>\n
	 */
	errorcode::sindyErrCode StopEditOperation() const;
	//@}

	//@{ @name ITransactionインターフェース
	/// トランザクションを開始する
	errorcode::sindyErrCode StartTransaction() const;
	/// トランザクションをコミットする
	errorcode::sindyErrCode CommitTransaction() const;
	/// トランザクションを破棄する
	errorcode::sindyErrCode AbortTransaction() const;
	//@}

	/**
	 * @brief パーソナル・ジオデータベース名かどうかを判断する
	 * 最後の4文字が「.mdb」かどうかで判断します。
	 * @param lpcszString	[in]	判定文字列
	 * @return パーソナル・ジオデータベースのファイル名ならtrue
	 */
	static bool IsPGDBFileName( LPCTSTR lpcszString );

	/**
	 * @brief OLEDB接続文字列名かどうかを判断する
	 * 最初の文字列が「Provider=」かどうかで判断します。
	 * @param lpcszString [in] 判定文字列
	 * @return OLEDBの接続文字列ならtrue
	 */
	static bool IsOLEDBConnectString( LPCTSTR lpcszString );

	/**
	 * @brief Shapeかどうかを判断する
	 * フォルダかどうかで判断します。
	 * @param lpcszString [in] 判定文字列
	 * @return Shapeの接続文字列（フォルダ）ならtrue
	 */
	static bool IsShapeFileName( LPCTSTR lpcszString );

public:
	bool m_bTrace; //!< 接続時のメッセージを出すかどうか（デフォルト：true）
protected:
	mutable CSPWorkspaceNameString m_spNameString;
};
#pragma warning(pop)
} // sindy

#endif // _WORKSPACE_H_
