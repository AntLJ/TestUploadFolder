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
#include "DiffAddSDE.h"
//#include "CreateFeature.h"
#include <sindy/workspace.h>
#include <sindy/schema.h>

#define	RECONCILE_COUNT	20	// ���R���T�C����

#define BASEDIST	2.0

bool CDiffAddSDE::run()
{
	crd_cnv cnv;
	m_cnv = cnv;

	// SpatialReference �擾
	long lLinkShapeIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(_T("SHAPE")));
	long lNodeShapeIndex = m_cInputData.ipNodeClass->GetFields()->_FindField(_bstr_t(_T("SHAPE")));
	IFieldPtr ipLinkShapeField = m_cInputData.ipLinkClass->GetFields()->GetField(lLinkShapeIndex);
	IFieldPtr ipNodeShapeField = m_cInputData.ipNodeClass->GetFields()->GetField(lNodeShapeIndex);
	IGeometryDefPtr ipLinkGeomDef, ipNodeGeomDef;
	ipLinkShapeField->get_GeometryDef(&ipLinkGeomDef);
	ipNodeShapeField->get_GeometryDef(&ipNodeGeomDef);
	ipLinkGeomDef->get_SpatialReference(&m_ipLinkSpRef);
	ipNodeGeomDef->get_SpatialReference(&m_ipNodeSpRef);
	if(!ipLinkGeomDef || !ipNodeGeomDef)
		return false;

	// �t�B�[���h�C���f�b�N�X�擾
	m_lNodeClassIndex = m_cInputData.ipNodeClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_node::kNodeClass));
	m_lRoadClassIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kRoadClass));
	m_lNaviClassIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kNaviClass));
	m_lMainLinkClassIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kMainLinkClass));
	m_lRoadWidthIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kRoadWidth));
	m_lUpDownClassIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kUpDownClass));
	m_lFromNodeIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kFromNodeID));
	m_lToNodeIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kToNodeID));
	m_lMIDIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(g_MID));
	m_lMDivision = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(g_MDivision));
	if(m_lNodeClassIndex < 0 || m_lRoadClassIndex < 0 || m_lNaviClassIndex < 0 || m_lMainLinkClassIndex < 0 || m_lRoadWidthIndex < 0 || m_lUpDownClassIndex < 0 || m_lFromNodeIndex < 0 || m_lToNodeIndex < 0)
		return false;

	// �����l��ݒ肷��
	m_mapNodeFieldValue[m_lNodeClassIndex] = 0;
	m_mapLinkFieldValue[m_lRoadClassIndex] = 6;
	m_mapLinkFieldValue[m_lNaviClassIndex] = 6;
	m_mapLinkFieldValue[m_lMainLinkClassIndex] = 1;
	m_mapLinkFieldValue[m_lRoadWidthIndex] = 2;
	m_mapLinkFieldValue[m_lUpDownClassIndex] = 0;

	// �e�X�g�p�̃T�[�o��MID��MDivision��������ΐݒ肵�Ȃ�
	if(m_lMIDIndex >= 0 && m_lMDivision >=0){
		m_mapLinkFieldValue[m_lMIDIndex] = 0;
		m_mapLinkFieldValue[m_lMDivision] = 0;
	}



	bool bReturn = false;

	// �C���|�[�g��̃��[�N�X�y�[�X�ҏW�J�n ======================================
	IWorkspaceEditPtr ipWorkspaceEdit;
	esriWorkspaceType esriType;
	m_cInputData.ipSDEWork->get_Type(&esriType);
	// SDE�ڑ��Ȃ�ҏW�J�n�I�����s��
	if(esriType == esriRemoteDatabaseWorkspace){
		ipWorkspaceEdit = m_cInputData.ipSDEWork;
		if(!ipWorkspaceEdit){
			std::cerr << "#ERROR\tIWorkspaceEdit��NULL\n";
			return false;
		}
		// ���܂��Ȃ��iSDE_LOGFILE�������I�ɍ쐬������j
		omajinai();

		// �ҏW�J�n
		std::cout << "�����ҏW�J�n����\n";
		if(S_OK != ipWorkspaceEdit->StartEditing(VARIANT_FALSE)){
			std::cerr << "#ERROR\tStartEditing()���s\n";
			return false;
		}
		if(S_OK != ipWorkspaceEdit->StartEditOperation()){
			std::cerr << "#ERROR\tStartEditOperation()���s\n";
			return false;
		}
	}
	// =================================================================================

	// �f�[�^�擾�{�C���|�[�g����
	bool bImport = false;
	m_cInputData.ipLinkClass->Insert(VARIANT_TRUE, &m_ipImportLinkCursor);
	m_cInputData.ipNodeClass->Insert(VARIANT_TRUE, &m_ipImportNodeCursor);
	if(!m_ipImportLinkCursor || !m_ipImportNodeCursor){
		std::cerr << "#ERROR\t�C���|�[�g�p�t�B�[�`���J�[�\���擾���s\n";
	} else {
		// �f�[�^�擾
		if(get_diffdata()){
			// �C���|�[�g����
			bImport = diff_import();
		}
	}


	// �C���|�[�g��̃��[�N�X�y�[�X�ҏW�I�� ======================================
	if(esriType == esriRemoteDatabaseWorkspace){
		if(bImport){
			HRESULT hr = ipWorkspaceEdit->StopEditOperation();
			if(S_OK == hr){
				bool bReconcile = true;
				int lCount = 0;
				// �ҏW�I������
				while(!DoReconcile(ipWorkspaceEdit)){
					lCount++;
					if(lCount > RECONCILE_COUNT){
						CString strReconcile;
						strReconcile.Format(_T("#ERROR\t���R���T�C����%d��s���܂������������܂���ł����̂ŏI�����܂���"), RECONCILE_COUNT);
						std::cerr << CT2A(strReconcile) << "\n";
						bReconcile = false;
						break;
					}
				}
				// �����܂�OK�Ȃ�ŏI�߂�l��true�ɂ���
				if(bReconcile)
					bReturn = true;
			} else {
				if(S_OK != ipWorkspaceEdit->AbortEditOperation())
					std::cerr << "#ERROR\tAbortOperation()���s\n";
				else
					std::cerr << "#NOTICE\tAbortOperation()���sOK\n";

				if(S_OK != ipWorkspaceEdit->StopEditing(VARIANT_FALSE))
					std::cerr << "#ERROR\tStopEditOperation()���s\n";
				else
					std::cerr << "#NOTICE\tStopEditOperation()���sO\n";
			}
		} else {
			if(S_OK != ipWorkspaceEdit->AbortEditOperation())
				std::cerr << "#ERROR\tAbortOperation()���s\n";
			else
				std::cerr << "#NOTICE\tAbortOperation()���sOK\n";

			if(S_OK != ipWorkspaceEdit->StopEditing(VARIANT_FALSE))
				std::cerr << "#ERROR\tStopEditOperation()���s\n";
			else
				std::cerr << "#NOTICE\tStopEditOperation()���sO\n";
		}
	}
	// =================================================================================

	return bReturn;
}

