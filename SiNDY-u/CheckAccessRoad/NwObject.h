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

// NwObject.h: CNwObject クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NWOBJECT_H__F7BD7C25_BCB2_4275_877B_C0C3B42E4194__INCLUDED_)
#define AFX_NWOBJECT_H__F7BD7C25_BCB2_4275_877B_C0C3B42E4194__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNwObject  
{
public:
	CNwObject();
	virtual ~CNwObject();

};
//----------------------------------------------------------------------------
//	フィーチャー管理クラス
//----------------------------------------------------------------------------
class CFeatureCopy
{
public:
	CFeatureCopy(void) : eOID(0),ipFeature(NULL),ipGeom(NULL){}
	virtual ~CFeatureCopy(){}

	void	setOID(long cVal)		{	eOID=cVal;				}
	long	getOID(void)			{	return(eOID);			}

	IFeaturePtr	 getFeature(void)	{ 	return(ipFeature);		}
	IGeometryPtr getGeometry(void)	{	return(ipGeom);			}
	void		 setFeature(IFeaturePtr	cPtr){ipFeature=cPtr;	}
	void		 setGeometry(IGeometryPtr cPtr){ipGeom=cPtr;	}

// メンバー
protected:
	IFeaturePtr		ipFeature;	//属性情報
	IGeometryPtr	ipGeom;		//形状情報
	long			eOID;		//OBJECTID
};
//----------------------------------------------------------------------------
//	道路リンク管理クラス
//----------------------------------------------------------------------------
class CLink : public CFeatureCopy
{
public:
	CLink(void) : ePos(0),eRouteInfID(0),eDouTypeStr(""),
				  eDouWidthStr(""),eSNID(0),eENID(0),eClip(false){}
	virtual ~CLink(){}

	void	setPos(int cX, int cY)	{ePos.setX(cX); ePos.setY(cY);	}
	int		getNodeX(void)			{return(ePos.getX());		}
	int		getNodeY(void)			{return(ePos.getY());		}
	void	setDouTypeStr(LPCTSTR cVal){eDouTypeStr=cVal;		}
	void	setDouWidthStr(LPCTSTR cVal){eDouWidthStr=cVal;		}
	CString	getDouTypeStr(void)		{return(eDouTypeStr);		}
	CString	getDouWidthStr(void)	{return(eDouWidthStr);		}
	int		getRouteInfID(void)		{return(eRouteInfID);		}
	void	setRouteInfID(int cVal)	{eRouteInfID = cVal;		}
	void	setClip(void)			{eClip=true;				}
	bool	getClip(void)			{return(eClip);				}
	void	setSNID(long cVal)		{eSNID=cVal;				}
	void	setENID(long cVal)		{eENID=cVal;				}
	long	getSNID(void)			{return(eSNID);				}
	long	getENID(void)			{return(eENID);				}

// メンバー
protected:
	CMeshPos		ePos;			//座標
	CString			eDouTypeStr;	//道路種別
	CString			eDouWidthStr;	//道路幅員
	long			eSNID;			//FROM_NODE_ID
	long			eENID;			//TO_NODE_ID
	int				eRouteInfID;	//アクセス道路リンク列の情報ID
	bool			eClip;			//追跡済みフラグ
};
//----------------------------------------------------------------------------
//	道路リンク管理クラス
//----------------------------------------------------------------------------
class CNode : public CFeatureCopy
{
public:
	CNode(void) : ePos(0){}
	virtual ~CNode(){}
	void	setPos(int cX, int cY)	{ePos.setX(cX); ePos.setY(cY);	}
	int		getNodeX(void)			{return(ePos.getX());		}
	int		getNodeY(void)			{return(ePos.getY());		}

// メンバー
protected:
	CMeshPos		ePos;		//座標
};
//----------------------------------------------------------------------------
//	リンクキュードキュメント管理クラス
//----------------------------------------------------------------------------
class CLQue
{
public:
	CLQue(void) : eLinkID(0), eInfID(0), eSNID(0), eENID(0),
					  eLinkDir_c(0), eSequence(0), eLastLink_f(0), eOneway_f(0), eByway_f(0), eBackRoad_f(0){}
	CLQue(long cLinkID, int cDir, int cSeq, int Last_f) :
				eLinkID(cLinkID), eLinkDir_c(cDir),	eSequence(cSeq), eLastLink_f(Last_f) {}
	virtual ~CLQue(){}

	long	GetLinkID(void)			{return(eLinkID);		}
	long	GetSNID(void)			{return(eSNID);			}
	long	GetENID(void)			{return(eENID);			}
	int		GetInfID(void)			{return(eInfID);		}
	short	GetLinkDir(void)		{return(eLinkDir_c);	}
	short	GetSequence(void)		{return(eSequence);		}
	short	GetLastLink_f(void)		{return(eLastLink_f);	}
	short	GetOneway_f(void)		{return(eOneway_f);		}

	void	SetLinkID(long eVal)	{eLinkID=eVal;			}
	void	SetInfID(int eVal)		{eInfID=eVal;			}
	void	SetLinkDir(short eVal)	{eLinkDir_c=eVal;		}
	void	SetSequence(short eVal)	{eSequence=eVal;		}
	void	SetLastLink_f(short eVal){eLastLink_f=eVal;		}
	void	SetSNID(long eVal)		{eSNID=eVal;			}
	void	SetENID(long eVal)		{eENID=eVal;			}
	void	SetOneway_f(int eVal)	{
		if(!eVal){ eOneway_f=0;	}
		else{      eOneway_f++;	}	//１：一方通行、２：双方向
	}
	void	SetSPos(long cX,long cY){eSpos.setX(cX);eSpos.setY(cY);}
	void	SetEPos(long cX,long cY){eEpos.setX(cX);eEpos.setY(cY);}
	long	GetSPosX(void)			{return( eSpos.getX());	}
	long	GetSPosY(void)			{return( eSpos.getY());	}
	long	GetEPosX(void)			{return( eEpos.getX());	}
	long	GetEPosY(void)			{return( eEpos.getY());	}

	short	GetByway_f(void)		{return(eByway_f);		}
	short	GetBackRoad_f(void)		{return(eBackRoad_f);	}
	void	SetByway_f(short eVal)	{eByway_f=eVal;			}
	void	SetBackRoad_f(short eVal){eBackRoad_f=eVal;		}

// メンバ変数
protected:
	long		eLinkID;
	int			eInfID;
	short		eLinkDir_c;
	short		eSequence;
	short		eLastLink_f;
	long		eSNID;
	long		eENID;
	CMeshPos	eSpos;			//始点ノード座標
	CMeshPos	eEpos;			//終点ノード座標

	//以下INF_～情報
	short		eByway_f;		//ぬけみちフラグ
	short		eBackRoad_f;	//うらみちフラグ
	short		eOneway_f;		//一方通行フラグ
};
#endif // !defined(AFX_NWOBJECT_H__F7BD7C25_BCB2_4275_877B_C0C3B42E4194__INCLUDED_)
