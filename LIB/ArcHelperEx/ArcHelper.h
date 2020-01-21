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
* @file ArcHelper.h
*
* @brief IApplication�Ɋ֘A���鏈�����s��ArcHelper�̒�`�t�@�C��
*/
#if !defined(AFX_ARCHELPER_H__E819DB32_9167_4CF3_9DF4_FF0DF22918B0__INCLUDED_)
#define AFX_ARCHELPER_H__E819DB32_9167_4CF3_9DF4_FF0DF22918B0__INCLUDED_	//!< �d���C���N���[�h�h�~

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Application.h"

/**
* @class CArcHelper
*
* @brief IApplication ���Ǘ�����N���X
*/
class CArcHelper : public CApplication
{
public:
	CArcHelper();
	virtual ~CArcHelper();

	//////////////////////////////////////////////////////////////////////
	// �������Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief ������
	*
	* @note �n���ꂽ�C���^�[�t�F�C�X�𔻒肵�ăN���X������������B
	*
	* @param hook [in] �I�u�W�F�N�g�̃C���X�^���X
	*/
	void Init( IDispatchPtr hook );

	/**
	* @brief ������
	*
	* @note �n���ꂽ�C���^�[�t�F�C�X�ŏ���������B
	*
	* @param ipApp [in] IApplication�̃C���X�^���X
	*/
	void Init( IApplicationPtr ipApp );

	/**
	* @brief ������
	*
	* @note �n���ꂽ�C���^�[�t�F�C�X�ŏ���������B
	*
	* @param ipEditor [in] IEditor �̃C���X�^���X
	*/
	void Init( IEditorPtr ipEditor );

	//////////////////////////////////////////////////////////////////////
	// ���������@�̔���p�֐�
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief �������ς݊m�F
	*
	* @note ���̃N���X���������ς݂ł��邩���m�F����B
	* @note ��{�I�ɂ��̊֐��́A���\�b�h�̗L�������e�X�g���邽�߁A�����œK�X���p�����B
	*
	* @retval TRUE �������ς�
	* @retval FALSE ��������
	*/
	BOOL CheckInit() const;

	/**
	* @brief �������ς݊m�F
	*
	* @note ���̃N���X�� IApplication �ŏ������ς݂ł��邩���m�F����B
	* @note ��{�I�ɂ��̊֐��́A���\�b�h�̗L�������e�X�g���邽�߁A�����œK�X���p�����B
	*
	* @retval TRUE �������ς�
	* @retval FALSE ��������
	*/
	BOOL CheckApp() const { return CheckInit(); }

	/**
	* @brief �������ς݊m�F
	*
	* @note ���̃N���X�� IEditor �ŏ������ς݂ł��邩���m�F����B
	* @note ��{�I�ɂ��̊֐��́A���\�b�h�̗L�������e�X�g���邽�߁A�����œK�X���p�����B
	*
	* @retval TRUE �������ς�
	* @retval FALSE ��������
	*/
	BOOL CheckEditor() const;

	//////////////////////////////////////////////////////////////////////
	// �I�u�W�F�N�g�擾�֐�
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief IApplication �̃C���X�^���X�擾
	*
	* @note �N���X���Ǘ����Ă��� IApplication �̃C���X�^���X��Ԃ��B
	*
	* @return IApplication �̃C���X�^���X
	*/
	IApplicationPtr GetApp() const { return (IApplication*)*this; }

	/**
	* @brief IActiveView �̃C���X�^���X�擾
	*
	* @param bMapDisplay	[in,optional]	�n�}�r���[�� ScreenDisplay �����ꍇ�� TRUE�i�f�t�H���g�j�A�J�����g�����ꍇ�� FALSE
	*
	* @return IActiveViewPtr
	*/
	IActiveViewPtr GetActiveView(  BOOL bMapDisplay = TRUE ) const
	{
		//return ( bMapDisplay ) ? GetFocusMap() : CApplication::GetActiveView(); <- 3�����Z�q�ŃX�}�[�g�|�C���^��Ԃ��ƍŏI�����K��NULL�ɂȂ�H�H�ibug 5797�j
		if( bMapDisplay )
			return GetFocusMap();
		else
			return CApplication::GetActiveView();
	}

	/**
	* @brief IMap �̃C���X�^���X�擾
	*
	* @note �N���X���Ǘ����Ă���C���X�^���X�ɉ����āA��������擾���� IMap �̃C���X�^���X��Ԃ��B
	*
	* @return IMap �̃C���X�^���X
	*/
	IMapPtr GetMap() const { return GetFocusMap(); }

	/**
	* @brief IScreenDisplay �̃C���X�^���X�擾
	*
	* @param bMapDisplay	[in,optional]	�n�}�r���[�� ScreenDisplay �����ꍇ�� TRUE�i�f�t�H���g�j�A�J�����g�����ꍇ�� FALSE
	*
	* @return IScreenDisplayPtr
	*/
	IScreenDisplayPtr GetScreenDisplay( BOOL bMapDisplay = TRUE ) const;

	/**
	* @brief IGraphicsContainer �̃C���X�^���X�擾
	*
	* @param bMapDisplay	[in,optional]	�n�}�r���[�� ScreenDisplay �����ꍇ�� TRUE�i�f�t�H���g�j�A�J�����g�����ꍇ�� FALSE
	*
	* @return IGraphicsContainerPtr
	*/
	IGraphicsContainerPtr GetGraphicsContainer( BOOL bMapDisplay = TRUE ) const;

	/**
	* @brief TOC���猻�ݑI������Ă��郌�C�����擾����
	*
	* @note ��{�I��TOC�Ō��ݑI�����Ă��郌�C����Ԃ��B
	*		�O���[�v���C���I������Ă���ꍇ�ɂ́A���̂Ȃ��̕ҏW�\����Ă��郌�C�����擾���Ă���
	*		���̃O���[�v���C���̒��ɕ����̕ҏW�\���C��������ꍇ�ɂ́A�I�����Ă���O���[�v���C����Ԃ��܂��B
	*
	* @param ppiLayer [out] �I������Ă��郌�C���̃C���X�^���X
	* @param bIsEditable [in]�@�ҏW�J�n����Ă��郌�C�����O���[�v���C���̉��̃��C������E���Ă��邩�H(�f�t�H���g�Ftrue)
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	ILayerPtr GetSelectedLayer( bool bIsEditable = true ) const;

	//////////////////////////////////////////////////////////////////////
	// ���̑��̊֐�
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief �Z���^�����O�Ɗg�嗦�ύX
	*
	* @note bScale �ɂO(Zero)�ȉ���ݒ肷��Ɗg�嗦�̕ύX�͍s���Ȃ��B
	*
	* @param piPoint [in] �ړ��ʒu
	* @param dScale [in,optional] �g�嗦
	* @param bMapDisplay [in,optional] GetActiveView() �̈��� bMapDisplay �Ɠ��`�B
	*/
	void SetCenter( IPointPtr piPoint, DOUBLE dScale = -1.0, BOOL bMapDisplay = TRUE ) const;

	/// TOC ���ĕ`�悷��
	void RefreshContentsView() const;
	//////////////////////////////////////////////////////////////////////
	// �ҏW�Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief �ҏW�����J�n
	*
	* @note �S�Ẵ��C���ɑ΂��A�ҏW�������J�n����B
	*
	* @retval TRUE �ҏW�����J�n
	* @retval FALSE �ҏW�����͊J�n�ł��Ȃ�
	*/
	BOOL StartEditOperation( void){ return SUCCEEDED(StartOperation()) ? TRUE : FALSE; }

	/**
	* @brief �ҏW�����I��
	*
	* @note ���ݍs���Ă���ҏW�������I������B
	*
	* @param �ҏW�I���}�[�N������
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL StopEditOperation( LPCTSTR lpcszEditName ){ return SUCCEEDED(StopOperation(lpcszEditName)) ? TRUE : FALSE; }

	/**
	* @brief �ҏW�������f
	*
	* @note ���ݍs���Ă���ҏW�����𒆒f����B�S�Ă̏����͖����ɂȂ�B
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL AbortEditOperation(){ return SUCCEEDED(AbortOperation()) ? TRUE : FALSE; }

	/**
	 * @brief �ҏW�J�n����Ă��邩�ǂ���
	 *
	 * @retval TRUE �ҏW�J�n����Ă���
	 * @retval FALSE �ҏW�J�n����Ă��Ȃ�
	 */
	BOOL IsStartEditing() const { return GetEditState() == esriStateEditing ? TRUE : FALSE; }
	//////////////////////////////////////////////////////////////////////
	// �X�i�b�v�Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief �X�i�b�v�|�C���g���擾����
	*
	* @note �X�i�b�v�|�C���g���擾���܂��B
	* @note �J�X�^���A�v���P�[�V�����ł͎g�p�ł��܂���B
	*
	* @param piPoint [in,retval] �}�E�X�|�C���^�̈ʒu
	*
	* @retval TURE �X�i�b�v�����B
	* @retval FALSE �X�i�b�v���Ă��Ȃ�
	*/
	BOOL Snap( IPointPtr piPoint) const;
	/**
	* @brief �X�i�b�v�|�C���g���擾����
	*
	* @note �X�i�b�v�|�C���g���擾���܂��B
	* @note �X�i�b�v�����_�������������ꍇ�A��ԋ߂������̂��̂�D�悵������΁A bFirstOnly �� FALSE ���w�肵�Ă��������i�܂��������Ă��܂���j�B
	*
	* @param piPoint         [in]       �}�E�X�|�C���^�̈ʒu
	* @param ipSnappedPoint  [retval]   �X�i�b�v�����|�C���g
	* @param bFirstOnly      [optional] �����������ꍇ�ɋ����𖳎����邩�ǂ���
	*
	* @retval TURE �X�i�b�v�����B
	* @retval FALSE �X�i�b�v���Ă��Ȃ�
	*/
	BOOL Snap( IPointPtr ipPoint, IPoint** ipSnappedPoint, BOOL bFirstOnly = TRUE ) const;

	/**
	* @brief �X�i�b�v�|�C���g���擾����
	*
	* @note �X�i�b�v�|�C���g���擾���܂��B�J�X�^���A�v���P�[�V�����p�̃R�[�h�͖������ł��B
	* @note bCheckClass �� FALSE �Ȃ� SnapType �� sindyeSnapNone�AsindyeSnapRoadIntersection�AsindyeSnapAny �̂����ꂩ�ɂȂ�܂��B
	* @note bCheckHitType �� TRUE �ɂ���ꍇ�� HitType �� NULL ���w�肵�Ȃ��ł��������B
	*
	* @param piPoint         [in]       �}�E�X�|�C���^�̈ʒu
	* @param ipSnappedPoints [retval]   �X�i�b�v�����|�C���g�̃R���N�V����
	* @param SnapType        [retval]   �X�i�b�v�����t�B�[�`���̃^�C�v
	* @param bCheckClass     [optional] �X�i�b�v�����t�B�[�`���̃N���X����肷�邩�ǂ���
	* @param bCheckHitType   [optional] �X�i�b�v�����|�C���g�̃^�C�v����肷�邩�ǂ���
	* @param HitType         [optional] �X�i�b�v�����|�C���g�̃^�C�v
	*
	* @retval TURE �X�i�b�v�����B
	* @retval FALSE �X�i�b�v���Ă��Ȃ�
	*/
	BOOL Snap( IPointPtr ipPoint, IPointCollection** ipSnappedPoints,
		sindyeSnappingType* type, BOOL bCheckClass = FALSE,
		BOOL bCheckHitType = FALSE, esriGeometryHitPartType* HitType = NULL ) const;
	//////////////////////////////////////////////////////////////////////
	// �C�x���g�ڑ��Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief IActiveView �ɐڑ�
	*
	* @param piUnk [in] �ڑ�����I�u�W�F�N�g�C���X�^���X
	* @param dwCookie [in/out] �ڑ����؂̃N�b�L�[�l
	*
	* @return �N�b�L�[�l
	*/	
	DWORD ConnectToActiveView( IUnknownPtr piUnk, DWORD& dwCookie ) const;

	/**
	* @brief IActiveView �̐ڑ�����
	*
	* @param dwCookie [in/out] �ڑ����̃N�b�L�[�l
	*
	* @return �N�b�L�[�l
	*/	
	DWORD DisconnectFromActiveView( DWORD& dwCookie ) const;
	//////////////////////////////////////////////////////////////////////
	// ���̑��̊֐�
	//////////////////////////////////////////////////////////////////////
	/**
	 * @brief �}�E�X�ʒu�� XY ����n�}��̍��W�ɕϊ�����
	 *
	 * @note ���݂̒n���Q�Ƃ��Z�b�g���܂�
	 *
	 * @return IPointPtr
	 */
	IPointPtr QueryMapPoint( LONG X, LONG Y ) const { return ToMapPoint( (IActiveViewPtr)GetFocusMap(), X, Y ); }

	/**
	 * @brief �s�N�Z������}�b�v���j�b�g�ɕϊ����܂�
	 *
	 * @return ���݂̃}�b�v���j�b�g�ł̒���
	 */
	DOUBLE ConvertPixelsToMapUnits( INT nPixels ) const { return ToMapUnits( (IActiveViewPtr)GetFocusMap(), nPixels ); }

	BOOL IsArcMapBase() const { return IsArcMap(); }

protected:
	mutable IWorkspacePtr m_ipReferenceWorkspace;	//!< REFERENCE �ւ̐ڑ��p
};

#endif // !defined(AFX_ARCHELPER_H__E819DB32_9167_4CF3_9DF4_FF0DF22918B0__INCLUDED_)
