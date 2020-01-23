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
	// DiffImportPGDB�̋��e�l�𓥏P
	const double ACCEPTABLE_X  = 0.000001;	///< ���b�V�����E��̍��W���r���鎞�̌덷���e�Ɏg�p�����l�i�o�x�j
	const double ACCEPTABLE_Y  = 0.000001;	///< ���b�V�����E��̍��W���r���鎞�̌덷���e�Ɏg�p�����l�i�ܓx�j
}

void MultiGeometry2Geometries( CRowBase& row, CTableContainer& table ); // PGFGDBConvertProcessor.cpp
void createIndexList( const CFieldMap& srcFields, const CFieldMap& dstFields, const Arguments& args, IndexList& idxs, bool& hasGeom ); // PGFGDBConvertProcessor.cpp

/**
 * @brief testGeom��baseGeom�ɃZ�O�����g��v�Ŋ܂܂�邩�ǂ����m�F����
 *
 * @param baseGeom [in] �܂ނق��̃W�I���g���i�|���S������j
 * @param testGeom [in] �܂܂����̃W�I���g���i�|�����C������j
 * @retval true testGeom��baseGeom�ɂӂ��܂��
 * @retval false testGeom��baseGeom�ɂӂ��܂�Ȃ�
 */
bool isContainAsSegment( IGeometryPtr baseGeom, IGeometryPtr testGeom );

/**
 * @brief testGeom��baseGeom�ɍ\���_��v�Ŋ܂܂�邩�ǂ����m�F����
 *
 * @param baseGeom [in] �܂ނق��̃W�I���g���i�|�����C������j
 * @param testGeom [in] �܂܂����̃W�I���g���i�}���`�|�C���g����j
 * @retval true testGeom��baseGeom�ɂӂ��܂��
 * @retval false testGeom��baseGeom�ɂӂ��܂�Ȃ�
 */
bool isContainAsVertex( IGeometryPtr baseGeom, IGeometryPtr testGeom );

namespace sindy {
	extern bool g_bSetHistory;
}

/**
 * @brief �אڃ}�[�W���s�N���X
 */
template <typename T>
class PGFGDBMergeProcessor : public Processor<T>
{
public:

	/// �R���X�g���N�^
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
		m_InternalName = uh::toTStr( version.GetInternalName().GetString());		//!< �c�[������

		// �ڑ��惁�b�V��DB�e�[�u�����̂��쐬
		if(meshTableOwner.empty())
		{
			m_connectMeshTblName  = meshTable;
		}
		else
		{
			m_connectMeshTblName = meshTableOwner + _T('.') + meshTable;
		}

	};
	/// �f�X�g���N�^
	virtual ~PGFGDBMergeProcessor(){};

	/**
	 * @brief  �������s
	 * @param  src  [in]  ���b�V���R�[�h
	 * @retval true:����
	 */
	virtual bool proc(const T& src);

	// @brief ���[�v�����O�������s
	bool preprocess() override;

	// @brief ���[�v�����㏈�����s
	bool postprocess() override;

	/**
	 * @brief  �����Ώۂ�Workspace��ԋp
	 * @retval �����Ώۂ�Workspace
	 */
	const CWorkspace& getWorkspace(){ return m_outputWorkspace; }

