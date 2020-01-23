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

#include "stdafx.h"
#include "Conditions.h"
#include "Notifier.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <codecvt>
#include <memory>
#include <regex>
#include <iostream>
#include <winlib/winutil.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <TDC/sindylib_base/RowBase.h>
#include <TDC/sindylib_base/GeometryBase.h>

// TODO: Utilityの構成を検討し直す
// 各処理で共通に使えるものと処理独自のものでUtilityを分ける
#include "ClipUtility.h"

using namespace std;
using namespace boost::property_tree;

namespace boost { namespace property_tree
{
    typedef string_path<uh::tstring, id_translator<uh::tstring> > tpath;
    typedef basic_ptree<uh::tstring, uh::tstring> tptree;
    typedef basic_ptree<uh::tstring, uh::tstring,
                        detail::less_nocase<uh::tstring> >
        tiptree;
} }

typedef std::basic_regex<TCHAR> tregex;
typedef std::match_results<const TCHAR *> tcmatch;
typedef std::match_results<uh::tstring::const_iterator> tsmatch;
typedef std::regex_token_iterator<uh::tstring::const_iterator> tregex_token_iterator;

// 条件式解析関数用の定数を定義
namespace condition_constant
{
	// タグ名の定義
	namespace tag
	{
		const uh::tstring kAnd(_T("And"));
		const uh::tstring kOr(_T("Or"));
		const uh::tstring kExpression(_T("expression"));
	} // tag

	// expressionタグの属性名の定義
	namespace expression_attribute
	{
		const uh::tstring kValue(_T("<xmlattr>.value"));		       //!< 条件式の属性名
		const uh::tstring kMessage(_T("<xmlattr>.message"));	       //!< 条件式に合わない場合のメッセージ属性名（互換性保持用）
		const uh::tstring kFalseMessage(_T("<xmlattr>.FalseMessage")); //!< 条件式に合わない場合のメッセージ属性名
		const uh::tstring kTrueMessage(_T("<xmlattr>.TrueMessage"));   //!< 条件式に合う場合のメッセージ属性名
		const uh::tstring kFormat(_T("<xmlattr>.format"));             //!< メッセージ出力形式の属性名
		const uh::tstring kErrorLevel(_T("<xmlattr>.level"));          //!< メッセージ出力時のエラーレベルの属性名
		const uh::tstring kErrorCode(_T("<xmlattr>.code"));            //!< メッセージ出力時のエラーコードの属性名
		const uh::tstring kTarget(_T("<xmlattr>.target"));             //!< メッセージ出力時のエラーコードの属性名
	} // expression_attribute

} // condition_constant

namespace
{
	// TODO: 構文チェック用のクラスにすることを検討
	/**
	 * @brief レコードの存在判定にEmpty以外を使用していないか確認する
	 * @warning 右辺値に対して、左辺値が妥当か判定したい場合には、
	 * left, rightに渡す引数を逆にして呼び出す
	 * @param[in] left  左辺値
	 * @param[in] right 右辺値
	 * @retval true  NULLを使用していない
	 * @retval false NULLを使用している
	 */
	bool checkRecordCompareEmpty(const ExpressionValue& left, const ExpressionValue& right)
	{
		// カラムが空でないもの、タイプがSrc,Dstでないものは判定の必要なし
		if( !left.col.empty() ) return true;
		if( left.type != ExpressionTypeSrc && left.type != ExpressionTypeDst ) return true;

		// 下記は開発当初の要件定義の内容と異なるので、コメントアウト
		// 開発当初、$(SRC.) <> $(DST.) は、"形状・属性のいずれかが異なるとき真"という条件だった。
		// 現在、上記のような判定を行う実装はなく、上記条件が指定されてしまった場合、
		// 使用者が意図しない挙動を引き起こす可能性があるので、このような入力は許さないことにする。
		// TODO: $(SRC.) <> $(DST.)の判定が実装されたら、このエラーチェックは見直す
		//if( right.col.empty() && (right.type==ExpressionTypeSrc || right.type==ExpressionTypeDst) )
		//	return true;

		// レコードの存在判定に empty 以外は許可しない
		return right.type==ExpressionTypeEmpty;
	}
	/**
	 * @brief Emptyの判定に対して正当な演算子を使用しているか確認する
	 * @param[in] left  左辺値
	 * @param[in] right 右辺値
	 * @param[in] ope   比較演算子
	 * @retval true  演算子は正当である
	 * @retval false 不正な演算子である
	 */
	bool checkEmptyOperator(const ExpressionValue& left, const ExpressionValue& right, const ExpressionOperator& ope)
	{
		// 右左辺値のいずれもEmptyでないなら、判定の必要なし
		if( left.type != ExpressionTypeEmpty && right.type != ExpressionTypeEmpty ) return true;

		// オペレータにis, is not以外は許可しない
		return ( ope.ope==ope.OperatorIs || ope.ope==ope.OperatorIsNot );
	}

	/**
	 * @brief 右左辺値の組合せが許容されるものか確認
	 * @detail 問題があった場合には、例外処理としてエラーメッセージを出力する
	 * @note ここでは形状判定に関する構文チェックを行わない。<br>
	 * 設定ファイルの読込み段階では、
	 * フィールド名だけで、それが形状フィールドかどうか判断できないためである。<br>
	 * (なお、iPCでは、基本的に形状フィールドはSHAPEという名前を使用することが決まっているため、
	 * iPC内のデータを扱うだけなら、この時点で判定可能である。
	 * ただ、外部のShapeデータ、特に海外のShapeデータを取り込みたい、といった要望があった場合に、
	 * 形状フィールド名がSHAPEであることは保障されないので、決め打ちにはしない）
	 * @param[in] left  左辺値
	 * @param[in] right 右辺値
	 * @param[in] ope   比較演算子
	 * @retval true  組合せは許容される
	 * @retval false 組合せは許容されない
	 */
	void checkAllowCondition(const ExpressionValue& left, const ExpressionValue& right, const ExpressionOperator& ope )
	{
		if(!checkRecordCompareEmpty(left, right)
		|| !checkRecordCompareEmpty(right, left) )
		{
			notifier::critical::versatile(L"$(SRC.)、$(DST.) は is empty, is not empty でしか判定できません");
		}

		if( !checkEmptyOperator(left, right, ope) )
		{
			notifier::critical::versatile(L"emptyの判定式に is, is not 以外を使用することはできません");
		}
	}
} // namespace

