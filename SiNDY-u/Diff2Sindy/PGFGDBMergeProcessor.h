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

#pragma once
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_core/ErrorCode.h>
#include <WinLib/VersionInfo.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include <TDC/sindylib_base/TableContainer.h>
#include "sindy/schema.h"
#include "sindy/libschema.h"
#include "Processor.h"
#include "DBReplicater.h"
#include "RowContainer.h"
#include "GeometryBase.h"
#include "ErrorObject2.h"
#include "Notifier.h"
#include "TableNamesSearcher.h"
#include "MergingDefinitionManager.h"
#include "Editor.h"

#define S2S_NEW_MERGE_ALGORITHM

namespace{
	// DiffImportPGDBの許容値を踏襲
	const double ACCEPTABLE_X  = 0.000001;	///< メッシュ境界上の座標を比較する時の誤差許容に使用される値（経度）
	const double ACCEPTABLE_Y  = 0.000001;	///< メッシュ境界上の座標を比較する時の誤差許容に使用される値（緯度）
}

void MultiGeometry2Geometries( CRowBase& row, CTableContainer& table ); // PGFGDBConvertProcessor.cpp
void createIndexList( const CFieldMap& srcFields, const CFieldMap& dstFields, const Arguments& args, IndexList& idxs, bool& hasGeom ); // PGFGDBConvertProcessor.cpp

/**
 * @brief testGeomがbaseGeomにセグメント一致で含まれるかどうか確認する
 *
 * @param baseGeom [in] 含むほうのジオメトリ（ポリゴン限定）
 * @param testGeom [in] 含まれる方のジオメトリ（ポリライン限定）
 * @retval true testGeomはbaseGeomにふくまれる
 * @retval false testGeomはbaseGeomにふくまれない
 */
bool isContainAsSegment( IGeometryPtr baseGeom, IGeometryPtr testGeom );

/**
 * @brief testGeomがbaseGeomに構成点一致で含まれるかどうか確認する
 *
 * @param baseGeom [in] 含むほうのジオメトリ（ポリライン限定）
 * @param testGeom [in] 含まれる方のジオメトリ（マルチポイント限定）
 * @retval true testGeomはbaseGeomにふくまれる
 * @retval false testGeomはbaseGeomにふくまれない
 */
bool isContainAsVertex( IGeometryPtr baseGeom, IGeometryPtr testGeom );

namespace sindy {
	extern bool g_bSetHistory;
}

/**
 * @brief 隣接マージ実行クラス
 */
template <typename T>
class PGFGDBMergeProcessor : public Processor<T>
{
public:

	/// コンストラクタ
	PGFGDBMergeProcessor(const CWorkspace& workspace, const uh::tstring& meshDbAccessInfo, const uh::tstring& meshTable, const uh::tstring& meshTableOwner, const MergingDefinitionManager& mergeDef, const Arguments& args )
	: Processor<T>(),
	  m_workspace(workspace),
	  m_meshDbAccessInfo(meshDbAccessInfo),
	  m_meshTable(meshTable),
	  m_meshTableOwner(meshTableOwner),
	  m_MergeDefMg(mergeDef),
	  m_args(args)
	{
		CVersion version;
		m_InternalName = uh::toTStr( version.GetInternalName().GetString());		//!< ツール名称

		// 接続先メッシュDBテーブル名称を作成
		if(meshTableOwner.empty())
		{
			m_connectMeshTblName  = meshTable;
		}
		else
		{
			m_connectMeshTblName = meshTableOwner + _T('.') + meshTable;
		}

	};
	/// デストラクタ
	virtual ~PGFGDBMergeProcessor(){};

	/**
	 * @brief  処理実行
	 * @param  src  [in]  メッシュコード
	 * @retval true:成功
	 */
	virtual bool proc(const T& src);

	// @brief ループ処理前処理実行
	bool preprocess() override;

	// @brief ループ処理後処理実行
	bool postprocess() override;

	/**
	 * @brief  処理対象のWorkspaceを返却
	 * @retval 処理対象のWorkspace
	 */
	const CWorkspace& getWorkspace(){ return m_outputWorkspace; }

private:

	CEditor m_workspace;				    //!< 出力先のDB
	CWorkspace  m_meshWorkspace;			//!< メッシュDB
	uh::tstring m_meshDbAccessInfo;			//!< メッシュDB接続情報
	uh::tstring m_meshTable;				//!< 参照先メッシュテーブル
	uh::tstring m_meshTableOwner;			//!< メッシュDBオーナー
	uh::tstring m_connectMeshTblName;		//!< 接続先メッシュDBテーブル名称
	MergingDefinitionManager m_MergeDefMg;	//!< マージ対象定義クラス
	uh::tstring m_InternalName;				//!< プログラム名(更新用)
	Arguments m_args;                       //!< オプション引数

	/**
	 * @brief  ワークスペース内のテーブルの名称をすべて取得
	 * @param  workspace    [in]  ワークスペース
	 * @param  vTableNames  [in]  テーブル名の一覧
	 * @retval true:処理成功
	 */
	bool getWorkspaceTableNames(const CWorkspace& workspace,std::vector<uh::tstring>& vTableNames);

	/**
	 * @brief  メッシュのポリゴンを取得する
	 * @param  meshCode  [in]  メッシュコード
	 * @retval メッシュのポリゴン
	 */
	IGeometryPtr createMeshPolygon(const uh::tstring& meshCode);

	/**
	 * @brief  引数のジオメトリに接するフィーチャを検索する空間参照フィルタを作成し、返却
	 * @param  pGeom  [in]  ジオメトリ
	 * @retval 空間参照フィルタ
	 */
	ISpatialFilterPtr createMeshSpatialFilter(const IGeometryPtr pGeom);

	/**
	 * @brief  不要構成点を削除する(croppingShape内で使用)
	 * @note   「不要構成点」とは「メッシュ境界に存在する削除しても形状に変化がない構成点」である
	 * @param  ipSrcGeom  [in]  削除対象のジオメトリ
	 * @param  meshPolygon  [in]  メッシュポリゴン
	 * @retval 不要構成点を削除したジオメトリ(リング形状)
	 */
	IGeometryPtr croppingRing(const IGeometryPtr ipSrcGeom, const IGeometryPtr meshPolygon);

