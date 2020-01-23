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
		info
	};
}; // err_type

// エラーコード
namespace err_code {
	enum ECode {
		NgOpenFile          = 1001, //!< ファイルオープン失敗
		NgOpenWorkspace     = 1002, //!< ワークスペース接続失敗
		NgOpenTable         = 1003, //!< テーブル取得失敗
		NgGetFields	        = 1004, //!< フィールドリスト取得失敗
		NgGetField	        = 1005, //!< フィールド取得失敗
		NgGetRow            = 1006, //!< レコード取得失敗
		NgGetRowBuffer      = 1007, //!< レコードバッファ取得失敗
		NgGetShape          = 1008, //!< 形状取得失敗
		//NgGetDomain         = 1009, //!< ドメイン取得失敗
		//NgGetDomainField    = 1010, //!< ドメインフィールド取得失敗
		//NgGetFieldMap       = 1011, //!< フィールドマップ取得失敗
		NgGetCursor         = 1012, //!< カーソル取得失敗
		NgGetAttr           = 1013, //!< 属性取得失敗
		//NgGetDomainValue    = 1014, //!< ドメイン値取得失敗
		//NgGetDataset        = 1015, //!< データセット取得失敗
		NgGetFieldType      = 1016, //!< フィールドタイプ取得失敗
		NgGetFieldIndex     = 1017, //!< フィールドインデックス取得失敗
		NgGetClipShape      = 1018, //!< クリップ形状取得失敗
		NgGetGeomType       = 1019, //!< ジオメトリタイプ取得失敗
		//NgGetPointCol       = 1020, //!< ポイントコレクション取得失敗
		NgGetVertexCount    = 1021, //!< 構成点数取得失敗
		//NgGetFieldLength    = 1022, //!< フィールド長取得失敗
		NgGetFieldPrecision = 1023, //!< 精度取得失敗

		NgCreateDir         = 1101, //!< ディレクトリ作成失敗
		NgMeshCodeDigit     = 1102, //!< メッシュコードの桁数が合っていない
		NgFieldType         = 1103, //!< フィールドタイプが変換対象外
		NgCreateClone       = 1104, //!< クローン作成製失敗
		NgGeomType          = 1105, //!< ジオメトリタイプが未対応
		NgIntersect         = 1106, //!< クリッピング失敗

		NgMoveFile          = 1201, //!< ファイル移動失敗
		NgCreateTable       = 1301, //!< テーブル作成失敗

		NgPutRowAttr        = 2001, //!< レコード属性付与失敗
		NgPutRowShape       = 2002, //!< レコード形状付与失敗
		NgPutFieldAttr      = 2003, //!< フィールド属性付与失敗
		NgPutWhere          = 2004, //!< Where句指定失敗
		//NgPutSubFields      = 2005, //!< SubFields指定失敗
		NgPutSpRel          = 2006, //!< 形状検索方法設定失敗
		NgPutRefGeom        = 2007, //!< 検索形状設定失敗

		//NgFGDB              = 3001, //!< FGDBでない
		//NgStartEdit         = 3002, //!< 編集開始失敗
		//NgAbortEdit         = 3003, //!< 編集破棄失敗
		//NgStopEdit          = 3004, //!< 編集終了失敗
		//NgStore             = 3005, //!< Store処理失敗
		NgFlush             = 3006, //!< Flush処理失敗

