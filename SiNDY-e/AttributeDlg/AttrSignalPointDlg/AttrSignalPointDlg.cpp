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
#include "AttrSignalPointDlg.h"
#include "WinMap.h"
#include "PixTwip.h"

#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/tstring.h>
#include <boost/lexical_cast.hpp>

#define MSGBOX(RESID)   { AttrMessageBox(AheLoadString(RESID), AheLoadString(IDS_CAPTION), MB_OK|MB_ICONSTOP|MB_TOPMOST, 0L); }

namespace sp = sindy::schema::adam_v2::signal_point;
namespace rls = sindy::schema::adam_v2::rel_lane_node_signal_point;
namespace ln = sindy::schema::adam_v2::lane_node;

namespace rups = sindy::schema::adam_v2::rel_updown_node_signal_point;
namespace upn = sindy::schema::adam_v2::updown_node;

const COLORREF CAttrSignalPointDlg::LINE_COLOR = RGB(0xC0, 0xC0, 0x80);

namespace
{
	/**
	 * @brief �e�[�u�����t�B�[���h�������𔻒肷��
	 * @param [in] ipTable �e�[�u��
	 * @param [in] fieldName �t�B�[���h��
	 * @retval true �t�B�[���h�������Ă���
	 * @retval false �t�B�[���h�������Ă��Ȃ�
	 */
	bool hasField(ITablePtr ipTable, LPCTSTR fieldName)
	{
		if (!ipTable)
			return false;

		IFieldPtr ipTmpField;
		return AheGetFieldByFieldName(ipTable, fieldName, &ipTmpField);
	}
}

extern "C" __declspec(dllexport) CExportDlg* __stdcall CreateDlg()
{
	return new CAttrSignalPointDlg();
}


CAttrSignalPointDlg::CAttrSignalPointDlg()
	: CAttrBaseDlg(SIGNAL_POINT_MAP, IDD_ATTR_SIGNALPOINT_DIALOG)
	, m_isDraw(false)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_HORIZONTAL_ANGLE,     CContainedWindow(  _T("EDIT"), this, IDC_EDIT_HORIZONTAL_ANGLE ) )
		( IDC_COMBO_SIGNAL_MOLD_C,       CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SIGNAL_MOLD_C ) )
		( IDC_EDIT_LAMP_COUNT,           CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LAMP_COUNT ) )
		( IDC_EDIT_ALTITUDE,             CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ALTITUDE))
		( IDC_BUTTON_TRANSFER_ALTITUDE,  CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_TRANSFER_ALTITUDE))
		( IDC_EDIT_GROUND_HEIGHT,        CContainedWindow(  _T("EDIT"), this, IDC_EDIT_GROUND_HEIGHT))
		( IDC_BUTTON_TRANSFER_GR_HEIGHT, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_TRANSFER_GR_HEIGHT))
		( IDC_EDIT_HEIGHT,               CContainedWindow(  _T("EDIT"), this, IDC_EDIT_HEIGHT))
		( IDC_EDIT_WIDTH,                CContainedWindow(  _T("EDIT"), this, IDC_EDIT_WIDTH))
		( IDC_CHECK_ARROW_F,             CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_ARROW_F ) )
		( IDC_CHECK_WARNING_F,           CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_WARNING_F ) )
		( IDC_CHECK_INDEPENDENT_F,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_INDEPENDENT_F ) )
		( IDC_COMBO_SPECULATION_C,       CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID,            CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) )
		( IDC_EDIT_SOURCE,               CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
	;
}

CAttrSignalPointDlg::~CAttrSignalPointDlg()
{
}

HWND CAttrSignalPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	try {
		m_hChildWnd = CAttrBaseDlg::Create(hWndParent, dwInitParam);
	} catch (...) {
		ATLASSERT(false);
	}
	return m_hChildWnd;
}

void CAttrSignalPointDlg::Delete()
{
	RefreshScreen();
	SetFocus();     // �q�_�C�A���O�Ƀt�H�[�J�X������Ɨ�����...�̂Ŗ{�̂̕��փt�H�[�J�X���ڂ�...

	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
	delete this;    // �K�� CreateDlg()�� new �Ŋm�ۂ����O��!!
}

LRESULT CAttrSignalPointDlg::OnClickedTransferAltitudeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	// �n�㍂���擾�B�擾�ł��Ȃ���΃G���[�o���ďI���
	double groundHeight = 0;
	if (!GetDoubleFromEditControl(IDC_EDIT_GROUND_HEIGHT, groundHeight) || groundHeight <= 0)
	{
		AheMessageBox(AheLoadString(IDS_ERR_GROUND_HEIGHT_INVALID), AheLoadString(IDS_ERR_TRANSFER), MB_OK);
		return 0;
	}
	// �I�����ꂽ�m�[�h��Z�l���擾����B�G���[���b�Z�[�W��GetNodeAlt�֐��̒��ŕ\��
	double nodeAlt = 0;
	if(!GetNodeAlt(nodeAlt))
		return 0;

	// �]�ʌ��m�[�h�{�n�㍂����΍�(Z�l)�ƂȂ�
	double signalPointAlt = nodeAlt + groundHeight;

	CString altStr = uh::str_util::format(_T("%f"), signalPointAlt);
	SetDlgItemText(IDC_EDIT_ALTITUDE, altStr);

	return 0;
}

