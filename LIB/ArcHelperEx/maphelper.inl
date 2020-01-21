/**
* @file MapHelper.inl
*
* @brief IMapに関連する処理を行うCMapHelperのインライン定義ファイル
*/
//////////////////////////////////////////////////////////////////////
// ワークスペースに関する関数
//////////////////////////////////////////////////////////////////////
// ワークスペースを取得
inline BOOL CMapHelper::GetWorkspace( IUnknownPtr piUnk, IWorkspace** ppiWorkspace)
{
	if( ( ! ppiWorkspace ) || ( ! piUnk ) )
		return FALSE;

	IWorkspacePtr ipWork( AheGetWorkspace( piUnk ) );
	*ppiWorkspace = ipWork;
	if( *ppiWorkspace )
		(*ppiWorkspace)->AddRef();

	return ( *ppiWorkspace ) ? TRUE : FALSE;
}
//////////////////////////////////////////////////////////////////////
// テーブル検索に関する関数
//////////////////////////////////////////////////////////////////////
// ITableインスタンスを取得
inline BOOL CMapHelper::GetTableByTableName( LPCTSTR lpcszTableName, ITable** ppiTable )
{
	return AheGetTableByTableName( m_piMap, lpcszTableName, ppiTable);
}

// IField インスタンスを取得
inline BOOL CMapHelper::GetFieldByFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, IField** ppiField )
{
	return AheGetFieldByFieldName( m_piMap, lpcszTableName, lpcszFieldName, ppiField);
}

// IField インスタンスを取得
inline BOOL CMapHelper::GetFieldByFieldName( ITablePtr piTable, LPCTSTR lpcszFieldName, IField** ppiRetField )
{
	return AheGetFieldByFieldName( piTable, lpcszFieldName, ppiRetField);
}

// IDomain インスタンスを取得
inline BOOL CMapHelper::GetDomainByFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, IDomain** ppiDomain )
{
	return AheGetDomainByFieldName( m_piMap, lpcszTableName, lpcszFieldName, ppiDomain);
}

//////////////////////////////////////////////////////////////////////
// レイヤ取得に関する関数
//////////////////////////////////////////////////////////////////////
inline BOOL CMapHelper::GetLayerByTableName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetLayerByTableName( m_piMap, lpcszLayerName, ppiRetLayer, bEditableOnly );
}

inline ILayerPtr CMapHelper::GetLayerByTableName( LPCTSTR lpcszLayerName, BOOL bEditableOnly/* = FALSE*/ )
{
	ILayerPtr ipLayer;
	AheGetLayerByTableName( m_piMap, lpcszLayerName, &ipLayer, bEditableOnly );
	return ipLayer;
}

inline BOOL CMapHelper::GetLayerByAliasName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer )
{
	return AheGetLayerByAliasName( m_piMap, lpcszLayerName, ppiRetLayer);
}

inline ILayerPtr CMapHelper::GetLayerByAliasName( LPCTSTR lpcszLayerName )
{
	ILayerPtr ipLayer;
	AheGetLayerByAliasName( m_piMap, lpcszLayerName, &ipLayer);
	return ipLayer;
}

inline BOOL CMapHelper::GetLayerByMXDName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer )
{
	return AheGetLayerByMXDName( m_piMap, lpcszLayerName, ppiRetLayer);
}

inline ILayerPtr CMapHelper::GetLayerByMXDName( LPCTSTR lpcszLayerName )
{
	ILayerPtr ipLayer;
	AheGetLayerByMXDName( m_piMap, lpcszLayerName, &ipLayer);
	return ipLayer;
}

inline BOOL CMapHelper::GetLayerByName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer, FINDTYPE type)
{
	return AheGetLayerByName( m_piMap, lpcszLayerName, ppiRetLayer, type);
}

inline ILayerPtr CMapHelper::GetLayerByName( LPCTSTR lpcszLayerName, FINDTYPE type )
{
	ILayerPtr ipLayer;
	AheGetLayerByName( m_piMap, lpcszLayerName, &ipLayer, type);
	return ipLayer;
}

inline BOOL CMapHelper::GetLayerFromFile( LPCTSTR lpcszLayerFile, ILayer** ppiRetLayer )
{
	return AheGetLayerFromFile( lpcszLayerFile, ppiRetLayer);
}

inline ILayerPtr CMapHelper::GetLayerFromFile( LPCTSTR lpcszLayerName )
{
	ILayerPtr ipLayer;
	AheGetLayerFromFile( lpcszLayerName, &ipLayer );
	return ipLayer;
}
//////////////////////////////////////////////////////////////////////
// フィーチャクラス取得に関する関数
//////////////////////////////////////////////////////////////////////
inline BOOL CMapHelper::GetFeatureClass( IUnknownPtr piUnk, IFeatureClass** ppiFeatureClass)
{
	return AheGetFeatureClass( m_piMap, piUnk, ppiFeatureClass);
}

/// レイヤが表示可能なスケールかどうかの判定
inline BOOL CMapHelper::IsVisibleScale( IGeoFeatureLayerPtr pGeoFeatureLayer)
{
	return AheIsVisibleScale( m_piMap, pGeoFeatureLayer);
}


