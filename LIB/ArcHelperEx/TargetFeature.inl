///inline BOOL CTargetItem::Attach( IFeaturePtr ipFeature, LONG lNewObjectID/* = -1*/ )
inline BOOL CTargetItem::Attach( IFeaturePtr ipFeature )
{
	m_ipFeature = ipFeature;
///	m_lNewObjectID = lNewObjectID;

	return TRUE;
}

inline CTargetItem& CTargetItem::operator=( const CTargetItem& cTargetItem )
{
	USES_CONVERSION;

	Attach( cTargetItem.m_ipFeature );

///	// 新しく作成されたフィーチャかどうかチェック
///	CComBSTR bstrName;
///	cTargetItem.GetFeatureClass()->get_OIDFieldName( &bstrName );
///	CComVariant vaID = cTargetItem.GetAttribute( OLE2T(bstrName) );
///	_ASSERTE( vaID.vt == VT_I4 );
///	if( vaID.lVal == -1 )	// 新しく作成されたフィーチャなら OID は -1 （CreateFeatureBuffer で作成されたときのみ）
///		m_lNewObjectID = cTargetItem.GetOID();

	return *this;
}

inline bool CTargetItem::operator!=( const CTargetItem& cTargetItem) const
{
	return ( operator==( cTargetItem)) ? FALSE : TRUE;
}

inline IFeaturePtr CTargetItem::GetFeature() const
{
	if( m_ipFeature == NULL )
	{
		_ASSERTE( m_ipFeature != NULL );
		return NULL;
	}

	return m_ipFeature;
}

inline IGeometryPtr CTargetItem::GetShape() const
{
	if( m_ipFeature == NULL )
	{
		_ASSERTE( m_ipFeature != NULL );
		return NULL;
	}

	IGeometryPtr ipGeom;
	m_ipFeature->get_Shape( &ipGeom );
	_ASSERT_EXPR( AheGetSpatialReference(ipGeom), _T("フィーチャの空間参照がNULL") );

	return ipGeom;
}

inline IGeometryPtr CTargetItem::GetShapeCopy() const
{
	if( m_ipFeature == NULL )
	{
		_ASSERTE( m_ipFeature != NULL );
		return NULL;
	}

	IGeometryPtr ipGeom;
	m_ipFeature->get_ShapeCopy( &ipGeom );

	return ipGeom;
}

inline BOOL CTargetItem::SetShape( IGeometryPtr piGeometry)
{
	if( m_ipFeature == NULL || piGeometry == NULL )
	{
		_ASSERTE( m_ipFeature != NULL && piGeometry != NULL );
		return FALSE;
	}

	return AheSetShape( m_ipFeature, piGeometry );
}

inline esriGeometryHitPartType CTargetItem::HitTest( IPointPtr piTestPoint, double dbTolerance, IPoint** piPoint, double* pdbHitDist, LONG* plPartIndex, LONG* plVertIndex, esriGeometryHitPartType testType/* = esriGeometryPartVertex|esriGeometryPartBoundary*/)
{
	return AheTestFeatureGeometryHit( m_ipFeature, piTestPoint, dbTolerance, piPoint, pdbHitDist, plPartIndex, plVertIndex, testType );
}

inline esriGeometryHitPartType CTargetItem::HitTest( IGeometryPtr piGeometry, IPointPtr piTestPoint, double dbTolerance, IPoint** piPoint, double* pdbHitDist, LONG* plPartIndex, LONG* plVertIndex, esriGeometryHitPartType testType/* = esriGeometryPartVertex|esriGeometryPartBoundary*/)
{
	return AheTestGeometryHit( piGeometry, piTestPoint, dbTolerance, piPoint, pdbHitDist, plPartIndex, plVertIndex, testType );
}

// 更新情報の設定
inline BOOL CTargetItem::SetModifyData( sindyUpdateType emUpdateType, LPSYSTEMTIME lpModifyTime)
{
	return AheSetModifyData( m_ipFeature, emUpdateType, lpModifyTime);
}

inline LONG CTargetItem::GetFieldMap( FIELDMAP& cNameMap, FIELDMAP& cAliasMap)
{
	return AheGetFieldMap( m_ipFeature, cNameMap, cAliasMap);
}

// 指定のジオメトリと交差する？
inline BOOL CTargetItem::IsCrosses( CTargetItem& cTargetItem )
{
	return this->IsCrosses( cTargetItem.GetShape() );
}

// 指定のジオメトリを含む？
inline BOOL CTargetItem::IsWithin( CTargetItem& cTargetItem )
{
	return this->IsWithin( cTargetItem.GetShape() );
}

// 指定のジオメトリに触れている？
inline BOOL CTargetItem::IsTouches( CTargetItem& cTargetItem )
{
	return this->IsTouches( cTargetItem.GetShape() );
}

// 指定ジオメトリに一致しない
inline BOOL CTargetItem::IsDisjoint( CTargetItem& cTargetItem )
{
	return this->IsDisjoint( cTargetItem.GetShape() );
}

// 指定ジオメトリに等しい
inline BOOL CTargetItem::IsEquals( CTargetItem& cTargetItem )
{
	return this->IsEquals( cTargetItem.GetShape() );
}

// 指定ジオメトリと重なる
inline BOOL CTargetItem::IsOverlaps( CTargetItem& cTargetItem )
{
	return this->IsOverlaps( cTargetItem.GetShape() );
}

// 指定ジオメトリを含む？
inline BOOL CTargetItem::IsContains( CTargetItem& cTargetItem )
{
	return this->IsContains( cTargetItem.GetShape() );
}

