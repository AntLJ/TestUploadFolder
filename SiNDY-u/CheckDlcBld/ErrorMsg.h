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
}; // err_type

// エラーコード
namespace err_code {
	enum ECode {
		NgDlcBldWithin             = 10001, //!< Withinしていない
		NgBldDlcContains           = 10002, //!< Containsしていない
		NgBldDlcTouches            = 10003, //!< Touchesしていない
		NgBldDlcDisjoint           = 10004, //!< Disjoiontしていない
		NgBldDlcCross              = 10005, //!< Crossしていない
		DlcBldWithin               = 11001, //!< Withinしている
		BldDlcContains             = 11002, //!< Containsしている
		BldDlcTouches              = 11003, //!< Touchesしている
		BldDlcDisjoint             = 11004, //!< Disjoiontしている
		BldDlcCross                = 11005, //!< Crossしている
		NgExistBld1                = 20001, //!< 対象Bld存在NG
		NgExistBld2                = 20002, //!< 対象Bld存在NG、隣接Bld内包NG
		NgExistBld3                = 20003, //!< 対象Bld存在NG、隣接Bld内包OK
		NgExistBld4                = 20004, //!< 対象Bld内包NG、隣接Bld内包OK
		NgExistBld5                = 20005, //!< 対象Bld＋隣接Bld形状内包OK
		NgBldDlcContTouch          = 21001, //!< 対象Bld内包NG
		NgBldDlcContTouchNear      = 30001, //!< 隣接Bld内包NG
		NgBldDlcRelation           = 40001, //!< DlcとBldの形状関係が不正

		NgOther         = 99999

	};
}; // err_code

/**
 * @brief ログ出力用クラス
 * @note  標準出力、標準エラー出力のみ
 */
class CLog {
public:
	CLog(){}
	~CLog(){
		if(m_ofsRunLog)
			m_ofsRunLog.close();
		if(m_ofsErrLog)
			m_ofsErrLog.close();
	}

public:
	/**
	 * @brief ログ出力クラスのインスタンス取得
	 * @return インスタンス
	 */
	static CLog& GetInstance(void){
		static CLog cInstance;
		return cInstance;
	}

	/**
	 * @brief ログ出力用のファイルストリームを設定する関数
	 * @param strFilePath [in] ログファイルパス
	 * @param strFilePath [out] ログファイルストリーム
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
	 * @note  ArcObjectsでのエラー出力用
	 * @param bCout     [in] 標準出力に出力するかどうか
	 * @param bCerr     [in] 標準エラー出力に出力するかどうか
	 * @param bRun      [in] 実行ログに出力するかどうか
	 * @param bErr      [in] エラーログに出力するかどうか
	 * @param errType   [in] エラータイプ
	 * @param strMsg1   [in] 出力メッセージ1
	 * @param strMsg2   [in] 出力メッセージ2
	 * @param strMsg3   [in] 出力メッセージ3
	 */
	void PrintLog(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			err_type::ECode errType,
			const CString& strMsg1,
			const CString& strMsg2 = _T(""),
			const CString& strMsg3 = _T(""));

	/**
	 * @brief メッセージ出力
	 * @note  最終的なログ出力部分のフォーマットをこの関数で設定
	 * @param bCout        [in] 標準出力に出力するかどうか
	 * @param bCerr        [in] 標準エラー出力に出力するかどうか
	 * @param bRun         [in] 実行ログに出力するかどうか
	 * @param bErr         [in] エラーログに出力するかどうか
	 * @param errType      [in] エラータイプ
	 * @param errCode      [in] エラーコード
	 * @param strTableName [in] フィーチャクラス名
	 * @param lOID         [in] フィーチャオブジェクトID
	 * @param strOtherMsg1 [in] その他メッセージ1
	 * @param strOtherMsg2 [in] その他メッセージ2
	 * @param strOtherMsg3 [in] その他メッセージ3
	 */
	void PrintLog(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			err_type::ECode errType,
			err_code::ECode errCode,
			const CString& strTableName,
			long lOID,
			const CString& strOtherMsg1 = _T(""),
			const CString& strOtherMsg2 = _T(""),
			const CString& strOtherMsg3 = _T(""));

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
	std::ofstream m_ofsRunLog; //!< 実行ログ用ファイルストリーム
	std::ofstream m_ofsErrLog; //!< エラーログ用ファイルストリーム
};

