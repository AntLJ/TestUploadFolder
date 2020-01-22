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

/** @file
 * CFacilPoint クラスのインプリメンテーション
 *
 * @author	shimano
 * @date	2003/04/09
 *
<<<<<<< FacilPoint.h
 * $Log: FacilPoint.h,v $
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * アクセス道路属性確認ファイル出力対応（Debugモード）
 *
=======
 * $Log: FacilPoint.h,v $
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * アクセス道路属性確認ファイル出力対応（Debugモード）
 *
 * Revision 1.1.1.2  2003/04/24 09:46:44  shimano
 * 新規作成
 *
>>>>>>> 1.1.1.2
 * Revision 1.0  2003/04/09 15:13:43  shimano
 * beta8対応。
 *
 */

#if !defined(AFX_FACILPOINT_H__83C7D98D_AABC_4A7B_80CD_255E1201B37A__INCLUDED_)
#define AFX_FACILPOINT_H__83C7D98D_AABC_4A7B_80CD_255E1201B37A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"MeshPos.h"
#include	"NwObject.h"

// Shape 関連
#include	"SHPWrap.h"

//施設ポイントリストの例）
//C	130211415	39643638	盤渓青少年キャンプ場	00-68-1
//C	130088575	39567661	定山渓自然の村			00-68-2
class CFacilPoint  
{
public:
	CFacilPoint();
	CFacilPoint(const CFacilPoint& cPoint);
	virtual ~CFacilPoint();

	// = 演算子
	CFacilPoint& operator = (const CFacilPoint& cPoint);

	// 比較演算子
	bool operator == (const CFacilPoint& cPoint) const;
	bool operator < (const CFacilPoint& cPoint) const;
	
	void	setType(int cVal);
	void	setLon(long cVal);
	void	setLat(long cVal);
	void	setKanji(CString cVal)	{ eKanji=cVal;			}
	void	setCode(CString cVal)	{ eCode=cVal;			}
	void	setFacilPointXY(void);
	

	int		getMeshcode(void) const		{ return(eMeshcode);	}
	int		getType(void) const			{ return(eType);		}
	long	getLon(void) const			{ return(eLon);			}
	long	getLat(void) const			{ return(eLat);			}
	CString	getKanji(void) const		{ return(eKanji);		}
	CString	getCode(void) const			{ return(eCode);		}
	double	getFacilDX(void) const		{ return(eDFacilPs.eX);	}
	double	getFacilDY(void) const		{ return(eDFacilPs.eY);	}
	double	getRoadDX(void) const		{ return(eDRoadPs.eX);	}
	double	getRoadDY(void) const		{ return(eDRoadPs.eY);	}
	int		getX(void) const			{ return(eX);			}
	int		getY(void) const			{ return(eY);			}

	CNwArea		getCheckArea(void)	{ return(eChkRect);		}
	void		setCheckArea(double cSx,double cSy,double cEx,double cEy){
							eChkRect.setSx(cSx);  eChkRect.setSy(cSy);
							eChkRect.setEx(cEx);  eChkRect.setEy(cEy);}

	void	setAccessSts(int cVal)		{ eAccessFind=cVal;		}
	int		getAccessSts(void) const	{ return(eAccessFind);	}
	void	setDistance(double cVal)	{ eDistance=cVal;		}
	double	getDistance(void) const		{ return(eDistance);	}
	CLink	&getNearLink(void)			{ return(eNearLink);	}
	CNode	&getNearNode(void)			{ return(eNearNode);	}
	//最近傍リンクの最近傍ポイント
	void		setNearestPoint(IPointPtr cPoint)	{ eNearestPoint = cPoint; }
	IPointPtr	getNearestPoint() const				{ return eNearestPoint; }
	void		setFacilPoint(IPointPtr cPoint)		{ eFacilPoint = cPoint; }
	IPointPtr	getFacilPoint() const				{ return eFacilPoint; }
	void		setCheckLine(IPolylinePtr cLine)	{ eCheckLine = cLine; }
	IPolylinePtr		getCheckLine() const		{ return eCheckLine; }

// メンバ変数
protected:
	int			eMeshcode;		//２次メッシュコード
	int			eType;			//キャンプ場(1)、オートキャンプ場(2)、ゴルフ場(3)、スキー場(4)
	long		eLon;			//経度（1/256）（長整数型）
	long		eLat;			//緯度（1/256）（長整数型）
	CString		eKanji;			//施設名称漢字
	CString		eCode;			//シーケンス
	int			eX;				//施設の真位置（1000000 x 1000000）
	int			eY;				//施設の真位置（1000000 x 1000000）
	CMapDPoint	eDFacilPs;		//施設の真位置（double）
	CMapDPoint	eDRoadPs;		//施設までの最短道路の位置（double）
	CNwArea		eChkRect;		//アクセス道路のチェックエリア座標範囲（最南西→最北東）
	CNode		eNearNode;		//施設までの最短リンク
	CLink		eNearLink;		//施設までの最短リンク
	double		eDistance;		//施設までの最短距離
	int			eAccessFind;	//アクセス道路到達ステータス
	IPointPtr	eFacilPoint;	//施設ポイント
	IPointPtr	eNearestPoint;	//アクセスポイントと最近傍道路リンクの最短ポイント(リンク上)
	IPolylinePtr	eCheckLine;	//施設と最近傍ポイントを結んだ線
};

#endif // !defined(AFX_FACILPOINT_H__83C7D98D_AABC_4A7B_80CD_255E1201B37A__INCLUDED_)
