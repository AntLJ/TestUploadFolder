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
#include <atlctrls.h>


/**
* @brief リストボックスコントロール管理用クラス
* @note  「駅名」と「路線名」のリストボックスを管理する
*/
class CSearchListCtrls
{
public:
	/// コンストラクタ
	CSearchListCtrls(void);
	/// デストラクタ
	~CSearchListCtrls(void);

	/// 紐付ける情報を保持する用構造体
	struct JoinLineInfo
	{
		JoinLineInfo( long id, long railway_line, CString station, CString line ):
			oid(id),railway_line_id(railway_line),station_name(station),line_name(line){}

		long oid;               //!< JOIN_LINE_INFOのOBJECTID
		long railway_line_id;   //!< RAILWAY_LINEのOBJECTID（JOIN_LINE_INFO.LINEID）
		CString station_name;   //!< 駅名（STATION_INFO.NAME）
		CString line_name;      //!< 路線名（RAILWAY_LINE.NAME）
	};


	/**
	* @brief 初期化関数
	* @param station_list [in] 駅名リストボックスのHWND
	* @param line_list [in]    路線名リストボックスのHWND
	*/
	void init( HWND station_list, HWND line_list );
	/**
	* @brief リストボックスの中身を更新する
	* @note  周辺の駅POIポイントを空間検索し、関連するテーブルを紐付けて駅名、路線名をリスト表示する
	* @param ipNodeFeature [in] 周辺検索する中心のノード（ポイント）
	* @param dRange [in] 検索範囲（単位：m）
	* @param [in] 検索する駅POIのフィーチャクラス
	*/
	void reflesh( IFeaturePtr ipNodeFeature, double dRange, IFeatureClassPtr ipPoiClass );
	/**
	* @brief リストボックスの項目を選択する
	* @note 保持している（reflesh()で検索された）情報に紐づく駅名、路線名を検索し、UI上で選択された状態にする
	* @param join_line_info_id [in] 選択対象の乗り入れ路線ID
	*/
	void select( long join_line_info_id );
	/**
	* @brief UIで選択されている駅名と路線名に紐づく乗り入れ路線IDを取得する
	* @return 乗り入れ路線ID
	*/
	long GetJoinLineInfoID();
	/**
	* @brief 路線IDに紐づく乗り入れ路線IDを取得する
	* @param railway_line_id [in] 鉄道路線ID
	* @return 乗り入れ路線ID
	*/
	long GetJoinLineInfoIDFromRailWayLine( long railway_line_id );
	/**
	* @brief 乗り入れ路線IDに紐づく路線IDを取得する
	* @param join_line_info [in] 乗り入れ路線ID
	* @return 鉄道路線ID
	*/
	long GetRailWayLineIDFromJoinLineInfo( long join_line_info );

	/**
	* @brief 駅名の選択が変更されたときに呼ばれる（べき）関数
	*/
	void StationSelChange();
	/**
	* @brief 路線名の選択が変更されたときに呼ばれる（べき）関数
	*/
	void LineSelChange();

public:
	CString selectedStation; //!< 選択されている駅名
	CString selectedLine;    //!< 選択されている路線名

private :
	CListBox m_lvStation;    //!< 駅名リストボックスコントロール
	CListBox m_lvLine;       //!< 路線名リストボックスコントロール

	std::map<long, std::list<JoinLineInfo>> m_JoinLineInfos; //!< 駅POIポイントに紐づく駅名、路線名情報
};

