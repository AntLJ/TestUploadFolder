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

// SquareHash.h: SquareHash クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQUAREHASH_H__12D63737_77DD_498A_B314_29654B13C9C3__INCLUDED_)
#define AFX_SQUAREHASH_H__12D63737_77DD_498A_B314_29654B13C9C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <map>

/**
 * @brief 平面ハッシュクラス基底定義
 */
interface SquareHashI
{
	/**
	 * @brief ハッシュ点クラス
	 */
	class HashPoint
	{
	public:
		HashPoint(int nX, int nY) :
		m_nX(nX),
		m_nY(nY)
		{
		}

		int x() const { return m_nX; }
		int y() const { return m_nY; }

		int m_nX;
		int m_nY;
	};

	typedef HashPoint(*HashFunc)(double, double);
};

/**
 * @brief 平面ハッシュクラステンプレート
 */
template <typename T>
class SquareHashT : public SquareHashI
{
public:
// 定義
	typedef T Member;
	typedef std::set<T> MemberSet;

	typedef std::map<int, MemberSet> YLine; //< Y軸
	typedef std::map<int, YLine> XYSquare; //< XY平面

	typedef typename XYSquare::const_iterator const_iterator;

// 構築/消滅
	SquareHashT(HashFunc pfHash) :
	m_pfHash(pfHash)
	{
	}

// 設定/取得
	/// 登録されている要素をクリアする
	void clear()
	{
		m_cMemberMap.clear();
	}

	/**
	 * @brief 指定したオブジェクトのコピーを、指定した１点に属する要素として登録する
	 *
	 * @param rMember [in] 要素としてコピー登録するオブジェクト
	 * @param dX [in] 要素のX座標
	 * @param dY [in] 要素のY座標
	 * @retval true 登録成功
	 * @retval false 登録失敗
	 */
	bool insert(T& rMember, double dX, double dY)
	{
		return insert(rMember, hash(dX, dY));
	}

	/**
	 * @brief 指定したオブジェクトのコピーを、指定したハッシュ平面座標に所属する要素として登録する
	 *
	 * @param rMember [in] 要素としてコピー登録するオブジェクト
	 * @param cPoint [in] ハッシュ平面座標
	 * @retval true 登録成功
	 * @retval false 登録失敗
	 */
	bool insert(T const& rMember, const HashPoint& cPoint)
	{
		return m_cMemberMap[cPoint.x()][cPoint.y()].insert(rMember).second;
	}

	/**
	 * @brief 指定したオブジェクトのコピーを、指定した矩形に属する要素として登録する
	 *
	 * @param rMember [in] 要素としてコピー登録するオブジェクト
	 * @param dMinX [in] 最小X座標
	 * @param dMinY [in] 最小Y座標
	 * @param dMaxX [in] 最大X座標
	 * @param dMaxY [in] 最大Y座標
	 */
	void insert(T const& rMember, double dMinX, double dMinY, double dMaxX, double dMaxY)
	{
		insert(rMember, hash(dMinX, dMinY), hash(dMaxX, dMaxY));
	}

	/**
	 * @brief 指定したオブジェクトのコピーを、指定したハッシュ平面矩形に所属する要素として登録する
	 *
	 * @param rMember [in] 要素としてコピー登録するオブジェクト
	 * @param cMinPoint [in] 最小ハッシュ平面座標
	 * @param cMaxPoint [in] 最大ハッシュ平面座標
	 */
	void insert(T const& rMember, const HashPoint& cMinPoint, const HashPoint& cMaxPoint)
	{
		for(int x = cMinPoint.x(); x <= cMaxPoint.x(); x++) {
			for(int y = cMinPoint.y(); y <= cMaxPoint.y(); y++) {
				m_cMemberMap[x][y].insert(rMember);
			}
		}
	}

	/// 先頭の要素を指す反復子を取得する
	const_iterator begin() const { return m_cMemberMap.begin(); }

	/// 最後尾の次の要素を指す反復子を取得する
	const_iterator end() const { return m_cMemberMap.end(); }