private:

	CEditor m_workspace;				    //!< �o�͐��DB
	CWorkspace  m_meshWorkspace;			//!< ���b�V��DB
	uh::tstring m_meshDbAccessInfo;			//!< ���b�V��DB�ڑ����
	uh::tstring m_meshTable;				//!< �Q�Ɛ惁�b�V���e�[�u��
	uh::tstring m_meshTableOwner;			//!< ���b�V��DB�I�[�i�[
	uh::tstring m_connectMeshTblName;		//!< �ڑ��惁�b�V��DB�e�[�u������
	MergingDefinitionManager m_MergeDefMg;	//!< �}�[�W�Ώے�`�N���X
	uh::tstring m_InternalName;				//!< �v���O������(�X�V�p)
	Arguments m_args;                       //!< �I�v�V��������

	/**
	 * @brief  ���[�N�X�y�[�X���̃e�[�u���̖��̂����ׂĎ擾
	 * @param  workspace    [in]  ���[�N�X�y�[�X
	 * @param  vTableNames  [in]  �e�[�u�����̈ꗗ
	 * @retval true:��������
	 */
	bool getWorkspaceTableNames(const CWorkspace& workspace,std::vector<uh::tstring>& vTableNames);

	/**
	 * @brief  ���b�V���̃|���S�����擾����
	 * @param  meshCode  [in]  ���b�V���R�[�h
	 * @retval ���b�V���̃|���S��
	 */
	IGeometryPtr createMeshPolygon(const uh::tstring& meshCode);

	/**
	 * @brief  �����̃W�I���g���ɐڂ���t�B�[�`�������������ԎQ�ƃt�B���^���쐬���A�ԋp
	 * @param  pGeom  [in]  �W�I���g��
	 * @retval ��ԎQ�ƃt�B���^
	 */
	ISpatialFilterPtr createMeshSpatialFilter(const IGeometryPtr pGeom);

	/**
	 * @brief  �s�v�\���_���폜����(croppingShape���Ŏg�p)
	 * @note   �u�s�v�\���_�v�Ƃ́u���b�V�����E�ɑ��݂���폜���Ă��`��ɕω����Ȃ��\���_�v�ł���
	 * @param  ipSrcGeom  [in]  �폜�Ώۂ̃W�I���g��
	 * @param  meshPolygon  [in]  ���b�V���|���S��
	 * @retval �s�v�\���_���폜�����W�I���g��(�����O�`��)
	 */
	IGeometryPtr croppingRing(const IGeometryPtr ipSrcGeom, const IGeometryPtr meshPolygon);

	/**
	 * @brief  �s�v�\���_���폜����
     * @note   �u�s�v�\���_�v�Ƃ́u���b�V�����E�ɑ��݂���폜���Ă��`��ɕω����Ȃ��\���_�v�ł���
	 * @param  ipSrcGeom  [in]  �폜�Ώۂ̃W�I���g��
	 * @param  meshPolygon  [in]  ���b�V���|���S��
	 * @retval �s�v�\���_���폜�����W�I���g��
	 */
	IGeometryPtr croppingShape(const IGeometryPtr ipSrcGeom, const IGeometryPtr meshPolygon);

	/**
	 * @brief  ���b�V���̍ő�l�A�ŏ��l���擾
	 * @param  ipMeshGeom  [in]  ���b�V���̃W�I���g��
	 * @param  minX  [out]  X�ŏ��l
	 * @param  minY  [out]  Y�ŏ��l
	 * @param  maxX  [out]  X�ő�l
	 * @param  maxY  [out]  Y�ő�l
	 * @retval true:��������
	 */
	bool getMeshXY(IGeometryPtr ipMeshGeom ,double& minX,double& minY,double& maxX,double& maxY);

	/**
	 * @brief  ���b�V�����ŕ��f����Ă���W�I���g�����A�}�[�W���K�v�����肷��
	 * @param  ipSrcGeom  [in]  ��r���̃W�I���g��
	 * @param  ipTgtGeom  [in]  ��r��̃W�I���g��
	 * @param  meshPolygon  [in]  ���b�V���|���S��
	 * @retval true:�}�[�W���K�v
	 */
	bool isNeedMerge(IGeometryPtr ipSrcGeom, IGeometryPtr ipTgtGeom, IGeometryPtr meshPolygon);

	/**
	 * @brief  �|�����C����From�����̍��W���擾
	 * @param  pGeom  [in]  �|�����C���̃W�I���g��
	 * @retval From�����̍��W
	 */
	WKSPoint getFromXY(const IGeometryPtr pGeom);

	/**
	 * @brief  �|�����C����To�����̍��W���擾
	 * @param  pGeom  [in]  �|�����C���̃W�I���g��
	 * @retval To�����̍��W
	 */
	WKSPoint getToXY(const IGeometryPtr pGeom);

	/**
	 * @brief  �|�����C���̃`�F�b�N���s��
	 * @note   �`�F�b�N���ڂ͈ȉ��ʂ�
	 *         �E���b�V�����E��3�{�ȏ�̃|�����C���Ɛڑ����Ă��Ȃ���
	 * @param  con  [in]  TableContainer
	 * @param  baseRow  [in]  ���Ȃ�|�����C��
	 * @param  vOids  [in]  ��r�Ώۂ̃|�����C����OID�Q
	 * @retval true:�`�F�b�NOK�@false:�`�F�b�NNG
	 */
	bool polylineCheck( CTableContainer &con, CGeometryBase &baseRow, std::vector<long> vOids);

	/**
	 * @brief  �}�[�W���ׂ������肵�A�}�[�W�Ώ�OID�̑�������̃��X�g���쐬����
	 * @param  con  [in]  TableContainer(���b�V���ɐڂ��Ă���W�I���g�����L���b�V������Ă������)
	 * @param  mergingDef  [in]  �}�[�W���{��`�N���X
	 * @param  meshPolygon  [in]  ���b�V���|���S��
	 * @retval �}�[�W�Ώ�OID�̑�������̃��X�g
	 */
	std::map<long, std::vector<long>> searchMergeFeature(CTableContainer &con, const MergingDefinition& mergingDef, IGeometryPtr meshPolygon);

	/**
	 * @brief  �}�[�W�Ώۂ�OID�̃��X�g�����j�[�N�ɂ���
	 * @param  vOidMap  [in]	�}�[�W�Ώۂ�OID���X�g key;oid value: vecter <oid>
	 * @retval ���j�[�N�ɂȂ���OID�̃��X�g key:oid value: vecter <oid> ��key ��vecter <oid>�̒��ōł��l������������
	 */
	std::map<long,std::vector<long>> uniqMergeOidList(std::map<long, std::vector<long>>& vOidMap);

	/**
	 * @brief �}�[�W�Ώۂ�OID���X�g����A�}�[�W�ΏۂƂȂ�`�󓯎m���d�Ȃ��Ă���v�f���폜����
	 * @note bug11708�Ή��Œǉ�
	 * @param vOidMap [in, out] �}�[�W�Ώۂ�OID���X�g key;oid value: vecter <oid>
	 * ��key ��vecter <oid>�̒��ōł��l������������
	 * @param con     [in]  TableContainer(���b�V���ɐڂ��Ă���W�I���g�����L���b�V������Ă������)
	 */
	void eraseOverlapFeatureList(std::map<long,std::vector<long>>& vOidMap, CTableContainer& con);

	/**
	 * @brief  �}�[�W���������{����
	 * @param  vUniqOidlMap  [in]  �}�[�W�Ώۂ�OID�̃��X�g key:oid value: vecter <oid> ��key ��vecter <oid>�̒��ōł��l������������
	 * @param  idxList [in] �����R�s�[���̃��[����`
	 * @param  con  [in]  TableContainer(���b�V���ɐڂ��Ă���W�I���g�����L���b�V������Ă������)
	 * @retval �Ȃ�
	 */
	void doMerge( const std::map<long,std::vector<long>>& vUniqOidlMap, const IndexList& idxList, CTableContainer &con);

	/**
	 * @brief �����ƌ`����R�s�[����
	 * @param listIdx [in] �����R�s�[�\�ȃC���f�b�N�X�̑Ή��\�ifirst:�\�[�X�� second:dst���j
	 * @param src [in] �R�s�[��
	 * @param dst [in] �V�K�쐬���ꂽ���R�[�h
	 * @param args [in] �I�v�V�����Ǘ��N���X
	 * @param geom [in] null����Ȃ�����src�̌`����D�悳���dst�ɃZ�b�g����
	 * @return ���������true
	 */
	bool updateRow( const IndexList& listIdx, const CSPRowBase& src, CSPRowBase& dst, const Arguments& args, IGeometryPtr geom );

	/**
	 * @brief  ���ʑ������ڂ�ݒ肷��
	 * @param  row  [in/out]  ������ݒ肷�郌�R�[�h
	 * @retval  true:�ݒ萬��
	 */
	bool setHistory(CRowContainer& row);
};