	/**
	 * @brief  不要構成点を削除する
     * @note   「不要構成点」とは「メッシュ境界に存在する削除しても形状に変化がない構成点」である
	 * @param  ipSrcGeom  [in]  削除対象のジオメトリ
	 * @param  meshPolygon  [in]  メッシュポリゴン
	 * @retval 不要構成点を削除したジオメトリ
	 */
	IGeometryPtr croppingShape(const IGeometryPtr ipSrcGeom, const IGeometryPtr meshPolygon);

	/**
	 * @brief  メッシュの最大値、最小値を取得
	 * @param  ipMeshGeom  [in]  メッシュのジオメトリ
	 * @param  minX  [out]  X最小値
	 * @param  minY  [out]  Y最小値
	 * @param  maxX  [out]  X最大値
	 * @param  maxY  [out]  Y最大値
	 * @retval true:処理成功
	 */
	bool getMeshXY(IGeometryPtr ipMeshGeom ,double& minX,double& minY,double& maxX,double& maxY);

	/**
	 * @brief  メッシュ区画で分断されているジオメトリが、マージが必要か判定する
	 * @param  ipSrcGeom  [in]  比較元のジオメトリ
	 * @param  ipTgtGeom  [in]  比較先のジオメトリ
	 * @param  meshPolygon  [in]  メッシュポリゴン
	 * @retval true:マージが必要
	 */
	bool isNeedMerge(IGeometryPtr ipSrcGeom, IGeometryPtr ipTgtGeom, IGeometryPtr meshPolygon);

	/**
	 * @brief  ポリラインのFrom方向の座標を取得
	 * @param  pGeom  [in]  ポリラインのジオメトリ
	 * @retval From方向の座標
	 */
	WKSPoint getFromXY(const IGeometryPtr pGeom);

	/**
	 * @brief  ポリラインのTo方向の座標を取得
	 * @param  pGeom  [in]  ポリラインのジオメトリ
	 * @retval To方向の座標
	 */
	WKSPoint getToXY(const IGeometryPtr pGeom);

	/**
	 * @brief  ポリラインのチェックを行う
	 * @note   チェック項目は以下通り
	 *         ・メッシュ境界で3本以上のポリラインと接続していないか
	 * @param  con  [in]  TableContainer
	 * @param  baseRow  [in]  元なるポリライン
	 * @param  vOids  [in]  比較対象のポリラインのOID群
	 * @retval true:チェックOK　false:チェックNG
	 */
	bool polylineCheck( CTableContainer &con, CGeometryBase &baseRow, std::vector<long> vOids);

	/**
	 * @brief  マージすべきか判定し、マージ対象OIDの総当たりのリストを作成する
	 * @param  con  [in]  TableContainer(メッシュに接しているジオメトリがキャッシュされているもの)
	 * @param  mergingDef  [in]  マージ実施定義クラス
	 * @param  meshPolygon  [in]  メッシュポリゴン
	 * @retval マージ対象OIDの総当たりのリスト
	 */
	std::map<long, std::vector<long>> searchMergeFeature(CTableContainer &con, const MergingDefinition& mergingDef, IGeometryPtr meshPolygon);

	/**
	 * @brief  マージ対象のOIDのリストをユニークにする
	 * @param  vOidMap  [in]	マージ対象のOIDリスト key;oid value: vecter <oid>
	 * @retval ユニークになったOIDのリスト key:oid value: vecter <oid> ※key はvecter <oid>の中で最も値が小さいもの
	 */
	std::map<long,std::vector<long>> uniqMergeOidList(std::map<long, std::vector<long>>& vOidMap);

	/**
	 * @brief マージ対象のOIDリストから、マージ対象となる形状同士が重なっている要素を削除する
	 * @note bug11708対応で追加
	 * @param vOidMap [in, out] マージ対象のOIDリスト key;oid value: vecter <oid>
	 * ※key はvecter <oid>の中で最も値が小さいもの
	 * @param con     [in]  TableContainer(メッシュに接しているジオメトリがキャッシュされているもの)
	 */
	void eraseOverlapFeatureList(std::map<long,std::vector<long>>& vOidMap, CTableContainer& con);

	/**
	 * @brief  マージ処理を実施する
	 * @param  vUniqOidlMap  [in]  マージ対象のOIDのリスト key:oid value: vecter <oid> ※key はvecter <oid>の中で最も値が小さいもの
	 * @param  idxList [in] 属性コピー時のルール定義
	 * @param  con  [in]  TableContainer(メッシュに接しているジオメトリがキャッシュされているもの)
	 * @retval なし
	 */
	void doMerge( const std::map<long,std::vector<long>>& vUniqOidlMap, const IndexList& idxList, CTableContainer &con);

	/**
	 * @brief 属性と形状をコピーする
	 * @param listIdx [in] 属性コピー可能なインデックスの対応表（first:ソース側 second:dst側）
	 * @param src [in] コピー元
	 * @param dst [in] 新規作成されたレコード
	 * @param args [in] オプション管理クラス
	 * @param geom [in] nullじゃない時はsrcの形状より優先されてdstにセットする
	 * @return 成功すればtrue
	 */
	bool updateRow( const IndexList& listIdx, const CSPRowBase& src, CSPRowBase& dst, const Arguments& args, IGeometryPtr geom );

	/**
	 * @brief  共通属性項目を設定する
	 * @param  row  [in/out]  属性を設定するレコード
	 * @retval  true:設定成功
	 */
	bool setHistory(CRowContainer& row);
};

