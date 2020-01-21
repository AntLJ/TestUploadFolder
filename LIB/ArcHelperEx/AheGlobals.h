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

/**
* @file
*
* @brief Arc Helper �O���[�o���֐���`�t�@�C��
*/
#if	!defined( __AHEGLOBALS_H__)
#define __AHEGLOBALS_H__	//!< �d���C���N���[�h�h�~

#if	defined( _UNICODE)
typedef std::map< std::wstring, LONG>	FIELDMAP;
#else
typedef std::map< std::string, LONG>	FIELDMAP;
#endif

#include "AheGlobalsCoord.h"	// URI ��͗p�֐��E���W�ϊ��Ȃ�
#include "AheGlobalsMisc.h"		// ���̑�
#include <ArcHelperEx/define.h>
#include <ArcHelperEx/define_enum.h>

/////////////////////////////////////////////////////////////////////////////
//
// �ݒ�l����
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief	�ݒ�ini�t�@�C���̃p�X�擾
 *
 * @param lpszPath				[out]		�ݒ�INI�t�@�C���p�X
 * @param nSize					[in]		�擾�o�b�t�@�T�C�Y
 *
 * @return �ݒ�INI�t�@�C���̃p�X�������ۑ�����o�b�t�@�T�C�Y
 */
int AheGetInifilePath( LPTSTR lpszPath, int nSize);

/**
 * @brief	�����i臒l�j�̐ݒ�� INI �t�@�C������擾����
 *
 * @param lpcszInifilePath		[in]		INI �t�@�C���̃p�X
 *
 * @return �����̒l
 */
UINT AheGetSelectTolerance( LPCTSTR lpcszInifilePath = NULL);

//////////////////////////////////////////////////////////////////////
// ���[�N�X�y�[�X�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////

/**
 * @brief ���[�N�X�y�[�X���擾����
 * @note IRow�AIDataset�AIWorkspace�̃C���^�[�t�F�[�X�������Ă���I�u�W�F�N�g�Ɍ���܂��B
 * @param ipUnk [in] IRow�AIDataset�AIWorkspace�̃C���^�[�t�F�[�X���������I�u�W�F�N�g
 * @return IWorkspace
 */
IWorkspacePtr AheGetWorkspace( IUnknown* ipUnk );

/**
 * @brief �����[�g���[�N�X�y�[�X���ǂ����𒲂ׂ�
 * @note IWorkspace::get_PathName()�Ńp�X�������Ă��邩�ǂ����Ŕ��肵�܂��B
 * @param ipWorkspace [in] ���[�N�X�y�[�X
 * @retval true �����[�g���[�N�X�y�[�X
 * @retval false �t�@�C���x�[�X�̃��[�N�X�y�[�X
 */
bool AheIsRemoteWorkspace( IWorkspace* ipWorkspace );

//////////////////////////////////////////////////////////////////////
// ����Ɋւ���֐�
//////////////////////////////////////////////////////////////////////

/**
* @brief ���H���C������
*
* @note ROAD_TABLE_NAME�̃e�[�u�����������C�����𔻒肷��B
*
* @param piLayer [in] ���肷�郌�C��
*
* @see ROAD_TABLE_NAME
*
* @retval TRUE ���H���C��
* @retval FALSE ���H���C���ł͂Ȃ�
*/
BOOL AheIsRoadLayer( ILayerPtr piLayer);

/**
* @brief �m�[�h���C������
*
* @note NODE_TABLE_NAME�̃e�[�u�����������C�����𔻒肷��B
*
* @param piLayer [in] ���肷�郌�C��
*
* @see NODE_TABLE_NAME
*
* @retval TRUE �m�[�h���C��
* @retval FALSE �m�[�h���C���ł͂Ȃ�
*/
BOOL AheIsNodeLayer( ILayerPtr piLayer);

/**
* @brief ���H�N���X����
*
* @note ROAD_TABLE_NAME�̃e�[�u���������t�B�[�`���N���X���𔻒肷��B
*
* @param piFeatureClass [in] �t�B�[�`���N���X
*
* @see ROAD_TABLE_NAME
*
* @retval TRUE ���H�t�B�[�`���N���X
* @retval FALSE ���H�t�B�[�`���N���X�ł͂Ȃ�
*/
BOOL AheIsRoadClass( IFeatureClassPtr piFeatureClass);

/**
* @brief �m�[�h�N���X����
*
* @note NODE_TABLE_NAME�̃e�[�u���������t�B�[�`���N���X���𔻒肷��B
*
* @param piFeatureClass [in] �t�B�[�`���N���X
*
* @see NODE_TABLE_NAME
*
* @retval TRUE �m�[�h�t�B�[�`���N���X
* @retval FALSE �m�[�h�t�B�[�`���N���X�ł͂Ȃ�
*/
BOOL AheIsNodeClass( IFeatureClassPtr piFeatureClass);

/**
* @brief ���H�t�B�[�`������
*
* @note ROAD_TABLE_NAME�̃e�[�u���������t�B�[�`�����𔻒肷��B
*
* @param piMap [in] �}�b�v�I�u�W�F�N�g�̃C���X�^���X
* @param piFeature [in] �t�B�[�`��
*
* @see ROAD_TABLE_NAME
*
* @retval TRUE ���H�t�B�[�`��
* @retval FALSE ���H�t�B�[�`���ł͂Ȃ�
*/
BOOL AheIsRoadFeature( IMapPtr piMap, IFeaturePtr piFeature);

/**
* @brief �m�[�h�t�B�[�`������
*
* @note NODE_TABLE_NAME�̃e�[�u���������t�B�[�`�����𔻒肷��B
*
* @param piMap [in] �}�b�v�I�u�W�F�N�g�̃C���X�^���X
* @param piFeature [in] �t�B�[�`��
*
* @see NODE_TABLE_NAME
*
* @retval TRUE �m�[�h�t�B�[�`��
* @retval FALSE �m�[�h�t�B�[�`���ł͂Ȃ�
*/
BOOL AheIsNodeFeature( IMapPtr piMap, IFeaturePtr piFeature);

/**
* @brief ���L���C������
* @param lpcszFeatureClassName [in] �t�B�[�`���N���X��
* @retval TRUE ���L���C��
* @retval FALSE ���L���C���ł͂Ȃ�
*/
BOOL AheIsChukiLayer( LPCTSTR lpcszFeatureClassName );

/**
* @brief POI���C������
* @param layerName [in] ���C����
* @retval TRUE POI���C��
* @retval FALSE POI���C���ł͂Ȃ�
*/
bool AheIsPoiLayer( const CString& layerName );

