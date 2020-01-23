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

#include "NWData.h"
#include "InputInfo.h"

using namespace std;

extern crd_cnv g_cnv;

class CImport
{
public:
	CImport(){}
	CImport( const CInputInfo& cInputInfo ){ m_cInputInfo = cInputInfo; }
	~CImport(){}

public:
	bool run();

private:

	/**
	 * @brief   �e�[�u��������
	 */
	void setTableType();

	/**
	 * @brief   2�����b�V���`��擾�֐�
	 * @param   lMeshCode    [in]  ���b�V���R�[�h
	 * @param   ipMeshGeom   [out] ���b�V���`��
	 * @return  true  ���b�V���`��擾����
	 *          false ���b�V���`��擾���s
	 */
	bool getMeshGeom( long lMeshCode, IGeometryPtr& ipMeshGeom );

	/**
	 * @brief   �ҏW�O���PGDB����X�V�����Z�b�g����
	 * @note    �ҏW�O���PGDB���r���A�u�ǉ��A�폜�A�`��X�V�A�����X�V�A�`��{�����X�V�v�����Z�b�g����
	 * @param   cBeforeData  [in]  �ҏW�OPGDB
	 * @param   cAfterData   [in]  �ҏW��PGDB
	 * @return  true  �X�V���Z�b�g����
	 *          false �X�V���Z�b�g���s
	 */
	bool setUpdateData( const CNWData& cBeforeData, const CNWData& cAfterData );

	/**
	 * @brief   �ҏW�O��̃f�[�^����X�V�����Z�b�g����
	 * @note    setUpdateData()�ŌĂ΂�A�e�[�u�����ɏ���������
	 *          m_mapData2Update, m_mapUpdate2Data�ɍX�V�����Z�b�g
	 * @param   strTableName      [in]  �w��e�[�u����
	 * @param   beforeTableData   [in]  �ҏW�O�̎w��e�[�u���f�[�^
	 * @param   afterTableData    [in]  �ҏW��̎w��e�[�u���f�[�^
	 * @param   dataType          [in]  �e�[�u���^�C�v
	 * @return  true  �X�V���Z�b�g����
	 *          false �X�V���Z�b�g���s
	 */
	bool setUpdateTableData( const CString& strTableName, const TableData& beforeTableData, const TableData& afterTableData, DATATYPE dataType );

	/**
	 * @brief   �R���t���N�g�m�F
	 * @param   cSDEFieldMap   [in]  SDE�̃t�B�[���h�}�b�v
	 * @param   cPGDBFieldMap  [in]  PGDB�̃t�B�[���h�}�b�v
	 * @param   ipSDERow       [in]  SDE�̃f�[�^
	 * @param   ipBeforeRow    [in]  �ҏW�OPGDB�̃f�[�^
	 * @param   ipAfterRow     [in]  �ҏW�OPGDB�̃f�[�^
	 * @param   exceptFieldSet [in]  �C���|�[�g���O�t�B�[���hset
	 * @return  true   �R���t���N�g���Ă���
	 *          false  �R���t���N�g���Ă��Ȃ�
	 */
	bool IsConflict( const CFieldMap& cSDEFieldMap, const CFieldMap& cPGDBFieldMap, const _IRowPtr& ipSDERow, const _IRowPtr& ipBeforeRow, const _IRowPtr& ipAfterRow, const std::set<CString>& exceptFieldSet);

	/**
	 * @brief 2�̃e�[�u���s�̑����������I�ɓ����ł��邩
	 * @note  �C���|�[�g���O�t�B�[���h��C���|�[�g�ΏۊO�̃t�B�[���h�������A�������S����v���邩������
	 * @param srcFieldMap    [in] ��r���̃t�B�[���h�}�b�v
	 * @param dstFieldMap    [in] ��r��̃t�B�[���h�}�b�v
	 * @param ipSrcRow       [in] ��r���̍s
	 * @param ipDstRow       [in] ��r��̍s
	 * @param exceptFieldSet [in] �C���|�[�g���O�t�B�[���hset
	 * @param sdeCheck       [in] SDE�Ƃ̔�r�ł��邩(SDE��PGDB�ł͈ꕔ�̓��ꖼ�̃t�B�[���h�̒l���قȂ邽��)
	 * @retval true  �����I�ɓ���
	 * @retval false �����I�ɓ����ł͂Ȃ�
	 */
	bool IsEqualAttributeBySubstance(
		const CFieldMap& srcFieldMap,
		const CFieldMap& dstFieldMap,
		const _IRowPtr& ipSrcRow,
		const _IRowPtr& ipDstRow,
		const std::set<CString>& exceptFieldSet,
		bool  sdeCheck = true );

