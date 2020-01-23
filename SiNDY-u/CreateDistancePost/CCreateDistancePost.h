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

#include <Workspace.h>
#include <GeometryBase.h>
#include <ArcHelperEx/Mesh.h>
#include "CFeatureClass.h"
#include "RowBase.h"
#include <boost/tuple/tuple.hpp>
#include "tracer/LinkTracer.h"

using namespace std;

namespace sindy
{
	
/**
 * @class CCreateDistancePost
 * @brief �����W�C���|�[�g�N���X
 * @file  CCreateDistancePost.h
 * @author �r�b�g�G�C�W�@�`
 * $Id$
*/
class CCreateDistancePost
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CCreateDistancePost()
	{
	};

	/**
	 * @brief �f�X�g���N�^
	 */
	~CCreateDistancePost(){};

	/**
	 * @brief ����DB�ڑ�
	 * @param lpcszConnection [in] �ڑ�������
	 * @return �ڑ��ł������ǂ���
	*/
	bool Connect(LPCTSTR lpcszConnection);

	/**
	 * @brief Distance_Post_Point�擾
	 * @return �擾�ł������ǂ���
	*/
	bool OpenDistancePostPoint();

	/**
	 * @brief ROAD_LINK/ROAD_NODE�擾
	 * @return �擾�ł������ǂ���
	*/
	bool OpenNetwork();

	/**
	 * @brief ���b�V���擾
	 * @return �擾�ł������ǂ���
	*/
	bool OpenMesh();

	/**
	 * @brief �Ώۓ��H���X�g���擾
	 * @param nLineNo [in] �t�@�C����̍s�ԍ�
	 * @return ���H���X�g���
	*/
	boost::tuple<long, long, CString> GetRoadList(long nLineNo) { return m_mapRoadList[nLineNo]; }

	/**
	 * @brief �H�����X�g�m�F
	 * @param lpcszFile [in] �H�����X�g�t�@�C���p�X
	 * @param dbPitch [in] �s�b�`
	 * @param mapContainer [out] 
	 * @param mapErr [out] �G���[������H���R�[�h�Q
	 * @return ���킩�ǂ��� 0:����A����ȊO�G���[
	*/
	long CheckRoadList(LPCTSTR lpcszFile, double dbPitch, std::map<long, std::pair<std::list<double>, std::map<double, long>>>& mapContainer, std::map<long,long>& mapErr);

	/**
	 * @brief �����W�|�C���g����
	 * @param mapValues [in] �����W�l�Q(OID,�����W)
	*/
	void searchDistancePoint(const std::map<double, long>& mapValues);

	/**
	 * @brief �����W�|�C���g�Q�擾
	 * @return �����W�|�C���g�Q
	*/
	std::map<double, CSPRowBase> findDistancePoints();

	/**
	 * @brief �����W�|�C���g�擾
	 * @param dbVal [in] �����W�l
	 * @return �����W�|�C���g
	*/
	CSPRowBase findDistancePoint(double dbVal);

	/**
	 * @brief �����W�|�C���g����
	 * @param ipGeom [in] �����`��
	 * @param dbBuf [in] �o�b�t�@�l
	 * @return ���HOID�Q
	*/
	std::list<long> searchDistancePostOnRoad(const IGeometryPtr& ipGeom, double dbBuf);

	/**
	 * @brief ���H����
	 * @param ipGeom [in] �����`��
	 * @param dbBuf [in] �o�b�t�@�l
	 * @return ���H�Q
	*/
	CFeatureClass::Container searchRoads(const IGeometryPtr& ipGeom, double dbBuf);

	/**
	 * @brief ���H���O����
	 * @param cRoads [in/out] ���H�Q
	 * @param listNotExcludeRoads [in] ���O���Ă͂����Ȃ����H�Q
	 * @param listRouteIDs [in] �o�H�T���ςݓ��HOID�Q
	*/
	void excludeRoad(CFeatureClass::Container& cRoads, const std::list<long>& listNotExcludeRoads, const std::list<long>& listRouteIDs);