SPConditionExpressions parse_condition( const std::pair<std::wstring,wptree>& pt )
{
	using namespace condition_constant;

	SPConditionExpressions c;
	// <or> とか <and>
	if( eq( pt.first, tag::kAnd ) || eq( pt.first, tag::kOr ) )
	{
		// TODO: ConditionExpressionContainerに持って行ってもよい
		c.reset( new ConditionExpressionContainer(eq( pt.first, tag::kAnd)) );
		for( const auto& v : pt.second.get_child(_T("")) )
			static_pointer_cast<ConditionExpressionContainer>(c)->vecExp.push_back( parse_condition(v) );
	}
	// <expression value="$(SRC.ORG_OBJID) = $(DST.OBJECTID)" /> とか
	else if( eq( pt.first, tag::kExpression ) ) {
		// parse_expressionとか関数化
		// TODO: ConditionExpressionにもっていってもよい
		c.reset( new ConditionExpression() );
		ConditionExpression& cond( *static_pointer_cast<ConditionExpression>(c) );
		uh::tstring exp = pt.second.get<uh::tstring>(expression_attribute::kValue);
		// 演算子で、右と左に分解する
		std::list<uh::tstring> listOperator = boost::assign::list_of(_T("<>"))(_T("!="))(_T("<="))(_T(">="))(_T("="))(_T(">"))(_T("<"))(_T("is not"))(_T("is")); // 順番注意！！
		uh::tstring::size_type pos = uh::tstring::npos;
		for( const auto& ope : listOperator )
		{
			if( uh::tstring::npos == ( pos = exp.find(ope) ) )
				continue;

			cond.expOperator.init(ope);
			// 右と左に分解
			if( !cond.leftValue.expression(exp.substr( 0, pos )) )
				return SPConditionExpressions();
			cond.rightValue.expression(exp.substr( pos+ope.size() ));

			// 右辺値・左辺値・演算子の組合せに不正がないか確認
			checkAllowCondition( cond.leftValue, cond.rightValue, ope );
			break;
		}
	}
	// message
	if( c )
	{
		// ノードから指定の属性値を取得できた場合に指定の処理を実行する
		// メッセージを確認
		boost::optional<uh::tstring> optMessage  = pt.second.get_optional<uh::tstring>(expression_attribute::kMessage);
		boost::optional<uh::tstring> optFMessage = pt.second.get_optional<uh::tstring>(expression_attribute::kFalseMessage);
		boost::optional<uh::tstring> optTMessage = pt.second.get_optional<uh::tstring>(expression_attribute::kTrueMessage);
		if( optMessage )
		{
			c->message.cond = false;
			c->message.init( optMessage.get() );
		}
		if( optFMessage )
		{
			c->message.cond = false;
			c->message.init( optFMessage.get() );
		}
		if( optTMessage )
		{
			c->message.cond = true;
			c->message.init( optTMessage.get() );
		}

		// TODO: 下記のログフォーマットの読込み部・メッセージの出力部は関数化を検討する
		// ログフォーマットを確認
		boost::optional<uh::tstring> optFormat  = pt.second.get_optional<uh::tstring>(expression_attribute::kFormat);
		if( optFormat )
		{
			auto formatCode = c->message.toFormatCode(optFormat.get());
			// 文字列を列挙子に変換して格納
			if( ExpressionMessage::FORMAT::UNKNOWN_FORMAT == formatCode )
			{
				// 定義外の列挙子なら、異常終了する
				std::wstringstream ss;
				ss << L" format に未定義の値があります : " << uh::toWStr(optFormat.get());
				notifier::critical::versatile( ss.str() );
			}
			c->message.format = formatCode;
		}

		// 以下はログフォーマットが指定されていた場合にのみ、読込む
		if( ExpressionMessage::FORMAT::LOG == c->message.format )
		{
			// SINDYSTDLOG形式で出力する場合のエラーコードを取得
			uh::tstring code;
			boost::optional<uh::tstring> optCode  = pt.second.get_optional<uh::tstring>(expression_attribute::kErrorCode);
			if( optCode )
				code = optCode.get();

			// SINDYSTDLOG形式で出力する場合のエラーレベルを取得
			uh::tstring level;
			boost::optional<uh::tstring> optLevel  = pt.second.get_optional<uh::tstring>(expression_attribute::kErrorLevel);
			if( optLevel )
			{
				notifier::error_level levelCode = notifier::getErrorLevel( uh::toStr(optLevel.get()) );
				// 列挙子が定義されていない文字列であるなら、異常終了する
				if( notifier::error_level::none == levelCode )
				{
					std::wstringstream ss;
					ss << L" level に未定義な値があります : " << uh::toWStr(optLevel.get());
					notifier::critical::versatile( ss.str() );
				}
				level = optLevel.get();
			}

			// ログに情報を出力する対象を取得
			// デフォルト値はDST側とする
			ExpressionMessage::LOG_TARGET target = ExpressionMessage::LOG_TARGET::DST;
			boost::optional<uh::tstring> optTarget  = pt.second.get_optional<uh::tstring>(expression_attribute::kTarget);
			if( optTarget )
			{
				target = c->message.toTargetCode(optTarget.get());
				// 定義外の文字列であるなら、異常終了する
				if( ExpressionMessage::LOG_TARGET::UNKNOWN_TARGET == target )
				{
					std::wstringstream ss;
					ss << L" target に未定義な値があります : " << uh::toWStr(optTarget.get());
					notifier::critical::versatile( ss.str() );
				}
			}

			// コード・レベル・対象のいずれかが指定されている場合にのみ、その情報を保持しておく
			if( !code.empty()
				|| !level.empty()
				|| (ExpressionMessage::LOG_TARGET::DST != target) )
				c->message.msgInfo = std::make_shared<ExpressionMessage::LogInfo>( level, code, target );
		}
	}

	return c;
}

