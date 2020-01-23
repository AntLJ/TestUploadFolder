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
 *	@file LinkPatch.h
 *	@brief �����N�p�b�`�N���X ��`��
 *	@author	F.Adachi
 *	@date	2005/11/9		�V�K�쐬
 *	$ID$
 */

#ifndef ___LINKPATCH___
#define ___LINKPATCH___

#include <boost/utility.hpp>
#include <vector>

/// <b>�����N�p�b�`�̃��R�[�h�N���X</b>
struct LinkPatchRecord
{
	std::string			m_Attribute;		//< �C������
	std::string			m_RevisedValue;		//< �C���l�i������j
	long				m_MeshCode;			//< �ΏۂQ�����b�V���R�[�h
	std::vector<long>	m_NodeIDCollection;	//< �Ώۃm�[�hID��(�Q�ȏ�)

	LinkPatchRecord(void);
	~LinkPatchRecord();

	typedef std::vector<long>::const_iterator NodeIDIter;
};


/// <b>�����N�p�b�`�N���X</b>
class LinkPatch : boost::noncopyable
{
public:
	LinkPatch(void);
	~LinkPatch();

	void assign( LinkPatchRecord cRecord );
	void clear(void);
	void write( std::ostream& crStream );
	void write( const char* cFilePath, bool cUseEUC );

	typedef std::vector<LinkPatchRecord>::const_iterator LinkPatchIter;
private:
	std::vector<LinkPatchRecord>	m_Data;		//< �����N�p�b�`�̃��R�[�h��

};

#endif
