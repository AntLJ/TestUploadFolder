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
#include <sindy/libschema.h>
#include <sindy/workspace.h>
#include "RelPoiTbl.h"

CRelPoiTbl::CRelPoiTbl(void) {
}

CRelPoiTbl::~CRelPoiTbl(void) {
}

bool CRelPoiTbl::readRelTbl(std::string sTblPath, MAP_REL_TBL &relTbl) {
	std::string sLayer, sNid, sPid, sMesh;			// �G���[���O�\���p
	sLayer = nsCommon::getLayerName(sTblPath);
	CString csDirPath((nsCommon::getPath(sTblPath)).c_str());

	IWorkspacePtr ipWorkspace = sindy::create_workspace(csDirPath);
	if (!ipWorkspace) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, (boost::format("���[�N�X�y�[�X�ڑ����s, dir=%s") % CT2CA(csDirPath)).str());
		return false;
	}

	ITablePtr ipTable;
	if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenTable(CComBSTR(sLayer.c_str()), &ipTable))) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, "�I�[�v�����s");
		return false;
	}
	
	_ICursorPtr ipCursor;
	if (FAILED(ipTable->Search(NULL,VARIANT_FALSE, &ipCursor))) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, "�J�[�\���擾���s");
		return false;
	}

	long nidFieldIdx = -1;
	long pidFieldIdx = -1;
	std::string sErrMsg;
	if (!nsCommon::getFieldIndex(ipTable, shp05::schema::road::rel_walk_node_general_poi::kNodeId, nidFieldIdx, sErrMsg) || 
		!nsCommon::getFieldIndex(ipTable, shp05::schema::road::rel_walk_node_general_poi::kPoiId, pidFieldIdx, sErrMsg)) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, sErrMsg);
		return false;
	}

	bool bOverlap = false;
	_IRowPtr ipRow;
	while (ipCursor->NextRow(&ipRow) == S_OK) {
		CComVariant nodeId, poiId;
		if (FAILED(ipRow->get_Value(nidFieldIdx, &nodeId)) ||
			FAILED(ipRow->get_Value(pidFieldIdx, &poiId))) {
			ERR_LOG(sLayer, sNid, sPid, sMesh, (boost::format("�l�擾���s, field=%s & %s")
					% shp05::schema::road::rel_walk_node_general_poi::kNodeId
					% shp05::schema::road::rel_walk_node_general_poi::kPoiId).str());
			return false;
		}
		nodeId.ChangeType(VT_I4);
		poiId.ChangeType(VT_I4);

		// �R���e�i�ɋl����
		MAP_REL_TBL::_Pairib pib = relTbl.insert(MAP_REL_TBL::value_type(nodeId.lVal, poiId.lVal));
		if (!pib.second) {
			sNid = std::to_string(nodeId.lVal);
			sPid = std::to_string(poiId.lVal);
			// �y�G���[�@�z�֘A�e�[�u����NODE_ID�d�������݂���B
			ERR_LOG(sLayer, sNid, sPid, sMesh, "NODE_ID�d��");
			bOverlap = true;
		}
	}

	if (bOverlap) {
		return false;
	}

	return true;
}
