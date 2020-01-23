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

class ZipCodeDataContainer
{
public:
	ZipCodeDataContainer(void);
	~ZipCodeDataContainer(void);

	// ポリゴンID、郵便番号、住所コード、面積の昇順
	bool operator < (const ZipCodeDataContainer& dst)
	{
		return
			(m_polygonId != dst.m_polygonId) ? (m_polygonId < dst.m_polygonId) :
			(0 != dst.m_zipcode.Compare(m_zipcode)) ? (0 < dst.m_zipcode.Compare(m_zipcode)) :
			(0 != dst.m_prefCode.Compare(m_prefCode)) ? (0 < dst.m_prefCode.Compare(m_prefCode)) :
			(0 != dst.m_cityCode.Compare(m_cityCode)) ? (0 < dst.m_cityCode.Compare(m_cityCode)) :
			(0 != dst.m_oazaCode.Compare(m_oazaCode)) ? (0 < dst.m_oazaCode.Compare(m_oazaCode)) :
			(0 != dst.m_azaCode.Compare(m_azaCode)) ? (0 < dst.m_azaCode.Compare(m_azaCode)) :
			(CalcArea(m_geometry) < CalcArea(dst.m_geometry));
	}

	bool operator == (const ZipCodeDataContainer& dst)
	{
		CString addrCode =
			m_prefCode + m_cityCode + m_oazaCode + m_azaCode;
		CString dstAddrCode =
			dst.m_prefCode + dst.m_cityCode + dst.m_oazaCode + dst.m_azaCode;

		return (m_polygonId == dst.m_polygonId) &&
			(0 == m_zipcode.Compare(dst.m_zipcode)) &&
			(0 == addrCode.Compare(dstAddrCode));
	}

	/**
	 * @brief 郵便番号の比較
	 * @param target [in] 比較対象
	 * @retval true 郵便番号が等しい
	 * @retval false 郵便番号が等しくない
	 */
	bool CompareZipCode(const ZipCodeDataContainer& target);

	/**
	 * @brief 11桁住所コードの比較
	 * @param target [in] 比較対象
	 * @retval true 住所コードが等しい
	 * @retval false 住所コードが等しくない
	 */
	bool CompareAddrCode(const ZipCodeDataContainer& target);

	/**
	 * @brief データの格納
	 * @param adminCityCode [in] 都市地図行政界のCITYCODE
	 * @param adminAddrCode [in] 都市地図行政界のADDRCODE、またはADDRCODE2
	 * @param adminZipCode [in] CTYCODE、ADDRCODEに紐づく郵便番号
	 * @param adminGeometry [in] 都市地図行政界のジオメトリ
	 */
	void SetData(
		const CString& adminCityCode,
		const CString& adminAddrCode,
		const CString& adminZipCode,
		const IGeometryPtr& adminGeometry);

	/**
	 * @brief m_geometryのマルチポリゴン判定
	 * @retval true マルチポリゴンである
	 * @retval false マルチポリゴンでない
	 */
	bool IsMultiPolygon();

	/**
	 * @brief ポリゴンIDをセットする
	 * @param id [in] ID
	 */
	void SetPolygonId(long id);
	
	/**
	 * @brief ポリゴンインデックスをセットする
	 * @param index [in] インデックス
	 */
	void SetGeometryIndex(long index){ m_geometryIndex = index; };

	/**
	 * @brief ジオメトリをセットする
	 * @param geometry [in] ジオメトリ
	 */
	void SetGeometry(const IGeometryPtr& geometry);

	/**
	 * @brief ポリゴンIDを取得する
	 * @retval ポリゴンID(文字列)
	 */
	long GetPolygonId() const { return m_polygonId; }

	/**
	 * @brief 郵便番号を取得する
	 * @retval 郵便番号
	 */
	CString GetZipCode() const { return m_zipcode; }

	/**
	 * @brief 都道府県コードを取得する
	 * @retval 都道府県コード
	 */
	CString GetPrefCode() const { return m_prefCode; }

	/**
	 * @brief 市区町村コードを取得する
	 * @retval 市区町村コード
	 */
	CString GetCityCode() const { return m_cityCode; }

	/**
	 * @brief 大字コードを取得する
	 * @retval 大字コード
	 */
	CString GetOazaCode() const { return m_oazaCode; }

	/**
	 * @brief 字コードを取得する
	 * @retval 字コード
	 */
	CString GetAzaCode() const { return m_azaCode; }

	/**
	 * @brief ジオメトリを取得する
	 * @retval ジオメトリ
	 */
	IGeometryPtr GetGeometry() const { return m_geometry; }

	/**
	 * @brief ポリゴンインデックスを取得する
	 * @retval ポリゴンインデックス
	 */
	long GetGeometryIndex() const { return m_geometryIndex; }

private:
	/**
	 * @brief 面積を求める
	 * @param geometry [in] ジオメトリ
	 * @retval ジオメトリの面積
	 */
	double CalcArea(const IGeometryPtr& geometry);

private:
	long m_polygonId; //!< ポリゴンID
	CString m_zipcode; //!< 郵便番号
	CString m_prefCode; //!< 都道府県コード
	CString m_cityCode; //!< 市区町村コード
	CString m_oazaCode; //!< 大字コード
	CString m_azaCode; //!< 字コード
	IGeometryPtr m_geometry; //!< ポリゴン形状
	long m_geometryIndex; //!< マルチポリゴン解消した際に振られるポリゴンインデックス
};

