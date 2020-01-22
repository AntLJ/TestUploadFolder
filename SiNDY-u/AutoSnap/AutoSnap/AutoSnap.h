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

#include <string>
#include <vector>

#include "Config.h"
#include "FileTable.h"

/**
 * @brief Main class of auto snap feature
 */
class CAutoSnap
{
	/**
	 * @brief Process result
	 */
	enum Result : int
	{
		SystemErr = -1,
		NoErr = 0,
		LogicErr = 1
	};

	/**
	 * @brief Layer information
	 */
	struct LayerInfo
	{
		IFeatureClassPtr feature; ///< Feature class pointer
		std::wstring filter; ///< SQL filter
		double length; ///< mini length value
	};

	/**
	 * @brief One snap record with extend snap layer information
	 */
	struct SnapLayerInfo : public LayerInfo
	{
		std::vector<LayerInfo> ext; ///< Extend layer
	};

	/**
	 * @brief Result of a feature
	 */
	struct SnapResult
	{
		IFeaturePtr ipFeature; ///< Feature pointer
		bool isIntersect; ///< Whether be intersected
	};

	/**
	 * @brief Result of a layer
	 */
	struct LayerResults
	{
		double length; ///< mini-length information
		std::vector<SnapResult> result; ///< Result of all features in current layer
	};

	/**
	 * @brief Result of all layers
	 * @details By the specification, 
	 * Intersection points between the line and the all specified layers must be the same Lon/Lat,
	 * Otherwise should report an error.
	 * So here just recording one point is enough.
	 */
	struct AllResults
	{
		std::vector<LayerResults> layers; ///< All results
		IPointPtr ipPoint; ///< The intersection point
	};

	/**
	 * @brief Mesh information
	 */
	struct MeshInfo
	{
		IGeometryPtr ipGeo;
		std::wstring str;
	};

public:
	/**
	 * @brief		Construct
	 * @param[in]	cfg  configure
	 */
	CAutoSnap(const AutoSnapConfig& cfg)
		: m_cfg(cfg){};

	/**
	 * @brief		Destruct
	 */
	~CAutoSnap(){};

	/**
	 * @brief		Run 
	 * @retval		true Succeed 
	 * @retval		false failed
	 */
	bool execute();

private:
	/**
	 * @brief		Single file or sde
	 * @retval		true Succeed 
	 * @retval		false failed
	 */
	bool ExecuteSingle();
	
	/**
	 * @brief		Multi files
	 * @retval		true Succeed 
	 * @retval		false failed
	 */
	bool ExecuteMultiFile();

	/**
	 * @brief		Multi files and multi logs
	 * @retval		true Succeed 
	 * @retval		false failed
	 */
	bool ExecuteMultiLog();

	/**
	 * @brief Process a record with oid
	 *
	 * @param[in] ipWorkspace Workspace object
	 * @param[in] ipFeatures Featuer class object
	 * @param[in] oid OID
	 * @param[in] lon x
	 * @param[in] lat y
	 * @param[in] ipMesh Mesh Object
	 * @return Succeed or not
	 */
	bool Process(const IWorkspaceEditPtr& ipWorkspace, const IFeatureClassPtr& ipFeatures, const std::wstring& oid, const std::wstring& lon, const std::wstring& lat, const IGeometryPtr& ipMesh);

	/**
	 * @brief Process all layers
	 *
	 * @param[in] ipTopo Line
	 * @param[in] ipEnvelope Envelope
	 * @param[out] result Result
	 * @retval -1 System error
	 * @retval 0 Normal
	 * @retval 1 Multiple intersection points
	 */
	int AllIntersect(const ITopologicalOperatorPtr& ipTopo, const IEnvelopePtr& ipEnvelope, AllResults& result);

	/**
	 * @brief Process a layer
	 *
	 * @param[in] ipTopo Line
	 * @param[in] ipEnvelope Envelope
	 * @param[in,out] result Result
	 * @param[out] ipPoint The intersection point
	 * @retval -1 System error
	 * @retval 0 Normal
	 * @retval 1 Multiple intersection points
	 */
	int LayerIntersect(const ITopologicalOperatorPtr& ipTopo, const IEnvelopePtr& ipEnvelope, LayerResults& result, IPointPtr& ipPoint);

	/**
	 * @brief Process a feature
	 *
	 * @param[in] ipTopo Line
	 * @param[in] ipEnvelope Envelope
	 * @param[in] ipFeature Feature
	 * @param[out] ipPoint The intersection point
	 * @retval -1 System error
	 * @retval 0 Normal
	 * @retval 1 Multiple intersection points
	 */
	int Intersect(const ITopologicalOperatorPtr& ipTopo, const IEnvelopePtr& ipEnvelope, const IFeaturePtr& ipFeature, IPointPtr& ipPoint);

