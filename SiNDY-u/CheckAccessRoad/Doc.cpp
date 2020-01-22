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

// Doc.cpp: CDoc クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckAccessRoad.h"
#include "Doc.h"
#include <fstream>
#include <algorithm>
#include <sindy/workspace.h>
#include <map.h>
#include <sindyk.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CDoc::CDoc():m_strLogFile(_T("accessroad_err.txt")),
	m_mkFileDir(_T(".")),
	m_CheckDist(100),
	m_DebugMode(false),
	m_FacilListWrite(false)
{
	m_Fp = NULL;
	m_ipWorkspace = NULL;
	m_ipLinkFeatureClass = NULL;
	m_ipNodeFeatureClass = NULL;
	m_ipAdmFeatureClass = NULL;
	m_ipBaseSiteFeatureClass = NULL;
	m_ipBaseLineFeatureClass = NULL;
	m_ipLQ_Route = NULL;
	m_ipINF_Route = NULL;
}
CDoc::~CDoc()
{
	deleteMap();
}
/**
* 施設情報ポイントコンテナ解放
* @note 施設情報ポイントコンテナを解放します。
* @param void
* @return	void
*/
void CDoc::deleteMap(void)
{
	m_FacilPoints.clear();
}
/**
* 施設情報ポイントファイルより、Shapefileに変換
* @note 施設情報ポイントファイルより、ShapeFileに変換します。
* @param 施設情報出力ファイル名
* @return	true：変換成功、false：変換失敗
*/
bool CDoc::convertFacilPoint(LPCTSTR sShapeFile)
{
	//施設情報ポイントのShape変換
	if( makeFacilPoint(sShapeFile))
	{
		//施設情報ポイント検査エリアのShape変換
		if( makeCheckArea(sShapeFile))
		{
			return true;
		}
		else
		{
			std::cerr << "施設情報ポイント検査エリアのShape変換失敗" << std::endl;
			return false;
		}
	} 
	else 
		return false;
}

// 施設情報を取得する一連の流れを実行
bool CDoc::getFacilPoint(void)
{
	deleteMap();

	std::vector<long> aContentsVec;

	if( !readContentsList(aContentsVec) ){
		std::cerr << "コンテンツリストの読み込みに失敗" << std::endl;
		return false;
	}
	HRESULT hr;
	IWorkspacePtr ipPOIWorkspace = sindy::_create_workspace(m_strPOIDB, hr );
	if( FAILED(hr) ){
		std::cerr << "POIDBのワークスペースの取得に失敗" << std::endl;
		return false;
	}
	IFeatureWorkspacePtr ipFeatureWorkspace(ipPOIWorkspace);

	IFeatureClassPtr ipPOIPointFeatureClass, ipPOISubPointFeatureClass;
	if( !OpenFeatureClass(ipFeatureWorkspace, sindy::schema::sindyk::poi_point::kTableName, &ipPOIPointFeatureClass) 
		|| !OpenFeatureClass(ipFeatureWorkspace, sindy::schema::sindyk::poi_sub_point::kTableName, &ipPOISubPointFeatureClass) ){
			std::cerr << "POIDBのフィーチャクラスの取得に失敗しました" << std::endl;
			return false;
	}

	using namespace sindy::schema::sindyk;

	BOOST_FOREACH( const long& rContentsID, aContentsVec){
		cout << "\r施設ポイントの取得中...";
		// POI_POINTを取ってくる
		CString aWhere1;
		aWhere1.Format(_T("%s = %d AND %s = %d"), poi_point::kContentsCode, rContentsID, poi_point::kDelete, poi_point::delete_code::kAlive );
		std::vector< CAdapt<IFeaturePtr> > aPOIPointFeatureVec;
		SelectByWhereClause(ipPOIPointFeatureClass, aWhere1, aPOIPointFeatureVec);

		// 取ってきたPOI_POINTを巡回
		BOOST_FOREACH( const CAdapt<IFeaturePtr>& rAFeature1, aPOIPointFeatureVec){
			const IFeaturePtr& rFeature1 = rAFeature1.m_T;

			// とりあえず施設ポイントを設定
			CFacilPoint aFacilPoint;
			WKSPoint aWKSPoint1 = {0,0};
			if( !GetFeaturePoint(rFeature1, aWKSPoint1) ){
				continue;
			}
			std::pair<int, int> aLL256Pair1 = getLL256(aWKSPoint1);
			aFacilPoint.setLon(aLL256Pair1.first);
			aFacilPoint.setLat(aLL256Pair1.second);
			aFacilPoint.setKanji(GetValue( rFeature1, poi_point::kName).bstrVal);

			CString aContentSeq=CString(GetValue( rFeature1, poi_point::kContentsSeq ).bstrVal);
			CString aCode; aCode.Format(_T("%d-%d-%s"), long(rContentsID/100000), rContentsID%100000, aContentSeq);
			aFacilPoint.setCode(aCode);

			// POI_POINTのIDに対応し、削除フラグが「0:生存」のSUBPOINTを取ってくる
			long aPOIPointID = -1;	rFeature1->get_OID(&aPOIPointID);

			CString aWhere2;
			aWhere2.Format(_T("%s = %d AND %s = %d"), poi_sub_point::kPOIPointID, aPOIPointID, poi_sub_point::kDelete, poi_sub_point::delete_code::kAlive );
			std::vector< CAdapt<IFeaturePtr> > aPOISubPointFeatureVec;
			long aCount = SelectByWhereClause(ipPOISubPointFeatureClass, aWhere2, aPOISubPointFeatureVec);

			// SUBPOINTがなければ、そのまま登録
			if( aCount == 0 ){
				aFacilPoint.setFacilPointXY();
				m_FacilPoints.push_back(aFacilPoint);
				cout << "\r施設ポイントの取得中...\t" << m_FacilPoints.size();
			}
			else{
				BOOST_FOREACH( const CAdapt<IFeaturePtr>& rAFeature2, aPOISubPointFeatureVec){
					const IFeaturePtr& rFeature2 = rAFeature2.m_T;

					WKSPoint aWKSPoint2 = {0,0};
					if( !GetFeaturePoint(rFeature2, aWKSPoint2) ){
						continue;
					}
					CFacilPoint aSubFacilPoint(aFacilPoint);
					std::pair<int, int> aLL256Pair2 = getLL256(aWKSPoint2);
					aSubFacilPoint.setLon(aLL256Pair2.first);
					aSubFacilPoint.setLat(aLL256Pair2.second);
					aSubFacilPoint.setFacilPointXY();
					m_FacilPoints.push_back(aSubFacilPoint);
					cout << "\r施設ポイントの取得中...\t" << m_FacilPoints.size();
				}
			}
		}
	}
	cout << "\r施設ポイントの取得終了\t" << m_FacilPoints.size() << endl;

	if( m_FacilListWrite ){
		cout << "\r施設リストの書き込み中";
		if( !writeFacilList() ){
			return false;
		}
		cout << "\r施設リストの書き込み終了\t" << m_FacilPoints.size() << std::endl;
	}

	return true;
}

// 施設情報のXML(Excelのスプレッドシート形式)を出力する
bool CDoc::writeFacilList()
{
	CString strOutFileName;
	strOutFileName.Format("%s\\access_list.xml", m_mkFileDir);

	ofstream aOutStream(strOutFileName);
	if( !aOutStream ){
		cout << (LPCSTR)CStringA(strOutFileName) << "の書き込みに失敗" << endl;
		return false;
	}
	aOutStream << "<?xml version=\"1.0\" encoding=\"Shift_JIS\"?>" << endl;
	aOutStream << "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\">" << endl;
	aOutStream << "<Worksheet ss:Name=\"access_list\">" << endl;
	aOutStream << "<Table>" << endl;

	aOutStream << "<Column ss:Width=\"60\"/>" << endl;	// 緯度の幅は60(実際には80に置き換えられる)
	aOutStream << "<Column ss:Width=\"60\"/>" << endl;	// 経度の幅は60(実際には80に置き換えられる)
	aOutStream << "<Column ss:Width=\"360\"/>" << endl;	// 施設名称の幅は360(実際には480に置き換えられる)
	aOutStream << "<Column ss:Width=\"240\"/>" << endl;	// シーケンス番号の幅は240(実際には320に置き換えられる)
	aOutStream << "<Column ss:Width=\"60\"/>" << endl;	// 備考欄の幅は60(実際には80に置き換えられる)

	CString aHeaderColumn[5] = { _T("経度"), _T("緯度"), _T("施設名称"), _T("シーケンス番号"), _T("備考") };
	writeExcelRow(aHeaderColumn, 5, &aOutStream);

	BOOST_FOREACH( const CFacilPoint& rFacilPoint, m_FacilPoints){
		CString aColumn[5];
		aColumn[0].Format("%d", rFacilPoint.getLon());
		aColumn[1].Format("%d", rFacilPoint.getLat());
		aColumn[2] = rFacilPoint.getKanji();
		aColumn[3] = rFacilPoint.getCode();
		writeExcelRow( aColumn, 5, &aOutStream );
	}

	aOutStream << "</Table>" << endl;
	aOutStream << "</Worksheet>" << endl;
	aOutStream << "</Workbook>" << endl;

	aOutStream.close();

	return true;
}

// Excelの行を書く
void CDoc::writeExcelRow(const CString* rStrAry, const int& rColNum, ofstream* pOfstream) const
{
	*pOfstream << "\t<Row>";
	for(int i=0; i<rColNum; ++i){
		writeExcelCell(rStrAry[i], pOfstream);
	}
	*pOfstream << "</Row>" << endl;
}

// Excelのセルの中身を書く
void CDoc::writeExcelCell(LPCTSTR strValue, ofstream* pOfstream) const
{
	*pOfstream << "<Cell><Data ss:Type=\"String\">" << (LPCSTR)CStringA(strValue) << "</Data></Cell>";
}

