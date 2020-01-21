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
* @file MenuHelper.h
*
* @brief CMenu �N���X�̒�`�t�@�C��
*
* @author ��t �q��
*
* @date   2002/01/23
*/
#if	!defined( __MENUHELPER_H__)
#define	__MENUHELPER_H__	//!< �d���C���N���[�h�h�~

/**
* @class CMenu
*
* @brief ICON�`��@�\���̃I�[�i�[�`�惁�j���[�w���p�N���X
*
* @note �c�[���o�[��A���j���[�o�[���痘�p�����֐��̂��߁A���j���[�\�����ɐe�ƂȂ�Window�̐�����s���Ă���
*/
namespace AHE
{
class CMenu
{
public:
	CMenu( void) : m_hParent( NULL), m_hMenu( NULL), m_atmMenuParent( 0)
	{
		hFont = reinterpret_cast< HFONT>( ::GetStockObject( DEFAULT_GUI_FONT));
		CXFrame = ::GetSystemMetrics( SM_CXFRAME);
		CYBorder = ::GetSystemMetrics( SM_CYBORDER);
		CXBorder = ::GetSystemMetrics( SM_CXBORDER);
		dwColor = ::GetSysColor( COLOR_GRAYTEXT);
		clrBkColorHighlight = ::GetSysColor( COLOR_HIGHLIGHT);
		clrTextColorHighlight = ::GetSysColor( COLOR_HIGHLIGHTTEXT);
		clrBkColorMenu = ::GetSysColor( COLOR_MENU);
		clrTextColorGray = ::GetSysColor( COLOR_GRAYTEXT);
		clrTextColorText = ::GetSysColor( COLOR_MENUTEXT);
//		listData.clear();
	}
	~CMenu( void);
	/** 
	* @brief ���j���[�쐬�B
	*
	* @note �ŏ�ʊK�w���쐬����ꍇ�A�e�ƂȂ�Window�̍쐬�𓯎��ɍs���B
	* @note �q���j���[�̏ꍇ�AWindow�̐����͍s���Ȃ��B
	*
	* @param blParent [in] �ŏ�ʃ��j���[�t���b�O
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL CreateMenu( BOOL blParent = FALSE);

	/** 
	* @brief ���j���[���ڂ̑}��
	*
	* @note �r�b�g�}�b�v�t�@�C���̊Ǘ��͋L���̂ݍs�����߁A�A�v���P�[�V�������Ń��j���[�\���I����폜����K�v������B
	* @note ������𖢎w��͋����I�ɃZ�p���[�^�Ƃ���
	*
	* @param unPosition [in] �}���ʒu(Win32SDK�Q�Ƃ̂���
	* @param unFlag [in] �}������t���O(Win32SDK�Q�Ƃ̂���
	* @param unIDNewItem [in] �}�����j���[����ID(Win32SDK�Q�Ƃ̂���
	* @param lpcszNewItem [in] ���j���[������
	* @param hBitmap [in] ���j���[���ڂɐݒ肳���r�b�g�}�b�v�n���h��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL InsertMenu( UINT unPosition, UINT unFlag, UINT unIDNewItem = -1, LPCTSTR lpcszNewItem = NULL, HBITMAP hBitmap = NULL);

	/** 
	* @brief �|�b�v�A�b�v���j���[��\��
	*
	* @param unFlag [in] ���j���[�\������t���O(Win32SDK�Q�Ƃ̂���
	* @param x [in] ���j���[�\���ʒu
	* @param y [in] ���j���[�\���ʒu
	* @param lpstRest [in] 
	*
	* @return �I�����ꂽ���j���[ID�B���I���͂O(Zero)���Ԃ����
	*/
	int TrackPopupMenu( UINT unFlag, int x, int y, LPCRECT lpstRect = NULL);

	/** 
	* @brief ���j���[�̃`�F�b�N
	*
	* @note �w��̃��j���[�Ƀ`�F�b�N�}�[�N�t���̐ݒ���s��
	*
	* @param nIDCheckItem [in] ���j���[�`�F�b�N�Ώ�ID(Win32SDK�Q�Ƃ̂���
	* @param nCheck [in] �`�F�b�N���(Win32SDK�Q�Ƃ̂���
	*
	* @return ���O�̃`�F�b�N��ԁBID�����݂��Ȃ��ꍇ�́A-1
	*/
	UINT CheckMenuItem( UINT nIDCheckItem, UINT nCheck);

	/** 
	* @brief ���j���[�̎g�p����
	*
	* @note �w��̃��j���[�Ɏg�p���̐ݒ���s��
	*
	* @param nIDEnableItem [in] ���j���[�g�p���Ώ�ID(Win32SDK�Q�Ƃ̂���
	* @param nEnable [in] �g�p�����(Win32SDK�Q�Ƃ̂���
	*
	* @return ���O�̎g�p����ԁBID�����݂��Ȃ��ꍇ�́A-1
	*/
	UINT EnableMenuItem( UINT nIDEnableItem, UINT nEnable);

	/** 
	* @brief ���j���[�̏�Ԏ擾
	*
	* @param nID [in] ���j���[�`�F�b�N�Ώ�ID(Win32SDK�Q�Ƃ̂���
	* @param nFlags [in] �`�F�b�N���(Win32SDK�Q�Ƃ̂���
	*
	* @return ���݂̃`�F�b�N��ԁBID�����݂��Ȃ��ꍇ�́A-1
	*/
	UINT GetMenuState( UINT nID, UINT nFlags ) const;

	/** 
	* @brief ���j���[�Ɋ܂܂���A�C�e���̌���Ԃ�
	*
	* @return ���j���[���ڐ�
	*/
	UINT GetMenuItemCount( ) const;

	/** 
	* @brief �e�t���̃��j���[�̐e���擾
	*
	* @note WM_MENUSELECT&SubclassWindow�΍�
	*
	* @return Window handle
	*/
	HWND GetParentWnd() const;


	/** 
	* @brief HMENU���N���X�Ɋ֘A�t����
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL Attach( HMENU hMenu);

	/** 
	* @brief HMENU�����̃N���X����؂藣��
	*
	* @note �؂藣����HMENU�́A���̃N���X���폜���Ă��f�X�g���N�^�ō폜����Ȃ��B
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	HMENU Detach( void);

private:
	/**
	* @brief ���j���[�폜�֐��B
	*
	* @note �K�w�ɂ��������čċN�I�ɍ폜�����s����B
	*
	* @param hMenu [in] �폜�ΏۂƂȂ郁�j���[�̃n���h��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL DestroyMenu( HMENU hMenu);

	/**
	* @brief �e�ƂȂ�Window�̍폜
	*
	* @note �e�ƂȂ��Ă���Window��j������
	*
	* @param hWnd [in] �eWindow�̃n���h��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL DestroyWindow( HWND hWnd);

	/**
	* @brief ���j���[����сA�e�ƂȂ�Window�̍폜
	*/
	void Delete( void);

	/**
	* @brief WindowsMessage�̃I�[�i�[�`��Ή��֐�
	*
	* @param nIDCtl [in] �`��A�C�e��ID
	* @param lpDrawItemStruct [in] �`����
	*/
	void OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	/**
	* @brief WindowsMessage�̃��j���[�T�C�Y�����֐�
	*
	* @note InsertMenu���Ɍv�Z���Ă������l��Ԃ��A�K���ȃT�C�Y�̃��j���[���쐬����B
	*
	* @param nIDCtl [in] �`��A�C�e��ID
	* @param lpDrawItemStruct [in/out] �`����
	*/
	void OnMeasureItem( int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	/**
	* @brief �e�ƂȂ�Window�̃N���X�o�^�֐�
	*
	* @note �I�[�i�`�惁�j���[�ɉ������邽�߂�Window�N���X�o�^����
	*/
	ATOM RegisterClass( void);

	HWND m_hParent;			//!< �ŏ�ʃ��j���[�̏ꍇ�̐eWindow�̃n���h��
	HMENU m_hMenu;			//!< �Ǘ��Ώۂ̃��j���[�n���h��
	ATOM m_atmMenuParent;	//!< �eWindow�N���X�o�^���

	static LPCTSTR m_lpcszMenuParent;	//!< �eWindow�̃N���X����

	/**
	* @brief Window�R�[���o�b�N�֐�
	*
	* @note �ŏチ�j���[�̐eWindow�̃R�[���o�b�N�֐�
	*
	* @param hWnd [in]
	* @param unMessage [in]
	* @param wParam [in]
	* @param lParam [in]
	*
	* @return HRESULT
	*/
	static HRESULT CALLBACK WindowProc( HWND hWnd, UINT unMessage, WPARAM wParam, LPARAM lParam);

	/**
	* @brief �`��ɗ��p����A�C�R���T�C�Y
	*
	* @note 16x16�̃T�C�Y�Ɍ��肵�Ă��邪�A�T�C�Y���擾���ĕ\�������邱�Ƃ��\�Ȃ̂ŁA�󋵂ɉ����Ă��イ�����̂��ƁB
	*/
	enum
	{
		_ICON_WIDTH = 16,	//!< �A�C�R���T�C�Y�i����
		_ICON_HEIGHT = 16	//!< �A�C�R���T�C�Y�i��
	};

	/**
	* @brief ���j���[���ڂɕt������f�[�^�Ǘ��̂��߂̍\����
	*
	* @note ���̃f�[�^�����j���[�A�C�e���̌ŗL�f�[�^�Ƃ��ĕۑ�������
	*/
	struct ITEMDATA
	{
		int			cbSize;				//!< �\���̃T�C�Y
		HBITMAP		hBitmap;			//!< �r�b�g�}�b�v
		UINT		unIDItem;			//!< ���j���[ID
		SIZE		stSize;				//!< ���j���[�Ƃ��ĕ\������ꍇ�̃T�C�Y
		TCHAR		lpItemString[1];	//!< ���j���[������
//		void		*mallocAddr;
	};

	HFONT		hFont;
	INT			CXFrame;
	INT			CYBorder;
	INT			CXBorder;
	DWORD		dwColor;
	COLORREF	clrBkColorHighlight;
	COLORREF	clrTextColorHighlight;
	COLORREF	clrBkColorMenu;
	COLORREF	clrTextColorGray;
	COLORREF	clrTextColorText;

//	list<LPVOID>		listData;

};
}
#endif	// __MENUHELPER_H__
