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
 * @file Workspaces.h
 * @brief CWorkspaces クラスのインターフェイス
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _WORKSPACES_H_
#define _WORKSPACES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "AnnotationClassTable.h"
#include "Workspace.h"

namespace sindy {
//using namespace sindy::annotation_class;

/**
 * @class CWorkspaces
 * @brief <b>ワークスペース接続管理クラス</b>
 * ワークスペースへの接続を管理します。
 * 内部で複数の接続を保持することが可能です。
 *
 * このクラスは主にチェッカーなどのArcMapを使用しない
 * アプリケーションで使用されます。
 * @warning このクラスはexportしていないメンバ変数を宣言していますが、privateなため外から
 * アクセスされることはありません。\n
 * なお、上記のような場合その旨ワーニングが出るのですが、ウザイのでワーニングを消しています。
 */
#pragma warning(push)
#pragma warning(disable : 4251)

class SINDYLIB_API CWorkspaces
{
public:
	CWorkspaces(){};
	virtual ~CWorkspaces(){};

public:
	/**
	 * @brief <b>文字化け対策用</b>\n
	 * 文字化け回避用のコードです。Connectする前に呼び出します。
	 */
	bool PreConnect();

	/**
	 * @brief データベースに接続する
	 * 一度接続すると、接続を内部で保持します。
	 * 複数の接続を保持することができます。
	 * SDEとパーソナル・ジオデータベースにのみ対応しています。
	 * @see ConnectToSDE
	 * @see ConnectToPGDB
	 * @param lpcszConnectString	[in]	接続文字列（<em>username</em>/<em>password</em>/<em>version</em>/<em>instance</em>/<em>servername</em>又はPGDBファイル名）
	 * @return IWorkspacePtr
	 */
	CWorkspace* Connect( LPCTSTR lpcszConnectString );

	/**
	 * @brief SDEデータベースに接続する
	 * 既に接続が存在する場合は既存の接続を返します。
	 * @param lpcszUserName		[in]	接続ユーザ名
	 * @param lpcszPassword		[in]	接続ユーザパスワード
	 * @param lpcszVersion		[in]	接続バージョン
	 * @param lpcszInstance		[in]	接続インスタンス
	 * @param lpcszServerName	[in]	接続サーバ名
	 * @return IWorkspacePtr
	 */
	CWorkspace* ConnectToSDE( LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName );

	/**
	 * @brief パーソナル・ジオデータベースに接続する
	 * 既に接続が存在する場合は既存の接続を返します。
	 * @param lpcszPGDBFileName
	 * @return IWorkspacePtr
	 */
	CWorkspace* ConnectToPGDB( LPCTSTR lpcszPGDBFileName );

	/**
	 * @brief 既存の接続を追加する
	 * @retval 引数がNULLの場合はsindyErr_COMInterfaceIsNullを返す
	 */
	errorcode::sindyErrCode AddConnection( IWorkspacePtr ipWorkspace );

	/**
	 * @brief <b>注記ルールテーブルクラスの参照を返す</b>\n
	 * まだルールワークスペースに接続していない場合はルールワークスペースに
	 * 接続してからテーブルを返します。\n
	 * ルールワークスペースへ接続する際にはSiNDY-eのインストールフォルダ内
	 * にあるsettings.iniファイルの中のservernameを参照します。
	 * このサーバ名で既に接続が存在する場合はその接続を利用します。
	 * もし、記述がないか、あるいはSiNDY-eがインストールされていなければ
	 * 既存の接続から探します。それでもない場合には、接続を促すダイアログ
	 * を表示します。\n
	 * 動的な接続を含むため、constな関数ではありません。
	 * @return 注記ルールテーブル
	 */
//	annotation_class::CAnnotationClassTable& GetAnnotationClassTable();

	/**
	 * @brief <b>ルール用ワークスペースへ接続する</b>\n
	 * まだルールワークスペースに接続していない場合はルールワークスペースに
	 * 接続します。既に接続がある場合は、それを返します。\n
	 * ルールワークスペースへ接続する際にはSiNDY-eのインストールフォルダ内
	 * にあるsettings.iniファイルの中のservernameを参照します。
	 * このサーバ名で既に接続が存在する場合はその接続を利用します。
	 * もし、記述がないか、あるいはSiNDY-eがインストールされていなければ
	 * 既存の接続から探します。それでもない場合には、接続を促すダイアログ
	 * を表示します。\n
	 * @retval const CWorkspace* ルールワークスペース
	 * @retval NULL 接続することができなかった
	 */
	const CWorkspace* CWorkspaces::ConnectToRuleWorkspace();
protected:
	/**
	 * @brief 既存の接続を探す
	 * @param lpcszUserName		[in]	接続ユーザ名
	 * @param lpcszVersion		[in]	バージョン
	 * @param lpcszServerName	[in]	サーバ名
	 * @return 接続が存在しなければNULLを返す
	 */
	const CWorkspace* FindWorkspace( LPCTSTR lpcszUserName,  LPCTSTR lpcszVersion, LPCTSTR lpcszServerName ) const;

	/**
	 * @brief 既存の接続を探す
	 * @param ipWorkspace		[in]	IWorkspacePtr
	 * @return 接続が存在しなければNULLを返す
	 */
	const CWorkspace* FindWorkspace( IWorkspacePtr ipWorkspace ) const;

	/**
	 * @brief 既存の接続をサーバ名で探す
	 * @param lpcszServerName	[in]	サーバ名
	 * @return 接続が存在しなければNULLを返す
	 */
	const CWorkspace* FindWorkspaceByServerName( LPCTSTR lpcszServerName ) const;

	/**
	 * @brief パーソナル・ジオデータベース名かどうかを判断する
	 * 最後の4文字が「.mdb」かどうかで判断します。
	 * @param lpcszString	[in]	判定文字列
	 * @return パーソナル・ジオデータベースのファイル名ならtrue
	 */
	bool IsPGDBFileName( LPCTSTR lpcszString ) const;
private:
	IDispatchPtr								m_ipDisp;					//!< IApplicationとか
	std::list<CWorkspace>						m_listWorkspace;			//!< 接続済みワークスペースのリスト
	annotation_class::CAnnotationClassTable		m_cAnnotationClassTable;	//!< 注記ルールテーブル
};
#pragma warning(pop)

} // sindy

#endif // _WORKSPACES_H_