	/**
	 * @brief   �R���t���N�g���Ă���f�[�^�̃X�e�[�^�X�ύX
	 * @note    ���̊֐����őS�e�[�u���̃R���t���N�g��Ԃ��`�F�b�N��
	 *          ����ɕR�t���f�[�^�̍X�V�X�e�[�^�X��ύX����
	 * @param   cBeforeData   [in]  �ҏW�OPGDB�f�[�^
	 * @param   cAfterData    [in]  �ҏW��PGDB�f�[�^
	 * @param   cSDEData      [in]  �C���|�[�g���SDE�f�[�^
	 * @return  true   �R���t���N�g���Ă���
	 *          false  �R���t���N�g���Ă��Ȃ�
	 */
	bool setConflictData( const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   �R���t���N�g���Ă���f�[�^�̃X�e�[�^�X�ύX
	 * @note    �e�e�[�u���̃C���|�[�g�Ώۃf�[�^���R���t���N�g���Ă���ꍇ�A�X�V�X�e�[�^�X��ύX����
	 *          �܂����̃��R�[�h�ɕR�t���C���|�[�g�Ώۃ��R�[�h�̍X�V�X�e�[�^�X���ύX����
	 * @param   dataStatus    [in]   �C���|�[�g�Ώۃf�[�^�̍X�V�^�C�v�i�V�K�A�폜�A�����X�V�A�`��X�V�A�����{�`��X�V�j
	 * @param   cBeforeData   [in]  �ҏW�OPGDB�f�[�^
	 * @param   strTableName  [in]  �e�[�u����
	 * @param   cAfterData    [in]  �ҏW��PGDGB�f�[�^
	 * @param   cSDEData      [in]  �C���|�[�g���SDE�f�[�^
	 * @return  true   ���ʐ���
	 *          false  ���ʎ��s
	 */
	bool setConflictTableData( DATASTATUS dataStatus, const CString& strTableName, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   �C���|�[�g�Ώۃf�[�^��2�����b�V�����E�Ɋ֘A����C���|�[�g�Ώۃf�[�^�̍X�V�X�e�[�^�X��ύX
	 * @param   dataStatus    [in]   �C���|�[�g�Ώۃf�[�^�̍X�V�^�C�v�i�V�K�A�폜�A�����X�V�A�`��X�V�A�����{�`��X�V�j
	 * @param   cBeforeData   [in]  �ҏW�OPGDB�f�[�^
	 * @param   cAfterData    [in]  �ҏW��PGDGB�f�[�^
	 * @param   cSDEData      [in]  �C���|�[�g���SDE�f�[�^
	 * @return  true   ���ʐ���
	 *          false  ���ʎ��s
	 */
	bool setNGDataForEdgeNode( DATASTATUS dataStatus, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   ���H�����N��FromTo�m�[�h���u0�v�̃P�[�X�����邽�߁A����ɕt������f�[�^�̍X�V�X�e�[�^�X��ύX
	 * @param   dataStatus    [in]  �X�V�\��f�[�^�̃X�e�[�^�X���
	 * @param   cBeforeData   [in]  �ҏW�OPGDB�f�[�^
	 * @param   cAfterData    [in]  �ҏW��PGDGB�f�[�^
	 * @param   cSDEData      [in]  �C���|�[�g���SDE�f�[�^
	 * @return  true   ���ʐ���
	 *          false  ���ʎ��s
	 */
	bool setNGDataForIDZeroNodeLink( DATASTATUS dataStatus, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   �R���t���N�g���Ă���f�[�^�̍X�V�X�e�[�^�X�ύX
	 * @note    �w��e�[�u���̃R���t���N�g���Ă���f�[�^�̍X�V�X�e�[�^�X��ύX����
	 *          �R���t���N�g�����f�[�^�ɕR�t�������N���X�V�X�e�[�^�X�ύX�̑ΏۂƂȂ�
	 * @param   strTableName    [in]     �R���t���N�g�����m�������e�[�u����
	 * @param   updateType      [in]     �C���|�[�g�Ώۃf�[�^�̍X�V�^�C�v�i�V�K�A�폜�A�����X�V�A�`��X�V�A�����{�`��X�V�j
	 * @param   cBeforeData     [in]     �ҏW�OPGDB�f�[�^([bug 10363]�ŃR���t���N�g�̔���ŕK�v�Ȃ��߁A�ҏW�O���ʁX�ɐݒ�)
	 * @param   cAfterData      [in]     �ҏW��PGDB�f�[�^(����)
	 * @param   cSDEData        [in]     �C���|�[�g���SDE�f�[�^
	 * @param   setSearchedLink [in,out] �T���ς݃����N
	 * @param   setNGLink       [in,out] NG�����N
	 * @return  true   ���ʐ���
	 *          false  ���ʎ��s
	 */
	bool setConflictTableData2( const CString& strTableName, UPDATETYPE updateType, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData, set<long>& setSearchedLink, set<long>& setNGLink );

	/**
	 * @brief   �X�V�Ώۂ̍s�ł��邩�`�F�b�N(bug10363�Ή��ŁA�����N�݂̂���e�[�u���S�ʂɏC��)
	 * @note    ���̊֐����ōX�V�Ώۃ����N���w���dataStatus�ɍX�V����
	 * @param   tableName    [in]  �e�[�u������(bug10363)
	 * @param   dataStatus   [in]  �X�V�Ώۃ����N�������ꍇ�ɐݒ肷��X�e�[�^�X���
	 * @param   lPGDBLinkID  [in]  PGDB�̃����NID
	 * @return  true   �X�V�Ώۃ����N�ł���
	 *          false  �X�V�Ώۃ����N�ł͂Ȃ�
	 */
	bool IsUpdateRow( LPCTSTR tableName, DATASTATUS dataStatus, long lPGDBLinkID );

	/**
	 * @brief   �X�V�f�[�^�̃X�e�[�^�X��ݒ肷��
	 * @note    ���̊֐��͓��H�m�[�h��p
	 * @param   dataStatus    [in]  ���H�m�[�h�ɐݒ肷��X�e�[�^�X���
	 * @param   setNGLink     [in]  NG�����N
	 * @param   cPGDBData     [in]  �ҏW�Oor�ҏW��PGDB�f�[�^
	 * @return  true   �X�e�[�^�X�ݒ萬��
	 *          false  �X�e�[�^�X�ݒ莸�s
	 */
	bool setUpdateNGNode( DATASTATUS dataStatus, const set<long>& setNGLink, const CNWData& cPGDBData );

	/**
	 * @brief   �X�V�f�[�^�̃X�e�[�^�X��ݒ肷��
	 * @note    ���̊֐��̓����N�������A�X�΁A����ʍs�A�ʍs�K���A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK����p
	 * @param   dataStatus    [in]  �ݒ肷��X�e�[�^�X���
	 * @param   strTableName  [in]  �ݒ肵�����f�[�^�̃e�[�u����
	 * @param   setNGLink     [in]  NG�����N
	 * @param   cPGDBData     [in]  �ҏW�Oor�ҏW��PGDB�f�[�^
	 * @return  true   �X�e�[�^�X�ݒ萬��
	 *          false  �X�e�[�^�X�ݒ莸�s
	 */
	bool setUpdateNGAGOPS( DATASTATUS dataStatus, const CString& strTableName, const set<long>& setNGLink, const CNWData& cPGDBData );

	/**
	 * @brief   �Ԏ�^�C�v��NG�Ȃ��̂�ݒ肷��
	 * @param   dataStatus    [in]  �ݒ肷��X�e�[�^�X���
	 * @param   cPGDBData     [in]  �ҏW�Oor�ҏW��PGDB�f�[�^
	 * @return  true   �X�e�[�^�X�ݒ萬��
	 *          false  �X�e�[�^�X�ݒ莸�s
	 */
	bool setUpdateNGVehicleType( DATASTATUS dataStatus, const CNWData& cPGDBData );

	/**
	 * @brief   NG�����N�ɑΉ�����X�V�f�[�^�̃X�e�[�^�X��ύX����
	 * @param   dataStatus  [in]  �ݒ肷��X�e�[�^�X���
	 * @param   setNGLink   [in]  NG�����N
	 * @param   cPGDBData   [in]  �ҏW�Oor�ҏW��PGDB
	 * @return  true   �X�e�[�^�X�ݒ萬��
	 *          false  �X�e�[�^�X�ݒ莸�s
	 */
	bool setUpdateNGData( DATASTATUS dataStatus, const set<long>& setNGLink, const CNWData& cPGDBData );

	/**
	 * @brief   �X�V�f�[�^�̃X�e�[�^�X��ݒ肷��
	 * @note    ���̃����N��iINF�j��p��LQ�͍��֐����őΉ�
	 * @param   dataStatus    [in]  �ݒ肷��X�e�[�^�X���
	 * @param   strTableName  [in]  �ݒ肵�����f�[�^�̃e�[�u����
	 * @param   setNGLink     [in]  NG�����N
	 * @param   cPGDBData     [in]  �ҏW�Oor�ҏW��PGDB�f�[�^
	 * @return  true   �X�e�[�^�X�ݒ萬��
	 *          false  �X�e�[�^�X�ݒ莸�s
	 */
	bool setUpdateNGInf( DATASTATUS dataStatus, const CString& strTableName, const set<long>& setNGLink, const CNWData& cPGDBData );

	/**
	 * @brief   �X�V�Ώۂ̃����N��H��A�X�e�[�^�X��ύX����֐�
	 * @note    �X�V�s�ɂ���̂͂��̊֐��̎�������
	 * @param   dataStatus             [in]      �ݒ肷��X�e�[�^�X���
	 * @param   lSearchNodeID          [in]      �ݒ肵�����f�[�^�̃e�[�u����
	 * @param   mapPGDBLink2FromToNode [in]      PGDB�̃����N�ɕR�t��FromTo�m�[�hID�̃}�b�v
	 * @param   mapPGDBNode2Link       [in]      PGDB�̃m�[�h�ɕR�t�������N�̃}�b�v
	 * @param   setSearchedLink        [in,out]  �T���ς݃����N
	 * @param   setNGLink              [in,out]  NG�����N
	 * @return  true   �X�e�[�^�X�ݒ萬��
	 *          false  �X�e�[�^�X�ݒ莸�s
	 */
	bool searchNoUpdateLink( DATASTATUS dataStatus, long lSearchNodeID, const map<long,pair<long,long>>& mapPGDBLink2FromToNode, const map<long,set<long>>& mapPGDBNode2Link, set<long>& setSearchedLink, set<long>& setNGLink );


	/**
	 * @brief   SDE�ɍX�V���𔽉f
	 * @param   cBeforeData  [in]  �ҏW�OPGDB�f�[�^
	 * @param   cAfterData   [in]  �ҏW��PGDB�f�[�^
	 * @param   cSDEData     [in]  SDE�f�[�^
	 * @return  true   ���f����
	 *          false  ���f���s
	 */
	bool reflectData( const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   SDE�̎w��e�[�u���ɍX�V���𔽉f
	 * @param   strTabelName [in]  �e�[�u����
	 * @param   cBeforeData  [in]  �ҏW�OPGDB�f�[�^
	 * @param   cAfterData   [in]  �ҏW��PGDB�f�[�^
	 * @param   cSDEData     [in]  SDE�f�[�^
	 * @return  true   ���f����
	 *          false  ���f���s
	 */
	bool reflectTableData( const CString& strTabelName, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   �X�V����SDE�ɔ��f�i�V�K�쐬�j
	 * @param   strTableName    [in]  �e�[�u����
	 * @param   lPGDBOID        [in]  PGDB��OID
	 * @param   cSDEData        [in]  SDE�f�[�^
	 * @param   cAfterData      [in]  �ҏW��PGDB�f�[�^
	 * @param   sdeTableData    [in]  SDE�̃e�[�u���f�[�^
	 * @param   afterTableData  [in]  �ҏW���PGDB�f�[�^
	 * @return  true   ���f����
	 *          false  ���f���s
	 */
	bool reflectNewData( const CString& strTableName, long lPGDBOID, const CNWData& cSDEData, const CNWData& cAfterData, const TableData& sdeTableData, const TableData& afterTableData );

	/**
	 * @brief   �X�V����SDE�ɔ��f�i�폜�j
	 * @param   strTableName    [in]  �e�[�u����
	 * @param   lPGDBOID        [in]  PGDB��OID
	 * @param   cSDEData        [in]  SDE�f�[�^
	 * @param   cBeforeData     [in]  �ҏW�OPGDB�f�[�^
	 * @param   sdeTableData    [in]  SDE�̃e�[�u���f�[�^
	 * @param   beforeTableData [in]  �ҏW�O��PGDB�f�[�^
	 * @return  true   ���f����
	 *          false  ���f���s
	 */
	bool reflectDeleteData( const CString& strTableName, long lPGDBOID, const CNWData& cSDEData, const CNWData& cBeforeData, const TableData& sdeTableData, const TableData& beforeTableData );

	/**
	 * @brief   �X�V����SDE�ɔ��f�i�����A�`��X�V�j
	 * @param   strTableName    [in]  �e�[�u����
	 * @param   lPGDBOID        [in]  PGDB��OID
	 * @param   updateType      [in]  �f�[�^�X�V���
	 * @param   cSDEData        [in]  SDE�f�[�^
	 * @param   cAfterData      [in]  �ҏW��PGDB�f�[�^
	 * @param   sdeTableData    [in]  SDE�̃e�[�u���f�[�^
	 * @param   afterTableData  [in]  �ҏW���PGDB�f�[�^
	 * @param   beforeTableData [in]  �ҏW�O��PGDB�f�[�^([bug 10363]�ŃC���|�[�g����Ȃ������ҏW���̏��O�t�B�[���h���o�͂��邽�ߎg��)
	 * @return  true   ���f����
	 *          false  ���f���s
	 */
	bool reflectUpdateData( const CString& strTableName, long lPGDBOID, UPDATETYPE updateType, const CNWData& cSDEData, const CNWData& cAfterData, const TableData& sdeTableData, const TableData& beforeTableData, const TableData& afterTableData );

	/**
	 * 
	 */
	CString getExceptFieldString(
		const TableData& beforeTableData,
		const TableData& afterTableData,
		const _IRowPtr& ipBeforeRow,
		const _IRowPtr& ipAfterRow,
		const std::set<long>& exceptFieldIndexSet
		);

	std::set<CString> getExceptFieldSet(LPCTSTR fieldName);

	/**
	 * @brief   FromTo�m�[�hID�t���ւ�
	 * @param   strTableName    [in]  �e�[�u����
	 * @param   cAfterData      [in]  �ҏW��PGDB�f�[�^
	 * @param   ipSDERow        [in]  SDE�̃��R�[�h�f�[�^
	 * @param   lPGDBNodeID     [in]  PGDB��From,To�m�[�hID
	 * @param   lNodeIndex      [in]  From,To�m�[�h�̃t�B�[���h�C���f�b�N�X
	 * @return  true   ���f����
	 *          false  ���f���s
	 */
	bool replaceFromToNodeID( const CString& strTableName, const CNWData& cAfterData, _IRowPtr& ipSDERow, long lPGDBNodeID, long lNodeIndex );


	/** 
	 * @brief   ��Ɨp�q�o�[�W�����̍쐬���͎g���܂킵��A���[�N�X�y�[�X�ɐڑ�
	 * @param   ipWorkspace        [in]  ��ƌ����[�N�X�y�[�X
	 * @param   ipSDEWork          [out] ��Ɨp���[�N�X�y�[�X
	 * @return  true   �q�o�[�W�����擾�{���[�N�X�y�[�X�ڑ�����
	 *          false  �q�o�[�W�����擾�{���[�N�X�y�[�X�ڑ����s
	 */ 
	bool createWorkVersion( IWorkspacePtr& ipWorkspace, IWorkspacePtr& ipSDEWork );

	/**
	 * @brief   SDE�̃��R�[�h��PGDB�̃f�[�^�𔽉f����i�`��{�����j
	 * @param   ipSDERow    [in]  SDE�̃��R�[�h
	 * @param   ipPGDBRow   [in]  PGDB�̃��R�[�h
	 * @param   cSDEField   [in]  SDE�̃t�B�[���h���
	 * @param   cPGDBField  [in]  PGDB�̃t�B�[���h���
	 * @param   exceptFieldSet [in] �C���|�[�g���O�t�B�[���h��set(bug 10363)
	 * @return  true   �f�[�^�ݒ萬��
	 *          false  �f�[�^�ݒ莸�s
	 */
	bool setRowData( _IRowPtr& ipSDERow, const _IRowPtr& ipPGDBRow, const CFieldMap& cSDEField, const CFieldMap& cPGDBField, const std::set<CString>& exceptFieldSet = std::set<CString>() );

	/**
	 * @brief   INF�ɕR�t��LQ���č\�z
	 * @note    �ҏW��PGDB��LQ��������SDE���ɍč\�z����
	 * @param   lSDEInfID       [in]  SDE��INF��OID
	 * @param   ipSDELqTable    [in]  SDE��LQ�e�[�u��
	 * @param   cSDELqField     [in]  SDE��LQ�e�[�u���̃t�B�[���h���
	 * @param   cAfterData      [in]  �ҏW��PGDB�̃e�[�u���f�[�^
	 * @param   setAfterLqData  [in]  �ҏW��PGDB��LQ�f�[�^
	 * @return  true   LQ�\�z����
	 *          false  LQ�\�z���s
	 */
	bool createLqData( long lSDEInfID, ITablePtr& ipSDELqTable, const CFieldMap& cSDELqField, const CNWData& cAfterData, const set<LqData>& setAfterLqData );

	/**
	 * @brief   LQ�폜
	 * @param   lSDEInfID     [in]  SDE�̍폜����INF��OID
	 * @param   ipSDELqTable  [in]  SDE��LQ�e�[�u��
	 * @return  true   LQ�폜����
	 *          false  LQ�폜���s
	 */
	bool deleteLqData( long lSDEInfID, ITablePtr& ipSDELqTable );

	/**
	 * @brief   �Ή��I�u�W�F�N�gID�擾�p�֐�
	 * @note    bug10363�ɔ����A�����N�݂̂̑Ή�����ĉ�
	 * @param   tableName [in]  �Ώۂ̃e�[�u����
	 * @param   cNWData   [in]  �ҏW�O���PGDB�f�[�^
	 * @param   lPgdbID   [in]  PGDB�̃I�u�W�F�N�gID
	 * @param   lOrgID    [out] SDE�̃I�u�W�F�N�gID
	 * @return  true   �Ή��I�u�W�F�N�gID�擾����
	 *          false  �Ή��I�u�W�F�N�gID�擾���s
	 */
	bool getOrgIDFromPgdbID( LPCTSTR tableName, const CNWData& cNWData, long lPgdbID, long& lOrgID );

	/**
	 * @brief   ���O�n���h���c�[���m�F�p�̃��O�擾�֐�
	 * @param   dbType        [in]  �ҏW�OPGDB�A�ҏW��PGDB�ASDE��
	 * @param   strTableName  [in]  �e�[�u����
	 * @param   lOID          [in]  �I�u�W�F�N�gID
	 * @param   bOK           [in]  SDE�ɔ��f�������ǂ���
	 * @param   strStatus     [in]  �f�[�^�̏�
	 * @return  �o�͂��郍�O
	 */
	CString getImportLog( DBTYPE dbType, const CString& strTableName, long lOID, bool bOK, const CString& strStatus );

	/**
	 * @brief   �G���[���O�o�͊֐�
	 * @param   errType        [in]  �G���[�^�C�v
	 * @param   strFuncName    [in]  �֐���
	 * @param   strComment     [in]  �G���[�̏�
	 * @param   strTableName   [in]  �e�[�u����
	 * @param   lOID           [in]  �I�u�W�F�N�gID
	 * @param   bOutTime       [in]  ���݂̎��Ԃ��o�͂��邩�ǂ���
	 */
	void writeErrorLog( ERRTYPE errType, const CString& strFuncName, const CString& strComment, const CString& strTableName, long lOID, bool bOutTime = false );

	/**
	 * @brief   �ҏW�J�n
	 * @param   ipWorkspace  [in]  ipWorkspace
	 * @return  true    �ҏW�J�n����
	 *          false   �ҏW�J�n���s
	 */
	bool StartEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief   �ҏW�I���J�n
	 * @param   bSave        [in]  �ۑ����邩�ǂ���
	 * @param   ipWorkspace  [in]  ipWorkspace
	 * @return  true    �ҏW�J�n����
	 *          false   �ҏW�J�n���s
	 */
	bool StopEdit( bool bSave, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief   ���R���T�C������
	 * @param   ipWorkspaceEdit  [in]  ���[�N�X�y�[�X
	 * @return  true    ��������
	 *          false   �������s
	 */
	bool DoReconcile( IWorkspaceEditPtr& ipWorkspaceEdit );

	/**
	 * @brief   �r�����f����
	 * @return  true    �����I��
	 *          false   �������s
	 */
	bool IsSuspension();

	/**
	 * @brief �C���|�[�g���O�t�B�[���h���X�g��ǂݍ���(bug 10363)
	 * @retval �ǂݍ��݂̐���
	 */
	bool readExceptFieldList();

	/**
	 * @brief �C���|�[�g���O�t�B�[���h�Ƃ��đÓ��ł��邩���m�F����(bug 10363)
	 * @note  ���X�C���|�[�g�ΏۊO�̃t�B�[���h��A���e�[�u����ID�������悤�Ȃ��̂͏��O�t�B�[���h�Ƃ��ĕs�K�Ɣ��f����
	 * @note  ArcHelperEx��IsEditableField�ƈႢ�AUPDATETYPE_C�͑Ó��Ȗ��̂Ƃ��Ď�舵��
	 * @note  �����ύX�̃c�[���ɂ���Ă�UPDATETYPE_C���X�V���邱�Ƃ����邩������Ȃ�����
	 * @param fieldName [in] �t�B�[���h����
	 * @retval true  �C���|�[�g���O�t�B�[���h�Ƃ��đÓ��ł���
	 * @retval false �C���|�[�g���O�t�B�[���h�Ƃ��đÓ��ł͂Ȃ�
	 */
	bool checkExceptField(const CString& fieldName);

private:
	CString m_strProgModifyDate;           //!< �c�[���X�V��
	CString m_strWorkVersionName;          //!< �ҏW�p�q�o�[�W�������i���s���ɖ��O��ݒ肷��j
	CInputInfo m_cInputInfo;               //!< �R�}���h���C���������
	CString m_strToolName;                 //!< �c�[����
	ISpatialReferencePtr m_ipSpRef;        //!< ��ԎQ�ƁiSDE�j

	long m_lMeshCode;      //!< �������̃��b�V���R�[�h
	ofstream m_ofs;        //!< ���O�p�X�g���[��
	ofstream m_ofsErr;     //!< �G���[���O�p�X�g���[��

	map<CString,DATATYPE> m_mapTableMap;   //!< �e�[�u�������X�g
	vector<pair<CString,DATASTATUS>> m_vecConflictTable;    //!< �R���t���N�g��������e�[�u���̏���
	vector<CString> m_vecReflectTable;     //!< SDE�Ƀf�[�^���f���鏈���̃e�[�u���̏���

	map<CString,map<long,UpdateData>> m_mapData2Update;         // �f�[�^��[�V�K�A�폜�A�`��X�V�A�����X�V�A�`��{�����X�V]�̃}�b�v
	map<CString,map<UPDATETYPE,set<long>>> m_mapUpdate2Data;    //!< [�V�K�A�폜�A�`��X�V�A�����X�V�A�`��{�����X�V]�ƃf�[�^�}�b�v

	map<CString,map<long,long>> m_mapPGDBOldID2SDENewID;  //!< �V�K�쐬�����f�[�^��SDE���OID�ƕҏW��f�[�^��OID�Ƃ̃}�b�v

	map<CString,set<CString>> m_exceptFieldMap; //!< �C���|�[�g���O�t�B�[���hmap(bug 10363)

	vector<CString> m_vecImportLog;  //!< �C���|�[�g���O
};
