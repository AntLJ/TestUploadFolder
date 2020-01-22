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
#include <list>
#include "ErrorCheck.h"
#include "define_enum.h"
#include "AheGeometryOp.h"
#include <math.h>



CErrorCheck::CErrorCheck(void)
{
}

CErrorCheck::~CErrorCheck(void)
{
}

bool CErrorCheck::MakeTKYList(FiveMeshInfo<TKYMeshInfo> &rTKY5MeshInfo)
{
	bool NGFlag = true;
	
	//�`�F�b�N�Y�����b�V���S�ẴI�u�W�F�N�g�ɑ΂��ČJ��Ԃ�
	TKYMeshData::iterator Titr = rTKY5MeshInfo.e_MainMesh.e_TKYMeshData.begin();
	for(Titr;Titr != rTKY5MeshInfo.e_MainMesh.e_TKYMeshData.end();Titr++){
		//�I�u�W�F�N�g�̕�ԓ_�S�Ăɑ΂��ČJ��Ԃ�
		std::multimap<coord,long>::iterator Oitr = Titr->second.e_CoordInfo.begin();
		for(Oitr;Oitr != Titr->second.e_CoordInfo.end() ;Oitr++){
			//�אڃ��b�V���Ƀ��b�V�����E����ň�v����|�C���g���Ȃ����`�F�b�N�����X�g�쐬
			std::pair<coord,long> aPoint(Oitr->first,Oitr->second);
			//�k���b�V��
			if (! MatchingCheck(aPoint,Titr->second,rTKY5MeshInfo.e_NorthMesh,rTKY5MeshInfo.e_MainMesh.e_MeshCode,Titr->first,Titr->second.e_FID) ){
				NGFlag = false;
			}
			//�상�b�V��
			if (! MatchingCheck(aPoint,Titr->second,rTKY5MeshInfo.e_SouthMesh,rTKY5MeshInfo.e_MainMesh.e_MeshCode,Titr->first,Titr->second.e_FID) ){
				NGFlag = false;
			}
			//�����b�V��
			if (! MatchingCheck(aPoint,Titr->second,rTKY5MeshInfo.e_EastMesh,rTKY5MeshInfo.e_MainMesh.e_MeshCode,Titr->first,Titr->second.e_FID) ){
				NGFlag = false;
			}
			//�����b�V��
			if (! MatchingCheck(aPoint,Titr->second,rTKY5MeshInfo.e_WestMesh,rTKY5MeshInfo.e_MainMesh.e_MeshCode,Titr->first,Titr->second.e_FID) ){
				NGFlag = false;
			}
		}
	}

	return NGFlag;
}