// �P�Ȃ�f�B���N�g����͏���
bool CDiffAddSDE::get_diffdata()
{
	// �����f�[�^�S�������b�V�����X�g�ŃC���|�[�g���邩
	if(m_cInputData.strMeshlist.IsEmpty()){
		// �����f�[�^�S��
		// �������f�[�^�̃f�B���N�g���\���ʂ�ɒH�邾���B�H�����Ƃ���Ƀf�[�^������Βǉ�����������
		CString strDiffShapeSearch = m_cInputData.strDiffShapeDir + _T("\\*");
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(strDiffShapeSearch, &fd);
		if(hFind != INVALID_HANDLE_VALUE){
			// ���[�g�p�X�ȉ��̃f�B���N�g���T��
			do {
				if(fd.cFileName[0] == '.')
					continue;
				CString strNextDir = m_cInputData.strDiffShapeDir + _T("\\") + fd.cFileName;
				DWORD dwFileAttribute = ::GetFileAttributes(strNextDir);
				if(dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY){
					CString strDiffShapeSearch2 = strNextDir + _T("\\*");
					WIN32_FIND_DATA fd2;
					HANDLE hFind2 = FindFirstFile(strDiffShapeSearch2, &fd2);
					if(hFind != INVALID_HANDLE_VALUE){
						do {
							if(fd2.cFileName[0] == '.')
								continue;
							CString strWorkspaceDir = strNextDir + _T("\\") + fd2.cFileName;
							DWORD dwFileAttribute = ::GetFileAttributes(strWorkspaceDir);
							if(dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY){
								// ���b�V�����X�g�Ƃ��Ēǉ�
								m_cInputData.setMeshlist;
								CString strShpName = strNextDir + _T("\\") + fd2.cFileName + _T("\\") + fd2.cFileName + _T("_diff.shp");
								if(PathFileExists(strNextDir))
									m_cInputData.setMeshlist.insert(fd2.cFileName);
							}
						} while( FindNextFile(hFind2, &fd2));
						FindClose(hFind2);
					}
				}
			} while( FindNextFile(hFind, &fd));
			FindClose(hFind);
		}
	}

	std::cout << "�������f�[�^�擾�����J�n��\n";
	long lCount = 0, lSize = m_cInputData.setMeshlist.size();
	CString strMesh;
	for(std::set<CString>::const_iterator it = m_cInputData.setMeshlist.begin(); it != m_cInputData.setMeshlist.end(); ++it){
		strMesh = *it;
		std::cout << ++lCount << " / " << lSize << " (" << CT2A(strMesh) << ")\r";
		// ���[�N�X�y�[�X�ƍ����f�[�^�N���X���擾����
		CString strDiffWork = m_cInputData.strDiffShapeDir + _T("\\") + strMesh.Left(4) + _T("\\") + strMesh;
		if(!get_diffdata(strDiffWork, strMesh)){
			return false;
		}
	}
	std::cout << lCount << " / " << lSize << " (" << CT2A(strMesh) << ")\n";
	std::cout << "�������f�[�^�擾�����I����\n";

	return true;
}

