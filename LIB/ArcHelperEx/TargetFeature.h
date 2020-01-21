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
* @file TargetFeature.h
*
* @brief �t�B�[�`���N���X�̒�`�t�@�C��
*/
#if	!defined( __TARGETFEATURE_H__)
#define	__TARGETFEATURE_H__	//!< �d���C���N���[�h�h�~

#include "define.h"
#include "AheGlobals.h"
#include "AheLayerFunctions.h"
#include "AheGeometryOp.h"
#include "TargetFeature.h"
#include <algorithm>

class CErrObjContainer;

#if	defined( _UNICODE)
/**
* @var FIELDMAP
*
* @note wstring ��UNICODE�Ή���
*
* @brief �f�[�^�Z�b�g�t�B�[���h���ƃC���f�b�N�X�Δ�R���|�[�l���g
*/
typedef std::map< std::wstring, LONG>	FIELDMAP;
#else
/**
* @var FIELDMAP
*
* @note string ��ASCII�Ή���
*
* @brief �f�[�^�Z�b�g�t�B�[���h���ƃC���f�b�N�X�Δ�R���|�[�l���g
*/
typedef std::map< std::string, LONG>	FIELDMAP;
#endif

/**
* @class CTargetItem
*
* @brief ����ΏۊǗ��N���X
*
* @note �I�������t�B�[�`���������X�g�Ǘ�����ۂ́A�Ǘ��v�f
* 
*/
class CTargetItem
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	CTargetItem( void);

	/**
	* @brief �R���X�g���N�^
	*
	* @param ipFeature [in] �Ǘ��Ώۃt�B�[�`��
	*/
///	CTargetItem( IFeaturePtr ipFeature, LONG lNewObjectID = -1 );
	CTargetItem( IFeaturePtr ipFeature );

	/**
	* @brief �f�X�g���N�^
	*/
	~CTargetItem( void);

	/**
	* @brief �Ǘ��f�[�^�ݒ�
	*
	* @note �C���X�g���N�^�ŏ������������̂�u��������B
	*
	* @param ipFeature		[in]			�Ǘ��Ώۃt�B�[�`��
///	* @param lNewObjectID	[in,optional]	CreateFeatureBuffer �ŐV�K�쐬���ꂽ�t�B�[�`���p
	*/
///	BOOL Attach( IFeaturePtr ipFeature, LONG lNewObjectID = -1 );
	BOOL Attach( IFeaturePtr ipFeature );

	/**
	* @brief �R�s�[�R���X�g���N�^
	*
	* @param cTargetItem [in] �R�s�[���C���X�^���X
	*
	* @return �������g�̃C���X�^���X
	*/
	CTargetItem& operator=( const CTargetItem& cTargetItem );

	/**
	* @brief ������r���Z�q
	*
	* @note ��r�́A�t�B�[�`����OID�ƃ��C���̃e�[�u�����Ŕ�r�����
	*
	* @param cTargetItem [in] ��r�ΏۃC���X�^���X
	*
	* @retval TURE ������
	* @retval FALSE �������Ȃ�
	*/
	bool operator==( const CTargetItem& cTargetItem ) const;

	/**
	* @brief �s������r���Z�q
	*
	* @note operator==���Z�̌��ʂ𔽓]���ĕԂ��Ă���
	* @note �{���́Astd::rel_ops::operator!=�𗘗p���Ă��炢�����C������B
	*
	* @param cTargetItem [in] ��r�ΏۃC���X�^���X
	*
	* @retval TURE �������Ȃ�
	* @retval FALSE ����
	*/
	bool operator!=( const CTargetItem& cTargetItem ) const;

	/**
	* @brief �召��r
	*
	* @note OID��召��r����BOID������̏ꍇ�ɂ́A���C�����Ŕ�r�B
	* @note �{���ł���΁A���C�����̂�D�悳����ׂ��ł��邪�A�d�������������Ƃƕ�����ł��邱�Ƃ����r�R�X�g����������OID�𗘗p���Ă���B
	* @note ���̃��\�b�h��list::sort�p�ɏ��������B
	* @note ���Ȃ݂ɁA>,>=,<=�́A���ꂼ��std::rel_ops::operator>/std::rel_ops::operator>=/std::rel_ops::operator<=�𗘗p���đΉ��̂��ƁB
	*
	* @retval TRUE ������
	* @retval FALSE �傫��
	*/
	bool operator<( const CTargetItem& cTargetItem ) const;

	/**
	* @brief �Ǘ��t�B�[�`���̎擾
	*
	* @return IFeaturePtr
	*/
	IFeaturePtr GetFeature() const;

	/**
	* @brief �Ǘ��t�B�[�`���̃W�I���g���擾
	*
	* @note ���̊֐��́AIFeature::get_Shape�Ŏ�������Ă���B
	* @note ���̂��߁A�`���ύX����ꍇ�ɂ͏\���ɒ��ӂ���K�v������B
	* @note �`��ύX�Ȃ��A���x�d���ɂ͂�����𐄑E�B
	*
	* @return IGeometryPtr
	*/
	IGeometryPtr GetShape() const;

	/**
	* @brief �Ǘ��t�B�[�`���̃W�I���g���擾
	*
	* @note ���̊֐��́AIFeature::get_ShapeCopy�Ŏ�������Ă��邱�Ƃɒ���
	* @note �`�󂪕ύX�����l�ȏꍇ�ɂ́A������𗘗p�̂��ƁB
	*
	* @return IGeometryPtr
	*/
	IGeometryPtr GetShapeCopy() const;

	/**
	* @brief �`��̐ݒ�
	*
	* @note �`����t�B�[�`���ɐݒ�
	*
	* @param piGeometry [in] �ݒ肷��`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL SetShape( IGeometryPtr piGeometry );

	/**
	 * @brief OBJECTID �������Ă��邩�ǂ���
	 *
	 * @retval TRUE �����Ă���
	 * @retval FALSE �����Ă��Ȃ�
	 */
	BOOL HasOID() const;

	/**
	 * @brief OBJECTID �̎擾
	 *
	 * @return OBJECTID�i���݂��Ȃ���� -1�j
	 */
	LONG GetOID() const;

	/**
	 * @brief IWorkspacePtr �̎擾
	 *
	 * @return IWorkspacePtr�i�擾���Ȃ���� NULL�j
	 */
	IWorkspacePtr GetWorkspace() const;

	/**
	 * @brief �t�B�[�`���W�I���g���� FromPoint ���擾����
	 *
	 * @note �t�B�[�`���̃W�I���g�����|�����C���̏ꍇ�Ɍ���
	 *
	 * @return IPoint �i�|�����C���ł͂Ȃ��ꍇ�� NULL�j
	 */
	IPointPtr GetFromPoint();

	/**
	 * @brief �t�B�[�`���W�I���g���� ToPoint ���擾����
	 *
	 * @note �t�B�[�`���̃W�I���g�����|�����C���̏ꍇ�Ɍ���
	 *
	 * @return IPoint �i�|�����C���ł͂Ȃ��ꍇ�� NULL�j
	 */
	IPointPtr GetToPoint();

	/**
	* @brief �Ǘ��t�B�[�`���̃W�I���g���^�C�v���擾
	*
	* @return �W�I���g���^�C�v
	*/
	esriGeometryType GetGeometryType() const;

	/**
	 * @brief �t�B�[�`���̃t�B�[�`���N���X���̎擾
	 *
	 * @note AheGetFeatureClassName() �����̂܂܎g�p
	 *
	 * @return CString
	 */
	CString GetFeatureClassName() const;

	/**
	 * @brief �t�B�[�`���̃I�[�i���擾
	 *
	 * @note AheGetFeatureClassOwnerName() �����̂܂܎g�p
	 *
	 * @return CString
	 */
	CString GetFeatureClassOwnerName() const;
	
	/**
	 * @brief �t�B�[�`���̃I�[�i��.�N���X���擾
	 *
	 * @return CString
	 */
	CString GetOwnerDotClassName() const;

	/**
	 * @brief �o�[�W�������擾
	 *
	 * @return CString
	 */
	CString GetVersionName() const;

	/**
	 * @brief �t�B�[�`���̃t�B�[�`���N���X�擾
	 *
	 * @note AheGetFeatureClass() �����̂܂܎g�p
	 *
	 * @return IFeatureClassPtr
	 */
	IFeatureClassPtr GetFeatureClass() const;

	/**
	* @brief �X�V���̐ݒ�
	*
	* @note operator/purpose/modifydate�ɏ���ݒ肷��B
	* @note operator��purpose�́ASiNDY�̃v���p�e�B�ݒ�t�@�C������擾�B
	* @note modifydate�̓p�����[�^�̃f�[�^�𗘗p�B�p�����[�^��NULL�̏ꍇ�A�����_�ł̃V�X�e���̃��[�J���^�C���擾�B
	*
	* @param emUpdateType [in] �X�V�^�C�v���
	* @param lpModifyTime [in] �X�V���ɐݒ肷��X�V����
	*
	* @see AheSetModifyData
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL SetModifyData( sindyUpdateType emUpdateType, LPSYSTEMTIME lpModifyTime = NULL);

	/**
	* @brief �Ǘ��Ώۃt�B�[�`�����o�b�t�@�T�C�Y���w�肵�ă|���S����
	*
	* @param dbBufferSize [in] �o�b�t�@������T�C�Y(map unit�l
	* @param ppiPolygon [in] �o�b�t�@�������`����󂯎��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL CreatePolygon( double dbBufferSize, IPolygon** ppiPolygon);

	/**
	* @brief �Ǘ��Ώۃt�B�[�`�����w�肵���_�Ńq�b�g�e�X�g
	*
	* @param piTestPoint [in] �e�X�g�����{����|�C���g
	* @param dbTolerance [in] �q�b�g�e�X�g�����臒l
	* @param piPoint [out] �q�b�g�����|�C���g
	* @param pdbHitDist [out] �w�肵���|�C���g������ۂɃq�b�g�����|�C���g�܂ł̋���
	* @param plPartIndex [out] �q�b�g�������_�̃|�C���g��ւ̃C���f�b�N�X
	* @param plVertIndex [out] �q�b�g�����ӂւ̃C���f�b�N�X
	*
	* @retval esriGeometryPartNone �w�肵���G���A���Ńq�b�g���Ȃ�
	* @retval esriGeometryPartVertex ���_�Ƀq�b�g����
	* @retval esriGeometryPartBoundary �ӏ�Ƀq�b�g����
	*/
	esriGeometryHitPartType HitTest( IPointPtr piTestPoint, double dbTolerance, IPoint** piPoint, double* pdbHitDist = NULL, LONG* plPartIndex = NULL, LONG* plVertIndex = NULL, esriGeometryHitPartType testType = (esriGeometryHitPartType)(esriGeometryPartVertex|esriGeometryPartBoundary) );

	/**
	* @brief �w��W�I���g�����w�肵���_�Ńq�b�g�e�X�g
	*
	* @param piGeometry [in] �q�b�g�e�X�g�Ώۂ̃W�I���g��
	* @param piTestPoint [in] �e�X�g�����{����|�C���g
	* @param dbTolerance [in] �q�b�g�e�X�g�����臒l
	* @param piPoint [out] �q�b�g�����|�C���g
	* @param pdbHitDist [out] �w�肵���|�C���g������ۂɃq�b�g�����|�C���g�܂ł̋���
	* @param plPartIndex [out] �q�b�g�������_�̃|�C���g��ւ̃C���f�b�N�X
	* @param plVertIndex [out] �q�b�g�����ӂւ̃C���f�b�N�X
	*
	* @retval esriGeometryPartNone �w�肵���G���A���Ńq�b�g���Ȃ�
	* @retval esriGeometryPartVertex ���_�Ƀq�b�g����
	* @retval esriGeometryPartBoundary �ӏ�Ƀq�b�g����
	*/
	esriGeometryHitPartType HitTest( IGeometryPtr piGeometry, IPointPtr piTestPoint, double dbTolerance, IPoint** piPoint, double* pdbHitDist = NULL, LONG* plPartIndex = NULL, LONG* plVertIndex = NULL, esriGeometryHitPartType testType = (esriGeometryHitPartType)(esriGeometryPartVertex|esriGeometryPartBoundary) );

	/**
	* @brief �Ǘ��Ώۃt�B�[�`�����͂ރG���x���[�v�̎擾
	*
	* @note ���݊Ǘ��ΏۂɂȂ��Ă���t�B�[�`�����܂���G���x���[�v��Ԃ��B
	*
	* @param ppiEnv [out] �Ǘ��t�B�[�`�����܂���G���x���[�v�̎擾
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetEnvelope( IEnvelope** ppiEnv);

	/**
	* @brief �Ǘ��Ώۃt�B�[�`���̃t�B�[���h���Ƃ��̃C���f�b�N�X�Ǘ��}�b�v���쐬����B
	*
	* @param cNameMap [out] �t�B�[���h���̂ƃJ�����C���f�b�N�X�̑Ή��}�b�v�R���e�i
	* @param cAliasMap [out] �t�B�[���h�G�C���A�X���̂ƃJ�����C���f�b�N�X�̑Ή��}�b�v�R���e�i
	*
	* @return �t�B�[���h��
	*/
	LONG GetFieldMap( FIELDMAP& cNameMap, FIELDMAP& cAliasMap);

	/**
	 * @brief PEC�����N���ǂ���
	 *
	 * @note ROADCLASS_C=27 or 28 �̏ꍇ�� PEC �����N�ɊY�����܂�
	 */
	BOOL IsPECLink();

	/**
	* @brief �w��̃W�I���g���Ƃ̌�������
	*
	* @see AheIsCross()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE ����
	* @retval FALSE �����
	*/
	BOOL IsCrosses( IGeometryPtr piGeom);
	/**
	* @brief �w��̃W�I���g���Ƃ̌�������
	*
	* @see AheIsCrosses()
	*
	* @param cTargetItem [in] ��r�ΏۊǗ��N���X
	*
	* @retval TRUE ����
	* @retval FALSE �����
	*/
	BOOL IsCrosses( CTargetItem& cTargetItem);

	/**
	* @brief �w��̃W�I���g���Ƃ̓����
	*
	* @see AheIsWithin()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE ����
	* @retval FALSE �����
	*/
	BOOL IsWithin( IGeometryPtr piGeom);

	/**
	* @brief �w��̃W�I���g���Ƃ̓����
	*
	* @see AheIsWithin2()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE ����
	* @retval FALSE �����
	*/
	BOOL IsWithin2( IGeometryPtr piGeom);

	/**
	* @brief �w��̃W�I���g���Ƃ̓����
	*
	* @see AheIsWithin()
	*
	* @param piGeom [in] ��r�ΏۊǗ��N���X
	*
	* @retval TRUE ����
	* @retval FALSE �����
	*/
	BOOL IsWithin( CTargetItem& cTargetItem);

	/**
	* @brief �w��̃W�I���g���Ƃ̐ڐG����
	*
	* @see AheIsTouch()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE �ڐG
	* @retval FALSE ��ڐG
	*/
	BOOL IsTouches( IGeometryPtr piGeom);
	/**
	* @brief �w��̃W�I���g���Ƃ̐ڐG����
	*
	* @see AheIsTouch()
	*
	* @param piGeom [in] ��r�ΏۊǗ��N���X
	*
	* @retval TRUE �ڐG
	* @retval FALSE ��ڐG
	*/
	BOOL IsTouches( CTargetItem& cTargetItem);

	/**
	* @brief �w��̃W�I���g���Ƃ̔�ڐG����
	*
	* @see AheIsDisjoint()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE ��ڐG
	* @retval FALSE �ڐG
	*/
	BOOL IsDisjoint( IGeometryPtr piGeom);
	/**
	* @brief �w��̃W�I���g���Ƃ̔�ڐG����
	*
	* @see AheIsDisjoint()
	*
	* @param piGeom [in] ��r�ΏۊǗ��N���X
	*
	* @retval TRUE ��ڐG
	* @retval FALSE �ڐG
	*/
	BOOL IsDisjoint( CTargetItem& cTargetItem);

	/**
	* @brief �w��̃W�I���g���Ƃ̍�������
	*
	* @see AheIsEqual()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE ����
	* @retval FALSE �񍇓�
	*/
	BOOL IsEquals( IGeometryPtr piGeom);
	/**
	* @brief �w��̃W�I���g���Ƃ̍�������
	*
	* @see AheIsEqual()
	*
	* @param piGeom [in] ��r�ΏۊǗ��N���X
	*
	* @retval TRUE ����
	* @retval FALSE �񍇓�
	*/
	BOOL IsEquals( CTargetItem& cTargetItem);

	/**
	* @brief �w��̃W�I���g���Ƃ̕����I�d������
	*
	* @see AheIsOverlap()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE �d��
	* @retval FALSE ��d��
	*/
	BOOL IsOverlaps( IGeometryPtr piGeom);
	/**
	* @brief �w��̃W�I���g���Ƃ̕����I�d������
	*
	* @see AheIsOverlap()
	*
	* @param piGeom [in] ��r�ΏۊǗ��N���X
	*
	* @retval TRUE �d��
	* @retval FALSE ��d��
	*/
	BOOL IsOverlaps( CTargetItem& cTargetItem);

	/**
	* @brief �w��̃W�I���g���Ƃ̕�܌���
	*
	* @see AheIsContain()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE ���
	* @retval FALSE ����
	*/
	BOOL IsContains( IGeometryPtr piGeom);
	/**
	* @brief �w��̃W�I���g���Ƃ̕�܌���
	*
	* @see AheIsContain2()
	*
	* @param piGeom [in] ��r�ΏۃW�I���g���C���X�^���X
	*
	* @retval TRUE ���
	* @retval FALSE ����
	*/
	BOOL IsContains2( IGeometryPtr piGeom);
	/**
	* @brief �w��̃W�I���g���Ƃ̕�܌���
	*
	* @see AheIsContain()
	*
	* @param piGeom [in] ��r�ΏۊǗ��N���X
	*
	* @retval TRUE ���
	* @retval FALSE ����
	*/
	BOOL IsContains( CTargetItem& cTargetItem);

	/**
	* @brief �Q�����b�V���m�[�h����
	*
	* @retval TRUE �Q�����b�V���m�[�h
	* @retval FALSE ��Q�����b�V���m�[�h
	*/
	BOOL IsBorderNode( void);

	/**
	* @brief �Q�����b�V���m�[�h����
	*
	* @retval TRUE �Q�����b�V���m�[�h
	* @retval FALSE ��Q�����b�V���m�[�h
	*/
	BOOL IsBorderPECNode( void);

	/**
	* @brief �t�B�[�`���̍폜
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	BOOL Delete();

	/**
	* @brief �`��`�F�b�N
	*
	* @note �`�F�b�N���ʂ̕������ GetErrMessage() �Ŏ擾�ł��܂�
	* @see GetErrMessage
	*
	* @param check				[out]			�`�F�b�N����
	* @param bCheckRoad			[in,optional]	���H�����N�ƃm�[�h�̐ڑ������`�F�b�N���邩�ǂ����i�f�t�H���g�FTRUE�j
	* @param bIsCheckAngle		[in,optional]	�Z�O�����g�ڑ��p�x���`�F�b�N���邩�ǂ����i�f�t�H���g�FTRUE�j
	* @param ipRule				[in,optional]	���[���I�u�W�F�N�g�i�f�t�H���g�FNULL�j
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	BOOL IsSafeGeometry( CErrObjContainer& errs, BOOL bCheckRoad = TRUE, BOOL bIsCheckAngle = TRUE, ISiNDYRule3Ptr ipRule = NULL );

	/** 
	 * @brief �|�C���g�� From �|�C���g�ł��邩�ǂ����̔���
	 *
	 * @see AheIsFromPoint()
	 *
	 * @param ipPointGeom	[in]			�|�C���g�W�I���g��
	 * @param ipSpRef		[in,optional]	ISpatialRefenrece
	 *
	 * @retval TRUE From �|�C���g�ł���
	 * @retval FALSE From �|�C���g�ł͂Ȃ��A���̓��C���t�B�[�`���ł͂Ȃ�
	 */
	BOOL IsFromPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef = NULL );

	/** 
	 * @brief �|�C���g�� To �|�C���g�ł��邩�ǂ����̔���
	 *
	 * @see AheIsToPoint()
	 *
	 * @param ipPointGeom	[in]			�|�C���g�W�I���g��
	 * @param ipSpRef		[in,optional]	ISpatialRefenrece
	 *
	 * @retval TRUE To �|�C���g�ł���
	 * @retval FALSE To �|�C���g�ł͂Ȃ��A���̓��C���t�B�[�`���ł͂Ȃ�
	 */
	BOOL IsToPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef = NULL );

	/**
	 * @brief �|�C���g���[�_�ł��邩�ǂ����̔���
	 *
	 * @see AheIsEndPoint()
	 *
	 * @param ipPointGeom	[in]			�|�C���g�W�I���g��
	 * @param ipSpRef		[in,optional]	ISpatialRefenrece
	 *
	 * @retval TRUE �[�_�ł���
	 * @retval FALSE�[�_�ł͂Ȃ��A���̓��C���t�B�[�`���ł͂Ȃ�
	 */
	BOOL IsEndPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef = NULL );
	
	/**
	* @brief �ҏW����
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	BOOL Store();

	/**
	* @brief �n���Q�Ƃ𒲐�
	*
	* @note �Ǘ��Ώۂ̃��C���̒n���Q�Ƃ��t�B�[�`���ɐݒ�
	*
	* @param piSpatialReference [in] �ݒ肷��n���Q�Ǝw�肵�Ȃ��ꍇ�́A�Ǘ��Ώۂ̃��C������擾���Đݒ�
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL SnapToSpatialReference( ISpatialReferencePtr piSpatialReference = NULL);

	/**
	* @brief �w�蕪�̃T�C�Y�g��
	*
	* @note TopologicalOperator::Buffer�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param dbDistance [in] �g���T�C�Y�iMapUNIT
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL Buffer( double dbDistance, IGeometry** ppiGeometry = NULL);

	/**
	* @brief �w��`��Ƃ̍����擾
	*
	* @note TopologicalOperator::Difference�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param piGeometrySrc [in] ��r�`��
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL Difference( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry = NULL);
	/**
	* @brief �w��`��Ƃ̍����擾
	*
	* @note TopologicalOperator::Difference�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param cTargetItem [in] ��r�`��
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL Difference( CTargetItem& cTargetItem, IGeometry** ppiGeometry = NULL);

	/**
	* @brief �w��`��Ƃ̋��ʌ`�󒊏o
	* 
	* @note TopologicalOperator::Difference�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param piGeometrySrc [in] ��r�`��
	* @param resultDimension [in]
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL Intersect( IGeometryPtr piGeometrySrc, esriGeometryDimension resultDimension, IGeometry** ppiGeometry = NULL);
	/**
	* @brief �w��`��Ƃ̋��ʌ`�󒊏o
	* 
	* @note TopologicalOperator::Difference�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param cTargetItem [in] ��r�`��
	* @param resultDimension [in]
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL Intersect( CTargetItem& cTargetItem, esriGeometryDimension resultDimension, IGeometry** ppiGeometry = NULL);

	/**
	* @brief �w��`��Ƃ̑����`�󒊏o
	* 
	* @note TopologicalOperator::Union�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param piGeometrySrc [in] ��r�`��
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL Union( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry = NULL);
	/**
	* @brief �w��`��Ƃ̑����`�󒊏o
	* 
	* @note TopologicalOperator::Union�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param cTargetItem [in] ��r�`��
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL Union( CTargetItem& cTargetItem, IGeometry** ppiGeometry = NULL);

	/**
	 * @brief �G���[���b�Z�[�W�̎擾
	 *
	 * @retval CString �G���[���b�Z�[�W
	 */
	CString GetErrMessage();

	/**
	 * @brief �G���[���b�Z�[�W�̍폜
	 */
	void RemoveErrMessage();

	/**
	* @brief �w��`��Ƃ̍����擾
	*
	* @note TopologicalOperator::SymmetricDifference�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param piGeometrySrc [in] ��r�`��
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL SymmetricDifference( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry);

	/**
	* @brief �w��`��Ƃ̍����擾
	*
	* @note TopologicalOperator::SymmetricDifference�����̏���
	* @note ppiGeometry��NULL�w�肵���ꍇ�A�W�I���g���͊Ǘ��Ώۃt�B�[�`���ɓK�p�����BStore�͍s��Ȃ��B
	*
	* @param ppiGeometry [out] �g�����ʌ`��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL SymmetricDifference( IGeometry** ppiGeometry);

	/**
	* @brief �|�C���g�R���N�V�����̎擾
	*
	* @param piGeometrySrc [in] �擾�`��
	*
	* @retval IPointCollection �̃C���X�^���X
	*/
	IPointCollectionPtr Collection( IGeometryPtr piGeometrySrc );

	/**
	* @brief �|�C���g�R���N�V�����̎擾
	*
	* @retval IPointCollection �̃C���X�^���X
	*/
	IPointCollectionPtr GetPointCollection();

	/**
	* @brief �|�����C���̎擾
	*
	* @param piGeometrySrc [in] �擾�`��
	*
	* @retval IPolyline �̃C���X�^���X
	*/
	IPolylinePtr GetPolyline( IGeometryPtr piGeometrySrc );

	/**
	* @brief �|�����C���̎擾
	*
	* @param piGeometrySrc [in] �擾�`��
	* @param ppiPointCollection [out] ���ʃ|�����C��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetPolyline( IGeometryPtr piGeometrySrc, IPolyline** ppiPolyline);

	/**
	* @brief �|�����C���̎擾
	*
	* @param ppiPointCollection [out] ���ʃ|�����C��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetPolyline( IPolyline** ppiPolyline);

	/**
	* @brief �|�����C���̎擾
	*
	* @retval IPolyline �̃C���X�^���X
	*/
	IPolylinePtr GetPolyline();

	/**
	* @brief �\���_���̎擾
	*
	* @retval �\���_��
	*/
	LONG GetVertexCount();

	/**
	* @brief �t�B�[�`���^�C�v�̎擾
	*
	* @retval sindyeFeatureKinds
	*/
	sindyeFeatureKinds GetFeatureKinds() const;

	/**
	 * �t�B�[�`�����瑮�����擾����
	 *
	 * @param ipFeature		[in]	�t�B�[�`���i���R�[�h�j
	 * @param lpcszFieldName	[in]	�擾����t�B�[���h��
	 * @param vaRetValue	[out]	�擾�ł����l
	 *
	 * @retval 
	 */
	void GetAttribute( IFeaturePtr ipFeature, LPCTSTR lpcszFieldName, CComVariant& vaRetValue ) const;

	/**
	 * �t�B�[�`�����瑮�����擾����
	 *
	 * @param ipFeature		[in]	�t�B�[�`���i���R�[�h�j
	 * @param lpcszFieldName	[in]	�擾����t�B�[���h��
	 *
	 * @retval �擾�ł����l
	 */
	CComVariant GetAttribute( IFeaturePtr ipFeature, LPCTSTR lpcszFieldName ) const;
	
	/**
	 * �t�B�[�`�����瑮�����擾����
	 *
	 * @param lpcszFieldName	[in]	�擾����t�B�[���h��
	 * @param vaRetValue	[out]	�擾�ł����l
	 *
	 * @retval 
	 */
	void GetAttribute( LPCTSTR lpcszFieldName, CComVariant& vaRetValue ) const;

	/**
	 * �t�B�[�`�����瑮�����擾����
	 *
	 * @param lpcszFieldName	[in]	�擾����t�B�[���h��
	 *
	 * @retval �擾�ł����l
	 */
	CComVariant GetAttribute( LPCTSTR lpcszFieldName ) const;

	/**
	 * �t�B�[�`���ɑ������Z�b�g����
	 * 
	 * @note Store() �͂��܂���
	 *
	 * @param ipFeature			[in]	�Z�b�g����t�B�[�`���i���R�[�h�j
	 * @param lpcszFieldName	[in]	�Z�b�g����t�B�[���h��
	 * @param vaValue			[in]	�Z�b�g����l
	 *
	 * @retval TRUE �Z�b�g�ł���
	 * @retval FALSE �Z�b�g�ł��Ȃ�����
	 */
	BOOL SetAttribute( IFeaturePtr ipFeature, LPCTSTR lpcszFieldName, CComVariant vaValue );
	
	/**
	 * �t�B�[�`���ɑ������Z�b�g����
	 * 
	 * @note Store() �͂��܂���
	 *
	 * @param lpcszFieldName	[in]	�Z�b�g����t�B�[���h��
	 * @param vaValue			[in]	�Z�b�g����l
	 *
	 * @retval TRUE �Z�b�g�ł���
	 * @retval FALSE �Z�b�g�ł��Ȃ�����
	 */
	BOOL SetAttribute( LPCTSTR lpcszFieldName, CComVariant vaValue);

	/**
	* @brief �t�B�[�`���� XOR �ŕ`�悷��
	*
	* @param ipFeature			[in]			�\������t�B�[�`��
	* @param ipScreen			[in]			IDisplay �̃C���X�^���X
	* @param dbBufferSize		[in]			�o�b�t�@����T�C�Y
	* @param bDrawVertex		[in]			���_��`�悷�邩�ǂ���
	* @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
	*/
	void DrawFeatureXOR( IFeaturePtr ipFeature, IScreenDisplayPtr ipScreen, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop = TRUE);
	
	/**
	* @brief �t�B�[�`���� XOR �ŕ`�悷��
	*
	* @param ipScreen			[in]			�\������t�B�[�`��
	* @param dbBufferSize		[in]			�o�b�t�@����T�C�Y
	* @param bDrawVertex		[in]			���_��`�悷�邩�ǂ���
	* @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
	*/
	void DrawFeatureXOR( IScreenDisplayPtr ipScreen, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop = TRUE);

	/**
	* @brief �t�B�[�`���̒��_�Ƀq�b�g�����ꍇ�͂��̃C���f�b�N�X�ԍ����A�q�b�g���Ȃ������ꍇ�� -1 ��Ԃ�
	*
	* @note TestGeometryHit() �̊ȈՔ�
	*
	* @param ipFeature		[in]			�ΏۂƂȂ� IFeature �̃C���X�^���X
	* @param ipPoint		[in]			�e�X�g���� IPoint �̃C���X�^���X
	* @param dTorelance		[in]			����
	*
	* @retval -1�ȊO �q�b�g�����C���f�b�N�X�ԍ�
	* @retval -1 �q�b�g���Ȃ�����
	**/
	LONG HitTestVertexIndex( IFeaturePtr ipFeature, IPointPtr ipPoint, double dTorelance);
	
	/**
	* @brief �t�B�[�`���̒��_�Ƀq�b�g�����ꍇ�͂��̃C���f�b�N�X�ԍ����A�q�b�g���Ȃ������ꍇ�� -1 ��Ԃ�
	*
	* @note TestGeometryHit() �̊ȈՔ�
	*
	* @param ipPoint		[in]			�e�X�g���� IPoint �̃C���X�^���X
	* @param dTorelance		[in]			����
	*
	* @retval -1�ȊO �q�b�g�����C���f�b�N�X�ԍ�
	* @retval -1 �q�b�g���Ȃ�����
	**/
	LONG HitTestVertexIndex( IPointPtr ipPoint, double dTorelance);

	/**
	 * �t�B�[�`�����ҏW�\���ǂ����`�F�b�N����
	 *
	 * @note ISiNDYRuleObj::IsEditableFeature() �����b�v���āATRUE �� FALSE ��Ԃ��܂��B
	 *
	 * @param ipFature	[in]	�`�F�b�N�Ώۃt�B�[�`��
	 * @param type		[in]	sindyeEditType
	 *
	 * @retval TRUE �ҏW�\
	 * @retval FALSE �ҏW�s�\
	 */
	BOOL IsEditableItem( _IRowPtr ipFeature, sindyeEditType type) const;

	/**
	 * �t�B�[�`�����ҏW�\���ǂ����`�F�b�N����
	 *
	 * @note ISiNDYRuleObj::IsEditableFeature() �����b�v���āATRUE �� FALSE ��Ԃ��܂��B
	 *
	 * @param ipFature	[in]	�`�F�b�N�Ώۃt�B�[�`��
	 * @param type		[in]	sindyeEditType
	 *
	 * @retval TRUE �ҏW�\
	 * @retval FALSE �ҏW�s�\
	 */
	BOOL IsEditableItem( sindyeEditType type) const;

	/**
	 * @brief �t�B�[�`���̍\���_���擾����
	 *
	 * @param ipFeature			[in]			IFeature �̃C���X�^���X
	 * @param lIndex			[in]			�擾����\���_�̃C���f�b�N�X
	 *
	 * @return IPoint �̃C���X�^���X
	 */
	IPointPtr GetPoint( LONG lIndex );

	/**
	 * @brief �t�B�[�`���̍\���_���폜����
	 *
	 * @param lIndex			[in]			�擾����\���_�̃C���f�b�N�X
	 *
	 * @return IPoint �̃C���X�^���X
	 */
	IPointCollectionPtr RemovePoint( LONG lIndex );

	/**
	 * @brief �������R�s�[����
	 *
	 * @param ipFeature			[in]			IFeature �̃C���X�^���X
	 * @param cNewItem			[in]			�A�C�e��
	 *
	 * @return 
	 */
	BOOL CopyAttribute( IFeaturePtr ipFeature, const CTargetItem& cNewItem );

	/**
	 * @brief �������R�s�[����
	 *
	 * @param ipFeature			[in]			IFeature �̃C���X�^���X
	 * @param ipNewFeature		[in]			IFeature �̃C���X�^���X
	 *
	 * @return 
	 */
	BOOL CopyAttribute( IFeaturePtr ipFeature, IFeaturePtr ipNewFeature );

	/**
	 * @brief �������R�s�[����
	 *
	 * @param cNewItem			[in]			�A�C�e��
	 *
	 * @return 
	 */
	//BOOL CopyAttribute( const CTargetItem& cNewItem );

	/**
	 * @brief �������R�s�[����
	 *
	 * @param ipNewFeature		[in]			IFeature �̃C���X�^���X
	 *
	 * @return 
	 */
	BOOL CopyAttribute( IFeaturePtr ipNewFeature );

	/**
	 * @brief �n���Q�Ǝ擾
	 *
	 * @return ISpatialReference �̃C���X�^���X
	 */
	ISpatialReferencePtr GetSpatialReference() const;

	/**
	 * @brief �����I�� Simplify() ������
	 *
	 * @see AheForceSimplify
	 *
	 * @retval TRUE ����
	 * @retval FALSE ���s
	 */
	BOOL Simplify();

	/**
 	 * @brief �W�I���g����Z�l�������Ă��邩
	 * @retval true  Z�l�����ł���
	 * @retval false Z�l�����ł͂Ȃ�
	 */
	bool HasZ();

protected:

	/**
	* @brief �\���_�ǉ�����
	*
	* @note �W�I���g���̔C�ӂ̈ʒu�ɍ\���_�ǉ�
	*
	* @retval TRUE ����I��
	* @retval FALSE �s���I��
	*/
	BOOL AddNewPoint( IGeometryPtr piGeometry, LONG lIndex, IPointPtr piNewPoint);

protected:
	IFeaturePtr m_ipFeature;			//!< �Ǘ��Ώۃt�B�[�`��
	CString m_strErrMessage;			//!< �G���[���b�Z�[�W�i�[�p
};

#include "TargetFeature.inl"

#endif	//__TARGETFEATURE_H__
