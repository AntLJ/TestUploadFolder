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

/**
 * @brief 設定ファイル解析用クラス
 */
class CFieldAttr {
public:
	CFieldAttr(){}
	~CFieldAttr(){}
public:
	bool Init(const CString& strFileAttrPath, const std::map<CString,esriFieldType>& mapOrgFieldType);

	std::map<CString,esriFieldType> GetOrgFieldType(){ return m_mapOrgFieldType; }
	std::list<PAIR_COL> GetListFieldName(){ return m_listFieldName; }
	std::map<CString,CString> GetOrgDstFieldName(){ return m_mapOrgDstFieldName; }
	std::map<CString,CString> GetDstOrgFieldName(){ return m_mapDstOrgFieldName; }
	std::map<ORG_COLNAME,std::set<CString>> GetColValue(){ return m_mapFieldValue; }
	std::map<ORG_COLNAME,TRANS_VALUE> GetTransValue(){ return m_mapTransValue; }
	std::map<DEST_COLNAME,CComVariant> GetDefautlValue(){ return m_mapDefaultValue; }
	std::map<DEST_COLNAME,std::map<CString,CString>> GetFieldInfo(){ return m_mapFieldInfo; }
	std::map<DEST_COLNAME,std::map<CString,CString>> GetAttrValue(){ return m_mapAttrValue; }

private:
	/**
	 * @brief 設定ファイル解析
	 * @param strFileAttrPath [in] 設定ファイルパス
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool AnalyzeFieldAttrFile(const CString& strFileAttrPath);

	/**
	 * @brief カラムの意味取得
	 * @param strFieldColumn [in] カラム文字列
	 * return field_attr_mark::ECode
	 */
	field_attr_mark::ECode GetFieldMarkType(const CString& strFieldColumn);

	/**
	 * @brief 設定情報カラム解析
	 * @note この関数を経由して丸め込み、属性指定、フィールド情報、初期値、属性指定＋格納値指定関数へ
	 * @param strOrgFieldName [in] 変換元フィールド名
	 * @param eOrgFieldMark [in] 変換元フィールドオプション情報
	 * @param strDstFieldName [in] 変換先フィールド名
	 * @param strColumnInfo [in] 指定カラム情報
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool SetOptionInfo(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strDstFieldName, const CString& strColumnInfo);

	/**
	 * @brief 丸め込み情報解析
	 * @param strOrgFieldName [in] 変換元フィールド名
	 * @param eOrgFieldMark [in] 変換元フィールドオプション情報
	 * @param strColumnInfo [in] 指定カラム情報
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool SetOptionRound(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief 属性指定情報解析
	 * @param strOrgFieldName [in] 変換元フィールド名
	 * @param eOrgFieldMark [in] 変換元フィールドオプション情報
	 * @param strColumnInfo [in] 指定カラム情報
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool SetOptionDesignation(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief フィールド情報解析
	 * @param strOrgFieldName [in] 変換元フィールド名
	 * @param strDstFieldName [in] 変換先フィールド名
	 * @param eOrgFieldMark [in] 変換元フィールドオプション情報
	 * @param strColumnInfo [in] 指定カラム情報
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool SetOptionFieldInfo(const CString& strOrgFieldName, const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief 初期値情報解析
	 * @param strDstFieldName [in] 変換先フィールド名
	 * @param eOrgFieldMark [in] 変換元フィールドオプション情報
	 * @param strColumnInfo [in] 指定カラム情報
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool SetOptionDefaultValue(const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief 属性指定＋格納値指定情報解析
	 * @param strDstFieldName [in] 変換先フィールド名
	 * @param eOrgFieldMark [in] 変換元フィールドオプション情報
	 * @param strColumnInfo [in] 指定カラム情報
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool SetOptionAttr(const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief 丸め込み時の文字列解析
	 * @param strOrgFieldName [in] 変換元フィールド名
	 * @param strColumnInfo [in] 指定カラム情報
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool GetStrRoundValue(const CString& strOrgFieldName, const CString& strColumnInfo);

	/**
	 * @brief シングルクォート文字列解析
	 * @param strOrgFieldName  [in] 変換元フィールド名
	 * @param strColumnInfoAll [in] 指定カラム情報
	 * @param strColumnInfo    [in] 指定カラム情報
	 * @param str1stValue      [in] オプション文字指定、なければ空文字指定（「/」「=」「:」など）
	 * @param strFindValue     [in] 分割文字列（丸め込み指定の場合「:'test'('hoge' 'boge' 'muge')」変換先の'test'を取得するためには「(」を指定、変換元の'hoge'や'boge'や'muge'を取得するには「 」（半角スペース）を指定する）
	 * @param bFind            [in] 分割文字列が見つからない場合エラーとするかどうか（true=見つからない場合はスルー, false=見つからなかったらエラー）
	 * @param bEscape          [in] 取得した値のシングルクォート「'」の扱い（true=エスケープする（DBに格納、DBと比較用）, false=エスケープしない（SQLのWhere句用））
	 * @param strSplitedValue  [out] strColumnInfoのstrFindValueで分割した値（strColumnInfo「'abc'/'fgb/bac'」をstrFindValue「/」で分割し、strSplitedValue「'abc'」を取得する）
	 * @param strLeftValue     [out] 分割した残りの値（strSplitedValue以外の値「'fgb/bac'」を取得）
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool GetSplitedValue(const CString& strOrgFieldName, const CString& strColumnInfoAll, const CString& strColumnInfo, const CString& str1stValue, const CString& strFindValue, bool bFind, bool bEscape, CString& strSplitedValue, CString& strLeftValue);

	/**
	 * @brief 丸め込み時の数値文字列解析
	 * @param strOrgFieldName [in] 変換元フィールド名
	 * @param strColumnInfo [in] 指定カラム文字列
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool GetNumRoundValue(const CString& strOrgFieldName, const CString& strColumnInfo);

	/**
	 * @brief 文字列取得
	 * @param lStart [in] 開始位置
	 * @param lEnd [in] 終了位置
	 * @param vecRound [in] 「'」分割後の文字列コンテナ
	 * @param bEscSQuart [in] 復元時の「'」の設定方法（true=エスケープする（DBに格納、DBと比較用）, false=エスケープしない（SQLのWhere句用））
	 * @return CString 復元後の文字列
	 */
	CString GetStrValue(long lStart, long lEnd, const std::vector<CString>& vecRound, bool bEscSQuart);

