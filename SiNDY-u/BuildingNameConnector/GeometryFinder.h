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

// GeometryFinder.h: CGeometryFinder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRYFINDER_H__9454D0CB_6EBE_4925_A360_E8D2D44FBEE1__INCLUDED_)
#define AFX_GEOMETRYFINDER_H__9454D0CB_6EBE_4925_A360_E8D2D44FBEE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGeometryFinder  
{
protected:
	class FeatureInfo
	{
	public:
		IFeaturePtr m_ipFeature;
		WKSEnvelope m_wksEnvelope;
	public:
		FeatureInfo() : m_ipFeature(0), m_wksEnvelope()
		{
			m_wksEnvelope.XMin = m_wksEnvelope.YMin = m_wksEnvelope.XMax = m_wksEnvelope.YMax = 0.0;
			m_ipFeature = 0;
		}
		FeatureInfo(IFeaturePtr ipFeature) : m_ipFeature(0), m_wksEnvelope()
		{
			m_ipFeature = ipFeature;
			IGeometryPtr ipGeometry = 0;
			ipFeature->get_ShapeCopy(&ipGeometry);
			IEnvelopePtr ipEnvelope = 0;
			ipGeometry->get_Envelope(&ipEnvelope);
			ipEnvelope->QueryWKSCoords(&m_wksEnvelope);
		}
		virtual ~FeatureInfo()
		{

		}
	};
	struct LessWKSEnvelope : public std::binary_function<WKSEnvelope, WKSEnvelope, bool>
	{
		bool operator()(const WKSEnvelope& e1, const WKSEnvelope& e2) const
		{
			return (e1.XMin != e2.XMin) ? (e1.XMin < e2.XMin) :
				   (e1.XMax != e2.XMax) ? (e1.XMax < e2.XMax) :
				   (e1.YMin != e2.YMin) ? (e1.YMin < e2.YMin) : (e1.YMax < e2.YMax);
		}
	};
	struct UnitPos
	{
		long x, y;
		UnitPos() : x(0), y(0) {}
		UnitPos(long a, long b) : x(a), y(b) {}
	};
	struct LessUnitPos : public std::binary_function<UnitPos, UnitPos, bool>
	{
		bool operator()(const UnitPos& p1, const UnitPos& p2) const
		{
			return (p1.x != p2.x) ? (p1.x < p2.x) : (p1.y < p2.y);
		}
	};
	typedef std::map<UnitPos, WKSEnvelope, LessUnitPos> UNITPOS;
	typedef std::map<WKSEnvelope, std::vector<FeatureInfo>, LessWKSEnvelope> UNITDATA;
	WKSEnvelope m_env; //!< 範囲矩形
	long m_xcount, m_ycount; //!< グリッド数
	double m_xwidth, m_ywidth; //!< 1グリッドのx, y方向の幅
	UNITPOS m_unitpos;
	UNITDATA m_unitdata;
	IFeatureClassPtr m_ipFeatureClass;
	void Initialize(const WKSEnvelope& env, long xcnt, long ycnt);
public:
	CGeometryFinder();
	/**
	 * @brief <b>コンストラクタ</b>\n
	 * @param env [in] 範囲矩形
	 * @param xcnt [in] X方向グリッド数
	 * @param ycnt [in] Y方向グリッド数
	 */
	CGeometryFinder(const WKSEnvelope& env, long xcnt = 1, long ycnt = 1);
	/**
	 * @brief <b>コンストラクタ</b>\n
	 * @param ipEnv [in] 範囲矩形
	 * @param xcnt [in] X方向グリッド数
	 * @param ycnt [in] Y方向グリッド数
	 */
	CGeometryFinder(IEnvelopePtr ipEnv, long xcnt = 1, long ycnt = 1);
	/**
	 * @biref <b>デストラクタ</b>\n
	 */
	virtual ~CGeometryFinder();
	void SetEnvelope(const WKSEnvelope& env, long xcnt = 1, long ycnt = 1)
	{
		m_env.XMin = m_env.YMin = m_env.XMax = m_env.YMax = 0.0;
		m_xcount = m_ycount = 0;
		m_xwidth = m_ywidth = 0.0;
		Initialize(env, xcnt, ycnt);
	}
	/**
	 * @brief <b>データをクリアします</b>\n
	 * @note 
	 */
	void Clear();
	/**
	 * @brief <b>データをセットします</b>\n
	 * @note
	 * @param ipFeatureClass [in] フィーチャークラス
	 * @param ipQueryFilter [in] フィルタ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SetData(IFeatureClassPtr ipFeatureClass, IQueryFilterPtr ipQueryFilter);
	/**
	 * @brief <b>候補の検索</b>\n
	 * @note 
	 * @param ipGeometry [in] 検索図形
	 * @param foundFeatures [out] 候補
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool FindCandidate(IGeometryPtr ipGeometry, std::vector< CAdapt<IFeaturePtr> >& foundFeatures);
	/**
	 * @brief <b>最近傍のフィーチャーを検出する</b>\n
	 * @note 
	 * @param longitude [in] 経度
	 * @param latitude [in] 緯度
	 * @param xrange [in] x方向の検索範囲
	 * @param yrange [in] y方向検索範囲
	 * @param ipFeature [out] 検出されたフィーチャー
	 * @param crslon [out] 最近傍点経度
	 * @param crslat [out] 最近傍点緯度
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool FindNearestFeature(double longitude, double latitude, double xrange, double yrange, IFeature** ipFeature, double* crslon, double* crslat);
protected:
	bool GetUnit(IGeometryPtr ipGeometry, std::vector<WKSEnvelope>& unit);
	bool GetUnit(const WKSEnvelope& env, std::vector<WKSEnvelope>& unit);
	bool IsOverlapped(const WKSEnvelope& e1, const WKSEnvelope& e2) const
	{
		return (! ((e1.XMin > e2.XMax) || (e1.XMax < e2.XMin) || (e1.YMin > e2.YMax) || (e1.YMax < e2.YMin)));
	}
};

#endif // !defined(AFX_GEOMETRYFINDER_H__9454D0CB_6EBE_4925_A360_E8D2D44FBEE1__INCLUDED_)
