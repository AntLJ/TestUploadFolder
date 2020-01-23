#include "stdafx.h"
#include "LogSys.h"
#include "TransParam.h"
#include "CheckWarningSign.h"

using namespace sindy::schema;

const double LATLNG_DIGIT_LEVEL = 0.0000125;	// 1���[�g��������̈ܓx�o�x�l	(�����傫�߂Ƀo�b�t�@���쐬���邽�ߖk�C���̈ܓx����ɐݒ�)

// �x���W���|�C���g�̓��H�����N�̒��������̎擾����
bool directDistance(const IPointPtr& warningSignPoint, const IPointPtr& nearestPoint, double& pointToPointDistance)
{
	// �x���W���|�C���g�Ɠ��H�����N�ŋߖT�|�C���g��XY���W���擾
	double ws_x = 0.0;
	double ws_y = 0.0;
	double near_x = 0.0;
	double near_y =0.0;
	if(FAILED(warningSignPoint->QueryCoords(&ws_x,&ws_y)) ||
		FAILED(nearestPoint->QueryCoords(&near_x,&near_y))) {
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�|�C���g��XY���W�擾�Ɏ��s���܂���"));
		return false;
	}

	// �����������Z�o
	crd_cnv crd;
	pointToPointDistance = crd.GetDist(ws_x, ws_y, near_x, near_y);
	return true;
}

// ���H�\����ʂ̌�������
bool getDomainName(const std::map<long, CString>& mapDomain, const long& codedVal, CString& domainName)
{
	auto it = mapDomain.find(codedVal);
	if(it != mapDomain.end()) {
		domainName = it->second;
	} else {
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("���H�\����ʃR�[�h�l�����݂��܂���"));
		return false;
	}
	return true;
}

// �ŋߖT���H�����N���̏o�͏���
bool checkWarningSign(const TransParam& transParam, const IFeatureClassPtr& ipRoadFC, const ISpatialReferencePtr& ipRoadSPRef, const long& roadClassIndex, const std::map<long, CAdapt<IPointPtr>>& mapGeo, const std::map<long, CString>& mapDomain, std::ofstream &ofsOutputLog)
{
	// �o�b�t�@�l(���[�g��)���ܓx�o�x�P�ʂɕϊ�
	double latLngBuffer = transParam.getBuffer() * LATLNG_DIGIT_LEVEL;

	for(std::map<long, CAdapt<IPointPtr>>::const_iterator it = mapGeo.begin(); it != mapGeo.end(); ++it)
	{
		// Buffer�쐬
		ITopologicalOperatorPtr ipTopo(it->second.m_T);
		IGeometryPtr ipBuffer;
		double bufferValue = latLngBuffer;
		if(FAILED(ipTopo->Buffer(bufferValue, &ipBuffer))) {
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�x���W���̃o�b�t�@�擾�Ɏ��s���܂���"));
			return false;
		}

		//��ԎQ�Ƃ̕t�^
		if(FAILED(ipBuffer->putref_SpatialReference( ipRoadSPRef))) {
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("��ԎQ�Ƃ̕t�^�Ɏ��s���܂���"));
			return false;
		}

		// Road_Link�̌���
		CString roadWhereCluse;
		roadWhereCluse = transParam.getRoadWhere();

		IFeatureCursorPtr ipRoadFeatureCursor;
		if (! common_proc::getFeatureCursor(ipRoadFC,road_link::kTableName, ipBuffer, esriSpatialRelIntersects, roadWhereCluse, ipRoadFeatureCursor))
			return false;

		long shOID = 0;			// �ŋߖT���H�����N��OID
		long shRoadClass = 0;	// �ŋߖT���H�����N�̓��H�W����ʃR�[�h
		double shDistance = DBL_MAX;	// �x���W���̍ŋߖT���H�����N�Ԃ̋���

		// �x���W���ƍŋߖT�̓��H�����N�擾����
		IFeaturePtr ipRoadFeature;
		while (ipRoadFeatureCursor->NextFeature( &ipRoadFeature) == S_OK) {
			long lOID = 0;
			CComVariant vaRoadClass;

			// ���HOID�̎擾
			if(FAILED(ipRoadFeature->get_OID(&lOID))){
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("���H�����N�̃I�u�W�F�N�gID�̎擾�Ɏ��s���܂���"));
				return false;
			}

			// ���H�\����ʂ̎擾
			if(FAILED(ipRoadFeature->get_Value(roadClassIndex, &vaRoadClass))) {
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("���H�\����ʂ̎擾�Ɏ��s���܂���"));
				return false;
			}

			// �x���W���|�C���g �� ���H�����N�Ԃ̍ŒZ�����̎Z�o
			double pointToPointDistance = -1.0;
			IGeometryPtr ipGeom;
			IPointPtr nearestPoint;
			if(FAILED(ipRoadFeature->get_ShapeCopy(&ipGeom))){
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�W�I���g���̃R�s�[�̎擾�Ɏ��s���܂���"));
				return false;
			}

			IProximityOperatorPtr ipProx(ipGeom);
			if(FAILED(ipProx->ReturnNearestPoint(it->second.m_T, esriNoExtension, &nearestPoint))){
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("���H�����N�ߐڃ|�C���g�̎擾�Ɏ��s���܂���"));
				return false;
			}

			// �x���W���Ɠ��H�����N�̒����������擾
			if(! directDistance(it->second.m_T, nearestPoint, pointToPointDistance)){
				return false;
			}

			//�ŋߖT���H�����N�̌����i�[
			/*
			 * ����������̏ꍇ�͓��H�\����ʃR�[�h�l�̏��������H�����N�����Ƃ���B
			 * �����A�R�[�h�l������̏ꍇ��OID�����������H�����N�����Ƃ���B
			 */
			if(pointToPointDistance >= 0.0) {
				if((shDistance > pointToPointDistance) ||
					(shDistance == pointToPointDistance && shRoadClass > vaRoadClass.lVal) ||
					(shDistance == pointToPointDistance && shRoadClass == vaRoadClass.lVal && shOID > lOID)) {
					shOID = lOID;
					shRoadClass = vaRoadClass.lVal;
					shDistance = pointToPointDistance;
				}
			}
		}

		// ���O�ɏo��
		CString strOut;

		if(shOID) {
			CString roadClassName;
			// �R�[�h�l�h���C�����瓹�H�\�����̂��擾
			if(! getDomainName(mapDomain, shRoadClass, roadClassName))
				return false;
			// �ŋߖT���H�����N�����o��
			strOut.Format( _T("%ld\t%ld\t%s\t%0.10f"), it->first, shOID, roadClassName, shDistance);
		} else {
			// �x���W����OID�̂ݏo��
			strOut.Format( _T("%ld\t\t\t"), it->first);
		}
		ofsOutputLog << CT2A( strOut ) << std::endl;
	}
	return true;
}