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

// TODO: Utility�̍\��������������
// �e�����ŋ��ʂɎg������̂Ə����Ǝ��̂��̂�Utility�𕪂���
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

// ��������͊֐��p�̒萔���`
namespace condition_constant
{
	// �^�O���̒�`
	namespace tag
	{
		const uh::tstring kAnd(_T("And"));
		const uh::tstring kOr(_T("Or"));
		const uh::tstring kExpression(_T("expression"));
	} // tag

	// expression�^�O�̑������̒�`
	namespace expression_attribute
	{
		const uh::tstring kValue(_T("<xmlattr>.value"));		       //!< �������̑�����
		const uh::tstring kMessage(_T("<xmlattr>.message"));	       //!< �������ɍ���Ȃ��ꍇ�̃��b�Z�[�W�������i�݊����ێ��p�j
		const uh::tstring kFalseMessage(_T("<xmlattr>.FalseMessage")); //!< �������ɍ���Ȃ��ꍇ�̃��b�Z�[�W������
		const uh::tstring kTrueMessage(_T("<xmlattr>.TrueMessage"));   //!< �������ɍ����ꍇ�̃��b�Z�[�W������
		const uh::tstring kFormat(_T("<xmlattr>.format"));             //!< ���b�Z�[�W�o�͌`���̑�����
		const uh::tstring kErrorLevel(_T("<xmlattr>.level"));          //!< ���b�Z�[�W�o�͎��̃G���[���x���̑�����
		const uh::tstring kErrorCode(_T("<xmlattr>.code"));            //!< ���b�Z�[�W�o�͎��̃G���[�R�[�h�̑�����
		const uh::tstring kTarget(_T("<xmlattr>.target"));             //!< ���b�Z�[�W�o�͎��̃G���[�R�[�h�̑�����
	} // expression_attribute

} // condition_constant

namespace
{
	// TODO: �\���`�F�b�N�p�̃N���X�ɂ��邱�Ƃ�����
	/**
	 * @brief ���R�[�h�̑��ݔ����Empty�ȊO���g�p���Ă��Ȃ����m�F����
	 * @warning �E�Ӓl�ɑ΂��āA���Ӓl���Ó������肵�����ꍇ�ɂ́A
	 * left, right�ɓn���������t�ɂ��ČĂяo��
	 * @param[in] left  ���Ӓl
	 * @param[in] right �E�Ӓl
	 * @retval true  NULL���g�p���Ă��Ȃ�
	 * @retval false NULL���g�p���Ă���
	 */
	bool checkRecordCompareEmpty(const ExpressionValue& left, const ExpressionValue& right)
	{
		// �J��������łȂ����́A�^�C�v��Src,Dst�łȂ����͔̂���̕K�v�Ȃ�
		if( !left.col.empty() ) return true;
		if( left.type != ExpressionTypeSrc && left.type != ExpressionTypeDst ) return true;

		// ���L�͊J�������̗v����`�̓��e�ƈقȂ�̂ŁA�R�����g�A�E�g
		// �J�������A$(SRC.) <> $(DST.) �́A"�`��E�����̂����ꂩ���قȂ�Ƃ��^"�Ƃ��������������B
		// ���݁A��L�̂悤�Ȕ�����s�������͂Ȃ��A��L�������w�肳��Ă��܂����ꍇ�A
		// �g�p�҂��Ӑ}���Ȃ������������N�����\��������̂ŁA���̂悤�ȓ��͂͋����Ȃ����Ƃɂ���B
		// TODO: $(SRC.) <> $(DST.)�̔��肪�������ꂽ��A���̃G���[�`�F�b�N�͌�����
		//if( right.col.empty() && (right.type==ExpressionTypeSrc || right.type==ExpressionTypeDst) )
		//	return true;

		// ���R�[�h�̑��ݔ���� empty �ȊO�͋����Ȃ�
		return right.type==ExpressionTypeEmpty;
	}
	/**
	 * @brief Empty�̔���ɑ΂��Đ����ȉ��Z�q���g�p���Ă��邩�m�F����
	 * @param[in] left  ���Ӓl
	 * @param[in] right �E�Ӓl
	 * @param[in] ope   ��r���Z�q
	 * @retval true  ���Z�q�͐����ł���
	 * @retval false �s���ȉ��Z�q�ł���
	 */
	bool checkEmptyOperator(const ExpressionValue& left, const ExpressionValue& right, const ExpressionOperator& ope)
	{
		// �E���Ӓl�̂������Empty�łȂ��Ȃ�A����̕K�v�Ȃ�
		if( left.type != ExpressionTypeEmpty && right.type != ExpressionTypeEmpty ) return true;

		// �I�y���[�^��is, is not�ȊO�͋����Ȃ�
		return ( ope.ope==ope.OperatorIs || ope.ope==ope.OperatorIsNot );
	}

