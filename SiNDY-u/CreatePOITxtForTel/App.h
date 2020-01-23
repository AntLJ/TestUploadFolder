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

#include "MasterDBCache.h"
#include "MasterListCache.h"
#include "Arguments.h"
#include "POIPointRecord.h"
#include "POISubPointRecord.h"
#include "Output.h"

typedef std::pair<CString, CString> CODEINFO;

class CApp
{
public:
	CApp(void) :
		m_lContentsCode_FID(-1), m_lSeq_FID(-1), m_lName_FID(-1), m_lTel_FID(-1), m_lDelete_FID(-1), m_lCityCode_FID(-1), m_lAddrCode_FID(-1), m_lAddrCode2_FID(-1),
		m_funcSeq_FID(-1), m_pstAttCode_FID(-1), m_priority_FID(-1), m_priorityAttCode_FID(-1), m_tollRoad_FID(-1), m_poiPointId_FID(-1), m_idnCode_FID(-1)
	{}
	~CApp(void){}

	/**
	 * @brief	�g�p�@
	 */
	static void Usage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool Init( int argc, _TCHAR* argv[] );

	/**
	 * @brief	���s
	 */
	void Run();

private:

	/**
	 * @brief	POI_POINT���R�[�h�i�[
	 * @param	ipFeature	[in]	POI�t�B�[�`��
	 * @param	pMaster		[in]	�}�X�^�[���
	 * @param	pUsable		[in]	���p���
	 */
	void SetPOI( IFeaturePtr ipFeature, const POIMASTERDBINFO* pMaster, const POIUSABLEINFO* pUsable );

	/**
	 * @brief	POI_SUB_POINT���R�[�h�i�[
	 * @param	poiPointId	[in]	POI_POINT_ID
	 */
	void SetPOISub( const long poiPointId );

	/**
	 * @brief	�s���E���擾
	 * @param	ipGeo		[in]		POI�ʒu
	 * @param	rCodeInfo	[in, out]	�Z���R�[�h���
	 * @retval	true  : ����
	 * @retval	false : ���s
	 */
	bool GetCityAdminInfo( IGeometryPtr ipGeo, CODEINFO& rCodeInfo );

	/**
	 * @brief	�ƌ`���擾
	 * @param	ipGeom	[in]	POI�ʒu
	 * @param	buildingID [in,out]	�ƌ`ID
	 * @retval	true  : �擾����
	 * @retval	false : �擾���s
	 */
	bool GetBuildingInfo( const IGeometryPtr& ipGeom, long& buildingID );

	/**
	 * @brief  POI_SUB_POINT����f�[�^��I��
	 * @note   DELETE_C=0(����)�̃t�B�[�`���������o�ϐ� m_mapPoiSub �Ɋi�[
	 * @retval true  : �擾����
	 * @retval false : �擾���s
	 */
	bool CollectPoiSubPointInfo();

private:
	CArguments	m_cArgs;	//!< ���s���I�v�V����

	IFeatureClassPtr		m_ipPOIPointFC;		//!< POI_POINT
	long	m_lContentsCode_FID, m_lSeq_FID, m_lName_FID, m_lTel_FID, m_lAcc_FID, m_lDelete_FID; //!< POI_POINT�t�B�[���h�C���f�b�N�X(�R���e���c�}�X�^ID, �V�[�P���X�A���́A�d�b�ԍ��A���x�R�[�h�A�폜�R�[�h)

	IFeatureClassPtr m_ipPOISubPointFC; //!< POI_SUB_POINT
	long m_funcSeq_FID, m_pstAttCode_FID, m_priority_FID, m_priorityAttCode_FID, m_tollRoad_FID, m_poiPointId_FID, m_idnCode_FID; //!< POI_SUB_POINT�t�B�[���h�C���f�b�N�X

	IFeatureClassPtr		m_ipCityAdminFC;	//!< CITY_ADMIN
	long	m_lCityCode_FID, m_lAddrCode_FID, m_lAddrCode2_FID;	//!< CITY_ADMIN�t�B�[���h�C���f�b�N�X(�s�撬���R�[�h, �Z���R�[�h�A�Z���R�[�h2)
	IFeatureClassPtr		m_ipBuildingFC;		///!< BUILDING

	CMasterDBCache			m_cDBCache;		//!< �}�X�^�[���
	CMasterListCache		m_cListCache;	//!< ��ʕ\���X�g���

	std::multiset<POIPointRecord> m_setPOI; //!< POI���R�[�h���(�ʒu�i������E�s���|�C���g��)
	std::multiset<POIPointRecord> m_setImpPOI; //!< POI���R�[�h���(�d�v�{��)
	std::multiset<POISubPointRecord> m_setPOISub; //!< POI_SUB_POINT���R�[�h���

	std::map<long, std::list<IFeaturePtr>> m_mapPoiSub; //!< IDNCODE�ōi�肱��POI_SUB_POINT�i�[�p key:POI_POINT_ID, value:Feature

	CString m_idnCode; //!< �I�v�V�����Ŏw�肳�ꂽIDNCODE(�J���}������΂��̂܂�)
	std::map<long, long> m_idnCodeCounter; //!< �L�[:IDNCODE(������) �l:���R�[�h�Ƃ��ďo�͂��ꂽ��

	Output m_outPOI; //!< POI_POINT�o��
	Output m_outPOISub; //!< POI_SUB_POINT�o��

};
