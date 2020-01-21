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

// crd_cnv, z19 を統合開始	1999.12.27 by t.kudo

#ifndef __CRD_CNV_HEADDER__
#define	__CRD_CNV_HEADDER__

#define _CRT_SECURE_NO_DEPRECATE 1 // ANSI-C標準の危険な関数の警告を出さない
#define _SCL_SECURE_NO_DEPRECATE 1 // C4996の警告を出さない
#pragma warning(disable: 4996)  // _SCL_SECURE_NO_DEPRECATEや_CRT_SECURE_NO_DEPRECATEでC4996が回避されない！！

// インクルードファイル
#include <stdio.h>
#include <stdlib.h>

// 2009.01.13
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

#if _MSC_VER >= 1400
#define _USE_MATH_DEFINES
#endif // _MSC_VER >= 1400

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

// DLL作成のため追加 2009.06.25
#ifdef	CRD_CNV_EXPORTS
#define	CRD_CNV_API	__declspec(dllexport)
#else
#ifdef	_USE_CRD_CNV_DLL	// DLLを使用する場合
#define	CRD_CNV_API	__declspec(dllimport)
#else
#define	CRD_CNV_API			// その他(スタティックライブラリ作成・使用時）
#endif	// _USE_CRD_CNV_DLL
#endif	// CRD_CNV_EXPORTS

// 構造体定義
struct	dms			/// 経緯度を表す(度分秒)
{
	int		deg;	///< 度
	int		min;	///< 分 
	double	sec;	///< 秒
};

struct	c19_origin	/// １９座標系原点用構造体
{
	dms		lon;	///< 経度
	dms 	lat;	///< 緯度	
};

struct	d_lonlat	/// 経緯度を表す(10進小数)
{
	double	lon;	///< 経度
	double	lat;	///< 緯度
};

struct	xy_coord	/// 直行座標系のXYを表す(19座標系、ガウス・クリューゲル座標系)
{
	double	x;		///< X座標（縦方向）
	double	y;		///< Y座標（横方向）
};

struct	coord19		/// 19座標系の系とXYを表す
{
	int		sys;	///< 系
	double	x;		///< X座標（縦方向）
	double	y;		///< Y座標（横方向）
};

// 1998.7.22 作成
struct	sys_num		/// １９座標系の系番号
{
	int		total;	///< 該当数
	int		sys[2];	///< 系番号
};

/**
 * @brief 地理座標変換クラス
*/
class CRD_CNV_API	crd_cnv
{
	private:
		/**
		* @class param_7
		* @brief 測地系変換用パラメータ（7パラメータ）
		* 
		* 2007.01.21	[Bug 5755] の要望により追加
		*/
		struct	param_7
		{
			double	T1;	///< X軸方向の平行移動量（cm）
			double	T2;	///< Y軸方向の平行移動量（cm）
			double	T3;	///< Z軸方向の平行移動量（cm）
			double	D;	///< スケール補正量(1D-8)
			double	R1;	///< X軸回りの回転（1/1000秒）
			double	R2;	///< Y軸回りの回転（1/1000秒）
			double	R3;	///< Z軸回りの回転（1/1000秒）

			param_7() {
				T1	= 0.0;
				T2	= 0.0;
				T3	= 0.0;
				D	= 0.0;
				R1	= 0.0;
				R2	= 0.0;
				R3	= 0.0;
			}
		};
	private:
		d_lonlat			Rad_Origin[19];					///< １９座標原点

		std::map<std::string, int>	m_mCode_Sys19;			///< 読み込んだパラメータを格納する

		/**
		* @brief １９座標系の表を作るメンバ関数
		* 
		* 2000.1.12 checked
		*/
		void				Make19tbl	(	void	);
		
		/**
		* @brief 赤道から緯度Phiまでの子午線孤長を得る
		* 
		* 2000.1.12 checked
		* @return 子午線孤長(m)
		*/
		double				M_Arc_Len	(
									double	Ra,				///< 地球楕円体の長半径（メートル）
									double	ea2,			///< [in] 第一離心率の2乗
									double	Phi				///< [in] 緯度
								);

		d_lonlat			Rad_Origin_Gauss[3];		///< ガウス・クリューゲル座標系用 2000.9.19
		/**
		* @brief ガウス・クリューゲル座標系の表を作るメンバ関数
		*/
		void				Make_Gauss_tbl	(	void	);
		// 2001.08.29 テスト用
		//double			M_Arc_Len_WGS84	(double);	// 2001.08.29 checked

		// 国際都市地図関連のメッシュ原点位置 2006.09.11
		d_lonlat			ORIGIN_JPN;
		d_lonlat			ORIGIN_EU;
		d_lonlat			ORIGIN_NA;
		d_lonlat			ORIGIN_AH;
		d_lonlat			ORIGIN_SEAA;
		d_lonlat			ORIGIN_SA;	// 2008.12.01
		d_lonlat			ORIGIN_SJ;	// 2011.12.08

		//--------------------------------------------------------------------
		/**
		* 2006.09.06 機能統一用
		* @brief メッシュ + (X,Y)から経緯度へ変換(double版)
		* @retval	0	成功
		* @retval	-1	失敗
		*/
		//--------------------------------------------------------------------
		int			MeshtoLL2_Core(
							unsigned int	meshcode,		///< [in]  メッシュコード
							double			x,				///< [in]  正規化座標X
							double			y,				///< [in]  正規化座標Y
							double			*lon,			///< [out]  経度
							double			*lat,			///< [out]  緯度
							double			offset_lon,		///< [in]  メッシュ原点のオフセット値（経度）
							double			offset_lat		///< [in]  メッシュ原点のオフセット値（緯度）
						);

		//--------------------------------------------------------------------
		/**
		* 2006.09.07 機能統一用
		* @brief メッシュ(文字列) + (X,Y)から経緯度へ変換(double版)
		* @retval	0	成功
		* @retval -1	失敗
		*/
		//--------------------------------------------------------------------
		int			MeshtoLL2_Core(
							char*	meshcode,		///< [in]  メッシュコード
							double	x,				///< [in]  正規化座標X
							double	y,				///< [in]  正規化座標Y
							double	*lon,			///< [out]  経度
							double	*lat,			///< [out]  緯度
							double	offset_lon,		///< [in]  メッシュ原点のオフセット値（経度）
							double	offset_lat		///< [in]  メッシュ原点のオフセット値（緯度）
						);

		//--------------------------------------------------------------------
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(世界共通版(日本も))
		*
		* 2006.09.06
		* @retval	0	成功
		* @retval -1	失敗
		*/
		//--------------------------------------------------------------------
		int			LLtoMesh_Core(
							double			lon,			///< [in]  経度
							double			lat,			///< [in]  緯度
							int				level,			///< [in]  メッシュのレベル (1,2,64)
							unsigned int	*meshcode,		///< [out]  メッシュコード
							int				*x,				///< [out]  正規化X
							int				*y,				///< [out]  正規化Y
							double			offset_lon,		///< [in]  メッシュ原点のオフセット値（経度）
							double			offset_lat,		///< [in]  メッシュ原点のオフセット値（緯度）
							int				no_mill=0		///< [in]  XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す） 
						);

		//--------------------------------------------------------------------
		/**
		* @brief 経緯度から、メッシュコード(文字列) + (x,y) を求める(世界共通版(日本も))
		*
		* 2006.09.08
		* @retval	0	成功
		* @retval -1	失敗
		*/
		//--------------------------------------------------------------------
		int			LLtoMesh_Core(
							double	lon,			///< [in]  経度
							double	lat,			///< [in]  緯度
							int		level,			///< [in]  メッシュのレベル (1,2,64)
							char	*meshcode,		///< [out]  メッシュコード
							int		*x,				///< [out]  正規化X
							int		*y,				///< [out]  正規化Y
							double	offset_lon,		///< [in]  メッシュ原点のオフセット値（経度）
							double	offset_lat,		///< [in]  メッシュ原点のオフセット値（緯度）
							int		no_mill=0		///< [in]  XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す） 
						);


		//--------------------------------------------------------------------
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		*
		* 指定したメッシュコードにたいして、正規化座標値を求める 2006.09.07
		* @retval	0	成功
		* @retval -1	失敗
		*/
		//--------------------------------------------------------------------
		int			LLtoMesh2_Core(
							double			c_lon,			///< [in]  経度
							double			c_lat,			///< [in]  緯度
							unsigned int	c_meshcode,		///< [in]  メッシュコード
							double			*c_x,			///< [out]  正規化X
							double			*c_y,			///< [out]  正規化Y
							double			offset_lon,		///< [in]  メッシュ原点のオフセット値（経度）
							double			offset_lat		///< [in]  メッシュ原点のオフセット値（緯度）
						);

		//--------------------------------------------------------------------
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		*
		* 指定したメッシュコード(文字列)にたいして、正規化座標値を求める 2006.09.08
		* @retval	0	成功
		* @retval -1	失敗
		*/
		//--------------------------------------------------------------------
		int			LLtoMesh2_Core(
							double	c_lon,			///< [in]  経度
							double	c_lat,			///< [in]  緯度
							char	*c_meshcode,	///< [out]  メッシュコード
							double	*c_x,			///< [out]  正規化X
							double	*c_y,			///< [out]  正規化Y
							double	offset_lon,		///< [in]  メッシュ原点のオフセット値（経度）
							double	offset_lat		///< [in]  メッシュ原点のオフセット値（緯度）
						);

		//=======================================================================
		//	下記3つのメンバ関数、BLHtoXYZ(), XYZtoBLH(), XYZtoXYZ()	は、
		//  [Bug 5755] の要望により追加 2008.01.21
		//	※【補足事項】
		//	この3つの測地系変換関数は、飛田 幹夫氏著書、「世界測地系と座標変換」
		//  で解説されている手法を元に作成されています。
		//=======================================================================
		//--------------------------------------------------------------------
		/**
		* @brief 地理座標値から三次元直交座標値への変換　
		*
		* (B,L,He)->(X,Y,Z)
		* @return	無し
		*/
		//--------------------------------------------------------------------
		void		BLHtoXYZ(
							double	c_dA,			///< [in]  回転楕円体長半径(m)
							double	c_dInvF,		///< [in]  扁平率fの逆数1/f
							double	c_dLon,			///< [in]  経度
							double	c_dLat,			///< [in]  緯度
							double	c_dHe,			///< [in]  楕円体高(m)
							double*	c_dX,			///< [out]  原点からのX軸方向の距離(m)
							double*	c_dY,			///< [out]  原点からのY軸方向の距離(m)
							double*	c_dZ			///< [out]  原点からのZ軸方向の距離(m)
						);

		//--------------------------------------------------------------------
		/**
		* @brief 三次元直交座標値から地理座標値への変換
		*
		* (X,Y,Z)->(B,L,He)
		* @return	無し
		*/
		//--------------------------------------------------------------------
		void		XYZtoBLH(
							double	c_dA,			///< [in]  回転楕円体長半径(m)
							double	c_dInvF,		///< [in]  扁平率fの逆数1/f
							double	c_dX,			///< [in]  原点からのX軸方向の距離(m)
							double	c_dY,			///< [in]  原点からのY軸方向の距離(m)
							double	c_dZ,			///< [in]  原点からのZ軸方向の距離(m)
							double*	c_dLon,			///< [out]  経度
							double*	c_dLat,			///< [out]  緯度
							double*	c_dHe			///< [out]  楕円体高(m)
						);


		//---------------------------------------------------------------------
		/**
		* @brief 三次元直交座標間での座標変換
		* @return	無し
		*/
		//---------------------------------------------------------------------
		void		XYZtoXYZ(
							param_7	c_pParam7,		///< [in]  変換に使用する7パラメータ
							double	c_dXi,			///< [in]  原点からのX軸方向の距離(m)
							double	c_dYi,			///< [in]  原点からのY軸方向の距離(m)
							double	c_dZi,			///< [in]  原点からのZ軸方向の距離(m)
							double*	c_dXo,			///< [out]  原点からのX軸方向の距離(m)
							double*	c_dYo,			///< [out]  原点からのY軸方向の距離(m)
							double*	c_dZo			///< [out]  原点からのZ軸方向の距離(m)
						);

