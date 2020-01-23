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

#include "Arguments.h"
#include "LogManager.h"
#include <TDC/sindylib_core/FieldMap.h>

class CheckStopPoint
{
public:
	/**
	 * @brief   ���H�����N�ێ��p�\����
	 */
	struct LINK {
		long roadClass;
		long linkClass;
		long upDownClass;
		long roadWidth;
		long fromID;
		long toID;
		bool foreOneway;
		bool revOneway;
		bool noPassage;
		bool outOfMesh;     //!< ���b�V���Ɛڂ��Ă��邪�A�����Ώۃ��b�V���O
		IGeometryPtr ipGeo;

		LINK(){
			roadClass    = -1;
			linkClass    = -1;
			upDownClass  = -1;
			roadWidth    = -1;
			fromID       = -1;
			toID         = -1;
			foreOneway   = false;
			revOneway    = false;
			noPassage    = false;
			outOfMesh    = false;
		};
	};

	/**
	 * @brief   ���H�m�[�h�ێ��p�\����
	 */
	struct NODE {
		long nodeClass;
		long signal;
		std::list<long> linkIDs; //!< �ڑ������N��
		IGeometryPtr ipGeo;

		NODE(){
			nodeClass = -1;
			signal    = -1;
		};
	};

	/**
	 * @brief   �ꎞ��~�ێ��p�\����
	 */
	struct STOP {
		long linkID;
		long nodeID;
		IPointPtr ipPoint;

		STOP(){
			linkID = -1;
			nodeID = -1;
		};

		bool operator == ( const STOP& obj ) const {
			return ( linkID == obj.linkID && nodeID == obj.nodeID );
		};

		bool operator < ( const STOP& obj ) const {
			if( linkID != obj.linkID )
				return ( linkID < obj.linkID );
			return ( nodeID < obj.nodeID );
		};
	};

	typedef std::map<long, LINK> LINKMAP; //!< key:�����NID�A value:�����N�������
	typedef std::map<long, NODE> NODEMAP; //!< key:�m�[�hID�A value:�m�[�h�������
	typedef std::map<long, STOP> STOPMAP; //!< key:�ꎞ��~�|�C���g�̃I�u�W�F�N�gID�Avalue:�ꎞ��~�������

public:
	/// �R���X�g���N�^
	CheckStopPoint( const Arguments& args ){
		m_strDBRoad      = (args.m_db_road).c_str();
		m_strDBRoad_past = (args.m_db_road_past).c_str();
		m_strDBStop      = (args.m_db_stop).c_str();
		m_strDBMesh      = (args.m_db_mesh).c_str();
		m_strOutput      = (args.m_output).c_str();
		m_strMeshlist    = (args.m_meshlist).c_str();
		m_extract        = args.m_extract;
		m_check          = args.m_check;
	};

	/// �f�X�g���N�^
	~CheckStopPoint(){};

	/**
	 * @brief   �������C���֐�
	 * @retval  true:  ��������
	 * @retval  false: �������s
	 */
	bool execute( );

	/**
	 * @brief   �e�평����
	 * @retval  true:  ����������
	 * @retval  false: ���������s
	 */
	bool init();

	/**
	 * @brief   �t�B�[�`���N���X�I�[�v�����t�B�[���h�}�b�v�쐬
	 * @param   name           [in]  �t�B�[�`���N���X��
	 * @param   ipWorkspace    [in]  �I�[�v�����邽�߂̃��[�N�X�y�[�X
	 * @param   ipFeatureClass [out] �擾����t�B�[�`���N���X
	 * @param   fieldMap       [out] �擾����t�B�[���h�}�b�v
	 * @return  true:����  false:���s
	 */
	bool OpenFeatureClass( const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, sindy::CFieldMap& fieldMap );