		NgAttrFormatEssField            = 4001, //!< 必須属性（変換元と変換先）が指定されていない
		NgAttrFormatOrgFieldEmpty       = 4002, //!< 変換元フィールド名が空
		NgAttrFormatDstFieldEmpty       = 4003, //!< 変換先フィールド名が空
		NgAttrFormatOrgFieldExceptNewOp = 4004, //!< 変換元フィールド名に新規作成フィールドオプション以外が指定されている
		NgAttrFormatOrgFieldName        = 4005, //!< 変換元フィールドが存在しない
		NgAttrFormatDstFieldSetOp       = 4006, //!< 変換先フィールドにオプションが指定されている
		NgAttrFormatOrgFieldDuplicate   = 4007, //!< 変換先フィールドが重複している
		NgAttrFormatDstFieldDuplicate   = 4008, //!< 変換元フィールドが重複している
		NgAttrFormatNewOp               = 4009, //!< 新規フィールドオプションは変換元フィールド名（第1フィールド）のみに使用可能
		NgAttrFormatNoOp                = 4010, //!< オプション指定なしは変換先フィールド名（第2フィールド）のみ許容
		NgAttrFormatUnknownOp           = 4011, //!< 設定オプション不明
		NgAttrFormatValue               = 4012, //!< 設定値のフォーマット不正
		NgAttrFormatValueAps            = 4013, //!< 設定値のフォーマット不正（「'」が不正）
		NgAttrFormatFieldInfo           = 4101, //!< フィールド設定のフォーマット不正
		NgAttrFormatUnknownFieldInfo    = 4102, //!< フィールド設定の設定項目が不正
		NgAttrFormatFieldInfoDuplicate  = 4103, //!< フィールド設定の設定項目が重複
		NgAttrFormatRound               = 4201, //!< 丸め込み設定のフォーマット不正
		NgAttrFormatRoundAps            = 4202, //!< 丸め込み設定のフォーマット不正（「'」不正）
		NgAttrFormatRoundDuplicate      = 4203, //!< 丸め込み設定の設定項目が重複
		NgAttrFormatDefaultDupicate     = 4301, //!< 初期値設定の設定項目が重複
		NgAttrFormatAttrAps             = 4401, //!< 属性切り出し設定のフォーマット不正（「'」不正）
		NgAttrFormatAttrDuplicate       = 4402, //!< 属性切り出し設定が重複
		NgAttrFormatNewDefault          = 4501, //!< 新規作成フィールドに初期値設定がされていない
		NgAttrFormatNewFieldValue       = 4502, //!< 新規作成フィールドに属性値設定がされている
		NgAttrFormatNewRound            = 4503, //!< 新規作成フィールドに丸め込み設定がされている
		NgAttrFormatExistDefault        = 4504, //!< 既存フィールドに初期値設定がされている
		NgAttrFormatExistAttr           = 4505, //!< 既存フィールドに属性切り出し値設定がされている
		NgAttrFormatNewFieldInfoType    = 4506, //!< 新規作成フィールドにフィールド設定のTYPEが指定がされていない
		NgAttrFormatExistFieldInfoType  = 4507, //!< 既存フィールドにフィールド設定のTYPEが指定がされている
		NgAttrFormatFieldInfoType       = 4601, //!< フィールド設定のTYPE設定値が不正
		NgAttrFormatFieldInfoTypeValue  = 4602, //!< フィールド設定で型にあった属性が指定されていない
		NgAttrFormatFieldInfoValue      = 4603, //!< フィールド設定で属性値が不正
		NgAttrFormatFieldInfoScale      = 4604, //!< フィールド設定でSCALE設定値が不正
		NgAttrFormatFieldInfoPrecision  = 4605, //!< フィールド設定でPRECISION設定値が不正
		NgAttrFormatFieldInfoLength     = 4606, //!< フィールド設定でLENGTH設定値が不正

		NgAddRow            = 6001, //!< レコード追加失敗
		NgAddField          = 6002, //!< フィールド追加失敗
		//NgAddAttr           = 6003, //!< 属性追加失敗
		//NgAddDomain         = 6004, //!< ドメイン追加失敗

		//NgUpdRow            = 6101, //!< レコード更新失敗
		//NgUpdField          = 6102, //!< フィールド更新失敗
		//NgUpdAttr           = 6103, //!< 属性更新失敗
		//NgUpdDomain         = 6104, //!< ドメイン更新失敗

		//NgDelRow            = 6201, //!< レコード削除失敗
		//NgDelField          = 6202, //!< フィールド削除失敗
		//NgDelAttr           = 6203, //!< 属性削除失敗
		//NgDelDomain         = 6204, //!< ドメイン削除失敗
		NgDelFile           = 6205, //!< ファイル削除失敗

		//GetManyRow          = 8001, //!< 複数レコード取得
		ExistField          = 8002, //!< 既存フィールド
		ChangePrecision     = 8201  //!< 精度設定変更

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
	static CLog& GetInstance(void){
		static CLog cInstance;
		return cInstance;
	}
	bool SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile);
	bool SetRunLog(const CString& strFilePath);
	bool SetErrLog(const CString& strFilePath);


	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 IFeatureClassPtr ipFeatureClass);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 ITablePtr ipTable);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 IFeatureClassPtr ipFeatureClass,
						 long lOID);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 ITablePtr ipTableClass,
						 long lOID);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 IFeaturePtr ipFeature);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 _IRowPtr ipRow);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg);

	/**
	 * @brief メッセージ出力
	 * @note  最終的なログ出力部分のフォーマットをこの関数で設定
	 * @param bCout [in] 標準出力に出力するかどうか
	 * @param bCerr [in] 標準エラー出力に出力するかどうか
	 * @param errType [in] エラータイプ
	 * @param errCode [in] エラーコード
	 * @param strTableName [in] フィーチャクラス名
	 * @param lOID [in] フィーチャオブジェクトID
	 * @param strOtherMsg [in] その他メッセージ
	 */
	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strTableName,
						 long lOID,
						 const CString& strOtherMsg);

	/**
	 * @brief メッセージ出力
	 * @note  この関数でcout,cerrに出力させている
	 * @param bCout [in] 標準出力に出力するかどうか
	 * @param bCerr [in] 標準エラー出力に出力するかどうか
	 * @param strMsg [in] ログ出力用メッセージ
	 */
	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 const CString& strMsg);

	/**
	 * @brief ツールバージョン出力
	 * @param bCout [in] 標準出力に出力するかどうか
	 * @param bCerr [in] 標準エラー出力に出力するかどうか
	 */
	static void PrintToolVersion(
						bool bCout,
						bool bCerr,
						bool bRun,
						bool bErr);

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

