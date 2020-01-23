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
#include "Arguments.h"
#include "CheckRelPointPolygon.h"

#include <define.h>
#include <define_enum.h>
#include <GlobalFunctions.h>
#include <AheGlobals.h>
#include <AheLayerFunctions.h>
#include <sindy/workspace/name.h>
#include <VersionInfo.h>
#include <TDC/useful_headers/str_util.h>

using namespace sindy;
using namespace sindy::schema;

// チェック対象が1000000件以上の場合は警告文を出す
const int WARNINGCNT = 1000000;

// ポイントとポリゴンの関係をチェックする
bool CCheckRelPointPolygon::Check(const Arguments& args)
{
	if(!Init(args))
		return false;

	// 開始時のログを書きこむ
	WriteStartLog(args);

	// ワークスペース取得
	IWorkspacePtr ipPointWork, ipPolyWork;
	if(!GetWorkspace(ipPointWork, ipPolyWork))
	{
		WriteEndLog(false);
		return false;
	}

	// フィーチャクラス取得
	IFeatureClassPtr ipPointFC, ipPolyFC;
	if(!GetFeatureClass(ipPointWork, ipPolyWork, ipPointFC, ipPolyFC))
	{
		WriteEndLog(false);
		return false;
	}
	
	long pointCnt = -1, polyCnt = -1; // チェック対象の数
	if(ipPointFC->FeatureCount(AheInitQueryFilter(NULL, m_PointSubF, m_PointWhere), &pointCnt) == S_OK &&
		ipPolyFC->FeatureCount(AheInitQueryFilter(NULL, m_PolySubF, m_PolyWhere), &polyCnt) == S_OK)
	{
		long allCheckCnt = pointCnt + polyCnt;
		if(allCheckCnt > WARNINGCNT)
		{
			std::cout << "# WARNING\tチェック対象が " << allCheckCnt << " 件あります" << std::endl
				<< "# チェックに時間がかかり、サーバにも負担がかかります" << std::endl;
		}
	}
	else
	{
		m_RunLog << "チェック対象のポリゴンもしくはポイントの件数取得に失敗しました" << std::endl;
		m_RunLog << "オプションの設定を確認してください" << std::endl;
		WriteEndLog(false);
		return false;
	}

	m_RunLog << "チェック対象ポイント：" << pointCnt << "件" << std::endl
		<< "チェック対象ポリゴン：" << polyCnt << "件" << std::endl;

	// ポイントチェック
	IFeatureCursorPtr ipCur = ipPointFC->_Search(AheInitQueryFilter(NULL, m_PointSubF, m_PointWhere), true);
	IFeaturePtr ipFeature;
	if(ipCur)
	{
		while(ipCur->NextFeature(&ipFeature) == S_OK && ipFeature)
		{
			CheckIntersects(ipFeature, ipPolyFC, _T("指定されたポリゴン上にありません"));
		}
	}
	else
	{
		m_RunLog << "チェック対象ポイントの取得に失敗しました" << std::endl;
		m_RunLog << "オプションの設定を確認してください" << std::endl;
		WriteEndLog(false);
		return false;
	}
	std::cout << "ポイントチェック完了" << std::endl;
	m_RunLog << "ポイントチェック完了" << std::endl;

	// ポリゴンチェック
	ipCur = ipPolyFC->_Search(AheInitQueryFilter(NULL, m_PolySubF, m_PolyWhere), true);
	if(ipCur)
	{
		while(ipCur->NextFeature(&ipFeature) == S_OK && ipFeature)
		{
			CheckIntersects(ipFeature, ipPointFC, _T("指定されたポイントを含んでいません"));
			CheckIntersects(ipFeature, ipPolyFC, _T("レイヤ同士で重なっています"), true);
		}
	}
	else
	{
		m_RunLog << "チェック対象ポリゴンの取得に失敗しました" << std::endl;
		m_RunLog << "オプションの設定を確認してください" << std::endl;
		WriteEndLog(false);
		return false;
	}
	std::cout << "ポリゴンチェック完了" << std::endl;
	m_RunLog << "ポリゴンチェック完了" << std::endl;

	WriteEndLog();
	return true;
}

// 初期化
bool CCheckRelPointPolygon::Init(const Arguments& args)
{
	using namespace std;
	m_PointConnect = args.m_PointConnect;
	m_PolyConnect = args.m_PolyConnect;
	m_Point = args.m_Point;
	m_Poly = args.m_Poly;
	m_PointSubF.Format(_T("%s,SHAPE"), kObjectID);
	m_PointWhere = args.m_PointWhere.c_str();
	m_PolySubF.Format(_T("%s,SHAPE"), kObjectID);
	m_PolyWhere = args.m_PolyWhere.c_str();
	m_RunLog.open(uh::toWStr(args.m_RunLog));
	m_ErrLog.open(uh::toWStr(args.m_ErrLog));

	if(!m_RunLog.is_open())
	{
		cerr << "実行ログファイルが開けませんでした" << endl;
		return false;
	}
	if(!m_ErrLog.is_open())
	{
		cerr << "エラーログファイルを開けませんでした" << endl;
		return false;
	}
	
	return true;
}