LRESULT CAttrSignalPointDlg::OnClickedTransferGroundHeightButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	// ��΍��𓾂�B�s���ł���΃G���[�o���ďI���B��΍�0���G���[�����ƂȂ�(�����̒l��0�͍l�����Ȃ����Ȃ���)
	double altitude = 0;
	if (!GetDoubleFromEditControl(IDC_EDIT_ALTITUDE, altitude) || altitude == 0)
	{
		AheMessageBox(AheLoadString(IDS_ERR_ALTITUDE_INVALID), AheLoadString(IDS_ERR_TRANSFER), MB_OK);
		return 0;
	}

	// �I�����ꂽ�m�[�h��Z�l���擾����B�G���[���b�Z�[�W��GetNodeAlt�֐��̒��ŕ\��
	double nodeAlt = 0;
	if(!GetNodeAlt(nodeAlt))
		return 0;

	// ��΍�����m�[�h���������������l���n�㍂�ƂȂ�
	double groundHeight = altitude - nodeAlt;

	CString groundHeightStr = uh::str_util::format(_T("%f"), groundHeight);
	SetDlgItemText(IDC_EDIT_GROUND_HEIGHT, groundHeightStr);

	return 0;
}

bool CAttrSignalPointDlg::Update()
{
	using namespace sindy::schema::adam_v2_adam_ipc_feature;
	using namespace uh;

	assert(m_ipFlexGrid);

	CString signalPointGid;
	IWorkspacePtr ipWorkspace;
	ITablePtr ipTable;

	{
		IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
		if (! ipFeature) { return false; }

		signalPointGid = AheGetAttribute(ipFeature, kGlobalID).bstrVal;
		ipFeature->get_Table(&ipTable);
		if (!ipTable) { return false; }
	}

	//if (! m_LinkagePoints.empty())
	{
		// �R�t�m�[�h�̍X�V���`�F�b�N
		std::set<LinkageNode> prev;    // �ŏ��̕R�t���m�[�h��GID�Q
		std::set<LinkageNode> curr;    // ���݂̕R�t�m�[�h��GID�Q
		std::set<LinkageNode> del;     // �폜�Ώۂ̕R�t���m�[�h��GID�Q
		std::set<LinkageNode> ins;     // �}���Ώۂ̕R�t�m�[�h��GID�Q

		prev = m_LinkagePoints;

		long lROWs = -1, lCOLs = -1;

		m_ipFlexGrid->get_Rows(&lROWs);
		m_ipFlexGrid->get_Cols(&lCOLs);

		for (long r=1 ; r<lROWs ; r++)
		{
			CString nameStr = (LPCTSTR)m_ipFlexGrid->GetTextMatrix(r, LAYER_COL);
			CString oidStr  = (LPCTSTR)m_ipFlexGrid->GetTextMatrix(r, OID_COL);

			if (!nameStr.IsEmpty() && !oidStr.IsEmpty())
			{
				ITablePtr ipNodeTable = AheOpenSameOwnerTable(ipTable, nameStr);
				CString gid = getGID(ipNodeTable, _ttol(oidStr));
				curr.insert(LinkageNode(nameStr, _ttol(oidStr), gid));
			}
		}

		// �u�ŏ��ɑ��݂���v   �� �u���݂ɑ��݂��Ȃ��v �� �폜�Ώ�
		// �u�ŏ��ɑ��݂��Ȃ��v �� �u���݂ɑ��݂���v   �� �}���Ώ�
		std::set_difference(prev.begin(), prev.end(), curr.begin(), curr.end(), std::inserter(del, del.begin()));
		std::set_difference(curr.begin(), curr.end(), prev.begin(), prev.end(), std::inserter(ins, ins.begin()));

		// �폜/�}���Ƃ��ɉ����Ȃ���΂����ŏI��!!!
		if (del.empty() && ins.empty()) { return true; }


		bool is_change = false;
		long err = 0;
		// �폜
		{
			std::map<CString, std::vector<CString>> del_ids;
			for (auto& itr : del)
			{
				if (!itr.layerName.IsEmpty()) {
					del_ids[itr.layerName].push_back(str_util::format(_T("'%s'"), itr.gID));
				}
				is_change = true;
			}


			for (auto& itr : del_ids)
			{
				CString field;
				CString featureclass;
				CString signalid;
				if (itr.first == ln::kTableName)
				{
					featureclass = rls::kTableName;
					field = rls::kLaneNodeGID;
					signalid = rls::kSignalPointGID;
				}
				else if (itr.first == upn::kTableName)
				{
					featureclass = rups::kTableName;
					field = rups::kUpdownNodeGID;
					signalid = rups::kSignalPointGID;
				}

				CString where_clause = uh::str_util::format(_T("%s = '%s' and %s in (%s)"), 
					signalid, signalPointGid, 
					field, uh::str_util::join(itr.second, _T(",")));
				IQueryFilterPtr ipQuery(CLSID_QueryFilter);
				ipQuery->put_WhereClause(where_clause.GetBuffer());

				ITablePtr ipRelTable = AheOpenSameOwnerTable(ipTable, featureclass);
				if (! AheIsEditableTable(ipRelTable)) { 
					err++;
					continue;
				}
				if (ipRelTable) {
					ipRelTable->DeleteSearchedRows(ipQuery);
				}
			}
		}

		// �}��
		{
			for (auto& itr : ins)
			{
				CString signalid;
				CString field;
				CString featureclass;
				CString relationtbl;
				if (itr.layerName == ln::kTableName)
				{
					signalid = rls::kSignalPointGID;
					field = rls::kLaneNodeGID;
					featureclass = ln::kTableName;
					relationtbl = rls::kTableName;
				}
				else if (itr.layerName == upn::kTableName)
				{
					signalid = rups::kSignalPointGID;
					field = rups::kUpdownNodeGID;
					featureclass = upn::kTableName;
					relationtbl = rups::kTableName;
				}

				ITablePtr ipRelTable  = AheOpenSameOwnerTable(ipTable, relationtbl);
				ITablePtr ipNodeTable = AheOpenSameOwnerTable(ipTable, featureclass);
				if (! (ipRelTable && ipNodeTable)) { assert(false); return false; /*continue*/; }

				if (! AheIsEditableTable(ipRelTable)) { err++; continue; }

				CString where_clause;
				where_clause.Format( _T("%s='%s'"), kGlobalID, itr.gID);

				IQueryFilterPtr ipQuery(CLSID_QueryFilter);
				ipQuery->put_WhereClause(where_clause.GetBuffer());

				_ICursorPtr ipSearch;
				ipNodeTable->Search(ipQuery, VARIANT_TRUE, &ipSearch);

				_IRowPtr ipRow;
				if (ipSearch && ipSearch->NextRow(&ipRow) == S_OK && ipRow)
				{
					_IRowPtr ipBuffer;
					ipRelTable->CreateRow(&ipBuffer);

					AheSetModifyData(ipBuffer, sindyUpdateTypeCreate);
					AheSetAttribute (ipBuffer, field,    itr.gID);
					AheSetAttribute (ipBuffer, signalid, signalPointGid);
					if (FAILED(ipBuffer->Store())) { err++; }
				}
				else
				{
					// ���݂��Ȃ��m�[�h���w�肳�ꂽ�ꍇ
					err++;
					MSGBOX(IDS_MSG_FAIL_INVALID_NODE);
				}
				is_change = true;
			}
		}
		if (err > 0) { return false; }

		if (is_change)
		{
			RefreshScreen();
			//IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
			//IPolygonPtr ipRefresh;
			//AheFeature2Polygon(ipFeature, 25, &ipRefresh);
			//IEnvelopePtr ipEnv;
			//ipRefresh->get_Envelope(&ipEnv);
			//m_cArcHelper.GetActiveView()->PartialRefresh(static_cast<esriViewDrawPhase>(esriViewGeography | esriViewGraphics), NULL, ipEnv);
		}
	}

	return true;
}

