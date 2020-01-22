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
		// オプション関連

		// ファイル関連
		NgLoadCnvParam		= 10002,
		NgOpenOutputFile	= 10003,
		NgGetSubFields		= 10006,
		NgEncode			= 10007,
		NgExistField		= 10008,
		
		// フォーマット関連
		NgOpEncode			= 11001,
		NgOpDelimiter		= 11002,
		NgOpNewline			= 11003,
		NgOpGeoref			= 11004,
		NgOpShapeType		= 11005,
		NgOpPolygonType		= 11006,

		// 変換テーブル関連
		NgLoadConvTable				= 12001,
		DuplicateConvTableGroup		= 12002,
		NgConTableTab				= 12003,
		DuplicateConvTableAttribute	= 12004,
		UnknownConvTableAttribute	= 12005,
		NoneConvTableGroup			= 12006,
		NgMatchGroup				= 12007,
		NoneConvTableAttribute		= 12008,
		NoneConvTableGroup2			= 12009,
		NgConvTableFieldName		= 12010,
		UnknownSearchType			= 12011,
		NgOidTableName				= 12012,

		// 変換フィールド対応表関連
		NgLoadConvField				= 13001,
		DuplicateConvFieldGroup		= 13002,
		NoneConvFieldGroup			= 13003,
		NgConvFieldTab				= 13004,
		UnknownConvFieldAttribute	= 13005,
		NoneConvFieldSrcFieldComma	= 13006,
		NgConvFieldFieldType		= 13007,
		NgConvFieldLength			= 13008,
		NgConvFieldNull				= 13009,
		DuplicateConvFieldRoundValue= 13010,
		NgConvFieldOptionValue		= 13011,
		DuplicateRange				= 13012,
		NoneRequiredRange			= 13013,
		NgSelectRange				= 13014,
		NgConfigRange				= 13015,
		NoneConvFieldGroup2			= 13016,
		DuplicateDstField			= 13017,
		NgRangeValue				= 13018,
		NgNullValue					= 13019,

		// 出力データ作成
		NgShapeType					= 30001,
		NgDataType					= 30002,
		NgGeomType					= 30003,
		NgTOKYOtoJGD2000_RP			= 30004,
		NgCoordIntegerDigitOver		= 30005,
		NgPointCoord				= 30006,

		// 出力データチェック
		NgCheckValidValue			= 40001,
		NgCheckValidRange			= 40002,
		NgCheckErrValue				= 40003,
		NgCheckErrRange				= 40004,
		NgCheckNull					= 40005,
		NgCheckIntegerDigitOver		= 40006,
		NgCheckStringLength			= 40007,
		NgCheckFieldType			= 40008,

		// ArcObjects関連
		NgDBConnect				= 50001,
		NgOpenTable				= 50002,
		NgGetRowCount			= 50003,
		NgGetCursor				= 50004,
		NgQueryDef				= 50005,
		NgJoinTable				= 50006,
		NgSetFilter				= 50007,
		NgEvalute				= 50008,
		NgGetFields				= 50009,
		NgGetFieldCount			= 50010,
		NgGetField				= 50011,
		NgGetGeomDef			= 50012,
		NgGetHasZ				= 50013,
		NgGetHasM				= 50014,
		NgGetGeomType			= 50015,
		NgNextRowCount			= 50016,
		NgGetValueCount			= 50017,
		NgFindField				= 50018,
		NgSetSubFields			= 50019,
		NgSetPostfixClause		= 50020,
		NgSetPrefixClause		= 50021,

		NgGetValue				= 51001,
		NgQueryInterface		= 51002,
		NgGetPointCount			= 51003,
		NgGetPoint				= 51004,
		NgGetExteriorRingCount	= 51005,
		NgQueryExteriorRings	= 51006,
		NgGetInteriorRingCount	= 51007,
		NgQueryInteriorRings	= 51008,
		NgQueryCoord			= 51009,
		NgGetZ					= 51010,
		NgGetM					= 51011,

		// その他
		NgChangeType			= 60001,

		NgOther					= 99999

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
			const CString strOID,
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
	std::ofstream m_ofsRunLog; //!< 実行ログ用ファイルストリーム
	std::ofstream m_ofsErrLog; //!< エラーログ用ファイルストリーム
};
