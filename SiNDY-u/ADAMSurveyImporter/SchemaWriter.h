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

#include "FieldType.h"
#include "DataAccessor.h"

/**
 * �X�L�[�}�������݊��N���X
 *
 * DB�ڑ��AArcObject���상�\�b�h���`
 */
namespace adam_v2
{
class SchemaWriter : public DataAccessor
{
public:
	struct OGID {
		long long   OBJECTID;
		uh::tstring GLOBALID;
	};

public:
	SchemaWriter(void);
	virtual ~SchemaWriter(void);
	/**
	 * �f�[�^�ǂݍ���
	 *
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	virtual bool Read(const uh::tstring& fileName) = 0;
	/**
	 * �f�[�^��������
	 *
	 * @pre	SetSRID(...) == true && 
	 *
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	virtual bool Write(const uh::tstring& tableName) = 0;

	/**
	 * �f�[�^�N���A
	 */
	void Clear();

	/**
	 * ���\�[�X�̐ݒ�
	 *
	 * @param[in]	company	���ʉ��
	 * @param[in]	date	�f�[�^��̓�
	 * @param[in]	lot     ���b�g��
	 */
	bool SetSource(const uh::tstring& company, const uh::tstring& date, const uh::tstring& lot);

protected:
	/**
	 * @brief		���[�N�X�y�[�X�ɐڑ��\���`�F�b�N���A�ݒ�
	 *
	 * @retval true ����
	 * @retval false ���s
	 *
	 * @note setParam() == true ��Ɏ��s���邱��
	 */
	bool SetWorkspace();
	/**
	 * @brief		�ڑ��\���`�F�b�N���A�ݒ�
	 *
	 * @param[in]	�e�[�u����
	 *
	 * @retval true ����
	 * @retval false ���s
	 *
	 * @note setParam() == true ��Ɏ��s���邱��
	 */
	bool SetFeature(const uh::tstring& tableName, IFeatureClass** ptr);
	bool SetTable(const uh::tstring& tableName, ITable** ptr);

	/**
	 * �ҏW�J�n/�I��
	 *
	 * @retval true ����
	 * @retval false ���s
	 */
	bool StartEdit(const IWorkspaceEditPtr& ip_edit);
	bool StopEdit(const IWorkspaceEditPtr& ip_edit);
	bool AbortEdit(const IWorkspaceEditPtr& ip_edit);

	/**
	 * ���̓f�[�^�̑��n�n(SRID)�ݒ�
	 *
	 * @note	SRID�͉��L�̂ݑΉ�
	 *			6668	���E���n�n(JGD2011)�A�ܓx�o�x
	 *			4612	���E���n�n(JGD2000)�A�ܓx�o�x
	 *			4301	���{���n�n(TOKYO)�A�ܓx�o�x
	 *
	 * @retval	true:	����
	 * @retval	false:	���s(���Ή���SRID)
	 */
	bool SetSRID(long long sr);

	// =============================================================================
	//  ArcObject���[�e�B�e�B�n(�ǂ�����LIB�ɂ��邩������Ȃ���...�Ƃ肠��������)
	// =============================================================================
	/**
	 * Z�l����̃W�I���g���ݒ�
	 *
	 * @param[in]	ifcp_feature	��ԎQ�Ǝ擾���t�B�[�`��
	 * @param[out]	ip_point		�|�C���g�W�I���g��
	 */
	void SetZPoint(const IFeatureClassPtr& ifcp_feature, IPointPtr& ip_point);
	void SetZPolyline(const IFeatureClassPtr& ifcp_feature, IPolylinePtr& ip_polyline);
	void SetZPolygon(const IFeatureClassPtr& ifcp_feature, IPolygonPtr& ip_polygon);

	/**
	 * ���n�n�ϊ�
	 *
	 * Tokyo���n�n���� srid�Œ�`���ꂽ���n�n�ɕϊ�����
	 * 
	 * @pre		ReadCrdParam(...) == true && SetSRID(...) == true
	 *
	 * @note	JGD2000��JGD2011�͓����ϊ�
	 *
	 * @param[in]	srid	SRID
	 * @param[in]	in		�ϊ��O���W
	 * @param[out]	out		�ϊ�����W
	 */
	bool crdcnvTokyo2Ref(const WKSPointZ& in, WKSPointZ& out);
	bool crdcnvTokyo2Ref(double in_x, double in_y, double in_z, double& out_x, double& out_y, double& out_z);

