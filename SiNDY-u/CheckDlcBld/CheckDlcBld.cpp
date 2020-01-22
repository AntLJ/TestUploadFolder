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

#include "stdafx.h"
#include "CheckDlcBld.h"

bool CCheckDlcBld::init(const Arguments& args)
{
	IWorkspacePtr ipWorkspace;
	if(!GetWorkspace(args.strSpatialDB.c_str(), ipWorkspace))
		return false;
	IFeatureClassPtr ipBuildingClass;
	if(!GetFeatureClass(sindy::schema::building::kTableName, ipWorkspace, ipBuildingClass))
		return false;

	CString strSpFC = sindy::schema::building::kTableName;
	if(!args.strSpatialDBSchema.empty())
		strSpFC = CString(args.strSpatialDBSchema.c_str()) + _T(".") + strSpFC;

	if(!GetSpatialReference(strSpFC, ipBuildingClass, m_ipSpRef))
		return false;

	if(!getMeshlist(args.strMeshlist.c_str(), m_setMesh))
		return false;

	m_strShapeDir = args.strShapeDir.c_str();

	return true;
}

bool CCheckDlcBld::getMeshlist(const CString& strMeshlist, std::set<CString>& setMesh)
{
	CString strMsg;
	std::ifstream ifs(strMeshlist);
	if(ifs.fail()){
		strMsg.Format(_T("���b�V�����X�g�I�[�v�����s"));
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg, strMeshlist);
		return false;
	}

	bool bMesh = true;
	char szLine[128];
	while(ifs.getline(szLine,128)){
		CString strMeshCode(szLine);
		if(strMeshCode.IsEmpty() || strMeshCode.Left(1) == _T("#"))
			continue;

		strMeshCode.Trim(_T(" "));
		strMeshCode.Trim(_T("�@"));

		// ���b�V�����X�g�̃��b�V���R�[�h��8���i�s�s�n�}���b�V���j�łȂ���NG
		if(8 != strMeshCode.GetLength()){
			strMsg.Format(_T("�s�s�n�}���b�V���R�[�h�łȂ�\t%s"), strMeshCode);
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
			bMesh = false;
			continue;
		}
		// �����݂̂ō\������Ă��邱��
		if(0 != strMeshCode.Compare(strMeshCode.SpanIncluding(_T("0123456789")))){
			strMsg.Format(_T("���b�V���R�[�h�ɐ����ȊO���g�p����Ă���\t%s"), strMeshCode);
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
			bMesh = false;
			continue;
		}
		setMesh.insert(strMeshCode);
	}
	return bMesh;
}

