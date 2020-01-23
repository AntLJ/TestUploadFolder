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
		NgRWLDuplicateData             = 10001, //!< データが重複している◆
		NgRWLExistFromNodeInRWN        = 10002, //!< FROMNODEがRWNに存在しない◆
		NgRWLFromNodeAndRWNGeomSame    = 10003, //!< FROMNODEIDのRWNと始点の形状が不一致◆
		NgRWLExistToNodeInRWN          = 10004, //!< TONODEIDのRWNが存在しない◆
		NgRWLToNodeAndRWNGeomSame      = 10005, //!< TONODEIDのRWNと終点の形状が不一致◆
		NgRWLSmallSegment              = 10006, //!< 微小セグメント(%[セグメント長]以下)が存在する◆
		NgRWLSameAttribute             = 10007, //!< 同一OBJECTID群の属性が不一致◆
		NgRWLSequenceDivID             = 10008, //!< 同一OBJECTID群のDIVIDが不連続◆
		NgRWLDuplicateVertex           = 10009, //!< 構成点が重複している◆
		NgRWLDuplicateVertexAllData    = 10010, //!< 全データ内で構成点が重複している◇
		NgRWLDuplicateSegment          = 10011, //!< 全データ内でセグメントが重複している◆
		NgRWLFirstDivLinkFromDivIDNot0 = 10012, //!< 最初のDIVIDを持つリンクのFROMDIVIDが0でない◆
		NgRWLLastDivLinkToDivIDNot0    = 10013, //!< 最後のDIVIDを持つリンクのTODIVIDが0でない◆
		NgRWLLoopLink                  = 10014, //!< ループリンクが存在する◆
		NgRWLGroupLink                 = 10015, //!< グループ化できない◆

		NgRWNDuplicateData             = 20001, //!< データが重複している◆
		NgRWNDivOIDisMinimumRWLOID     = 20002, //!< DIVIDを持つノードOBJECTIDが接続リンクの最小OBJECTIDになっていない◆
		NgRWNSequenceDivID             = 20003, //!< DIVIDを持つ同一OBJECTIDのノードがリンクの向きにDIVIDが連続していない◆
		NgRWNDuplicateGeom             = 20004, //!< 形状が重複している◆
		NgRWNExistInRWL                = 20005, //!< RWLのFROMTONODEに存在しない◆
		//NgRWNGeomSameRWL               = 20006, //!< RWLのFROMTONODEに形状一致しない◇

		NgRSADuplicateData             = 30001, //!< データが重複している◆

		NgRWLExistInBefore             = 11001, //!< 変換前データに存在しない◆
		NgRWLSameAttrAsBefore          = 11002, //!< 変換前属性と不一致◆
		NgRWLSameShapeAsBefore         = 11003, //!< 変換前形状と不一致◆
		NgRWLSameAttrAsSplitData       = 11004, //!< 分割後データの属性同士が不一致◆
		NgRWLSameRoadAttrAsSplitData   = 11005, //!< ROADATTR_Cが変換前属性と不一致（リンク内属性分割処理）◆
		NgRWLSameWalkableAsSplitData   = 11006, //!< WALKABLE_Cが変換前属性と不一致（歩行者ノード分割・歩行者リンクマージ処理）◆

		NgRWNExistInBefore             = 21001, //!< 変換前データに存在しない◆
		NgRWNSameAttrAsBefore          = 21002, //!< 変換前属性と不一致◆
		//NgRWNSameShapeAsBefore         = 21003, //!< 変換前形状と不一致◇

		NgRWLExistInAfter              = 12001, //!< 変換後データに存在しない◆
		NgRWLMergeData                 = 12002, //!< マージ対象リンクでない◆
		NgRWLMergeData2                = 12003, //!< マージ対象リンクが人車共用でない◆

		NgRWNSameShape                 = 22001, //!< 変換後データに同一形状のデータが存在しない◆

		NgHNDuplicateData              = 40001, //!< データが重複している◆
		NgHNExistInRWL                 = 40002, //!< 対応するリンクが存在しない◆a
		NgHNSameHeight                 = 40003, //!< 同一LINKIDIDのリンクで接続点の高さが不一致◆a
		//NgHNFrom0Height                = 40004, //!< 同一LINKIDIDのリンクで連続状態の開始点の高さが0でない◆a
		//NgHNTo0Height                  = 40005, //!< 同一LINKIDIDのリンクで連続状態の終了点の高さが0でない◆a

		NgHNExistInBefore              = 41001, //!< 変換前データに存在しない◆a
		//NgHNSameAttrAsBefore           = 41002, //!< 変換前属性と不一致◆a
		//NgHNSameShapeAsBefore          = 41003, //!< 変換前形状と不一致◇

		NgHNExistInAfter               = 42001, //!< 変換後データに存在しない◆
		NgHNSameLinkIdAsAfter          = 42002, //!< 変換後LINKIDと不一致◆a
		NgHNSameAttrAsAfter            = 42003, //!< 変換後属性と不一致◆a

		NgHNExistInAfterRWN            = 43001, //!< 変換後RWNにデータが存在しない◆a
		NgHNReconstractHNData          = 43002, //!< 高さノードデータを再構築できない◆a
		NgHNReconstractNGHNData        = 43003, //!< 高さノードデータを再構築したデータが不正◆a

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
	 * @param lDivID       [in] フィーチャオブジェクトDIVID
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
			long lDivID,
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

