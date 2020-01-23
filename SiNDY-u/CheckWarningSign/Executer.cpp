#include "stdafx.h"
#include "LogSys.h"
#include "common.h"
#include "TransParam.h"
#include "CheckWarningSign.h"
#include "Executer.h"

#include <WinLib/VersionInfo.h>
#include <WinLib/CStringTokenizer.h>

using namespace sindy::schema;

// 実行ログ・エラーログの作成
bool initLog(const TransParam& transParam)
{
	if (! LogSys::getInstance().setProcLog(transParam.getRunLog())) {
		std::cerr << "ERROR\t" << "実行ログの作成に失敗しました\t" << CT2A(transParam.getRunLog()) << std::endl;
		return false;
	}
	if (! LogSys::getInstance().setErrLog(transParam.getErrLog())) {
		std::cerr << "ERROR\t" << "エラーログの作成に失敗しました\t" << CT2A(transParam.getErrLog()) << std::endl;
		return false;
	}
	LogSys::getInstance().writeErrLog(false, false, _T("# FREESTYLELOG\n"));
	LogSys::getInstance().writeErrLog(false, false, _T("# フラグ\tレイヤ名\tオブジェクトID\tエラーレベル\tエラーメッセージ\n"));
	return true;
}

// ツール情報の出力
void procInfoWrite(TransParam& transParam)
{
	// ツール情報の表示
	CVersion toolVersion;
	CString toolInfo;
	toolInfo.Format(_T("%s VERSION:%s\n"), toolVersion.GetOriginalFilename(), toolVersion.GetFileVersion());
	LogSys::getInstance().writeProcLog(true, false, toolInfo);
	// オプションの表示
	LogSys::getInstance().writeProcLog(true, false, CA2CT(transParam.getOptions().c_str()));
}

// 出力ログの作成
bool openOutputLog(const TransParam& transParam, std::ofstream	&ofsOutputLog)
{
	ofsOutputLog.open( transParam.getOutputLog());
	if(! ofsOutputLog.is_open()){
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("出力ログのオープンに失敗しました"));
		return false;
	}

	return true;
}

// コード値ドメインリストの作成
bool domainlist(const IFieldPtr& ipRoadField, std::map<long, CString>& mapDomain)
{
	IDomainPtr ipDomain;
	if(ipRoadField->get_Domain(&ipDomain) != S_OK){
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("コード値ドメインの取得に失敗しました"));
		return false;
	}
	ICodedValueDomainPtr ipRoadDomain = ipDomain;

	long count = 0;
	if(FAILED(ipRoadDomain->get_CodeCount(&count))){
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("コード値ドメインのコード数の取得に失敗しました"));
		return false;
	}

	for(int i = 0; i < count; ++i){
		CComVariant rVal;
		CComBSTR rName;
		if(FAILED(ipRoadDomain->get_Value(i, &rVal))){
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("コード値の取得に失敗しました"));
			return false;
		}
		if(FAILED(ipRoadDomain->get_Name(i, &rName))){
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("コード名の取得に失敗しました"));
			return false;
		}
		mapDomain.insert(std::make_pair(rVal.lVal, CString(rName)));
	}
	return true;
}

// 道路リンクデータの取得
bool getRoadLink(const TransParam& transParam, const IWorkspacePtr& ipRoadWorkspace, IFeatureClassPtr& ipRoadFC, ISpatialReferencePtr& ipRoadSPRef, std::map<long, CString>& mapDomain, long& roadClassIndex)
{
	// 道路フィーチャクラスの取得
	if (! common_proc::getFeatureClass(ipRoadWorkspace, road_link::kTableName, transParam.getRoadOwner(), ipRoadFC))
		return false;

	// 空間参照取得
	if(FAILED(((IGeoDatasetPtr)ipRoadFC)->get_SpatialReference( &ipRoadSPRef))){
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("空間参照の取得に失敗しました"));
		return false;
	}

	// 道路リンクテーブルのフィールド群取得
	IFieldsPtr ipRoadFields;
	if (! common_proc::getFields(road_link::kTableName, ipRoadFC, ipRoadFields))
		return false;

	if(ipRoadFC->FindField(CComBSTR(road_link::kRoadClass), &roadClassIndex) != S_OK ) {
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("道路標示種別インデックスの検索に失敗しました"));
		return false;
	}

	IFieldPtr ipRoadField;
	if (! common_proc::getField(road_link::kTableName, ipRoadFields, roadClassIndex, ipRoadField))
		return false;

	// 道路種別ドメイン取得処理
	if(! domainlist(ipRoadField,mapDomain))
		return false;

	return true;
}