	/**
	 * ��{�^ ���� CComVariant���擾
	 *
	 * @note	esriFieldTypeInteger�AesriFieldTypeDouble�AesriFieldTypeString �̂ݑΉ�
	 *
	 * @param[in]	type		esriFieldType
	 * @return		CComVariant
	 */
	static CComVariant GetVariant(const esriFieldType& type)
	{
		CComVariant vaVal;

		switch(type)
		{
		case esriFieldTypeInteger:
			vaVal.vt = VT_I4;
			break;
		case esriFieldTypeDouble:
			vaVal.vt = VT_R8;
			break;
		case esriFieldTypeString:
			vaVal.vt = VT_BSTR;
			break;
		default:
			vaVal.vt = VT_I4;
			assert(false);
			break;
		}

		return vaVal;
	}

	template<typename T>
	static CComVariant GetVariant(const esriFieldType& type, T value)
	{
		CComVariant vaVal((T)value);
		vaVal.ChangeType(GetVariant(type).vt);

		return vaVal;
	}
	template<>
	static CComVariant GetVariant<uh::tstring>(const esriFieldType& type, uh::tstring value)
	{
		CComVariant vaVal((BSTR)(value.c_str()));
		vaVal.ChangeType(GetVariant(type).vt);

		return vaVal;
	}

	static bool IsNULL(const CComVariant& vaVal)
	{
		if(vaVal.vt == VT_NULL)
		{
			return true;
		}

		return false;
	}

	/**
	 * CComVariant ���� ��{�^�l�擾
	 *
	 * @param[in]	vaVal	CComVariant
	 * @return		��{�^�l
	 *
	 */
	template<typename T>
	static T GetValue(CComVariant& vaVal)
	{
		if(IsNULL(vaVal))
		{
			//assert(false);
			return 0;
		}
		return vaVal.lVal;
	}
	template<>
	static int GetValue<int>(CComVariant& vaVal)
	{
		if(IsNULL(vaVal))
		{
			//assert(false);
			return 0;
		}
		vaVal.ChangeType(VT_I2);
		return vaVal.iVal;
	}
	template<>
	static long GetValue<long>(CComVariant& vaVal)
	{
		if(IsNULL(vaVal))
		{
			//assert(false);
			return 0L;
		}
		vaVal.ChangeType(VT_I4);
		return vaVal.lVal;
	}
	template<>
	static long long GetValue<long long>(CComVariant& vaVal)
	{
		if(IsNULL(vaVal))
		{
			//assert(false);
			return 0LL;
		}
		vaVal.ChangeType(VT_I8);
		return vaVal.llVal;
	}
	template<>
	static double GetValue<double>(CComVariant& vaVal)
	{
		if(IsNULL(vaVal))
		{
			//assert(false);
			return 0;
		}
		vaVal.ChangeType(VT_R8);
		return vaVal.dblVal;
	}
	template<>
	static uh::tstring GetValue<uh::tstring>(CComVariant& vaVal)
	{
		if(IsNULL(vaVal))
		{
			//assert(false);
			return _T("");
		}
		vaVal.ChangeType(VT_BSTR);
		uh::tstring value = COLE2T(vaVal.bstrVal);

		return value;
	}

	/**
	 * �����l�̐ݒ�(NULL���l������ IRowPtr->put_Value �̑��)
	 *
	 * @param[in]	ipRow		�C���|�[�g��
	 * @param[in]	index		�t�B�[���h�C���f�b�N�X
	 * @param[in]	field		���͒l
	 * @param[in]	nullValue	NULL�s���t�B�[���h�̏ꍇ�ɐݒ肷��NULL�l
	 */
	template<typename T>
	bool SetValue(IRowBufferPtr ipRow, long index, const FieldType<T>& field, T nullValue)
	//void SetValue(IRowBufferPtr ipRow, long index, const FieldType<int>& field, int nullValue)
	{
		CComVariant vaVal;
		
		//���L���\�b�h�𗘗p����ƃ����N�G���[...??
		//IFieldPtr ipField = ipRow->GetFields()->GetField(index);
		//esriFieldType type = ipField->GetType();

		//ipRow ���� esriFieldType�^�擾
		IFieldsPtr ipFields;
		IFieldPtr  ipField;
		esriFieldType type = esriFieldTypeInteger;
		ipRow->get_Fields(&ipFields);
		if (!ipFields)
		{
			LogUtil::printError(72, "�t�B�[���h�擾���s\t�C���f�b�N�X�ԍ�:" + std::to_string(index) + "�̃t�B�[���h�擾�Ɏ��s���܂���");
			return false;
		}
		ipFields->get_Field(index, &ipField);
		
		if (!ipField)
		{
			LogUtil::printError(72, "�t�B�[���h�擾���s\t�C���f�b�N�X�ԍ�:" + std::to_string(index) + "�̃t�B�[���h�擾�Ɏ��s���܂���");
			return false;
		}
		ipField->get_Type(&type);

		//���͂�NULL��
		if(field.GetIsNull())
		{
			//NULL�t�B�[���h��
			if(AheIsNullableField(ipField)/*ipField->GetIsNullable() == VARIANT_TRUE*/)
			{
				//NULL�ݒ�
				vaVal.vt = VT_NULL;
			}
			else
			{
				//��NULL��NULL�l�ݒ�
				vaVal = GetVariant(type, nullValue);
			}
		}
		else
		{
			//double�̌����ς��̂ŕ�����œ����
			CComBSTR vaValStr(field.GetText().c_str());
			vaVal = vaValStr;

			//���̂܂�
			//vaVal = GetVariant(type, field.GetValue());
		}

		ipRow->put_Value(index, vaVal);

		return true;
	}