BOOL CAttrSignalPointDlg::ErrorCheck()
{
	ATLASSERT(m_pFeatureDefList);
	for (auto& itr : *m_pFeatureDefList)
	{
		const CString strTableName = itr.GetTableName();
		if (strTableName == sp::kTableName)
		{
			IFeaturePtr ipFeature(itr.GetFeature());

			LONG lOID = 0;
			ipFeature->get_OID(&lOID);

			CFieldDef* pDef = NULL;
			// �p�x
			pDef = GetCurrentFeatureDef()->GetFieldDef(sp::kTableName, sp::kHorizontalAngle);
			if (pDef)
			{
				long lAngle = pDef->m_vaValue.dblVal;
				if (! (0.0 <= lAngle && lAngle < 360.0))
				{
					MSGBOX(IDS_MSG_FAIL_OUT_OF_RANGE);
					return FALSE;
				}
			}

			// ����
		}
	}
	return TRUE;
}

BOOL CAttrSignalPointDlg::SetCurrentFeatureDefIndex(LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable)
{
#if defined(_DEBUG)
	if (lTableIndex < 0 || lRowIndex < 0) { return FALSE; }
#endif
	//m_ipFlexGrid�̑I����ԃN���A
	m_ipFlexGrid->put_Col(-1);
	m_ipFlexGrid->put_Row(-1);

	RefreshScreen();

	BOOL bRet = CAttrBaseDlg::SetCurrentFeatureDefIndex(lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable);
	if (! bRet) { return FALSE; }

	long lEditableTable = SetRelationValue();
	m_ipFlexGrid->put_Editable(flexEDNone);
	m_ipFlexGrid->put_Enabled(VARIANT_TRUE);

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X�𐧌䂷��
	// �ҏW�J�n�O�A�ҏW�J�n��ł��ҏW�s�̎��� flexEDKbdMouse �ɂ���
	if (IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable)
	{
		// �ҏW�J�n����Ă��鎞�� SetFocus()���Ă���
		if (lEditableTable > 0)
		{
			m_ipFlexGrid->put_Editable(flexEDKbdMouse);
		}
		::SetFocus(GetDlgItem(IDC_EDIT_HORIZONTAL_ANGLE));
	}

	IFeaturePtr signalPoint = GetCurrentFeatureDef()->GetFeature();
	if (!signalPoint)
		return FALSE;

	ITablePtr signalPointTable = AheGetTable(signalPoint);
	if (!signalPointTable)
		return FALSE;

	/**
	 * @brief ���݂��Ȃ��t�B�[���h���O���[�A�E�g���郉���_��
	 * @param ipTable [in] �e�[�u��
	 * @param fieldName [in] �t�B�[���h��
	 * @param resources [in] �t�B�[���h�Ɉˑ�����R���g���[���̃��\�[�XID�Q
	 */
	static auto grayoutNotExistFieldControl = [&](ITablePtr ipTable, LPCTSTR fieldName, const std::list<INT>& resources)
	{
		if(!hasField(ipTable, fieldName))
		{
			for(INT resourceId : resources)
			{
				SetDlgStatus( resourceId, FALSE );
			}
		}
	};
	
	using namespace sindy::schema::adam_v2;

	// �n�㍂/�c��/�����֌W�̃R���g���[��ID�Q
	static std::list<INT> groundHeightRelResouces = boost::assign::list_of<INT>(IDC_STATIC_GROUND_HEIGHT)(IDC_EDIT_GROUND_HEIGHT)(IDC_BUTTON_TRANSFER_ALTITUDE)(IDC_BUTTON_TRANSFER_GR_HEIGHT);
	static std::list<INT> heightRelResouces = boost::assign::list_of<INT>(IDC_STATIC_HEIGHT)(IDC_EDIT_HEIGHT);
	static std::list<INT> widthRelResouces  = boost::assign::list_of<INT>(IDC_STATIC_WIDTH)(IDC_EDIT_WIDTH);

	// �n�㍂�֌W�̃t�B�[���h���Ȃ���΃O���[�A�E�g
	grayoutNotExistFieldControl(signalPointTable, signal_point::kGroundHeight, groundHeightRelResouces);

	// �c���֌W�̃t�B�[���h���Ȃ���΃O���[�A�E�g
	grayoutNotExistFieldControl(signalPointTable, signal_point::kHeight, heightRelResouces);

	// �����֌W�̃t�B�[���h���Ȃ���΃O���[�A�E�g
	grayoutNotExistFieldControl(signalPointTable, signal_point::kWidth, widthRelResouces);

	return bRet;
}

