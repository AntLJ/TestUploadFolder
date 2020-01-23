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


#include <stdafx.h>

#include "diffadd.h"
#include <sindy/workspace.h>

bool CDiffAdd::run()
{
	// �����f�[�^�ɒǉ����郊�X�g�����b�V�����ɕ����Ă���
	if(!get_diffaddlist())
		return false;

	std::cout << "�������ǉ������J�n������\n";

	// �ǉ�����f�[�^�����b�V�����Ƀ��[�v����
	long lMeshCount = 0, lMeshSize = m_mapAddList.size();
	for(std::map<CString,std::set<long>>::const_iterator it = m_mapAddList.begin(); it != m_mapAddList.end(); ++it){
		std::cout << "======= " << ++lMeshCount << " / "  << lMeshSize << " (" << CT2A(it->first) <<")=======\n";

		// ���f�[�^�̃f�B���N�g���\��
		// [�w��f�B���N�g��]-[2�����b�V���f�B���N�g��]-[bas_link.shp]
		CString strBaseWork;
		strBaseWork.Format(_T("%s\\%s"), m_cInputData.strSShapeDir, it->first);
		IWorkspacePtr ipBaseWork = sindy::create_workspace(strBaseWork);
		if(!ipBaseWork){
			std::cerr << "#ERROR\t" << CT2A(strBaseWork) << "\tS�Ќ��f�[�^���[�N�X�y�[�X�I�[�v�����s\n";
			continue;
		}
		IFeatureClassPtr ipBaseLinkClass;
		IFeatureWorkspacePtr(ipBaseWork)->OpenFeatureClass(CComBSTR(g_sbaselink), &ipBaseLinkClass);
		if(!ipBaseLinkClass){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\tS�Ќ��f�[�^�t�B�[�`���N���X�I�[�v�����s\n";
			continue;
		}
		long lFIDIndex = ipBaseLinkClass->GetFields()->_FindField(_bstr_t(g_FID));
		if(lFIDIndex < 0){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\tS�Ќ��f�[�^��FID�t�B�[���h�C���f�b�N�X�擾���s\n";
			continue;
		}

		// �����f�[�^�̃f�B���N�g���\��
		// [�w��f�B���N�g��]-[1�����b�V���f�B���N�g��]-[2�����b�V���f�B���N�g��]-[2�����b�V��_diff.shp]
		CString strDiffWork;
		strDiffWork.Format(_T("%s\\%s\\%s"), m_cInputData.strDiffShapeDir, (it->first).Left(4), it->first);
		IWorkspacePtr ipDiffWork = sindy::create_workspace(strDiffWork);
		if(!ipDiffWork){
			std::cerr << "#ERROR\t" << CT2A(strDiffWork) << "\t�����f�[�^���[�N�X�y�[�X�I�[�v�����s\n";
			continue;
		}
		IFeatureClassPtr ipDiffLinkClass;
		IFeatureWorkspacePtr(ipDiffWork)->OpenFeatureClass(CComBSTR(it->first+_T("_diff")), &ipDiffLinkClass);
		if(!ipDiffLinkClass){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\t�����f�[�^�t�B�[�`���N���X�I�[�v�����s\n";
			continue;
		}
		long lMIDIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_MID));
		long lMDivisionIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_MDivision));
		if(lMIDIndex < 0 || lMDivisionIndex < 0){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\t�����f�[�^��" << CT2A(g_MID) << "," << CT2A(g_MDivision) << "�t�B�[���h�C���f�b�N�X�擾���s\n";
			continue;
		}



		// �ǉ��p�̃f�[�^�����f�[�^����擾���Ă���
		std::map<long,CAdapt<IFeaturePtr>> mapAddData;
		IFeatureCursorPtr ipBaseLinkCursor;
		ipBaseLinkClass->Search(NULL,VARIANT_FALSE,&ipBaseLinkCursor);
		if(!ipBaseLinkCursor){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\tS�Ќ��f�[�^�J�[�\���擾���s\n";
			continue;
		}
		IFeaturePtr ipBaseLink;
		// �߂�ǂ����[����S�Ќ��f�[�^�S������ǉ��Ώۃf�[�^���i�荞��
		// �iS�Ќ��f�[�^��1���b�V��1�������Ȃ����낤���瑬�x�̓L�j�V�i�C�j
		while(S_OK == ipBaseLinkCursor->NextFeature(&ipBaseLink)){
			CComVariant vaFID;
			ipBaseLink->get_Value(lFIDIndex, &vaFID);
			if(it->second.find(vaFID.lVal) != it->second.end())
				mapAddData[vaFID.lVal] = ipBaseLink;
		}


		// SHP�n���h��
		CSHPHandle cSHPHdl;		// �V�F�C�v�n���h���N���X(Shapelib)
		CDBFHandle cDBFHdl;		// DBF�t�@�C���n���h���N���X
		CString strDiffShp;
		strDiffShp.Format(_T("%s\\%s_diff"), strDiffWork, it->first);
		if(!cDBFHdl.open(CT2A(strDiffShp), "rb+")){
			continue;
		}
		if(!cSHPHdl.open(CT2A(strDiffShp), "rb+")){
			continue;
		}
		int iMIDFIndex = cDBFHdl.get_FieldID(CT2A(g_MID));
		int iMDivisionFIndex = cDBFHdl.get_FieldID(CT2A(g_MDivision));
		if(iMIDFIndex < 0 || iMDivisionFIndex < 0){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\t�����f�[�^��" << CT2A(g_MID) << "," << CT2A(g_MDivision) << "�t�B�[���h�C���f�b�N�X�擾���s\n";
			continue;
		}
		// SHP�Ƀf�[�^��ǉ�����
		long lAddCount = 0, lAddSize = mapAddData.size();
		for(std::map<long,CAdapt<IFeaturePtr>>::const_iterator itAdd = mapAddData.begin(); itAdd != mapAddData.end(); ++itAdd){
			std::cout << ++lAddCount << " / " << lAddSize << " ��\r";

			IGeometryPtr ipGeom;
			itAdd->second.m_T->get_ShapeCopy(&ipGeom);
			long lPointCount = 0;
			IPointCollectionPtr(ipGeom)->get_PointCount( &lPointCount );
			CSHPPointArray aPointArray;
			for(long l = 0; l < lPointCount; ++l){
				IPointPtr ipPoint;
				IPointCollectionPtr(ipGeom)->get_Point( l, &ipPoint );
				double dX = 0.0, dY = 0.0;
				ipPoint->QueryCoords( &dX, &dY );
				CSHPPoint aSHPPoint( dX, dY );
				aPointArray.push_back(aSHPPoint);
			}
			// shape�`��ɂ���
			CSHPObject aArcObj(SHPT_ARC, aPointArray);
			// �����f�[�^�ɂɃt�B�[�`����ǉ��ishape�f�[�^��˂����ށj
			int nID = cSHPHdl.write_Object(&aArcObj, -1);
			if(nID < 0){
				std::cerr << "#ERROR\t" << CT2A(it->first) << "\t" << itAdd->first << "\t" << "�ǉ����s\n";
				continue;
			}
			cDBFHdl.write_Attribute(nID, iMIDFIndex, itAdd->first);
			cDBFHdl.write_Attribute(nID, iMDivisionFIndex, m_cInputData.lMDivision);
		}
		std::cout << lAddCount << " / " << lAddSize << " ��\n";
		cSHPHdl.close ();
		cDBFHdl.close ();
	}
	std::cout << "�������ǉ������I��������\n";

	return true;
}

bool CDiffAdd::get_diffaddlist()
{
	std::ifstream ifs;
	ifs.open(m_cInputData.strDiffAddList);
	if(ifs.fail())
		return false;

	char cLine[256];
	while(ifs.getline(cLine,256)){
		CString strLine(cLine);

		int iPos = 0;
		CString strMesh = strLine.Tokenize(_T("\t"),iPos);
		long lFID = _ttol(strLine.Tokenize(_T("\t"),iPos));

		if(!(m_mapAddList[strMesh].insert(lFID).second)){
			std::cerr << "#WARNING\t" << CT2A(strMesh) << "\t" << lFID << "\t��������\n";
		}
	}

	return true;
}
