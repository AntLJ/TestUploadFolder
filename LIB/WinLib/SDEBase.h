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

#include "ArcHelperEx/sde_ondemand.h" // TODO: ���Ƃ�������
#include <boost/tuple/tuple.hpp>
//#include "ListString.h"
#include <algorithm>
#include <boost/iterator_adaptors.hpp>
#include <boost/shared_ptr.hpp>

// #define _USE_SDE_UPDATE update�J�[�\�����g���Ȃ��炵���̂ŕ���

/**
 * @class CSDEException
 * @brief SDE API ��O�N���X
 */
class CSDEException : public SE_ERROR
{
public:
	CSDEException() : errcode(SE_SUCCESS)
	{
		sde_error = ext_error = 0;
		err_msg1[0] = err_msg2[0] = '\0';
	}
	CString ToString() const;
	long errcode; //!< SDE API ���Ԃ����G���[�R�[�h
};

/**
 * @brief �X�g���[���Ǘ��N���X
 * @note �X�g���[���̍쐬�E�J�����Ǘ����܂��B
 */
class CSDEStream
{
public:
	friend class CSDECursor;
	friend class CSDEBase;
	CSDEStream() : stream_(NULL), errcode_(0) {}
	CSDEStream(SE_CONNECTION conn) : stream_(NULL), errcode_(0) { StreamCreate( conn ); }
	virtual ~CSDEStream() { StreamFree(); }
	operator SE_STREAM() { return stream_; }
	long StreamCreate(SE_CONNECTION conn) { if( conn ) errcode_ = SSE_stream_create( conn, &stream_ ); return errcode_; }
	long StreamFree(){ if( stream_ ) { errcode_ = SSE_stream_free( stream_ ); stream_ = NULL; } return errcode_; }
	/// �Ō�̃G���[�R�[�h
	long err() const { return errcode_; }
	/// �Ō�̃G���[�𕶎���ŕԂ�
	CString GetErrorMessage() const;
protected:
	SE_STREAM stream_;
	long errcode_; //!< �ŏI�G���[�R�[�h
};
typedef boost::shared_ptr<CSDEStream> CSPSDEStream;

/**
 * @class CSDECursor
 * @brief �����J�[�\���Ƃ��ē��삷��N���X
 * 
 * @note �C�e���[�^���g�p����ꍇ�A�t�F�b�`���̃G���[��
 * CSDEException�̗�O�Ƃ��Ĕ�΂��܂��B
 * NULL OK�ȃJ�����ŏW���֐������g�p�����ꍇ�ɁANULL�����o�����
 * �G���[���������܂��̂ŁA�K����O��⑫���Ă��������B
 */
class CSDECursor
{
	friend class CSDEBase;
public:
	CSDECursor();
	CSDECursor(SE_CONNECTION conn);
	operator SE_STREAM() { return (SE_STREAM)*stream_; }
	/// �t�B�[���h��`�N���X
	class Field
	{
	public:
		CString name;
		long sde_type;
		long size;
	};
	typedef std::vector<CComVariant> val_type;

	//@{ @name �C�e���[�^��`
	struct iterator
		: boost::iterator_adaptor<iterator,CSDECursor*, val_type, boost::forward_traversal_tag>
	{
		iterator( CSDECursor* p )
			: iterator_adaptor<iterator,CSDECursor*, val_type, boost::forward_traversal_tag>( p ){}
		void increment();
		val_type& dereference() const { return base_reference()->val_; }

#ifdef _USE_SDE_UPDATE
		/// �X�V���[�h�̏ꍇ�ɍX�V���ʂ�K�p����
		long store();
#endif // ifdef _USE_SDE_UPDATE
	};
	/// begin, end �̂��������Ȓ�`
	iterator begin(){ return ++iterator(this); }
	iterator end()  { return iterator(NULL); }
	//@}

	/// �t�B�[���h���
	const std::vector<Field>& fields() const { return fields_; }

	/// �Ō�̃G���[�R�[�h
	long err() const { return stream_->err(); }
	/// �Ō�̃G���[�𕶎���ŕԂ�
	CString GetErrorMessage() const { return stream_->GetErrorMessage(); }
private:
	CSPSDEStream stream_;
	std::vector<Field> fields_; //!< �t�B�[���h��`���X�g
	val_type val_;
};

/**
 * @class CSDEBase
 * @biref SE_CONNECTION���Ǘ�����N���X
 */
class CSDEBase
{
public:
	CSDEBase() : conn_(NULL), attached_(false) {}
	CSDEBase(SE_CONNECTION conn) : conn_(conn), attached_(true) {}
	CSDEBase( const CSDEBase& obj ) : conn_(obj.conn_), attached_(true) {}
	CSDEBase( IUnknown* ipUnk ) : conn_(NULL), attached_(false) { attach( ipUnk ); }
	virtual ~CSDEBase() { disconnect(); }

	SE_CONNECTION handle(){	return conn_; }

	void attach( IUnknown* ipUnk );
	void connect( IUnknown* ipUnk );
	void disconnect();

	/// ���������s����
	CSDECursor search( LPCSTR format, ... ) const;
#ifdef _USE_SDE_UPDATE
	CSDECursor update( LPCSTR format, ... ) const;
#endif // ifdef _USE_SDE_UPDATE

	LPCTSTR getLastQuery() const { return lastquery_; }
protected:
	CSDECursor query( bool bUpdate, LPCSTR query ) const;
protected:
	SE_CONNECTION conn_; //!< SDE�ڑ��n���h��
	bool attached_; //!< �A�^�b�`���ǂ���\n�V�K�ɐڑ������ꍇ��false
	boost::tuple<CString,CString,CString>	conninfo_; //!< �V�K�ɐڑ������ꍇ�̐ڑ����
	mutable CString lastquery_; //!< �ŏI�N�G��
};
