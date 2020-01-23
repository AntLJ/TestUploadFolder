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

/**
 * @brief シェープ、SDEなどのレコードデータ確保用のクラス
 */
class CRecodeBase {
public:
	CRecodeBase(){}
	~CRecodeBase(){}

public:
	/**
	 * @brief 属性値確保用関数（IFeature用）
	 * @param strTableName         [in] フィーチャのテーブル名
	 * @param ipFeature            [in] 属性取得対象フィーチャ
	 * @param mapFieldIndex2Name   [in] フィールドインデックスとフィールド名称のマップ
	 * @param mapFieldName2Index   [in] フィールド名称とフィールドインデックスのマップ
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	virtual bool SetData(const CString& strTableName, IFeaturePtr ipFeature, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index);

	/**
	 * @brief 属性値確保用関数（IRow用）
	 * @param strTableName         [in] レコードのテーブル名
	 * @param ipRow                [in] 属性取得対象レコード
	 * @param mapFieldIndex2Name   [in] フィールドインデックスとフィールド名称のマップ
	 * @param mapFieldName2Index   [in] フィールド名称とフィールドインデックスのマップ
	 * @param strOIDFieldName      [in] OBJECTIDフィールドが存在しない時の代用フィールド
	 * @param strDivIDFieldName    [in] OBJECTIDフィールドが存在しない時の代用フィールド
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	virtual bool SetData(const CString& strTableName, _IRowPtr ipRow, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index, const CString& strOIDFieldName = _T(""), const CString& strDivIDFieldName = _T(""));

public:
	UNIQID eObjectID;                   //!< OBJECTID＋DIVID
	std::vector<CComVariant> eVecAttr;  //!< 形状以外の属性値確保用
	std::vector<LONLAT> eVecLonLat;     //!< 緯度経度に落とし込んだ形状情報
};


/**
 * @brief シェープ、SDEなどのテーブル情報確保用
 * @note レコード情報も確保する
 */
class CTableBase {
public:
	CTableBase() : m_bBefore(false){}
	~CTableBase(){}

public:
	/**
	 * @brief 初期化用関数
	 * @param bBefore       [in] 変換前か変換後テーブルか
	 * @param ipWork        [in] ワークスペース
	 * @param strTableName  [in] テーブル名称
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	virtual bool Init(bool bBefore, IWorkspacePtr ipWork, const CString& strTableName);

	/**
	 * @brief データ読み込み用関数
	 * @note 継承先で定義させる
	 */
	virtual bool LoadData() = 0;

protected:
	/**
	 * @brief フィールド情報取得関数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	virtual bool SetFieldInfo(const CString& strTableName, ITablePtr ipTable);

public:
	bool m_bBefore;                                 //!< true:変換前, false:変換後
	CString m_strTableName;                         //!< テーブル名称（before,after付）
	IFeatureClassPtr m_ipFeatureClass;              //!< フィーチャクラス
	ITablePtr m_ipTable;

	std::map<long,CString> m_mapFieldIndex2Name;    //!< テーブルのフィールドインデックスとフィールド名称のマップ
	std::map<CString,long> m_mapFieldName2Index;    //!< テーブルのフィールド名とフィールドインデックスのマップ

	std::map<long,CString> m_mapAttrIndex2Name;     //!< 属性確保用のフィールドインデックスとフィールド名称のマップ
	std::map<CString,long> m_mapAttrName2Index;     //!< 属性確保用のフィールドインデックスとフィールド名称のマップ

};

