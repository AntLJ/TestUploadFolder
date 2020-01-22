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

#ifndef	_POINT_ADMIN_H
#define	_POINT_ADMIN_H

// 2002.03.20 by kudo
// 2003.04.25 距離管理用のバッファを指定サイズ以降は切り捨てる、limitBuf()を追加
#include <vector>
#include <set>

/**
	点と点の距離と位置を管理するクラス
*/
class Pt_Dist_Admin_Array
{
	private:
		struct	Pt_Dist_Admin
		{
			public:
				LONG	eIndx_1;	///< 点列１のインデックス
				LONG	eIndx_2;	///< 点列２のインデックス
				double	eDist;		///< 2点間の距離
			public:
						Pt_Dist_Admin	()
						{
							eIndx_1	= 0;
							eIndx_2 = 0;
							eDist	= 0.0;
						}
						Pt_Dist_Admin	(	LONG	cIndx_1,	///< インデックス１
											LONG	cIndx_2,	///< インデックス２
											double	cDist)		///< 距離
						{
							eIndx_1	= cIndx_1;
							eIndx_2	= cIndx_2;
							eDist	= cDist;
						}
						~Pt_Dist_Admin	() {}
				void	operator=(const Pt_Dist_Admin cData)	///< 代入演算子定義
				{
					eIndx_1	= cData.eIndx_1;
					eIndx_2	= cData.eIndx_2;
					eDist	= cData.eDist;
				}
			};

		struct lessC : public std::binary_function<Pt_Dist_Admin, Pt_Dist_Admin, bool>
		{
			// 距離だけの比較でよい
			bool operator () (const Pt_Dist_Admin cData1, const Pt_Dist_Admin cData2) const
			{
				bool	ret	= false;
				if(cData1.eDist != cData2.eDist) {
					if(cData1.eDist < cData2.eDist) {
						ret	= true;
					}
				}
				return (ret);
			}
		};
		std::multiset<Pt_Dist_Admin, lessC>				m_PtDistTbl;
		std::multiset<Pt_Dist_Admin, lessC>::iterator	m_CurIndx;
	public:
		Pt_Dist_Admin_Array	()	///< コンストラクタ
		{
			m_PtDistTbl.erase(m_PtDistTbl.begin(),m_PtDistTbl.end());
			m_CurIndx	= m_PtDistTbl.begin();
		}
		~Pt_Dist_Admin_Array() {}	///< デストラクタ

		/**
			データを追加する
		*/
		void	pushData	(	LONG	cIndx1,	///< インデックス１[IN]
								LONG	cIndx2,	///< インデックス２[IN]
								double	cDist)	///< 距離[IN]
		{
			Pt_Dist_Admin		aTmpDat(cIndx1, cIndx2, cDist);
			m_PtDistTbl.insert	(aTmpDat);
		}

		/**
			距離管理バッファを最初の位置から指定サイズまで
			に切り詰める
			@return 無し
		*/
		void	limitBuf	(	LONG	cSize	)	///< リミットサイズ
		{
			std::multiset<Pt_Dist_Admin, lessC>				a_TmpPtDistTbl;
			std::multiset<Pt_Dist_Admin, lessC>::iterator	a_Indx;
			// 指定サイズが現在のバッファサイズより大きいばあいは何もしない
			int		a_iCur_Size		= m_PtDistTbl.size();
			if( a_iCur_Size > cSize ) {
				int	i = 0;
				for( a_Indx = m_PtDistTbl.begin(), i = 0; i < cSize; a_Indx++, i++ )
				{
					a_TmpPtDistTbl.insert	( *a_Indx );
				}
				// 一度消す
				m_PtDistTbl.clear();
				m_PtDistTbl	= a_TmpPtDistTbl;	// 全部コピー
			}
		}

		/**
			最初のデータを取り出す
			@retval	true	取り出せた
			@retval false	取り出せない
		*/
		int		getBegin	(	LONG	*cIndx1,	///< インデックス１[OUT]
								LONG	*cIndx2,	///< インデックス２[OUT]
								double	*cDist)		///< 距離[OUT]
		{
			m_CurIndx		= m_PtDistTbl.begin();
			if(m_CurIndx == m_PtDistTbl.end()) {	// 空だった場合
				*cIndx1		= 0;
				*cIndx2		= 0;
				*cDist		= 0.0;
				return	(false);	
			}
			Pt_Dist_Admin	aTmpDat;
			aTmpDat			= *m_CurIndx;
			*cIndx1			= aTmpDat.eIndx_1;
			*cIndx2			= aTmpDat.eIndx_2;
			*cDist			= aTmpDat.eDist;
			return	(true);
		}

		/**
			次のデータを取り出す
			@retval	true	取り出せた
			@retval false	取り出せない
		*/
		int		getNext		(	LONG	*cIndx1,	///< インデックス１[OUT]
								LONG	*cIndx2,	///< インデックス２[OUT]
								double	*cDist)		///< 距離[OUT]
		{
			m_CurIndx++;	// イテレータを１進める
			if(m_CurIndx == m_PtDistTbl.end()) {	// 空だった場合
				*cIndx1		= 0;
				*cIndx2		= 0;
				*cDist		= 0.0;
				return	(false);	
			}
			Pt_Dist_Admin	aTmpDat;
			aTmpDat			= *m_CurIndx;
			*cIndx1			= aTmpDat.eIndx_1;
			*cIndx2			= aTmpDat.eIndx_2;
			*cDist			= aTmpDat.eDist;
			return	(true);
		}
};

#endif	//	_POINT_ADMIN_H
