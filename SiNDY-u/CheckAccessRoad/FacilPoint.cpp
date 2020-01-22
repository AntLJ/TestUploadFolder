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

// FacilPoint.cpp: CFacilPoint �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckAccessRoad.h"
#include "FacilPoint.h"
#include "crd_cnv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CFacilPoint::CFacilPoint():eMeshcode(0),eType(0),eLon(0),eLat(0),eKanji(""),eCode("")
{}
CFacilPoint::CFacilPoint(const CFacilPoint& cPoint) :
	eType(cPoint.eType),
	eMeshcode(cPoint.eMeshcode),
	eLon(cPoint.eLon),
	eLat(cPoint.eLat),
	eKanji(cPoint.eKanji),
	eCode(cPoint.eCode),
	eDFacilPs(cPoint.eDFacilPs),
	eDRoadPs(cPoint.eDRoadPs),
	eX(cPoint.eX),
	eY(cPoint.eY),
	eDistance(0),
	eAccessFind(0)
{}
CFacilPoint::~CFacilPoint()
{}

/**
 * �{�ݏ��|�C���g��ʂ̓o�^
 * @note �{�ݏ��|�C���g��ʂ̓o�^���s���܂��B
 * @param  cVal		[in]	�{�ݏ���ʃR�[�h�l
 * @return �Ȃ�
 */
void	CFacilPoint::setType(int cVal){
	eType=cVal;
}
/**
 * �{�ݏ��|�C���g�̐��K�����W���v�Z
 * @note �{�ݏ��|�C���g�̐��K�����W���v�Z���s���܂��B
 * @param  �Ȃ�
 * @return �Ȃ�
 */
void	CFacilPoint::setFacilPointXY(void)
{
	CMeshPos	aMP;
	aMP.LL256toLL(eLon,eLat,&eDFacilPs.eX,&eDFacilPs.eY);
	aMP.LLtoMesh(eDFacilPs.eX,	//�ܓx
				 eDFacilPs.eY,	//�o�x
				 2,				//���b�V���R�[�h���x��
				 &eMeshcode,	//���b�V���R�[�h
				 &eX, &eY);		//���K�����W
}
/**
 * �{�ݏ��|�C���g�̐��K�����W���v�Z
 * @note �{�ݏ��|�C���g�̐��K�����W���v�Z���s���܂��B
 * @param  �Ȃ�
 * @return �Ȃ�
 */
/*
void	CFacilPoint::setRoadPointXY(int aType,double cLon,double cLat)
{
	double	xmin=.0,xmax=.0,ymin=.0,ymax=.0; 
	switch( aType ){
		case 1:	//�ŋߖT���H
			eDRoadPs.eX = cLon;
			eDRoadPs.eY = cLat;

			//���n�̌������胍�W�b�N�C��	2003/10/7
			if( getFacilDY()<getRoadDY()){
				xmin=getFacilDX();	ymin=getFacilDY();
				xmax=getRoadDX();	ymax=getRoadDY();
			}
			else{
				xmin=getRoadDX();	ymin=getRoadDY();
				xmax=getFacilDX();	ymax=getFacilDY();
			}

			//�O�ڎl�p�`�̓o�^
			setCrossArea(xmin,ymin,xmax,ymax);
			break;
		case 2:	//�ŋߖT���H�̎��ɋ߂����H
			eDRoadPs2.eX = cLon;
			eDRoadPs2.eY = cLat;
			if( getFacilDX()<getRoad2DX()){	xmin=getFacilDX();	xmax=getRoad2DX();}
			else{							xmin=getRoad2DX();	xmax=getFacilDX();}
			if( getFacilDY()<getRoad2DY()){	ymin=getFacilDY();	ymax=getRoad2DY();}
			else{							ymin=getRoad2DY();	ymax=getFacilDY();}
			//�O�ڎl�p�`�̓o�^
			setCrossArea2(xmin,ymin,xmax,ymax);
			break;
		default:
			break;
	}
#ifdef	_DEBUG_
	int	aMeshcode1=0,aX1=0,aY1=0;
	int	aMeshcode2=0,aX2=0,aY2=0;
	CMeshPos	aMP;
	aMP.LLtoMesh(xmin,ymin,
				 2,				//���b�V���R�[�h���x��
				 &aMeshcode1,	//���b�V���R�[�h
				 &aX1, &aY1);		//���K�����W
	aMP.LLtoMesh(xmax,ymax,
				 2,				//���b�V���R�[�h���x��
				 &aMeshcode2,	//���b�V���R�[�h
				 &aX2, &aY2);	//���K�����W
//	ATLTRACE(TEXT("��������( %d )�F[%d,%d-%d - %d,%d-%d]\n"),aType,
//							aMeshcode1,aX1,aY1,aMeshcode2,aX2,aY2);
#endif
}
*/
/**
 * �{�ݏ��|�C���g�o�ܓx�̓o�^
 * @note �{�ݏ��|�C���g�o�ܓx�̓o�^���s���܂��B
 * @param  cVal		[in]	�{�ݏ���ʃR�[�h�l
 * @return �Ȃ�
 */
void	CFacilPoint::setLon(long cVal)
{
	eLon=cVal;
}
/**
 * �{�ݏ��|�C���g�o�ܓx�̓o�^
 * @note �{�ݏ��|�C���g�o�ܓx�̓o�^���s���܂��B
 * @param  cVal		[in]	�{�ݏ���ʃR�[�h�l
 * @return �Ȃ�
 */
void	CFacilPoint::setLat(long cVal)
{
	eLat=cVal;
}
// = ���Z�q
CFacilPoint& CFacilPoint::operator = (const CFacilPoint& cPoint)
{
        eType  = cPoint.eType;
		eLon   = cPoint.eLon;
		eLat   = cPoint.eLat;
        eKanji = cPoint.eKanji;
        eCode  = cPoint.eCode;
		eX     = cPoint.eX;
		eY     = cPoint.eY;
		eMeshcode = cPoint.eMeshcode;
        return(*this);
}

// ��r���Z�q
bool CFacilPoint::operator == (const CFacilPoint& cPoint) const
{       
        if(eKanji != cPoint.eKanji) return(false);
		if(eLon != cPoint.eLon) return(false);
		if(eLat != cPoint.eLat) return(false);
        return(eCode == cPoint.eCode);
}
bool CFacilPoint::operator < (const CFacilPoint& cPoint) const
{
        if(eLon < cPoint.eLon) return(true);
        if(eLon > cPoint.eLon) return(false);
        if(eLat < cPoint.eLat) return(true);
        if(eLat > cPoint.eLat) return(false);
        return(eKanji < cPoint.eKanji);
}
