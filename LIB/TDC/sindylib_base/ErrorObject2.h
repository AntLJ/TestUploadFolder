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
 * @file ErrorObjectBase.h
 * @brief <b>CErrorObjectBase�ACErrorObjectsBase��`�t�@�C��</b>\n
 * @author �Z�p�J�����V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _ERROROBJECTBASE_H_
#define _ERROROBJECTBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SiNDYLogFunctions.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include <boost/tuple/tuple.hpp>

namespace sindy {

/**
 * @struct stErrorInfoBase
 * @brief <b>�G���[���i�[�\����</b>
 */
struct stErrorInfoBase
{
	IGeometryPtr			m_ipErrGeom;	//!< �G���[�`��\n�t�B�[�`���̌`��̎Q�Ƃ͊댯�Ȃ̂ŁA�K���R�s�[���쐬���邱��
	long					m_lFieldIndex;	//!< �t�B�[���h�C���f�b�N�X�ԍ�
	CString					m_strMsg;		//!< �G���[���b�Z�[�W
	errorcode::sindyErrCode	m_emErr;		//!< �G���[�R�[�h
	long					m_lScale;		//!< �G���[�Q�ƃX�P�[��
	sindyErrLevel			m_emLevel;		//!< �G���[���x��
};

/**
 * @class CErrorInfoBase
 * @brief <b>�G���[���i�[�N���X</b>
 */
class CErrorInfoBase
{
	friend class CErrorInfoBase;
public:
	CErrorInfoBase();
	virtual ~CErrorInfoBase();

	CErrorInfoBase& operator=( const CErrorInfoBase& cInfo );
	CErrorInfoBase( const CErrorInfoBase& cInfo ){ *this = cInfo; }

	CErrorInfoBase& operator=( const stErrorInfoBase& stInfo );
	CErrorInfoBase( const stErrorInfoBase& stInfo ){ *this = stInfo; }

	CErrorInfoBase& operator=( errorcode::sindyErrCode emErr ) { init(); SetErrorCode( emErr ); return *this; }
	CErrorInfoBase( errorcode::sindyErrCode emErr ){ *this = emErr; }

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
	CErrorInfoBase( errorcode::sindyErrCode emErr, sindyErrLevel emLevel, IGeometry* ipGeom = (IGeometry*)NULL, long lFieldIndex = -1, long lScale = -1, LPCTSTR lpcszFormat = NULL, ... );

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
	/**
	* @brief �G���[�����擾����
	* @return �u�G���[�R�[�h���b�Z�[�W�i�G���[���b�Z�[�W�j�v�̌`���̕�����
	*/
	CString ToString() const;
protected:
	void SetErrorMessageV( LPCTSTR lpcszFormat, va_list args );
private:
	void init();
private:
#pragma warning(push)
#pragma warning(disable:4251)
	stErrorInfoBase m_stInfo;
#pragma warning(pop)
};

typedef boost::shared_ptr<CErrorInfoBase> CSPErrorInfoBase;

/**
 * @class CErrorInfosBase
 * @brief <b>�G���[���i�[�R���e�i</b>
 */
class CErrorInfosBase
{
	friend class CErrorInfosBase;
public:
	CErrorInfosBase();
	virtual ~CErrorInfosBase();

	CErrorInfosBase( const CErrorInfosBase& cInfos ){ *this = cInfos; }
	CErrorInfosBase& operator=( const CErrorInfosBase& cInfos );
	//@{ @name �R���e�i����n
	typedef std::list<CSPErrorInfoBase>::iterator iterator;
	typedef std::list<CSPErrorInfoBase>::const_iterator const_iterator;
	typedef std::list<CSPErrorInfoBase>::reverse_iterator reverse_iterator;
	typedef std::list<CSPErrorInfoBase>::const_reverse_iterator const_reverse_iterator;
	iterator begin(){ return m_listInfo.begin(); }
	iterator end(){ return m_listInfo.end(); }
	reverse_iterator rbegin(){ return m_listInfo.rbegin(); }
	reverse_iterator rend(){ return m_listInfo.rend(); }
	const_iterator begin() const { return m_listInfo.begin(); }
	const_iterator end() const { return m_listInfo.end(); }
	const_reverse_iterator rbegin() const { return m_listInfo.rbegin(); }
	const_reverse_iterator rend() const { return m_listInfo.rend(); }
	CSPErrorInfoBase push_back( const CSPErrorInfoBase& cInfo );
	void clear();
	bool empty() const { return m_listInfo.empty(); }
	//@}
	errorcode::sindyErrCode GetDefaultErrCode( sindyErrLevel* pLevel = NULL ) const;
	/**
	* @brief �G���[���𕶎���Ƃ��Ď擾����
	* @param [in] bUnique �d�����郁�b�Z�[�W���Ȃ��ꍇ��true
	* @return �R���e�i���̑S�ẴG���[���
	*/
	CString ToString( bool bUnique = true ) const;
private:
#pragma warning(push)
#pragma warning(disable:4251)
	std::list<CSPErrorInfoBase> m_listInfo;
#pragma warning(pop)
};

/**
 * @class CErrorObjectBase
 * @brief <b>�G���[�I�u�W�F�N�g�N���X</b>
 * ���̃N���X�̓t�B�[�`���E���R�[�h�P�ʂł̃G���[�𕡐��i�[����N���X�ł��B\n
 * SiNDY�`���̃��O���o�͂���@�\��L���܂��B���̏ꍇ�A�G���[�����������
 * �����s�G���[���o�͂��܂��B
 */
class CErrorObjectBase
{
public:
	CErrorObjectBase();
	virtual ~CErrorObjectBase();

	CErrorObjectBase( const CErrorObjectBase& cErrObj ){ *this = cErrObj; }
	CErrorObjectBase( const CRowBase& cRow ){ *this = cRow; }
	CErrorObjectBase( const CRowBase& cRow, const CSPErrorInfoBase& cInfo );
	CErrorObjectBase& operator=( const CErrorObjectBase& cErrObj );
	CErrorObjectBase& operator=( const CRowBase& cRow );

	void SetErrorObject( const CRowBase& cRow );
	void SetErrorObject( const boost::tuple<CString, long, CGeometryBase>& obj );
	const boost::tuple<CString, long, CGeometryBase>* GetErrorObject() const { return &m_ObjData; }
	virtual void Print( FILE* out, sindyLogFormat emFormat = sindyLogFormat_Common, sindyLogLevel emLevel = sindyLogLevel_ERR ) const;
	//@{ @name �R���e�i����n
	typedef CErrorInfosBase::iterator iterator;
	typedef CErrorInfosBase::const_iterator const_iterator;
	iterator begin(){ return m_cInfos.begin(); }
	iterator end(){ return m_cInfos.end(); }
	const_iterator begin() const { return m_cInfos.begin(); }
	const_iterator end() const { return m_cInfos.end(); }
	CSPErrorInfoBase& push_back( const CSPErrorInfoBase& cInfo );
	CErrorInfosBase& push_back( const CErrorInfosBase& cInfos );
	//@}
	errorcode::sindyErrCode GetDefaultErrCode( sindyErrLevel* pLevel = NULL ) const { return m_cInfos.GetDefaultErrCode( pLevel ); }
protected:
#pragma warning(push)
#pragma warning(disable:4251)
	boost::tuple<CString, long, CGeometryBase> m_ObjData;	//!< boost::tuple<�e�[�u����,�I�u�W�F�N�gID,�`��>
	CErrorInfosBase	m_cInfos;
#pragma warning(pop)
};

typedef boost::shared_ptr<CErrorObjectBase> CSPErrorObjectBase;

/**
 * @class CErrorObjects
 * @brief <b>�G���[�I�u�W�F�N�g�i�[�R���e�i�N���X</b>
 */
class CErrorObjectsBase
{
public:
	explicit CErrorObjectsBase();
	virtual ~CErrorObjectsBase();

	//@{ @name �R���e�i����n
	typedef std::list<CSPErrorObjectBase>::iterator iterator;
	typedef std::list<CSPErrorObjectBase>::const_iterator const_iterator;
	typedef std::list<CSPErrorObjectBase>::reverse_iterator reverse_iterator;
	typedef std::list<CSPErrorObjectBase>::const_reverse_iterator const_reverse_iterator;
	iterator begin(){ return m_listErrorObject.begin(); }
	iterator end(){ return m_listErrorObject.end(); }
	reverse_iterator rbegin(){ return m_listErrorObject.rbegin(); }
	reverse_iterator rend(){ return m_listErrorObject.rend(); }
	const_iterator begin() const { return m_listErrorObject.begin(); }
	const_iterator end() const { return m_listErrorObject.end(); }
	const_reverse_iterator rbegin() const { return m_listErrorObject.rbegin(); }
	const_reverse_iterator rend() const { return m_listErrorObject.rend(); }
	virtual CSPErrorObjectBase& push_back( const CSPErrorObjectBase& obj );
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
	std::list<CSPErrorObjectBase>	m_listErrorObject;	//!< �G���[�I�u�W�F�N�g�i�[�R���e�i
	CString					m_strErr;			//!< ������ϊ��p
	bool					m_bOutputInstantly;	//!< ���A���^�C�����O�o�͂����邩�ǂ����̃t���O
	sindyLogFormat			m_emFormat;			//!< ���O�o�̓t�H�[�}�b�g
	sindyLogLevel			m_emLevel;			//!< ���O�o�̓��x��
	FILE*					m_hFile;			//!< ���O�o�̓t�@�C���n���h��
#pragma warning(pop)
};

} // sindy

#endif // ifndef _ERROROBJECTBASE_H_
