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

#pragma once

#include <windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#define _ATL_TMP_NO_CSTRING     // ATL��WTL��CString�����Ԃ邽��
#include <atlapp.h>             // WTL���g�p
#include <atlmisc.h>            // CPoint CSize CRect
#include <atlctrls.h>
#include <atlstr.h>             // CString
#include <atlcrack.h>           // ���b�Z�[�W�n

#include <boost/utility.hpp>
#include <list>
#include <map>

#include "CommonData.h"

/**
* @brief �wAOI�|���S���̏����ɓ�������ListViewCtrl
* @note  ���X�g�r���[�̕\�������Ńf�[�^���Ǘ�����ƃt�B�[�`���؂�ւ��̍ۂɕs�s�����o�����߁A
* @note  ��x�\�������f�[�^�͓����I�ɕێ����A�ĕ\���ł���悤�ɂ���
* @note  �ėp�I�ɍ�낤�Ƃ��č���
* @note  �\�z�i�K�ł́A���X�g�r���[��̃f�[�^��list<list<CString>>�𓙉��Ɉ�����悤�ɂ��悤�Ƃ��Ă���
* @note  ([][]�̂悤�ɎQ�Ƃł���̂ŁA�f�[�^�̎󂯓n����vector�ł��������ǂ�������������Ȃ�)
*/
class CAOIListViewCtrl final: boost::noncopyable
{
public:
	/**
	* @brief �e�v�f�ɃJ���������i�[����f�[�^�^
	*/
	typedef std::list<CString> COLUMN_NAMES;

	/**
	* @brief ����������
	* @warning �����̊J�n���� ��x�����K���Ăяo������
	* @param[in] hwnd    �E�B���h�E�n���h��
	* @param[in] id      ���̃C���X�^���X�ŊǗ����郊�X�g�r���[��ID
	* @param[in] columns �e��̃J���������i�[�������X�g
	* @param[in] option  ���X�g�r���[�ɐݒ肷��I�v�V����
	*/
	void Init( HWND hwnd, UINT id, const COLUMN_NAMES& columns, DWORD option );

	/**
	* @brief ���X�g�r���[�̕\�����N���A����
	* @note �����ɕێ����Ă���f�[�^�͏�������Ȃ�
	*/
	void Clear();

	/**
	* @brief ���X�g�r���[��̕\��������ŕێ����Ă���f�[�^�ɍ��킹��
	* @warning �ҏW�ΏۂƂȂ�t�B�[�`���[���؂�ւ������K���Ăяo������
	* @param[in] lOID  �ҏW�Ώۃt�B�[�`���[��OBJECTID
	* @retval true  ����
	* @retval false �����I�ɕێ����Ă���f�[�^����ł��������߁A�\���ł��Ȃ�����
	*/
	bool Review( long lOID );

	/**
	* @brief ���X�g�r���[�̕\�����N���A���A�����ŕێ����Ă���f�[�^���S�ď���
	*/
	void DeleteAll();

	/**
	* @brief ���X�g�r���[�̕\�����N���A���A�����ŕێ�����Ă���f�[�^������
	* @param[in] lOID �f�[�^�������I�u�W�F�N�gID
	*/
	void Delete( long lOID );

	/**
	* @brief �w��s�̃`�F�b�N�{�b�N�X���`�F�b�N����Ă��邩
	* @param[in] lineCount �`�F�b�N����Ă��邩���肷��s�ԍ�
	* @retval true  �`�F�b�N����Ă���
	* @retval false �`�F�b�N����Ă��Ȃ�
	*/
	bool IsChecked( int lineCount ) const;

	/**
	* @brief ���X�g�r���[�ŕ\������Ă���s�̐����擾
	* @return �s��
	*/
	int GetLineCount() const;

	/**
	* @brief ���X�g�r���[�ŕ\������Ă���f�[�^�����邩����
	* @retval true  �\������Ă���f�[�^�͂Ȃ�
	* @retval false �\������Ă���f�[�^������
	*/
	bool IsEmpty() const;

	/*
	* @brief ���X�g�r���[��̎w��J�����Ɏw��̒l�����݂��邩�ۂ�
	* @param[in]   value      �w��̒l
	* @param[in]   columnName �l�̑��݂��m�F����J������
	* @retval true  �l�����݂���
	* @retval false �l�͑��݂��Ȃ�
	*/
	bool IsExist( const CString& value, const CString& columnName ) const;

	/**
	* @brief ���X�g�r���[�Ɉ�s���̃f�[�^��ǉ�����
	* @note  �ǉ����ꂽ�f�[�^������I�ێ�����
	* @param[in] lList �ǉ�����f�[�^�̃��X�g�ilist��size()���J�������j
	*/
	void AddItem( const AOI_LINE& lList );