	/**
	 * @brief   �e�[�u���I�[�v�����t�B�[���h�}�b�v�쐬
	 * @param   name           [in]  �t�B�[�`���N���X��
	 * @param   ipWorkspace    [in]  �I�[�v�����邽�߂̃��[�N�X�y�[�X
	 * @param   ipTable        [out] �擾����e�[�u��
	 * @param   fieldMap       [out] �擾����t�B�[���h�}�b�v
	 * @return  true:����  false:���s
	 */
	bool OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, sindy::CFieldMap& fieldMap );

	/**
	 * @brief   ���b�V���|���S���擾
	 * @param   mesh      [in]  2�����b�V���R�[�h
	 * @param   ipMeshGeo [out] ���b�V���|���S���̃W�I���g��
	 * @retval  true:  ����
	 * @retval  false: ���s
	 */
	bool GetMeshPolygon( long mesh, IGeometryPtr& ipMeshGeo );

	/**
	 * @brief   ���b�V�����̊����ꎞ��~���擾
	 * @param   ipMeshGeo [in]  ���b�V���|���S���̃W�I���g��
	 * @param   stops     [out] ���������NID�Ɨ��o�m�[�hID��set 
	 * @retval  true:  �擾����
	 * @retval  false: �擾���s
	 */
	bool GetStopInMesh( const IGeometryPtr& ipMeshGeo, STOPMAP& stops );

	/**
	 * @brief   ���b�V�����̓��H�����N�Ɠ��H�m�[�h���擾
	 * @note    ���łɁA�m�[�h�ɑ΂���ڑ������N���ێ�����
	 * @param   ipMeshGeo [in]  ���b�V���|���S���̃W�I���g��
	 * @param   linkMap   [out] �擾�������H�����NMap(�L�[�F�����N��ObjectID)
	 * @param   nodeMap   [out] �m�[�h�ɑ΂���ڑ������NIDs��Map(�L�[�F�m�[�h��ObjectID)
	 * @param   ipLinkFC  [in]  �ΏۂƂ��铹�H�����N�t�B�[�`���N���X
	 * @param   linkFM    [in]  linkFC�̃t�B�[���h�}�b�v
	 * @param   ipNodeFC  [in]  �ΏۂƂ��铹�H�m�[�h�t�B�[�`���N���X
	 * @param   nodeFM    [in]  nodeFC�̃t�B�[���h�}�b�v
	 * @retval  true:  ����
	 * @retval  false: ���s
	 */
	bool GetLinkNodeInMesh( const IGeometryPtr& ipMeshGeo, LINKMAP& linkMap, NODEMAP& nodeMap,
		IFeatureClassPtr& ipLinkFC, sindy::CFieldMap& linkFM, IFeatureClassPtr& ipNodeFC, sindy::CFieldMap& nodeFM );

	/**
	 * @brief   ���o�������C���֐�
	 * @param   linkMap  [in]  ���H�����NMap
	 * @param   nodeMap  [in]  ���H�m�[�hMap
	 * @param   stops    [in]  �����ꎞ��~��Set
	 * @param   mesh     [in]  ���b�V���R�[�h(���O�p)
	 */
	void Extract( const LINKMAP& linkMap, const NODEMAP& nodeMap, const STOPMAP& stops, long mesh );

	/**
	 * @brief   �`�F�b�N�������C���֐�
	 * @param   linkMap       [in]  ���H�����NMap
	 * @param   nodeMap       [in]  ���H�m�[�hMap
	 * @param   linkMap_past  [in]  �o�N�ω��O�̓��H�����NMap
	 * @param   nodeMap_past  [in]  �o�N�ω��O�̓��H�m�[�hMap
	 * @param   stops         [in]  �����ꎞ��~��Set
	 * @param   mesh          [in]  ���b�V���R�[�h(���O�p)
	 */
	void ErrorCheck( const LINKMAP& linkMap, const NODEMAP& nodeMap, const LINKMAP& linkMap_past, const NODEMAP& nodeMap_past, const STOPMAP& stops, long mesh );

	/**
	 * @brief   ���݂��Ȃ����̂ƌ��Ȃ����H��ʂ��H
	 * @note    roadclass_c >= 201 �̓��H�����N�́A���݂��Ȃ����̂Ƃ��Ă݂Ȃ��B
	 * @param   roadClass [in]  ���H���
	 * @retval  true:  ���݂��Ȃ����̂ƌ��Ȃ�
	 * @retval  false: ���ʂɏ����Ώ�
	 */
	bool IsNoTarget( long roadClass );

	/**
	 * @brief   �����NID�ƃm�[�hID�̑g�ݍ��킹�������ꎞ��~�ɑ��݂��邩�H
	 * @param   linkID  [in]  �����N��ObjectID
	 * @param   nodeID  [in]  �m�[�h��ObjectID
	 * @param   stops   [in]  �����ꎞ��~��Set
	 * @retval  true:  ���݂���
	 * @retval  false: ���݂��Ȃ�
	 */
	bool FindStopPoint( long linkID, long nodeID, const STOPMAP& stops );


	/**
	 * @brief   �����N�ƃm�[�h�̊֌W����A���o�����𖞂������m�F����
	 * @param   link     [in]  �����N�̏���ێ������\����
	 * @param   nodeID   [in]  �m�[�h��ObjectID
	 * @param   links    [in]  �����NMap
	 * @param   nodeMap  [in]  �m�[�hMap
	 * @retval  true:  ���o�����𖞂���(���o�Ώ�)
	 * @retval  false: ���o�����𖞂����Ȃ�
	 */
	bool IsExtractTarget( const LINK& link, long nodeID, const LINKMAP& links, const NODEMAP& nodeMap );

	/**
	 * @brief   �����NID���烊���N���擾���A��v�n�����ȏォ�m�F����
	 * @note    ��v�n�����ȏ�Fupdownclass_c not in (0,6) or (updownclass_c in (0,6) and roadclass_c in (1,2,3,103,104,105))
	 * @param   links   [in]  ���H�����N���Q
	 * @param   linkID  [in]  �����N��ObjectID
	 * @retval  true:  ��v�n�����ȏナ���N
	 * @retval  false: ��L�ȊO
	 */
	bool IsPrefOrOver( const LINKMAP& links, long linkID );

	/**
	 * @brief   �����NID���烊���N���擾���A�����ȏォ�m�F����
	 * @note    �����ȏ�Fupdownclass_c not in (0,6) or (updownclass_c in (0,6) and roadclass_c in (1,103))
	 * @param   links   [in]  ���H�����N���Q
	 * @param   linkID  [in]  �����N��ObjectID
	 * @retval  true:  �����ȏナ���N
	 * @retval  false: ��L�ȊO
	 */
	bool IsCountryOrOver( const LINKMAP& links, long linkID );

	/**
	 * @brief   ���]�ԓ��ł͂Ȃ�(�����N���(not 8))��HWY�ł͂Ȃ�(�n�C�E�F�C�H��(0,6))���m�F����
	 * @param   link  [in]  �����N�̏���ێ������\����
	 * @retval  true:  not ���]�ԓ� && not HWY
	 * @retval  false: ��L�ȊO
	 */
	bool IsNotBicycleNotHWY( const LINK& link );

	/**
	 * @brief   �ʍs�֎~�����Ă��Ȃ����A����ʍs�̌��������Ȃ����m�F����
	 * @param   link   [in]  �����N�̏���ێ������\����
	 * @param   nodeID [in]  �m�[�hID
	 * @retval  true:  ���Ȃ��i�ꎞ��~�t�^�Ώہj
	 * @retval  false: ��肠��
	 */
	bool IsRegOK( const LINK& link, long nodeID );

	/**
	 * @brief   �����N��������_�ł��邩�𔻒肷��
	 * @param   link   [in]  �����N�̏���ێ������\����
	 * @retval  true:  ������_�ł���
	 * @retval  false: ������_�łȂ�
	 */
	bool IsRoundAbout( const LINK& link );

	/**
	 * @brief   �����N���{���i�㉺�����j�����N�ł��邩�𔻒肷��
	 * @param   link   [in]  �����N�̏���ێ������\����
	 * @retval  true:  �{���i�㉺�����j�����N�ł���
	 * @retval  false: �{���i�㉺�����j�����N�łȂ�
	 */
	bool IsMainUpDown( const LINK& link );

	/**
	 * @brief   �����N��HWY�{���ł��邩�𔻒肷��
	 * @param   link   [in]  �����N�̏���ێ������\����
	 * @retval  true:  HWY�{���ł���
	 * @retval  false: HWY�{���łȂ�
	 */
	bool IsHighwayMain( const LINK& link );

	/**
	 * @brief 2�����b�V�����E�m�[�h���ǂ����𔻒肷��
	 * @param node  [in]   ���H�m�[�h�̏����i�[�����\����
	 * @retval true  ���b�V�����E�m�[�h�ł���
	 * @retval false ���b�V�����E�m�[�h�ł͂Ȃ�
	 */
	bool IsMeshEdge( const NODE& node );

	/**
	* @brief �����_�m�[�h���ǂ����𔻒肷��𔻒肷��
	* @param node  [in]   ���H�m�[�h�̏����i�[�����\����
	* @retval true  �����_�m�[�h�ł���
	* @retval false �����_�m�[�h�ł͂Ȃ�
	*/
	bool IsCross(const NODE& node);

	/**
	 * @brief ���H�m�[�h���M���@�t�������_�ł��邩�ǂ����𔻒肷��
	 * @param node  [in]   ���H�m�[�h�̏����i�[�����\����
	 * @retval true  �M���@�t�������_�ł���
	 * @retval false �M���@�t�������_�ł͂Ȃ�
	 */
	bool IsSignalCross( const NODE& node );

	/**
	 * @brief   2�̃����N���Ȃ��p�x�����߂�
	 * @param   link1  [in]  �����N1
	 * @param   link2  [in]  �����N2
	 * @retval  double: �p�x(0�`180��)
	 */
	double GetAngle( const LINK& link1, const LINK& link2 );

	/**
	 * @brief  �ꎞ��~�|�C���g�𐶐�����֐�
	 * @note   �ꎞ��~�t�^�c�[���Ɠ��l�̃A���S���Y���Ő��������
	 * @param  linkID  [in] ���������NID
	 * @param  nodeID  [in] ���o�m�[�hID
	 * @param  link    [in] ���������N���
	 * @param  node    [in] ���o�m�[�h���
	 * @param  ipSpRef [in] �ꎞ��~�|�C���g�ɓ��Ă��ԎQ��
	 * @return �ꎞ��~�|�C���g�t�^�c�[���Ɠ����A���S���Y���Ő������ꂽ�|�C���g
	 */
	IGeometryPtr CreateStopPointGeometry(long linkID, long nodeID, const LINK& link,  const NODE& node, ISpatialReferencePtr ipSpRef);