	/**
	 * @brief オプション文字削除（先頭文字削除）
	 * @param strColumnInfo [in] 指定カラム文字列
	 * @return CString オプション文字削除後の文字列
	 */
	CString GetColumnValue(const CString& strColumnInfo);

	/**
	 * @brief 文字列解析
	 * @param strFieldName [in] フィールド名
	 * @param strColumnInfo [in] 指定カラム情報
	 * @param strRetValue [out] 変換後文字列
	 * @param bEscSQuart [in] 復元時の「'」の設定方法（true=エスケープする（DBに格納、DBと比較用）, false=エスケープしない（SQLのWhere句用））
	 * @param bOptionDel [in] 指定カラム情報のオプション文字を削除するかどうか（先頭文字の削除と同義）
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool GetSettingValue(const CString& strFieldName, const CString& strColumnInfo, CString& strRetValue, bool bEscSQuart, bool bOptionDel);

	/**
	 * @brief フィールドタイプ（esriFieldType）取得
	 * @param strOrgFieldName [in] 変換元フィールド名
	 * @param strDstFieldName [in] 変換先フィールド名
	 * @param mapFieldInfo [in] フィールド情報
	 * @param eFieldType [out] フィールドタイプ
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool GetFieldType(const CString& strOrgFieldName, const CString& strDstFieldName, const std::map<CString,CString>& mapFieldInfo, esriFieldType& eFieldType);

	/**
	 * @brief 新規作成フィールドかどうかで設定項目に過不足がないかチェック
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckInfoByNewField();

	/**
	 * @brief フィールド設定情報の格納値チェック
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckFieldInfo();

	/**
	 * @brief フィールド設定情報で新規作成フィールドかどうかで設定項目に過不足がないかチェック
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckFieldTypeByNewField();

	/**
	 * @brief フィールド設定情報でフィールドタイプによる設定項目に過不足がないかチェック
	 * @note  Integer:スケール×、精度○、長さ×
	 * @note  String :スケール×、精度×、長さ○
	 * @note  Double :スケール○、精度○、長さ×
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckFieldInfoByFieldType();

	/**
	 * @brief フィールド設定情報の設定値がフィールドタイプに合っているかチェック
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckFieldInfoValue();

	/**
	 * @brief フィールド設定情報の設定値が範囲内に収まっているか
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckFieldInfoValue2();

	/**
	 * @brief 初期値の設定値がフィールドタイプに合っているかチェック
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckDefaultValue();

	/**
	 * @brief 属性指定の設定値がフィールドタイプに合っているかチェック
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckFieldValue();

	/**
	 * @brief 丸め込み指定の設定値がフィールドタイプに合っているかチェック
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckTransValue();

	/**
	 * @brief 属性切り出しの設定値がフィールドタイプに合っているかチェック
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckAttrValue();

	/**
	 * @brief 設定値がフィールドタイプに合っているかチェック
	 * @param strFieldName [in] 指定フィールドのフィールド名
	 * @param strOption [in] 処理名
	 * @param strValue [in] 設定値
	 * @param eFieldType [in] 指定フィールドのフィールドタイプ
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool CheckValueOfFieldType(const CString& strFieldName, const CString& strOption, const CString& strValue, esriFieldType eFieldType);

private:
	std::map<CString,esriFieldType> m_mapOrgFieldType;        //!< 変換元フィールドタイプ表
	std::list<PAIR_COL> m_listFieldName;                      //!< 変換元・先フィールド名リスト
	std::map<CString,CString> m_mapOrgDstFieldName;           //!< 変換元・先フィールド名対応表
	std::map<CString,CString> m_mapDstOrgFieldName;           //!< 変換先・元フィールド名対応表
	std::map<ORG_COLNAME,std::set<CString>> m_mapFieldValue;            //!< 属性指定用map
	std::map<ORG_COLNAME,TRANS_VALUE> m_mapTransValue;                  //!< 丸め込み用map
	std::map<DEST_COLNAME,CComVariant> m_mapDefaultValue;               //!< 新規作成フィールドのデフォルト値
	std::map<DEST_COLNAME,std::map<CString,CString>> m_mapFieldInfo;    //!< フィールド設計情報
	std::map<DEST_COLNAME,std::map<CString,CString>> m_mapAttrValue;    //!< 属性取得用
};