bool CDiffAddSDE::get_diffdata(const CString& strDiffWork, const CString& strMesh)
{
	// ���[�N�X�y�[�X�ƍ����f�[�^�N���X���擾����
	IWorkspacePtr ipDiffWork = sindy::create_workspace(strDiffWork);
	if(ipDiffWork){
		IFeatureClassPtr ipDiffLinkClass;
		IFeatureWorkspacePtr(ipDiffWork)->OpenFeatureClass(CComBSTR(strMesh+_T("_diff")), &ipDiffLinkClass);
		if(ipDiffLinkClass){
			// �����f�[�^�ǉ�����
			if(!get_diffdata_main(ipDiffWork, ipDiffLinkClass, strMesh))
				return false;
		} else {
			std::cerr << "#ERROR\t" << CT2A(strDiffWork) << "\t�����t�B�[�`���N���X�I�[�v�����s\n";
		}
	} else {
		std::cerr << "#ERROR\t" << CT2A(strDiffWork) << "\t�����f�[�^���[�N�X�y�[�X�I�[�v�����s\n";
	}
	// �t�B�[�`���N���X���I�[�v���ł��Ȃ����x�Ȃ�X���[��OK
	return true;
}

bool CDiffAddSDE::get_diffdata_main(IWorkspace* ipDiffWork, IFeatureClass* ipDiffLinkClass, const CString& strMesh)
{
	// �����f�[�^�̃t�B�[���h�C���f�b�N�X���擾���Ă���
	long lFIDIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_FID));
	long lMDivisionIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_MDivision));
	long lMIDIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_MID));
	if(lFIDIndex < 0 || lMDivisionIndex < 0 || lMIDIndex < 0){
		std::cerr << "#ERROR\t" << CT2A(strMesh) << "�t�B�[���h�C���f�b�N�X�擾���s\n";
		return false;
	}
	long lMesh = _ttol(strMesh);

	std::set<std::vector<XY>> setOldLinkGeom;	// ����`�󂪂���ꍇ�̓C���|�[�g�����Ȃ�

	// �܂��̓C���|�[�g���i�����f�[�^�j��FromTo�m�[�hID�̐ݒ�{�����NID�̎擾
	IFeatureCursorPtr ipDiffLinkCursor;
	ipDiffLinkClass->Search(NULL, VARIANT_FALSE, &ipDiffLinkCursor);
	if(!ipDiffLinkCursor){
		std::cerr << "#ERROR\t" << CT2A(strMesh) << "�J�[�\���擾���s\n";
		return false;
	}
	IFeaturePtr ipDiffLink;
	while(S_OK == ipDiffLinkCursor->NextFeature(&ipDiffLink)){
		IGeometryPtr ipGeom;
		CComVariant vaFID, vaMDivision, vaMID;
		ipDiffLink->get_Value(lFIDIndex, &vaFID);
		ipDiffLink->get_Value(lMDivisionIndex, &vaMDivision);
		ipDiffLink->get_Value(lMIDIndex, &vaMID);
		ipDiffLink->get_ShapeCopy(&ipGeom);
		if(!ipGeom){
			std::cerr << "#ERROR\t" << CT2A(strMesh) << "\tFID : " << vaFID.lVal << "\t�`��NULL\n";
			continue;
		}
		CLinkOID cLinkOID;
		cLinkOID.lMesh = lMesh;
		cLinkOID.lFID = vaFID.lVal;
		long lPointCount = 0;
		IPointCollectionPtr(ipGeom)->get_PointCount(&lPointCount);
		std::vector<XY> vecXY;
		for(long l=0;l<lPointCount;++l){
			XY cXY;
			IPointPtr ipPoint;
			IPointCollectionPtr(ipGeom)->get_Point(l,&ipPoint);
			ipPoint->QueryCoords(&cXY.X,&cXY.Y);
			vecXY.push_back(cXY);
		}
		if(setOldLinkGeom.find(vecXY) != setOldLinkGeom.end()){
			std::cerr << "#ERROR\t" << CT2A(strMesh) << "\tFID : " << vaFID.lVal << "\t����`�󂪂���̂ŃX���[\n";
			continue;
		}
		setOldLinkGeom.insert(vecXY);

		m_mapOldNewLink[vaFID.lVal] = 0;

		IPointPtr ipFPoint, ipTPoint;
		IPolylinePtr(ipGeom)->get_FromPoint(&ipFPoint);
		IPolylinePtr(ipGeom)->get_ToPoint(&ipTPoint);

		XY cFromXY, cToXY;
		ipFPoint->QueryCoords(&cFromXY.X,&cFromXY.Y);
		ipTPoint->QueryCoords(&cToXY.X,&cToXY.Y);

		long lFromID = 0, lToID = 0;
		if(m_mapOldNodeXY.insert(std::pair<XY,long>(cFromXY,m_lOldNodeID)).second){
			lFromID = m_lOldNodeID;
			m_mapOldNewNode[lFromID] = 0;
			m_mapOldNodeData[lFromID] = cFromXY;
			m_mapOldNodeXYs[lFromID] = cFromXY;
			m_lOldNodeID++;
		} else {
			lFromID = m_mapOldNodeXY[cFromXY];
		}
		if(m_mapOldNodeXY.insert(std::pair<XY,long>(cToXY,m_lOldNodeID)).second){
			lToID = m_lOldNodeID;
			m_mapOldNewNode[lToID] = 0;
			m_mapOldNodeData[lToID] = cToXY;
			m_mapOldNodeXYs[lToID] = cToXY;
			m_lOldNodeID++;
		} else {
			lToID = m_mapOldNodeXY[cToXY];
		}

		m_mapOldNodeLinks[lFromID].insert(cLinkOID);
		m_mapOldNodeLinks[lToID].insert(cLinkOID);
		m_mapOldLinkFTNode[cLinkOID] = std::pair<long,long>(lFromID,lToID);
		CDiffData cDiffData;
		cDiffData.cLinkOID = cLinkOID;
		cDiffData.lMDivision = vaMDivision.lVal;
		cDiffData.lMID = vaMID.lVal;
		cDiffData.ipGeom = ipGeom;
		m_mapOldLinkData[cLinkOID] = cDiffData;
	}

	return true;
}

