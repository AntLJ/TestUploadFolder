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
 * CFacilPoint �N���X�̃C���v�������e�[�V����
 *
 * @author	shimano
 * @date	2003/04/09
 *
<<<<<<< FacilPoint.h
 * $Log: FacilPoint.h,v $
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * �A�N�Z�X���H�����m�F�t�@�C���o�͑Ή��iDebug���[�h�j
 *
=======
 * $Log: FacilPoint.h,v $
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * �A�N�Z�X���H�����m�F�t�@�C���o�͑Ή��iDebug���[�h�j
 *
 * Revision 1.1.1.2  2003/04/24 09:46:44  shimano
 * �V�K�쐬
 *
>>>>>>> 1.1.1.2
 * Revision 1.0  2003/04/09 15:13:43  shimano
 * beta8�Ή��B
 *
 */

#if !defined(AFX_FACILPOINT_H__83C7D98D_AABC_4A7B_80CD_255E1201B37A__INCLUDED_)
#define AFX_FACILPOINT_H__83C7D98D_AABC_4A7B_80CD_255E1201B37A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"MeshPos.h"
#include	"NwObject.h"

// Shape �֘A
#include	"SHPWrap.h"

//�{�݃|�C���g���X�g�̗�j
//C	130211415	39643638	�Ռk���N�L�����v��	00-68-1
//C	130088575	39567661	��R�k���R�̑�			00-68-2
class CFacilPoint  
{
public:
	CFacilPoint();
	CFacilPoint(const CFacilPoint& cPoint);
	virtual ~CFacilPoint();

	// = ���Z�q
	CFacilPoint& operator = (const CFacilPoint& cPoint);

	// ��r���Z�q
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
	//�ŋߖT�����N�̍ŋߖT�|�C���g
	void		setNearestPoint(IPointPtr cPoint)	{ eNearestPoint = cPoint; }
	IPointPtr	getNearestPoint() const				{ return eNearestPoint; }
	void		setFacilPoint(IPointPtr cPoint)		{ eFacilPoint = cPoint; }
	IPointPtr	getFacilPoint() const				{ return eFacilPoint; }
	void		setCheckLine(IPolylinePtr cLine)	{ eCheckLine = cLine; }
	IPolylinePtr		getCheckLine() const		{ return eCheckLine; }

// �����o�ϐ�
protected:
	int			eMeshcode;		//�Q�����b�V���R�[�h
	int			eType;			//�L�����v��(1)�A�I�[�g�L�����v��(2)�A�S���t��(3)�A�X�L�[��(4)
	long		eLon;			//�o�x�i1/256�j�i�������^�j
	long		eLat;			//�ܓx�i1/256�j�i�������^�j
	CString		eKanji;			//�{�ݖ��̊���
	CString		eCode;			//�V�[�P���X
	int			eX;				//�{�݂̐^�ʒu�i1000000 x 1000000�j
	int			eY;				//�{�݂̐^�ʒu�i1000000 x 1000000�j
	CMapDPoint	eDFacilPs;		//�{�݂̐^�ʒu�idouble�j
	CMapDPoint	eDRoadPs;		//�{�݂܂ł̍ŒZ���H�̈ʒu�idouble�j
	CNwArea		eChkRect;		//�A�N�Z�X���H�̃`�F�b�N�G���A���W�͈́i�œ쐼���Ŗk���j
	CNode		eNearNode;		//�{�݂܂ł̍ŒZ�����N
	CLink		eNearLink;		//�{�݂܂ł̍ŒZ�����N
	double		eDistance;		//�{�݂܂ł̍ŒZ����
	int			eAccessFind;	//�A�N�Z�X���H���B�X�e�[�^�X
	IPointPtr	eFacilPoint;	//�{�݃|�C���g
	IPointPtr	eNearestPoint;	//�A�N�Z�X�|�C���g�ƍŋߖT���H�����N�̍ŒZ�|�C���g(�����N��)
	IPolylinePtr	eCheckLine;	//�{�݂ƍŋߖT�|�C���g�����񂾐�
};

#endif // !defined(AFX_FACILPOINT_H__83C7D98D_AABC_4A7B_80CD_255E1201B37A__INCLUDED_)
