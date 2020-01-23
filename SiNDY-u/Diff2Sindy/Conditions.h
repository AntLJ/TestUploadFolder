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
	ExpressionTypeUnknown, //!< 未定義
	ExpressionTypeSrc,     //!< ソース型
	ExpressionTypeDst,     //!< DEST型
	ExpressionTypeOpt,     //!< オプション型
	ExpressionTypePOD,     //!< 実数値型
	ExpressionTypeEmpty,   //!< 存在しない（is empty）
	ExpressionTypeNull,    //!< NULL（is null）
	ExpressionTypeNow,     //!< 現在時刻
	ExpressionTypeCache,   //!< キャッシュされた値
};

class ExpressionValue : public ExpressionImpl
{
public:
	/// 形状情報をもつCComVariant型のvtの値を独自定義
	const static VARTYPE VT_GEOMETRY = (VT_BYREF | VT_I4);

	/// $(SRC.)、あるいは、$(DST.)が指定された場合に代入する特別な値を定義
	const static CComVariant EXIST_VALUE;

	ExpressionValue() : type(ExpressionType::ExpressionTypeUnknown) {}
	ExpressionValue( const ExpressionValue& o) : ExpressionImpl(o), type(o.type), col(o.col), val(o.val), expstr(o.expstr) {}
	~ExpressionValue(){}

	bool expression( const uh::tstring& exp );
	CComVariant value(VARTYPE vt) const;
	/// SRC又はDSTがEmpty（見つからなかった）かどうかを返す
	bool isEmpty() const;

	ExpressionType type;
	uh::tstring col;
	mutable CComVariant val; //!< オプションタイプ及び、直接定義タイプ用
	uh::tstring expstr;
};

class ExpressionMessage : public ExpressionImpl
{
public:
	/// メッセージの出力形式を定義
	enum FORMAT
	{
		UNKNOWN_FORMAT, //!< 未定義の出力形式
		COMMENT,        //!< SiNDY標準ログフォーマットのコメント形式で出力
		LOG,            //!< SINDYSTDLOG形式で出力
	};
	/// ログに情報を出力する対象を定義
	enum LOG_TARGET
	{
		UNKNOWN_TARGET, //!< 未定義の対象
		SRC,            //!< SRC側のレコードを対象にする
		DST,           //!< DST側のレコードを対象にする
	};

	/**
	 * @brief 文字列に対応するFormatの識別子を取得
	 * @param [in] str 解析する文字列
	 * @retval true  文字列の解析に成功し、解析結果を識別子としてメンバ変数に格納できた
	 * @retval false 文字列の解析に失敗した
	 */
	static FORMAT toFormatCode( const uh::tstring& str );

	/**
	 * @brief 文字列に対応するLOG_TARGETの識別子を取得
	 * @param [in] str 解析する文字列
	 * @retval true  文字列の解析に成功し、解析結果を識別子としてメンバ変数に格納できた
	 * @retval false 文字列の解析に失敗した
	 */
	static LOG_TARGET toTargetCode( const uh::tstring& str );

	/// SINDYSTDLOG形式で出力する際に必要となるメッセージ以外の情報を格納するクラス
	class LogInfo
	{
	public:
		/**
		 * @brief コンストラクタ
		 * @note エラーコードは英数字に対応できるように文字列として保持
		 * @param[in] level  エラーレベル
		 * @param[in] code   エラーコード
		 * @param[in] target コンバート処理において、SRC側・DST側 どちらのログを出力するか
		 */
		LogInfo(const uh::tstring& level, const uh::tstring& code, LOG_TARGET target):
			m_level(level),m_code(code),m_target(target){}