// コンテンツリストを読み込む
bool CDoc::readContentsList(std::vector<long>& rContentsVec)
{
	ifstream aStream;
	aStream.open(m_strContentsList);
	if( !aStream )
	{
		CString	aStr; aStr.Format(" Open Error [%s]",m_strContentsList);
		AfxMessageBox(aStr,MB_ICONHAND);
		return false;
	}

	std::string aLine;
	while( getline( aStream, aLine ) )
	{
		std::string::size_type aPos = aLine.find_first_of("-");
		if( std::string::npos != aPos )
		{
			std::string aZen = aLine.substr(0, aPos);
			std::string aKou = aLine.substr(aPos+1);
			long aZenNum = atol(aZen.c_str());
			long aKouNum = atol(aKou.c_str());
			rContentsVec.push_back(aZenNum*100000+aKouNum);
		}
	}
	std::sort(rContentsVec.begin(), rContentsVec.end());
	std::unique(rContentsVec.begin(), rContentsVec.end());

	aStream.close();

	return true;
}

/**
* 施設情報コンテナより、Shapefileに変換
* @note 施設情報コンテナより、ShapeFileに変換します。
* @param 施設情報出力ファイル名
* @return	true：変換成功、false：変換失敗
*/
bool CDoc::makeFacilPoint(LPCTSTR sFname)
{
	int	aFID[10];
	char *aFStr[] = {"KANJI","SEQCODE","MESHCODE","XPOS","YPOS","TYPE","LON","LAT"};
	// SHPデータの新規作成
	if( !m_SHPHdl.create(sFname, SHPT_POINT)){
		cerr << sFname << " :ShpFile Create ERROR." << endl;
		return false ;
	}
	// DBFデータの新規作成
	if( !m_DBFHdl.create(sFname)){
		cerr << sFname << " :DbfFile Create ERROR." << endl;
		return false ;
	}
	// DBFのフィールドの作成
	for( int i=0,j=0; i<8; i++ ){
		if( i < 2 ){
			if((aFID[i] = m_DBFHdl.addField(aFStr[i],FTString,128,0)) < 0){
				cerr << aFStr[i] << " :Field Create ERROR." << endl;
				return false ;
			}
		}
		else{
			if((aFID[i] = m_DBFHdl.addField(aFStr[i],FTInteger,8,0)) < 0){
				cerr << aFStr[i] << " :Field Create ERROR." << endl;
				return false ;
			}
		}
	}
	//施設情報ポイントの変換
	int i=0;
	for( FacilArray::iterator it = m_FacilPoints.begin(); it!=m_FacilPoints.end(); it ++, i++ ){
		double		aX = it->getFacilDX();
		double		aY = it->getFacilDY();
		if( i%10==0 )
			cout << "\r施設ポイントのShape変換中...\t" << i+1;

		// shape形状の登録
		CSHPPoint	aPoint(aX,aY);
		CSHPObj		aArcObj(SHPT_POINT, aPoint);
		int aShapeID = m_SHPHdl.writeObject(aArcObj, -1);
		if(aShapeID < 0){
			cerr << it->getMeshcode() << "\t[" << it->getKanji() << "]";
			cerr << "\tオブジェクトの書き込み失敗:"<< std::endl;
			continue;
		}
		int j = 0;
		// 施設名称（漢字）
		if( !m_DBFHdl.writeStringAttribute(aShapeID, aFID[j], it->getKanji().GetBuffer(256))){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// シーケンスコード
		if( !m_DBFHdl.writeStringAttribute(aShapeID, aFID[j], it->getCode().GetBuffer(256))){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// メッシュコード
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getMeshcode())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// 正規化Ｘ座標
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getX())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// 正規化Ｙ座標
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getY())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// 施設種別コード
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getType())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// 経度
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getLon())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// 緯度
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getLat())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
	}
	cout << "\r施設ポイントのShape変換終了\t\t" << m_FacilPoints.size() << endl;
	return true;
}
/**
* 施設情報ポイント検査エリアのShape変換
* @note 施設情報コンテナより、検索対象矩形ShapeFileに変換します。
* @param 施設情報検査エリア出力ファイル名
* @return	true：変換成功、false：変換失敗
*/
bool CDoc::makeCheckArea(LPCTSTR sFname)
{
	if( !m_FacilPoints.size())	//データなし
		return false;
	int		aFID[10];
	char   *aFStr[] = {"KANJI","SEQCODE","MESHCODE","TYPE"};
	char	aFname[256];
	sprintf(aFname,"%s_Area",sFname);
	if( !m_SHPHdl.create(aFname, SHPT_POLYGON)){
		cerr << sFname << " :ShpFile Create ERROR." << endl;
		return false ;
	}
	// DBFデータの新規作成
	if( !m_DBFHdl.create(aFname)){
		cerr << sFname << " :DbfFile Create ERROR." << endl;
		return false ;
	}
	// DBFのフィールドの作成
	for( int i=0; i<4; i++ ){
		if( i < 2 ){
			if((aFID[i] = m_DBFHdl.addField(aFStr[i],FTString,128,0)) < 0){
				cerr << aFStr[i] << " :Field Create ERROR." << endl;
				return false ;
			}
		}
		else{
			if((aFID[i] = m_DBFHdl.addField(aFStr[i],FTInteger,8,0)) < 0){
				cerr << aFStr[i] << " :Field Create ERROR." << endl;
				return false ;
			}
		}
	}
	//施設情報ポイントの変換
	int i=0;
	for( FacilArray::iterator it = m_FacilPoints.begin(); it!=m_FacilPoints.end(); it++, i++ ){
		double		aDX = it->getFacilDX();
		double		aDY = it->getFacilDY();
		int			aX  = it->getX();
		int			aY  = it->getY();
		int			aBaseMesh = it->getMeshcode();

		//施設ポイントを中心に矩形を作成する距離
		// ×１．２　指定距離より余分にバッファを持たせるため
		// ×１００　2次メッシュコードを100万×100万の正規化XY座標で表した場合に
		//			 X、Yとも一辺の長さを10kmと仮定すると、1座標あたり0.01mとなるため、100倍する
		int			aDist = m_CheckDist*1.2*100;		//施設ポイントを中心エリア検索（ｍ）に変換

		//検査エリア左下座標
		CMeshPos	aSPos(aBaseMesh,aX,aY);
		aSPos.SetLeftDownPos(aBaseMesh,aX,aY,aDist);
		//検査エリア右上座標
		CMeshPos	aEPos(aBaseMesh,aX,aY);
		aEPos.SetRightTopPos(aBaseMesh,aX,aY,aDist);
		if( !aSPos.GetMeshcode() || !aEPos.GetMeshcode()){
			continue;
		}
		// ポイント配列に登録
		CSHPPointArray aPointArray; // 形状クラス
		aPointArray.append(aSPos.getLon(), aSPos.getLat());	//(1)最南西		   2○－－－－－○3
		aPointArray.append(aSPos.getLon(), aEPos.getLat());	//(2)最北西         | アクセス道|
		aPointArray.append(aEPos.getLon(), aEPos.getLat());	//(3)最北東         | チェックの|
		aPointArray.append(aEPos.getLon(), aSPos.getLat());	//(4)最南東         |    範囲   |
		aPointArray.append(aSPos.getLon(), aSPos.getLat());	//(5)最南西      1,5○－－－－－○4

		// shapeオブジェクトの作成
		CSHPObj aPolyObj(SHPT_POLYGON, aPointArray);

		// (このとき、インデックスを-1にすると最後に追加)
		int aShapeID = m_SHPHdl.writeObject(aPolyObj, -1);
		if( aShapeID < 0 ){ // 0未満はエラー
			cerr << "Shapeオブジェクトの追加に失敗:" << endl;
			continue;
		}
		//チェック範囲を登録
		it->setCheckArea(aSPos.getLon(), aSPos.getLat(), aEPos.getLon(), aEPos.getLat());

		int j = 0;
		// 施設名称（漢字）
		if( !m_DBFHdl.writeStringAttribute(aShapeID, aFID[j], it->getKanji().GetBuffer(256))){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// シーケンスコード
		if( !m_DBFHdl.writeStringAttribute(aShapeID, aFID[j], it->getCode().GetBuffer(256))){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// メッシュコード
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getMeshcode())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// 施設種別コード
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getType())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
	}
	return true;
}
/**
* アクセス道路チェック結果ファイルオープン
* @note アクセス道路チェック結果ファイルをオープンします。
* @param なし
* @return	なし
*/
void CDoc::CheckStart(void)
{
	//エラーログファイルの指定
	char	sAccessLogFile[256];
	sprintf(sAccessLogFile,"%s\\%s",getLogDirStr(),getLogFileStr());
	if( NULL!=(m_Fp=fopen(sAccessLogFile,"w"))){
		CheckStop();
		if( NULL==(m_Fp=fopen(sAccessLogFile,"a"))){
			cerr << "ERROR " << sAccessLogFile <<" Open.";
		}
	}
}
/**
* アクセス道路チェック結果ファイルクローズ
* @note アクセス道路チェック結果ファイルをクローズします。
* @param なし
* @return	なし
*/
void CDoc::CheckStop(void)
{
	if( m_Fp ){
		(void)fclose(m_Fp);
		m_Fp = NULL;
	}
}
/**
* 施設情報ポイントファイルより、Shapefileに変換
* @note 施設情報ポイントファイルより、ShapeFileに変換します。
* @return	0：正常終了、1:変換失敗、3：変換したShapeにエラーあり
*/
int CDoc::checkFacilPoint(void)
{
	int iRET=0;
	//施設情報ポイントとアクセス道路の判定開始
	CheckStart();
	CString	aStr;
	int i=0;
	for( FacilArray::iterator it = m_FacilPoints.begin(); it!=m_FacilPoints.end(); it++, i++ ){
		//チェック進行状況
		cout << "\rcheck→ " << i+1 << "/" << m_FacilPoints.size();
		cout << "  " << it->getMeshcode();
		cout << "  " << it->getKanji().GetBuffer(256) << flush;

		//チェック距離をセット(指定距離の10倍にして、指定距離以上の場合でも距離計測が出来るようにしておく)
		it->setDistance(m_CheckDist*10);

		//チェック用矩形ポリゴン作成
		CNwArea	aNwArea = it->getCheckArea();
		IEnvelopePtr ipNwAreaEnv( CLSID_Envelope );
		ipNwAreaEnv->PutCoords(aNwArea.getSx(), aNwArea.getSy(), aNwArea.getEx(), aNwArea.getEy());

		//-------------------------------------------------------------------------------------
		//この施設に対する、チェックエリア内の最近傍リンクを取得
		//-------------------------------------------------------------------------------------
		IPointPtr ipFacilPoint(CLSID_Point);
		ipFacilPoint->PutCoords(it->getFacilDX(), it->getFacilDY());

		//施設ポイントの登録
		it->setFacilPoint(ipFacilPoint);

		using namespace sindy::schema;

		IFeatureCursorPtr	ipLinkFeatureCursor;
		esriSpatialRelEnum aSpatialRel = esriSpatialRelIntersects;					//内包＋クロス
		long lCount = SelectByShape(ipNwAreaEnv, m_ipLinkFeatureClass, &ipLinkFeatureCursor, aSpatialRel);
		if( lCount > 0 ){
			IFeaturePtr		ipLinkFeature;
			while(( S_OK==ipLinkFeatureCursor->NextFeature( &ipLinkFeature ))&&(ipLinkFeature != NULL)){
				if( !isNaviLink(VIEW_ARC_FERRY,ipLinkFeature))
					continue;
				IGeometryPtr	ipLinkGeom;
				if( SUCCEEDED( ipLinkFeature->get_ShapeCopy( &ipLinkGeom ))){
					CComVariant vaVal = GetValue(ipLinkFeature,_T("OBJECTID"));
					CComVariant vaSID = GetValue(ipLinkFeature,road_link::kFromNodeID);
					CComVariant vaEID = GetValue(ipLinkFeature,road_link::kToNodeID);					
					if( vaVal.vt==VT_NULL )
						continue;					
					//無視する道路種別かどうかをチェックする
					if( isIgnoreLink(ipLinkFeature)){
						continue;
					}
					//アクセスポイントと最近傍リンクの距離を計算
					IPointPtr ipNearestPoint;
					double	aNearDistance = GetNearestDist(ipLinkGeom, ipFacilPoint, ipNearestPoint);
					//もし距離がおなじ場合は、追跡対象リンク以外は除外する
					if(aNearDistance == it->getDistance() ){
						//基本道路条件に一致するかどうか
						if(! isNaviLink(VIEW_ARC_NAVI, ipLinkFeature)){
							//基本道路ではない場合、RouteLqrfをチェック
							if(! isLinkQueRoute(ipLinkFeature)){
								continue;
							}
						}
					}
					//距離を判別
					if( aNearDistance <= it->getDistance()){
						//最近傍のリンク距離を保持する(指定距離以上でも最近傍リンクを保持する)
						it->setDistance(aNearDistance);
						//指定距離未満の場合は登録する
						if(aNearDistance <= m_CheckDist){
							it->getNearLink().setOID(vaVal.intVal);
							it->getNearLink().setSNID(vaSID.intVal);
							it->getNearLink().setENID(vaEID.intVal);
							it->getNearLink().setFeature(ipLinkFeature);
							it->getNearLink().setGeometry(ipLinkGeom);
							it->setNearestPoint(ipNearestPoint);
						}
					}
				}
			}
			//目視チェック用ライン(CheckLine)　→後で水系チェックにも使用する
			if(! CreateCheckLineShape(&m_FacilPoints[i])){
				std::cerr << "目視チェック用ラインの作成に失敗しました" << std::endl;
				return 1;
			}
			//-------------------------------------------------------------------------------------
			//最近傍リンクが、基本道路（幅員5.5ｍ以上の道路）に到達できるか判定
			//-------------------------------------------------------------------------------------
			IFeaturePtr	 ipCheckFeature = it->getNearLink().getFeature();
			IGeometryPtr ipCheckGeom    = it->getNearLink().getGeometry();
			LinkArray aTraceLinkArray;
			aTraceLinkArray.insert(it->getNearLink().getOID());
			if( ipCheckFeature!=NULL ){
				it->setAccessSts(CheckAccess(ipCheckFeature, ipCheckGeom, aTraceLinkArray));
			}
		}
		//最近傍リンク追跡結果を受けて、追加のチェックを実行
		switch(it->getAccessSts()){
		case 0:		//最近傍リンクが見つからなかった場合
			break;
		case 1:
		case 2:
			//追跡成功した場合は水系チェック
			if(isCrossSuikei(&m_FacilPoints[i])){
				//水系エラーに変更
				it->setAccessSts(5);
			}
			break;
		default:
			//最近傍リンクに接続しているリンクをチェック対象にする
			IGeometryPtr ipNerestGeom = it->getNearLink().getGeometry();
			//一方通行を考慮する
			int aOneway = GetValue(it->getNearLink().getFeature(), road_link::kOneway).intVal;
			//条件なし一方通行の場合は、終点 or 始点のみ追跡する
			if(isOnewayLink(aOneway)){
				//リンク→ポイント列
				IPointCollectionPtr ipPointCol(it->getNearLink().getGeometry());
				IPointPtr ipTmpPoint;
				if(aOneway==1){
					//正方向の場合はリンクの始点
					ipPointCol->get_Point(0, &ipTmpPoint);
				}
				else{
					//逆方向の場合はリンクの終点
					long aPointCount=0;
					ipPointCol->get_PointCount(&aPointCount);
					ipPointCol->get_Point(aPointCount - 1, &ipTmpPoint);
				}
				ipNerestGeom = ipTmpPoint;
			}
			//接続リンクをチェックする
			if(CheckConnectLink(it->getFacilPoint(), ipNerestGeom, it->getNearLink().getOID())){
				//最近傍に接続しているリンクが、正しいアクセス道路の場合は【WARNING】
				it->setAccessSts(10);
			}
			else{
				//Bug:1569 対応	2003/5/19	離島はチェック対象としない を引き継いでチェック場所変更
				//追跡に成功しなかった場合は離島フラグチェック
				CComVariant vaVal = GetValue(it->getNearLink().getFeature(),road_link::kIsland);
				if( vaVal.intVal==1 ){
					it->setAccessSts(7);
				}
			}
			break;
		}
		//-------------------------------------------------------------------------------------
		//アクセス道路チェック結果の出力
		//-------------------------------------------------------------------------------------
		if (! showAccessRoad(i+1,&m_FacilPoints[i]))
			iRET = 3;
	}
	cout << "                                                                                   \r";
	cout << "\rアクセス道路チェック終了\t\t" << m_FacilPoints.size() << endl;
	deleteMap();
	CheckStop();
	return iRET;
}

