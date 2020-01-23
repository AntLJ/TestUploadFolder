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
 * @file ErrorObject.h
 * @brief <b>CErrorObject�ACErrorObjects��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _ERROROBJECT_H_
#define _ERROROBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SiNDYLogFunctions.h"
#include "Row.h"
#include "Geometry.h"
#include <boost/tuple/tuple.hpp>

namespace sindy {

/**
 * @struct stErrorInfo
 * @brief <b>�G���[���i�[�\����</b>
 */
struct stErrorInfo
{
	IGeometryPtr			m_ipErrGeom;	//!< �G���[�`��\n�t�B�[�`���̌`��̎Q�Ƃ͊댯�Ȃ̂ŁA�K���R�s�[���쐬���邱��
	long					m_lFieldIndex;	//!< �t�B�[���h�C���f�b�N�X�ԍ�
	CString					m_strMsg;		//!< �G���[���b�Z�[�W
	errorcode::sindyErrCode	m_emErr;		//!< �G���[�R�[�h
	long					m_lScale;		//!< �G���[�Q�ƃX�P�[��
	sindyErrLevel			m_emLevel;		//!< �G���[���x��
};

/**
 * @class CErrorInfo
 * @brief <b>�G���[���i�[�N���X</b>
 */
class SINDYLIB_API CErrorInfo
{
	friend class CErrorInfo;
public:
	CErrorInfo();
	virtual ~CErrorInfo();

	CErrorInfo& operator=( const CErrorInfo& cInfo );
	CErrorInfo( const CErrorInfo& cInfo ){ *this = cInfo; }

	CErrorInfo& operator=( const stErrorInfo& stInfo );
	CErrorInfo( const stErrorInfo& stInfo ){ *this = stInfo; }

	CErrorInfo& operator=( errorcode::sindyErrCode emErr ) { init(); SetErrorCode( emErr ); return *this; }
	CErrorInfo( errorcode::sindyErrCode emErr ){ *this = emErr; }

	/**
	 * @brief �S�Ă̒l���Z�b�g�ł���R���X�g���N�^
	 *
	 * @note emErr��sindyErr_NoErr�̏ꍇ��emLevel�̒l�Ɋւ�炸
	 * ���sindyErrLevel_INFO���Z�b�g����܂�
	 *
	 * @param emErr			[in]			�G���[�R�[�h
	 * @param emLevel		[in]			�G���[���x��
	 * @param ipGeom		[in,optional]	�G���[���̎Q�ƌ`��i�R�s�[�����A�f�t�H���g�FNULL�j
	 * @param lFieldIndex	[in,optional]	�G���[�̌����ƂȂ����t�B�[���h�̃C���f�b�N�X�ԍ��i�f�t�H���g�F-1�j
	 * @param lScale		[in,optional]	�G���[���Q�Ƃ��鎞�̐����X�P�[���l�i�f�t�H���g�F-1�j
	 * @param lpcszFormat	[in,optional]	�G���[���b�Z�[�W�p�t�H�[�}�b�g�i�f�t�H���g�FNULL�j
	 * @param ...			[in,optional]	�G���[���b�Z�[�W�i�ϒ������j
	 */
	CErrorInfo( errorcode::sindyErrCode emErr, sindyErrLevel emLevel, IGeometry* ipGeom = (IGeometry*)NULL, long lFieldIndex = -1, long lScale = -1, LPCTSTR lpcszFormat = NULL, ... );

