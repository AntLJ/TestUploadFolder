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
* @file ArcHelper.cpp
*
* @brief IApplication�Ɋ֘A���鏈�����s��ArcHelper�̎����t�@�C��
*/
#include "StdAfx.h"
#include "ArcHelper.h"
#include "AheGlobals.h"
#include "AheGlobalsX.h"
#include "InifileHelper.h"
#include "Map.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR WARN_DO_INITIALIZATION      = _T("CArcHelper::Init() �ŏ��������Ă�������");
	LPCTSTR INIT_WARN                   = _T("�x��");
	LPCTSTR MESSAGE_BFIRSTONLY_IS_FALSE = _T("bFirstOnly == FALSE �̃R�[�h�͎������Ă��܂���");
	LPCTSTR NOT_IMPLEMENTED             = _T("������");
#else
	LPCTSTR WARN_DO_INITIALIZATION      = _T("Do initialization at CArcHelper::Init().");
	LPCTSTR INIT_WARN                   = _T("Warning");
	LPCTSTR MESSAGE_BFIRSTONLY_IS_FALSE = _T("It isn't implemented ( bFirstOnly == FALSE ).");
	LPCTSTR NOT_IMPLEMENTED             = _T("Not implemented.");

#endif // SINDY_FOR_ENGLISH
}

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CArcHelper::CArcHelper()
{
}

CArcHelper::~CArcHelper()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// �������Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
void CArcHelper::Init( IApplicationPtr ipApp )
{
	ATLASSERT( ipApp != NULL );

	((CComPtr<IApplication>&)*this) = (IApplication*)ipApp;

	GetEditor()->EnableUndoRedo( VARIANT_TRUE );	// UndoRedo ��L����
}

void CArcHelper::Init( IEditorPtr ipEditor )
{
	ATLASSERT( ipEditor != NULL );

	IApplicationPtr ipApp;
	ipEditor->get_Parent( &ipApp );
	if( ipApp )
		Init( ipApp );
}

void CArcHelper::Init( IDispatchPtr hook )
{
	ATLASSERT( hook != NULL );

	IApplicationPtr ipApp( hook );
	if( ipApp )
		Init( ipApp );
	else {
		IEditorPtr ipEditor( ipApp );
		if( ipEditor )
			Init( ipEditor );
		else
			_ASSERTE(false); // �T�|�[�g���Ă��Ȃ�
	}
}
/////////////////////////////////////////////////////////////////////////////
//
// ���������@�̔���p�֐�
//
/////////////////////////////////////////////////////////////////////////////
BOOL CArcHelper::CheckInit() const
{
	if( ! p )
	{
		MessageBox( GetHWND(),	WARN_DO_INITIALIZATION, INIT_WARN, MB_OK );

		return FALSE;
	}
	return TRUE;
}

