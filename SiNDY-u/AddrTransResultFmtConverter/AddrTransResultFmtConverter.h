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

#include "Argument.h"

/**
 * @brief   結合列のstruct
 */
struct CombinePairs
{
	int firstColumn;
	int secondColumn;
	CombinePairs():firstColumn(0),secondColumn(0){}
	CombinePairs(int inputFstClm, int inputSndClm):firstColumn(inputFstClm),secondColumn(inputSndClm){}
};

/**
 * @brief   ファイルラインのstruct
 */
struct GroupLines
{
	map<int, wstring>   mapGroupLines;
	int                 lineNum;
	GroupLines():lineNum(0){}
	GroupLines(int inputLineNum):lineNum(inputLineNum){}
};

/**
 * @class  AddrTransResultFmtConverter
 * @brief  メインルーチンクラス
 */
class AddrTransResultFmtConverter
{
public:
	/**
	* @brief   初期化
	* @param   [in] argc コマンドライン引数の数
	* @param   [in] argv コマンドライン引数の内容
	* @retval  true:成功 false:失敗
	*/
	bool init( int argc, TCHAR** argv );

	/**
	* @brief   実行
	* @retval  true:成功 false:失敗
	*/
	bool run();

	/**
	 * @brief   city.txtファイルの処理
	 */
	void convertCity();

	/**
	 * @brief   addr[**].txtファイルの処理
	 */
	void convertAddr();

	/**
	* @brief    aza[**].txtファイルの作成
	* @param    [in] pFilePath          入力ファイルパス
	* @retval   true:成功  false:失敗
	*/
	bool createAzaFile(const wstring& pFilePath);

	/**
	* @brief    oaza[**].txtファイルの作成
	* @param    [in] pFilePath          入力ファイルパス
	* @retval   true:成功  false:失敗
	*/
	bool createOazaFile(const wstring& pFilePath);

	/**
	* @brief    ファイル行の分割
	* @param    [in] str                ファイルの行
	* @param    [in] delim              分割符号
	*/
	void split(const std::wstring& str, const std::wstring& delim);

	/**
	* @brief    結果ファイルの書き込む
	* @param    [in] ret                出力内容
	*/
	void writeResultFile(map<int, wstring>& ret);

	/**
	* @brief    ファイル名称の取得
	* @param    [in] path               ファイルのパス
	*/
	void getFileNames(const wchar_t* path);

	/**
	* @brief    run.logの行の整理
	* @param    [in] inFileName         入力ファイル名称
	* @param    [in] outFileName        出力ファイル名称
	* @param    [in] inRecordNum        入力レコード数
	* @param    [in] outRecordNum       出力レコード数
	* @param    [in] bNormal            処理の中でエラーがあるか
	* @retval   マージした文字列
	*/
	wstring mergeRunLogLine(const wstring& inFileName, const wstring& outFileName, int inRecordNum, int outRecordNum, bool bNormal = true);

	/**
	* @brief    err.logの行の整理
	* @param    [in] sFileName          元ファイル名称
	* @param    [in] lineNum            行数
	* @param    [in] errMsg             エラーメッセージ
	* @param    [in] info1              情報1
	* @param    [in] info2              情報2
	* @param    [in] info3              情報3
	* @retval   マージした文字列
	*/
	wstring mergeErrLogLine(const wstring& sFileName, int lineNum, const wstring& errMsg, const wstring& info1, const wstring& info2, const wstring& info3 = L"");

	/**
	* @brief    出力ファイルの統計
	*/
	void outputFileStat();

	/**
	* @brief    実行ログとエラーログのメッセージ
	*/
	void writeInformation();

	/**
	* @brief    時間記録
	* @param    [in] phaseName          開始時間または完了時間
	* @retval   時間情報
	*/
	wstring timeMemo(const wstring& phaseName);

private:
	Argument                            m_argument;              //!< 引数
	FileController                      m_errLogger;             //!< エラーログファイル
	FileController                      m_runLogger;             //!< 実行ログファイル
	FileController                      m_sourceFile;            //!< 入力ファイル
	FileController                      m_resultFile;            //!< 出力ファイル

	map<wstring, int>                   m_mapFileHeadStr2Int;    //!< city.txtファイルのヘッダ
	map<int, wstring>                   m_mapFileLine;           //!< ファイルのライン<key,value>(列のindex,列の内容)
	vector<wstring>                     m_vecNameYomi2;          //!< 「名称2」または「読み2」を含む列

	vector<wstring>                     m_vecAddressFileName;    //!< addr[**].txtファイル名(path含む)
	map<int, CombinePairs>              m_mapCombineColumnIdxs;  //!< 結合列の索引
	map<wstring, map<int, wstring>>     m_mapOazaFile;           //!< oaza[**].txtのコンテナ
	map<wstring, vector<GroupLines>>    m_mapErrCheck;           //!< addr[**].txtをoaza[**].txt抜くの部分
	vector<wstring>                     m_vecRunInfo;            //!< 実行メッセージのコンテナ
};