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
#include "sdebase.h"

/**
 * @brief �ҏW�������f����i�����u��C�f�[�^�C���|�[�g�v�̃��C���j�N���X
 */
class CImport :
	public CSDEBase
{
public:
	/**
	 * @brief �R���X�g���N�^�B
	 */
	CImport(void);

	/**
	 * @brief �f�X�g���N�^�B
	 */
	~CImport(void);

	/**
	 * @brief �������֐��B
	 *
	 * @param args [in] ���s���I�v�V����
	 */
	bool init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );

private:
	CString m_OldPath;         //!< �ҏW�OPGDB�̃x�[�X�p�X
	CString m_NewPath;         //!< �ҏW��PGDB�̃x�[�X�p�X
	ID_SET m_AppendID;         //!< ���b�V�����E�̑����ύX�ɂ��A�V�K�ǉ������ɂȂ������̂��Ǘ�����ID�Z�b�g
	ID_SET m_TargetID;         //!< �����ς݂̃I�u�W�F�N�g�̌���ID���Ǘ�����Z�b�g
	long m_OrgIdIndex;         //!< �I���W�i���I�u�W�F�N�gID�̃t�B�[���h�C���f�b�N�X
	long m_ChgStatusIndex;     //!< �X�V�X�e�[�^�X�̃t�B�[���h�C���f�b�N�X
	long m_OrgModifyDateIndex; //!< �I���W�i���̍ŏI�X�V���t�̃t�B�[���h�C���f�b�N�X
	COMMAND m_Command;         //!< ���f�p�R�}���h�i�ǉ��̂݁E�X�V�E�폜�̂݁j
	std::map<COMMAND,CString> m_CommandString; //!< ���b�V�����X�g�̃R�}���h�ɑΉ�������{��\�L
	bool m_tdcMode;            //!< TDC�D�惂�[�h
	bool m_geospaceMode;       //!< Geospace���[�h
	list<CString> m_layerList; //!< Geospace���[�h�Ώۃ��C�����X�g
	COleDateTime m_availDate;  //!< �L�����t(poi�C���|�[�g���g�p)
	bool m_emptyDBMode;        //!< ��DB�C���|�[�g���[�h

	std::map<CString,vector<long>> m_updateOid; //!< �����ς�(�폜�ȊO)ORG_OBJID�i�[
	std::map<CString,vector<long>> m_deleteOid; //!< �����ς�(�폜)ORG_OBJID�i�[
	std::map<CString,vector<long>> m_crossMeshOldOid; //!< OLD���Ń��b�V���܂����ł���ORG_OBJID�i�[
	std::map<CString,vector<long>> m_crossMeshNewOid; //!< NEW���Ń��b�V���܂����ł���ORG_OBJID�i�[
	std::map<CString,vector<long>> m_onlyNewOid; //!< NEW���ɂ������Ȃ�ORG_OBJID�i�[

	/**
	 * @brief ���b�V���P�ʂł̍����C���|�[�g�i���f�j�֐��B
	 *
	 * @param cCommand		[in]	���f�p�R�}���h�i�ǉ��̂݁E�X�V�E�폜�̂݁E�폜�{�ǉ��j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnExecMesh(COMMAND cCommand);

	/**
	 * @brief PGDB�I�[�v���֐��B
	 *
	 * @param cBasePath		[in]	PGDB�x�[�X�p�X
	 *
	 * @retval NULL�ȊO	PGDB�̃��[�N�X�y�[�X
	 * @retval NULL		���s�A�G���[
	 */
	IWorkspacePtr fnOpenLocalDatabase(LPCTSTR cBasePath);
	
	/**
	 * @brief PGDB�I�[�v���֐��B
	 * @note  "�`\hoge.mdb(gdb)"�܂Ŏw�肳��Ă���ꍇ�ɁAfnOpenLocalDatabase�����ŌĂ΂�܂�
	 *
	 * @param cFilePath		[in]	PGDB�p�X
	 *
	 * @retval NULL�ȊO	PGDB�̃��[�N�X�y�[�X
	 * @retval NULL		���s�A�G���[
	 */
	IWorkspacePtr fnOpenWorkspaceFromFile(LPCTSTR cFilePath);

	/**
	 * @brief PGDB���̎w��C���f�b�N�X�̃t�B�[�`���N���X�I�[�v���֐��B
	 *
	 * @param ipWorkspace	[in]	PGDB�̃��[�N�X�y�[�X
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval NULL�ȊO	�t�B�[�`���N���X
	 * @retval NULL		���s�A�G���[
	 */
	IFeatureClassPtr fnOpenLocalFeatureClass(IWorkspacePtr ipWorkspace, long cIndex);

	/**
	 * @brief ���b�V�����E���l�����Ă̔�r�ƕύX�E�폜�֐��B
	 *
	 * @param ipOldClass	[in]	�ҏW�O�̃t�B�[�`���N���X
	 * @param ipNewClass	[in]	�ҏW��̃t�B�[�`���N���X
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCompare(IFeatureClassPtr ipOldClass, IFeatureClassPtr ipNewClass, long cIndex);

	/**
	 * @brief 	���b�V�����E���l�����Ȃ��Ă������̂̔�r�ƕύX�E�폜�֐��B
	 *	�@�@�@�i���L�ƃ|�C���g���Y�����邪�A���L���Ȃ��Ȃ����̂ŁA�����|�C���g�̂݁j
	 *
	 * @param ipOldClass	[in]	�ҏW�O�̃t�B�[�`���N���X
	 * @param ipNewClass	[in]	�ҏW��̃t�B�[�`���N���X
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnComparePoint(IFeatureClassPtr ipOldClass, IFeatureClassPtr ipNewClass, long cIndex);

	/**
	 * @brief ipSrcFeature�����Ƀt�B�[�`����V�K�ǉ����܂��B
	 * @note  �֐�����m_ipFeatureClasses[cIndex]����InsertCursor���܂��B
	          �C���|�[�g�悪��ł���(�I�u�W�F�N�g���d�Ȃ铙�̃P�A�͍l���Ȃ�)���ƑO��ł��B
	 * @param ipSrcFeature [in] �ҏW��PGDB�̃t�B�[�`��
	 * @param cIndex       [in] �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param updateStatus [in,opt] �V�K�ǉ��t�B�[�`���ɃZ�b�g����UPDATE_C�̒l(�w�肵�Ȃ����fnSetCommonAttr()�ł̏������ʂ��̂܂�)
	 * @retval true  ����
	 * @retval false ���s�A�G���[
	 */
	bool fnInsertIntoEmptyDB( const IFeaturePtr& ipNewFeature, long cIndex, long updateStatus = -1 );


	/**
	 * @brief POI_POINT�̔�r�ƕύX�֐��B
	 * @param ipOldClass [in] �ҏW�O�̃t�B�[�`���N���X
	 * @param ipNewClass [in] �ҏW��̃t�B�[�`���N���X
	 * @param cIndex     [in] �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @retval true  ����
	 * @retval false ���s�A�G���[
	 */
	bool fnComparePoi( const IFeatureClassPtr& ipOldClass, const IFeatureClassPtr& ipNewClass, long cIndex );

	/**
	 * @brief POI_POINT�̑����ύX�ƍX�V���s��
	 * @param sindyFeature [in] SDE��̃t�B�[�`��
	 * @param pgdbFeature  [in] �ҏW��iPGDB�j�̃t�B�[�`��
	 * @param cIndex       [in] �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param oid          [in] SDE��̃I�u�W�F�N�gID
	 * @param message      [in] �X�V���悤�Ƃ��Ă�����e�i���O�o�͗p�j
	 * @retval true  ����
	 * @retval false ���s�A�G���[
	 */
	bool fnSiNDYPoiChange( IFeaturePtr& sindyFeature, const IFeaturePtr& pgdbFeature, long cIndex, long oid, CString& message );

	/**
	 * @brief Store����
	 *
	 * @param sindyFeature [in] Store����t�B�[�`��
	 * @param cIndex       [in] �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param message      [in,out] ���O�p���b�Z�[�W
	 *
	 * @retval true ����
	 * @retval false ���s�A�G���[
	 */
	bool fnStoreFeature( const IFeaturePtr& sindyFeature, long cIndex, long oid, CString &message);


	/**
	 * @brief 	���b�V���|���S�����폜�A���b�V�����E�͐؂����čX�V
	 *	�@�@�@�i�폜�̂ݖ��͍폜�{�ǉ��̎��Ɏg�p�B�ŋ߂ł͂قƂ�ǎg���Ȃ��͂��j
	 *
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnClipData(long cIndex);

	/**
	 * @brief 	�V�K�ǉ������ꊇ�ǉ��֐��B
	 *
	 * @param ipFeatureClass	[in]	�ҏW��̃t�B�[�`���N���X
	 * @param cIndex			[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnAddNew(IFeatureClassPtr ipFeatureClass, long cIndex);

	/**
	 * @brief �t�B�[�`����ǉ�����
	 *
	 * @param ipInsCursor [in,out] �t�B�[�`���ǉ��p�t�B�[�`���J�[�\��
	 * @param ipBuffer    [in]     �ǉ�����t�B�[�`��
	 * @param aID         [out]    �ǉ������t�B�[�`���̃I�u�W�F�N�gID
	 * @param cIndex      [in]     �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param message     [in,out]     ���O�p���b�Z�[�W
	 *
	 * @retval true ����
	 * @retval false ���s�A�G���[
	 */
	bool fnInsertFeature(IFeatureCursorPtr& ipInsCursor, const IFeatureBufferPtr& ipBuffer, CComVariant &aID, long cIndex, CString& message);


	/**
	 * @brief 	Export���SiNDY��ɕύX��������ꂽ���A�X�V���t2��̔�r�Ŕ��f����֐��B
	 *          �֐����Ɩ߂�l���t���ۂ��C�����邪�A�X�V�ł��Ȃ��E�X�V���Ă͂����Ȃ�����false��Ԃ�
	 *
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cObjID		[in]	SDE��̃I�u�W�F�N�gID
	 * @param ipOrgFeature	[in]	SDE��̃t�B�[�`���iNULL�̎��͓����Ō�������j
	 * @param ipPgdbFeature	[in]	�ҏW��iPGDB�j�̃t�B�[�`��
	 * @param cChgType		[in]	�X�V���悤�Ƃ��Ă�����e�i���O�o�͗p�j
	 *
	 * @retval true		�ύX�Ȃ����͕ύX����ł��X�V�͉�
	 * @retval false	�폜����Ă��閔�͍����D�惂�[�h�ŕύX����Ă���
	 */
	bool fnSiNDYChange(long cIndex, long cObjID, IFeaturePtr ipOrgFeature, IFeaturePtr ipPgdbFeature, LPCTSTR cChgType);

	/**
	 * @brief 	���b�V�����܂������I�u�W�F�N�g���𔻒肷��֐��B
	 *
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cObjID	[in]	SDE��̃I�u�W�F�N�gID
	 * @param ipFeature	[in]	SDE��̃t�B�[�`���iNULL�̎��͓����Ō�������j
	 *
	 * @retval true		�ύX�Ȃ����͕ύX����ł��X�V�͉�
	 * @retval false	�폜����Ă��閔�͍����D�惂�[�h�ŕύX����Ă���
	 */
	bool fnOverMesh(long cIndex, long cObjID, IFeaturePtr ipFeature = NULL);

	/**
	 * @brief 	���b�V�����܂������I�u�W�F�N�g���𔻒肷��֐��B
	 *
	 * @param ipNewClass	[in]	�ҏW��̃t�B�[�`���N���X
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cObjID		[in]	SDE��̃I�u�W�F�N�gID
	 * @param ipNewFeature	[in]	�ҏW��̃t�B�[�`���i�폜�Ώۂ̎���NULL�j
	 * @param cStatus		[in]	�X�V�X�e�[�^�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnClipAndApend(IFeatureClassPtr ipNewClass, long cIndex, long cObjID, IFeaturePtr ipNewFeature, long cStatus);

	/**
	 * @brief 	���b�V�����܂����ł��Ȃ����̂̒P���X�V�֐��B
	 *
	 * @param ipNewFeature	[in]	�ҏW��̃t�B�[�`��
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cObjID		[in]	SDE��̃I�u�W�F�N�gID
	 * @param cStatus		[in]	�X�V�X�e�[�^�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnReflectToSiNDY(IFeaturePtr ipNewFeature, long cIndex, long cObjID, long cStatus);


	/**
	 * @brief �t�B�[�`������t�B�[�`���֌`����R�s�[
	 *
	 * @param ipTgtFeature [out] �R�s�[�����t�B�[�`��(IFeature�܂���IFeatureBuffer)
	 * @param ipSrcFeature [in]  �R�s�[����t�B�[�`��
	 * @param cIndex       [in]  �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cObjID       [in]  �I�u�W�F�N�gID(���O�o�͗p)
	 *
	 * @retval true ����
	 * @retval false ���s
	 */
	template <typename T>
	bool fnReflectShape( T& ipTgtFeature, const IFeaturePtr& ipSrcFeature, long cIndex, long cObjID );

	/**
	 * @brief 	���b�V���|���S���ŃN���b�v���čX�V�܂��͍폜����֐��B
	 *
	 * @param ipCursor	[in]	�X�V�Ώۂ����������J�[�\��
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnCutObject(IFeatureCursorPtr ipCursor, long cIndex);

	/**
	 * @brief 	�������ꂽ�I�u�W�F�N�g�𖳏����ō폜����֐��i��Ƀ|�C���g�p�j�B
	 *          DeleteSearchedRows�ł�������邪�A���O���o�����߁i�����H�j�ɂ���悤�Ȃ��́B�������ŋ߂͂قƂ�ǎg���Ȃ��͂�
	 *
	 * @param ipCursor	[in]	�폜�Ώۂ����������J�[�\��
	 * @param cIndex	[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnDeletePoint(IFeatureCursorPtr ipCursor, long cIndex);

	/**
	 * @brief 	���f�Ώۑ����̃R�s�[�֐��B
	 *
	 * @param ipTgtFeature	[in]	�R�s�[��t�B�[�`��
	 * @param ipSrcFeature	[in]	�R�s�[���t�B�[�`��
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cField		[out]	�X�V�����t�B�[���h���i���O�o�͗p�j
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnReflectAttr(IFeaturePtr ipTgtFeature, IFeaturePtr ipSrcFeature, long cIndex, CString& cField);

	/**
	 * @brief 	���b�V���|���S���ŃN���b�v�����`������߂�֐��B
	 *          fnGetRemakeShape�Ǝ��Ă���悤�Ɍ����邪�A������́A���b�V���|���S���Ő؂������c��̃W�I���g�������߂�
	 *
	 * @param ipFeature		[in]	�Ώۃt�B�[�`��
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cObjID		[in]	SDE��̃I�u�W�F�N�gID
	 * @param cForceMode	[in]	true�F�P���ɃN���b�v�����`������߂� false:���b�V�����E���֌W���邩���ȈՓI�ɋ��߂�i�H�j���O�Ɠ��e�̊֌W���킩��Ȃ��Ȃ���
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[
	 */
	IGeometryPtr fnGetClipShape(IFeaturePtr ipFeature, long cIndex, long cObjID, bool cForceMode = false);

	/**
	 * @brief 	���b�V�����E�̃I�u�W�F�N�g�ɑ΂��āA���Y���b�V���O�����Ɠ��Y���b�V�����̐؂�\����s���֐��B
	 * �@�@�@�@ �}���`�p�[�g�ɂȂ邱�Ƃ�����A�I���W�i��ID���������̂�1�ɂ܂Ƃ߂悤�Ƃ��邪�A�������ύX�ɂȂ��Ă�����̂�����ΐV�K�ǉ������Ƃ���
	 *
	 * @param ipNewClass	[in]	�ҏW��̃t�B�[�`���N���X
	 * @param ipFeature		[in]	�ҏW��̃t�B�[�`��
	 * @param cIndex		[in]	�e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param cObjID		[in]	SDE��̃I�u�W�F�N�gID
	 *
	 * @retval NULL�ȊO	���Z��̃W�I���g��
	 * @retval NULL		���s�A�G���[
	 */
	IGeometryPtr fnGetRemakeShape(IFeatureClassPtr ipNewClass, IFeaturePtr ipFeature, long cIndex, long cObjID);
	
	/**
	 * @brief ���b�V�����̉ƌ`�A�K���|���S�����폜����B���b�V�����܂����ꍇ�́A�͂ݏo�����̌`����c���B
	 *        GEOSPACE�C���|�[�g(-G�I�v�V�����w�莞)�̏����B
	 *
	 * @param cIndex [in] �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 *
	 * @retval true  �폜�A�`��ύX����
	 * @retval false �폜�A�`��ύX���s�A�G���[
	 */
	bool fnSiNDYDelete( const int cIndex );
	
	/**
	 * @brief fnSiNDYDelete�̒��Ŗ{���ɍ폜���s���֐�
	 *
	 * @param cIndex    [in] �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param ipFeature [in] �폜����t�B�[�`��
	 * @param oid       [in] �폜����t�B�[�`����OBJECTID (���O�o�͗p)
	 * @param message   [in] ���O�o�͗p���b�Z�[�W(�ƌ`�̏ꍇGEOSPACEID���o�͂���K�v������)
	 * @retval true  �폜����
	 * @retval false �폜���s�A�G���[
	 */
	bool fnSiNDYDeleteCore( const int cIndex, const IFeaturePtr& ipFeature, const long oid, const CString& message );
	
	/**
	 * @brief fnSiNDYDelete�̒��Ń��b�V���`��Ő؂��鏈��(�`��ύX)���s���֐�
	 *
	 * @param cIndex     [in] �e�[�u���̉��Ԗڂ̃t�B�[�`���N���X���������C���f�b�N�X
	 * @param ipFeature  [in] �؂���t�B�[�`��
	 * @param oid        [in] �؂���t�B�[�`����OBJECTID (���O�o�͗p)
	 * @param ipDiffGeom [in] �؂����̌`��
	 *
	 * @retval true  �`��ύX����
	 * @retval false �`��ύX���s�A�G���[
	 */
	bool fnSiNDYPutDifferenceGeom( const int cIndex, const IFeaturePtr& ipFeature, const long oid, const IGeometryPtr& ipDiffGeom );

};