/// IEditor ���g�p�\���ǂ����̔���p
BOOL CArcHelper::CheckEditor() const
{
	if( ! GetEditor() )
	{
		_ASSERTE(false);

		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//
// �I�u�W�F�N�g�擾�֐�
//
/////////////////////////////////////////////////////////////////////////////
IScreenDisplayPtr CArcHelper::GetScreenDisplay( BOOL bMapDisplay/* = TRUE*/ ) const
{
	IActiveViewPtr ipView( GetActiveView(bMapDisplay) );
	IScreenDisplayPtr ipDisp;
	// [Bug 6141]�e���v���[�g�I���Ńe���v���[�g��ؑւ���Ɨ�����o�O�Ή�
	if( ipView )
		ipView->get_ScreenDisplay( &ipDisp );

	return ipDisp;
}
IGraphicsContainerPtr CArcHelper::GetGraphicsContainer( BOOL bMapDisplay/* = TRUE*/ ) const
{
	IGraphicsContainerPtr ipContainer;
	IActiveViewPtr ipActiveView( GetActiveView( bMapDisplay ) );
	// [Bug 6141]�e���v���[�g�I���Ńe���v���[�g��ؑւ���Ɨ�����o�O�Ή�
	if( ipActiveView )
		ipActiveView->get_GraphicsContainer( &ipContainer );

	return ipContainer;
}

// TOC���猻�ݑI������Ă��郌�C�����擾����
ILayerPtr CArcHelper::GetSelectedLayer( bool bIsEditable /* = true */) const
{
	ILayerPtr ipLayer;
	AheGetSelectedLayer( GetDocument(), &ipLayer, bIsEditable );
	return ipLayer;
}

/////////////////////////////////////////////////////////////////////////////
//
// ���̑��̊֐�
//
/////////////////////////////////////////////////////////////////////////////

// �Z���^�����O�Ɗg�嗦�ύX
void CArcHelper::SetCenter( IPointPtr piPoint, DOUBLE dScale/*  = -1.0 */, BOOL bMapDisplay/* = TRUE */ ) const
{
	IActiveViewPtr ipActiveView = GetActiveView( bMapDisplay );
	IEnvelopePtr ipEnv = NULL;

	if ( ipActiveView != NULL ) {
		ipActiveView->get_Extent( &ipEnv );
		ipEnv->CenterAt( piPoint );
		ipActiveView->put_Extent( ipEnv );

		if( dScale > 0.0 )
			GetMap()->put_MapScale( dScale );

		// �ĕ`��
		ipActiveView->Refresh();
		GetScreenDisplay()->UpdateWindow();
	}
}

void CArcHelper::RefreshContentsView() const
{
	// TOC�ĕ`��
	IContentsViewPtr ipContentsView;
	IMxDocumentPtr ipMxDoc( GetDocument() );
	LONG lContentsCount;
	CComVariant va;
	va.vt = VT_NULL;

	if( ipMxDoc != NULL ) 
	{
		ipMxDoc->get_ContentsViewCount( &lContentsCount );
		for( LONG i = 0; i < lContentsCount; i++ )
		{
			ipMxDoc->get_ContentsView( i, &ipContentsView );
			if( ipContentsView != NULL ) ipContentsView->Refresh( va );
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
//
// �X�i�b�v�Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
// �X�i�b�v�|�C���g���擾����
BOOL CArcHelper::Snap( IPointPtr piPoint ) const
{
	VARIANT_BOOL vaIsSnapped = VARIANT_FALSE;

	/// �X�i�b�v���擾
	ISnapEnvironmentPtr ipSnapEnv( GetEditor() );

	if ( ipSnapEnv != NULL )
		ipSnapEnv->SnapPoint( piPoint, &vaIsSnapped );

	if( vaIsSnapped == VARIANT_TRUE )
		return TRUE;

	return FALSE;
}
// �X�i�b�v�|�C���g���擾����
BOOL CArcHelper::Snap( IPointPtr piPoint, IPoint** ipSnappedPoint, BOOL bFirstOnly ) const
{
	IPointCollectionPtr ipSnappedPoints = NULL;
	LONG lSnappedPointCount = 0;
	sindyeSnappingType type;

	///  bFirstOnly == FALSE �̃R�[�h������������O���Ă�������
	if( bFirstOnly == FALSE ) 
	{
		::MessageBox( ::GetActiveWindow(),
			MESSAGE_BFIRSTONLY_IS_FALSE,
			NOT_IMPLEMENTED, MB_OK );
		return FALSE;
	}

	if( Snap( piPoint, &ipSnappedPoints, &type ) == TRUE ) {
	
		ipSnappedPoints->get_PointCount( &lSnappedPointCount );

		for( LONG i = 0; i < lSnappedPointCount; i++ ) {
			IPointPtr ipSnappedPointTmp = NULL;
			ipSnappedPoints->get_Point( i, &ipSnappedPointTmp );
			if( i == 0 && bFirstOnly == TRUE ) {
				(*ipSnappedPoint) = ipSnappedPointTmp;
				(*ipSnappedPoint)->AddRef();
			}
		}

		return TRUE;
	} else 
		return FALSE;
}
// �X�i�b�v�|�C���g���擾����
BOOL CArcHelper::Snap(IPointPtr					piPoint, 
						  IPointCollection**		ipSnappedPoints,
						  sindyeSnappingType*		SnapType,
						  BOOL						bCheckClass,
						  BOOL						bCheckHitType,
						  esriGeometryHitPartType*	HitType) const
{
	DOUBLE		dSnapTolerance = 0.0;
	BOOL		bSnapped = FALSE;

	/// �R���N�V�����쐬
	(*ipSnappedPoints) = NULL;
	::CoCreateInstance( CLSID_Multipoint, NULL, CLSCTX_ALL, IID_IPointCollection, (void **)&(*ipSnappedPoints) );
	// �X�i�b�v�|�C���g�͈��͌X�̃t�B�[�`���N���X�̋�ԎQ�Ƃ�
	// �t�^���ׂ������AMultipoint�ɂ܂Ƃ߂�ƂȂ�ƃf�[�^�t���[���̋�ԎQ��
	// �����Z�b�g����]�n���Ȃ��c
	ISpatialReferencePtr ipMapSpRef;
	GetMap()->get_SpatialReference( &ipMapSpRef );
	((IGeometryPtr)(*ipSnappedPoints))->putref_SpatialReference( ipMapSpRef );

	/// ArcMap �x�[�X�̏ꍇ
	if( CheckApp() || CheckEditor() ) {

		LONG					lSnapCount = 0;
		esriSnapToleranceUnits	type;

		/// �X�i�b�v���擾
		ISnapEnvironmentPtr ipSnapEnv( GetEditor() );

		if ( ipSnapEnv == NULL )
			return FALSE;

		/// �X�i�b�v�G�[�W�F���g�̐�
		ipSnapEnv->get_SnapAgentCount( &lSnapCount );
		/// �X�i�b�v����
		ipSnapEnv->get_SnapTolerance( &dSnapTolerance );
		ipSnapEnv->get_SnapToleranceUnits( &type );
		if( type == esriSnapTolerancePixels ) {
			DOUBLE d = AheConvertPixelsToMapUnits( GetScreenDisplay(), dSnapTolerance );
			dSnapTolerance = d;
		}
//		/// �X�i�b�v��`�쐬
//		IGeometryPtr ipGeom( GetEnvelope( piPoint, dSnapTolerance ) );

		for( LONG i = 0; i < lSnapCount; i++ ) {
			ISnapAgentPtr ipSnapAgent;
			VARIANT_BOOL vaIsSnapped = VARIANT_FALSE;
			
			/// �|�C���g�R�s�[
			IClonePtr ipClone( piPoint ), ipClone2 = NULL;
			ipClone->Clone( &ipClone2 );
			IPointPtr ipSnappedPoint( ipClone2 );

			ipSnapEnv->get_SnapAgent( i, &ipSnapAgent );
			ipSnapAgent->Snap( NULL, ipSnappedPoint, dSnapTolerance, &vaIsSnapped );
			/// �X�i�b�v����
			if( vaIsSnapped == VARIANT_TRUE ) {

				bSnapped = TRUE;

				ATLASSERT( SnapType != NULL );

#ifdef USE_ROADINTERSECTIONSNAPAGENT
				/// �X�i�b�v�����t�B�[�`������
				IRoadIntersectionSnapAgentPtr ipIntersectSnapAgent( ipSnapAgent );
				if( ipIntersectSnapAgent != NULL ) 
					(*SnapType) = sindyeSnapRoadIntersection;
				else
#endif // ifdef IRoadIntersectionSnapAgentPtr
					if( SnapType )
						(*SnapType) = sindyeSnapAny;
				
				/// �t�B�[�`���N���X�𒲂ׂ�ꍇ
				if( bCheckClass == TRUE ) {					
					IFeatureSnapAgentPtr ipFeatureSnapAgent( ipSnapAgent );
					IFeatureClassPtr ipFeatureClass = NULL;

					/// �ڍׂȃX�i�b�v�t�B�[�`���̓���
					if( ipFeatureSnapAgent != NULL )
					{
						ipFeatureSnapAgent->get_FeatureClass( &ipFeatureClass );
						if( SnapType )
						{
							if( AheIsRoadClass( ipFeatureClass ) )
								(*SnapType) = sindyeSnapRoad;
							else if( AheIsNodeClass( ipFeatureClass ) )
								(*SnapType) = sindyeSnapNode;
						}

						/// �q�b�g�^�C�v�̓���
						if( bCheckHitType == TRUE ) {

							ATLASSERT( HitType != NULL );

							ipFeatureSnapAgent->get_HitType( HitType );
						}
					}
				}
						
			} 
		}
	}

	return bSnapped;
}
/////////////////////////////////////////////////////////////////////////////
//
// �C�x���g�ڑ��Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
// IActiveView �ɐڑ�
DWORD CArcHelper::ConnectToActiveView( IUnknownPtr ipUnk, DWORD& dwCookie ) const
{
  if (GetActiveView() == NULL)
    return (DWORD)0;

	//Avoid multiple connects
	if (dwCookie)
		return dwCookie;

	//Connect to active view events
	HRESULT hr = AtlAdvise(GetActiveView(), ipUnk, IID_IActiveViewEvents, &dwCookie);
	if (FAILED(hr)) {
		::MessageBox(::GetActiveWindow(),
			_T("Could not connect to ActiveView events"),
			_T("Command Implementing Events"), MB_OK);

		return (DWORD)0;
	}
	return dwCookie;
}

// IActiveView �̐ڑ�����
DWORD CArcHelper::DisconnectFromActiveView( DWORD& dwCookie ) const
{
	if ( dwCookie == 0 || GetActiveView() == NULL )
		return dwCookie;

	//Disconnect from ActiveView events
	HRESULT hr = AtlUnadvise(GetActiveView(), IID_IActiveViewEvents, dwCookie);
	if (FAILED(hr)) {
		::MessageBox(::GetActiveWindow(),
			_T("Could not disconnect from ActiveView events"),
			_T("Command Implementing Events"), MB_OK);

		return dwCookie;
	}
	dwCookie = (DWORD)0;

	return dwCookie;
}