bool CDiffAddSDE::diff_import()
{
	std::cout << "�������f�[�^�C���|�[�g�����J�n��\n";

	ISpatialFilterPtr ipSpFil(CLSID_SpatialFilter);

	// 2�����b�V���[�_�������c���Ă��܂��m�[�h���L�[�v���Ă���
	// ���Ƃł��̃m�[�h�ɕR�t�������N�̒[�_��Z�����邽��...
	std::set<long> setOldMeshEdgeNode;

	std::cout << "�m�[�h�C���|�[�g������...\n";
	long lNodeCount = 0, lNodeSize = m_mapOldNodeData.size();

	// �C���|�[�g�p�̃f�[�^���擾������C���|�[�g���J�n����
	// �܂��̓m�[�h����
	for(std::map<long,XY>::const_iterator itNode = m_mapOldNodeData.begin(); itNode != m_mapOldNodeData.end(); ++itNode){
		lNodeCount++;
		if(lNodeCount % 100 == 0)
			std::cout << lNodeCount << " / " << lNodeSize << "\r";

		int imesh,ix,iy;
		m_cnv.LLtoMesh(itNode->second.X,itNode->second.Y,2,&imesh,&ix,&iy);
		IPointPtr ipNodePoint(CLSID_Point);
		ipNodePoint->PutCoords(itNode->second.X,itNode->second.Y);
		ipNodePoint->putref_SpatialReference(m_ipNodeSpRef);
		bool bMeshEdgeNode = false;
		// ���b�V�����E�����`�F�b�N
		if(ix == 0 || ix == 1000000 || iy == 0 || iy == 1000000){
			m_mapNodeFieldValue[m_lNodeClassIndex] = 9;
			// ���b�V�����E�m�[�h��1�{���������N���ڑ����Ă�����̂͌�ŏ���������
			if(m_mapOldNodeLinks[itNode->first].size() == 1){
				bMeshEdgeNode = true;
				setOldMeshEdgeNode.insert(itNode->first);
			}
		} else {
			// �m�[�h�N���X�̐ݒ�i�[�_�A�����_�A�_�~�[���j
			if(m_mapOldNodeLinks[itNode->first].size() == 1)
				m_mapNodeFieldValue[m_lNodeClassIndex] = 2;	// �[�_
			else if(m_mapOldNodeLinks[itNode->first].size() == 2)
				m_mapNodeFieldValue[m_lNodeClassIndex] = 0;	// �_�~�[
			else if(m_mapOldNodeLinks[itNode->first].size() > 2)
				m_mapNodeFieldValue[m_lNodeClassIndex] = 1;	// �����_
			else
				std::cerr << "#ERROR\t" << itNode->first << "�́[�ǂ������Ă�\n";
		}
		// ���b�V�����E�m�[�h��1�{���������N���ڑ����Ă�����̂͌�ŏ���������
		if(!bMeshEdgeNode){
			// ��L�ȊO�̓m�[�h�쐬����
			IFeatureBufferPtr ipNodeBuffer = create_FeatureBuffer(m_cInputData.ipNodeClass, IGeometryPtr(ipNodePoint), m_mapNodeFieldValue);
			if(!ipNodeBuffer){
				std::cerr << "#ERROR\t" << itNode->first << "�́[�ǃo�b�t�@��NULL���Ă�\n";
				continue;
			}
			CComVariant vaImportNodeOID;
			if(S_OK != m_ipImportNodeCursor->InsertFeature(ipNodeBuffer, &vaImportNodeOID) || vaImportNodeOID.lVal < 0){
				std::cerr << "#ERROR\t" << itNode->first << "�́[�ǂ��������ς���\n";
				continue;
			}
			// ���V�m�[�h�}�b�v�ɓo�^
			m_mapOldNewNode[itNode->first] = vaImportNodeOID.lVal;
		}
	}
	std::cout << lNodeCount << " / " << lNodeSize << "\n";
	std::cout << "�m�[�h�C���|�[�g�����I��\n";

	std::cout << "�����N�C���|�[�g������...\n";
	long lLinkCount = 0, lLinkSize = m_mapOldLinkData.size();

	// ���̓����N���C���|�[�g
	for(std::map<CLinkOID,CDiffData>::const_iterator itLink = m_mapOldLinkData.begin(); itLink != m_mapOldLinkData.end(); ++itLink){
		lLinkCount++;
		if(lLinkCount % 100 == 0)
			std::cout << lLinkCount << " / " << lLinkSize << "\r";

		// �e�X�g�T�[�o�Ȃǂ�MID��MDivison�t�B�[���h�������ꍇ�͐ݒ肵�Ȃ�
		if(m_lMIDIndex >= 0 && m_lMDivision >= 0){
			m_mapLinkFieldValue[m_lMIDIndex] = itLink->second.lMID;
			m_mapLinkFieldValue[m_lMDivision] = itLink->second.lMDivision;
		}
		IGeometryPtr ipSetLinkGeom = itLink->second.ipGeom;

		// ���b�V�����E�m�[�h��1�{���������N���ڑ����Ă�����͓̂��ꏈ��
		// from���̏���
		if(setOldMeshEdgeNode.end() != setOldMeshEdgeNode.find(m_mapOldLinkFTNode[itLink->first].first)){
			ipSetLinkGeom = replaceMeshEdge(ipSetLinkGeom, true);
			if(!ipSetLinkGeom){
				std::cerr << "#NOTICE\t" << itLink->second.cLinkOID.lMesh << "\tOID : " << itLink->second.cLinkOID.lFID << "\t2�����b�V�����E�����N�Ń����N�����Z���̂ŃC���|�[�g���܂���ł���\n";
				// �`��NULL�̏ꍇ�͒[�_�폜���K�v���m�F���邱��
				if(m_mapOldNodeLinks[m_mapOldLinkFTNode[itLink->first].second].size() == 1){
					// �폜
					delete_createnode(m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].second]);
				} else {
					// 2���H�̏ꍇ�͒[�_�m�[�h�A3���H�̏ꍇ�͋@�\�����m�[�h�ɂȂ�̂ōX�V�������K�v
					update_createnode(m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].second], m_mapOldNodeLinks[m_mapOldLinkFTNode[itLink->first].second].size());
				}
				continue;
			}
			// �Z�������[�_�̃m�[�h��ǉ����Ă��
			IPointPtr ipSetPoint;
			IPolylinePtr(ipSetLinkGeom)->get_FromPoint(&ipSetPoint);
			m_mapNodeFieldValue[m_lNodeClassIndex] = 2;
			IFeatureBufferPtr ipNodeBuffer = create_FeatureBuffer(m_cInputData.ipNodeClass, IGeometryPtr(ipSetPoint), m_mapNodeFieldValue);
			if(!ipNodeBuffer){
				std::cerr << "#ERROR\t" << m_mapOldLinkFTNode[itLink->first].first << "�[�_�́[�ǃo�b�t�@��NULL���Ă�\n";
				continue;
			}
			CComVariant vaImportNodeOID;
			if(S_OK != m_ipImportNodeCursor->InsertFeature(ipNodeBuffer, &vaImportNodeOID) || vaImportNodeOID.lVal < 0){
				std::cerr << "#ERROR\t" << m_mapOldLinkFTNode[itLink->first].first << "�[�_�́[�ǂ��������ς���\n";
				continue;
			}
			// ���V�m�[�h�}�b�v���X�V
			m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].first] = vaImportNodeOID.lVal;
		}
		// to���̏���
		if(setOldMeshEdgeNode.end() != setOldMeshEdgeNode.find(m_mapOldLinkFTNode[itLink->first].second)){
			ipSetLinkGeom = replaceMeshEdge(ipSetLinkGeom, false);
			if(!ipSetLinkGeom){
				std::cerr << "#NOTICE\t" << itLink->second.cLinkOID.lMesh << "\tOID : " << itLink->second.cLinkOID.lFID << "\t2�����b�V�����E�����N�Ń����N�����Z���̂ŃC���|�[�g���܂���ł���\n";
				// �`��NULL�̏ꍇ�͒[�_�폜���K�v���m�F���邱��
				if(m_mapOldNodeLinks[m_mapOldLinkFTNode[itLink->first].first].size() == 1){
					// �폜
					delete_createnode(m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].first]);
				} else {
					// 2���H�̏ꍇ�͒[�_�m�[�h�A3���H�̏ꍇ�͋@�\�����m�[�h�ɂȂ�̂ōX�V�������K�v
					update_createnode(m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].first], m_mapOldNodeLinks[m_mapOldLinkFTNode[itLink->first].first].size());
				}
				continue;
			}
			// �Z�������[�_�̃m�[�h��ǉ����Ă��
			IPointPtr ipSetPoint;
			IPolylinePtr(ipSetLinkGeom)->get_ToPoint(&ipSetPoint);
			m_mapNodeFieldValue[m_lNodeClassIndex] = 2;
			IFeatureBufferPtr ipNodeBuffer = create_FeatureBuffer(m_cInputData.ipNodeClass, IGeometryPtr(ipSetPoint), m_mapNodeFieldValue);
			if(!ipNodeBuffer){
				std::cerr << "#ERROR\t" << m_mapOldLinkFTNode[itLink->first].second << "�[�_�́[�ǃo�b�t�@��NULL���Ă�\n";
				continue;
			}
			CComVariant vaImportNodeOID;
			if(S_OK != m_ipImportNodeCursor->InsertFeature(ipNodeBuffer, &vaImportNodeOID) || vaImportNodeOID.lVal < 0){
				std::cerr << "#ERROR\t" << m_mapOldLinkFTNode[itLink->first].second << "�[�_�́[�ǂ��������ς���\n";
				continue;
			}
			// ���V�m�[�h�}�b�v���X�V
			m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].second] = vaImportNodeOID.lVal;
		}

		// �VFromToNodeID��ݒ肷��
		m_mapLinkFieldValue[m_lFromNodeIndex] = m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].first];
		m_mapLinkFieldValue[m_lToNodeIndex] = m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].second];

		IFeatureBufferPtr ipLinkBuffer = create_FeatureBuffer(m_cInputData.ipLinkClass, ipSetLinkGeom, m_mapLinkFieldValue);
		if(!ipLinkBuffer){
			std::cerr << "#ERROR\t" << itLink->first.lMesh << " : " << itLink->first.lFID << "��񂭃o�b�t�@��NULL���Ă�\n";
			continue;
		}
		CComVariant vaImportLinkOID;
		if(S_OK != m_ipImportLinkCursor->InsertFeature(ipLinkBuffer, &vaImportLinkOID) || vaImportLinkOID.lVal < 0){
			std::cerr << "#ERROR\t" << itLink->first.lMesh << " : " << itLink->first.lFID << "��񂭂��������ς���\n";
			continue;
		}
	}
	std::cout << lLinkCount << " / " << lLinkSize << "\n";
	std::cout << "�����N�C���|�[�g�����I��\n";

	if(S_OK != m_ipImportNodeCursor->Flush()){
		std::cerr << "#ERROR\t�́[�ǃJ�[�\��Flush()�����ς���\n";
		return false;
	}
	if(S_OK != m_ipImportLinkCursor->Flush()){
		std::cerr << "#ERROR\t��񂭃J�[�\��Flush()�����ς���\n";
		return false;
	}

	std::cout << "�������f�[�^�C���|�[�g�����I����\n";

	return true;
}

