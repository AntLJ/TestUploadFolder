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


class FeatureReader
{
public:
	/**
	 * @brief   コンストラクタ
	 */	
	FeatureReader(void){}
	explicit FeatureReader(const IFeatureClassPtr& feature_class) : m_FeatureClass(feature_class){}
	FeatureReader(const IFeatureClassPtr& feature_class, const IFeatureClassPtr& mesh_class): 
		m_FeatureClass(feature_class),
		m_BaseMeshClass(mesh_class)
	{}

	/**
	 * @brief   デストラクタ
	 */	
	virtual ~FeatureReader(void){};

	/**
	 * @brief   データ読み込み
	 * @detail  引数で指定したwhere句に該当するフィーチャをキャッシュする
	 * @param[in] where_clause  検索条件
	 * @retval  true  成功
	 * @retval  false 失敗
	 */	
	bool read(const CString& where_clause);

	/**
	 * @brief   データ読み込み
	 * @param[in] basemesh_code  検索対象範囲となるベースメッシュコード
	 * @retval  true  成功
	 * @retval  false 失敗
	 */	
	bool read(long basemesh_code);

	/**
	 * @brief   データ読み込み
	 * @param[in] geo           空間検索の範囲となるジオメトリ
	 * @param[in] where_clause  検索条件
	 * @retval  true  成功
	 * @retval  false 失敗
	 */	
	bool read(const IGeometryPtr& geo, const CString& where_clause = _T(""));

	/**
	 * @brief   データ読み込み
	 * @param[in] geo           空間検索の元となるジオメトリ
	 * @param[in] meter         geo の周囲何メートルに対して検索を行うか
	 * @param[in] where_clause  検索条件
	 * @retval  true  成功
	 * @retval  false 失敗
	 */	
	bool readAround(const IGeometryPtr& geo, double meter, const CString& where_clause = _T(""));

	/**
	 * @brief   データ読み込み
	 * @param[in] basemesh_code  検索対象範囲となるベースメッシュコード
	 * @param[in] where_clause   検索条件
	 * @retval  true  成功
	 * @retval  false 失敗
	 */	
	bool read(long basemesh_code, const CString& where_clause);

	/**
	 * @brief   フィーチャ群を返す
	 * @return  保持しているフィーチャ群
	 */	
	const std::vector<IFeaturePtr>& getFeatures() const { return m_Features; }

	/**
	 * @brief   保持しているフィーチャ数を返す
	 * @return  保持しているフィーチャ数
	 */	
	unsigned long count() const{ return m_Features.size(); }

private:
	/**
	 * @brief   ジオメトリから指定距離だけバッファリンクした形状を作成する
	 * @param[in] geo    バッファリング元のジオメトリ
	 * @param[in] meter  バッファリング距離(メートル)
	 * @return  バッファリンクした形状
	 */	
	IGeometryPtr getBuffer( const IGeometryPtr& geo, double meter ) const;

	/**
	 * @brief   指定したメッシュコードのジオメトリを取得する
	 * @param[in] basemesh_code  ベースメッシュコード
	 * @retval null以外  メッシュ形状
	 * @retval nul以外   メッシュ形状の取得失敗
	 */	
	IGeometryPtr getBasemeshShape( long basemesh_code ) const;

private:
	IFeatureClassPtr m_FeatureClass;		//!< 読み込み対象のフィーチャクラス
	IFeatureClassPtr m_BaseMeshClass;		//!< 検索で使用するメッシュフィーチャクラス

	std::vector<IFeaturePtr> m_Features;	//!< 読み込んだフィーチャ群
};