bool CErrorCheck::CheckJGDData(FiveMeshInfo<JGDMeshInfo> &rJGD5MeshInfo,std::ofstream *pErrorFile,std::string aLayerName,double aBorder)
{
	bool bErrFlag = true;
	
	//TKY���X�g���Ȃ��ꍇ�́B�`�F�b�N���Ȃ�
	if( m_CheckList.size() == 0) return true;
	//�����o�ϐ��ł��郊�X�g��ǂݍ���TKY�Ő��藧���Ă����אڐ��������藧���Ă��邩�`�F�b�N����B
	std::map<CheckKey,CheckElement>::iterator Litr = m_CheckList.begin();
	//long count = 0;
	long aMeshCode = Litr->first.e_NMeshCode;
	for(Litr;Litr != m_CheckList.end();Litr++)
	{
		
		//Error���ɏo�͂���DIVID���\�z(���H�E���s�҃f�[�^�ȊO��NULL�ŏo�͂��邽��)
		std::string aStrMDIVID = (Litr->first.e_MDIVID == DEFAULT_VALUE ? "" : std::to_string(Litr->first.e_MDIVID));
		std::string aStrNDIVID = (Litr->first.e_NDIVID == DEFAULT_VALUE ? "" : std::to_string(Litr->first.e_NDIVID));

		//�Y�����b�V���̃I�u�W�F�N�g�f�[�^�R���e�i����`�F�b�N�ΏۃI�u�W�F�N�g�f�[�^�ƊY���|�C���g�f�[�^�擾
		JGDObjectData aMObjectData;
		std::pair<long,coord> aMPointInfo;
		if( !GetObjectData(&aMObjectData,&aMPointInfo,rJGD5MeshInfo.e_MainMesh,Litr,0))
		{
			//Error�o��
			*pErrorFile << 0 << "\t" << Litr->first.e_MMeshCode << "_" << aLayerName.c_str() << "\t" << Litr->first.e_MFID  << "\tfatal error\t8888\t�`�F�b�N�ΏۃI�u�W�F�N�g�����݂��܂���\t"
				<< Litr->first.e_MMeshCode << "\t" << Litr->first.e_MOID <<"\t" << aStrMDIVID << "\t" << Litr->first.e_MPointNum << "\t"
				<< Litr->first.e_NMeshCode << "\t" << Litr->first.e_NOID <<"\t" << aStrNDIVID << "\t" << Litr->first.e_NPointNum << std::endl;
			bErrFlag = false;
			continue;
		}

		//�אڃ��b�V���̃I�u�W�F�N�g�f�[�^�R���e�i����`�F�b�N�ΏۃI�u�W�F�N�g�f�[�^�ƊY���|�C���g�f�[�^�擾
		JGDObjectData aNObjectData;
		std::pair<long,coord> aNPointInfo;
		bool aCheck = false;
		if( Litr->first.e_NMeshCode == rJGD5MeshInfo.e_SouthMesh.e_MeshCode){
			aCheck = GetObjectData(&aNObjectData,&aNPointInfo,rJGD5MeshInfo.e_SouthMesh,Litr,1);
		}
		else if( Litr->first.e_NMeshCode == rJGD5MeshInfo.e_WestMesh.e_MeshCode){
			aCheck = GetObjectData(&aNObjectData,&aNPointInfo,rJGD5MeshInfo.e_WestMesh,Litr,1);
		}
		else if( Litr->first.e_NMeshCode == rJGD5MeshInfo.e_EastMesh.e_MeshCode){
			aCheck = GetObjectData(&aNObjectData,&aNPointInfo,rJGD5MeshInfo.e_EastMesh,Litr,1);
		}
		else if( Litr->first.e_NMeshCode == rJGD5MeshInfo.e_NorthMesh.e_MeshCode){
			aCheck = GetObjectData(&aNObjectData,&aNPointInfo,rJGD5MeshInfo.e_NorthMesh,Litr,1);
		}


		if(!aCheck ){
			//Error�o��
			*pErrorFile << 0 << "\t" << Litr->first.e_MMeshCode << "_" << aLayerName.c_str() << "\t" << Litr->first.e_NFID  << "\tfatal error\t9999\t�`�F�b�N�Ώە�ԓ_�����݂��܂���\t"
				<< Litr->first.e_MMeshCode << "\t" << Litr->first.e_MOID <<"\t" << aStrMDIVID << "\t" << Litr->first.e_MPointNum << "\t"
				<< Litr->first.e_NMeshCode << "\t" << Litr->first.e_NOID <<"\t" << aStrNDIVID << "\t" << Litr->first.e_NPointNum << std::endl;
			bErrFlag = false;
			continue;
		}

		//�擾�����|�C���g�f�[�^�̍��W����v���邩�`�F�b�N
		double dX = abs(aMPointInfo.second.e_X - aNPointInfo.second.e_X);
		double dY = abs(aMPointInfo.second.e_Y - aNPointInfo.second.e_Y);

		if( (dX != 0) || (dY != 0)){
			*pErrorFile << 0 << "\t" << Litr->first.e_MMeshCode << "_" << aLayerName.c_str() << "\t" << Litr->first.e_MFID << "\terror\t10001\t���W�������Ƃ�Ă��܂���\t"
				<< Litr->first.e_MMeshCode << "\t" << Litr->first.e_MOID <<"\t" << aStrMDIVID << "\t" << Litr->first.e_MPointNum << "\t"
				<< Litr->first.e_NMeshCode << "\t" << Litr->first.e_NOID <<"\t" << aStrNDIVID << "\t" << Litr->first.e_NPointNum 
				<< "(" << dX << "," << ")" << std::endl;
			bErrFlag = false;
			continue;
		}

		//�p�x�`�F�b�N
		//�p�x�Q�b�g
		CheckElement aArg={0,0,0,0};
		if(rJGD5MeshInfo.e_MainMesh.e_JGDMeshData.begin()->second.e_ShapeType == esriGeometryPolygon)
		{
			//�|���S���̏ꍇ�A�p���\������3�_�����߂�B(�Y�����b�V�����Ɨאڃ��b�V������)
			ArgPoints aArgPoints;
			if( !( GetArgPoint(&aArgPoints.e_StartNum, aMObjectData, Litr->first.e_MPointNum)&&GetArgPoint(&aArgPoints.e_EndNum, aNObjectData, Litr->first.e_NPointNum) )) return false;
			GetPolygonArg(&aArg,aMPointInfo.first,aNPointInfo.first,aMObjectData.e_PointSet,aNObjectData.e_PointSet,aArgPoints);
		}
		else if(rJGD5MeshInfo.e_MainMesh.e_JGDMeshData.begin()->second.e_ShapeType == esriGeometryPolyline) 
		{
			GetPolyLineArg(&aArg,aMPointInfo.first,aNPointInfo.first,aMObjectData.e_PointSet,aNObjectData.e_PointSet);
		}

		double dArg1 = fabs(aArg.e_arg1 - Litr->second.e_arg1);
		double dArg2 = fabs(aArg.e_arg2 - Litr->second.e_arg2);
		double dArg3 = fabs(aArg.e_arg3 - Litr->second.e_arg3);
		double dArg4 = fabs(aArg.e_arg4 - Litr->second.e_arg4);

#ifdef _DEBUG
		if (dArg1 > 180)
			*pErrorFile << dArg1 << "��180�x�𒴂��Ă��܂�" << std::endl;
		if (dArg2 > 180)
			*pErrorFile << dArg1 << "��180�x�𒴂��Ă��܂�" << std::endl;
		if (dArg3 > 180)
			*pErrorFile << dArg1 << "��180�x�𒴂��Ă��܂�" << std::endl;
		if (dArg4 > 180)
			*pErrorFile << dArg1 << "��180�x�𒴂��Ă��܂�" << std::endl;
#endif


		if( ( fabs(dArg1) > aBorder) || ( fabs(dArg2) > aBorder) || ( fabs(dArg3) > aBorder) || ( fabs(dArg4) > aBorder)){
			//error�o��
			*pErrorFile << 0 << "\t" << Litr->first.e_MMeshCode << "_" << aLayerName.c_str() << "\t" << Litr->first.e_MFID  << "\terror\t10002\t�p�x�������Ƃ�Ă��܂���\t"
				<< Litr->first.e_MMeshCode << "\t" << Litr->first.e_MOID <<"\t" << aStrMDIVID << "\t" << Litr->first.e_MPointNum << "\t"
				<< Litr->first.e_NMeshCode << "\t" << Litr->first.e_NOID <<"\t" << aStrNDIVID << "\t" << Litr->first.e_NPointNum <<
				 "\t" << dArg1 << "\t" << dArg2 << "\t" << dArg3 << "\t" << dArg4 << std::endl;
			bErrFlag = false;
		}		

	}

	return bErrFlag;

}

