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

// AttrHeightNode.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrHeightNode.h"

using namespace sindy::schema;

BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		RCSPACE(20)
		BEGINROWS(WRCT_REST,0,0)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_COMBO_LAYER1)
				RCTOFIT(IDC_EDIT_ID1)
				RCREST(IDC_EDIT_WALKCLASS1)
				RCSPACE(20)
				RCTOFIT(IDC_COMBO_HEIGHT1)
				RCSPACE(20)
			ENDGROUP()
			RCSPACE(5)
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_COMBO_LAYER2)
				RCTOFIT(IDC_EDIT_ID2)
				RCREST(IDC_EDIT_WALKCLASS2)
				RCSPACE(20)
				RCTOFIT(IDC_COMBO_HEIGHT2)
				RCSPACE(20)
			ENDGROUP()
			RCSPACE(5)
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
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrHeightNode��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrHeightNode();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHeightNode �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrHeightNode::CAttrHeightNode()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrHeightNode::CAttrHeightNode() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_HEIGHT_NODE_DLG),
	m_focusedHeight( linkNo::unkown ),
	m_forceCancel( false )
{
	// �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_LAYER1, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_LAYER1 ) )
		( IDC_EDIT_ID1, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ID1 ) )
		( IDC_COMBO_HEIGHT1, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_HEIGHT1 ) )
		( IDC_COMBO_LAYER2, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_LAYER2 ) )
		( IDC_EDIT_ID2, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ID2 ) )
		( IDC_COMBO_HEIGHT2, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_HEIGHT2 ) )
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}

