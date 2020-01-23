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
#include "Log.h"
#include "RelPoiTbl.h"
#include "PoiFile.h"

class CCheckWNRelPoiPoint
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param cParam	[in] �p�����[�^
	 */
	CCheckWNRelPoiPoint(CParam& cParam);
	~CCheckWNRelPoiPoint();

	/**
	 * @brief	�`�F�b�N���s
	 * @return			��������
	 */
	bool execute();

	/**
	 * @brief	�G���[�̑��ݗL��
	 * @return			��������
	 */
	bool isError() {return m_isError;};

protected:
	/**
	 * @brief	��������
	 * @return			��������
	 */
	bool init();

	/**
	 * @brief	���b�V���R�[�h�`�F�b�N
	 * @return			��������
	 */
	bool checkMeshCode();

	/**
	 * @brief	SHAPE�f�[�^�̃p�X�쐬
	 * @param	strMesh		[in]	���b�V���R�[�h
	 * @return	std::string			SHAPE�f�[�^�ւ̃p�X
	 */
	std::string makeShapeDirName(const std::string& strMesh);

	/**
	 * @brief	�֌W�`�F�b�N
	 * @param	sShapeDir	[in]	SHAPE�f�[�^�̊�p�X
	 * @param	sMeshcode	[in]	�`�F�b�N�Ώۂ̃��b�V���R�[�h
	 * @param	mapRelTbl	[in]	���H�E���s�҃m�[�h-POI�֘A�e�[�u��
	 * @param	mapPoiData	[in]	POI�f�[�^
	 * @param	bContinue	[out]	�������s����
	 * @return						��������
	 */	
	bool checkRelation(const std::string& sShapeDir,
					   const std::string& sMeshcode,
					   const MAP_REL_TBL& mapRelTbl,
					   const MAP_POI_DATA& mapPoiData,
					   bool& bContinue);

	/**
	 * @brief	�֘A�e�[�u���`�F�b�N
	 * @return			��������
	 */
	bool checkRelTbl();

	/**
	 * @brief	�|�C���g�擾
	 * @param	ipGeometry	[in]	�擾�Ώۂ�Geometry
	 * @param	rWKSPoint	[out]	���W���Z�b�g���ꂽWKSPoint
	 * @return						��������
	 */	
	bool getPoint(const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint );

	/**
	 * @brief	�����`�F�b�N
	 * @param	poiPoint	[in]	POI�f�[�^���W
	 * @param	wnPoint		[in]	���H�E���s�҃m�[�h���W
	 * @param	dDistance	[out]	POI�f�[�^���W�̓��H�E���s�҃m�[�h���W�ԋ���
	 * @return						��������
	 */	
	bool checkDiscance(const WKSPoint& poiPoint, const WKSPoint& wnPoint, double& dDistance);

private:
	bool m_isError;							// �G���[���ݗL��

	CParam m_CParam;						// �p�����[�^���
	crd_cnv m_Ccrd_cnv;						// �n�����W�ϊ��N���X

	std::string m_sRelTblLayer;				// �`�F�b�N�Ώۊ֘A�e�[�u�����C����
	MAP_REL_TBL m_mapRelTbl;				// �`�F�b�N�Ώۊ֘A�e�[�u���R���e�i
	MAP_POI_DATA m_mapPoiData;				// POI�f�[�^�R���e�i
	MAP_REL_TBL m_mapMwnChk;				// ���H�E���s�҃f�[�^�p�R���e�i
	std::vector<std::string> m_vecMeshList;	// ���b�V�����X�g�R���e�i
};
