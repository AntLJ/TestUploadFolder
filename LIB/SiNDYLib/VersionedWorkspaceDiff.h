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
 * @file VersionedWorkspaceDiff.h
 * @brief CWorkspace�N���X��`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _VERSIONEDWORKSPACEDIFF_H_
#define _VERSIONEDWORKSPACEDIFF_H_

#include "Workspace.h"
#include "ErrorCode.h"
#include <map>
#include <list>

namespace sindy {
class CRow;
class SINDYLIB_API CVersionedWorkspaceDiff : public CWorkspace
{
public:
	/**
	 * @enum sindyDifferenceType
	 * @brief <b>esriDifferenceType�𕡐��w��ł���悤�i���o�����O������������</b>\n
	 * Differences�̈����Ƃ��Ďg�p���܂��B
	 */
	enum sindyDifferenceType {
		sindyDifferenceTypeInsert = 1,
		sindyDifferenceTypeDeleteNoChange = 2,
		sindyDifferenceTypeUpdateNoChange = 4,
		sindyDifferenceTypeUpdateUpdate = 8,
		sindyDifferenceTypeUpdateDelete = 16,
		sindyDifferenceTypeDeleteUpdate = 32,
	};

	explicit CVersionedWorkspaceDiff();
	virtual ~CVersionedWorkspaceDiff();

	explicit CVersionedWorkspaceDiff( IWorkspace* lp )
	{
		*this = lp;
	}
	explicit CVersionedWorkspaceDiff( LPCTSTR lpcszConnectString )
	{
		Connect( lpcszConnectString );
	}
	CVersionedWorkspaceDiff& operator=( IWorkspace* lp )
	{
		AtlComPtrAssign( (IUnknown**)&p, lp );
		m_spNameString.reset( (CWorkspaceNameString*)NULL );
		clear();

		return *this;
	}

	/**
	 * @brief <b>���[�N�X�y�[�X�Ԃł̍������擾����</b>\n
	 * @param cWork			[in]			��r���[�N�X�y�[�X
	 * @param mapDiff		[out]			�擾���ꂽ����
	 * @param ipProgress	[in,optional]	�i���m�F�v���O���X�i�f�t�H���g�FNULL�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Differences( IFeatureWorkspace* ipWork, sindyDifferenceType emDiffType, IProgressor* ipProgress = NULL );

	/**
	 * @brief <b>�擾�����ɃR���t���N�g���܂܂�邩�ǂ������`�F�b�N����</b>\n
	 * @retval true �R���t���N�g������
	 * @retval false �R���t���N�g�Ȃ�
	 */
	bool HasConflict() const;

	typedef std::list<CRow> CRowList;
	typedef std::map<esriDifferenceType,CRowList> CDiffRowMap;
	//@{ @name �C�e���[�^
	typedef CRowList::const_iterator const_iterator;
	const_iterator end( esriDifferenceType emType ) const;
	const_iterator begin( esriDifferenceType emType ) const;
	//@}
	void clear();
private:
	errorcode::sindyErrCode Differences( IEnumDatasetName* ipDatasetNames, IFeatureWorkspace* ipWork, sindyDifferenceType emDiffType, IProgressor* ipProgress = NULL );
	long DiffCur2Map( esriDifferenceType emType, IDifferenceCursor* ipCursor, IFields* ipFields );
private:
#pragma warning(push)
#pragma warning(disable:4251)
	CDiffRowMap m_mapDiff;	// �����i�[�p
#pragma warning(pop)
};

} // sindy

#endif // ifndef _VERSIONEDWORKSPACEDIFF_H_