bool CErrorCheck::MatchingCheck(std::pair<coord,long> &rCoordInfo, TKYObjectData &rMTKYObjectData, TKYMeshInfo &rNTKYMeshInfo,long aMMeshCode,const ObjectDataKey &aObjectDataKey,long aMFID)
{
	//�אڃ��b�V����TKY�I�u�W�F�N�g�ɂ�������C�e���[�^
	TKYMeshData::iterator Nitr = rNTKYMeshInfo.e_TKYMeshData.begin();
	for(Nitr;Nitr != rNTKYMeshInfo.e_TKYMeshData.end() ;Nitr++){
		FIDPair aFIDPair;
		aFIDPair.e_MFID = aMFID;
		aFIDPair.e_NFID = Nitr->second.e_FID;
		
		size_t count = Nitr->second.e_CoordInfo.count(rCoordInfo.first);
		std::multimap<coord,long>::iterator Pitr = Nitr->second.e_CoordInfo.find(rCoordInfo.first);
		if(Pitr == Nitr->second.e_CoordInfo.end())
		{
			continue;
		}

		for(unsigned long i = 0;i < count ;i++){
			CheckElement aArg = {0,0,0,0};
			//�V�F�C�v���|�����C���A�|���S���̏ꍇ,�p�����߂�
			if(Nitr->second.e_ShapeType == esriGeometryPolyline) GetPolyLineArg(&aArg,rCoordInfo.second,Pitr->second,rMTKYObjectData.e_PointSet ,Nitr->second.e_PointSet);
			if(Nitr->second.e_ShapeType == esriGeometryPolygon ) {
				//�|���S���̏ꍇ�A�p���\������3�_�����߂�B(�Y�����b�V�����Ɨאڃ��b�V������)
				ArgPoints aArgPoints;
				if( !( GetArgPoint(&aArgPoints.e_StartNum, rMTKYObjectData, rCoordInfo.second) && GetArgPoint(&aArgPoints.e_EndNum, Nitr->second, Pitr->second) )) return false;
				GetPolygonArg(&aArg,rCoordInfo.second,Pitr->second,rMTKYObjectData.e_PointSet,Nitr->second.e_PointSet,aArgPoints);
			}
			//���X�g�̍s�쐬
			InsertRow(aMMeshCode, aObjectDataKey.e_OID, aObjectDataKey.e_DIVID, rCoordInfo.second, rNTKYMeshInfo.e_MeshCode, Nitr->first.e_OID, Nitr->first.e_DIVID, Pitr->second, aArg,aFIDPair);
			Pitr++;
		}
	}
	return true;
}

