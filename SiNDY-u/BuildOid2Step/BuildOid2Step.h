#pragma once

#include "stdafx.h"
#include <TDC/sindylib_core/FieldMap.h>
#include "Arguments.h"
#include "Logger.h"

// RTree�̃��������[�N����̂��߁Aboost 1.56���g�p
// �؂�ւ��ăr���h���邱��
#include <boost/version.hpp>

#if BOOST_VERSION != 105600
#error "RTree�̃��������[�N�΍�̂��߁Aboost 1.56�Ńr���h���ĉ������B"
#endif

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/next_prior.hpp>

#pragma warning(disable:4503)

typedef double coordinateType;
static const size_t dimension = 2;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<coordinateType, dimension, bg::cs::cartesian> BGPoint;
typedef bg::model::box<BGPoint> BGBox;
typedef std::pair<BGBox, unsigned> value;
typedef bgi::rtree<value, bgi::quadratic<16>> BGRTree;


typedef std::map<long, IFeaturePtr> FeatureMap;
typedef std::set<unsigned int> MeshList;
typedef std::map<unsigned int, IGeometryPtr> MeshGeoMap;
typedef std::pair<unsigned int, IGeometryPtr> MeshGeo;


/**
 * @brief   ���C���N���X
 */
class CBuildOid2Step
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CBuildOid2Step( const Arguments& args ) : m_args( args ), m_bufDistance(0.0){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CBuildOid2Step(){};

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

	/**
	 * @brief   �����|���S������
	 * @param	ipBufferedMesh	[in] �o�b�t�@�t�^�ς݃��b�V��
	 * @param	rTree			[out] ��ԃL���b�V��
	 * @param	featureMap		[out] �t�B�[�`���}�b�v
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool searchBuild( const IGeometryPtr& ipBufferedMesh, BGRTree& rTree, FeatureMap& featureMap );

	/**
	 * @brief   �K���|���S���X�V
	 * @param	mesh			[in] ���b�V��
	 * @param	ipBufferedMesh	[in] �o�b�t�@�t�^�ς݃��b�V��
	 * @param	rTree			[in] ��ԃL���b�V��
	 * @param	featureMap		[in] �t�B�[�`���}�b�v
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool updateStep( const MeshGeo& mesh, const IGeometryPtr& ipBufferedMesh, const BGRTree& rTree, const FeatureMap& featureMap );

	/**
	 * @brief	���[�N�X�y�[�X�擾
	 * @param	db			[in] �ڑ���DB
	 * @param	ipWorkspace	[in] ���[�N�X�y�[�X
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool openWorkspace( const CString& db, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	�f�t�H���g�o�[�W������
	 * @param	ipWorkspace	[in] ���[�N�X�y�[�X
	 * @retval	true  �f�t�H���g
	 * @retval	false ����ȊO
	 */
	bool isDefaultVersion( const IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	���b�V�����X�g�擾
	 * @param	fileName	[in] �t�@�C��
	 * @param	meshList	[out] ���b�V�����X�g
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool createMeshList( const CString& fileName, MeshList& meshList );

	/**
	 * @brief	�t�B�[�`���N���X�擾
	 * @param	ipWorkspace		[in] ���[�N�X�y�[�X
	 * @param	layerName		[in] ���C����
	 * @param	ipFeatureClass	[out] �t�B�[�`���N���X
	 * @param	fieldMap		[out] �t�B�[���h�}�b�v
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool openFeatureClass( const IWorkspacePtr& ipWorkspace, const CString& layerName, IFeatureClassPtr& ipFeatureClass, sindy::CFieldMap& fieldMap );

	/**
	 * @brief	�o�[�W�����Ή����C����
	 * @param	ipFeatureClass	[in] �t�B�[�`���N���X
	 * @retval	true  �o�[�W�����Ή�
	 * @retval	false �o�[�W������Ή�
	 */
	bool isVersioned( const IFeatureClassPtr& ipFeatureClass );

	/**
	 * @brief	�ҏW�J�n
	 * @param	ipWorkspace	[in/out] ���[�N�X�y�[�X
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool startEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	�ҏW�I��
	 * @param	ipWorkspace	[in/out] ���[�N�X�y�[�X
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool stopEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	�ҏW�j��
	 * @param	ipWorkspace	[in/out] ���[�N�X�y�[�X
	 */
	static void abortEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	���b�V���|���S���擾
	 * @param	meshList	[in] ���b�V�����X�g
	 * @param	meshGeoMap	[out] ���b�V���W�I���g���}�b�v
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool getMeshPolygon( const MeshList& meshList, MeshGeoMap& meshGeoMap );

	/**
	 * @brief	�o�b�t�@�t�^
	 * @param	ipGeometry	[in] �W�I���g��
	 * @param	buf			[in] �o�b�t�@�����i���[�g���j
	 * @return	�o�b�t�@��̌`��
	 */
	IGeometryPtr createBuffer( const IGeometryPtr& ipGeometry, double buf );

	/**
	 * @brief	�{�b�N�X�쐬
	 * @param	ipGeometry	[in] �W�I���g��
	 * @return	�{�b�N�X
	 */
	BGBox getBox( const IGeometryPtr& ipGeometry );

	/**
	 * @brief	�K���ƌ����̋�Ԋ֌W���́iRTree�j
	 * @param	ipGeometry	[in] �W�I���g��
	 * @param	rTree		[in] ��ԃL���b�V��
	 * @param	featureMap	[in] �t�B�[�`���}�b�v
	 * @param	id			[out] ����ID
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool analyzeRefByRTree( const IGeometryPtr& ipGeometry,  const BGRTree& rTree, const FeatureMap& featureMap, long& id );

private:

	Arguments m_args;									//!< ����
	IWorkspacePtr m_ipWorkspace;						//!< ���[�N�X�y�[�X
	IWorkspacePtr m_ipMeshWorkspace;					//!< ���b�V�����[�N�X�y�[�X

	IFeatureClassPtr m_ipBuilding;						//!< �����|���S��
	IFeatureClassPtr m_ipBuildingStep;					//!< �K���|���S��
	IFeatureClassPtr m_ipMeshFc;						//!< 3�����b�V��
	sindy::CFieldMap m_BuildingFieldMap;				//!< �t�B�[���h�}�b�v�i�����|���S���j
	sindy::CFieldMap m_BuildingStepFieldMap;			//!< �t�B�[���h�}�b�v�i�K���|���S���j
	sindy::CFieldMap m_meshFieldMap;					//!< �t�B�[���h�}�b�v�i3�����b�V���j
	ISpatialReferencePtr m_ipSpatialRef;				//!< ��ԎQ��
	MeshList m_meshList;								//!< ���b�V�����X�g
	MeshGeoMap m_meshGeoMap;							//!< ���b�V���W�I���g���}�b�v
	double m_bufDistance;								//!< ���b�V���o�b�t�@�����i���[�g���j
};

