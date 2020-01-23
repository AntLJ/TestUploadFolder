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
 * @file ModifyCheck.h
 */
#ifndef _MODIFYCHECK_H_
#define _MODIFYCHECK_H_

#include "resource.h"       // メイン シンボル

#include "exportimport.h"
#include "sde_ondemand.h"
#include "time.h"

namespace sindy {
class CRow;

#ifdef _TEST_NEW_MODIFYCHECK
extern SINDYLIB_API TCHAR g_strModifyCheckProcedure[];	//!< ModifyCheckプロシージャ名
#endif // ifdef _TEST_NEW_MODIFYCHECK

class SINDYLIB_API CModifyCheck
{
/*	struct ADDSDATA
	{
		CString Operator;
		LONG PurposeID;
		CString Purpose;
		tm DateTime;
		BOOL UsefulDateTime;
		LONG UpdateTypeID;
		CString UpdateType;
		LONG StateID;
	};

	struct STRLONG
	{
		CString TABLE;
		LONG OID;
		BOOL DELETED;
	};*/
	public:
	/**
	 * @class CModifyInfo
	 * @brief <b>オブジェクトの更新属性を管理するクラス</b>\n
	 * DLL外でのメモリ処理を避けるため、コンストラクタをはじめとする
	 * メモリ処理部はすべて*.cppに記述してあります
	 */
	class CModifyInfo
	{
	public:
		CModifyInfo();
		virtual ~CModifyInfo();

		CModifyInfo( const CModifyInfo& cInfo ) { *this = cInfo; }
		CModifyInfo& operator=( const CModifyInfo& cInfo );

		void SetOperator( LPCTSTR lpcszName );
		void SetTime( LPCTSTR lpcszTime );
		void SetTime( LPSYSTEMTIME lpsysTime );
		void SetTime( const tm& tmTime );
		void SetTableName( LPCTSTR lpcszName );
		void SetUpdateType( LPCTSTR lpcszType );
		void SetPurpose( LPCTSTR lpcszPurpose );
		LPCTSTR GetOperator() const { return m_strOperator; }
		LPCTSTR GetTime() const { return m_strTime; }
		LPCTSTR GetTableName() const { return m_strTableName; }
		LPCTSTR GetUpdateType() const { return m_strUpdateType; }
		LPCTSTR GetPurpose() const { return m_strPurpose; }
	public:
		bool m_bIsModified;	//!< チェックに引っかかった場合は情報を取れる・取れないに関わらずtrue（プロシージャでエラーが発生したかどうかの確認用）

		long m_lOID;
		long m_lPurposeID;
		long m_lStateID;
		long m_lUpdateTypeID;
		bool m_bIsLocal;
		bool m_bDeleted;
		bool m_bUsefulTime;
//		tm   m_tmTime;
	private:
		CString m_strOperator;
		CString m_strTime;
		CString m_strTableName;
		CString m_strUpdateType;
		CString m_strPurpose;
	};

	CModifyCheck();
	virtual ~CModifyCheck();

	bool IsModified( const CRow& cRow, CModifyInfo& cInfo );
private:
	/**
	 * @brief
	 */
	BOOL CreateConnection( const CRow& cRow, CString& strVersion, CString& strDatasetName, CString& strConnStr );

	/**
	 * @brief 自分の編集系統をカンマ区切りで取得する
	 *
	 * @param cSdeConnect	[in]	セッション
	 * @param lpcszVer		[in]	バージョン名（接続ユーザ名はつかない）
	 *
	 * @retval CString
	 */
	CString GetEditStateChain( SE_CONNECTION conn, LPCTSTR lpcszVer );

	/** 
	 * @brief ステートが生きているかどうか
	 *
	 * @param cSdeConnect	[in]	セッション
	 * @param lpcszVer		[in]	編集中のバージョン名
	 * @param lpcszIDs		[in]	検査対象のステート文字列（カンマ区切り）
	 * @param nActiveType	[out]	-1:Activeではない 0:編集中又はポスト後のステート 1:ローカルバージョンのみに保存されているステート
	 *
	 * @retval -1 ステートは生きてない
	 * @retval -1以外 ステートは生きている（ステート番号）
	 */
	LONG IsActiveStates( SE_CONNECTION conn, LPCTSTR lpcszVer, LPCTSTR lpcszIDs, INT& nActiveType );

	/**
	 * @brief Aテーブルからフィーチャのステートを取得する
	 *
	 * @param cSdeConnect		[in]			セッション
	 * @param lpcszTableID		[in]			SDEに登録されているテーブルID
	 * @param lObjectID			[in]			ステートを取得するフィーチャのOBJECTID
	 * @param listAddsData		[out]			取得したステートリスト
	 * @param lpcszExcludeIDs	[in,optional]	除外リスト
	 *
	 * @return 取得したステートのID（カンマ区切り）
	 */
	CString GetAStates( SE_CONNECTION conn, LPCTSTR lpcszTableID, LONG lObjectID, std::list<CModifyInfo>& listAdds, LPCTSTR lpcszExcludeIDs = NULL );

	/**
	 * @brief Dテーブルからフィーチャのステートを取得する
	 *
	 * @param cSdeConnect		[in]			セッション
	 * @param lpcszTableID		[in]			SDEに登録されているテーブルID
	 * @param lObjectID			[in]			ステートを取得するフィーチャのOBJECTID
	 * @param lpcszExcludeIDs	[in,optional]	除外リスト
	 *
	 * @return 取得したステートのID（カンマ区切り）
	 */
	CString GetDStates( SE_CONNECTION conn, LPCTSTR lpcszTableID, LONG lObjectID, LPCTSTR lpcszExcludeIDs = NULL );

	/**
	* @brief 入力されたユーザーの A テーブルと D テーブルを取得する
	*
	* @param cSdeConnect	[in] SdeConnect オブジェクト（SDE との接続セッション）
	* @param vaUser			[in] 現在接続しているユーザー名（IPropertySet から取得）
	*
	* @retval TRUE	取得成功
	* @retval FALSE 取得失敗
	**/	
	BOOL GetTableList( SE_CONNECTION conn, const VARIANT& vaUser );
	
	/**
	* @brief 変更チェック対象のオブジェクトのID
	*
	* @param cSdeConnect	[in] SdeConnect オブジェクト（SDE との接続セッション）
	* @param strTable		[in] 検索対象テーブル
	* @param bVersion		[in] Version 文字列
	* @param lOID			[in] 変更チェック対象のオブジェクトのID
	* @param sMessage		[out] 変更されていた場合のメッセージ
	*
	* @retval TRUE	変更された
	* @retval FALSE 変更されていない
	**/
	bool IsModifiedByLocal( SE_CONNECTION conn, LPCTSTR lpcszTable, LPCTSTR lpcszVersion, long lOID, CModifyInfo& cInfo );
private:
#pragma warning(push)
#pragma warning(disable: 4251)
#ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
	static std::map<CString, SE_CONNECTION>	g_mapConnection;			//!< SdeConnect オブジェクトを接続文字列ごとに保持
#endif // ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
	static std::map<CString, CString>		g_mapTabls;					//!< A テーブルと D テーブルの名前を保持
	CString									m_strEditStateChain;		//!< 編集系統保持用

	bool									m_bUseProcedure;
	bool									m_bUncheck;					//!< ModifyCheckを使用するかどうか
	bool									m_bExistProcedure;
#pragma warning(pop)
};

} // sindy

#endif // ifndef _MODIFYCHECK_H_
