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
 * @file ErrorCodeBase.h
 * @brief <b>�G���[�R�[�h�\��`�t�@�C��<\b>\n
 * ���̃t�@�C���ł̓G���[�R�[�h���`���Ă��܂��B
 * SiNDY�Ŏg�p�����G���[�R�[�h�͑S�Ă��̃t�@�C���Œ�`�����K�v������܂��B
 * �l�X�ȏ�ʂŎg�p�ł���悤�ɁA�}�N���`���Œ�`���Ă��܂��B
 * ��ʃR�[�h���K�v�ȃt�@�C����ERROR_CODE�}�N�����`���A
 * ���̌�ɂ��̃t�@�C�����C���N���[�h���Ă��������B
 * �C���N���[�h���ERROR_CODE�}�N����undef���ĉ������B
 *
 * ��`�����G���[�R�[�h�̎�ނ͎��̒ʂ�F
 *
 * �E�����`�F�b�N�i�v���O�������W�b�N�G���[�j
 * �E�P�̃`�F�b�N�i�I�u�W�F�N�g�P�ʃ`�F�b�N�G���[�j
 * �E�����`�F�b�N�i�I�u�W�F�N�g�Ԃ̘_���`�F�b�N�G���[�j
 * �E���̑��̃G���[
 *
 * �^�U���������֐��͂Ȃ�ׂ����̃R�[�h��Ԃ��悤�ɂ��ĉ������B
 * <H4>��Fenum�Ƃ��Ďg�p����ꍇ</H4>
 * @code
 * enum ECode {
 * #define ERROR_CODE(id,label) id,
 * #include "ErrorCodeBase.h"
 * #undef ERROR_CODE
 * };
 * @endcode
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef SINDY_FOR_ENGLISH
ERROR_CODE(sindyErr_NoErr                               ,�G���[�Ȃ�)
ERROR_CODE(sindyErr_NoImplimentation                    ,������)
////////////////////////////////////////////////////////
// ���W�X�g���E�ݒ�
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_RegNoSiNDY                          ,���W�X�g����SiNDY�֘A�̐ݒ肪�Ȃ�)
ERROR_CODE(sindyErr_RegNoInstallRoot                    ,���W�X�g���ɃC���X�g�[���t�H���_���i�[����Ă��Ȃ�)
ERROR_CODE(sindyErr_RegNoInifileName                    ,���W�X�g����ini�t�@�C�������i�[����Ă��Ȃ�)
ERROR_CODE(sindyErr_IniNoServerName                     ,ini�t�@�C���ɃT�[�o�����i�[����Ă��Ȃ�)
ERROR_CODE(sindyErr_RegOpenFailed                       ,���W�X�g���̃I�[�v���Ɏ��s)
ERROR_CODE(sindyErr_RegWriteFailed                      ,���W�X�g���̏������݂Ɏ��s)
ERROR_CODE(sindyErr_RegReadFialed                       ,���W�X�g���̓ǂݍ��݂Ɏ��s)
////////////////////////////////////////////////////////
// �����`�F�b�N�i�v���O�������W�b�N�G���[�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_ArgIsNull                           ,������NULL)
ERROR_CODE(sindyErr_ArgLimitOver                        ,�����͈̔͂��z��O)
ERROR_CODE(sindyErr_ArgIsConsistency                    ,�������m�̐���������Ȃ�)
////////////////////////////////////////////////////////
// �����o�`�F�b�N�i�v���O�������W�b�N�G���[�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_ClassMemberMissing                  ,�N���X�̃����o������Ȃ��i�������s���j)
////////////////////////////////////////////////////////
// �v���O�������W�b�N�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_FunctionFailed                      ,�֐��Ăяo���Ɏ��s)
ERROR_CODE(sindyErr_MemoryAllocationFailed              ,�������m�ۂɎ��s)
ERROR_CODE(sindyErr_BufferIsNotEnough                   ,�o�b�t�@�̈悪����Ȃ�)
ERROR_CODE(sindyErr_ThrowException                      ,��O����)
ERROR_CODE(sindyErr_InvalidTypeInfo                     ,�^��񂪈�v���Ȃ�)
ERROR_CODE(sindyErr_AlgorithmFailed                     ,�A���S���Y���G���[)
ERROR_CODE(sindyErr_MethodLocked                        ,�֐��͎g�p�֎~)
ERROR_CODE(sindyErr_ObjectLocked                        ,�I�u�W�F�N�g�����b�N����Ă���)
////////////////////////////////////////////////////////
// COM�G���[�i�v���O�������W�b�N�G���[�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_COMInterfaceIsNull                  ,�C���^�[�t�F�[�X�|�C���^��NULL)
ERROR_CODE(sindyErr_COMCreateInstanceFailed             ,�C���X�^���X�쐬�Ɏ��s)
ERROR_CODE(sindyErr_COMQueryInterfaceFailed             ,QueryInterface�Ɏ��s)
ERROR_CODE(sindyErr_COMFunctionFailed                   ,�֐��Ăяo���Ɏ��s)
ERROR_CODE(sindyErr_COMWrappedIntarfaceIsNull           ,���b�v���Ă���͂���COM�C���^�[�t�F�[�X��NULL)
////////////////////////////////////////////////////////
// ArcObjects�G���[�i�v���O�������W�b�N�G���[�H�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_AOConnectDBFailed                   ,�f�[�^�x�[�X�ւ̐ڑ��Ɏ��s)
ERROR_CODE(sindyErr_AOStoreFailed                       ,Store()���s)
ERROR_CODE(sindyErr_AODeleteFailed                      ,Delete()���s)
ERROR_CODE(sindyErr_AOCreateRowFailed                   ,CreateRow()���s)
ERROR_CODE(sindyErr_AOCreateFeatureFailed               ,CreateFeature()���s)
ERROR_CODE(sindyErr_AOGetValueFailed                    ,get_Value()���s)
ERROR_CODE(sindyErr_AOUnsupportValueType                ,�擾�������R�[�h�l���A���҂��Ă���^�ł͂Ȃ�)
ERROR_CODE(sindyErr_AOPutValueFailed                    ,put_Value()���s)
ERROR_CODE(sindyErr_AOLayerNotStartEdit                 ,�ҏW�J�n���ꂽ���C���ł͂���܂���)
ERROR_CODE(sindyErr_AOLayerNotAllowCreate               ,�쐬�\�ȃ��C���ł͂���܂���)
ERROR_CODE(sindyErr_AOLayerNotSelected                  ,���C���̑I���Ɏ��s���܂����BTOC ����P�̃��C����I�����Ă��������B)
ERROR_CODE(sindyErr_AOGetWorkspaceFailed                ,Workspace ���擾�ł��܂���ł���)
ERROR_CODE(sindyErr_AOGetFeatureClassFailed             ,�t�B�[�`���N���X�̎擾�Ɏ��s���܂����B)
ERROR_CODE(sindyErr_AOLayerNotAllowSelect               ,�I���ł��Ȃ����C���ł��B)
ERROR_CODE(sindyErr_AOGetFieldIndexFailed               ,�t�B�[���h�C���f�b�N�X���擾�ł��܂���ł���)
ERROR_CODE(sindyErr_AOStartEditOperationFailed          ,�ҏW���J�n���邱�Ƃ��ł��܂���ł����B)
ERROR_CODE(sindyErr_AOFeatureSearchError                ,�t�B�[�`���N���X�̌����ŃG���[���������܂����B\n�G���[�R�[�h�� %d �ł��B)
ERROR_CODE(sindyErr_AOConnectEventsHasCookie            ,���ɃC�x���g�ڑ��ς�)
ERROR_CODE(sindyErr_AOConnectEventsFailed               ,�C�x���g�ڑ��Ɏ��s)
ERROR_CODE(sindyErr_AODisconnectEventsFailed            ,�C�x���g�ڑ��������s)
ERROR_CODE(sindyErr_AOErr                               ,�s����ArcObjects�n�G���[)
////////////////////////////////////////////////////////
// SDE �n�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_SDEConnectFailed                    ,SDE C API�ł�SDE�ւ̐ڑ������s)
ERROR_CODE(sindyErr_SDEDisconnectFailed                 ,SDE C API�ł�SDE����̐ؒf�Ɏ��s)
ERROR_CODE(sindyErr_SDEAPICallFailed                    ,SDE C API�̃R�[���Ɏ��s)
ERROR_CODE(sindyErr_SDEInvalidateFieldType              ,SDE C API�Ō����ł��Ȃ���^�C�v)
ERROR_CODE(sindyErr_SDEQueryFailed                      ,SDE C API�ł̃N�G���[�Ɏ��s)
ERROR_CODE(sindyErr_SDEGetStateInfoFailed               ,SDE C API�ł̃X�e�[�g���̎擾�Ɏ��s)
ERROR_CODE(sindyErr_SDEVersionChangeStateFailed         ,SDE C API�ł̃o�[�W�����̃X�e�[�g�ύX�Ɏ��s)
////////////////////////////////////////////////////////
// �����n�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_FieldLengthLimitOver                ,�t�B�[���h�����I�[�o�[���Ă���)
ERROR_CODE(sindyErr_FieldRangeLimitOver                 ,�t�B�[���h���e�͈͂��I�[�o�[���Ă���)
ERROR_CODE(sindyErr_FieldDomainLimitOver                ,�t�B�[���h�h���C���ɒ�`����Ă��Ȃ��l���Z�b�g���Ă���)
ERROR_CODE(sindyErr_FieldPrecisionLimitOver             ,�t�B�[���h�������I�[�o�[���Ă���)
ERROR_CODE(sindyErr_SetNullValueToNotNullField          ,NOT NULL�t�B�[���h��NULL�l���Z�b�g���Ă���)
////////////////////////////////////////////////////////
// �W�I���g���n�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_GeometryNotFound                    ,�W�I���g�������݂��Ȃ�)
ERROR_CODE(sindyErr_GeometryNotSupported                ,�T�|�[�g����Ă��Ȃ��W�I���g��)
ERROR_CODE(sindyErr_GeometryPartTooMach                 ,�|�C���g�E���C���W�I���g���p�[�g������ȏ㑶�݂���)
ERROR_CODE(sindyErr_GeometryExteriorTooMach             ,�|���S���W�I���g���̃G�N�X�e���A����ȏ㑶�݂���)
ERROR_CODE(sindyErr_GeometryNotCached                   ,�W�I���g���L���b�V�����K�v�ȑ���ŃL���b�V�����쐬����Ă��Ȃ�)
ERROR_CODE(sindyErr_GeometryPointNotFound               ,�w�肳�ꂽ�\���_�����݂��Ȃ�)
ERROR_CODE(sindyErr_GeometryFunctionFailed              ,�W�I���g�����Z�Ɏ��s)
ERROR_CODE(sindyErr_GeometryChangeToNull                ,�W�I���g���������Ȃ鑀��͕s��)
ERROR_CODE(sindyErr_GeometrySegmentCountIsNotEnough     ,�W�I���g�����\������Z�O�����g�̐�������Ȃ�)
ERROR_CODE(sindyErr_GeometrySegmentTooShort             ,�W�I���g�����\������Z�O�����g���Z������)
ERROR_CODE(sindyErr_GeometrySegmentsAngleOverFlow       ,�W�I���g�����\������Z�O�����g�Ԃ̊p�x���͈͊O)
ERROR_CODE(sindyErr_GeometryOverlapped                  ,�W�I���g�������̃W�I���g���Əd�����Ă���)
ERROR_CODE(sindyErr_GeometryOneselfTouches              ,�W�I���g�����������g�ɐڐG���Ă���)
ERROR_CODE(sindyErr_GeometryRooped                      ,�W�I���g�������[�v���Ă���)
ERROR_CODE(sindyErr_GeometryNonSimple                   ,�W�I���g�����V���v���ł͂Ȃ�)
ERROR_CODE(sindyErr_GeometryNonSimpleRingOrientation    ,A polygon is topologically simple�A but its rings may not be oriented correctly (outer rings - cw�A inner rings - ccw).)
ERROR_CODE(sindyErr_GeometryNonSimpleSegmentOrientation ,Individual segments are not consistantly oriented. The 'to' point of seg i should be incident on the 'from' point of seg i+1.)
ERROR_CODE(sindyErr_GeometryNonSimpleShortSegments      ,Some segments are shorter than allowed by the system units of the spatial reference associated with the geometry.)
ERROR_CODE(sindyErr_GeometryNonSimpleSelfIntersections  ,The interior of each part (rings�A planar paths) must not intersect itself or other parts.)
ERROR_CODE(sindyErr_GeometryNonSimpleUnclosedRing       ,The last segment in a ring must have its 'to' point incident on the 'from' point of the first segment.)
ERROR_CODE(sindyErr_GeometryNonSimpleEmptyPart          ,The geometry contains an empty part.)      
////////////////////////////////////////////////////////
// �ҏW�n�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_EditStartFailed                     ,StartEditing�Ɏ��s)
ERROR_CODE(sindyErr_EditStopFailed                      ,StopEditing�Ɏ��s)
ERROR_CODE(sindyErr_EditMustBeReconcile                 ,StopEditing�Ɏ��s�i���R���T�C�����K�v�j)
ERROR_CODE(sindyErr_EditOperationStartFailed            ,StartEditOperaton�Ɏ��s)
ERROR_CODE(sindyErr_EditOperationStopFailed             ,StopEditOperation�Ɏ��s)
ERROR_CODE(sindyErr_EditOperationAbortFailed            ,AbortEditOperation�Ɏ��s)
ERROR_CODE(sindyErr_EditReconcileFailed                 ,���R���T�C���Ɏ��s)
ERROR_CODE(sindyErr_EditPostFailed                      ,�|�X�g�Ɏ��s)
ERROR_CODE(sindyErr_EditConflictDetected                ,�ҏW���R���t���N�g���Ă���)
ERROR_CODE(sindyErr_EditCantRecognizedOriginalEdit      ,�J�X�^���|�X�g�����[�J�����R���T�C�������f�ł��Ȃ�)
ERROR_CODE(sindyErr_EditCustomPostNoEXTCLSID            ,EXTCLSID���o�^����Ă��Ȃ����߂ɃJ�X�^���|�X�g�ł��Ȃ�)
ERROR_CODE(sindyErr_EditParentIsProtected               ,�e�o�[�W�������v���e�N�g����Ă��邽�߂Ƀ��[�J�����R���T�C���ł��Ȃ�)
ERROR_CODE(sindyErr_EditParentIsNotProtected            ,�e�o�[�W�������v���e�N�g����Ă��Ȃ����߂ɃJ�X�^���|�X�g�ł��Ȃ�)
ERROR_CODE(sindyErr_EditVersionIsNotChild               ,�ҏW���̃o�[�W���������q�o�[�W�����ł͂Ȃ�)
ERROR_CODE(sindyErr_EditParentVersionIsChild            ,�ҏW���̃o�[�W�����̐e�o�[�W���������q�o�[�W�����ł���)
ERROR_CODE(sindyErr_EditCustomPostFailed                ,�J�X�^���|�X�g�Ɏ��s)
ERROR_CODE(sindyErr_EditLocalReconcileFailed            ,���[�J�����R���T�C���Ɏ��s)
ERROR_CODE(sindyErr_VersionNotFound                     ,�o�[�W������������Ȃ�)
ERROR_CODE(sindyErr_VersionNotCreated                   ,�o�[�W�����쐬�Ɏ��s)
ERROR_CODE(sindyErr_VersionNotChanged                   ,�o�[�W�����ύX�Ɏ��s)
ERROR_CODE(sindyErr_VersionNotDeleted                   ,�o�[�W�����폜�Ɏ��s)
ERROR_CODE(sindyErr_EditAlreadyStarted                  ,���ɕҏW���J�n����Ă���)
ERROR_CODE(sindyErr_EditNotStarted                      ,�ҏW���J�n����Ă��Ȃ�)
ERROR_CODE(sindyErr_EditWorkspaceNotFound               ,�ҏW���̃��[�N�X�y�[�X��������Ȃ�)
ERROR_CODE(sindyErr_EditWorkspaceIsNotVersioned         ,�ҏW���̃��[�N�X�y�[�X�̓o�[�W�����Ή��ł͂Ȃ�)
ERROR_CODE(sindyErr_FDOReconcileVersionNotAvailable     ,Unable to reconcile: the target version is currently being reconciled against[FDO_E_RECONICLE_VERSION_NOT_AVAILABLE = [-2147217137]].)
ERROR_CODE(sindyErr_FDOVersionBeingEdited               ,Operation not allowed while the version is being edited[FDO_E_VERSION_BEING_EDITED = [-2147217139]].)
ERROR_CODE(sindyErr_FDOVersionNotFound                  ,The version could not be located[FDO_E_VERSION_NOT_FOUND = [-2147217146]].)
ERROR_CODE(sindyErr_FDOVersionIsProtected               ,Operation not allowed because the version is protected[FDO_E_VERSION_IS_PROTECTED = [-2147217141]].)
ERROR_CODE(sindyErr_FDOVersionReconcileLost             ,Post not allowed after undoing a reconcile[FDO_E_VERSION_RECONCILE_LOST = [-2147217136].)
ERROR_CODE(sindyErr_FDOVersionRedefined                 ,The version has been redefined to reference a new database state[FDO_E_VERSION_REDEFINED = [-2147217147].)
ERROR_CODE(sindyErr_RowCreateFailed                     ,�t�B�[�`���i���R�[�h�j�̍쐬�Ɏ��s)
////////////////////////////////////////////////////////
// �P�̃`�F�b�N�i�I�u�W�F�N�g�P�ʃ`�F�b�N�G���[�j
////////////////////////////////////////////////////////
// ���H�n
ERROR_CODE(sindyErr_RoadHasNoFromID                     ,���H�����N��FROM�m�[�hID���Ȃ�)
ERROR_CODE(sindyErr_RoadHasNoToID                       ,���H�����N��TO�m�[�hID���Ȃ�)
ERROR_CODE(sindyErr_RoadFromToPointIsEqualNodePoint     ,���H�����N��FROM�ATO�̍��W�ƁA�֘A���Ă���m�[�h�̍��W����v���Ȃ�)
ERROR_CODE(sindyErr_SegAttrGeometryNotWithinRoad        ,�����N�������̌`�󂪓��H�����N�̌`��Ɋ܂܂�Ă��Ȃ�)
ERROR_CODE(sindyErr_GradGeometryNotWithinRoad           ,�X�΂̌`�󂪓��H�����N�̌`��Ɋ܂܂�Ă��Ȃ�)
ERROR_CODE(sindyErr_RoadNodeClassInvalid                ,�m�[�h��ʃR�[�h����������)
ERROR_CODE(sindyErr_InfTableIsNotMatchLQTable           ,INF�̃e�[�u���Ƒ΂ɂȂ�LQ�̃e�[�u���ł͂Ȃ�)
ERROR_CODE(sindyErr_InfIDInvalid                        ,INF��OID��LQ��INF_ID����v���Ȃ�)
ERROR_CODE(sindyErr_NodeOfMeshEdgeNotExistMeshEdge      ,�񎟃��b�V��������iPEC�j�m�[�h�Ȃ̂Ƀ��b�V������ɑ��݂��Ȃ�)
ERROR_CODE(sindyErr_NormalNodeExistMeshEdge             ,�񎟃��b�V��������iPEC�j�m�[�h�ȊO�Ȃ̂Ƀ��b�V������ɑ��݂��Ă���)
ERROR_CODE(sindyErr_RoadMiddlePointExistMeshEdge        ,���H�����N�̒[�_�ȊO���񎟃��b�V��������ɑ��݂��Ă���)
ERROR_CODE(sindyErr_RoadSegmentCrossMeshEdge            ,���H�����N���񎟃��b�V�������ׂ��ł���)
ERROR_CODE(sindyErr_NodeExist4MeshEdge                  ,�m�[�h���񎟃��b�V��4���ɑ��݂��Ă���)
ERROR_CODE(sindyErr_RoadEndPointExist4MeshEdge          ,���H�����N�̒[�_���񎟃��b�V��4���ɑ��݂��Ă���)
ERROR_CODE(sindyErr_NetworkLinkLoop                     ,���H�����N�����[�v���Ă��܂�)

ERROR_CODE(sindyErr_RuleRelationshipContainGeometryNotWithinRoad  ,�`�󂪓��H�����N�̌`��Ɋ܂܂�Ă��Ȃ�)
ERROR_CODE(sindyErr_RuleRelationshipContainGeometryNotOnWalkNode  ,�`��̎n�I�_�����s�҃m�[�h��ɂ���܂���)

ERROR_CODE(sindyErr_LinkQueueNotRoad                    ,�����N��ɓ��H���Ȃ����̂�����܂�)
ERROR_CODE(sindyErr_RoadBadLQRF                         ,�����N�񑶍݃t���O������������܂���)
ERROR_CODE(sindyErr_LinkQueueBadDirection               ,�����N��̌����Ɠ��H�̌����������Ă��܂���)
ERROR_CODE(sindyErr_LinkQueueBadSequence                ,�����N��̏���������������܂���)
ERROR_CODE(sindyErr_LinkQueueNotLastLinkF               ,�����N��̏I�[�t���O������܂���)
ERROR_CODE(sindyErr_LinkQueueLastLinkFs                 ,�����N��̏I�[�t���O����������܂�)

// ���L�n
ERROR_CODE(sindyErr_AnnoStrLenNotSame                   ,���L������̎��ۂ̒����ƒ��L������������Ȃ�)
ERROR_CODE(sindyErr_AnnoStrIsNotHalfNumber              ,���L�������ʂ������ԍ��E��v�n�����ԍ��E�R�x�Ȃ̂ɔ��p�����ȊO���g�p����Ă���)
ERROR_CODE(sindyErr_AnnoStrFistIsHalfZero               ,���L�������ʂ������ԍ��E��v�n�����ԍ��E�R�x�̎��ɔ��p������0���擪�ɂ���)
ERROR_CODE(sindyErr_AnnoStrIsNotWide                    ,���L�������ʂ��S�p������݂̂̎�ʂȂ̂Ɏw�肳�ꂽ�S�p�����ȊO�̂��̂��g�p����Ă���)
ERROR_CODE(sindyErr_AnnoStrMountainInvalid              ,���L�������ʂ��R�x�̎��̕����񂪂�������)
ERROR_CODE(sindyErr_AnnoStrICJCFTMustOver4              ,�C���^�[�`�F���W�A�W�����N�V�����A�t�F���[�^�[�~�i���Ȃ̂�4�����ȉ��ɂȂ��Ă���)
ERROR_CODE(sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT    ,�C���^�[�`�F���W�A�W�����N�V�����A�t�F���[�^�[�~�i���Ȃ̂ɍŌ��2�������u�h�b�v�A�u�i�b�v�A�u�e�s�v�ł͂Ȃ�)
ERROR_CODE(sindyErr_AnnoStrJCButLast3IsNotJCT           ,�s�s�n�}���L�Ŋ��W�����N�V�����Ȃ̂ɍŌ��3�������u�i�b�s�v�ł͂Ȃ�)
ERROR_CODE(sindyErr_AnnoStrFirstIsWideZero              ,���L�������ʂ��S�p������݂̂̎�ʂȂ̂ɐ擪�̕������S�p0�ɂȂ��Ă���)
ERROR_CODE(sindyErr_AnnoStrRouteNumberOverFlow          ,���L�������ʂ���v�n�����ԍ��E�����ԍ��̂Ƃ��ɓ��͒l���ԍ��͈̔͂��z���Ă���)
ERROR_CODE(sindyErr_AnnoStrInvalidChouon                ,�S�p�����L�����S�p�Ђ炪�ȁE�S�p�J�^�J�i�ȊO�̌�ɂ��Ă���)
ERROR_CODE(sindyErr_AnnoStrLenInvalid                   ,���L�����񒷂�����1<=����2<=����3�ɂȂ��Ă��Ȃ�)
ERROR_CODE(sindyErr_AnnoStrAngleInvalid                 ,���L������ׂ̗荇��3�_�̂Ȃ��p���s��)
ERROR_CODE(sindyErr_AnnoCodeInvalid                     ,���L��ʂ���������)
ERROR_CODE(sindyErr_AnnoDispTypeInvalid                 ,���L��ʂƕ\���^�C�v�̐���������Ȃ�)
ERROR_CODE(sindyErr_AnnoDispLineIllegalDouble           ,�s���ȓ�s���L)
ERROR_CODE(sindyErr_AnnoDispPositionInvalid             ,�^�ʒu�ƕ\���ʒu�̎�ʕʂ̈ʒu�֌W����������)
ERROR_CODE(sindyErr_AnnoDispTargetStrInvalid            ,�\���ʒu�̃^�[�Q�b�g������R�[�h����������)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalid             ,�\���ʒu�̃t�H���g�T�C�Y�R�[�h����������)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalidWarn         ,�\���ʒu�̃t�H���g�T�C�Y�R�[�h��10)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalidKanji        ,�\���ʒu�̃t�H���g�T�C�Y�R�[�h��10�̎��Ɋ������g�p���Ă���)
ERROR_CODE(sindyErr_AnnoStringNumNotEqualVertexNum      ,�\���ʒu�̍\���_���ƕ���������v���Ȃ�)
ERROR_CODE(sindyErr_AnnoDispRelIDInvalid                ,�\���ʒu�̊֘AID����������)
ERROR_CODE(sindyErr_AnnoDispLineTypeInvalid             ,�\���ʒu�̃��C���^�C�v�����[���ɓK�����Ȃ�)
ERROR_CODE(sindyErr_AnnoDispExceptExist                 ,���������Ă͂����Ȃ���ʂŒ��������Ă���)
ERROR_CODE(sindyErr_AnnoDispBoundsIsOverlaped           ,���L�\�������񓯎m���d�Ȃ��Ă���)
ERROR_CODE(sindyErr_AnnoMarkAndMarkIsOverlaped          ,���L�L�����m���d�Ȃ��Ă���)
ERROR_CODE(sindyErr_AnnoMarkAndDispBoundsIsOverlaped    ,���L�L���ƁA���̋L���ɑ����Ȃ����̒��L�����񂪏d�Ȃ��Ă���)
ERROR_CODE(sindyErr_AnnoMarkAndOtherMeshIsOverlaped     ,���L�L���ƁA���̃��b�V���̒��L�i�L��or������j���d�Ȃ��Ă���)
ERROR_CODE(sindyErr_AnnoDispAndOtherMeshIsOverlaped     ,���L������ƁA���̃��b�V���̒��L�i�L��or������j���d�Ȃ��Ă���)
ERROR_CODE(sindyErr_AnnoToStringBoundsLengthLimitOver   ,�^�ʒu���璍�L������܂ł̋������z��͈͊O)
ERROR_CODE(sindyErr_AnnoNoMarkButStringBoundsOutside    ,������݂̂Ȃ̂ɕ�����|���S�����ɐ^�ʒu���Ȃ�)
ERROR_CODE(sindyErr_AnnoFlagButStringBoundsInside       ,���g���Ȃ̂ɕ�����|���S�����ɐ^�ʒu������)
ERROR_CODE(sindyErr_AnnoDispLineShouldNotExist          ,������\�������Ȃ̂ɑΉ��X�P�[���̕\���ʒu�����݂���)
ERROR_CODE(sindyErr_AnnoDispLineShouldExist             ,������\���L��Ȃ̂ɑΉ��X�P�[���̕\���ʒu�����݂��Ȃ�)
// ���[���n
ERROR_CODE(sindyErr_NoLogin                             ,���O�C�����Ă��Ȃ�)
ERROR_CODE(sindyErr_LoginFailed                         ,���O�C���Ɏ��s����)
ERROR_CODE(sindyErr_LoginCanceled                       ,���O�C�����L�����Z������)
ERROR_CODE(sindyErr_RuleNotFound                        ,�K�؂ȕҏW���[����������Ȃ�)
ERROR_CODE(sindyErr_RuleCantCached                      ,���[���̃L���b�V������邱�Ƃ��ł��Ȃ�����)
ERROR_CODE(sindyErr_RuleNotAllowedMove                  ,�ҏW���[���ňړ�����������Ă���)
ERROR_CODE(sindyErr_RuleNotAllowedCreate                ,�ҏW���[���ō쐬����������Ă���)
ERROR_CODE(sindyErr_RuleNotAllowedDelete                ,�ҏW���[���ō폜����������Ă���)
ERROR_CODE(sindyErr_RuleNotAllowedEditAttribute         ,�ҏW���[���ő����ҏW����������Ă���)
ERROR_CODE(sindyErr_RuleNotAllowedEditByLockArea        ,�G���A�����b�N����Ă���)
ERROR_CODE(sindyErr_RuleNotAllowedEditByEditArea        ,�G���A���ҏW�s�\�ł���)
ERROR_CODE(sindyErr_AlreadyModified                     ,���ɕҏW����Ă���)
////////////////////////////////////////////////////////
// �֘A���`�F�b�N�i�I�u�W�F�N�g�Ԃ̘_���`�F�b�N�G���[�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_RelationIDInvalid                   ,�֘AID��OBJECTID����v���Ȃ�)
ERROR_CODE(sindyErr_RelFeatureNotFound                  ,�֘A�t�B�[�`����������Ȃ�����)
ERROR_CODE(sindyErr_SearchFailed                        ,�����Ɏ��s����)
ERROR_CODE(sindyErr_BuildingStepMustExistOnBuilding     ,�K���͉ƌ`��ɑ��݂���K�v������܂��B)
ERROR_CODE(sindyErr_NoFeaturesForFilling                ,�͈͓��Ɍ����ߑΏۂ̃t�B�[�`���𔭌��o���܂���ł���)
ERROR_CODE(sindyErr_LinkCirculation                     ,�����N���z���Ă��܂��I)
ERROR_CODE(sindyErr_LinkBranches                        ,�����N�����򂵂Ă��܂��I)
ERROR_CODE(sindyErr_LinkNotContinuance                  ,�����N�񂪘A�����Ă��܂���I)
ERROR_CODE(sindyErr_NoXorFeatures                       ,XOR �ɊY������}�`���쐬�ł��܂���ł����B)
ERROR_CODE(sindyErr_NotAllowTouchMesh                   ,�X�΁E�����N�������͂Q�����b�V�����E�ɐڂ��邱�Ƃ͂ł��܂���)
ERROR_CODE(sindyErr_RoadNodeAlreadyExist                ,���Ƀm�[�h�����݂��Ă��܂��B)
ERROR_CODE(sindyErr_NotAllowIndependentNode             ,�P�ƃm�[�h�͍쐬�ł��܂���B)
ERROR_CODE(sindyErr_CreateOnlyOnRoadNode                ,���H�m�[�h��ȊO�ɂ͍쐬�ł��܂���B)
ERROR_CODE(sindyErr_BuildingStepMustExistOnOneBuilding  ,�K���͂P�̉ƌ`��ɑ��݂���K�v������܂��B)
ERROR_CODE(sindyErr_PolygonAlreadyExist                 ,�w�肵���ꏊ�ɂ́A���Ƀ|���S�������݂��Ă��܂��I)
ERROR_CODE(sindyErr_RoadLinkNotSelected                 ,���H�����N���I������܂���ł����B)
ERROR_CODE(sindyErr_LinkQueueNeedsTwoRoads              ,�����N��͂Q�ȏ�̃����N�ō\������K�v������܂��B)
ERROR_CODE(sindyErr_CreateInteriorPolygonFailed         ,�|���S���쐬���ɃG���[���������܂����B\nAddressBar �Ō��݂̃A�h���X���擾���ăo�O�񍐂��Ă��������B)
ERROR_CODE(sindyErr_NodeClassInvalid                    ,�m�[�h��ʂ̐ݒ�Ɏ��s���܂����B\n�ȉ��̂��Ƃ��m�F���Ă��������B\n\n�E�Q�����b�V�����m�[�h�ɕ����̓��H��ڑ��ł��Ȃ��B\n�E�Q�����b�V����ւ̒P�Ɠ��H�͍쐬�ł��Ȃ�)
ERROR_CODE(sindyErr_BuildingOnly                        ,�ƌ`���C���͒P�Ƃł͕ҏW�ł��܂���B�����K�����C�������邱�Ƃ��m�F���Ă��������B)
ERROR_CODE(sindyErr_NoSameNode							,��v����m�[�h��������Ȃ�)

ERROR_CODE(sindyErr_BadModelContain                     ,�܂ޑ��Ɗ܂܂�鑤�̌`�󂪈�v���܂���)
ERROR_CODE(sindyErr_BadModelContainContain              ,�܂܂�鑤������܂���)

////////////////////////////////////////////////////////
// �����`�F�b�N
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_CheckReturnWarning                  ,�I�u�W�F�N�g�_���`�F�b�N�Ń��[�j���O�����o)
ERROR_CODE(sindyErr_CheckReturnErr                      ,�I�u�W�F�N�g�_���`�F�b�N�ŃG���[�����o)
////////////////////////////////////////////////////////
// ���̑��̃G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_TableNotFound                       ,�e�[�u���̓ǂݍ��݂Ɏ��s����)
ERROR_CODE(sindyErr_FieldMapNotCreated                  ,�t�B�[���h�Ή��\�̍쐬�Ɏ��s����)
ERROR_CODE(sindyErr_FieldNotFound                       ,�t�B�[���h�����݂��Ȃ�)
ERROR_CODE(sindyErr_UnexpectedError                     ,�\�z�O�̃G���[���������܂����B)
ERROR_CODE(sindyErr_UserCanceled                        ,�L�����Z�����܂����B)
ERROR_CODE(sindyErr_CreateAttributeDlgFailed            ,�����_�C�A���O�̍쐬�Ɏ��s���܂����B)
ERROR_CODE(sindyErr_NoDataOnLQTable                     ,LQ �e�[�u���Ƀf�[�^������܂���ł����B)
ERROR_CODE(sindyErr_NoDataForOutput                     ,�o�͂���f�[�^������܂���B)
ERROR_CODE(sindyErr_TooManyDatas                        ,�o�͂���f�[�^�ʂ��������܂��B\n��x�ɏo�͂ł���f�[�^��1000�s�܂łł��B)
ERROR_CODE(sindyErr_NoSearchResult                      ,�������ʂ͂���܂���ł����B)
ERROR_CODE(sindyErr_UpdateLQTableFailed                 ,�����N��e�[�u���iLQ�j�̍X�V�Ɏ��s���܂����B)
ERROR_CODE(sindyErr_UpdateInfTableFailed                ,�����N��e�[�u���iINFO�j�̍X�V�Ɏ��s���܂����B)
ERROR_CODE(sindyErr_DeleteLQFailed                      ,�����N��̍폜�Ɏ��s���܂����B)
ERROR_CODE(sindyErr_SameConditionExists                 ,���ɒǉ�����Ă�������ł��B)
ERROR_CODE(sindyErr_InvalidSearchCondition              ,�����Ɏ��s���܂����B\n�����������m�F���Ă��������B)
ERROR_CODE(sindyErr_CacheNotReflected                   ,�L���b�V���𔽉f���邱�Ƃ��ł��Ȃ�)
ERROR_CODE(sindyErr_UnknownTableType                    ,�z��O�̃e�[�u���^�C�v)
ERROR_CODE(sindyErr_FeatureCacheNotFound                ,�t�B�[�`���̃L���b�V�����쐬����Ă��Ȃ�)
ERROR_CODE(sindyErr_CantElasticTableType                ,�L�k�\�ȃe�[�u���^�C�v�ł͂Ȃ�)
ERROR_CODE(sindyErr_ObjectAlreadyExist					,�I�u�W�F�N�g�����ɑ��݂��Ă���)
ERROR_CODE(sindyErr_ObjectNotExist						,�I�u�W�F�N�g�����݂��Ȃ�)
////////////////////////////////////////////////////////
// ���C���n�̃G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_NoFeatureLayer                      ,�t�B�[�`�����C���ł͂���܂���B)
ERROR_CODE(sindyErr_ChukiLayer                          ,���L���C���ł��B)
ERROR_CODE(sindyErr_NoVisible                           ,��\�����C���ł��B)
ERROR_CODE(sindyErr_NoMovable                           ,�ړ��\�ȃ��C���ł͂���܂���B)
ERROR_CODE(sindyErr_NoDeletable                         ,�폜�\�ȃ��C���ł͂���܂���B)
ERROR_CODE(sindyErr_NoAttributeEditable                 ,�����ҏW�\�ȃ��C���ł͂���܂���B)
ERROR_CODE(sindyErr_SharedNoMovable                     ,���L�ړ��Ώۃ��C�����ړ��\���C���ł͂���܂���B)
ERROR_CODE(sindyErr_SharedNoLayer                       ,���L�ҏW�Ώۃ��C���i%s�j���ǂݍ��܂�Ă��܂���B)
ERROR_CODE(sindyErr_NoVisibleFeature                    ,���C�� : %s OBJECTID : %d : ��\�����C���ɑ����Ă��܂��B)
#else
ERROR_CODE(sindyErr_NoErr                               ,No error)
ERROR_CODE(sindyErr_NoImplimentation                    ,Not implemented)
////////////////////////////////////////////////////////
// ���W�X�g���E�ݒ�
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_RegNoSiNDY                          ,There are no SiNDY related settings in the registry.)
ERROR_CODE(sindyErr_RegNoInstallRoot                    ,There is no installation folder in the registry.)
ERROR_CODE(sindyErr_RegNoInifileName                    ,There is no ini file name in the registry.)
ERROR_CODE(sindyErr_IniNoServerName                     ,There is no server name in the ini file.)
ERROR_CODE(sindyErr_RegOpenFailed                       ,Failed to open the registry.)
ERROR_CODE(sindyErr_RegWriteFailed                      ,Failed to write to the registry.)
ERROR_CODE(sindyErr_RegReadFialed                       ,Failed to read the registry.)
////////////////////////////////////////////////////////
// �����`�F�b�N�i�v���O�������W�b�N�G���[�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_ArgIsNull                           ,The argument is NULL.)
ERROR_CODE(sindyErr_ArgLimitOver                        ,The range of the argument was unexpected.)
ERROR_CODE(sindyErr_ArgIsConsistency                    ,The arguments are inconsistent.)
////////////////////////////////////////////////////////
// �����o�`�F�b�N�i�v���O�������W�b�N�G���[�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_ClassMemberMissing                  ,There is not enough class members (initialization insufficient).)
////////////////////////////////////////////////////////
// �v���O�������W�b�N�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_FunctionFailed                      ,Failed to call a function.)
ERROR_CODE(sindyErr_MemoryAllocationFailed              ,Failed to acquire memory.)
ERROR_CODE(sindyErr_BufferIsNotEnough                   ,The buffer area is insufficient.)
ERROR_CODE(sindyErr_ThrowException                      ,An exception occurred.)
ERROR_CODE(sindyErr_InvalidTypeInfo                     ,The type information does not match.)
ERROR_CODE(sindyErr_AlgorithmFailed                     ,Algorithm error)
ERROR_CODE(sindyErr_MethodLocked                        ,Use of the function is prohibited.)
ERROR_CODE(sindyErr_ObjectLocked                        ,The object is locked.)
////////////////////////////////////////////////////////
// COM�G���[�i�v���O�������W�b�N�G���[�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_COMInterfaceIsNull                  ,The interface pointer is NULL.)
ERROR_CODE(sindyErr_COMCreateInstanceFailed             ,Failed to create an instance.)
ERROR_CODE(sindyErr_COMQueryInterfaceFailed             ,Failed to invoke QueryInterface.)
ERROR_CODE(sindyErr_COMFunctionFailed                   ,Failed to call a function.)
ERROR_CODE(sindyErr_COMWrappedIntarfaceIsNull           ,The COM interface that is supposed to be wrapped is NULL.)
////////////////////////////////////////////////////////
// ArcObjects�G���[�i�v���O�������W�b�N�G���[�H�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_AOConnectDBFailed                   ,Failed to connect to the database.)
ERROR_CODE(sindyErr_AOStoreFailed                       ,Failed to Store().)
ERROR_CODE(sindyErr_AODeleteFailed                      ,Failed to Delete().)
ERROR_CODE(sindyErr_AOCreateRowFailed                   ,Failed to CreateRow().)
ERROR_CODE(sindyErr_AOCreateFeatureFailed               ,Failed to CreateFeature().)
ERROR_CODE(sindyErr_AOGetValueFailed                    ,Failed to get_Value().)
ERROR_CODE(sindyErr_AOUnsupportValueType                ,The acquired record value is not of the expected type.)
ERROR_CODE(sindyErr_AOPutValueFailed                    ,Failed to put_Value().)
ERROR_CODE(sindyErr_AOLayerNotStartEdit                 ,This is not a layer for which editing has been started.)
ERROR_CODE(sindyErr_AOLayerNotAllowCreate               ,This is not a layer that can be created.)
ERROR_CODE(sindyErr_AOLayerNotSelected                  ,Failed to select the layer. Select one layer from the TOC.)
ERROR_CODE(sindyErr_AOGetWorkspaceFailed                ,The workspace could not be acquired.)
ERROR_CODE(sindyErr_AOGetFeatureClassFailed             ,Failed to acquire the feature class.)
ERROR_CODE(sindyErr_AOLayerNotAllowSelect               ,This layer cannot be selected.)
ERROR_CODE(sindyErr_AOGetFieldIndexFailed               ,The field index could not be acquired.)
ERROR_CODE(sindyErr_AOStartEditOperationFailed          ,Editing could not be started.)
ERROR_CODE(sindyErr_AOFeatureSearchError                ,An error occurred during the feature class search. \nThe error code is %d.)
ERROR_CODE(sindyErr_AOConnectEventsHasCookie            ,The event is already connected.)
ERROR_CODE(sindyErr_AOConnectEventsFailed               ,Failed to connect the event)
ERROR_CODE(sindyErr_AODisconnectEventsFailed            ,Failed to disconnect the event.)
ERROR_CODE(sindyErr_AOErr                               ,Unknown ArcObjects error.)
////////////////////////////////////////////////////////
// SDE �n�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_SDEConnectFailed                    ,Failed to connect to SDE by SDE C API)
ERROR_CODE(sindyErr_SDEDisconnectFailed                 ,Failed to disconnect from SDE by SDE C API)
ERROR_CODE(sindyErr_SDEAPICallFailed                    ,Failed to call the SDE C API)
ERROR_CODE(sindyErr_SDEInvalidateFieldType              ,Column type that cannot be looked up by SDE C API)
ERROR_CODE(sindyErr_SDEQueryFailed                      ,Failed to query by SDE C API)
ERROR_CODE(sindyErr_SDEGetStateInfoFailed               ,Failed to get the state information by SDE C API)
ERROR_CODE(sindyErr_SDEVersionChangeStateFailed         ,Failed to change the state of version by SDE C API)
////////////////////////////////////////////////////////
// �����n�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_FieldLengthLimitOver                ,The field length has been exceeded.)
ERROR_CODE(sindyErr_FieldRangeLimitOver                 ,The allowable field range has been exceeded.)
ERROR_CODE(sindyErr_FieldDomainLimitOver                ,A value not defined for the field domain has been set.)
ERROR_CODE(sindyErr_FieldPrecisionLimitOver             ,The number of digits for the field has been exceeded.)
ERROR_CODE(sindyErr_SetNullValueToNotNullField          ,A NULL value has been set for a NOT NULL field.)
////////////////////////////////////////////////////////
// �W�I���g���n�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_GeometryNotFound                    ,The geometry does not exist.)
ERROR_CODE(sindyErr_GeometryNotSupported                ,Unsupported geometry)
ERROR_CODE(sindyErr_GeometryPartTooMach                 ,Two or more point/line geometry parts exist.)
ERROR_CODE(sindyErr_GeometryExteriorTooMach             ,Two or more polygon geometry exteriors exist.)
ERROR_CODE(sindyErr_GeometryNotCached                   ,Cache was not created for an operation that required geometry cache.)
ERROR_CODE(sindyErr_GeometryPointNotFound               ,The specified composition point does not exist.)
ERROR_CODE(sindyErr_GeometryFunctionFailed              ,Failed to calculate the geometry.)
ERROR_CODE(sindyErr_GeometryChangeToNull                ,An operation that results in the loss of the geometry is not possible.)
ERROR_CODE(sindyErr_GeometrySegmentCountIsNotEnough     ,There is not enough segments to make up the geometry.)
ERROR_CODE(sindyErr_GeometrySegmentTooShort             ,The segments that make up the geometry are too short.)
ERROR_CODE(sindyErr_GeometrySegmentsAngleOverFlow       ,The angles between the segments that make up the geometry are outside of the allowable range.)
ERROR_CODE(sindyErr_GeometryOverlapped                  ,The geometry duplicates another geometry.)
ERROR_CODE(sindyErr_GeometryOneselfTouches              ,The geometry touches itself.)
ERROR_CODE(sindyErr_GeometryRooped                      ,The geometry is looped.)
ERROR_CODE(sindyErr_GeometryNonSimple                   ,The geometry is not simple.)
ERROR_CODE(sindyErr_GeometryNonSimpleRingOrientation    ,A polygon is topologically simple�A but its rings may not be oriented correctly (outer rings - cw�A inner rings - ccw).)
ERROR_CODE(sindyErr_GeometryNonSimpleSegmentOrientation ,Individual segments are not consistantly oriented. The 'to' point of seg i should be incident on the 'from' point of seg i+1.)
ERROR_CODE(sindyErr_GeometryNonSimpleShortSegments      ,Some segments are shorter than allowed by the system units of the spatial reference associated with the geometry.)
ERROR_CODE(sindyErr_GeometryNonSimpleSelfIntersections  ,The interior of each part (rings�A planar paths) must not intersect itself or other parts.)
ERROR_CODE(sindyErr_GeometryNonSimpleUnclosedRing       ,The last segment in a ring must have its 'to' point incident on the 'from' point of the first segment.)
ERROR_CODE(sindyErr_GeometryNonSimpleEmptyPart          ,The geometry contains an empty part.)      
////////////////////////////////////////////////////////
// �ҏW�n�G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_EditStartFailed                     ,Failed to invoke StartEditing.)
ERROR_CODE(sindyErr_EditStopFailed                      ,Failed to invoke StopEditing.)
ERROR_CODE(sindyErr_EditMustBeReconcile                 ,Failed to invoke StopEditing (reconciliation is required).)
ERROR_CODE(sindyErr_EditOperationStartFailed            ,Failed to invoke StartEditOperaton.)
ERROR_CODE(sindyErr_EditOperationStopFailed             ,Failed to invoke StopEditOperation.)
ERROR_CODE(sindyErr_EditOperationAbortFailed            ,Failed to invoke AbortEditOperation.)
ERROR_CODE(sindyErr_EditReconcileFailed                 ,Failed to reconcile)
ERROR_CODE(sindyErr_EditPostFailed                      ,Failed to post)
ERROR_CODE(sindyErr_EditConflictDetected                ,Editing is conflicting)
ERROR_CODE(sindyErr_EditCantRecognizedOriginalEdit      ,It is not possible to determine whether local reconcile or custom post)
ERROR_CODE(sindyErr_EditCustomPostNoEXTCLSID            ,Cannot custom post because EXTCLSID is not registered)
ERROR_CODE(sindyErr_EditParentIsProtected               ,Local reconcile is not possible because parent version is protected)
ERROR_CODE(sindyErr_EditParentIsNotProtected            ,Custom post is not possible because parent version is not protected)
ERROR_CODE(sindyErr_EditVersionIsNotChild               ,The version name under editing is not the child version)
ERROR_CODE(sindyErr_EditParentVersionIsChild            ,The parent version name of the version under editing is child version)
ERROR_CODE(sindyErr_EditCustomPostFailed                ,Failed to custom post)
ERROR_CODE(sindyErr_EditLocalReconcileFailed            ,Failed to local reconcile)
ERROR_CODE(sindyErr_VersionNotFound                     ,Version cannot be found)
ERROR_CODE(sindyErr_VersionNotCreated                   ,Failure to create version)
ERROR_CODE(sindyErr_VersionNotChanged                   ,Failure to modify version)
ERROR_CODE(sindyErr_VersionNotDeleted                   ,Failure to delete version)
ERROR_CODE(sindyErr_EditAlreadyStarted                  ,Editing has already been started.)
ERROR_CODE(sindyErr_EditNotStarted                      ,Editing has not been started.)
ERROR_CODE(sindyErr_EditWorkspaceNotFound               ,The workspace being edited could not be found.)
ERROR_CODE(sindyErr_EditWorkspaceIsNotVersioned         ,The workspace being edited is not a supported version.)
ERROR_CODE(sindyErr_FDOReconcileVersionNotAvailable     ,Unable to reconcile: the target version is currently being reconciled against[FDO_E_RECONICLE_VERSION_NOT_AVAILABLE = [-2147217137]].)
ERROR_CODE(sindyErr_FDOVersionBeingEdited               ,Operation not allowed while the version is being edited[FDO_E_VERSION_BEING_EDITED = [-2147217139]].)
ERROR_CODE(sindyErr_FDOVersionNotFound                  ,The version could not be located[FDO_E_VERSION_NOT_FOUND = [-2147217146]].)
ERROR_CODE(sindyErr_FDOVersionIsProtected               ,Operation not allowed because the version is protected[FDO_E_VERSION_IS_PROTECTED = [-2147217141]].)
ERROR_CODE(sindyErr_FDOVersionReconcileLost             ,Post not allowed after undoing a reconcile[FDO_E_VERSION_RECONCILE_LOST = [-2147217136].)
ERROR_CODE(sindyErr_FDOVersionRedefined                 ,The version has been redefined to reference a new database state[FDO_E_VERSION_REDEFINED = [-2147217147].)
ERROR_CODE(sindyErr_RowCreateFailed                     ,Failed to create the feature (record).)
////////////////////////////////////////////////////////
// �P�̃`�F�b�N�i�I�u�W�F�N�g�P�ʃ`�F�b�N�G���[�j
////////////////////////////////////////////////////////
// ���H�n
ERROR_CODE(sindyErr_RoadHasNoFromID                     ,Road link does not have FROM node ID)
ERROR_CODE(sindyErr_RoadHasNoToID                       ,Road link does not have TO node ID)
ERROR_CODE(sindyErr_RoadFromToPointIsEqualNodePoint     ,The coordinates of FROM and TO of road link and those of related nodes do not match)
ERROR_CODE(sindyErr_SegAttrGeometryNotWithinRoad        ,Shape of the road structure (Segment Attr) is not included in the shape of a road link)
ERROR_CODE(sindyErr_GradGeometryNotWithinRoad           ,Shape of the slope (gradient) is not included in the shape of a road link)
ERROR_CODE(sindyErr_RoadNodeClassInvalid                ,Node type code is invalid)
ERROR_CODE(sindyErr_InfTableIsNotMatchLQTable           ,It is not a LQ table paired with INF table)
ERROR_CODE(sindyErr_InfIDInvalid                        ,OID of INF and INF_ID of LQ do not match)
ERROR_CODE(sindyErr_NodeOfMeshEdgeNotExistMeshEdge      ,The node is not on the mesh boundary line even though it is a "node on the secondary mesh boundary line")
ERROR_CODE(sindyErr_NormalNodeExistMeshEdge             ,The node is on the mesh boundary line even though it is not a "node on the secondary mesh boundary line")
ERROR_CODE(sindyErr_RoadMiddlePointExistMeshEdge        ,A point other than the end point of a road link is present on the secondary mesh boundary line.)
ERROR_CODE(sindyErr_RoadSegmentCrossMeshEdge            ,Road link is across the secondary mesh boundary)
ERROR_CODE(sindyErr_NodeExist4MeshEdge                  ,Node is present at the four corners of the secondary mesh)
ERROR_CODE(sindyErr_RoadEndPointExist4MeshEdge          ,End points of a road link is present at the four corners of the secondary mesh)
ERROR_CODE(sindyErr_NetworkLinkLoop                     ,Road link is a loop)

ERROR_CODE(sindyErr_RuleRelationshipContainGeometryNotWithinRoad  ,Shape is not included in the shape of a road link)
ERROR_CODE(sindyErr_RuleRelationshipContainGeometryNotOnWalkNode  ,Start and end points of the shape are not on the pedestrian node)

ERROR_CODE(sindyErr_LinkQueueNotRoad                    ,There is a link queue without corresponding road)
ERROR_CODE(sindyErr_RoadBadLQRF                         ,Link queue presence flag is incorrect)
ERROR_CODE(sindyErr_LinkQueueBadDirection               ,Direction of road network and link queue does not match)
ERROR_CODE(sindyErr_LinkQueueBadSequence                ,Link queue sequence is incorrect)
ERROR_CODE(sindyErr_LinkQueueNotLastLinkF               ,Termination flag of a link queue does not exist)
ERROR_CODE(sindyErr_LinkQueueLastLinkFs                 ,There are several termination flags for the link queue)

// ���L�n
ERROR_CODE(sindyErr_AnnoStrLenNotSame                   ,Number of characters does not match the actual length of the label string)
ERROR_CODE(sindyErr_AnnoStrIsNotHalfNumber              ,Although the label string class is "national highway number", "regional highway number", or "mountain", one-byte numeric number is not used)
ERROR_CODE(sindyErr_AnnoStrFistIsHalfZero               ,One-byte numeric number 0 is at the beginning when the label string class is "national highway number", "regional highway number", or "mountain")
ERROR_CODE(sindyErr_AnnoStrIsNotWide                    ,Although the label string class is only supposed to have two-byte character, those other than the specified two-byte characters are used)
ERROR_CODE(sindyErr_AnnoStrMountainInvalid              ,Character string is invalid when label string class is "mountain")
ERROR_CODE(sindyErr_AnnoStrICJCFTMustOver4              ,It is 4 characters or less even though it is an interchange, junction or ferry terminal)
ERROR_CODE(sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT    ,Although it is an interchange, junction or ferry terminal, the last 2 characters are not "IC", "JC", or "FT")
ERROR_CODE(sindyErr_AnnoStrJCButLast3IsNotJCT           ,Although it is a city map and a junction, the last 3 characters are not "JCT" )
ERROR_CODE(sindyErr_AnnoStrFirstIsWideZero              ,Although the label string class is only supposed to have two-byte character,  the first letter is a two-byte 0)
ERROR_CODE(sindyErr_AnnoStrRouteNumberOverFlow          ,Input value exceeds the number range when label string class is "national highway number" or "regional highway number")
ERROR_CODE(sindyErr_AnnoStrInvalidChouon                ,Two-byte macron is present behind the characters other than two-byte Hiragana and Katakana)
ERROR_CODE(sindyErr_AnnoStrLenInvalid                   ,Label string length is not Name1<=Name2<=Name3)
ERROR_CODE(sindyErr_AnnoStrAngleInvalid                 ,The angle formed by three adjacent points of a label string is invalid)
ERROR_CODE(sindyErr_AnnoCodeInvalid                     ,Label class is invalid)
ERROR_CODE(sindyErr_AnnoDispTypeInvalid                 ,Label class and display type do not match)
ERROR_CODE(sindyErr_AnnoDispLineIllegalDouble           ,Invalid two-line label)
ERROR_CODE(sindyErr_AnnoDispPositionInvalid             ,Positional relationship of each type of display position and true position is invalid)
ERROR_CODE(sindyErr_AnnoDispTargetStrInvalid            ,Target string code of the display position is invalid)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalid             ,Font size code of the display position is invalid)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalidWarn         ,Font size code of the display position is 10)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalidKanji        ,Kanji is used when font size code of the display position is 10)
ERROR_CODE(sindyErr_AnnoStringNumNotEqualVertexNum      ,Number of characters does not match the number of configuration points of the display position)
ERROR_CODE(sindyErr_AnnoDispRelIDInvalid                ,Relational ID of the display position is invalid)
ERROR_CODE(sindyErr_AnnoDispLineTypeInvalid             ,Line type of the display position does not match the rules)
ERROR_CODE(sindyErr_AnnoDispExceptExist                 ,A class that must be present in every scale is missing in some scale)
ERROR_CODE(sindyErr_AnnoDispBoundsIsOverlaped           ,Label display strings are overlapping)
ERROR_CODE(sindyErr_AnnoMarkAndMarkIsOverlaped          ,Label symbols are overlapping)
ERROR_CODE(sindyErr_AnnoMarkAndDispBoundsIsOverlaped    ,Label symbol and another label string that does not belong to the symbol are overlapping)
ERROR_CODE(sindyErr_AnnoMarkAndOtherMeshIsOverlaped     ,Label symbol and label (symbol or string) in another mesh are overlapping)
ERROR_CODE(sindyErr_AnnoDispAndOtherMeshIsOverlaped     ,Label string and label (symbol or string) in another mesh are overlapping)
ERROR_CODE(sindyErr_AnnoToStringBoundsLengthLimitOver   ,Distance from the true position to the label string is out of the assumed range)
ERROR_CODE(sindyErr_AnnoNoMarkButStringBoundsOutside    ,Although there is only character string, true position is not present inside of the character string polygon)
ERROR_CODE(sindyErr_AnnoFlagButStringBoundsInside       ,Although it is a balloon label, the true position is present inside of the character string polygon)
ERROR_CODE(sindyErr_AnnoDispLineShouldNotExist          ,Although it is "string no display", the display position of the corresponding scale is present)
ERROR_CODE(sindyErr_AnnoDispLineShouldExist             ,Although it is "string display", the display position of the corresponding scale is not present)
// ���[���n
ERROR_CODE(sindyErr_NoLogin                             ,You are not logged in.)
ERROR_CODE(sindyErr_LoginFailed                         ,Failed to log in.)
ERROR_CODE(sindyErr_LoginCanceled                       ,Cancelled login.)
ERROR_CODE(sindyErr_RuleNotFound                        ,Appropriate editing rules could not be found.)
ERROR_CODE(sindyErr_RuleCantCached                      ,Cache for the rules could not be created.)
ERROR_CODE(sindyErr_RuleNotAllowedMove                  ,Movement is limited by the editing rules.)
ERROR_CODE(sindyErr_RuleNotAllowedCreate                ,Creation is limited by the editing rules.)
ERROR_CODE(sindyErr_RuleNotAllowedDelete                ,Deletion is limited by the editing rules.)
ERROR_CODE(sindyErr_RuleNotAllowedEditAttribute         ,Attribute editing is limited by the editing rules.)
ERROR_CODE(sindyErr_RuleNotAllowedEditByLockArea        ,The area is locked.)
ERROR_CODE(sindyErr_RuleNotAllowedEditByEditArea        ,The area cannot be edited.)
ERROR_CODE(sindyErr_AlreadyModified                     ,Already edited.)
////////////////////////////////////////////////////////
// �֘A���`�F�b�N�i�I�u�W�F�N�g�Ԃ̘_���`�F�b�N�G���[�j
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_RelationIDInvalid                   ,The associated ID and OBJECTID do not match.)
ERROR_CODE(sindyErr_RelFeatureNotFound                  ,The associated features could not be found.)
ERROR_CODE(sindyErr_SearchFailed                        ,Failed to search.)
ERROR_CODE(sindyErr_BuildingStepMustExistOnBuilding     ,Building step must exist on a building )
ERROR_CODE(sindyErr_NoFeaturesForFilling                ,A feature with holes target to be filled could not be found within the range.)
ERROR_CODE(sindyErr_LinkCirculation                     ,Link is circulating!)
ERROR_CODE(sindyErr_LinkBranches                        ,Link is branching!)
ERROR_CODE(sindyErr_LinkNotContinuance                  ,Link queue is not continuous!)
ERROR_CODE(sindyErr_NoXorFeatures                       ,The geometry corresponding to XOR could not be created.)
ERROR_CODE(sindyErr_NotAllowTouchMesh                   ,Slope/road structure cannot be in contact with the secondary mesh boundary)
ERROR_CODE(sindyErr_RoadNodeAlreadyExist                ,A node already exists)
ERROR_CODE(sindyErr_NotAllowIndependentNode             ,You cannot create an independent node)
ERROR_CODE(sindyErr_CreateOnlyOnRoadNode                ,You can create it only on the road node)
ERROR_CODE(sindyErr_BuildingStepMustExistOnOneBuilding  ,Building step must exist on a building )
ERROR_CODE(sindyErr_PolygonAlreadyExist                 ,A polygon already exists at the specified location!)
ERROR_CODE(sindyErr_RoadLinkNotSelected                 ,Road link was not selected)
ERROR_CODE(sindyErr_LinkQueueNeedsTwoRoads              ,Link queue must be composed of more than two links.)
ERROR_CODE(sindyErr_CreateInteriorPolygonFailed         ,An error has occurred during the polygon creation. \n Please get the current address and report it as a bug.)
ERROR_CODE(sindyErr_NodeClassInvalid                    ,Failed to set the node class \n Please check the following. \n\n You cannot connect multiple roads to the secondary mesh node. \n\n You cannot create a single road on the secondary mesh.)
ERROR_CODE(sindyErr_BuildingOnly                        ,Building layer cannot be edited independently. Please make sure that there is a building_step layer.)
ERROR_CODE(sindyErr_NoSameNode							,Matching node cannot be found)

ERROR_CODE(sindyErr_BadModelContain                     ,The shapes of the containing side and contained side do not match.)
ERROR_CODE(sindyErr_BadModelContainContain              ,There is no contained side.)

////////////////////////////////////////////////////////
// �����`�F�b�N
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_CheckReturnWarning                  ,A warning was detected during the object logic check.)
ERROR_CODE(sindyErr_CheckReturnErr                      ,An error was detected during the object logic check.)
////////////////////////////////////////////////////////
// ���̑��̃G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_TableNotFound                       ,Failed to read the table.)
ERROR_CODE(sindyErr_FieldMapNotCreated                  ,Failed to create the field correspondence table.)
ERROR_CODE(sindyErr_FieldNotFound                       ,The field does not exist.)
ERROR_CODE(sindyErr_UnexpectedError                     ,An unexpected error occurred.)
ERROR_CODE(sindyErr_UserCanceled                        ,The process was cancelled.)
ERROR_CODE(sindyErr_CreateAttributeDlgFailed            ,Failed to create the attribute dialog box.)
ERROR_CODE(sindyErr_NoDataOnLQTable                     ,There was no data in LQ table)
ERROR_CODE(sindyErr_NoDataForOutput                     ,There is no data to output.)
ERROR_CODE(sindyErr_TooManyDatas                        ,There is too much data to output. \nUp to 1000 lines of data can be output at one time.)
ERROR_CODE(sindyErr_NoSearchResult                      ,There were no search results.)
ERROR_CODE(sindyErr_UpdateLQTableFailed                 ,Failed to update the link queue table (LQ).)
ERROR_CODE(sindyErr_UpdateInfTableFailed                ,Failed to update the link queue table (INFO).)
ERROR_CODE(sindyErr_DeleteLQFailed                      ,Failed to delete the link queue)
ERROR_CODE(sindyErr_SameConditionExists                 ,This condition has already been added.)
ERROR_CODE(sindyErr_InvalidSearchCondition              ,Failed to perform search. \nCheck the search conditions.)
ERROR_CODE(sindyErr_CacheNotReflected                   ,Cannot apply the data in cache.)
ERROR_CODE(sindyErr_UnknownTableType                    ,Unexpected table type)
ERROR_CODE(sindyErr_FeatureCacheNotFound                ,The feature cache has not been created.)
ERROR_CODE(sindyErr_CantElasticTableType                ,Table type that cannot be expanded or contracted.)
ERROR_CODE(sindyErr_ObjectAlreadyExist					,The object already exists.)
ERROR_CODE(sindyErr_ObjectNotExist						,The object does not exist.)
////////////////////////////////////////////////////////
// ���C���n�̃G���[
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_NoFeatureLayer                      ,This is not a feature layer.)
ERROR_CODE(sindyErr_ChukiLayer                          ,This is Annotation layer.)
ERROR_CODE(sindyErr_NoVisible                           ,This is an undisplayed layer.)
ERROR_CODE(sindyErr_NoMovable                           ,This is not a layer that can be moved.)
ERROR_CODE(sindyErr_NoDeletable                         ,This is not a layer that can be deleted.)
ERROR_CODE(sindyErr_NoAttributeEditable                 ,This is not a layer for which the attributes can be edited.)
ERROR_CODE(sindyErr_SharedNoMovable                     ,The shared movement target layer is not a layer that can be moved.)
ERROR_CODE(sindyErr_SharedNoLayer                       ,The shared edit target layer (%s) has not been read.)
ERROR_CODE(sindyErr_NoVisibleFeature                    ,Layer: %s OBJECTID: %d: Belongs to an undisplayed layer.)
#endif // ifndef SINDY_FOR_ENGLISH
