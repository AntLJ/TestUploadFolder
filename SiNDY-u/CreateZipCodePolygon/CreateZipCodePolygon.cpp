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

#include "CreateZipCodePolygon.h"
#include "ShapeFileManager.h"
#include "EditManager.h"

#include <AddrLib/AreacodeHelper.h>
#include <AddrLib/JusyoHelper.h>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/enumerate_elements.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;

namespace
{
	/**
	 * @brief フィールドを作成する
	 * @param name [in] フィールド名
	 * @param type [in] フィールドタイプ
	 * @param length [in] フィールドサイズ
	 * @retval IFieldEditPtr 処理に成功
	 * @retval nullptr 処理に失敗
	 */
	IFieldEditPtr CreateField(const CString& name, esriFieldType type, int length)
	{
		IFieldEditPtr field(CLSID_Field);

		if( FAILED(field->put_Name(CComBSTR(name))) )
			return nullptr;

		if( FAILED(field->put_Type(type)) )
			return nullptr;

		// 用途が限定的なのでprecisionとlengthを混ぜている
		// フィールド定義に変更があったら、ここの処理も見直したほうが良い
		switch(type)
		{
		case esriFieldTypeInteger:
			if( FAILED(field->put_Precision(length)) ) return nullptr;
			break;
		case esriFieldTypeString:
			if( FAILED(field->put_Length(length)) ) return nullptr;
			break;
		default:
			return nullptr;
			break;
		}
		return field;
	}

	/**
	 * @brief フィールドコレクションを作成する
	 * @param createdFields [in] フィールド情報
	 * @param fields [in,out] フィールドコレクション
	 * @retval true 処理に成功
	 * @retval false 処理に失敗
	 */
	bool CreateFields(
		const vector<CAdapt<IFieldEditPtr>>& createdFields,
		IFieldsEditPtr& fields)
	{
		for( auto& field : createdFields )
		{
			if( !field.m_T )
			{
				cerr << "フィールド作成に失敗" << endl;
				return false;
			}

			if( FAILED(fields->AddField(field.m_T)) )
			{
				cerr << "AddFieldに失敗" << endl;
				return false;
			}
		}
		return true;
	}