	/**
	 * @brief 指定した座標を含むハッシュ平面に登録された要素一覧を取得する
	 *
	 * @param dX [in] X座標
	 * @param dY [in] Y座標
	 * @return 要素一覧。そのハッシュ平面に一度も要素が登録されたことがない場合NULL。
	 */
	const MemberSet* searchMemberSet(double dX, double dY) const
	{
		return searchMemberSet(hash(dX, dY));
	}

	/**
	 * @brief 指定したハッシュ平面に登録された要素一覧を取得する
	 *
	 * @param cPoint [in] ハッシュ平面座標
	 * @return 要素一覧。そのハッシュ平面に一度も要素が登録されたことがない場合NULL。
	 */
	const MemberSet* searchMemberSet(const HashPoint& cPoint) const
	{
		// Y軸を求める
		XYSquare::const_iterator it1 = m_cMemberMap.find(cPoint.x());
		if(it1 == m_cMemberMap.end())
			return 0;

		// ハッシュ平面を求める
		YLine::const_iterator it2 = it1->second.find(cPoint.y());
		if(it2 == it1->second.end())
			return 0;

		// ハッシュ平面に登録された要素一覧を返す
		return &it2->second;
	}

	/**
	 * @brief 指定した矩形と重なるハッシュ平面に登録された要素一覧を取得する
	 *
	 * @param dMinX [in] 最小X座標
	 * @param dMinY [in] 最小Y座標
	 * @param dMaxX [in] 最大X座標
	 * @param dMaxY [in] 最大Y座標
	 * @param rMemberSet [out] 要素一覧取得先
	 */
	void getMemberSet(double dMinX, double dMinY, double dMaxX, double dMaxY, MemberSet& rMemberSet) const
	{
		getMemberSet(hash(dMinX, dMinY), hash(dMaxX, dMaxY), rMemberSet);
	}

	/**
	 * @brief 指定したハッシュ平面矩形に登録された要素一覧を取得する
	 *
	 * @param cMinPoint [in] 最小ハッシュ平面座標
	 * @param dMaxPoint [in] 最大ハッシュ平面座標
	 * @param rMemberSet [out] 要素一覧取得先
	 */
	void getMemberSet(const HashPoint& cMinPoint, const HashPoint& cMaxPoint, MemberSet& rMemberSet) const
	{
		for(int x = cMinPoint.x(); x <= cMaxPoint.x(); x++) {
			XYSquare::const_iterator it1 = m_cMemberMap.find(x);
			if(it1 == m_cMemberMap.end())
				continue;

			for(int y = cMinPoint.y(); y <= cMaxPoint.y(); y++) {
				YLine::const_iterator it2 = it1->second.find(y);
				if(it2 == it1->second.end())
					continue;

				for(MemberSet::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
					rMemberSet.insert(*it3);
				}
			}
		}
	}

	/**
	 * @brief 全要素に対して関数f()を適用する
	 *
	 * @param f [in] 関数クラス
	 */
	template <typename Function>
	void for_each(Function& f)
	{
		for(const_iterator it1 = begin(); it1 != end(); ++it1) {
			const YLine& rYLine = it1->second;
			for(YLine::const_iterator it2 = rYLine.begin(); it2 != rYLine.end(); ++it2) {
				const MemberSet& rMemberSet = it2->second;
				for(MemberSet::const_iterator it3 = rMemberSet.begin(); it3 != rMemberSet.end(); ++it3) {
					f(*it3);
				}
			}
		}
	}

	/**
	 * @brief 指定した座標が属するハッシュ平面座標を取得する
	 *
	 * @note デフォルトの処理は、倍精密度座標の小数点以下を切り捨てるだけ
	 * @return ハッシュ平面座標
	 */
	virtual HashPoint hash(double dX, double dY) const
	{
		return m_pfHash ? m_pfHash(dX, dY) : HashPoint(static_cast<int>(dX), static_cast<int>(dY));
	}

private:
	XYSquare m_cMemberMap; ///< ハッシュ平面座標別要素一覧
	HashFunc m_pfHash; ///< ハッシュ関数
};

#endif // !defined(AFX_SQUAREHASH_H__12D63737_77DD_498A_B314_29654B13C9C3__INCLUDED_)
