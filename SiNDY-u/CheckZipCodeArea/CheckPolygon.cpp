#include "stdafx.h"
#include "CheckPolygon.h"
#include <TDC/useful_headers/text_util.h>
#include <WinLib/type_convert.h>

// プロトタイプ宣言
bool IsAdjacent(const IGeometryPtr& baseGeometry, const IGeometryPtr& compGeometry);

using namespace sindy::schema;

namespace ErrorCode
{
	// CheckPolygonのエラーコードは004xxxとする
	const CString DoesNotMergePolygon = _T("004001");
}

CheckPolygon::CheckPolygon()
{
}


CheckPolygon::~CheckPolygon()
{
}

bool CheckPolygon::Init()
{
	RUNLOGGER.Info(_T("ポリゴンチェック初期化…\n"));

	if (m_args.m_mesh.empty())
	{
		RUNLOGGER.Error(_T("メッシュを指定してください\n"));
		return false;
	}
	m_meshes = uh::text_util::ToLongList(m_args.m_mesh);
	
	// SDE(郵便番号ポリゴン)接続
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンDB接続先への接続に失敗\n"));
		return false;
	}

	// 郵便番号ポリゴン取得
	m_ipZipCodeArea = zipWorkspace.OpenTable(zipcode_area::kTableName);
	if (!m_ipZipCodeArea)
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンフィーチャクラス取得失敗\n"));
		return false;
	}

	// 空間参照取得
	((IGeoDatasetPtr)m_ipZipCodeArea)->get_SpatialReference(&m_ipSpRef);
	if (!m_ipSpRef)
	{
		RUNLOGGER.Error(_T("空間参照取得失敗"));
		return false;
	}

	IFieldsPtr ipFields;
	m_ipZipCodeArea->get_Fields(&ipFields);
	if (!ipFields)
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンフィールド対応表取得失敗\n"));
		return false;
	}
	if (sindy::errorcode::sindyErr_NoErr != m_zipCodeAreaFieldMap.CreateFieldMap(ipFields))
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンフィールド対応表取得失敗\n"));
		return false;
	}

	return true;
}

bool CheckPolygon::Check()
{
	RUNLOGGER.Info(_T("ポリゴンチェック実行…\n"));
	std::cout << "ポリゴンの整合チェック実行…" << std::endl;

	bool result = true;
	for (auto& mesh : m_meshes)
	{
		RUNLOGGER.Log(_T("メッシュ: %ld のチェック中\n"), mesh);
		std::cout << "メッシュ: " << mesh << " のチェック中" << std::endl;

		IGeometryPtr ipMeshGeometry = MESH2POLY(mesh, m_ipSpRef);
		result &= CheckPolygonDuplication(ipMeshGeometry);
	}

	RUNLOGGER.Info(_T("チェック終了\n"));
	std::cout << "チェック終了" << std::endl;
	return result;
}

