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

#define FN_LONGITUDE	_T("LONGITUDE")
#define FN_LATITUDE		_T("LATITUDE")
#define FN_CITYMESH		_T("CITYMESH")
#define FN_BASEMESH		_T("BASEMESH")
#define FN_MIDDLEMESH	_T("MIDDLEMESH")
#define FN_MESHX		_T("MESHX")
#define FN_MESHY		_T("MESHY")
#define FN_POINTS		_T("POINTS")	// [Bug 5691]
#define FN_ISLINEAR		_T("ISLINEAR")	// [Bug 5691]
#define FN_LENGTH		_T("LENGTH")	// [Bug 7086]
#define FN_AREA			_T("AREA")		// [Bug 7086]
#define FN_DUMMY		_T("DUMMY")		// Ver0.8���

/**
 * @class	CFieldInfo
 * @brief	�o�̓t�B�[���h���
 */
class CFieldInfo
{
public:
	CFieldInfo( const CString& strName, LONG lFID, bool bDomain = false, bool bShapeField = false, bool bDummy = false, CString strDummyVal = _T("") )
		: m_strName(strName), m_lFID(lFID), m_bDomain(bDomain), m_bShapeField(bShapeField), m_bDummy(bDummy), m_strDummyVal(strDummyVal)
	{
	}

	CString	m_strName;		//!< �t�B�[���h��
	LONG	m_lFID;			//!< �t�B�[���h�C���f�b�N�X
	bool	m_bDomain;		//!< �h���C�����o�́H
	bool	m_bShapeField;	//!< �ʒu���t�B�[���h�H
	bool	m_bDummy;		//!< �_�~�[�t�B�[���h�H(Ver0.8���)
	CString m_strDummyVal;	//!< �_�~�[�t�B�[���h�o�͏��(Ver0.8���)
};

typedef std::list<CFieldInfo>	FIELDLIST;

/**
 * @class	CInitFile
 * @brief	�ݒ�t�@�C���Ǘ��N���X
 */
class CInitFile
{
public:
	CInitFile(void) : m_nShapeFieldCount(0), m_nMeshLevel(2){};
	~CInitFile(void){};

	/**
	 * @brief	������
	 * @param	lpcszFile	[in]	�t�@�C����
	 * @param	ipTable		[in]	�e�[�u��
	 * @param	bShape		[in]	�e�[�u���ɐ}�`���܂ނ��ǂ����H
	 * @param	bSkip		[in]	���ڂ������ꍇ�G���[�Ƃ��Ȃ�(�X�L�b�v���ďo�͂���)
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool init( LPCTSTR lpcszFile, const ITablePtr& ipTable, const bool& bShape, const bool& bSkip );

	/**
	 * @brief	�t�B�[���h���X�g�擾
	 * @return	�t�B�[���h���X�g
	 */
	const FIELDLIST& getFieldList(){ return m_listField; }

	/**
	 * @brief	�ʒu���t�B�[���h�w�萔
	 * @return	�ʒu���t�B�[���h��
	 */
	int getShapeFieldCount() const{ return m_nShapeFieldCount; }

	/**
	 * @brief	���b�V�����x���擾
	 * @return	���b�V�����x��
	 */
	int getMeshLevel() const{ return m_nMeshLevel; }

private:

	/**
	 * @brief	�ʒu���Ɋ֘A����t�B�[���h�H
	 * @param	strField	[in]	�t�B�[���h��
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool isShapeField( const CString& strField )
	{
		static LPCTSTR g_lpcszShapeField[] = { FN_ISLINEAR, FN_LONGITUDE, FN_LATITUDE, 
											   FN_CITYMESH, FN_BASEMESH, FN_MIDDLEMESH, FN_MESHX, FN_MESHY, FN_POINTS, 
											   FN_LENGTH, FN_AREA, NULL };
		for( int i=0; g_lpcszShapeField[i] != NULL; ++i )
		{
			if( strField == g_lpcszShapeField[i] )
				return true;
		}
		return false;
	}

private:
	
	FIELDLIST	m_listField;			//!< �t�B�[���h���X�g

	int			m_nShapeFieldCount;		//<! �ʒu���t�B�[���h��
	int			m_nMeshLevel;			//!< ���b�V��XY�o�͂���ۂɃ��b�V�����x��(crd_cnv�ɂȂ����1,2,64)
};