	void SetErrorCode( errorcode::sindyErrCode emErr ){ m_stInfo.m_emErr = emErr; }
	void SetErrorLevel( sindyErrLevel emLevel ){ m_stInfo.m_emLevel = emLevel; }
	void SetErrorGeometry( IGeometry* ipGeom ){ m_stInfo.m_ipErrGeom = ipGeom; }
	void SetErrorField( long lFieldIndex ){ m_stInfo.m_lFieldIndex = lFieldIndex; }
	void SetErrorScale( long lScale ){ m_stInfo.m_lScale = lScale; }
	void SetErrorMessage( LPCTSTR lpcszFormat, ... );
	errorcode::sindyErrCode GetErrorCode() const { return m_stInfo.m_emErr; }
	sindyErrLevel GetErrorLevel() const { return m_stInfo.m_emLevel; }
	const IGeometryPtr GetErrorGeometry() const { return m_stInfo.m_ipErrGeom; }
	long GetErrorField() const { return m_stInfo.m_lFieldIndex; }
	long GetErrorScale() const { return m_stInfo.m_lScale; }
	LPCTSTR GetErrorMessage() const { return m_stInfo.m_strMsg; }
protected:
	void SetErrorMessageV( LPCTSTR lpcszFormat, va_list args );
private:
	void init();
private:
#pragma warning(push)
#pragma warning(disable:4251)
	stErrorInfo m_stInfo;
#pragma warning(pop)
};

/**
 * @class CErrorInfos
 * @brief <b>�G���[���i�[�R���e�i</b>
 */
class SINDYLIB_API CErrorInfos
{
	friend class CErrorInfos;
public:
	CErrorInfos();
	virtual ~CErrorInfos();

	CErrorInfos( const CErrorInfos& cInfos ){ *this = cInfos; }
	CErrorInfos& operator=( const CErrorInfos& cInfos );
	//@{ @name �R���e�i����n
	typedef std::list<CErrorInfo>::iterator iterator;
	typedef std::list<CErrorInfo>::const_iterator const_iterator;
	typedef std::list<CErrorInfo>::reverse_iterator reverse_iterator;
	typedef std::list<CErrorInfo>::const_reverse_iterator const_reverse_iterator;
	iterator begin(){ return m_listInfo.begin(); }
	iterator end(){ return m_listInfo.end(); }
	reverse_iterator rbegin(){ return m_listInfo.rbegin(); }
	reverse_iterator rend(){ return m_listInfo.rend(); }
	const_iterator begin() const { return m_listInfo.begin(); }
	const_iterator end() const { return m_listInfo.end(); }
	const_reverse_iterator rbegin() const { return m_listInfo.rbegin(); }
	const_reverse_iterator rend() const { return m_listInfo.rend(); }
	CErrorInfo& push_back( const CErrorInfo& cInfo );
	void clear();
	iterator erase(iterator& cIt) { return(m_listInfo.erase(cIt)); }	// [bug 8646]�Ή��Œǉ�
	const_iterator erase(const_iterator& cIt) { return(m_listInfo.erase(cIt)); }	// [bug 8646]�Ή��Œǉ�
	bool empty() const { return m_listInfo.empty(); }
	//@}
	errorcode::sindyErrCode GetDefaultErrCode( sindyErrLevel* pLevel = NULL ) const;
private:
#pragma warning(push)
#pragma warning(disable:4251)
	std::list<CErrorInfo> m_listInfo;
#pragma warning(pop)
};

/**
 * @class CErrorObject
 * @brief <b>�G���[�I�u�W�F�N�g�N���X</b>
 * ���̃N���X�̓t�B�[�`���E���R�[�h�P�ʂł̃G���[�𕡐��i�[����N���X�ł��B\n
 * SiNDY�`���̃��O���o�͂���@�\��L���܂��B���̏ꍇ�A�G���[�����������
 * �����s�G���[���o�͂��܂��B
 */
class SINDYLIB_API CErrorObject
{
public:
	CErrorObject();
	virtual ~CErrorObject();

	CErrorObject( const CErrorObject& cErrObj ){ *this = cErrObj; }
	CErrorObject( const CRow& cRow ){ *this = cRow; }
	CErrorObject( const CRow& cRow, const CErrorInfo& cInfo );
	CErrorObject& operator=( const CErrorObject& cErrObj );
	CErrorObject& operator=( const CRow& cRow );

