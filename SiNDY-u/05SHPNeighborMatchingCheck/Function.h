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

#pragma once
#include <set>
#include <map>
#include "crd_cnv.h"

namespace Function{
	//string �� wstring
	std::wstring filestr(const char* str);
	//�אڃ��b�V���̎擾middle�p
	long GetMeshCode(long aMeshCode, long x,long y,std::string aDataSet);
}



//ID�̖��O�ƃC���f�b�N�X�i�[�p�\����
struct ID{
	std::string e_Name;
	long e_Index;
};


//ID�i�[�p�\����
struct IDs{
	ID e_ID;
	ID e_DIVID;
};


namespace ArcFunction{
	//���[�N�X�y�[�X�擾�`�t�B�[�`���N���X�擾
	bool GetFeatureClass(IFeatureClassPtr &ipFeatureClass,std::string aSHPPath,std::string aDataSet);
	//SHP(TKY)�̃f�[�^���擾����֐�
	bool GetTKYInfo(const Parameters &rParameters, TKYMeshData *pTKYMeshData, long aMeshCode,/*IFeatureClassPtr &ipMeshClass,*/std::string aLayerType,std::map<long,FID_Set> *pOIDSet, bool *pCheck);
	//SHP(JGD)�̃f�[�^���擾����֐�
	bool GetJGDInfo(const Parameters &rParameters, JGDMeshData *pJGDMeshData, long aMeshCode,IFeatureClassPtr &ipMeshClass,std::string aLayerType,std::map<long,FID_Set> &rOIDSet,bool *pCheck);
	//SHP�t�@�C���̃��[�N�X�y�[�X�擾�`�t�B�[�`���N���X�擾
	bool GetSHPFeatureClass(IFeatureClassPtr &ipFeatureClass,IDs *pIDs, std::string aOrgPath,std::string aLayerType,std::string aDataSet, long aMeshCode,std::string aGeoType, bool *pCheck, bool aWalkFlag);
	//�w�肳�ꂽ���b�V����SHP�f�[�^���i�[����Ă���p�X�쐬�֐�
	bool MakeSHPPath(std::string aOrgPath,std::string aDataSet, long aMeshCode,std::string *pSHPPath);
	//�t�B���^�^���A�J�[�\�����擾���A���̃f�[�^���R���e�i�Ɋi�[����֐�
	template <class Type>
	bool GetObjectInfo(std::map<ObjectDataKey,Type> *pMeshData, IQueryFilterPtr ipQueryFilter,IFeatureClassPtr &ipFeatureClass, std::string aLayerType, long aMeshCode,std::string aGeoType,std::map<long,FID_Set> *pOIDSet,IDs aIDs)
	{
		std::set<long> aOIDSet;
		//�J�[�\���擾���A�Ԃ��
		IFeatureCursorPtr ipFeatureCursor = ipFeatureClass->_Search(ipQueryFilter,VARIANT_FALSE);
		IFeaturePtr ipFeature;

		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL )
		{
			Type aObjectData;//TKYObjectData��������JGDObjectData�i�[�p�ϐ�

			//FID�擾
			long aFID = 0;
			ipFeature->get_OID(&aFID);
			aObjectData.e_FID = aFID;
			//OBJECTID�܂��͂���ɑ�������ID�擾
			ObjectDataKey aObjectKey;
			CComVariant aOID;
			if(FAILED(ipFeature->get_Value(aIDs.e_ID.e_Index, &aOID))){
				std::cerr << aMeshCode << "_" << aLayerType << "FID:" << aFID << "��" << aIDs.e_ID.e_Name << "�擾�Ɏ��s���܂���" << std::endl;
				return false;
			}
			aObjectKey.e_OID = aOID.lVal;
			//DIVID�擾
			CComVariant aDIVID;
			if( ! aIDs.e_DIVID.e_Name.empty() ) {
				if(FAILED(ipFeature->get_Value(aIDs.e_DIVID.e_Index, &aDIVID))){
					std::cerr << aMeshCode << "_" << aLayerType << "FID:" << aFID << "��" << aIDs.e_DIVID.e_Name << "�擾�Ɏ��s���܂���" << std::endl;
					return false;
				}
				aObjectKey.e_DIVID = aDIVID.lVal;
			}
			//�`��擾
			IGeometryPtr ipGeometry;
			if(FAILED(ipFeature->get_ShapeCopy( &ipGeometry))){
				std::cerr << aMeshCode << "_" << aLayerType << " FID:" << aFID << "�̌`��̎擾�Ɏ��s���܂���" << std::endl;
				return false;
			}
			//�`��̃^�C�v�擾
			esriGeometryType aShapeType;
			if(FAILED(  ipGeometry->get_GeometryType( &aShapeType))){
				std::cerr << aMeshCode << "_" << aLayerType << " FID:" << aFID << "�̌`���ʂ̎擾�Ɏ��s���܂���" << std::endl;
				return false;
			}
			//�|���S���̏ꍇ�A�����̃����N�ō\������Ă��邩�`�F�b�N����B
			if(aShapeType == esriGeometryPolygon){
				IGeometryCollectionPtr ipGeometryCollection(ipGeometry);
				long aCount;
				ipGeometryCollection->get_GeometryCount(&aCount);
				if( aCount > 1){
					aObjectData.e_multi = true;
				}else
				{
					aObjectData.e_multi = false;
				}
			}else
			{
				aObjectData.e_multi =false;
			}

			
			aObjectData.e_ShapeType = aShapeType;
			//�\���_�Ȃǃ`�F�b�N�ɕK�v�ȏ����擾�A�܂����b�V�����E���ɐڂ��Ă���ꍇ�̂݁A�R���e�i�Ƀf�[�^�}��
			bool aCheck = false;
			if( aShapeType == esriGeometryPoint ){
				//�|�C���g�p�֐�
				if(! GetPointInfo(ipGeometry,&aObjectData,aGeoType,aMeshCode,&aOIDSet,aFID,&aCheck))
				   return false;
			}
			else if( aShapeType == esriGeometryPolyline){
				//�|�����C���p�֐�
				if( !GetPolylineInfo( ipGeometry,&aObjectData,aGeoType,aMeshCode,&aOIDSet,aFID,&aCheck))
					return false;
			}
			else if( aShapeType == esriGeometryPolygon){
				//�|�����C��&�|���S���p�֐�
				if(! GetPolygonInfo(ipGeometry,&aObjectData,aGeoType,aMeshCode,&aOIDSet,aFID,&aCheck))
					return false;
			}
			else{
				std::cerr << aMeshCode << "_" << aLayerType << " FID:" << aFID << "�̌`��ɑΉ����Ă��܂���J���҂ɖ₢���킹�Ă�������" << std::endl;
				return false;
			}

			//�R���e�i�Ƀf�[�^�}��
			if( aCheck ){
				pMeshData->insert(std::pair<ObjectDataKey,Type>(aObjectKey,aObjectData));
			}

	
		}
		if(aGeoType == "TKY"){
			pOIDSet->insert(std::pair<long,FID_Set>(aMeshCode,aOIDSet));
		}

