/**
* @file MapHelper.inl
*
* @brief IMap�Ɋ֘A���鏈�����s��CMapHelper�̃C�����C����`�t�@�C��
*/
//////////////////////////////////////////////////////////////////////
// ���[�N�X�y�[�X�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
// ���[�N�X�y�[�X���擾
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
// �e�[�u�������Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
// ITable�C���X�^���X���擾
inline BOOL CMapHelper::GetTableByTableName( LPCTSTR lpcszTableName, ITable** ppiTable )
{
	return AheGetTableByTableName( m_piMap, lpcszTableName, ppiTable);
}

// IField �C���X�^���X���擾
inline BOOL CMapHelper::GetFieldByFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, IField** ppiField )
{
	return AheGetFieldByFieldName( m_piMap, lpcszTableName, lpcszFieldName, ppiField);
}

// IField �C���X�^���X���擾
inline BOOL CMapHelper::GetFieldByFieldName( ITablePtr piTable, LPCTSTR lpcszFieldName, IField** ppiRetField )
{
	return AheGetFieldByFieldName( piTable, lpcszFieldName, ppiRetField);
}

// IDomain �C���X�^���X���擾
inline BOOL CMapHelper::GetDomainByFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, IDomain** ppiDomain )
{
	return AheGetDomainByFieldName( m_piMap, lpcszTableName, lpcszFieldName, ppiDomain);
}

//////////////////////////////////////////////////////////////////////
// ���C���擾�Ɋւ���֐�
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
// �t�B�[�`���N���X�擾�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
inline BOOL CMapHelper::GetFeatureClass( IUnknownPtr piUnk, IFeatureClass** ppiFeatureClass)
{
	return AheGetFeatureClass( m_piMap, piUnk, ppiFeatureClass);
}

/// ���C�����\���\�ȃX�P�[�����ǂ����̔���
inline BOOL CMapHelper::IsVisibleScale( IGeoFeatureLayerPtr pGeoFeatureLayer)
{
	return AheIsVisibleScale( m_piMap, pGeoFeatureLayer);
}


//////////////////////////////////////////////////////////////////////
// ���C�����擾�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////

//!< ���C���̃e�[�u�������擾�itype = 0�j
inline BOOL CMapHelper::GetLayerTableName( IUnknownPtr piUnk, BSTR* pbstrLayerName)
{
	return AheGetLayerTableName( m_piMap, piUnk, pbstrLayerName);
}

//!< ���C���̃G�C���A�X�����擾�itype = 1�j
inline BOOL CMapHelper::GetLayerAliasName( IUnknownPtr piUnk, BSTR* pbstrLayerName)
{
	return AheGetLayerAliasName( m_piMap, piUnk, pbstrLayerName);
}

//!< *.mxd�t�@�C���ɕۑ�����Ă��郌�C�������擾�itype = 2�j
inline BOOL CMapHelper::GetLayerMXDName( IUnknownPtr piUnk, BSTR* pbstrLayerName)
{
	return AheGetLayerMXDName( m_piMap, piUnk, pbstrLayerName);
}

//!< ��L3�̊֐���type�Ő؂蕪���Ď��s
inline BOOL CMapHelper::GetLayerName( IUnknownPtr piUnk, BSTR* pbstrName, FINDTYPE type)
{
	return AheGetLayerName( m_piMap, piUnk, pbstrName, type);
}

//!< ��L3�̊֐���type�Ő؂蕪���Ď��s
inline BOOL CMapHelper::GetLayerName( IUnknownPtr piUnk, LPCTSTR& lpszName, FINDTYPE type)
{
	return AheGetLayerName( m_piMap, piUnk, lpszName, type);
}

//!< �e�[�u�������擾���邽�߂� IDataset �I�u�W�F�N�g���擾����
inline BOOL CMapHelper::GetDatasetForTableName( IUnknownPtr piUnk, IDataset** ppiDataset)
{
	return AheGetDatasetForTableName( m_piMap, piUnk, ppiDataset);
}

//////////////////////////////////////////////////////////////////////
// �t�B�[�`���I���Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
// �w��̃G���A���ɂ���t�B�[�`���[��I������
inline LONG CMapHelper::SelectByShape( IGeometryPtr piGeom, CTargetItems& cTargetItems, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bVisibleOnly/* = FALSE*/, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheSelectByShape( m_piMap, piGeom, cTargetItems, spatialRel, lpcszSubFields, lpcszWhereClause, bVisibleOnly, bEditableOnly );
}

// �w��̃G���A���ɂ���t�B�[�`���[��I������
inline LONG CMapHelper::SelectByShapeFromLayer( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/ )
{
	return AheSelectByShapeFromLayer( ipGeometry, cTargetItems, ipLayer, spatialRel, lpcszSubFields, lpcszWhereClause );
}

// �w��̃G���A���ɂ���t�B�[�`���[��I������
inline LONG CMapHelper::SelectByShapeFromFeatureClass( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/ )
{
	return AheSelectByShapeFromFeatureClass( ipGeometry, cTargetItems, ipFeatureClass, spatialRel, lpcszSubFields, lpcszWhereClause );
}

// �w��̃G���A���ɂ���t�B�[�`����I������
inline LONG CMapHelper::SelectByShapeFromLayers( IGeometryPtr ipGeom, CTargetItems& cTargetItems, LPCTSTR lpcszLayersName, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheSelectByShapeFromLayers( m_piMap, ipGeom, cTargetItems, lpcszLayersName, spatialRel, lpcszSubFields, lpcszWhereClause, bEditableOnly );
}

// �n���ꂽ�t�B�[�`���i�m�[�h����j�ɐڑ�����Ă��铹�H���R���N�V�����ɓ���ĕԂ�
inline LONG CMapHelper::GetConnectedRoad( IFeaturePtr piFeature, CTargetItems& cTargetItems)
{
	return AheGetConnectedRoad( piFeature, cTargetItems);
}

// �t�B�[�`���̍\���_��ɕʂ̃I�u�W�F�N�g�̍\���_�����݂��邩���ׂ�
inline LONG CMapHelper::CheckVertexPoint( double dTol, IFeaturePtr piFeature, IFeaturePtr piOwnFeature, BOOL bDeleteEndNode, IFeature** ppiEndNode, LPCTSTR lpcszLayerNames)
{
	return AheCheckVertexPoint( m_piMap, dTol, piFeature, piOwnFeature, bDeleteEndNode, ppiEndNode, lpcszLayerNames);
}

/// �m�[�h���C�����擾����
inline ILayerPtr CMapHelper::GetNodeLayer( BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetNodeLayer( m_piMap, bEditableOnly );
}

/// ���H���C�����擾����
inline ILayerPtr CMapHelper::GetRoadLayer( BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetRoadLayer( m_piMap, bEditableOnly );
}

/// �m�[�h�N���X���擾����
inline IFeatureClassPtr CMapHelper::GetNodeClass( BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetNodeClass( m_piMap, bEditableOnly );
}

/// ���H�N���X���擾����
inline IFeatureClassPtr CMapHelper::GetRoadClass( BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetRoadClass( m_piMap, bEditableOnly );
}