bool parse_column( const std::pair<std::wstring,wptree>& v2, IndexList& idxList )
{
	const uh::tstring kValueAttributeDef( _T("<xmlattr>.value"));
	const uh::tstring kNameAttributeDef(  _T("<xmlattr>.name"));
	const uh::tstring kUpdateAttributeDef(_T("<xmlattr>.update"));
	const uh::tstring kTrue(_T("TRUE"));
	const uh::tstring kFalse(_T("FALSE"));
	const uh::tstring kUpdateValueDef(_T("UpdateValue"));
	const uh::tstring kRelationDef(_T("Relation"));

	// DST側
	uh::tstring dstName = v2.second.get<uh::tstring>(kNameAttributeDef);
	std::transform( dstName.begin(), dstName.end(), dstName.begin(), ::toupper ); // 大文字変換
	auto& idxPairList = idxList[dstName];
	// update属性（デフォルトtrue）
	boost::optional<uh::tstring> update = v2.second.get_optional<uh::tstring>(kUpdateAttributeDef);
	SPConditionExpressions exps;
	if( update && eq( update.get(), kFalse ) )
	{
		IndexPair idxPair;
		idxPair.dstName = dstName;
		idxPair.type = IndexPair::ValType::None; // 更新なし
		idxPairList.push_back(std::make_pair(idxPair,exps));
	}
	else {
		// value属性
		// これが定義されているとUpdateValueは無視されるので注意
		boost::optional<uh::tstring> valueAttribute = v2.second.get_optional<uh::tstring>(kValueAttributeDef);
		if( valueAttribute )
		{
			IndexPair idxPair;
			idxPair.dstName = dstName;
			parse_value(valueAttribute.get(), idxPair);
			idxPairList.push_back(std::make_pair(idxPair,exps));
		}
		else {
			bool usedUpdateValue = false, usedRelation = false; // UpdateValueタグが使用されているか、Relationタグが使用されているか
			std::list<pair<IndexPair, SPConditionExpressions>> tempList; // Relationタグの一時格納用
			for( const auto& v3 : v2.second.get_child(_T("")))
			{
				// UpdateValueタグ
				if( eq( v3.first, kUpdateValueDef ) )
				{
					usedUpdateValue = true;
					IndexPair idxPair;
					idxPair.dstName = dstName;
					SPConditionExpressions exps;
					if( !parse_updatevalue(v3, idxPair, exps) )
						return false;

					idxPairList.push_back(std::make_pair(idxPair,exps));
				}
				// Relationタグ
				else if( eq( v3.first, kRelationDef ) )
				{
					usedRelation = true;
					IndexPair idxPair;
					idxPair.dstName = dstName;
					SPConditionExpressions exps;
					if( !parse_relation(v3, idxPair, exps) )
						return false;
					tempList.push_back(std::make_pair(idxPair, exps));
				}
				else if( !isIgnoreTag( v3.first ) )
				{
					std::wstringstream ss;
					ss << _T("未定義なタグ : ") << v3.first;
					notifier::critical::versatile( ss.str() );
					return false;
				}
			}
			// UpdateValueタグを使用していない場合のみ、Relationタグが有効
			if(!usedUpdateValue)
			{
				for( const auto& relation : tempList )
				{
					idxPairList.push_back(relation);
				}
			}
			// UpdateValue、Relationの両方とも使用されている場合、Relationは無視する
			else if(usedRelation)
			{
				std::wstringstream ss;
				ss << kUpdateValueDef << _T("が定義されているため、") << kRelationDef << _T("タグは無視されます:") << idxList.table << _T(".") << dstName;
				notifier::versatile( ss.str() );
			}
		}
	}
	return true;
}

bool parse_updatevalue( const std::pair<std::wstring, boost::property_tree::wptree>& v3, IndexPair& idxPair, SPConditionExpressions& exps )
{
	const uh::tstring kValueAttributeDef(_T("<xmlattr>.value"));
	// <value>読み取り
	parse_value(v3.second.get<uh::tstring>(kValueAttributeDef), idxPair);
	// 条件文読み取り
	if( !getExpressions(v3, exps) )
		return false;
	return true;
}

bool parse_relation(const std::pair<std::wstring, boost::property_tree::wptree>& v3, IndexPair& idxPair, SPConditionExpressions& exps )
{
	const uh::tstring kForeignKeyAttributeDef(_T("<xmlattr>.foreignkey"));
	// <foreignkey>読み取り
	if( !parse_foreignkey(v3.second.get<uh::tstring>(kForeignKeyAttributeDef), idxPair) )
		return false;
	if( !getExpressions(v3, exps) )
		return false;
	return true;
}