bool CheckPolygon::CheckPolygonDuplication(const IGeometryPtr& meshGeometry)
{
	// [チェック] 隣接するポリゴンの郵便番号の組み合わせが同じだったらエラー
	// メッシュ内の郵便番号ポリゴンを取り、郵便番号1・2のペアをキーとするmapに情報を格納する
	auto& spatialFilter = AheInitSpatialFilter(nullptr, meshGeometry);
	CString subFields = uh::str_util::format(_T("%s, %s, %s, %s"),
		zipcode_area::kObjectID, zipcode_area::kZipCode1, zipcode_area::kZipCode2, zipcode_area::kShape);

	IFeatureCursorPtr ipCursor;
	auto query = AheInitQueryFilter(spatialFilter, subFields);
	if (S_OK != m_ipZipCodeArea->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		return false;
	}

	std::map<std::pair<CString, CString>, std::vector<std::pair<long, IGeometryPtr>>> mapZipCodeArea;
	long idxZipCode1 = m_zipCodeAreaFieldMap.FindField(zipcode_area::kZipCode1);
	long idxZipCode2 = m_zipCodeAreaFieldMap.FindField(zipcode_area::kZipCode2);
	IFeaturePtr ipFeature;
	while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature)
	{
		long oid = 0;
		ipFeature->get_OID(&oid);

		CComVariant varZipCode1, varZipCode2;
		ipFeature->get_Value(idxZipCode1, &varZipCode1);
		ipFeature->get_Value(idxZipCode2, &varZipCode2);
		CString strZipCode1(varZipCode1.bstrVal);
		CString strZipCode2(varZipCode2.bstrVal);
		IGeometryPtr ipGeometry;
		ipFeature->get_ShapeCopy(&ipGeometry);
		// TODO:
		// RecycringがTrueだからget_ShapeだとipGeometryのアドレス皆同じになってしまう。
		// それだと後の隣接判定がうまくいかないのでget_ShapeCopyにする。
		// RecycringはFalseでget_Shapeにしたほうが早いんだろうか…後で調べる。

		auto key = (strZipCode1 < strZipCode2) ?
			std::make_pair(strZipCode1, strZipCode2) : std::make_pair(strZipCode2, strZipCode1);

		mapZipCodeArea[key].emplace_back(oid, ipGeometry);
	}

	// エラーチェック
	bool hasError = false;
	for (const auto& data : mapZipCodeArea)
	{
		long dataCount = data.second.size();
		if (1 == dataCount)
			continue;

		// 同じ郵便番号の組み合わせのポリゴンが複数いる場合
		const auto& idGeometry = data.second;
		for (long i = 0; i < dataCount; ++i)
		{
			IRelationalOperatorPtr ipRelOpe(idGeometry[i].second);
			for (long j = i + 1; j < dataCount; ++j)
			{
				// まずDisjoint
				VARIANT_BOOL disjoint = VARIANT_FALSE;
				ipRelOpe->Disjoint(idGeometry[j].second, &disjoint);
				if (disjoint)
					continue;

				// 隣接チェック
				if (!IsAdjacent(idGeometry[i].second, idGeometry[j].second))
					continue;

				// 隣接しているならエラー出力
				hasError = true;

				// 郵便番号1つだけ持つ場合は、処理の都合上、1個めがNULLになっている
				// ログの見栄えがちょっと良くないのでNULLは出力しないようにする
				CString errInfo;
				if (data.first.first.IsEmpty())
					errInfo = uh::str_util::format(_T("%s\t"), data.first.second);
				else
					errInfo = uh::str_util::format(_T("%s\t%s"), data.first.first, data.first.second);

				ERRLOGGER.writeStdLog(
					zipcode_area::kTableName,
					idGeometry[i].first,
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::DoesNotMergePolygon,
					_T("同一郵便番号の隣接ポリゴンがマージされていない"),
					errInfo
				);
			}
		}
	}

	return !hasError;
}

/**
 * @brief ポリゴンが隣接(辺の一部を共有している)かどうかを確認する
 * MapCheckのSamePropertyChkと同じ
 * @param baseGeometry [in] ベースのポリゴン
 * @param compGeometry [in] 検査対象のポリゴン
 * @retval true 隣接している
 * @retval false 隣接していない
 */
bool IsAdjacent(const IGeometryPtr& baseGeometry, const IGeometryPtr& compGeometry)
{
	ITopologicalOperatorPtr ipBaseTopo, ipCompTopo;

	// ポリゴンの枠を取得
	IGeometryPtr ipBaseBoundary, ipCompBoundary;
	ipBaseTopo = baseGeometry;
	ipBaseTopo->get_Boundary(&ipBaseBoundary);

	ipCompTopo = compGeometry;
	ipCompTopo->get_Boundary(&ipCompBoundary);

	// 枠のIntersect(ポリゴン同士の接線のジオメトリ)を取得
	// NoDimension指定すると得られるジオメトリは元ジオメトリ次第(この場合Polyline)
	ipBaseTopo = ipBaseBoundary;
	IGeometryPtr ipResultGeometry(CLSID_GeometryBag);
	ISpatialReferencePtr ipSpRef;
	baseGeometry->get_SpatialReference(&ipSpRef);
	ipResultGeometry->putref_SpatialReference(ipSpRef);
	if (FAILED(ipBaseTopo->Intersect(ipCompBoundary, esriGeometryNoDimension, &ipResultGeometry)) ||
		!ipResultGeometry)
	{
		// ここに入ってきたら隣接ではないことにする
		return false;
	}

	VARIANT_BOOL vaEmpty = VARIANT_FALSE;
	ipResultGeometry->get_IsEmpty(&vaEmpty);
	return !(VB2bool(vaEmpty));
}