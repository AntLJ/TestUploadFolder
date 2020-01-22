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

/*! \file FeatureTree.h
        \brief CFeatureTree �N���X�̃C���^�[�t�F�C�X
        \author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
        \date 2002-02-01
*/
#if !defined(AFX_FEATURETREE_H__5A8AAB20_068E_4C6C_B8E8_F7EFD0B9370A__INCLUDED_)
#define AFX_FEATURETREE_H__5A8AAB20_068E_4C6C_B8E8_F7EFD0B9370A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FeatureDef.h"
#include "LQRowDef.h"

const unsigned int WM_TREESELECTCHANGE = RegisterWindowMessage(_T("WM_TREESELECTCHANGE"));      //!< �c���[��őI����Ԃ��ω��������ɔ������郁�b�Z�[�W
const unsigned int WM_REFRESHFEATURES  = RegisterWindowMessage(_T("WM_REFRESHFEATURES"));       //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W
const unsigned int WM_CHANGECURRENTFEATURE = RegisterWindowMessage(_T("WM_CHANGECURRENTFEATURE"));	//!< �I������Ă���t�B�[�`����ύX�˗����󂯂邽�߂̃��b�Z�[�W

/*!     \class CFeatureTree
        \brief �I�����ꂽ�t�B�[�`�������C���A�t�B�[�`���A�O���e�[�u���A�\���_�Ńc���[��ɕ\������N���X

        ����������ꍇ�͈ȉ��̃��\�b�h���Ă�ŉ������B

        Init()
        SetFeatures()

        �I�����ꂽ�t�B�[�`����\���_�̋����\�����ĕ`�悷��ɂ́A WM_REFRESHFEATURES ���|�X�g���Ă��������B

        \b Example:

        ::SendMessage( m_pTreeCtrl, WM_REFRESHFEATURES, NULL, NULL );

        �I����Ԃ��ς�����ꍇ�ɂ� WM_TREESELECTCHANGE ���b�Z�[�W���o���܂��B
*/
class CFeatureTree : 
        public CWindowImpl<CFeatureTree, CTreeViewCtrlEx, CControlWinTraits>
{
public:
    DECLARE_WND_SUPERCLASS( _T("CFeatureTree"), CTreeViewCtrl::GetWndClassName() )
    CFeatureTree();
    virtual ~CFeatureTree();

    /**
     * �c���[�ɓo�^����Ă���t�B�[�`���̎��
     * @see ItemKind()
     */
    enum ITEMKINDS {
		NOKINDS,
		LAYERITEM,
		FEATUREITEM,
		OUTERTABLEITEM,
		OUTERROWITEM,
		FEATUREPOINTITEM
    };

/////////////////////////////////////////////////////////////////////////////
//
// CFeatureTree ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CFeatureTree)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown )
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown )
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDoubleClick )
    MESSAGE_HANDLER(WM_REFRESHFEATURES, OnRefresh)
	MESSAGE_HANDLER(WM_CHANGECURRENTFEATURE, OnSetCurrentFeature )
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// �c���[�r���[�C�x���g�n���h��
//
/////////////////////////////////////////////////////////////////////////////
    LRESULT OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT OnRButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT OnLButtonDoubleClick( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnSetCurrentFeature( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
/////////////////////////////////////////////////////////////////////////////
//
// �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	/**
	 * @brief ArcObjects �w���p�[�N���X���Z�b�g����
	 *
	 * @param pHelper ArcObjects �w���p�[�N���X�̃|�C���^
	 */
    void Init( IApplication* ipApp );

	/**
	 * @brief �t�B�[�`����`�N���X�̃��X�g���c���[�ɃZ�b�g����
	 *
	 * @param pFeatureDefList       [in]        �c���[�ɃZ�b�g����t�B�[�`����`�N���X
	 *
	 * @note ������ĂԂƂ��̕��ǉ�����A���̏ꍇ�̓���͕ۏ؂��܂���B
	 */
    void SetFeatures( std::list<CFeatureDef>* pFeatureDefList );

	/**
	 * @brief �����N���`�N���X�̃��X�g���c���[�ɃZ�b�g����
	 *
	 * @param pFeatureDefList       [in]        �c���[�ɃZ�b�g����t�B�[�`����`�N���X
	 *
	 * @note ������ĂԂƂ��̕��ǉ�����A���̏ꍇ�̓���͕ۏ؂��܂���B
	 */
	void SetLQs( std::list<CLQRowDef>* pFeatureDefList );

	/**
	 * @brief ���ݑI������Ă���A�C�e���� _ISet �ɓ���ĕԂ�
	 *
	 * @param ipSet [out]        _ISet
	 *
	 * @note �R���e�i���ɓ���̂� IFeature ���� IRow �ɂȂ�܂��B
	 */
    void GetSelectedFeatures( _ISet** ipSet );

	/**
	 * @brief ���ݑI������Ă���A�C�e������C���f�b�N�X�ԍ����擾����
	 *
	 * @param lFeatureIndex	[out]        �t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
	 * @param lTableIndex	[out]        �e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
	 * @param lRowIndex		[out]        ���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
	 *
	 * @note �I������Ă���A�C�e�����t�B�[�`���̏ꍇ�́AlTableIndex �� lRowIndex �͂��ꂼ�� -1 �ɂȂ�܂��B
	 */
	void GetSelectedFeatures( long& lFeatureIndex, long& lTableIndex, long& lRowIndex );

	/**
	 * @brief �c���[�R���g���[����̃A�C�e���̎�ނ𔻒肷��
	 *
	 * @param item  [in]        ���肵�����A�C�e��
	 *
	 * @return ���茋�ʁi@see ITEMKINDS�j
	 */
    INT ItemKind( HTREEITEM item );

	/**
	 * @brief HTREEITEM �\���̂��� CRowDef ����肵�ĕԂ�
	 *
	 * @param item          [in]        �c���[��̃A�C�e��
	 *
	 * @return ���݂��Ȃ���� NULL�A����΂��̃|�C���^��Ԃ�
	 */
    CRowDef* GetRowDef( HTREEITEM item );

	/**
	 * @brief HTREEITEM �\���̂��� CLQRowDef ����肵�ĕԂ�
	 *
	 * @param item		[in]	�c���[��̃A�C�e��
	 *
	 * @return ���݂��Ȃ���� NULL�A����΂��̃|�C���^��Ԃ�
	 */
	CLQRowDef* GetLQRowDef( HTREEITEM item );

	/**
	 * @brief HTREEITEM �\���̂��� CFeatureDef ����肵�ĕԂ�
	 *
	 * @param item          [in]        �c���[��̃A�C�e��
	 *
	 * @return ���݂��Ȃ���� NULL�A����΂��̃|�C���^��Ԃ�
	 */
	CFeatureDef* GetFeatureDef( HTREEITEM item );

	/**
	 * @brief �n�C���C�g�p�̃R���N�V�����Ȃǂ�����
	 *
	 * @note �_�C�A���O�̏I�����ɌĂ�ł�������
	 */
    void Clear();
private:
	/**
	 * @brief �I������Ă���t�B�[�`�����n�C���C�g����
	 *
	 * @param bRefresh      [in]	�P���ȍĕ`��̏ꍇ�� FALSE�i�f�t�H���g�j�A�n�C���C�g����t�B�[�`�����ς��Ƃ��� TRUE ��n��
	 */
    void HilitSelectedFeatures( BOOL bRefresh = FALSE );

	/**
	 * @brief �I������Ă��郊���N����n�C���C�g����
	 *
	 * @param ipGeom		[in]	�����N��̌`���n��	
	 * @param bRefresh      [in]	�P���ȍĕ`��̏ꍇ�� FALSE�i�f�t�H���g�j�A�n�C���C�g����t�B�[�`�����ς��Ƃ��� TRUE ��n��
	 */
	void HilitSelectedLQ( IGeometryPtr ipGeom,  BOOL bRefresh = FALSE );

	/**
	 * @brief �I�������t�B�[�`����TOC�ɕ\������Ă��郌�C�������擾����
	 *
	 * @param ipFeature		[in]	�t�B�[�`��
	 * @param bstrLayerName	[out]	TOC�ŕ\������Ă��郌�C����
	 */
	void GetLayerName( IFeaturePtr ipFeature, CComBSTR& bstrLayerName);

	/**
	 * @brief TOC�őI�����Ă��郌�C�������擾����
	 *
	 * @note �����̃��C����I�����Ă���ꍇ�͉����Ԃ��Ȃ��B�܂�IGeoFeatureLayer�̂Ƃ��̂ݒl��Ԃ�
	 *
	 * @param ipFeature		[in]	�����\���������t�B�[�`��
	 * @param bstrLayerName	[out]	TOC�ŕ\������Ă��郌�C����
	 */
	void GetSelectedLayerName( IFeaturePtr ipFeature, CComBSTR& bstrLayerName);

	/**
	 * @brief �n���ꂽ���C�����ƃc���[�̃e�L�X�g���������ǂ����̔���
	 *
	 * @param bstrLayerName		[in]	���C����
	 * @param item				[in]	���肷��A�C�e��
	 *
	 * @return �����Ȃ� TRUE�A�Ⴄ�Ȃ� FALSE
	 */
	BOOL IsSameText( CComBSTR bstrLayerName, HTREEITEM item );

	/**
	 * @brief �������C���������A�C�e����T��
	 *
	 * @param bstrLayerName		[in]	�T�����C����
	 *
	 * @return ����ΊY�����郌�C���̃A�C�e����Ԃ��B�Ȃ���� NULL ��Ԃ�
	 */
	HTREEITEM SearchLayerName(  CComBSTR lpcszLayerName  );

	/**
	 * @brief ���C������OBJECTID����A�C�e����T��
	 *
	 * @note �����Ō������C�����Ƃ̓c���[��ŕ\������Ă��郌�C�����ł�
	 *
	 * @param lpcszLayerName	[in]	���C����
	 * @param lOID				[in]	OBJECTID
	 *
	 * @return ����ΊY�����郌�C���̃A�C�e����Ԃ��B�Ȃ���� NULL ��Ԃ�
	 */
	HTREEITEM SearchOID( LPCTSTR lpcszLayerName, LONG lOID );

	/**
	 * @brief �t�B�[�`�����c���[�ɓo�^����
	 *
	 * ���C��			0x8000000
	 * �t�B�[�`��		0x4000000 + �t�B�[�`���J�E���g
	 * �e�[�u��			0x2000000 + �e�[�u���J�E���g
	 * ���R�[�h			0x1000000 + ���R�[�h�J�E���g
	 * �\���_���x��		0x800000  + �t�B�[�`���J�E���g
	 * �\���_			0x400000  + �\���_�J�E���g
	 *
	 * @param pFeatureDef	[in]	�o�^����t�B�[�`����`�N���X�̃��X�g
	 */
	void Add( std::list<CFeatureDef>* pFeatureDef );

	/**
	 * @brief �����N����c���[�ɓo�^����
	 *
	 * ���C��		0x8000000
	 * �t�B�[�`��	0x4000000 + �t�B�[�`���J�E���g
	 * �e�[�u��		0x2000000 + �e�[�u���J�E���g
	 * ���R�[�h		0x1000000 + ���R�[�h�J�E���g
	 * �\���_���x��	0x800000  + �t�B�[�`���J�E���g
	 * �\���_		0x400000  + �\���_�J�E���g
	 *
	 * @param pFeatureDef	[in]	�o�^����t�B�[�`����`�N���X�̃��X�g
	 */
	void AddLQ( std::list<CLQRowDef>* pFeatureDef );

	/**
	 * @brief �A�C�e�����c���[�ɓo�^����
	 *
	 * @param bIsRoot			[in]	�e�A�C�e���i���C���j�̏ꍇ�� TRUE�A����ȊO�� FALSE
	 * @param lpItem			[in]	�e�ɂȂ�A�C�e��
	 * @param lptstrNodeName	[in]	�ǉ�����A�C�e���̃e�L�X�g
	 *
	 * @return �ǉ������A�C�e��
	 */
    HTREEITEM AddItem( bool bIsRoot, HTREEITEM lpItem, LPCTSTR lptstrNodeName );

	/**
	 * @brief �O���e�[�u�����c���[�ɓo�^����
	 *
	 * @param item				[in]	�e�ɓ�����t�B�[�`���̃c���[�A�C�e��
	 * @param pRelTableDef		[in]	�o�^����O���e�[�u����`�N���X�̃|�C���^
	 * @param lTableIndex		[in]	pRelTableDef �̃C���f�b�N�X�ԍ�
	 */
	void AddOuterTable( HTREEITEM& item, CRelationalTableDef* pRelTableDef, LONG lTableIndex );

	/**
	 * @brief �}�E�X�{�^���̃N���b�N����������֐�
	 *
	 * @param uMsg          [in]        UINT
	 * @param wParam        [in]        WPARAM
	 * @param lParam        [in]        LPARAM
	 * @param bHandled      [out]        BOOL
	 * @param bDlbClick     [in]        �_�u���N���b�N�̃C�x���g�̏ꍇ�� TRUE
	 *
	 * @return LONG
	 */
	LONG OnMouseDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL bDblClick );

	/**
	 * @brief �����N���p�}�E�X�{�^���̃N���b�N����������
	 *
	 * @param uMsg		[in]	UINT
	 * @param wParam	[in]	WPARAM
	 * @param lParam	[in]	LPARAM
	 * @param bHandled	[out]	BOOL
	 * @param bDlbClick	[in]	�_�u���N���b�N�̃C�x���g�̏ꍇ�� TRUE
	 *
	 * @return LONG
	 */
	LONG OnLQMouseDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL bDblClick );

	/**
	 * @brief �c���[��ŉE�N���b�N���������Ƃ��ɃR�s�[�̃��j���[���o���܂�
	 *
	 * @param uMsg          [in]        UINT
	 * @param wParam        [in]        WPARAM �c���[�̎w��ԍ�(�ォ��)
	 * @param lParam        [in]        LPARAM
	 * @param bHandled      [out]       BOOL
	 *
	 * @return BOOL
	 */
	BOOL ShowMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief �c���[��ŉE�N���b�N���������Ƃ��ɃR�s�[�̃��j���[���o���܂�
	 *
	 * @param bstrTree	[in]	CComBSTR �I������OID
	 */
    void  WriteClipBoard(CComBSTR bstrTree);

	/**
	* @brief �|�C���g��XYZ���W���𕶎���Ŏ擾����
	* @param point XYZ���擾�������|�C���g
	* @return XYZ���W�̕����� (X=* Y=* Z=*)
	*/
	CString getXYZAsString(IPointPtr point) const;

/////////////////////////////////////////////////////////////////////////////
//
// �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	BOOL m_bIsLineHilight;			//!< ���C�����n�C���C�g���邩�̃t���O
	BOOL m_bIsVertexHilight;		//!< �\���_���n�C���C�g���邩�̃t���O
private:
    CArcHelper m_cArcHelper;					//!< �w���p�[�N���X
    std::list<CFeatureDef>* m_pFeatureDefList;	//!< �t�B�[�`���ۑ��p
	std::list<CLQRowDef>*	m_pLQDefList;		//!< �t�B�[�`���ۑ��p

    _ISetPtr		m_arrayHilitFeatures;	//!< �n�C���C�g����t�B�[�`���ۑ��p
    IPointPtr	m_ipHilitPoint;			//!< �n�C���C�g����|�C���g�ۑ��p
	IGeometryPtr	m_ipHilightGeometry;//!< �n�C���C�g����`��
	bool		m_bIsLQAttribute;		//!< �����N���\�������ǂ���				
    bool		m_bIsRButtonClick;		//!< �E�N���b�N�������ꂽ���ǂ����̃t���O		
};

#endif // !defined(AFX_FEATURETREE_H__5A8AAB20_068E_4C6C_B8E8_F7EFD0B9370A__INCLUDED_)

