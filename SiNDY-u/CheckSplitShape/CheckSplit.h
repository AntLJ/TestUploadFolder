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

class CCheckSplit {
public:
	CCheckSplit() : m_CheckMode(check_mode::kNone), m_lMeshDigit(0), m_bJGD2000(false){}
	~CCheckSplit(){}

public:
	bool Init(const Arguments& argument);
	bool run(bool &bCheckErr);

private:
	/**
	 * @brief メッシュリスト取得
	 * @param strMeshlistPath  [in] メッシュリストへのパス
	 * @retval true  処理成功
	 * @retval true  処理失敗
	 */
	bool LoadMeshlist(const CString& strMeshlistPath);

	/**
	 * @brief シェープファイルへのパス取得
	 * @param strBasePath  [in] ベースパス
	 * @param strMeshCode  [in] メッシュコード
	 * @return シェープファイルのパス
	 */
	CString GetWorkShpPath(const CString& strBasePath, const CString& strMeshCode);

public:
	CString m_strBeforeShpPath;              //!< 変換前シェープファイル格納先ディレクトリ
	CString m_strBeforeDBPath;               //!< 変換前DB接続先
	CString m_strAfterShpPath;               //!< 変換後シェープファイル格納先ディレクトリ
	CString m_strMeshDBPath;                 //!< メッシュ形状用DBパス
	CString m_strMeshSchema;                 //!< メッシュ形状用スキーマ名
	check_mode::ECode m_CheckMode;           //!< チェックモード
	std::set<CString> m_setMeshList;         //!< メッシュリスト
	bool m_bJGD2000;                         //!< JGD2000化するかどうか（高さノード用）
	double m_dMinSegment;                    //!< 微小セグメント長

	IFeatureClassPtr m_ipMeshClass;          //!< メッシュ形状取得用フィーチャクラス
	IFeatureClassPtr m_ipBeforeHNodeClass;   //!< 変換前相対高さフィーチャクラス
	ITablePtr m_ipAfterHNodeTable;           //!< 変換後相対高さテーブル

	long m_lMeshDigit;                       //!< 
};