void parse_value( const uh::tstring& valueAttribute, IndexPair& idxPair )
{
	// value属性
	ExpressionValue exp;
	exp.expression(valueAttribute);
	switch(exp.type)
	{
	case ExpressionTypeSrc:
		idxPair.type = IndexPair::ValType::RowValue;
		idxPair.srcName = exp.col;
		break;
	case ExpressionTypeOpt:
		idxPair.type = IndexPair::ValType::OptValue;
		idxPair.srcName = exp.col;
		break;
	case ExpressionTypePOD:
		idxPair.type = IndexPair::ValType::PodValue;
		idxPair.val = exp.expstr.c_str();
		break;
	case ExpressionTypeNull:
		idxPair.type = IndexPair::ValType::PodValue;
		idxPair.val.vt = VT_NULL;
		break;
	case ExpressionTypeNow:
		idxPair.type = IndexPair::ValType::Now;
		break;
	case ExpressionTypeDst:
		{
			std::wstringstream ss;
			ss << _T("ColumnタグのvalueにはDSTは指定できません : ") << exp.expstr;
			notifier::critical::versatile( ss.str() );
		}
		break;
	default:
		{
			std::wstringstream ss;
			ss << _T("サポートされていない書式です : ") << exp.expstr;
			notifier::critical::versatile( ss.str() );
		}
		break;
	}
}

bool parse_foreignkey( const uh::tstring& foreignkeyAttribute, IndexPair& idxPair )
{
	uh::tstring foreignkey(foreignkeyAttribute);
	boost::trim(foreignkey);
	// ****.****を見つける
	tsmatch m;
	tregex r( _T(".+\\.\\w+") );
	if( std::regex_match( foreignkey, m, r ) )
	{
		auto vecRet = uh::str_util::split(foreignkey.c_str(), _T("."));
		idxPair.foreignTable = vecRet[0];
		idxPair.foreignField = vecRet[1];
	}
	else
	{
		uh::tstring s = _T("foreignkeyは「<テーブル名>.<フィールド名>」の書式である必要があります。");
		notifier::critical::versatile( s );
		return false; // 規定外のフォーマット
	}

	idxPair.type = IndexPair::ValType::Cache; // キャッシュタイプ
	return true;
}

bool getExpressions( const std::pair<std::wstring, boost::property_tree::wptree>& v3, SPConditionExpressions& exps )
{
	for( const auto& v4 : v3.second.get_child(_T("")))
	{
		SPConditionExpressions expsTmp = parse_condition(v4);
		if( !expsTmp )
			continue;
		
		if( !exps )
			exps = expsTmp;
		else {
			std::wstringstream ss;
			ss << _T("条件を組み合わせる場合は<and>又は<or>タグを使用してください : ") << v4.first;
			notifier::critical::versatile( ss.str() );
			return false;
		}
	}
	return true;
}

bool getRelationField(const IndexList& idxList, std::map<uh::tstring, std::map<uh::tstring, uh::tstring>>& relationFields)
{
	for(const auto& tblInfo : idxList)
	{
		// Column情報
		for(const auto& columnInfo : tblInfo.second)
		{
			// Cacheタイプのみ解析
			if(columnInfo.first.type != IndexPair::ValType::Cache)
				continue;

			// expression取得
			ConditionExpression& cond( *std::static_pointer_cast<ConditionExpression>(columnInfo.second) );
			// どちらもキャッシュタイプは不正
			if(cond.rightValue.type == cond.leftValue.type)
			{
				std::wstringstream ss;
				ss << _T("[Relation]タグのexpressionが不正です：") << tblInfo.first;
				notifier::critical::versatile(ss.str());
				return false;
			}
			else if(cond.rightValue.type == ExpressionType::ExpressionTypeCache)
				relationFields[columnInfo.first.foreignTable][cond.rightValue.col] = columnInfo.first.foreignField;
			else if(cond.leftValue.type == ExpressionType::ExpressionTypeCache)
				relationFields[columnInfo.first.foreignTable][cond.leftValue.col] = columnInfo.first.foreignField;
			else
			{
				std::wstringstream ss;
				ss << _T("[Relation]タグのexpressionに$(CHE.***)がありません：") << tblInfo.first;
				notifier::critical::versatile(ss.str());
				return false;
			}
		}
	}
	return true;
}

/// 無視して良いタグかどうか
bool isIgnoreTag( const uh::tstring& tag )
{
	if( eq( tag, _T("<xmlattr>") ) ||
		eq( tag, _T("<xmlcomment>") ) )
		return true;
	return false;
}