// �������s
template <typename T>
bool PGFGDBMergeProcessor<T>::proc(const T& src)
{
	uh::tstring mesh(src);

	// ���O
	{
		std::wstringstream ss;
		ss << _T("�אڃ}�[�W�J�n") <<  _T(" �Ώۃ��b�V��:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	// ���b�V����DB�ɐڑ�
	if( !m_meshWorkspace.Connect(m_meshDbAccessInfo.c_str()))
		notifier::critical::asCannotConnectDB(m_meshDbAccessInfo.c_str());

	// �������Q�Ɨp�̒l��Argments�ɐݒ�
	m_args.m_systemValue[Arguments::MESHCODE] = mesh;

	// �ҏW�J�n
	if( !m_workspace.StartOperation() )
		notifier::critical::versatile(_T("�ҏW�J�n�Ɏ��s"));

	// ���b�V����`���擾
	IGeometryPtr meshPolygon = createMeshPolygon(mesh);
	if ( !meshPolygon )
	{
		std::wstringstream ss;
		ss << _T("���b�V���`��̎擾�Ɏ��s���܂��� ���b�V���R�[�h : ") <<  mesh;
		std::cerr << uh::toStr(ss.str()) << std::endl;
		notifier::versatile(ss.str(), std::wcout);
		notifier::versatile(ss.str(), std::wcerr);
		
		// �ҏW�I��
		if( !m_workspace.StopOperation() )
			notifier::critical::versatile(_T("�ҏW�I���Ɏ��s"));

		// false�ɂ��Ă��܂�����}�[�W�����S�̂��I����true�ŕԋp
		return true;
	}

	// �}�[�W��`�t�@�C���ɋL�ڂ��ꂽ�e�[�u�����������[�v
	for(unsigned  i = 0 ; i < m_MergeDefMg.size(); ++i)
	{

		MergingDefinition mergingDef = m_MergeDefMg.get(i);

		// ��Ԍ����t�@�C���^�[���擾
		ISpatialFilterPtr pMeshFilter = createMeshSpatialFilter( meshPolygon );

		// �t�B�[�`���N���X���擾
		IFeatureClassPtr pMergeTableClass = m_workspace.Workspace().OpenSameOwnersTable(mergingDef.m_strTableName.c_str());
		if( !pMergeTableClass )
			continue;

		// ���b�V���ɐڂ���t�B�[�`����S�Ď擾���A�L���b�V������
		CTableFinder finder;
		{
			CModel model;
			model.Add( AheGetFeatureClassName(pMergeTableClass), sindy::schema::kObjectID, NULL, NULL );
			finder.InitCollection( (ITablePtr)pMergeTableClass, model );
		}
		CTableContainer& con = *finder.FindTable(AheGetFeatureClassName(pMergeTableClass));
		con._Select( (IQueryFilterPtr)pMeshFilter, false );
		con.CreateCache();

		// ���b�V���ɐڂ���t�B�[�`�������ׂČ������A�}�[�W�Ώۂ̃t�B�[�`����OID�Ŏ擾����
		std::map<long, std::vector<long>> vOidMap = searchMergeFeature(con, mergingDef, meshPolygon);

		// OID�̃��X�g��uniq�ɂ���
		std::map<long,std::vector<long>> vUniqOidlMap = uniqMergeOidList(vOidMap);

		// uniq��OID�̃��X�g����d�����Ă�����̂��폜
		eraseOverlapFeatureList(vUniqOidlMap, con);

		// �X�V�����邽�߂̋��ʃC���f�b�N�X���X�g���쐬����
		bool hasGeom = false;
		createIndexList( *con.GetFieldMap(), *con.GetFieldMap(), m_args, mergingDef.m_idxList, hasGeom );

		// OID�̃��X�g�����Ƀ}�[�W�������{
		doMerge(vUniqOidlMap, mergingDef.m_idxList, con);

		// �ۑ����{
		CErrorObjectsBase err;
		bool backup = g_bSetHistory;
		g_bSetHistory = m_MergeDefMg.isSetHistory(); // �ۑ����ɗ�����t�^���邩�ǂ���
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

	// �ҏW�I��
	if( !( m_workspace.StopOperation() && m_workspace.SaveEditing() ) )
		notifier::critical::versatile(_T("�ҏW�I���Ɏ��s"));

	// ���O
	{
		std::wstringstream ss;
		ss << _T("�אڃ}�[�W�I��") <<  _T(" �Ώۃ��b�V��:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	return true;
}

// ���[�v�����O�������s
template <typename T>
bool PGFGDBMergeProcessor<T>::preprocess()
{
	// �ڑ��m�F
	if( !m_workspace.Workspace() )
	{
		notifier::critical::versatile(_T("�o�͐��DB�ɐڑ����s"));
		return false;
	}

	// �ҏW�Ώۂ��o�[�W�����Ή�����Ή����ɉ����āA�ҏW���J�n����
	std::list<uh::tstring> listTarget;
	for( unsigned  i = 0 ; i < m_MergeDefMg.size(); ++i )
		listTarget.push_back( m_MergeDefMg.get(i).m_strTableName );

	return m_workspace.StartEditingByObjectType(listTarget); // �G���[���b�Z�[�W��CEditor�ŏo��
}

// ���[�v�����㏈�����s
template <typename T>
bool PGFGDBMergeProcessor<T>::postprocess()
{
	return m_workspace.StopEditing(true); // �G���[���b�Z�[�W��CEditor�ŏo��
}

//�}�[�W�Ώ�OID�̑�������̃��X�g�����j�[�N�ɂ���
template <typename T>
std::map<long,std::vector<long>> PGFGDBMergeProcessor<T>::uniqMergeOidList(std::map<long, std::vector<long>>& vOidMap)
{
	//////////////////////////////////////////////////////////////////////////
	// �������̃}�[�W�Ώ�OID�̑�������̃��X�g�͈ȉ��̂悤�ɂȂ��Ă���
	// key 1 value  { 3     }
	// key 3 value  { 1, 5  }
	// key 5 value  { 3     }
	//
	// ������ȉ��̂悤�ɏC�����A�ԋp����̂��{�֐��̖ړI�ł���
	//
	// key 1 value  { 1, 3, 5 }
	//
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ���ꂼ����}�[�W���ȉ��̂悤�ȏ�Ԃɂ���
	// key 1 value  { 1, 3, 5 }
	// key 3 value  { 1, 3, 5 }
	// key 5 value  { 1, 3, 5 }
	//
	//////////////////////////////////////////////////////////////////////////
	for(auto& mergeOids : vOidMap)
	{
		long targetOid = mergeOids.first;
		std::vector<long>& vMergeList = mergeOids.second;

		//�}�[�W�Ώۂ����Ȃ��ꍇ�̓X�L�b�v
		if(vMergeList.empty())
			continue;

		//�}�[�W�Ώۂ�OID�������X�g���A���g�̃}�[�W�ΏۂƂ��Ēǉ����ׂ�OID�����X�g��
		std::vector<long> vAddOids;
		for(long mergeOid : vMergeList)
		{
			if (vOidMap.find(mergeOid) == vOidMap.end())
				 continue;

			copy(vOidMap[mergeOid].begin(), vOidMap[mergeOid].end(), std::back_inserter(vAddOids) );
		}
		// �}�[�W���肪���g���}�[�W�ΏۂƂ��Ă��Ȃ��\�������邽�߁A
		// �ꉞ�A�������g�����X�g�ɉ����Ă���
		vAddOids.push_back(targetOid);
		std::sort(vAddOids.begin(), vAddOids.end());
		vAddOids.erase(unique(vAddOids.begin(), vAddOids.end()), vAddOids.end());

		//��ō쐬�������X�g���̊eOID�ɑ΂��A�݂����}�[�W�ΏۂƂ��ēo�^������
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
	// Map�̓����ƃ��j�[�N�ňȉ��̂悤�ȏ�Ԃɂ���
	// key 1 value  { 1, 3, 5 }
	//
	//////////////////////////////////////////////////////////////////////////
	std::map<long,std::vector<long>> vUniqOidlMap;
	for(auto& mergeOids : vOidMap)
	{
		std::vector<long>& vec = mergeOids.second;

		// �L�[�����Ȃ����̂�continue
		if(vec.empty())
			continue;

		// �\�[�g���ă��j�[�N
		std::sort(vec.begin(),vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());

		// ���j�[�N�ɂ������̂��i�[
		// �������Ŋ���vec��OID�Ń\�[�g����Ă���͂�
		vUniqOidlMap[vec[0]] = vec;
	}

	return vUniqOidlMap;
}

// �}�[�W���ׂ������肵�A�}�[�W�Ώ�OID�̑�������̃��X�g���쐬����
template <typename T>
std::map<long, std::vector<long>> PGFGDBMergeProcessor<T>::searchMergeFeature( CTableContainer &con, const MergingDefinition& mergingDef, IGeometryPtr meshPolygon )
{
	std::map<long, std::vector<long>> vOidMap;

#ifdef S2S_NEW_MERGE_ALGORITHM
	IGeometryPtr meshBoundary; // ���b�V���g�i�|�����C���j
	ITopologicalOperatorPtr(meshPolygon)->get_Boundary(&meshBoundary);
	ITopologicalOperatorPtr topoMesh(meshBoundary);
#endif // if S2S_NEW_MERGE_ALGORITHM

	for( CContainerBase::const_iterator it1 = con.begin(); it1 != con.end() ; ++it1 )
	{
		// �Ō�̍s�͎����̎�������Q�d���[�v�ׁ̈Acontinue
		if(it1 == con.end())
			continue;
	   
		CSPRowContainer sprow1 = CAST_ROWC(*it1);
		CRowContainer& row1 = *sprow1;
		CGeometryBase& geom1 = *row1.GetShape();

#ifdef S2S_NEW_MERGE_ALGORITHM
		// ���b�V���g�Ƃ�intersect�𐶐�
		IGeometryPtr boundary1;
		ITopologicalOperatorPtr((IGeometry*)geom1)->get_Boundary(&boundary1);
		IGeometryPtr border1;
		// ����̎����ł�Intersect�`����擾����
		esriGeometryDimension dim = ( geom1.GetDimension() == esriGeometry1Dimension ) ? esriGeometry0Dimension : esriGeometry1Dimension;
		topoMesh->Intersect( boundary1, dim, &border1 );
#endif // if S2S_NEW_MERGE_ALGORITHM

		// �L�[�����݂��Ȃ��ꍇ�쐬
#ifdef _DEBUG
		long oid = row1.GetOID();
#endif // ifdef _DEBUG
		if (!vOidMap.count(row1.GetOID()))
			vOidMap[row1.GetOID()] =  std::vector<long>();

		// it1�̎�����擾
		CContainerBase::const_iterator it2 = it1;
		for( ++it2 ; it2 != con.end(); ++it2 )
		{
			CSPRowContainer sprow2 = CAST_ROWC(*it2);
			CRowContainer& row2 = *sprow2;
			CGeometryBase& geom2 = *row2.GetShape();

#ifdef S2S_NEW_MERGE_ALGORITHM
			// border1�Ƃ�intersect�𐶐�
			IGeometryPtr boundary2;
			ITopologicalOperatorPtr((IGeometry*)geom2)->get_Boundary(&boundary2);
			IGeometryPtr border2;
			ITopologicalOperatorPtr(boundary2)->Intersect( border1, dim, &border2 );

			switch( dim )
			{
			case esriGeometry1Dimension:
				// ���������`��̂��ꂼ��̃p�[�g���Ageom1,geom2���Z�O�����g�Ƃ���
				// �ۗL���Ă���΃}�[�W�ΏۂƂ���
				if( !( isContainAsSegment( (IGeometry*)geom1, border2 ) &&
					isContainAsSegment( (IGeometry*)geom2, border2 ) ) )
					continue;
				break;
			case esriGeometry0Dimension:
				// �|�����C���̏ꍇ�́Aborder2�̓|�C���g�ɂȂ�
				// ���������|�C���g��geom1,geom2���\���_�Ƃ���
				// �ۗL���Ă���΃}�[�W�ΏۂƂ���
				if( ! ( isContainAsVertex( (IGeometry*)geom1, border2 ) &&
					isContainAsVertex( (IGeometry*)geom2, border2 ) ) )
					continue;
				break;
			default:
				break;
			}
#else
			// ���b�V�����E��Ő���(�|���S���̏ꍇ)�A�܂��̓|�C���g(���C���̏ꍇ)�����L���Ă��邩
			try {
				if (!isNeedMerge((IGeometry*)geom1,(IGeometry*)geom2,meshPolygon))
					continue;
			}
			catch(...)
			{
				std::wstringstream ss;
				ss << _T("��O�����������׃}�[�W���s���܂���ł���") <<  _T(" ��r��OID: ") << row2.GetOID();
				notifier::reportObjInfo( row1.GetTableName(), row1.GetOID(), uh::tstring(ss.str()), std::wcout );
				continue;
			}
#endif // if S2S_NEW_MERGE_ALGORITHM
			// ��r���Ɣ�r�Ώۂő����͓����łȂ����̂�continue
			if(mergingDef.m_cCondition.exps && !mergingDef.m_cCondition.exps->eval( CAST_ROWB(sprow1), CAST_ROWB(sprow2), m_args, std::list<uh::tstring>() ))
			{
				std::wstringstream ss;
				ss << _T("�����̒l�����l�ł͂Ȃ��׃}�[�W���s���܂���ł���") <<  _T(" ��r��OID: ") << row2.GetOID();
				notifier::reportObjInfo( row1.GetTableName(), row1.GetOID(), uh::tstring(ss.str()), std::wcout );
				continue;
			}

			// �}�[�W�Ώۂł���ꍇ�AOID��ێ�
			vOidMap[row1.GetOID()].push_back(row2.GetOID());
			// �L�[�����݂��Ȃ��ꍇ�쐬
			if (!vOidMap.count(row2.GetOID()))
				vOidMap[row2.GetOID()] =  std::vector<long>();
			// �Ώۂ̕����i�[
			vOidMap[row2.GetOID()].push_back(row1.GetOID());
		}
		// �ڑ����Ă���|�����C����3�ȏ゠���������b�V�����E�Őڂ��郉�C����3�{�ȏ゠�邩����
		if( ( row1.GetShape()->GetGeometryType() == esriGeometryPolyline ) &&
			vOidMap[row1.GetOID()].size() >= 2 &&
			!polylineCheck( con, geom1, vOidMap[row1.GetOID()] ))
		{
			std::wstringstream ss;
			uh::tstring oidlist;
			std::for_each( vOidMap[row1.GetOID()].begin(), vOidMap[row1.GetOID()].end(), [&oidlist](long oid){if (!oidlist.empty()){ oidlist += _T(","); } oidlist += std::to_wstring(oid);});
			ss << _T("���b�V�����E�Őڂ��郉�C����3�{�ȏ゠�邽�߃}�[�W���܂���") <<  _T(" ��r��OID : ") << oidlist;
			notifier::reportObjInfo( row1.GetTableName(), row1.GetOID(), uh::tstring(ss.str()), std::wcout );

			// ���X�g���珜�O
			vOidMap.erase( row1.GetOID() );
		}

	}
	return vOidMap;
}


// �|�����C���̍��W���擾
WKSPoint getWKSPointXY(const IGeometryPtr pGeom , bool directionTo)
{
	WKSPoint wksPoint = {};

	esriGeometryType geomType = esriGeometryNull;
	pGeom->get_GeometryType( &geomType );
	// �|�����C���łȂ��ꍇ�͋�̍��W�����^�[��
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

// �|�����C����From�����̍��W���擾
template <typename T>
WKSPoint PGFGDBMergeProcessor<T>::getFromXY(const IGeometryPtr pGeom)
{
	return getWKSPointXY(pGeom, false);
}

// �|�����C����To�����̍��W���擾
template <typename T>
WKSPoint PGFGDBMergeProcessor<T>::getToXY(const IGeometryPtr pGeom)
{
	return getWKSPointXY(pGeom, true);
}

template <typename T>
bool PGFGDBMergeProcessor<T>::polylineCheck( CTableContainer &con, CGeometryBase &geom, std::vector<long> vOids)
{
	unsigned fromMatchCnt(0),toMatchCnt(0);

	// ��r����From,To�̃|�C���g���擾
	WKSPoint wksSrcFromPoint = getFromXY((IGeometry*)geom);
	WKSPoint wksSrcToPoint =  getToXY((IGeometry*)geom);

	for(const auto& oid : vOids)
	{
		CRowContainer& targetRow = *CAST_ROWC(*con.find_by_key(sindy::schema::kObjectID, oid));
		CGeometryBase& targetGeom =  *targetRow.GetShape();

		WKSPoint wksTgtFromPoint = getFromXY((IGeometry*)targetGeom);
		WKSPoint wksTgtToPoint = getToXY((IGeometry*)targetGeom);

		// ��r��From�Ɣ�r��To,From����v���邩
		if( ( wksSrcFromPoint.X == wksTgtToPoint.X && wksSrcFromPoint.Y == wksTgtToPoint.Y ) ||
			( wksSrcFromPoint.X == wksTgtFromPoint.X && wksSrcFromPoint.Y == wksTgtFromPoint.Y ) )
		{
			++fromMatchCnt;
		}

		// ��r��To�Ɣ�r��To,From����v���邩
		if( ( wksSrcToPoint.X == wksTgtToPoint.X && wksSrcToPoint.Y == wksTgtToPoint.Y ) ||
			( wksSrcToPoint.X == wksTgtFromPoint.X && wksSrcToPoint.Y == wksTgtFromPoint.Y ) )
		{
			++toMatchCnt;
		}
	}

	// ��̃|�����C���ɕ����ڑ�����Ă���
	if ( fromMatchCnt > 1 ||  toMatchCnt > 1)
		return false;

	return true;
}

// �}�[�W�Ώۂ̃��X�g����A�`�󂪏d�Ȃ��Ă�����̂��폜����
template <typename T>
void PGFGDBMergeProcessor<T>::eraseOverlapFeatureList(std::map<long,std::vector<long>>& vOidMap, CTableContainer& con)
{
	// �`��̏d�Ȃ�`�F�b�N�p�̃����_��
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

	// �d�����Ă�����̂��폜����
	for(auto itOidMapPair = vOidMap.begin(); itOidMapPair != vOidMap.end(); )
	{
		auto& oidList = itOidMapPair->second;
		if( checkShapeOverlap(oidList) )
			itOidMapPair = vOidMap.erase(itOidMapPair);
		else
			++itOidMapPair;
	}
}

// �}�[�W�������s
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

			// �������g��continue
			if(mergeOids.first == targetOid)
				continue;

			sindyErrCode err = sindyErr_NoErr;
			IGeometryPtr unionGeom = CGeometryBase::Union( baseGeom, targetGeom, &err );
			if( sindyErr_NoErr != err )
			{
				notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(),notifier::error_level::error, _T("�}�[�W�Ɏ��s"), std::wcout );
				notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(),notifier::error_level::error,  _T("�}�[�W�Ɏ��s"), std::wcerr );
				continue;
			}
			VARIANT_BOOL isEmpty = VARIANT_FALSE;
			unionGeom->get_IsEmpty( &isEmpty );

			if( isEmpty )
			{
				notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(),notifier::error_level::error, _T("�}�[�W������`�󂪏���"), std::wcout );
				notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(),notifier::error_level::error, _T("�}�[�W������`�󂪏���"), std::wcerr );
				continue; // �����͌p������
			}
			// �X�V
			updateRow( idxList, CAST_ROWB(spTargetRow), CAST_ROWB(spBaseRow), m_args, unionGeom );
			// �}�[�W����鑤�͍폜
			targetRow.Delete();
		}
		// �`������ׂă}�[�W���Ă���}���`�p�[�g������
		if( m_args.m_dissolveMulti )
			MultiGeometry2Geometries( *CAST_ROWB(spBaseRow), con );

		// ���O
		{
			uh::tstring oidlist;
			for (unsigned i = 0; i < mergeOids.second.size(); ++i )
			{
				if (i != 0){ oidlist += _T(","); }
				oidlist += std::to_wstring(mergeOids.second[i]) ;
			}
			std::wstringstream ss;
			ss << _T("�}�[�W����") <<  _T(" �}�[�W���{OID: ") << oidlist;
			notifier::reportObjInfo( baseRow.GetTableName(), baseRow.GetOID(), uh::tstring(ss.str()), std::wcout );
		}
	}
}