		//---------------------------------------------------------------------
		/**
		* @brief 経緯度の測地系相互変換関数
		*
		* 7パラメータと変換前後の楕円体の情報があればこの関数で経緯度の相互
		* 変換が可能。現状東京測地系(Tokyo97)と世界測地系(JGD2000)間の変換しか
		* 行わないのと、混乱を避けるため取りあえずプライベートにしておく
		* @return	無し
		*/
		//---------------------------------------------------------------------
		void		BLHtoBLH(
							double	c_dBefore_A,	///< [in]  変換前座標系の準拠する楕円体長半径(m)
							double	c_dBefore_InvF,	///< [in]  変換前座標系の準拠する楕円体の扁平率
							double	c_dAfter_A,		///< [in]  変換前座標系の準拠する楕円体長半径(m)
							double	c_dAfter_InvF,	///< [in]  変換前座標系の準拠する楕円体の扁平率
							param_7	c_pBtoAParam7,	///< [in]  変換に使用する7パラメータ
							double	c_dBefore_Lon,	///< [in]  変換前経度
							double	c_dBefore_Lat,	///< [in]  変換前緯度
							double	c_dBefore_He,	///< [in]  変換前楕円体高(m)
							double*	c_dAfter_Lon,	///< [out]  変換後経度
							double*	c_dAfter_Lat,	///< [out]  変換後緯度
							double*	c_dAfter_He		///< [out]  変換後楕円体高(m)
						);

		//---------------------------------------------------------------------
		/**
		* @brief 経緯度から１９座標系のＸＹへ変換(楕円体情報を指定)
		* @return 無し
		*/
		//---------------------------------------------------------------------
		void	LLtoXY19_Core	(
							double		Ra,			///< [in] 地球楕円体の長半径（メートル）
							double		ea2,		///< [in] 第一離心率の2乗
							double		eb2,		///< [in] 第二離心率の2乗
							int			axis,		///< [in] 該当する座標系の番号
							double		lon,		///< [in] 経度
							double		lat,		///< [in] 緯度
							int			mode,		///< [in] 変換モード(0=日本測地系,1=世界測地系)
							double		*x,			///< [out] 19座標系のX
							double		*y			///< [out] 19座標系のY
						);

		//------------------------------------------------------------
		/**
		* @brief １９座標系のＸＹから経緯度へ変換(楕円体情報を指定)
		* @return 無し
		*/
		//------------------------------------------------------------
		void 	XY19toLL_Core	(
							double		Ra,			///< [in] 地球楕円体の長半径（メートル）
							double		ea2,		///< [in] 第一離心率の2乗
							double		eb2,		///< [in] 第二離心率の2乗
							double		f,			///< [in] 楕円体の扁平率
							int			axis,		///< [in] 該当する座標系の番号
							double		x,			///< [in] 北方向の座標値
							double		y,			///< [in] 東方向の座標値
							int			mode,		///< [in] 変換モード(0=日本測地系,1=世界測地系)
							double		*lon,		///< [in] 経度
							double		*lat		///< [in] 緯度
						);

		//---------------------------------------------------------------------
		/**
		* @brief ２地点の経緯度から、２点間の距離 + 距離(X,Y)mを求める
		* 
		* 注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
		* @return		２地点間の距離
		*/
		//---------------------------------------------------------------------
		double	GetDistXY_Core	(
							double		Ra,			///< [in] 地球楕円体の長半径（メートル）
							double		ea2,		///< [in] 第一離心率の2乗
							double		eLon1,		///< [in] 経度１
							double		eLat1,		///< [in] 緯度１
							double		eLon2,		///< [in] 経度２
							double		eLat2,		///< [in] 緯度２
							double		*cDist_X,	///< [out] 距離X(m)
							double		*cDist_Y	///< [out] 距離Y(m)
						);

		//---------------------------------------------------------------------
		/**
		* @brief	赤道から緯度Phiまでの子午線孤長を得る
		*
		* 【参考】
		*	http://vldb.gsi.go.jp/sokuchi/surveycalc/algorithm/b2s/b2s.htm
		* @return 子午線孤長(m)
		*/
		//---------------------------------------------------------------------
		double	M_Arc_Len_Core	(
							double		Ra,		///< [in] 地球楕円体の長半径（メートル）
							double		ea2,	///< [in] 第一離心率の2乗
							double		Phi		///< [in] 緯度
						);

	private:
		
		//------------------------------------------------------------------------------------------------
		/* 2009年01月13日 地域毎のパラメータでの変換用に追加 */
		//------------------------------------------------------------------------------------------------
		enum {
			JGD2000_RP_UNINITED	= 0,///< 未初期化
			CONVED_REGIONPRM,		///< 地域毎パラメータで変換
			CONVED_3PRM				///< 3パラメータで変換された
		};

		/**
		* @class param_info
		* @brief TKY2JGDの地域パラメータテーブル用
		*/
		struct	param_info	
		{
			int		e_imeshcode;	///< 3次メッシュコード
			double	e_dDlon;		///< 経度
			double	e_dDlat;		///< 緯度

			param_info(){		///< コンストラクタ
				e_imeshcode	= 0;
				e_dDlon		= 0.0;
				e_dDlat		= 0.0;
			};
			~param_info() {	///< デストラクタ
			};
			int	operator<(const param_info &c_info) const{ ///< 比較演算子定義
				int ret	= 0;
				if(	(e_imeshcode != c_info.e_imeshcode) ) {
					if(e_imeshcode < c_info.e_imeshcode)
						ret	= 1;
				}
				return(ret);
			};
		};

		std::map<int, std::set<param_info, std::less<param_info>>> m_outCode2nearCodes;	///< パラメータ外の3次メッシュと最近傍パラメータとのマップ（パラメータ外の計算を高速化させるため）

		std::set<param_info, std::less<param_info> >	m_spParam;				///< 読み込んだパラメータを格納する
		std::set<int, std::less<int> >					m_siTokyo97_Area;		///< IPCでTokyo97に該当する2次メッシュ
		bool											m_bJGD2000_RP_Inited;	///< JGD2000変換機能初期化フラグ

		//---------------------------------------------------------------------
		/** 
		* @brief 経緯度から、3次メッシュコード + (x,y) を求める
		* @return	無し
		*/
		//---------------------------------------------------------------------
		void		LLto3rdMesh	(
							double	lon,			///< [in]  経度
							double	lat,			///< [in]  緯度
							int		*meshcode,		///< [out]  3次メッシュコード
							int		*x,				///< [out]  正規化X
							int		*y,				///< [out]  正規化Y
							int		no_mill			///< [in]  XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）
						);

		//---------------------------------------------------------------------
		/**
		* @brief 3次メッシュコード + (x,y) から、経緯度を求める 2007.09.13
		* @return	成功	true
		* @return	失敗	false
		*/
		//---------------------------------------------------------------------
		bool		Mesh3rdtoLL(
							int		meshcode,		///< [in]  メッシュコード
							int		x,				///< [in]  正規化X
							int		y,				///< [in]  正規化Y
							double*	lon,			///< [out]  経度
							double*	lat				///< [out]  緯度
						);

		//---------------------------------------------------------------------
		/**
		* @brief 原点メッシュから(x,y)離れた所の3次メッシュコードを求める
		* @return	成功：メッシュコード
		* @return	失敗： -1
		*/
		//---------------------------------------------------------------------
		int			Get3rdMeshCode(
							int		meshcode,		///< [in]  3次メッシュコード
							int		xdist,			///< [in]  原点メッシュからX方向へ何メッシュ離れているか
							int		ydist			///< [in]  原点メッシュからY方向へ何メッシュ離れているか
						);

		//---------------------------------------------------------------------
		/**
		* @brief バイリニア補間で値を求めてくる
		* @return	計算値
		*/
		//---------------------------------------------------------------------
		double		Bilinear(
							double	c_dLU,			///< [in]  左上の値
							double	c_dRU,			///< [in]  右上の値
							double	c_dLD,			///< [in]  左下の値
							double	c_dRD,			///< [in]  右下の値
							double	c_dPtX,			///< [in]  矩形内のX方向位置(0.0<=X<=1.0)
							double	c_dPtY			///< [in]  矩形内のY方向位置(0.0<=Y<=1.0)
						);

		//---------------------------------------------------------------------
		/** 
		* @brief パラメータテーブルから左下、左上、右上、右下のパラメータを取得する
		* @return	失敗 == 0
		* @return	パラメータの範囲内で変換 == 1
		* @return	パラメータの範囲外で変換(精度が範囲内より甘い) == 2
		*/
		//---------------------------------------------------------------------
		bool		GetParam(
							int			c_iMeshCode,///< [in]  メッシュコード
							param_info*	c_pLD_Prm,	///< [out]  左下のパラメータ
							param_info*	c_pLU_Prm,	///< [out]  左上のパラメータ
							param_info*	c_pRU_Prm,	///< [out]  右上のパラメータ
							param_info*	c_pRD_Prm,	///< [out]  右下のパラメータ
							bool		c_bIPC_Flag	///< [in]	IPC固有変換をするかどうか true=IPC固有変換 false=世間一般対応変換
						);

		//---------------------------------------------------------------------
		/** 
		* @brief パラメータテーブルから距離的に一番近いパラメータを取得する
		* @return	失敗 == 0
		* @return	パラメータの範囲内で変換 == 1
		* @return	パラメータの範囲外で変換(精度が範囲内より甘い) == 2
		*/
		//---------------------------------------------------------------------
		bool		GetNearestParam(
							double		c_dLon,			///< [in]   経度
							double		c_dLat,			///< [in]   緯度
							param_info*	c_pNearest_Prm,	///< [out]  左下のパラメータ
							bool		c_bIPC_Flag		///< [in]	IPC固有変換をするかどうか true=IPC固有変換 false=世間一般対応変換
						);

		//---------------------------------------------------------------------
		/** 
		* @brief パラメータの範囲をチェックする
		* @return	範囲内 == true
		* @return	範囲外 == false
		*/
		//---------------------------------------------------------------------
		bool		ChkParamRange(
						long	c_lLD3rdMesh,			///< [in] 左下の3次メッシュコード
						long	c_lRU3rdMesh,			///< [in] 右上の3次メッシュコード
						long	c_lTgt3rdMesh			///< [in] ターゲットの3次メッシュコード
					);

		//---------------------------------------------------------------------
		/**
		* @brief IPCのTokyo97エリアのメッシュかどうかを調べる
		* @return	失敗 == false
		* @return	成功 == true
		*/
		//---------------------------------------------------------------------
		bool		Is_IPC_Tokyo97_Area(
							int				c_iMeshCode		///< [in]  2次メッシュコード
						);

		//---------------------------------------------------------------------
		/**
		* @brief 経緯度の測地系相互変換
		* @return	地域パラメータの範囲内で変換 == 1
		* @return	地域パラメータの範囲外で変換(精度が範囲内より甘い) == 2
		*/
		//---------------------------------------------------------------------
		int			LLConv(
							double	c_dBefore_Lon,	///< [in]  変換前経度
							double	c_dBefore_Lat,	///< [in]  変換前緯度
							double*	c_dAfter_Lon,	///< [out]  変換後経度
							double*	c_dAfter_Lat,	///< [out]  変換後緯度
							int		c_iMode,		///< [in]  変換モード（0=Tokyo->JGD2000への変換 1=JGD2000->Tokyoへの変換）
							bool	c_bIPC_Flag		///< [in]  IPC固有変換をするかどうかtrue=IPC固有変換 false=世間一般対応変換
						);

	public:
		//---------------------------------------------------------------------
		/**
		* @brief JGD2000パラメータファイルを読み込み内部テーブルを作成
		*
		* @code
		* <例>
		* double	blon = 0.0, blat = 0.0; // 変換前経緯度
		* double	alon = 0.0, alat = 0.0; // 変換後経緯度
		* crd_cnv	crd;
		* crd.JGD2000_RP_Init("TKY2JGD.par");
		* crd.TOKYOtoJGD2000_RP(blon, blat, &alon, &alat, false);
		* @endcode
		* @return	成功 == true, 失敗 == false
		*/
		//---------------------------------------------------------------------
		bool		JGD2000_RP_Init(
							char*			c_fname			///< [in]  読み込みファイル名
						);

		//------------------------------------------------------------------------------------------------

	public:

		crd_cnv()		///< コンストラクタ
		{
			// 19座標系原点の初期化
			Rad_Origin[0].lat	= 0.575958653158128774940394123404985293745994567871093750000000;
			Rad_Origin[0].lon	= 2.260201381332656467293418245390057563781738281250000000000000;
			Rad_Origin[1].lat	= 0.575958653158128774940394123404985293745994567871093750000000;
			Rad_Origin[1].lon	= 2.286381320112572002045681074378080666065216064453125000000000;
			Rad_Origin[2].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[2].lon	= 2.306743494719171838625015880097635090351104736328125000000000;
			Rad_Origin[3].lat	= 0.575958653158128774940394123404985293745994567871093750000000;
			Rad_Origin[3].lon	= 2.330014551412429746335419622482731938362121582031250000000000;
			Rad_Origin[4].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[4].lon	= 2.344558961845716105187875655246898531913757324218750000000000;
			Rad_Origin[5].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[5].lon	= 2.373647782712288378803577870712615549564361572265625000000000;
			Rad_Origin[6].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[6].lon	= 2.394009957318888659472122526494786143302917480468750000000000;
			Rad_Origin[7].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[7].lon	= 2.417281014012146567182526268879882991313934326171875000000000;
			Rad_Origin[8].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[8].lon	= 2.440552070705404030803720161202363669872283935546875000000000;
			Rad_Origin[9].lat	= 0.698131700797731791219291608285857364535331726074218750000000;
			Rad_Origin[9].lon	= 2.458005363225347572608825430506840348243713378906250000000000;
			Rad_Origin[10].lat	= 0.767944870877504959238990522862877696752548217773437500000000;
			Rad_Origin[10].lon	= 2.447824275922047210229948177584446966648101806640625000000000;
			Rad_Origin[11].lat	= 0.767944870877504959238990522862877696752548217773437500000000;
			Rad_Origin[11].lon	= 2.482730860961933849750948866130784153938293457031250000000000;
			Rad_Origin[12].lat	= 0.767944870877504959238990522862877696752548217773437500000000;
			Rad_Origin[12].lon	= 2.517637446001820489271949554677121341228485107421875000000000;
			Rad_Origin[13].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[13].lon	= 2.478367537831947853277370086289010941982269287109375000000000;
			Rad_Origin[14].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[14].lon	= 2.225294796292770271861627406906336545944213867187500000000000;
			Rad_Origin[15].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[15].lon	= 2.164208272472968541677573739434592425823211669921875000000000;
			Rad_Origin[16].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[16].lon	= 2.286381320112572002045681074378080666065216064453125000000000;
			Rad_Origin[17].lat	= 0.349065850398865895609645804142928682267665863037109375000000;
			Rad_Origin[17].lon	= 2.373647782712288378803577870712615549564361572265625000000000;
			Rad_Origin[18].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[18].lon	= 2.687807048071267690403374217567034065723419189453125000000000;

			// 日本
			ORIGIN_JPN.lon	= 100.0;	// 東経100度
			ORIGIN_JPN.lat	= 0.0;
			// 東ヨーロッパ
			ORIGIN_EU.lon	= -30.0;	// 西経30度
			ORIGIN_EU.lat	= 10.0;
			// 北アメリカ
			ORIGIN_NA.lon	= -150.0;// 西経150度 2003.05.08修正
			ORIGIN_NA.lat	= 10.0;
			// アラスカ・ハワイ
			ORIGIN_AH.lon	= -180.0;// 西経 2005.12.22修正
			ORIGIN_AH.lat	= 10.0;
			// 東南アジア・オーストラリア
			ORIGIN_SEAA.lon	= 80.0;// 東経80度
			ORIGIN_SEAA.lat	= -45.0;// 南緯45度
			// 南アメリカ 2008.12.01 追加
			ORIGIN_SA.lon	= -120.0;	// 西経120度
			ORIGIN_SA.lat	= -56.0;	// 南緯56度
			// 新海外都市地図用 2008.12.01 追加
			ORIGIN_SJ.lon	= -180.0;	// 西経180度
			ORIGIN_SJ.lat	= -90.0;	// 南緯90度

			//--------------------------------------------------------------------------------------------------------
			// IPCで以前日本測地系→Tokyo97へ対応をおこなった箇所をメッシュ単位
			// で登録。
			// ※IPCでは以前ずれの大きな離島について、Tokyo97に合わせる処理を行った。
			//   TKY2JGD.parのシフト量は、日本測地系→世界測地系へ直接変換するための
			//   シフト量であるため、Tokyo97対応を行った地域では地域毎のパラメータを
			//   使用するとずれが大きくなってしまうため、このような対応を行う必要がある。

			// 宮古島、伊良部島
			m_siTokyo97_Area.insert ( 372501 );	m_siTokyo97_Area.insert ( 372502 );	m_siTokyo97_Area.insert ( 372503 );
			m_siTokyo97_Area.insert ( 372511 );	m_siTokyo97_Area.insert ( 372512 );	m_siTokyo97_Area.insert ( 372513 );
			m_siTokyo97_Area.insert ( 372521 );	m_siTokyo97_Area.insert ( 372522 );	m_siTokyo97_Area.insert ( 372523 );
			m_siTokyo97_Area.insert ( 372531 );	m_siTokyo97_Area.insert ( 372532 );
			// 多良間島、水納島
			m_siTokyo97_Area.insert ( 362475 );	m_siTokyo97_Area.insert ( 372405 );	m_siTokyo97_Area.insert ( 372415 );
			// 石垣島
			m_siTokyo97_Area.insert ( 362430 );	m_siTokyo97_Area.insert ( 362431 );	m_siTokyo97_Area.insert ( 362440 );	
			m_siTokyo97_Area.insert ( 362441 );	m_siTokyo97_Area.insert ( 362442 );	m_siTokyo97_Area.insert ( 362450 );	
			m_siTokyo97_Area.insert ( 362451 );	m_siTokyo97_Area.insert ( 362452 );	m_siTokyo97_Area.insert ( 362461 );	
			m_siTokyo97_Area.insert ( 362462 );	m_siTokyo97_Area.insert ( 362472 );	
			// 西表島
			m_siTokyo97_Area.insert ( 362326 );	m_siTokyo97_Area.insert ( 362327 );	m_siTokyo97_Area.insert ( 362420 );
			m_siTokyo97_Area.insert ( 362335 );	m_siTokyo97_Area.insert ( 362336 );	m_siTokyo97_Area.insert ( 362337 );
			m_siTokyo97_Area.insert ( 362345 );	m_siTokyo97_Area.insert ( 362346 );	m_siTokyo97_Area.insert ( 362347 );
			m_siTokyo97_Area.insert ( 362356 );
			// 仲御神島
			m_siTokyo97_Area.insert ( 362324 );
			// 波照間島
			m_siTokyo97_Area.insert ( 362305 );	m_siTokyo97_Area.insert ( 362306 );
			// 与那国島
			m_siTokyo97_Area.insert ( 362257 );	m_siTokyo97_Area.insert ( 362350 );
			// 北大東島
			m_siTokyo97_Area.insert ( 383172 );
			// 南大東島
			m_siTokyo97_Area.insert ( 383151 ); m_siTokyo97_Area.insert ( 383152 );
			m_siTokyo97_Area.insert ( 383161 ); m_siTokyo97_Area.insert ( 383162 );
			// 青ヶ島
			m_siTokyo97_Area.insert ( 483956 );
			// 硫黄島
			m_siTokyo97_Area.insert ( 374102 );	m_siTokyo97_Area.insert ( 374112 );	m_siTokyo97_Area.insert ( 374113 );
			//--------------------------------------------------------------------------------------------------------

			m_bJGD2000_RP_Inited	= false;

		}
		~crd_cnv()		///< デストラクタ
		{
		}

		//---------------------------------------------------------------------
		/**
		    原点コード
			座標変換の世界共通化のため追加　2006.09.06 
		*/
		//---------------------------------------------------------------------
		enum {
			JPN_ORIGIN	= 0,	///< 日本
			EU_ORIGIN,			///< 東ヨーロッパ
			NA_ORIGIN,			///< 北アメリカ
			AH_ORIGIN,			///< アラスカ・ハワイ
			SEAA_ORIGIN,		///< 東南アジア・オーストラリア
			SA_ORIGIN,			///< 南アメリカ 2008.12.01 追加
			SJ_ORIGIN			///< 新国際都市地図 2011.12.08 追加
		};

		//---------------------------------------------------------------------
		/**
		* @brief 19座標系の北海道の5桁コードと系の対応を初期化
		*
		* Sys_Number()の、(経度,緯度)＋5桁コード指定版を使用する際に使用する。
		* 引数を指定しない、又はNULLを指定した場合は、デフォルト設定される。
		* デフォルト設定をそのまま使用する場合は、Sys_Number_Init()指定は不要。
		* デフォルト設定では、住所DBのY1005相当のコードに対応。
		* ファイルを指定した場合は、ファイルからコードと系の対応を読み込む。
		* ファイル指定はY1005から、5桁コードに追加等の変更が発生した場合に
		* 使用する。
		* ※ファイルのフォーマットは、
		* <5桁コード>\\t<系番号>
		* 先頭に'#'をつけた場合はコメント行として認識される
		*
		* @code
		* <例>
		* char*	code	= "01202";
		* crd_cnv	crd;
		* crd.Sys_Number_Init("Hokkaido_Sys19.tbl");
		* crd.Sys_Number(140.8, 40.8, code);
		* @endcode
		* @return	成功 == true, 失敗 == false
		*/
		//---------------------------------------------------------------------
		bool		Sys_Number_Init(
							char*			c_fname=NULL	///< [in]  読み込みファイル名
						);

		/**
		* 
		* @brief 経緯度から一番近いところの19座標系の系を得る
		* 
		* （なんか無駄なことをしているっぽい?）2000/1/12 checked ？？
		* @code
		* <例>
		* int		axis = 0;	// 系番号を格納
		* crd_cnv	crd;
		* axis = crd.Sys_Number(140.0, 39.0);
		* printf("系 = %d\n", axis );
		* @endcode
		* @return     系番号
		*/
		int			Sys_Number	(
							double	lon,	///< [in]  経度
							double	lat		///< [in]  緯度
						);

		/**
		* 
		* @brief 経緯度と市区町村コード(5桁)から19座標系の系を得る
		* 
		* <参考リンク> 国土地理院のHP、
		*  地理院ホーム  > 測量法  > 平面直角座標系（平成十四年国土交通省告示第九号） 
		* http://www.gsi.go.jp/LAW/heimencho.html
		* @code
		* <例>
		* int		axis = 0;	// 系番号を格納
		* crd_cnv	crd;
		* axis = crd.Sys_Number(140.0, 39.0);
		* printf("系 = %d\n", axis );
		* @endcode
		* @return     系番号(市区町村コードや経緯度に間違いがある場合は0を返却)
		*/
		int			Sys_Number	(
							double	lon,	///< [in]  経度
							double	lat,	///< [in]  緯度
							char*	code5	///< [in]  市区町村コード(5桁)
						);

		/**
		* @brief メッシュコードから該当する１９座標系の数と番号を得る
		* 
		* 該当する系がない場合、数、番号ともに0を返す 2000.1.12 checked
		* @code
		* <例>
		* int		meshcode = 594141;
		* sys_num*	kei = NULL;
		* crd_cnv	crd;
		* kei	=	crd.Sys_Number(meshcode);
		* printf("該当数=%d: 系1=%d 系2=%d\n", kei->total, kei->sys[0], kei->sys[1]);
		* @endcode
		* @return 構造体 sys_num へのポインタ
		*/
		sys_num*	Sys_Number	(
							int		meshcode	///< [in]  メッシュコード
						);

		/**
		* @brief メッシュコードから該当する１９座標系の数と番号を得る
		* 
		* 該当する系がない場合、数、番号ともに0を返す 2000.1.12 checked
		* @code
		* <例>
		* int		meshcode = 594141;
		* sys_num	kei;
		* crd_cnv	crd;
		* crd.Sys_Number(meshcode, &kei);
		* printf("該当数=%d: 系1=%d 系2=%d\n", kei.total, kei.sys[0], kei.sys[1]);
		* @endcode
		*/
		void		Sys_Number	(
							int		meshcode,		///< [in]  メッシュコード
							sys_num *sys			///< [out]  系番号
						);

