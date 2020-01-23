#include "stdafx.h"
#include "MakeSHP.h"
#include "common.h"
#include "LogSys.h"

using namespace shp05::schema::road::height_node;

const long EXTNUM=5;														// Shapeファイル拡張子数
const CString EXT[EXTNUM] = {_T("shp"), _T("shx"), _T("dbf"), _T("prj"), _T("cpg")};	// Shapeファイル拡張子
const long SHPHEADERSIZE = 100;												// shpヘッダ領域
const long SHPRECSIZE = 8 + 4 + 8 + 8;										// shapeレコードサイズ レコードヘッダ部[8] + シェープタイプ[4] + 座標XY[8+8]
const long DBFDEFAULTSIZE = 32 + 32*7 + 1 + 1;								// 予約ヘッダ部(32バイト)とフィールド宣言分(フィールド数×32バイト)とフィールドの終わりを示す符号(1バイト) + 末尾につく符号(1バイト)も足す。
const long DBFRECSIZE = 7*9 + 1;											// フィールド数(7) × フィールド長(9) + 削除判定用の1byte
const unsigned long MAX_FILE_SIZE = LONG_MAX;								// ファイル分割サイズ LONG_MAX:2147483647 ※約2GB。2,147,483,648[byte](2*1024*1024*1024) 

MakeSHP::MakeSHP(void)
	:m_fileCount(0), m_objectidIndex(0), m_linkid1Index(0), m_divid1Index(0), m_relHeight1Index(0),
	m_linkid2Index(0), m_divid2Index(0), m_relHeight2Index(0), m_shpTotalSize(0), m_dbfTotalSize(0)
{ 
}


MakeSHP::~MakeSHP(void)
{
}

bool MakeSHP::Init(LPCTSTR outputDir)
{
	// 既存ファイルの削除
	CString fileName = ::kLayerName;
	
	CString fullFileName;
	for (int i = 0; i < EXTNUM; ++i) {
		// 分割ファイルが存在する場合があるためワイルドカードを使用して検索
		fullFileName.Format(_T("%s\\%s*.%s"), outputDir, fileName, EXT[i]);
		HANDLE hFind;
		WIN32_FIND_DATA fd;
		hFind = FindFirstFile(CT2CW(fullFileName), &fd); // 最初のファイル検索
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				CString deleteFile;
				deleteFile.Format(_T("%s\\%s"), outputDir, fd.cFileName);
				if (DeleteFile(deleteFile) == 0) {
					LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ファイル削除に失敗しました"), fullFileName);
					return false;
				}
			} while(FindNextFile(hFind, &fd)); // 次のファイル検索
		}
	}
	// ワークスペースの取得
	if (! common_proc::GetWorkspace(outputDir, m_ipWorkspace))
		return false;
	// 空Shapeの作成
	CString shpName;
	shpName.Format(_T("%s_%d"), ::kLayerName, m_fileCount);
	++m_fileCount;
	if (! MakeVacuityShp(shpName))
		return false;
	// インデックス取得
	if (! SetFieldIndex())
		return false;
	return true;
}