// �����ƌ`����R�s�[
template <typename T>
bool PGFGDBMergeProcessor<T>::updateRow( const IndexList& listIdx, const CSPRowBase& src, CSPRowBase& dst, const Arguments& args, IGeometryPtr geom )
{
	// TODO: PGFGDBConvertProcessor.cpp��Update�֐��Ƌ��ʂ��Ă��鏈���͈�Z�߂ɂ���

	// �L���b�V���쐬
	// �S�Ă��X�V�ΏۂɂȂ�킯�ł͂Ȃ��̂ŁA�L���b�V���쐬�͍X�V�Ώۂ݂̂Ɍ��肷��
	// ����ɂ��p�t�H�[�}���X�̒ቺ������ꍇ�́A�ꊇ�L���b�V������������i�������v���Ǝv���j
	dst->CreateCache();

	// ����
	for( const auto& idxExps : listIdx )
	{
		for( const auto& idxExp : idxExps.second )
		{
			const IndexPair& idx = idxExp.first;
			const SPConditionExpressions& exps = idxExp.second;
			if( !exps || exps->eval( src, dst, args, std::list<uh::tstring>() ) ) // �����Ȃ���������
			{
				if( IndexPair::None == idx.type || 				        // �R�s�[�Ώۂł͂Ȃ�
					IndexPair::RowValue == idx.type && 0 > idx.srcIdx ) // ���R�[�h�l�Ȃ̂ɃC���f�b�N�X�ԍ����L���ł͂Ȃ�
					continue;

				dst->SetValue( idx.dstIdx, idx.value(*src) );
				break; // �̗p�����l�͈�����i�揟���j
			}
		}
	}
	// �`��
	if( !geom )
		geom = src->GetShapeCopy();

	if( geom )
		dst->SetShape(geom);

	return true;
}