/**
* @brief �m�[�h���C�����擾
*
* @note TOC����ANODE_TABLE_NAME�̃e�[�u�����������C�����擾
*
* @param piMap			[in]			�}�b�v�I�u�W�F�N�g�̃C���X�^���X
* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
*
* @see NODE_TABLE_NAME
*
* @return ILayerPtr
*/
ILayerPtr AheGetNodeLayer( IMapPtr piMap, BOOL bEditableOnly = FALSE );

/**
* @brief ���H���C�����擾
*
* @note TOC����AROAD_TABLE_NAME�̃e�[�u�����������C�����擾
*
* @param piMap			[in]			�}�b�v�I�u�W�F�N�g�̃C���X�^���X
* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
*
* @see ROAD_TABLE_NAME
*
* @return ILayerPtr
*/
ILayerPtr AheGetRoadLayer( IMapPtr piMap, BOOL bEditableOnly = FALSE );

/**
* @brief �m�[�h�N���X���擾
*
* @note TOC����ANODE_TABLE_NAME�̃e�[�u���������t�B�[�`���N���X���擾
*
* @param piMap			[in]			�}�b�v�I�u�W�F�N�g�̃C���X�^���X
* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
*
* @see ROAD_TABLE_NAME
*
* @return IFeatureClassPtr
*/
IFeatureClassPtr AheGetNodeClass( IMapPtr piMap, BOOL bEditableOnly = FALSE );

/**
* @brief ���H�N���X���擾
*
* @note TOC����AROAD_TABLE_NAME�̃e�[�u���������t�B�[�`���N���X���擾
*
* @param piMap			[in]			�}�b�v�I�u�W�F�N�g�̃C���X�^���X
* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
*
* @see ROAD_TABLE_NAME
*
* @return IFeatureClassPtr
*/
IFeatureClassPtr AheGetRoadClass( IMapPtr piMap, BOOL bEditableOnly = FALSE );

/**
 * @brief �n���ꂽ�t�B�[�`���������C���ɑ�������̂��ǂ������ׂ�
 *
 * @param ipMap			[in]			Map
 * @param ipFeature		[in]			�����Ώۃt�B�[�`��
 * @retval FALSE �s�����C���ɑ����Ă���A���̓G���[
 * @retval TRUE �����C���ɑ����Ă���
 */
BOOL AheIsVisibleFeature( IMap* ipMap, IFeature* ipFeature );

/// �t�B�[�`���A�e�[�u�����������郌�C�����f�[�^�t���[�����猟������
std::list<CAdapt<ILayerPtr> > AheFindLayers( IMap* ipMap, IUnknown* ipUnk, bool bVislbleOnly = false, bool bEditableOnly = false );

/// ���C�����\������Ă��邩�ǂ������`�F�b�N����
BOOL AheIsVisibleLayer( IMapLayers* ipMap, ILayer* ipLayer );

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

/**
 * �e�[�u�������擾���邽�߂� IDataset �I�u�W�F�N�g���擾����
 *
 * IFeatureClass->get_Dataset() �œ�����I�u�W�F�N�g�Ƃ͈Ⴄ���Ƃɒ��ӁI�I
 *
 * @param ipUnk				[in]			IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @param ipDataset			[out]			�擾���ꂽ IDataset �I�u�W�F�N�g
 *
 * @retval TURE IDataset ���擾�ł���
 * @retval FALSE IDataset ���擾�ł��Ȃ�����
 */
BOOL AheGetDatasetForTableName( IMapPtr piMap, IUnknownPtr ipUnk, IDataset** ipDataset );

/**
 * @brief ���C�����̔�r
 *
 * @param piLayer			[in]			ILayer �̃C���X�^���X
 * @param lpcszLayerName	[in]			��r�Ώۂ̃��C����
 *
 * @retval TURE ��v
 * @retval FALSE �s��v
 */
BOOL AheCompareLayerName( ILayerPtr piLayer, LPCTSTR lpcszLayerName);

/**
 * @brief �t�B�[�`���N���X���̔�r
 *
 * @param piFeatureClass	[in]			IFeatureClass �̃C���X�^���X
 * @param lpcszLayerName	[in]			��r�Ώۂ̃��C����
 *
 * @retval TURE ��v
 * @retval FALSE �s��v
 */
BOOL AheCompareFeatureName( IFeatureClassPtr piFeatureClass, LPCTSTR lpcszLayerName);

/**
 * @brief �t�B�[���h������ IField �̃C���X�^���X�擾����
 *
 * @param piMap				[in]			IMap �̃C���X�^���X
 * @param lpszTableName		[in]			�擾������ IField �̃e�[�u����
 * @param lpszFieldName		[in]			�擾������ IField �̗�
 * @param ipRetField		[out]			�擾���ꂽ IField �̃C���X�^���X
 *
 * @retval TURE IFeild ���擾�ł���
 * @retval FALSE IFeild ���擾�ł��Ȃ�����
 */
BOOL AheGetFieldByFieldName( IMapPtr piMap, LPCTSTR lpszTableName, LPCTSTR lpszFieldName, IField** ipField );

/**
 * @brief �t�B�[���h������ IField �̃C���X�^���X�擾����
 *
 * @param ipTable			[in]			ITable �̃C���X�^���X
 * @param lpszFieldName		[in]			�擾������ ��
 * @param ipRetField		[out]			�擾���ꂽ IField �̃C���X�^���X
 *
 * @retval TURE IFeild ���擾�ł���
 * @retval FALSE IFeild ���擾�ł��Ȃ�����
 */
BOOL AheGetFieldByFieldName( ITablePtr ipTable, LPCTSTR lpszFieldName, IField** ipRetField );

/**
 * @brief ���R�[�h���ύX����Ă��邩
 *
 * @param ipRow			[in]			�������� IRow
 *
 * @retval TRUE �ύX����Ă���
 * @retval FALSE �ύX����Ă��Ȃ�
 */
BOOL AheRowIsChanged( _IRowPtr ipRow );

/**
 * @brief �`�󂪕ύX����Ă��邩
 *
 * @param ipFeature		[in]			�������� IFeature
 *
 * @retval TRUE �ύX����Ă���
 * @retval FALSE �ύX����Ă��Ȃ�
 */
BOOL AheShapeIsChanged( IFeaturePtr ipFeature );

