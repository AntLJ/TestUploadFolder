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

#ifndef OPERATOR_MODIFY_STAMPER_H_
#define OPERATOR_MODIFY_STAMPER_H_

#include <arctl/field/index.h>
#include <sindy/stamper/modify_stamper.h>
#include <sindy/stamper/modify_date_stamper.h>
#include <sindy/schema/common.h>
#include <Rule.h>

namespace sindy {

/**
 * @brief SiNDY-e�ɂ��X�V���ׂ��ꂽ���Ƃ��������������󂷂�N���X
 *
 * @par ���󂳂�鑮��:
 * - �ŏI�X�V����
 * - �X�V���e
 * - �I�y���[�^��
 * - ��ƖړIID
 * - �i�ꍇ�ɂ��j�N���[���t���O
 * - �i�ꍇ�ɂ��j���n�����t���O
 */
class operator_modify_stamper : public modify_stamper
{
public:
	operator_modify_stamper(CRule& rRule);

protected:
	virtual bool stamp(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const;

private:
	CRule& m_rRule;
};

inline operator_modify_stamper::operator_modify_stamper(CRule& rRule) :
m_rRule(rRule)
{
}

inline bool operator_modify_stamper::stamp(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const
{
	using namespace arctl::field;
	using namespace schema::ipc_table;
	using namespace schema::fieldsurvey_table;

	// �ŏI�X�V������ݒ肷��B
	if(! operator_modify_date_stamper(ipRowBuffer)(ipRowBuffer))
		return false;

	atl2::valid(ipRowBuffer->put_Value(index_t<kUpdateType>(ipRowBuffer), _variant_t(static_cast<long>(eUpdateType))));
	atl2::valid(ipRowBuffer->put_Value(index_t<kOperator>(ipRowBuffer), _variant_t(m_rRule.GetOperatorName())));
	atl2::valid(ipRowBuffer->put_Value(index_t<kPurpose>(ipRowBuffer), _variant_t(m_rRule.GetWorkPurposeID())));

	ISiNDYRulePtr ipRule( CLSID_SiNDYRuleObj );
	VARIANT_BOOL vaIsClaim = VARIANT_FALSE;
	ipRule->IsClaim( &vaIsClaim );
	if(vaIsClaim){
		const index_t<kUserClaim> aIndex(ipRowBuffer);
		if(aIndex >= 0) {
			atl2::valid(ipRowBuffer->put_Value(aIndex, _variant_t(1L)));
		}
	}

	VARIANT_BOOL vaIsGencho = VARIANT_FALSE;
	ipRule->IsGencho( &vaIsGencho );
	if(vaIsGencho){
		const index_t<kFieldSurvey> aIndex(ipRowBuffer);
		if(aIndex >= 0) {
			atl2::valid(ipRowBuffer->put_Value(aIndex, _variant_t(1L)));
		}
	}

	return true;
}

} // namespace sindy

#endif // OPERATOR_MODIFY_STAMPER_H_