//////////////////////////////////////////////////////////////////////
// レイヤ名取得に関する関数
//////////////////////////////////////////////////////////////////////

//!< レイヤのテーブル名を取得（type = 0）
inline BOOL CMapHelper::GetLayerTableName( IUnknownPtr piUnk, BSTR* pbstrLayerName)
{
	return AheGetLayerTableName( m_piMap, piUnk, pbstrLayerName);
}

//!< レイヤのエイリアス名を取得（type = 1）
inline BOOL CMapHelper::GetLayerAliasName( IUnknownPtr piUnk, BSTR* pbstrLayerName)
{
	return AheGetLayerAliasName( m_piMap, piUnk, pbstrLayerName);
}

//!< *.mxdファイルに保存されているレイヤ名を取得（type = 2）
inline BOOL CMapHelper::GetLayerMXDName( IUnknownPtr piUnk, BSTR* pbstrLayerName)
{
	return AheGetLayerMXDName( m_piMap, piUnk, pbstrLayerName);
}

//!< 上記3つの関数をtypeで切り分けて実行
inline BOOL CMapHelper::GetLayerName( IUnknownPtr piUnk, BSTR* pbstrName, FINDTYPE type)
{
	return AheGetLayerName( m_piMap, piUnk, pbstrName, type);
}

//!< 上記3つの関数をtypeで切り分けて実行
inline BOOL CMapHelper::GetLayerName( IUnknownPtr piUnk, LPCTSTR& lpszName, FINDTYPE type)
{
	return AheGetLayerName( m_piMap, piUnk, lpszName, type);
}

//!< テーブル名を取得するための IDataset オブジェクトを取得する
inline BOOL CMapHelper::GetDatasetForTableName( IUnknownPtr piUnk, IDataset** ppiDataset)
{
	return AheGetDatasetForTableName( m_piMap, piUnk, ppiDataset);
}

//////////////////////////////////////////////////////////////////////
// フィーチャ選択に関する関数
//////////////////////////////////////////////////////////////////////
// 指定のエリア内にあるフィーチャーを選択する
inline LONG CMapHelper::SelectByShape( IGeometryPtr piGeom, CTargetItems& cTargetItems, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bVisibleOnly/* = FALSE*/, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheSelectByShape( m_piMap, piGeom, cTargetItems, spatialRel, lpcszSubFields, lpcszWhereClause, bVisibleOnly, bEditableOnly );
}

// 指定のエリア内にあるフィーチャーを選択する
inline LONG CMapHelper::SelectByShapeFromLayer( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/ )
{
	return AheSelectByShapeFromLayer( ipGeometry, cTargetItems, ipLayer, spatialRel, lpcszSubFields, lpcszWhereClause );
}

// 指定のエリア内にあるフィーチャーを選択する
inline LONG CMapHelper::SelectByShapeFromFeatureClass( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/ )
{
	return AheSelectByShapeFromFeatureClass( ipGeometry, cTargetItems, ipFeatureClass, spatialRel, lpcszSubFields, lpcszWhereClause );
}

// 指定のエリア内にあるフィーチャを選択する
inline LONG CMapHelper::SelectByShapeFromLayers( IGeometryPtr ipGeom, CTargetItems& cTargetItems, LPCTSTR lpcszLayersName, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheSelectByShapeFromLayers( m_piMap, ipGeom, cTargetItems, lpcszLayersName, spatialRel, lpcszSubFields, lpcszWhereClause, bEditableOnly );
}

// 渡されたフィーチャ（ノード限定）に接続されている道路をコレクションに入れて返す
inline LONG CMapHelper::GetConnectedRoad( IFeaturePtr piFeature, CTargetItems& cTargetItems)
{
	return AheGetConnectedRoad( piFeature, cTargetItems);
}

// フィーチャの構成点上に別のオブジェクトの構成点が存在するか調べる
inline LONG CMapHelper::CheckVertexPoint( double dTol, IFeaturePtr piFeature, IFeaturePtr piOwnFeature, BOOL bDeleteEndNode, IFeature** ppiEndNode, LPCTSTR lpcszLayerNames)
{
	return AheCheckVertexPoint( m_piMap, dTol, piFeature, piOwnFeature, bDeleteEndNode, ppiEndNode, lpcszLayerNames);
}

/// ノードレイヤを取得する
inline ILayerPtr CMapHelper::GetNodeLayer( BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetNodeLayer( m_piMap, bEditableOnly );
}

/// 道路レイヤを取得する
inline ILayerPtr CMapHelper::GetRoadLayer( BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetRoadLayer( m_piMap, bEditableOnly );
}

/// ノードクラスを取得する
inline IFeatureClassPtr CMapHelper::GetNodeClass( BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetNodeClass( m_piMap, bEditableOnly );
}

/// 道路クラスを取得する
inline IFeatureClassPtr CMapHelper::GetRoadClass( BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetRoadClass( m_piMap, bEditableOnly );
}
