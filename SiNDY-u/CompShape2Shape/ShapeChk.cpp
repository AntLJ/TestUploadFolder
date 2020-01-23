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

#include "ShapeChk.h"

CShapeChk::~CShapeChk(void)
{
}
/**
 * @brief	�`��`�F�b�N���s
 * @return	bool
 */
bool CShapeChk::app(void)
{
	//shape�ǂݍ���(��r��)
	ShapeInfo a_OriShapeInfo;
	int a_OriShapeType = 0;
	if(! load_shape_info(&a_OriShapeInfo,m_sFullPassOri,&a_OriShapeType))
		return false;
	//shape�^�C�v���|�C���g�������ꍇ�A�`�F�b�N�����Ȃ�
	if( a_OriShapeType == SHPT_POINT ){
		return true;
	}
	//shape�ǂݍ���(��r��)
	ShapeInfo a_DestShapeInfo;
	int a_DestShapeType = 0;
	load_shape_info(&a_DestShapeInfo,m_sFullPassDest,&a_DestShapeType);
	if( a_DestShapeType != a_OriShapeType ){
		cerr << "#ERROR ��r��<" << m_sFullPassOri.c_str() << ">�Ɣ�r��<" << m_sFullPassDest.c_str() << ">��Shpae�^�C�v���قȂ�܂��B" << endl;
		return false;
	}
	//2��shape�̌`���r
	if(!comp_Shp_info(a_OriShapeInfo,a_DestShapeInfo))
	{
		return false;
	}
	
	return true;

}
/**
* @brief	Shape�t�@�C����ǂݍ���
* @param	string	[in] �Ώ�Shape�t�@�C���̃t���p�X
* @return	bool
*/
bool CShapeChk::load_shape_info(ShapeInfo* p_ShapeInfo, const string& r_sFullPath,int* p_ShapeType)
{
	CSHPHandle		a_cSHPHdl;
	CDBFHandle		a_cDBFHdl;
	//Shape DBF�t�@�C���̃I�[�v��
	if( !a_cSHPHdl.open((char*)r_sFullPath.c_str(),"rb") || !a_cDBFHdl.open((char*)r_sFullPath.c_str(),"rb"))
	{
		cerr << "#ERROR SHP�t�@�C��<" << r_sFullPath.c_str() << ">�̌`��̃I�[�v���Ɏ��s!" << endl;
		return false;
	}
	//Shape�̃I�u�W�F�N�g���E�^�C�v���擾
	int aShpCount=0;
	double min_bound[4],max_bound[4];
	if( !a_cSHPHdl.get_FileInfo(&aShpCount,p_ShapeType,min_bound,max_bound))
	{
		cerr << "#ERROR SHP�t�@�C��<" << r_sFullPath.c_str() << ">�̃I�u�W�F�N�g���擾�Ɏ��s!" << endl;
		return false;
	}
	//Shape�^�C�v���|�C���g�������ꍇ�A�����I���B
	if( *p_ShapeType == SHPT_POINT)
		return true;
	//DBF�̃I�u�W�F�N�g�����擾
	int aDbfCount = a_cDBFHdl.get_RecordCount();
	if( aShpCount != aDbfCount ){
		cerr << "#ERROR <" << r_sFullPath.c_str() << ">��.shp��.dbf�̃��R�[�h������v���܂���!" << endl;
		return false;
	}
	//�I�u�W�F�N�g�����A�`��̏��A�����̏��擾
	for(int i=0; i<aShpCount; ++i)
	{
		ObjectInfo a_ObjectInfo;
		//�`����̎擾
		if( !get_Obj_info(&a_ObjectInfo,a_cSHPHdl,i)){
			cerr << "#ERROR SHP�t�@�C��<" << r_sFullPath.c_str() << ">�̌`����擾�Ɏ��s!" << endl;
			continue;
		}
		int OID;
		//�������(�I�u�W�F�N�gID�܂���FID���܂�)�̎擾
		if( !get_Dbf_info(&OID,&a_ObjectInfo,a_cDBFHdl,i)){
			cerr << "#ERROR SHP�t�@�C��<" << r_sFullPath.c_str() << ">�̑������擾�Ɏ��s!" << endl;
			continue;
		}
		//�I�u�W�F�N�g�����R���e�i�ɑ}��
		p_ShapeInfo->insert(pair<int,ObjectInfo>(OID,a_ObjectInfo));
	}
	//SHP,DBF�t�@�C���̃N���[�Y
	a_cSHPHdl.close();
	a_cDBFHdl.close();
	return true;
}
/**
* @brief	SHP�̌`�����ǂݍ���
* @param	ObjectInfo* [in] �I�u�W�F�N�g���
* @param	CSHPHandle [in] �Ώ�Shape�̃n���h�����O�N���X
* @param	int	[in] �Ώ�Object��Index
* @return	bool
*/
bool CShapeChk::get_Obj_info(ObjectInfo* pObjectInfo,CSHPHandle &r_cSHPHdl, int a_index)
{
	CSHPObject aObj;
	//�`��擾
	if( -1 == r_cSHPHdl.read_Object(a_index,&aObj))
		return false;
	//�p�[�g���̎擾
	pObjectInfo->e_PartCount = aObj.get_PartCount();
	//�p�[�g���̎n�_�C���f�b�N�X�擾
	for(int i=0; i<pObjectInfo->e_PartCount; ++i){
		//�p�[�g�̏��擾
		CSHPPart aSHPPart;
		aObj.get_Part(i,&aSHPPart);
		int aStartIndex;
		int aType;
		aSHPPart.get_Part(&aStartIndex,&aType);
		pObjectInfo->e_PartIndexInfo.insert(pair<int,int>(i,aStartIndex));
	}
	//��ԓ_���̎擾
	pObjectInfo->e_PointCount = aObj.get_PointCount();
	//��ԓ_�����̏��擾
	for(int j=0; j<pObjectInfo->e_PointCount; ++j)
	{
		//�|�C���g���擾
		CSHPPoint a_Point;
		aObj.get_Point(j,&a_Point);
		//���W�擾
		d_lonlat a_lonlat;
		a_lonlat.lon = a_Point.get_X();
		a_lonlat.lat = a_Point.get_Y();
		//�R���e�i�ɏ���}��
		pObjectInfo->e_PointsInfo.insert( pair<int,d_lonlat>(j,a_lonlat));
	}
	return true;
}
/**
* @brief	SHP�̑�������ǂݍ���
* @param	int*	[out] �I�u�W�F�N�gID�܂���FID
* @param	ObjectInfo* [out] �I�u�W�F�N�g���
* @param	CDBFHandle [in] �Ώ�Shape�̃n���h�����O�N���X
* @param	int [in] �Ώ�Object��Index
* @return	bool
*/
bool CShapeChk::get_Dbf_info(int *pID, ObjectInfo *pObjectInfo, CDBFHandle &r_cDBFHdl, int a_index)
{
	//�I�u�W�F�N�gID or FID �̓ǂݍ���
	if( m_iExeMode & ON_FID_MODE )
		//FID�̓ǂݍ���
		*pID = a_index;
	else {
		//�I�u�W�F�N�gID�̓ǂݍ���
		int a_FieldIndex = r_cDBFHdl.get_FieldID("OBJECTID");
		r_cDBFHdl.read_Attribute(a_index, a_FieldIndex, pID);
	}
	return true;
}
/**
* @brief	SHP�̌`���r���s��
* @param	ShapeInfo&	[in] ��r����SHP���
* @param	ShapeInfo&	[in] ��r���SHP���
* @return	bool
*/
bool CShapeChk::comp_Shp_info(ShapeInfo &r_OriShapeInfo, ShapeInfo &r_DestShapeInfo)
{
	int iErrFlag = 0;
	//��r��Shape�̌`�����Ƀ`�F�b�N���s���B
	ShapeInfo::iterator ori_itr = r_OriShapeInfo.begin();
	for( ;ori_itr != r_OriShapeInfo.end(); ++ori_itr ){
		//�`�F�b�N�Ώ�Shape����r��ɑ��݂��邩�`�F�b�N
		ShapeInfo::iterator dest_itr = r_DestShapeInfo.find(ori_itr->first);
		if( dest_itr == r_DestShapeInfo.end()){
			*m_pErrorLog
				<< "0\t" << m_sLayerName.c_str() << "\t" << ori_itr->first << "\t"
				<< ori_itr->second.e_PointsInfo[0].lon << "\t" << ori_itr->second.e_PointsInfo[0].lat << "\tERROR\t"
				<< "DATAFINDERR(�`��)\t�Ή�����I�u�W�F�N�g����r��ɑ��݂��܂���\t"
				<< m_MeshCode << endl;
			iErrFlag = 1;
			continue;
		}
		//��ԓ_������v���邩�`�F�b�N
		if( ori_itr->second.e_PointCount != dest_itr->second.e_PointCount ){
			*m_pErrorLog
				<< "0\t" << m_sLayerName.c_str() << "\t" << ori_itr->first << "\t"
				<< ori_itr->second.e_PointsInfo[0].lon << "\t" << ori_itr->second.e_PointsInfo[0].lat << "\tERROR\t"
				<< "POINTNUMERR(�`��)\t��ԓ_�̌����قȂ��Ă��܂�(��r��:" << ori_itr->second.e_PointCount << "��r��:" << dest_itr->second.e_PointCount << ")\t"
				<< m_MeshCode << endl;
			iErrFlag = 1;
			continue;
		}
		//��ԓ_���������ł��邱�Ƃ��`�F�b�N
		if( ori_itr->second.e_PointCount == 1){
			*m_pErrorLog
				<< "0\t" << m_sLayerName.c_str() << "\t" << ori_itr->first << "\t"
				<< ori_itr->second.e_PointsInfo[0].lon << "\t" << ori_itr->second.e_PointsInfo[0].lat << "\tERROR\t"
				<< "POINTNUMERR(�`��)\t��ԓ_����1�ł�" 
				<< m_MeshCode << endl;
			iErrFlag = 1;
			continue;
		}
		//��������v���邩�`�F�b�N
		double a_DistDiff;
		DistInfo a_DistInfo = {0.0,0.0};
		//�����̎擾���@���}���`�p�[�g�Ƃ���ȊO�ŕ�����
		int aCount = ori_itr->second.e_PartCount;
		if(ori_itr->second.e_PartCount > 1){
			Shp_Dist_chk_multi(&a_DistDiff,&a_DistInfo, ori_itr->second,dest_itr->second);
		}else{
			Shp_Dist_chk(&a_DistDiff,&a_DistInfo, ori_itr->second,dest_itr->second);
		}

		//�����̍��̃p�[�Z���e�[�W�����߂�
		double a_DistDiffPar = a_DistDiff / (a_DistInfo.e_DestDist / 100);

		//�p�[�Z���e�[�W���{�[�_�[���ゾ������NG
		if( a_DistDiffPar > m_dDistBoder ){
			*m_pErrorLog
				<< "0\t" << m_sLayerName.c_str() << "\t" << ori_itr->first << "\t"
				<< ori_itr->second.e_PointsInfo[0].lon << "\t" << ori_itr->second.e_PointsInfo[0].lat << "\tERROR\t"
				<< "DISTERR(�`��)\t�I�u�W�F�N�g�̋������قȂ�܂�\t"
				<< m_MeshCode << "\t" << a_DistInfo.e_OriDist << "\t" << a_DistInfo.e_DestDist << "\t" <<  a_DistDiff << "\t" << a_DistDiffPar << endl;
			iErrFlag = 1;
			continue;
		}
	}

	if (iErrFlag == 1)
	{
		return false;
	}
	else 
	{
		return true;
	}
}
/**
* @brief	�I�u�W�F�N�g�̋����`�F�b�N���s��
* @param	double		[out] ��r���Ɣ�r��̃I�u�W�F�N�g�̋����̍�
* @param	DistInfo*	[out] ��r���E��r��̃I�u�W�F�N�g�̋����i�[
* @param	ObjectInfo&	[in] ��r���̃I�u�W�F�N�g���
* @param	ObjectInfo&	[in] ��r��̃I�u�W�F�N�g���
* @return	bool
*/
bool CShapeChk::Shp_Dist_chk(double* p_DistDiff, DistInfo* p_DistInfo, ObjectInfo &r_OriObjectInfo, ObjectInfo &r_DestObjectInfo)
{
	//p_DistDiff(�����̍��v�l)�̏�����
	*p_DistDiff = 0.0;
	//�Z�O�����g���m�̋����̍��̐�Βl�����ߍ��v���o��
	crd_cnv a_Crd;
	PointsInfo::iterator ori_itr = r_OriObjectInfo.e_PointsInfo.begin();
	PointsInfo::iterator dest_itr = r_DestObjectInfo.e_PointsInfo.begin();
	for(int i=0; i<r_OriObjectInfo.e_PointCount-1; ++i){
		//��r���A��r��I�u�W�F�N�g�̃Z�O�����g�̋������o��(�P��m)
		//��r���A��r��Z�O�����g�̎n�_�A�I�_�擾
		d_lonlat a_OriFrom = ori_itr->second;
		d_lonlat a_OriTo = (++ori_itr)->second;
		d_lonlat a_DestFrom = dest_itr->second;
		d_lonlat a_DestTo = (++dest_itr)->second;
		//�����擾
		double a_OriSegDist = a_Crd.GetDist(a_OriFrom,a_OriTo);

		//���E���n�n�̋����̏ꍇ�́A���C�u�������g�p
		double a_DestSegDist = a_Crd.GetDist_JGD2000(a_DestFrom.lon,a_DestFrom.lat,a_DestTo.lon,a_DestTo.lat);

		//�e�Z�O�����g�̍��̐�Βl������,p_DistDiff�ɑ����B
		*p_DistDiff += fabs( a_OriSegDist - a_DestSegDist );
		//�e�Z�O�����g�̋����𑍋����ɑ���
		p_DistInfo->e_OriDist += a_OriSegDist;
		p_DistInfo->e_DestDist += a_DestSegDist;
	}
	return true;
}
/**
* @brief	�I�u�W�F�N�g�̋����`�F�b�N���s��(�}���`�p�[�g�p)
* @param	double		[out] ��r���Ɣ�r��̃I�u�W�F�N�g�̋����̍�
* @param	DistInfo*	[out] ��r���E��r��̃I�u�W�F�N�g�̋����i�[
* @param	ObjectInfo&	[in] ��r���̃I�u�W�F�N�g���
* @param	ObjectInfo&	[in] ��r��̃I�u�W�F�N�g���
* @return	bool
*/
bool CShapeChk::Shp_Dist_chk_multi(double *p_DistDiff, DistInfo *p_DistInfo, ObjectInfo &r_OriObjectInfo, ObjectInfo &r_DestObjectInfo)
{
	//p_DistDiff(�����̍��v�l)�̏�����
	*p_DistDiff = 0.0;
	//�Z�O�����g���m�̋����̍��̐�Βl�����ߍ��v���o��
	crd_cnv a_Crd;
	PointsInfo::iterator ori_itr = r_OriObjectInfo.e_PointsInfo.begin();
	PointsInfo::iterator dest_itr = r_DestObjectInfo.e_PointsInfo.begin();
	//�p�[�g���J��Ԃ��B
	for(int i=0;i<r_OriObjectInfo.e_PartCount;++i){
		//part�̎n�_�E�I�_Index�擾
		int aStartIndex;
		int aEndIndex;
		if( i==0 )
			aStartIndex =0;
		else 
			aStartIndex = r_OriObjectInfo.e_PartIndexInfo[i];
		if( i==(r_OriObjectInfo.e_PartCount-1) )
			aEndIndex = (r_OriObjectInfo.e_PointCount-1);
		else
			aEndIndex = r_OriObjectInfo.e_PartIndexInfo[i+1]-1;
		//part�̎n�_����I�_�܂ł̃Z�O�����g���̋������擾
		for( int j=aStartIndex; j<aEndIndex; ++j){
			//��r���A��r��I�u�W�F�N�g�̃Z�O�����g�̋������o��(�P��m)
			//��r���A��r��Z�O�����g�̎n�_�A�I�_�擾
			d_lonlat a_OriFrom = ori_itr->second;
			d_lonlat a_OriTo = (++ori_itr)->second;
			d_lonlat a_DestFrom = dest_itr->second;
			d_lonlat a_DestTo = (++dest_itr)->second;
			//�����擾
			double a_OriSegDist = a_Crd.GetDist(a_OriFrom,a_OriTo);
			
			//���E���n�n�̋����̏ꍇ�́A���C�u�������g�p
			double a_DestSegDist = a_Crd.GetDist_JGD2000(a_DestFrom.lon,a_DestFrom.lat,a_DestTo.lon,a_DestTo.lat);

			//�e�Z�O�����g�̍��̐�Βl������,p_DistDiff�ɑ����B
			*p_DistDiff += fabs( a_OriSegDist - a_DestSegDist );
			//�e�Z�O�����g�̋����𑍋����ɑ���
			p_DistInfo->e_OriDist += a_OriSegDist;
			p_DistInfo->e_DestDist += a_DestSegDist;
		}
		//���̃p�[�g�Ɉڂ�ׁAPointsInfo�̃C�e���[�^��1�i�߂�
		++ori_itr;
		++dest_itr;		
	}


	return true;
}
