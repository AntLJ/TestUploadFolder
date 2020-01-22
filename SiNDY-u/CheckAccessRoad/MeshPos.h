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

// MeshPos.h: CMeshPos クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHPOS_H__29250B53_22A9_45BA_B805_54E5D5FCF33E__INCLUDED_)
#define AFX_MESHPOS_H__29250B53_22A9_45BA_B805_54E5D5FCF33E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	<crd_cnv.h>

//	実数座標(x,y)クラス
class CMapDPoint
{
public:
	CMapDPoint():eX(.0),eY(.0){}
	CMapDPoint(double cX,double cY){eX=cX;eY=cY;}

	// = 演算子
	CMapDPoint& operator = (const CMapDPoint& cPoint)
		{eX = cPoint.eX; eY = cPoint.eY; return( *this); }
	
	//比較演算子
	bool operator == (const CMapDPoint &cPoint) const{
		return(eX == cPoint.eX && eY == cPoint.eY);}
	bool operator != (const CMapDPoint &cPoint) const{
		return(eX != cPoint.eX || eY != cPoint.eY);}
	bool operator < (const CMapDPoint &cPoint) const{
		return((eX < cPoint.eX)||(eX == cPoint.eX && eY < cPoint.eY));}
	bool operator > (const CMapDPoint &cPoint) const{
		return((eX > cPoint.eX)||(eX == cPoint.eX && eY > cPoint.eY));}

	void Add(double cX,double cY){eX=cX;eY=cY;}
	// 点と点との距離の計算
	double		distance(const CMapDPoint& cPoint) const{
        double aDx = cPoint.eX - eX;
        double aDy = cPoint.eY - eY;
        return(sqrt(aDx * aDx + aDy * aDy));
	}

public:
	double		eX;			// Ｘ座標
	double		eY;			// Ｙ座標
};
// 定数定義
#define		RA			6377397.15500		///< 地球楕円体(ベッセル)の長半径（メートル）
#define		RB			6356078.96300		///< 地球楕円体(ベッセル)の短半径（メートル）
#define		EA2			0.006674372231315	///< 第一離心率
#define		EB2			0.006719218798677	///< 第二離心率	
#define		M0			0.9999				///< 中央経線での縮尺係数
/** C1～C6は子午線孤長を求める際に使われるパラメータ */
#define		C1			1.005037306048555	///< 
#define		C2			0.005047849240300
#define		C3			0.000010563786831
#define		C4			0.000000020633322
#define		C5			0.000000000038853
#define		C6			0.000000000000070
// ガウス座標系用
#define		G_A			1.0050373060874
#define		G_B			0.002523924637
#define		G_C			0.0000026409446
#define		G_D			0.000000003438362
// WGS84変換用
#define		DX			582.0
#define		DY			105.0
#define		DZ			414.0
#define		ALFA		-5.04206228354e-6
#define		BETA		-1.6968478839e-6
#define		GAMMA		1.4932261378e-5
#define		DELTA		8.3e-6
// WGS84楕円体
#define		WGS84_RA	6378137.00000		///< 地球楕円体(WGS84)の長半径（メートル）2000.10.11
#define		WGS84_RB	6356752.31424		///< 地球楕円体(WGS84)の短半径（メートル）2000.10.11
#define		WGS84_EA2	0.006694379990132
#define		WGS84_EB2	0.006739496742267

// 共通
#define		D_MILLION	1000000.0
#define		MILLION		1000000
#define		V_PI		3.141592653589

//矩形管理クラス
class CNwArea{
public:
		CNwArea(){xmin=xmax=ymin=ymax=.0;}
		CNwArea(double xmin,double ymin,double xmax,double ymax){
			if( xmin < xmax ){ setSx(xmin); setEx(xmax);}
			else{			   setSx(xmax); setEx(xmin);}
			if( ymin < ymax ){ setSy(ymin); setEy(ymax);}
			else{			   setSy(ymax); setEy(ymin);}
		}
		virtual ~CNwArea(){}

		void	setSx(double cVal){ xmin=cVal;		}
		void	setSy(double cVal){ ymin=cVal;		}
		void	setEx(double cVal){ xmax=cVal;		}
		void	setEy(double cVal){ ymax=cVal;		}
		double	getSx(void)		  { return xmin;	}
		double	getSy(void)		  { return ymin;	}
		double	getEx(void)		  { return xmax;	}
		double	getEy(void)		  { return ymax;	}

private:
		double	xmin;
		double	xmax;
		double	ymin;
		double	ymax;
};

//座標管理クラス
class CMeshPos
{
public:
	virtual ~CMeshPos();