	/**
	 * @brief Get the features with envelope and filter
	 *
	 * @param[in] ipClass Feature Class
	 * @param[in] strFilter Filter
	 * @param[in] ipEnv Envelope
	 * @param[out] result Features of result
	 * @return Succeed or not
	 */
	bool GetSnapFeature(const IFeatureClassPtr& ipClass, std::wstring& strFilter, const IEnvelopePtr& ipEnv, LayerResults& result);

	/**
	 * @brief Insert the intersection point to the main feature
	 *
	 * @param[in] result The layers and features
	 * @retval 0 normal
	 * @retval -1 System error
	 * @retval 1 Short segment
	 */
	int InsertMainLayer(const AllResults& result);

	/**
	 * @brief Insert the intersection point to the extended feature
	 *
	 * @param[in] result The layers and features
	 * @param[in] ipEnvelope Envelope
	 * @return Succeed or not.
	 */
	bool InsertExtLayer(const AllResults& result, const IEnvelopePtr& ipEnvelope);

	/**
	 * @brief The point in the mesh which is provided by mesh list
	 *
	 * @param[in] ipPoint The layers and features
	 * @param[out] n Index number of mesh object
	 * @return In or not
	 */
	bool IsInMesh(const IPointPtr& ipPoint, size_t& n);

	/**
	 * @brief The point in the mesh which is provided by mesh list
	 *
	 * @param[in] ipPoint The layers and features
	 * @param[in] ipGeo Geometry object
	 * @return In or not
	 */
	bool CAutoSnap::IsInMesh(const IPointPtr& ipPoint, const IGeometryPtr& ipGeo);

	/**
	 * @brief The point in on the mesh border
	 *
	 * @param[in] ipPoint The layers and features
	 * @param[in] ipMesh Mesh Object
	 * @return On or not
	 */
	bool IsOnMeshBorder(const IPointPtr& ipPoint, const IGeometryPtr& ipMesh);

	/**
	 * @brief Check special case
	 * @details When there are only two points on the line and
	 *  All of them are in the range.
	 *
	 * @param[in] ipPointCollection The geometry point collection
	 * @param[in] ipEnvelope Envelope
	 * @return Is or not
	 */
	bool IsTooShort(const IPointCollectionPtr& ipPointCollection, const IEnvelopePtr& ipEnvelope);

	/**
	 * @brief Initialize log file
	 *
	 * @param[in] strname Log filename
	 * @return Succeed or not
	 */
	bool InitLogFile(const std::wstring& strname);

	/**
	 * @brief Initialize log file
	 *
	 * @param[in] OID OID
	 * @param[in] ipPoint Point
	 * @param[in] errMsg Message
	 * @return Succeed or not
	 */
	bool Log(const wchar_t* OID, const IPointPtr& ipPoint, const wchar_t* errMsg);

	/**
	 * @brief Initialize log file
	 *
	 * @param[in] OID OID
	 * @param[in] strLon x
	 * @param[in] strLat y
	 * @param[in] errMsg Message
	 * @return Succeed or not
	 */
	bool Log(const wchar_t* OID, const wchar_t* strLon, const wchar_t* strLat, const wchar_t* errMsg);

	/**
	 * @brief Initialize log file
	 *
	 * @param[in] OID OID
	 * @param[in] ipPoint Point
	 * @param[in] nMsg RC id
	 * @return Succeed or not
	 */
	bool Log(const wchar_t* OID, const IPointPtr& ipPoint, unsigned int nMsg);

	/**
	 * @brief Initialize log file
	 *
	 * @param[in] OID OID
	 * @param[in] strLon x
	 * @param[in] strLat y
	 * @param[in] nMsg RC id
	 * @return Succeed or not
	 */
	bool Log(const wchar_t* OID, const wchar_t* strLon, const wchar_t* strLat, unsigned int nMsg);

	/**
	 * @brief Add a point to the feature
	 *
	 * @param[in] ipFeature The feature 
	 * @param[in] ipPoint The point
	 * @param[in] length Short length
	 * @param[in] isBreak Whether break when the inserted point cause short length
	 * @retval 0 normal
	 * @retval 1 Short segment
	 * @retval -1 System error
	 */
	int InsertPoint(const IFeaturePtr& ipFeature, const IPointPtr& ipPoint, double length, bool isBreak);

	/**
	 * @brief Is the point on the line
	 *
	 * @param[in] ipPoint The checked point
	 * @param[in] ipFrom From point of a line
	 * @param[in] ipTo To point of a line
	 * @return Is or not
	 */
	bool IsWithIn(const IPointPtr& ipPoint, const IPointPtr& ipFrom, const IPointPtr& ipTo);

