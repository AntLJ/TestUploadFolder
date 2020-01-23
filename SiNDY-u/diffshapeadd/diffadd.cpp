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
	// 差分データに追加するリストをメッシュ毎に分けておく
	if(!get_diffaddlist())
		return false;

	std::cout << "◆◆◆追加処理開始◆◆◆\n";

	// 追加するデータをメッシュ毎にループ処理
	long lMeshCount = 0, lMeshSize = m_mapAddList.size();
	for(std::map<CString,std::set<long>>::const_iterator it = m_mapAddList.begin(); it != m_mapAddList.end(); ++it){
		std::cout << "======= " << ++lMeshCount << " / "  << lMeshSize << " (" << CT2A(it->first) <<")=======\n";

		// 元データのディレクトリ構造
		// [指定ディレクトリ]-[2次メッシュディレクトリ]-[bas_link.shp]
		CString strBaseWork;
		strBaseWork.Format(_T("%s\\%s"), m_cInputData.strSShapeDir, it->first);
		IWorkspacePtr ipBaseWork = sindy::create_workspace(strBaseWork);
		if(!ipBaseWork){
			std::cerr << "#ERROR\t" << CT2A(strBaseWork) << "\tS社元データワークスペースオープン失敗\n";
			continue;
		}
		IFeatureClassPtr ipBaseLinkClass;
		IFeatureWorkspacePtr(ipBaseWork)->OpenFeatureClass(CComBSTR(g_sbaselink), &ipBaseLinkClass);
		if(!ipBaseLinkClass){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\tS社元データフィーチャクラスオープン失敗\n";
			continue;
		}
		long lFIDIndex = ipBaseLinkClass->GetFields()->_FindField(_bstr_t(g_FID));
		if(lFIDIndex < 0){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\tS社元データのFIDフィールドインデックス取得失敗\n";
			continue;
		}

		// 差分データのディレクトリ構造
		// [指定ディレクトリ]-[1次メッシュディレクトリ]-[2次メッシュディレクトリ]-[2次メッシュ_diff.shp]
		CString strDiffWork;
		strDiffWork.Format(_T("%s\\%s\\%s"), m_cInputData.strDiffShapeDir, (it->first).Left(4), it->first);
		IWorkspacePtr ipDiffWork = sindy::create_workspace(strDiffWork);
		if(!ipDiffWork){
			std::cerr << "#ERROR\t" << CT2A(strDiffWork) << "\t差分データワークスペースオープン失敗\n";
			continue;
		}
		IFeatureClassPtr ipDiffLinkClass;
		IFeatureWorkspacePtr(ipDiffWork)->OpenFeatureClass(CComBSTR(it->first+_T("_diff")), &ipDiffLinkClass);
		if(!ipDiffLinkClass){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\t差分データフィーチャクラスオープン失敗\n";
			continue;
		}
		long lMIDIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_MID));
		long lMDivisionIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_MDivision));
		if(lMIDIndex < 0 || lMDivisionIndex < 0){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\t差分データの" << CT2A(g_MID) << "," << CT2A(g_MDivision) << "フィールドインデックス取得失敗\n";
			continue;
		}



		// 追加用のデータを元データから取得しておく
		std::map<long,CAdapt<IFeaturePtr>> mapAddData;
		IFeatureCursorPtr ipBaseLinkCursor;
		ipBaseLinkClass->Search(NULL,VARIANT_FALSE,&ipBaseLinkCursor);
		if(!ipBaseLinkCursor){
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\tS社元データカーソル取得失敗\n";
			continue;
		}
		IFeaturePtr ipBaseLink;
		// めんどくせーからS社元データ全件から追加対象データを絞り込む
		// （S社元データが1メッシュ1万件もないだろうから速度はキニシナイ）
		while(S_OK == ipBaseLinkCursor->NextFeature(&ipBaseLink)){
			CComVariant vaFID;
			ipBaseLink->get_Value(lFIDIndex, &vaFID);
			if(it->second.find(vaFID.lVal) != it->second.end())
				mapAddData[vaFID.lVal] = ipBaseLink;
		}


		// SHPハンドル
		CSHPHandle cSHPHdl;		// シェイプハンドルクラス(Shapelib)
		CDBFHandle cDBFHdl;		// DBFファイルハンドルクラス
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
			std::cerr << "#ERROR\t" << CT2A(it->first) << "\t差分データの" << CT2A(g_MID) << "," << CT2A(g_MDivision) << "フィールドインデックス取得失敗\n";
			continue;
		}
		// SHPにデータを追加する
		long lAddCount = 0, lAddSize = mapAddData.size();
		for(std::map<long,CAdapt<IFeaturePtr>>::const_iterator itAdd = mapAddData.begin(); itAdd != mapAddData.end(); ++itAdd){
			std::cout << ++lAddCount << " / " << lAddSize << " 件\r";

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
			// shape形状にする
			CSHPObject aArcObj(SHPT_ARC, aPointArray);
			// 差分データににフィーチャを追加（shapeデータを突っ込む）
			int nID = cSHPHdl.write_Object(&aArcObj, -1);
			if(nID < 0){
				std::cerr << "#ERROR\t" << CT2A(it->first) << "\t" << itAdd->first << "\t" << "追加失敗\n";
				continue;
			}
			cDBFHdl.write_Attribute(nID, iMIDFIndex, itAdd->first);
			cDBFHdl.write_Attribute(nID, iMDivisionFIndex, m_cInputData.lMDivision);
		}
		std::cout << lAddCount << " / " << lAddSize << " 件\n";
		cSHPHdl.close ();
		cDBFHdl.close ();
	}
	std::cout << "◆◆◆追加処理終了◆◆◆\n";

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
			std::cerr << "#WARNING\t" << CT2A(strMesh) << "\t" << lFID << "\t複数あり\n";
		}
	}

	return true;
}