	/**
	 * @brief �E���Ӓl�̑g���������e�������̂��m�F
	 * @detail ��肪�������ꍇ�ɂ́A��O�����Ƃ��ăG���[���b�Z�[�W���o�͂���
	 * @note �����ł͌`�󔻒�Ɋւ���\���`�F�b�N���s��Ȃ��B<br>
	 * �ݒ�t�@�C���̓Ǎ��ݒi�K�ł́A
	 * �t�B�[���h�������ŁA���ꂪ�`��t�B�[���h���ǂ������f�ł��Ȃ����߂ł���B<br>
	 * (�Ȃ��AiPC�ł́A��{�I�Ɍ`��t�B�[���h��SHAPE�Ƃ������O���g�p���邱�Ƃ����܂��Ă��邽�߁A
	 * iPC���̃f�[�^�����������Ȃ�A���̎��_�Ŕ���\�ł���B
	 * �����A�O����Shape�f�[�^�A���ɊC�O��Shape�f�[�^����荞�݂����A�Ƃ������v�]���������ꍇ�ɁA
	 * �`��t�B�[���h����SHAPE�ł��邱�Ƃ͕ۏႳ��Ȃ��̂ŁA���ߑł��ɂ͂��Ȃ��j
	 * @param[in] left  ���Ӓl
	 * @param[in] right �E�Ӓl
	 * @param[in] ope   ��r���Z�q
	 * @retval true  �g�����͋��e�����
	 * @retval false �g�����͋��e����Ȃ�
	 */
	void checkAllowCondition(const ExpressionValue& left, const ExpressionValue& right, const ExpressionOperator& ope )
	{
		if(!checkRecordCompareEmpty(left, right)
		|| !checkRecordCompareEmpty(right, left) )
		{
			notifier::critical::versatile(L"$(SRC.)�A$(DST.) �� is empty, is not empty �ł�������ł��܂���");
		}

		if( !checkEmptyOperator(left, right, ope) )
		{
			notifier::critical::versatile(L"empty�̔��莮�� is, is not �ȊO���g�p���邱�Ƃ͂ł��܂���");
		}
	}
} // namespace