void CAttrSignalPointDlg::CreateControlRelation()
{
	ATLASSERT(IsWindow());

	if (m_cControlRel.size() > 0)
	{
		// �e�E�B���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND(m_hWnd);
	}
	else
	{
		m_cControlRel.SetHWND(m_hWnd);

		// �M���@�|�C���g�p
		m_cControlRel.SetControl(IDC_EDIT_HORIZONTAL_ANGLE,     TYPE_EDITDOUBLE, 0, 0, sp::kHorizontalAngle,  NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_COMBO_SIGNAL_MOLD_C,       TYPE_COMBO,      0, 0, sp::kSignalMoldC,      NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_LAMP_COUNT,           TYPE_EDIT,       0, 0, sp::kLampCount,        NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_CHECK_ARROW_F,             TYPE_CHECKBOX,   0, 0, sp::kArrowF,           NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_CHECK_WARNING_F,           TYPE_CHECKBOX,   0, 0, sp::kWarningF,         NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_CHECK_INDEPENDENT_F,       TYPE_CHECKBOX,   0, 0, sp::kIndependentF,     NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_COMBO_SPECULATION_C,       TYPE_COMBO,      0, 0, sp::kSperculationC,    NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE,               TYPE_EDITSTRING, 0, 0, sp::kSource,           NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE_ID,            TYPE_EDITSTRING, 0, 0, sp::kSourceID,         NULL, sp::kTableName, 0, -1, FOLLOW_READONLY);
		m_cControlRel.SetControl(IDC_EDIT_ALTITUDE,             TYPE_EDITDOUBLE, 0, 0, virtual_field::kShapeZ, NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_BUTTON_TRANSFER_ALTITUDE,  TYPE_BUTTON,     0, 0);
		m_cControlRel.SetControl(IDC_EDIT_GROUND_HEIGHT,        TYPE_EDITDOUBLE, 0, 0, sp::kGroundHeight, NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_BUTTON_TRANSFER_GR_HEIGHT, TYPE_BUTTON,     0, 0);
		m_cControlRel.SetControl(IDC_EDIT_HEIGHT,               TYPE_EDITDOUBLE, 0, 0, sp::kHeight, NULL, sp::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_WIDTH,                TYPE_EDITDOUBLE, 0, 0, sp::kWidth, NULL, sp::kTableName);

		//std::list<CString> arr;
		//arr.push_back(sr::kName);
		//arr.push_back(sr::kLaneNodeGID);
		//m_cControlRel.SetControl(IDC_VSFLEXGRID_NODE,     TYPE_OTHER,     0, 0, arr, NULL, sr::kTableName, sr::kSignalPointGID);
	}
}