IGeometryPtr CDiffAddSDE::replaceMeshEdge(IGeometry* ipGeom, bool bFrom)
{
	IClonePtr ipClone;
	IClonePtr(ipGeom)->Clone(&ipClone);
	IGeometryPtr ipLinkGeom(ipClone);

	long lPointCount = 0;
	IPointPtr ipPoint1, ipPoint2, ipSetPoint(CLSID_Point);
	WKSPoint wks1,wks2;
	double dBaseDist = BASEDIST;
	bool bBase = false;
	IPointCollectionPtr(ipLinkGeom)->get_PointCount(&lPointCount);

	long lRemoveCount = 0;
	for(lRemoveCount = 0; lRemoveCount < lPointCount-1; ++lRemoveCount){
		if(bFrom){
			IPointCollectionPtr(ipLinkGeom)->get_Point(lRemoveCount,&ipPoint1);
			IPointCollectionPtr(ipLinkGeom)->get_Point(lRemoveCount+1,&ipPoint2);
		} else {
			IPointCollectionPtr(ipLinkGeom)->get_Point(lPointCount-(lRemoveCount+1),&ipPoint1);
			IPointCollectionPtr(ipLinkGeom)->get_Point(lPointCount-(lRemoveCount+2),&ipPoint2);
		}
		ipPoint1->QueryCoords(&wks1.X,&wks1.Y);
		ipPoint2->QueryCoords(&wks2.X,&wks2.Y);
		double dDist = m_cnv.GetDist(wks1.X,wks1.Y,wks2.X,wks2.Y);
		double dLinkLen = 0.0;
		if(dDist < dBaseDist){
			dBaseDist -= dDist;
		} else {
			bBase = true;
			break;
		}
	}
	// �����N��BASEDIST�ȉ��̏ꍇ�̓C���|�[�g�ΏۂƂ��Ȃ�
	// ��FromTo�m�[�h�͊��ɃC���|�[�g����Ă���̂ō폜�������K�v���m�F����
	if(!bBase){
		return NULL;
	} else {
		WKSPoint wksF,wksT, wksHalf;
		wksF = wks1;
		wksT = wks2;
		for(long l = 0; l < 30; ++l){
			wksHalf.X = (wksF.X+wksT.X)/2.0;
			wksHalf.Y = (wksF.Y+wksT.Y)/2.0;
			double dDist = m_cnv.GetDist(wks1.X,wks1.Y,wksHalf.X,wksHalf.Y);
			if(0.001 > abs(dDist-dBaseDist)){
				break;
			}
			if(dDist > dBaseDist){
				wksT = wksHalf;
			} else {
				wksF = wksHalf;
			}
		}
		if(lRemoveCount > 0){
			if(bFrom)
				IPointCollectionPtr(ipLinkGeom)->RemovePoints(0, lRemoveCount);
			else
				IPointCollectionPtr(ipLinkGeom)->RemovePoints(lPointCount-(lRemoveCount), lRemoveCount);
		}
		ipSetPoint->PutCoords(wksHalf.X,wksHalf.Y);
		IPointCollectionPtr(ipLinkGeom)->get_PointCount(&lPointCount);
		if(bFrom)
			IPointCollectionPtr(ipLinkGeom)->UpdatePoint(0,ipSetPoint);
		else
			IPointCollectionPtr(ipLinkGeom)->UpdatePoint(lPointCount-1,ipSetPoint);

		return ipLinkGeom;
	}
}

