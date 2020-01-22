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
#include <atlbase.h>
#include <atlstr.h>
#include <vector>
#include <set>

class FileFinder
{
public:
	FileFinder(void);
	virtual ~FileFinder(void);

	/**
	 * @brief <b>�����̎��s</b>\n
	 * @param path [in] �������J�n����p�X
	 * @param pattern [in] �t�@�C�����̃p�^�[��
	 * @param subDirectories [in] true�̏ꍇ�̓T�u�f�B���N�g������������
	 * @return ����I������true, �G���[��������false
	 */
	virtual bool Search(LPCTSTR path, LPCTSTR pattern, bool subDirectories = true);

	/**
	 * @brief <b>���X�g�̃N���A</b>\n
	 */
	virtual void Clear();

	/**
	 * @brief <b>���o���ꂽ�t�@�C�����̎擾</b>\n
	 * @return ���o���ꂽ�t�@�C����
	 */
	virtual int GetCount() const;

	/**
	 * @brief <b>�t�@�C�����̎擾</b>\n
	 * @return �t�@�C����
	 */
	virtual ATL::CString GetFilePath(int index) const;
	virtual ATL::CString operator[](int index) const { return GetFilePath(index); }

	// ���O����f�B���N�g���̐ݒ�
	// ���O�ɐݒ肵�Ȃ���΂Ȃ�Ȃ�
	// �f�B���N�g�����݂̂̂�OK
//	virtual void PutExcludeDirectory(LPCTSTR lpDir);
//	virtual void ClearExcludeDirectory();

	void SetDisplayStatus(bool disp = true) { m_DisplayStatus = disp; }

protected:
	/**
	 * @brief <b>�t�H���_�̌���</b>\n
	 * @param path [in] �������J�n����p�X
	 */
	virtual bool SearchDirectories(LPCTSTR path, bool subDirectories, std::set<ATL::CString> &directories);

private:
	std::vector<ATL::CString> m_FileList; //!< ���o���ꂽ�t�@�C�����X�g
	std::set<ATL::CString> m_ExDir; // ���O�t�H���_

	bool m_DisplayStatus;

	friend class FileFinderTest;
};