BOOL CAttrSignalPointDlg::SetRelationValue(CString gID)
{
	using namespace sindy::schema;

	//::MessageBox(m_cArcHelper.GetHWND(), _T("called SetRelationValue"), AheLoadString(IDS_CAPTION), MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
	m_LinkagePoints.clear();
	ClearRelationLine();
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (! ipFeature) { return FALSE; }

	// ���Y�M���@�|�C���g�t�B�[�`����GLOBALID�擾
	if (gID.IsEmpty())
		gID = AheGetAttribute(ipFeature, adam_v2_adam_ipc_feature::kGlobalID).bstrVal;

	IWorkspacePtr ipWorkspace;
	ITablePtr     ipTable;
	{
		ipFeature->get_Table(&ipTable);
		IDatasetPtr ipDataset(ipTable);
		if (ipDataset) { 
			ipDataset->get_Workspace(&ipWorkspace);
		}
	}
	if (! (ipWorkspace && ipTable))
		return FALSE;


	CString aRelTableName[2];
	CString aRelSignalID[2];
	CString aRelNodeID[2];
	CString aRelNodeName[2];

	aRelTableName[0] = rls::kTableName;
	aRelSignalID [0] = rls::kSignalPointGID;
	aRelNodeID   [0] = rls::kLaneNodeGID;
	aRelNodeName [0] = ln::kTableName;

	aRelTableName[1] = rups::kTableName;
	aRelSignalID [1] = rups::kSignalPointGID;
	aRelNodeID   [1] = rups::kUpdownNodeGID;
	aRelNodeName [1] = upn::kTableName;

	LONG lEditableNum = 0;
	for (long i=0 ; i<2 ; i++)
	{
		ITablePtr ipRelTable = AheOpenSameOwnerTable(ipWorkspace, ipTable, aRelTableName[i]);
		if (! ipRelTable) { continue; }

		IQueryFilterPtr ipQuery(CLSID_QueryFilter);
		CString where_clause, sub_fields;
		where_clause.Format(_T("%s='%s'"), aRelSignalID[i], gID);
		sub_fields  .Format(_T("%s,%s,%s"),rups::kObjectID, aRelSignalID[i], aRelNodeID[i]);
		ipQuery->put_WhereClause(where_clause.GetBuffer());
		ipQuery->put_SubFields  (sub_fields  .GetBuffer());

		_ICursorPtr ipCursor;
		ipRelTable->Search(ipQuery, VARIANT_TRUE, &ipCursor);
		if (! ipCursor)
			continue;

		long lIndex = -1;
		ipRelTable->FindField(const_cast<BSTR>(aRelNodeID[i].GetBuffer()), &lIndex);
		if (lIndex < 0)
			continue;

		_IRowPtr ipRow;
		while (ipCursor && ipCursor->NextRow(&ipRow) == S_OK)
		{
			if (! ipRow)
				break;

			long relOid =AheGetAttribute(ipRow, rups::kObjectID).lVal;

			CComVariant shp = AheGetAttribute(ipRow, _T("SHAPE"));
			if (IFeatureClassPtr(ipRelTable)) {
			CString msg;
			msg.Format(_T("%d"), shp.vt);
			MessageBox(msg);
			}

			// GID
			CComVariant v;
			ipRow->get_Value(lIndex, &v);
			assert(v.vt == VT_BSTR);
			// OID
			ITablePtr ipNodeTable = AheOpenSameOwnerTable(ipWorkspace, ipTable, aRelNodeName[i]);
			long oid = getOID( ipNodeTable , v.bstrVal);

			m_LinkagePoints.insert(LinkageNode(aRelNodeName[i], oid, v.bstrVal));

			//�\���p�|�����C���ݒ�
			{
				setLine(
					aRelTableName[i], relOid,
					aRelNodeName[i], v.bstrVal,
					sp::kTableName, gID
					);
			}
		}

		// VS GridCtrl�֓o�^
		m_ipFlexGrid->put_Rows(m_LinkagePoints.size() + 1);
		int n = 1;
		for (const auto& itr : m_LinkagePoints)
		{
			CString strOid = uh::str_util::ToString(itr.oID);
			m_ipFlexGrid->put_TextMatrix(n, LAYER_COL, BSTR(itr.layerName.GetString()));
			m_ipFlexGrid->put_TextMatrix(n, OID_COL, strOid.GetBuffer());
			n++;
		}

		if (AheIsEditableTable(ipTable)) { lEditableNum++; }
	}

	DrawRelationLine();

	return BOOL(lEditableNum); // AheIsEditableTable(ipRelTable);
}

void CAttrSignalPointDlg::DrawNodePoint(const IPointPtr& ipPoint)
{
	if (ipPoint)
	{
		//���ɏ����Ă���̂͏���
		if(m_isDraw)
		{
			//�`���ď���
			EraseNodePoint(m_ipPoint);
		}

		//�`��
		DrawPoint(ipPoint);
		m_isDraw = true;
	}
	else
	{
		//���ɏ����Ă���̂͏���
		EraseNodePoint(m_ipPoint);
	}

	m_ipPoint = ipPoint;
}

void CAttrSignalPointDlg::UpdateNodePoint()
{
	if(m_isDraw)
	{
		//�������`��
		DrawPoint(m_ipPoint);
	}
}

void CAttrSignalPointDlg::EraseNodePoint(const IPointPtr& ipPoint)
{
	if(m_isDraw && ipPoint)
	{
		//�������`���ď���
		DrawPoint(ipPoint);
	}

	m_isDraw = false;
	m_ipPoint = nullptr;
}

void CAttrSignalPointDlg::ClearNodePoint()
{
	m_isDraw = false;
	m_ipPoint = nullptr;
}

void CAttrSignalPointDlg::RefreshScreen(bool drawObj/* = false*/)
{
	//::MessageBox(m_cArcHelper.GetHWND(), _T("called RefreshScreen"), AheLoadString(IDS_CAPTION), MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
	//// �O�̂��̂��c���Ă��邱�Ƃ�����̂ŁA��x�t���b�V��
	//// ToDo: ��ʑS�̂�������̂ŗv�C��
	//m_cArcHelper.GetActiveView()->PartialRefresh(static_cast<esriViewDrawPhase>(esriViewGeography | esriViewGraphics), nullptr, nullptr);
	//m_ipPoint = nullptr;

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	if(!drawObj)	//�I�u�W�F�N�g�ĕ`��Ȃ�
	{
		ClearNodePoint();
		ClearRelationLine();
	}
	else			//�I�u�W�F�N�g�ĕ`�悠��
	{
		//�ĕ`�悠��폜
		EraseNodePoint(m_ipPoint);
		UpdateRelationLine();
	}
}