	void SetErrorObject( const CRow& cRow );
	void SetErrorObject( const boost::tuple<CString, long, CGeometry>& obj );
	const boost::tuple<CString, long, CGeometry>* GetErrorObject() const { return &m_ObjData; }
	virtual void Print( FILE* out, sindyLogFormat emFormat = sindyLogFormat_Common, sindyLogLevel emLevel = sindyLogLevel_ERR ) const;
	//@{ @name �R���e�i����n
	typedef CErrorInfos::iterator iterator;
	typedef CErrorInfos::const_iterator const_iterator;
	iterator begin(){ return m_cInfos.begin(); }
	iterator end(){ return m_cInfos.end(); }
	const_iterator begin() const { return m_cInfos.begin(); }
	const_iterator end() const { return m_cInfos.end(); }
	CErrorInfo& push_back( const CErrorInfo& cInfo );
	CErrorInfos& push_back( const CErrorInfos& cInfos );
	iterator erase(iterator& cIt){return(m_cInfos.erase(cIt));}	// [bug 8646]�Ή��Œǉ�
	const_iterator erase(const_iterator& cIt){return(m_cInfos.erase(cIt));}	// [bug 8646]�Ή��Œǉ�
	//@}
	errorcode::sindyErrCode GetDefaultErrCode( sindyErrLevel* pLevel = NULL ) const { return m_cInfos.GetDefaultErrCode( pLevel ); }
protected:
#pragma warning(push)
#pragma warning(disable:4251)
	boost::tuple<CString, long, CGeometry> m_ObjData;	//!< boost::tuple<�e�[�u����,�I�u�W�F�N�gID,�`��>
	CErrorInfos	m_cInfos;
#pragma warning(pop)
};

/**
 * @class CErrorObjects
 * @brief <b>�G���[�I�u�W�F�N�g�i�[�R���e�i�N���X</b>
 */
class SINDYLIB_API CErrorObjects
{
public:
	explicit CErrorObjects();
	virtual ~CErrorObjects();

	//@{ @name �R���e�i����n
	typedef std::list<CErrorObject>::iterator iterator;
	typedef std::list<CErrorObject>::const_iterator const_iterator;
	iterator begin(){ return m_listErrorObject.begin(); }
	iterator end(){ return m_listErrorObject.end(); }
	const_iterator begin() const { return m_listErrorObject.begin(); }
	const_iterator end() const { return m_listErrorObject.end(); }
	virtual CErrorObject& push_back( const CErrorObject& obj );
	void clear();
	bool empty() const { return m_listErrorObject.empty(); }
	//@} 
	errorcode::sindyErrCode GetDefaultErrCode( sindyErrLevel* pLevel = NULL ) const;
	LPCTSTR ToString();
	void SetFileHandle( FILE* out, sindyLogFormat emFormat = sindyLogFormat_Common, sindyLogLevel emLevel = sindyLogLevel_INFO, bool bInstantly = true ){
		m_hFile = out;
		m_emFormat = emFormat;
		m_emLevel = emLevel;
		m_bOutputInstantly = bInstantly;
	}
private:
	/// �����o�ϐ���S�ď���������
	void init( bool bIsConstructor = false );
protected:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list<CErrorObject>	m_listErrorObject;	//!< �G���[�I�u�W�F�N�g�i�[�R���e�i
	CString					m_strErr;			//!< ������ϊ��p
	bool					m_bOutputInstantly;	//!< ���A���^�C�����O�o�͂����邩�ǂ����̃t���O
	sindyLogFormat			m_emFormat;			//!< ���O�o�̓t�H�[�}�b�g
	sindyLogLevel			m_emLevel;			//!< ���O�o�̓��x��
	FILE*					m_hFile;			//!< ���O�o�̓t�@�C���n���h��
#pragma warning(pop)
};

} // sindy

#endif // ifndef _ERROROBJECT_H_
