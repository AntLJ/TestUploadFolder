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


typedef CAttrBaseDlg ATTR_BASE_CLASS;

#define XMD_H
#include "util/Jpeg.h"

const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W

// �摜�����N���X(CitySurveyPointDlg����p�N��)
// CJpeg��TrackLogManager�̉摜�`��p�N���X�B
// �摜�ǂݍ��ݓ��̊�{�@�\���p�����A�����_�C�A���O�p�̕`��@�\�����������̂�CJpeg2�N���X�B
class CJpeg2 : public CJpeg
{
public:
	CJpeg2()
	{
	}
	virtual ~CJpeg2()
	{
	}

	/*
	 * @brief �ێ����Ă���摜��`�悷��
	 * @note �摜�̂���Ȃ��𔻒肵�āA���ꂼ��̏ꍇ�ɓK�����`��p�֐����Ăт܂�
	 * @param hWnd [in] �E�B���h�E�̃n���h��
	 * @param rc   [in] �`��͈�
	 */
	void draw(HWND hWnd, RECT& rc) const
	{
		m_cDib.m_pImage ? draw_Picture(hWnd, rc) : draw_NoPicture(hWnd, rc);
	}
	
	/*
	 * @brief �ێ����Ă���摜��`�悷��
	 * @param hWnd [in] �E�B���h�E�̃n���h��
	 * @param rc   [in] �`��͈�
	 */
	void draw_Picture(HWND hWnd, RECT& rc) const
	{
		HDC dc = ::GetDC(hWnd);
		::SetStretchBltMode(dc, HALFTONE);
		::StretchDIBits(dc, rc.left, rc.top, abs(rc.left-rc.right), abs(rc.bottom-rc.top),
			0, 0, m_cDib.m_biHeader.biWidth, m_cDib.m_biHeader.biHeight, m_cDib.m_pImage, (BITMAPINFO *)&m_cDib.m_biHeader, DIB_RGB_COLORS, SRCCOPY);
		::ReleaseDC(hWnd, dc);
	}

	/*
	 * @brief �摜���Ȃ��ꍇ�E�ǂݍ��݂Ɏ��s���Ă���ꍇ�ɍ����h��Ԃ�
	 * @param hWnd [in] �E�B���h�E�̃n���h��
	 * @param rc   [in] �`��͈�
	 */
	void draw_NoPicture(HWND hWnd, RECT& rc) const
	{
		HRGN rgn = ::CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
		HDC dc = ::GetDC(hWnd);
		::GetClipRgn(dc, rgn);
		::FillRgn(dc, rgn, static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
		PAINTSTRUCT paint;
		::BeginPaint(hWnd, &paint);
		::SetBkColor(dc, RGB(0, 0, 0));
		::SetTextColor(dc, RGB(255, 255, 255));
		::EndPaint(hWnd, &paint);
		::DeleteObject(rgn); // GDI���[�N����ibug 9786�j
		::ReleaseDC(hWnd, dc);
	}
};

// �B�e�|�C���g�ێ��N���X
struct PicPoint
{
	PicPoint()
	{
		m_x = 0;
		m_y = 0;
		m_pictureSeq = 0;
		m_angle = 0;
	}

	/**
	 * @brief �R���X�g���N�^
	 * @note �t�B�[�`������xy���W�A�摜�i�[���A�B�e�������擾�E�ێ����܂�
	 * @param ipFeature    �B�e�|�C���g�t�B�[�`��
	 * @param sequenceIdx  �u�摜�i�[���v�t�B�[���h�C���f�b�N�X
	 * @param directionIdx �u�B�e�����v�t�B�[���h�C���f�b�N�X
	 */
	PicPoint( const IFeaturePtr& ipFeature, long sequenceIdx, long directionIdx )
	{
		// Point�ȊO���Ȃ��O��
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );

		if( !ipGeom )
		{
			// �B�e�|�C���g��NULL�����蓾��̂�0�����Ă����B
			m_x = 0;
			m_y = 0;
		}
		else
		{
			((IPointPtr)ipGeom)->QueryCoords( &m_x, &m_y );
		}

		CComVariant vaSequence;
		ipFeature->get_Value( sequenceIdx, &vaSequence );
		m_pictureSeq = vaSequence.lVal;

		CComVariant vaAngle;
		ipFeature->get_Value( directionIdx, &vaAngle );
		m_angle = vaAngle.lVal;
	}

public:
	double m_x; //!< �o�x
	double m_y; //!< �ܓx
	long m_pictureSeq; //!< �i�[��
	long m_angle; //!< �B�e����(�x) �^����0�x�Ƃ��č����ɑ����Ă���
};

/////////////////////////////////////////////////////////////////////////////
// CAttrSpotSurveyPointDlg
class CAttrSpotSurveyPointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrSpotSurveyPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_BUTTON_ILLUST, BN_CLICKED, OnClickedIllustButton)
	COMMAND_HANDLER(IDC_BUTTON_POINT, BN_CLICKED, OnClickedPointButton)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
	MSG_WM_HSCROLL(OnHScroll)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS);