// ���[�N�X�y�[�X�ɑ��݂���e�[�u�����̂̈ꗗ���擾����
template <typename T>
bool PGFGDBMergeProcessor<T>::getWorkspaceTableNames(const CWorkspace& workspace,std::vector<uh::tstring>& vTableNames)
{
	// �f�[�^�Z�b�g�擾
	IEnumDatasetNamePtr ipEnumDataset = workspace.GetDatasetNames(esriDTAny);
	if ( !ipEnumDataset )
	{
		return false;
	}

	// �e�[�u���ꗗ���擾
	TableNamesSearcher searcher;
	searcher.getWorkspaceTableNames(ipEnumDataset, vTableNames);
	return true;
}

// ���b�V���̃|���S�����擾
template <typename T>
IGeometryPtr PGFGDBMergeProcessor<T>::createMeshPolygon(const uh::tstring& meshCode)
{
	// �Ώۓs�s�n�}���b�V���̃|���S�����擾
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

	// �P�������݂��鎖��O��Ƃ���
	IFeaturePtr pMeshFearure;
	pMeshCursor->NextFeature(&pMeshFearure);
	if ( ! pMeshFearure )
	{
		return pMeshFearure;
	}

	// �`��̃R�s�[���擾
	IGeometryPtr pMesh;
	pMeshFearure->get_ShapeCopy(&pMesh);

	return pMesh;
}