/**
* @brief	到達可能チェック
* @note	該当リンクが経路対象道路に到達できるかチェック
* @param	piFeature	[in]	到達可能性をチェックしたいフィーチャ
* @param	piGeometry	[in]	到達可能性をチェックしたいフィーチャのジオメトリ
* @param	rLink		[out]	追跡済み確認用
* @return	int	アクセスコード
*/
int CDoc::CheckAccess(const IFeaturePtr piFeature, const IGeometryPtr piGeometry, LinkArray& rLink)
{
	//最近傍リンクのリンクが条件なしの通行禁止の場合はエラー
	if(isNopassLink(piFeature)){
		//通行禁止エラー
		return 9;
	}
	//最近傍リンクは、基本道路＋道路幅員5.5m以上か判定
	else if(isNaviLink(VIEW_ARC_NAVI, piFeature)){
		return IsTraceSuccessLink(piFeature, piGeometry, rLink);
	}
	//最近傍リンクがアクセス道路リンクの場合
	else if(isLinkQueRoute(piFeature)){
		int aStat=0;
		switch( LinkTrace(piFeature,piGeometry, rLink)){
		case 1:				//【SUCCESS】基本道路に接続
			aStat = 1; 
			break;
		case 2:				//【SUCCESS】ルートリンク列経由で基本道路に接続
			aStat = 2; 
			break;
		case 8:				// 要目視確認判定追加 ADD 2006/9/30
			aStat = 8;
			break;
		default:		    //【ERROR】施設まで到達できない
			aStat = 3;
			break;
		}
		return aStat;
	}
	else{
		//最近傍リンクがどこにも追跡できないケース
		return 3; //【ERROR】施設まで到達できない
	}
}

//施設ポイントと最近傍ポイントを直線で結びSHAPE化する
bool CDoc::CreateCheckLineShape(CFacilPoint *cFacil)
{
	//OID=0は対象としない
	if(cFacil->getNearLink().getOID()){
		//施設ポイントと最近傍リンクのポイントを直線で結ぶ
		//施設ポイント
		IPointPtr ipPoint1 = cFacil->getFacilPoint();
		//最近傍ポイント
		IPointPtr ipPoint2 = cFacil->getNearestPoint();
		//直線の作成
		IPolylinePtr	ipCheckLine( CLSID_Polyline );
		ipCheckLine->SetEmpty();
		((IPointCollectionPtr)ipCheckLine)->AddPoint( (IPointPtr)ipPoint1 );
		((IPointCollectionPtr)ipCheckLine)->AddPoint( (IPointPtr)ipPoint2 );
		//登録する
		cFacil->setCheckLine(ipCheckLine);

		double dX1=.0, dY1=.0, dX2=.0, dY2=.0;
		ipPoint1->get_X(&dX1);	ipPoint1->get_Y(&dY1);
		ipPoint2->get_X(&dX2);	ipPoint2->get_Y(&dY2);

		//水系交差確認用、ラインShape作成
		static	bool bStart = true;
		static  int aHdl;
		if( bStart ){
			char	sShapeFileStr[256];
			sprintf(sShapeFileStr,"%s\\%s",getLogDirStr(),"CheckLine");
			if( !m_SHPHdl.create(sShapeFileStr, SHPT_ARC)){
				cerr << " :ShpFile Create ERROR." << endl;	return false ;
			}
			if( !m_DBFHdl.create(sShapeFileStr)){
				cerr << " :DbfFile Create ERROR." << endl;	return false ;
			}
			if((aHdl = m_DBFHdl.addField("HitTestID",FTInteger,8,0)) < 0){
				cerr << " :Field Create ERROR." << endl;	return false ;
			}
		}
		bStart = false;
		CSHPPointArray aPointArray;	aPointArray.append(dX1,dY1);
		aPointArray.append(dX2,dY2);
		CSHPObj aArcObj(SHPT_ARC, aPointArray);
		int aShapeID = m_SHPHdl.writeObject(aArcObj, -1);
		if(aShapeID < 0){
			cerr << "\tオブジェクトの書き込み失敗:"<< std::endl;
			return false;
		}
		m_DBFHdl.writeIntegerAttribute(aShapeID,aHdl,cFacil->getMeshcode());
	}
	return true;
}