		/**
		* @brief 経緯度から１９座標系のＸＹへ変換
		* @code
		* <例>
		* int		axis = 10;
		* double	lon = 140.0, lat = 40;
		* xy_coord*	ret_xyc = NULL;
		* crd_cnv	crd;
		* ret_xyc = crd.LLtoXY19(axis, lon, lat);
		* printf("系=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, ret_xyc->x, ret_xyc->y);
		* @endcode
		* @return 構造体 xy_coord へのポインタ
		*/
		xy_coord*	LLtoXY19	(
							int		axis,	///< [in]  該当する座標系の番号
							double	lon,	///< [in]  経度
							double	lat		///< [in]  緯度
						);

		/**
		* @brief 経緯度から最も近い１９座標系の系とＸＹを返す
		* @code
		* <例>
		* double	lon = 140.0, lat = 40;
		* coord19*	ret_c19 = NULL;
		* crd_cnv	crd;
		* ret_c19 = crd.LLtoXY19(lon, lat);
		* printf("系=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", ret_c19->sys, ret_c19->x, ret_c19->y);
		* @endcode
		* @return 構造体 coord19 へのポインタ
		*/
		coord19*	LLtoXY19 (
							double  lon,	///< [in]  経度
							double  lat		///< [in]  緯度
						);	
		/**
		* @brief 経緯度から１９座標系のＸＹへ変換
		* @code
		* <例>
		* int		axis = 10;
		* double	lon = 140.0, lat = 40;
		* xy_coord	ret_xyc;
		* crd_cnv	crd;
		* crd.LLtoXY19(axis, lon, lat, &ret_xyc);
		* printf("系=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, ret_xyc.x, ret_xyc.y);
		* @endcode
		*/
		void		LLtoXY19	(
							int			axis,       ///< [in]  該当する座標系の番号
							double		lon,        ///< [in]  経度
							double		lat,        ///< [in]  緯度
							xy_coord	*coord		///< [out]  19座標系のXY
						);

		/**
		* @brief 経緯度から最も近い１９座標系の系とＸＹを返す
		* 
		* 1998.7.14
		* @code
		* <例>
		* double	lon = 140.0, lat = 40;
		* coord19	ret_c19;
		* crd_cnv	crd;
		* crd.LLtoXY19(lon, lat, &ret_c19);
		* printf("系=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", ret_c19.sys, ret_c19.x, ret_c19.y);
		* @endcode
		*/
		void		LLtoXY19 (
							double		lon,		///< [in]  経度
							double		lat,		///< [in]  緯度
							coord19		*sys_xy		///< [out]  19座標系の系とXYを表す
						);

		/**
		* @brief 経緯度から１９座標系のＸＹへ変換
		* 
		* 2000.1.12
		* @code
		* <例>
		* int		axis = 10;
		* double	lon = 140.0, lat = 40;
		* double	x = 0.0, y = 0.0;	// 19座標系のXY
		* crd_cnv	crd;
		* crd.LLtoXY19(axis, lon, lat, &x, &y);
		* printf("系=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, x, y);
		* @endcode
		*/
		void		LLtoXY19	(	
							int			axis,		///< [in]  該当する座標系の番号
							double		lon,		///< [in]  経度
							double		lat,		///< [in]  緯度
							double		*x,			///< [out]  19座標系のX(緯度方向)
							double		*y			///< [out]  19座標系のY(経度方向)
						);

		/**
		* @brief 経緯度から１９座標系のＸＹへ変換（世界測地系版）
		* 
		* 2000.1.12
		* @code
		* <例>
		* int		axis = 10;
		* double	lon = 140.0, lat = 40;
		* double	x = 0.0, y = 0.0;	// 19座標系のXY
		* crd_cnv	crd;
		* crd.LLtoXY19_JGD2000(axis, lon, lat, &x, &y);
		* printf("系=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, x, y);
		* @endcode
		*/
		void		LLtoXY19_JGD2000	(	
							int			axis,		///< [in]  該当する座標系の番号
							double		lon,		///< [in]  経度
							double		lat,		///< [in]  緯度
							double		*x,			///< [out]  19座標系のX(緯度方向)
							double		*y			///< [out]  19座標系のY(経度方向)
						);

		/**
		* @brief １９座標系のＸＹから経緯度へ変換
		* @code
		* <例>
		* int		axis = 10;
		* double	x = 10000.0, y = 5000.0;	// 19座標系のXY
		* d_lonlat*	ll;
		* crd_cnv	crd;
		* ll = crd.XY19toLL(axis, x, y);
		* printf("lon=%0.10f, lat=%0.10f\n", ll->lon, ll->lat);
		* @endcode
		* @return	構造体 d_lonlat へのポインタ
		*/
		d_lonlat*	XY19toLL	(	
							int		axis,		///< [in]  該当する座標系の番号
							double	x,			///< [in]  19座標系のX(緯度方向)
							double	y			///< [in]  19座標系のY(経度方向)
						);
		
		/**
		* @brief １９座標系のＸＹから経緯度へ変換
		* @code
		* <例>
		* int		axis = 10;
		* double	x = 10000.0, y = 5000.0;	// 19座標系のXY
		* d_lonlat	ll;
		* crd_cnv	crd;
		* crd.XY19toLL(axis, x, y, &ll);
		* printf("lon=%0.10f, lat=%0.10f\n", ll.lon, ll.lat);
		* @endcode
		*/
		void		XY19toLL	(
							int			axis,		///< [in]  該当する座標系の番号
							double		x,			///< [in]  19座標系のX(緯度方向)
							double		y,			///< [in]  19座標系のY(経度方向)
							d_lonlat	*lonlat		///< [out]  10進で表した経度緯度(ex:130.23432342....)
						);
		
		/**
		* @brief １９座標系のＸＹから経緯度へ変換
		* @code
		* <例>
		* int		axis = 10;
		* double	x = 10000.0, y = 5000.0;	// 19座標系のXY
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.XY19toLL(axis, x, y, &lon, &lat);
		* printf("lon=%0.10f, lat=%0.10f\n", lon, lat);
		* @endcode
		*/
		void 		XY19toLL	(
							int			axis,	///< [in]  該当する座標系の番号
							double		x,		///< [in]  19座標系のX(緯度方向)
							double		y,		///< [in]  19座標系のY(経度方向)
							double		*lon,	///< [out]  経度
							double		*lat	///< [out]  緯度
					);

	/**
		* @brief １９座標系のＸＹから経緯度へ変換（世界測地系版）
		* @code
		* <例>
		* int		axis = 10;
		* double	x = 10000.0, y = 5000.0;	// 19座標系のXY
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.XY19toLL_JGD2000(axis, x, y, &lon, &lat);
		* printf("lon=%0.10f, lat=%0.10f\n", lon, lat);
		* @endcode
		*/
		void 		XY19toLL_JGD2000	(
							int			axis,	///< [in]  該当する座標系の番号
							double		X,		///< [in]  19座標系のX(緯度方向)
							double		Y,		///< [in]  19座標系のY(経度方向)
							double		*lon,	///< [out]  経度
							double		*lat	///< [out]  緯度
					);
		
		/**
		* @brief １９座標系の(X,Y)から、メッシュコード + (x,y) を求める
		* @code
		* <例>
		* int		axis = 10;
		* double	x_19 = 10000.0, y_19 = 5000.0;	// 19座標系のXY
		* int		meshlv = 64;	// 都市地図レベルに設定
		* int		mesh = 0;
		* int		x = 0, y = 0;
		* crd_cnv	crd;
		* crd.XY19toMesh(axis, x_19, y_19, meshlv, &mesh, &x, &y);
		* printf("mesh=%d, x=%d, y=%d\n", mesh, x, y);
		* @endcode
		* @retval		0	成功
		* @retval		-1	失敗
		*/
		int			XY19toMesh	(	
							int		sys,		///< [in]  19座標系の系番号
							double	x_19,		///< [in]  19座標系のX(緯度方向)
							double	y_19,		///< [in]  19座標系のY(経度方向)
							int		level,		///< [in]  メッシュのレベル (1,2,64)
							int		*meshcode,	///< [out]  メッシュコード
							int		*x,			///< [out]  正規化X
							int		*y			///< [out]  正規化Y
						);
		
		/**
		* @brief １９座標系の系番号とメッシュコード + (x,y)から、１９座標系の(X,Y)を求める
		* @code
		* <例>
		* int		axis = 10;
		* int		mesh = 594141;
		* int		x = 100000, y = 200000;	// メッシュの正規化XY
		* double	x_19 = 0.0, y_19 = 0.0;	// 19座標系のXY
		* crd_cnv	crd;
		* crd.MeshtoXY19(axis, mesh, x, y, &x_19, &y_19);
		* printf("系=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, x_19, y_19);
		* @endcode
		*/
		void		MeshtoXY19	(		
							int		sys,		///< [in]  19座標系の系番号
							int		meshcode,	///< [in]  メッシュコード
							int		x,			///< [in]  正規化X
							int		y,			///< [in]  正規化Y
							double	*x_19,		///< [out]  19座標系のX(緯度方向)
							double	*y_19		///< [out]  19座標系のY(経度方向)
						);
		
		/**
		* @brief 東経から、UTM座標のゾーン番号を得る
		* 
		* ※境界値をどうするかがちょっと問題
		*  2001.08.28
		* @code
		* <例>
		* double	lon = 140.0, lat = 40.0;
		* int		zone = 0;
		* double	ori_lon = 0.0, ori_lat = 0.0;	// ゾーンの原点経緯度
		* crd_cnv	crd;
		* zone	= crd.GetUTMZone(lon, lat, &ori_lon, &ori_lat);
		* printf("ゾーン=%d: 原点(lon,lat)=(%0.10f,%0.10f)\n", zone, ori_lon, ori_lat);
		* @endcode
		* @return  ゾーン番号: 対象範囲外
		* @return  -1 : 対象範囲外
		*/
		int			GetUTMZone	(	
							double	c_lon,			///< [in]  東経?度
							double	c_lat,			///< [in]  緯度(ダミー)
							double	*c_orig_lon,	///< [out]  原点経度
							double	*c_orig_lat		///< [out]  原点緯度
						);
		
		/**
		* @brief UTMのゾーン番号から原点経緯度を得る
		* 
		* ※境界値をどうするかがちょっと問題
		* 2001.08.28
		* @code
		* <例>
		* int		zone = 54;
		* double	ori_lon = 0.0, ori_lat = 0.0;	// ゾーンの原点経緯度
		* crd_cnv	crd;
		* crd.GetUTMOrigLL(zone, &ori_lon, &ori_lat);
		* printf("ゾーン=%d: 原点(lon,lat)=(%0.10f,%0.10f)\n", zone, ori_lon, ori_lat);
		* @endcode
		* @retval	0	ゾーン番号が見つかった
		* @retval	-1	ゾーン番号が見つからない
		*/
		int			GetUTMOrigLL	(
							int		c_zone,			///< [in]  ゾーン番号
							double	*c_orig_lon,	///< [out]  原点経度
							double	*c_orig_lat		///< [out]  原点緯度
						);
		
		/**
		* @brief UTM座標系のＸＹから経緯度(Bessel)へ変換
		*
		* 2001.08.28
		* @code
		* <例>
		* int		zone = 54;
		* double	x = 100000, y = 150000;
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.UTMtoLL(zone, x, y, &lon, &lat);
		* printf("(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	成功
		* @retval	-1 	失敗
		*/
		int 		UTMtoLL	(
							int		c_zone,		///< [in]  UTMのゾーン番号
							double	x,			///< [in]  北方向の座標値(m)
							double	y,			///< [in]  東方向の座標値(m)
							double	*c_lon,		///< [out]  経度(decimal)
							double	*c_lat		///< [out]  緯度(decimal)
						);
		int 		UTMtoLL_WGS84	(
							int		c_zone,		///< [in]  UTMのゾーン番号
							double	x,			///< [in]  北方向の座標値(m)
							double	y,			///< [in]  東方向の座標値(m)
							double	*c_lon,		///< [out]  経度(decimal)
							double	*c_lat		///< [out]  緯度(decimal)
						);
		int 		UTMtoLL_Core	(
							double ra,			///< [in] 地球楕円体の長半径（メートル）
							double ea2,			///< [in] 第一離心率の2乗
							double eb2,			///< [in] 第二離心率の2乗
							int		c_zone,		///< [in]  UTMのゾーン番号
							double	x,			///< [in]  北方向の座標値(m)
							double	y,			///< [in]  東方向の座標値(m)
							double	*c_lon,		///< [out]  経度(decimal)
							double	*c_lat		///< [out]  緯度(decimal)
						);
		