	/*
	 * @brief 郵便番号ポリゴン出力先フィールドの生成
	 * @param fields [in,out] フィールドコレクション
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool CreateZipCodePolygonField(IFieldsEditPtr& fields)
	{
		vector<CAdapt<IFieldEditPtr>> createdFields;
		createdFields.push_back(CreateField(shape_field::kObjectId, esriFieldTypeInteger, 9));
		createdFields.push_back(CreateField(shape_field::kZipCode, esriFieldTypeString, 7));
		// Shapeフィールドは空間参照を使う関係で別関数で追加する
		return CreateFields(createdFields, fields);
	}

	/*
	 * @brief 郵便番号テーブル出力先フィールドの生成
	 * @param fields [in,out] フィールドコレクション
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool CreateZipCodeTableField(IFieldsEditPtr& fields)
	{
		vector<CAdapt<IFieldEditPtr>> createdFields;
		createdFields.push_back(CreateField(table_field::kPolygonId, esriFieldTypeInteger, 9));
		createdFields.push_back(CreateField(table_field::kPrefCode, esriFieldTypeString, 2));
		createdFields.push_back(CreateField(table_field::kCityCode, esriFieldTypeString, 3));
		createdFields.push_back(CreateField(table_field::kOazaCode, esriFieldTypeString, 3));
		createdFields.push_back(CreateField(table_field::kAzaCode, esriFieldTypeString, 3));

		return CreateFields(createdFields, fields);
	}

	/*
	 * @brief 都市地図行政界フィーチャカーソル取得
	 * @param ipFeatureClass [in] フィーチャクラス
	 * @param ipFeatureCursor [out] フィーチャカーソル
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetFeatureCursor(
		const IFeatureClassPtr& ipFeatureClass,
		IFeatureCursorPtr& ipFeatureCursor)
	{
		CString subFields;
		subFields.Format(_T("%s,%s,%s,%s,%s"),
			city_admin::kObjectID, city_admin::kCityCode, city_admin::kAddrCode,
			city_admin::kAddrCode2, _T("SHAPE"));

		// ADDRCODEが999999(別荘地、自衛隊敷地など)は郵便番号を持たない(データ上特定できない)ので除外
		// エリア種別「海」は除外
		CString whereClause;
		whereClause.Format(_T("%s <> '%s' and %s <> %d"),
			city_admin::kAddrCode, _T("999999"),
			city_admin::kAreaClass, city_admin::area_class::kSea);

		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		ipQueryFilter->put_SubFields(CComBSTR(subFields));
		ipQueryFilter->put_WhereClause(CComBSTR(whereClause));

		ipFeatureClass->Search(ipQueryFilter, VARIANT_TRUE, &ipFeatureCursor);
		if( !ipFeatureCursor )
		{
			cerr << "都市地図行政界検索失敗" << endl;
			return false;
		}
		return true;
	}
} // namespace

// 初期化
bool CCreateZipCodePolygon::init()
{
	if( !m_logManager.OpenRunLog(m_args.m_runLog.c_str()) )
	{
		cerr << "実行ログのオープンに失敗：" << m_args.m_runLog << endl;
		return false;
	}

	if( !m_logManager.OpenErrLog(m_args.m_errLog.c_str()) )
	{
		cerr << "エラーログのオープンに失敗：" << m_args.m_errLog << endl;
		return false;
	}

	// 実行ログに実行情報書き出し
	CVersion version;
	CString versionInfo;
	versionInfo.Format(_T("%s FileVersion: %s"),
		version.GetOriginalFilename(), version.GetFileVersion());
	m_logManager.WriteRunLog(versionInfo);
	// 指定オプション
	m_logManager.WriteRunLog(m_args.GetAllOptionInfo());
	// 処理開始日時
	CString startTime = _T("Start :") + m_logManager.GetCurrentTimeString();
	m_logManager.WriteRunLog(startTime);

	// エラーログに都市地図行政界ポリゴン接続先出力
	m_logManager.WriteErrComment(_T("SINDYSTDLOG"));
	m_logManager.WriteErrComment(m_args.m_cityDb.c_str());

	return true;
}

// 実行
bool CCreateZipCodePolygon::execute()
{
	// 初期化
	if( !init() )
		return false;

	// 住所マスタ関係
	if( !JusyoInit() )
		return false;

	// 以降、出力モードごとに処理を分ける
	if (ExecuteMode::shp == m_args.m_executeMode)
		return executeShp();
	else if (ExecuteMode::sde == m_args.m_executeMode)
		return executeSde();
	else
		return false;
}

// Shp出力用
bool CCreateZipCodePolygon::executeShp()
{
	// CityAdmin関係
	IFeatureClassPtr ipCityAdminFc;
	if( !CityAdminInit(ipCityAdminFc) )
		return false;

	// Shp出力先関係
	IFeatureClassPtr ipZipCodePolygonFc;
	ITablePtr ipZipCodeTable;
	if( !ShapeInit(ipZipCodePolygonFc, ipZipCodeTable) )
		return false;

	cout << "処理リスト作成..." << endl;
	map<CString,vector<pair<CString,long>>> zipCodeBaseMap;
	IFeatureCursorPtr ipSearchCursor;
	if( !GetFeatureCursor(ipCityAdminFc, ipSearchCursor) )
	{
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipSearchCursor->NextFeature(&ipFeature) && ipFeature )
	{
		long objectId = 0;
		ipFeature->get_OID(&objectId);

		CComVariant cityCode, addrCode;
		ipFeature->get_Value(m_idxCityCode, &cityCode);
		ipFeature->get_Value(m_idxAddrCode, &addrCode);

		CString adminCityCode = cityCode;
		CString adminAddrCode = addrCode;

		CString zipCode;
		if( !GetZipCode(adminCityCode, adminAddrCode, objectId, zipCode) )
			continue;

		zipCodeBaseMap[zipCode].push_back(make_pair(adminCityCode+adminAddrCode, objectId));

		// ADDRCODE2が000000は通称住所の設定がないということなので処理飛ばす
		CComVariant tsushoAddrCode;
		ipFeature->get_Value(m_idxAddrCode2, &tsushoAddrCode);
		CString tsushoAdminAddrCode = tsushoAddrCode;
		if( 0 == tsushoAdminAddrCode.Compare(_T("000000")) )
			continue;

		CString tsushoZipCode;
		if( !GetZipCode(adminCityCode, tsushoAdminAddrCode, objectId, tsushoZipCode) )
			continue;

		zipCodeBaseMap[tsushoZipCode].push_back(make_pair(adminCityCode+tsushoAdminAddrCode, objectId));
	}

	long maxPolygonId = 0;
	cout << "データ出力..." << endl;
	for( const auto& zipCodeBase : zipCodeBaseMap )
	{
		// 同一郵便番号となるポリゴンをマージする
		IGeometryBagPtr ipMergeGeometryBag(CLSID_GeometryBag);
		ipMergeGeometryBag->putref_SpatialReference(m_cityAdminSpatialReference);
		IGeometryCollectionPtr ipMergeGeometryCollection(ipMergeGeometryBag);

		// 住所コードとオブジェクトIDをそれぞれ格納
		vector<CString> addrCodes;
		vector<long> targetOids;
		for( const auto& addrOid : zipCodeBase.second )
		{
			targetOids.push_back(addrOid.second);
			addrCodes.push_back(addrOid.first);
		}

		// OBJECTID IN (xx, xx, ...)でなるべく検索回数を少なくする
		// 念のため、1000件ずつ処理されるようにuseful_headersのenumerate_elementsを使う
		map<pair<CString,long>,IGeometryPtr> oidGeometryMap;
		uh::ElementsStrings oidStrings = uh::enumerate_elements(targetOids);
		for(auto& oidString : oidStrings)
		{
			CString whereClause;
			whereClause.Format(_T("%s IN (%s)"),
				city_admin::kObjectID,
				uh::toTStr(oidString).c_str());

			IQueryFilterPtr ipQuery(CLSID_QueryFilter);
			ipQuery->put_WhereClause(CComBSTR(whereClause));
			IFeatureCursorPtr ipCursor;
			ipCityAdminFc->Search(ipQuery, VARIANT_TRUE, &ipCursor);

			IFeaturePtr ipFeature;
			while( S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature )
			{
				IGeometryPtr ipGeometry;
				ipFeature->get_ShapeCopy(&ipGeometry);
				ipMergeGeometryCollection->AddGeometry(ipGeometry);

				// 処理対象11桁に対するジオメトリを保持
				// TODO: この辺は2度手間なので、もっと良い方法があればそちらを採用する
				long oid = 0;
				ipFeature->get_OID(&oid);
				CComVariant cityCode, addrCode;
				ipFeature->get_Value(m_idxCityCode, &cityCode);
				ipFeature->get_Value(m_idxAddrCode, &addrCode);
				CString elevenCode = CString(cityCode) + CString(addrCode);
				if( addrCodes.end() != find(addrCodes.begin(), addrCodes.end(), elevenCode) )
				{
					oidGeometryMap[make_pair(elevenCode,oid)] = ipGeometry;
				}

				CComVariant tsushoAddrCode;
				ipFeature->get_Value(m_idxAddrCode2, &tsushoAddrCode);
				CString tsushoElevenCode = CString(cityCode) + CString(tsushoAddrCode);
				if( addrCodes.end() != find(addrCodes.begin(), addrCodes.end(), tsushoElevenCode) )
				{
					oidGeometryMap[make_pair(tsushoElevenCode,oid)] = ipGeometry;
				}
			}
		}

		// ConstructUnionでマージ
		IPolygon4Ptr ipMergePolygon(CLSID_Polygon);
		ipMergePolygon->putref_SpatialReference(m_cityAdminSpatialReference);
		IEnumGeometryPtr ipEnumMergeGeometry(ipMergeGeometryBag);
		if(FAILED(((ITopologicalOperatorPtr)ipMergePolygon)->ConstructUnion(ipEnumMergeGeometry)))
		{
			m_logManager.WriteErrLog(
				city_admin::kTableName,
				zipCodeBase.second.begin()->second, // oidはzipCodeBaseの先頭のやつ
				Error,
				error_message::kFailMerge);
			break;
		}

		// マルチポリゴンの確認
		IGeometryBagPtr ipGeometryBag;
		ipMergePolygon->get_ConnectedComponentBag(&ipGeometryBag);
		IGeometryCollectionPtr ipGeometryCollection(ipGeometryBag);
		if( !ipGeometryCollection )
		{
			cerr << "マルチポリゴン分割に失敗(ジオメトリコレクションではない？)" << endl;
			continue;
		}

		// マルチポリゴンでなければここで出力
		long geometryCount = 0;
		ipGeometryCollection->get_GeometryCount(&geometryCount);
		if( 1 == geometryCount )
		{
			++maxPolygonId;
			IGeometryPtr ipMergeGeometry(ipMergePolygon);
			OutputFeatureClass(ipZipCodePolygonFc, maxPolygonId, ipMergeGeometry, zipCodeBase.first);
			sort(addrCodes.begin(), addrCodes.end());
			addrCodes.erase(unique(addrCodes.begin(), addrCodes.end()),addrCodes.end());
			OutputTable(ipZipCodeTable, maxPolygonId, addrCodes);
			continue;
		}

		// マルチポリゴンなら分割
		for(long index=0; index<geometryCount; ++index)
		{
			IGeometryPtr separatedGeometry;
			ipGeometryCollection->get_Geometry(index, &separatedGeometry);

			separatedGeometry->putref_SpatialReference(m_cityAdminSpatialReference);
			separatedGeometry->SnapToSpatialReference();

			vector<CString> separatedAddrCodes;
			for( auto& data : oidGeometryMap )
			{
				// 未処理の住所コードについてのみ処理(テーブルデータで重複が起きるため)
				const auto& findResult = 
					find(separatedAddrCodes.begin(), separatedAddrCodes.end(), data.first.first);

				if( findResult != separatedAddrCodes.end() )
					continue;

				// Disjointならcontinue
				IRelationalOperatorPtr ipRelOpe(separatedGeometry);
				VARIANT_BOOL isDisjoint = VARIANT_FALSE;
				ipRelOpe->Disjoint(data.second, &isDisjoint);
				if( isDisjoint )
					continue;

				// Contains(同一形状含む)でないならcontinue
				VARIANT_BOOL isContain = VARIANT_FALSE;
				ipRelOpe->Contains(data.second, &isContain);
				if( !isContain )
					continue;

				// ここまで来たら「処理済み」
				separatedAddrCodes.push_back(data.first.first);
			}
			// 出力
			++maxPolygonId;
			OutputFeatureClass(ipZipCodePolygonFc, maxPolygonId, separatedGeometry, zipCodeBase.first);

			sort(separatedAddrCodes.begin(), separatedAddrCodes.end(),
				[](const CString& lhs, const CString& rhs){return lhs.Compare(rhs) < 0;});
			OutputTable(ipZipCodeTable, maxPolygonId, separatedAddrCodes);
		}
	}
	return true;
}

bool CCreateZipCodePolygon::JusyoInit()
{
	cout << "住所DB接続..." << endl;
	addr::CJusyoHelper jusyoHelper;
	if( !jusyoHelper.Connect(
		m_args.m_jusyoUser.c_str(),
		m_args.m_jusyoUser.c_str(),
		m_args.m_jusyoDb.c_str()) )
	{
		cerr << "住所DB接続失敗" << endl;
		return false;
	}

	cout << "住所マスタ取得..." << endl;
	CString whereClause;
	whereClause.Format(
		_T("%s is not null and %s = '000000'"),
		areacode_master::kZipCode,
		areacode_master::kHaishiDate
		);

	CString subFields;
	subFields.Format(
		_T("%s, %s, %s, %s, %s"),
		areacode_master::kKenCode,
		areacode_master::kShiCode,
		areacode_master::kOazaCode,
		areacode_master::kAzaCode,
		areacode_master::kZipCode
		);

	addr::CJusyoRecordSet records;
	jusyoHelper.Select(areacode_master::kTableName, whereClause, subFields, records);

	records.begin();
	while( !records.end() )
	{
		CString zipcode = records.GetStringValue(areacode_master::kZipCode);
		CString kenCode = records.GetStringValue(areacode_master::kKenCode);
		CString shiCode = records.GetStringValue(areacode_master::kShiCode);
		CString oazaCode = records.GetStringValue(areacode_master::kOazaCode);
		CString azaCode = records.GetStringValue(areacode_master::kAzaCode);

		m_addrZipCodeMap[kenCode+shiCode+oazaCode+azaCode] = zipcode;
		records.next();
	}

	return true;
}

bool CCreateZipCodePolygon::CityAdminInit(IFeatureClassPtr& ipCityAdminFc)
{
	cout << "都市地図行政界DB接続..." << endl;
	CString workspaceName = CString(m_args.m_cityDb.c_str());
	IFeatureWorkspacePtr ipWorkspace(create_workspace(workspaceName));
	if( !ipWorkspace )
	{
		cerr << "都市地図行政界のワークスペース取得失敗： " <<
			uh::toStr(m_args.m_cityDb) << endl;
		return false;
	}

	ipWorkspace->OpenFeatureClass(CComBSTR(city_admin::kTableName), &ipCityAdminFc);
	if( !ipCityAdminFc )
	{
		cerr << "都市地図行政界の取得失敗" << endl;
		return false;
	}

	ipCityAdminFc->FindField(CComBSTR(city_admin::kCityCode), &m_idxCityCode);
	ipCityAdminFc->FindField(CComBSTR(city_admin::kAddrCode), &m_idxAddrCode);
	ipCityAdminFc->FindField(CComBSTR(city_admin::kAddrCode2), &m_idxAddrCode2);
	if( m_idxCityCode == -1 || m_idxAddrCode == -1 || m_idxAddrCode2 == -1 )
	{
		cerr << "都市地図行政界フィールドインデックス取得失敗" << endl;
		return false;
	}

	if( FAILED(((IGeoDatasetPtr)ipCityAdminFc)->get_SpatialReference(&m_cityAdminSpatialReference)) )
	{
		cerr << "都市地図行政界空間参照取得失敗" << endl;
		return false;
	}
	return true;
}

bool CCreateZipCodePolygon::ShapeInit(IFeatureClassPtr& ipZipCodePolygonFc, ITablePtr& ipZipCodeTable)
{
	cout << "出力先作成..." << endl;
	ShapeFileManager shpManager;
	if( !shpManager.OpenWorkspace(m_args.m_outputDir.c_str()) )
	{
		cerr << "データ出力先のオープンに失敗" << endl;
		return false;
	}
	IFieldsEditPtr ipPolygonFields(CLSID_Fields);
	if( !CreateZipCodePolygonField(ipPolygonFields) ||
		!shpManager.CreateShapeField(ipPolygonFields, m_cityAdminSpatialReference) )
	{
		cerr << "ポリゴンファイルのフィールド設定に失敗" << endl;
		return false;
	}
	ipZipCodePolygonFc = 
		shpManager.CreateFeatureClass(m_args.m_shapeName.c_str(), ipPolygonFields);
	if( !ipZipCodePolygonFc )
	{
		cerr << "ポリゴンファイルの作成に失敗" << endl;
		return false;
	}
	IFieldsEditPtr ipTableFields(CLSID_Fields);
	if( !CreateZipCodeTableField(ipTableFields) )
	{
		cerr << "テーブルファイルのフィールド設定に失敗" << endl;
		return false;
	}
	ipZipCodeTable = shpManager.CreateTable(m_args.m_tableName.c_str(), ipTableFields);
	if( !ipZipCodeTable )
	{
		cerr << "テーブルファイルの作成に失敗" << endl;
		return false;
	}
	return true;
}

bool CCreateZipCodePolygon::GetZipCode(
	const CString& cityCode, const CString& addrCode, long oid, CString& zipCode)
{
	CString addrElevenCode = cityCode + addrCode;
	zipCode = TransAddrcode2ZipCode(addrElevenCode);
	if( zipCode.IsEmpty() )
	{
		m_logManager.WriteErrLog(
			city_admin::kTableName,
			oid,
			Error,
			error_message::kNonExistingCode,
			addrElevenCode);
		return false;
	}
	return true;
}

CString CCreateZipCodePolygon::TransAddrcode2ZipCode(const CString& elevenCode )
{
	auto findResult = m_addrZipCodeMap.find(elevenCode);
	return (m_addrZipCodeMap.end() == findResult) ? CString() : findResult->second;
}

void CCreateZipCodePolygon::OutputFeatureClass(IFeatureClassPtr& zipCodePolygon,
	long polygonId, const IGeometryPtr& ipGeometry, const CString& zipCode)
{
	// 出力先
	IFieldsPtr newFields;
	zipCodePolygon->get_Fields(&newFields);

	long idxPolygonId = -1, idxZipCode = -1;
	newFields->FindField(CComBSTR(shape_field::kObjectId), &idxPolygonId);
	newFields->FindField(CComBSTR(shape_field::kZipCode), &idxZipCode);

	IFeatureCursorPtr fetureCursor;
	if( FAILED(zipCodePolygon->Insert(VARIANT_TRUE, &fetureCursor)) )
	{
		cerr << "インサートカーソル取得失敗" << endl;
		return;
	}

	IFeatureBufferPtr featureBuffer;
	zipCodePolygon->CreateFeatureBuffer(&featureBuffer);
	if( !featureBuffer )
	{
		cerr << "CreateFeatureBuffer失敗" << endl;
		return;
	}

	if( FAILED(featureBuffer->putref_Shape(ipGeometry)) )
	{
		cerr << "putref_Shape失敗 PolygonID : " << polygonId << endl;
		return;
	}

	if( FAILED(featureBuffer->put_Value(idxPolygonId, CComVariant(polygonId))) ||
		FAILED(featureBuffer->put_Value(idxZipCode, CComVariant(zipCode))))
	{
		cerr << "put_Value失敗 PolygonID : " << 	polygonId << endl;
		return;
	}

	CComVariant tempIndex;
	if( FAILED(fetureCursor->InsertFeature(featureBuffer, &tempIndex)) )
	{
		cerr << "InsertFeature失敗 PolygonID : " << polygonId << endl;
		return;
	}

	if( FAILED(fetureCursor->Flush()) )
	{
		cerr << "Flush失敗" << endl;
	}
}

void CCreateZipCodePolygon::OutputFeatureClass(
	IFeatureBufferPtr& featureBuffer, IFeatureCursorPtr& featureCursor,
	const sindy::CFieldMap& fieldMap,
	const IGeometryPtr& ipGeometry, const pair<CString, CString>& zipCodePair)
{
	if (FAILED(featureBuffer->putref_Shape(ipGeometry)))
	{
		cerr << "putref_Shape失敗" << endl;
		return;
	}

	// 郵便番号セット
	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kZipCode1),
		CComVariant(zipCodePair.first))))
	{
		cerr << "put_Value失敗" << endl;
		return;
	}

	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kZipCode2),
		CComVariant(zipCodePair.second))))
	{
		cerr << "put_Value失敗" << endl;
		return;
	}

	// 共通属性セット
	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kOperator),
		CComVariant(m_commonAttr.GetLastUserName()))))
	{
		cerr << "put_Value失敗" << endl;
		return;
	}
	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kModifyProgName),
		CComVariant(m_commonAttr.GetToolName()))))
	{
		cerr << "put_Value失敗" << endl;
		return;
	}
	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kProgModifyDate),
		CComVariant(m_commonAttr.GetModifyDate()))))
	{
		cerr << "put_Value失敗" << endl;
		return;
	}

	CComVariant tempIndex;
	if (FAILED(featureCursor->InsertFeature(featureBuffer, &tempIndex)))
	{
		cerr << "InsertFeature失敗" << endl;
		return;
	}
}

void CCreateZipCodePolygon::OutputTable(
	ITablePtr& zipCodeTable, long polygonId, const vector<CString>& addrCodes)
{
	// 出力先
	IFieldsPtr newFields;
	zipCodeTable->get_Fields( &newFields );

	long idxPolygonId = -1, idxPrefCode = -1, idxCityCode = -1,
		idxOazaCode = -1, idxAzaCode = -1;
	newFields->FindField(CComBSTR(table_field::kPolygonId), &idxPolygonId);
	newFields->FindField(CComBSTR(table_field::kPrefCode), &idxPrefCode);
	newFields->FindField(CComBSTR(table_field::kCityCode), &idxCityCode);
	newFields->FindField(CComBSTR(table_field::kOazaCode), &idxOazaCode);
	newFields->FindField(CComBSTR(table_field::kAzaCode), &idxAzaCode);

	_ICursorPtr cursor;
	if( FAILED(zipCodeTable->Insert(VARIANT_TRUE, &cursor)) )
	{
		cerr << "インサートカーソル取得失敗" << endl;
		return;
	}

	IRowBufferPtr rowBuffer;
	zipCodeTable->CreateRowBuffer(&rowBuffer);
	if( !rowBuffer )
	{
		cerr << "CreateRowBuffer失敗" << endl;
		return;
	}

	for( const auto& addrCode : addrCodes )
	{
		if( FAILED(rowBuffer->put_Value(idxPolygonId, CComVariant(polygonId))) ||
			FAILED(rowBuffer->put_Value(idxPrefCode, CComVariant(addrCode.Left(2)))) ||
			FAILED(rowBuffer->put_Value(idxCityCode, CComVariant(addrCode.Mid(2,3)))) ||
			FAILED(rowBuffer->put_Value(idxOazaCode, CComVariant(addrCode.Mid(5,3)))) ||
			FAILED(rowBuffer->put_Value(idxAzaCode, CComVariant(addrCode.Right(3)))) )
		{
			cerr << "put_Value失敗 PolygonID : " << polygonId << endl;
			continue;
		}

		CComVariant vaTemp;
		if( FAILED(cursor->InsertRow(rowBuffer, &vaTemp)) )
		{
			cerr << "InsertRow失敗 PolygonID : " << polygonId << endl;
			continue;
		}
	}

	if( FAILED(cursor->Flush()) )
	{
		cerr << "Flush失敗" << endl;
	}
}

// Sde出力用
bool CCreateZipCodePolygon::executeSde()
{
	// CityAdmin
	IFeatureClassPtr ipCityAdminFc;
	if (!CityAdminInit(ipCityAdminFc))
		return false;

	// SDE出力先
	CWorkspace zipWorkspace(m_args.m_outputDb.c_str());
	IFeatureClassPtr ipZipCodePolygonFc(zipWorkspace.OpenTable(zipcode_area::kTableName));
	if (!ipZipCodePolygonFc)
	{
		cerr << "郵便番号ポリゴンフィーチャクラスの取得失敗" << endl;
		return false;
	}

	ISpatialReferencePtr ipZipSpatialReference;
	if (FAILED(((IGeoDatasetPtr)ipZipCodePolygonFc)->get_SpatialReference(&ipZipSpatialReference)))
	{
		cerr << "郵便番号ポリゴン空間参照取得失敗" << endl;
		return false;
	}

	// フィールドマップ作成
	IFieldsPtr ipFields;
	ipZipCodePolygonFc->get_Fields(&ipFields);
	if (!ipFields)
	{
		cerr << "郵便番号ポリゴンフィールド情報取得失敗" << endl;
		return false;
	}
	sindy::CFieldMap zipPolygonFieldMap;
	zipPolygonFieldMap.CreateFieldMap(ipFields);

	cout << "処理リスト作成..." << endl;
	map<pair<CString, CString>, vector<long>> zipCodeBaseMap;
	IFeatureCursorPtr ipSearchCursor;
	if (!GetFeatureCursor(ipCityAdminFc, ipSearchCursor))
	{
		return false;
	}

	IFeaturePtr ipFeature;
	while (S_OK == ipSearchCursor->NextFeature(&ipFeature) && ipFeature)
	{
		long objectId = 0;
		ipFeature->get_OID(&objectId);

		CComVariant cityCode, addrCode;
		ipFeature->get_Value(m_idxCityCode, &cityCode);
		ipFeature->get_Value(m_idxAddrCode, &addrCode);

		CString adminCityCode = cityCode;
		CString adminAddrCode = addrCode;

		CString zipCode;
		if (!GetZipCode(adminCityCode, adminAddrCode, objectId, zipCode))
		{
			cerr << "郵便番号取得失敗 ADDRCODE: " <<
				uh::toStr(adminCityCode.GetString()) <<
				uh::toStr(adminAddrCode.GetString()) << endl;
			continue;
		}

		CComVariant tsushoAddrCode;
		ipFeature->get_Value(m_idxAddrCode2, &tsushoAddrCode);
		CString tsushoAdminAddrCode = tsushoAddrCode;

		CString tsushoZipCode;
		if (0 != tsushoAdminAddrCode.Compare(_T("000000")))
		{
			// 通称住所コードが000000なら通称郵便番号はNULL
			// 000000でないなら郵便番号取得
			if (!GetZipCode(adminCityCode, tsushoAdminAddrCode, objectId, tsushoZipCode))
			{
				cerr << "郵便番号取得失敗 ADDRCODE: " <<
					uh::toStr(adminCityCode.GetString()) <<
					uh::toStr(tsushoAdminAddrCode.GetString()) << endl;
				continue;
			}
		}

		// 両方一緒なら通称の方をNULLにする
		if (0 == tsushoZipCode.Compare(zipCode))
			tsushoZipCode = _T("");

		zipCodeBaseMap[make_pair(zipCode, tsushoZipCode)].push_back(objectId);
	}

	cout << "データ出力..." << endl;

	EditManager editManager(zipWorkspace);
	if (!editManager.StartEdit(false))
	{
		cerr << "編集開始失敗" << endl;
		editManager.AbortEdit();
		return false;
	}

	IFeatureCursorPtr fetureCursor;
	HRESULT hr = ipZipCodePolygonFc->Insert(VARIANT_TRUE, &fetureCursor);
	if (FAILED(hr))
	{
		cerr << "インサートカーソル取得失敗" << endl;
		editManager.AbortEdit();
		return false;
	}

	IFeatureBufferPtr featureBuffer;
	ipZipCodePolygonFc->CreateFeatureBuffer(&featureBuffer);
	if (!featureBuffer)
	{
		cerr << "CreateFeatureBuffer失敗" << endl;
		editManager.AbortEdit();
		return false;
	}

	for (const auto& zipCodeBase : zipCodeBaseMap)
	{
		// 郵便番号1、2の組み合わせが同一のポリゴンをマージする
		IGeometryBagPtr ipMergeGeometryBag(CLSID_GeometryBag);
		ipMergeGeometryBag->putref_SpatialReference(ipZipSpatialReference);
		IGeometryCollectionPtr ipMergeGeometryCollection(ipMergeGeometryBag);

		// OBJECTID IN (xx, xx, ...)でなるべく検索回数を少なくする
		// 念のため、1000件ずつ処理されるようにuseful_headersのenumerate_elementsを使う
		map<pair<CString, long>, IGeometryPtr> oidGeometryMap;
		uh::ElementsStrings oidStrings = uh::enumerate_elements(zipCodeBase.second);
		for (auto& oidString : oidStrings)
		{
			CString whereClause;
			whereClause.Format(_T("%s IN (%s)"),
				city_admin::kObjectID,
				uh::toTStr(oidString).c_str());

			IQueryFilterPtr ipQuery(CLSID_QueryFilter);
			ipQuery->put_WhereClause(CComBSTR(whereClause));
			IFeatureCursorPtr ipCursor;
			ipCityAdminFc->Search(ipQuery, VARIANT_TRUE, &ipCursor);

			IFeaturePtr ipFeature;
			while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature)
			{
				IGeometryPtr ipGeometry;
				ipFeature->get_ShapeCopy(&ipGeometry);
				ipMergeGeometryCollection->AddGeometry(ipGeometry);
			}
		}

		// ConstructUnionでマージ
		IPolygon4Ptr ipMergePolygon(CLSID_Polygon);
		ipMergePolygon->putref_SpatialReference(ipZipSpatialReference);
		IEnumGeometryPtr ipEnumMergeGeometry(ipMergeGeometryBag);
		ITopologicalOperatorPtr ipTopo(ipMergePolygon);
		if (FAILED((ipTopo->ConstructUnion(ipEnumMergeGeometry))))
		{
			m_logManager.WriteErrLog(
				city_admin::kTableName,
				zipCodeBase.second.front(), // oidはzipCodeBaseの先頭のやつ
				Error,
				error_message::kFailMerge);
			break;
		}

		// マルチポリゴンの確認
		IGeometryBagPtr ipGeometryBag;
		ipMergePolygon->get_ConnectedComponentBag(&ipGeometryBag);
		IGeometryCollectionPtr ipGeometryCollection(ipGeometryBag);
		if (!ipGeometryCollection)
		{
			cerr << "マルチポリゴン分割に失敗(ジオメトリコレクションではない？)" << endl;
			continue;
		}

		// マルチポリゴンでなければここで出力
		long geometryCount = 0;
		ipGeometryCollection->get_GeometryCount(&geometryCount);
		if (1 == geometryCount)
		{
			OutputFeatureClass(featureBuffer, fetureCursor, zipPolygonFieldMap,
				ipMergePolygon, zipCodeBase.first);
			continue;
		}

		// マルチポリゴンなら分割
		for (long index = 0; index < geometryCount; ++index)
		{
			IGeometryPtr separatedGeometry;
			ipGeometryCollection->get_Geometry(index, &separatedGeometry);
			separatedGeometry->putref_SpatialReference(ipZipSpatialReference);
			separatedGeometry->SnapToSpatialReference();

			// 出力
			OutputFeatureClass(featureBuffer, fetureCursor, zipPolygonFieldMap,
				separatedGeometry, zipCodeBase.first);
		}

		if (FAILED(fetureCursor->Flush()))
		{
			cerr << "Flush失敗" << endl;
			editManager.AbortEdit();
			return false;
		}
	}
	editManager.StopEdit(true);

	return true;
}
