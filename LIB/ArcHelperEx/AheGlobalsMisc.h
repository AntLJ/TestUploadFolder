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
 * @note AheGlobals.h/cpp�����Ɂu���̑��v�ɑ�����֐��Q�𕪗��������́B
 *       �o�܂͉��L���r���[���Q�Ƃ̎��B
 *       ArcHelperEx�̃��t�@�N�^�����O�icrd_cnv�֌W�j http://orias.mr.ipc.pioneer.co.jp/reviewboard/r/2424/
 *       AheGlobals��AheGlobalsCoord�ւ̕��������ōςނ��Ǝv��ꂽ���A
 *       �E�����R�[�h�ɉe����^���Ȃ��悤�ɂ��邽�߁AAheGlobals.h��AheGlobalsCoord.h��include����K�v������B
 *       �EAheFact()��AheGlobalsCoord.cpp���Ŏg�p����Ă���B
 *       �Ƃ������R����AAheFact()��AheGlobals.h�ɂ���ƃw�b�_�̏z�Q�Ƃ��������邽�߁A
 *       AheGlobalsMisc.h/cpp�Ƃ��ĕ�������K�v���������B
 *       ����ȊO�̊֐����ꏏ�Ɉړ������̂́A�P�Ƀ��C�u�����Ƃ��Ă̑̍ق̂��߂ł���B
 */
#pragma once

//////////////////////////////////////////////////////////////////////
// ���̑��̊֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief �t�B�[�`���̍\���_���擾����
 *
 * @param ipFeature			[in]			IFeature �̃C���X�^���X
 * @param lIndex			[in]			�擾����\���_�̃C���f�b�N�X
 *
 * @return IPoint �̃C���X�^���X
 */
IPointPtr AheGetPoint( IFeaturePtr ipFeature, LONG lIndex );

/**
 * @brief �K������߂鏈��
 *
 * @param lBase			[in]			�K�悷�錳�̐�
 * @param lCnt			[in]			�K�悷���
 *
 * @return �v�Z����
 */
long AheFact(long lBase, long lCnt);

/**
 * @brief ���ݕ\�����Ă���X�P�[���ŕ\���ł��郌�C�����ǂ����̔���
 *
 * @param piMap				[in]			IMap �̃C���X�^���X
 * @param pGeoFeatureLayer	[in]			IGeoFeatureLayer �̃C���X�^���X
 *
 * @retval TURE ���ݕ\�����Ă���X�P�[���ŕ\���\
 * @retval FALSE ���ݕ\�����Ă���X�P�[���ŕ\���s�\
 */
BOOL AheIsVisibleScale( IMapPtr piMap, IGeoFeatureLayerPtr pGeoFeatureLayer );

/**
 * @brief �v���O����ID����c�[�����擾����
 *
 * @note �c�[���o�[�ɑ��݂���ꍇ�̂݁A�擾�ł��܂��B�Ȃ��A�擾���ɐV���ɃC���X�^���X�͐������܂���B
 *
 * @param ipDoc				[in]			IDocumentPtr
 * @param lpcszProgramID	[in]			�v���O����ID
 *
 * @return ICommandPtr
 */
_ICommandPtr AheGetCommandByProgramID( IDocumentPtr ipDoc, LPCTSTR lpcszProgramID );

/**
 * @brief �v���O����ID����c�[�����擾����
 *
 * @note �c�[���o�[�ɑ��݂���ꍇ�̂݁A�擾�ł��܂��B�Ȃ��A�擾���ɐV���ɃC���X�^���X�͐������܂���B
 *
 * @param ipDoc				[in]			IDocumentPtr
 * @param lpcszProgramID	[in]			�v���O����ID
 *
 * @return ICommandItemPtr
 */
ICommandItemPtr AheGetCommandItemByProgramID( IDocumentPtr ipDoc, LPCTSTR lpcszProgramID );

/**
 * @brief �N���[�����擾����
 *
 * @note �N���[�����쐬�ł��Ȃ����̂�����܂�
 *
 * @param ipUnk				[in]	�N���[���쐬��
 *
 * @retval IClonePtr	�N���[��
 * @retval NULL			�쐬�ł��Ȃ�����
 */
IClonePtr AheGetClone( IUnknownPtr ipUnk );

/**
 * @brief VARIANT �l�̔�r
 *
 * @param va1				[in]			��r���� VARIANT �l
 * @param va2				[in]			��r��� VARIANT �l
 *
 * @retval TURE ��v
 * @retval FALSE �s��v
 */
BOOL IsSameVariantObject( VARIANT va1, VARIANT va2 );

/**
 * @brief IField �� NULL OK ���ǂ����̔���
 *
 * @param ipField			[in]			���肵���� IField �̃C���X�^���X
 *
 *
 * @retval TURE NULL������
 * @retval FALSE NULL�������Ȃ�
 */
BOOL AheIsNullableField( IFieldPtr ipField);

/**
 * @brief �W�I���g������|�����C���擾
 *
 * @param ipGeometry	[in]	�ϊ��W�I���g��
 *
 * @return IPolylinePtr
 */
BOOL AheGeometry2Polyline(IGeometryPtr ipGeometry, IPolyline** ppiPolyline);

/**
 * @brief �W�I���g������|�C���g�R���N�V�����擾
 *
 * @param ipGeometry	[in]	�ϊ��W�I���g��
 *
 * @return IPointCollectionPtr
 */
BOOL AheGeometry2PointCollection(IGeometryPtr ipGeometry, IPointCollection** ppiPointCollection);

/**
 * @brief ���E�ǂ������
 *
 * @param ipSPoint	[in]	�X�^�[�g�ۂ����
 * @param ipSPoint	[in]	�~�h���ۂ����
 * @param ipSPoint	[in]	�G���h�ۂ����
 *
 * @return TRUE:������
 * @return FALSE:�E����
 */
BOOL AheIsRightAndLeft(IPointPtr ipSPoint, IPointPtr ipMPoint, IPointPtr ipEPoint);

/**
 * @brief �n���Q�Ƃ����킹��
 *
 * @param ipGeom	[in]	IGeometryPtr
 * @param ipSpRef	[in]	ISpatialReferencePtr
 *
 * @retval TRUE ����
 * @retval FALSE ���s
 */
BOOL AheSnapToSpatialReference( IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef );

/**
 * @brief �����I�� Simplify ��������
 *
 * @note �`��ɂ���Ă����� Simplify ��ύX���Ă��܂�
 *
 * @param ipGeom	[in]	�W�I���g��
 *
 * @retval TRUE ����
 * @retval FALSE ���s
 */
BOOL AheForceSimplify( IGeometryPtr ipGeom );

/**
 * @brief �n���Q�Ƃ����킹����A�����I�� Simplify ��������
 *
 * @see AheSnapToSpatialReference
 * @see AheForceSimplify
 *
 * @param ipGeom	[in]	IGeometryPtr
 * @param ipSpRef	[in]	ISpatialReferencePtr
 *
 * @retval TRUE ����
 * @retval FALSE ���s
 */
BOOL AheSnapToSpatialReferenceAndForceSimplify( IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef );

/**
 * @brief �����񃊃\�[�X���當������擾����
 *
 * @return CString
 */
inline CString AheLoadString( UINT nID ){ CString str; if( str.LoadString( nID ) ) return str; else _ASSERTE(false); return (LPCTSTR)NULL; }

/**
 * @brief �����񃊃\�[�X���當������擾����
 *
 * @return CStringA
 */
inline CStringA AheLoadStringA( UINT nID ){ CStringA str; if( str.LoadString( nID ) ) return str; else _ASSERTE(false); return (LPCSTR)NULL; }

/**
 * @brief �����񃊃\�[�X���當������擾����
 *
 * @return CStringW
 */
inline CStringW AheLoadStringW( UINT nID ){ CStringW str; if( str.LoadString( nID ) ) return str; else _ASSERTE(false); return (LPCWSTR)NULL; }

CString AheGetErrorMessage();

void AheTraceErrorMessage( LPCTSTR lpcszMessage );

BOOL AheHasSameSpatialReference( IGeometryPtr ipGeom1, IGeometryPtr ipGeom2 );

/**
 * @brief �E�B���h�E�̃e�L�X�g���擾����
 *
 * @param hWnd		[in]	�E�B���h�E�n���h��
 *
 * @return CString
 */
CString AheGetWindowText( HWND hWnd );

/**
 * @brief ���݂̓��t�Ǝ����𕶎���ŕԂ�
 *
 * @param lpSystemTime	[in,optional]	�w�肷��Ƃ��̎��Ԃ𕶎���ɂ��ĕԂ��i�f�t�H���g�FNULL�j
 *
 * @return CString
 */
CString AheGetNow( LPSYSTEMTIME lpSystemTime = NULL );

/**
 * @brief VARIANT -> CString �ϊ�
 *
 * @param va			[in]	�ϊ��������l
 *
 * @return CString
 */
CString AheVariant2Str( const CComVariant &va );

/**
 * ������̒�����L�[���[�h�ƕ�����l���擾���A���l�ɕϊ����ĕԂ�
 *
 * @param CString&		[in/out] ������  
 * @param CString		[in]	 ������l
 * @param LPCTSTR		[in]	 �L�[���[�h
 * @param INT&			[out]	 ���l�̌���
 * @param INT			[in]	 0:���� or 1:����
 *
 * @retval TURE ����Ȑ��l
 * @retval FALSE �ُ�Ȑ��l
 */
BOOL AheGetKeywordValue( CString&, CString, LPCTSTR, CString&, INT );