		/**
		* @brief 経緯度(Bessel)からUTM座標にする
		* @code
		* <例>
		* int		zone = 54;
		* double	lon = 140.0, lat = 40.0;
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoUTM(zone, lon, lat, &x, &y);
		* printf("ゾーン=%d: (X,Y)=(%0.10f,%0.10f)\n", zone, x, y);
		* @endcode
		* @retval	0	成功
		* @retval	-1 	失敗
		*/
		int			LLtoUTM	(
							int		c_zone,		///< [in]  ゾーン番号
							double	lon,		///< [in]  経度
							double	lat,		///< [in]  緯度
							double	*c_x,		///< [out]  UTM座標系のX座標値(m)
							double	*c_y		///< [out]  UTM座標系のY座標値(m)
						);
		int			LLtoUTM_WGS84	(
							int		c_zone,		///< [in]  ゾーン番号
							double	lon,		///< [in]  経度
							double	lat,		///< [in]  緯度
							double	*c_x,		///< [out]  UTM座標系のX座標値(m)
							double	*c_y		///< [out]  UTM座標系のY座標値(m)
						);
		int			LLtoUTM_Core	(
							double ra,			///< [in] 地球楕円体の長半径（メートル）
							double ea2,			///< [in] 第一離心率の2乗
							double eb2,			///< [in] 第二離心率の2乗
							int		c_zone,		///< [in]  ゾーン番号
							double	lon,		///< [in]  経度
							double	lat,		///< [in]  緯度
							double	*c_x,		///< [out]  UTM座標系のX座標値(m)
							double	*c_y		///< [out]  UTM座標系のY座標値(m)
						);
		/**
		* @brief 経緯度とそこから経度方向にX(m),緯度方向にY(m)離れたところの経緯度を求める（統一版）
		* 
		* ※収束しないケースがあるとの報告があったので収束回数の制限を設定(50回)
	　	*   計算内で一番近い結果になった経緯度を返すように変更。
		* @code
		* <例>
		* int	digit = 0;	// 桁数
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* int	count = 0;
		* // 東方向に100m、北方向に200mはなれたところの座標を求める
		* count	= crd.GetLLPt_Core(140.0, 39.0, 100.0, 200.0, &lon, &lat);
		* printf("(経度,緯度,count)=(%0.10f,%0.10f,%d)\n", lon, lat, count);
		* @endcode
		* @return		収束までの回数 \n
		* @return		-1 = 制限回数内に収束しない
		*/
		int			GetLLPt_Core(
							double		ra,			///< [in]  地球楕円体の長半径（m）
							double		ea2,		///< [in]  第一離心率の2乗
							double		eLon1,		///< [in]  経度1
							double		eLat1,		///< [in]  緯度1
							double		cDist_X,	///< [in]  距離X(m)
							double		cDist_Y,	///< [in]  距離Y(m)
							int			mode,		///< [in]  変換モード(0=日本測地系,1=世界測地系)
							double		*eLon2,		///< [out]  経度2
							double		*eLat2		///< [out]  緯度2
						);

		/**
		* @brief １０進から度、分、秒へ変換
		* @code
		* <例>
		* double	lon = 140.50423;// 経度(10進)
		* dms		mes_pt;			// 度分秒値
		* crd_cnv	crd;
		* crd.DECtoDMS(lon, &mes_pt);
		* printf("%0.10f=%d度%d分%0.5f秒\n", lon, mes_pt.deg, mes_pt.min, mes_pt.sec);
		* @endcode
		*/
		void		DECtoDMS	(
							double		lonlat,			///< [in]  経緯度（10進）
							dms			*mes_pt			///< [out]  変換後の測点の経緯度（度、分、秒）
						);
		
		/**
		* @brief １０進から度、分、秒へ変換
		* @code
		* <例>
		* double	lon = 140.50423;// 経度(10進)
		* dms*		mes_pt = NULL;	// 度分秒値
		* crd_cnv	crd;
		* mes_pt	= crd.DECtoDMS(lon);
		* printf("%0.10f=%d度%d分%0.5f秒\n", lon, mes_pt->deg, mes_pt->min, mes_pt->sec);
		* @endcode
		* @return	構造体 dms へのポインタ
		*/
		dms*		DECtoDMS	(	double	lonlat	);	///< [in]  経緯度（10進）
		
		/**
		* @brief 度、分、秒から１０進へ変換
		* 
		* 2000.03.25
		* @code
		* <例>
		* dms		mes_pt;		// 度分秒値
		* double	lon = 0.0;	// 経度(10進)
		* crd_cnv	crd;
		* mes_pt.deg = 140; mes_pt.min = 30; mes_pt.sec = 15.25;
		* crd.DMStoDEC(mes_pt, &lon);
		* printf("%d度%d分%0.5f秒=%0.10f\n", mes_pt.deg, mes_pt.min, mes_pt.sec, lon);
		* @endcode
		*/
		void		DMStoDEC	(
							dms		mes_pt,		///< [in]  経緯度（10進）
							double	*lonlat		///< [out]  変換後の測点の経緯度（度、分、秒）
						);
		
		/**
		* @brief 度、分、秒から１０進へ変換
		* 
		* 2000.03.25
		* @code
		* <例>
		* dms		mes_pt;		// 度分秒値
		* double	lon = 0.0;	// 経度(10進)
		* crd_cnv	crd;
		* mes_pt.deg = 140; mes_pt.min = 30; mes_pt.sec = 15.25;
		* lon		= crd.DMStoDEC(mes_pt);
		* printf("%d度%d分%0.5f秒=%0.10f\n", mes_pt.deg, mes_pt.min, mes_pt.sec, lon);
		* @endcode
		* @return	doubleの緯度および経度値
		*/
		double		DMStoDEC	(	dms		mes_pt	);	///< [in]  経緯度（10進）
		
		/**
		* @brief ２地点の経緯度から、２点間の距離を求める(単位: m)
		* 
		* 注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
		* @code
		* <例>
		* double	length = 0.0;
		* double	lon1 = 140.0, lat1 = 40.0, lon2 = 141.0, lat2 = 40.0;
		* crd_cnv	crd;
		* length	= crd.GetDist(lon1, lat1, lon2, lat2);
		* printf("距離=%0.10f(m)\n", length);
		* @endcode
		* @return		２地点間の距離
		*/
		double		GetDist	(
							double		eLon1,		///< [in]  経度1
							double		eLat1,		///< [in]  緯度1
							double		eLon2,		///< [in]  経度2
							double		eLat2		///< [in]  緯度2
						);
		
		/**
		* @brief ２地点の経緯度から、２点間の距離を求める(単位: m)
		* 
		* 注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
		* @code
		* <例>
		* double	length = 0.0;
		* d_lonlat	lonlat1, lonlat2;
		* lonlat1.lon = 140.0; lonlat1.lat = 40.0; lonlat2.lon = 141.0; lonlat2.lat = 40.0;
		* crd_cnv	crd;
		* length	= crd.GetDist(lonlat1, lonlat2);
		* printf("距離=%0.10f(m)\n", length);
		* @endcode
		* @return		２地点間の距離
		*/
		double		GetDist	(
							d_lonlat	lonlat1,	///< [in]  経緯度1
							d_lonlat	lonlat2		///< [in]  経緯度2
						);

		/**
		* @brief ２地点の経緯度から、２点間の距離を求める(単位: m)（世界測地系版）
		* 
		* 注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
		* @code
		* <例>
		* double	length = 0.0;
		* double	lon1 = 140.0, lat1 = 40.0, lon2 = 141.0, lat2 = 40.0;
		* crd_cnv	crd;
		* length	= crd.GetDist_JGD2000(lon1, lat1, lon2, lat2);
		* printf("距離=%0.10f(m)\n", length);
		* @endcode
		* @return		２地点間の距離
		*/
		double		GetDist_JGD2000	(
							double		eLon1,		///< [in]  経度1
							double		eLat1,		///< [in]  緯度1
							double		eLon2,		///< [in]  経度2
							double		eLat2		///< [in]  緯度2
						);
		
		/**
		* @brief ２地点のメッシュ+(x,y)から、２点間の距離を求める（単位: m) 
		* 
		* 2001.07.23
		* @code
		* <例>
		* double	length = 0.0;
		* crd_cnv	crd;
		* length	= crd.GetMeshDist(594141, 10000, 50000, 594142, 1000000, 200000);
		* printf("距離=%0.10f(m)\n", length);
		* @endcode
		* @return		２地点間の距離
		*/
		double		GetMeshDist(
							int			c_mesh1,	///< [in]  メッシュコード1
							int			c_x1,		///< [in]  X座標1
							int			c_y1,		///< [in]  Y座標1
							int			c_mesh2,	///< [in]  メッシュコード2
							int			c_x2,		///< [in]  X座標2
							int			c_y2		///< [in]  Y座標2
						);
		
		/**
		* @brief ２地点の経緯度から、２点間の距離 + 距離(X,Y)mを求める
		* 
		* 注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
		* @code
		* <例>
		* double	length = 0.0;
		* double	xdist = 0.0, ydist = 0.0; // X,Y方向の距離
		* crd_cnv	crd;
		* length	= crd.GetDistXY(140.0, 40.0, 141.0, 41.0, &xdist, &ydist);
		* printf("距離=%0.10f(m) 距離(X)=%0.10f(m) 距離(Y)=%0.10f(m)\n", length, xdist, ydist);
		* @endcode
		* @return		２地点間の距離
		*/
		double		GetDistXY	(
							double		eLon1,		///< [in]  経度1
							double		eLat1,		///< [in]  緯度1
							double		eLon2,		///< [in]  経度2
							double		eLat2,		///< [in]  緯度2
							double		*cDist_X,	///< [out]  距離X(m)
							double		*cDist_Y	///< [out]  距離Y(m)
						);

		/**
		* @brief ２地点の経緯度から、２点間の距離 + 距離(X,Y)mを求める（世界測地系版）
		* 
		* 注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
		* @code
		* <例>
		* double	length = 0.0;
		* double	lon1 = 0.0, lat1 = 0.0, lon2 = 0.0, lat2 = 0.0;
		* crd_cnv	crd;
		* length	= crd.GetDistXY_JGD2000(lon1, lat1, lon2, lat2, &xdist, &ydist);
		* printf("距離=%0.10f(m) 距離(X)=%0.10f(m) 距離(Y)=%0.10f(m)\n", length, xdist, ydist);
		* @endcode
		* @return		２地点間の距離
		*/
		double		GetDistXY_JGD2000	(
							double		eLon1,		///< [in]  経度1
							double		eLat1,		///< [in]  緯度1
							double		eLon2,		///< [in]  経度2
							double		eLat2,		///< [in]  緯度2
							double		*cDist_X,	///< [out]  距離X(m)
							double		*cDist_Y	///< [out]  距離Y(m)
						);

