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

struct stModel {
	CString ParentName; //!< �e���̖��O
	CString ParentKey;  //!< �e���̃L�[��
	CString Name;       //!< �q���̖��O
	CString OuterKey;   //!< �q���̃L�[��
	CString ParentStrictKey; //!< �e���̐���L�[��   ��Ffeature_rel��poi_point�̏ꍇ��geometrytype_c
	CComVariant ParentStrictValue; //!< �e���̐���l ��Ffeature_rel��poi_point�̏ꍇ��1L
};

class CModel
{
public:
	CModel();
	virtual ~CModel();

	typedef std::multimap<CString,stModel> TYPEDEF_MODEL;
	typedef std::pair<CString,stModel> TYPEDEF_MODEL_PAIR;
	typedef TYPEDEF_MODEL::iterator iterator;
	typedef TYPEDEF_MODEL::const_iterator const_iterator;

	/// ���f���̃g�b�v���疖�[�܂ł��V���A���Ɏ擾����
	std::list<CString> GetOrderdNameList( LPCTSTR lpcszTopLevelParentName ) const;

	/// �e�ɓ����郂�f�������擾����
	std::list<CString> GetParent( LPCTSTR lpcszName ) const;

	/// �q�ɂ����郂�f�������擾����
	std::list<CString> GetChild( LPCTSTR lpcszName ) const;

	/// ���f����ǉ�����
	void Add( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey ) {
		AddModel( lpcszChild, lpcszChildKey, lpcszParent, lpcszParentKey );
	}
	void Add( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey, LPCTSTR lpcszParentStrictKey, const CComVariant& vaStrictValue );

	/// �q�̃��f��������L�[�����擾����
	std::list<stModel> GetKeys( LPCTSTR lpcszName ) const;

	/// �e�̃��f��������L�[�����擾����
	std::list<stModel> GetKeysByParentName( LPCTSTR lpcszName ) const;

	/// �e�q�̃��f��������L�[�����擾����
	stModel GetKey( LPCTSTR lpcszName, LPCTSTR lpcszParent ) const;

	iterator begin() { return m_model.begin(); }
	const_iterator begin() const { return m_model.begin(); }
	iterator end() { return m_model.end(); }
	const_iterator end() const { return m_model.end(); }
protected:
	TYPEDEF_MODEL::iterator AddModel( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey );
protected:
	TYPEDEF_MODEL m_model;
};
