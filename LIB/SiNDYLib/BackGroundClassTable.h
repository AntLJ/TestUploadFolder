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
 * @file BackGroundClassTable.h
 * @brief <b>CBackGroundClassTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _BACKGROUNDCLASSTABLE_H_
#define _BACKGROUNDCLASSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "BackgroundClass.h"
#include <list>

namespace sindy {
namespace background_class {
/**
 * @class CBackGroundClassTable
 * @brief <b>�w�i��ʊǗ��e�[�u���N���X</b>\n
 * ���̃N���X��REFERENCE.BACKGROUNDCLASS�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CBackGroundClassTable : public CTable  
{
public:
	CBackGroundClassTable() : m_bCached(false) {}
	virtual ~CBackGroundClassTable() {}

	CBackGroundClassTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CBackGroundClassTable& operator=(const CBackGroundClassTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>���L��ʂŌ������s��</b>
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param emAnnoLayer	[in]	�^�ʒu���C���^�C�v
	 * @param emCode		[in]	���L���
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const CBackGroundClass* GetRowByAnnoCode( /*sindyAnnotationLayer emAnnoLayer, annotation_code::ECode emCode*/ ) const;

	/**
	 * @brief <b>�e�[�u����S�ăL���b�V������</b>\n
	 * �e�[�u���̓��e��S�ăL���b�V�����܂��B
	 * bDetatch��true�ɂ���ƁA�ǂݍ��񂾃��R�[�h�y��
	 * �e�[�u����DB�ւ̐ڑ���؂藣���܂��B
	 */
	long CreateTableCache( bool bDetatch = false );

	/**
	 * @brief <b>���ɃL���b�V������Ă��邩�ǂ���</b>\n
	 * @return �L���b�V������Ă����true
	 */
	bool IsCached() const { return m_bCached; }
	/**
	 * @brief <b>�L���b�V�����N���A����</b>\n
	 */
	void ClearTableCache();
	//@{ @name �L���b�V���R���e�i�Q�ƌn��`
	typedef std::list<CBackGroundClass>::const_iterator const_iterator;
	const_iterator begin() const { return m_listCache.begin(); }
	const_iterator end() const { return m_listCache.end(); }
	//@}

private:
//	std::map<annotation_code::ECode,CBackGroundClass>	m_mapCache;		//!< �L���b�V���p���X�g�i�L�[�͔w�i��ʁj
	std::list<CBackGroundClass>	m_listCache;	//!< �L���b�V���p���X�g
	bool												m_bCached;		//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};

} // background_class

} // sindy

#endif // _BACKGROUNDCLASSTABLE_H_
