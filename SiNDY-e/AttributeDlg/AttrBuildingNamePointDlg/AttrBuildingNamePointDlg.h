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


#ifndef __ATTRBUILDINGNAMEPOINTDLG_H_
#define __ATTRBUILDINGNAMEPOINTDLG_H_

#include "resource.h"
#include "WinLib/Str2.h"

// ���X�g�r���[�J�������́A�T�C�Y
// TODO:�p�ꉻ�Ή�
const LPTSTR BNP_SRC_COLUMN0 = _T("�f��OID");
const int BNP_SRC_COLUMN0_SZ = 64;
const LPTSTR BNP_SRC_COLUMN1 = _T("����");
const int BNP_SRC_COLUMN1_SZ = 200;
const LPTSTR BNP_SRC_COLUMN2 = _T("�o�T�R�[�h");
const int BNP_SRC_COLUMN2_SZ = 112;

namespace list_col
{
	namespace building_src
	{
		enum Ecode
		{
			SrcID,     // �f��OID
			Name,      // ����
			SrcCode,   // �o�T�R�[�h

			ColumnNum, // �s��
		};
	}
}

// ���X�g�r���[�J������
#define BNP_SRC_COLUMN_COUNT 3

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrBuildingNamePointDlg
class CAttrBuildingNamePointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CAttrBuildingNamePointDlg();
	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~CAttrBuildingNamePointDlg()
	{
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrBuildingNamePointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_EDIT_NAME, EN_UPDATE, OnEnUpdateEditString)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHOID, BN_CLICKED, OnBnClickedButtonSearchoid)
	COMMAND_HANDLER(IDC_EDIT_SURVEYDATE, EN_UPDATE, OnEnUpdateEditString)
	COMMAND_HANDLER(IDC_BUTTON_PRIORITYC, BN_CLICKED, OnBnClickedButtonPriorityc)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_EDIT_BUILDINGOID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHOID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAMESEPARATED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_YOMISEPARATED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_BLDGCLASS1C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_BLDGCLASS2C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_BLDGCLASS3C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_FLOORS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SOURCETYPEC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SRCOID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SURVEYDATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ANYPLACEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_PRIORITYC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PRIORITYC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_MATCHPER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_LIST_SRCPOINT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()


private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	/**
	 * @brief �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief WM_COMMAND �p�C�x���g�n���h��
	 * @note  �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
	 * @note  ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief string�^�p�C�x���g�n���h��
	 * @note  IDC_EDIT_NAME�F���͎��̌`���Ɋւ�炸�A�S�p�ɕϊ����܂��B
	 * @note  IDC_EDIT_SURVEYDATE�F���͎��̌`���Ɋւ�炸���p�ɕϊ����A�s���ȕ����͓��͂ł��Ȃ��悤�ɂ��܂��B
	 */
	LRESULT OnEnUpdateEditString(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/** 
	 * @brief �q�_�C�A���O���쐬���܂��B
	 * @note  ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
	 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
	 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
	 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
	 */
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);

	/**
	 * @brief �쐬�����q�_�C�A���O���폜���܂��B
	 * @note �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
	 */
	virtual void Delete();

	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strClassName = lpcszFeatureClassName;};

	/**
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
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

	LRESULT OnBnClickedButtonSearchoid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonPriorityc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


private:
	/**
	 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
	 * @note ���ۂ̍쐬��ControlRel�ɔC���܂��B
	 */
	void CreateControlRelation();

	/**
	 * @brief �c�[���`�b�v���쐬���܂��B
	 * @note �쐬����c�[���`�b�v�͑����ɕύX��������Ȃ����߁A�����ςȂ��Ŗ��Ȃ�
	 * @param nTargetItem	[in]	�c�[���`�b�v���쐬����R���g���[���A�C�e����ID
	*/
	void SetToolTip(INT nTargetItem);

	/**
	 * @brief �����r�����̃|�C���g�ɕR�Â������r�����̑f�ރ|�C���g�̏������X�g�o�͂���
	 */
	void OutputBuildingSrcInfo();

	/**
	* @brief  �S�p�p�C�v��������NAME_SEPARATED��NAME�Ɠ���ł��邩�m�F���܂��B
	* @param  vaNameSep	[in]	NAME_SEPARATED�̒l
	* @param  vaName	[in]	NAME�̒l
	* @retval true		�`�F�b�NOK
	* @retval false		�`�F�b�NNG
	*/
	bool checkNameSeparated(const CComVariant& vaNameSep, const CComVariant& vaName);

	/**
	* @brief  YOMI_SEPARATED���S�p�J�^�J�i����ёS�p�p�C�v�݂̂ł��邩�m�F���܂��B
	* @param  vaVal	[in]	YOMI_SEPARATED�̒l
	* @retval true	�`�F�b�NOK
	* @retval false	�`�F�b�NNG
	*/
	bool checkYomiSeparated(const CComVariant& vaVal);


/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass;  //!< �e�R���g���[���E�B���h�E
	bool                            m_bShowSrcInfo; //!< �����r�����̑f�ރ|�C���g�̏����o�͂�����

	static LVCOLUMN m_ColumnInfo[];  //!< �f�ރ|�C���g�֘A���̃��X�g��`

};

#endif // __ATTRBUILDINGNAMEPOINTDLG_H_