// �����Ώۂ̋�Ԍ����N�G���t�B���^���쐬
template <typename T>
ISpatialFilterPtr PGFGDBMergeProcessor<T>::createMeshSpatialFilter(IGeometryPtr pGeom)
{
	// �ԋp�l
	ISpatialFilterPtr ret;

	IGeometryPtr ipBoundary;
	ITopologicalOperatorPtr( pGeom )->get_Boundary(&ipBoundary);
	//�p���̃t�B�[���h�l���擾
	IFeatureClassPtr pMeshClass = m_meshWorkspace.OpenTable(m_meshTable.c_str());
	CComBSTR bstrFieldName;
	pMeshClass->get_ShapeFieldName(&bstrFieldName);

	ret = AheInitSpatialFilter(
			nullptr,
			ipBoundary,
			bstrFieldName,
			esriSearchOrderSpatial,
			esriSpatialRelIntersects); // Touches���ƕʂ̃��b�V���g�Ń}�[�W���ꂽ���̂������ΏۂƂȂ�Ȃ�
	return ret;
}

// ���b�V���|���S�����̓��b�V�����E���C���擾�֐�
// ���E��̌����Ȕ�r�����邽�߂̍ő�ŏ��l���擾
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

// ���e�͈͂������������W�i�l�j�̈�v�m�F�֐�
bool fnCompXY(double cPoint1, double cPoint2, double cAcceptable)
{
	if(fabs(cPoint1 - cPoint2) <= cAcceptable){
		return true;
	}else{
		return false;
	}
}

