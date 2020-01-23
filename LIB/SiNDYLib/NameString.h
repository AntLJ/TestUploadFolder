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
 * @file NameString.h
 * @brief <b>オブジェクト名称クラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏\n
 * $Id$
 */
#ifndef _NAMESTRING_H_
#define _NAMESTRING_H_

#include "exportimport.h"
#include <boost/shared_ptr.hpp>

namespace sindy {
/**
 * @brief <b>ワークスペースを一意に特定するための文字列を管理するクラス</b>\n
 * このクラスは、主にCRowクラスでの名称キャッシュ機構の一環として作成されました。\n
 * 従って、文字列のみを管理しIWorkspace等のCOMオブジェクトは管理対象外です。\n
 * 
 * 取得・管理できる情報は以下のとおり：
 * @li オーナー名（SDEの場合のみ）
 * @li インスタンス名（SDEの場合のみ）
 * @li サーバ名（SDEの場合のみ）
 * @li バージョン名（SDEの場合のみ）
 * @li ワークスペースパス名（ファイルベースの場合）
 *
 * SDEベースかファイルベースかを判断する場合には、IsSDE関数を使用してください。
 */
class SINDYLIB_API CWorkspaceNameString
{
public:
	CWorkspaceNameString();
	virtual ~CWorkspaceNameString();

	CWorkspaceNameString( IUnknown* ipUnk )
	{
		SetName( ipUnk );
	}

	bool operator==( const CWorkspaceNameString& obj ) const {
		return IsSameSource( obj );
	}

	bool operator <( const CWorkspaceNameString& obj ) const;

	/**
	 * @brief <b>ワークスペースから文字列を取得する</b>\n
	 * ipUnkは次のインターフェースのどれかをサポートしている必要があります：
	 * @li IWorkspace
	 * @li IPropertySet
	 * @li IDataset
	 * @li ITable
	 * @li IRow
	 * @param ipUnk		[in]	オブジェクト
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool SetName( IUnknown* ipUnk );

	/** 
	 * @brief <b>接続ユーザ名を取得する</b>\n
	 * 既に情報がセットされている必要があります。
	 * @return 接続ユーザ名（大文字固定）
	 */
	LPCTSTR GetUserName() const {
		return m_strUserName;
	}

	/** 
	 * @brief <b>サーバー名を取得する</b>\n
	 * 既に情報がセットされている必要があります。
	 * @return サーバー名（子文字固定）
	 */
	LPCTSTR GetServerName() const {
		return m_strServerName;
	}

	/**
	 * @brief <b>インスタンスを取得する</b>\n
	 * 既に情報がセットされている必要があります。
	 * @return インスタンス文字列
	 */
	LPCTSTR GetInstanceName() const {
		return m_strInstanceName;
	}

	/**
	 * @brief <b>バージョン名を取得する</b>\n
	 * 既に情報がセットされている必要があります。
	 * @return バージョン名
	 */
	LPCTSTR GetVersionName() const {
		return m_strVersionName;
	}

	/**
	 * @brief <b>ファイルベースのワークスペースのパスを取得する</b>\n
	 * ファイルベースかSDEベースかを判断するにはIsSDEを使用してください。\n
	 * 既に情報がセットされている必要があります。
	 * @return パス名
	 */
	LPCTSTR GetPathName() const {
		if( ! IsSDE() )
			return GetConnectString();
		else
			return (LPCTSTR)NULL;
	}

	/**
	 * @brief <b>接続文字列を取得する</b>\n
	 * ここで言う接続文字列とは、SiNDYLibで定義した独自形式の
	 * 文字列のことです。
	 * @return 接続文字列（フォーマット：user[owner.version]@server:instance又はPGDBファイルパス）
	 */
	LPCTSTR GetConnectString() const {
		return m_strConnectString;
	}

	/**
	 * @brief <b>ファイルベースかSDEベースかを判断する</b>\n
	 * 既に情報がセットされている必要があります。
	 * @retval true SDEベース
	 * @retval false ファイルベース
	 */
	bool IsSDE() const {
		return m_bIsSDE;
	}