namespace
{

/**
 * @brief ログに出力するメッセージ以外のエラー情報を引数に格納
 * @param[out] errCode  エラーコード格納先
 * @param[out] errLevel エラーレベル格納先
 * @param[in]  msgInfo  エラー情報を保持するインスタンス
 */
void setErrorLogInfo(
	uh::tstring& errCode,
	boost::optional< notifier::error_level >& errLevel,
	const ExpressionMessage::MessageInfoPtr& msgInfo)
{
	if(!msgInfo) return;
	// エラーコードは指定された値をそのまま格納
	errCode = msgInfo->code();

	// 文字列を列挙子に変換
	notifier::error_level level = notifier::getErrorLevel( uh::toStr(msgInfo->level()) );
	// 対象外の文字列であった場合には指定なしと同じ扱いとする
	if(level == notifier::error_level::none) return;
	errLevel = level;
}

/**
 * @brief 条件評価の結果に合わせてメッセージを出力する
 * @param[in] condResult 条件評価の結果
 * @param[in] message    出力するメッセージの情報をもつオブジェクト
 * @param[in] src        比較元
 * @param[in] dst        比較先
 * @param[in] opt        コマンドライン引数
 */
inline void outputMessage(
	bool condResult,
	ExpressionMessage& message,
	const sindy::CSPRowBase& src,
	const sindy::CSPRowBase& dst,
	const Arguments& opt)
{
	if( message.empty() || (condResult !=  message.cond) )
		return;

	switch(message.format)
	{
	case ExpressionMessage::FORMAT::COMMENT:
	default: // フォーマットが未定義ならコメントとして出力
		notifier::versatile( message.string( src, dst, opt ) );
		break;
	case ExpressionMessage::FORMAT::LOG:
		{
			const sindy::CSPRowBase* row = nullptr;
			// 指定に合わせて出力対象を変更
			if(message.msgInfo)
			{
				if( ExpressionMessage::LOG_TARGET::SRC==message.msgInfo->target() )
					row = &src;
				else
					row = &dst;
			}
			// 指定がない場合にはDST側の情報を出力
			else row = &dst;

			if( !(*row) )
			{
				std::wstringstream ss;
				ss << L"NULLを参照しようとしました: Msg[" << message.string( src, dst, opt ) << L"]";
				notifier::versatile(ss.str());
			}
			else
			{
				// エラーコード・エラーレベルが指定されている場合は指定された値を出力
				uh::tstring code;
				boost::optional< notifier::error_level > levelOption;
				setErrorLogInfo(code, levelOption, message.msgInfo);

				// メッシュコードはArgmentsから取得(定義がない場合は空文字列にする)
				uh::tstring mesh = opt.GetSystemValue(Arguments::MESHCODE, _T(""));

				notifier::reportObjInfo(
					(*row)->GetTableName(),
					(*row)->GetOID(),
					nullptr, nullptr,
					levelOption,
					code,
					message.string(src, dst, opt),
					mesh,
					std::wcerr
					);
			}
		}
		break;
	}
}

} // namespace


bool ConditionExpressionContainer::eval( const sindy::CSPRowBase& src, const sindy::CSPRowBase& dst, const Arguments& opt, std::list<uh::tstring>& messages ) const
{
	bool ret = isAndContainer ? true : false; // 返り値
	for( const auto& exp : vecExp )
	{
		if( !exp )
			continue; // 中身のない<and>や<or>の場合

		if( !exp->eval( src, dst, opt, messages ) )
		{
			if( isAndContainer )
			{
				ret = false;
				break;
			}
		}
		else {
			if( !isAndContainer )
			{
				ret = true;
				break;
			}
		}
	}
	// messageの処理
	outputMessage( ret, message, src, dst, opt );
	return ret;
}

// ExpressionValue
// $(SRC.)、$(DST.)が指定された場合に返す定数を定義
// 外部から指定できない形式の文字列として定義する(この文字列を外部ファイル指定するとエラーになる)
const CComVariant ExpressionValue::EXIST_VALUE = _T("$(COLUMN_EMPTY.)");

bool ExpressionValue::expression( const uh::tstring& expStr )
{
	uh::tstring exp(expStr);
	boost::trim(exp);
	// $()を見つける
	tsmatch m;
	tregex r( _T("\\$\\((.*)\\)") );
	//		tregex_token_iterator first( begin(exp), end(exp), r );
	if( std::regex_search( exp, m, r ) )
		//		if( first != tregex_token_iterator() )
	{
		// SRC ... ExpressionTypeSrc
		// DST ... ExpressionTypeDsr
		// OPT ... ExpressionTypeOpt
		// CHE ... ExpressionTypeChe
		//			uh::tstring typeStr = first->str().substr( 0, 4 );
		uh::tstring typeStr = m.str(1).substr( 0, 4 );          // TOOD: それぞれのタイプでクラス化すべき
		if( 0 == _tcsicmp(_T("SRC."), typeStr.c_str()))
			type = ExpressionTypeSrc;
		else if( 0 == _tcsicmp(_T("DST."), typeStr.c_str()))
			type =  ExpressionTypeDst;
		else if( 0 == _tcsicmp(_T("OPT."), typeStr.c_str()))
			type = ExpressionTypeOpt;
		else if( 0 == _tcsicmp(_T("NOW."), typeStr.c_str()))
			type = ExpressionTypeNow;
		else if( 0 == _tcsicmp(_T("CHE."), typeStr.c_str()))
			type = ExpressionTypeCache;
		else {
			std::wstringstream ss;
			ss << _T("expressionは「SRC.」「DST.」「OPT.」「NOW.」「CHE.」で始まる必要があります : ") << exp;
			notifier::critical::versatile( ss.str() );
			return false; // 規定外のフォーマット
		}
		// カラム名
		col = m.str(1).substr(4);
	}
	else {
		if( eq( exp, _T("empty") ) )
			type = ExpressionTypeEmpty;
		else if( eq( exp, _T("null") ) )
		{
			type = ExpressionTypeNull;
			val.vt = VT_NULL;
		}
		else {
			type = ExpressionTypePOD;
			expstr = exp.c_str();
		}
	}
	return true;
}

namespace
{
	/**
	 * @brief CRowBaseのポインタのアドレスを保持するCComVariantを返す
	 * @param[in] row CRowBaseの実体のアドレスをもつインスタンス
	 * @return CRowBaseの実体のアドレスを保持するCComVariant
	 */
	inline CComVariant getRefRowBaseVariant(sindy::CSPRowBase& row)
	{
		CComVariant vari;
		vari.vt = ExpressionValue::VT_GEOMETRY;
		vari.pintVal = (int*)row.get();
		return vari;
	}
} // namespace

