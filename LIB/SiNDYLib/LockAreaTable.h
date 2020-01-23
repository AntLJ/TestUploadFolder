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
 * @file LockAreaTable.h
 * @brief <b>CLockAreaTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _LOCKAREATABLE_H_
#define _LOCKAREATABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "LockArea.h"

namespace sindy {
namespace lockarea {
/**
 * @class CLockArea
 * @brief </b>�ҏW���b�N�G���A�Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.LOCKAREA�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CLockAreaTable : public CTable  
{
public:
	CLockAreaTable() : m_bCached(false) {}
	virtual ~CLockAreaTable() {}

	CLockAreaTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CLockAreaTable& operator=(const CLockAreaTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}

	/**
	 * @brief <b>���b�N����Ă��邩�ǂ������`�F�b�N����</b>\n
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lConnUserID	[in]	�ڑ����[�U��ID
	 * @param lLayerNameID	[in]	���C����ID
	 * @param ipGeom		[in]	�`�F�b�N�G���A
	 * @retval true ���b�N����Ă���
	 * @retval false ���b�N����Ă��Ȃ�
	 */
	bool IsLocked( long lConnUserID, long lLayerNameID, IGeometry* ipGeom ) const;

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
	std::map<long, std::map<long, CLockArea> >			m_mapCache;	//!< �L���b�V���p���X�g
	bool												m_bCached;	//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // lockarea

} // sindy

#endif // _LOCKAREATABLE_H_