	template<typename T>
	bool SetValue(IRowBufferPtr ipRow, long index, T value, T nullValue)
	//void SetValue(IRowBufferPtr ipRow, long index, const FieldType<int>& field, int nullValue)
	{
		CComVariant vaVal;
		
		//���L���\�b�h�𗘗p����ƃ����N�G���[...??
		//IFieldPtr ipField = ipRow->GetFields()->GetField(index);
		//esriFieldType type = ipField->GetType();

		//ipRow ���� esriFieldType�^�擾
		IFieldsPtr ipFields;
		IFieldPtr  ipField;
		esriFieldType type = esriFieldTypeInteger;
		ipRow->get_Fields(&ipFields);
		if (!ipFields)
		{
			LogUtil::printError(72, "�t�B�[���h�擾���s\t�C���f�b�N�X�ԍ�:" + std::to_string(index) + "�̃t�B�[���h�擾�Ɏ��s���܂���");
			return false;
		}
		ipFields->get_Field(index, &ipField);
		
		if (!ipField)
		{
			LogUtil::printError(72, "�t�B�[���h�擾���s\t�C���f�b�N�X�ԍ�:" + std::to_string(index) + "�̃t�B�[���h�擾�Ɏ��s���܂���");
			return false;
		}
		ipField->get_Type(&type);

		//NULL�`�F�b�N�Ȃ�
		//���̂܂ܓ����
		vaVal = GetVariant(type, value);
		
		ipRow->put_Value(index, vaVal);

		return true;
	}

	bool SetNULL(IRowBufferPtr ipRow, long index);