CComVariant ExpressionValue::value(VARTYPE vt) const
{
	long idx = -1;
	sindy::CSPRowBase row;
	switch( type )
	{
	case ExpressionTypeSrc:
	case ExpressionTypeDst:
		row = ( ExpressionTypeSrc == type ) ? psrc : pdst;
		if( eq( col, _T("TABLE") ) )
			return CComVariant(row->GetOwnerTableName()); // "TABLE"は特別処理
		if( !row )
			return CComVariant(); // TargetCondition、Convert処理のDST側ループのUpdateConditionでここに来るので、エラーとしない

		// カラムの存在判定
		idx = row->GetFieldMap()->HasField(col.c_str());
		if( 0 > idx )
		{
			// カラムが空文字列なら、存在の有無を判定するための条件式なので特別な値を返す
			if( col.empty() )
				return CComVariant(EXIST_VALUE);

			std::wstringstream ss;
			ss << _T("条件で指定されているカラムが存在しません：") << col;
			notifier::critical::versatile(ss.str());
			return CComVariant();
		}

		// 形状フィールドが指定されていたら、CRowBaseの実体のアドレスを取得
		// (使用するのは形状だけだが、CRowBase::GetShape()は形状がキャッシュされていないとNULLを返す。
		//  CRowBaseを(IGeometry*)でキャストすると、キャッシュがない場合はDBから形状を取得できるので、
		//  CRowBaseの実体のアドレスを持つようにする)
		if( row->GetFieldMap()->GetShapeIndex() == idx )
			return getRefRowBaseVariant(row);

		return row->GetValue(col.c_str());
		break;
	case ExpressionTypeOpt:
		if( VT_EMPTY == val.vt )
		{
			if( vt != VT_NULL && vt != VT_EMPTY )
			{
				val = popt->GetValue(col.c_str());
				if( VT_EMPTY != val.vt )
					val.ChangeType(vt);
				else
				{
					std::wstringstream ss;
					ss << _T("--valueオプションで値がセットされていません：") << col;
					notifier::critical::versatile(ss.str());
					_ASSERTE( VT_EMPTY != val.vt );
				}
			}
			else
				return popt->GetValue(col.c_str()); // 型が決まらないうちはキャッシュに入れない
		}
		else
			return val;
		break;
	case ExpressionTypePOD:
		if( VT_EMPTY == val.vt )
		{
			if( vt != VT_NULL && vt != VT_EMPTY )
			{
				val = expstr.c_str();
				val.ChangeType( vt );
			}
			else
				return CComVariant(expstr.c_str()); // 型が決まらないうちはキャッシュに入れない
		}
		else
			return val;
		break;
	// TODO: CHEの場合を追加
	// CHEの場合は別処理としているため、こちらに入ってこない
	case ExpressionTypeCache:
		break;
	default: _ASSERTE(false); break;
	}
	return val;
}

bool ExpressionValue::isEmpty() const
{
	switch( type )
	{
	case ExpressionTypeSrc: return !psrc; break;
	case ExpressionTypeDst: return !pdst; break;
	case ExpressionTypeEmpty: return true; break; // 条件式だが、Emptyには変わりないので
	default: _ASSERTE(false); break;
	}
	return false;
}

namespace
{
	/**
	 * @brief 引数のいずれかが形状比較用のデータを保持しているか判定
	 * @retval true  形状比較用のデータを保持している
	 * @retval false 形状比較用のデータはもたない
	 */
	inline bool isGeometryCompare(const CComVariant& left, const CComVariant& right)
	{
		return (left.vt==ExpressionValue::VT_GEOMETRY
			|| right.vt==ExpressionValue::VT_GEOMETRY);
	}

	/**
	 * @brief CComVariantから形状を取得
	 * @param[in] vari 形状をもつCComVariant
	 * @return 形状
	 */
	inline IGeometryPtr VariToGeometry(const CComVariant& vari)
	{
		_ASSERTE( vari.vt==ExpressionValue::VT_GEOMETRY );
		return (IGeometry*)(*reinterpret_cast<sindy::CRowBase*>(vari.pintVal));
	}