void CAttrSignalPointDlg::DrawPoint(const IPointPtr& ipPoint)
{
	AheDrawPoint(m_cArcHelper.GetDisplay(), ipPoint, TRUE, kDRAW_SIZE, kDRAW_RED, kDRAW_GRN, kDRAW_BLU);
}

void __stdcall CAttrSignalPointDlg::OnKeyDownGrid()
{
	BYTE pKeyState[256] = {};
	GetKeyboardState(pKeyState);

	if (pKeyState[VK_ESCAPE] & 0x80)
	{
		//MessageBox(_T("ESC��������܂���"));
		//::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		GetParent().PostMessage(WM_CLOSE);
	}

	// �^�u�Ŏ��̃R���g���[���ֈړ��ł���悤�ɏC��
	if (pKeyState[VK_TAB] == 0) { return; }

	long lROWs, lROW;
	long lCOLs, lCOL;

	m_ipFlexGrid->get_Rows(&lROWs);
	m_ipFlexGrid->get_Cols(&lCOLs);
	m_ipFlexGrid->get_Row (&lROW);
	m_ipFlexGrid->get_Col (&lCOL);

	if ((lROW+1) == lROWs && (lCOL+1) == lCOLs)
	{
		DrawNodePoint(m_ipPoint);

		NextDlgCtrl();
	}
	else if (pKeyState[VK_SHIFT] && lROW == 1 && lCOL == 0)
	{
		PrevDlgCtrl();
	}
}

void __stdcall CAttrSignalPointDlg::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	// �E�N���b�N�ȊO�͉����������Ȃ�
	if (! (Button & 0x02)) { return; }

	// �ҏW���J�n���������Ȃ�
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()){ return; }

	POINT pt = {XTwipsToPixels(X), YTwipsToPixels(Y)};
	GetDlgItem(IDC_VSFLEXGRID_NODE).ClientToScreen(&pt);

	long lROWs, lROW;
	long lCOLs, lCOL;

	m_ipFlexGrid->get_Rows(&lROWs);
	m_ipFlexGrid->get_Cols(&lCOLs);
	m_ipFlexGrid->get_Row (&lROW);
	m_ipFlexGrid->get_Col (&lCOL);


	AHE::CMenu menu;
	menu.CreateMenu(TRUE);
	menu.InsertMenu(0, MF_BYPOSITION|MF_STRING, 1000, _T("�ǉ�"));
	menu.InsertMenu(1, MF_BYPOSITION|MF_STRING, 1001, _T("�폜"));

	INT iRet = menu.TrackPopupMenu(TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y);

	// NOTE: Change �̎����͂���ł悢�H
	BOOL bChanged = GetCurrentFeatureDef()->GetFieldDef(sp::kTableName, sp::kLampCount)->m_bChanged;
	switch (iRet)
	{
	case 1000:
		{
			m_ipFlexGrid->put_Rows(lROWs + 1);
			bChanged = TRUE;
		}
		break;
	case 1001:
		if (lROW > 0 )
		{
			CString layerName = (LPCTSTR)m_ipFlexGrid->GetTextMatrix(lROW, LAYER_COL);
			CString oidStr    = (LPCTSTR)m_ipFlexGrid->GetTextMatrix(lROW, OID_COL);
			CString str = uh::str_util::format(_T("�R�t���m�[�h[%s : %ld]���폜���Ă���낵���ł����H"), layerName, _ttol(oidStr) );

			int iRet = MessageBox(str, _T("�폜�m�F"), MB_YESNOCANCEL | MB_ICONQUESTION | MB_TOPMOST);
			if (iRet == IDYES)
			{
				m_ipFlexGrid->RemoveItem(lROW);
				bChanged = TRUE;
			}
		}
		break;
	default:
		break;
	}
	if (bChanged)
	{
		SetButton(TRUE);
		GetCurrentFeatureDef()->GetFieldDef(sp::kTableName, sp::kModifyDate)->m_bChanged = TRUE;
	}
}

void __stdcall CAttrSignalPointDlg::OnCellChangedGrid()
{
	using namespace sindy::schema;

	long Row = -1;
	long Col = -1;
	HRESULT hr1 = m_ipFlexGrid->get_Row(&Row);
	HRESULT hr2 = m_ipFlexGrid->get_Col(&Col);
	OutputDebugString(uh::str_util::format(_T("����\t%x\t%x"), hr1, hr2));

	CString featureName = (LPCTSTR)m_ipFlexGrid->GetTextMatrix(Row, LAYER_COL);
	CString gid         = (LPCTSTR)m_ipFlexGrid->GetTextMatrix(Row, OID_COL);
	if (featureName.IsEmpty() || gid.IsEmpty()) { return; }

	// TODO: SetRelationValue()�Ə��������p�����ق����ǂ�...
	IWorkspacePtr ipWorkspace;
	ITablePtr     ipTable;
	{
		IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
		if (ipFeature)
		{
			ipFeature->get_Table(&ipTable);
			IDatasetPtr ipDataset(ipTable);
			if (ipDataset) { ipDataset->get_Workspace(&ipWorkspace); }
		}
	}
	if (! (ipWorkspace && ipTable)) { return; }

	IFeatureClassPtr ipFeatureClass = AheOpenSameOwnerTable(ipWorkspace, ipTable, featureName);
	if (! ipFeatureClass) { return; }

	CString where_clause;
	where_clause.Format(_T("%s='%s'"), adam_v2_adam_ipc_feature::kGlobalID, gid);
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_WhereClause(where_clause.GetBuffer());

	IFeaturePtr       ipFeature;
	IFeatureCursorPtr ipCursor;
	ipFeatureClass->Search(ipQuery, VARIANT_TRUE, &ipCursor);
	if (ipCursor && ipCursor->NextFeature(&ipFeature) == S_OK && ipFeature)
	{
		IGeometryPtr ipShape;
		ipFeature->get_ShapeCopy(&ipShape);

		IPointPtr ipPoint(ipShape);

		EraseNodePoint(m_ipPoint);
		DrawNodePoint(ipShape);
	}
}