/**
 * @brief �����t�B�[�`�����ǂ����̃e�X�g
 *
 * @param ipFeature1	[in]			��r���� IFeature �̃C���X�^���X
 * @param ipFeature2	[in]			��r��� IFeature �̃C���X�^���X
 *
 * @retval TURE ��v
 * @retval FALSE �s��v
 */
BOOL AheIsSameFeature( IFeaturePtr ipFeature1, IFeaturePtr ipFeature2 );

/**
 * @brief �����W�I���g�����ǂ����̃e�X�g
 *
 * @param ipGeom1		[in]			��r���� IGeometry �̃C���X�^���X
 * @param ipGeom2		[in]			��r��� IGeometry �̃C���X�^���X
 *
 * @retval TURE ��v
 * @retval FALSE �s��v
 */
BOOL AheIsSameGeometry( IGeometryPtr ipGeom1, IGeometryPtr ipGeom2 );

/**
 * @brief ����t�B�[���h�̒l���������ǂ���
 *
 * @param ipFeature1	[in]			��r���� IFeature �̃C���X�^���X
 * @param ipFeature2	[in]			��r��� IFeature �̃C���X�^���X
 * @param bstrFieldName [in]			��r����t�B�[���h��
 *
 * @retval TURE ��v
 * @retval FALSE �s��v
 */
BOOL AheIsSameField( IFeaturePtr ipFeature1, IFeaturePtr ipFeature2, BSTR bstrFieldName );

/**
 * @brief ����2�̃��R�[�h�̑����l���������ǂ���
 *
 * @param ipRow1		[in]			��r���郌�R�[�h�i_IRowPtr or IFeaturePtr�j
 * @param ipRow2		[in]			��r���郌�R�[�h�i_IRowPtr or IFeaturePtr�j
 *
 * @note SiNDY-e �I�ɕҏW�\�ȃt�B�[���h�̂݃`�F�b�N���܂�
 *       �����̃t�B�[�`���ɑ��݂���t�B�[���h�̂݃`�F�b�N���܂�
 * 
 * @retval TRUE  ���������l������
 * @retval FALSE �قȂ鑮���l������
 */
BOOL AheIsSameAttributes( _IRowPtr ipRow1, _IRowPtr ipRow2 );

/**
 * �ҏW�ł���t�B�[���h���ǂ����̔�����s��
 *
 * @param ipField		[in]			���肷��t�B�[���h
 * @param bForCopy		[in,optional]	�R�s�[�����ǂ���\n�R�s�[�̎��͂������̃t�B�[���h���ҏW�Ɣ��f����܂��B
 *
 * @return �ҏW�ł���t�B�[���h�Ȃ� TRUE�A�ҏW�ł��Ȃ��Ȃ� FALSE ��Ԃ�
 *
 * @note �啶���������֌W�Ȃ�����ł��܂��B
 *
 * @note ���̊֐��ł͖{���ҏW�ł��Ȃ��t�B�[���h�̂ق��ɁASiNDY-e �I�Ƀ��[�U�ɕύX���ė~�����Ȃ��t�B�[���h���܂�ł��܂��B
 * @note ����������邱�Ƃ��\�z�����̂ŁA���̏ꍇ�͓K�X�ǉ�����K�v������܂��B
 */
BOOL AheIsEditableField( IFieldPtr ipField, bool bForCopy = FALSE );

/**
 * @brief �n���ꂽ�t�B�[�`���̎�ނ�Ԃ�
 *
 * @param ipFeature	[in]			���ׂ� IFeature �̃C���X�^���X
 *
 * @return �t�B�[�`���̎�ށisindyeFeatureKinds�j
 */
sindyeFeatureKinds AheGetFeatureKinds( IFeaturePtr ipFeature );

/////////////////////////////////////////////////////////////////////////////
// �t�B�[���h�Ɋւ���֐�
/////////////////////////////////////////////////////////////////////////////
/*
 * @biref IFields���擾����
 * @param ipUnk [in] IFields�����o����I�u�W�F�N�g
 * �T�|�[�g����I�u�W�F�N�g�͈ȉ��̃C���^�[�t�F�[�X�������́F
 * @li IFields
 * @li _IRow
 * @li ITable
 * @li _ICursor
 */
IFieldsPtr AheGetFields( IUnknown* ipUnk );

/*
 * @brief �t�B�[���h���Ńt�B�[���h�C���f�b�N�X���擾����
 * @param ipFields [in] IFields
 * @param lpcszFieldName [in] �t�B�[���h��
 * �t�B�[���h���ƌ���������̑啶������������v���Ȃ��ꍇ���l��
 * ���A���[�v���񂵂Ĉ���t�B�[���h�����m�F���܂��B
 * �]���āAIFields::FindField�͎g�p���܂���B
 * @retval -1 �t�B�[���h����������Ȃ��A���͈�����NULL
 * @retval -1�ȊO �t�B�[���h�C���f�b�N�X
 */
long AheFindField( IFields* ipFields, LPCTSTR lpcszFieldName );

/**
 * @brief Shape�t�B�[���h��Z�l�������𔻒f����
 * @param ipField [in] �Ώۃt�B�[���h
 * @retval true Z�l�����ł���
 * @retval false Z�l�����ł͂Ȃ�
 */
bool AheHasZ(IFieldPtr ipField);

/**
 * @brief �t�B�[�`���N���X��Z�l�������𔻒f����
 * @param ipFeatureClass [in] �Ώۃt�B�[�`���N���X
 * @retval true Z�l�����ł���
 * @retval false Z�l�����ł͂Ȃ�
 */
bool AheHasZ(IFeatureClassPtr ipFeatureClass);

/**
 * @brief �W�I���g����Z�l�������𔻒f����
 * @param ipGeometry [in] �ΏۃW�I���g��
 * @retval true Z�l�����ł���
 * @retval false Z�l�����ł͂Ȃ�
 */
bool AheGeoHasZ(IGeometryPtr ipGeometry);

/**
 * @brief  �W�I���g����Z�l�������Ă���ꍇ�A�W�I���g����Z�l0��t�^����
 * @detail Z�l�������Ă��Ȃ��ꍇ�A�������Ȃ�
 * @note ���ꂪ�Ȃ���Z�l�ɔ񐔂��t�^����ċ�������Ȃ�P�[�X������
         �}���`�|�C���g�ɂ͑Ή����Ă��Ȃ��̂ł�����(���݂܂��� by ta_suga)
 * @param ipGeometry [in/out] �ΏۃW�I���g��
 * @retval true �t�^����
 * @retval false �t�^���s
 */
