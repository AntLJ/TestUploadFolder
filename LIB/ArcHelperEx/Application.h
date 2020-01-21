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
 * @file Application.h
 * @brief CApplication�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <atl2/seal.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @class CApplication
 * @brief <b>IApplication���b�v�N���X</b>\n
 * ���̃N���X��CArcHelper�ɑւ��A�v���P�[�V�������b�v�N���X�ł��B
 * ���̃N���X�ł́AIApplicaton�̑S�ẴC���^�[�t�F�[�X�����b�v���Ă��܂��B
 * �܂��A����ȊO�ɂ��L�p�Ǝv������̂ɂ��Ă̓��b�v���Ă���܂��B\n\n
 * ISiNDYApplication���g�p����ꍇ�́A���̃N���X�͎g�p���Ȃ��ŉ������B
 */
class CApplication : public atl2::seal<CComPtr<IApplication> >
{
public:
	CApplication() : m_bIsStartEditOperation( false )
	{
	}

	virtual ~CApplication()
	{
	}

	CApplication( IApplication* lp )
	{
		*this = lp;
	}

	CApplication& operator=( IApplication* lp )
	{
        if(*this!=lp)
    		AtlComPtrAssign((IUnknown**)&p, lp);

        return *this;
	}

	IApplication** Release()
	{
		CComPtr<IApplication>::Release();
		return &p;
	}

	//@{ @name IApplication�C���^�[�t�F�[�X
	IDocumentPtr GetDocument() const;
	IExtensionPtr FindExtensionByCLSID( IUID* ExtensionCLSID ) const;
	IExtensionPtr FindExtensionByName( LPCTSTR extensionName ) const;
	HWND GetHWND() const;
	bool IsDialogVisible() const;
	HRESULT LockCustomization( LPCTSTR Password, ICustomizationFilter* custFilter );
	HRESULT GetName( LPTSTR Name, int nLen ) const;
	HRESULT NewDocument( bool selectTemplate, LPCTSTR templatePath );
	HRESULT OpenDocument( LPCTSTR Path );
	HRESULT PrintDocument() const;
	HRESULT PrintPreview() const;
	HRESULT RefreshWindow() const;
	HRESULT SaveAsDocument( LPCTSTR saveAsPath, bool saveAsCopy ) const;
	HRESULT SaveDocument( LPCTSTR saveAsPath ) const;
	HRESULT ShowDialog( long dialogID, VARIANT& bShow, VARIANT& result ) const;
	IStatusBarPtr GetStatusBar() const;
	ITemplatesPtr GetTemplates() const;
	HRESULT UnlockCustomization( LPCTSTR Password );
	IDispatchPtr GetVBE() const;
	bool GetVisible() const;
	HRESULT PutVisible( bool Visible );
	//@}
	/**
	 * @brief <b>ProgID����Extension��T��</b>\n
	 * @param progID	[in]	ProgID�ie.x. "esriCore.Editor"�j
	 * @retval NULL		�G���[���͑��݂��Ȃ�
	 * @retval NULL�ȊO	Extension
	 */
	IExtensionPtr FindExtensionByProgID( LPCTSTR progID ) const;

	//@{ @name IMxApplication�C���^�[�t�F�[�X
	IDisplayPtr GetDisplay() const;
	//@}

	/**
	 * @brief <b>TOC���\������f�[�^�t���[�����擾����</b>\n
	 * �f�[�^�t���[������������ꍇ�́A��ɃA�N�e�B�u�Ȃ��̂�
	 * �擾���܂��B
	 * @return IMapPtr
	 */
	IMapPtr GetFocusMap() const;

	/**
	 * @brief <b>�t�H�[�J�X�̂���n�}��ʂ̃X�N���[�����擾����</b>\n
	 * @return IScreenDisplayPtr
	 */
	IScreenDisplayPtr GetFocusMapScreenDisplay() const;

	/**
	 * @brief <b>�t�H�[�J�X�̂���n�}��ʂ̃E�B���h�E�n���h�����擾����</b>\n
	 * GetHWND�Ƃ͎擾�ł���E�B���h�E�n���h�����قȂ邱�Ƃɒ��ӂ��Ă��������B
	 * @return HWND
	 */
	HWND GetFocusMapHWND() const;

	/**
	 * @brief <b>�G�f�B�^�R���|�[�l���g���擾����</b>\n
	 * SiNDYEditor��D��I�Ɏ擾���܂��BSiNDYEditor���擾�ł��Ȃ��ꍇ�ɂ̂�
	 * ArcMap�W����Editor���擾���܂��BArcMap�W����SiNDYEditor���ǂ����́A
	 * ISiNDYEditor�C���^�[�t�F�[�X���擾�ł��邩�ǂ����Ŋm�F���Ă��������B
	 * @retval NULL		�擾�ł��Ȃ��A���̓G���[�i���������Ȃǁj
	 * @retval NULL�ȊO	IEditorPtr
	 */
	IEditorPtr GetEditor() const;

	/**
	 * @brief <b>���݂̃r���[�I�u�W�F�N�g���擾����</b>\n
	 * �f�[�^�r���[�̏ꍇ��Map�A���C�A�E�g�r���[�̏ꍇ��PageLayout
	 * �����ꂼ��CoClass�ɂȂ�܂��̂Œ��ӂ��ĉ������B
	 * @return IActiveViewPtr
	 */
	IActiveViewPtr GetActiveView() const;

	/**
	 * @brief <b>���݂̃r���[�̋�`���擾����</b>\n
	 * ���̊֐��Ŏ擾������`�́A
	 * IActiveView::get_Extent()�œ�������̂ƈꏏ
	 * �ł��B
	 * @return �r���[��`
	 */
	IEnvelopePtr GetViewBounds() const;

	/**
	 * @biref <b>�A�N�e�B�u�f�[�^�t���[���ɓǂݍ��܂�Ă���e�[�u���E�t�B�[�`���N���X���擾����</b>\n
	 * �A�N�e�B�u�f�[�^�t���[���ɓǂݍ��܂�Ă���ꍇ�Ɍ���Ԃ��܂��B�Q�Ɨp�e�[�u���ȂǁA
	 * �A�N�e�B�u�f�[�^�t���[���ɓǂݍ��܂�Ă��Ȃ�
	 * �e�[�u�����擾�������ꍇ��OpenTableByTableName���g�p���Ă��������B
	 * nFindCount�̒l��1�ȏ�̏ꍇ�́A��舵���ɒ��ӂ��ĉ������B
	 * @see AheGetTableByTableName
	 * @param lpcszOwnerTableName	[in]			�I�[�i�[.�e�[�u����
	 * @param nFindCount			[in]			�������������i�����e�[�u���̏ꍇ�̓J�E���g���Ȃ��j
	 * @param bEditableOnly			[in,optional]	�ҏW�\�Ȃ��̂̂ݎ擾�������ꍇ��true
	 * @return �e�[�u���E�t�B�[�`���N���X���擾�ł��Ȃ��ꍇ��NULL
	 */
	ITablePtr GetTableByTableName( LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true ) const;

	/**
	 * @brief <b>�A�N�e�B�u�f�[�^�t���[���ɓǂݍ��܂�Ă���e�[�u���E�t�B�[�`���N���X���擾����</b>\n
	 * ���̊֐��͓�����GetTableByTableName���Ăт܂����A���̌�QueryInterface��
	 * IFeatureClass�C���^�[�t�F�[�X�������ǂ������`�F�b�N���܂��B
	 * nFindCount�̒l��1�ȏ�̏ꍇ�́A��舵���ɒ��ӂ��ĉ������B
	 * @see GetTableByTableName
	 * @see GetTableByTableName
	 * @param lpcszOwnerTableName	[in]			�I�[�i�[.�e�[�u����
	 * @param nFindCount			[in]			�������������i�����e�[�u���̏ꍇ�̓J�E���g���Ȃ��j
	 * @param bEditableOnly			[in,optional]	�ҏW�\�Ȃ��̂̂ݎ擾�������ꍇ��true
	 * @return �t�B�[�`���N���X���擾�ł��Ȃ��ꍇ��NULL
	 */
	IFeatureClassPtr GetFeatureClassByTableName( LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true ) const;

	/**
	 * @brief <b>�f�o�C�X���W -> �}�b�v���W�ϊ�</b>\n
	 * ���̊֐���IDisplayTransformation::ToMapPoint�̃V���[�g�J�b�g�ł��B
	 * @param ipView	[in]	�ϊ��Ώۃr���[
	 * @param X			[in]	�f�o�C�X���W
	 * @param Y			[in]	�f�o�C�X���W
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO �}�b�v���W
	 */
	IPointPtr ToMapPoint( IActiveView* ipView, long X, long Y ) const;

	/**
	 * @brief <b>Pixel -> �}�b�v�����ϊ�</b>\n
	 * @param ipView	[in]	�ϊ��Ώۃr���[
	 * @param nPixel	[in]	�s�N�Z����
	 * @retval 0�ȉ�	�G���[
	 * @retval ��L�ȊO	�}�b�v����
	 */
	double ToMapUnits( IActiveView* ipView, long nPixel ) const;

	/**
	 * @brief <b>�L�[�R�[�h�ł̊e���ʑ�����s��</b>
	 *
	 **/
	bool KeyZooMove( IActiveView* ipView, long lKeyCode, long lShift ) const;

	//@{ @name Editor�n����
	/**
	 * @brief <b>�X�i�b�v����</b>
	 * @param ipPoint [in,out] �X�i�b�v�Ώۃ|�C���g
	 * @retval true �X�i�b�v�ł���
	 * @retval false �X�i�b�v���Ȃ�����
	 */
	bool Snap( IPoint* ipPoint ) const;

	/**
	 * @brief <b>�ҏW�J�n��Ԃ��擾����</b>\n
	 * @retval esriStateEditing				�A�N�e�B�u�ȃf�[�^�t���[���ŕҏW���ł���
	 * @retval esriStateNotEditing			�ҏW�J�n����Ă��Ȃ�
	 * @retval esriStateEditingUnfocused	��A�N�e�B�u�ȃf�[�^�t���[���ŕҏW���ł���
	 */
	esriEditState GetEditState() const;

	HRESULT StartOperation();
	HRESULT StopOperation( LPCTSTR lpcszMenuText );
	HRESULT AbortOperation();
	//@}

	/**
	 * @brief <b>ArcMap���ǂ����𔻒f����</b>\n
	 * @retval true	ArcMap�ł���
	 * @retval false ArcMap�ł͂Ȃ�
	 */
	inline bool IsArcMap() const { CComBSTR bstrName; p->get_Name( &bstrName ); return ( 0 == wcscmp( bstrName, L"ArcMap" ) ) ? true : false; }

	/// ���[���I�u�W�F�N�g���擾����
	ISiNDYRulePtr GetRule() const;

	// StarEditOperation�����Ă��邩�ǂ������擾����
	// [Bug 10107]�o�H�O���H�ɕt�^���ꂽVICS�����N�񂪈ړ��i�L�k�j�E�폜�ł��Ȃ�
	// �֘A�ŁASDE��10.1�ڍs��StartEditOperation��2�d�|���͎��s����悤�ɂȂ�������
	// StartEditOperation�̃t���O�Ǘ�(m_bIsStartEditOperation)����
	inline bool IsStartEditOperation() const { return m_bIsStartEditOperation;};
protected:
	/**
	 * @brief <b>�O���[�v���C���̒�����e�[�u�����擾����</b>\n
	 * ���̊֐���GetTableByTableName����Ă΂�܂��B
	 * nFindCount�͊֐������ŏ���������܂���̂ŁA���ӂ��ĉ������B
	 * @param ipCompLayer			[in]			�O���[�v���C��
	 * @param lpcszOwnerTableName	[in]			�I�[�i�[.�e�[�u����
	 * @param nFindCount			[in]			�������������i�����e�[�u���̏ꍇ�̓J�E���g���Ȃ��j
	 * @param bEditableOnly			[in,optional]	�ҏW�\�Ȃ��̂̂ݎ擾�������ꍇ��true
	 */
	ITablePtr GetTableByTableNameFromCompositeLayer( ICompositeLayerPtr ipCompLayer, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true ) const;
protected:
	mutable ISiNDYRulePtr m_ipRule; //!< ���[���I�u�W�F�N�g�L���b�V��
private:
	bool m_bIsStartEditOperation; //!< StartEditOperation�����Ă��邩�ǂ���
};

#endif // _APPLICATION_H_