SPConditionExpressions parse_condition( const std::pair<std::wstring,wptree>& pt )
{
	using namespace condition_constant;

	SPConditionExpressions c;
	// <or> �Ƃ� <and>
	if( eq( pt.first, tag::kAnd ) || eq( pt.first, tag::kOr ) )
	{
		// TODO: ConditionExpressionContainer�Ɏ����čs���Ă��悢
		c.reset( new ConditionExpressionContainer(eq( pt.first, tag::kAnd)) );
		for( const auto& v : pt.second.get_child(_T("")) )
			static_pointer_cast<ConditionExpressionContainer>(c)->vecExp.push_back( parse_condition(v) );
	}
	// <expression value="$(SRC.ORG_OBJID) = $(DST.OBJECTID)" /> �Ƃ�
	else if( eq( pt.first, tag::kExpression ) ) {
		// parse_expression�Ƃ��֐���
		// TODO: ConditionExpression�ɂ����Ă����Ă��悢
		c.reset( new ConditionExpression() );
		ConditionExpression& cond( *static_pointer_cast<ConditionExpression>(c) );
		uh::tstring exp = pt.second.get<uh::tstring>(expression_attribute::kValue);
		// ���Z�q�ŁA�E�ƍ��ɕ�������
		std::list<uh::tstring> listOperator = boost::assign::list_of(_T("<>"))(_T("!="))(_T("<="))(_T(">="))(_T("="))(_T(">"))(_T("<"))(_T("is not"))(_T("is")); // ���Ԓ��ӁI�I
		uh::tstring::size_type pos = uh::tstring::npos;
		for( const auto& ope : listOperator )
		{
			if( uh::tstring::npos == ( pos = exp.find(ope) ) )
				continue;

			cond.expOperator.init(ope);
			// �E�ƍ��ɕ���
			if( !cond.leftValue.expression(exp.substr( 0, pos )) )
				return SPConditionExpressions();
			cond.rightValue.expression(exp.substr( pos+ope.size() ));

			// �E�Ӓl�E���Ӓl�E���Z�q�̑g�����ɕs�����Ȃ����m�F
			checkAllowCondition( cond.leftValue, cond.rightValue, ope );
			break;
		}
	}
	// message
	if( c )
	{
		// �m�[�h����w��̑����l���擾�ł����ꍇ�Ɏw��̏��������s����
		// ���b�Z�[�W���m�F
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

		// TODO: ���L�̃��O�t�H�[�}�b�g�̓Ǎ��ݕ��E���b�Z�[�W�̏o�͕��͊֐�������������
		// ���O�t�H�[�}�b�g���m�F
		boost::optional<uh::tstring> optFormat  = pt.second.get_optional<uh::tstring>(expression_attribute::kFormat);
		if( optFormat )
		{
			auto formatCode = c->message.toFormatCode(optFormat.get());
			// �������񋓎q�ɕϊ����Ċi�[
			if( ExpressionMessage::FORMAT::UNKNOWN_FORMAT == formatCode )
			{
				// ��`�O�̗񋓎q�Ȃ�A�ُ�I������
				std::wstringstream ss;
				ss << L" format �ɖ���`�̒l������܂� : " << uh::toWStr(optFormat.get());
				notifier::critical::versatile( ss.str() );
			}
			c->message.format = formatCode;
		}

		// �ȉ��̓��O�t�H�[�}�b�g���w�肳��Ă����ꍇ�ɂ̂݁A�Ǎ���
		if( ExpressionMessage::FORMAT::LOG == c->message.format )
		{
			// SINDYSTDLOG�`���ŏo�͂���ꍇ�̃G���[�R�[�h���擾
			uh::tstring code;
			boost::optional<uh::tstring> optCode  = pt.second.get_optional<uh::tstring>(expression_attribute::kErrorCode);
			if( optCode )
				code = optCode.get();

			// SINDYSTDLOG�`���ŏo�͂���ꍇ�̃G���[���x�����擾
			uh::tstring level;
			boost::optional<uh::tstring> optLevel  = pt.second.get_optional<uh::tstring>(expression_attribute::kErrorLevel);
			if( optLevel )
			{
				notifier::error_level levelCode = notifier::getErrorLevel( uh::toStr(optLevel.get()) );
				// �񋓎q����`����Ă��Ȃ�������ł���Ȃ�A�ُ�I������
				if( notifier::error_level::none == levelCode )
				{
					std::wstringstream ss;
					ss << L" level �ɖ���`�Ȓl������܂� : " << uh::toWStr(optLevel.get());
					notifier::critical::versatile( ss.str() );
				}
				level = optLevel.get();
			}

			// ���O�ɏ����o�͂���Ώۂ��擾
			// �f�t�H���g�l��DST���Ƃ���
			ExpressionMessage::LOG_TARGET target = ExpressionMessage::LOG_TARGET::DST;
			boost::optional<uh::tstring> optTarget  = pt.second.get_optional<uh::tstring>(expression_attribute::kTarget);
			if( optTarget )
			{
				target = c->message.toTargetCode(optTarget.get());
				// ��`�O�̕�����ł���Ȃ�A�ُ�I������
				if( ExpressionMessage::LOG_TARGET::UNKNOWN_TARGET == target )
				{
					std::wstringstream ss;
					ss << L" target �ɖ���`�Ȓl������܂� : " << uh::toWStr(optTarget.get());
					notifier::critical::versatile( ss.str() );
				}
			}

			// �R�[�h�E���x���E�Ώۂ̂����ꂩ���w�肳��Ă���ꍇ�ɂ̂݁A���̏���ێ����Ă���
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

	// DST��
	uh::tstring dstName = v2.second.get<uh::tstring>(kNameAttributeDef);
	std::transform( dstName.begin(), dstName.end(), dstName.begin(), ::toupper ); // �啶���ϊ�
	auto& idxPairList = idxList[dstName];
	// update�����i�f�t�H���gtrue�j
	boost::optional<uh::tstring> update = v2.second.get_optional<uh::tstring>(kUpdateAttributeDef);
	SPConditionExpressions exps;
	if( update && eq( update.get(), kFalse ) )
	{
		IndexPair idxPair;
		idxPair.dstName = dstName;
		idxPair.type = IndexPair::ValType::None; // �X�V�Ȃ�
		idxPairList.push_back(std::make_pair(idxPair,exps));
	}
	else {
		// value����
		// ���ꂪ��`����Ă����UpdateValue�͖��������̂Œ���
		boost::optional<uh::tstring> valueAttribute = v2.second.get_optional<uh::tstring>(kValueAttributeDef);
		if( valueAttribute )
		{
			IndexPair idxPair;
			idxPair.dstName = dstName;
			parse_value(valueAttribute.get(), idxPair);
			idxPairList.push_back(std::make_pair(idxPair,exps));
		}
		else {
			bool usedUpdateValue = false, usedRelation = false; // UpdateValue�^�O���g�p����Ă��邩�ARelation�^�O���g�p����Ă��邩
			std::list<pair<IndexPair, SPConditionExpressions>> tempList; // Relation�^�O�̈ꎞ�i�[�p
			for( const auto& v3 : v2.second.get_child(_T("")))
			{
				// UpdateValue�^�O
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
				// Relation�^�O
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
					ss << _T("����`�ȃ^�O : ") << v3.first;
					notifier::critical::versatile( ss.str() );
					return false;
				}
			}
			// UpdateValue�^�O���g�p���Ă��Ȃ��ꍇ�̂݁ARelation�^�O���L��
			if(!usedUpdateValue)
			{
				for( const auto& relation : tempList )
				{
					idxPairList.push_back(relation);
				}
			}
			// UpdateValue�ARelation�̗����Ƃ��g�p����Ă���ꍇ�ARelation�͖�������
			else if(usedRelation)
			{
				std::wstringstream ss;
				ss << kUpdateValueDef << _T("����`����Ă��邽�߁A") << kRelationDef << _T("�^�O�͖�������܂�:") << idxList.table << _T(".") << dstName;
				notifier::versatile( ss.str() );
			}
		}
	}
	return true;
}

bool parse_updatevalue( const std::pair<std::wstring, boost::property_tree::wptree>& v3, IndexPair& idxPair, SPConditionExpressions& exps )
{
	const uh::tstring kValueAttributeDef(_T("<xmlattr>.value"));
	// <value>�ǂݎ��
	parse_value(v3.second.get<uh::tstring>(kValueAttributeDef), idxPair);
	// �������ǂݎ��
	if( !getExpressions(v3, exps) )
		return false;
	return true;
}

bool parse_relation(const std::pair<std::wstring, boost::property_tree::wptree>& v3, IndexPair& idxPair, SPConditionExpressions& exps )
{
	const uh::tstring kForeignKeyAttributeDef(_T("<xmlattr>.foreignkey"));
	// <foreignkey>�ǂݎ��
	if( !parse_foreignkey(v3.second.get<uh::tstring>(kForeignKeyAttributeDef), idxPair) )
		return false;
	if( !getExpressions(v3, exps) )
		return false;
	return true;
}

void parse_value( const uh::tstring& valueAttribute, IndexPair& idxPair )
{
	// value����
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
			ss << _T("Column�^�O��value�ɂ�DST�͎w��ł��܂��� : ") << exp.expstr;
			notifier::critical::versatile( ss.str() );
		}
		break;
	default:
		{
			std::wstringstream ss;
			ss << _T("�T�|�[�g����Ă��Ȃ������ł� : ") << exp.expstr;
			notifier::critical::versatile( ss.str() );
		}
		break;
	}
}

