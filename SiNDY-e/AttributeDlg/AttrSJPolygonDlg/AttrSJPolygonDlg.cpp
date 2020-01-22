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

// AttrSJPolygonDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrSJPolygonDlg.h"
#include <boost/assign/list_of.hpp>

const double  EARTH_R  = 6370.0;

using namespace sindy::schema;

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
		BEGINROWS(WRCT_REST,0,0)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LANDCOVER)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LANDCOVER)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LANDUSE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LANDUSE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_ROADFLAG)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCREST(IDC_STATIC_AREA)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrSJPolygonDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrSJPolygonDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSJPolygonDlg::CAttrSJPolygonDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrSJPolygonDlg::CAttrSJPolygonDlg() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_SINDYCHILDDLG)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_COMBO_LANDCOVER, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_LANDCOVER ) )
		( IDC_COMBO_LANDUSE, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_LANDUSE ) )
		( IDC_CHECK_ROADFLAG, CContainedWindow(  _T("CHECK"), this, IDC_CHECK_ROADFLAG ) );
}

/** 
 * @fn HWND CAttrSJPolygonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrSJPolygonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrSJPolygonDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrSJPolygonDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrSJPolygonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * �܂��A���e�ԍ��̒l�����������āA���[������擾���Ă���
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrSJPolygonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ROAD_POLYGON�Ȃ�΁AROAD_F�̃`�F�b�N�{�b�N�X�\���A����ȊO�Ȃ�Δ�\��
	CWindow cRoadCheckBox( GetDlgItem( IDC_CHECK_ROADFLAG ) );
	if( m_strClassName == sj::kRoadTableName )
		cRoadCheckBox.ShowWindow( SW_SHOW );
	else
		cRoadCheckBox.ShowWindow( SW_HIDE );

	// RAILWAY_POLYGON�Ȃ�ΕK���R���{�{�b�N�XDISABLE
	if( m_strClassName == sj::kRailwayTableName )
	{
		SetDlgStatus( IDC_COMBO_LANDCOVER, false );
		SetDlgStatus( IDC_COMBO_LANDUSE, false );
	}

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// ROAD_POLYGON�Ȃ�LCCLS�ALUCLS�̃R���{�{�b�N�X�̐���
		if( m_strClassName == sj::kRoadTableName )
			SetRoadFlag( (bool)(GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sj::kRoadF )->m_vaValue.lVal), false );

		// �ŏ��Ƀt�H�[�J�X�����R���g���[��
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	// �ʐϕ\���iArcHelper������ɂ����Ă����������j
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if( ipFeature )
	{
		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );
		if( ipGeom )
		{
			double dArea = 0;
			((IAreaPtr)ipGeom)->get_Area(&dArea);
			IPointPtr ipPoint(CLSID_Point);
			((IAreaPtr)ipGeom)->QueryCentroid(ipPoint);
			ipPoint->putref_SpatialReference( AheGetSpatialReference( ipGeom ) );
			double dY = 0;
			ipPoint->get_Y(&dY);
			// ���݂̈ܓx�Ɛԓ��Ƃ̊��������߂�
			double dRatio = EARTH_R * sin(M_PI / 2.0 - dY * M_PI / 180.0) * 2.0 * M_PI/360.0;
			dArea *= fabs(dRatio * EARTH_R * 2.0 * M_PI / 360.0);
			dArea *= 1000000.0;
			CString strArea;
			strArea.Format( _T("AERA : %.2lfm2"), dArea );
			CStatic cStatic( GetDlgItem( IDC_STATIC_AREA ) );
			cStatic.SetWindowText(strArea );
		}
	}
    return bRet;
}

/**
 * @fn void CAttrSJPolygonDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrSJPolygonDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// LAND_COVER
		m_cControlRel.SetControl( IDC_COMBO_LANDCOVER, TYPE_COMBO, 0, 0, sj::kLCCLS, 0, m_strClassName );
		// LAND_USE
		m_cControlRel.SetControl( IDC_COMBO_LANDUSE, TYPE_COMBO, 0, 0, sj::kLUCLS, 0, m_strClassName );
		// ROAD_F
		if( m_strClassName == sj::kRoadTableName )
			m_cControlRel.SetControl( IDC_CHECK_ROADFLAG, TYPE_CHECKBOX, 0, 0, sj::kRoadF, 0, m_strClassName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, m_strClassName );
	}
}

// ���H�t���O�̃`�F�b�N�{�b�N�X�Ǘ��֐�
void CAttrSJPolygonDlg::SetRoadFlag( bool bCheck, bool bReset )
{
	// ROAD_F��1�̏ꍇ�ɂ́ALCCLS�ALUCLS�̕ύX�͂ł��Ȃ�
	// ROAD_F��0�̏ꍇ�ɂ́ALCCLS�ALUCLS�̕ύX���ł���
	SetDlgStatus( IDC_COMBO_LANDCOVER, !bCheck );
	SetDlgStatus( IDC_COMBO_LANDUSE, !bCheck );

	if( bReset )
	{
		// ���Z�b�g����ꍇ�ɂ́A�J�[�\��0�ɂ���i0�͏����l�j
		CComboBox cComboLC( GetDlgItem( IDC_COMBO_LANDCOVER ) ), cComboLU( GetDlgItem( IDC_COMBO_LANDUSE ) );
		cComboLC.SetCurSel( 0 );
		cComboLU.SetCurSel( 0 );
	}
}

// �G���[�`�F�b�N
BOOL CAttrSJPolygonDlg::ErrorCheck()
{
	// ROAD_POLYGON�ŁAROAD_F=1�ł���΂悵
	// ���m�ɂ́AROAD_F=1�̏ꍇ�ɂ͂��̎q�_�C�A���O�ŁALAND_COVER=0�ɖ�����肵�Ă���̂ŁA�G���[�ɂ͂Ȃ�Ȃ�
	if( m_strClassName == sj::kRoadTableName )
	{
		if( GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sj::kRoadF )->m_vaValue.lVal == 1 )
			return TRUE;
	}

	// LAND_COVER=0�̓G���[
	if( GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sj::kLCCLS )->m_vaValue.lVal == 0L )
	{
		long lOID = 0;
		GetCurrentFeatureDef()->GetFeature()->get_OID(&lOID);
		CString strMessage;
		strMessage.Format( AheLoadString( IDS_UNDEFINEDERROR ), m_strClassName, lOID );
		AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH);
		return FALSE;
	}

	return TRUE;
}
