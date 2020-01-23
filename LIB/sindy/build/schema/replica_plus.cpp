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
#include <sindy/schema/replica_plus.h>

namespace replica_plus {

namespace schema
{
	// �f�t�H���g�����꒍�L
	namespace mlang_default_anno {
		const _TCHAR kTableName[] = _T("mlang_default_anno");

		const _TCHAR kMulti_F[] = _T("multi_f");				//!< �������̕ێ��t���O
		const _TCHAR kAnnoClass_C[] = _T("annoclass_c");		//!< ���L���
		const _TCHAR kAnnoClassName[] = _T("annoclass_name");	//!< ���L����
		const _TCHAR kMatchingStr[] = _T("matching_str");		//!< �}�b�`���O������
		const _TCHAR kLangCode[] = _T("langcode");				//!< ����R�[�h
		const _TCHAR kName[] = _T("name");						//!< ����
	} // namespace mlang_default_anno

	// �����ꋤ��
	namespace mlang_common {
		const _TCHAR kNameKanji[] = _T("name_kanji");		//!< ���{�ꖼ��
		const _TCHAR kNameYomi[] = _T("name_yomi");			//!< ���{��ǂ�
		const _TCHAR kLangCode[] = _T("langcode");			//!< ����R�[�h
		const _TCHAR kName[] = _T("name");					//!< ����
	}

	// ����������_����
	namespace mlang_road_node {
		const _TCHAR kTableName[] = _T("mlang_road_node");

		const _TCHAR kNodeId[] = _T("node_id");		//!< ���H�m�[�hID
	} // namespace mlang_road_node

	// ��������ʈē�
	namespace mlang_inf_dirguide {
		const _TCHAR kTableName[] = _T("mlang_inf_dirguide");

	} // namespace mlang_inf_dirguide

}

}