// �s�v�\���_���폜������̌`�󋁂߂�֐�(�K�v�ɉ����ă|���S���������O�ɕ������čč\������)�B
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

// PointCollection����w�肵���C���f�b�N�X��WKSPoint���擾����
WKSPoint getWKSPointFromPointCollection(IPointCollectionPtr ipPointCollection, int idx)
{
	WKSPoint wksPoint = {};
	IPointPtr pPoint;
	ipPointCollection->get_Point(idx, &pPoint);
	pPoint->QueryCoords( &wksPoint.X , &wksPoint.Y );

	return wksPoint;

}

// �s�v�\���_���폜������̌`������߂�֐��i�����O�P�ʁj�B
template <typename T>
IGeometryPtr PGFGDBMergeProcessor<T>::croppingRing(IGeometryPtr ipSrcGeom, IGeometryPtr meshPolygon)
{
	// ���b�V���̍ő�l�A�ŏ��l���擾
	double aMinX, aMinY, aMaxX, aMaxY;
	getMeshXY(meshPolygon, aMinX, aMinY, aMaxX, aMaxY);

	IGeometryPtr ipResultGeom =  AheCreateClone(ipSrcGeom);
	IPointCollectionPtr ipPointCollection = ipResultGeom;
	long aPointCount = 0;
	ipPointCollection->get_PointCount( &aPointCount );

	// 4�_�͎O�p�`�ň�_�ł��폜�����ƒ����ɂȂ��Ă��܂��B
	if(!(aPointCount > 4))
	{
		return ipResultGeom;
	}

	std::list<long> deleteInex;
	for( int i = 0; i < aPointCount; ++i )
	{
		// ��r����3�_�̃C���f�b�N�X����
		long next = i + 1;
		long nextnext = i + 2;
		// ������̎���A���̎����|�C���g�̍ő�l�𒴂����ꍇ�A�n�_(0)����n�߂�
		next = ( next < aPointCount ? next : next - aPointCount );
		nextnext  = ( nextnext < aPointCount ? nextnext : nextnext - aPointCount );

		// ��̃|�C���g
		WKSPoint p1 = getWKSPointFromPointCollection(ipPointCollection, i);
		WKSPoint p2 = getWKSPointFromPointCollection(ipPointCollection, next);
		WKSPoint p3 = getWKSPointFromPointCollection(ipPointCollection, nextnext);

		// ���b�V�����E�̓���ӏ�ŘA�����Ă��邩�H
		if( p1.X == aMinX && p2.X == aMinX && p3.X == aMinX ||
			p1.X == aMaxX && p2.X == aMaxX && p3.X == aMaxX ||
			p1.Y == aMinY && p2.Y == aMinY && p3.Y == aMinY ||
			p1.Y == aMaxY && p2.Y == aMaxY && p3.Y == aMaxY )
		{
			// �l���ɂ���ꍇ�͘A�����Ă��Ă����O
			if ( (aMinX == p2.X || p2.X == aMaxX) && (aMinY == p2.Y || p2.Y == aMaxY))
			{
				continue;
			}
			// �A�����Ă���ꍇ�͍폜�ΏۂƂ��ăC���f�b�N�X��ێ�
			deleteInex.push_back(next);
		}
	}

	// �A�����Ă���\���_���폜
	bool isDellStartEnd(false);
	for( auto i : deleteInex )
	{
		// �n�I�_�̏ꍇ�̓t���O�ō폜���ꂽ����ێ�
		if ( i == 0 ||  i == aPointCount-1)
		{
			isDellStartEnd = true;
		}
		// �폜
		ipPointCollection->RemovePoints( i, 1 );
	}

	// �n�I�_���폜���ꂽ�ꍇ�A�n�_�I�_���č쐬���Ȃ���
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

// �`��I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i���b�V�����E��Ő��������L���Ă��邱�Ɓj
template <typename T>
bool PGFGDBMergeProcessor<T>::isNeedMerge(IGeometryPtr ipSrcGeom, IGeometryPtr ipTgtGeom, IGeometryPtr meshPolygon)
{
	if( !( ipSrcGeom && ipTgtGeom && meshPolygon ) )
		return false;

	// �ꉞ�����̃W�I���g���^�C�v�m�F
	esriGeometryType srcGeomType = esriGeometryNull;
	esriGeometryType tgtGeomType = esriGeometryNull;
	ipSrcGeom->get_GeometryType( &srcGeomType );
	ipTgtGeom->get_GeometryType( &tgtGeomType );

	if( srcGeomType != tgtGeomType )
		return false;

	// �|���S���̏ꍇ�̓��b�V�����E��3�_����ł����ꍇ�^�񒆂��폜����
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

	// ���b�V���̍ő�l�A�ŏ��l���擾
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
			// �l���̃|�C���g�����L���Ă���ꍇ
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
				// ���̃��b�V���̃I�u�W�F�N�g�ƃ}�[�W
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.X == aMinX ){
						if( fnCompXY( srcPoint.Y, tgtPoint.Y, ACCEPTABLE_Y) ){
							return true;
						}
					}
				}
			}else if( srcPoint.X == aMaxX ){
				// �E�̃��b�V���̃I�u�W�F�N�g�ƃ}�[�W
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.X == aMaxX ){
						if( fnCompXY( srcPoint.Y, tgtPoint.Y, ACCEPTABLE_Y) ){
							return true;
						}
					}
				}
			}else if( srcPoint.Y == aMinX ){
				// ���̃��b�V���̃I�u�W�F�N�g�ƃ}�[�W
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.Y == aMinY ){
						if( fnCompXY( srcPoint.X, tgtPoint.X, ACCEPTABLE_X) ){
							return true;
						}
					}
				}
			}else if( srcPoint.Y == aMaxY ){
				// ��̃��b�V���̃I�u�W�F�N�g�ƃ}�[�W
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
