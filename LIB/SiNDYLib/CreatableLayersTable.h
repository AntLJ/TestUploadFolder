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
 * @file CreatableLayersTable.h
 * @brief <b>CCreatableLayersTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _CREATABLELAYERSTABLE_H_
#define _CREATABLELAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "CreatableLayers.h"
//#include "RuleDef.h"

class CListString;
namespace sindy {
namespace creatablelayers {
/**
 * @class CCreatableLayersTable
 * @brief <b>�쐬�\���C���Ǘ��e�[�u���N���X</b>\n
 * ���̃N���X��REFERNCE.CREATABLELAYERS�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CCreatableLayersTable : public CTable  
{
public:
	CCreatableLayersTable() : m_bCached(false) {}
	virtual ~CCreatableLayersTable() {}

	CCreatableLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CCreatableLayersTable& operator=(const CCreatableLayersTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>��ƖړI�ԍ��ƃ��C�����ɑΉ����郌�R�[�h���L���b�V������T���ĕԂ�</b>
	 * �L���b�V������Ă�����̂��猟�����s���܂��̂ŁA
	 * �L���b�V�����쐬����Ă��邱�Ƃ��O��ł��B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const CCreatableLayers* Find( long lPurposeID, long lLayerNameID ) const;
	CCreatableLayers* Find( long lPurposeID, long lLayerNameID );

	/**
	 * @brief <b>��ƖړI�ԍ��ƃ��C�����ɑΉ����郌�R�[�h���f�[�^�x�[�X����T���ĕԂ�</b>\n
	 * �L���b�V���������Ă��L���b�V�����g�p�����Ƀf�[�^�x�[�X��
	 * ���ږ₢���킹���s���܂��B\n
	 * �������݂���ꍇ�ł���ɍŏ��Ɍ����������̂̂ݕԂ��܂��B
	 * ���̍ہA�g���[�X���O�ɂ��̎|�\������܂��B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	_IRowPtr CCreatableLayersTable::GetRow( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>�쐬�\���C���̈ꗗ��Ԃ�</b>\n
	 * �i�[�R���e�i�̓N���A���܂���̂ŁA�����̃A�C�e��������ꍇ�A���̂܂�
	 * �ǉ�����܂��B�܂��A�d�����l�����܂���̂ŁA�l������ꍇ�� sort�Aunique
	 * ���Ă��������B
	 * @param lPurposeID	[in]		��ƖړIID
	 * @param listLayerName	[out]		�쐬�\���C�����X�g
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetCreatableLayers( long lPurposeID, CListString& listLayerName ) const;

	/**
	 * @brief <b>�쐬�\���C�����ǂ����`�F�b�N����</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	�`�F�b�N�Ώۂ̃��C��ID
	 * @retval true �쐬�\
	 * @retval false �쐬�s�\
	 */
	bool IsCreatableLayer( long lPurposeID, long lLayerNameID ) const { return ( Find( lPurposeID, lLayerNameID ) ) ? true : false; }

	/**
	 * @brief <b>�n���ꂽ�I�u�W�F�N�g���쐬�\���ǂ������`�F�b�N����</b>
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	lpszMessage���[�\������
	 * @param lpszMessage	[out]	�`�F�b�N���ʃ��b�Z�[�W
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsCreatable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

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
	 * @brief <b>���ɃL���b�V������Ă��邩�ǂ���</b>\n
	 * @return �L���b�V������Ă����true
	 */
	bool IsCached() const { return m_bCached; }
	/**
	 * @brief <b>�L���b�V�����N���A����</b>\n
	 */
	void ClearTableCache();

	//@{ @name �ҏW�n�֐�
	/**
	 * @brief <b>���C����ǉ�����</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C��ID
	 * @return �Y�����R�[�h�����ɂ���ꍇ���͎��s�̏ꍇ��NULL���A�����ꍇ�͐V�K�쐬���Ă��̃��R�[�h��Ԃ�
	 */
	CCreatableLayers* AddLayer( long lPurposeID, long lLayerNameID );

	/**
	 * @brief <b>���C�����폜����</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C��ID
	 * sindyErrCode
	 */
	errorcode::sindyErrCode DeleteLayer( long lPurposeID, long lLayerNameID );
	//@}

	//@{ @name �R���e�i����n�֐�
	typedef std::map<long, CCreatableLayers> CCreatableLayersList;
	typedef std::map<long, CCreatableLayersList> CCreatableLayersLists;
	CCreatableLayersList::const_iterator by_purpose_begin(long lPurposeID) const {
		CCreatableLayersLists::const_iterator it = m_mapCache.find( lPurposeID );
		if( it != m_mapCache.end() )
			return it->second.begin();
		else
			return CCreatableLayersList::const_iterator();
	}
	CCreatableLayersList::const_iterator by_purpose_end(long lPurposeID) const {
		CCreatableLayersLists::const_iterator it = m_mapCache.find( lPurposeID );
		if( it != m_mapCache.end() )
			return it->second.end();
		else
			return CCreatableLayersList::const_iterator();
	}
	//@}
private:
	CCreatableLayersLists	m_mapCache;	//!< �L���b�V���p���X�g
	bool					m_bCached;	//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // creatablelayers

} // sindy

#endif // _CREATABLELAYERSTABLE_H_
