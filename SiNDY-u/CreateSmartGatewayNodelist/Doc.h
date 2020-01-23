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
#include "SDEManager.h"
#include <atl2/seal.h>

struct FacilInfo
{
	long	eRoad_Code;
	long	eRoad_Seq;
};

class CDoc
{
private:
	/**
	 * @brief DB処理クラス
	 */
	CSDEManager m_cSDEManager;
	/**
	 * @brief 対象サーバ名
	 */
	CString m_strServer;
	/**
	 * @brief 対象インスタンス
	 */
	CString m_strInstance;
	/**
	 * @brief 対象ユーザ名
	 */
	CString m_strUser;
	/**
	 * @brief 対象パスワード
	 */
	CString m_strPassword;
	/**
	 * @brief 対象バージョン
	 */
	CString m_strVersion;
	/**
	 * @brief 出力ディレクトリ
	 */
	CString m_strOutputDir;
	/**
	 * @brief 対象DBワークスペース
	 */
	IWorkspacePtr	m_ipWorkspace;
	/**
	 * @brief 出力ストリーム
	 */
	ofstream m_ofstream;
public:
	/*
	 * @brief コンストラクタ
	 */
	CDoc(void);
	/*
	 * @brief デストラクタ
	 */
	~CDoc(void);
	/**
	 * @brief インポート設定の取得
	 * @note  設定ファイルを読み込み、インポートの設定を取得する
	 * @param strConfigFile [in] 設定ファイル名
	 * @retval true 取得成功
	 * @retval false 取得失敗
	 */
	bool getenv( const char* strConfigFile );
	/**
	 * @brief メインの処理
	 * @return bool
	 */
	bool Proc(void);
	/**
	 * @brief FACIL_INFO_POINTからスマートICのROAD_CODE,ROAD_SEQの取得
	 * @param r_eFacilInfoVector [out] ROAD_CODE,ROAD_SEQ格納用ベクター
	 * @return bool
	 */	
	bool GetSmartICPoint(vector<FacilInfo>& r_eFacilInfoVector);
	/**
	 * @brief HIGHWAY_NODEのジオメトリ取得
	 * @param r_eFacilInfoVector	[in]  ROAD_CODE,ROAD_NODEが格納されたベクター
	 * @param r_GeomVector			[out] ジオメトリ格納用ベクター
	 * @return bool
	 */
	bool GetHwyNodeGeom(const vector<FacilInfo>& r_eFacilInfoVector, vector<atl2::seal<IGeometryPtr>>& r_GeomVector);
	/**
	 * @brief ROAD_NODE のオブジェクトIDの取得
	 * @prama r_GeomVector	[in]　HIGHWAY_NODE　ジオメトリ格納ベクター
	 * @param r_ObjectIDSet	[out] OBJECTID格納用セット
	 * @return bool
	 */
	bool GetRoadNodeOID(const vector<atl2::seal<IGeometryPtr> >& r_GeomVector, set<long>& r_ObjectIDSet);
	/**
	 * @brief SDEのオープン
	 * @note 設定されているSDEに接続し、対象レイヤを開く
	 * @retval true 取得成功
	 * @retval false 取得失敗
	 */
	bool OpenSDE(void);
	/**
	 * @brief SDEクローズ
	 */
	void Close();
	/*
	 * @brief 出力ストリームの設定
	 */
	bool SetOfstream();
};