bool CCheckDlcBld::run(bool& bErr)
{
	bErr = false;

	long lCounter = 0, lMeshSize = m_setMesh.size();
	for(const auto mesh : m_setMesh){
		lCounter++;
		CString strPrint;
		strPrint.Format(_T("#%s   (%ld / %ld ��)"), mesh, lCounter, lMeshSize);
		CLog::GetInstance().PrintLog(true, false, true, false, strPrint);

		// �s�s�n�}���b�V����shapefile�p�X���擾
		CString strWorkspacePath = getShapeWorkspacePath(mesh);

		// ���[�N�X�y�[�X�擾
		IWorkspacePtr ipWork;
		if(!GetWorkspace(strWorkspacePath, ipWork)){
			bErr = true;
			continue;
		}

		// �������C���A���������C���擾
		CString strBldName, strDlcName;
		strBldName.Format(_T("%s_%s"), mesh, shp05::schema::citymap::building::kSuffix);
		strDlcName.Format(_T("%s_%s"), mesh, shp05::schema::citymap::bld_line::kSuffix);
		IFeatureClassPtr ipBldClass, ipDlcClass;
		if(!GetFeatureClass(strBldName, ipWork, ipBldClass) ||
			!GetFeatureClass(strDlcName, ipWork, ipDlcClass)){
			bErr = true;
			continue;
		}

		// �������C����OBJECTID�t�B�[���h�C���f�b�N�X�擾
		long lBldObjectidIndex = -1;
		IFieldsPtr ipBldFields;
		if(!GetFields(strDlcName, ipBldClass, ipBldFields)){
			bErr = true;
			continue;
		}
		if(!GetFieldIndex(strDlcName, shp05::schema::kObjectId, ipBldFields, lBldObjectidIndex) || 0 > lBldObjectidIndex){
			bErr = true;
			continue;
		}

		// ���������C���̌���ID�t�B�[���h�C���f�b�N�X�擾
		long lDlcBldIdIndex = -1;
		IFieldsPtr ipDlcFields;
		if(!GetFields(strDlcName, ipDlcClass, ipDlcFields)){
			bErr = true;
			continue;
		}
		if(!GetFieldIndex(strDlcName, shp05::schema::citymap::bld_line::kBldId, ipDlcFields, lDlcBldIdIndex) || 0 > lDlcBldIdIndex){
			bErr = true;
			continue;
		}

		// �����f�[�^��\�߃}�b�s���O���Ă���
		IFeatureCursorPtr ipBldCursor;
		if(!GetFeatureCursor(strBldName, ipBldClass, NULL, ipBldCursor)){
			bErr = true;
			continue;
		}
		std::map<long,CAdapt<IFeaturePtr>> mapBldFeature;
		IFeaturePtr ipBld;
		while(S_OK == ipBldCursor->NextFeature(&ipBld) && ipBld){
			// �����t�B�[�`����OBJECTID�擾
			CComVariant vaBldObjectId;
			if(!GetValue(strBldName, shp05::schema::kObjectId, ipBld, lBldObjectidIndex, vaBldObjectId)){
				bErr = true;
				continue;
			}
			mapBldFeature.insert(std::make_pair(vaBldObjectId.lVal, ipBld));
		}

		// ���������C����Ńt�B�[�`���擾���[�v
		IFeatureCursorPtr ipDlcCursor;
		if(!GetFeatureCursor(strDlcName, ipDlcClass, NULL, ipDlcCursor)){
			bErr = true;
			continue;
		}
		IFeaturePtr ipDlc;
		while(S_OK == ipDlcCursor->NextFeature(&ipDlc) && ipDlc){
			// �������t�B�[�`����FID�擾
			long lDlcFID = 0;
			if(!GetOID(strDlcName, ipDlc, lDlcFID)){
				bErr = true;
				continue;
			}

			// �������t�B�[�`���̌���ID�擾
			CComVariant vaBldId;
			if(!GetValue(strDlcName, shp05::schema::citymap::bld_line::kBldId, ipDlc, lDlcBldIdIndex, vaBldId)){
				bErr = true;
				continue;
			}
			// �������̌`��擾
			IGeometryPtr ipDlcGeom;
			if(!GetGeometry(strDlcName, ipDlc, ipDlcGeom)){
				bErr = true;
				continue;
			}
			// �������̋�ԎQ�ƕt�^
			if(!SetSpatialReference(strDlcName, m_ipSpRef, ipDlcGeom)){
				bErr = true;
				continue;
			}
			std::vector<IGeometryPtr> vecDlcSegGeom;
			if(!GetSegmentCenterPoints(strDlcName, lDlcFID, ipDlcGeom, vecDlcSegGeom)){
				bErr = true;
				continue;
			}

			// ����ID�̌����t�B�[�`���擾
			const auto itBldFeature = mapBldFeature.find(vaBldId.lVal);
			if(mapBldFeature.cend() == itBldFeature){
				// �����ɗ��鎞�_�ŃG���[�����A�G���[���e������ɍו�������
				// �擾�ł��Ȃ��ꍇ�͋ߗׂ̌����t�B�[�`�����������Ă݂�
				if(!ArouondBld(strBldName, vaBldId.lVal, ipBldClass, lBldObjectidIndex, strDlcName, lDlcFID, ipDlcGeom, vecDlcSegGeom, false, false)){
					// �ߗׂ̌����t�B�[�`���ɂ������Ȃ��ꍇ
					CLog::GetInstance().PrintLog(false, true, false, true, err_type::error, err_code::NgExistBld2, strDlcName, lDlcFID, strBldName, std::to_string(vaBldId.lVal).c_str());
				} else {
					// �ߗׂ̌����t�B�[�`���ɓ���ꂽ�ꍇ
					CLog::GetInstance().PrintLog(false, true, false, true, err_type::error, err_code::NgExistBld3, strDlcName, lDlcFID, strBldName, std::to_string(vaBldId.lVal).c_str());
				}
				bErr = true;
				continue;
			}

			// �����̌`��擾
			IGeometryPtr ipBldGeom;
			if(!GetGeometry(strBldName, itBldFeature->second.m_T, ipBldGeom)){
				bErr = true;
				continue;
			}
			// �����̋�ԎQ�ƕt�^
			if(!SetSpatialReference(strBldName, m_ipSpRef, ipBldGeom)){
				bErr = true;
				continue;
			}

			// �`��}�b�`���Ă��邩�`�F�b�N�i�ȈՔŁj
			bool bMatch = false;
			if(!MatchingBldDlc(strBldName, vaBldId.lVal, ipBldGeom, strDlcName, lDlcFID, ipDlcGeom, bMatch)){
				bErr = true;
				continue;
			}
			// �}�b�`���Ă���Ύ��̑�������
			if(bMatch)
				continue;

			// �`��}�b�`���Ă��邩�`�F�b�N�i�ڍהŁj
			std::vector<IGeometryPtr> vecDlcNotMatchSegGeom;
			if(!MatchingBldDlcSegment(strBldName, vaBldId.lVal, ipBldGeom, strDlcName, lDlcFID, vecDlcSegGeom, vecDlcNotMatchSegGeom)){
				bErr = true;
				continue;
			}
			// �}�b�`���Ă���Ύ��̑�������
			if(vecDlcNotMatchSegGeom.empty())
				continue;

			// �ꕔ������v���Ă��邩�ǂ���
			bool bSomeMatch = false;
			if(vecDlcSegGeom.size() != vecDlcNotMatchSegGeom.size()){
				bSomeMatch = true;
			}

			// �ߗׂ̌����|���S���ɕR�t���Ă��邩�`�F�b�N
			if(!ArouondBld(strBldName, vaBldId.lVal, ipBldClass, lBldObjectidIndex, strDlcName, lDlcFID, ipDlcGeom, vecDlcNotMatchSegGeom, bSomeMatch)){
				bErr = true;
				continue;
			}
		}
	}
	return true;
}

