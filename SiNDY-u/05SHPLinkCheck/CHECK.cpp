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

#include "StdAfx.h"
#include "CHECK.h"
#include <math.h>
#include <iomanip>

//�����_�ȉ��̕\������
#define  DECIMAL 15

CCHECK::CCHECK(void)
{
}

CCHECK::~CCHECK(void)
{
}

bool CCHECK::CheckLinkSHP(std::map<std::pair<long, long>, WKSPoint> & rNodeSet, std::vector<LINK> & rLinkSet,long rSecondMesh, bool bWalkOpt)
{
	std::vector<LINK>::iterator Linkitr = rLinkSet.begin();
	std::map<std::pair<long, long>, WKSPoint>::iterator Nodeitr;

	bool aCheck = true;

	while(Linkitr != rLinkSet.end())
	{
		long aID,FID;								//aID:FROMNODEID,TONODEID�i�[�p OID:�����N��OID
		long aDivID = -1;							//aDivID:FROMDIVID,TODIVID�i�[�p�B�����l-1
		WKSPoint aFFirstPoint,aFEndPoint;			//aFFirstPoint:�����N���C�����瓾���n�_���W�i�[�p,aFEndPoint:�����N���C�����瓾���I�_���W�i�[�p
		WKSPoint aTFirstPoint,aTEndPoint;			//aTFirstPoint:�m�[�h���C�����瓾���n�_���W���ؗp�̍��W�i�[�p,aTEndPoint�F�m�[�h���C�����瓾���I�_���W���ؗp�̍��W�i�[�p
		double aFirstDeltaX=0,aFirstDeltaY=0;		//�����N���C���n�_���W��FromNodeID�����m�[�h���C���̍��W�̌덷�i�[�p
		double aEndDeltaX=0,aEndDeltaY=0;			//�����N���C���I�_���W��ToNodeID�����m�[�h���C���̍��W�̌덷�i�[�p
		bool aFirstCheck=true;                      //�G���[�̎�ނ���肷�邽�߂̕ϐ�
		bool aEndCheck = true;                      //�G���[�̎�ނ���肷�邽�߂̕ϐ�
		std::pair<long, long> pairID;

		//�����N��FID
		FID = Linkitr->FID;

		//�n�_�̃`�F�b�N
		aID = Linkitr->FromNodeID;
		aFFirstPoint = Linkitr->FirstPoint;
		
		if(bWalkOpt){
			aDivID = Linkitr->FromDivID;
		}
		pairID.first = aID;
		pairID.second = aDivID;

		//FromNodeID�����m�[�h�̍��W���m�[�h���C�����猟��
		Nodeitr = rNodeSet.find(pairID);

		if(Nodeitr == rNodeSet.end()){
			std::cout << "error\t" << rSecondMesh << "\t" << "FID:" << FID << "�̃����N�̎n�_�m�[�h("  << "OBJECTID:" << aID;
			if(bWalkOpt){
				std::cout << ", DIVID:" << aDivID;
			}
			std::cout << ")�͑��݂��܂���" << std::endl;
			aCheck = false;
		}
		else{
			aTFirstPoint = Nodeitr->second;
			//�����N���C���̎n�_���W�ƁAFromNodeID�����m�[�h���C���̍��W���r
			if((aFFirstPoint.X != aTFirstPoint.X )|| (aFFirstPoint.Y != aTFirstPoint.Y)){

				aFirstDeltaX= fabs(aFFirstPoint.X-aTFirstPoint.X);
				aFirstDeltaY= fabs(aFFirstPoint.Y-aTFirstPoint.Y);

				if((aFirstDeltaX != 0) || (aFirstDeltaY != 0)){

					//error
					aFirstCheck = false;
					aCheck = false;
				}
			}
		}

		//�I�_�̃`�F�b�N

		aID = Linkitr->ToNodeID;
		aFEndPoint = Linkitr->EndPoint;

		if(bWalkOpt){
			aDivID = Linkitr->ToDivID;
		}
		pairID.first = aID;
		pairID.second = aDivID;

		//ToNodeID�����m�[�h�̍��W���m�[�h���C�����猟��
		Nodeitr = rNodeSet.find(pairID);
		aTEndPoint = Nodeitr->second;

		if(Nodeitr == rNodeSet.end()){
			std::cout << "error\t" << rSecondMesh << "\t" << "FID:" << FID << "�̃����N�̏I�_�m�[�h("  << "OBJECTID:" << aID;
			if(bWalkOpt){
				std::cout << ", DIVID:" << aDivID;
			}
			std::cout << ")�͑��݂��܂���" << std::endl;
			aCheck=false;
		}
		else{
			//�����N���C���̏I�_���W�ƁAToNodeID�����m�[�h���C���̍��W���r
			if((aFEndPoint.X != aTEndPoint.X) ||(aFEndPoint.Y != aTEndPoint.Y)){

				aEndDeltaX= fabs(aFEndPoint.X-aTEndPoint.X);
				aEndDeltaY= fabs(aFEndPoint.Y-aTEndPoint.Y);

				if((aEndDeltaX != 0) || (aEndDeltaY != 0)){

					//error
					aEndCheck = false;
					aCheck = false;
				}
			}
		}

		//�G���[�̎�ނ��o��
		if((aFirstCheck == false) && (aEndCheck == false)){
			if((aFFirstPoint.X == aTEndPoint.X) && (aFFirstPoint.Y==aTEndPoint.Y)&&
				(aFEndPoint.X == aTFirstPoint.X)&&(aFEndPoint.Y == aTFirstPoint.Y)){
					std::cout << "error\t" << rSecondMesh << "\tFID:" << FID <<"\t�����N�̌������t�ł�"<<std::endl;
			}
			else{
				std::cout << "error\t" << rSecondMesh << "\tFID:" << FID <<"\t�N�I�_�̈ʒu�ɍ��Ⴊ����܂�"<<std::endl;
				std::cout << "\t\t" << "�N�_���W�̍���(" << std::fixed << std::setprecision(DECIMAL) << aFirstDeltaX << "," << std::fixed << std::setprecision(DECIMAL) << aFirstDeltaY << ")" << std::endl;
				std::cout << "\t\t" << "�I�_���W�̍���(" << std::fixed << std::setprecision(DECIMAL) << aEndDeltaX << "," << std::fixed << std::setprecision(DECIMAL) << aEndDeltaY << ")" << std::endl;
			}
		}
		else if(aFirstCheck == false){
			std::cout << "error\t" << rSecondMesh << "\tFID:" << FID <<"\t�N�_�̈ʒu�ɍ��Ⴊ����܂�"<<std::endl;
			std::cout << "\t\t" << "�N�_���W�̍���(" << std::fixed << std::setprecision(DECIMAL) << aFirstDeltaX << "," << std::fixed << std::setprecision(DECIMAL) << aFirstDeltaY << ")" << std::endl;
		}
		else if(aEndCheck == false){
			std::cout<< "error\t" << rSecondMesh << "\tFID:" << FID <<"\t�I�_�̈ʒu�ɍ��Ⴊ����܂�"<<std::endl;
			std::cout << "\t\t" << "�I�_���W�̍���(" << std::fixed << std::setprecision(DECIMAL) << aEndDeltaX << "," << std::fixed << std::setprecision(DECIMAL) << aEndDeltaY << ")" << std::endl;
		}
		Linkitr++;
	}
	return aCheck;
}
