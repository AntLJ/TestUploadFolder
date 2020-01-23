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

#ifndef __CNVADDRCODE_H__
#define __CNVADDRCODE_H__

#include "TempCodeTable.h"
#include "Oracl.h"

// 定義値
#define PROGNAME _T("CnvExTPAddrcode.exe")

// エラーコード
#define O_UPDATE						1
#define O_UPDATE_TMP					2
#define E_UPDATE_PROGNAME				3
#define E_UPDATE_MODIFYDATE				4
#define E_UPDATE_ADDRCODE				5
#define E_STORE							6
#define O_UPDATE_DATA					7
#define	C_NOCHANGE_KARICODE				8
#define O_UPDATE_HAISHI_ADDRCODE		9
#define O_UPDATE_DATA_HAISHI_ADDRCODE	10
#define E_NOUPDATE_HAISHI_ADDRCODE		11
#define E_NOUPDATE_NG_ADDRCODE			12


// 住所コードの状態
#define SIKOU_CODE	1	// 施行状態
#define HAISHI_CODE	2	// 廃止状態
#define NG_CODE		3	// 無効コード

typedef std::map<CString, CString> CODEMAP;

/** 
 * @class	CCnvAddrcode
 */
class CCnvAddrcode
{
public:
	CCnvAddrcode(void);
	virtual ~CCnvAddrcode(void);

	/**
	 * @brief	使用法
	 */
	static void printUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	コマンドライン引数の数
	 * @param	argv	[in]	コマンドライン引数
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	実行
	 * @return	bool
	 */
	bool execute();

private:

	/**
	 * @brief	環境変数から必要情報の設定
	 * @return	bool
	 */
	bool setInfoFromEnv();

	/**
	 * @brief	11桁住所コードがキーで、最新11桁住所コードが値のmapコンテナを作成
	 * @return	bool
	 */
	bool makeAddrcodeMap();

	/**
	 * @brief	仮コードがキーで、旧コードが値のmapコンテナ作成
	 * @return	bool
	 */
	bool makeTmpcodeMap();

	/**
	 * @brief	編集用子バージョン作成
	 * @return	bool
	 */
	bool makeChildVersion();

	/**
	 * @brief	編集開始
	 * @return	bool
	 */
	bool editStart();

	/**
	 * @brief	編集終了
	 * @return	bool
	 */
	bool editStop();

	/**
	 * @brief	拡張電話番号検索ポイント編集
	 * @param	ipRow	[in]	電話番号検索ポイント
	 */
	void editExTPPoint( _IRow* ipRow );

	/**
	 * @brief	最新11桁住所コード取得関数
	 * @param	strCode		[in]	11桁住所コード
	 * @param	strNewCode	[out]	最新11桁住所コード(存在するなら)
	 * @retval	0 : 住所変更なし
	 * @retval	1 : 住所変更有り
	 * @retval	2 : 予期せぬエラー
	 */
	int getNewAddrcode( const CString& strCode, CString& strNewCode )
	{
		CODEMAP::iterator itr = m_mapAddrcode.find( strCode );
		if( itr != m_mapAddrcode.end() )
		{
			strNewCode = itr->second;
			return ( strCode == itr->second? 0 : 1 );
		}
		else
			return 2;
	}

	/**
	 * @brief	仮コードかチェックし、仮コードなら前世代のコードを取得
	 * @param	strCode		[in]	11桁住所コード
	 * @param	strOldCode	[out]	仮コードの前世代の住所コード(存在するなら)
	 * @retval	true : 仮コード
	 * @retval	false : 仮コードでない
	 */
	bool isTmpCode( const CString& strCode, CString& strOldCode )
	{
		CODEMAP::iterator itr = m_mapTmpcode.find( strCode );
		if( itr != m_mapTmpcode.end() )
		{
			strOldCode = itr->second;
			return true;
		}
		else
			return false;
	}

	/**
	 * @brief	11桁住所コードの現在の状態取得
	 * @param	strCode	[in]	住所コード
	 * @retval	SIKOU_CODE	施行されているコード
	 * @retval	HAISHI_CODE	廃止されているコード
	 * @retval	NG_CODE		無効なコード
	 */
	int	checkAddrCodeState( const CString& strCode );

	/**
	 * @brief	ログヘッダー出力
	 */
	void printHeader()
	{
		std::cout << "#SINDYSTDLOG\n"
				  << "#\t\t\t\t\t\t\t\t更新前コード\t更新後コード\t住所名\t初期座標提供元コード\t調査結果コード" << std::endl;
	}

	/**
	 * @brief	ログ出力
	 * @param	ipRow		[in]	Row
	 * @param	nErrorCode	[in]	エラーコード
	 * @param	strOldCode	[in]	更新前コード
	 * @param	strNewCode	[in]	更新後コード
	 */
	void printLog( _IRow* ipRow, int nErrorCode, const CString& strOldCode = "", const CString& strNewCode = "" );

private:

	bool		m_bEditMode;		//!< 編集モード?

	IWorkspacePtr		m_ipWorkspace;		//!< ワークスペース
	IWorkspaceEditPtr	m_ipEditWorkspace;	//!< 編集用ワークスペース

	CString		m_strExTPFName;		//!< 拡張電話番号検索ポイントフィーチャクラス名
	ITablePtr	m_ipExTPTable;		//!< 拡張電話番号検索ポイントテーブル名

	ODatabase	m_cJDBNew;			//!< 住所DBソース(新)
	ODatabase	m_cJDBOld;			//!< 住所DBソース(旧)
	
	LONG m_lAddrcode_FID;			//!< 住所コードフィールドＩＤ
	LONG m_lAddrName_FID;			//!< 住所名フィールドＩＤ
	LONG m_lPointStatC_FID;			//!< 調査結果コードフィールドＩＤ
	LONG m_lInfoSrcC_FID;			//!< 初期座標提供元コードフィールドＩＤ
	LONG m_lProgModifyDate_FID;		//!< 最終プログラム更新日時フィールドＩＤ
	LONG m_lModifyProgName_FID;		//!< 最終更新プログラム名フィールドＩＤ
	
	CODEMAP		m_mapAddrcode;		//!< 11桁住所コードがキーで、最新11桁住所コードが値
	CODEMAP		m_mapTmpcode;		//!< 仮コードがキーで、旧コードが値

	CTempCodeTable	m_cTmpTable;	//!< 仮コード対応表

};


#endif // __CNVADDRCODE_H__
