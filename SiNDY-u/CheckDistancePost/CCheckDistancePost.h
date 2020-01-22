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
#include "CLogger.h"

using namespace std;

namespace sindy
{
	
/**
 * @class CCheckDistancePost
 * @brief �����W�`�F�b�N�c�[���N���X
 * @file  CCheckDistancePost.h
 * @author �r�b�g�G�C�W�@�`
 * $Id$
*/
class CCheckDistancePost
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CCheckDistancePost()
	{
	}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~CCheckDistancePost(){}

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
	 * @param mapValues [in] �����W�l�Q(�����W�l, OID)
	*/
	void searchDistancePoint(const std::map<double, long>& mapValues);

	/**
	 * @brief �����W�|�C���g�Q�擾
	 * @return �����W�|�C���g�Q(�����W�l, CSRowBase)
	*/
	std::map<double, CSPRowBase> findDistancePoints();

	/**
	 * @brief �����W�|�C���g�擾
	 * @param dbVal [in] �����W�l
	 * @return �����W�|�C���g
	*/
	CSPRowBase findDistancePoint(double dbVal);

	/**
	 * @brief �����W�|�C���g�Q�`��擾
	 * @return �����W�|�C���g�Q�`��
	*/
	IGeometryPtr createDistancePointGeom();

	/**
	 * @brief ���H����
	 * @param ipGeom [in] �����`��
	 * @param dbBuf [in] �o�b�t�@�l
	 * @param roadCode [in] �H���R�[�h ����_�A���_���ΏۘH���R�[�h�������H�ɏ���Ă���Ƃ��Ɍ���
	 * @return ���H�Q
	*/
	CFeatureClass::Container searchRoads(const IGeometryPtr& ipGeom, double dbBuff, const long& roadCode = -1);

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
	* @brief �����W�|�C���g�t�B�[�`���N���X�擾
	* @return �����W�|�C���g�t�B�[�`���N���X
	*/
	IFeatureClassPtr getDistancePost() { return m_cDistancePostClass; }

	/**
	* @brief ���H�m�[�h�t�B�[�`���N���X�擾
	* @return ���H�m�[�h�t�B�[�`���N���X
	*/
	IFeatureClassPtr getRoadNode() { return m_cRoadNodeClass; }

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
	 * @brief �����W�l�Ԃ̋����Ƌ����W�|�C���g�Ԃ̋������`�F�b�N [bug 12155]
	 * @param pointLen [in] �����W�|�C���g�Ԃ̓��H����
	 * @param distanceDiff [in] �����W�l�̍�
	 * @param pitch [in] �s�b�`
	 * @param range [in] ���e�l
	 * @return �G���[�����邩�ǂ���
	*/
	bool checkRoadLen(const double& pointLen, const double& distanceDiff, const double& pitch, const double& range);

	/**
	 * @brief COM�I�u�W�F�N�g�̃R�s�[���쐬����
	 * @param ipUnk [in] �N���[����
	 * @return �N���[��
	*/
	static IClonePtr Clone( const IUnknownPtr& ipUnk );

	/**
	 * @brief �؂�l��
	 * @param dbVal [in] �l
	 * @param nLen [in] ��
	 * @return �؂�l�߂��l
	*/
	CString convertStr(double dbVal, long nLen=2);

	/**
	 * @brief �����_�܂ł̋����擾 [bug12155]
	 * @param ipPolyline [in] ���C���`��
	 * @param ipPoint [in] �����|�C���g
	 * @return �����_�܂ł̋��� [m]
	*/
	double split_point_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint);

	/**
	 * @brief ���H�����iOID�w��j[bug12155]
	 * @param listRouteIDs [in] OID�Q
	 * @return �������H�Q
	*/
	CFeatureClass::Container searchRoadsByOID(const std::list<long>& listRouteIDs);

	/**
	* @brief �ΏۘH����̓��H�ł��邩�`�F�b�N [bug12155]
	* @param roadContainer [in] ���H�Q
	* @param roadCode      [in] �H���R�[�h
	* @param roadCodeIdx   [in] �H���R�[�h�̃t�B�[���h�C���f�b�N�X
	* @retval true  �ΏۘH����̓��H�ł���
	* @retval false �ΏۘH����̓��H�łȂ�
	*/
	bool checkOnHWY(const CFeatureClass::Container& roadContainer, const long& roadCode, long roadCodeIdx);

	/**
	* @brief �S�̂̌o�H�`����쐬���� [bug12155]
	*        routePolyline�͌`��������Ă���O��œ��H�����N�Q��routePolyline�Ɍp�������Ă����B
	* @param routePolyline [in, out] �o�H�`��
	* @param roadContainer [in] ���H�����N�Q
	* @param routeIDs      [in] ���H�����N��OID�Q
	* @param ipPrjCoordinateSystem [in] ���W�n�ϊ��p
	*/
	void getRoutePolyline( IPolylinePtr& routePolyline, const CFeatureClass::Container& roadContainer, const list<long>& routeIDs, const IProjectedCoordinateSystemPtr& ipPrjCoordinateSystem );

	/**
	* @brief �|�����C���ɏ���Ă���|�C���g�Ԃ̋������擾���� [bug12155]
	*        (point2 - point1)
	* @param polyline [in] �|�����C���`��
	* @param point1   [in] �|�C���g�`��1
	* @param point2   [in] �|�C���g�`��2
	* @return �|�C���g�ԋ��� [m]
	*/
	double getPointLen( const IPolylinePtr& polyline, const IPointPtr& point1, const IPointPtr& point2 );

private:
	CWorkspace m_cWorkspace;                    ///< ���[�N�X�y�[�X
	CFeatureClass m_cDistancePostClass;         ///< �����W�|�C���g�N���X
	CFeatureClass m_cRoadLinkClass;             ///< ���H�����N�N���X
	CFeatureClass m_cRoadNodeClass;             ///< ���H�m�[�h�N���X
	CFeatureClass m_cBaseMeshClass;             ///< �Q�����b�V���N���X

	std::map<long, boost::tuple<long, long, CString>> m_mapRoadList; ///< �H�����X�g�Q
};

}