bool AheSetZeroToZ(IGeometryPtr ipGeometry);

//////////////////////////////////////////////////////////////////////
// �t�B�[�`���̑����Ɋւ���֐�
//////////////////////////////////////////////////////////////////////

/**
 * �t�B�[�`���ɑ������Z�b�g����
 * 
 * @note Store() �͂��܂���
 *
 * @param ipRow				[in]	�Z�b�g����t�B�[�`���i���R�[�h�j
 * @param lpcszFieldName	[in]	�Z�b�g����t�B�[���h��
 * @param vaValue			[in]	�Z�b�g����l
 *
 * @retval TRUE �Z�b�g�ł���
 * @retval FALSE �Z�b�g�ł��Ȃ�����
 */
BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, const VARIANT& vaValue );
BOOL AheSetAttribute( IRowBufferPtr ipRow, long lIndex, const VARIANT& vaValue );
BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, long lValue );
BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, LPCTSTR lpcszValue );
BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, double dValue );
inline BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, long lValue ){
	return AheSetAttribute( ipRow, lpcszFieldName, CComVariant(lValue) );
}
inline BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, LPCTSTR lpcszValue ){
	return AheSetAttribute( ipRow, lpcszFieldName, CComVariant(lpcszValue) );
}
inline BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, const double dValue ){
	return AheSetAttribute( ipRow, lpcszFieldName, CComVariant(dValue) );
}

/**
 * �t�B�[�`�����瑮�����擾����
 *
 * @param ipRow				[in]	�t�B�[�`���i���R�[�h�j
 * @param lpcszFieldName	[in]	�擾����t�B�[���h��
 *
 * @retval �擾�ł����l
 */
CComVariant AheGetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName );
CComVariant AheGetAttribute( IRowBufferPtr ipRow, long lIndex );

/**
 * @brief �������R�s�[����
 *
 * @param ipSourceRow       [in]			�R�s�[��
 * @param ipDestRow         [in]			�R�s�[��
 * @param bIsSplitMode		[in,optional]	�t�B�[�`���������̑����R�s�[�̏ꍇ��TRUE���w�肷��i�f�t�H���g�FFALSE�j
 *
 * @return ����I�������� TRUE
 */
BOOL AheCopyAttribute( IRowBufferPtr ipSourceRow, IRowBufferPtr ipDestRow, BOOL bIsSplitMode = FALSE );

#ifndef __BUILD_SINDYRULE__
/**
 * @brief �t�B�[�`���̏C�������X�V
 *
 * @param piRow					[in]			�C�������X�V����t�B�[�`��
 * @param emUpdateType			[in]			�C�����R
 * @param lpSystemTime			[in]			�V�X�e���̕ێ����鎞��
 *
 * @retval TURE �X�V����
 * @retval FALSE �X�V�ُ�
 */
BOOL AheSetModifyData( IRowBufferPtr piRow, sindyUpdateType emUpdateType, LPSYSTEMTIME lpSystemTime = NULL);
#endif //__BUILD_SINDYRULE__

/**
 * @brief �t�B�[�`���Ƀf�t�H���g�l��ݒ�
 *
 * @note �t�B�[�`����V�K�ɍ쐬�����ꍇ�͕K���Ă�ł�������
 *
 * @param ipRow					[in]			IRow�AIFeature
 *
 * @retval TRUE �X�V����
 * @retval FALSE �n���ꂽ�t�B�[�`���� NULL
 */
BOOL AheSetDefaultData( IRowBufferPtr ipRow );

/**
 * @brief �V�K�t�B�[�`���i���R�[�h�j���쐬
 *
 * @param ipUnk					[in]			IFeature�AIRow�AITable�AIDataset�AIFeatureClass �Ȃ� IDataset �����o�������
 * @param ipGeom				[in]			�W�I���g���i�t�B�[�`���̏ꍇ�̂ݗ��p�����f�t�H���g�� NULL�j
 * @param bSetDefaultData		[in]			�f�t�H���g�l�����邩�ǂ����i�f�t�H���g�� TRUE�j
 *
 * @note ArcGIS 8.2 ����A�W�I���g���̒n���Q�Ƃ������Ƃ��킹�Ȃ��Ƃ����悤�ɂȂ������߁A�쐬
 * @note ipGeom �͒n���Q�Ƃ����킹�Ă��Ȃ��Ă� OK�B�K�� IGeoDataset::get_SpatialReference() �Œn���Q�Ƃ��擾���ăW�I���g���ɗ^���Ă���B
 *
 * @return �쐬���ꂽ IRow �܂��� IFeature
 */
_IRowPtr AheCreateRow( IUnknownPtr ipUnk, IGeometryPtr ipGeom = NULL, BOOL bSetDefaultData = TRUE );

/**
 * @brief �V�K�t�B�[�`���i���R�[�h�j���쐬
 *
 * @param ipUnk					[in]			IFeature�AIRow�AITable�AIDataset�AIFeatureClass �Ȃ� IDataset �����o�������
 * @param ipGeom				[in]			�W�I���g���i�t�B�[�`���̏ꍇ�̂ݗ��p�����f�t�H���g�� NULL�j
 * @param bSetDefaultData		[in]			�f�t�H���g�l�����邩�ǂ����i�f�t�H���g�� TRUE�j
 *
 * @note ArcGIS 8.2 ����A�W�I���g���̒n���Q�Ƃ������Ƃ��킹�Ȃ��Ƃ����悤�ɂȂ������߁A�쐬
 * @note ipGeom �͒n���Q�Ƃ����킹�Ă��Ȃ��Ă� OK�B�K�� IGeoDataset::get_SpatialReference() �Œn���Q�Ƃ��擾���ăW�I���g���ɗ^���Ă���B
 *
 * @return �쐬���ꂽ IRowBuffer �܂��� IFeatureBuffer
 */
IRowBufferPtr AheCreateRowBuffer( IUnknownPtr ipUnk, IGeometryPtr ipGeom = NULL, BOOL bSetDefaultData = TRUE );

/**
 * @brief �t�B�[�`���Ɍ`��𔽉f������
 *
 * @note IFeature::putref_Shape() �Ō`�󂪕ς��ꍇ�����������߁A����̉����
 *
 * @param ipFeature				[in]			IFeatureBufferPtr
 * @param ipGeometry			[in]			���f���������W�I���g��
 *
 * @retval TRUE ���f����
 * @retval FALSE ���f���s
 */