		/// エラーレベルを返す
		const uh::tstring& level() const { return m_level; }
		/// エラーコードを返す
		const uh::tstring& code() const { return m_code; }
		/// ログの出力対象を返す
		const LOG_TARGET target() const { return m_target; }
	private:
		uh::tstring m_level;  //!< エラーレベル
		uh::tstring m_code;   //!< エラーコード
		LOG_TARGET  m_target; //!< ログの出力対象を指定
	};
	/// ErrorInfo用のスマートポインタを別名定義
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
	uh::tstring msg;        //!< ログに出力するメッセージ
	bool cond;              //!< 条件の成否どちらでメッセージが出力されるかのフラグ
	FORMAT format;          //!< メッセージの出力形式(デフォルト値:COMMENT)
	MessageInfoPtr msgInfo; //!< ログ出力に必要なメッセージ以外の情報(エラーコード・エラーレベル等)
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
	/// 演算子
	enum EnumOperator : int
	{
		OperatorUnknown,       //!< 未設定
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
	/// 条件タイプの定義
	enum ConditionType
	{
		Unknown, //!< 不明
		Create, //!< 作成条件
		Delete, //!< 削除条件
		Update //!< 更新条件
	};

	Condition() : type(ConditionType::Unknown){}
	Condition( ConditionType t ) : type(t) {}
	~Condition(){}

	ConditionType type; //!< 条件のタイプ（作成、削除、更新）
	SPConditionExpressions exps; //!< 条件
};

typedef std::shared_ptr<Condition> SPCondition;

/// 値のコピーを行う際の対応を管理するクラス
class IndexPair
{
public:
	/// 値のタイプ
	enum ValType : long
	{
		None, //!< コピーしない
		RowValue, //!< レコード値
		OptValue, //!< オプション値
		PodValue, //!< 固定値
		Now, //!< 現在時刻
		Cache, //!< キャッシュ値
	};

	IndexPair() : type(ValType::None), srcIdx(-1), dstIdx(-1) {}

	CComVariant value( const sindy::CRowBase& src ) const;

	ValType type; //!< 値のタイプ
	long srcIdx; //!< SRC側のインデックス番号
	long dstIdx; //!< DST側のインデックス番号
	uh::tstring srcName; //!< SRC側のカラム名
	uh::tstring dstName; //!< DST側のカラム名
	mutable CComVariant val; //!< PodValue用
	uh::tstring foreignTable; //!< 外部テーブル名
	uh::tstring foreignField; //!< 外部テーブルのフィールド名(DST)
};

/// カラム対応表 first:カラム名 second:カラムの対応管理クラスと条件のペアのリスト
class IndexList : public std::map<uh::tstring,std::list<std::pair<IndexPair,SPConditionExpressions>>>
{
public:
	IndexList() : copyall(true), copyshape(true), bulkexe(false) {}
	uh::tstring table; //!< テーブル名
	bool copyall;   //!< 全てのカラムをコピー対象とするかどうか（デフォルト：true）
	bool copyshape; //!< 形状をコピー対象とするかどうか（デフォルト：true）
	bool bulkexe;   //!< 全国一括で処理するかどうか（デフォルト：false）
};

SPConditionExpressions parse_condition( const std::pair<std::wstring,boost::property_tree::wptree>& pt );
bool parse_column( const std::pair<std::wstring,boost::property_tree::wptree>& v2, IndexList& idxList );
bool parse_updatevalue( const std::pair<std::wstring, boost::property_tree::wptree>& v3, IndexPair& idxPair, SPConditionExpressions& exps );
bool parse_relation(const std::pair<std::wstring, boost::property_tree::wptree>& v3, IndexPair& idxPair, SPConditionExpressions& exps );
void parse_value( const uh::tstring& valueAttribute, IndexPair& idxPair );
bool parse_foreignkey( const uh::tstring& foreignkeyAttribute, IndexPair& idxPair );
bool getExpressions( const std::pair<std::wstring, boost::property_tree::wptree>& v3, SPConditionExpressions& exps );
bool getRelationField(const IndexList& idxList, std::map<uh::tstring, std::map<uh::tstring, uh::tstring>>& relationFields);

/// 無視して良いタグかどうか
bool isIgnoreTag( const uh::tstring& tag );