	/**
	 * GLOBALID�̐ݒ�
	 */
	bool SetGlobalId(IRowBufferPtr ipRow, long index);

	
	/**
	 * SOUCREID ����v����t�B�[�`���擾
	 *
	 * @note	�ŏ��Ɍ��������t�B�[�`�����擾
	 *
	 * @param[in]	ipFC				�����Ώۂ̃t�B�[�`���N���X
	 * @param[in]	source_id			�����l
	 * @param[in]	field_name			�����t�B�[���h��
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	same_company		���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 *
	 * @return	�t�B�[�`��
	 */
	IRowBufferPtr GetRow(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);
	IFeaturePtr GetFeature(const IFeatureClassPtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);
	IRowBufferPtr GetRow(const ITablePtr& ipFC, uh::tstring source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);
	IFeaturePtr GetFeature(const IFeatureClassPtr& ipFC, uh::tstring source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * SOUCREID ����v����t�B�[�`�������݂��邩
	 *
	 * @param[in]	ipFC				�����Ώۂ̃t�B�[�`���N���X
	 * @param[in]	source_id			�����l
	 * @param[in]	field_name			�����t�B�[���h��
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	same_company		���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 *
	 * @retval	true	���݂���
	 * @retval	false	���݂��Ȃ�
	 *
	 * @deprecated	DB���擾���邽�ߕp�����p����ƃp�t�H�[�}���X�ɉe������B�g�����ɒ��ӁB
	 */
	bool ExistRow(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * SOUCREID ����v����t�B�[�`����GLOBALID�擾
	 *
	 * @param[in]	ipFC				�����Ώۂ̃t�B�[�`���N���X
	 * @param[in]	source_id			�����l
	 * @param[in]	field_name_sid		�����t�B�[���h��
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	field_name_guid		GLOBALID�t�B�[���h��
	 * @param[in]	same_company		���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 *
	 * @return	GLOBALID	�擾�ł��Ȃ��ꍇ�� _T("")
	 *
	 * @TODO	DB���擾�B�L���b�V�����������p�t�H�[�}���X�������B
	 */
	uh::tstring GetGID(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), uh::tstring field_name_guid = _T("GLOBALID"), bool same_company = true);

	/**
	 * SOUCREID ����v����t�B�[�`����OBJECTID�擾
	 *
	 * @param[in]	ipFC				�����Ώۂ̃t�B�[�`���N���X
	 * @param[in]	source_id			�����l
	 * @param[in]	field_name_sid		�����t�B�[���h��
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	field_name_oid		OBJECTID�t�B�[���h��
	 * @param[in]	same_company		���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 *
	 * @return	GLOBALID	�擾�ł��Ȃ��ꍇ�� -1
	 *
	 * @TODO	DB���擾�B�L���b�V�����������p�t�H�[�}���X�������B
	 */
	long long GetOID(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), uh::tstring field_name_oid = _T("OBJECTID"), bool same_company = true);

	/**
	 * �����l�̎擾
	 *
	 * @param[in]	ipRow				�C���|�[�g��
	 * @param[in]	source_id			�����l
	 * @param[in]	field_name_sid		�����t�B�[���h��
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	same_company		���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 *
	 * @return		�����l(�擾�ł��Ȃ��ꍇ�́A0(���̒l�𗘗p����邱�Ƃ͑z�肵�Ă��Ȃ�))
	 */
	template<typename T>
	T GetValue(const ITablePtr& ipFC,  long long source_id, uh::tstring target_field_name, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true)
	{
		//T retVal;

		if(!ipFC)
		{
			assert(false);
			return 0;
		}

		IRowBufferPtr ipRow = GetRow(ipFC, source_id, field_name_sid, field_name_source, same_company);

		if(!ipRow)
		{
			assert(false);
			return 0;
		}

		CComVariant vaVal = AheGetAttribute(ipRow, target_field_name.c_str());

		return GetValue<T>(vaVal);
	}

	/**
	 * �����N�̍\���_�����擾
	 *
	 * @param[in]	ipF	�����N�t�B�[�`��
	 * @return		�\���_��
	 */
	long GetPointCount4Link(const IFeaturePtr& ipF);

	/**
	 * �|�C���g�̍\���_�����擾
	 *
	 * �����NID
	 *
	 * @param[in]	ipLinkFC			�|�C���g�t�B�[�`��
	 * @param[in]	ipNodeT				�|�C���g��e�[�u��
	 * @param[in]	fieldNama_link		�����Ώۂ̃|�C���g��̃����N�t�B�[���h��
	 * @param[in]	linkSid				�����Ώۂ̃����NID
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	same_company		���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 *
	 * @return		�\���_��
	 */
	long GetPointCount4Point(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNodeT, const uh::tstring& fieldNama_link, long long linkSid, uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * �����N�ɕR�t���|�C���gID����擾
	 *
	 * @param[in]	ipLinkFC			�|�C���g�t�B�[�`��
	 * @param[in]	ipNodeT				�|�C���g��e�[�u��
	 * @param[in]	fieldNama_link		�����Ώۂ̃|�C���g��̃����N�t�B�[���h��
	 * @param[in]	linkSid				�����Ώۂ̃����NID
	 * @param[in]	field_name_target	�擾�Ώۂ̃t�B�[���h��
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	same_company		���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 *
	 * @return		�L�[:�V�[�P���X�ԍ��A�l:�擾�Ώےl
	 */
	std::map<int, uh::tstring> GetPointIDs(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNodeT, const uh::tstring& fieldNama_link, long long linkSid, uh::tstring field_name_target, uh::tstring field_name_sequence = _T("SEQUENCE"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * �V�[�P���X�ŏ�/�ő�̃m�[�hGID�擾
	 *
	 * @param[in]	ipNodeFC			�|�C���g�t�B�[�`��
	 * @param[in]	link_id				�����NID
	 * @param[in]	field_name_link_gid	NQ�����N�t�B�[���h��
	 * @param[in]	field_name_node_gid	NQ�m�[�h�t�B�[���h��
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	same_company		���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 *
	 */
	uh::tstring GetMinSequenceNodeGID(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNqT, long long link_id, uh::tstring field_name_link_gid, uh::tstring field_name_node_gid, uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);
	uh::tstring GetMaxSequenceNodeGID(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNqT, long long link_id, uh::tstring field_name_link_gid, uh::tstring field_name_node_gid, uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * �֘A�e�[�u���ɕR�t���t�B�[�`��GID��̎擾
	 *
	 * @param[in]	ipRelT					�֘A�e�[�u��
	 * @param[in]	ipObjFC					�擾�Ώۃt�B�[�`��
	 * @param[in]	ipTargetFC				�����Ώۃt�B�[�`��
	 * @param[in]	targetSID				SID
	 * @param[in]	field_name_obj_gid		�֘A�e�[�u���̎擾�Ώۃt�B�[�`��GID�t�B�[���h��
	 * @param[in]	field_name_target_gid	�֘A�e�[�u���̎擾�Ώۃt�B�[�`��GID�t�B�[���h��
	 * @param[in]	query					�ǉ��N�G��
	 * @param[in]	field_name_source		SOUCRE�t�B�[���h��
	 * @param[in]	same_company			���ꑪ�ʉ��(SOURCE�t�B�[���h)�ōi�荞�ނ�
	 */
	std::vector<uh::tstring> GetFeature4Rel(const ITablePtr& ipRelT, const IFeatureClassPtr& ipObjFC, const IFeatureClassPtr& ipTargetFC, long long targetSID, uh::tstring field_name_obj_gid, uh::tstring field_name_target_gid, uh::tstring query = _T(""), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * �N�G���q�b�g�����̎擾
	 */
	long GetCount( const ITablePtr& ipT, const uh::tstring& whereQuery, uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * �o�^�ς�SOURCE_ID���X�g�̎擾
	 *
	 * @param[out]	list				�o�^�ς�SOURCE_ID���X�g
	 * @param[in]	ipTable				�擾�Ώۃt�B�[�`��/�e�[�u��
	 * @param[in]	field_name_sid		SOUCREID�t�B�[���h��
	 * @param[in]	field_name_source	SOUCRE�t�B�[���h��
	 * @param[in]	same_source			����\�[�X(�쐬��� + �f�[�^��̓� + ���b�g��) �ōi�荞�ނ�
	 * @retval	true	���X�g�擾����
	 * @retval	false	���X�g�擾���s(0�����X�g���܂�)
	 */
	bool GetRegisterdSIDList(
		std::set<long long>& list,
		const ITablePtr& ipTable,
		uh::tstring field_name_sid = _T("SOURCEID"),
		uh::tstring field_name_source = _T("SOURCE"),
		bool same_source = true
		);

	/**
	* �o�^�ς�SOURCE_ID���X�g�̎擾
	*
	* @param[out]	dic					BJECTID �� GLOBALID �̎���
	* @param[in]	ipTable				�擾�Ώۃt�B�[�`��/�e�[�u��
	* @param[in]	field_name_sid		SOUCREID�t�B�[���h��
	* @param[in]	field_name_oid      OBJECTID�t�B�[���h��
	* @param[in]	field_name_gid      GLOBADID�t�B�[���h��
	* @param[in]	field_name_source	SOUCRE�t�B�[���h��
	* @param[in]	same_source			����\�[�X(�쐬��� + �f�[�^��̓� + ���b�g��) �ōi�荞�ނ�
	* @retval	true	���X�g�擾����
	* @retval	false	���X�g�擾���s(0�����X�g���܂�)
	*/
	bool GetRegisterdOGIDList(
		std::map<long long, OGID>& dic,
		const ITablePtr& ipTable,
		uh::tstring field_name_sid = _T("SOURCEID"),
		uh::tstring field_name_oid = _T("OBJECTID"),
		uh::tstring field_name_gid = _T("GLOBALID"),
		uh::tstring field_name_source = _T("SOURCE"),
		bool same_source = true
		);

private:
	//�Ή����鑪�n�nSRID
	static const long long SRID_TOKYO;		///< ���E���n�n(JGD2011)�A�ܓx�o�x
	static const long long SRID_JGD2000;	///< ���E���n�n(JGD2000)�A�ܓx�o�x
	static const long long SRID_JGD2011;	///< ���{���n�n(TOKYO)�A�ܓx�o�x
protected:
	IWorkspacePtr	_workspace;		///< ���[�N�X�y�[�X

	long long		_srid;					///< SRID
	uh::tstring		_source;				///< ���\�[�X
	uh::tstring		_company;				///< �쐬���
	uh::tstring		_date;					///< �f�[�^��̓�
	uh::tstring		_lot;					///< ���b�g��

#ifdef _USE_FLUSH_COUNT
	static const long long FLUSH_COUNT_LIMIT;
	long long _flush_count;
#endif
};
typedef std::shared_ptr<SchemaWriter> SchemaWriterPtr;

};	//namespace adam_v2

