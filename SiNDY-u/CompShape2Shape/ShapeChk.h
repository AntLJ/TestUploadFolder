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
#include "stdafx.h"


using namespace std;
typedef map<int,d_lonlat>	PointsInfo;	//オブジェクトの形状(点列)情報(補間点と座標)
typedef map<int,int>	PartIndexInfo; //オブジェクトの各パートのインデックス情報

struct ObjectInfo{
	PointsInfo e_PointsInfo;
	int e_PointCount;
	int e_PartCount;
	PartIndexInfo e_PartIndexInfo;
};										
typedef map<int,ObjectInfo> ShapeInfo;	//SHPのオブジェクト情報
//比較元・比較先距離情報格納用構造体
struct DistInfo{
	double e_OriDist;
	double e_DestDist;
};

class CShapeChk
{
public:
	CShapeChk(ofstream* p_ErrorLog,int aMeshCode,int a_iExeMode, string& a_sLayerName, string& a_sFullPathOri,string& a_sFullPathDest,double a_dDistBoder)
	{
		m_pErrorLog		= p_ErrorLog;
		m_MeshCode		= aMeshCode;
		m_iExeMode		= a_iExeMode;
		m_sLayerName	= a_sLayerName;
		m_sFullPassOri	= a_sFullPathOri;
		m_sFullPassDest = a_sFullPathDest;
		m_dDistBoder	= a_dDistBoder;
		
	}
	~CShapeChk(void);
	/**
	 * @brief	形状チェック実行
	 * @return	bool
	 */
	bool	app(void);

private:
	/**
	* @brief	Shapeファイルを読み込む
	* @param	string	[in] 対象Shapeファイルのフルパス
	* @return	bool
	*/
	bool	load_shape_info(ShapeInfo* p_ShapeInfo, const string& r_sFullPath, int* p_ShapeType);

	/**
	* @brief	SHPの形状情報を読み込む
	* @param	ObjectInfo* [in] オブジェクト情報
	* @param	CSHPHandle [in] 対象Shapeのハンドリングクラス
	* @param	int	[in] 対象ObjectのIndex
	* @return	bool
	*/
	bool	get_Obj_info(ObjectInfo* pObjectInfo,CSHPHandle& r_cSHPHdl,int a_index);

	/**
	* @brief	SHPの属性情報を読み込む
	* @param	int*	[out] オブジェクトIDまたはFID
	* @param	ObjectInfo* [out] オブジェクト情報
	* @param	CDBFHandle [in] 対象Shapeのハンドリングクラス
	* @param	int [in] 対象ObjectのIndex
	* @return	bool
	*/
	bool	get_Dbf_info(int* pID,ObjectInfo* pObjectInfo, CDBFHandle& r_cDBFHdl,int a_index);

	/**
	* @brief	SHPの形状比較を行う
	* @param	ShapeInfo&	[in] 比較元のSHP情報
	* @param	ShapeInfo&	[in] 比較先のSHP情報
	* @return	bool
	*/
	bool	comp_Shp_info(ShapeInfo& r_OriShapeInfo, ShapeInfo& r_DestShapeInfo);

	/**
	* @brief	オブジェクトの距離チェックを行う
	* @param	double*		[out] 比較元と比較先のオブジェクトの距離の差
	* @param	DistInfo*	[out] 比較元・比較先のオブジェクトの距離格納
	* @param	ObjectInfo&	[in] 比較元のオブジェクト情報
	* @param	ObjectInfo&	[in] 比較先のオブジェクト情報
	* @return	bool
	*/
	bool	Shp_Dist_chk(double* p_DistDiff, DistInfo* p_DistInfo, ObjectInfo& r_OriObj3ctInfo, ObjectInfo& r_DestObjectInfo);

	/**
	* @brief	オブジェクトの距離チェックを行う(マルチパート用)
	* @param	double*		[out] 比較元と比較先のオブジェクトの距離の差
	* @param	DistInfo*	[out] 比較元・比較先のオブジェクトの距離格納
	* @param	ObjectInfo&	[in] 比較元のオブジェクト情報
	* @param	ObjectInfo&	[in] 比較先のオブジェクト情報
	* @return	bool
	*/
	bool	Shp_Dist_chk_multi(double* p_DistDiff, DistInfo* p_DistInfo, ObjectInfo& r_OriObj3ctInfo, ObjectInfo& r_DestObjectInfo);

private:
	ofstream*	m_pErrorLog;	
	int			m_MeshCode;
	int			m_iExeMode;				// 0:OBJECTIDで紐付け、1:FIDで紐付け
	double		m_dDistBoder;
	string		m_sLayerName;
	string		m_sFullPassOri;
	string		m_sFullPassDest;
	
	
};