BOOL AheSetShape( IFeatureBufferPtr ipFeature, IGeometryPtr ipGeometry );
//////////////////////////////////////////////////////////////////////
// �f�o�b�O�p�֐�
//////////////////////////////////////////////////////////////////////
/**
 * IRow �̃g���[�X������i�f�o�b�O�ł̂݁j
 *
 * @param ipRow			[in]	IRowBufferPtr
 * @param lpcszMessage	[in]	���b�Z�[�W
 */
void AheTraceRow( IRowBufferPtr ipRow, LPCTSTR lpcszMessage );

//////////////////////////////////////////////////////////////////////
// ���W�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief �s�N�Z��->LonLat�ϊ�
 *
 * @param pIDisplay	[in]			IDisplay �̃C���X�^���X
 * @param dPixelUnits		[in]			�s�N�Z���l
 *
 * @return ���W��ł̋���
 */
DOUBLE AheConvertPixelsToMapUnits( IDisplayPtr pIDisplay, double dPixelUnits);

/**
 * @brief XY -> IPoint�ϊ�
 *
 * @param pIDisplay	[in]			IDisplay �̃C���X�^���X
 * @param X					[in]			X ���W�l
 * @param Y					[in]			Y ���W�l
 * @param ipPoint			[out]			�ϊ���� IPoint �̃C���X�^���X
 */
void AheQueryMapPoint( IDisplayPtr pIDisplay, const LONG X, const LONG Y, IPoint** ipPoint);