		/**
		* @brief メッシュ+(x,y) の2点の正規化ポイントでの差分を求める
		*
		* ※メッシュの桁数がいっしょのときのみ
		* @code
		* <例>
		* double	xdiff = 0.0, ydiff = 0.0; // X,Y方向の距離
		* crd_cnv	crd;
		* crd.GetNrmDiff(594141, 10000, 50000, 594142, 1000000, 200000, &xdiff, &ydiff);
		* printf("差分X=%0.10f, 差分Y=%0.10f\n", xdiff, ydiff);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			GetNrmDiff(
							int			c_mesh1,	///< [in]  メッシュコード
							int			c_x1,		///< [in]  正規化座標X
							int			c_y1,		///< [in]  正規化座標Y
							int			c_mesh2,	///< [in]  メッシュコード
							int			c_x2,		///< [in]  正規化座標X
							int			c_y2,		///< [in]  正規化座標Y
							double		*c_dx,		///< [out]  正規化座標差分X
							double		*c_dy		///< [out]  正規化座標差分Y
						);
		
		/**
		* @brief メッシュ+(x,y) の2点の正規化ポイントでの差分を求める
		* @code
		* <例>
		* double	xdiff = 0.0, ydiff = 0.0; // X,Y方向の距離
		* crd_cnv	crd;
		* crd.GetNrmDiff2(594141, 10000, 50000, 594142, 1000000, 200000, &xdiff, &ydiff);
		* printf("差分X=%0.10f, 差分Y=%0.10f\n", xdiff, ydiff);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			GetNrmDiff2(
							int			c_mesh1,	///< [in]  メッシュコード
							int			c_x1,		///< [in]  正規化座標X
							int			c_y1,		///< [in]  正規化座標Y
							int			c_mesh2,	///< [in]  メッシュコード
							int			c_x2,		///< [in]  正規化座標X
							int			c_y2,		///< [in]  正規化座標Y
							double		*c_dx,		///< [out]  正規化座標距離X
							double		*c_dy		///< [out]  正規化座標距離Y
						);
		
		/**
		* @brief メッシュ + (X,Y)から経緯度へ変換
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL(594141, 10000, 20000, &lon, &lat);
		* printf("(経度,緯度)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			MeshtoLL	(
							int		meshcode,		///< [in]  メッシュコード
							int		x,				///< [in]  正規化座標X
							int		y,				///< [in]  正規化座標Y
							double	*lon,			///< [out]  経度
							double	*lat			///< [out]  緯度
						);

		/**
		* @brief メッシュ + (X,Y)から経緯度へ変換(double)
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL2(594141, 10000.25, 20000.125, &lon, &lat);
		* printf("(経度,緯度)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			MeshtoLL2	(
							int		meshcode,		///< [in]  メッシュコード
							double	x,				///< [in]  正規化座標X
							double	y,				///< [in]  正規化座標Y
							double	*lon,			///< [out]  経度
							double	*lat			///< [out]  緯度
						);
		
		/**
		* @brief メッシュ + (X,Y)から1/256秒単位の経緯度へ変換
		* @code
		* <例>
		* int		lon256 = 0, lat256 = 0;
		* crd_cnv	crd;
		* crd.MeshtoLL256(604000, 0, 0, &lon256, &lat256);
		* printf("1/256秒(経度,緯度)=(%d, %d)\n", lon256, lat256);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			MeshtoLL256	(
							int		meshcode,	///< [in]  メッシュコード
							int		x,			///< [in]  正規化座標X
							int		y,			///< [in]  正規化座標Y
							int		*lon256,	///< [out]  経度
							int		*lat256		///< [out]  緯度
						);
		
		/**
		* @brief １０進小数の経緯度から1/256秒単位の経緯度へ変換
		* 
		* 経度、緯度の入力がintになっていたのを修正 2008.01.21
		* @code
		* <例>
		* int		lon256 = 0, lat256 = 0;
		* crd_cnv	crd;
		* crd.LLtoLL256(140.0, 40.0, &lon256, &lat256);
		* printf("1/256秒(経度,緯度)=(%d, %d)\n", lon256, lat256);
		* @endcode
		*/
		void		LLtoLL256	(
							double	lon,		///< [in]  経度
							double	lat,		///< [in]  緯度
							int		*lon256,	///< [out]  経度(1/256単位)
							int		*lat256		///< [out]  緯度(1/256単位)
						);
		
		/**
		* @brief 1/256秒単位の経緯度から、１０進小数の経緯度へ変換
		* @code
		* <例>
		* double	lon = 0, lat = 0;
		* crd_cnv	crd;
		* crd.LL256toLL(129024000, 36864000, &lon, &lat);
		* printf("(経度,緯度)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		*/
		void		LL256toLL	(
							int		lon256,		///< [in]  経度
							int		lat256,		///< [in]  緯度
							double	*lon,		///< [out]  経度
							double	*lat		///< [out]  緯度
						);
		
		/**
		* @brief メッシュの角から角の距離を求める(m)
		* @code
		* <例>
		* double	udist = 0.0, ddist = 0.0, ldist = 0.0, rdist = 0.0;
		* crd_cnv	crd;
		* crd.CtoCDist(594141, &udist, &ddist, &ldist, &rdist);
		* printf("上辺=%0.5f(m), 下辺=%0.5f(m)\n", udist, ddist);
		* printf("左辺=%0.5f(m), 右辺=%0.5f(m)\n", ldist, rdist);
		* @endcode
		*/
		void		CtoCDist	(
							int		meshcode,	///< [in]  メッシュコード
							double	*udist,		///< [out]  メッシュの上辺距離
							double	*ddist,		///< [out]  メッシュの下辺距離
							double	*ldist,		///< [out]  メッシュの左辺距離
							double	*rdist		///< [out]  メッシュの右辺距離
						);
		
		/**
		* @brief 原点メッシュから(x,y)メッシュ離れた所のメッシュコードを求める
		* 
		* 2001.3.20 マイナス方向でのバグを修正
		* @code
		* <例>
		* int		meshcode = 0;	// 求めるメッシュコード
		* crd_cnv	crd;
		* meshcode = crd.GetMeshCode(594141, 1, 10);
		* printf("メッシュ %d のX方向=%d,Y方向=%d 隣のメッシュは %d\n", 594141, 1, 10, meshcode );
		* @endcode
		* @return		成功：メッシュコード
		* @return		失敗： -1
		*/
		int			GetMeshCode(
							int		meshcode,	///< [in]  メッシュコード
							int		xdist,		///< [in]  原点メッシュからX方向へ何メッシュ離れているか
							int		ydist		///< [in]  原点メッシュからY方向へ何メッシュ離れているか
						);

		/**
		* @brief 原点メッシュから(x,y)メッシュ離れた所のメッシュコードを求める（新国際都市地図版）
		* 
		* 2001.3.20 マイナス方向でのバグを修正
		* @code
		* <例>
		* int		meshcode = 0;	// 求めるメッシュコード
		* crd_cnv	crd;
		* meshcode = crd.GetMeshCode(594141, 1, 10);
		* printf("メッシュ %d のX方向=%d,Y方向=%d 隣のメッシュは %d\n", 594141, 1, 10, meshcode );
		* @endcode
		* @return		成功：メッシュコード
		* @return		失敗： -1
		*/
		int			GetMeshCode_SJ(
							unsigned int	meshcode,	///< [in]  メッシュコード
							int				xdist,		///< [in]  原点メッシュからX方向へ何メッシュ離れているか
							int				ydist		///< [in]  原点メッシュからY方向へ何メッシュ離れているか
						);
		
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める
		* @code
		* <例>
		* int		lv = 2;	// 2次メッシュ
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // 594141,1000000,1000000 のようになっても許容する場合
		* crd.LLtoMesh(141.250, 39.750, lv, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // 594141,1000000,1000000 は許容しない場合→594152,0,0と返したい場合
		* crd.LLtoMesh(141.250, 39.750, lv, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			LLtoMesh	(
							double		lon,		///< [in]  経度
							double		lat,		///< [in]  緯度
							int			level,		///< [in]  メッシュのレベル (1,2,64)
							int			*meshcode,	///< [out]  メッシュコード
							int			*x,			///< [out]  正規化X
							int			*y,			///< [out]  正規化Y
							int			no_mill=0	///< [in]  XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す） 
						);
		
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		* 
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* @code
		* <例>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh2(141.250, 39.750, 594141, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.5f,%0.5f)\n", 594141, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			LLtoMesh2	(
							double		c_lon,			///< [in]  経度
							double		c_lat,			///< [in]  緯度
							int			c_meshcode,		///< [in]  メッシュコード
							double		*c_x,			///< [out]  正規化X
							double		*c_y			///< [out]  正規化Y
						);
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		* 
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* @code
		* <例>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh2(141.250, 39.750, 594141, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.10f,%0.10f)\n", 594141, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			LLtoMesh2	(
							double		c_lon,			///< [in]  経度
							double		c_lat,			///< [in]  緯度
							int			c_meshcode,		///< [in]  メッシュコード
							int			*c_x,			///< [out]  正規化X
							int			*c_y			///< [out]  正規化Y
						);
		
		/**
		@brief ○次メッシュの(x,y)から、×次メッシュの(x,y)に変換
		* @code
		* <例>
		* int	mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // 2次メッシュを都市地図メッシュに変換
		* crd.MeshtoMesh(594141, 0, 0, 64, &mesh, &x, &y);
		* printf("変換後(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			MeshtoMesh	(
							int		s_mesh,		///< [in]  元メッシュコード
							int		s_x,		///< [in]  正規化X
							int		s_y,		///< [in]  正規化Y
							int		level,		///< [in]  メッシュの次数 (1,2,64)
							int		*d_mesh,	///< [out]  変換後メッシュコード
							int		*d_x,		///< [out]  正規化X
							int		*d_y		///< [out]  正規化Y
						);
		
		/**
		* @brief ○次メッシュの(x,y)から、×次メッシュの(x,y)に変換
		*
		* 内部での求め方がMeshtoMeshとは違う。いづれどちらかに決める予定。 2000.10.06
		* @code
		* <例>
		* int	mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // 1次メッシュを2次メッシュに変換
		* crd.MeshtoMesh2(5940, 0, 0, 2, &mesh, &x, &y);
		* printf("変換後(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			MeshtoMesh2	(
							int		s_mesh,		///< [in]  元メッシュコード
							int		s_x,		///< [in]  正規化X
							int		s_y,		///< [in]  正規化Y
							int		level,		///< [in]  メッシュの次数 (1,2,64)
							int		*d_mesh,	///< [out]  変換後メッシュコード
							int		*d_x,		///< [out]  正規化X
							int		*d_y		///< [out]  正規化Y
						);
		
		/**
		* @brief メッシュコードの桁数判定
		* 
		* 2000.4.6
		* @code
		* <例>
		* int	digit = 0;	// 桁数
		* crd_cnv	crd;
		* digit	= crd.GetMeshDigit(594141);
		* printf("メッシュ桁数=%d\n", digit);
		* @endcode
		* @return		成功 = コードの桁数 2,4,6,8
		* @return		失敗 = -1
		*/
		int			GetMeshDigit	(	unsigned int	s_mesh	);		///< [in]  元メッシュコード

		/**
		* @brief 経緯度とそこから経度方向にX(m),緯度方向にY(m)離れたところの経緯度を求める
		* 
		* ※収束しないケースがあるとの報告があったので収束回数の制限を設定(50回)
	　	*   計算内で一番近い結果になった経緯度を返すように変更。
		* @code
		* <例>
		* int	digit = 0;	// 桁数
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* int	count = 0;
		* // 東方向に100m、北方向に200mはなれたところの座標を求める
		* count	= crd.GetLLPt(140.0, 39.0, 100.0, 200.0, &lon, &lat);
		* printf("(経度,緯度,count)=(%0.10f,%0.10f,%d)\n", lon, lat, count);
		* @endcode
		* @return		収束までの回数 \n
		* @return		-1 = 制限回数内に収束しない
		* 2000.4.13
		*/
		int			GetLLPt(
							double		eLon1,		///< [in]  経度1
							double		eLat1,		///< [in]  緯度1
							double		cDist_X,	///< [in]  距離X(m)
							double		cDist_Y,	///< [in]  距離Y(m)
							double		*eLon2,		///< [out]  経度2
							double		*eLat2		///< [out]  緯度2
						);

		/**
		* @brief 経緯度とそこから経度方向にX(m),緯度方向にY(m)離れたところの経緯度を求める（世界測地系版）
		* 
		* ※収束しないケースがあるとの報告があったので収束回数の制限を設定(50回)
	　	*   計算内で一番近い結果になった経緯度を返すように変更。
		* @code
		* <例>
		* int	digit = 0;	// 桁数
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* int	count = 0;
		* // 東方向に100m、北方向に200mはなれたところの座標を求める
		* count	= crd.GetLLPt(140.0, 39.0, 100.0, 200.0, &lon, &lat);
		* printf("(経度,緯度,count)=(%0.10f,%0.10f,%d)\n", lon, lat, count);
		* @endcode
		* @return		収束までの回数 \n
		* @return		-1 = 制限回数内に収束しない
		*/
		int			GetLLPt_JGD2000(
							double		eLon1,		///< [in]  経度1
							double		eLat1,		///< [in]  緯度1
							double		cDist_X,	///< [in]  距離X(m)
							double		cDist_Y,	///< [in]  距離Y(m)
							double		*eLon2,		///< [out]  経度2
							double		*eLat2		///< [out]  緯度2
						);
		