// 警戒標識データの取得
bool getWarningSign(const TransParam& transParam, const IWorkspacePtr& ipWSWorkspace, IFeatureClassPtr& ipWSFC, std::map<long, CAdapt<IPointPtr>>& mapGeo)
{
	// 警戒標識フィーチャクラス取得
	CComBSTR WSTableName = _T("warning_sign");
	if (! common_proc::getFeatureClass(ipWSWorkspace, WSTableName, transParam.getWaringSignOwner(), ipWSFC))
		return false;

	// フィーチャ取得
	CString WSWhereCluse;
	IFeatureCursorPtr ipWSFeatureCursor;
	if (! common_proc::getFeatureCursor(ipWSFC, WSTableName, NULL, esriSpatialRelUndefined,WSWhereCluse, ipWSFeatureCursor))
		return false;

	long OIDIndex = 0;
	if(ipWSFC -> FindField(_T("OBJECTID"),&OIDIndex) != S_OK ) {
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("警戒標識オブジェクトIDインデックスの検索に失敗しました"));
		return false;
	}

	IFeaturePtr ipFeature;
	while(ipWSFeatureCursor->NextFeature(&ipFeature) == S_OK ) {
		CComVariant id;
		if(FAILED(ipFeature->get_Value(OIDIndex, &id))){
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("警戒標識のオブジェクトIDの取得に失敗しました"));
			return false;
		}
		IGeometryPtr ipGeo;
		if(FAILED(ipFeature->get_ShapeCopy(&ipGeo))) {
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("ジオメトリのコピーの取得に失敗しました"));
			return false;
		}
		IPointPtr ipPoint(ipGeo);
		mapGeo.insert(std::make_pair(id.lVal, ipPoint));	// 警戒標識のOIDと形状を格納
	}
	return true;
}

// 全体処理
bool exec(const TransParam &transParam)
{
	LogSys::getInstance().writeProcLog(true, true, _T("変換開始\n"));

	std::map<long, CAdapt<IPointPtr>> mapGeo;		// 警戒標識データ情報格納用
	std::map<long, CString> mapDomain;				// 道路表示種別データ格納用

	// 出力ログの作成処理
	std::ofstream	ofsOutputLog;
	if(! openOutputLog(transParam, ofsOutputLog))
		return false;

	// ワークスペース取得処理
	IWorkspacePtr ipRoadWorkspace, ipWSWorkspace;
	IFeatureClassPtr ipRoadFC,ipWSFC;
	long roadClassIndex = 0;

	if (! common_proc::getWorkspace(transParam.getRoadDB(), ipRoadWorkspace))
		return false;
	if (! common_proc::getWorkspace(transParam.getWarningSignDB(), ipWSWorkspace))
		return false;

	// 警戒標識データの取得処理
	if(! getWarningSign(transParam, ipWSWorkspace, ipWSFC,mapGeo))
		return false;

	// 道路リンクデータの取得処理
	ISpatialReferencePtr ipRoadSPRef;
	if(! getRoadLink(transParam, ipRoadWorkspace, ipRoadFC, ipRoadSPRef, mapDomain, roadClassIndex))
		return false;

	// 最近傍道路リンク取得処理
	if (! checkWarningSign(transParam, ipRoadFC, ipRoadSPRef, roadClassIndex, mapGeo, mapDomain, ofsOutputLog))
		return false;

	LogSys::getInstance().writeProcLog(true, true, _T("変換完了\n"));
	return true;
}