/**
* @brief	接続リンクのチェック
* @note	該当リンクに接続しているリンクが、到達可能リンクかをチェック
* @param	piFacilPoint		[in]	チェックしたい施設ポイント
* @param	piNearestGeometry	[in]	接触判定するジオメトリ
* @param	cTracedLinkOID		[out]	最近傍リンクのOID
* @return	true	接続リンクが上位道路に接続
false	接続リンクも上位道路に接続しない
*/
bool CDoc::CheckConnectLink(const IPointPtr piFacilPoint, const IGeometryPtr piNearestGeometry, long cTracedLinkOID)
{
	//接続リンクの取得
	IFeatureCursorPtr ipConnectFeatureCursor;
	esriSpatialRelEnum	aSpatialRel = esriSpatialRelTouches;	//接触
	long lCount = SelectByShape(piNearestGeometry, m_ipLinkFeatureClass, &ipConnectFeatureCursor, aSpatialRel);
	if( lCount > 0 ){
		IFeaturePtr		ipConnectFeature;
		while(S_OK==ipConnectFeatureCursor->NextFeature(&ipConnectFeature)){
			//無視する道路種別かどうかをチェック
			if(isIgnoreLink(ipConnectFeature)){
				continue;
			}
			//ジオメトリの取得
			IGeometryPtr	ipConnectGeometry;
			ipConnectFeature->get_ShapeCopy(&ipConnectGeometry);
			IPointPtr ipNearestPoint;
			double aDist = GetNearestDist(ipConnectGeometry, piFacilPoint, ipNearestPoint);
			//指定距離より近いかを判定
			if(aDist <= m_CheckDist){
				//近い場合は追跡を行う
				long aOID=0;
				ipConnectFeature->get_OID(&aOID);
				//追跡済みのリンクIDと一致した場合は、除外する
				//一通を考慮した場合に、最近傍リンクが再度検索されることがあるので
				if(cTracedLinkOID == aOID){
					continue;
				}
				//追跡済みリンクとして登録
				LinkArray aTraceLinkArray;
				aTraceLinkArray.insert(aOID);
				bool aIsSuccess=false;
				switch(CheckAccess(ipConnectFeature, ipConnectGeometry, aTraceLinkArray))
				{
					//1 or 2の場合は、追跡OK
				case 1:	//【SUCCESS】基本道路に接続
				case 2:	//【SUCCESS】ルートリンク列経由で基本道路に接続
					aIsSuccess = true;	
					break;
					//上記以外は追跡NG
				default:
					break;
				}
				//成功した場合は、trueを返す　//経路対象に到達できない場合は、次の接続リンクをチェック
				if(aIsSuccess){
					return true;
				}
			}
		}
	}
	//接続リンク全てが経路対象道路に到達できない
	return false;
}
/**
* アクセス道路のリンク追跡
* @note 指定アクセス道路が、基本道路に接続するまでリンク追跡を行います。
* @param IFeaturePtr	(I)	リンクフィーチャへのポインタ
* @param IGeometryPtr	(I)	リンクジオメトリへのポインタ
* @param LinkArray		(I)	施設ポイント周辺の道路リンクコンテナ
* @return	true:基本道路まで追跡成功、false：基本道路まで追跡失敗
*/
int	CDoc::LinkTrace(const IFeaturePtr piFeature, const IGeometryPtr piGeometry, LinkArray& rLink)
{
	//-------------------------------------------------------------------------------------
	//リンク追跡したリンクが、基本道路に到達できるか判定
	//-------------------------------------------------------------------------------------
	int		isOver = 3;	//デフォルトは【ERROR】施設に到達できない をセット

	using namespace sindy::schema;

	esriSpatialRelEnum	aSpatialRel = esriSpatialRelTouches;	//接触
	IFeatureCursorPtr	ipChkFeatureCursor;
	CComVariant vaVa = GetValue(piFeature,_T("OBJECTID"));

	//一方通行を考慮するため、一方通行コードを取得しておく
	int aPriorLinkOneway = GetValue(piFeature, road_link::kOneway).intVal;

	long lCount = SelectByShape(piGeometry, m_ipLinkFeatureClass, &ipChkFeatureCursor, aSpatialRel);
	if( lCount > 0 ){
		IFeaturePtr		ipNextFeature;
		IGeometryPtr	ipNextGeometry;
		while(( S_OK==ipChkFeatureCursor->NextFeature( &ipNextFeature ))&&(ipNextFeature!=NULL)){
			//Add 2006/9/30 SHIMANO
			//追跡範囲が広域サイズになったら追跡終了
			if( rLink.size() > 100 ){ //事象木繰り返し回数を監視
				isOver = 8;
				break;
			}
			//無視する道路種別かどうかをチェックする
			if( isIgnoreLink(ipNextFeature)){
				continue;
			}
			//追跡の場合、条件なしの通行禁止リンクは追跡対象外
			if( isNopassLink(ipNextFeature)){
				continue;
			}
			//条件なし一方通行を考慮して追跡できるか?
			if( isOnewayLink(aPriorLinkOneway)){
				//対象施設に到達できるかどうかなので、一方通行に逆走しているリンクであればOK
				//対象施設からは、一方通行で帰れなくてもOKというチェック仕様
				long aCommonNode=0;	//共通して持っているべきノードID
				if(aPriorLinkOneway==1){
					//正方向の場合はFromNodeを共通のノードとしていなければいけない
					aCommonNode = GetValue(piFeature, road_link::kFromNodeID).lVal;
				}
				else{
					//逆方向の場合はToNodeを共通のノードとしていなければいけない
					aCommonNode = GetValue(piFeature, road_link::kToNodeID).lVal;
				}
				long aFromNode = GetValue(ipNextFeature, road_link::kFromNodeID).lVal;
				long aToNode = GetValue(ipNextFeature, road_link::kToNodeID).lVal;
				//FromToどちらとも一致しなければ追跡対象外
				if( aFromNode != aCommonNode && aToNode != aCommonNode){
					continue;
				}
			}
			//このリンクは追跡済？
			if( !isTraceOverLink(ipNextFeature,rLink)){
				if( SUCCEEDED( ipNextFeature->get_ShapeCopy( &ipNextGeometry ))){
					CComVariant vaVal = GetValue(ipNextFeature,_T("OBJECTID"));
					//最近傍リンクは基本道路＋道路幅員5.5m以上か判定
					if( isNaviLink(VIEW_ARC_NAVI,ipNextFeature)){
						//追跡に成功したかどうか。一方通行を考慮して追跡できない場合もある
						isOver = IsTraceSuccessLink(ipNextFeature, ipNextGeometry, rLink);
						//成功した場合は、これ以上追跡しない
						if(isOver == 1 || isOver == 2){
							break;
						}
					}
					//接続している道路を全て判定
					else if( isLinkQueRoute(ipNextFeature)){
						isOver = LinkTrace(ipNextFeature, ipNextGeometry, rLink);
						if( isOver == 1 || isOver == 2 ){
							//【SUCCESS】アクセス道路経由で基本道路に接続
							isOver = 2;
							break;
						}
					}
				}
			}
		}
	}
	return( isOver );
}
/**
* アクセス道路検索：このリンクは追跡済みか判定
* @note 指定アクセス道路追跡で、このリンクは追跡済みか判定します
* @param IFeaturePtr	(I)	リンクフィーチャへのポインタ
* @param LinkArray		(I)	施設ポイント周辺の道路リンクコンテナ
* @return	true:追跡済み道路である、false：追跡済み道路ではない
*/
bool CDoc::isTraceOverLink(IFeaturePtr piFeature,LinkArray& rLink)
{
	long aBaseLinkID =  GetValue(piFeature,_T("OBJECTID")).lVal;
	if( rLink.find(aBaseLinkID) != rLink.end()){
		return true;
	}

	rLink.insert(aBaseLinkID);

	return false;
}
/**
* 施設とアクセス道路の間に水系があるか判定
* @note 施設とアクセス道路の間に水系があるか判定を行います。
* @param CFacilPoint	(I)	施設アクセス道路情報
* @return	true:水系あり、false：水系なし
*/
bool CDoc::isCrossSuikei(CFacilPoint *cFacil)
{
	using namespace sindy::schema;

	bool aSts=false;
	if( cFacil!=NULL ){
		//チェック用ライン(施設ポイントと最近傍リンクの最短点を直線で結んだライン)
		IPolylinePtr ipCheckLine = cFacil->getCheckLine();

		//水系のヒットテスト(直線にヒットするかどうか)
		IFeatureCursorPtr	ipCrossFeatureCursor;
		esriSpatialRelEnum	aSpatialRel = esriSpatialRelCrosses;		//クロス
		//水系との交差判定
		long lCount = SelectByShape(ipCheckLine, m_ipBaseSiteFeatureClass, &ipCrossFeatureCursor, aSpatialRel);
		if( lCount > 0 ){
			IFeaturePtr		ipChkFeature;
			while(( S_OK==ipCrossFeatureCursor->NextFeature( &ipChkFeature ))&&(ipChkFeature != NULL)){
				IGeometryPtr	ipChkGeom;
				if( SUCCEEDED( ipChkFeature->get_ShapeCopy( &ipChkGeom ))){
					CComVariant vaVal = GetValue(ipChkFeature,base_site::kSC4BgClass);
					switch( vaVal.intVal ){
					case base_site::bg_class::kSea		:	//海
					case base_site::bg_class::kRiver	:	//河川・沢・谷
					case base_site::bg_class::kLake		:	//湖・池・沼
					case base_site::bg_class::kMarsh	:	//湿地
						aSts = true;	//水系と交差している		
						break;
					default:
						break;
					}
				}
			}
		}
		//BaseSiteで水系とヒットしていなければ、BaseLineもチェック
		if(!aSts){
			lCount = SelectByShape(ipCheckLine, m_ipBaseLineFeatureClass, &ipCrossFeatureCursor, aSpatialRel);
			if( lCount > 0 ){
				IFeaturePtr		ipChkFeature;
				while(( S_OK==ipCrossFeatureCursor->NextFeature( &ipChkFeature ))&&(ipChkFeature != NULL)){
					IGeometryPtr	ipChkGeom;
					if( SUCCEEDED( ipChkFeature->get_ShapeCopy( &ipChkGeom ))){
						CComVariant vaOID = GetValue(ipChkFeature,_T("OBJECTID"));
						CComVariant vaVal = GetValue(ipChkFeature,base_line::kBgClass);
						switch( vaVal.intVal ){
						case base_line::bg_class::kRiver		:	//河川・沢・谷
						case base_line::bg_class::kBreakwater	:	//防波堤
							aSts = true;	//水系と交差している		
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
	return( aSts );
}
/**
*	@brief	無視するリンクかどうか
*	@note	道路種別(細街路L3,フェリー航路,仮想リンク,ブリッジリンク,施設出入口/内リンク,各社向け特殊リンク) 
*	@retval	true	無視するリンク
false	無視しないリンク
*/
bool CDoc::isIgnoreLink(_IRowPtr ipRow)
{
	using namespace sindy::schema;

	int aRoad_C = GetValue(ipRow, road_link::kRoadClass).intVal;
	if( aRoad_C==road_link::road_class::kNarrowL3
		|| aRoad_C==road_link::road_class::kFerryNoDisp
		|| aRoad_C==road_link::road_class::kFerryS2
		|| aRoad_C==road_link::road_class::kFerryS3
		|| aRoad_C==road_link::road_class::kFerryS4
		|| aRoad_C==road_link::road_class::kVirtual
		||	aRoad_C==road_link::road_class::kBridge
		|| aRoad_C==road_link::road_class::kFacilityEntrance
		|| aRoad_C==road_link::road_class::kParkingEntrance
		|| aRoad_C==road_link::road_class::kParking
		|| aRoad_C==road_link::road_class::kMatching
		|| aRoad_C==road_link::road_class::kMatchingBT )
	{
		//無視するリンク
		return true;
	}
	return false;
}

/**
*	@brief	最近傍ポイントとの距離取得
*	@param	piTargetGeom	[in]	計測したいジオメトリ
*	@param	piSorcePoint	[in]	基準となるポイント
*	@param	rNearestPoint	[out]	最近傍ポイント
*	@retval	距離
*/
double CDoc::GetNearestDist(const IGeometryPtr piTargetGeom, const IPointPtr piSorcePoint, IPointPtr& rNearestPoint)
{
	//ソースポイントのXYを取得
	WKSPoint aSorceWKS = {0,0};;
	piSorcePoint->get_X(&aSorceWKS.X);
	piSorcePoint->get_Y(&aSorceWKS.Y);

	//IProximityOperatorで最も近いポイントを取得
	IProximityOperatorPtr ipProximity(piTargetGeom);
	ipProximity->ReturnNearestPoint(piSorcePoint, esriNoExtension, &rNearestPoint);
	//最近傍ポイントのXYを取得
	WKSPoint aNearestWKS = {0,0};
	rNearestPoint->get_X(&aNearestWKS.X);
	rNearestPoint->get_Y(&aNearestWKS.Y);

	//距離計算
	CMeshPos	aMeshPos;
	return aMeshPos.GetDist(aSorceWKS.X, aSorceWKS.Y, aNearestWKS.X, aNearestWKS.Y);	
}

/**
*	@brief	条件なしの通行禁止リンクかどうか
*	@note	通行禁止種別コード(条件なし, 通行不可, 条件なし通行禁止(関係者以外))
*	@retval	true	条件なしの通行禁止
false	条件なしの通行禁止以外
*/
bool CDoc::isNopassLink(_IRowPtr ipRow)
{
	using namespace sindy::schema;

	int aNopass_C = GetValue(ipRow, road_link::kNoPassage).intVal;
	if( aNopass_C==road_link::no_passage::kNoPassage
		|| aNopass_C==road_link::no_passage::kInpassable
		|| aNopass_C==road_link::no_passage::kRestricted )
	{
		//条件なし通行禁止
		return true;
	}
	return false;
}

/**
*	@brief	条件なしの一方通行かどうか
*	@note	一方通行種別種別コード(条件なし一方通行(正方向/逆方向))
*	@retval	true	条件なし一方通行
false	条件なし一方通行ではない
*/
bool CDoc::isOnewayLink(int cOneway)
{
	using namespace sindy::schema;

	if( cOneway==road_link::oneway::kFore
		|| cOneway==road_link::oneway::kReverse)
	{
		//条件なし一方通行
		return true;
	}
	return false;
}

/** 
*	@brief	追跡の末にたどり着いたリンクを追跡する
*	@note	追跡の末に到達したリンクが一方通行の場合は一方通行ではない基本道路に到達するまで追跡する
*	@param	ipFeature	[in]	追跡フィーチャ
*	@param	ipGeom	[in]	追跡ジオメトリ
*	@param	cLink [in] 施設ポイント周辺の道路リンクコンテナ
*	@retval	追跡コード
*/
int CDoc::IsTraceSuccessLink(IFeaturePtr ipFeature, IGeometryPtr ipGeom, LinkArray& rLink)
{
	using namespace sindy::schema;

	//追跡に成功したかどうか。
	//一方通行を考慮して追跡できない場合を考慮する
	int isOver=1;
	//一方通行の場合は 一方通行ではない基本道路＋道路幅員5.5m以上リンクに接続するまで追跡する
	int aOneway = GetValue(ipFeature, road_link::kOneway).intVal;
	if(isOnewayLink(aOneway)){
		isOver = LinkTrace(ipFeature, ipGeom, rLink);
	}
	return isOver;
}

/**
* アクセス道路チェック結果の出力
* @note アクセス道路チェック結果の出力を行います。
* @param int			(I)	施設情報番号（1～）
* @param CFacilPoint	(I)	施設アクセス道路情報
* @return	bool
*/

bool CDoc::showAccessRoad(int cNo,CFacilPoint *cFacil)
{	
	bool bRET=true;
	IFeaturePtr ipFeature = cFacil->getNearLink().getFeature();

	//正しいアクセス道路は、ログ出力しない
	if(( cFacil->getAccessSts()==1 )||( cFacil->getAccessSts()==2 ))
		return true;

	using namespace sindy::schema;

	CString	aMsg1("");
	CString	aMsg2("");
	CString	aDouClassStr("");
	CString	aDouWidthStr("");
	GetValueStringWithDomain(ipFeature, road_link::kRoadClass, &aDouClassStr);
	GetValueStringWithDomain(ipFeature, road_link::kRoadWidth, &aDouWidthStr);
	aMsg1.Format("%d\t%d\t%d",cFacil->getMeshcode(),cFacil->getX(),cFacil->getY());

	//エラーログに施設情報のシーケンス番号が出力されるように,
	//各caseに cFacil->getCode()); を追加 2007/3/26 masuda
	switch( cFacil->getAccessSts()){
	case 1:	//（正常）アクセス道路あり【基本道路で誘導】
		aMsg2.Format("\tNAVI\t%7.3f\t%ld\t%ld\t正しいアクセス道\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		break;
	case 2:	//（正常）アクセス道路あり【ILQ_Routeで誘導】
		aMsg2.Format("\tROUTE\t%7.3f\t%ld\t%ld\t正しいアクセス道\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode()); 
		break;
	case 3:	//（エラー）施設まで到達できない
		aMsg2.Format("\tFATAL\t%7.3f\t%ld\t%ld\t上位接続道路へ到達できない\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode()); 
		cout << "\rERROR：施設まで到達できない... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		bRET=false;
		break;
	case 5:	//（ワーニング）水系と交差している
		aMsg2.Format("\tWARNING\t%7.3f\t%ld\t%ld\t施設と道路の間に水系がある\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode()); 
		cout << "\rWARNING：水系と交差している... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		break;
	case 7:	//（ワーニング）離島リンク
		aMsg2.Format("\tWARNING\t%7.3f\t%ld\t%ld\t離島リンク\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rWARNING：離島リンク... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		break;
	case 8:	//（ワーニング）要目視確認
		aMsg2.Format("\tWARNING\t%7.3f\t%ld\t%ld\t要目視確認\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rWARNING：要目視確認... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		break;
	case 9: //(エラー)　最近傍リンクが通行禁止
		aMsg2.Format("\tERROR\t%7.3f\t%ld\t%ld\t最近傍リンクが条件なしの通行禁止\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rERROR：通行禁止リンク... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		bRET=false;
		break;
	case 10: // (ワーニング) 最近傍の接続リンクが正しいアクセス道
		aMsg2.Format("\tWARNING\t%7.3f\t%ld\t%ld\t最近傍に接続しているリンクが正しいアクセス道\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rWARNING：最近傍の接続リンクが正しい... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		break;
	default: //（エラー）アクセス道路なし
		aMsg2.Format("\tFATAL\t%7.3f\t%ld\t%ld\tアクセス道路なし\t\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rERROR：アクセス道路なし... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		bRET=false;
		break;
	}
	//エラーログファイルへ書き込み
	if( m_Fp ){	fprintf(m_Fp,"%s%s\n",aMsg1,aMsg2);	}
	ATLTRACE(TEXT("%s%s\n"),aMsg1,aMsg2);

	return bRET;
}
/**
* 空間検索エンジン
* @note 指定フィーチャクラスと切り出し用ジオメトリより、空間検索を行います。
* @param IGeometryPtr		(I)	切り出し用ジオメトリ
* @param IFeatureClassPtr	(I)	検索対象フェーチャクラス
* @param IFeatureCursor	(O)	フィーチャカーソルへのポインタ
* @param esriSpatialRelEnum(I)	空間検索演算オペレータ	
* @return	true：変換成功、false：変換失敗
*/
long CDoc::SelectByShape(IGeometryPtr       ipGeom, 
						 IFeatureClassPtr   ipFeatureClass, 
						 IFeatureCursor**   ipFeatureCursor,
						 esriSpatialRelEnum spatialRel )
{
	LONG              lFeatureCount = 0;

	/// クエリフィルタ作成
	if( ipFeatureClass != NULL ){
		CComBSTR			bstrFieldName;
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ISpatialFilterPtr	ipFilter( CLSID_SpatialFilter );
		ipFilter->put_GeometryField( bstrFieldName );
		ipFilter->putref_Geometry( ipGeom );
		ipFilter->put_SpatialRel( spatialRel );

		/// 検索結果が 0 ならリターン
		ipFeatureClass->FeatureCount( ipFilter, &lFeatureCount );
		if( lFeatureCount < 1 ) {
			return 0;
		}
		/// 検索
		ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ipFeatureCursor );
	}
	return lFeatureCount;
}

long CDoc::SelectByWhereClause(IFeatureClassPtr& r_ipFeatureClass, LPCTSTR strWhereClause, std::vector< CAdapt<IFeaturePtr> >& rFeatureVec ) const
{
	rFeatureVec.clear();

	IQueryFilterPtr	ipFilter( CLSID_QueryFilter );
	ipFilter->put_WhereClause(CComBSTR(strWhereClause));

	IFeatureCursorPtr ipFeatureCursor;

	/// 検索
	r_ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipFeatureCursor );

	// OBJECTIDでソートしたいため、mapを作る
	// ソートするのは、何回やっても結果が変わらないようにするため
	// (検索に引っかかった順では処理するたびに並びが変わってしまう)
	typedef std::pair<long, CAdapt<IFeaturePtr> > IDFPair;
	std::map< long, CAdapt<IFeaturePtr> > aTmpMap;

	IFeaturePtr ipFeature;
	while(S_OK==ipFeatureCursor->NextFeature(&ipFeature)){
		long lObjectID = -1;
		ipFeature->get_OID(&lObjectID);
		aTmpMap.insert( IDFPair(lObjectID, ipFeature) );
	}

	// mapに格納された順にvectorに追加していく
	BOOST_FOREACH( const IDFPair& rIDFPair, aTmpMap){
		rFeatureVec.push_back(rIDFPair.second);
	}

	return rFeatureVec.size();
}

bool CDoc::GetFeaturePoint(const IFeaturePtr& r_ipFeature, WKSPoint& rWKSPoint) const
{
	IGeometryPtr ipGeometry;
	if( FAILED( r_ipFeature->get_Shape(&ipGeometry) ) ){
		return false;
	}
	IPointPtr ipPoint(ipGeometry);
	ipPoint->QueryCoords(&(rWKSPoint.X), &(rWKSPoint.Y));

	return true;
}

std::pair<int, int> CDoc::getLL256(const WKSPoint& rWKSPoint) const
{
	std::pair<int, int> aPair;
	static crd_cnv crd;
	crd.LLtoLL256(rWKSPoint.X, rWKSPoint.Y, &(aPair.first), &(aPair.second) );
	return aPair;
}

bool CDoc::OpenFeatureClass(const IFeatureWorkspacePtr& r_ipFeatureWorkspace, LPCTSTR strFeatureClassName, IFeatureClass** ppFeatureClass)
{
	if( FAILED(r_ipFeatureWorkspace->OpenFeatureClass( CComBSTR(strFeatureClassName), ppFeatureClass) ) ){
		std::cerr << (LPCSTR)CStringA(strFeatureClassName) << "\tのオープンに失敗" << std::endl;
		return false;
	}
	return true;
}

bool CDoc::OpenTable(const IFeatureWorkspacePtr& r_ipFeatureWorkspace, LPCTSTR strTableName, ITable** ppTable)
{
	if( FAILED(r_ipFeatureWorkspace->OpenTable( CComBSTR(strTableName) , ppTable) ) ){
		std::cerr << (LPCSTR)CStringA(strTableName) << "\tのオープンに失敗" << std::endl;
		return false;
	}
	return true;
}

/**
* 施設情報ポイントファイルより、Shapefileに変換
* @note 施設情報ポイントファイルより、ShapeFileに変換します。
* @param 施設情報出力ファイル名
* @return	true：変換成功、false：変換失敗
*/
bool CDoc::makeAccessRoadList(void)
{
	CString	 aStr;
	cout << "\rアクセス道路リスト作成開始." << endl;
	if(( m_ipINF_Route==NULL )||( m_ipLQ_Route==NULL )){
		return false;
	}
	std::set<long>			aVRouteInfID;
	std::set<long>			aVRouteLinkID;
	std::set<CAccessRoad>	SetAccessRoad;
	//アクセス道情報ＩＤ登録
	IQueryFilterPtr		piQueryFilter;
	if( SUCCEEDED(piQueryFilter.CreateInstance(CLSID_QueryFilter))){
		if( SUCCEEDED(piQueryFilter->put_WhereClause(CComBSTR("ROUTECLASS_C=1")))){
			_ICursorPtr	piCursor;
			long lCount=0;
			m_ipINF_Route->RowCount(piQueryFilter, &lCount);
			if( S_OK == m_ipINF_Route->Search(piQueryFilter, VARIANT_FALSE, &piCursor)){
				_IRowPtr piRow;
				while( S_OK == piCursor->NextRow( &piRow )){
					CComVariant vaVal;
					vaVal = GetValue(piRow,_T("OBJECTID"));
					if( vaVal.vt!=VT_NULL ){
						long aInfID = vaVal.intVal;
						aVRouteInfID.insert(aInfID);
						cout << "\rアクセス道路情報ＩＤ登録中... "<<  aVRouteInfID.size();
					}
				}
			}
		}
	}
	//アクセス道リンクＩＤ登録
	_ICursorPtr	piCursor;
	m_ipLQ_Route->Search(0, VARIANT_FALSE, &piCursor);
	_IRowPtr piRow;
	while( S_OK == piCursor->NextRow( &piRow )){
		_variant_t vaVal;
		vaVal = GetValue(piRow,"Inf_ID");	
		if(aVRouteInfID.find((long)vaVal) != aVRouteInfID.end()) {
			vaVal = GetValue(piRow, "LINK_ID");
			aVRouteLinkID.insert((long)vaVal);
			cout << "\rアクセス道路リンクＩＤ登録中... " << aVRouteLinkID.size();
		}
	}
	cout << "                                                                           \r";	//クリア

	using namespace sindy::schema;

	//アクセス道路（経路対象外 or 道路幅員5.5ｍ未満）リンク情報をファイル出力
	CString	aFileName1;
	CString	aFileName2;
	aFileName1.Format("%s\\%s.txt",getLogDirStr(),"access_road");
	aFileName2.Format("%s\\%s_debug.txt",getLogDirStr(),"access_road");
	if( aVRouteLinkID.size()){

		//		cout << "\rSDE空間検索中.." << endl;
		FILE	*fp1=NULL,*fp2=NULL;	int i=0;		
		if( NULL!=(fp1=fopen(aFileName1,"wb"))){			//リリース用
			if( m_DebugMode )  fp2=fopen(aFileName2,"w");	//ディバグ用
			IFeatureCursorPtr	ipFeatureCursor;
			aStr.Format("ROUTE_LQRF=1");
			CComBSTR	aComBStr = aStr;
			IQueryFilterPtr	ipQueryFilter(CLSID_QueryFilter);
			ipQueryFilter->put_WhereClause(aComBStr);
			if( S_OK==m_ipLinkFeatureClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor)&&(ipFeatureCursor != NULL)){
				IFeaturePtr ipFeatures;
				while( S_OK==ipFeatureCursor->NextFeature(&ipFeatures)){
					_variant_t vaValLinkID = GetValue(ipFeatures,"OBJECTID");	
					if(aVRouteLinkID.find((long)vaValLinkID) != aVRouteLinkID.end()) {
						CComVariant vaVal[4];
						vaVal[0] =GetValue(ipFeatures,road_link::kTmpMeshCode);
						vaVal[1] =GetValue(ipFeatures,road_link::kFromNodeID);
						vaVal[2] =GetValue(ipFeatures,road_link::kToNodeID);
						vaVal[3] =GetValue(ipFeatures,_T("OBJECTID"));

						//無視する道路種別かどうかをチェックする
						if( isIgnoreLink(ipFeatures)){
							continue;
						}

						if(( vaVal[1].vt!=VT_NULL )&&( vaVal[2].vt!=VT_NULL )&&( vaVal[3].vt!=VT_NULL )){
							int  aMeshcode = 0;
							long aSNodeID  = vaVal[1].intVal;
							long aENodeID  = vaVal[2].intVal;
							long aLinkID   = vaVal[3].intVal;
							//							if( !aMeshcode )	//Del 2004/11/16
							{
								IGeometryPtr ipGeometry;
								if( SUCCEEDED( ipFeatures->get_ShapeCopy(&ipGeometry))){
									IPointCollectionPtr ipPointCollection(ipGeometry);
									long	aPointCount=0;
									ipPointCollection->get_PointCount(&aPointCount);
									CMapDPoint dPs[3];
									//									if( aPointCount>0 )	//修正 2004/11/15
									if( aPointCount>1 )
									{
										int			aX=0,aY=0, aIdx=1;
										IPointPtr	ipPoint[2];
										ipPointCollection->get_Point(0, &ipPoint[0]);
										ipPoint[0]->get_X(&dPs[0].eX);
										ipPoint[0]->get_Y(&dPs[0].eY);
										if( aPointCount>2 ){
											aIdx=2;		//修正メッシュ境界が起終点の道路を想定
										}
										ipPointCollection->get_Point(aPointCount-aIdx, &ipPoint[1]);
										ipPoint[1]->get_X(&dPs[1].eX);
										ipPoint[1]->get_Y(&dPs[1].eY);
										dPs[2].eX = (double)(dPs[0].eX+dPs[1].eX)/2.0;
										dPs[2].eY = (double)(dPs[0].eY+dPs[1].eY)/2.0;

										CMeshPos	aMP;
										aMP.LLtoMesh(dPs[2].eX,dPs[2].eY,
											2,				//メッシュコードレベル
											&aMeshcode,	//メッシュコード
											&aX, &aY);		//正規化座標
									}
								}
							}
							if( fp1 ){
								CAccessRoad	aRoad(aMeshcode,aSNodeID,aENodeID);
								SetAccessRoad.insert(aRoad);
								//								fprintf( fp1,"%d\t0x%x\t0x%x\n",aMeshcode,aSNodeID,aENodeID);
							}
							if( fp2 ){
								CString	aRoadStr;
								CString	aNaviStr;
								CString	aWidthStr;
								GetValueStringWithDomain(ipFeatures, road_link::kRoadClass, &aRoadStr);
								GetValueStringWithDomain(ipFeatures, road_link::kNaviClass, &aNaviStr);
								GetValueStringWithDomain(ipFeatures, road_link::kRoadWidth, &aWidthStr);
								//								fprintf( fp2,"%d\t%ld\t%ld\t%ld\t%s\t%s\t%s\n",
								//											aMeshcode,aSNodeID,aENodeID,aLinkID,aRoadStr,aNaviStr,aWidthStr);
								fprintf( fp2,"0\tROAD_LINK\t%ld\t\t\tアクセス道路\t%d\t%s\t%s\t%s\n",
									aLinkID,aMeshcode,aRoadStr,aNaviStr,aWidthStr);
							}
							cout << "\rアクセス道路リスト作成中... " << ++i << "/" << aVRouteLinkID.size();
						}
						else{
							TRACE("ERROR	ROAD_LINK Attr is NULL\n");	}
					}
				}
			}
		}
		//並べ替えた道路を出力
		i=0;
		for( set<CAccessRoad>::iterator it = SetAccessRoad.begin(); it!=SetAccessRoad.end(); it++){
			CAccessRoad	aRoad = *it;
#ifdef	_DEBUG	
			fprintf( fp1,"%d\t%ld\t%ld\n",aRoad.getMeshcode(),aRoad.getFromId(),aRoad.getToId());
#else
			fprintf( fp1,"%d\t0x%x\t0x%x\n",aRoad.getMeshcode(),aRoad.getFromId(),aRoad.getToId());
#endif
			cout << "\rアクセス道路リスト整列中... " << ++i << "/" << SetAccessRoad.size();
		}
		if( fp1 )(void)fclose(fp1);
		if( fp2 )(void)fclose(fp2);
	}
	cout << "                                                                           \r";	//クリア
	cout << "\rアクセス道路リスト作成終了\t" << aVRouteLinkID.size() << endl;
	cout << "FILE:" << aFileName1.GetBuffer(256) << endl;

	aVRouteInfID.clear();
	aVRouteLinkID.clear();
	SetAccessRoad.clear();

	return true;
}
/**
* ＳＤＥ接続
* @note ＳＤＥへの接続、フィーチャポインタを取得します。
* @param なし
* @return	true：変換成功、false：変換失敗
*/
bool CDoc::Init(void)
{
	//地図切り出し用SDE接続
	HRESULT hr;
	m_ipWorkspace = sindy::_create_workspace(m_strRoadDB, hr );
	if( FAILED(hr) ){
		std::cerr << "地図切り出し用DBワークスペース取得失敗" << std::endl;
		return false;
	}

	using namespace sindy::schema;

	//対象フィーチャクラスポインタの取得
	IFeatureWorkspacePtr ipFeatureWorkspace(m_ipWorkspace);	
	if( !OpenFeatureClass( ipFeatureWorkspace, road_link::kTableName, &m_ipLinkFeatureClass)
		|| !OpenFeatureClass( ipFeatureWorkspace, road_node::kTableName, &m_ipNodeFeatureClass)
		|| !OpenFeatureClass( ipFeatureWorkspace, base_site::kTableName, &m_ipBaseSiteFeatureClass)
		|| !OpenFeatureClass( ipFeatureWorkspace, base_line::kTableName, &m_ipBaseLineFeatureClass)
		|| !OpenTable( ipFeatureWorkspace, lq_route::kTableName, &m_ipLQ_Route)
		|| !OpenTable( ipFeatureWorkspace, inf_route::kTableName, &m_ipINF_Route) ){
			std::cerr << "地図切り出し用フィーチャクラス取得失敗" << std::endl;
			return false;
	}
	return true;
}
/**
* SDE接続終了
* @note	SDEワークスペースインターフェース参照カウンタの解放
* @param	なし
* @return	なし
*/
void CDoc::Close(void)
{
	m_ipWorkspace=NULL;
	m_ipLinkFeatureClass=NULL;
	m_ipNodeFeatureClass=NULL;
	m_ipAdmFeatureClass=NULL;
	m_ipBaseSiteFeatureClass=NULL;
	m_ipBaseLineFeatureClass=NULL;
	m_ipLQ_Route = NULL;
	m_ipINF_Route = NULL;
}
/**
* 指定フェーチャのフィールド値ドメインを取得【UTIL】
* @note 指定フェーチャのフィールド値ドメインを取得します
* @param ipRow				[in]	IRowインタフェースへのポインタ
* @param lpcszFieldName	[in]	フィールド名
* @param coString			[out]	コード値ドメイン名
* @return	指定フィールド値を返す
*/
void CDoc::GetValueStringWithDomain(const IFeaturePtr ipFeatures, LPCTSTR lpcszFieldName, CString* coString)
{
	*coString=_T("");
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;
	if( ipFeatures == NULL )
		return;

	USES_CONVERSION;

	IFieldsPtr	ipFields;
	ipFeatures->get_Fields( &ipFields );
	long		lFieldIndex;
	ipFields->FindField( CComBSTR(lpcszFieldName), &lFieldIndex );
	if( lFieldIndex >= 0 ){
		ipFeatures->get_Value( lFieldIndex, &vaValue );
		IDomainPtr	ipDomain;
		IFieldPtr	ipField;
		ipFields->get_Field(lFieldIndex, &ipField);
		if( ipField!=NULL && SUCCEEDED(ipField->get_Domain(&ipDomain)) && ipDomain != 0) {
			// ドメインを取得できたらドメイン名を返す
			BSTR aDomainStr;
			char tmpStr[256];
			if(variantToDomainName(ICodedValueDomainPtr(ipDomain), vaValue, &aDomainStr)) {
				CString strAlias = OLE2T(aDomainStr);	//Add 2006/9/29 SHIMANO
				strcpy(tmpStr, strAlias);
				*coString = tmpStr;
			}
		}
	}	
	return;
}
/**
* 指定フェーチャのフィールド値ドメインを取得【UTIL】
* @note コード値ドメインインタフェースに問い合わせ、指定したVARIANT値に対応するドメイン名を取得します
* @param ipCodedValueDomain	[in]	ICodedValueDomainインタフェースへのポインタ
* @param cVariant				[in]	フィールド名
* @param coDomainName			[out]	コード値ドメイン名
* @return	true:取得成功   false:取得失敗
*/
/// コード値ドメインインタフェースに問い合わせ、指定したVARIANT値に対応するドメイン名を取得
bool CDoc::variantToDomainName(ICodedValueDomain* ipCodedValueDomain, const _variant_t& cVariant, BSTR* coDomainName)
{
	if( !ipCodedValueDomain) {
		ATLTRACE(_T("コード値ドメインインタフェースがNULL\n"));
		return false;
	}
	long aCodeCount;
	ipCodedValueDomain->get_CodeCount(&aCodeCount);

	for(int i=0; i<aCodeCount; ++i) {
		_variant_t aVar;
		ipCodedValueDomain->get_Value(i, &aVar);
		if(cVariant == aVar) {
			ipCodedValueDomain->get_Name(i, coDomainName);
			return true;
		}
	}
	return false;
}
/**
* 指定フェーチャのフィールド値を返す【UTIL】
* @note 指定フェーチャインタフェースの、指定フィールド名より、フィールド値を返します
* @param ipRow				[in]	IRowインタフェースへのポインタ 
* @param lpcszFieldName	[in]	フィールド名
* @return	指定フィールド値を返す
*/
CComVariant CDoc::GetValue(const _IRowPtr ipRow, const LPCTSTR lpcszFieldName)
{
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;
	if( ipRow == NULL ) return vaValue;

	IFieldsPtr	ipFields;	ipRow->get_Fields( &ipFields );
	LONG		lFieldIndex;
	ipFields->FindField( CComBSTR( lpcszFieldName ), &lFieldIndex );
	long lFieldCount;
	ipFields->get_FieldCount( &lFieldCount );

	if( lFieldIndex >= 0 ){
		ipRow->get_Value( lFieldIndex, &vaValue );
	}	
	return vaValue;
}
/**
* 指定フェーチャのフィールド値を返す
* @note 指定フェーチャインタフェースの、指定フィールド名より、フィールド値を返します
* @param ipFeatures		[in]	フィーチャインタフェースへのポインタ
* @param lpcszFieldName	[in]	フィールド名
* @return	指定フィールド値を返す
*/
CComVariant CDoc::GetValue(const IFeaturePtr ipFeatures, const LPCTSTR lpcszFieldName)
{
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;
	if( ipFeatures == NULL ) return vaValue;

	IFieldsPtr	ipFields;	ipFeatures->get_Fields( &ipFields );
	LONG		lFieldIndex;
	ipFields->FindField( CComBSTR( lpcszFieldName ), &lFieldIndex );
	if( lFieldIndex >= 0 ){
		ipFeatures->get_Value( lFieldIndex, &vaValue );
	}	
	return vaValue;
}
/**
* 指定道路OBJECTIDより、アクセス道か判定
* @note 指定道路幅員コードより、アクセス道か判定します
* @param 道路幅員コード
* @return	true：アクセス道路である、false：アクセス道路ではない
*/
bool CDoc::isLinkQueRoute(IFeaturePtr ipFeature)
{
	using namespace sindy::schema;

	if( 1 == GetValue(ipFeature,road_link::kRouteLQRF).intVal){
		return true;
	}
	return false;
}
/**
* 指定道路幅員コードより、アクセス道か判定
* @note 指定道路幅員コードより、アクセス道か判定します
* @param 道路幅員コード
* @return	true：アクセス道路である、false：アクセス道路ではない
*/
bool CDoc::isNaviRoadWidth(const int cWidth, const int cRoadClass)
{
	using namespace sindy::schema;

	bool aSts = false;
	switch( cRoadClass ){
	case road_link::road_class::kHighway:				//高速自動車道路	
	case road_link::road_class::kCityHighway:			//都市高速道路
	case road_link::road_class::kCountry:				//一般国道	
	case road_link::road_class::kPrefectureMain:		//主要地方道(都府県)
	case road_link::road_class::kCityMain:				//主要地方道(指定市)
	case road_link::road_class::kPrefectureNormal:		//一般都道府県道
	case road_link::road_class::kCityNormal:			//指定市の一般市道
	case road_link::road_class::kTollCountry:			//一般国道(有料)
	case road_link::road_class::kTollPrefectureMain:	//主要地方道(都府県)(有料)
	case road_link::road_class::kTollCityMain:			//主要地方道(指定市)(有料)
	case road_link::road_class::kTollPrefectureNormal:	//一般都道府県道(有料)
	case road_link::road_class::kTollCityNormal:		//指定市の一般市道(有料)
	case road_link::road_class::kTollOther:				//その他の道路(有料)
		//		case road_link::road_class::kOther:					//その他の道路
		//		case road_link::road_class::kOtherNoDisp:			//格下げその他道
		aSts = true;
		break;
	default:
		break;
	}
	//その他道以上なら、道路幅員判定しない
	if( aSts ){
		return( aSts );
	}
	switch( cWidth ){
	case road_link::road_width::kUnsurveyed:		//未調査・作業中
	case road_link::road_width::kBetween30And55:	//幅員3.0m以上5.5m未満
	case road_link::road_width::kUnder30:			//幅員3.0m未満
		break;
	case road_link::road_width::kOver130:			//幅員13.0m以上
	case road_link::road_width::kBetween55And130:	//幅員5.5m以上13.0m未満
		aSts = true;
		break;
	default:
		break;
	}
	return( aSts );
}
/**
* 道路種別コードより、一般都道府県道以上の道路か判定
* @note	道路種別コードより、一般都道府県道以上の道路か判定します
* @param	道路リンクフィーチャへのポインタ
* @return	true：県道以上道路である、false：県道以上の道路ではない
*/
bool CDoc::isKendoClassLink(const IFeaturePtr piFeature)
{
	bool	aSts=false;

	using namespace sindy::schema;

	if( piFeature!=NULL ){
		CComVariant vaVal[2];
		vaVal[0] = GetValue(piFeature,_T("OBJECTID"));
		vaVal[1] = GetValue(piFeature,road_link::kRoadClass);
		switch( vaVal[1].intVal ){
		case road_link::road_class::kHighway:				//高速自動車道路
		case road_link::road_class::kCityHighway:			//都市高速道路
		case road_link::road_class::kCountry:				//一般国道
		case road_link::road_class::kTollCountry:			//一般国道(有料)
		case road_link::road_class::kPrefectureMain:		//主要地方道(都府県)
		case road_link::road_class::kTollPrefectureMain:	//主要地方道(都府県)(有料)
		case road_link::road_class::kCityMain:				//主要地方道(指定市)
		case road_link::road_class::kTollCityMain:			//主要地方道(指定市)(有料)
		case road_link::road_class::kPrefectureNormal:		//一般都道府県道
		case road_link::road_class::kTollPrefectureNormal:	//一般都道府県道(有料)
		case road_link::road_class::kCityNormal:			//指定市の一般市道
		case road_link::road_class::kTollCityNormal:		//指定市の一般市道(有料)
			aSts = true;
			break;
		default: break;
		}
	}
	return( aSts );
}
/**
* 指定道路種別コードより、基本道路か判定
* @note 指定道路種別コードより、基本道路か判定します
* @param 道路種別コード
* @return	true：基本道路である、false：基本道路ではない
*/
bool CDoc::isNaviLink(const int cViewMode,const IFeaturePtr piFeature)
{
	if( piFeature == NULL )
		return false;

	using namespace sindy::schema;

	CComVariant vaVal[4];
	vaVal[0] = GetValue(piFeature,_T("OBJECTID"));
	vaVal[1] = GetValue(piFeature,road_link::kRoadClass);
	vaVal[2] = GetValue(piFeature,road_link::kRoadWidth);
	vaVal[3] = GetValue(piFeature,road_link::kNaviClass);
	int aRoadClass = vaVal[1].intVal;
	int aRoadWidth = vaVal[2].intVal;
	int aNaviClass = vaVal[3].intVal;

	bool	aSts = false;
	switch( cViewMode ){
	case VIEW_ARC_FERRY:
		switch( aRoadClass ){
		case road_link::road_class::kFerryNoDisp:	//フェリー航路(S4破線表示部)
		case road_link::road_class::kFerryS2:		//フェリー航路(S3破線表示部)
		case road_link::road_class::kFerryS3:		//フェリー航路(S2破線表示部)
		case road_link::road_class::kFerryS4:		//フェリー航路(非表示部)
			break;
		default:
			aSts = true;
			break;
		}
		break;
	case VIEW_ARC_DISP :
		switch( aRoadClass ){
		case road_link::road_class::kHighway:				//高速自動車道路	
		case road_link::road_class::kCityHighway:			//都市高速道路
		case road_link::road_class::kFreeHighway:			//無料高速
		case road_link::road_class::kCountry:				//一般国道
		case road_link::road_class::kTollCountry:			//一般国道(有料)
		case road_link::road_class::kPrefectureMain:		//主要地方道(都府県)
		case road_link::road_class::kTollPrefectureMain:		//主要地方道(都府県)(有料)
		case road_link::road_class::kCityMain:				//主要地方道(指定市)
		case road_link::road_class::kTollCityMain:			//主要地方道(指定市)(有料)
		case road_link::road_class::kPrefectureNormal:		//一般都道府県道
		case road_link::road_class::kTollPrefectureNormal:	//一般都道府県道(有料)
		case road_link::road_class::kCityNormal:			//指定市の一般市道
		case road_link::road_class::kTollCityNormal:		//指定市の一般市道(有料)
		case road_link::road_class::kOther:					//その他の道路
		case road_link::road_class::kTollOther:				//その他の道路(有料)
		case road_link::road_class::kOtherNoDisp:			//格下げその他道
			aSts = true;
			break;
		case road_link::road_class::kParking:				// 駐車場内道路
		default: break;
		}
		break;
	case VIEW_ARC_NAVI :
		switch( aNaviClass ){
		case road_link::navi_class::kHighway:				//高速自動車道路
		case road_link::navi_class::kCityHighway:			//都市高速道路
		case road_link::navi_class::kCountry:				//一般国道
		case road_link::navi_class::kPrefectureMain:		//主要地方道(都府県)
		case road_link::navi_class::kCityMain:				//主要地方道(指定市)
		case road_link::navi_class::kPrefectureNormal:		//一般都道府県道
		case road_link::navi_class::kCityNormal:			//指定市の一般市道
		case road_link::navi_class::kOther:					//その他道路
		case road_link::navi_class::kLinkCountry:		 	//接続国道
		case road_link::navi_class::kTollCountry:			//一般国道(有料)
		case road_link::navi_class::kTollPrefectureMain:	//主要地方道(都府県)(有料)
		case road_link::navi_class::kTollCityMain:			//主要地方道(指定市)(有料)
		case road_link::navi_class::kTollPrefectureNormal:	//一般都道府県道(有料)
		case road_link::navi_class::kTollCityNormal:		//指定市の一般市道(有料)
		case road_link::navi_class::kTollOther:				//その他道路(有料)
			aSts = true;
			break;
		default:			break;							//経路対象外道路
		}
		break;
	}
	//経路対象道路で、道路幅員が、５．５ｍ未満の場合はアクセス道路と判定しない
	if( cViewMode != VIEW_ARC_FERRY ){
		if( aSts ){
			//			if( !isNaviRoadWidth(aRoadWidth,aRoadClass))	//Del Ver.1.2 2003/9/29
			if(( aRoadClass==road_link::navi_class::kOther )||( aRoadClass==road_link::road_class::kOtherNoDisp )){
				if( !isNaviRoadWidth(aRoadWidth,aNaviClass)){
					aSts = false;
				}
			}
		}
	}
	return aSts;
}
//------------------------------[ end of file ]------------------------------
