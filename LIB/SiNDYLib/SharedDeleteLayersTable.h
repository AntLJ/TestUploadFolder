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
 * @file SharedDeleteLayersTable.h
 * @brief <b>CSharedDeleteLayersTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _SHAREDDELETELAYERSTABLE_H_
#define _SHAREDDELETELAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "SharedDeleteLayers.h"
#include <list>

class CListLong;
namespace sindy {
namespace shareddeletelayers {
/**
 * @class CSharedDeleteLayersTable
 * @brief <b>���L�폜�Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.SHAREDDELETELAYERS�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CSharedDeleteLayersTable : public CTable
{
public:
	CSharedDeleteLayersTable() : m_bCached(false) {}
	virtual ~CSharedDeleteLayersTable() {}

	CSharedDeleteLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CSharedDeleteLayersTable& operator=(const CSharedDeleteLayersTable& lp )
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
//	const CSharedDeleteLayers* Find( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>�n���ꂽ���C����ID�ɑΉ����鋤�L�폜�Ώۃ��C����ID��Ԃ�</b>\n
	 * @param lLayerNameID		[in]	���C����ID
	 * @param listLayerNameID	[out]	���C����ID���i�[���郊�X�g
	 * @retval 0�ȏ� ���L�폜�Ώۃ��C����
	 * @retval -1 �G���[
	 */
	long GetSharedDeleteLayers( long lLayerNameID, CListLong& listLayerNameID ) const;

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
	std::list<CSharedDeleteLayers>	m_listCache;	//!< �L���b�V���p���X�g
	bool							m_bCached;		//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // shareddeletelayers

} // sindy

#endif // _SHAREDDELETELAYERSTABLE_H_