ALT_MSG_MAP(IDC_EDIT_NG_ATTR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NG_COMMENT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_ILLUST)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_POINT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/**
	 * WM_COMMAND �p�C�x���g�n���h��
	 *
	 * �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
	 * ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
	 */
	LRESULT OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// �����摜�T���l�C���`��
	LRESULT OnPaint( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// �f�[�^�r���[�`��
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// ���N���b�N�C�x���g
	LRESULT OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// ���_�u���N���b�N�C�x���g
	LRESULT OnLButtonDblClk( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// �X���C�_�[�����������̃C�x���g
	void OnHScroll( int nCode, short nPos, HWND hWnd );

	/// �C���X�g�\���{�^��
	LRESULT OnClickedIllustButton( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled );

	/// �ړ��O��m�F�{�^��
	LRESULT OnClickedPointButton( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled );


private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrSpotSurveyPointDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();

	/**
	 * @brief  �ړ��O�|�C���g�擾
	 * @return �|�C���g�擾����
	 */
	bool LoadSurveyPointOrg();

	/**
	 * @brief �C���X�g�`��擾
	 * @param [in] ipWorkspace ���[�N�X�y�[�X
	 * @param [in] owner       �I�[�i��
	 * @param [in] table       �e�[�u����
	 * @param [in] id          ObjectID
	 * @return �C���X�g�`��
	 */
	IGeometryPtr GetIllustGeometry( const IWorkspacePtr& ipWorkspace, const CString& owner, const CString& table, const long id );

	/**
	 * @brief �C���X�g�`��`��
	 */
	void DrawIllustGeometry();

	/**
	 * @brief �ړ��O�|�C���g�`��
	 */
	void DrawOrgPointGeometry();

	/**
	 * @brief �B�e�|�C���g�`��
	 */
	void DrawPicturePoint();

	/**
	 * @brief ���[�N�X�y�[�X�擾
	 * @return �I�𒆂�SPOT_SURVEY_POINT�����݂��郏�[�N�X�y�[�X
	 */
	IWorkspacePtr GetWorkspace();

	/**
	 * @brief �N���b�N�����摜�̉摜�i�[�����擾
	 * @note XY���W�͂��̃_�C�A���O����_�ɂ������́B�X�N���[���̐�΍��W�ł͂Ȃ��B
	 * @param [in] clickedX       �N���b�N�n�_��X���W
	 * @param [in] clickedY       �N���b�N�n�_��Y���W
	 * @return �N���b�N�����摜�̉摜�i�[��
	 */
	int getClickedPictureSeq( long clickedX, long clickedY );

	/**
	 * @brief �T���l�C�����ꂼ��̕`��ʒu�E���E�������擾
	 * @return <�摜�i�[��,�T���l�C���̕`��ʒu�E���E����>
	 */
	std::map<long,CRect> getThumbSize();

	/**
	 * @brief �B�e�|�C���g�ɕR�Â��摜�擾
	 */
	void FindPicture();

	/**
	 * @brief ���W�X�g������摜�p�X���擾
	 */
	void GetRegCode();

	/**
	 * @brief �B�e�|�C���g�n�C���C�g
	 * @note  �O�p���V���{�����쐬���ĕ`�悵�܂��B
	 * @param [in] ipScreenDisplay IDisplay�̃C���X�^���X
	 * @param [in] ipPoint         �Ώۃ|�C���g
	 * @param [in] bXOR            XOR�`�悷�邩�ǂ���
	 * @param [in] size            �V���{���̑傫��
	 * @param [in] red             �V���{���̐F�v�f(��)
	 * @param [in] green           �V���{���̐F�v�f(��)
	 * @param [in] blue            �V���{���̐F�v�f(��)
	 * @param [in] angle           �V���{���̉�]�p
	 * @param [in] bDoStartStop    StartDrawing, StopDrawing���邩�ǂ����i�f�t�H���g true�j
	 */
	void DrawPicturePointAllow( IDisplayPtr ipScreenDisplay, IPointPtr ipPoint, BOOL bXOR = FALSE, long size = 10, long red = 200, long green = 200, long blue = 200, long angle = 0, bool bDoStartStop = true );

	/**
	 * @brief �n�C���C�g�`��Ώۂ̃W�I���g���ϐ���nullptr���Z�b�g���܂�
	 */
	void SetNull2DrawGeometry();

	/**
	 * @brief �B�e�摜�|�C���g���̍ĕ`��
	 */
	void RefreshPicturePointAllow();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////

private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E
	
	HWND                  m_hChildWnd;          //!< �q�_�C�A���O�n���h��
	long                  m_objectID;           //!< �I�u�W�F�N�gID
	CString               m_baseDir;            //!< �摜�t�H���_
	CString               m_toolPath;           //!< �摜�\���c�[���̃p�X
	long                  m_slider;             //!< �X���C�_�[����p
	std::vector<CString>  m_picPath;            //!< �摜�p�X�i�[�p

	bool                  m_illustIsDrawed;     //!< �C���X�g�`�悵�Ă邩�ǂ���
	bool                  m_orgPointIsDrawed;   //!< �����O�|�C���g��`�悵�Ă��邩�ǂ���
	IGeometryPtr          m_surveyPointGeom;    //!< �����|�C���g�W�I���g��
	IGeometryPtr          m_orgSurveyPointGeom; //!< �����O�|�C���g�W�I���g��
	IGeometryPtr          m_illustPointGeom;    //!< �C���X�g�|�C���g�W�I���g��
	IGeometryPtr          m_illustLineGeom;     //!< �C���X�g���C���W�I���g��
	IGeometryPtr          m_illustPolygonGeom;  //!< �C���X�g�|���S���W�I���g��
	long                  m_pictureSeqIdx;      //!< �摜�i�[���t�B�[���h�̃C���f�b�N�X
	long                  m_directionIdx;       //!< �B�e�����t�B�[���h�̃C���f�b�N�X
	int                   m_selectedPicSeq;     //!< �I�𒆂̎B�e�|�C���g�摜�V�[�P���X
	std::vector<PicPoint> m_picPointVec;        //!< �B�e�|�C���g�Q
	std::vector<CJpeg2>   m_pictureVec;         //!< �B�e�摜�Q
};
