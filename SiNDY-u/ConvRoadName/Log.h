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

// エラータイプ
namespace err_type {
	enum ECode {
		fatal,
		error,
		warning,
		info,
		debug
	};
};

// エラーコード
namespace err_code {
	enum ECode {
		// ArcObjects関連
		NgDBConnect				= 10001,
		NgOpenTable				= 10002,
		NgGetIndex				= 10003,
		NgSetFilter				= 10004,
		NgGetRowCount			= 10005,
		NgGetCursor				= 10006,
		NgGetOID				= 10007,
		NgGetVal				= 10008,
		
		// 変換元データ関連
		FailOpenInputFile		= 20001,
		NgDictionaryFieldNum	= 20002,
		NgDictionaryFieldVal	= 20003,
		NgDictionaryRecord		= 20004,
		DuplicateDictionary		= 20005,
		DuplicateData			= 20006,
		NoRelData				= 20007,
		NullStrData				= 20008,

		// 出力先関連
		FailDelFile				= 30001,
		NoInited				= 30002,
		FailOpenOutputFile		= 30003,

		// 文字列分割処理関連
		NgSplitFunc				= 40001,
		NgParenConsis			= 40002,
		NgSpClsConsis			= 40003,
		DiffSplitCount			= 40004,
		UnsymmetricalParen		= 40005,
		UnsymmetricalWord		= 40006


	};
}; // err_code

class Log {
public:
	Log(){};
	~Log(){
		if (m_ofsRunLog)
			m_ofsRunLog.close();
		if (m_ofsErrLog)
			m_ofsErrLog.close();
	}

public:
	/**
	* @brief ログ出力クラスのインスタンス取得
	* @return インスタンス
	*/
	static Log& GetInstance(void) {
		static Log cInstance;
		return cInstance;
	}

	/**
	* @brief ログ出力用のファイルストリームを設定する関数
	* @param strFilePath [in] ログファイルパス
	* @param ofsLogFile [out] ログファイルストリーム
	* @retval true  処理成功
	* @retval false 処理失敗
	*/
	bool SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile);

	/**
	* @brief 実行ログのファイルストリームを設定する関数
	* @param strFilePath [in] 実行ログファイルパス
	* @retval true  処理成功
	* @retval false 処理失敗
	*/
	bool SetRunLog(const CString& strFilePath);

	/**
	* @brief エラーログのファイルストリームを設定する関数
	* @param strFilePath [in] エラーログファイルパス
	* @retval true  処理成功
	* @retval false 処理失敗
	*/
	bool SetErrLog(const CString& strFilePath);

	/**
	* @brief メッセージ出力
	* @note  ArcObjects以外でのエラー出力用
	* @param bCout     [in] 標準出力に出力するかどうか
	* @param bCerr     [in] 標準エラー出力に出力するかどうか
	* @param bRun      [in] 実行ログに出力するかどうか
	* @param bErr      [in] エラーログに出力するかどうか
	* @param errType   [in] エラータイプ
	* @param errCode   [in] エラーコード
	* @param ...       [in] 追加引数
	*/
	void PrintLog1(
		bool bCout,
		bool bCerr,
		bool bRun,
		bool bErr,
		err_type::ECode errType,
		err_code::ECode errCode,
		...);

	/**
	* @brief メッセージ出力
	* @note  ArcObjectsでのエラー出力用
	* @param bCout     [in] 標準出力に出力するかどうか
	* @param bCerr     [in] 標準エラー出力に出力するかどうか
	* @param bRun      [in] 実行ログに出力するかどうか
	* @param bErr      [in] エラーログに出力するかどうか
	* @param errType   [in] エラータイプ
	* @param errCode   [in] エラーコード
	* @param strTableName	[in] テーブル名
	* @param strOID    [in] キーID
	* @param ...       [in] 追加引数
	*/
	void PrintLog2(
		bool bCout,
		bool bCerr,
		bool bRun,
		bool bErr,
		err_type::ECode errType,
		err_code::ECode errCode,
		const CString& strTableName,
		const long oid,
		...);

	/**
	* @brief メッセージ出力
	* @note  この関数でcout,cerrに出力させている
	* @param bCout      [in] 標準出力に出力するかどうか
	* @param bCerr      [in] 標準エラー出力に出力するかどうか
	* @param bRun       [in] 実行ログに出力するかどうか
	* @param bErr       [in] エラーログに出力するかどうか
	* @param strMsg     [in] ログ出力用メッセージ
	* @param bLineBreak [in] 改行するか
	*/
	void PrintLog(
		bool bCout,
		bool bCerr,
		bool bRun,
		bool bErr,
		const CString& strMsg,
		bool bLineBreak = true);

private:
	/**
	* @brief エラーメッセージ文字列取得
	* @param errCode [in] エラーコード
	* @return エラーコードに対応するエラーメッセージ文字列
	*/
	static CString GetErrCodeMsg(err_code::ECode errCode);

	/**
	* @brief エラータイプ文字列取得
	* @param errType [in] エラータイプ
	* @return エラータイプに対応する文字列
	*/
	static CString GetErrTypeMsg(err_type::ECode errType);

private:
	std::ofstream m_ofsRunLog;
	std::ofstream m_ofsErrLog;
};

