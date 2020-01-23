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

#include "LayerBase.h"

/**
 * @brief RWLのグループ化を管理するための構造体
 */
struct RWLGROUP {
	long eGroupID;                   //!< グループID
	std::vector<long> eVecDivID;     //!< グループに所属するDIVID群（格納順はDIVID順）
	std::vector<LONLAT> eVecLonLat;  //!< DIVID群でグループ化された形状
};

/**
 * @brief RWLのレコード単位の統括クラス
 */
class CRWLDiv : public CRecodeBase {
public:
	CRWLDiv(){}
	~CRWLDiv(){}
public:
	virtual bool SetData(const CString& strTableName, IFeaturePtr ipFeature, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index);

public:
	UNIQID eFromNodeID;
	UNIQID eToNodeID;
};


/**
 * @brief RWLデータ統括クラス
 */
class CRWLAll: public CTableBase {
public:
	CRWLAll(){}
	~CRWLAll(){}

public:
	/**
	 * @brief RWL用データ読み込み
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	virtual bool LoadData();

	/**
	 * @brief グループ化
	 * @note 分割前データ用の関数
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool ConstractGroupBefore();

	/**
	 * @brief グループ化
	 * @note 分割後データ用の関数
	 * @param mapBeforeDivGroup  [in] 分割前グループ情報
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool ConstractGroupAfter(const std::map<long,std::vector<RWLGROUP>>& mapBeforeDivGroup);

private:
	/**
	 * @brief グループ化した区間の形状取得
	 * @param lOID            [in] 取得対象のOID
	 * @param vecDivID        [in] 取得対象のグループ化した区間（DIVID群）
	 * @param vecGroupLonLat  [out] 取得形状
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool ConstractGroupLonLat(long lOID, const std::vector<long>& vecDivID, std::vector<LONLAT>& vecGroupLonLat);

public:
	std::map<UNIQID,CRWLDiv> m_mapData;                   //!< DIVID単位のデータ情報
	std::map<UNIQID,std::set<UNIQID>> m_mapNode2Link;     //!< ノードIDとリンクIDのマップ
	std::map<long,std::set<long>> m_mapDivOID;            //!< オブジェクトIDに所属するDIVIDのグループ
	std::map<long,std::vector<RWLGROUP>> m_mapDivGroup;   //!< グループ情報（OID,グループID,グループ情報）

	std::map<std::set<LONLAT>,UNIQID> m_mapWalkHCSegment;     //!< 人車共用歩行者リンクのセグメントリスト（歩行者リンクで歩行者リンク種別が人車共用の場合）
	std::map<std::set<LONLAT>,UNIQID> m_mapRoadWalkHCSegment; //!< 人車共用リンクのセグメントリスト（歩行者リンクで歩行者リンク種別が人車共用の場合、もしくは、walkable_cが人車共用の場合）
};