bool CDiffAddSDE::delete_createnode(long lNewOID)
{
	IFeaturePtr ipDelNode;
	m_cInputData.ipNodeClass->GetFeature(lNewOID, &ipDelNode);
	if(!ipDelNode){
		std::cerr << "#ERROR\t" << lNewOID << "\t�����[�_�m�[�h�������s\n";
		return false;
	}
	if(S_OK != ipDelNode->Delete()){
		std::cerr << "#ERROR\t" << lNewOID << "\t�����[�_�m�[�h�폜���s\n";
		return false;
	}
	if(S_OK != ipDelNode->Store()){
		std::cerr << "#ERROR\t" << lNewOID << "\t�����[�_�m�[�h�폜��̍X�V���s\n";
		return false;
	}
	return true;
}

bool CDiffAddSDE::update_createnode(long lNewOID, long lNodeCount)
{
	long lValue = sindy::schema::road_node::node_class::kTerminal;
	if(lNodeCount == 2){
		lValue = sindy::schema::road_node::node_class::kTerminal;	// �[�_�m�[�h
	} else if(lNodeCount == 3){
		lValue = 0;	// �@�\�����m�[�h
	} else if(lNodeCount > 3){
		// 4�{�ȏ�Ȃ�1�{����Ă������_�m�[�h�Ȃ̂ōX�V�̕K�v�Ȃ�
		return true;
	}
	
	IFeaturePtr ipUpdateNode;
	m_cInputData.ipNodeClass->GetFeature(lNewOID, &ipUpdateNode);
	if(!ipUpdateNode){
		std::cerr << "#ERROR\t" << lNewOID << "\t�X�V�m�[�h�������s\n";
		return false;
	}

	if(S_OK != ipUpdateNode->put_Value(m_lNodeClassIndex, CComVariant(lValue))){
		std::cerr << "#ERROR\t" << lNewOID << "\t�X�V�m�[�h�폜���s\n";
		return false;
	}
	if(S_OK != ipUpdateNode->Store()){
		std::cerr << "#ERROR\t" << lNewOID << "\t�X�V�m�[�h�폜��̍X�V���s\n";
		return false;
	}
	return true;
}