	/**
	 * @brief <b>同じソースかどうかを判断する</b>
	 * 接続プロパティで判断します。
	 */
	bool IsSameSource( const CWorkspaceNameString& cNameString ) const;

private:
#pragma warning(push)
#pragma warning(disable:4251)
	CString m_strUserName;
	CString m_strServerName;
	CString m_strInstanceName;
	CString m_strVersionName;
	CString m_strConnectString;
#pragma warning(pop)
	bool m_bIsSDE;
};

typedef boost::shared_ptr<CWorkspaceNameString> CSPWorkspaceNameString;

/**
 * @brief <b>テーブルを一意に特定するための文字列を管理するクラス</b>\n
 * このクラスは、主にCRowクラスでの名称キャッシュ機構の一環として作成されました。\n
 * 従って、文字列のみを管理しITable等のCOMオブジェクトは管理対象外です。\n
 * 
 * 取得・管理できる情報は以下のとおり：
 * @li 接続ユーザ名（SDEの場合のみ、大文字固定）
 * @li インスタンス名（SDEの場合のみ）
 * @li サーバ名（SDEの場合のみ）
 * @li バージョン名（SDEの場合のみ）
 * @li ワークスペースパス名（ファイルベースの場合）
 * @li テーブル名（大文字固定）
 * @li テーブルオーナー名（大文字固定）
 *
 * SDEベースかファイルベースかを判断する場合には、IsSDE関数を使用してください。
 */
class SINDYLIB_API CTableNameString : public CWorkspaceNameString
{
public:
	CTableNameString();
	virtual ~CTableNameString();

	CTableNameString( IUnknown* ipUnk )
	{
		SetName( ipUnk );
	}

	bool operator==( const CTableNameString& obj ) const {
		return ( 0 == m_strTableName.CompareNoCase( obj.m_strTableName ) &&
			IsSameSource( obj ) );
	}

	bool operator<( const CTableNameString& obj ) const;

	/**
	 * @brief <b>テーブルからテーブル名称文字列を取得する</b>\n
	 * ipUnkは次のインターフェースのどれかをサポートしている必要があります：
	 * @li Table
	 * @li Row
	 * @param ipUnk		[in]	オブジェクト
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool SetName( IUnknown* ipUnk );

	/**
	 * @brief <b>テーブル名を取得する</b>\n
	 * 既に情報がセットされている必要があります。
	 * @return テーブル名（大文字固定）
	 */
	LPCTSTR GetTableName() const {
		return m_strTableName;
	}

	/**
	 * @brief <b>オーナー名を取得する</b>\n
	 * 既に情報がセットされている必要があります。
	 * @return オーナー名（大文字固定）
	 */
	LPCTSTR GetOwnerName() const {
		return m_strOwnerName;
	}

	/**
	 * @brief <b>オーナー名を持つかどうかをチェックする</b>\n
	 * 既に情報がセットされている必要があります。
	 * @retval true オーナー名を持つ
	 * @retval false オーナー名を持たない
	 */
	bool HasOwnerName() const {
		return m_strOwnerName.IsEmpty() ? false : true;
	}
	/**
	 * @brief <b>オーナ名.テーブル名を取得する</b>\n
	 * 既に情報がセットされている必要があります。\n
	 * @return オーナ名.テーブル名（大文字固定）
	 */
	LPCTSTR GetOwnerTableName() const {
		return m_strOwnerTableName;
	}

	/**
	 * @brief <b>同じソースかどうかを判断する</b>
	 * オーナ名と接続プロパティで判断します。
	 */
	bool IsSameSource( const CTableNameString& cNameString ) const;
private:
#pragma warning(push)
#pragma warning(disable:4251)
	CString m_strTableName;
	CString m_strOwnerName;
	CString m_strOwnerTableName;
#pragma warning(pop)
};

typedef boost::shared_ptr<CTableNameString> CSPTableNameString;

} // sindy

#endif // ifndef _NAMESTRING_H_
