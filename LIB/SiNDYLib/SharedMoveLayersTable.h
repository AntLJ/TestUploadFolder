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
 * @file SharedMoveLayersTable.h
 * @brief <b>CSharedMoveLayersTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _SHAREDMOVELAYERSTABLE_H_
#define _SHAREDMOVELAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "SharedMoveLayers.h"
#include <list>

class CListLong;
namespace sindy {
namespace sharedmovelayers {
/**
 * @class CSharedMoveLayersTable
 * @brief <b>���L�ړ��Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.SHAREDMOVELAYERS�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CSharedMoveLayersTable : public CTable
{
public:
	CSharedMoveLayersTable() : m_bCached(false) {}
	virtual ~CSharedMoveLayersTable() {}

	CSharedMoveLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CSharedMoveLayersTable& operator=(const CSharedMoveLayersTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>��ƖړI�ԍ��ƃ��C�����ɑΉ����郌�R�[�h��Ԃ�</b>
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
//	const CSharedMoveLayers* Find( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>�n���ꂽ���C����ID�ɑΉ����鋤�L�ړ��Ώۃ��C����ID��Ԃ�</b>\n
	 * @param lLayerNameID		[in]	���C����ID
	 * @param listLayerNameID	[out]	���C����ID���i�[���郊�X�g
	 * @retval 0�ȏ� ���L�ړ��Ώۃ��C����
	 * @retval -1 �G���[
	 */
	long GetSharedMoveLayers( long lLayerNameID, CListLong& listLayerNameID ) const;

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
	std::list<CSharedMoveLayers>	m_listCache;	//!< �L���b�V���p���X�g
	bool							m_bCached;		//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // sharedmovelayers

} // sindy

#endif // _SHAREDMOVELAYERSTABLE_H_
