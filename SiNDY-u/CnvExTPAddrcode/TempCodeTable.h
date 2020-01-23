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

// TempCodeTable.h: CTempCodeTable �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPCODETABLE_H__AAFCFA8F_BDA3_41E9_8E67_6222C77C4491__INCLUDED_)
#define AFX_TEMPCODETABLE_H__AAFCFA8F_BDA3_41E9_8E67_6222C77C4491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @class	CTempCodeTable
 * @brief	���R�[�h�Ή��\�N���X
 */
class CTempCodeTable  
{
public:
	CTempCodeTable() : m_bIsSet(false){};
	virtual ~CTempCodeTable(){};

	/**
	 * @brief	���R�[�h�Ή��\�t�@�C�����[�h
	 * @param	lpcszFile	[in]	�t�@�C����
	 * @return	bool
	 */
	bool load( LPCTSTR lpcszFile );

	/**
	 * @brief	�����ݒ���ɂ���Ă�H
	 * @return	bool
	 */
	bool isSetting() const { return m_bIsSet; }

	/**
	 * @brief	�w��R�[�h����u���ł���V�R�[�h����H
	 * @param	strAddrCode		[in]	�w��R�[�h
	 * @param	strNewAddrCode	[out]	�u�����ׂ��R�[�h
	 * @retval	true  : �u���R�[�h������
	 * @retval	false : �Ȃ�
	 */
	bool getTaiouCode( const CString& strAddrCode, CString& strNewAddrCode )
	{
		std::multimap<CString, CString>::const_iterator itr=m_mapTmpTable.find( strAddrCode );
		if( itr != m_mapTmpTable.end() )
		{
			strNewAddrCode = itr->second;
			return true;
		}
		else
			return false;
	}



///////////////////////////////
//	�����o�ϐ�
///////////////////////////////

private:

	bool	m_bIsSet;								//!< �Z�b�g����Ă���H

	std::multimap<CString, CString>	m_mapTmpTable;	//!< ���R�[�h�Ή��\

};

#endif // !defined(AFX_TEMPCODETABLE_H__AAFCFA8F_BDA3_41E9_8E67_6222C77C4491__INCLUDED_)
