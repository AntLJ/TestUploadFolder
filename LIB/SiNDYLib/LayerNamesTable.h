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
 * @file LayerNamesTable.h
 * @brief <b>CLayerNamesTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _LAYERNAMESTABLE_H_
#define _LAYERNAMESTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "LayerNames.h"
#include <list>

namespace sindy {
namespace layernames {
/**
 * @class CLayerNamesTable
 * @brief <b>���C�����̊Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.LAYERNAMES�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CLayerNamesTable : public CTable  
{
public:
	CLayerNamesTable() : m_bCached(false) {}
	virtual ~CLayerNamesTable() {}

	CLayerNamesTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CLayerNamesTable& operator=(const CLayerNamesTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>���C����ID���烌�C�������擾����</b>\n
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lLayerNameID	[in]	���C����ID
	 * @param nLen			[in]	�o�b�t�@���[�\������
	 * @param lpszName		[out]	�i�[�o�b�t�@
	 * @retval LPCTSTR lpszName
	 * @retval NULL �G���[���͑Ή�������̂����݂��Ȃ�
	 */
	LPCTSTR Find( long lLayerNameID, int nLen, LPTSTR lpszName ) const;

	/**
	 * @brief <b>���C�����ɑΉ����郌�C����ID���擾����</b>
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lpcszLayerName	[in]	���C����
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �o�^����Ă��Ȃ�
	 */
	long Find( LPCTSTR lpcszLayerName ) const;

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
	std::list<CLayerNames>	m_listCache;	//!< �L���b�V���p���X�g
	bool					m_bCached;		//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // layernames

} // sindy

#endif // _LAYERNAMESTABLE_H_