		/**
		* @brief 経緯度からガウス・クリューゲル座標系のX,Y(m)へ変換
		* 
		* 2000.9.19
		* @code
		* <例>
		* xy_coord*	ret_xyc = NULL;
		* crd_cnv	crd;
		* ret_xyc	= crd.LLtoXYGs(3, 8.5, 50.0);
		* printf("ゾーン=%d: 位置(X,Y)=(%0.10f,%0.10f)\n", 2, ret_xyc->x, ret_xyc->y);
		* @endcode
		* @return	構造体 xy_coord へのポインタ
		*/
		xy_coord*	LLtoXYGs	(
							int			axis,	///< [in]  該当するゾーンの番号(2～4)
							double		lon,	///< [in]  経度
							double		lat		///< [in]  緯度
						);
		
		/**
		* @brief 経緯度からガウス・クリューゲル座標系のX,Y(m)へ変換
		* 
		* 2000.9.19
		* @code
		* <例>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoXYGs(3, 8.5, 50.0, &x, &y);
		* printf("ゾーン=%d: 位置(X,Y)=(%0.10f,%0.10f)\n", 2, x, y);
		* @endcode
		*/
		void		LLtoXYGs	(
							int			axis,	///< [in]  該当するゾーンの番号(2～4)
							double		lon,	///< [in]  経度
							double		lat,	///< [in]  緯度
							double		*x,		///< [out]  ガウス・クリューゲル座標系のX
							double		*y		///< [out]  ガウス・クリューゲル座標系のY
						);
		
		/**
		* @brief ガウス・クリューゲル座標系のX,Y(m)から経緯度へ変換
		* 
		* 2000.9.20
		* @code
		* <例>
		* d_lonlat	ll;
		* crd_cnv	crd;
		* crd.XYGstoLL(3, 5540399.35009, 3464156.57117, &ll);
		* printf("(経度,緯度)=(%0.10f,%0.10f)\n", ll.lon, ll.lat);
		* @endcode
		*/
		void 		XYGstoLL	(
							int			axis,		///< [in]  該当するゾーンの番号(2～4)
							double		x,			///< [in]  北方向の座標値(m)
							double		y,			///< [in]  東方向の座標値(m)
							d_lonlat	*lonlat		///< [out]  10進で表した経緯度(ex:130.23432342....)
						);
		
		/**
		* @brief ガウス・クリューゲル座標系のX,Y(m)から経緯度へ変換
		* 
		* 2000.9.20
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.XYGstoLL(3, 5540399.35009, 3464156.57117, &lon, &lat);
		* printf("(経度,緯度)=(%0.10f,%0.10f)\n", lon, lat);
		* @endcode
		*/
		void		XYGstoLL	(
							int			axis,	///< [in]  該当するゾーンの番号(2～4)
							double		X,		///< [in]  北方向の座標値(m)
							double		Y,		///< [in]  東方向の座標値(m)
							double		*lon,	///< [out]  経度
							double		*lat	///< [out]  緯度
						);
		/**
		* @brief ベッセル楕円体の緯度経度より、WGS84の緯度経度を求める
		* 
		* 2000.10.06
		* 
		* ※値が不正確であるため、確認の上このメソッドについては削除する予定
		*（おかしい?）
		*/
		/* 2009.06.09 コメントアウト
		void		BslLLtoWGS84LL	(
							double		bsl_lon,		///< ベッセル経度
							double		bsl_lat,		///< ベッセル緯度
							double		bsl_hei,		///< ベッセル楕円体高
							double		*wgs84_lon,		///< WGS84経度
							double		*wgs84_lat,		///< WGS84緯度
							double		*wgs84_hei		///< WGS84楕円体高
						);
		*/

		/**
		* ベッセル楕円体の緯度経度より、WGS84の緯度経度を求める
		* WGS84の緯度経度をベッセル楕円体の緯度経度へ変換	（おかしい?）
		* // 2000.10.10
		*/
		/* 2009.05.21 無効なのでコメントアウト
		void		WGS84LLtoBslLL	(
							double		*wgs84_lon,		///< WGS84経度
							double		*wgs84_lat,		///< WGS84緯度
							double		bsl_lon,		///< ベッセル経度
							double		bsl_lat			///< ベッセル緯度
						);
						*/
		/**
		* @brief メッシュ + (X,Y)から経緯度へ変換(EU諸国版)
		*
		* 2001.2.28
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL_EU(60700000, 0, 0, &lon, &lat);
		* printf("(経度,緯度)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	成功
		* @retval	-1	失敗
		*/
		int			MeshtoLL_EU(
							unsigned int	meshcode,	///< [in]  メッシュコード
							int				x,			///< [in]  正規化座標X
							int				y,			///< [in]  正規化座標Y
							double			*lon,		///< [out]  経度
							double			*lat		///< [out]  緯度
						);
		
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める
		* 
		* 2002.2.28
		* @code
		* <例>
		* int		lv = 64;	// 都市地図メッシュ
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxxxx,1000000,1000000 のようになっても許容する場合
		* crd.LLtoMesh_EU(40.0, 50.0, lv, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // xxxxxxxx,1000000,1000000 は許容しない場合→xxxxxxxx,0,0と返したい場合
		* crd.LLtoMesh_EU(40.0, 50.0, lv, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	成功
		* @retval	-1	失敗
		*/
		int			LLtoMesh_EU(
							double			lon,		///< [in]  経度
							double			lat,		///< [in]  緯度
							int				level,		///< [in]  メッシュのレベル (1,2,64)
							unsigned int	*meshcode,	///< [out]  メッシュコード
							int				*x,			///< [out]  正規化X
							int				*y,			///< [out]  正規化Y
							int				no_mill=0	///< [in]  XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）2006.09.06 
						);

		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* @code
		* <例>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_EU2(40.0, 50.0, 60700000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.5f,%0.5f)\n", 60700000, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			LLtoMesh_EU2	(
							double			c_lon,			///< [in]  経度
							double			c_lat,			///< [in]  緯度
							unsigned int	c_meshcode,		///< [in]  メッシュコード
							double			*c_x,			///< [out]  正規化X
							double			*c_y			///< [out]  正規化Y
						);
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* @code
		* <例>
		* int		x = 0, y = 0;
		* crd_cnv	crd;
		* crd.LLtoMesh_EU2(40.0, 50.0, 60700000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", 60700000, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			LLtoMesh_EU2	(
							double			c_lon,			///< [in]  経度
							double			c_lat,			///< [in]  緯度
							unsigned int	c_meshcode,		///< [in]  メッシュコード
							int				*c_x,			///< [out]  正規化X
							int				*c_y			///< [out]  正規化Y
						);
		
		
		/**
		* @brief メッシュ + (X,Y)から経緯度へ変換(NA諸国版) 
		* 
		* 2002.2.28
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL_NA(75200000, 0, 0, &lon, &lat);
		* printf("(経度,緯度)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	成功
		* @retval	-1	失敗
		*/
		int			MeshtoLL_NA(
							unsigned int	meshcode,	///< [in]  メッシュコード
							int			x,			///< [in]  正規化座標X
							int			y,			///< [in]  正規化座標Y
							double		*lon,		///< [out]  経度
							double		*lat		///< [out]  緯度
						);
		
		/**
		* @brief 北アメリカ対応版 
		* 
		* 経緯度から、メッシュコード + (x,y) を求める 2001.2.28
		* @code
		* <例>
		* int		lv = 64;	// 都市地図メッシュ
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxxxx,1000000,1000000 のようになっても許容する場合
		* crd.LLtoMesh_NA(-130.0, 60.0, lv, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // xxxxxxxx,1000000,1000000 は許容しない場合→xxxxxxxx,0,0と返したい場合
		* crd.LLtoMesh_NA(-130.0, 60.0, lv, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	成功
		* @retval	-1	失敗
		*/
		int			LLtoMesh_NA(
							double			lon,		///< [in]  経度
							double			lat,		///< [in]  緯度
							int				level,		///< [in]  メッシュのレベル (1,2,64)
							unsigned int	*meshcode,	///< [out]  メッシュコード
							int				*x,			///< [out]  正規化X
							int				*y,			///< [out]  正規化Y
							int				no_mill=0	///< [in]  XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）2006.09.06 
						);

		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		* 
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* @code
		* <例>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_NA2(-130.0, 60.0, 75200000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.5f,%0.5f)\n", 75200000, x, y);
		* @endcode
		* @retval	0	成功
		* @retval	-1	失敗
		*/
		int			LLtoMesh_NA2(
							double			c_lon,			///< [in]  経度
							double			c_lat,			///< [in]  緯度
							unsigned int	c_meshcode,		///< [in]  メッシュコード
							double			*c_x,			///< [out]  正規化X
							double			*c_y			///< [out]  正規化Y
						);
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		* 
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* @code
		* <例>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_NA2(-130.0, 60.0, 75200000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", 75200000, x, y);
		* @endcode
		* @retval	0	成功
		* @retval	-1	失敗
		*/
		int			LLtoMesh_NA2(
							double			c_lon,			///< [in]  経度
							double			c_lat,			///< [in]  緯度
							unsigned int	c_meshcode,		///< [in]  メッシュコード
							int				*c_x,			///< [out]  正規化X
							int				*c_y			///< [out]  正規化Y
						);

		/**
		* @brief メッシュ + (X,Y)から経緯度へ変換(アラスカ・ハワイ版) 
		* 
		* 2005.12.22
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL_AH(75200000, 0, 0, &lon, &lat);
		* printf("(経度,緯度)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	成功
		* @retval	-1	失敗
		*/
        int			MeshtoLL_AH(
							unsigned int	meshcode,		///< [in]  メッシュコード
							int				x,				///< [in]  正規化座標X
							int				y,				///< [in]  正規化座標Y
							double			*lon,			///< [out]  経度
							double			*lat			///< [out]  緯度
						);

        /**
        * @brief 経緯度から、メッシュコード + (x,y) を求める(アラスカ・ハワイ版) 
		* 
		* 2005.12.22
		* @code
		* <例>
		* int		lv = 64;	// 都市地図メッシュ
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxxxx,1000000,1000000 のようになっても許容する場合
		* crd.LLtoMesh_AH(-160.0, 60.0, lv, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // xxxxxxxx,1000000,1000000 は許容しない場合→xxxxxxxx,0,0と返したい場合
		* crd.LLtoMesh_AH(-160.0, 60.0, lv, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval 0   成功
        * @retval -1  失敗
        */
        int			LLtoMesh_AH(
							double			lon,			///< [in]  経度
							double      	lat,			///< [in]  緯度
							int      	  	level,			///< [in]  メッシュのレベル (1,2,64)
							unsigned int	*meshcode,		///< [out]  メッシュコード
							int         	*x,				///< [out]  正規化X
							int         	*y,				///< [out]  正規化Y
							int				no_mill=0		///< [in]  XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）2006.09.06 
                        );

		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		* 
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* @code
		* <例>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_AH2(-160.0, 60.0, 75200000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.5f,%0.5f)\n", 75200000, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			LLtoMesh_AH2(
							double			c_lon,			///< [in]  経度
							double			c_lat,			///< [in]  緯度
							unsigned int	c_meshcode,		///< [in]  メッシュコード
							double			*c_x,			///< [out]  正規化X
							double			*c_y			///< [out]  正規化Y
						);
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
		*
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* @code
		* <例>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_AH2(-160.0, 60.0, 75200000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", 75200000, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		int			LLtoMesh_AH2(
							double			c_lon,			///< [in]  経度
							double			c_lat,			///< [in]  緯度
							unsigned int	c_meshcode,		///< [in]  メッシュコード
							int				*c_x,			///< [out]  正規化X
							int				*c_y			///< [out]  正規化Y
						);

		//=============================================================================
		// ここから下は新規追加メソッド 2006,09.06
		//=============================================================================

		//---------------------------------------------------------------------
		/**
		* @brief メッシュコードの桁数判定(文字列用) 
		* 
		* 2006.9.7
		* @code
		* <例>
		* int	digit = 0;	// 桁数
		* crd_cnv	crd;
		* digit	= crd.GetMeshDigit("59414130");
		* printf("メッシュ桁数=%d\n", digit);
		* @endcode
		* @return		成功 = コードの桁数 2,4,6,8
		* @return		失敗 = -1
		*/
		//---------------------------------------------------------------------
		int			GetMeshDigit	(
							char*	s_mesh			///< [in]  元メッシュコード
						);
		

		//---------------------------------------------------------------------
		/**
		* @brief メッシュ + (X,Y)から経緯度へ変換(世界共通版(日本も)) 
		*
		* メッシュのレベル指定については、世界のメッシュでは先頭が0になる場合があるので
		* どのレベルのメッシュとして扱うかを明示するために存在する。2006.09.06
		* 例）メッシュ 5941 レベル 64 → 00005941のように扱う
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* // 南アメリカ用メッシュとして扱う
		* crd.MeshtoLL_World(69600000, 0, 0, 64, crd_cnv::SA_ORIGIN, &lon, &lat);
		* printf("南アメリカ用(経度,緯度)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	成功
		* @retval	-1	失敗
		*/
		//---------------------------------------------------------------------
		int			MeshtoLL_World(
							unsigned int		c_meshcode,		///< [in]  メッシュコード
							int					c_x,			///< [in]  正規化座標X
							int					c_y,			///< [in]  正規化座標Y
							int					c_level,		///< [in]  メッシュのレベル (1,2,64)
							int					c_origin_code,	///< [in]  原点コード
							double				*c_lon,			///< [out]  経度
							double				*c_lat			///< [out]  緯度
						);

		//---------------------------------------------------------------------
		/**
        * @brief 経緯度から、メッシュコード + (x,y) を求める(世界共通版(日本も))
		* 
		* 2006.09.08
		* @code
		* <例>
		* int		lv = 64;	// 都市地図メッシュ
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxx,1000000,1000000 のようになっても許容する場合(南アメリカメッシュとして扱う)
		* crd.LLtoMesh_World(-60.0, -10.0, lv, crd_cnv::SA_ORIGIN, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // xxxxxx,1000000,1000000 は許容しない場合→xxxxxx,0,0のように返したい場合
		* crd.LLtoMesh_World(-60.0, -10.0, lv, crd_cnv::SA_ORIGIN, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
        * @retval 0   成功
        * @retval -1  失敗
		*/
		//---------------------------------------------------------------------
        int			LLtoMesh_World(
							double				c_lon,			///< [in]  経度
							double				c_lat,			///< [in]  緯度
							int					c_level,		///< [in]  メッシュのレベル (1,2,64)
							int					c_origin_code,	///< [in]  原点コード
							unsigned int		*c_meshcode,	///< [out]  メッシュコード
							int					*c_x,			///< [out]  正規化X
							int					*c_y,			///< [out]  正規化Y
							int					c_no_mill=0		///< [in]  XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）2006.09.06 
                        );
		
		//---------------------------------------------------------------------
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)(世界共通版(日本も)) 
		*
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* 
		* メッシュのレベル指定については、世界のメッシュでは先頭が0になる場合があるので
		* どのレベルのメッシュとして扱うかを明示するために存在する。
		* 例）メッシュ 333 レベル 64 → 00003333のように扱う
		* 2006.09.06
		* @code
		* <例>
		* int		lv = 64;	// 都市地図メッシュ
		* double	x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxx,1000000,1000000 のようになっても許容する場合(南アメリカメッシュとして扱う)
		* crd.LLtoMesh2_World(-60.0, -10.0, 69600000, 64, crd_cnv::SA_ORIGIN, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.10f,%0.10f)\n", 69600000, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		//---------------------------------------------------------------------
		int			LLtoMesh2_World(
							double				c_lon,			///< [in]  経度
							double				c_lat,			///< [in]  緯度
							unsigned int		c_meshcode,		///< [in]  メッシュコード
							int					c_level,		///< [in]  メッシュのレベル (1,2,64)
							int					c_origin_code,	///< [in]  原点コード
							double				*c_x,			///< [out]  正規化X
							double				*c_y			///< [out]  正規化Y
						);