void CAttrSignalPointDlg::DrawRelationLine()
{
	for(auto& line : m_mDrawLine)
	{
		if(!line.second.isDraw)
		{
			DrawPolyLine( line.second.ipLine, line.second.color);
			line.second.isDraw = true;
		}
	}
}

void CAttrSignalPointDlg::EraseRelationLine()
{
	for(auto& line : m_mDrawLine)
	{
		//�������`���ď���
		if(line.second.isDraw)
		{
			DrawPolyLine( line.second.ipLine, line.second.color);
			line.second.isDraw = false;
		}
	}
}

void CAttrSignalPointDlg::ClearRelationLine()
{
	//EraseRelationLine();
	m_mDrawLine.clear();
}

void CAttrSignalPointDlg::UpdateRelationLine()
{
	for(auto& line : m_mDrawLine)
	{
		if(line.second.isDraw)
		{
			//�`���Ă���Ε`��
			DrawPolyLine( line.second.ipLine, line.second.color);
		}
	}
}

void CAttrSignalPointDlg::DrawPolyLine(const IPolylinePtr& ipLine, const DWORD& color)
{
	AheDrawPolyline(m_cArcHelper.GetDisplay(), ipLine, color, 5, esriSLSSolid, TRUE, TRUE);
}

void CAttrSignalPointDlg::setLine(
	const CString& relTableName , long relOid, 
	const CString& nodeTableName , CString nodeGid, 
	const CString& signalTableName , CString signalGid
	)
{
	using namespace sindy::schema;

	if(nodeGid.IsEmpty() || relOid < 1 || signalGid.IsEmpty())
		return;

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if(!ipFeature)
		return;

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
		return;


	// �M���@�|�C���g�̌`����擾
	IGeometryPtr ipSignalGeo;
	long signalOid(0);
	{
		IFeatureClassPtr ipFC = AheOpenSameOwnerTable(ipTable, signalTableName);
		if(!ipFC)
			return;

		CString where_clause;
		where_clause.Format(_T("%s='%s'"), adam_v2_adam_ipc_feature::kGlobalID, signalGid);
		IQueryFilterPtr ipQuery(CLSID_QueryFilter);
		ipQuery->put_WhereClause(where_clause.GetBuffer());
		IFeatureCursorPtr ipCursor;
		ipFC->Search(ipQuery, VARIANT_FALSE, &ipCursor);
		if(!ipCursor)
			return;

		IFeaturePtr ipF;
		if (ipCursor && ipCursor->NextFeature(&ipF) == S_OK && ipF)
		{
			ipF->get_ShapeCopy(&ipSignalGeo);
			ipF->get_OID(&signalOid);
		}
	}

	// �R�t����̃m�[�h�`����擾
	IGeometryPtr ipNodeGeo;
	long nodeOid(0);
	{
		IFeatureClassPtr ipFC = AheOpenSameOwnerTable(ipTable, nodeTableName);
		if(!ipFC)
		{
			return;
		}

		CString where_clause;
		IQueryFilterPtr ipQuery(CLSID_QueryFilter);
		where_clause.Format(_T("%s='%s'"), adam_v2_adam_ipc_feature::kGlobalID, nodeGid);
		ipQuery->put_WhereClause(where_clause.GetBuffer());
		IFeatureCursorPtr ipCursor;
		ipFC->Search(ipQuery, VARIANT_FALSE, &ipCursor);
		if(!ipCursor)
		{
			return;
		}
		IFeaturePtr ipF;
		if (ipCursor && ipCursor->NextFeature(&ipF) == S_OK && ipF)
		{
			ipF->get_ShapeCopy(&ipNodeGeo);
			ipF->get_OID(&nodeOid);
		}
	}

	// �M���@�|�C���g�ƕR�t����m�[�h�����ԃ|�����C���`����쐬���A
	// �`�悷�郉�C���֓o�^
	if(ipSignalGeo && ipNodeGeo)
	{
		PolyLineInfo::Key key;
		key.signalNameID.className = signalTableName;
		key.signalNameID.oid = signalOid;
		key.signalNameID.gid = signalGid;
		key.nodeNameID.className = nodeTableName;
		key.nodeNameID.oid = nodeOid;
		key.nodeNameID.gid = nodeGid;
		key.relNameID.className = relTableName;
		key.relNameID.oid = relOid;

		PolyLineInfo::Value value;

		IPolylinePtr ipLine(CLSID_Polyline);
		IPointCollectionPtr(ipLine)->AddPoint((IPointPtr)ipSignalGeo);
		IPointCollectionPtr(ipLine)->AddPoint((IPointPtr)ipNodeGeo);
		value.ipLine = ipLine;
		value.color = LINE_COLOR;
		value.isDraw = false;

		m_mDrawLine[key] = value;
	}
}