IFeatureBufferPtr CDiffAddSDE::create_FeatureBuffer(IFeatureClass* ipFeatureClass, IGeometry* ipGeom, std::map<long,CComVariant>& mapFieldValue)
{
	IFeatureBufferPtr ipFeatureBuffer;
	if(S_OK != ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer) || !ipFeatureBuffer)
		return NULL;

	if(S_OK != ipFeatureBuffer->putref_Shape(IGeometryPtr(ipGeom)))
		return NULL;

	long lFieldCount = 0;
	IFieldsPtr ipFields;
	ipFeatureBuffer->get_Fields(&ipFields);
	ipFields->get_FieldCount(&lFieldCount);
	for(long l = 0; l < lFieldCount; l++){
		IFieldPtr ipField;
		ipFields->get_Field(l, &ipField);
		if(!ipField)
			return NULL;

		CComBSTR bstrFieldName;
		ipField->get_Name(&bstrFieldName);

		if(_tcsicmp(CString(bstrFieldName), _T("OBJECTID")) != 0 && _tcsicmp(CString(bstrFieldName), _T("SHAPE")) != 0){
			// �w��l������ΐݒ�A������Ώ����l
			if(mapFieldValue.find(l) == mapFieldValue.end()){
				// �����l
				CComVariant vaDefaultValue;
				// �ҏW�ҁA�X�V�����A�X�V���@�͌��ߑł�
				if(_tcsicmp(CString(bstrFieldName), sindy::schema::ipc_table::kOperator ) == 0)
					vaDefaultValue = CComBSTR(g_UpdateOperator);
				else if(_tcsicmp(CString(bstrFieldName), sindy::schema::ipc_table::kModifyDate) == 0)
					vaDefaultValue = CComBSTR(GetNowTime());
				else if(_tcsicmp(CString(bstrFieldName), sindy::schema::ipc_table::kUpdateType) == 0)
					vaDefaultValue = CComVariant((LONG)1);	// �V�K�쐬
				else
					ipField->get_DefaultValue(&vaDefaultValue);
				ipFeatureBuffer->put_Value(l, vaDefaultValue);
			} else {
				// �w��l
				ipFeatureBuffer->put_Value(l, mapFieldValue[l]);
			}
		}
	}
	return ipFeatureBuffer;
}