bool CErrorCheck::GetPolyLineArg(CheckElement *pArg,long aMPointNum,long aNPointNum,std::map<long,IPointPtr> &rMPointSet,std::map<long,IPointPtr> &rNPointSet)
{
	size_t aMSize = rMPointSet.size();
	size_t aNSize = rNPointSet.size();
	long k,l;
	//�p�����߂�ۂɕK�v�ȕ�ԓ_�̕�ԓ_�ԍ��ݒ�
	if(aMPointNum == (aMSize-1)){
		k = aMPointNum-1;
	}
	else{
		k = 1;
	}
	
	if(aNPointNum == (aNSize-1)){
		l = aNPointNum-1;
	}
	else{
		l = 1;
	}

	std::map<long,IPointPtr>::iterator itr1,itrC,itr2;
	//�p�����߂�(�@����)
	itr1 = rMPointSet.find(k);
	itrC = rMPointSet.find(aMPointNum);
	itr2 = rNPointSet.find(l);

	pArg->e_arg1 = AheGetAngle(itr1->second,itrC->second,itr2->second);
	if (pArg->e_arg1 < 0)
	{
		pArg->e_arg1 = 360 + pArg->e_arg1;
	}

	return true;
}

bool CErrorCheck::GetPolygonArg(CheckElement *pArg,long aMPointNum,long aNPointNum, std::map<long,IPointPtr> &rMPointSet,std::map<long,IPointPtr> &rNPointSet,ArgPoints &rArgPoints)
{

	std::map<long,IPointPtr>::iterator itr1,itrC,itr2;
	itrC = rMPointSet.find(aMPointNum);
	//�p�x�@�����߂�B
	itr1 = rMPointSet.find(rArgPoints.e_StartNum.l);
	itr2 = rNPointSet.find(rArgPoints.e_EndNum.l);
	pArg->e_arg1 = AheGetAngle(itr1->second,itrC->second,itr2->second);
	if (pArg->e_arg1 < 0)
	{
		pArg->e_arg1 = 360 + pArg->e_arg1;
	}

	//�p�x�A�����߂�B
	itr1 = rMPointSet.find(rArgPoints.e_StartNum.l);
	itr2 = rNPointSet.find(rArgPoints.e_EndNum.k);
	pArg->e_arg2 = AheGetAngle(itr1->second,itrC->second,itr2->second);
	if (pArg->e_arg2 < 0)
	{
		pArg->e_arg2 = 360 + pArg->e_arg2;
	}

	//�p�x�B�����߂�B
	itr1 = rMPointSet.find(rArgPoints.e_StartNum.k);
	itr2 = rNPointSet.find(rArgPoints.e_EndNum.l);
	pArg->e_arg3 = AheGetAngle(itr1->second,itrC->second,itr2->second);
	if (pArg->e_arg3 < 0)
	{
		pArg->e_arg3 = 360 + pArg->e_arg3;
	}

	//�p�x�C�����߂�
	itr1 = rMPointSet.find(rArgPoints.e_StartNum.k);
	itr2 = rNPointSet.find(rArgPoints.e_EndNum.k);
	pArg->e_arg4 = AheGetAngle(itr1->second,itrC->second,itr2->second);
	if (pArg->e_arg4 < 0)
	{
		pArg->e_arg4 = 360 + pArg->e_arg4;
	}

	return true;
}




