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


// �ȉ��� ifdef �u���b�N�� DLL ����ȒP�ɃG�N�X�|�[�g������}�N�����쐬����W���I�ȕ��@�ł��B 
// ���� DLL ���̂��ׂẴt�@�C���̓R�}���h���C���Œ�`���ꂽ EXPORTDLG_EXPORTS �V���{��
// �ŃR���p�C������܂��B���̃V���{���͂��� DLL ���g�p����ǂ̃v���W�F�N�g��ł�����`�łȂ�
// ��΂Ȃ�܂���B���̕��@�ł̓\�[�X�t�@�C���ɂ��̃t�@�C�����܂ނ��ׂẴv���W�F�N�g�� DLL 
// ����C���|�[�g���ꂽ���̂Ƃ��� EXPORTDLG_API �֐����Q�Ƃ��A���̂��߂��� DLL �͂��̃}�N 
// ���Œ�`���ꂽ�V���{�����G�N�X�|�[�g���ꂽ���̂Ƃ��ĎQ�Ƃ��܂��B
/*#ifdef EXPORTDLG_EXPORTS
#define EXPORTDLG_API __declspec(dllexport)
#else
#define EXPORTDLG_API __declspec(dllimport)
#endif
*/

#ifndef __EXPORTDLG_H__
#define __EXPORTDLG_H__

class CArcHelper;
class CLQRowDef;
class CFeatureDef;

class CExportDlg
{
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL) = 0;
	virtual HWND GetDlg() = 0;
	virtual void Delete() = 0;
	virtual void SetArcHelper( IApplication* ipApp ) = 0;
	virtual void SetAliasOrField( BOOL bAliasOrField ) = 0;
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ) = 0;
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ) = 0;
	virtual BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable ) = 0;
	virtual void ClearFeatureDefs() = 0;
	virtual BOOL ErrorCheck() = 0;
	virtual LONG GetTabNum() = 0;
	virtual void SetTabNum(LONG lTabNum) = 0;
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName) = 0;
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ) = 0;
};
#endif  