	CMeshPos(void) : eMesh(0), eX(0), eY(0) { }
	CMeshPos(int cMesh) : eMesh(cMesh), eX(0), eY(0) { }
	CMeshPos(int cMesh, int cX, int cY ) : eMesh(cMesh), eX(cX), eY(cY){ }
	CMeshPos(double cLon, double cLat, int cMeshCode );
	int	LLtoMesh(	double	lon,		// 緯度
					double	lat,		// 経度
					int		level,		// メッシュのレベル (1,2,64)
					int		*meshcode,	// メッシュコード
					int		*x,			// 正規化Ｘ
					int		*y);		// 正規化Ｙ
	void LL256toLL(	int		lon256,		// 経度
					int		lat256,		// 緯度
					double	*lon,		// 経度
					double	*lat);		// 緯度
	bool MeshtoLL(  int		cMeshcode,	// ２次メッシュコード
				    int		cX,			// 正規化座標X
				    int		cY,			// 正規化座標Y
				    double	*cLon,		// 経度
				    double	*cLat);		// 緯度
	double GetDistXY(double	eLon1,		// 経度１
					 double	eLat1,		// 緯度１
					 double	eLon2,		// 経度２
					 double	eLat2,		// 緯度２
					 double	*cDist_X,	// 距離X(m)
					 double	*cDist_Y);	// 距離Y(m)
	void GetLLPnt(double	eLon1,		// 経度１
				  double	eLat1,		// 緯度１
				  double	cDist_X,	// 距離Ｘ(m)
				  double	cDist_Y,	// 距離Ｙ(m)
				  double	*eLon2,		// 経度２
				  double	*eLat2);	// 緯度２
	double	GetDist(double	eLon1,		// 経度１
				    double	eLat1,		// 緯度１
					double	eLon2,		// 経度２
					double	eLat2);		// 緯度２

	bool		SetLeftDownPos(int cMeshcode,int cX, int cY,int cDist);
	bool		SetRightTopPos(int cMeshcode,int cX, int cY,int cDist);
	CMapDPoint	GetMeshCorner( int cMeshcode );
	int			GetNextMeshCode(int meshcode,int xdist,int ydist);
	int			GetMeshcode(void)			{ return eMesh;				}
	void		setUnitSize(int cUnitSize)  { mUnitSize = cUnitSize;	}
	int			getUnitSize(void)           { return mUnitSize;			}
	int			getX(void)					{ return eX;				}
	int			getY(void)					{ return eY;				}
	void		setX(int cVal)				{ eX = cVal;				}
	void		setY(int cVal)				{ eY = cVal;				}

	void		setLon(double cVal)			{ eDps.eX = cVal;	}
	void		setLat(double cVal)			{ eDps.eY = cVal;	}
	double		getLon(void)				{ return eDps.eX;	}
	double		getLat(void)				{ return eDps.eY;	}
	// 線分のX軸との角度を求める
	double angle(const CMapDPoint& eP1, const CMapDPoint& eP2) const{
        double aX = (double)(eP2.eX - eP1.eX);
        double aY = (double)(eP2.eY - eP1.eY);
        double aDistance = sqrt(aX * aX + aY * aY);
        double aTheta = acos(aX / aDistance);
        if(aY < 0.0) aTheta = 2.0 * M_PI - aTheta;
        if(aTheta >= (2.0 * M_PI)) aTheta -= (2.0 * M_PI);
        return(aTheta);
	}
	// 点と線分との距離の計算
	double distance(const CMapDPoint& cPoint,const CMapDPoint& eP1, const CMapDPoint& eP2) const{
        // 線分が点
        if( eP1==eP2 )
			return(cPoint.distance(eP1));
 
        double aTheta = 2.0 * M_PI - angle(eP1,eP2);
        double aSinValue = sin(aTheta);
        double aCosValue = cos(aTheta);
 
        double aEdgeX = (double)(cPoint.eX - eP1.eX);
        double aEdgeY = (double)(cPoint.eY - eP1.eY);
        double aValue0 = aEdgeX * aCosValue - aEdgeY * aSinValue;
        if(aValue0 < 0.0) return(cPoint.distance(eP1));
        double aDistance = fabs(aEdgeX * aSinValue + aEdgeY * aCosValue);
 
        aEdgeX = (double)(eP2.eX - eP1.eX);
        aEdgeY = (double)(eP2.eY - eP1.eY);
        double aValue1 = aEdgeX * aCosValue - aEdgeY * aSinValue;
        if(aValue0 > aValue1) return(cPoint.distance(eP2));
 
        return(aDistance);
	}


private:
	int			eMesh;			// ２次メッシュコード
	int			eX;				// 正規化Ｘ座標
	int			eY;				// 正規化Ｙ座標
	CMapDPoint	eDps;			// 経緯度
	int			mUnitSize;
};

#endif // !defined(AFX_MESHPOS_H__29250B53_22A9_45BA_B805_54E5D5FCF33E__INCLUDED_)
