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
		// 全体
		NgAddrDBConnectString	= 10001,

		// 郵便番号と住所紐付け
		NgDuplicateRelAddr		= 20001,
		NgAddrCode				= 20002,
		NgAddr11Dir				= 21001,
		NgAddr11File			= 21002,
		NgAddr11FileOpen		= 21003,
		NgAddr11Record			= 21004,
		NgAddr11Duplicate		= 21005,
		NgAddr11AccCode2		= 21006,

		// 郵便番号ポリゴン
		NgZipPriority			= 30001,
		NgOverlapZipValue		= 30002,
		NgSameZipCode			= 30003,

		// 重心
		NgGetCentroid			= 40001,
		NgGetCentroid2			= 40002,

		// 市区町村代表点
		NgDuplicateAddrRec		= 50001,
		NgAddrCode5Digit		= 50002,
		NgZipCodeUnder00		= 50003,

		// ArcObjects関連
		NgDBConnect				= 80001,
		NgOpenTable				= 80002,
		NgPutWhereClause		= 80003,
		NgGetCursor				= 80004,
		NgGetFields				= 80005,
		NgGetFieldCount			= 80006,
		NgGetField				= 80007,
		NgGetFieldName			= 80008,

		NgGetValue				= 81001,
		NgGetEnumDatasetName	= 81002,
		NgResetEnumDatasetName	= 81003,
		NgGetTableName			= 81004,
		NgDeleteTable			= 81005,
		NgGetShape				= 81006,
		NgGetOID				= 81007,
		NgQueryInterface		= 81008,

		NgPutName				= 82001,
		NgPutType				= 82002,
		NgPutEditable			= 82003,
		NgPutIsNullable			= 82004,
		NgPutLength				= 82005,
		NgPutPrecision			= 82006,
		NgPutSpRef				= 82007,
		NgPutGeoType			= 82008,
		NgPutGeoDef				= 82009,
		NgPutCoords				= 82010,
		NgAddField				= 82101,
		NgCreateTable			= 82201,

		NgInsertCursor			= 83001,
		NgCreateRowBuffer		= 83002,
		NgPutValue				= 83003,
		NgInsertRow				= 83004,
		NgFlush					= 83005,
		NgPutShape				= 83006,

		NgUnion					= 84001,

		// その他
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
