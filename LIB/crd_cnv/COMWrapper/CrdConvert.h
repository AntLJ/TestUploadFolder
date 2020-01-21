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

// CrdConvert.h : CCrdConvert の宣言

#pragma once
#include "resource.h"       // メイン シンボル

#include "crdcnvLib_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM の完全サポートを含んでいない Windows Mobile プラットフォームのような Windows CE プラットフォームでは、単一スレッド COM オブジェクトは正しくサポートされていません。ATL が単一スレッド COM オブジェクトの作成をサポートすること、およびその単一スレッド COM オブジェクトの実装の使用を許可することを強制するには、_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA を定義してください。ご使用の rgs ファイルのスレッド モデルは 'Free' に設定されており、DCOM Windows CE 以外のプラットフォームでサポートされる唯一のスレッド モデルと設定されていました。"
#endif

/**
* @class crd_cnvをVB.NETで使用するためのCOMラッパ
*/
class ATL_NO_VTABLE CCrdConvert :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCrdConvert, &CLSID_CrdConvert>,
	public ICoordConverter3
{
public:
	CCrdConvert()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CRDCONVERT)

BEGIN_COM_MAP(CCrdConvert)
	COM_INTERFACE_ENTRY(ICoordConverter)
	COM_INTERFACE_ENTRY(ICoordConverter2)
	COM_INTERFACE_ENTRY(ICoordConverter3)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	/**
	* @brief メッシュ + (X,Y)から経緯度へ変換
	* @param mesh [in] メッシュコード
	* @param x [in] X座標
	* @param y [in] Y座標
	* @param lon [out] 緯度
	* @param lat [out] 経度
	*/
	STDMETHOD(MeshtoLL)(LONG mesh, LONG x, LONG y, DOUBLE* lon, DOUBLE* lat);

	/**
	* @brief 経緯度から、メッシュコード + (x,y) を求める
	* @param lon [in] 緯度
	* @param lat [in] 経度
	* @param level [in] 変換レベル
	* @param mesh [out] メッシュコード
	* @param x [out] X座標
	* @param y [out] Y座標
	*/
	STDMETHOD(LLtoMesh)(DOUBLE lon, DOUBLE lat, LONG level, LONG* mesh, LONG* x, LONG* y);

	/**
	* @brief 原点メッシュから(x,y)メッシュ離れた所のメッシュコードを求める
	* @param code [in] メッシュコード
	* @param xdist [in] X値
	* @param ydist [in] Y値
	* @param mesh [out] メッシュコード
	*/
	STDMETHOD(GetMeshCode)(LONG code, LONG xdist, LONG ydist, LONG* mesh);

	/**
	* @brief ２地点のメッシュ+(x,y)から、２点間の距離を求める（単位: m) 
	* @param mesh1 [in] メッシュコード
	* @param x1 [in] X座標
	* @param y1 [in] Y座標
	* @param mesh2 [in] メッシュコード
	* @param x2 [in] X座標
	* @param y2 [in] Y座標
	* @param dist [out] ２点間の距離
	*/
	STDMETHOD(GetMeshDist)(LONG mesh1, LONG x1, LONG y1, LONG mesh2, LONG x2, LONG y2, DOUBLE* dist);

	/**
	* @brief Tokyo->JGD2000への座標変換
	* @param before_lon [in] 変換前経度
	* @param before_lat [in] 変換前緯度
	* @param before_he [in] 楕円体高(m)
	* @param after_lon [out] 変換後経度
	* @param after_lat [out] 変換後緯度
	* @param after_he [out] 楕円体高(m)
	*/
	STDMETHOD(TOKYOtoJGD2000)(DOUBLE before_lon, DOUBLE before_lat, DOUBLE before_he, DOUBLE* after_lon, DOUBLE* after_lat, DOUBLE* after_he);

	/**
	* @brief JGD2000->Tokyoへの座標変換
	* @param before_lon [in] 変換前経度
	* @param before_lat [in] 変換前緯度
	* @param before_he [in] 楕円体高(m)
	* @param after_lon [out] 変換後経度
	* @param after_lat [out] 変換後緯度
	* @param after_he [out] 楕円体高(m)
	*/
	STDMETHOD(JGD2000toTOKYO)(DOUBLE before_lon, DOUBLE before_lat, DOUBLE before_he, DOUBLE* after_lon, DOUBLE* after_lat, DOUBLE* after_he);

	/**
	* @brief パラメータファイル読み込み
	* @param bstrFile [in] パラメータファイルパス
	* @param bRet [out] 読み込みが成功したかどうか
	*/
	STDMETHOD(JGD2000_RP_Init)(BSTR bstrFile, VARIANT_BOOL* bRet);

	/**
	* @brief Tokyo->JGD2000への座標変換
	* @param before_lon [in] 変換前経度
	* @param before_lat [in] 変換前緯度
	* @param after_lon [out] 変換後経度
	* @param after_lat [out] 変換後緯度
	* @param biPCFlag [in] IPC固有変換をするかどうか
	* @param bRet [out] パラメータが読み込まれたかどうか
	*/
	STDMETHOD(TOKYOtoJGD2000_RP)(DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_lon, DOUBLE* after_lat, VARIANT_BOOL biPCFlag, VARIANT_BOOL* bRet);

	/**
	* @brief JGD2000->Tokyoへの座標変換
	* @param before_lon [in] 変換前経度
	* @param before_lat [in] 変換前緯度
	* @param after_lon [out] 変換後経度
	* @param after_lat [out] 変換後緯度
	* @param biPCFlag [in] IPC固有変換をするかどうか
	* @param bRet [out] パラメータが読み込まれたかどうか
	*/
	STDMETHOD(JGD2000toTOKYO_RP)(DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_lon, DOUBLE* after_lat, VARIANT_BOOL biPCFlag, VARIANT_BOOL* bRet);

		/**
	* @brief 19座標系->経緯度への座標変換
	* @param axis [in] 該当する座標系の番号
	* @param before_x [in] 変換前の19座標系のX(緯度方向)
	* @param before_y [in] 変換前の19座標系のY(経度方向)
	* @param after_lon [out] 変換後経度
	* @param after_lat [out] 変換後緯度
	*/
	STDMETHOD(XY19toLL)(INT axis, DOUBLE before_x, DOUBLE before_y, DOUBLE* after_lon, DOUBLE* after_lat);

	/**
	* @brief 経緯度->19座標系への座標変換
	* @param axis [in] 該当する座標系の番号
	* @param before_lon [in] 変換前経度
	* @param before_lat [in] 変換前緯度
	* @param after_x [out] 変換後の19座標系のX(緯度方向)
	* @param after_y [out] 変換後の19座標系のY(経度方向)
	*/
	STDMETHOD(LLtoXY19)(INT axis, DOUBLE before_lon, DOUBLE before_lat, DOUBLE* after_x, DOUBLE* after_y);

private:
	crd_cnv m_cnv;           ///!< crd_cnv

};

OBJECT_ENTRY_AUTO(__uuidof(CrdConvert), CCrdConvert)