	/*
	* @brief �w��J�����̃��X�g�r���[��ł̃C���f�b�N�X�ԍ����擾
	* @param[in]   columnName �C���f�b�N�X�ԍ����擾����J������
	* @retval 0�ȏ� �w��J�����̃��X�g�r���[��ł̃C���f�b�N�X�ԍ�
	* @retval -1    �w��J�����������J���������݂��Ȃ�
	*/
	int GetColumnIndex( const CString& columnName ) const;

	/**
	* @brief �w��s�̎w��J�����̒l���擾����
	* @param[in] lineIndex  �l���擾����s�̃C���f�b�N�X�ԍ�
	* @param[in] columnName �w��J������
	* @return    strValue   �w��s�̎w��J�����̒l
	*/
	CString GetValue( unsigned int lineIndex , const CString& columnName) const;

	/**
	* @brief �e�s�̎w��J�����̒l�����X�g���������̂�Ԃ�
	* @param[in] columnName  �w��J������
	* @param[in] isCheckOnly �`�F�b�N�{�b�N�X���`�F�b�N����Ă�����̂�����Ԃ����ۂ�
	*/
	AOI_COLUMN MakeTargetColumnList(const CString& columnName, bool isCheckOnly = false) const;

	/**
	* @brief �w��s�̃`�F�b�N��ύX����
	* @param[in] lineIndex �w��s
	* @param[in] checkOn   TRUE�Ȃ�`�F�b�N������AFALSE�Ȃ�`�F�b�N�����Ȃ�
	*/
	void ChangeChecked( unsigned int lineIndex, BOOL checkOn );

	/**
	* @brief �w��J�����̒l��list���ɑ��݂���Ȃ�A���̍s�Ƀ`�F�b�N������
	* @param[in] columnName �w��J������
	* @param[in] lList      �`�F�b�N������l�̃��X�g
	*/
	void SetCheckedOn( const CString& columnName, const AOI_COLUMN& lList );

	/**
	* @brief �w��J�����̒l��list���ɑ��݂���s���폜����
	* @note  �����f�[�^���폜����
	* @param[in] columnName �w��J������
	* @param[in] lList      �폜����f�[�^�̃��X�g
	* @retval true  �폜���Œ���A�s��ꂽ
	* @retval false ��x���폜���s���Ȃ�����
	*/
	bool DeleteItem( const CString& columnName, const AOI_COLUMN& lList );

	/*
	* @brief ���X�g�r���[��̎w��J������list���̒l�����݂��邩�ۂ�
	* @param[in]   values     ���݂𔻒肷��l���i�[�������X�g
	* @param[in]   columnName �l�̑��݂��m�F����J������
	* @retval true  ���X�g���̂����ꂩ�̒l�����݂���
	* @retval false ���X�g���̂�����̒l�����݂��Ȃ�
	*/
	bool IsExist( const AOI_COLUMN& values, const CString& columnName ) const;

	/*
	* @brief ���X�g�r���[��̎w��J������list���̒l�����݂��Ȃ��s���擾
	* @param[in]   values          ���̃��X�g���ɑ��݂���l�����s�͎擾���Ȃ�
	* @param[in]   eraseNonChecked �`�F�b�N�{�b�N�X���`�F�b�N����Ă��Ȃ����̂������Ȃ�true��n��
	* @param[in]   columnName      �l�̑��݂��m�F����J������
	* @return retList  ���X�g���̂�����̒l�����݂��Ȃ��s�̃��X�g
	*/
	AOI_MATRIX GetNonExistValue( const AOI_COLUMN& values, bool eraseNonChecked , const CString& columnName ) const;

	/// OBJECTID�Ɋ֘A�t�����X�g�f�[�^���擾
	std::map< long, AOI_MATRIX >& getRelationData() { return m_OID2Values; }

private:
	/**
	* @brief ���X�g�r���[�Ɉ�s���̃f�[�^��ǉ�����
	* @note  �ǉ����ꂽ�f�[�^������I�ɕێ����邱�Ƃ͂Ȃ�
	* @param[in] lList �ǉ�����f�[�^�̃��X�g�ilist��size()���J�������j
	*/
	void addData( const AOI_LINE& lList );

	CListViewCtrl m_lvCtrl; //!< ���̃C���X�^���X�ŊǗ����郊�X�g�r���[
	long m_OID;    //!< m_ViewID�ɑΉ��t��OBJECTID
	std::map< long, AOI_MATRIX > m_OID2Values; //!< ID�ɑ΂��ĕێ�����Ă���l���i�[
};