bool CErrorCheck::InsertRow(long aMMeshCode,long aMOID, long aMDIVID, long aMPointNum, long aNMeshCode, long aNOID, long aNDIVID, long aNPointNum, CheckElement aArg,FIDPair aFIDPair)
{
	CheckKey aCheckKey;
	aCheckKey.e_MMeshCode = aMMeshCode;
	aCheckKey.e_MOID = aMOID;
	aCheckKey.e_MDIVID = aMDIVID;
	aCheckKey.e_MPointNum = aMPointNum;
	aCheckKey.e_NMeshCode = aNMeshCode;
	aCheckKey.e_NOID = aNOID;
	aCheckKey.e_NDIVID = aNDIVID;
	aCheckKey.e_NPointNum = aNPointNum;
	aCheckKey.e_MFID = aFIDPair.e_MFID;
	aCheckKey.e_NFID = aFIDPair.e_NFID;

	m_CheckList.insert(std::pair<CheckKey,CheckElement>(aCheckKey,aArg));

	return true;
}
bool CErrorCheck::GetObjectData(JGDObjectData *pObjectData,std::pair<long,coord> *pPointInfo, JGDMeshInfo &rJGDMeshInfo, std::map<CheckKey,CheckElement>::iterator &Litr, long judge)
{
	std::map<ObjectDataKey,JGDObjectData>::iterator Oitr;
	//judge=0�ŊY�����b�V���̃I�u�W�F�N�g���Ajudge=1�ŗאڃ��b�V���̃I�u�W�F�N�g���擾
	if(judge == 0){
		ObjectDataKey aObjectDataKey;
		aObjectDataKey.e_OID = Litr->first.e_MOID;
		aObjectDataKey.e_DIVID = Litr->first.e_MDIVID;
		Oitr = rJGDMeshInfo.e_JGDMeshData.find(aObjectDataKey);
	}
	else if(judge == 1){
		ObjectDataKey aObjectDataKey;
		aObjectDataKey.e_OID = Litr->first.e_NOID;
		aObjectDataKey.e_DIVID = Litr->first.e_NDIVID;
		Oitr = rJGDMeshInfo.e_JGDMeshData.find(aObjectDataKey);
	}

	if(Oitr == rJGDMeshInfo.e_JGDMeshData.end())
	{
		return false;
	}
	*pObjectData = Oitr->second;
	//�Ώۃ|�C���g�擾
	std::multimap<long,coord>::iterator Pitr;
	if(judge == 0) Pitr = Oitr->second.e_PointInfo.find(Litr->first.e_MPointNum);
	else if(judge == 1) Pitr = Oitr->second.e_PointInfo.find(Litr->first.e_NPointNum);
	if(Pitr == Oitr->second.e_PointInfo.end())
	{
		return false;
	}

	std::pair<long,coord> aPointInfo(Pitr->first,Pitr->second);
	*pPointInfo = aPointInfo;
	
	return true;
}