	/**
	 * @brief 引数がもつ形状情報を比較演算子に従って比較する
	 * @param[in] left     左辺値
	 * @param[in] right    右辺値
	 * @param[in] compOpe  比較演算子
	 * @retval true  形状が一致している
	 * @retval false 形状が一致していない
	 */
	inline bool geometryCompare(const CComVariant& left,
		const CComVariant& right,
		ExpressionOperator::EnumOperator compOpe)
	{
		/// 形状比較条件の識別子を列挙
		enum GeometryComp : int
		{
			EQUALS,
			NOT_EQUALS,
			IS,
			IS_NOT,
			IS_NULL,
			IS_NOT_NULL,
			IS_EMPTY,
			IS_NOT_EMPTY,
		};

		// 左辺値・オペレータ・右辺値組合せを定義(ここにある組合せ以外はエラー)
		// <左辺値のVARENUM, オペレータ, 右辺値のVARENUM>
		const std::map< GeometryComp, std::tuple<VARTYPE, ExpressionOperator::EnumOperator, VARTYPE> > allow = boost::assign::map_list_of
			// IGeometryPtr == IGeometryPtr
			(GeometryComp::EQUALS, std::make_tuple(ExpressionValue::VT_GEOMETRY, ExpressionOperator::OperatorEqual, ExpressionValue::VT_GEOMETRY))
			// IGeometryPtr != IGeometryPtr
			(GeometryComp::NOT_EQUALS, std::make_tuple(ExpressionValue::VT_GEOMETRY, ExpressionOperator::OperatorNotEqual, ExpressionValue::VT_GEOMETRY))
			// IGeometryPtr is IGeometryPtr
			(GeometryComp::IS, std::make_tuple(ExpressionValue::VT_GEOMETRY, ExpressionOperator::OperatorIs, ExpressionValue::VT_GEOMETRY))
			// IGeometryPtr is not IGeometryPtr
			(GeometryComp::IS_NOT, std::make_tuple(ExpressionValue::VT_GEOMETRY, ExpressionOperator::OperatorIsNot, ExpressionValue::VT_GEOMETRY))

			/* TODO: NULL・空形状の判定が必要になったなら、この部分のコメントアウトを外す
					 十分なテストデータを用意することができない&要望がないため、この機能の実装は見送る
			// IGeometryPtr is null
			(GeometryComp::IS_NULL, std::make_tuple(ExpressionValue::USER_VT_GEOMETRY, ExpressionOperator::OperatorIs, VT_NULL))
			// IGeometryPtr is not null
			(GeometryComp::IS_NOT_NULL, std::make_tuple(ExpressionValue::USER_VT_GEOMETRY, ExpressionOperator::OperatorIsNot, VT_NULL))
			// IGeometryPtr is empty
			(GeometryComp::IS_EMPTY, std::make_tuple(ExpressionValue::USER_VT_GEOMETRY, ExpressionOperator::OperatorIs, VT_EMPTY))
			// IGeometryPtr is not empty
			(GeometryComp::IS_NOT_EMPTY, std::make_tuple(ExpressionValue::USER_VT_GEOMETRY, ExpressionOperator::OperatorIsNot, VT_EMPTY))
			*/
			;

		auto it = std::find_if(allow.cbegin(), allow.cend(),
			[&]( const std::pair<GeometryComp, std::tuple<VARTYPE, ExpressionOperator::EnumOperator, VARTYPE>>& allowPair)
			{
				return (std::get<0>(allowPair.second) == left.vt)
					&& (std::get<1>(allowPair.second) == compOpe)
					&& (std::get<2>(allowPair.second) == right.vt);
			});

		// 比較演算子の指定が誤っていたら、エラーを出力して異常終了する
		if(it==allow.cend())
		{
			std::wstringstream ss;
			ss << L"形状比較条件式が誤っています";
			notifier::critical::versatile(ss.str());
		}

		switch(it->first)
		{
		case GeometryComp::EQUALS:
		case GeometryComp::IS:
			return TRUE==AheIsEqual(VariToGeometry(left), VariToGeometry(right));
			break;
		case GeometryComp::NOT_EQUALS:
		case GeometryComp::IS_NOT:
			return FALSE==AheIsEqual(VariToGeometry(left), VariToGeometry(right));
			break;
		/* TODO: NULL・空形状の判定が必要になったなら、この部分のコメントアウトを外す
				 十分なテストデータを用意することができない&要望がないため、この機能の実装は見送る
		case GeometryComp::IS_NULL:
			return (nullptr==VariToGeometry(left));
			break;
		case GeometryComp::IS_NOT_NULL:
			return (nullptr!=VariToGeometry(left));
			break;
		case GeometryComp::IS_EMPTY:
			{
				// TODO: ここはよくよく検討の上、実装のこと
				// ShapeファイルではNULL不許可なので、is emptyはtrue
				// → 同じ設定ファイルをSDEに実行したら nullptr許可なのでfalse
				// 同じデータなのに結果が違う! とかにならないか？
				IGeometryPtr ipCheckedGeometry(VariToGeometry(left));

				// nullは空ではないのでfalseとしている
				if(!ipCheckedGeometry) return false;
				// TODO: "空"の基準をツール内で統一するか検討
				// (統一すると仕様書書くときに非常に助かる)
				// 下記関数はnullも空として判定するので注意
				return IsEmpty(ipCheckedGeometry);
			}
			break;
		case GeometryComp::IS_NOT_EMPTY:
			{
				// TODO: IS_EMPTYの内容に合わせて修正すること
				IGeometryPtr ipCheckedGeometry(VariToGeometry(left));
				// nullは空ではないのでfalseとしている
				if(!ipCheckedGeometry) return false;
				return !IsEmpty(ipCheckedGeometry);
			}
			break;
		*/
		default:
			_ASSERTE(false); break;
		}
		return false;
	}
} // namespace

bool ExpressionOperator::eval( const ExpressionValue& left, const ExpressionValue& right ) const
{
	if( EnumOperator::OperatorUnknown == ope )
		notifier::critical::versatile(_T("条件式のオペレータが解釈できませんでした"));

	// is empty の処理
	// どちらかがExpressionTypeEmptyであること前提
	if( ExpressionTypeEmpty == left.type || ExpressionTypeEmpty == right.type )
	{
		switch(ope)
		{
		case EnumOperator::OperatorIs:
			return (ExpressionTypeEmpty ==  left.type &&  right.isEmpty())
				|| (ExpressionTypeEmpty == right.type && left.isEmpty() );
			break;
		case EnumOperator::OperatorIsNot:
			return (ExpressionTypeEmpty ==  left.type &&  !right.isEmpty())
				|| (ExpressionTypeEmpty == right.type && !left.isEmpty() );
			break;
		// この他の演算子は設定ファイル読込み時に排斥しているはず
		default: _ASSERTE(false); break;
		}
		return false;
	}

	CComVariant vaLeft, vaRight;
	// Opt型、POD型は初めは型がわからないため、value(VARTYPE)で
	// 型が決まるようになっている（中でChangeTypeしている）
	// RowBaseから取得する際は、VARTYPEは無視しているので、まず
	// RowBaseの方の値を取り、そのVARTYPEでChangeTypeした値を
	// 取得する。
	// ちなみに、RowBaseの値がNULLの場合はその次の時にChangeTypeする
	if( left.type == ExpressionTypeOpt || left.type == ExpressionTypePOD )
	{
		vaRight = right.value(VT_I4);
		vaLeft = left.value(vaRight.vt);
	}
	else if( right.type == ExpressionTypeOpt || right.type == ExpressionTypePOD )
	{
		vaLeft = left.value(VT_I4);
		vaRight = right.value(vaLeft.vt);
	}
	else {
		if( left.type == ExpressionTypeNull )
			vaLeft.vt = VT_NULL;
		else
			vaLeft = left.value(VT_I4);
		if( right.type == ExpressionTypeNull )
			vaRight.vt = VT_NULL;
		else
			vaRight = right.value(VT_I4);
	}

	// 比較対象が形状なら、専用の処理で判定する
	if( isGeometryCompare(vaLeft, vaRight) )
		return geometryCompare(vaLeft, vaRight, ope);

	// 型を揃える
	if( vaLeft.vt != vaRight.vt )
	{
		if( ( vaLeft.vt == VT_R4 || vaLeft.vt == VT_R8 ) && ( vaRight.vt == VT_I2 || vaRight.vt == VT_I4 ) )
			vaRight.ChangeType( vaLeft.vt );
		else if( ( vaLeft.vt == VT_I2 || vaLeft.vt == VT_I4 ) && ( vaRight.vt == VT_R4 || vaRight.vt == VT_R8 ) )
			vaLeft.ChangeType( vaRight.vt );
	}
	switch( ope )
	{
	case EnumOperator::OperatorIs:
	case EnumOperator::OperatorEqual:        return vaLeft == vaRight; break;
	case EnumOperator::OperatorEqualBigger:  return (vaLeft < vaRight || vaLeft == vaRight); break;
	case EnumOperator::OperatorEqualSmaller: return (vaLeft > vaRight || vaLeft == vaRight); break;
	case EnumOperator::OperatorBigger:       return vaLeft < vaRight; break;
	case EnumOperator::OperatorSmaller:      return vaLeft > vaRight; break;
	case EnumOperator::OperatorIsNot:
	case EnumOperator::OperatorNotEqual:     return vaLeft != vaRight; break;
	default: break;
	}
	return false;
}

