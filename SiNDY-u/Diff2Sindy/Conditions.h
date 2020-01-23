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

#include <RowBase.h>
#include "Arguments.h"
#include "PathUtility.h"

namespace boost { namespace property_tree
{
	template<class Key, class Data, class KeyCompare/* = std::less<Key>*/ >
    class basic_ptree;
    typedef basic_ptree<std::wstring, std::wstring, std::less<std::wstring> > wptree;
} }

class ExpressionImpl
{
public:
	ExpressionImpl(){ Detach(); }
	ExpressionImpl( const ExpressionImpl& o) : psrc(o.psrc), pdst(o.pdst), popt(o.popt)
	{
		Detach();
	}
	~ExpressionImpl(){}
	void Attach( const sindy::CSPRowBase& src, const sindy::CSPRowBase& dst, const Arguments& opt )
	{
		psrc = src;
		pdst = dst;
		popt = &opt;
	}
	sindy::CSPRowBase psrc;
	sindy::CSPRowBase pdst;
	const Arguments* popt;
	void Detach()
	{
		psrc.reset();
		pdst.reset();
		popt = nullptr;
	}
};

enum ExpressionType {
	ExpressionTypeUnknown, //!< ����`
	ExpressionTypeSrc,     //!< �\�[�X�^
	ExpressionTypeDst,     //!< DEST�^
	ExpressionTypeOpt,     //!< �I�v�V�����^
	ExpressionTypePOD,     //!< �����l�^
	ExpressionTypeEmpty,   //!< ���݂��Ȃ��iis empty�j
	ExpressionTypeNull,    //!< NULL�iis null�j
	ExpressionTypeNow,     //!< ���ݎ���
	ExpressionTypeCache,   //!< �L���b�V�����ꂽ�l
};

class ExpressionValue : public ExpressionImpl
{
public:
	/// �`���������CComVariant�^��vt�̒l��Ǝ���`
	const static VARTYPE VT_GEOMETRY = (VT_BYREF | VT_I4);

	/// $(SRC.)�A���邢�́A$(DST.)���w�肳�ꂽ�ꍇ�ɑ��������ʂȒl���`
	const static CComVariant EXIST_VALUE;

	ExpressionValue() : type(ExpressionType::ExpressionTypeUnknown) {}
	ExpressionValue( const ExpressionValue& o) : ExpressionImpl(o), type(o.type), col(o.col), val(o.val), expstr(o.expstr) {}
	~ExpressionValue(){}

	bool expression( const uh::tstring& exp );
	CComVariant value(VARTYPE vt) const;
	/// SRC����DST��Empty�i������Ȃ������j���ǂ�����Ԃ�
	bool isEmpty() const;

	ExpressionType type;
	uh::tstring col;
	mutable CComVariant val; //!< �I�v�V�����^�C�v�y�сA���ڒ�`�^�C�v�p
	uh::tstring expstr;
};

class ExpressionMessage : public ExpressionImpl
{
public:
	/// ���b�Z�[�W�̏o�͌`�����`
	enum FORMAT
	{
		UNKNOWN_FORMAT, //!< ����`�̏o�͌`��
		COMMENT,        //!< SiNDY�W�����O�t�H�[�}�b�g�̃R�����g�`���ŏo��
		LOG,            //!< SINDYSTDLOG�`���ŏo��
	};
	/// ���O�ɏ����o�͂���Ώۂ��`
	enum LOG_TARGET
	{
		UNKNOWN_TARGET, //!< ����`�̑Ώ�
		SRC,            //!< SRC���̃��R�[�h��Ώۂɂ���
		DST,           //!< DST���̃��R�[�h��Ώۂɂ���
	};

	/**
	 * @brief ������ɑΉ�����Format�̎��ʎq���擾
	 * @param [in] str ��͂��镶����
	 * @retval true  ������̉�͂ɐ������A��͌��ʂ����ʎq�Ƃ��ă����o�ϐ��Ɋi�[�ł���
	 * @retval false ������̉�͂Ɏ��s����
	 */
	static FORMAT toFormatCode( const uh::tstring& str );

	/**
	 * @brief ������ɑΉ�����LOG_TARGET�̎��ʎq���擾
	 * @param [in] str ��͂��镶����
	 * @retval true  ������̉�͂ɐ������A��͌��ʂ����ʎq�Ƃ��ă����o�ϐ��Ɋi�[�ł���
	 * @retval false ������̉�͂Ɏ��s����
	 */
	static LOG_TARGET toTargetCode( const uh::tstring& str );

