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
 * @file MovableLayersTable.h
 * @brief <b>CMovableLayersTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _MOVABLELAYERSTABLE_H_
#define _MOVABLELAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "MovableLayers.h"

class CListString;
namespace sindy {
namespace movablelayers {
/**
 * @class CMovableLayersTable
 * @brief <b>�ړ��\���C���Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.MOVABLELAYERS�e�[�u���̃��b�v�N���X�ł��B
 */
class SINDYLIB_API CMovableLayersTable : public CTable  
{
public:
	CMovableLayersTable() : m_bCached(false) {}
	virtual ~CMovableLayersTable() {}


	CMovableLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CMovableLayersTable& operator=(const CMovableLayersTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>��ƖړI�ԍ��ƃ��C�����ɑΉ����郌�R�[�h��Ԃ�</b>\n
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const CMovableLayers* Find( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>�ړ��\���C���̃��X�g��Ԃ�</b>\n
	 * �i�[�R���e�i�̓N���A���܂���̂ŁA�����̃A�C�e��������ꍇ�A���̂܂�
	 * �ǉ�����܂��B�܂��A�d�����l�����܂���̂ŁA�l������ꍇ�� sort�Aunique
	 * ���Ă��������B
	 * @param lPurposeID	[in]		��ƖړIID
	 * @param listLayerName	[out]		�ړ��\���C�����X�g
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetMovableLayers( long lPurposeID, CListString& listLayerName ) const;

	/**
	 * @brief <b>�ړ��\���C�����ǂ����`�F�b�N����</b>\n
	 * @warning ���̊֐��ł́A�����ɂ�鐧���̓`�F�b�N�ΏۊO�ł��B
	 * �ʏ��IsMovable���g�p���Ă��������B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	�`�F�b�N�Ώۂ̃��C��ID
	 * @retval true �ړ��\
	 * @retval false �ړ��s�\
	 */
	bool IsMovableLayer( long lPurposeID, long lLayerNameID ) const { return ( Find( lPurposeID, lLayerNameID ) ) ? true : false; }

	/**
	 * @brief <b>�n���ꂽ�I�u�W�F�N�g���ړ��\���ǂ������`�F�b�N����</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	lpszMessage���[�\������
	 * @param lpszMessage	[out]	�`�F�b�N���ʃ��b�Z�[�W
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsMovable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

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

/*	//@{ @name �C�e���[�^
	typedef std::map<long, std::map<long, CMovableLayers> >::iterator iterator;
	typedef std::map<long, std::map<long, CMovableLayers> >::const_iterator const_iterator;
	iterator begin() { return m_mapCache.begin(); }
	iterator end() { return m_mapCache.end(); }
	const_iterator begin() const { return m_mapCache.begin(); }
	const_iterator end() const { return m_mapCache.end(); }
	//@}*/
private:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::map<long, std::map<long, CMovableLayers> >		m_mapCache;	//!< �L���b�V���p���X�g
#pragma warning(pop)
	bool												m_bCached;	//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
} // editmesh

} // sindy

#endif // _MOVABLELAYERSTABLE_H_
