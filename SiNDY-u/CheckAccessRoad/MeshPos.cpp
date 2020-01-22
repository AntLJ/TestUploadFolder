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

// MeshPos.cpp: CMeshPos �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshPos.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
CMeshPos::~CMeshPos()
{
}
//---------------------------------------------------------------------
//	�Q�����b�V���R�[�h�i�w:�x�j����A�w�苗���ɂ���|�C���g�̌o�ܓx���擾
//---------------------------------------------------------------------
bool CMeshPos::SetLeftDownPos(int cMeshcode,int cX, int cY,int cDist)
{
	int	aMeshcode = cMeshcode;
	int	aMoveX	= cX - cDist;						//�œ쐼�|�C���g���擾
	int	aMoveY	= cY - cDist;
	double aLon=.0, aLat =.0;

	eX = aMoveX;
	eY = aMoveY;
	MeshtoLL( cMeshcode, aMoveX, aMoveY, &aLon, &aLat);

	//�ړ����W�擾
	if(( aMoveX < 0 )&&( aMoveY < 0 )){		
		eMesh = GetNextMeshCode(cMeshcode,-1,-1);	//�אڃ��b�V���R�[�h�{���W�擾
		eX    = MILLION - abs(aMoveX);
		eY    = MILLION - abs(aMoveY);
		MeshtoLL( eMesh, eX, eY, &aLon, &aLat);
	}
	else if( aMoveX < 0 ){		
		eMesh = GetNextMeshCode(cMeshcode,-1,0);	//�אڃ��b�V���R�[�h�{���W�擾
		eX    = MILLION - abs(aMoveX);
		MeshtoLL( eMesh, eX, aMoveY, &aLon, &aLat);
	}
	else if( aMoveY < 0 ){		
		eMesh = GetNextMeshCode(cMeshcode,0,-1);	//�אڃ��b�V���R�[�h�{���W�擾
		eY    = MILLION - abs(aMoveY);
		MeshtoLL( eMesh, aMoveX, eY, &aLon, &aLat);
	}
	return true;
}
//---------------------------------------------------------------------
//	�Q�����b�V���R�[�h�i�w:�x�j����A�w�苗���ɂ���|�C���g�̌o�ܓx���擾
//---------------------------------------------------------------------
bool CMeshPos::SetRightTopPos(int cMeshcode, int cX, int cY,int cDist)
{
	int	aMeshcode = cMeshcode;
	int	aMoveX = cX + cDist;						//�Ŗk���|�C���g���擾
	int	aMoveY = cY + cDist;
	double aLon=.0, aLat =.0;

	eX = aMoveX;
	eY = aMoveY;
	MeshtoLL( cMeshcode, aMoveX, aMoveY, &aLon, &aLat);

	//�ړ����W�擾
	if(( aMoveX > MILLION )&&( aMoveY > MILLION )){		
		eMesh = GetNextMeshCode(cMeshcode,1,1);		//�אڃ��b�V���R�[�h�{���W�擾
		eX    = aMoveX-MILLION;
		eY    = aMoveY-MILLION;
		MeshtoLL( eMesh, eX, eY, &aLon, &aLat);
	}
	else if( aMoveX > MILLION ){
		eMesh = GetNextMeshCode(cMeshcode,1,0);		//�אڃ��b�V���R�[�h�{���W�擾
		eX    = aMoveX-MILLION;
		MeshtoLL( eMesh, eX, aMoveY, &aLon, &aLat);
	}
	else if( aMoveY > MILLION ){
		eMesh = GetNextMeshCode(cMeshcode,0,1);		//�אڃ��b�V���R�[�h�{���W�擾
		eY    = aMoveY-MILLION;
		MeshtoLL( eMesh, aMoveX, eY, &aLon, &aLat);
	}
	return true;
}
//---------------------------------------------------------------------
//	���b�V�� + (X,Y)����ܓx�o�x�֕ϊ�
//	@retval	0	����
//	@retval -1	���s
//---------------------------------------------------------------------
bool CMeshPos::MeshtoLL( int		cMeshcode,	// �Q�����b�V���R�[�h
					     int		cX,			// ���K�����WX
					     int		cY,			// ���K�����WY
					     double		*cLon,		// �o�x
					     double		*cLat)		// �ܓx
{
	int		atai[6];
	double	wid1	= 1.0,	hei1	= 2.0 / 3.0;
	double	wid2	= 0.0;
	double	wid64	= 0.0;
	double	hei2	= 0.0;
	double	hei64	= 0.0;

	wid2	= 7.0 / 60.0 + 30.0 / 3600.0; 
	hei2	= 5.0 / 60.0;
	wid64	= wid2 / 8.0;
	hei64	= hei2 / 8.0;

	for(int i=0; i<6; i++){
		atai[i]	= 0;
	}
	// �Q�����b�V���R�[�h
	for(int i=0; i<2; i++){
		atai[i]	   = cMeshcode % 10;
		cMeshcode /= 10;
	}
	atai[2]	= cMeshcode % 100;
	atai[3]	= cMeshcode / 100;
	*cLat	= atai[3] / 1.5   + hei2 * atai[1] + cY * hei2 / D_MILLION;
	*cLon	= atai[2] + 100.0 + wid2 * atai[0] + cX * wid2 / D_MILLION;

	setLon(*cLon);
	setLat(*cLat);

	return true;
}
//---------------------------------------------------------------------
//	�ܓx�o�x����A���b�V���R�[�h + (x,y) �����߂�
//	@retval	0	����
//	@retval -1	���s
//---------------------------------------------------------------------
int	CMeshPos::LLtoMesh	(	double		lon,		// �ܓx
							double		lat,		// �o�x
							int			level,		// ���b�V���̃��x�� (1,2,64)
							int			*meshcode,	// ���b�V���R�[�h
							int			*x,			// ���K���w
							int			*y)			// ���K���x
{
	double	wid1		= 1.0,	hei1		= 2.0 / 3.0;
	double	wid2		= 0.0,	hei2		= 0.0;
	double	wid64		= 0.0,	hei64		= 0.0;
	double	lat_amari1	= 0.0,	lon_amari1	= 0.0;
	double	lat_amari2	= 0.0,	lon_amari2	= 0.0;
	double	lat_amari3	= 0.0,	lon_amari3	= 0.0;
	double	x_step_1	= 0.0,	y_step_1	= 0.0;
	double	x_step_2	= 0.0,	y_step_2	= 0.0;
	double	x_step_3	= 0.0,	y_step_3	= 0.0;
	int		up_1st		= 0,	low_1st		= 0;
	int		up_2nd		= 0,	low_2nd		= 0;
	int		up_64		= 0,	low_64		= 0;

	x_step_1	= wid1 / D_MILLION;
	y_step_1	= hei1 / D_MILLION;

	wid2		= 7.0 * 1.0 / 60.0 + 30.0 * 1.0 / 3600.0; 
	hei2		= 5.0 * 1.0 / 60.0;

	x_step_2	= wid2 / D_MILLION;
	y_step_2	= hei2 / D_MILLION;

	wid64		= wid2 / 8.0;
	hei64		= hei2 / 8.0;

	x_step_3	= wid64 / D_MILLION;
	y_step_3	= hei64 / D_MILLION;
	
	up_1st		= (int)(lat * 1.5);
	low_1st		= (int)(lon - 100.0);

	lat_amari1	= lat - (up_1st / 1.5);
	lon_amari1	= lon - (100.0 + low_1st);

	switch(level)
	{
		case	1:
			*meshcode	= up_1st * 100 + low_1st;
			*x			= (int)(lon_amari1 / x_step_1 + 0.5);
			*y			= (int)(lat_amari1 / y_step_1 + 0.5);
			return(0);
		case	2:
			up_2nd		= (int)(lat_amari1 / hei2);
			low_2nd		= (int)(lon_amari1 / wid2);
			lat_amari2	= lat_amari1 - up_2nd * hei2;
			lon_amari2	= lon_amari1 - low_2nd * wid2;
			*meshcode	= up_1st * 10000 + low_1st * 100 +
							up_2nd * 10 + low_2nd;
			*x			= (int)(lon_amari2 / x_step_2 + 0.5);
			*y			= (int)(lat_amari2 / y_step_2 + 0.5);
			return(0);
		case	64:
			up_2nd		= (int)(lat_amari1 / hei2);
			low_2nd		= (int)(lon_amari1 / wid2);
			lat_amari2	= lat_amari1 - up_2nd * hei2;
			lon_amari2	= lon_amari1 - low_2nd * wid2;
			up_64		= (int)(lat_amari2 / hei64);
			low_64		= (int)(lon_amari2 / wid64);
			lat_amari3	= lat_amari2 - up_64 * hei64;
			lon_amari3	= lon_amari2 - low_64 * wid64;
			*meshcode	= up_1st * MILLION + low_1st * 10000 +
							up_2nd * 1000 + low_2nd * 100 +
							up_64 * 10 + low_64;
			*x			= (int)(lon_amari3 / x_step_3 + 0.5);
			*y			= (int)(lat_amari3 / y_step_3 + 0.5);
			return(0);
		default:
			break;
	}
	return(-1);
}
//---------------------------------------------------------------------
//	1/256�b�P�ʂ̈ܓx�o�x����A�ʏ�̈ܓx�o�x�֕ϊ�
//---------------------------------------------------------------------
void	CMeshPos::LL256toLL	(	int		lon256,		// �o�x
								int		lat256,		// �ܓx
								double	*lon,		// �o�x
								double	*lat)		// �ܓx
{
	*lat	= (lat256 / 3600.0 / 256.0);
	*lon	= (lon256 / 3600.0 / 256.0);
}
//---------------------------------------------------------------------
// �o�ܓx
//---------------------------------------------------------------------
CMeshPos::CMeshPos(double cLon, double cLat, int cMeshCode )
{
	eMesh = cMeshCode;
	mUnitSize  = 1000000;
	CMapDPoint	aCorner = GetMeshCorner( cMeshCode );

	if( ( cMeshCode / 10000000 ) != 0 ){		// CityMap
		eX = (int)((cLon -aCorner.eX) / ((7.5/60.0)/8.0) * (double)getUnitSize());
		eY = (int)((cLat -aCorner.eY) / ((5.0/60.0)/8.0) * (double)getUnitSize());
	}
	else if( ( cMeshCode / 100000 ) != 0 ){		// Base
		eX = (int)((cLon -aCorner.eX) / (7.5/60.0) * (double)getUnitSize());
		eY = (int)((cLat -aCorner.eY) / (5.0/60.0) * (double)getUnitSize());
	}
	else if( ( cMeshCode / 1000 ) != 0 ){		// MIDDLE
		eX = (int)((cLon -aCorner.eX) / (120.0/60.0) * (double)getUnitSize());
		eY = (int)((cLat -aCorner.eY) / (80.0/60.0) * (double)getUnitSize());
	}
	else {	// Top
		eX = (int)((cLon -aCorner.eX) / (1920.0/60.0) * (double)getUnitSize());
		eY = (int)((cLat -aCorner.eY) / (1280.0/60.0) * (double)getUnitSize());
	}
}
//---------------------------------------------------------------------
//	�Q�����b�V�����̓쐼�̌o�ܓx�����߂�
//---------------------------------------------------------------------
CMapDPoint	CMeshPos::GetMeshCorner( int cMeshcode )
{
	CMapDPoint	aCorner;

	int	aBaseMesh = cMeshcode / 100;		// Base
	int aCity = cMeshcode % 100;			// City RowCol
	int	aWork1,aWork2,aWork3,aWork4;

	if( ( cMeshcode / 10000000 ) != 0 ){	// CityMap
		int	aBaseMesh = cMeshcode / 100;	// Base
		int aCity = cMeshcode % 100;		// City RowCol
		aWork1 = aBaseMesh / 10000;
		aWork2 = (aBaseMesh / 100) % 100;
		aWork3 = (aBaseMesh / 10) % 10;
		aWork4 = aBaseMesh % 10;
		aCorner.eX = (double) (aWork2+100) + (double) aWork4 / 8.0;
		aCorner.eY = (double) (8*aWork1+aWork3) / 12.0;
		aCorner.eX += (double) (((7.5/60.0)/8.0)*(aCity%10));
		aCorner.eY += (double) (((5/60.0)/8.0)*(aCity/10));

	}
	else if( ( cMeshcode / 100000 ) != 0 ){	// Base
		aWork1 = cMeshcode / 10000;
		aWork2 = (cMeshcode / 100) % 100;
		aWork3 = (cMeshcode / 10) % 10;
		aWork4 = cMeshcode % 10;
		aCorner.eX = (double) (aWork2+100) + (double) aWork4 / 8.0;
		aCorner.eY = (double) (8*aWork1+aWork3) / 12.0;
	}
	else if( ( cMeshcode / 1000 ) != 0 ){	// MIDDLE
		aWork1 = cMeshcode / 100;
		aWork2 = cMeshcode % 100;
		aCorner.eX = (double) (aWork2+100) ;
		aCorner.eY = (double) (8*aWork1) / 12.0;
	}
	else{									// Top
		switch (cMeshcode){
			case 0:							// 00
				aCorner.eX = 104.0;
				aCorner.eY = 14.0;
				break;
			case 1:							// 01
				aCorner.eX = 136.0;
				aCorner.eY = 14.0;
				break;
			case 10:						// 10
				aCorner.eX = 104.0;
				aCorner.eY = 35.333333333333;
				break;
			case 11:						// 11
				aCorner.eX = 136.0;
				aCorner.eY = 35.333333333333;
				break;
		}
	}
	return aCorner;
}
//---------------------------------------------------------------------
//	���_���b�V������(x,y)���ꂽ���̃��b�V���R�[�h���擾
//	@return		�����F���b�V���R�[�h
//	@return		���s�F -1
// 		+-----+-----+-----+
// 		|-1,0 | 0,1 | 1,1 |
// 		+-----+-----+-----+
// 		|-1,0 | 0,0 | 1,0 |
// 		+-----+-----+-----+
// 		|-1,-1| 0,-1| 1,-1|
// 		+-----+-----+-----+ ..(X,Y)�̓��͈����d�l
//---------------------------------------------------------------------
int CMeshPos::GetNextMeshCode(int meshcode,	//����b�V���R�[�h
							  int xdist,	// ���_���b�V������X�����։����b�V������Ă��邩
							  int ydist)	// ���_���b�V������Y�����։����b�V������Ă��邩
{
	#define		MILLION			1000000
	int	meshunit	= 0;
	int	roop_count	= 0;
	int	num1[6]		= {0,0,0,0,0,0};
	int	dist[2]		= {0,0};
	int	retmesh		= 0;
	int	tmp1		= 0;
	int	tmp2		= 0;

	dist[0]			= xdist;
	dist[1]			= ydist;
	int mc			= meshcode;
	int	digit		= 6;	// ���b�V���R�[�h�̌���

	switch( digit ){
		case 4:	meshunit=1; roop_count=0; break;	// �P�����b�V��
		case 6: meshunit=2;	roop_count=2; break;	// �Q�����b�V��
		case 8: meshunit=64;roop_count=4; break;	// �W�����b�V��
		default: return(-1);
	}
	// �����΂�΂�ɂ���(59 41 4 1 7 7 ������)
	for(int i = 0; i < roop_count; i++){
		num1[i]	= mc % 10;
		mc /= 10;
	}
	num1[roop_count]	= mc % 100;
	num1[roop_count+1]	= mc / 100;
	int	j = 0;
	if(meshunit == 64) {			// j = 0 ... ���E�����ɗ��ꂽ���b�V���R�[�h					
		for(j = 0; j < 2; j++) {	// j = 1 ... �㉺�����ɗ��ꂽ���b�V���R�[�h
			num1[j]	+= dist[j];
			if(num1[j] > 7) {
				num1[j+2] += num1[j] / 8;			// ���̑���グ
				num1[j]	  %= 8;
				if(num1[j+2] > 7) {
					num1[j+4] += num1[j+2] / 8;		// ���̑���グ
					num1[j+2] %= 8;
				}
			} else
			if(num1[j] < 0) {
				num1[j]		-= 7;
				num1[j+2]	-= (-num1[j] / 8);		// ���̌J����
				num1[j]		=  7 - (-num1[j] % 8);
				if(num1[j+2] < 0) {
					num1[j+2]-= 7;
					num1[j+4]-= (-num1[j+2] / 8);	// ���̌J����
					num1[j+2]= 7 - (-num1[j+2] % 8);
				}
			}
		}
		retmesh	= num1[5] * MILLION	+ num1[4] * 10000 + 
				  num1[3] * 1000+ num1[2] * 100 +
			      num1[1] * 10	+ num1[0];
	} else	
	// �Q�����b�V���p
	if(meshunit == 2) {				// j = 0 ... ���E�����ɗ��ꂽ���b�V���R�[�h,		
		for(j = 0; j < 2; j++) {	// j = 1 ... �㉺�����ɗ��ꂽ���b�V���R�[�h	
			num1[j] += dist[j];
			if(num1[j] > 7) {
				num1[j+2]	+= num1[j] / 8;		// ���̌J�グ
				num1[j]		%= 8;
			} else
			if(num1[j] < 0) {
				num1[j]		-= 7;
				num1[j+2]	-= (-num1[j] / 8);	// ���̌J����
				num1[j]		=  7 - (-num1[j] % 8);
			}
		}
		retmesh	= num1[3] * 10000 + num1[2] * 100 +
					num1[1] * 10  + num1[0];
	} else
	if(meshunit == 1) { // �P�����b�V���̏ꍇ
		// ���E�ɗ��ꂽ���b�V���R�[�h
		num1[0] += dist[0];
		num1[1] += dist[1];
		retmesh	= num1[1] * 100 + num1[0];

	} else {
		return(-1);
	}
	return(retmesh);
}
//---------------------------------------------------------------------
//	�Q�n�_�̈ܓx�o�x����A�Q�_�Ԃ̋��������߂�(�P��: m)
//	@return		�Q�n�_�Ԃ̋���
//---------------------------------------------------------------------
double	CMeshPos::GetDist(double	eLon1,		// �o�x�P
						  double	eLat1,		// �ܓx�P
						  double	eLon2,		// �o�x�Q
						  double	eLat2)		// �ܓx�Q
{
	double aFaiRad		= (eLat2 + eLat1) * V_PI / 180.0 / 2.0;
	double aDeltaX		= (eLon2 - eLon1) * V_PI / 180.0;
	double aDeltaY		= (eLat2 - eLat1) * V_PI / 180.0;
	double aSinVal		= sin(aFaiRad);
	double aCosVal		= cos(aFaiRad);

	double D	= 1.0 - EA2 * aSinVal * aSinVal;
	double M	= 6334834.0 / sqrt(D * D * D);
	double N	= 6377397.0 / sqrt(D);

	double aDistY	= M * aDeltaY;
	double aDistX	= N * aCosVal * aDeltaX;

	return sqrt(aDistX * aDistX + aDistY * aDistY);
}
//---------------------------------------------------------------------
//	�Q�n�_�̈ܓx�o�x����A�Q�_�Ԃ̋��� + ����(X,Y)m�����߂�
//	@return		�Q�n�_�Ԃ̋���
//---------------------------------------------------------------------
double	CMeshPos::GetDistXY	(	double		eLon1,		// �o�x�P
								double		eLat1,		// �ܓx�P
								double		eLon2,		// �o�x�Q
								double		eLat2,		// �ܓx�Q
								double		*cDist_X,	// ����X(m)
								double		*cDist_Y)	// ����Y(m)
{
	double aFaiRad		= (eLat2 + eLat1) * V_PI / 180.0 / 2.0;
	double aDeltaX		= (eLon2 - eLon1) * V_PI / 180.0;
	double aDeltaY		= (eLat2 - eLat1) * V_PI / 180.0;

	double aSinVal		= sin(aFaiRad);
	double aCosVal		= cos(aFaiRad);

	double D			= 1.0 - EA2 * aSinVal * aSinVal;
	double M			= 6334834.0 / sqrt(D * D * D);
	double N			= 6377397.0 / sqrt(D);

	double aDistY		= M * aDeltaY;
	double aDistX		= N * aCosVal * aDeltaX;

	*cDist_X	= aDistX;
	*cDist_Y	= aDistY;

	return sqrt(aDistX * aDistX + aDistY * aDistY);
}
//---------------------------------------------------------------------
// 	�ܓx�o�x�Ƃ�������o�x������Xm,�ܓx������Ym���ꂽ�Ƃ���̈ܓx�o�x���擾
//---------------------------------------------------------------------
void CMeshPos::GetLLPnt(double		eLon1,		// �o�x�P
						double		eLat1,		// �ܓx�P
						double		cDist_X,	// �����w(m)
						double		cDist_Y,	// �����x(m)
						double		*eLon2,		// �o�x�Q
						double		*eLat2)		// �ܓx�Q
{
	double  cur_lon = 0.0;
	double  cur_lat = 0.0;
	double	min_lon	= 0.0;
	double	min_lat	= 0.0;
	double	max_lon	= 0.0;
	double	max_lat	= 0.0;
	double	d_x1	= 0.0;
	double	d_y1	= 0.0;
	double	d_x2	= 0.0;
	double	d_y2	= 0.0;

	if(cDist_X < 0){
		min_lon	= -180.0;
		max_lon	= eLon1;
	}
	else{
		min_lon	= eLon1;
		max_lon	= 180.0;
	}
	if(cDist_Y < 0){
		min_lat	= -90.0;
		max_lat	= eLat1;
	}
	else{
		min_lat	= eLat1;
		max_lat	= 90;
	}
	double dist	= sqrt(cDist_X * cDist_X + cDist_Y * cDist_Y);
	// X����
	while(1){
		cur_lon	= (max_lon + min_lon) / 2.0;
		cur_lat	= (max_lat + min_lat) / 2.0;
		double t_dist	= GetDistXY(eLon1, eLat1, cur_lon, cur_lat, &d_x1, &d_y1);
		if(fabs(t_dist - dist) > 0.01){
			if(d_x1 - cDist_X > 0)
				max_lon	= (max_lon + min_lon) / 2.0;
			else
				min_lon	= (max_lon + min_lon) / 2.0;
			if(d_y1 - cDist_Y > 0)
				max_lat	= (max_lat + min_lat) / 2.0;
			else
				min_lat	= (max_lat + min_lat) / 2.0;
		}
		else{
			break;
		}
	}
	*eLon2	= cur_lon;
	*eLat2	= cur_lat;
}
//------------------------------[ end of file ]------------------------------
