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

// MeshPos.h: CMeshPos �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHPOS_H__29250B53_22A9_45BA_B805_54E5D5FCF33E__INCLUDED_)
#define AFX_MESHPOS_H__29250B53_22A9_45BA_B805_54E5D5FCF33E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	<crd_cnv.h>

//	�������W(x,y)�N���X
class CMapDPoint
{
public:
	CMapDPoint():eX(.0),eY(.0){}
	CMapDPoint(double cX,double cY){eX=cX;eY=cY;}

	// = ���Z�q
	CMapDPoint& operator = (const CMapDPoint& cPoint)
		{eX = cPoint.eX; eY = cPoint.eY; return( *this); }
	
	//��r���Z�q
	bool operator == (const CMapDPoint &cPoint) const{
		return(eX == cPoint.eX && eY == cPoint.eY);}
	bool operator != (const CMapDPoint &cPoint) const{
		return(eX != cPoint.eX || eY != cPoint.eY);}
	bool operator < (const CMapDPoint &cPoint) const{
		return((eX < cPoint.eX)||(eX == cPoint.eX && eY < cPoint.eY));}
	bool operator > (const CMapDPoint &cPoint) const{
		return((eX > cPoint.eX)||(eX == cPoint.eX && eY > cPoint.eY));}

	void Add(double cX,double cY){eX=cX;eY=cY;}
	// �_�Ɠ_�Ƃ̋����̌v�Z
	double		distance(const CMapDPoint& cPoint) const{
        double aDx = cPoint.eX - eX;
        double aDy = cPoint.eY - eY;
        return(sqrt(aDx * aDx + aDy * aDy));
	}

public:
	double		eX;			// �w���W
	double		eY;			// �x���W
};
// �萔��`
#define		RA			6377397.15500		///< �n���ȉ~��(�x�b�Z��)�̒����a�i���[�g���j
#define		RB			6356078.96300		///< �n���ȉ~��(�x�b�Z��)�̒Z���a�i���[�g���j
#define		EA2			0.006674372231315	///< ��ꗣ�S��
#define		EB2			0.006719218798677	///< ��񗣐S��	
#define		M0			0.9999				///< �����o���ł̏k�ڌW��
/** C1�`C6�͎q�ߐ��ǒ������߂�ۂɎg����p�����[�^ */
#define		C1			1.005037306048555	///< 
#define		C2			0.005047849240300
#define		C3			0.000010563786831
#define		C4			0.000000020633322
#define		C5			0.000000000038853
#define		C6			0.000000000000070
// �K�E�X���W�n�p
#define		G_A			1.0050373060874
#define		G_B			0.002523924637
#define		G_C			0.0000026409446
#define		G_D			0.000000003438362
// WGS84�ϊ��p
#define		DX			582.0
#define		DY			105.0
#define		DZ			414.0
#define		ALFA		-5.04206228354e-6
#define		BETA		-1.6968478839e-6
#define		GAMMA		1.4932261378e-5
#define		DELTA		8.3e-6
// WGS84�ȉ~��
#define		WGS84_RA	6378137.00000		///< �n���ȉ~��(WGS84)�̒����a�i���[�g���j2000.10.11
#define		WGS84_RB	6356752.31424		///< �n���ȉ~��(WGS84)�̒Z���a�i���[�g���j2000.10.11
#define		WGS84_EA2	0.006694379990132
#define		WGS84_EB2	0.006739496742267

// ����
#define		D_MILLION	1000000.0
#define		MILLION		1000000
#define		V_PI		3.141592653589

//��`�Ǘ��N���X
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

//���W�Ǘ��N���X
class CMeshPos
{
public:
	virtual ~CMeshPos();

	CMeshPos(void) : eMesh(0), eX(0), eY(0) { }
	CMeshPos(int cMesh) : eMesh(cMesh), eX(0), eY(0) { }
	CMeshPos(int cMesh, int cX, int cY ) : eMesh(cMesh), eX(cX), eY(cY){ }
	CMeshPos(double cLon, double cLat, int cMeshCode );
	int	LLtoMesh(	double	lon,		// �ܓx
					double	lat,		// �o�x
					int		level,		// ���b�V���̃��x�� (1,2,64)
					int		*meshcode,	// ���b�V���R�[�h
					int		*x,			// ���K���w
					int		*y);		// ���K���x
	void LL256toLL(	int		lon256,		// �o�x
					int		lat256,		// �ܓx
					double	*lon,		// �o�x
					double	*lat);		// �ܓx
	bool MeshtoLL(  int		cMeshcode,	// �Q�����b�V���R�[�h
				    int		cX,			// ���K�����WX
				    int		cY,			// ���K�����WY
				    double	*cLon,		// �o�x
				    double	*cLat);		// �ܓx
	double GetDistXY(double	eLon1,		// �o�x�P
					 double	eLat1,		// �ܓx�P
					 double	eLon2,		// �o�x�Q
					 double	eLat2,		// �ܓx�Q
					 double	*cDist_X,	// ����X(m)
					 double	*cDist_Y);	// ����Y(m)
	void GetLLPnt(double	eLon1,		// �o�x�P
				  double	eLat1,		// �ܓx�P
				  double	cDist_X,	// �����w(m)
				  double	cDist_Y,	// �����x(m)
				  double	*eLon2,		// �o�x�Q
				  double	*eLat2);	// �ܓx�Q
	double	GetDist(double	eLon1,		// �o�x�P
				    double	eLat1,		// �ܓx�P
					double	eLon2,		// �o�x�Q
					double	eLat2);		// �ܓx�Q

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
	// ������X���Ƃ̊p�x�����߂�
	double angle(const CMapDPoint& eP1, const CMapDPoint& eP2) const{
        double aX = (double)(eP2.eX - eP1.eX);
        double aY = (double)(eP2.eY - eP1.eY);
        double aDistance = sqrt(aX * aX + aY * aY);
        double aTheta = acos(aX / aDistance);
        if(aY < 0.0) aTheta = 2.0 * M_PI - aTheta;
        if(aTheta >= (2.0 * M_PI)) aTheta -= (2.0 * M_PI);
        return(aTheta);
	}
	// �_�Ɛ����Ƃ̋����̌v�Z
	double distance(const CMapDPoint& cPoint,const CMapDPoint& eP1, const CMapDPoint& eP2) const{
        // �������_
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
	int			eMesh;			// �Q�����b�V���R�[�h
	int			eX;				// ���K���w���W
	int			eY;				// ���K���x���W
	CMapDPoint	eDps;			// �o�ܓx
	int			mUnitSize;
};

#endif // !defined(AFX_MESHPOS_H__29250B53_22A9_45BA_B805_54E5D5FCF33E__INCLUDED_)
