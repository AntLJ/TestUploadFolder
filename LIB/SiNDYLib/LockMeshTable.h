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
 * @file LockMeshTable.h
 * @brief <b>CLockMeshTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _LOCKMESHTABLE_H_
#define _LOCKMESHTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "LockMesh.h"
#include <list>

namespace sindy {
namespace lockmesh {
/**
 * @class CLockMeshTable
 * @brief <b>�ҏW���b�N���b�V���Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.LOCKMESH�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CLockMeshTable : public CTable  
{
public:
	CLockMeshTable() : m_bCached(false) {}
	virtual ~CLockMeshTable() {}

	CLockMeshTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CLockMeshTable& operator=(const CLockMeshTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>���b�N����Ă��邩�ǂ������`�F�b�N����</b>
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lConnUserID	[in]	�ڑ����[�U��ID
	 * @param lLayerNameID	[in]	���C����ID
	 * @param lCode			[in]	���b�V���R�[�h
	 * @retval true ���b�N����Ă���
	 * @retval false ���b�N����Ă��Ȃ�
	 */
	bool IsLocked( long lConnUserID, long lLayerNameID, long lCode ) const;

	/**
	 * @brief <b>�e�[�u����S�ăL���b�V������</b>\n
	 * �e�[�u���̓��e��S�ăL���b�V�����܂��B
	 * bDetatch��true�ɂ���ƁA�ǂݍ��񂾃��R�[�h�y��
	 * �e�[�u����DB�ւ̐ڑ���؂藣���܂��B
	 */
	long CreateTableCache( bool bDetatch = false );

	/**
	 * @brief <b>�L���b�V�����N���A����</b>\n
	 */
	void ClearTableCache();

	/**
	 * @brief <b>���ɃL���b�V������Ă��邩�ǂ���</b>\n
	 * @return �L���b�V������Ă����true
	 */
	bool IsCached() const { return m_bCached; }
private:
	std::map<long, std::map<long, std::list<CLockMesh> > >	m_mapCache;	//!< �L���b�V���p���X�g
	bool													m_bCached;	//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // lockmesh

} // sindy

#endif // _LOCKMESHTABLE_H_