bool ConditionExpression::eval( const sindy::CSPRowBase& src, const sindy::CSPRowBase& dst, const Arguments& opt, std::list<uh::tstring>& /*messages*/ ) const
{
	leftValue.Attach(src, dst, opt);
	rightValue.Attach(src, dst, opt);
	bool ret = expOperator.eval( leftValue, rightValue );
	leftValue.Detach();
	rightValue.Detach();
	// messageの処理
	outputMessage(ret, message, src, dst, opt);
	return ret;
}

bool ExpressionMessage::init( const uh::tstring& message )
{
	// プレースフォルダと置き換え後の文字列を作成する
	tsmatch m;
	tregex r( _T("\\$\\(.*?\\)") ); // 「$()」の最短一致
	auto first = message.begin();
	auto last = message.end();
	while( std::regex_search( first, last, m, r ) )
	{
		uh::tstring exp = m.str();
		holders[exp].expression(exp);
		first = m[0].second;
	}
	msg = message;

	return true;
}

// TODO: 似た処理が続くので、共通化を検討
ExpressionMessage::FORMAT ExpressionMessage::toFormatCode( const uh::tstring& str )
{
	// TODO: メッセージの出力形式が増えるなら、下記に要素を足す
	const std::map<const _TCHAR*, FORMAT> cnvMap = boost::assign::map_list_of
		(_T("comment"),FORMAT::COMMENT)
		(_T("log"),FORMAT::LOG)
		;

	auto it = std::find_if(cnvMap.cbegin(), cnvMap.cend(),
		[&str](const std::pair<const _TCHAR*,FORMAT>& mapPair)
		{
			return ( 0==_tcsicmp(mapPair.first, str.c_str()) );
		} );
	if( cnvMap.cend()==it ) return FORMAT::UNKNOWN_FORMAT;
	return it->second;
}

ExpressionMessage::LOG_TARGET ExpressionMessage::toTargetCode( const uh::tstring& str )
{
	// TODO: ログへ出力する対象が増えるなら、下記に要素を足す
	const std::map<const _TCHAR*, LOG_TARGET> cnvMap = boost::assign::map_list_of
		(_T("DST"),LOG_TARGET::DST)
		(_T("SRC"),LOG_TARGET::SRC)
		;

	auto it = std::find_if(cnvMap.cbegin(), cnvMap.cend(),
		[&str](const std::pair<const _TCHAR*,LOG_TARGET>& mapPair)
		{
			return ( 0==_tcsicmp(mapPair.first, str.c_str()) );
		} );
	if( cnvMap.cend()==it ) return LOG_TARGET::UNKNOWN_TARGET;
	return it->second;
}

uh::tstring ExpressionMessage::string( const sindy::CSPRowBase& src, const sindy::CSPRowBase& dst, const Arguments& opt )
{
	if( msg.empty() )
		return msg;

	Attach( src, dst, opt );

	uh::tstring ret = msg;
	for( auto& holder : holders )
	{
		holder.second.Attach( psrc, pdst, *popt );
		CComVariant va = holder.second.value(VT_BSTR); // PODじゃないとVT指定は効かないので注意！！
		va.ChangeType(VT_BSTR);
		uh::tstring valstr = ( VT_NULL == va.vt ) ? _T("<null>") : va.bstrVal;
		auto pos = ret.find( holder.first, 0 );
		while( pos != uh::tstring::npos )
		{
			ret.replace( pos, holder.first.length(), valstr );
			pos = ret.find( holder.first, 0 );
		}
		holder.second.Detach();
	}

	Detach();

	return ret;
}

CComVariant IndexPair::value( const sindy::CRowBase& src ) const
{
	switch(type)
	{
	case RowValue:
	// Cacheの場合はひとまず、srcの値を格納し、後処理でsrcの値から挿げ替える
	case Cache:
		if( 0 <= srcIdx )
			return src.GetValue(srcIdx);
		break;
	case Now:
		val = (LPCTSTR)winutil::GetTime();
		break;
	case PodValue:
	default:
		break;
	}
	return val;
}
