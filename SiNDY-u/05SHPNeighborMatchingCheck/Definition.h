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
#include<vector>
#include<map>
#include<set>

//�ݒ�t�@�C������ǂݍ��ރp�����[�^
struct Parameters
{
	std::string e_MeshList;			//�`�F�b�N�Ώۃ��b�V�����X�g
	std::string e_LayerList;		//�`�F�b�N�Ώۃ��C�����X�g
	std::string e_TKYDataPath;		//���f�[�^(�������n�n)�̃f�[�^���i�[����Ă���f�B���N�g��
	std::string e_JGDDataPath;		//�ϊ���f�[�^(���E���n�n)�̃f�[�^���i�[����Ă���f�B���N�g��
	std::string e_Area;				//�f�[�^�S�Ẵ��b�V�����`�F�b�N����ꍇ:all �ꕔ���`�F�b�N���s���ꍇ:part
	std::string e_DataSet;			//�`�F�b�N�Ώۃf�[�^�Z�b�g(top,middle,base,city,road)
	std::string e_ErrorDir;			//�G���[�t�@�C���̏o�̓t�H���_
	double		e_Border;			//�p�x�̋��e�덷�ݒ�
	bool		e_WalkFlag;			//���H�E���s�҃f�[�^���ǂ���(�I�u�W�F�N�g�L�[:OBJECTID��DIVID)
};
//���W
struct coord{
	double	e_X;
	double	e_Y;
	bool operator < (const coord &rCoord) const{
		if( e_X != rCoord.e_X){
			return e_X < rCoord.e_X;
		}else
		{
			return e_Y < rCoord.e_Y;
		}
	}
	bool operator == (const coord &rCoord) const{
		if( e_X == rCoord.e_X){
			return e_Y == rCoord.e_Y;
		}
		else{
			return false;
		}
	}
};

//TKY�I�u�W�F�N�g�����i�[���邽�߂̍\����
struct TKYObjectData{
	long e_FID;
	esriGeometryType e_ShapeType;
	std::map<long,IPointPtr> e_PointSet;
	std::multimap<long,coord> e_PointInfo;					//��ԓ_�ԍ�����̍��W�����p
	std::multimap<coord,long> e_CoordInfo;					//���W����̕�ԓ_�ԍ������p
	bool e_multi;												//�|���S���������̃����O�ō쐬����Ă��邩����p
};

//JGD�I�u�W�F�N�g�����i�[���邽�߂̍\����
struct JGDObjectData{
	long e_FID;
	esriGeometryType e_ShapeType;
	std::map<long,IPointPtr> e_PointSet;
	std::multimap<long,coord> e_PointInfo;					//��ԓ_�ԍ�������W�����p
	std::multimap<coord,long> e_CoordInfo;					//���W�����ԓ_�ԍ������p
	bool e_multi;												//�|���S���������̃����O�ō쐬����Ă��邩����p
};

//�I�u�W�F�N�g�̃f�t�H���g�l
const long DEFAULT_VALUE = -1;

//TKY,JGD�̃��b�V�����̃L�[�̒l���i�[���邽�߂̍\����
struct ObjectDataKey{
	long e_OID;			//OBJECTID�������͂��ꑊ����ID
	long e_DIVID;		//DIVID(���H�E���s�҃f�[�^�̏ꍇ�̂ݎg�p)

	ObjectDataKey(void):e_OID(DEFAULT_VALUE),e_DIVID(DEFAULT_VALUE){};

	//��r�֐�
	//std::map�œƎ��̌^(�\���̂Ȃ�)���L�[�ɂ���ɂ͍\���̓��̔�r�֐����K�v�Binsert()��find()�Ŏg�p�����
	bool operator< (const ObjectDataKey& aObjectDataKey) const
	{
		//e_OID��r
		if(e_OID < aObjectDataKey.e_OID)
			return true;
		if(e_OID > aObjectDataKey.e_OID)
			return false;
		//e_DIVID��r
		if(e_DIVID < aObjectDataKey.e_DIVID)
			return true;
		if(e_DIVID > aObjectDataKey.e_DIVID)
			return false;
		return false;
	};
};

//TKY,JGD�̃��b�V�������i�[���邽�߂̍\����
//multimap�ɂ��Ȃ��Ⴞ�߁I�I�I�I�I
typedef std::map<ObjectDataKey,TKYObjectData> TKYMeshData;
typedef std::map<ObjectDataKey,JGDObjectData> JGDMeshData;


struct TKYMeshInfo{
	long e_MeshCode;
	TKYMeshData e_TKYMeshData;
};

struct JGDMeshInfo{
	long e_MeshCode;
	JGDMeshData e_JGDMeshData;
};


template <class Type>
struct FiveMeshInfo
{
	Type e_MainMesh;
	Type e_NorthMesh;
	Type e_SouthMesh;
	Type e_EastMesh;
	Type e_WestMesh;
};


//��r���X�g�쐬�ׂ̈̍\����
struct CheckKey{
	long e_MMeshCode;			//���ڃ��b�V���̃��b�V���R�[�h
	long e_MOID;				//���ڃ��b�V������OID
	long e_MDIVID;				//���ڃ��b�V������DIVID(���H�E���s�҃f�[�^�̏ꍇ�̂ݎg�p)
	long e_MPointNum;			//���ڃ��b�V�����̕�ԓ_�ԍ�
	long e_NMeshCode;			//�אڃ��b�V���̃��b�V���R�[�h
	long e_NOID;				//�אڃ��b�V������OID
	long e_NDIVID;				//�אڃ��b�V������DIVID(���H�E���s�҃f�[�^�̏ꍇ�̂ݎg�p)
	long e_NPointNum;			//�אڃ��b�V�����̕�ԓ_�ԍ�
	long e_MFID;				//���ڃ��b�V������FID
	long e_NFID;				//�אڃ��b�V������FID

	bool operator < (const CheckKey & rCheckKey) const {
		if (e_MMeshCode != rCheckKey.e_MMeshCode){
			return (e_MMeshCode < rCheckKey.e_MMeshCode);
		}
		else if (e_NMeshCode != rCheckKey.e_NMeshCode)
		{
			return (e_NMeshCode < rCheckKey.e_NMeshCode );

		}
		else if (e_MOID != rCheckKey.e_MOID)
		{
			return (e_MOID < rCheckKey.e_MOID);
		}
		else if (e_MPointNum != rCheckKey.e_MPointNum)
		{
			return (e_MPointNum < rCheckKey.e_MPointNum);
		}
		else if (e_NOID != rCheckKey.e_NOID)
		{
			return (e_NOID < rCheckKey.e_NOID);
		}
		else if (e_NPointNum != rCheckKey.e_NPointNum)
		{
			return (e_NPointNum < rCheckKey.e_NPointNum);
		}
		else if (e_MFID != rCheckKey.e_MFID)
		{
			return (e_MFID < rCheckKey.e_MFID);
		}
		else
		{
			return (e_NFID < rCheckKey.e_NFID);
		}

	}
};

struct FIDPair{
	long e_MFID;
	long e_NFID;
};

struct CheckElement{

	double e_arg1;
	double e_arg2;
	double e_arg3;
	double e_arg4;

	bool operator != (const CheckElement & rCheckElement) const{
		if( e_arg1 != rCheckElement.e_arg1 || e_arg2 != rCheckElement.e_arg2 
			|| e_arg3 != rCheckElement.e_arg3 || e_arg4 != rCheckElement.e_arg4)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};

typedef std::map<CheckKey,CheckElement> CheckList;

typedef std::set<long> FID_Set;