CString CCheckDlcBld::getShapeWorkspacePath(const CString& strMesh)
{
	CString strPath;
	strPath.Format(_T("%s\\%s\\%s\\%s"), m_strShapeDir, strMesh.Left(4), strMesh.Left(6), strMesh);
	return strPath;
}

bool CCheckDlcBld::MatchingBldDlc(const CString& strBldName, long lBldOid, IGeometryPtr ipBldGeom, const CString& strDlcName, long lDlcFid, IGeometryPtr ipDlcGeom, bool& bMatch)
{
	// �}�b�`���O�t���O��������
	bMatch = false;

	// �������x�[�X�ɑ�������Contains���Ă��邩
	IRelationalOperatorPtr ipBldRelOp(ipBldGeom);
	if(!ipBldRelOp){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("IRelationalOperator�֕ϊ��̎��s"));
		return false;
	}
	VARIANT_BOOL vaIsContains = VARIANT_FALSE;
	if(FAILED(ipBldRelOp->Contains(ipDlcGeom, &vaIsContains))){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcContains, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
		return false;
	}
	// Contains���Ă���ΏI��
	if(vaIsContains){
		bMatch = true;
		return true;
	}

	// �������x�[�X�ɑ�������Disjoint���Ă��邩
	VARIANT_BOOL vaIsDisjoint = VARIANT_FALSE;
	if(FAILED(ipBldRelOp->Disjoint(ipDlcGeom, &vaIsDisjoint))){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcDisjoint, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
		return false;
	}
	// Disjoint���Ă���ΏI���i�ߗ׌���������������j
	if(vaIsDisjoint)
		return true;

	// Disjoint���Ă��Ȃ��ꍇ�͉�������ڂ��Ă���͂��Ȃ̂ōׂ����m�F����

	// �������x�[�X�ɑ�������Crosses���Ă��邩
	VARIANT_BOOL vaIsCrosses = VARIANT_FALSE;
	if(FAILED(ipBldRelOp->Crosses(ipDlcGeom, &vaIsCrosses))){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcCross, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
		return false;
	}
	// Cross���Ă���Εs���`��ŏI��
	if(vaIsCrosses){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcCross, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
		return false;
	}

	return true;
}