bool parse_foreignkey( const uh::tstring& foreignkeyAttribute, IndexPair& idxPair )
{
	uh::tstring foreignkey(foreignkeyAttribute);
	boost::trim(foreignkey);
	// ****.****��������
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
		uh::tstring s = _T("foreignkey�́u<�e�[�u����>.<�t�B�[���h��>�v�̏����ł���K�v������܂��B");
		notifier::critical::versatile( s );
		return false; // �K��O�̃t�H�[�}�b�g
	}

	idxPair.type = IndexPair::ValType::Cache; // �L���b�V���^�C�v
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
			ss << _T("������g�ݍ��킹��ꍇ��<and>����<or>�^�O���g�p���Ă������� : ") << v4.first;
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
		// Column���
		for(const auto& columnInfo : tblInfo.second)
		{
			// Cache�^�C�v�̂݉��
			if(columnInfo.first.type != IndexPair::ValType::Cache)
				continue;

			// expression�擾
			ConditionExpression& cond( *std::static_pointer_cast<ConditionExpression>(columnInfo.second) );
			// �ǂ�����L���b�V���^�C�v�͕s��
			if(cond.rightValue.type == cond.leftValue.type)
			{
				std::wstringstream ss;
				ss << _T("[Relation]�^�O��expression���s���ł��F") << tblInfo.first;
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
				ss << _T("[Relation]�^�O��expression��$(CHE.***)������܂���F") << tblInfo.first;
				notifier::critical::versatile(ss.str());
				return false;
			}
		}
	}
	return true;
}

