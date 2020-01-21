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

#ifndef CRD_CNV_MESH_CODE_RANGE_H_
#define	CRD_CNV_MESH_CODE_RANGE_H_

#pragma warning( push )
#pragma warning( disable : 4284 )
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#pragma warning( pop )
#include "crd_cnv.h"

//////////////////////////////////////////////////////////////////////
// mesh_code_range
//////////////////////////////////////////////////////////////////////

// 定義
//////////////////////////////////////////////////////////////////////

/**
 * @brief 特定の範囲に存在するメッシュを列挙するクラス。
 *
 * @code
 * // 出力 : 533934 533935 533936 533944 533945 533946
 * std::copy(
 *	mesh_code_range(crd_cnv(), 533945, -1, -1, 1, 0).begin(),
 *	mesh_code_range::static_end(),
 *	std::ostream_iterator<int>(std::cout, "\n"));
 * @endcode
 */
class mesh_code_range
{
public:
// 定義
	/**
	 * @brief 反復子。
	 */
	class iterator : public boost::iterator_facade<iterator, const int, boost::bidirectional_traversal_tag, const int>
	{
	public:
	// 構築/消滅
		iterator(const iterator& rIterator) :
		m_pGenerator(rIterator.m_pGenerator),
		m_nX(rIterator.m_nX),
		m_nY(rIterator.m_nY)
		{
		}

	private:
	// 定義
		friend class mesh_code_range;
		friend class boost::iterator_core_access;

	// 構築/消滅
		iterator(const mesh_code_range* pGenerator, int nX, int nY) :
		m_pGenerator(pGenerator),
		m_nX(nX),
		m_nY(nY)
		{
		}

		const int dereference() const
		{
			return m_pGenerator->getMeshCode(m_nX, m_nY);
		}

		void increment()
		{
			m_nX++;
			if(m_nX == m_pGenerator->m_nXCount) {
				m_nX = 0;
				m_nY++;
			}
		}

		void decrement()
		{
			m_nX--;
			if(m_nX == -1) {
				m_nX = m_pGenerator->m_nXCount - 1;
				m_nY--;
			}
		}

		bool equal(iterator const& other) const
		{
			if(! m_pGenerator) {
				return other.m_pGenerator ? other.equal(*this) : true;
			}

			if(! other.m_pGenerator) {
				return 
					(other.m_nX == 0 && other.m_nY == 0) ?
					(m_nX == other.m_nX && m_nY == other.m_nY) :
					(m_nY == m_pGenerator->m_nYCount);
			}

			return m_pGenerator == other.m_pGenerator && m_nX == other.m_nX && m_nY == other.m_nY;
		}

	// 変数
		const mesh_code_range* m_pGenerator;
		int m_nX;
		int m_nY;
	};

	typedef boost::reverse_iterator<iterator> reverse_iterator;

	typedef iterator const_iterator;
	typedef reverse_iterator const_reverse_iterator;

// 構築/消滅
	mesh_code_range(const crd_cnv& rCrdCnv, int nOrgMeshCode, int nXCount, int nYCount);
	mesh_code_range(const crd_cnv& rCrdCnv, int nOrgMeshCode, int nYMin, int nYMax, int nXMin, int nXMax);

// 反復子の生成
	iterator begin() const { return iterator(this, 0, 0); }
	iterator end() const { return iterator(this, 0, m_nYCount); }

	reverse_iterator rbegin() const { return reverse_iterator(end()); }
	reverse_iterator rend() const { return reverse_iterator(begin()); }

	static iterator static_begin() { return iterator(0, 0, 0); }
	static iterator static_end() { return iterator(0, -1, -1); }

	static reverse_iterator static_rbegin() { return reverse_iterator(static_end()); }
	static reverse_iterator static_rend() { return reverse_iterator(static_begin()); }

private:
// 定義
	friend class iterator;

// 設定
	void init(int nOrgMeshCode, int nXCount, int nYCount);

// 取得
	int getMeshCode(int nX, int nY) const;

// 変数
	const crd_cnv& m_rCrdCnv; ///< 座標変換オブジェクト。
	int m_nOrgMeshCode; ///< 南西端メッシュコード。
	int m_nXCount; ///< 東方向に並ぶメッシュ数。
	int m_nYCount; ///< 北方向に並ぶメッシュ数。
};

// 実装
//////////////////////////////////////////////////////////////////////

/**
 * @brief 南西端のメッシュコードを指定して構築する。
 *
 * @param rCrdCnv [in,ref] 座標変換オブジェクト。
 * @param nOrgMeshCode [in] 南西端メッシュコード。
 * @param nXCount [in] 東方向に並ぶメッシュ数。
 * @param nYCount [in] 北方向に並ぶメッシュ数。
 */
inline mesh_code_range::mesh_code_range(const crd_cnv& rCrdCnv, int nOrgMeshCode, int nXCount, int nYCount) :
m_rCrdCnv(rCrdCnv)
{
	if(nXCount == 0 || nYCount == 0) {
		nXCount = nYCount = 0;
	}
	init(nOrgMeshCode, nXCount, nYCount);
}

/**
 * @brief 基準メッシュコードと範囲を指定して構築する。
 *
 * @param rCrdCnv [in,ref] 座標変換オブジェクト。
 * @param nOrgMeshCode [in] 南西端メッシュコード。
 * @param nXMin [in] 東西方向最小相対メッシュコード。
 * @param nYMin [in] 南北方向最小相対メッシュコード。
 * @param nXMax [in] 東西方向最大相対メッシュコード。
 * @param nYMax [in] 南北方向最大相対メッシュコード。
 */
inline mesh_code_range::mesh_code_range(const crd_cnv& rCrdCnv, int nOrgMeshCode, int nXMin, int nYMin, int nXMax, int nYMax) :
m_rCrdCnv(rCrdCnv)
{
	if(nXMin || nYMin) {
		nOrgMeshCode = const_cast<crd_cnv&>(m_rCrdCnv).GetMeshCode(nOrgMeshCode, nXMin, nYMin);
	}

	init(nOrgMeshCode, nXMax - nXMin + 1, nYMax - nYMin + 1);
}

/**
 * @brief メンバ変数に値を設定する。
 *
 * @param nOrgMeshCode [in] 南西端メッシュコード。
 * @param nXCount [in] 東方向に並ぶメッシュ数。
 * @param nYCount [in] 北方向に並ぶメッシュ数。
 */
inline void mesh_code_range::init(int nOrgMeshCode, int nXCount, int nYCount)
{
	m_nOrgMeshCode = nOrgMeshCode;
	m_nXCount = nXCount;
	m_nYCount = nYCount;
}

/**
 * @brief 相対メッシュコードを絶対メッシュコードに変換する
 *
 * @param nX [in] 東西方向相対メッシュコード。
 * @param nY [in] 南北方向相対メッシュコード。
 */
inline int mesh_code_range::getMeshCode(int nX, int nY) const
{
	return const_cast<crd_cnv&>(m_rCrdCnv).GetMeshCode(m_nOrgMeshCode, nX, nY);
}

#endif // CRD_CNV_MESH_CODE_RANGE_H_