/*inline BOOL CTargetItem::CompareLayerName( LPCTSTR lpcszTableName)
{
	return AheCompareLayerName( m_piLayer, lpcszTableName);
}*/

// フィーチャの削除
inline BOOL CTargetItem::Delete()
{
	ATLASSERT( NULL != m_ipFeature);
	return SUCCEEDED( m_ipFeature->Delete());
}

inline BOOL CTargetItem::IsFromPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	return ::AheIsFromPoint( ipPointGeom, m_ipFeature, ipSpRef );
}

inline BOOL CTargetItem::IsToPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	return ::AheIsToPoint( ipPointGeom, m_ipFeature, ipSpRef );
}

inline BOOL CTargetItem::IsEndPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	return ::AheIsEndPoint( ipPointGeom, m_ipFeature, ipSpRef );
}

inline BOOL CTargetItem::Difference( CTargetItem& cTargetItem, IGeometry** ppiGeometry)
{
	return this->Difference( cTargetItem.GetShapeCopy(), ppiGeometry);
}

inline BOOL CTargetItem::Intersect( CTargetItem& cTargetItem, esriGeometryDimension resultDimension, IGeometry** ppiGeometry)
{
	return this->Intersect( cTargetItem.GetShapeCopy(), resultDimension, ppiGeometry);
}

inline BOOL CTargetItem::Union( CTargetItem& cTargetItem, IGeometry** ppiGeometry)
{
	return this->Union( cTargetItem.GetShapeCopy(), ppiGeometry);
}

inline CString CTargetItem::GetErrMessage()
{
	return m_strErrMessage;
}

inline void CTargetItem::RemoveErrMessage()
{
	m_strErrMessage = _T("");
}

inline IFeatureClassPtr CTargetItem::GetFeatureClass() const
{
	return AheGetFeatureClass( m_ipFeature );
}

inline CString CTargetItem::GetFeatureClassName() const
{
	return AheGetFeatureClassName( m_ipFeature );
}

inline CString CTargetItem::GetFeatureClassOwnerName() const
{
	return AheGetFeatureClassOwnerName( m_ipFeature );
}

inline CString CTargetItem::GetOwnerDotClassName() const
{
	return AheGetOwnerDotClassName( m_ipFeature );
}


inline BOOL CTargetItem::SymmetricDifference( IGeometry** ppiGeometry)
{
	IGeometryPtr piGeometry;

	m_ipFeature->get_ShapeCopy( &piGeometry);
	return this->SymmetricDifference( piGeometry, ppiGeometry);
}

inline BOOL CTargetItem::GetPolyline( IPolyline** ppiPolyline)
{
	IGeometryPtr piGeometry;

	m_ipFeature->get_ShapeCopy( &piGeometry);
	return this->GetPolyline( piGeometry, ppiPolyline);
}

inline sindyeFeatureKinds CTargetItem::GetFeatureKinds() const
{
	return AheGetFeatureKinds( m_ipFeature );
}

inline CComVariant CTargetItem::GetAttribute( LPCTSTR lpcszFieldName) const
{
	return AheGetAttribute( m_ipFeature, lpcszFieldName );
}

inline BOOL CTargetItem::SetAttribute( LPCTSTR lpcszFieldName, CComVariant vaValue)
{
	return this->SetAttribute( m_ipFeature, lpcszFieldName, vaValue);
}

inline void CTargetItem::DrawFeatureXOR( IScreenDisplayPtr ipScreen, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop)
{
	this->DrawFeatureXOR( m_ipFeature, ipScreen, dbBufferSize, bDrawVertex, bDoStartStop);
}

inline LONG CTargetItem::HitTestVertexIndex( IPointPtr ipPoint, double dTorelance)
{
	return this->HitTestVertexIndex( m_ipFeature, ipPoint, dTorelance );
}

inline BOOL CTargetItem::IsEditableItem( sindyeEditType type ) const
{
	return IsEditableItem( m_ipFeature, type );
}

inline IPointPtr CTargetItem::GetPoint( LONG lIndex )
{
	return AheGetPoint( m_ipFeature, lIndex );
}
inline IPointCollectionPtr CTargetItem::GetPointCollection()
{
	IPointCollectionPtr ipPoints;
	AheGeometry2PointCollection( GetShapeCopy(), &ipPoints );
	return ipPoints;
}
inline IPolylinePtr CTargetItem::GetPolyline()
{
	IGeometryPtr piGeometry;

	m_ipFeature->get_ShapeCopy( &piGeometry);
	return this->GetPolyline( piGeometry);
}

inline void CTargetItem::GetAttribute( LPCTSTR lpcszFieldName, CComVariant& vaRetValue) const
{
	vaRetValue = GetAttribute( m_ipFeature, lpcszFieldName);
	return;
}
/*inline BOOL CTargetItem::CopyAttribute( const CTargetItem& cNewItem )
{
	return this->CopyAttribute( m_ipFeature, cNewItem );
}*/

inline BOOL CTargetItem::CopyAttribute(IFeaturePtr ipNewFeature)
{
	return this->CopyAttribute( m_ipFeature, ipNewFeature );
}

inline ISpatialReferencePtr CTargetItem::GetSpatialReference() const
{
	return AheGetSpatialReference( m_ipFeature );
}
inline IPointCollectionPtr CTargetItem::RemovePoint( LONG lIndex )
{
	return AheRemovePoint( m_ipFeature, lIndex );
}
inline BOOL CTargetItem::Simplify()
{
	return AheForceSimplify( GetShape() );
}