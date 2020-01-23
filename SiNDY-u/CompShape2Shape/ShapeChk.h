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
#include "stdafx.h"


using namespace std;
typedef map<int,d_lonlat>	PointsInfo;	//�I�u�W�F�N�g�̌`��(�_��)���(��ԓ_�ƍ��W)
typedef map<int,int>	PartIndexInfo; //�I�u�W�F�N�g�̊e�p�[�g�̃C���f�b�N�X���

struct ObjectInfo{
	PointsInfo e_PointsInfo;
	int e_PointCount;
	int e_PartCount;
	PartIndexInfo e_PartIndexInfo;
};										
typedef map<int,ObjectInfo> ShapeInfo;	//SHP�̃I�u�W�F�N�g���
//��r���E��r�拗�����i�[�p�\����
struct DistInfo{
	double e_OriDist;
	double e_DestDist;
};

class CShapeChk
{
public:
	CShapeChk(ofstream* p_ErrorLog,int aMeshCode,int a_iExeMode, string& a_sLayerName, string& a_sFullPathOri,string& a_sFullPathDest,double a_dDistBoder)
	{
		m_pErrorLog		= p_ErrorLog;
		m_MeshCode		= aMeshCode;
		m_iExeMode		= a_iExeMode;
		m_sLayerName	= a_sLayerName;
		m_sFullPassOri	= a_sFullPathOri;
		m_sFullPassDest = a_sFullPathDest;
		m_dDistBoder	= a_dDistBoder;
		
	}
	~CShapeChk(void);
	/**
	 * @brief	�`��`�F�b�N���s
	 * @return	bool
	 */
	bool	app(void);

private:
	/**
	* @brief	Shape�t�@�C����ǂݍ���
	* @param	string	[in] �Ώ�Shape�t�@�C���̃t���p�X
	* @return	bool
	*/
	bool	load_shape_info(ShapeInfo* p_ShapeInfo, const string& r_sFullPath, int* p_ShapeType);

	/**
	* @brief	SHP�̌`�����ǂݍ���
	* @param	ObjectInfo* [in] �I�u�W�F�N�g���
	* @param	CSHPHandle [in] �Ώ�Shape�̃n���h�����O�N���X
	* @param	int	[in] �Ώ�Object��Index
	* @return	bool
	*/
	bool	get_Obj_info(ObjectInfo* pObjectInfo,CSHPHandle& r_cSHPHdl,int a_index);

	/**
	* @brief	SHP�̑�������ǂݍ���
	* @param	int*	[out] �I�u�W�F�N�gID�܂���FID
	* @param	ObjectInfo* [out] �I�u�W�F�N�g���
	* @param	CDBFHandle [in] �Ώ�Shape�̃n���h�����O�N���X
	* @param	int [in] �Ώ�Object��Index
	* @return	bool
	*/
	bool	get_Dbf_info(int* pID,ObjectInfo* pObjectInfo, CDBFHandle& r_cDBFHdl,int a_index);

	/**
	* @brief	SHP�̌`���r���s��
	* @param	ShapeInfo&	[in] ��r����SHP���
	* @param	ShapeInfo&	[in] ��r���SHP���
	* @return	bool
	*/
	bool	comp_Shp_info(ShapeInfo& r_OriShapeInfo, ShapeInfo& r_DestShapeInfo);

	/**
	* @brief	�I�u�W�F�N�g�̋����`�F�b�N���s��
	* @param	double*		[out] ��r���Ɣ�r��̃I�u�W�F�N�g�̋����̍�
	* @param	DistInfo*	[out] ��r���E��r��̃I�u�W�F�N�g�̋����i�[
	* @param	ObjectInfo&	[in] ��r���̃I�u�W�F�N�g���
	* @param	ObjectInfo&	[in] ��r��̃I�u�W�F�N�g���
	* @return	bool
	*/
	bool	Shp_Dist_chk(double* p_DistDiff, DistInfo* p_DistInfo, ObjectInfo& r_OriObj3ctInfo, ObjectInfo& r_DestObjectInfo);

	/**
	* @brief	�I�u�W�F�N�g�̋����`�F�b�N���s��(�}���`�p�[�g�p)
	* @param	double*		[out] ��r���Ɣ�r��̃I�u�W�F�N�g�̋����̍�
	* @param	DistInfo*	[out] ��r���E��r��̃I�u�W�F�N�g�̋����i�[
	* @param	ObjectInfo&	[in] ��r���̃I�u�W�F�N�g���
	* @param	ObjectInfo&	[in] ��r��̃I�u�W�F�N�g���
	* @return	bool
	*/
	bool	Shp_Dist_chk_multi(double* p_DistDiff, DistInfo* p_DistInfo, ObjectInfo& r_OriObj3ctInfo, ObjectInfo& r_DestObjectInfo);

private:
	ofstream*	m_pErrorLog;	
	int			m_MeshCode;
	int			m_iExeMode;				// 0:OBJECTID�ŕR�t���A1:FID�ŕR�t��
	double		m_dDistBoder;
	string		m_sLayerName;
	string		m_sFullPassOri;
	string		m_sFullPassDest;
	
	
};