// 処理実行
template <typename T>
bool PGFGDBMergeProcessor<T>::proc(const T& src)
{
	uh::tstring mesh(src);

	// ログ
	{
		std::wstringstream ss;
		ss << _T("隣接マージ開始") <<  _T(" 対象メッシュ:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	// メッシュのDBに接続
	if( !m_meshWorkspace.Connect(m_meshDbAccessInfo.c_str()))
		notifier::critical::asCannotConnectDB(m_meshDbAccessInfo.c_str());

	// 条件式参照用の値をArgmentsに設定
	m_args.m_systemValue[Arguments::MESHCODE] = mesh;

	// 編集開始
	if( !m_workspace.StartOperation() )
		notifier::critical::versatile(_T("編集開始に失敗"));

	// メッシュ矩形を取得
	IGeometryPtr meshPolygon = createMeshPolygon(mesh);
	if ( !meshPolygon )
	{
		std::wstringstream ss;
		ss << _T("メッシュ形状の取得に失敗しました メッシュコード : ") <<  mesh;
		std::cerr << uh::toStr(ss.str()) << std::endl;
		notifier::versatile(ss.str(), std::wcout);
		notifier::versatile(ss.str(), std::wcerr);
		
		// 編集終了
		if( !m_workspace.StopOperation() )
			notifier::critical::versatile(_T("編集終了に失敗"));

		// falseにしてしまったらマージ処理全体が終わる為trueで返却
		return true;
	}

	// マージ定義ファイルに記載されたテーブル分だけループ
	for(unsigned  i = 0 ; i < m_MergeDefMg.size(); ++i)
	{

		MergingDefinition mergingDef = m_MergeDefMg.get(i);

		// 空間検索ファイルターを取得
		ISpatialFilterPtr pMeshFilter = createMeshSpatialFilter( meshPolygon );

		// フィーチャクラスを取得
		IFeatureClassPtr pMergeTableClass = m_workspace.Workspace().OpenSameOwnersTable(mergingDef.m_strTableName.c_str());
		if( !pMergeTableClass )
			continue;

		// メッシュに接するフィーチャを全て取得し、キャッシュする
		CTableFinder finder;
		{
			CModel model;
			model.Add( AheGetFeatureClassName(pMergeTableClass), sindy::schema::kObjectID, NULL, NULL );
			finder.InitCollection( (ITablePtr)pMergeTableClass, model );
		}
		CTableContainer& con = *finder.FindTable(AheGetFeatureClassName(pMergeTableClass));
		con._Select( (IQueryFilterPtr)pMeshFilter, false );
		con.CreateCache();

		// メッシュに接するフィーチャをすべて検査し、マージ対象のフィーチャをOIDで取得する
		std::map<long, std::vector<long>> vOidMap = searchMergeFeature(con, mergingDef, meshPolygon);

		// OIDのリストをuniqにする
		std::map<long,std::vector<long>> vUniqOidlMap = uniqMergeOidList(vOidMap);

		// uniqなOIDのリストから重複しているものを削除
		eraseOverlapFeatureList(vUniqOidlMap, con);

		// 更新をするための共通インデックスリストを作成する
		bool hasGeom = false;
		createIndexList( *con.GetFieldMap(), *con.GetFieldMap(), m_args, mergingDef.m_idxList, hasGeom );

		// OIDのリストを元にマージ処理実施
		doMerge(vUniqOidlMap, mergingDef.m_idxList, con);

		// 保存実施
		CErrorObjectsBase err;
		bool backup = g_bSetHistory;
		g_bSetHistory = m_MergeDefMg.isSetHistory(); // 保存時に履歴を付与するかどうか
		con.Store(err);
		g_bSetHistory = backup;
		if( sindy::errorcode::sindyErr_NoErr != err.GetDefaultErrCode() )
		{
			for( auto& e : err )
			{
				for( auto& info : *e )
				{
					const auto& errObj = *e->GetErrorObject();
					notifier::reportObjInfo( (LPCTSTR)errObj.get<0>(), errObj.get<1>(), notifier::error_level::error, uh::toTStr(info->GetErrorMessage() ), std::wcerr );
				}
			}
			return false;
		}
	}

	// 編集終了
	if( !( m_workspace.StopOperation() && m_workspace.SaveEditing() ) )
		notifier::critical::versatile(_T("編集終了に失敗"));

	// ログ
	{
		std::wstringstream ss;
		ss << _T("隣接マージ終了") <<  _T(" 対象メッシュ:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	return true;
}

// ループ処理前処理実行
template <typename T>
bool PGFGDBMergeProcessor<T>::preprocess()
{
	// 接続確認
	if( !m_workspace.Workspace() )
	{
		notifier::critical::versatile(_T("出力先のDBに接続失敗"));
		return false;
	}

	// 編集対象がバージョン対応か非対応かに応じて、編集を開始する
	std::list<uh::tstring> listTarget;
	for( unsigned  i = 0 ; i < m_MergeDefMg.size(); ++i )
		listTarget.push_back( m_MergeDefMg.get(i).m_strTableName );

	return m_workspace.StartEditingByObjectType(listTarget); // エラーメッセージはCEditorで出す
}

// ループ処理後処理実行
template <typename T>
bool PGFGDBMergeProcessor<T>::postprocess()
{
	return m_workspace.StopEditing(true); // エラーメッセージはCEditorで出す
}

//マージ対象OIDの総当たりのリストをユニークにする
template <typename T>
std::map<long,std::vector<long>> PGFGDBMergeProcessor<T>::uniqMergeOidList(std::map<long, std::vector<long>>& vOidMap)
{
	//////////////////////////////////////////////////////////////////////////
	// ※引数のマージ対象OIDの総当たりのリストは以下のようになっている
	// key 1 value  { 3     }
	// key 3 value  { 1, 5  }
	// key 5 value  { 3     }
	//
	// これを以下のように修正し、返却するのが本関数の目的である
	//
	// key 1 value  { 1, 3, 5 }
	//
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// それぞれをマージし以下のような状態にする
	// key 1 value  { 1, 3, 5 }
	// key 3 value  { 1, 3, 5 }
	// key 5 value  { 1, 3, 5 }
	//
	//////////////////////////////////////////////////////////////////////////
	for(auto& mergeOids : vOidMap)
	{
		long targetOid = mergeOids.first;
		std::vector<long>& vMergeList = mergeOids.second;

		//マージ対象がいない場合はスキップ
		if(vMergeList.empty())
			continue;

		//マージ対象のOIDが持つリストより、自身のマージ対象として追加すべきOIDをリスト化
		std::vector<long> vAddOids;
		for(long mergeOid : vMergeList)
		{
			if (vOidMap.find(mergeOid) == vOidMap.end())
				 continue;

			copy(vOidMap[mergeOid].begin(), vOidMap[mergeOid].end(), std::back_inserter(vAddOids) );
		}
		// マージ相手が自身をマージ対象としていない可能性もあるため、
		// 一応、自分自身もリストに加えておく
		vAddOids.push_back(targetOid);
		std::sort(vAddOids.begin(), vAddOids.end());
		vAddOids.erase(unique(vAddOids.begin(), vAddOids.end()), vAddOids.end());

		//上で作成したリスト内の各OIDに対し、互いをマージ対象として登録し合う
		for(long addOid : vAddOids)
		{
			if(vOidMap.find(addOid) == vOidMap.end())
				continue;

			std::vector<long>& vAddOidList = vOidMap[addOid];
			if(std::find(vAddOidList.begin(), vAddOidList.end(), targetOid) == vAddOidList.end())
				vAddOidList.push_back(targetOid);

			if(std::find(vMergeList.begin(), vMergeList.end(), addOid) == vMergeList.end())
				vMergeList.push_back(addOid);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Mapの特性とユニークで以下のような状態にする
	// key 1 value  { 1, 3, 5 }
	//
	//////////////////////////////////////////////////////////////////////////
	std::map<long,std::vector<long>> vUniqOidlMap;
	for(auto& mergeOids : vOidMap)
	{
		std::vector<long>& vec = mergeOids.second;

		// キーしかないものはcontinue
		if(vec.empty())
			continue;

		// ソートしてユニーク
		std::sort(vec.begin(),vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());

		// ユニークにしたものを格納
		// ※ここで既にvecはOIDでソートされているはず
		vUniqOidlMap[vec[0]] = vec;
	}

	return vUniqOidlMap;
}

// マージすべきか判定し、マージ対象OIDの総当たりのリストを作成する
template <typename T>
std::map<long, std::vector<long>> PGFGDBMergeProcessor<T>::searchMergeFeature( CTableContainer &con, const MergingDefinition& mergingDef, IGeometryPtr meshPolygon )
{
	std::map<long, std::vector<long>> vOidMap;

#ifdef S2S_NEW_MERGE_ALGORITHM
	IGeometryPtr meshBoundary; // メッシュ枠（ポリライン）
	ITopologicalOperatorPtr(meshPolygon)->get_Boundary(&meshBoundary);
	ITopologicalOperatorPtr topoMesh(meshBoundary);
#endif // if S2S_NEW_MERGE_ALGORITHM

	for( CContainerBase::const_iterator it1 = con.begin(); it1 != con.end() ; ++it1 )
	{
		// 最後の行は自分の次を見る２重ループの為、continue
		if(it1 == con.end())
			continue;
	   
		CSPRowContainer sprow1 = CAST_ROWC(*it1);
		CRowContainer& row1 = *sprow1;
		CGeometryBase& geom1 = *row1.GetShape();

#ifdef S2S_NEW_MERGE_ALGORITHM
		// メッシュ枠とのintersectを生成
		IGeometryPtr boundary1;
		ITopologicalOperatorPtr((IGeometry*)geom1)->get_Boundary(&boundary1);
		IGeometryPtr border1;
		// 一つ下の次元でのIntersect形状を取得する
		esriGeometryDimension dim = ( geom1.GetDimension() == esriGeometry1Dimension ) ? esriGeometry0Dimension : esriGeometry1Dimension;
		topoMesh->Intersect( boundary1, dim, &border1 );
#endif // if S2S_NEW_MERGE_ALGORITHM

		// キーが存在しない場合作成
#ifdef _DEBUG
		long oid = row1.GetOID();
#endif // ifdef _DEBUG
		if (!vOidMap.count(row1.GetOID()))
			vOidMap[row1.GetOID()] =  std::vector<long>();

		// it1の次から取得
		CContainerBase::const_iterator it2 = it1;
		for( ++it2 ; it2 != con.end(); ++it2 )
		{
			CSPRowContainer sprow2 = CAST_ROWC(*it2);
			CRowContainer& row2 = *sprow2;
			CGeometryBase& geom2 = *row2.GetShape();

#ifdef S2S_NEW_MERGE_ALGORITHM
			// border1とのintersectを生成
			IGeometryPtr boundary2;
			ITopologicalOperatorPtr((IGeometry*)geom2)->get_Boundary(&boundary2);
			IGeometryPtr border2;
			ITopologicalOperatorPtr(boundary2)->Intersect( border1, dim, &border2 );

			switch( dim )
			{
			case esriGeometry1Dimension:
				// 生成した形状のそれぞれのパートを、geom1,geom2がセグメントとして
				// 保有していればマージ対象とする
				if( !( isContainAsSegment( (IGeometry*)geom1, border2 ) &&
					isContainAsSegment( (IGeometry*)geom2, border2 ) ) )
					continue;
				break;
			case esriGeometry0Dimension:
				// ポリラインの場合は、border2はポイントになる
				// 生成したポイントをgeom1,geom2が構成点として
				// 保有していればマージ対象とする
				if( ! ( isContainAsVertex( (IGeometry*)geom1, border2 ) &&
					isContainAsVertex( (IGeometry*)geom2, border2 ) ) )
					continue;
				break;
			default:
				break;
			}
#else
			// メッシュ境界上で線分(ポリゴンの場合)、またはポイント(ラインの場合)を共有しているか
			try {
				if (!isNeedMerge((IGeometry*)geom1,(IGeometry*)geom2,meshPolygon))
					continue;
			}
			catch(...)
			{
				std::wstringstream ss;
				ss << _T("例外が発生した為マージを行いませんでした") <<  _T(" 比較先OID: ") << row2.GetOID();
				notifier::reportObjInfo( row1.GetTableName(), row1.GetOID(), uh::tstring(ss.str()), std::wcout );
				continue;
			}
#endif // if S2S_NEW_MERGE_ALGORITHM
			// 比較元と比較対象で属性は同じでないものはcontinue
			if(mergingDef.m_cCondition.exps && !mergingDef.m_cCondition.exps->eval( CAST_ROWB(sprow1), CAST_ROWB(sprow2), m_args, std::list<uh::tstring>() ))
			{
				std::wstringstream ss;
				ss << _T("属性の値が同値ではない為マージを行いませんでした") <<  _T(" 比較先OID: ") << row2.GetOID();
				notifier::reportObjInfo( row1.GetTableName(), row1.GetOID(), uh::tstring(ss.str()), std::wcout );
				continue;
			}

			// マージ対象である場合、OIDを保持
			vOidMap[row1.GetOID()].push_back(row2.GetOID());
			// キーが存在しない場合作成
			if (!vOidMap.count(row2.GetOID()))
				vOidMap[row2.GetOID()] =  std::vector<long>();
			// 対象の方も格納
			vOidMap[row2.GetOID()].push_back(row1.GetOID());
		}
		// 接続しているポリラインが3つ以上あった時メッシュ境界で接するラインが3本以上あるか判定
		if( ( row1.GetShape()->GetGeometryType() == esriGeometryPolyline ) &&
			vOidMap[row1.GetOID()].size() >= 2 &&
			!polylineCheck( con, geom1, vOidMap[row1.GetOID()] ))
		{
			std::wstringstream ss;
			uh::tstring oidlist;
			std::for_each( vOidMap[row1.GetOID()].begin(), vOidMap[row1.GetOID()].end(), [&oidlist](long oid){if (!oidlist.empty()){ oidlist += _T(","); } oidlist += std::to_wstring(oid);});
			ss << _T("メッシュ境界で接するラインが3本以上あるためマージしません") <<  _T(" 比較先OID : ") << oidlist;
			notifier::reportObjInfo( row1.GetTableName(), row1.GetOID(), uh::tstring(ss.str()), std::wcout );

			// リストから除外
			vOidMap.erase( row1.GetOID() );
		}

	}
	return vOidMap;
}


// ポリラインの座標を取得
WKSPoint getWKSPointXY(const IGeometryPtr pGeom , bool directionTo)
{
	WKSPoint wksPoint = {};

	esriGeometryType geomType = esriGeometryNull;
	pGeom->get_GeometryType( &geomType );
	// ポリラインでない場合は空の座標をリターン
	if( geomType != esriGeometryPolyline )
		return wksPoint;

	IPointPtr ipPoint;

	if (directionTo)
	{
		((IPolylinePtr)pGeom)->get_ToPoint( &ipPoint );
	}
	else
	{
		((IPolylinePtr)pGeom)->get_FromPoint( &ipPoint );
	}
	ipPoint->QueryCoords( &wksPoint.X, &wksPoint.Y );

	return wksPoint;
}

// ポリラインのFrom方向の座標を取得
template <typename T>
WKSPoint PGFGDBMergeProcessor<T>::getFromXY(const IGeometryPtr pGeom)
{
	return getWKSPointXY(pGeom, false);
}

// ポリラインのTo方向の座標を取得
template <typename T>
WKSPoint PGFGDBMergeProcessor<T>::getToXY(const IGeometryPtr pGeom)
{
	return getWKSPointXY(pGeom, true);
}

template <typename T>
bool PGFGDBMergeProcessor<T>::polylineCheck( CTableContainer &con, CGeometryBase &geom, std::vector<long> vOids)
{
	unsigned fromMatchCnt(0),toMatchCnt(0);

	// 比較元のFrom,Toのポイントを取得
	WKSPoint wksSrcFromPoint = getFromXY((IGeometry*)geom);
	WKSPoint wksSrcToPoint =  getToXY((IGeometry*)geom);

	for(const auto& oid : vOids)
	{
		CRowContainer& targetRow = *CAST_ROWC(*con.find_by_key(sindy::schema::kObjectID, oid));
		CGeometryBase& targetGeom =  *targetRow.GetShape();

		WKSPoint wksTgtFromPoint = getFromXY((IGeometry*)targetGeom);
		WKSPoint wksTgtToPoint = getToXY((IGeometry*)targetGeom);

		// 比較元Fromと比較先To,Fromが一致するか
		if( ( wksSrcFromPoint.X == wksTgtToPoint.X && wksSrcFromPoint.Y == wksTgtToPoint.Y ) ||
			( wksSrcFromPoint.X == wksTgtFromPoint.X && wksSrcFromPoint.Y == wksTgtFromPoint.Y ) )
		{
			++fromMatchCnt;
		}

		// 比較元Toと比較先To,Fromが一致するか
		if( ( wksSrcToPoint.X == wksTgtToPoint.X && wksSrcToPoint.Y == wksTgtToPoint.Y ) ||
			( wksSrcToPoint.X == wksTgtFromPoint.X && wksSrcToPoint.Y == wksTgtFromPoint.Y ) )
		{
			++toMatchCnt;
		}
	}

	// 一つのポリラインに複数接続されている
	if ( fromMatchCnt > 1 ||  toMatchCnt > 1)
		return false;

	return true;
}

// マージ対象のリストから、形状が重なっているものを削除する
template <typename T>
void PGFGDBMergeProcessor<T>::eraseOverlapFeatureList(std::map<long,std::vector<long>>& vOidMap, CTableContainer& con)
{
	// 形状の重なりチェック用のラムダ式
	auto checkShapeOverlap = [&con](std::vector<long>& oidlist)
	{
		for(auto itBase = oidlist.begin(); itBase!=oidlist.end(); ++itBase)
		{
			CRowContainer& baseRow = *CAST_ROWC(*con.find_by_key(sindy::schema::kObjectID, *itBase));
			CGeometryBase& baseGeom =  *baseRow.GetShape();
			for(auto itTarget=std::next(itBase); itTarget!=oidlist.end(); ++itTarget)
			{
				CRowContainer& targetRow = *CAST_ROWC(*con.find_by_key(sindy::schema::kObjectID, *itTarget));
				CGeometryBase& targetGeom =  *targetRow.GetShape();
				if( baseGeom.Overlaps((IGeometry*)targetGeom)
					|| baseGeom.Contains((IGeometry*)targetGeom) 
					|| targetGeom.Contains((IGeometry*)baseGeom))
				{
					return true;
				}
			}
		}
		return false;
	};

	// 重複しているものを削除する
	for(auto itOidMapPair = vOidMap.begin(); itOidMapPair != vOidMap.end(); )
	{
		auto& oidList = itOidMapPair->second;
		if( checkShapeOverlap(oidList) )
			itOidMapPair = vOidMap.erase(itOidMapPair);
		else
			++itOidMapPair;
	}
}

// マージ処理実行
template <typename T>
void PGFGDBMergeProcessor<T>::doMerge(const std::map<long,std::vector<long>>& vUniqOidlMap, const IndexList& idxList, CTableContainer &con)
{
	for (const auto& mergeOids : vUniqOidlMap )
	{
		CSPRowContainer spBaseRow = CAST_ROWC(*con.find_by_key(sindy::schema::kObjectID, mergeOids.first));
		CRowContainer& baseRow = *spBaseRow;
		CGeometryBase& baseGeom = *baseRow.GetShape();

		for (long targetOid : mergeOids.second)
		{
			// using namespace
			using namespace sindy::errorcode;

			CSPRowContainer spTargetRow = CAST_ROWC(*con.find_by_key(sindy::schema::kObjectID, targetOid));
			CRowContainer& targetRow = *spTargetRow;
			CGeometryBase& targetGeom = *targetRow.GetShape();

			// 自分自身はcontinue
			if(mergeOids.first == targetOid)
				continue;

			sindyErrCode err = sindyErr_NoErr;
			IGeometryPtr unionGeom = CGeometryBase::Union( baseGeom, targetGeom, &err );
			if( sindyErr_NoErr != err )
			{
				notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(),notifier::error_level::error, _T("マージに失敗"), std::wcout );
				notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(),notifier::error_level::error,  _T("マージに失敗"), std::wcerr );
				continue;
			}
			VARIANT_BOOL isEmpty = VARIANT_FALSE;
			unionGeom->get_IsEmpty( &isEmpty );

			if( isEmpty )
			{
				notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(),notifier::error_level::error, _T("マージしたら形状が消滅"), std::wcout );
				notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(),notifier::error_level::error, _T("マージしたら形状が消滅"), std::wcerr );
				continue; // 処理は継続する
			}
			// 更新
			updateRow( idxList, CAST_ROWB(spTargetRow), CAST_ROWB(spBaseRow), m_args, unionGeom );
			// マージされる側は削除
			targetRow.Delete();
		}
		// 形状をすべてマージしてからマルチパートを解消
		if( m_args.m_dissolveMulti )
			MultiGeometry2Geometries( *CAST_ROWB(spBaseRow), con );

		// ログ
		{
			uh::tstring oidlist;
			for (unsigned i = 0; i < mergeOids.second.size(); ++i )
			{
				if (i != 0){ oidlist += _T(","); }
				oidlist += std::to_wstring(mergeOids.second[i]) ;
			}
			std::wstringstream ss;
			ss << _T("マージ成功") <<  _T(" マージ実施OID: ") << oidlist;
			notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(), uh::tstring(ss.str()), std::wcout );
		}
	}
}

// 属性と形状をコピー
template <typename T>
bool PGFGDBMergeProcessor<T>::updateRow( const IndexList& listIdx, const CSPRowBase& src, CSPRowBase& dst, const Arguments& args, IGeometryPtr geom )
{
	// TODO: PGFGDBConvertProcessor.cppのUpdate関数と共通している処理は一纏めにする

	// キャッシュ作成
	// 全てが更新対象になるわけではないので、キャッシュ作成は更新対象のみに限定する
	// これによりパフォーマンスの低下がある場合は、一括キャッシュを検討する（多分大丈夫だと思う）
	dst->CreateCache();

	// 属性
	for( const auto& idxExps : listIdx )
	{
		for( const auto& idxExp : idxExps.second )
		{
			const IndexPair& idx = idxExp.first;
			const SPConditionExpressions& exps = idxExp.second;
			if( !exps || exps->eval( src, dst, args, std::list<uh::tstring>() ) ) // 条件なし＝無条件
			{
				if( IndexPair::None == idx.type || 				        // コピー対象ではない
					IndexPair::RowValue == idx.type && 0 > idx.srcIdx ) // レコード値なのにインデックス番号が有効ではない
					continue;

				dst->SetValue( idx.dstIdx, idx.value(*src) );
				break; // 採用される値は一つだけ（先勝ち）
			}
		}
	}
	// 形状
	if( !geom )
		geom = src->GetShapeCopy();

	if( geom )
		dst->SetShape(geom);

	return true;
}

// ワークスペースに存在するテーブル名称の一覧を取得する
template <typename T>
bool PGFGDBMergeProcessor<T>::getWorkspaceTableNames(const CWorkspace& workspace,std::vector<uh::tstring>& vTableNames)
{
	// データセット取得
	IEnumDatasetNamePtr ipEnumDataset = workspace.GetDatasetNames(esriDTAny);
	if ( !ipEnumDataset )
	{
		return false;
	}

	// テーブル一覧を取得
	TableNamesSearcher searcher;
	searcher.getWorkspaceTableNames(ipEnumDataset, vTableNames);
	return true;
}

// メッシュのポリゴンを取得
template <typename T>
IGeometryPtr PGFGDBMergeProcessor<T>::createMeshPolygon(const uh::tstring& meshCode)
{
	// 対象都市地図メッシュのポリゴンを取得
	IFeatureClassPtr pMeshClass = m_meshWorkspace.OpenTable(m_connectMeshTblName.c_str());
	IFeatureCursorPtr pMeshCursor;
	pMeshClass->Search(
		AheInitQueryFilter(
		nullptr,
		nullptr,
		uh::toTStr( uh::toStr( sindy::schema::category::mesh_code_table::kMeshCode ) + " = " + uh::toStr( meshCode )).c_str()
		),
		VARIANT_TRUE,
		&pMeshCursor
		);

	// １つだけ存在する事を前提とする
	IFeaturePtr pMeshFearure;
	pMeshCursor->NextFeature(&pMeshFearure);
	if ( ! pMeshFearure )
	{
		return pMeshFearure;
	}

	// 形状のコピーを取得
	IGeometryPtr pMesh;
	pMeshFearure->get_ShapeCopy(&pMesh);

	return pMesh;
}

// 処理対象の空間検索クエリフィルタを作成
template <typename T>
ISpatialFilterPtr PGFGDBMergeProcessor<T>::createMeshSpatialFilter(IGeometryPtr pGeom)
{
	// 返却値
	ISpatialFilterPtr ret;

	IGeometryPtr ipBoundary;
	ITopologicalOperatorPtr( pGeom )->get_Boundary(&ipBoundary);
	//継承のフィールド値を取得
	IFeatureClassPtr pMeshClass = m_meshWorkspace.OpenTable(m_meshTable.c_str());
	CComBSTR bstrFieldName;
	pMeshClass->get_ShapeFieldName(&bstrFieldName);

	ret = AheInitSpatialFilter(
			nullptr,
			ipBoundary,
			bstrFieldName,
			esriSearchOrderSpatial,
			esriSpatialRelIntersects); // Touchesだと別のメッシュ枠でマージされたものが検索対象とならない
	return ret;
}

// メッシュポリゴン又はメッシュ境界ライン取得関数
// 境界上の厳密な比較をするための最大最小値も取得
template <typename T>
bool PGFGDBMergeProcessor<T>::getMeshXY(IGeometryPtr ipMechGeom ,double& minX,double& minY,double& maxX,double& maxY)
{
	IPointCollectionPtr ipPointCollection(ipMechGeom);

	long aPointCount = 0;
	ipPointCollection->get_PointCount( &aPointCount );
	std::vector<WKSPoint> pPt( aPointCount );
	ipPointCollection->QueryWKSPoints( 0, aPointCount, &pPt.at(0) );

	double aMinX, aMinY, aMaxX, aMaxY;
	aMinX = aMinY = 1000000.0;
	aMaxX = aMaxY = -1000000.0;
	for( int j = 0; j < aPointCount; j++ ) {
		if(aMinX > pPt[j].X)
			aMinX = pPt[j].X;
		if(aMinY > pPt[j].Y)
			aMinY = pPt[j].Y;
		if(aMaxX < pPt[j].X)
			aMaxX = pPt[j].X;
		if(aMaxY < pPt[j].Y)
			aMaxY = pPt[j].Y;
	}

	maxX = aMaxX;
	maxY = aMaxY;
	minX = aMinX;
	minY = aMinY;
	return true;
}

// 許容範囲を持たせた座標（値）の一致確認関数
bool fnCompXY(double cPoint1, double cPoint2, double cAcceptable)
{
	if(fabs(cPoint1 - cPoint2) <= cAcceptable){
		return true;
	}else{
		return false;
	}
}

// 不要構成点を削除した後の形状求める関数(必要に応じてポリゴンをリングに分解して再構成する)。
template <typename T>
IGeometryPtr PGFGDBMergeProcessor<T>::croppingShape(IGeometryPtr ipSrcGeom, IGeometryPtr meshPolygon)
{
	IGeometryPtr ipResultGeom;
	IGeometryCollectionPtr ipGeoCol = ipSrcGeom;
	long aPart = 0;
	ipGeoCol->get_GeometryCount(&aPart);

	if(aPart == 1){
		ipResultGeom = croppingRing(ipSrcGeom, meshPolygon);
	}else{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ISpatialReferencePtr ipSpRef;
		ipSrcGeom->get_SpatialReference(&ipSpRef);
		ipPolygon->putref_SpatialReference(ipSpRef);
		IGeometryCollectionPtr ipResultGeoCol = ipPolygon;
		for(int i = 0; i < aPart; i++){
			IGeometryPtr ipSubGeom, ipResultSub;
			ipGeoCol->get_Geometry(i, &ipSubGeom);
			ipResultSub = croppingRing(ipSubGeom, meshPolygon);
			ipResultGeoCol->AddGeometry(ipResultSub);
		}
		ipPolygon->SimplifyPreserveFromTo();
		ipResultGeom = ipPolygon;
	}
	return ipResultGeom;
}

// PointCollectionから指定したインデックスのWKSPointを取得する
WKSPoint getWKSPointFromPointCollection(IPointCollectionPtr ipPointCollection, int idx)
{
	WKSPoint wksPoint = {};
	IPointPtr pPoint;
	ipPointCollection->get_Point(idx, &pPoint);
	pPoint->QueryCoords( &wksPoint.X , &wksPoint.Y );

	return wksPoint;

}

// 不要構成点を削除した後の形状を求める関数（リング単位）。
template <typename T>
IGeometryPtr PGFGDBMergeProcessor<T>::croppingRing(IGeometryPtr ipSrcGeom, IGeometryPtr meshPolygon)
{
	// メッシュの最大値、最少値を取得
	double aMinX, aMinY, aMaxX, aMaxY;
	getMeshXY(meshPolygon, aMinX, aMinY, aMaxX, aMaxY);

	IGeometryPtr ipResultGeom =  AheCreateClone(ipSrcGeom);
	IPointCollectionPtr ipPointCollection = ipResultGeom;
	long aPointCount = 0;
	ipPointCollection->get_PointCount( &aPointCount );

	// 4点は三角形で一点でも削除されると直線になってしまう。
	if(!(aPointCount > 4))
	{
		return ipResultGeom;
	}

	std::list<long> deleteInex;
	for( int i = 0; i < aPointCount; ++i )
	{
		// 比較する3点のインデックス決定
		long next = i + 1;
		long nextnext = i + 2;
		// もし基準の次や、次の次がポイントの最大値を超えた場合、始点(0)から始める
		next = ( next < aPointCount ? next : next - aPointCount );
		nextnext  = ( nextnext < aPointCount ? nextnext : nextnext - aPointCount );

		// 基準のポイント
		WKSPoint p1 = getWKSPointFromPointCollection(ipPointCollection, i);
		WKSPoint p2 = getWKSPointFromPointCollection(ipPointCollection, next);
		WKSPoint p3 = getWKSPointFromPointCollection(ipPointCollection, nextnext);

		// メッシュ境界の同一辺上で連続しているか？
		if( p1.X == aMinX && p2.X == aMinX && p3.X == aMinX ||
			p1.X == aMaxX && p2.X == aMaxX && p3.X == aMaxX ||
			p1.Y == aMinY && p2.Y == aMinY && p3.Y == aMinY ||
			p1.Y == aMaxY && p2.Y == aMaxY && p3.Y == aMaxY )
		{
			// 四隅にある場合は連続していても除外
			if ( (aMinX == p2.X || p2.X == aMaxX) && (aMinY == p2.Y || p2.Y == aMaxY))
			{
				continue;
			}
			// 連続している場合は削除対象としてインデックスを保持
			deleteInex.push_back(next);
		}
	}

	// 連続している構成点を削除
	bool isDellStartEnd(false);
	for( auto i : deleteInex )
	{
		// 始終点の場合はフラグで削除された事を保持
		if ( i == 0 ||  i == aPointCount-1)
		{
			isDellStartEnd = true;
		}
		// 削除
		ipPointCollection->RemovePoints( i, 1 );
	}

	// 始終点が削除された場合、始点終点を再作成しつなげる
	if ( isDellStartEnd )
	{
		IRingPtr ipRing = ipResultGeom;
		if( ipRing ){
			ipRing->Close();
		}else{
			IPolygonPtr ipPolygon = ipResultGeom;
			if( ipPolygon ){
				ipPolygon->Close();
				ipPolygon->SimplifyPreserveFromTo();
			}else{
				ipResultGeom = NULL;
			}
		}
	}

	return ipResultGeom;
}

// 形状的にマージ対象になるかを判定する関数（メッシュ境界上で線分を共有していること）
template <typename T>
bool PGFGDBMergeProcessor<T>::isNeedMerge(IGeometryPtr ipSrcGeom, IGeometryPtr ipTgtGeom, IGeometryPtr meshPolygon)
{
	if( !( ipSrcGeom && ipTgtGeom && meshPolygon ) )
		return false;

	// 一応両方のジオメトリタイプ確認
	esriGeometryType srcGeomType = esriGeometryNull;
	esriGeometryType tgtGeomType = esriGeometryNull;
	ipSrcGeom->get_GeometryType( &srcGeomType );
	ipTgtGeom->get_GeometryType( &tgtGeomType );

	if( srcGeomType != tgtGeomType )
		return false;

	// ポリゴンの場合はメッシュ境界上3点並んでいた場合真ん中を削除する
	IGeometryPtr ipSrcGeomCp,ipTgtGeomCp;
	if( srcGeomType == esriGeometryPolygon && tgtGeomType == esriGeometryPolygon )
	{
		ipSrcGeomCp = croppingShape(ipSrcGeom,meshPolygon);
		ipTgtGeomCp = croppingShape(ipTgtGeom,meshPolygon);
	}
	else
	{
		ipSrcGeomCp = ipSrcGeom;
		ipTgtGeomCp = ipTgtGeom;
	}

	IPointCollectionPtr ipPointCollectionS(ipSrcGeomCp);
	IPointCollectionPtr ipPointCollectionT(ipTgtGeomCp);
	long aPointCountS = 0;
	long aPointCountT = 0;
	ipPointCollectionS->get_PointCount( &aPointCountS );
	ipPointCollectionT->get_PointCount( &aPointCountT );

	// メッシュの最大値、最少値を取得
	double aMinX, aMinY, aMaxX, aMaxY;
	getMeshXY(meshPolygon, aMinX, aMinY, aMaxX, aMaxY);

	if( srcGeomType == esriGeometryPolygon && tgtGeomType == esriGeometryPolygon ){
		std::vector<WKSPoint> aSrcPoints( aPointCountS );
		ipPointCollectionS->QueryWKSPoints( 0, aPointCountS, &aSrcPoints.at(0) );
		std::vector<WKSPoint> aTgtPoints( aPointCountT );
		ipPointCollectionT->QueryWKSPoints( 0, aPointCountT, &aTgtPoints.at(0) );

		for(int i = 0; i < aPointCountS -1; i++){
			if(aSrcPoints[i].X == aMinX && aSrcPoints[i+1].X == aMinX){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == aMinX  && aTgtPoints[j+1].X == aMinX){
						if(fnCompXY(aSrcPoints[i].Y, aTgtPoints[j+1].Y, ACCEPTABLE_Y) && fnCompXY(aSrcPoints[i+1].Y, aTgtPoints[j].Y, ACCEPTABLE_Y)){
							return true;
						}
					}
				}
			}else if(aSrcPoints[i].X == aMaxX && aSrcPoints[i+1].X == aMaxX){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == aMaxX  && aTgtPoints[j+1].X == aMaxX){
						if(fnCompXY(aSrcPoints[i].Y, aTgtPoints[j+1].Y, ACCEPTABLE_Y) && fnCompXY(aSrcPoints[i+1].Y, aTgtPoints[j].Y, ACCEPTABLE_Y)){
							return true;
						}
					}
				}
			}else if(aSrcPoints[i].Y == aMinY && aSrcPoints[i+1].Y == aMinY){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].Y == aMinY  && aTgtPoints[j+1].Y == aMinY){
						if(fnCompXY(aSrcPoints[i].X, aTgtPoints[j+1].X, ACCEPTABLE_X) && fnCompXY(aSrcPoints[i+1].X, aTgtPoints[j].X, ACCEPTABLE_X)){
							return true;
						}
					}
				}
			}else if(aSrcPoints[i].Y == aMaxY && aSrcPoints[i+1].Y == aMaxY){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].Y == aMaxY  && aTgtPoints[j+1].Y == aMaxY){
						if(fnCompXY(aSrcPoints[i].X, aTgtPoints[j+1].X, ACCEPTABLE_X) && fnCompXY(aSrcPoints[i+1].X, aTgtPoints[j].X, ACCEPTABLE_X)){
							return true;
						}
					}
				}
			}
			// 四隅のポイントを共有している場合
			if( aSrcPoints[i].X == aMinX && aSrcPoints[i].Y == aMinY ||
					  aSrcPoints[i].X == aMaxX && aSrcPoints[i].Y == aMinY ||
					  aSrcPoints[i].X == aMinX && aSrcPoints[i].Y == aMaxY ||
					  aSrcPoints[i].X == aMaxX && aSrcPoints[i].Y == aMaxY ){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == aSrcPoints[i].X  && aTgtPoints[j].Y == aSrcPoints[i].Y){
						return true;
					}
				}
			}
		}
	}else if( srcGeomType == esriGeometryPolyline && tgtGeomType == esriGeometryPolyline ){
		std::vector<WKSPoint> aSrcPoints( aPointCountS );
		ipPointCollectionS->QueryWKSPoints( 0, aPointCountS, &aSrcPoints.at(0) );
		std::vector<WKSPoint> aTgtPoints( aPointCountT );
		ipPointCollectionT->QueryWKSPoints( 0, aPointCountT, &aTgtPoints.at(0) );

		for( const auto& srcPoint : aSrcPoints ){
			if( srcPoint.X == aMinX ){
				// 左のメッシュのオブジェクトとマージ
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.X == aMinX ){
						if( fnCompXY( srcPoint.Y, tgtPoint.Y, ACCEPTABLE_Y) ){
							return true;
						}
					}
				}
			}else if( srcPoint.X == aMaxX ){
				// 右のメッシュのオブジェクトとマージ
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.X == aMaxX ){
						if( fnCompXY( srcPoint.Y, tgtPoint.Y, ACCEPTABLE_Y) ){
							return true;
						}
					}
				}
			}else if( srcPoint.Y == aMinX ){
				// 下のメッシュのオブジェクトとマージ
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.Y == aMinY ){
						if( fnCompXY( srcPoint.X, tgtPoint.X, ACCEPTABLE_X) ){
							return true;
						}
					}
				}
			}else if( srcPoint.Y == aMaxY ){
				// 上のメッシュのオブジェクトとマージ
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.Y == aMaxY ){
						if( fnCompXY( srcPoint.X, tgtPoint.X, ACCEPTABLE_X) ){
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
