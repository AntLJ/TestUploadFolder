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
 * @file QueryFunctions.h
 * @brief <b>ArcObjects�O���[�o���w���p�[�֐���`�t�@�C��</b>\n
 * ���̃t�@�C���ɂ́A��ɃN�G���Ɋւ���O���[�o���֐�����`����Ă��܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ARCHELPEREX_GLOBALFUNCTIONS_H_
#define _ARCHELPEREX_GLOBALFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief �N�G���t�B���^�ݒ�p�֐�
 * �C���X�^���X���쐬����Ă��Ȃ��ꍇ�̓C���X�^���X��CLSID_QueryFilter�ō쐬���܂��B
 * �C���X�^���X���쐬����Ă���΁A���݂̐ݒ���㏑�����܂��B
 * 
 * ��ԃN�G���t�B���^���쐬����ꍇ�́A���AheInitSpatialFilter���g�p���Ă��������B
 *
 * @return IQueryFilterPtr
 */
IQueryFilterPtr AheInitQueryFilter( IQueryFilter* ipFilter							= NULL,
											LPCTSTR lpcszSubFields					= NULL,
											LPCTSTR lpcszWhereClause				= NULL,... );
/**
 * @brief ��ԃN�G���t�B���^�ݒ�p�֐�
 * �C���X�^���X���쐬����Ă��Ȃ��ꍇ�̓C���X�^���X��CLSID_SpatialFilter�ō쐬���܂��B
 * �C���X�^���X���쐬����Ă���΁A���݂̐ݒ���㏑�����܂��B
 * 
 * �T�u�t�B�[���h��WHERE��̐ݒ��AheInitQueryFilter���g�p���Ă��������B
 *
 * @param ipFilter						[in,optional]	�ݒ肷��t�B���^�BNULL�̏ꍇ�͐V�K�ɍ쐬���܂��B�i�f�t�H���g�FNULL�j
 * @param pGeom							[in,optional]	�����G���A�i�f�t�H���g�FNULL�j
 * @param lpcszGeomField				[in,optional]	�`��t�B�[���h���i�f�t�H���g�FNULL�j
 *                                                      �K��IFeatureClass::get_ShapeFieldName()�̖��̂��g�p���Ă�������
 * @param order							[in,optional]	�����D�揇�ʁi�f�t�H���g�FesriSearchOrderSpatial�j
 * @param SpatialRel					[in,optional]	��Ԍ������@�i�f�t�H���g�FesriSpatialRelIntersects�j
 * @param lpcszSpatialRelDescription	[in,optional]	��Ԍ������@�w�蕶����i�f�t�H���g�FNULL�j
 *
 * @return ISpatialFilterPtr
 */
ISpatialFilterPtr AheInitSpatialFilter( ISpatialFilter* ipFilter					= NULL,
												IGeometry* pGeom					= NULL,
												LPCTSTR lpcszGeomField				= _T("Shape"),
												esriSearchOrder order				= esriSearchOrderSpatial,
												esriSpatialRelEnum SpatialRel		= esriSpatialRelIntersects,
												LPCTSTR lpcszSpatialRelDescription	= NULL );

/// OBJECTID���擾����
long AheGetOID( _IRow* ipRow );

/// �t�B�[�`������`����擾����
IGeometryPtr AheGetShape( IFeature* ipFeature );

/// �t�B�[�`���`��R�s�[���擾����
IGeometryPtr AheGetShapeCopy( IFeature* ipFeature );

/**
 * @brief <b>��`->�|���S���ϊ�</b>\n
 * @return �|���S��
 */
//IPolygonPtr SINDYLIB_API AheEnvelope2Polygon( IEnvelope* ipEnv ); <- CGeometry::ToPolygon()

/**
 * @brief <b>COM�I�u�W�F�N�g�̃R�s�[���쐬����</b>\n
 * IClone�C���^�[�t�F�[�X�������Ă���K�v������܂��B
 * @param ipUnk	[in]	IUnknown*
 * @return IClonePtr
 */
IClonePtr AheCreateClone( IUnknown* ipUnk );

/**
 * @brief <b>���C���t�@�C�����烌�C�����擾����</b>\n
 * @param lpcszLayerFile	[in]	���C���t�@�C���i�t���p�X�j
 * @return ILayerPtr
 */
ILayerPtr AheOpenLayerFromFile( LPCTSTR lpcszLayerFile );

/**
 * @brief <b>�v���p�e�B�Z�b�g���r����</b>\n
 * ��r����v���p�e�B�͈ȉ��̂Ƃ���F\n
 * @li USER
 * @li SERVER
 * @li VERSION
 * @param ipProp1	[in]	�v���p�e�B�Z�b�g�P
 * @param ipProp2	[in]	�v���p�e�B�Z�b�g�Q
 * @return bool
 */
bool AheIsSamePropertySet( IPropertySet* ipProp1, IPropertySet* ipProp2 );

/**
 * @brief <b>�Z�p���[�^�t������������X�g�Ɋi�[����</b>\n
 * �Ή��ł���`���Fstr1;str2;str3�Ȃ�
 * @param lpcszTargetStr	[in]	����������������
 * @param lpcszSep			[in]	�Z�p���[�^
 * @param listStr			[out]	������̃��X�g
 */
void AheSepStr2listString( LPCTSTR lpcszTargetStr, LPCTSTR lpcszSep, std::list<CString>& listStr );

/**
 * @brief <b>�Z�p���[�^�t����������}�b�v�Ɋi�[����</b>\n
 * �Ή��ł���`���Fstr1=hoge1;str2=hoge2;str3=hoge3�Ȃ�\n
 * ��L�̏ꍇ�́AlpcszSep1���u;�vlpcszSep2���u=�v�ɂȂ�܂��B
 * @param lpcszTargetStr	[in]	����������������
 * @param lpcszSep1			[in]	�Z�p���[�^�P
 * @param lpcszSep2			[in]	�Z�p���[�^�Q
 * @param mapStr			[out]	������̃}�b�v
 */
void AheSepStr2mapString( LPCTSTR lpcszTargetStr, LPCTSTR lpcszSep1, LPCTSTR lpcszSep2, std::map<CString,CString>& mapStr );

/**
 * @brief <b>��ԎQ�Ƃ̍ŏ��P�ʋ�����Ԃ�</b>\n
 * @param ipSpRef	[in]	��ԎQ��
 * @retval 0�ȉ�	�G���[
 * @retval ��L�ȊO	�ŏ��P�ʋ���(���e�l)
 */
double AheGetXYDomain1UnitLength( ISpatialReference* ipSpRef );

/**
 * @brief PGDB�ҏW���[�h���ǂ����𒲂ׂ�
 * @return PGDB�ҏW���[�h�Ȃ�true
 */
bool AheIsPGDBEditMode();

/**
 * @brief �d���������郌�C�����ǂ���
 * @return �d�����Ȃ�true
 */
bool AheIsOverlapableLayer( const CString& layername );

/**
 * @brief SiNDY-c�̃e�[�u���ɌŗL�̃t�B�[���h���ǂ����𒲂ׂ�
 * �ȉ��̃t�B�[���h����SiNDY-c�t�B�[���h�Ƃ݂Ȃ�\n
 * @li ORG_OBJID
 * @li ORG_MODIFIY_DATE
 * @li ORG_OPERATOR
 * @li CHECK_C
 * @param strField		[in]	�t�B�[���h��
 * @return ��L���ꂩ�Ȃ�true
 */
bool AheIsSindycField( const CString& strField );

// �g���[�X�n�֐�
/**
 * @brief <b>�W�I���g�����g���[�X����</b>\n
 * @param ipGeom		[in]	�W�I���g��
 * @param lpcszHeader	[in]	���b�Z�[�W
 */
#ifdef _DEBUG
void AheTraceGeometry( IGeometry* ipGeom, LPCTSTR lpcszHeader );
#else
#define AheTraceGeometry (void*)0;
#endif // ifdef _DEBUG

/**
 * @brief <b>��ԎQ�Ƃ��g���[�X����</b>\n
 * @param ipSpRef		[in]	��ԎQ��
 * @param lpcszHeader	[in]	���b�Z�[�W
 */
#ifdef _DEBUG
void AheTraceSpatialReference( ISpatialReference* ipSpRef, LPCTSTR lpcszHeader );
void AheTraceSpatialReference( IGeometry* ipGeom, LPCTSTR lpcszHeader );
inline void AheTraceSpatialReference( IGeometry* ipGeom, LPCTSTR lpcszHeader )
{
	if( ipGeom )
	{
		ISpatialReferencePtr ipSpRef;
		ipGeom->get_SpatialReference( &ipSpRef );
		AheTraceSpatialReference( ipSpRef, lpcszHeader );
	}
}
#else
#define AheTraceSpatialReference (void*)0;
#endif // ifdef _DEBUG

/**
 * @brief �����x�̋�ԎQ�Ƃ��ǂ���
 *
 * @param ipSpRef		[in]	��ԎQ��
 *
 * @return bool
 */
bool AheIsHighPrecision(ISpatialReferencePtr ipSpRef);

/**
 * @brief �n�C���C�g�ݒ���擾����
 * @note�@���Ń��W�X�g�������ɍs���܂�
 *
 * @param strXPath [in] �ݒ���擾����XML�m�[�h���w��XPath
 * @param nRed [out] RGB()�}�N���p�̒l(��)
 * @param nGreen [out] RGB()�}�N���p�̒l(��)
 * @param nBlue [out] RGB()�}�N���p�̒l(��)
 * 
 * @return �ݒ�̎擾�ɐ���������true�A���s������false
 */
bool GetHighlightColor( const _bstr_t& strXPath, int& nRed, int& nGreen, int& nBlue );

#endif // _ARCHELPEREX_GLOBALFUNCTIONS_H_
