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
#include "Util.h"
#include <WinLib/ExcelHelper.h>
#include "BusPointCommon/RegexChk.h"
#include "BusPointCommon/Time.h"

/**
 * @class	CImportBusPoint
 * @brief	バスデータのチェック＆インポートのためのメインクラス
 */
class CBusPointExcelChk
{
public:
	/**
	 * @brief	コンストラクタ
 	 * @param	vmArgs [in] 各種オプション
	 */
	CBusPointExcelChk( const Arguments& Args ){
		m_strInFile  = (Args.in_list).c_str();
		m_strChkLog  = (Args.check_log).c_str();
	};

	/**
	 * @brief	デストラクタ
	 */
	~CBusPointExcelChk(){};

	/**
	 * @brief	各種処理を実行するために最初に呼ばれる関数
	 * @retval	true  処理成功
	 * @retval	false 処理失敗
	 */
	bool execute();

private:

	typedef std::map<CString, CString>     BUSCONTAINER; //!< バスレコード管理用のフィールド名・値map
	typedef std::vector<BUSCONTAINER>      BUSCONTVEC;   //!< バスレコード群のVector
	typedef std::pair<CString, BUSCONTVEC> BUSSHEETPAIR; //!< バスレコード群とシート名のPair
	typedef std::vector<BUSSHEETPAIR>      BUSSHEETVEC;  //!< バスシート群のVector
	typedef std::map<CString, BUSSHEETVEC> BUSFILEMAP;   //!< バスの事業者ファイル名とExcelシート群のMap

	typedef std::pair<CString, CString>    CCPAIR;
	typedef std::pair<CCPAIR, CCPAIR>      CCPAIRPAIR;


	/**
	 * @brief	各種初期化
	 * @retval	true  処理成功
	 * @retval	false 処理失敗
	 */
	bool Init();

	/**
	 * @brief	調査結果ファイルリストを読み込み、ファイルパス1行ずつをリストに格納
	 * @retval	true  処理成功
	 * @retval	false 処理失敗
	 */
	bool LoadList();
	
	/**
	 * @brief	列名をリストに格納
	 * @note	各シートや各ファイル間で万一列名が揺れた場合を考え、独自に格納する
	 */
	void SetColumnName();

	/**
	 * @brief	調査結果ファイルから、レコード情報をファイル名・シート名で階層キーのコンテナに格納
	 * @param	busInputs [out] ファイル名・シート名で階層キーの調査結果ファイルレコードコンテナ
	 */
	void LoadExcelData( BUSFILEMAP& busInputs );

	/**
	 * @brief	調査結果ファイルの1行から、コンテナに読み込み
	 * @param	ipWorkSheet [in]  調査結果ワークシート
	 * @param	lineIdx     [in]  行番号
	 * @param	busInput    [out] 調査結果格納コンテナ
	 * @retval	true  1行読み込み成功
	 * @retval	false 1行読み込み失敗
	 */
	bool GetLineFromSheet( const Excel::_WorksheetPtr& ipWorkSheet, long lineIdx, BUSCONTAINER& busInput );

	/**
	 * @brief   セルの値を取得
	 * @param   sa        [in]  Excelシート1行分のSafeArray
	 * @param   Idx       [in]  取得したいセルのインデックス
	 * @retval  セルの値
	 */
	 CString GetCellStr( CComSafeArray<VARIANT>& sa, long idx )
	 {
		CComVariant vaValue;
		long cellIdx[2] = {1, idx};
		sa.MultiDimGetAt( cellIdx, vaValue );
		return CellToString( vaValue );
	 };

	/**
	 * @brief	セルの値をCStringで返す
	 * @param	vaValue   [in] セルの情報を持ったCComVariant型
	 * @retval	retStr    CString型のセルの値
	 */
	CString CellToString( const CComVariant& vaValue )
	{
		CString retStr;
		if( VT_R8 == vaValue.vt){
			retStr.Format(_T("%.lf"), vaValue.dblVal);				
		}
		else{
			retStr = (VT_BSTR == vaValue.vt)? vaValue.bstrVal : _T("");
		}
		return retStr;
	};

	/**
	 * @brief	調査結果ファイルチェック
	 * @param	busInputs [in] ファイル名・シート名で階層キーの調査結果ファイルレコードコンテナ
	 * @retval	true  エラー無し
	 * @retval	false エラー有り
	 */
	bool InputCheck( const BUSFILEMAP& busInputs );

	/**
	 * @brief	単一レコード内チェック
	 * @note	NULL不許可, サイズオーバー, フィールド間整合 等
	 * @param	busInput [in]  調査結果格納コンテナ
	 * @param	errMsgs  [out] エラーメッセージのlist
	 * @retval	true  エラー無し
	 * @retval	false エラー有り
	 */
	bool CheckByLine( const BUSCONTAINER& busInput, std::list<CString>& errMsgs );

	/**
	 * @brief	シート内チェック
	 * @note	シーケンス欠番, レコード間不整合 等
	 * @param	busInputs [in]  調査結果格納コンテナ
	 * @param	errMsgs   [out] エラーメッセージのlist
	 * @retval	true  エラー無し
	 * @retval	false エラー有り
	 */
	bool CheckBySheet( const BUSSHEETPAIR& busInputs, std::list<CString>& errMsgs ); 

	/**
	 * @brief	ファイル内チェック
	 * @note	同一停留所でのフィールド不整合
	 * @param	busInputs [in]  調査結果格納コンテナ
	 * @param	errMsgs   [out] エラーメッセージのlist
	 * @retval	true  エラー無し
	 * @retval	false エラー有り
	 */
	bool CheckByFileSameStop( const std::map<CCPAIR, BUSCONTVEC>& busInputs, std::list<CString>& errMsgs ); 

	/**
	 * @brief	ファイル内チェック
	 * @note	同一緯度経度で停留所コードor標柱番号確認
	 * @param	busInputs [in]  調査結果格納コンテナ
	 * @param	errMsgs   [out] エラーメッセージのlist
	 * @retval	true  エラー無し
	 * @retval	false エラー有り
	 */
	bool CheckByFileSameCoord( const std::map<CCPAIR, BUSCONTVEC>& busInputs, std::list<CString>& errMsgs ); 

private:

	CString            m_strInFile;         //!< 調査結果ファイルリスト名
	CString            m_strChkLog;         //!< チェックモードログファイル名

	std::ofstream      m_ofsChk;            //!< チェックログ

	std::vector<std::string> m_fileList;    //!< 調査結果ファイルリスト

	std::list<CString> m_columnNameList;    //!< カラム名格納用リスト
};
