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
#include "TransParam.h"
#include "FieldAttr.h"

class CConvertShape {
public:
	CConvertShape(){}
	~CConvertShape(){}

public:
	/**
	 * @brief 初期化
	 * @param cTransParam [in] コマンドライン引数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool Init(const CTransParam& cTransParam);

	bool run();

private:
	/**
	 * @brief メッシュ関連初期化
	 * @note  メッシュクラス、メッシュ形状、出力パスの設定を行う
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool InitMesh();

	/**
	 * @brief メッシュ関連初期化
	 * @note  InitMesh()から呼び出される関数
	 * @param strMeshlistPath [in] メッシュリストへのパス
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool LoadMeshlist(const CString& strMeshlistPath);

	/**
	 * @brief メッシュクラスのテーブル名取得
	 * @note  メッシュコードが4,6,8桁で接続するメッシュクラスが違うため
	 * @param lDigit [in] メッシュコードの桁数
	 * @param strMeshCode [in] メッシュコード
	 * @param strMeshTableName [out] メッシュクラスのテーブル名
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool GetMeshTableName(long lDigit, const CString& strMeshCode, CString& strMeshTableName);

	/**
	 * @brief メッシュ形状取得
	 * @note  メッシュコードのメッシュ形状取得
	 * @param lMeshCode [in] メッシュコード
	 * @param ipMeshGeom [out] メッシュ形状
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool GetMeshGeom(long lMeshCode, IGeometryPtr& ipMeshGeom);

	/**
	 * @brief SHAPE出力先のパス作成
	 * @param lMeshCode [in] メッシュコードの桁数
	 * @param strMeshCode [in] メッシュコード
	 * @param strOutputDir [out] SHAPE出力先のパス
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool CreateOutputDir(long lMeshDigit, const CString& strMeshCode, CString& strOutputDir);

private:
	CTransParam m_cTransParam;
	CFieldAttr m_cFieldAttr;

	IWorkspacePtr m_ipTargetWork;
	ITablePtr m_ipTargetTable;
	std::list<PAIR_COL> m_listColName;
	IWorkspacePtr m_ipMeshWork;
	IFeatureClassPtr m_ipMeshClass;  //!< 処理によってCity,Base,Middleになる
	//MESHLIST m_setMeshlist;
	std::map<long,std::pair<CString,CAdapt<IGeometryPtr>>> m_mapMeshGeom;
	std::map<ORG_COLNAME, CString> m_mapColValue;			// 属性指定用map
	std::map<ORG_COLNAME, TRANS_VAL> m_mapTransValue;		// 丸め込み用map
};