		//---------------------------------------------------------------------
		/**
		* @brief 経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)(世界共通版(日本も)) 
		*
		* 指定したメッシュコードにたいして、正規化座標値を求める
		* 
		* メッシュのレベル指定については、世界のメッシュでは先頭が0になる場合があるので
		* どのレベルのメッシュとして扱うかを明示するために存在する。
		* 例）メッシュ 333 レベル 64 → 00003333のように扱う
		* 2006.09.12
		* @code
		* <例>
		* int		lv = 2;	// 2次メッシュ
		* int		x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxx,1000000,1000000 のようになっても許容する場合(南アメリカメッシュとして扱う)
		* crd.LLtoMesh2_World(-60.0, -10.0, 69600000, 64, crd_cnv::SA_ORIGIN, &x, &y);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", 69600000, x, y);
		* @endcode
		* @retval	0	成功
		* @retval -1	失敗
		*/
		//---------------------------------------------------------------------
		int			LLtoMesh2_World(
							double				c_lon,			///< [in]  経度
							double				c_lat,			///< [in]  緯度
							unsigned int		c_meshcode,		///< [in]  メッシュコード
							int					c_level,		///< [in]  メッシュのレベル (1,2,64)
							int					c_origin_code,	///< [in]  原点コード
							int					*c_x,			///< [out]  正規化X
							int					*c_y			///< [out]  正規化Y
						);

		//=============================================================================
		// ここまで 2006,09.06
		//=============================================================================

		//=======================================================================
		//	下記2つのメンバ関数、TOKYOtoJGD2000(), JGD2000toTOKYO()	は、
		//  [Bug 5755] の要望により追加 2008.01.21
		//	※【補足事項】
		//	この2つの変換関数は、飛田 幹夫氏著書、「世界測地系と座標変換」
		//  で解説されている手法を元に作成されています。
		//=======================================================================
		//---------------------------------------------------------------------
		/**
		* @brief Tokyo->JGD2000への座標変換
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0, he = 0.0;
		* crd_cnv	crd;
		* crd.TOKYOtoJGD2000(140.0, 40.0, 0.0, &lon, &lat, &he);
		* printf("世界測地系経緯度(lon,lat,he)=(%0.10f,%0.10f,%0.10f)\n", lon, lat, he);
		* @endcode
		* @return 無し
		*/
		//---------------------------------------------------------------------
		void		TOKYOtoJGD2000	(
							double	c_dBefore_Lon,	///< [in]  変換前経度
							double	c_dBefore_Lat,	///< [in]  変換前緯度
							double	c_dBefore_He,	///< [in]  変換前楕円体高(m)
							double*	c_dAfter_Lon,	///< [out]  変換後経度
							double*	c_dAfter_Lat,	///< [out]  変換元緯度
							double*	c_dAfter_He		///< [out]  変換後楕円体高(m)
						);

		//---------------------------------------------------------------------
		/**
		* @brief JGD2000->Tokyoへの座標変換
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0, he = 0.0;
		* crd_cnv	crd;
		* crd.JGD2000toTOKYO(140.0, 40.0, 0.0, &lon, &lat, &he);
		* printf("東京測地系経緯度(lon,lat,he)=(%0.10f,%0.10f,%0.10f)\n", lon, lat, he);
		* @endcode
		* @return 無し
		*/
		//---------------------------------------------------------------------
		void		JGD2000toTOKYO	(
							double	c_dBefore_Lon,	///< [in]  変換前経度
							double	c_dBefore_Lat,	///< [in]  変換前緯度
							double	c_dBefore_He,	///< [in]  変換前楕円体高(m)
							double*	c_dAfter_Lon,	///< [out]  変換後経度
							double*	c_dAfter_Lat,	///< [out]  変換元緯度
							double*	c_dAfter_He		///< [out]  変換後楕円体高(m)
						);

		//=======================================================================
		//	下記2つのメンバ関数、TOKYOtoJGD2000_RP(), JGD2000toTOKYO_RP()
		//  は、地域毎パラメータでの変換、IPC固有での変換対応箇所があるため追加。
		//	※【補足事項】
		//	この2つの変換関数は、飛田 幹夫氏著書、「世界測地系と座標変換」
		//  で解説されている手法を元に作成されています。
		//=======================================================================
		//---------------------------------------------------------------------
		/**
		* @brief 地域毎パラメータを使用したTokyo->JGD2000への座標変換
		* 
		* 使用前にJGD2000_RP_Initで地域パラメータを読み込む必要がある
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.JGD2000_RP_Init("TKY2JGD.par");	// 地域パラメータ読み込み
		* // IPCの部分的に東京測地系→Tokyo97に対応したデータの座標値変換を行う場合
		* crd.TOKYOtoJGD2000_RP(124.25, 24.5, &lon, &lat, true);
		* printf("世界測地系経緯度(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* // 一般に出回っているデータ(東京測地系）の座標値変換を行う場合
		* crd.TOKYOtoJGD2000_RP(124.25, 24.5, &lon, &lat, false);
		* printf("世界測地系経緯度(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* @endcode
		* @return	0 = 変換エラー（未初期化で使用した場合等）
		* @return	1 = 地域毎のパラメータで変換された場合3パラメータで変換された場合
		* @return	2 = 3パラメータで変換された場合
		*/
		//---------------------------------------------------------------------
		int			TOKYOtoJGD2000_RP	(
							double	c_dBefore_Lon,	///< [in]  変換前経度
							double	c_dBefore_Lat,	///< [in]  変換前緯度
							double*	c_dAfter_Lon,	///< [out]  変換後経度
							double*	c_dAfter_Lat,	///< [out]  変換元緯度
							bool	c_bIPC_Flag		///< [in]  IPC固有変換をするかどうかtrue=IPC固有変換 false=世間一般対応変換
						);

		//---------------------------------------------------------------------
		/**
		* @brief 地域毎パラメータを使用したJGD2000->Tokyoへの座標変換
		* 
		* 使用前にJGD2000_RP_Initで地域パラメータを読み込む必要がある
		* @code
		* <例>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.JGD2000_RP_Init("TKY2JGD.par");	// 地域パラメータ読み込み
		* // IPCの部分的に東京測地系→Tokyo97に対応したデータの座標値変換を行う場合
		* crd.JGD2000toTOKYO_RP(124.25040, 24.50547, &lon, &lat, true);
		* printf("東京測地系経緯度(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* // 一般に出回っているデータ(東京測地系）の座標値変換を行う場合
		* crd.JGD2000toTOKYO_RP(124.25040, 24.50547, &lon, &lat, false);
		* printf("東京測地系経緯度(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* @endcode
		* @return	0 = 変換エラー（未初期化で使用した場合等）
		* @return	1 = 地域毎のパラメータで変換された場合3パラメータで変換された場合
		* @return	2 = 3パラメータで変換された場合
		*/
		//---------------------------------------------------------------------
		int			JGD2000toTOKYO_RP	(
							double	c_dBefore_Lon,	///< [in]  変換前経度
							double	c_dBefore_Lat,	///< [in]  変換前緯度
							double*	c_dAfter_Lon,	///< [out]  変換後経度
							double*	c_dAfter_Lat,	///< [out]  変換元緯度
							bool	c_bIPC_Flag		///< [in]  IPC固有変換をするかどうかtrue=IPC固有変換 false=世間一般対応変換
						);

		//d_lonlat*	GetLLPt		(double, double, double, double);				// 2000.4.13
		//void		GetLLPt		(double, double, double, double, d_lonlat *);	// 2000.4.13
		//void		LLtoXYGs	(int, double, double, xy_coord *);			///< 経緯度からガウス座標系のXYへ変換2000.9.19
		// 2001.08.29 とりあえずテスト用
		//int			WGS84UTMtoLL	(int, double, double, double *, double *);	// 2001.08.28 UTM座標から、経緯度(WGS84)にする 
		//int			LLtoWGS84UTM	(int, double, double, double *, double *);	// 2001.08.29 UTM座標から、経緯度(WGS84)にする 
};


//	WINDOWS用define 1998.7.28 by 工藤
#ifndef _USE_MATH_DEFINES // @see math.h
#ifndef M_PI
#ifndef PI
#define		PI			3.14159265358979323846
#endif
#define	M_PI	PI
#endif
#endif

#if defined(__cplusplus)
}
#endif

#endif	// __CRD_CNV_HEADDER__
