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
 * @file EditMeshTable.h
 * @brief <b>CEditMeshTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _EDITMESHTABLE_H_
#define _EDITMESHTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "EditMesh.h"
#include "Geometry.h"
#include <list>

namespace sindy {
class CGeometry;
namespace editmesh {
/**
 * @class CEditMeshTable
 * @brief <b>�ҏW�\���b�V���Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X�́AREFERNCE.EDITMESH�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CEditMeshTable : public CTable  
{
public:
	CEditMeshTable() : m_bCached(false) {}
	virtual ~CEditMeshTable() {}

	CEditMeshTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CEditMeshTable& operator=(const CEditMeshTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>�ҏW�\�ȃG���A���ǂ������`�F�b�N����</b>\n
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @param lOperatorID	[in]	��ƎҖ�ID
	 * @param ipGeom		[in]	�`�F�b�N�G���A
	 * @retval true �ҏW�\
	 * @retval false �ҏW�s��
	 */
	bool IsEditable( long lPurposeID, long lLayerNameID, long lOperatorID, IGeometry* ipGeom ) const;

	/**
	 * @brief <b>�e�[�u����S�ăL���b�V������</b>\n
	 * �e�[�u���̓��e��S�ăL���b�V�����܂��B
	 * bDetatch��true�ɂ���ƁA�ǂݍ��񂾃��R�[�h�y��
	 * �e�[�u����DB�ւ̐ڑ���؂藣���܂��B
	 *
	 * @note ����̍�ƖړI�����L���b�V���������ꍇ��lPurposeID�ɍ�ƖړIID���Z�b�g���܂��B
	 * �f�t�H���g�̏�Ԃł͑S��ƖړI���L���b�V�����܂��B
	 * 
	 * @param bDetatch [in,optional] �L���b�V��������ڑ���؂藣���i�f�t�H���g�Ffalse�j
	 * @param lPurposeID [in,optional] ��ƖړI�����肵�ăL���b�V�����쐬����i�f�t�H���g�F-1�j
	 */
	long CreateTableCache( bool bDetatch = false, long lPurposeID = -1 );

	/**
	 * @brief <b>�L���b�V�����N���A����</b>\n
	 */
	void ClearTableCache();

	/**
	 * @brief <b>���ɃL���b�V������Ă��邩�ǂ���</b>\n
	 * @return �L���b�V������Ă����true
	 */
	bool IsCached() const { return m_bCached; }

	/**
	 * @brief <b>��Ɖ\�G���A��ݒ肷��</b>\n
	 * @param ipGeom [in] �Z�b�g����G���A
	 * @note NULL���Z�b�g����Ƌ�ɂȂ�܂�
	 */
	void SetGlobalWorkArea( IGeometry* ipGeom );

	/**
	 * @brief <b>��Ɖ\�G���A���擾����</b>\n
	 * @return ��Ɖ\�G���A
	 */
	const CGeometry& GetGlobalWorkArea() const;

private:
	CGeometry m_cGlobalWorkArea; //!< �S�Ẵ��C���E��ƖړI�ɉۂ������ƃG���A�i�L���b�V���Ώۂł͂Ȃ��A��Ȃ琧���Ȃ��j
	std::map<long, std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > > >	m_mapCache;	//!< �L���b�V���p���X�g
	bool m_bCached;	//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // editmesh

} // sindy

#endif // _EDITMESHTABLE_H_
