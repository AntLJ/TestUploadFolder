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

class CCheckDlcBld {
public:
	CCheckDlcBld(){}
	~CCheckDlcBld(){}

public:
	/**
	 * @brief 初期化関数
	 * @param args   [in] コマンドライン引数
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool init(const Arguments& args);

	/**
	 * @brief 処理実行関数
	 * @param bErr   [out] チェック処理にエラーが存在したかどうか（true=エラーが存在する, false=存在しない）
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool run(bool& bErr);

private:
	/**
	 * @brief メッシュリスト取得関数
	 * @param strMeshlist   [in] メッシュリストのパス
	 * @param setMesh       [out] メッシュリスト
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool getMeshlist(const CString& strMeshlist, std::set<CString>& setMesh);

	/**
	 * @brief shapefileのパス作成関数
	 * @param strMeshlist   [in] strMeshパス
	 * @return shapefileへのパス
	 */
	CString getShapeWorkspacePath(const CString& strMesh);

	/**
	 * @brief 建物形状をベースに装飾線形状が内包されているかチェックする関数
	 * @param strBldName    [in] 建物レイヤ名
	 * @param lBldOid       [in] 建物フィーチャOBJECTID
	 * @param ipBldGeom     [in] 建物形状
	 * @param strDlcName    [in] 装飾線レイヤ名
	 * @param lDlcFid       [in] 装飾線フィーチャFID（OBJECTIDではない）
	 * @param ipDlcGeom     [in] 装飾線形状
	 * @param bMatch        [out] 建物形状に内包されているかどうか（true=内包している, false=内包していない）
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool MatchingBldDlc(const CString& strBldName, long lBldOid, IGeometryPtr ipBldGeom, const CString& strDlcName, long lDlcFid, IGeometryPtr ipDlcGeom, bool& bMatch);

	/**
	 * @brief 建物形状をベースに装飾線形状が内包されているかチェックする関数
	 * @param strBldName            [in] 建物レイヤ名
	 * @param lBldOid               [in] 建物フィーチャOBJECTID
	 * @param ipBldGeom             [in] 建物形状
	 * @param strDlcName            [in] 装飾線レイヤ名
	 * @param lDlcFid               [in] 装飾線フィーチャFID（OBJECTIDではない）
	 * @param vecDlcSegGeom         [in] 装飾線セグメント中点群
	 * @param vecNotMatchDlcSegGeom [out] マッチングしなかった装飾線セグメント中点
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool MatchingBldDlcSegment(const CString& strBldName, long lBldOid, IGeometryPtr ipBldGeom, const CString& strDlcName, long lDlcFid, const std::vector<IGeometryPtr>& vecDlcSegGeom, std::vector<IGeometryPtr>& vecNotMatchDlcSegGeom);

	/**
	 * @brief 形状をセグメント単位に分割し、セグメントの中点を取得する関数
	 * @param strTableName          [in] レイヤ名
	 * @param lOID                  [in] フィーチャOID
	 * @param ipGeom                [in] 形状
	 * @param vecSegCenterPointGeom [out] セグメントの中点形状リスト
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool GetSegmentCenterPoints(const CString& strTableName, long lOID, IGeometryPtr ipGeom, std::vector<IGeometryPtr>& vecSegCenterPointGeom);

	/**
	 * @brief 近隣の建物で検索する関数
	 * @param strBldName        [in] 建物レイヤ名
	 * @param lBldOid           [in] 建物フィーチャOBJECTID
	 * @param ipBldClass        [in] 建物フィーチャクラス
	 * @param lBldObjectIdIndex [in] 建物のOBJECTIDフィールドインデックス
	 * @param strDlcName        [in] 装飾線レイヤ名
	 * @param lDlcFid           [in] 装飾線フィーチャFID（OBJECTIDではない）
	 * @param ipDlcGeom         [in] 装飾線形状
	 * @param vecDlcSegGeom     [in] 装飾線セグメント中点群
	 * @param bSomeMatch        [in] 一部マッチしているかどうか
	 * @param bMsg              [in] マッチング時のメッセージを出力するかどうか（true=出力する, false=出力しない）
	 * @return true  処理成功
	 * @return false 処理失敗
	 */
	bool ArouondBld(const CString& strBldName, long lBldOid, IFeatureClassPtr ipBldClass, long lBldObjectIdIndex, const CString& strDlcName, long lDlcOid, IGeometryPtr ipDlcGeom, const std::vector<IGeometryPtr>& vecDlcSegGeom, bool bSomeMatch, bool bMsg = true);

private:
	ISpatialReferencePtr m_ipSpRef;   //!< 空間参照
	std::set<CString> m_setMesh;      //!< メッシュリスト
	CString m_strShapeDir;            //!< チェック対象Shapefileのベースパス
};