CString CDiffAddSDE::GetNowTime()
{
	SYSTEMTIME stSystemTime;
	::GetLocalTime( &stSystemTime);
	CString strStrModifyDate;
	strStrModifyDate.Format(TEXT( "%04d/%02d/%02d %02d:%02d:%02d"), stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	return strStrModifyDate;
}


bool CDiffAddSDE::DoReconcile(IWorkspaceEdit* ipWorkspaceEdit)
{
	// StopEditing()�Ɏ��s�����烊�R���T�C��
	HRESULT hr = ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
	bool bRet = false;
	switch(hr)
	{
		case FDO_E_VERSION_REDEFINED:
		{
			std::cout << "�����R���T�C����...\n";
			std::cerr << "#NOTECE\t���R���T�C�����s���܂�\n";
			IVersionEditPtr ipVersionEdit(ipWorkspaceEdit);
			if( ipVersionEdit )
			{
				IVersionInfoPtr ipVersionInfo, ipParentVersionInfo;
				IVersionPtr ipVersion( ipVersionEdit );
				CComBSTR bstrVersionName, bstrParentVersionName;
				if(ipVersion){
					if( SUCCEEDED( ipVersion->get_VersionInfo( &ipVersionInfo ) ) )
						ipVersionInfo->get_VersionName( &bstrVersionName );
				}
				if(!CString(bstrVersionName).IsEmpty()){
					VARIANT_BOOL vbConflicts = VARIANT_FALSE;
					HRESULT hrReconcile = hr = ipVersionEdit->Reconcile( bstrParentVersionName, &vbConflicts );
					switch( hrReconcile )
					{
						case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE:
							std::cerr << "#ERROR\t���R���T�C�����s�FFDO_E_RECONCILE_VERSION_NOT_AVAILABLE\n";
							break;
						case FDO_E_VERSION_BEING_EDITED:
							std::cerr << "#ERROR\t���R���T�C�����s�FFDO_E_VERSION_BEING_EDITED\n";
							break;
						case FDO_E_VERSION_NOT_FOUND:
							std::cerr << "#ERROR\t���R���T�C�����s�FFDO_E_VERSION_NOT_FOUND\n";
							break;
						case S_OK:
							// �R���t���N�g�����ꍇ�̓G���[�Ƃ����A���̂܂ܔ�����
							// �R���t���N�g���̏����͌Ăяo����ɔC����
							if( vbConflicts )
								std::cerr << "#ERROR\t���R���T�C�������F�R���t���N�g���������܂���\n";
							else {
								// �w���v�ɂ́AReconcile���I��������StopEditing�������I�ɌĂ΂��悤�Ȃ��Ƃ�
								// �����Ă������̂ŁA�������Ȃ��Ă������̂��ȁH�H
								std::cerr << "#NOTICE\t���R���T�C�������F�ҏW�I��\n";
								std::cout << "�������R���T�C������\t�ҏW�I������\n";
							}
							bRet = true;
						break;
						default:
							std::cerr << "#ERROR\t���R���T�C�����Ɍ����s���̃G���[���������܂���\n";
							break;
					}
				}
				else
					std::cerr << "#ERROR\tIVersionEdit����o�[�W�������̎擾���s\n";
			}
			else
				std::cerr << "#ERROR\tIVersionEdit�̎擾���s\n";
		}
			break;
		case S_OK:
			std::cout << "�����ҏW�ۑ���������\n";
			bRet = true;
			break;
		default:
			std::cerr << "#ERROR\t�ҏW�ۑ��̏������ɃG���[���������܂���\n";
			break;
	}
	return bRet;
}




void CDiffAddSDE::omajinai()
{
	std::vector<long> tmplist;
	for(long l = 1; l <= 1000; ++l)	// 1000�ŗǂ��H
		tmplist.push_back(l);

	_ICursorPtr ipCursor;

	SAFEARRAYBOUND bound = { (ULONG)tmplist.size(), 0 };
	SAFEARRAY* pSa = SafeArrayCreate( VT_I4, 1, &bound );
	LONG* pArray;
	SafeArrayAccessData( pSa, (void**)&pArray );

	long i = 0;
	for( std::vector<long>::const_iterator it = tmplist.begin(); it != tmplist.end(); ++it, ++i )
		pArray[i] = *it;

	SafeArrayUnaccessData( pSa );
	VARIANT vaArray;
	VariantInit( &vaArray );
	vaArray.vt = VT_ARRAY | VT_I4;
	vaArray.parray = pSa;

	ITablePtr(m_cInputData.ipLinkClass)->GetRows(vaArray, VARIANT_TRUE, &ipCursor);

	VariantClear( &vaArray );
}