private:

	CString          m_strDBRoad;          //!< ���HDB�ڑ�������
	CString          m_strDBRoad_past;     //!< �o�N�ω��O�̓��HDB�ڑ�������
	CString          m_strDBStop;          //!< �ꎞ��~DB�ڑ�������
	CString          m_strDBMesh;          //!< ���b�V��DB�ڑ�������
	CString          m_strOutput;          //!< ���O�p�X
	CString          m_strMeshlist;        //!< ���b�V�����X�g�p�X
	bool             m_extract;            //!< ���o���[�h��
	bool             m_check;              //!< �`�F�b�N���[�h��

	IWorkspacePtr    m_ipRoadWorkspace;      //!< ���HDB���[�N�X�y�[�X
	IWorkspacePtr    m_ipRoadPastWorkspace;  //!< ���HDB���[�N�X�y�[�X
	IWorkspacePtr    m_ipStopWorkspace;      //!< �ꎞ��~DB���[�N�X�y�[�X
	IWorkspacePtr    m_ipMeshWorkspace;      //!< ���b�V��DB���[�N�X�y�[�X

	IFeatureClassPtr m_ipRoadLinkFC;         //!< ���H�����N�t�B�[�`���N���X
	IFeatureClassPtr m_ipRoadLinkFC_past;    //!< �o�N�ω��O�̓��H�����N�t�B�[�`���N���X
	IFeatureClassPtr m_ipRoadNodeFC;         //!< ���H�m�[�h�t�B�[�`���N���X
	IFeatureClassPtr m_ipRoadNodeFC_past;    //!< �o�N�ω��O�̓��H�m�[�h�t�B�[�`���N���X
	IFeatureClassPtr m_ipStopPointFC;        //!< �ꎞ��~�t�B�[�`���N���X
	IFeatureClassPtr m_ipBaseMeshFC;         //!< �x�[�X���b�V���t�B�[�`���N���X

	sindy::CFieldMap m_RoadLinkFM;           //!< ���H�����N�t�B�[���h�}�b�v
	sindy::CFieldMap m_RoadNodeFM;           //!< ���H�m�[�h�t�B�[���h�}�b�v
	sindy::CFieldMap m_RoadLinkFM_past;      //!< �o�N�ω��O�̓��H�����N�t�B�[���h�}�b�v
	sindy::CFieldMap m_RoadNodeFM_past;      //!< �o�N�ω��O�̓��H�m�[�h�t�B�[���h�}�b�v
	sindy::CFieldMap m_StopPointFM;          //!< �ꎞ��~�t�B�[���h�}�b�v
	sindy::CFieldMap m_BaseMeshFM;           //!< �x�[�X���b�V���t�B�[���h�}�b�v

	CLogManager      m_logManager;           //!< ���O�Ǘ��N���X

	std::set<long>   m_meshList;             //!< ���b�V�����X�g
};
