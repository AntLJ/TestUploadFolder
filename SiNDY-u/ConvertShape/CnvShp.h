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

#include "global.h"

const long MAX_FILE_SIZE = (long)(1.5 * 1024 * 1024 * 1024);	// 1.5GB = 1,610,612,736 [byte]
const long FETCH_SIZE = 100000;

class CCnvShp
{
public:
	CCnvShp(void);
	CCnvShp(const ITablePtr& ipTable,
		    const std::list<PAIR_COL>& listFieldName,
			const std::map<ORG_COLNAME,std::set<CString>>& mapFieldValue,
			const std::map<ORG_COLNAME,TRANS_VALUE>& mapTransValue,
			const std::map<DEST_COLNAME,CComVariant>& mapDefaultValue,
			const std::map<DEST_COLNAME,std::map<CString,CString>>& mapFieldInfo,
			const std::map<DEST_COLNAME,std::map<CString,CString>>& mapAttrValue,
			const IGeometryPtr& ipGeom,
			const CString& strOut,
			bool bNoShape,
			const CString& strWhereQuery,
			const CString& strWhereLoop,
			const CONVERT_INFO& stCnvInfo
			);
	~CCnvShp(void);

	bool Convert();

	/**
	 * @brief シェープファイル名変更
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool ShapeNameChange(const long lDivideNum, const CString csOutFile);

	/**
	 * @brief 変換情報取得
	 */
	void GetConvertInfo(CONVERT_INFO& stCnvInfo);

private:
	/**
	 * @brief 出力用ディレクトリ作成
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool MakeDir();

	/**
	 * @brief 出力用ディレクトリ作成
	 * @param ipBaseFeature [in] クリップ対象フィーチャ
	 * @param ipBaseGeom [in] クリップ対象フィーチャの形状
	 * @param ipClipGeom [in] クリップ形状
	 * @param ipOutGeom [out] クリップ後の形状
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool GetClipGeom(const IFeaturePtr& ipBaseFeature, const IGeometryPtr& ipBaseGeom, const IGeometryPtr& ipClipGeom, IGeometryPtr& ipOutGeom);

	/**
	 * @brief シェープファイル移動
	 * @param strOrgFile [in] 移動元ファイル
	 * @param strDestFile [in] 移動先ファイル
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool MoveShape(const CString& strOrgFile, const CString& strDestFile);

	/**
	 * @brief シェープファイルの属性レコードサイズ取得（1レコード分）
	 * @param listField [in] フィールド情報リスト
	 * @return 1レコードのサイズ
	 */
	long GetSizeOneRecord(const std::list<PAIR_FIELD>& listField);

	/**
	 * @brief シェープファイルの形状のデータサイズ取得（1レコード分）
	 * @param ipRow [in] 処理対象形状のフィーチャ
	 * @param ipGeom [in] 処理対象形状
	 * @param lSize [out] 形状データのサイズ
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool GetSizeGeom(const _IRowPtr& ipRow, const IGeometryPtr& ipGeom, long& lSize);

	/**
	 * @brief クローンフィールド作成
	 * @param ipTable [in] クローン元フィールドのテーブル
	 * @param ipFields [in] クローン元のフィールド
	 * @param strOrgFieldName [in] 変換元のフィールド名称
	 * @param strDstFieldName [in] 変換先のフィールド名称
	 * @param ipDstField [out] クローンフィールド
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool GetOrgCloneField(ITablePtr ipTable, IFieldsPtr ipFields, const CString& strOrgFieldName, const CString& strDstFieldName, IFieldPtr& ipDstField);

	/**
	 * @brief フィールド情報をセット
	 * @param strOrgFieldName [in] 変換元のフィールド名称
	 * @param strDstFieldName [in] 変換先のフィールド名称
	 * @param ipDstField [in,out] 処理対象フィールド
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool SetFieldInfos(const CString& strOrgFieldName, const CString& strDstFieldName, IFieldPtr& ipDstField);

	/**
	 * @brief フィールド情報をセット
	 * @param strFieldName [in] フィールド名称
	 * @param strFieldInfo [in] 設定情報
	 * @param strFieldValue [in] 設定値
	 * @param ipDstField [in,out] 処理対象フィールド
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool SetFieldInfo(const CString& strFieldName, const CString& strFieldInfo, const CString& strFieldValue, IFieldPtr& ipDstField);

	/**
	 * @brief Where句取得
	 * @return Where句
	 */
	CString GetWhere();

private:
	ITablePtr m_ipTable;
	IGeometryPtr m_ipClipGeom;
	CString m_strOutFile;

	std::map<CString,long> m_mapOrgFieldNameIndex;
	
	std::list<PAIR_COL> m_listFieldName;                      //!< 変換元・先フィールド名リスト
	std::map<CString,CString> m_mapOrgDstFieldName;           //!< 変換元・先フィールド名対応表
	std::map<CString,CString> m_mapDstOrgFieldName;           //!< 変換先・元フィールド名対応表
	std::map<ORG_COLNAME,std::set<CString>> m_mapFieldValue;	        //!< 属性指定用map
	std::map<ORG_COLNAME,TRANS_VALUE> m_mapTransValue;                  //!< 丸め込み用map
	std::map<DEST_COLNAME,CComVariant> m_mapDefaultValue;               //!< 新規作成フィールドのデフォルト値
	std::map<DEST_COLNAME,std::map<CString,CString>> m_mapFieldInfo;    //!< フィールド設計情報
	std::map<DEST_COLNAME,std::map<CString,CString>> m_mapAttrValue;    //!< 属性取得用

	CString m_strWhereQuery;          //!< 全体のWhere句
	CString m_strWhereLoop;           //!< フィーチャ取得ループ時のフィルタ
	bool m_bNoShape;                  //!< 形状を出力するかどうか（true:出力しない, false:出力する）
	CONVERT_INFO m_stCnvInfo;		  //!< 変換情報
};