/// �������ėǂ��^�O���ǂ���
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
 * @brief ���O�ɏo�͂��郁�b�Z�[�W�ȊO�̃G���[���������Ɋi�[
 * @param[out] errCode  �G���[�R�[�h�i�[��
 * @param[out] errLevel �G���[���x���i�[��
 * @param[in]  msgInfo  �G���[����ێ�����C���X�^���X
 */
void setErrorLogInfo(
	uh::tstring& errCode,
	boost::optional< notifier::error_level >& errLevel,
	const ExpressionMessage::MessageInfoPtr& msgInfo)
{
	if(!msgInfo) return;
	// �G���[�R�[�h�͎w�肳�ꂽ�l�����̂܂܊i�[
	errCode = msgInfo->code();

	// �������񋓎q�ɕϊ�
	notifier::error_level level = notifier::getErrorLevel( uh::toStr(msgInfo->level()) );
	// �ΏۊO�̕�����ł������ꍇ�ɂ͎w��Ȃ��Ɠ��������Ƃ���
	if(level == notifier::error_level::none) return;
	errLevel = level;
}

/**
 * @brief �����]���̌��ʂɍ��킹�ă��b�Z�[�W���o�͂���
 * @param[in] condResult �����]���̌���
 * @param[in] message    �o�͂��郁�b�Z�[�W�̏������I�u�W�F�N�g
 * @param[in] src        ��r��
 * @param[in] dst        ��r��
 * @param[in] opt        �R�}���h���C������
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
	default: // �t�H�[�}�b�g������`�Ȃ�R�����g�Ƃ��ďo��
		notifier::versatile( message.string( src, dst, opt ) );
		break;
	case ExpressionMessage::FORMAT::LOG:
		{
			const sindy::CSPRowBase* row = nullptr;
			// �w��ɍ��킹�ďo�͑Ώۂ�ύX
			if(message.msgInfo)
			{
				if( ExpressionMessage::LOG_TARGET::SRC==message.msgInfo->target() )
					row = &src;
				else
					row = &dst;
			}
			// �w�肪�Ȃ��ꍇ�ɂ�DST���̏����o��
			else row = &dst;

			if( !(*row) )
			{
				std::wstringstream ss;
				ss << L"NULL���Q�Ƃ��悤�Ƃ��܂���: Msg[" << message.string( src, dst, opt ) << L"]";
				notifier::versatile(ss.str());
			}
			else
			{
				// �G���[�R�[�h�E�G���[���x�����w�肳��Ă���ꍇ�͎w�肳�ꂽ�l���o��
				uh::tstring code;
				boost::optional< notifier::error_level > levelOption;
				setErrorLogInfo(code, levelOption, message.msgInfo);

				// ���b�V���R�[�h��Argments����擾(��`���Ȃ��ꍇ�͋󕶎���ɂ���)
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
	bool ret = isAndContainer ? true : false; // �Ԃ�l
	for( const auto& exp : vecExp )
	{
		if( !exp )
			continue; // ���g�̂Ȃ�<and>��<or>�̏ꍇ

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
	// message�̏���
	outputMessage( ret, message, src, dst, opt );
	return ret;
}

// ExpressionValue
// $(SRC.)�A$(DST.)���w�肳�ꂽ�ꍇ�ɕԂ��萔���`
// �O������w��ł��Ȃ��`���̕�����Ƃ��Ē�`����(���̕�������O���t�@�C���w�肷��ƃG���[�ɂȂ�)
const CComVariant ExpressionValue::EXIST_VALUE = _T("$(COLUMN_EMPTY.)");

bool ExpressionValue::expression( const uh::tstring& expStr )
{
	uh::tstring exp(expStr);
	boost::trim(exp);
	// $()��������
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
		uh::tstring typeStr = m.str(1).substr( 0, 4 );          // TOOD: ���ꂼ��̃^�C�v�ŃN���X�����ׂ�
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
			ss << _T("expression�́uSRC.�v�uDST.�v�uOPT.�v�uNOW.�v�uCHE.�v�Ŏn�܂�K�v������܂� : ") << exp;
			notifier::critical::versatile( ss.str() );
			return false; // �K��O�̃t�H�[�}�b�g
		}
		// �J������
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
	 * @brief CRowBase�̃|�C���^�̃A�h���X��ێ�����CComVariant��Ԃ�
	 * @param[in] row CRowBase�̎��̂̃A�h���X�����C���X�^���X
	 * @return CRowBase�̎��̂̃A�h���X��ێ�����CComVariant
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
			return CComVariant(row->GetOwnerTableName()); // "TABLE"�͓��ʏ���
		if( !row )
			return CComVariant(); // TargetCondition�AConvert������DST�����[�v��UpdateCondition�ł����ɗ���̂ŁA�G���[�Ƃ��Ȃ�

		// �J�����̑��ݔ���
		idx = row->GetFieldMap()->HasField(col.c_str());
		if( 0 > idx )
		{
			// �J�������󕶎���Ȃ�A���݂̗L���𔻒肷�邽�߂̏������Ȃ̂œ��ʂȒl��Ԃ�
			if( col.empty() )
				return CComVariant(EXIST_VALUE);

			std::wstringstream ss;
			ss << _T("�����Ŏw�肳��Ă���J���������݂��܂���F") << col;
			notifier::critical::versatile(ss.str());
			return CComVariant();
		}

		// �`��t�B�[���h���w�肳��Ă�����ACRowBase�̎��̂̃A�h���X���擾
		// (�g�p����̂͌`�󂾂������ACRowBase::GetShape()�͌`�󂪃L���b�V������Ă��Ȃ���NULL��Ԃ��B
		//  CRowBase��(IGeometry*)�ŃL���X�g����ƁA�L���b�V�����Ȃ��ꍇ��DB����`����擾�ł���̂ŁA
		//  CRowBase�̎��̂̃A�h���X�����悤�ɂ���)
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
					ss << _T("--value�I�v�V�����Œl���Z�b�g����Ă��܂���F") << col;
					notifier::critical::versatile(ss.str());
					_ASSERTE( VT_EMPTY != val.vt );
				}
			}
			else
				return popt->GetValue(col.c_str()); // �^�����܂�Ȃ������̓L���b�V���ɓ���Ȃ�
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
				return CComVariant(expstr.c_str()); // �^�����܂�Ȃ������̓L���b�V���ɓ���Ȃ�
		}
		else
			return val;
		break;
	// TODO: CHE�̏ꍇ��ǉ�
	// CHE�̏ꍇ�͕ʏ����Ƃ��Ă��邽�߁A������ɓ����Ă��Ȃ�
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
	case ExpressionTypeEmpty: return true; break; // �����������AEmpty�ɂ͕ς��Ȃ��̂�
	default: _ASSERTE(false); break;
	}
	return false;
}

namespace
{
	/**
	 * @brief �����̂����ꂩ���`���r�p�̃f�[�^��ێ����Ă��邩����
	 * @retval true  �`���r�p�̃f�[�^��ێ����Ă���
	 * @retval false �`���r�p�̃f�[�^�͂����Ȃ�
	 */
	inline bool isGeometryCompare(const CComVariant& left, const CComVariant& right)
	{
		return (left.vt==ExpressionValue::VT_GEOMETRY
			|| right.vt==ExpressionValue::VT_GEOMETRY);
	}

	/**
	 * @brief CComVariant����`����擾
	 * @param[in] vari �`�������CComVariant
	 * @return �`��
	 */
	inline IGeometryPtr VariToGeometry(const CComVariant& vari)
	{
		_ASSERTE( vari.vt==ExpressionValue::VT_GEOMETRY );
		return (IGeometry*)(*reinterpret_cast<sindy::CRowBase*>(vari.pintVal));
	}

	/**
	 * @brief ���������`������r���Z�q�ɏ]���Ĕ�r����
	 * @param[in] left     ���Ӓl
	 * @param[in] right    �E�Ӓl
	 * @param[in] compOpe  ��r���Z�q
	 * @retval true  �`�󂪈�v���Ă���
	 * @retval false �`�󂪈�v���Ă��Ȃ�
	 */
	inline bool geometryCompare(const CComVariant& left,
		const CComVariant& right,
		ExpressionOperator::EnumOperator compOpe)
	{
		/// �`���r�����̎��ʎq���
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

		// ���Ӓl�E�I�y���[�^�E�E�Ӓl�g�������`(�����ɂ���g�����ȊO�̓G���[)
		// <���Ӓl��VARENUM, �I�y���[�^, �E�Ӓl��VARENUM>
		const std::map< GeometryComp, std::tuple<VARTYPE, ExpressionOperator::EnumOperator, VARTYPE> > allow = boost::assign::map_list_of
			// IGeometryPtr == IGeometryPtr
			(GeometryComp::EQUALS, std::make_tuple(ExpressionValue::VT_GEOMETRY, ExpressionOperator::OperatorEqual, ExpressionValue::VT_GEOMETRY))
			// IGeometryPtr != IGeometryPtr
			(GeometryComp::NOT_EQUALS, std::make_tuple(ExpressionValue::VT_GEOMETRY, ExpressionOperator::OperatorNotEqual, ExpressionValue::VT_GEOMETRY))
			// IGeometryPtr is IGeometryPtr
			(GeometryComp::IS, std::make_tuple(ExpressionValue::VT_GEOMETRY, ExpressionOperator::OperatorIs, ExpressionValue::VT_GEOMETRY))
			// IGeometryPtr is not IGeometryPtr
			(GeometryComp::IS_NOT, std::make_tuple(ExpressionValue::VT_GEOMETRY, ExpressionOperator::OperatorIsNot, ExpressionValue::VT_GEOMETRY))

			/* TODO: NULL�E��`��̔��肪�K�v�ɂȂ����Ȃ�A���̕����̃R�����g�A�E�g���O��
					 �\���ȃe�X�g�f�[�^��p�ӂ��邱�Ƃ��ł��Ȃ�&�v�]���Ȃ����߁A���̋@�\�̎����͌�����
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

		// ��r���Z�q�̎w�肪����Ă�����A�G���[���o�͂��Ĉُ�I������
		if(it==allow.cend())
		{
			std::wstringstream ss;
			ss << L"�`���r������������Ă��܂�";
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
		/* TODO: NULL�E��`��̔��肪�K�v�ɂȂ����Ȃ�A���̕����̃R�����g�A�E�g���O��
				 �\���ȃe�X�g�f�[�^��p�ӂ��邱�Ƃ��ł��Ȃ�&�v�]���Ȃ����߁A���̋@�\�̎����͌�����
		case GeometryComp::IS_NULL:
			return (nullptr==VariToGeometry(left));
			break;
		case GeometryComp::IS_NOT_NULL:
			return (nullptr!=VariToGeometry(left));
			break;
		case GeometryComp::IS_EMPTY:
			{
				// TODO: �����͂悭�悭�����̏�A�����̂���
				// Shape�t�@�C���ł�NULL�s���Ȃ̂ŁAis empty��true
				// �� �����ݒ�t�@�C����SDE�Ɏ��s������ nullptr���Ȃ̂�false
				// �����f�[�^�Ȃ̂Ɍ��ʂ��Ⴄ! �Ƃ��ɂȂ�Ȃ����H
				IGeometryPtr ipCheckedGeometry(VariToGeometry(left));

				// null�͋�ł͂Ȃ��̂�false�Ƃ��Ă���
				if(!ipCheckedGeometry) return false;
				// TODO: "��"�̊���c�[�����œ��ꂷ�邩����
				// (���ꂷ��Ǝd�l�������Ƃ��ɔ��ɏ�����)
				// ���L�֐���null����Ƃ��Ĕ��肷��̂Œ���
				return IsEmpty(ipCheckedGeometry);
			}
			break;
		case GeometryComp::IS_NOT_EMPTY:
			{
				// TODO: IS_EMPTY�̓��e�ɍ��킹�ďC�����邱��
				IGeometryPtr ipCheckedGeometry(VariToGeometry(left));
				// null�͋�ł͂Ȃ��̂�false�Ƃ��Ă���
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
		notifier::critical::versatile(_T("�������̃I�y���[�^�����߂ł��܂���ł���"));

	// is empty �̏���
	// �ǂ��炩��ExpressionTypeEmpty�ł��邱�ƑO��
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
		// ���̑��̉��Z�q�͐ݒ�t�@�C���Ǎ��ݎ��ɔr�˂��Ă���͂�
		default: _ASSERTE(false); break;
		}
		return false;
	}

	CComVariant vaLeft, vaRight;
	// Opt�^�APOD�^�͏��߂͌^���킩��Ȃ����߁Avalue(VARTYPE)��
	// �^�����܂�悤�ɂȂ��Ă���i����ChangeType���Ă���j
	// RowBase����擾����ۂ́AVARTYPE�͖������Ă���̂ŁA�܂�
	// RowBase�̕��̒l�����A����VARTYPE��ChangeType�����l��
	// �擾����B
	// ���Ȃ݂ɁARowBase�̒l��NULL�̏ꍇ�͂��̎��̎���ChangeType����
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

	// ��r�Ώۂ��`��Ȃ�A��p�̏����Ŕ��肷��
	if( isGeometryCompare(vaLeft, vaRight) )
		return geometryCompare(vaLeft, vaRight, ope);

	// �^�𑵂���
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
	// message�̏���
	outputMessage(ret, message, src, dst, opt);
	return ret;
}

bool ExpressionMessage::init( const uh::tstring& message )
{
	// �v���[�X�t�H���_�ƒu��������̕�������쐬����
	tsmatch m;
	tregex r( _T("\\$\\(.*?\\)") ); // �u$()�v�̍ŒZ��v
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

// TODO: ���������������̂ŁA���ʉ�������
ExpressionMessage::FORMAT ExpressionMessage::toFormatCode( const uh::tstring& str )
{
	// TODO: ���b�Z�[�W�̏o�͌`����������Ȃ�A���L�ɗv�f�𑫂�
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
	// TODO: ���O�֏o�͂���Ώۂ�������Ȃ�A���L�ɗv�f�𑫂�
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
		CComVariant va = holder.second.value(VT_BSTR); // POD����Ȃ���VT�w��͌����Ȃ��̂Œ��ӁI�I
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
	// Cache�̏ꍇ�͂ЂƂ܂��Asrc�̒l���i�[���A�㏈����src�̒l����}���ւ���
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