// ワークスペース取得
bool CCheckRelPointPolygon::GetWorkspace(IWorkspacePtr &ipPointWork, IWorkspacePtr &ipPolyWork)
{
	ipPointWork = INamePtr(create_workspace_name(m_PointConnect.c_str()))->_Open();
	if(!ipPointWork)
	{
		m_RunLog << "ポイントの接続先に接続できませんでした" << std::endl;
		return false;
	}
	std::cout << uh::toStr(m_PointConnect) << "に接続しました" << std::endl;
	
	ipPolyWork = INamePtr(create_workspace_name(m_PolyConnect.c_str()))->_Open();
	if(!ipPolyWork)
	{
		m_RunLog << "ポリゴンの接続先に接続できませんでした" << std::endl;
		return false;
	}
	std::cout << uh::toStr(m_PolyConnect) << "に接続しました" << std::endl;

	return true;
}

// フィーチャクラス取得
bool CCheckRelPointPolygon::GetFeatureClass(IWorkspacePtr ipPointWork, IWorkspacePtr ipPolyWork, IFeatureClassPtr& ipPointFC, IFeatureClassPtr& ipPolyFC)
{
	IFeatureWorkspacePtr(ipPointWork)->OpenFeatureClass(CComBSTR(m_Point.c_str()), &ipPointFC);

	if(!ipPointFC)
	{
		m_RunLog << "[point]で指定されたフィーチャクラスを取得できませんでした" << std::endl;
		return false;
	}

	IFeatureWorkspacePtr(ipPolyWork)->OpenFeatureClass(CComBSTR(m_Poly.c_str()), &ipPolyFC);
	if(!ipPolyFC)
	{
		m_RunLog << "[polygon]で指定されたフィーチャクラスを取得できませんでした" << std::endl;
		return false;
	}
	return true;
}

// 指定されたフィーチャが指定されたフィーチャクラスに重なっているかをチェックする
void CCheckRelPointPolygon::CheckIntersects(IFeaturePtr ipFeature, IFeatureClassPtr ipFeatureClass, const CString& strErrMsg, bool bIsMyself)
{
	long OID = ipFeature->GetOID();
	IGeometryPtr ipGeom = ipFeature->GetShape();
	esriGeometryType type = ipGeom->GetGeometryType();
	IFeatureCursorPtr ipFeatureCur;
	CString strName = AheGetFeatureClassName(ipFeature);

	// 包含されているか確認(intersectsとなっているか)
	LONG cnt = -1;
	switch(type)
	{
	case esriGeometryPoint:
		cnt= AheSelectByShapeAndGetCount(ipGeom, ipFeatureClass, &ipFeatureCur, esriSpatialRelIntersects, m_PolySubF, m_PolyWhere);
		break;
	case esriGeometryPolygon:
		cnt= AheSelectByShapeAndGetCount(ipGeom, ipFeatureClass, &ipFeatureCur, esriSpatialRelIntersects, m_PointSubF, m_PointWhere);
		break;
	default:
		WriteErrLog(strName, OID, _T("形状を持たないオブジェクトがあります"));
		// fall through
		return;
	}

	// 同レイヤとの比較の場合
	if(bIsMyself)
	{
		--cnt; // 自分自身が含まれるので
		if(cnt > 0)
			WriteErrLog(strName, OID, strErrMsg);
	}
	else if(cnt == 0)
		WriteErrLog(strName, OID, strErrMsg);

}

// 開始時のログを書きこむ
void CCheckRelPointPolygon::WriteStartLog(const Arguments& args)
{
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CVersion cVer;
	// 実行ログ
	m_RunLog << (LPCSTR)CStringA(cVer.GetInternalName());
	m_RunLog << "\tFILEVERSION:" << (LPCSTR)CStringA(cVer.GetFileVersion());
	m_RunLog << "\tPRODUCTVERSION:" << (LPCSTR)CStringA(cVer.GetProductVersion()) << std::endl;
	m_RunLog << "\n[option]---------------------------------------" << std::endl;
	m_RunLog << "ポイントの接続先\t" << uh::toStr(args.m_PointConnect) << std::endl;
	m_RunLog << "ポリゴンの接続先\t" << uh::toStr(args.m_PolyConnect) << std::endl;
	m_RunLog << "ポイント\t" << uh::toStr(m_Point) << std::endl;
	m_RunLog << "ポリゴン\t" << uh::toStr(m_Poly) << std::endl;
	m_RunLog << "WHERE句(ポイント)\t" << CT2A(m_PointWhere) << std::endl;
	m_RunLog << "WHERE句(ポリゴン)\t" << CT2A(m_PolyWhere) << std::endl;
	m_RunLog << "-----------------------------------------------\n" << std::endl;

	CString strTime = uh::str_util::ToString(&aTime);
	m_RunLog << "開始時刻\t" << CT2A(strTime) << std::endl;

	// エラーログ
	m_ErrLog << "# SINDYSTDLOG" << std::endl;
	m_ErrLog << "# --- エラー出力 ---" << std::endl;
}

// エラーログを書く
void CCheckRelPointPolygon::WriteErrLog(const CString& strObject, long lOID, const CString& strErrMsg)
{
	m_ErrLog << "0\t" << CT2A(strObject) << "\t" << lOID << "\t\t\t\t\t" << CT2A(strErrMsg) << "\t" << std::endl;
}

// 終了時のログを書きこむ
void CCheckRelPointPolygon::WriteEndLog(bool success /*= true*/)
{
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CString strTime = uh::str_util::ToString(&aTime);
	m_RunLog << "終了時刻\t" << CT2A(strTime) << std::endl;
	if(success)
	{
		m_RunLog << "正常終了" << std::endl;
		m_ErrLog << "#正常終了" << std::endl;
	}
	else
	{
		m_RunLog << "異常終了" << std::endl;
		m_ErrLog << "#異常終了" << std::endl;
	}
}
