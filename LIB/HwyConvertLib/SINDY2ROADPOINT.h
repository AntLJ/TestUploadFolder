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

// SINDY2ROADPOINT.h: SINDY2ROADPOINT クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2ROADPOINT_H__42268805_C56C_44B7_8181_34D96A3EC8B1__INCLUDED_)
#define AFX_SINDY2ROADPOINT_H__42268805_C56C_44B7_8181_34D96A3EC8B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"
#include "Customer.h"
#include "FacilNameChangeTable.h"

// roadpoint.txtの１レコードの情報を格納する構造体
struct RoadPoint
{
	bool				eHwyModeFlag;				/// ハイウェイモードフラグ

	long				ePrefMesh;					/// 代表ポイント2次メッシュコード
	long				ePrefX;						/// 代表ポイント正規化X座標
	long				ePrefY;						/// 代表ポイント正規化Y座標
	long				eAccessPointID;				/// 取得元Access_PointのObjectID（メッシュ内シーケンス番号、施設ポイントID）
	long				eMesh;						/// 施設ポイント2次メッシュコード
	long				eX;							/// 施設ポイント正規化X座標
	long				eY;							/// 施設ポイント正規化Y座標
	long				eAdmin;						/// 市区町村コード
	long				eFacilInfoPointAdmin;		/// 施設代表ポイントの市区町村コード
	long				eFacilInfoPointID;			/// 属する施設のFacil_Info_PointのObjectID（施設ID）
	long				eSeq;						/// 路線内シーケンス番号
	std::string			eName;						/// 道路施設名称
	std::string			eYomi;						/// 施設名称読み
	long				eRoadCode;					/// 路線コード
	long				eFacilClassAndInOutCode;	/// 施設種別＋出入口コード
	long				eFacilClass;				/// 施設種別コード
	long				eInOut;						/// 出入口コード
	long				ePntPos;					/// ポイント位置コード
	std::string			ePntName;					/// ポイント位置名称
	std::string			ePntYomi;					/// ポイント位置読み
	std::vector<long>	eSapaInfo;					/// SA/PA情報（25桁）
	long				eOriginal;					/// 独自名称フラグ
	long				eDummyFacil;				/// ダミー施設情報フラグ

	// 以下はSiRiUS用
	long				eSearchClass;				/// 検索用施設種別コード
	long				ePrefMeshForSirius;			/// 代表ポイント2次メッシュコード
	long				ePrefXForSirius;			/// 代表ポイント正規化X座標
	long				ePrefYForSirius;			/// 代表ポイント正規化Y座標
	long				eDirection;					/// 上下線種別コード [Bug6428]で追加

	RoadPoint();
	virtual ~RoadPoint() {}
};

// roadpoint.txtのレコードを、路線コード＋路線内シーケンス番号を基準に並べるためのファンクタ
struct lessRoadPoint : public std::binary_function<RoadPoint,RoadPoint,bool>
{
	bool operator ()(const RoadPoint &rp1, const RoadPoint &rp2) const
	{
		// 路線内施設位置コードとは別に格納した路線コードと路線内シーケンス番号を基準にソートするように修正
		return (rp1.eRoadCode != rp2.eRoadCode) ? rp1.eRoadCode < rp2.eRoadCode : rp1.eSeq < rp2.eSeq;
	}
};

typedef std::multiset<RoadPoint,lessRoadPoint> RoadPointArray;


