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
#include "FieldDef.h"

/**
 * @brief 検索方法タイプ
 */
namespace SEARCHTYPE {
	enum ECode {
		UNKNOWN_TYPE = 0,	//!< 未設定
		NORMAL,				//!< 通常版の検索
		QUERYDEF_LIMITED,	//!< QueryDefを使用したテーブル結合検索（機能限定版）
		QUERYDEF_FREE		//!< QueryDefを使用したテーブル結合検索（限定解除版）
	};
}

// 変換テーブルクラス
class CConvTableInfo {
public:
	CConvTableInfo() : m_searchType(SEARCHTYPE::UNKNOWN_TYPE){}
	~CConvTableInfo(){}

public:
	void clear(){
		m_GroupName.Empty();
		m_searchType = SEARCHTYPE::UNKNOWN_TYPE;
		m_srcTableName.Empty();
		m_dstFileName.Empty();
		m_subTableNames.Empty();
		m_joinKeys.Empty();
		m_oidTableName.Empty();
		m_oidField.Empty();
		m_postfix.Empty();
		m_prefix.Empty();
		m_vecFieldDef.clear();
		m_vecOrderbyField.clear();
	};

	/**
	 * @brief ORDER BY句に指定されたフィールドリストを取得
	 */
	void CreateOrderByFieldList();

public:
	CString m_GroupName;		//!< グループ名
	SEARCHTYPE::ECode m_searchType;	//!< 検索タイプ
	CString m_srcTableName; 	//!< 変換元テーブル名
	CString m_dstFileName;		//!< 変換先ファイル名
	CString m_subTableNames;	//!< テーブル結合用テーブル名
	CString m_joinKeys;			//!< 変換元結合キー
	CString m_oidTableName; 	//!< エラー出力用のID特定用テーブル名
	CString m_oidField;			//!< エラー出力用のID特定用フィールド名
	CString m_postfix;			//!< QueryDef->PostfixClause用
	CString m_prefix;			//!< QueryDef->PrefixClause用
	std::vector<CFieldDefInfo> m_vecFieldDef;	//!< 変換フィールド対応表
	std::vector<CString> m_vecOrderbyField;	//!< postfixで指定されたORDER BY句のフィールドリスト
};

// 変換情報クラス
class CConvertTable {
public:
	CConvertTable(){}
	~CConvertTable(){}

public:
	/**
	 * @brief 初期化
	 * @param convTalePath [in] 変換テーブルのパス
	 * @param convFieldPath [in] 変換フィールド対応表のパス
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool init(const CString& convTablePath, const CString& convFieldPath);

	/**
	 * @brief 変換テーブルと変換フィールド対応表のチェック
	 * @note 実データとして存在するかのチェックを行う
	 * @param ワークスペース [in] ipFWork
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool checkConvTableFeild(const IFeatureWorkspacePtr& ipFWork);

private:
	/**
	 * @brief 変換テーブル読み込み
	 * @param convTablePath [in] 変換テーブルのパス
	 * @param mapConvTableInfo [out] 変換情報セット
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool loadConvTable(const CString& convTablePath, std::map<CString,CConvTableInfo>& mapConvTableInfo);

	/**
	 * @brief 変換フィールド対応表読み込み
	 * @param convFieldPath [in] 変換フィールド対応表のパス
	 * @param mapConvTableInfo [out] 変換情報セット
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool loadConvField(const CString& convFieldPath, std::map<CString,CConvTableInfo>& mapConvTableInfo);

	/**
	 * @brief 変換情報グループチェック
	 * @note loadTableField,loadConvFieldで取得した変換情報セットのグループに差異がないか
	 * @param mapConvTableInfo1 [in] 変換情報セット1
	 * @param mapConvTableInfo2 [in] 変換情報セット2
	 * @retval true 差異なし
	 * @retval false 差異あり
	 */
	bool checkConvGroup(const std::map<CString,CConvTableInfo>& mapConvTableInfo1, const std::map<CString,CConvTableInfo>& mapConvTableInfo2);

	/**
	 * @brief 変換テーブルチェック
	 * @param cConvTableInfo [in] 変換情報セット
	 * @retval true 属性不正なし
	 * @retval false 属性不正あり
	 */
	bool checkConvTableInfo(const CConvTableInfo& cConvTableInfo);

	/**
	 * @brief 変換フィールド対応表チェック
	 * @param cConvTableInfo [in] 変換情報セット
	 * @retval true 属性不正なし
	 * @retval false 属性不正あり
	 */
	bool checkConvFieldInfo(const CConvTableInfo& cConvTableInfo);

public:
	std::map<CString,CConvTableInfo> m_mapConvTableInfo; //!< 変換情報
};