	/// SINDYSTDLOG�`���ŏo�͂���ۂɕK�v�ƂȂ郁�b�Z�[�W�ȊO�̏����i�[����N���X
	class LogInfo
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 * @note �G���[�R�[�h�͉p�����ɑΉ��ł���悤�ɕ�����Ƃ��ĕێ�
		 * @param[in] level  �G���[���x��
		 * @param[in] code   �G���[�R�[�h
		 * @param[in] target �R���o�[�g�����ɂ����āASRC���EDST�� �ǂ���̃��O���o�͂��邩
		 */
		LogInfo(const uh::tstring& level, const uh::tstring& code, LOG_TARGET target):
			m_level(level),m_code(code),m_target(target){}

		/// �G���[���x����Ԃ�
		const uh::tstring& level() const { return m_level; }
		/// �G���[�R�[�h��Ԃ�
		const uh::tstring& code() const { return m_code; }
		/// ���O�̏o�͑Ώۂ�Ԃ�
		const LOG_TARGET target() const { return m_target; }
	private:
		uh::tstring m_level;  //!< �G���[���x��
		uh::tstring m_code;   //!< �G���[�R�[�h
		LOG_TARGET  m_target; //!< ���O�̏o�͑Ώۂ��w��
	};
	/// ErrorInfo�p�̃X�}�[�g�|�C���^��ʖ���`
	typedef std::shared_ptr<LogInfo> MessageInfoPtr;

	ExpressionMessage() : cond(false), format(FORMAT::COMMENT) {}
	ExpressionMessage( const ExpressionMessage& o ) : ExpressionImpl(o), msg(o.msg), holders(o.holders), cond(o.cond), format(o.format){}
	ExpressionMessage( const uh::tstring& message, bool condition ) : cond(condition), format(FORMAT::COMMENT) { init(message); }
	~ExpressionMessage(){}

	bool init( const uh::tstring& message );

	uh::tstring string( const sindy::CSPRowBase& src, const sindy::CSPRowBase& dst, const Arguments& opt );

	bool empty() const
	{
		return msg.empty();
	}
	uh::tstring msg;        //!< ���O�ɏo�͂��郁�b�Z�[�W
	bool cond;              //!< �����̐��ۂǂ���Ń��b�Z�[�W���o�͂���邩�̃t���O
	FORMAT format;          //!< ���b�Z�[�W�̏o�͌`��(�f�t�H���g�l:COMMENT)
	MessageInfoPtr msgInfo; //!< ���O�o�͂ɕK�v�ȃ��b�Z�[�W�ȊO�̏��(�G���[�R�[�h�E�G���[���x����)
	mutable std::map<uh::tstring,ExpressionValue> holders;
};

class ConditionExpressions
{
public:
	ConditionExpressions(){}
	ConditionExpressions( const ConditionExpressions& o) : message(o.message){}
	~ConditionExpressions(){}

	virtual	bool eval( const sindy::CSPRowBase& src, const sindy::CSPRowBase& dst, const Arguments& opt, std::list<uh::tstring>& messages ) const = 0;
	virtual bool isContainer() const = 0;
	mutable ExpressionMessage message;
};
typedef std::shared_ptr<ConditionExpressions> SPConditionExpressions;

class ExpressionOperator
{
public:
	/// ���Z�q
	enum EnumOperator : int
	{
		OperatorUnknown,       //!< ���ݒ�
		OperatorEqual,         //!< =
		OperatorEqualBigger,   //!< <=
		OperatorEqualSmaller,  //!< >=
		OperatorBigger,        //!< <
		OperatorSmaller,       //!< >
		OperatorNotEqual,      //!< !=, <>
		OperatorIs,            //!< is
		OperatorIsNot,         //!< is not
	};

	ExpressionOperator() : ope(EnumOperator::OperatorUnknown){}
	ExpressionOperator( const ExpressionOperator& o){ ope = o.ope; }
	ExpressionOperator( const uh::tstring& strOpe ) : ope(EnumOperator::OperatorUnknown)
	{
		init( strOpe );
	}
	~ExpressionOperator(){}
	bool init( const uh::tstring& strOpe )
	{
		if( strOpe == _T("=")  )      ope = EnumOperator::OperatorEqual;
		else if( strOpe == _T("<=") ) ope = EnumOperator::OperatorEqualBigger;
		else if( strOpe == _T(">=") ) ope = EnumOperator::OperatorEqualSmaller;
		else if( strOpe == _T("<")  ) ope = EnumOperator::OperatorBigger;
		else if( strOpe == _T(">")  ) ope = EnumOperator::OperatorSmaller;
		else if( strOpe == _T("!=") ) ope = EnumOperator::OperatorNotEqual;
		else if( strOpe == _T("<>") ) ope = EnumOperator::OperatorNotEqual;
		else if( eq( strOpe, _T("is not") ) ) ope = EnumOperator::OperatorIsNot;
		else if( eq( strOpe, _T("is") ) ) ope = EnumOperator::OperatorIs;
		return ope != EnumOperator::OperatorUnknown;
	}
	bool eval( const ExpressionValue& left, const ExpressionValue& right ) const;
	EnumOperator ope;
};

class ConditionExpressionContainer : public ConditionExpressions
{
public:
	ConditionExpressionContainer() : isAndContainer(true){}
	ConditionExpressionContainer(bool isAnd) : isAndContainer(isAnd){}
	ConditionExpressionContainer( const ConditionExpressionContainer& o) : isAndContainer(o.isAndContainer), vecExp(o.vecExp){}
	~ConditionExpressionContainer(){}

	bool eval( const sindy::CSPRowBase& src, const sindy::CSPRowBase& dst, const Arguments& opt, std::list<uh::tstring>& messages ) const override;
	bool isContainer() const override { return true; }
	bool isAndContainer;
	std::vector<SPConditionExpressions> vecExp;
};

class ConditionExpression : public ConditionExpressions
{
public:
	ConditionExpression(){}
	~ConditionExpression(){}

	bool eval( const sindy::CSPRowBase& src, const sindy::CSPRowBase& dst, const Arguments& opt, std::list<uh::tstring>& messages ) const override;
	bool isContainer() const override { return false; }

	mutable ExpressionValue leftValue;
	mutable ExpressionValue rightValue;
	ExpressionOperator expOperator;
};

class Condition
{
public:
	/// �����^�C�v�̒�`
	enum ConditionType
	{
		Unknown, //!< �s��
		Create, //!< �쐬����
		Delete, //!< �폜����
		Update //!< �X�V����
	};

	Condition() : type(ConditionType::Unknown){}
	Condition( ConditionType t ) : type(t) {}
	~Condition(){}

	ConditionType type; //!< �����̃^�C�v�i�쐬�A�폜�A�X�V�j
	SPConditionExpressions exps; //!< ����
};

typedef std::shared_ptr<Condition> SPCondition;

/// �l�̃R�s�[���s���ۂ̑Ή����Ǘ�����N���X
class IndexPair
{
public:
	/// �l�̃^�C�v
	enum ValType : long
	{
		None, //!< �R�s�[���Ȃ�
		RowValue, //!< ���R�[�h�l
		OptValue, //!< �I�v�V�����l
		PodValue, //!< �Œ�l
		Now, //!< ���ݎ���
		Cache, //!< �L���b�V���l
	};

	IndexPair() : type(ValType::None), srcIdx(-1), dstIdx(-1) {}

	CComVariant value( const sindy::CRowBase& src ) const;

	ValType type; //!< �l�̃^�C�v
	long srcIdx; //!< SRC���̃C���f�b�N�X�ԍ�
	long dstIdx; //!< DST���̃C���f�b�N�X�ԍ�
	uh::tstring srcName; //!< SRC���̃J������
	uh::tstring dstName; //!< DST���̃J������
	mutable CComVariant val; //!< PodValue�p
	uh::tstring foreignTable; //!< �O���e�[�u����
	uh::tstring foreignField; //!< �O���e�[�u���̃t�B�[���h��(DST)
};

/// �J�����Ή��\ first:�J������ second:�J�����̑Ή��Ǘ��N���X�Ə����̃y�A�̃��X�g
class IndexList : public std::map<uh::tstring,std::list<std::pair<IndexPair,SPConditionExpressions>>>
{
public:
	IndexList() : copyall(true), copyshape(true), bulkexe(false) {}
	uh::tstring table; //!< �e�[�u����
	bool copyall;   //!< �S�ẴJ�������R�s�[�ΏۂƂ��邩�ǂ����i�f�t�H���g�Ftrue�j
	bool copyshape; //!< �`����R�s�[�ΏۂƂ��邩�ǂ����i�f�t�H���g�Ftrue�j
	bool bulkexe;   //!< �S���ꊇ�ŏ������邩�ǂ����i�f�t�H���g�Ffalse�j
};

SPConditionExpressions parse_condition( const std::pair<std::wstring,boost::property_tree::wptree>& pt );
bool parse_column( const std::pair<std::wstring,boost::property_tree::wptree>& v2, IndexList& idxList );
bool parse_updatevalue( const std::pair<std::wstring, boost::property_tree::wptree>& v3, IndexPair& idxPair, SPConditionExpressions& exps );
bool parse_relation(const std::pair<std::wstring, boost::property_tree::wptree>& v3, IndexPair& idxPair, SPConditionExpressions& exps );
void parse_value( const uh::tstring& valueAttribute, IndexPair& idxPair );
bool parse_foreignkey( const uh::tstring& foreignkeyAttribute, IndexPair& idxPair );
bool getExpressions( const std::pair<std::wstring, boost::property_tree::wptree>& v3, SPConditionExpressions& exps );
bool getRelationField(const IndexList& idxList, std::map<uh::tstring, std::map<uh::tstring, uh::tstring>>& relationFields);

/// �������ėǂ��^�O���ǂ���
bool isIgnoreTag( const uh::tstring& tag );

