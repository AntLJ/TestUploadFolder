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
#include "Arguments.h"
#include "ConvertTable.h"
#include "OutputFormat.h"
#include "OutputString.h"

/**
 * @brief データ変換クラス
 */
class CADAMConverter
{
public:
	CADAMConverter(){}
	~CADAMConverter(){}

public:
	/**
	 * @brief  初期化
	 * @param args [in] コマンドライン引数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool init(const Arguments& args);

	/**
	 * @brief  処理実行
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool run();

private:
	/** 
	 * @brief フィールドインデックス更新
	 * @note 変換テーブルに設定されているフィールドインデックスを更新する
	 * @param ipCursor [in] 変換元テーブルのカーソル
	 * @param cConvertTableInfo [in,out] 変換テーブル情報
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool updateFieldIndex(const _ICursorPtr& ipCursor, CConvTableInfo& cConvertTableInfo);

	/** 
	 * @brief サブフィールド取得
	 * @note  変換フィールド対応表を元に作成する
	 * @param vecFieldDef [in] 変換対象テーブルの変換フィールド対応表
	 * @param oidField [in] エラー出力用フィールド名
	 * @param subFields [out] サブフィールド
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool getSubFields(const std::vector<CFieldDefInfo>& vecFieldDef, const CString& oidField, CString& subFields);

	/** 
	 * @brief カーソル取得（通常テーブル版）
	 * @param cConvTableInfo [in] 変換対象の変換テーブル情報
	 * @param strSubFields [in] サブフィールド
	 * @param ipCursor [out] 取得カーソル
	 * @param size [out] レコード総数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool getCursorForNormal(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor, long& size);

	/** 
	 * @brief カーソル取得（テーブル結合版）
	 * @param cConvTableInfo [in] 変換対象の変換テーブル情報
	 * @param strSubFields [in] サブフィールド
	 * @param ipCursor [out] 取得カーソル
	 * @param size [out] レコード総数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool getCursorForQueryDef(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor, long& size);

	/** 
	 * @brief カーソル取得（IQueryDef使用）
	 * @param cConvTableInfo [in] 変換対象の変換テーブル情報
	 * @param strSubFields [in] サブフィールド
	 * @param ipCursor [out] 取得カーソル
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool getCursorFromQueryDef(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor);

	/** 
	 * @brief レコード総数取得（テーブル結合版）
	 * @param cConvTableInfo [in] 変換対象の変換テーブル情報
	 * @param size [out] レコード総数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool getRowCountForQueryDef(const CConvTableInfo& cConvTableInfo, long &size);

	/** 
	 * @brief ZM値使用フラグ＋形状タイプ取得
	 * @param strSrcTableName [in] 変換元テーブル名
	 * @param ipCursor [in] 変換対象のカーソル
	 * @param isZ [out] Z値使用フラグ
	 * @param isM [out] M値使用フラグ
	 * @param geomType [out] 形状タイプ
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool getHasZM(const CString& strSrcTableName, const _ICursorPtr& ipCursor, bool& isZ, bool& isM, esriGeometryType& geomType);

	/**
	 * @brief 出力ファイルに書き出し
	 * @param ipCursor [in] 出力文字のエンコード
	 * @param cOutputString [in] 文字出力クラス
	 * @param cConvTableInfo [in] 換対象の変換テーブル情
	 * @param ofs [in,out] 出力ファイルのストリーム
	 * @param size [in] レコード数（進行状況出力用）
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool outputData(const _ICursorPtr& ipCursor, COutputString& cOutputString, const CConvTableInfo& cConvTableInfo, std::ofstream& ofs, long size);
	bool outputUniqData(const _ICursorPtr& ipCursor, COutputString& cOutputString, const CConvTableInfo& cConvTableInfo, std::ofstream& ofs, long size);

	/**
	 * @brief 出力ファイルに書き出し
	 * @param encode [in] 出力文字のエンコード
	 * @param strOutputData [in] 出力文字列
	 * @param ofs [in,out] 出力ファイルのストリーム
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool writeData(const ENCODE::ECode encode, const CString& strOutputData, std::ofstream& ofs);

private:
	CConvertTable m_cConvertTable;	//!< 変換情報
	COutputFormat m_cOutputFormat;	//!< 出力フォーマット
	CString m_strOutputDir;			//!< 出力ディレクトリ
	IWorkspacePtr m_ipSrcWork;		//!< 変換元ワークスペース
	crd_cnv m_cnv;
};

