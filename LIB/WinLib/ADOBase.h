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

#include <vector>
#include <boost/iterator_adaptors.hpp>
/**
 * @class CADOCursor
 * @brief �����J�[�\���Ƃ��ē��삷��N���X
 */
class CADOCursor
{
	friend class CADOBase;
public:
	CADOCursor() : errcode_(0) {}
	CADOCursor(_Recordset* stream) : stream_(stream), errcode_(0){}

	/// �t�B�[���h��`�N���X
	class Field
	{
	public:
		CString name;
		long sde_type;
		long size;
	};

	//@{ @name �C�e���[�^��`
	typedef std::vector<CComVariant> val_type;
	struct iterator
		: boost::iterator_adaptor<iterator,CADOCursor*, val_type,boost::forward_traversal_tag>
	{
		iterator( CADOCursor* p )
			: iterator_adaptor<iterator,CADOCursor*, val_type, boost::forward_traversal_tag>( p ){}
		void iterator::getvalue();
		void increment();
		val_type& dereference() const { return base_reference()->val_; }

		/// �X�V���ʂ�K�p����
		long store();
	};
	/// begin, end �̂��������Ȓ�`
	iterator begin(){ if( ! stream_->EndOfFile ){ iterator it(this); it.getvalue(); return it; } return end(); }
	iterator end()  { return iterator(NULL); }
	//@}

	/// �Ō�̃G���[�𕶎���ŕԂ�
	CString GetErrorMessage() const;

	/// �Ō�̃G���[�R�[�h
	long err() const { return errcode_; }

	/// �t�B�[���h���
	const std::vector<Field>& fields() const { return fields_; }

private:
	_RecordsetPtr stream_; //!< �J�[�\���p�X�g���[��
	HRESULT errcode_; //!< �ŏI�G���[�R�[�h
	std::vector<Field> fields_; //!< �t�B�[���h��`���X�g
	val_type val_;
};

/**
 * @class CADOBase
 * @biref _Connection*���Ǘ�����N���X
 */
class CADOBase
{
public:
	CADOBase() : conn_(NULL), attached_(false) {}
	CADOBase(_Connection* conn) : conn_(conn), attached_(true) {}
	CADOBase( const CADOBase& obj ) : conn_(obj.conn_), attached_(true) {}
	CADOBase( LPCTSTR lpcszPath ) : attached_(false) { connect(lpcszPath); }
	virtual ~CADOBase() { disconnect(); }

	CADOBase& operator=( _Connection* conn )
	{
		disconnect();
		attach( conn );
		return *this;
	}
	_Connection* handle(){	return conn_; }

	void attach( _Connection* conn );
/**
 * @brief ADO�ڑ�����
 * @note .xls�A.csv�A.mdb���Ώ�
 * @param lpcszPath [in] �t���p�X
 * @param errMsg [out,optional] ���s�����ꍇ�A_com_error::ErrorMessage()�̒l
 * @retval true ����
 * @retval false ���s
 */
	bool connect( LPCTSTR lpcszPath, CString& errMsg = CString() );
	bool connect_by_string( LPCTSTR lpcszConnectString );
	void disconnect();

	/**
	 * @brief SQL���������s����
	 * �ǂݎ���p�y��forward��p�̃J�[�\�����g�p����
	 * select�������s���܂��B
	 */
	CADOCursor search( LPCSTR format, ... ) const;
	/**
	 * @brief �X�V�J�[�\���pSQL�����s����
	 * �X�V�\��select���𔭍s����ꍇ�Ɏg�p���܂��B
	 * �ύX�𔽉f����ɂ́Aiterator::store()�����s����
	 * �K�v������܂��B
	 * @see iterator::store
	 */
	CADOCursor update( LPCSTR format, ... ) const;
	/**
	 * @brief �C���T�[�g�pSQL�����s����
	 * insert�������s��J�[�\�����삪�s�v�Ȃ��̂͂��̊֐���
	 * ���s����K�v������܂��iupdate�֐��ł̓G���[���o��j
	 */
	CADOCursor insert( LPCSTR format, ... ) const;

	LPCTSTR getLastQuery() const { return lastquery_; }
protected:
	CADOCursor query( bool is_update, LPCSTR query, bool is_nocursor = false ) const;
protected:
	_ConnectionPtr conn_; //!< ADO�ڑ��I�u�W�F�N�g
	bool attached_; //!< �A�^�b�`���ǂ���\n�V�K�ɐڑ������ꍇ��false
	mutable CString lastquery_; //!< �ŏI�N�G��
};