	/**
	 * @brief ���H�����p�`��擾
	 * @param cRoads [in/out] ���H�Q
	 * @param nBase [in] �I�t�Z�b�g�l
	 * @param listExcludeMesh [in] ���O���b�V���Q
	 * @param listMesh [in/out] �����Ώۃ��b�V���R�[�h�Q
	 * @return �����`��
	*/
	IGeometryPtr searchRoadGeoms(const CFeatureClass::Container& cRoads, long nBase, const std::list<long>& listExcludeMesh, std::list<long>& listMesh);

	/**
	 * @brief ���H�����N�t�B�[�`���N���X�擾
	 * @return ���H�����N�t�B�[�`���N���X
	*/
	IFeatureClassPtr getRoadLink() { return m_cRoadLinkClass; }

	/**
	 * @brief �����W�|�C���g�`��쐬
	 * @param ipPolyline [in] ���C���`��
	 * @param dbFirst [in] �n�_
	 * @param dbEnd [in] �I�_
	 * @param dbPitch [in] �s�b�`
	 * @param dbLen [in] �����Ώۋ���
	 * @param nSplitNum [in] ������
	 * @param mapPoints [in/out] �쐬�|�C���g�Q
	 * @param dbLenStart [in] �n�_����(���H�̎n�_����n�_�܂ł̋���)
	 * @param dbLenEnd [in] �I�_����(���H�̏I�_����I�_�܂ł̋���)
	 * @param bPitch [in] �s�b�`�P�ʂŕ������邩�ǂ����i�P�ʍ��킹�����_�ƕ␳�_���̂ݗL���j
	 * @return �쐬�ł������ǂ���
	*/
	bool createDistancePoint(const IPolylinePtr& ipPolyline, double dbFirst, double dbEnd, double dbPitch, double dbLen, long nSplitNum, std::map<double, IPointPtr>& mapPoints, double dbLenStart, double dbLenEnd, bool bPitch);

	/**
	 * @brief �����W�|�C���g�쐬
	 * @param ipPoint [in] �쐬�|�C���g�`��
	 * @param dbVal [in] �����W�l
	 * @param nLineNo [in] �t�@�C���s�ԍ�
	 * @return �����W�|�C���g
	*/
	CSPRowBase createDistancePoint(const IPointPtr& ipPoint, double dbVal, long nLineNo);

	/**
	 * @brief �_�擾�i�����j
	 * @param ipPolyline [in] ���C���`��
	 * @param dbVal [in] ����
	 * @return �����_
	*/
	IPointPtr split_distance(const IPolylinePtr& ipPolyline, double dbVal);

	/**
	 * @brief �����_�擾�i�|�C���g�j
	 * @param ipPolyline [in] ���C���`��
	 * @param ipPoint [in] �����|�C���g
	 * @param dbVal [in] ����
	 * @param dbLen [out] �����i���C���̎n�_���番���_�܂ł̋���)
	 * @return �����_
	*/
	IPointPtr split_point(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint, double dbVal, double& dbLen);

	/**
	 * @brief �ҏW�J�n
	*/
	void startEdit();

	/**
	 * @brief �ҏW�I��
	*/
	void stopEdit();

	/**
	 * @brief �ҏW�j��
	*/
	void abortEdit();

	/**
	 * @brief �R�X�g�t�@�C���ǂݍ���(���H���)
	 * @param lpcszFile [in] �R�X�g�t�@�C���p�X
	*/
	void ReadRoadCost(LPCTSTR lpcszFile);

	/**
	 * @brief �R�X�g�t�@�C���ǂݍ���(�����N���)
	 * @param lpcszFile [in] �R�X�g�t�@�C���p�X
	*/
	void ReadLinkCost(LPCTSTR lpcszFile);