bool CCheckDlcBld::MatchingBldDlcSegment(const CString& strBldName, long lBldOid, IGeometryPtr ipBldGeom, const CString& strDlcName, long lDlcFid, const std::vector<IGeometryPtr>& vecDlcSegGeom, std::vector<IGeometryPtr>& vecNotMatchDlcSegGeom)
{
	// �������x�[�X�ɑ�������Contains���Ă��邩
	IRelationalOperatorPtr ipBldRelOp(ipBldGeom);
	if(!ipBldRelOp){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("IRelationalOperator�֕ϊ��̎��s"));
		return false;
	}

	for(long l = 0; l < vecDlcSegGeom.size(); ++l){
		IGeometryPtr ipDlcSegCenterGeom = vecDlcSegGeom.at(l);
		// �������x�[�X�ɂ��̒��_��Contains���Ă��邩
		VARIANT_BOOL vaIsCenterContains = VARIANT_FALSE;
		if(FAILED(ipBldRelOp->Contains(ipDlcSegCenterGeom, &vaIsCenterContains))){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcContains, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
			return false;
		}
		// Contains���Ă���Ύ��̃Z�O�����g��
		if(vaIsCenterContains)
			continue;

		// �������x�[�X�ɒ��_��Touches���Ă��邩
		VARIANT_BOOL vaIsCenterTouches = VARIANT_FALSE;
		if(FAILED(ipBldRelOp->Touches(ipDlcSegCenterGeom, &vaIsCenterTouches))){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcTouches, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
			return false;
		}
		// Touches���Ă��Ȃ��ꍇ�͖��}�b�`���O�R���e�i�֊i�[
		if(!vaIsCenterTouches)
			vecNotMatchDlcSegGeom.push_back(ipDlcSegCenterGeom);
	}

	return true;
}

bool CCheckDlcBld::GetSegmentCenterPoints(const CString& strTableName, long lOID, IGeometryPtr ipGeom, std::vector<IGeometryPtr>& vecSegCenterPointGeom)
{
	ISegmentCollectionPtr ipSegCol(ipGeom);
	if(!ipSegCol){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("ISegmentCollection�֕ϊ��̎��s"));
		return false;
	}
	long lSegCount = 0;
	if(S_OK != ipSegCol->get_SegmentCount(&lSegCount)){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("�Z�O�����g���擾���s"));
		return false;
	}
	for(long l = 0; l < lSegCount; ++l){
		// �Z�O�����g���擾
		_ISegmentPtr ipSeg;
		if(S_OK != ipSegCol->get_Segment(l, &ipSeg)){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("�Z�O�����g�擾���s"));
			return false;
		}
		// �Z�O�����g�̒��_���擾
		IPointPtr ipCenterPoint(CLSID_Point);
		ipSeg->QueryPoint(esriNoExtension, 0.5, VARIANT_TRUE, ipCenterPoint);

		IGeometryPtr ipCenterGeom(ipCenterPoint);
		if(!ipCenterGeom){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("IGeometry�֕ϊ����s"));
			return false;
		}

		// ��ԎQ�Ƃ�t�^
		if(!SetSpatialReference(strTableName, m_ipSpRef, ipCenterGeom))
			return false;

		vecSegCenterPointGeom.push_back(ipCenterGeom);
	}
	return true;
}