CString CAttrSignalPointDlg::getGID(const ITablePtr& table, long oid)
{
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	_IRowPtr row;
	table->GetRow(oid, &row);

	if(! row)
		return CString();

	return AheGetAttribute(row, kGlobalID).bstrVal;
}

long CAttrSignalPointDlg::getOID(const ITablePtr& table, const CString& gid)
{
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	CString whereClause = uh::str_util::format(_T("%s='%s'"), kGlobalID, gid);

	_ICursorPtr cur;
	if( FAILED( table->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &cur) ))
		return -1;

	_IRowPtr row;
	if( FAILED( cur->NextRow(&row)) || !row )
		return -1;

	long oid(0);
	row->get_OID(&oid);
	
	return oid;
}

bool CAttrSignalPointDlg::GetDoubleFromEditControl(long controlId, double& value)
{
	CString controlStr;
	CEdit(GetDlgItem(controlId)).GetWindowText(controlStr);
	double retVal = 0;
	try
	{
		retVal = boost::lexical_cast<double>(controlStr.GetString());
	}
	catch (...)
	{
		return false;
	}
	value = retVal;

	return true;
}

bool CAttrSignalPointDlg::GetTargetNodeInfo(CString& tableName, long& nodeId)
{
	// �s�����擾
	long rowCount = m_ipFlexGrid->Rows;

	// �s��2�s�����Ȃ�NG
	if (rowCount < 2)
		return false;

	// �s��2�s(�w�b�_�s���܂߂�̂�)�ł���΁A�I���Ɋւ�炸1�s�ڂ̃f�[�^���擾
	if (rowCount == 2)
	{
		CComVariant vaId = grid_util::GetValue(m_ipFlexGrid, 1, OID_COL);
		CString layerName = (LPCTSTR)m_ipFlexGrid->GetTextMatrix(1, LAYER_COL);
		vaId.ChangeType(VT_I4);
		nodeId = vaId.lVal;
		tableName = layerName;;

		return true;
	}

	// �����܂ŗ���Ƃ������Ƃ̓O���b�h��̑S�f�[�^�������B�I����1���ł���ꍇ�̂݁A����ID��Ԃ�
	// �J�����g�s������ė���
	long curRow = -1;
	m_ipFlexGrid->get_Row(&curRow);

	// �J�����g�s��1�����Ȃ�΁A�I������Ă��Ȃ��Ƃ�������(-1�͖��I����0�̓w�b�_)
	// TODO: �����s�I�����l������ƁA�������������Ƃ��Ȃ���΂Ȃ�Ȃ�
	//       �������A�����s�I��������K�v�͂Ȃ����A�폜�������܂Ƃ߂Ă͏o���Ȃ��悤�Ȃ̂ł���ł悵�Ƃ���
	//       �����I������Ă���ꍇ�͍Ō�ɑI�����ꂽ���̂��ΏۂƂȂ�
	if(curRow < 1)
		return false;

	CComVariant vaId = grid_util::GetValue(m_ipFlexGrid, curRow, OID_COL);
	CString layerName = (LPCTSTR)m_ipFlexGrid->GetTextMatrix(curRow, LAYER_COL);
	vaId.ChangeType(VT_I4);
	nodeId = vaId.lVal;
	tableName = layerName;

	return true;
}


bool CAttrSignalPointDlg::GetNodeAlt(double& nodeAlt)
{
	// �m�[�h�������Ȃ�I���ς݃m�[�h���ЂƂI�΂���B�P�m�[�h�Ȃ�B��̊֘A�Ԑ��m�[�h��]�ʌ��m�[�h�Ƃ���
	CString nodeTableName;
	long targetNodeId = 0;
	if(!GetTargetNodeInfo(nodeTableName, targetNodeId))
	{
		AheMessageBox(AheLoadString(IDS_ERR_CANNOT_SPECIFY_ONENODE), AheLoadString(IDS_ERR_TRANSFER), MB_OK);
		return false;
	}

	// �]�ʌ��m�[�h��T���B������Ȃ���΃G���[
	IFeaturePtr signalPoint = GetCurrentFeatureDef()->GetFeature();
	ITablePtr nodeTable = AheOpenSameOwnerTable(AheGetTable(signalPoint), nodeTableName);
	if (!nodeTable)
	{
		AheMessageBox(AheLoadString(IDS_ERR_CANNOT_GET_NODE_FC), AheLoadString(IDS_ERR_TRANSFER), MB_OK);
		return false;
	}
	_IRowPtr ipRow;
	nodeTable->GetRow(targetNodeId, &ipRow);
	if (!ipRow)
	{
		AheMessageBox(uh::str_util::format(AheLoadString(IDS_ERR_NOT_EXIST_NODE), targetNodeId), AheLoadString(IDS_ERR_TRANSFER), MB_OK);
		return false;
	}

	// �]�ʌ��m�[�h�{�n�㍂����΍�(Z�l)�ƂȂ�
	IPointPtr ipPoint = AheGetPoint(ipRow, 0);
	ipPoint->get_Z(&nodeAlt);

	return true;
}