		return true;

	}
	
	//�|�C���g�p�I�u�W�F�N�g���擾�֐�
	template <class Type>
	bool GetPointInfo(const IGeometryPtr &ipGeometry, Type *pObjectData,std::string aGeoType,long aMeshCode,std::set<long> *pOIDSet,long &aFID,bool *pCheck) 
	{
		//�`���_��
		IPointPtr ipPoint(ipGeometry);
		
		//���W�擾
		coord aCoord;
		if(FAILED(ipPoint->get_X(&aCoord.e_X)) || FAILED(ipPoint->get_Y(&aCoord.e_Y))){
			std::cerr << "�ܓx�o�x�擾�Ɏ��s" << std::endl;
			return false;
		}
		if(aGeoType == "TKY"){
			crd_cnv aCrd;
			int X,Y;
			if(aCrd.LLtoMesh2(aCoord.e_X,aCoord.e_Y,aMeshCode,&X,&Y) == -1){
				std::cerr << "�ܓx�o�x�̍��W�ϊ��Ɏ��s���܂���" << std::endl;
				return false;
			}
			//middle�̏ꍇ�̍��W�͈̔͂� 0 <= X <= 2000000, 0 <= Y <= 2000000
			//middle�ȊO�̍��W�͈̔͂� 0 <= X <= 1000000, 0 <= Y <= 1000000
			//���W��胁�b�V�����E����ɐڂ��Ă���I�u�W�F�N�g�����f����B
			long MAX_Coord = 0;
			if( (aMeshCode >= 2922) && (aMeshCode <= 6853) ){
				MAX_Coord = 2000000;
			}else 
			{
				MAX_Coord = 1000000;
			}

			if((X == 0) || ( X == MAX_Coord) || ( Y == 0) || ( Y == MAX_Coord))
			{
				Insert(&pObjectData->e_PointInfo,&pObjectData->e_CoordInfo,0,aCoord);
				pOIDSet->insert(aFID);
				*pCheck = true;
			}
			
		}else
		{
		//���W���R���e�i�ɑ}��
		Insert(&pObjectData->e_PointInfo,&pObjectData->e_CoordInfo,0,aCoord);
		*pCheck = true;
		}

		return true;
	}

	template <class Type>
	bool GetPolylineInfo(const IGeometryPtr &ipGeometry, Type *pObjectData ,std::string aGeoType,long aMeshCode,std::set<long> *pOIDSet,long &aFID,bool *pCheck)
	{

		//�`���_��̏W���ɕϊ�
		IPointCollectionPtr ipPointCollection(ipGeometry);
		long aPointCount;
		if(FAILED(ipPointCollection->get_PointCount(&aPointCount))){
			std::cerr << "�\���_�̎擾�Ɏ��s" << std::endl;
			return false;
		}

		//TKY�̏ꍇ�A�n�_�A�I�_�����E����ɂ��邩�`�F�b�N
		//���b�V�����E���ɐڂ��Ă��邩�̃`�F�b�N�p�ϐ�(JGD�͂��Ȃ炸ture)
		bool aborderchk = false;
		if( aGeoType == "TKY")
		{
			IPointPtr ipFromPoint,ipToPoint;
			if(FAILED(ipPointCollection->get_Point(0,&ipFromPoint)) || FAILED(ipPointCollection->get_Point((aPointCount-1),&ipToPoint)))
			{
				std::cerr << "�n�_or�I�_�̎擾�Ɏ��s" << std::endl;
				return false;
			}
			crd_cnv aCrd;
			//���W�̎擾
			coord aFromCoord,aToCoord;
			if(FAILED(ipFromPoint->get_X(&aFromCoord.e_X))||FAILED(ipFromPoint->get_Y(&aFromCoord.e_Y))||FAILED(ipToPoint->get_X(&aToCoord.e_X))||FAILED(ipToPoint->get_Y(&aToCoord.e_Y)))
			{
				std::cerr << "�n�_or�I�_�̈ܓx�o�x�擾�Ɏ��s���܂���" <<std::endl;
				return false;
			}

			int From_X,From_Y,To_X,To_Y;
			if( (aCrd.LLtoMesh2(aFromCoord.e_X,aFromCoord.e_Y,aMeshCode,&From_X,&From_Y)==-1)||(aCrd.LLtoMesh2(aToCoord.e_X,aToCoord.e_Y,aMeshCode,&To_X,&To_Y)==-1))
			{
				std::cerr << "�n�_or�I�_�̈ܓx�o�x�����W�ϊ��Ɏ��s���܂���" << std::endl;
				return false;
			}

			//middle�̏ꍇ�̍��W�͈̔͂� 0 <= X <= 2000000, 0 <= Y <= 2000000
			//middle�ȊO�̍��W�͈̔͂� 0 <= X <= 1000000, 0 <= Y <= 1000000
			//���W��胁�b�V�����E����ɐڂ��Ă���I�u�W�F�N�g�����f����B�@
			long MAX_Coord = 0;
			if( (aMeshCode >= 2922) && (aMeshCode <= 6853) ){
				MAX_Coord = 2000000;
			}else 
			{
				MAX_Coord = 1000000;
			}

			//�n�_�����E����ɂ��邩�̃`�F�b�N
			if((From_X == 0) || (From_X == MAX_Coord) || (From_Y == 0) || (From_Y == MAX_Coord))
			{
				aborderchk = true;
			}//�I�_�����E����ɂ��邩�̃`�F�b�N
			else if((To_X == 0) || (To_X == MAX_Coord) || (To_Y == 0) || (To_Y == MAX_Coord))
			{
				aborderchk = true;
			}

		}
		else if( aGeoType == "JGD")
		{
			aborderchk = true;
			*pCheck = true;
		}

		//���E����ɐڂ��Ă�������i�[
		if( aborderchk){
			int i = 0;
			for(i;i < aPointCount; i++)
			{
				//i�Ԗڂ�iPointoPtr���擾
				IPointPtr ipPoint;
				coord aCoord;
				if(FAILED(ipPointCollection->get_Point(i,&ipPoint))
					||FAILED(ipPoint->get_X(&(aCoord.e_X))) ||FAILED(ipPoint->get_Y(&(aCoord.e_Y))))
				{
					std::cout << "�\���_���̎擾�Ɏ��s���܂���" << std::endl;
					return false;
				}
	
				//iPointPtr�i�[�R���e�i�A���W�i�[�R���e�i�ɑ}��
				pObjectData->e_PointSet.insert(std::pair<long,IPointPtr>(i,ipPoint));
				Insert(&pObjectData->e_PointInfo,&pObjectData->e_CoordInfo,i,aCoord);
		
			}
			if(aGeoType == "TKY"){
			//FID���R���e�i�Ɋi�[
			pOIDSet->insert(aFID);
			*pCheck = true;
			}
		}

	
		return true;
	}



	//�|���S���p�I�u�W�F�N�g���擾�֐�
	template <class Type>
	bool GetPolygonInfo(const IGeometryPtr &ipGeometry, Type *pObjectData/*std::map<long,IPointPtr> *pPointSet, std::multimap<Type1,Type2> *pPointInfo*/,std::string aGeoType,long aMeshCode,std::set<long> *pOIDSet,long &aFID,bool *pCheck)
	{
		//�`���_��̏W���ɕϊ�
		IPointCollectionPtr ipPointCollection(ipGeometry);
		long aPointCount;
		if(FAILED(ipPointCollection->get_PointCount(&aPointCount))){
			std::cerr << "�\���_���̎擾�Ɏ��s" << std::endl;
			return false;
		}
		//�\���_�����b�V�����E�ɐڂ��Ă��邩�̃`�F�b�N(TKY����)
		bool aborderchk = false;
		if( aGeoType == "TKY")
		{
			IPointPtr ipchkPoint;
			int i=0;
			for(i;i < aPointCount;i++)
			{
				//i�Ԗڂ�iPointPtr���擾
				coord aCoord;
				if(FAILED(ipPointCollection->get_Point(i,&ipchkPoint))
				||FAILED(ipchkPoint->get_X(&(aCoord.e_X))) ||FAILED(ipchkPoint->get_Y(&(aCoord.e_Y))))
				{
					std::cout << "�\���_���̎擾�Ɏ��s���܂���" << std::endl;
					return false;
				}
				int X,Y;
				crd_cnv aCrd_cnv;
				if(aCrd_cnv.LLtoMesh2(aCoord.e_X,aCoord.e_Y,aMeshCode,&X,&Y) == -1){
					std::cerr << "�ܓx�o�x�̍��W�ϊ��Ɏ��s���܂���" << std::endl;
					return false;
				}

				//middle�̏ꍇ�̍��W�͈̔͂� 0 <= X <= 2000000, 0 <= Y <= 2000000
				//middle�ȊO�̍��W�͈̔͂� 0 <= X <= 1000000, 0 <= Y <= 1000000
				//���W��胁�b�V�����E����ɐڂ��Ă���I�u�W�F�N�g�����f����B�@
				long MAX_Coord = 0;
				if( (aMeshCode >= 2922) && (aMeshCode <= 6853) ){
					MAX_Coord = 2000000;
				}else 
				{
					MAX_Coord = 1000000;
				}
				if((X == 0) || ( X == MAX_Coord) || ( Y == 0) || ( Y == MAX_Coord))
				{
					aborderchk = true;
					break;
				}
			}
		}
		else if( aGeoType == "JGD")
		{
			aborderchk = true;
			*pCheck = true;
		}

		//�\���_�̏����擾���A�R���e�ipPotinSet(iPointPtr�i�[�R���e�i),pPointInfo(�e��ԓ_���W�i�[�R���e�i) 
		if(aborderchk){
			IPointPtr ipPoint;
			int i=0;
			for (i;i < aPointCount;i++)
			{
				//i�Ԗڂ�iPointPtr���擾
				coord aCoord;
				if(FAILED(ipPointCollection->get_Point(i,&ipPoint))
					||FAILED(ipPoint->get_X(&(aCoord.e_X))) ||FAILED(ipPoint->get_Y(&(aCoord.e_Y))))
				{
					std::cout << "�\���_���̎擾�Ɏ��s���܂���" << std::endl;
					return false;
				}	

				//iPointPtr�i�[�R���e�i�A���W�i�[�R���e�i�ɑ}��
				(pObjectData->e_PointSet).insert(std::pair<long,IPointPtr>(i,ipPoint));
				Insert( &pObjectData->e_PointInfo, &pObjectData->e_CoordInfo,i,aCoord);
			
			}
			if(aGeoType == "TKY"){
			//FID���R���e�i�Ɋi�[
			pOIDSet->insert(aFID);
			*pCheck = true;
			}
		}

		return true;
	}

	bool Insert(std::multimap<long,coord> *pPointInfo, std::multimap<coord,long> *pCoordInfo, long aNum,coord aCoord);



}
