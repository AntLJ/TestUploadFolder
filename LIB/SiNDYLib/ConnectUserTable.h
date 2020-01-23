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
 * @file ConnectUserTable.h
 * @brief <b>CConnectUserTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _CONNECTUSERTABLE_H_
#define _CONNECTUSERTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "ConnectUser.h"

namespace sindy {
namespace connectuser {
/**
 * @class CConectUserTable
 * @brief <b>�ڑ����[�U���Ǘ��e�[�u���N���X</b>\n
 * ���̃N���X��REFERENCE.CONNECTUSER�e�[�u���̃��b�v�N���X�ł��B
 */
class CConnectUserTable : public CTable  
{
public:
	CConnectUserTable() : m_bCached(false) {}
	virtual ~CConnectUserTable(){}

	CConnectUserTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CConnectUserTable& operator=(const CConnectUserTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>�e�[�u����S�ăL���b�V������</b>\n
	 * �e�[�u���̓��e��S�ăL���b�V�����܂��B
	 * bDetatch��true�ɂ���ƁA�ǂݍ��񂾃��R�[�h�y��
	 * �e�[�u����DB�ւ̐ڑ���؂藣���܂��B
	 */
	long CreateTableCache( bool bDetatch = false );

	/**
	 * @brief <b>�ڑ����[�U���ɑΉ�����ID��Ԃ�</b>\n
	 * @param lpcszConnUser	[in]	�ڑ����[�U��
	 * @retval 0�ȏ� �ڑ����[�U��ID
	 * @retval -1 �o�^����Ă��Ȃ�
	 */
	long GetConnectUserID( LPCTSTR lpcszConnUser ) const;

	/**
	 * @brief <b>���ɃL���b�V������Ă��邩�ǂ���</b>\n
	 * @return �L���b�V������Ă����true
	 */
	bool IsCached() const { return m_bCached; }
	/**
	 * @brief <b>�L���b�V�����N���A����</b>\n
	 */
	void ClearTableCache();
private:
	std::list<CConnectUser>		m_listCache;	//!< �L���b�V���p���X�g
	bool						m_bCached;		//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};

} // workpurpose

} // sindy

#endif // _CONNECTUSERTABLE_H_