//////////////////////////////////////////////////////////////////////
// �h���C���Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief �w�肳�ꂽ���̂̃R�[�h�l�h���C�����擾����
 *
 * @param ipCodedValueDomain		[in]			ICodedValueDomain �̃C���X�^���X
 * @param lpszCodedValueDomainName	[in]			�R�[�h�l�h���C���̖���
 * @param va						[out]			�擾�����l
 *
 * @retval TURE �R�[�h�l�h���C��������
 * @retval FALSE �R�[�h�l�h���C�����Ȃ�
 */
BOOL AheCodedValueDomainName2VARIANT( ICodedValueDomainPtr ipCodedValueDomain, LPCTSTR lpszCodedValueDomainName, VARIANT& va );

/**
 * @brief �w�肳�ꂽ�R�[�h�l�h���C���̖��̎擾����
 *
 * @param ipCodedValueDomain		[in]			ICodedValueDomain �̃C���X�^���X
 * @param va						[in]			�R�[�h�l�h���C���̖���
 * @param lpszCodedValueDomainName	[out]			�擾�����l
 *
 * @retval TURE �R�[�h�l�h���C��������
 * @retval FALSE �R�[�h�l�h���C�����Ȃ�
 */
BOOL AheVARIANT2CodedValueDomainName( ICodedValueDomainPtr ipCodedValueDomain, VARIANT va, CString& lpszCodedValueDomainName );

/**
 * �I�u�W�F�N�g�ƃt�B�[���h�����瑊������R�[�h�l�h���C���̕�����ƒl���擾����
 *
 * @param ipUnk			[in]	IRow�܂���IFeature�I�u�W�F�N�g
 * @param lpszFieldName	[in]	�Ώۃt�B�[���h��
 * @param strDomainName	[out]	�擾���ꂽ�R�[�h�l�h���C���̕�����
 * @param vaValue		[out]	�擾���ꂽ�l
 *
 * @retval TURE ����I��
 * @retval FALSE �ُ�I��
 */
BOOL AheGetDomainNameAndValue( IUnknownPtr ipUnk, LPCTSTR lpszFieldName, CString& strDomainName, VARIANT& vaValue );

/**
 * @brief �I�u�W�F�N�g�ƃt�B�[���h�����瑊������R�[�h�l�h���C���̕�������擾����
 *
 * @param ipUnk			[in]	IRow�܂���IFeature�I�u�W�F�N�g
 * @param lpszFieldName	[in]	�Ώۃt�B�[���h��
 *
 * @return �R�[�h�l�h���C���̕�������擾�i�R�[�h�l�h���C���̕�������擾�ł��Ȃ������ꍇ�͒l�����̂܂ܕԂ��j
 */
CComVariant AheGetDomainName( IUnknownPtr ipUnk, LPCTSTR lpszFieldName );

/**
 * �e�[�u�����ƃt�B�[���h������h���C�����擾����
 *
 * @param lpszTableName			[in]	�e�[�u����
 * @param lpszFieldName			[in]	�t�B�[���h��
 * @param ipDomain				[out]	�h���C��
 *
 * @retval TURE �h���C�������擾�ł���
 * @retval FALSE �h���C�������擾�ł��Ȃ�����
 */
BOOL AheGetDomainByFieldName( IMapPtr piMap, LPCTSTR lpszTableName, LPCTSTR lpszFieldName, IDomain** ipDomain );
//////////////////////////////////////////////////////////////////////
// ����Ɋւ���֐�
//////////////////////////////////////////////////////////////////////

/**
 * �m�[�h�t�B�[�`�����I�[���ǂ����̃`�F�b�N
 *
 * @param ipNodeFeature [out] ���ׂ�m�[�h���܂܂�Ă���t�B�[�`��
 *
 * @retval TURE �[�_
 * @retval FALSE �[�_�ȊO
 */
BOOL AheIsEndNode( IFeaturePtr ipNodeFeature );

//////////////////////////////////////////////////////////////////////
// �t�B�[�`���I���Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief ISpatialFilter ���쐬����
 *
 * @param ipGeom			[in]			�I��͈�
 * @param ipFeatureClass	[in]			�t�B�[�`���N���X
 * @param spatialRel        [optional]		�I����@
 * @param lpcszSubFields	[in,optional]	�擾����t�B�[���h��
 * @param lpcszWhereClause	[in,optional]	WHERE ��
 *
 * @return IQueryFilterPtr
 */
IQueryFilterPtr AheGetSpatialFilter( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL/*, ISpatialReferencePtr ipSpRef = NULL*/ );
/**
 * �^����ꂽ�W�I���g���͈͓̔��ɑ��݂���t�B�[�`��������
 *
 * ipFeatureCursor �̓C���X�^���X���쐬�����ɓn���Ă�������
 *
 * @param ipGeom            [in]           �I��͈�
 * @param ipFeatureClass    [in]     	   �t�B�[�`���N���X
 * @param ipFeatureCursor   [retval]       �R���N�V����
 * @param spatialRel        [optional]	   �I����@
 * @param lpcszSubFields	[in,optional]  �擾����t�B�[���h��
 * @param lpcszWhereClause	[in,optional]  WHERE ��
 *
 * @return �����ł����t�B�[�`���̐�
 */
LONG AheSelectByShapeAndGetCount( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ipFeatureCursor, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bRecycling = FALSE );

/**
 * �^����ꂽ�W�I���g���͈͓̔��ɑ��݂���t�B�[�`��������
 *
 * @param ipGeom            [in]           �I��͈�
 * @param ipFeatureClass    [in]     	   �t�B�[�`���N���X
 * @param spatialRel        [optional]	   �I����@
 * @param lpcszSubFields	[in,optional]  �擾����t�B�[���h��
 * @param lpcszWhereClause	[in,optional]  WHERE ��
 * @param bRecycling		[in,optional]  �f�t�H���g�FFALSE
 *
 * @return IFeatureCursorPtr
 */
IFeatureCursorPtr AheSelectByShape( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bRecycling = FALSE );

/**
 * �^����ꂽ�W�I���g���͈͓̔��ɑ��݂���t�B�[�`��������
 *
 * @param ipGeom            [in]           �I��͈�
 * @param ipFeatureClass    [in]     	   ���C��
 * @param spatialRel        [optional]	   �I����@
 * @param lpcszSubFields	[in,optional]  �擾����t�B�[���h��
 * @param lpcszWhereClause	[in,optional]  WHERE ��
 * @param bRecycling		[in,optional]  �f�t�H���g�FFALSE
 *
 * @see AheSelectByShape
 *
 * @return IFeatureCursorPtr
 */
IFeatureCursorPtr AheSelectByShapeFromLayer( IGeometryPtr ipGeom, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bRecycling = FALSE );
//////////////////////////////////////////////////////////////////////
// �Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief �t�B�[�`���̃|�C���g����擾����
 *
 * @param ipFeature				[in]			�ΏۂƂȂ� IFeature �̃C���X�^���X
 * @param ipRetCol				[out]			�t�B�[�`���̃|�C���g�R���N�V�����̃C���X�^���X
 */
void AheGetFeatureVertex( IFeaturePtr ipFeature, IPointCollection** ipRetCol);

/**
 * @brief �t�B�[�`���̂ǂ̕����Ƀq�b�g���邩���ׂ�
 *
 * @param piFeature				[in]			�ΏۂƂȂ� IFeature �̃C���X�^���X
 * @param piTestPoint			[in]			�������� IPoint �̃C���X�^���X			
 * @param dbTorelance			[in]			����
 * @param piPoint				[out]			�q�b�g���� IPoint �̃C���X�^���X
 * @param pdbHitDist			[out,optional]	�q�b�g�����_�ƃe�X�g�_�Ƃ̋���
 * @param plPartIndex			[out,optional]	�q�b�g�����ӂ̃C���f�b�N�X
 * @param plVertexIndex			[out,optional]	�q�b�g�������_�̃C���f�b�N�X
 * @param testType				[in,optional]	�e�X�g����^�C�v�i�f�t�H���g�F���_�A�Ӂj
 *
 * @return �q�b�g�^�C�v
 */
esriGeometryHitPartType AheTestFeatureGeometryHit( IFeaturePtr piFeature, IPointPtr piTestPoint, double dbTorelance, IPoint** piPoint, double* pdbHitDist = NULL, LONG* lPartIndex = NULL, LONG* lVertexIndex = NULL, esriGeometryHitPartType testType = (esriGeometryHitPartType)(esriGeometryPartVertex|esriGeometryPartBoundary) );

/**
 * @brief �W�I���g���̂ǂ̕����Ƀq�b�g���邩���ׂ�
 *
 * @param ipGeom				[in]			�ΏۂƂȂ� IGeometry �̃C���X�^���X
 * @param piTestPoint			[in]			�������� IPoint �̃C���X�^���X			
 * @param dwTorelance			[in]			����
 * @param piPoint				[out]			�q�b�g���� IPoint �̃C���X�^���X
 * @param pdwHitDist			[out,optional]	�q�b�g�����_�ƃe�X�g�_�Ƃ̋���
 * @param plHitPartIndex		[out,optional]	�q�b�g�����p�[�g�̃C���f�b�N�X
 * @param plHitSegmentIndex		[out,optional]	�q�b�g�����ӂ̃C���f�b�N�X
 * @param testType				[in,optional]	�e�X�g����^�C�v�i�f�t�H���g�F���_�A�Ӂj
 *
 * @return �q�b�g�^�C�v
 */
esriGeometryHitPartType AheTestGeometryHit( IGeometryPtr ipGeom, IPointPtr ipTestPoint, double dwTorelance, IPoint** ipPoint, double* pdwHitDist, LONG* plHitPartIndex, LONG* plHitSegmentIndex, esriGeometryHitPartType testType = (esriGeometryHitPartType)(esriGeometryPartVertex|esriGeometryPartBoundary) );

/**
 * @brief �t�B�[�`���̒��_�Ƀq�b�g�����ꍇ�͂��̃C���f�b�N�X�ԍ����A�q�b�g���Ȃ������ꍇ�� -1 ��Ԃ�
 *
 * @note TestGeometryHit() �̊ȈՔ� (2 �́A�K��lVertexIndex��Ԃ��B���L�Ŏg�p)
 *
 * @param ipFeature				[in]			�ΏۂƂȂ� IFeature �̃C���X�^���X
 * @param ipPoint				[in]			�e�X�g���� IPoint �̃C���X�^���X
 * @param dTorelance			[in]			����
 *
 * @retval -1�ȊO �q�b�g�����C���f�b�N�X�ԍ�
 * @retval -1 �q�b�g���Ȃ�����
 */
LONG AheHitTestVertexIndex( IFeaturePtr ipFeature, IPointPtr ipPoint, double dTorelance);
LONG AheHitTestVertexIndex2( IFeaturePtr ipFeature, IPointPtr ipPoint, double dTorelance);

/////////////////////////////////////////////////////////////////////////////
//
// �t�B�[�`���ҏW�Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief �t�B�[�`���̌`�󂩂�|�C���g���폜����
 *
 * @param ipFeature				[in]	�t�B�[�`��
 * @param lIndex				[in]	�\���_�C���f�b�N�X
 *
 * @return IPointCollectionPtr
 */
IPointCollectionPtr AheRemovePoint( IFeaturePtr ipFeature, LONG lIndex );

/**
 * ���C���t�B�[�`�����|�C���g�W�I���g���ŃX�v���b�g����
 *
 *   ��ɓ��H���X�v���b�g����̂Ɏg�p���܂��B
 *   SplitRoadByPoint() �Ŏg�p����܂��B�ʏ�g�p����ꍇ��
 *   SplitRoadByPoint() ���g�p���Ă��������B
 *
 * @param ipPointGeom   [in]  �X�v���b�g�|�C���g�W�I���g��
 * @param ipLineFeature [in]  �X�v���b�g�Ώۂ̃��C���t�B�[�`��
 *
 * @return �X�v���b�g������̃t�B�[�`���Z�b�g
 */
//_ISetPtr AheSplitLineByPoint( IFeaturePtr ipLineFeature,  IGeometryPtr ipSplitPointGeom);

/**
 * @brief ���C���t�B�[�`�����|�C���g�R���N�V�����ŃX�v���b�g����
 *
 * @param piLineFeature			[in]			�X�v���b�g���������C���t�B�[�`��
 * @param piSplitPoipnts		[in]			�X�v���b�g���� IPointCollection �̃C���X�^���X
 *
 * @return �X�v���b�g�������ʂ̃t�B�[�`���S���i�[���� ISet �̃C���X�^���X
 */
_ISetPtr AheSplitLineByPoints( IFeaturePtr piLineFeature, IPointCollectionPtr piSplitPoipnts);

/**
 * @brief �t�B�[�`������o�b�t�@�t���|���S�����擾����
 *
 * @param ipFeature			[in]			�|���S�����擾����Ώۃt�B�[�`��
 * @param dbBufferSize		[in]			�o�b�t�@����T�C�Y
 * @param ipPolygon			[out]			�擾�����|���S��
 */
void AheFeature2Polygon( IFeaturePtr ipFeature, double dbBufferSize, IPolygon** ipPolygon);

/**
 * @brief �W�I���g������o�b�t�@�t���|���S�����擾����
 *
 * @param ipGeom			[in]			�|���S�����擾����ΏۃW�I���g��
 * @param dbBufferSize		[in]			�o�b�t�@����T�C�Y
 * @param ipPolygon			[out/optional]	�擾�����|���S��
 */
IPolygonPtr AheGeometry2Polygon( IGeometryPtr ipGeom, double dbBufferSize, IPolygon** ipPolygon=NULL);

/**
 * @brief �t�B�[���h���̂ƃG�C���A�X�̃C���f�b�N�X�}�b�v���擾���܂��B
 *
 * @param piFeature			[in]			�C���f�b�N�X�}�b�v���擾����Ώۃt�B�[�`��
 * @param cNameMap			[out]			�t�B�[���h���̂Ƃ��̃C���f�b�N�X�̃}�b�v�R���e�i
 * @param cAliasMap			[out]			�G�C���A�X���̂Ƃ��̃C���f�b�N�X�̃}�b�v�R���e�i
 *
 * @return �t�B�[�`���̃t�B�[���h��
 */
LONG AheGetFieldMap( IFeaturePtr piFeature, FIELDMAP& cNameMap, FIELDMAP& cAliasMap);

/**
 * �|�C���g����o�b�t�@����������`��Ԃ�
 *
 *   �w�肳�ꂽ�|�C���g�ƒ��������`���쐬���܂��B
 *   �t�B�[�`���̑I���Ȃǂɑ����g�p����Ă��܂��B
 *
 * @param ipPoint [in]  �|�C���g
 * @param pixel   [in]  �s�N�Z��
 * @param IEnvelopePtr
 */
BOOL AheGetEnvelope( IPointPtr ipPoint, IDisplayPtr piScrnDisp, LONG pixel, IEnvelope** ppiEnvelope);

/**
 * �|�C���g����o�b�t�@����������`��Ԃ�
 *
 *   �w�肳�ꂽ�|�C���g�ƒ��������`���쐬���܂��B
 *   �t�B�[�`���̑I���Ȃǂɑ����g�p����Ă��܂��B
 *
 * @param ipPoint      [in]  �|�C���g
 * @param dTolerance   [in]  ��
 * @return IEnvelopePtr
 */
BOOL AheGetEnvelope( IPointPtr ipPoint, DOUBLE dTolerance, IEnvelope** ppiEnvelope);

/**
 * @brief	�w��̌`�󂪁A�Q�_�Ԃ����Ԓ����ƌ�������_����擾
 *
 * @param piBaseGeometry		[in]		�`�F�b�N�Ώۂ̌`��
 * @param dbFromX				[in]		�`�F�b�N�Ώې����̊J�n���W
 * @param dbFromY				[in]		�`�F�b�N�Ώې����̊J�n���W
 * @param dbToX					[in]		�`�F�b�N�Ώې����̏I�_���W
 * @param dbToY					[in]		�`�F�b�N�Ώې����̏I�_���W
 * @param piPointCollection		[out]		�����_�W��
 *
 * @return ���TURE��Ԃ�
 */
//BOOL AheGetPointsCrossedLine( IGeometryPtr piBaseGeometry, double dbFromX, double dbFromY, double dbToX, double dbToY, IPointCollectionPtr piPointCollection);

/**
 * ���x���t�����b�Z�[�W�o�͊֐�
 *
 *   �w�肳�ꂽ���x���ɂ��������āA���b�Z�[�W���o�͂���B
 *
 * @param lpcszMsg      [in]  ���b�Z�[�W
 * @param lpcszCap   [in]  �L���v�V����
 * @param uType   [in]  ���b�Z�[�W�{�b�N�X�̓��e�Ɠ���
 * @param uLevel   [in]  �o�̓��x��
 * @return select
 */
int AheMessageBox( LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel = WARNLEVEL_MIDDLE);

/**
 * @brief	���x���t�����b�Z�[�W�o�͊֐�
 *
 * @param hWnd				[in]		���b�Z�[�W�{�b�N�X�ɓn���E�B���h�E�n���h��
 * @param lpcszMsg			[in]		���b�Z�[�W�̓��e
 * @param lpcszCap			[in]		���b�Z�[�W�{�b�N�X�̃L���v�V����
 * @param uType				[in]		���b�Z�[�W�{�b�N�X�̃{�^���̃^�C�v
 * @param uLevel			[in]		���b�Z�[�W�̃��x��
 *
 * @return ���[�U�[�̑I��l
 */
int AheMessageBox( HWND hWnd, LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel = WARNLEVEL_MIDDLE);

/**
* @brief ZooMoveTool�̃L�[�ړ��g��k������
*
* @note ZooMoveTool�Ŏ��������L�[�{�[�h�ł̏��������𒊏o
*
* @param piActView [in] �r���[
* @param lKeyCode [in] ���z�L�[�R�[�h
* @param lShift [in] �C���L�[�H�X�e�[�^�X
* @param pblHandled [out] �����ς݃t���O
*
* @retval TURE ����I��
* @retval FALSE �ُ�I��
*/
BOOL AheZooMoveKeyDownProc( IActiveViewPtr piActView, LONG lKeyCode, LONG lShift, BOOL* pblHandled);

/**
* @brief �J�[�\���ʒu�ɉ�ʂ̒��S���ړ�����
*
* @note ��2������[Bug 3772]�g�傪�����Ȃ��Ȃ�o�O�ւ̑Ή��������ۂɁA��UTransform�C�x���g���Ƃ߂āAVisibleBounds�ŗ̈��
*       �ݒ���s�����Ƃ���A[Bug 5495]���������Ă��܂��A�C�x���g���Ƃ߂Ă��܂��ƕ`��̕ύX�͂����Ă��A�����I�ɂ͑O�̍��W�ʒu��
*       ���邱�Ƃ��킩�������߁AMoveCenterToMousePointer�̌�ɃC�x���g�𔭍s���鏈�����Ȃ��ꍇ�ɂ́A�C�x���g�Ƃ߂Ȃ����Ƃɂ���
*
* @param piActiveView [in] �}�b�v�̃r���[
* @param bSuppressEvents [in] Transform�C�x���g���Ƃ߂邩�ۂ��i�f�t�H���g(true)�̓C�x���g���Ƃ߂�j
*
* @retval TRUE ����I��
* @retval FALSE �ُ�I��
*/
BOOL AheMoveCenterToMousePointer( IActiveViewPtr piActiveView, bool bSuppressEvents = true );

/**
* @brief �J�[�\�����}�b�v�̃Z���^�[�Ɉړ�
*
* @param piActiveView [in] �}�b�v�̃r���[
*
* @retval TRUE ����I��
* @retval FALSE �ُ�I��
*/
BOOL AheMoveCursor2Center( IActiveViewPtr piActiveView);

/**
* @brief �w��X�e�b�v�����̈ړ�
*
* @param piActiveView [in] �}�b�v�̃r���[
* @param nXStep [in] �ړ��P��
* @param nYStep [in] �ړ��P��
* @retval TRUE ����I��
* @retval FALSE �ُ�I��
*/
BOOL AheShiftMap( IActiveViewPtr piActView, int nXStep, int nYStep);

// MapFAN��ZM�l�ɑ������g�嗦�ł̏k������
void AheZoomUp( IActiveViewPtr piActiveView);
void AheZoomDown( IActiveViewPtr piActiveView);

// �ĕ`�惂�[�h���擾
bool AheGetRefreshMode();

/**
* @brief ������}���ĕ`��
*
* @param piActiveView [in] �}�b�v�̃r���[
* @param ipEnv [in] �ĕ`���`
*/
void AheRefresh(IActiveViewPtr piActView, IEnvelopePtr ipEnv=NULL);

/////////////////////////////////////////////////////////////////////////////
//
// ���W�X�g���̊�{�A�N�Z�X�֐�
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief	���W�X�g���̃L�[���쐬����
 *
 * @param lpszKey				[in]		�L�[��
 * @param hKey					[in]		�L�[�̃n���h��
 *
 * @return �쐬���ꂽ REG KEY
 */
HKEY  EmxCreateRegKey( LPCTSTR lpszKey, HKEY hKey );

/**
 * @brief	�w�肳�ꂽ���W�X�g���̃L�[�̒l�iDWORD�j����������
 *
 * @param lpszValueName			[in]		�l
 * @param hKey					[in]		�L�[�̃n���h��
 *
 * @retval TURE �������߂�
 * @retval FALSE �������ُ݈�
 */BOOL  EmxSetRegDword( LPCTSTR lpszValueName, DWORD dwData, HKEY hKey );

/**
 * @brief	�w�肳�ꂽ���W�X�g���̃L�[�̒l�iDWORD�j���擾����
 *
 * @param lpszValueName			[in]		�l�̖��O
 * @param lpdwBuff				[out]		�擾�����l
 * @param hKey					[in]		�L�[�̃n���h��
 *
 * @retval TURE �ǂݍ��߂�
 * @retval FALSE �ǂݍ��ُ݈�
 */
BOOL  EmxGetRegDword( LPCTSTR lpszValueName, DWORD* lpdwBuff, HKEY hKey );

/**
 * @brief	�w�肳�ꂽ���W�X�g���̃L�[�̒l�i������j����������
 *
 * @param lpszValueName			[in]		�l�̖��O
 * @param lpdwBuff				[out]		�擾����������
 * @param hKey					[in]		�L�[�̃n���h��
 *
 * @retval TURE �������߂�
 * @retval FALSE �������ُ݈�
 */
BOOL  EmxSetRegString( LPCTSTR lpszValueName, LPCTSTR lpcszString, HKEY hKey );

/**
 * @brief	�w�肳�ꂽ���W�X�g���̃L�[�̒l�i������j���擾����
 *
 * @param lpszValueName			[in]		�l�̖��O
 * @param lpdwBuff				[out]		�擾����������
 * @param hKey					[in]		�L�[�̃n���h��
 *
 * @retval TURE �ǂݍ��߂�
 * @retval FALSE �ǂݍ��ُ݈�
 */
BOOL  EmxGetRegString( LPCTSTR lpszValueName, LPTSTR lpReturnedString, DWORD nSize, HKEY hKey);
BOOL  EmxGetRegString( LPCTSTR lpszValueName, LPTSTR lpReturnedString, HKEY hKey);

 /**
  * @brief ��ԎQ�Ƃ̍ŏ����������߂�
  *
  * @param ipSpRef			[in]			��ԎQ��
  *
  * @retval DOUBLE �ŏ������iipSpRef �� NULL �̏ꍇ�� -1.0�j
  */
DOUBLE AheGetXYDomain1UnitLength( ISpatialReferencePtr ipSpRef );

/**
 * @brief ��ԎQ�Ƃ̐��x���ׂ��������擾����
 *
 * @param ipSpRef1			[in]			��ԎQ��
 * @param ipSpRef2			[in]			��ԎQ��
 *
 * @return ISpatialReferencePtr
 */
ISpatialReferencePtr AheGetMinimumSpatialReference( ISpatialReferencePtr ipSpRef1, ISpatialReferencePtr ipSpRef2 );

#endif	//__AHEGLOBALS_H__