	/**
	 * @brief Is short segment
	 *
	 * @param[in] ipPointA Point a
	 * @param[in] ipPointB Point b
	 * @param[in] length Length of short distance
	 * @return Is or not
	 */
	bool IsShort(const IPointPtr& ipPointA, const IPointPtr& ipPointB, double length);

	/**
	 * @brief Calculate the length between two points
	 *
	 * @param[in] ipPointA Point a
	 * @param[in] ipPointB Point b
	 * @param[out] length Length
	 * @return Cucceed or not
	 */
	bool GetLength(const IPointPtr& ipPointA, const IPointPtr& ipPointB, double& length);

	/**
	 * @brief Get the endpoint and the extend line by Lon/Lat
	 *
	 * @param[in] ipPointCollection Point collection
	 * @param[in] lon Lon
	 * @param[in] lat Lat
	 * @param[out] nIndex Number of the matched end point
	 * @param[out] ipPoint Endpoint object
	 * @param[out] ipExt Extend line
	 * @return Succeed or not
	 */
	bool GetPoint(const IPointCollectionPtr& ipPointCollection, const std::wstring& lon, const std::wstring& lat, long& nIndex, IPointPtr& ipPoint, IPolylinePtr& ipExt);

	/**
	 * @brief Get feature by id
	 *
	 * @param[in] ipFeatures Feature ipFeatures
	 * @param[in] oid Object ID
	 * @return Feature
	 */
	IFeaturePtr GetFeature(const IFeatureClassPtr& ipFeatures, const std::wstring& oid);

	/**
	 * @brief Create Envelope
	 *
	 * @param[in] ipPoint Center point
	 * @param[in] radius Radius 
	 * @return Envelope
	 */
	IEnvelopePtr CreateEnvelope(const IPointPtr& ipPoint, double radius);

	/**
	 * @brief		Initialize
	 * @retval		true Succeed
	 * @retval		false Failed
	 */
	bool init();

	/**
	 * @brief Read all mesh object
	 *
	 * @return Succeed or not
	 */
	bool ReadMeshList();

	/**
	 * @brief Read snap layer configure file
	 * @param[in] ipWorkspace Workspace object
	 * @param[out] ipFeatures Feature Class object
	 * @return Succeed or not
	 */
	bool ReadSnapLayerFile(const IFeatureWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatures);

	/**
	 * @brief Open snap records file
	 *
	 * @param[in] strName Filename
	 * @param[out] ft Filetable
	 * @return succeed or not
	 */
	bool OpenSnapRecordsFile(const std::wstring& strName, CFileTable& ft);

	/**
	 * @brief Get min-length by name
	 *
	 * @param[in] str name
	 * @param[out] value value
	 * @return succeed or not
	 */
	bool GetMiniLength(const std::wstring& str, double& value);

	/**
	 * @brief Whether the point equals to lon/lat
	 *
	 * @param[in] ipPoint IPoint
	 * @param[in] lon Lon string value
	 * @param[in] lat Lat string value
	 * @return equals or not
	 */
	bool Equal(const IPointPtr& ipPoint, const std::wstring& lon, const std::wstring& lat);

	/**
	 * @brief Lon/Lat of A is same as B
	 *
	 * @param[in] ipPointA IPoint
	 * @param[in] ipPointB IPoint
	 * @return equals or not
	 */
	bool Equal(const IPointPtr& ipPointA, const IPointPtr& ipPointB);

	/**
	 * @brief Get the FGDB filename via mesh code
	 *
	 * @param[in] str Mesh code
	 * @return filename
	 */
	std::wstring GetFilenameByMesh(const std::wstring& str);

	/** 
	 * @brief Get the log filename via mesh code
	 *
	 * @param[in] str Mesh code
	 * @return filename
	 */
	std::wstring GetLogNameByMesh(const std::wstring& str);

	/** 
	 * @brief Get the snap log filename via mesh code
	 *
	 * @param[in] str Mesh code
	 * @return filename
	 */
	std::wstring GetSnapLogNameByMesh(const std::wstring& str);

	/**
	 * @brief Save workspace
	 *
	 * @param[in] ipEdit Workspace object
	 * @return Succeed or not
	 */
	bool SaveEdit(const IWorkspaceEditPtr& ipEdit);

	/**
	 * @brief Get mesh object
	 *
	 * @param[in] strMesh Mesh number
	 * @param[in] ipClass Mesh class object
	 * @return Mesh object geometry object
	 */
	IGeometryPtr GetMeshObject(const std::wstring& strMesh, const IFeatureClassPtr& ipClass);

private:
	AutoSnapConfig m_cfg; ///< Configure
	std::wstring m_strSnapName; ///< Snap class name
	std::vector<SnapLayerInfo> m_layers; ///< Being snapped Layers
	CFileTable m_ftLog; ///< Log file object
	std::vector<MeshInfo> m_meshList; ///< Mesh object
};
