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

#include <boost/tuple/tuple.hpp>

/**
 * @class CRuleModelNetwork
 * @brief �l�b�g���[�N���f���̃e�[�u�������`����N���X
 *
 * �V�����l�b�g���[�N���f�����`�������ꍇ�́AInitRule()��
 * �ǉ����Ă��������B
 */
class CRuleModelNetwork
{
public:
	CRuleModelNetwork();
	virtual ~CRuleModelNetwork();

	typedef std::list<std::pair<CString,CString> > TRuleContainer; //!< ���[���R���e�i�̌^
	typedef TRuleContainer::iterator iterator;
	typedef TRuleContainer::const_iterator const_iterator;
	iterator begin(){ return m_cRule.begin(); }
	const_iterator begin() const { return m_cRule.begin(); }
	iterator end(){ return m_cRule.end(); }
	const_iterator end() const { return m_cRule.end(); }

	bool IsNetworkLink( LPCTSTR lpcszName ) const;
	bool IsNetworkNode( LPCTSTR lpcszName ) const;
	CString GetPairName( LPCTSTR lpcszName ) const;
private:
	void InitRule();
	void ClearRule();
private:
	TRuleContainer m_cRule; //!< first:���C�� second:�|�C���g
};

/**
 * @class CRuleRelationshipContain
 * @brief ��܊֌W�̃e�[�u�������`����N���X
 *
 * �V������܊֌W���`�������ꍇ�́AInitRule()��
 * �ǉ����Ă��������B
 */
class CRuleRelationshipContain
{
public:
	CRuleRelationshipContain();
	virtual ~CRuleRelationshipContain();
	typedef std::list<std::pair<CString,CString> > TRuleValue; //!< ���[���R���e�i�̒l�̌^
	typedef TRuleValue::iterator value_iterator;
	typedef TRuleValue::const_iterator const_value_iterator;
	typedef std::map<CString,TRuleValue> TRuleContainer; //!< ���[���R���e�i�̌^
	typedef TRuleContainer::iterator iterator;
	typedef TRuleContainer::const_iterator const_iterator;
	iterator begin(){ return m_cRule.begin(); }
	const_iterator begin() const { return m_cRule.begin(); }
	iterator end(){ return m_cRule.end(); }
	const_iterator end() const { return m_cRule.end(); }
	value_iterator begin( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].begin(); }
	const_value_iterator begin( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].begin();
	}
	value_iterator end( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].end(); }
	const_value_iterator end( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].end();
	}
	iterator find( LPCTSTR lpcszTableName );
	iterator find_value( LPCTSTR lpcszTableName );
	/// �u�܂ށv���e�[�u�������L�[�Ƃ��Č���
	const_iterator find( LPCTSTR lpcszTableName ) const;
	/**
	 * @brief �u�܂܂��v���e�[�u�������L�[�Ƃ��Č���
	 * �Ԃ����C�e���[�^�́u�܂ށv���ł��鎖�ɒ��ӂ��Ă�������
	 */
	const_iterator find_value( LPCTSTR lpcszTableName ) const;
private:
	void InitRule();
	void ClearRule();
private:
	TRuleContainer m_cRule; // key:�܂ޕ� valuelist.first: �܂܂��� second:�O���L�[
};

/**
 * @class CRuleRelationTable
 * @brief �O���e�[�u���Ƃ̊֘A���`����N���X
 *
 * �V�����֌W���`�������ꍇ�́AInitRule()��
 * �ǉ����Ă��������B
 */
class CRuleRelationTable
{
public:
	CRuleRelationTable();
	virtual ~CRuleRelationTable();

	typedef std::list<boost::tuple<CString,CString,CString> > TRuleValue; //!< ���[���R���e�i�̒l�̌^
	typedef TRuleValue::iterator value_iterator;
	typedef TRuleValue::const_iterator const_value_iterator;
	typedef std::map<CString,TRuleValue> TRuleContainer; //!< ���[���R���e�i�̌^
	typedef TRuleContainer::iterator iterator;
	typedef TRuleContainer::const_iterator const_iterator;
	iterator begin(){ return m_cRule.begin(); }
	const_iterator begin() const { return m_cRule.begin(); }
	iterator end(){ return m_cRule.end(); }
	const_iterator end() const { return m_cRule.end(); }
	value_iterator begin( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].begin(); }
	const_value_iterator begin( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].begin();
	}
	value_iterator end( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].end(); }
	const_value_iterator end( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].end();
	}
private:
	void InitRule();
	void ClearRule();
private:
	TRuleContainer m_cRule; // key:�x�[�X�e�[�u���� valuelist.first:�֘A�t���O�t�B�[���h�� valuelist.second:�֘A�e�[�u���� valuelist.third:�O���L�[
};

/**
 * @class CRuleLinkQueue
 * @brief �����N��Ƃ��̊֘A���`����N���X
 *
 * �V�����֌W���`�������ꍇ�́AInitRule()��
 * �ǉ����Ă��������B
 */
class CRuleLinkQueue
{
public:
	CRuleLinkQueue();
	virtual ~CRuleLinkQueue();

	typedef std::list<boost::tuple<CString,CString,CString,CString> > TRuleValue; //!< ���[���R���e�i�̒l�̌^
	typedef TRuleValue::iterator value_iterator;
	typedef TRuleValue::const_iterator const_value_iterator;
	typedef std::map<CString,TRuleValue> TRuleContainer; //!< ���[���R���e�i�̌^
	typedef TRuleContainer::iterator iterator;
	typedef TRuleContainer::const_iterator const_iterator;
	iterator begin(){ return m_cRule.begin(); }
	const_iterator begin() const { return m_cRule.begin(); }
	iterator end(){ return m_cRule.end(); }
	const_iterator end() const { return m_cRule.end(); }
	value_iterator begin( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].begin(); }
	const_value_iterator begin( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].begin();
	}
	value_iterator end( LPCTSTR lpcszKey ){ return m_cRule[lpcszKey].end(); }
	const_value_iterator end( LPCTSTR lpcszKey ) const {
		return (*const_cast<TRuleContainer*>(&m_cRule))[lpcszKey].end();
	}
private:
	void InitRule();
	void ClearRule();
private:
	TRuleContainer m_cRule;
};

extern const CRuleModelNetwork        g_cRuleModelNetwork;        //!< �l�b�g���[�N���f����`�N���X
extern const CRuleRelationshipContain g_cRuleRelationshipContain; //!< ����֘A��`�N���X
extern const CRuleRelationTable       g_cRuleRelationTable;       //!< �O���e�[�u���Q�ƒ�`�N���X
extern const CRuleLinkQueue           g_cRuleLinkQueue;           //!< �����N�L���[��`�N���X