	/**
	 * @brief �P�ʕϊ�
	 * @param dbValue [in] �ϊ����l
	 * @param esriSrc [in] �ϊ����P��
	 * @param esriDest [in] �ϊ���P��
	 * @return �ϊ��l
	*/
	double convert(double dbValue, esriUnits esriSrc, esriUnits esriDest);

	/**
	 * @brief ����(km)�擾
	 * @param ipPolyline [in] ���C���`��
	 * @return ����(km)
	*/
	double GetLength( const IPolylinePtr& ipPolyline);

	/**
	 * @brief �t�B�[���h�ԍ��擾
	 * @param lpcszField [in] �t�B�[���h��
	 * @return �t�B�[���h�ԍ�
	*/
	long GetFieldIndex(LPCTSTR lpcszField);

	/**
	 * @brief COM�I�u�W�F�N�g�̃R�s�[���쐬����
	 * @param ipUnk [in] �N���[����
	 * @return �N���[��
	*/
	static IClonePtr Clone( const IUnknownPtr& ipUnk );

	/**
	 * @brief ���H�����iOID�w��j
	 * @param listRouteIDs [in] OID�Q
	 * @return �������H�Q
	*/
	CFeatureClass::Container searchRoads(const std::list<long>& listRouteIDs);

	/**
	 * @brief �����_�܂ł̋����擾
	 * @param ipPolyline [in] ���C���`��
	 * @param ipPoint [in] �����|�C���g
	 * @return �����_�܂ł̋���
	*/
	double split_point_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint);

	/**
	 * @brief ��������2�_�Ԃ̋������擾
	 * @param ipPolyline [in] ���C���`��
	 * @param ipPoint1 [in] �n�_
	 * @param ipPoint2 [out] ���_
	 * @return ��������2�_�Ԃ̋���
	*/
	double split_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint1, const IPointPtr& ipPoint2);

	/**
	 * @brief �؂�l��
	 * @param dbVal [in] �l
	 * @param nLen [in] ��
	 * @return �؂�l�߂��l
	*/
	CString convertStr(double dbVal, long nLen=2);

	/**
	* @brief �������鋗���W�|�C���g���擾����
	* @param existDistanceList		[in]  ���������W�|�C���g�Q
	* @param mapRows				[in]  ���������W�|�C���g��CRowBase�Q
	* @param ipPrjCoordinateSystem	[in]  ���W�n�ύX�p
	* @param ipAllPolyline			[out] �H���`��i�[�p
	* @param mapPoints				[out] �������鋗���W�|�C���g�i�[�p
	* @param dbPitch				[in]  �����s�b�`
	* @param dbBuff					[in]  �����N����o�b�t�@
	* @param correctionPointLen		[in]  �␳�_�ԋ���
	* @param errorMsg				[out] �G���[���b�Z�[�W�i�[�p
	* @retval true : �擾����(�G���[����)
	* @retval false: �擾���s(�G���[�L��)
	*/
	bool getCreateDistancePost(
		const std::list<double>& existDistanceList, 
		const std::map<double, sindy::CSPRowBase>& mapRows, 
		const IProjectedCoordinateSystemPtr& ipPrjCoordinateSystem, 
		IPolylinePtr& ipAllPolyline,
		std::map<double, IPointPtr>& mapPoints, 
		double dbPitch,
		double dbBuff,
		double correctionPointLen, 
		CString& errorMsg);

private:
	CWorkspace m_cWorkspace;                    ///< ���[�N�X�y�[�X
	CFeatureClass m_cDistancePostClass;         ///< �����W�|�C���g�N���X
	CFeatureClass m_cRoadLinkClass;             ///< ���H�����N�N���X
	CFeatureClass m_cRoadNodeClass;             ///< ���H�m�[�h�N���X
	CFeatureClass m_cBaseMeshClass;             ///< �Q�����b�V���N���X

	std::map<long, boost::tuple<long, long, CString>> m_mapRoadList; ///< �H�����X�g�Q
};

}
