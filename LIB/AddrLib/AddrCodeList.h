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

#pragma once

#include <set>
#include <SiNDYLib/FeatureClass.h>

namespace addr
{

using namespace sindy;

/**
 * @class	CAddrCodeList
 * @brief	�Z���R�[�h���X�g�Ǘ��N���X
 * @note	�s�s�n�}�s���E����Z���R�[�h�𐶐�����`�ł��B
 *			(�Z���R�[�h�𒼐ڎw��E�R�[�h���X�g�w�����)
 */
class CAddrCodeList : public CFeatureClass
{
public:

	explicit CAddrCodeList(void){}
	virtual ~CAddrCodeList(void){}

	CAddrCodeList( sindyTableType::ECode emTableType, ITable* lp ){ SetObject( emTableType, lp ); }
	CAddrCodeList( sindyTableType::ECode emTableType, IFeatureClass* lp ){ SetObject( emTableType, lp ); }
	CAddrCodeList( ITable* lp ){ SetObject( lp ); }
	CAddrCodeList( IFeatureClass* lp ){	SetObject( lp ); }

	/**
	 * @brief	<b>�S�����X�g�쐬<b>
	 * @note	�s�s�s���E���炷�ׂĂ̏Z���R�[�h���X�g�𐶐�
	 * @param	lLevel	[in]	�R�[�h���x��[1-11]
	 * @return	bool
	 */
	bool MakeCodeListAll( long lLevel );

	/**
	 * @brief	<b>�R�[�h�͈͎w��</b>
	 * @note	�w�肵���͈͂ŁA�s�s�n�}�s���E�ɑ��݂���Z���R�[�h���X�g�𐶐�
	 * @param	lpcszStartRange	[in]	�J�n�R�[�h
	 * @param	lpcszEndRange	[in]	�I���R�[�h
	 * @return	bool
	 */
	bool MakeCodeListFromRange( const _TCHAR* lpcszStartRange, const _TCHAR* lpcszEndRange );

	/**
	 * @brief	<b>�R�[�h���X�g�t�@�C������ǉ�</b>
	 * @param	lpcszFile	[in]	�R�[�h���X�g
	 * @param	lLevel		[in]	�����ڂ܂œǂݍ��݁H[1-11](�f�t�H���g0 : 1�s�����̂܂ܓǂݍ���)
	 * @return	bool
	 */
	bool AddFromFile( const _TCHAR* lpcszFile, long lLevel = 0 );

	/**
	 * @brief	<b>�R�[�h�ǉ�</b>
	 * @param	lpcszCode	[in]	�Z���R�[�h
	 */
	void Add( const _TCHAR* lpcszCode ){ m_listCode.insert(lpcszCode); }

	typedef std::set<CString>	CodeList;
	typedef CodeList::iterator iterator;
	typedef CodeList::const_iterator const_iterator;

	iterator begin(){ return m_listCode.begin(); }
	iterator end(){ return m_listCode.end(); }
	size_t size(){ return m_listCode.size(); }
	void clear(){ return m_listCode.clear(); }

private:

	/**
	 * @brief	<b>�N�G������ǉ�</b>
	 * @param	lpcszWhere		[in]	Where��
	 * @param	lLevel			[in]	�Z���R�[�h��[1-11]
	 * @return	bool
	 */
	bool AddFromQuery( const _TCHAR* lpcszWhere, long lLevel );

private:

	CodeList	m_listCode;	//!< �Z���R�[�h���X�g�m�F�p

};

}	// namespace addr