/** 
 * @fn HWND CAttrHeightNode::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrHeightNode::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrHeightNode::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrHeightNode::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

// �uOK�v�{�^������
void CAttrHeightNode::SetButton( BOOL changed )
{
	// CAttrBaseDlg::SetButton( changed ) �ł́uOK�v���ׂ��Ȃ��P�[�X������B
	// �e�_�C�A���O�iAttributeDlg�j����put_EditType()�ŐV�K�쐬���Ȃǂɋ����I��OK��Enable�ɂ��郂�[�h�ɐݒ肳���悤���B
	//
	// �����ł́A�s���ȏꏊ�ɍ쐬�����̂�h�����߁A����͎ז��ȊO�̉����ł��Ȃ����A�q�_�C�A���O������
	// ����������@��������Ȃ��̂ŁA�����ɐe�̃R���g���[�����E���Ă��Ē��ڑ��삷��B�B
	if( m_forceCancel )
	{
		CWindow(::GetDlgItem( m_hAttributeDlgWnd, IDOK )).EnableWindow( FALSE );
		return;
	}

	ATTR_BASE_CLASS::SetButton( changed );
}

// �R���g���[���w�i�F�ύX
LRESULT CAttrHeightNode::OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �_���`�F�b�N�ŃG���[�ƂȂ�����Ԃł́uOK�v�������������Ȃ��̂����ASketchTool����Ă΂ꂽ�ۂɂ�
	// �q�_�C�A���O�̏������̌�ɍX�ɋ����I��Enable�ɂ���Ă��܂��B
	// ���̌�A�����炩��X�ɖ����ɂ���^�C�~���O�i�C�x���g�j�����ɂȂ��̂ŁA���傤���Ȃ������ɌĂ΂�邱���ł��B�B
	// ��������Ȃ��ƁASketchTool�ō쐬���ĊJ��������ɁuOK�v�ł��Ă��܂�
	SetButton( ATTR_BASE_CLASS::Changed() );
	return ATTR_BASE_CLASS::OnCtlColorEdit( uMsg, wParam, lParam, bHandled );
}

void CAttrHeightNode::EnableControls( bool editing )
{
	// ��ɕҏW�s�̃R���g���[��
	const static std::vector<UINT> ctrls = boost::assign::list_of
		(IDC_COMBO_LAYER1)
		(IDC_EDIT_ID1)
		(IDC_EDIT_WALKCLASS1)
		(IDC_COMBO_LAYER2)
		(IDC_EDIT_ID2)
		(IDC_EDIT_WALKCLASS2);

	for( const auto& id : ctrls )
	{
		// �G�f�B�b�g�{�b�N�X�Ȃ�ReadOnly
		CEdit edit( GetDlgItem(id) );
		edit.SetReadOnly( TRUE );

		// �R���{�Ȃ�Disable...�߂�ǂ����[��
		if ( !(edit.GetStyle() & ES_READONLY) )
			edit.EnableWindow( FALSE );
	}
	
	// �ҏW�J�n�ŕҏW�\�ȏꍇ�̂�
	const static std::vector<UINT> heights = boost::assign::list_of
		(IDC_COMBO_HEIGHT1)
		(IDC_COMBO_HEIGHT2)
		(IDC_EDIT_SOURCE);

	for( const auto& id : heights )
	{
		GetDlgItem( id ).EnableWindow( bool2BOOL( editing ) );
	}

}

// ���s�Ҏ�ʔ��f
void CAttrHeightNode::SetWalkclassToCtrl()
{
	if( !m_relativeLinks.IsExist() )
		return;

	static const std::map<linkNo::eCode, UINT> ctrlIDs = boost::assign::map_list_of
		( linkNo::link1, IDC_EDIT_WALKCLASS1 )
		( linkNo::link2, IDC_EDIT_WALKCLASS2 );

	for( const auto& id : ctrlIDs )
	{
		GetDlgItem( id.second ).SetWindowText( 
			m_relativeLinks[ id.first ].m_walkclass );
	}
}

// �R���{�{�b�N�̃X���X�g�쐬
void CAttrHeightNode::CreateHeightCombo( UINT id, long height )
{
	// 2 �` -2 �̘A�ԍ쐬
	// TODO: �����Ƃ��Ȃ烌���W�h���C������ׂ�
	std::vector<long> vals(5);
	std::iota( vals.rbegin(), vals.rend(), -2 );

	CComboBox cb( GetDlgItem(id) );
	cb.ResetContent();
	for( const long val : vals )
	{
		int idx = cb.AddString( uh::str_util::ToString( val ) );

		// �I�����ׂ�������������I�����Ă���
		if( val == height )
			cb.SetCurSel( idx );

		// ���X�g�̃A�C�e���ɍ�����R�t���Ă���
		cb.SetItemData( idx, val );
	}

}

// �����̊Ǘ����鍂�������e�R���g���[���ɔ��f����
void CAttrHeightNode::ApplyToCtrl()
{
	// �R���{�{�b�N�X���X�g�̃A�C�e����I������
	auto selectComboItem = []( HWND hwnd, const CString& item )
	{
		CComboBox cb( hwnd );
		int num = cb.GetCount();
		for( int i=0; i<num; ++i )
		{
			CComBSTR bstr;
			cb.GetLBTextBSTR( i, (BSTR&)bstr );
			if( item == bstr )
			{
				cb.SetCurSel( i );
				return;
			}
		}
	};

	// �^�v���͂���܂�g�������Ȃ����A���Ǐ��I�Ȃ̂ŃN���X�����͂������ȁB�B�B
	typedef boost::tuple< linkNo::eCode, UINT, CString> linkno_ctrl_field;
	static const std::vector< linkno_ctrl_field > tupleFields = boost::assign::tuple_list_of
		( linkNo::link1, IDC_COMBO_LAYER1, height_node::kLinkID1Table  )
		( linkNo::link2, IDC_COMBO_LAYER2, height_node::kLinkID2Table  )
		( linkNo::link1, IDC_EDIT_ID1, height_node::kLinkID1  )
		( linkNo::link2, IDC_EDIT_ID2, height_node::kLinkID2 );

	// �R���g���[���ɒl��������
	for( auto& fields : tupleFields )
	{
		// �^�v����W�J
		linkNo::eCode linkNo = linkNo::unkown;
		UINT ctrl = 0;
		CString field;
		boost::tie( linkNo, ctrl, field ) = fields;

		// �Ώۂ�ControlDef��T��
		auto it = boost::find_if( m_cControlRel, [&field](CControlDef& def){
			return (field.CompareNoCase( def.GetFieldName() ) == 0);
		});

		if( it == m_cControlRel.end() )
		{
			_ASSERT( false );
			continue;
		}
		
		// �_�C�A���O�ɒl���Z�b�g
		const auto& linkInfo = m_relativeLinks[linkNo];
		switch (it->m_type)
		{
		case TYPE_EDIT:
			GetDlgItem( ctrl ).SetWindowText( uh::str_util::ToString( linkInfo.m_id ) );
			break;
		case TYPE_COMBO:
			selectComboItem( GetDlgItem( ctrl ), linkInfo.m_layer );
			break;
		default:
			break;
		}

		// ControlDef�ɂ����f
		it->SetValueFromControl();
	}
	
	// �����͕ʂŁB�B
	SetWalkclassToCtrl();
}

/**
 * @fn BOOL CAttrHeightNode::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrHeightNode::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	bool editing = IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable;
	if( editing )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_HEIGHT1) );
	}

	// �����ȊO��Disable�ɂ��Ă���
	EnableControls( editing );

	// ���΍����R���{�{�b�N�X
	auto& getHeight = [&](const CString& field){
		return GetCurrentFeatureDef()->GetFieldDef( height_node::kTableName, field )->m_vaValue.lVal;
	};
	CreateHeightCombo( IDC_COMBO_HEIGHT1, getHeight( height_node::kLinkID1Height ) );
	CreateHeightCombo( IDC_COMBO_HEIGHT2, getHeight( height_node::kLinkID2Height ) );

	// �����N�̏����擾
	const auto& heightNode = GetCurrentFeatureDef()->GetFeature();
	CString err;
	if( !m_relativeLinks.Reset( heightNode, m_cArcHelper, err ) ||
		!m_relativeLinks.CheckLogic(err) )
	{
		if( editing )
		{
			// [�L�����Z��] ���������Ȃ�
			m_forceCancel = true;
			::MessageBox( nullptr, err, AheLoadString( IDS_DIALOG_NAME ), MB_OK | MB_ICONERROR );
			EnableControls(false);
			return FALSE;
		}
	}

	// �V�K���́iSketchTool�ō쐬���ꂽ�j��������R���g���[���ɒl�𔽉f
	if( m_relativeLinks.IsCreated() )
	{
		ApplyToCtrl();
	}
	
	// ���s�Ҏ�ʂ͕ʃe�[�u���̏��Ȃ̂Ŏ��O�ŁB�B
	SetWalkclassToCtrl();
	
	return bRet;
}

/**
 * @fn void CAttrHeightNode::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrHeightNode::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �����N�P�̃e�[�u��
		m_cControlRel.SetControl( IDC_COMBO_LAYER1, TYPE_COMBO, 0, 0, height_node::kLinkID1Table, 0, height_node::kTableName );
		// �����N�P��ID
		m_cControlRel.SetControl( IDC_EDIT_ID1, TYPE_EDIT, 0, 0, height_node::kLinkID1, 0, height_node::kTableName );
		// �����N�P�̍��� �����W�h���C�����E�}���Ȃ��̂�TYPE_OTHER�ɂ��Ă���
		m_cControlRel.SetControl( IDC_COMBO_HEIGHT1, TYPE_OTHER, 0, 0, height_node::kLinkID1Height, 0, height_node::kTableName );
		// �����N2�̃e�[�u��
		m_cControlRel.SetControl( IDC_COMBO_LAYER2, TYPE_COMBO, 0, 0, height_node::kLinkID2Table, 0, height_node::kTableName );
		// �����N2��ID
		m_cControlRel.SetControl( IDC_EDIT_ID2, TYPE_EDIT, 0, 0, height_node::kLinkID2, 0, height_node::kTableName );
		// �����N2�̍���
		m_cControlRel.SetControl( IDC_COMBO_HEIGHT2, TYPE_OTHER, 0, 0, height_node::kLinkID2Height, 0, height_node::kTableName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, height_node::kTableName );
	}
}

namespace highlite_color{
	const COLORREF flat = RGB(0,255,0);
	const COLORREF lower = RGB(0,0,255);
	const COLORREF heiger = RGB(255,0,0);
}

// �ĕ`��
LRESULT CAttrHeightNode::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	if( !m_relativeLinks.IsExist() )
		return 0;

	// �n�C���C�g�̃��C����`��
	auto highliteLink = [&]( const IFeaturePtr& link, COLORREF color, bool outline )
	{
		auto geom = AheGetShapeCopy( link );
		AheDrawPolyline(m_cArcHelper.GetDisplay(), geom, color, 5, esriSLSSolid, TRUE, FALSE);
		if( outline )
		{
			AheDrawFeatureXOR( m_cArcHelper.GetDisplay(), link, AheConvertPixelsToMapUnits( m_cArcHelper.GetDisplay(), 10 ), FALSE );
		}
	};

	for( const auto& link : m_relativeLinks )
	{
		// �t�H�[�J�X���������Ă�΃A�E�g���C���\��
		bool outline = (link.first == m_focusedHeight);

		if( !link.second.m_feature )
			continue;

		// ����ɂ���ăn�C���C�g�F�𕪂���
		COLORREF color = 0;
		switch( m_relativeLinks.GetRelativeHeight( link.first ) )
		{
		case height_info::flat:
			color = highlite_color::flat; break;
		case height_info::lower:
			color = highlite_color::lower; break;
		case height_info::higher:
			color = highlite_color::heiger; break;
		default:
			break;
		}

		highliteLink( link.second.m_feature, color, outline );
	}
	
	SetButton( ATTR_BASE_CLASS::Changed() );

	return 0;
}

// �������ύX���ꂽ
LRESULT CAttrHeightNode::OnHeightChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �R���{�{�b�N�X�ɂ���ăt�B�[���h���قȂ�
	CString field;
	linkNo::eCode linkNo = linkNo::unkown;
	switch (wID)
	{
	case IDC_COMBO_HEIGHT1:
		field = height_node::kLinkID1Height;
		linkNo = linkNo::link1;
		break;
	case IDC_COMBO_HEIGHT2:
		field = height_node::kLinkID2Height;
		linkNo = linkNo::link2;
		break;
	default:
		break;
	}
	
	// �R���{�{�b�N�X����V�����l���擾
	CComboBox cb(hWndCtl);
	long newVal = cb.GetItemData( cb.GetCurSel() );

	// �_�C�A���O�Ǘ��p�̒l���X�V
	GetCurrentFeatureDef()->GetFieldDef( height_node::kTableName, field )->SetNewValue( newVal, hWndCtl );

	// �����Ǘ��p�̒l���X�V
	m_relativeLinks.UpdateHeight( linkNo, newVal );

	// �n�C���C�g�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);

	SetButton( ATTR_BASE_CLASS::Changed() );

	return 0;
}

// �t�H�[�J�X�擾��
LRESULT CAttrHeightNode::OnHeightFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �A�E�g���C���n�C���C�g�t���O�𗧂ĂĂ���
	switch (wID)
	{
	case IDC_COMBO_HEIGHT1:
		m_focusedHeight = linkNo::link1; break;
	case IDC_COMBO_HEIGHT2:
		m_focusedHeight = linkNo::link2; break;
	default:
		m_focusedHeight = linkNo::unkown; break;
	}

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	return 0;
}

// �t�H�[�J�X���O���Ƃ�
LRESULT CAttrHeightNode::OnHeightKillFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_focusedHeight = linkNo::unkown;	
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	return 0;
}