bool CCheckDlcBld::ArouondBld(const CString& strBldName, long lBldOid, IFeatureClassPtr ipBldClass, long lBldObjectIdIndex, const CString& strDlcName, long lDlcFid, IGeometryPtr ipDlcGeom, const std::vector<IGeometryPtr>& vecDlcSegGeom, bool bSomeMatch, bool bMsg)
{
	// �������`��Ō�������Ԍ�������
	ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
	if(S_OK != ipSpFilter->putref_Geometry(ipDlcGeom) ||
		S_OK != ipSpFilter->put_SpatialRel(esriSpatialRelIntersects)){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("��ԃt�B���^�쐬���s"));
			return false;
	}
	IFeatureCursorPtr ipBldCursor;
	if(!GetFeatureCursor(strBldName, ipBldClass, ipSpFilter, ipBldCursor))
		return false;

	std::vector<IGeometryPtr> vecTmp = vecDlcSegGeom;
	IFeaturePtr ipBld;
	while(S_OK == ipBldCursor->NextFeature(&ipBld) && ipBld){
		// ������OBJECTID���擾
		CComVariant vaBldObjectId;
		if(!GetValue(strBldName, shp05::schema::kObjectId, ipBld, lBldObjectIdIndex, vaBldObjectId))
			return false;

		// ���Ɍ����ς݂̌����̓X���[
		if(vaBldObjectId.lVal == lBldOid)
			continue;

		// �����̌`��擾
		IGeometryPtr ipBldGeom;
		if(!GetGeometry(strBldName, ipBld, ipBldGeom))
			return false;

		// �����ɋ�ԎQ�ƕt�^
		if(!SetSpatialReference(strBldName, m_ipSpRef, ipBldGeom))
			return false;

		// �`��}�b�`���Ă��邩�`�F�b�N�i�ȈՔŁj
		bool bMatch = false;
		if(!MatchingBldDlc(strBldName, vaBldObjectId.lVal, ipBldGeom, strDlcName, lDlcFid, ipDlcGeom, bMatch)){
			return false;
		}
		// �}�b�`���Ă���Ύ��̑�������
		if(bMatch){
			if(bMsg)
				CLog::GetInstance().PrintLog(false, true, false, true, err_type::warning, err_code::NgExistBld4, strDlcName, lDlcFid, strBldName, std::to_string(vaBldObjectId.lVal).c_str());
			return true;
		}

		// �`��}�b�`���Ă��邩�`�F�b�N�i�ڍה�
		std::vector<IGeometryPtr> vecNotMatchTmp;
		if(MatchingBldDlcSegment(strBldName, vaBldObjectId.lVal, ipBldGeom, strDlcName, lDlcFid, vecTmp, vecNotMatchTmp)){
			if(vecNotMatchTmp.empty()){
				if(bMsg){
					if(bSomeMatch)
						CLog::GetInstance().PrintLog(false, true, false, true, err_type::warning, err_code::NgExistBld5, strDlcName, lDlcFid, strBldName, std::to_string(vaBldObjectId.lVal).c_str());
					else
						CLog::GetInstance().PrintLog(false, true, false, true, err_type::warning, err_code::NgExistBld4, strDlcName, lDlcFid, strBldName, std::to_string(vaBldObjectId.lVal).c_str());
				}
				return true;
			}
		}
		vecTmp = vecNotMatchTmp;
	}
	if(bMsg){
		// �}�b�`���O������̂�������΃G���[
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::error, err_code::NgBldDlcContTouchNear, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
	}

	return false;
}
