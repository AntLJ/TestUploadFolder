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

#include "ShapeChk.h"

CShapeChk::~CShapeChk(void)
{
}
/**
 * @brief	形状チェック実行
 * @return	bool
 */
bool CShapeChk::app(void)
{
	//shape読み込み(比較元)
	ShapeInfo a_OriShapeInfo;
	int a_OriShapeType = 0;
	if(! load_shape_info(&a_OriShapeInfo,m_sFullPassOri,&a_OriShapeType))
		return false;
	//shapeタイプがポイントだった場合、チェックをしない
	if( a_OriShapeType == SHPT_POINT ){
		return true;
	}
	//shape読み込み(比較先)
	ShapeInfo a_DestShapeInfo;
	int a_DestShapeType = 0;
	load_shape_info(&a_DestShapeInfo,m_sFullPassDest,&a_DestShapeType);
	if( a_DestShapeType != a_OriShapeType ){
		cerr << "#ERROR 比較元<" << m_sFullPassOri.c_str() << ">と比較先<" << m_sFullPassDest.c_str() << ">でShpaeタイプが異なります。" << endl;
		return false;
	}
	//2つのshapeの形状比較
	if(!comp_Shp_info(a_OriShapeInfo,a_DestShapeInfo))
	{
		return false;
	}
	
	return true;

}
/**
* @brief	Shapeファイルを読み込む
* @param	string	[in] 対象Shapeファイルのフルパス
* @return	bool
*/
bool CShapeChk::load_shape_info(ShapeInfo* p_ShapeInfo, const string& r_sFullPath,int* p_ShapeType)
{
	CSHPHandle		a_cSHPHdl;
	CDBFHandle		a_cDBFHdl;
	//Shape DBFファイルのオープン
	if( !a_cSHPHdl.open((char*)r_sFullPath.c_str(),"rb") || !a_cDBFHdl.open((char*)r_sFullPath.c_str(),"rb"))
	{
		cerr << "#ERROR SHPファイル<" << r_sFullPath.c_str() << ">の形状のオープンに失敗!" << endl;
		return false;
	}
	//Shapeのオブジェクト数・タイプを取得
	int aShpCount=0;
	double min_bound[4],max_bound[4];
	if( !a_cSHPHdl.get_FileInfo(&aShpCount,p_ShapeType,min_bound,max_bound))
	{
		cerr << "#ERROR SHPファイル<" << r_sFullPath.c_str() << ">のオブジェクト数取得に失敗!" << endl;
		return false;
	}
	//Shapeタイプがポイントだった場合、処理終了。
	if( *p_ShapeType == SHPT_POINT)
		return true;
	//DBFのオブジェクト数を取得
	int aDbfCount = a_cDBFHdl.get_RecordCount();
	if( aShpCount != aDbfCount ){
		cerr << "#ERROR <" << r_sFullPath.c_str() << ">の.shpと.dbfのレコード数が一致しません!" << endl;
		return false;
	}
	//オブジェクト数分、形状の情報、属性の情報取得
	for(int i=0; i<aShpCount; ++i)
	{
		ObjectInfo a_ObjectInfo;
		//形状情報の取得
		if( !get_Obj_info(&a_ObjectInfo,a_cSHPHdl,i)){
			cerr << "#ERROR SHPファイル<" << r_sFullPath.c_str() << ">の形状情報取得に失敗!" << endl;
			continue;
		}
		int OID;
		//属性情報(オブジェクトIDまたはFIDを含む)の取得
		if( !get_Dbf_info(&OID,&a_ObjectInfo,a_cDBFHdl,i)){
			cerr << "#ERROR SHPファイル<" << r_sFullPath.c_str() << ">の属性情報取得に失敗!" << endl;
			continue;
		}
		//オブジェクト情報をコンテナに挿入
		p_ShapeInfo->insert(pair<int,ObjectInfo>(OID,a_ObjectInfo));
	}
	//SHP,DBFファイルのクローズ
	a_cSHPHdl.close();
	a_cDBFHdl.close();
	return true;
}
/**
* @brief	SHPの形状情報を読み込む
* @param	ObjectInfo* [in] オブジェクト情報
* @param	CSHPHandle [in] 対象Shapeのハンドリングクラス
* @param	int	[in] 対象ObjectのIndex
* @return	bool
*/
bool CShapeChk::get_Obj_info(ObjectInfo* pObjectInfo,CSHPHandle &r_cSHPHdl, int a_index)
{
	CSHPObject aObj;
	//形状取得
	if( -1 == r_cSHPHdl.read_Object(a_index,&aObj))
		return false;
	//パート数の取得
	pObjectInfo->e_PartCount = aObj.get_PartCount();
	//パート毎の始点インデックス取得
	for(int i=0; i<pObjectInfo->e_PartCount; ++i){
		//パートの情報取得
		CSHPPart aSHPPart;
		aObj.get_Part(i,&aSHPPart);
		int aStartIndex;
		int aType;
		aSHPPart.get_Part(&aStartIndex,&aType);
		pObjectInfo->e_PartIndexInfo.insert(pair<int,int>(i,aStartIndex));
	}
	//補間点数の取得
	pObjectInfo->e_PointCount = aObj.get_PointCount();
	//補間点数分の情報取得
	for(int j=0; j<pObjectInfo->e_PointCount; ++j)
	{
		//ポイント情報取得
		CSHPPoint a_Point;
		aObj.get_Point(j,&a_Point);
		//座標取得
		d_lonlat a_lonlat;
		a_lonlat.lon = a_Point.get_X();
		a_lonlat.lat = a_Point.get_Y();
		//コンテナに情報を挿入
		pObjectInfo->e_PointsInfo.insert( pair<int,d_lonlat>(j,a_lonlat));
	}
	return true;
}
/**
* @brief	SHPの属性情報を読み込む
* @param	int*	[out] オブジェクトIDまたはFID
* @param	ObjectInfo* [out] オブジェクト情報
* @param	CDBFHandle [in] 対象Shapeのハンドリングクラス
* @param	int [in] 対象ObjectのIndex
* @return	bool
*/
bool CShapeChk::get_Dbf_info(int *pID, ObjectInfo *pObjectInfo, CDBFHandle &r_cDBFHdl, int a_index)
{
	//オブジェクトID or FID の読み込み
	if( m_iExeMode & ON_FID_MODE )
		//FIDの読み込み
		*pID = a_index;
	else {
		//オブジェクトIDの読み込み
		int a_FieldIndex = r_cDBFHdl.get_FieldID("OBJECTID");
		r_cDBFHdl.read_Attribute(a_index, a_FieldIndex, pID);
	}
	return true;
}
/**
* @brief	SHPの形状比較を行う
* @param	ShapeInfo&	[in] 比較元のSHP情報
* @param	ShapeInfo&	[in] 比較先のSHP情報
* @return	bool
*/
bool CShapeChk::comp_Shp_info(ShapeInfo &r_OriShapeInfo, ShapeInfo &r_DestShapeInfo)
{
	int iErrFlag = 0;
	//比較元Shapeの形状を基準にチェックを行う。
	ShapeInfo::iterator ori_itr = r_OriShapeInfo.begin();
	for( ;ori_itr != r_OriShapeInfo.end(); ++ori_itr ){
		//チェック対象Shapeが比較先に存在するかチェック
		ShapeInfo::iterator dest_itr = r_DestShapeInfo.find(ori_itr->first);
		if( dest_itr == r_DestShapeInfo.end()){
			*m_pErrorLog
				<< "0\t" << m_sLayerName.c_str() << "\t" << ori_itr->first << "\t"
				<< ori_itr->second.e_PointsInfo[0].lon << "\t" << ori_itr->second.e_PointsInfo[0].lat << "\tERROR\t"
				<< "DATAFINDERR(形状)\t対応するオブジェクトが比較先に存在しません\t"
				<< m_MeshCode << endl;
			iErrFlag = 1;
			continue;
		}
		//補間点数が一致するかチェック
		if( ori_itr->second.e_PointCount != dest_itr->second.e_PointCount ){
			*m_pErrorLog
				<< "0\t" << m_sLayerName.c_str() << "\t" << ori_itr->first << "\t"
				<< ori_itr->second.e_PointsInfo[0].lon << "\t" << ori_itr->second.e_PointsInfo[0].lat << "\tERROR\t"
				<< "POINTNUMERR(形状)\t補間点の個数が異なっています(比較元:" << ori_itr->second.e_PointCount << "比較先:" << dest_itr->second.e_PointCount << ")\t"
				<< m_MeshCode << endl;
			iErrFlag = 1;
			continue;
		}
		//補間点数が複数であることをチェック
		if( ori_itr->second.e_PointCount == 1){
			*m_pErrorLog
				<< "0\t" << m_sLayerName.c_str() << "\t" << ori_itr->first << "\t"
				<< ori_itr->second.e_PointsInfo[0].lon << "\t" << ori_itr->second.e_PointsInfo[0].lat << "\tERROR\t"
				<< "POINTNUMERR(形状)\t補間点数が1です" 
				<< m_MeshCode << endl;
			iErrFlag = 1;
			continue;
		}
		//距離が一致するかチェック
		double a_DistDiff;
		DistInfo a_DistInfo = {0.0,0.0};
		//距離の取得方法をマルチパートとそれ以外で分ける
		int aCount = ori_itr->second.e_PartCount;
		if(ori_itr->second.e_PartCount > 1){
			Shp_Dist_chk_multi(&a_DistDiff,&a_DistInfo, ori_itr->second,dest_itr->second);
		}else{
			Shp_Dist_chk(&a_DistDiff,&a_DistInfo, ori_itr->second,dest_itr->second);
		}

		//距離の差のパーセンテージを求める
		double a_DistDiffPar = a_DistDiff / (a_DistInfo.e_DestDist / 100);

		//パーセンテージがボーダーより上だったらNG
		if( a_DistDiffPar > m_dDistBoder ){
			*m_pErrorLog
				<< "0\t" << m_sLayerName.c_str() << "\t" << ori_itr->first << "\t"
				<< ori_itr->second.e_PointsInfo[0].lon << "\t" << ori_itr->second.e_PointsInfo[0].lat << "\tERROR\t"
				<< "DISTERR(形状)\tオブジェクトの距離が異なります\t"
				<< m_MeshCode << "\t" << a_DistInfo.e_OriDist << "\t" << a_DistInfo.e_DestDist << "\t" <<  a_DistDiff << "\t" << a_DistDiffPar << endl;
			iErrFlag = 1;
			continue;
		}
	}

	if (iErrFlag == 1)
	{
		return false;
	}
	else 
	{
		return true;
	}
}
/**
* @brief	オブジェクトの距離チェックを行う
* @param	double		[out] 比較元と比較先のオブジェクトの距離の差
* @param	DistInfo*	[out] 比較元・比較先のオブジェクトの距離格納
* @param	ObjectInfo&	[in] 比較元のオブジェクト情報
* @param	ObjectInfo&	[in] 比較先のオブジェクト情報
* @return	bool
*/
bool CShapeChk::Shp_Dist_chk(double* p_DistDiff, DistInfo* p_DistInfo, ObjectInfo &r_OriObjectInfo, ObjectInfo &r_DestObjectInfo)
{
	//p_DistDiff(距離の合計値)の初期化
	*p_DistDiff = 0.0;
	//セグメント同士の距離の差の絶対値を求め合計を出す
	crd_cnv a_Crd;
	PointsInfo::iterator ori_itr = r_OriObjectInfo.e_PointsInfo.begin();
	PointsInfo::iterator dest_itr = r_DestObjectInfo.e_PointsInfo.begin();
	for(int i=0; i<r_OriObjectInfo.e_PointCount-1; ++i){
		//比較元、比較先オブジェクトのセグメントの距離を出す(単位m)
		//比較元、比較先セグメントの始点、終点取得
		d_lonlat a_OriFrom = ori_itr->second;
		d_lonlat a_OriTo = (++ori_itr)->second;
		d_lonlat a_DestFrom = dest_itr->second;
		d_lonlat a_DestTo = (++dest_itr)->second;
		//距離取得
		double a_OriSegDist = a_Crd.GetDist(a_OriFrom,a_OriTo);

		//世界測地系の距離の場合は、ライブラリを使用
		double a_DestSegDist = a_Crd.GetDist_JGD2000(a_DestFrom.lon,a_DestFrom.lat,a_DestTo.lon,a_DestTo.lat);

		//各セグメントの差の絶対値を求め,p_DistDiffに足す。
		*p_DistDiff += fabs( a_OriSegDist - a_DestSegDist );
		//各セグメントの距離を総距離に足す
		p_DistInfo->e_OriDist += a_OriSegDist;
		p_DistInfo->e_DestDist += a_DestSegDist;
	}
	return true;
}
/**
* @brief	オブジェクトの距離チェックを行う(マルチパート用)
* @param	double		[out] 比較元と比較先のオブジェクトの距離の差
* @param	DistInfo*	[out] 比較元・比較先のオブジェクトの距離格納
* @param	ObjectInfo&	[in] 比較元のオブジェクト情報
* @param	ObjectInfo&	[in] 比較先のオブジェクト情報
* @return	bool
*/
bool CShapeChk::Shp_Dist_chk_multi(double *p_DistDiff, DistInfo *p_DistInfo, ObjectInfo &r_OriObjectInfo, ObjectInfo &r_DestObjectInfo)
{
	//p_DistDiff(距離の合計値)の初期化
	*p_DistDiff = 0.0;
	//セグメント同士の距離の差の絶対値を求め合計を出す
	crd_cnv a_Crd;
	PointsInfo::iterator ori_itr = r_OriObjectInfo.e_PointsInfo.begin();
	PointsInfo::iterator dest_itr = r_DestObjectInfo.e_PointsInfo.begin();
	//パート数繰り返す。
	for(int i=0;i<r_OriObjectInfo.e_PartCount;++i){
		//partの始点・終点Index取得
		int aStartIndex;
		int aEndIndex;
		if( i==0 )
			aStartIndex =0;
		else 
			aStartIndex = r_OriObjectInfo.e_PartIndexInfo[i];
		if( i==(r_OriObjectInfo.e_PartCount-1) )
			aEndIndex = (r_OriObjectInfo.e_PointCount-1);
		else
			aEndIndex = r_OriObjectInfo.e_PartIndexInfo[i+1]-1;
		//partの始点から終点までのセグメント毎の距離を取得
		for( int j=aStartIndex; j<aEndIndex; ++j){
			//比較元、比較先オブジェクトのセグメントの距離を出す(単位m)
			//比較元、比較先セグメントの始点、終点取得
			d_lonlat a_OriFrom = ori_itr->second;
			d_lonlat a_OriTo = (++ori_itr)->second;
			d_lonlat a_DestFrom = dest_itr->second;
			d_lonlat a_DestTo = (++dest_itr)->second;
			//距離取得
			double a_OriSegDist = a_Crd.GetDist(a_OriFrom,a_OriTo);
			
			//世界測地系の距離の場合は、ライブラリを使用
			double a_DestSegDist = a_Crd.GetDist_JGD2000(a_DestFrom.lon,a_DestFrom.lat,a_DestTo.lon,a_DestTo.lat);

			//各セグメントの差の絶対値を求め,p_DistDiffに足す。
			*p_DistDiff += fabs( a_OriSegDist - a_DestSegDist );
			//各セグメントの距離を総距離に足す
			p_DistInfo->e_OriDist += a_OriSegDist;
			p_DistInfo->e_DestDist += a_DestSegDist;
		}
		//次のパートに移る為、PointsInfoのイテレータを1進める
		++ori_itr;
		++dest_itr;		
	}


	return true;
}