bool MakeSHP::InsertRecord(const CnvData& cnvData, const long startID)
{
	IFeatureCursorPtr ipFeatureCursor;
	if (FAILED(m_ipFeatureClass->Insert(VARIANT_TRUE, &ipFeatureCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィーチャカーソルの挿入に失敗しました"), ::kLayerName);
		return false;
	}
	// 変換したデータのinsert
	std::map<long, OutSHPRec>::const_iterator outRecItr = (0 == startID) ? cnvData.begin() : cnvData.find(startID);
	for (; outRecItr != cnvData.end(); ++outRecItr) {
		IFeatureBufferPtr ipFeatureBuffer;
		if (FAILED(m_ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィーチャバッファの作成に失敗しました"), ::kLayerName, outRecItr->first);
			return false;
		}
		// 属性の設定
		if (! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, shp05::schema::kObjectId, m_objectidIndex, CComVariant(outRecItr->first)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kLinkId1, m_linkid1Index, CComVariant(outRecItr->second.e_linkid1)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kDivId1, m_divid1Index, CComVariant(outRecItr->second.e_divid1)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kRelHeight1, m_relHeight1Index, CComVariant(outRecItr->second.e_height1)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kLinkId2, m_linkid2Index, CComVariant(outRecItr->second.e_linkid2)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kDivId2, m_divid2Index, CComVariant(outRecItr->second.e_divid2)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kRelHeight2, m_relHeight2Index, CComVariant(outRecItr->second.e_height2)))
			return false;
		// 形状の設定
		if (FAILED(ipFeatureBuffer->putref_Shape(outRecItr->second.e_ipGeoemtry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状付与に失敗しました"), ::kLayerName);
			return false;
		}
		// フィーチャ登録
		CComVariant id;
		if (FAILED(ipFeatureCursor->InsertFeature(ipFeatureBuffer, &id))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィーチャ追加に失敗しました"), ::kLayerName);
			return false;
		}
		m_shpTotalSize += SHPRECSIZE;
		m_dbfTotalSize += DBFRECSIZE;
		// 次のレコードを登録するとshp or dbfが2GBを越える場合、フラッシュしてShapeを作り直す。
		if (m_shpTotalSize > (MAX_FILE_SIZE-SHPRECSIZE) || m_dbfTotalSize > (MAX_FILE_SIZE-DBFRECSIZE)) {
			// ここまでの登録
			if (FAILED(ipFeatureCursor->Flush())) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィーチャのフラッシュに失敗しました"), ::kLayerName);
				return false;
			}
			// 空Shape作成
			CString shpName;
			shpName.Format(_T("%s_%d"), ::kLayerName, m_fileCount);
			++m_fileCount;
			if (! MakeVacuityShp(shpName))
				return false;
			// 次のレコードから新しいShapeへ登録をする。
			long nextID = (++outRecItr)->first;
			if (! InsertRecord(cnvData, nextID))
				return false;
			return true;
		}
	}
	// フラッシュ
	if (FAILED(ipFeatureCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィーチャのフラッシュに失敗しました"), ::kLayerName);
		return false;
	}

	return true;
}

bool MakeSHP::ReName(LPCTSTR outputDir)
{
	if (1 != m_fileCount)
		return true;

	CString beforeBaseName, afterBaseName;
	beforeBaseName.Format(_T("%s\\%s_0"), outputDir, ::kLayerName);
	afterBaseName.Format(_T("%s\\%s"), outputDir, ::kLayerName);

	// Shape識別子分繰返し
	for (int i = 0; i < EXTNUM; ++i) {
		CString beforeName, afterName;
		beforeName.Format(_T("%s.%s"), beforeBaseName, EXT[i]);
		afterName.Format(_T("%s.%s"), afterBaseName, EXT[i]);
		if(PathFileExists(beforeName)) {
			if (rename(CT2CA(beforeName), CT2CA(afterName)) != 0) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ファイル名変更に失敗しました"),  beforeName);
				return false;
			}
		}
	}
	return true;
}

bool MakeSHP::MakeVacuityShp(LPCTSTR shpName)
{
	CString errMsg;
	// 属性フィールド作成
	IFieldsEditPtr ipFields(CLSID_Fields);
	if (! MakeAttrField(shp05::schema::kObjectId, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kLinkId1, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kDivId1, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kRelHeight1, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kLinkId2, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kDivId2, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kRelHeight2, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields))
		return false;
	// 形状フィールド作成
	if (! MakeShapeField(esriGeometryPoint, ipFields))
		return false;
	// フィーチャクラス作成
	if (FAILED(((IFeatureWorkspacePtr)m_ipWorkspace)->CreateFeatureClass(CComBSTR(shpName), ipFields, 0, 0, esriFTSimple, CComBSTR(shp05::schema::kShape), CComBSTR(_T("")), &m_ipFeatureClass))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("空Shape作成に失敗しました"), shpName);
		return false;
	}
	// Shapeサイズのリセット
	m_shpTotalSize = SHPHEADERSIZE;
	m_dbfTotalSize = DBFDEFAULTSIZE;
	return true;
}

bool MakeSHP::MakeAttrField(LPCTSTR fieldName,
							const esriFieldType fieldType,
							const VARIANT_BOOL editable,
							const VARIANT_BOOL nullable,
							const long fieldLength,
							const long precision,
							IFieldsEditPtr& ipFields)
{
	CString errMsg;
	IFieldEditPtr ipFieldEdit(CLSID_Field);
	if (FAILED(ipFieldEdit->put_Name(CComBSTR(fieldName))) ||
		FAILED(ipFieldEdit->put_Type(fieldType)) ||
		FAILED(ipFieldEdit->put_Editable(editable)) ||
		FAILED(ipFieldEdit->put_IsNullable(nullable))){
			errMsg.Format(_T("フィールド作成に失敗しました:%s"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::kLayerName);
			return false;
	}
	if (0 != fieldLength) {
		if (FAILED(ipFieldEdit->put_Length(fieldLength))) {
			errMsg.Format(_T("フィールド長の設定に失敗しました：%s"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::kLayerName);
			return false;
		}
	}
	if (0 != precision)
		ipFieldEdit->PutPrecision(precision);
	if (FAILED(ipFields->AddField((IFieldPtr)ipFieldEdit))) {
		errMsg.Format(_T("フィールド追加に失敗しました:%s"), fieldName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::kLayerName);
		return false;
	}
	return true;
}

bool MakeSHP::MakeShapeField(const esriGeometryType geoType, IFieldsEditPtr& ipFields)
{

	// GeometryDefの作成
	IGeometryDefEditPtr ipGeoDef(CLSID_GeometryDef);
	IUnknownCoordinateSystemPtr ipUnkCoordSys(CLSID_UnknownCoordinateSystem);
	if (FAILED(ipGeoDef->putref_SpatialReference(ipUnkCoordSys)) ||
		FAILED(ipGeoDef->put_GeometryType(geoType))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状定義設定に失敗しました"), ::kLayerName);
		return false;
	}

	// 形状フィールドの作成	
	IFieldEditPtr ipFieldEdit(CLSID_Field);
	if (FAILED(ipFieldEdit->put_Name(CComBSTR(shp05::schema::kShape))) ||
		FAILED(ipFieldEdit->put_Type(esriFieldTypeGeometry)) ||
		FAILED(ipFieldEdit->put_IsNullable(VARIANT_FALSE)) ||
		FAILED(ipFieldEdit->putref_GeometryDef(ipGeoDef))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールド作成に失敗しました"), ::kLayerName);
			return false;
	}
	if (FAILED(ipFields->AddField((IFieldPtr)ipFieldEdit))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールド追加に失敗しました"), ::kLayerName);
		return false;
	}
	return true;
}

bool MakeSHP::SetFieldIndex()
{
	// 各種インデックス取得
	IFieldsPtr ipFields;
	if (FAILED(m_ipFeatureClass->get_Fields(&ipFields))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィールド取得に失敗しました"), ::kLayerName);
		return false;
	}
	if (FAILED(ipFields->FindField(CComBSTR(shp05::schema::kObjectId), &m_objectidIndex)) ||
		FAILED(ipFields->FindField(CComBSTR(::kLinkId1), &m_linkid1Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kDivId1), &m_divid1Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kRelHeight1), &m_relHeight1Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kLinkId2), &m_linkid2Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kDivId2), &m_divid2Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kRelHeight2), &m_relHeight2Index)))
		return false;
	return true;
}