class SINDY2ROADPOINT : public SINDY2INHWY  
{
public:
	SINDY2ROADPOINT(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2ROADPOINT();

	virtual HRESULT Convert(void);
	virtual HRESULT Write			(std::string cFileName, bool cHwyModeFlag, long cFormat, long cJctDataType);
	HRESULT makeJctBranchData(void);

	/**
	 * @brief 施設名称変換（仕向け先指定）
	 * @note	施設名称変換が不要な仕向け先が指定された場合は何もしない。
	 * @param	cCustomer	[in]	仕向け先
	 * @param	cpFacilNameChangeTable	[in]	施設名称変換テーブル
	 */
	void changeFacilName( Customer cCustomer, FacilNameChangeTable* cpFacilNameChangeTable );

	/**
	 * @brief 施設名称変換
	 * @param	cpFacilNameChangeTable	[in]	施設名称変換テーブル
	 */
	void changeFacilName( FacilNameChangeTable* cpFacilNameChangeTable );

private:
	virtual HRESULT Write			(RoadPointArray* cpData, std::string cFileName, bool cHwyModeFlag, long cFormat);
	virtual HRESULT WriteForSirius	(RoadPointArray* cpData, std::string cFileName, long cFormat);
	virtual HRESULT WriteAll		(std::string cFileName);
	void changeFacilName(FacilNameChangeTable* cpFacilNameChangeTable, RoadPointArray& crData, std::string cFileName);

public:
	static HRESULT GetHwyModeFlag				(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, bool *cTarget);
	static HRESULT GetPrefPoint					(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY);
	static HRESULT GetPrefPointForSirius		(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY);
	static HRESULT GetMeshSequence				(IFeaturePtr ipAccessPoint, long *cMeshSeq);
	static HRESULT GetBaseMeshCoord				(IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY);
	static HRESULT GetBaseAdmin					(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cAdminCode);
	static HRESULT GetFacilInfoPointAdmin		(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cAdminCode);
	static HRESULT GetFacilInfoPointID			(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cID);
	static HRESULT GetSequence					(IFeaturePtr ipAccessPoint, long *cSequence);
	static HRESULT GetSearchClass				(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cSearchClass);
	static HRESULT GetRoadCode					(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cRoadCode);
	static HRESULT GetFacilClassAndInOutCode	(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cFacilClass);
	static HRESULT GetPntPos					(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cPntPos, std::set<long> *cWasteID = NULL);
	static HRESULT GetName						(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, std::string *cName, std::string *cYomi);
	static HRESULT GetPntName					(IFeaturePtr ipAccessPoint, long cPntPos, std::string *cName, std::string *cYomi);
	static HRESULT GetSapaInfo					(IFeaturePtr ipAccessPoint, std::vector<long> *cSapaInfo);
	static HRESULT GetOriginalFlag				(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cOriginal);
	static HRESULT GetDummyFacilFlag			(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cDummyFacilFlag);

private:
	RoadPointArray mRoadPointArray;			// jct_interflowのデータ（主にroadpoint.txt用）
	RoadPointArray mRoadPointBranchArray;	// jct_branchのデータ（主にroadpoint.bunnki.txt用）

public:
	typedef RoadPointArray::const_iterator Iter;

	/**
	 * @brief データ列の先頭を指すconst_iteratorを返す
	 * @param[in]	isBranch	trueならjct_branchのデータ、falseなら通常（jct_interflow）のデータを扱う
	 */
	Iter begin( bool isBranch = false ) const { return isBranch ? mRoadPointBranchArray.begin() : mRoadPointArray.begin(); }

	/**
	 * @brief データ列の末尾を指すconst_iteratorを返す
	 * @param[in]	isBranch	trueならjct_branchのデータ、falseなら通常（jct_interflow）のデータを扱う
	 */
	Iter end( bool isBranch = false ) const { return isBranch ? mRoadPointBranchArray.end() : mRoadPointArray.end(); }

	/**
	 * @brief データ列のサイズを返す
	 * @param[in]	isBranch	trueならjct_branchのデータ、falseなら通常（jct_interflow）のデータを扱う
	 */
	std::size_t size( bool isBranch = false ) const { return isBranch ? mRoadPointBranchArray.size() : mRoadPointArray.size(); }
};

#endif // !defined(AFX_SINDY2ROADPOINT_H__42268805_C56C_44B7_8181_34D96A3EC8B1__INCLUDED_)
