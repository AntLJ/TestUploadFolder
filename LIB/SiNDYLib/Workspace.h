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

#include "../LIB/WinLib/ComWrapper.h"
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

	explicit CWorkspace( IWorkspace* lp )
	{
		*this = lp;
	}
	explicit CWorkspace( LPCTSTR lpcszConnectString )
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
		LPCTSTR lpcszServerName )
	{
		ConnectToSDE( lpcszUserName, lpcszPassword, lpcszVersion, lpcszInstance, lpcszServerName );
	}
	bool operator ==( const CWorkspace& cWorkspace ) const;

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
	 * @brief <b>文字化け対策用</b>\n
	 * 文字化け回避用のコードです。Connectする前に呼び出します。
	 */
	bool PreConnect();

	/**
	 * @brief <b>データベースに接続する</b>\n
	 * 一度接続すると、接続を内部で保持します。
	 * 複数の接続を保持することができます。
	 * SDEとパーソナル・ジオデータベースにのみ対応しています。
	 * @see ConnectToSDE
	 * @see ConnectToPGDB
	 * @param lpcszConnectString	[in]	接続文字列（<em>username</em>/<em>password</em>/<em>version</em>/<em>instance</em>/<em>servername</em>又はPGDBファイル名）
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
	 * @brief テーブルをオープンする
	 * @param lpcszTableName	[in]	テーブル名
	 * @return ITablePtr
	 */
	ITablePtr OpenTable( LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>サーバ名などを管理するメンバの共有コピーを返す</b>\n
	 * m_spNameStringの実体のshared_ptrを返します。
	 * @return boost::shared_ptr<CWorkspaceNameString>
	 */
	CSPWorkspaceNameString GetNameString() const;

	//@{ @name IWorkspaceEditインターフェース（一部）
	/**
	 * @brief <b>編集開始する</b>\n
	 * カスタムポスト用の場合はg_bCustomPostModeフラグを
	 * trueにします。
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
	 * @brief <b>オペレーションの開始</b>\n
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
	 * @brief <b>接続情報をメンバに確保する</b>\n
	 */
//	void InitProperties();
protected:
	mutable CSPWorkspaceNameString m_spNameString;
};
#pragma warning(pop)
} // sindy

#endif // _WORKSPACE_H_
