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
#include <sindy/schema/egg.h>

namespace egg {

namespace schema
{
	namespace objectid {
		const _TCHAR kObjectId[] = _T("objectid");
	} // namespace objectid

	namespace update_info {
		const _TCHAR kUpdateInfo[] = _T("update_info");
	}

	namespace common_mlang {
		const _TCHAR kId[] = _T("id");
		const _TCHAR kLangCode[] = _T("langcode");
		const _TCHAR kName[] = _T("name");
	} // namespace common_mlang

	// �����꒍�L�p����
	namespace common_mlang_anno {
		const _TCHAR kDefaultName[] = _T("default_name");
	} // common_mlang_anno

	// ������g�b�v���L
	namespace ml_top_annotation {
		const _TCHAR kTableName[] = _T("ml_top_annotation");
	} // ml_top_annotation

	// ������~�h�����L
	namespace ml_middle_annotation {
		const _TCHAR kTableName[] = _T("ml_middle_annotation");
	} // ml_middle_annotation

	// ������x�[�X���L
	namespace ml_base_annotation {
		const _TCHAR kTableName[] = _T("ml_base_annotation");
	} // ml_base_annotation

	// ������s�s�n�}���L
	namespace ml_city_annotation {
		const _TCHAR kTableName[] = _T("ml_city_annotation");
	} // ml_city_annotation

	// ����������_����
	namespace ml_road_node {
		const _TCHAR kTableName[] = _T("ml_road_node");
	} // ml_road_node

	// ��������ʈē�
	namespace ml_inf_dirguide {
		const _TCHAR kTableName[] = _T("ml_inf_dirguide");
		const _TCHAR kNameSeq[] = _T("name_seq");
	} // ml_inf_dirguide

	// �����N�񋤒ʕ�
	namespace common_linkqueue {
		const _TCHAR kInfId[] = _T("inf_id");
		const _TCHAR kLinkId[] = _T("link_id");
		const _TCHAR kLinkDir[] = _T("linkdir_c");
		const _TCHAR kSequence[] = _T("sequence");
	} // common_linkqueue

	// �C���X�g���
	namespace inf_illust {
		const _TCHAR kTableName[] = _T("inf_illust");
		const _TCHAR kIllust[] = _T("illust_c");
		const _TCHAR kFileName[] = _T("file_name");
	} // inf_illust

	// �C���X�g�����N��
	namespace lq_illust {
		const _TCHAR kTableName[] = _T("lq_illust");
	} // lq_illust
}

}
