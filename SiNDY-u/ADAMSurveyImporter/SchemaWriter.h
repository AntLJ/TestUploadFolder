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
 * スキーマ書き込み基底クラス
 *
 * DB接続、ArcObject操作メソッドを定義
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
	 * データ読み込み
	 *
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	virtual bool Read(const uh::tstring& fileName) = 0;
	/**
	 * データ書き込み
	 *
	 * @pre	SetSRID(...) == true && 
	 *
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	virtual bool Write(const uh::tstring& tableName) = 0;

	/**
	 * データクリア
	 */
	void Clear();

	/**
	 * 情報ソースの設定
	 *
	 * @param[in]	company	測量会社
	 * @param[in]	date	データ受領日
	 * @param[in]	lot     ロット名
	 */
	bool SetSource(const uh::tstring& company, const uh::tstring& date, const uh::tstring& lot);

protected:
	/**
	 * @brief		ワークスペースに接続可能かチェックし、設定
	 *
	 * @retval true 成功
	 * @retval false 失敗
	 *
	 * @note setParam() == true 後に実行すること
	 */
	bool SetWorkspace();
	/**
	 * @brief		接続可能かチェックし、設定
	 *
	 * @param[in]	テーブル名
	 *
	 * @retval true 成功
	 * @retval false 失敗
	 *
	 * @note setParam() == true 後に実行すること
	 */
	bool SetFeature(const uh::tstring& tableName, IFeatureClass** ptr);
	bool SetTable(const uh::tstring& tableName, ITable** ptr);

	/**
	 * 編集開始/終了
	 *
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool StartEdit(const IWorkspaceEditPtr& ip_edit);
	bool StopEdit(const IWorkspaceEditPtr& ip_edit);
	bool AbortEdit(const IWorkspaceEditPtr& ip_edit);

	/**
	 * 入力データの測地系(SRID)設定
	 *
	 * @note	SRIDは下記のみ対応
	 *			6668	世界測地系(JGD2011)、緯度経度
	 *			4612	世界測地系(JGD2000)、緯度経度
	 *			4301	日本測地系(TOKYO)、緯度経度
	 *
	 * @retval	true:	成功
	 * @retval	false:	失敗(未対応のSRID)
	 */
	bool SetSRID(long long sr);

	// =============================================================================
	//  ArcObjectユーティティ系(どこかのLIBにあるかもしれないが...とりあえず自作)
	// =============================================================================
	/**
	 * Z値ありのジオメトリ設定
	 *
	 * @param[in]	ifcp_feature	空間参照取得元フィーチャ
	 * @param[out]	ip_point		ポイントジオメトリ
	 */
	void SetZPoint(const IFeatureClassPtr& ifcp_feature, IPointPtr& ip_point);
	void SetZPolyline(const IFeatureClassPtr& ifcp_feature, IPolylinePtr& ip_polyline);
	void SetZPolygon(const IFeatureClassPtr& ifcp_feature, IPolygonPtr& ip_polygon);

	/**
	 * 測地系変換
	 *
	 * Tokyo測地系から sridで定義された測地系に変換する
	 * 
	 * @pre		ReadCrdParam(...) == true && SetSRID(...) == true
	 *
	 * @note	JGD2000とJGD2011は同じ変換
	 *
	 * @param[in]	srid	SRID
	 * @param[in]	in		変換前座標
	 * @param[out]	out		変換後座標
	 */
	bool crdcnvTokyo2Ref(const WKSPointZ& in, WKSPointZ& out);
	bool crdcnvTokyo2Ref(double in_x, double in_y, double in_z, double& out_x, double& out_y, double& out_z);

	/**
	 * 基本型 から CComVariantを取得
	 *
	 * @note	esriFieldTypeInteger、esriFieldTypeDouble、esriFieldTypeString のみ対応
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
	 * CComVariant から 基本型値取得
	 *
	 * @param[in]	vaVal	CComVariant
	 * @return		基本型値
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
	 * 属性値の設定(NULLを考慮した IRowPtr->put_Value の代替)
	 *
	 * @param[in]	ipRow		インポート先
	 * @param[in]	index		フィールドインデックス
	 * @param[in]	field		入力値
	 * @param[in]	nullValue	NULL不許可フィールドの場合に設定するNULL値
	 */
	template<typename T>
	bool SetValue(IRowBufferPtr ipRow, long index, const FieldType<T>& field, T nullValue)
	//void SetValue(IRowBufferPtr ipRow, long index, const FieldType<int>& field, int nullValue)
	{
		CComVariant vaVal;
		
		//下記メソッドを利用するとリンクエラー...??
		//IFieldPtr ipField = ipRow->GetFields()->GetField(index);
		//esriFieldType type = ipField->GetType();

		//ipRow から esriFieldType型取得
		IFieldsPtr ipFields;
		IFieldPtr  ipField;
		esriFieldType type = esriFieldTypeInteger;
		ipRow->get_Fields(&ipFields);
		if (!ipFields)
		{
			LogUtil::printError(72, "フィールド取得失敗\tインデックス番号:" + std::to_string(index) + "のフィールド取得に失敗しました");
			return false;
		}
		ipFields->get_Field(index, &ipField);
		
		if (!ipField)
		{
			LogUtil::printError(72, "フィールド取得失敗\tインデックス番号:" + std::to_string(index) + "のフィールド取得に失敗しました");
			return false;
		}
		ipField->get_Type(&type);

		//入力がNULLか
		if(field.GetIsNull())
		{
			//NULL可フィールドか
			if(AheIsNullableField(ipField)/*ipField->GetIsNullable() == VARIANT_TRUE*/)
			{
				//NULL設定
				vaVal.vt = VT_NULL;
			}
			else
			{
				//非NULLのNULL値設定
				vaVal = GetVariant(type, nullValue);
			}
		}
		else
		{
			//doubleの桁が変わるので文字列で入れる
			CComBSTR vaValStr(field.GetText().c_str());
			vaVal = vaValStr;

			//そのまま
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
		
		//下記メソッドを利用するとリンクエラー...??
		//IFieldPtr ipField = ipRow->GetFields()->GetField(index);
		//esriFieldType type = ipField->GetType();

		//ipRow から esriFieldType型取得
		IFieldsPtr ipFields;
		IFieldPtr  ipField;
		esriFieldType type = esriFieldTypeInteger;
		ipRow->get_Fields(&ipFields);
		if (!ipFields)
		{
			LogUtil::printError(72, "フィールド取得失敗\tインデックス番号:" + std::to_string(index) + "のフィールド取得に失敗しました");
			return false;
		}
		ipFields->get_Field(index, &ipField);
		
		if (!ipField)
		{
			LogUtil::printError(72, "フィールド取得失敗\tインデックス番号:" + std::to_string(index) + "のフィールド取得に失敗しました");
			return false;
		}
		ipField->get_Type(&type);

		//NULLチェックなし
		//そのまま入れる
		vaVal = GetVariant(type, value);
		
		ipRow->put_Value(index, vaVal);

		return true;
	}

	bool SetNULL(IRowBufferPtr ipRow, long index);

	/**
	 * GLOBALIDの設定
	 */
	bool SetGlobalId(IRowBufferPtr ipRow, long index);

	
	/**
	 * SOUCREID が一致するフィーチャ取得
	 *
	 * @note	最初に見つかったフィーチャを取得
	 *
	 * @param[in]	ipFC				検索対象のフィーチャクラス
	 * @param[in]	source_id			検索値
	 * @param[in]	field_name			検索フィールド名
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	same_company		同一測量会社(SOURCEフィールド)で絞り込むか
	 *
	 * @return	フィーチャ
	 */
	IRowBufferPtr GetRow(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);
	IFeaturePtr GetFeature(const IFeatureClassPtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);
	IRowBufferPtr GetRow(const ITablePtr& ipFC, uh::tstring source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);
	IFeaturePtr GetFeature(const IFeatureClassPtr& ipFC, uh::tstring source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * SOUCREID が一致するフィーチャが存在するか
	 *
	 * @param[in]	ipFC				検索対象のフィーチャクラス
	 * @param[in]	source_id			検索値
	 * @param[in]	field_name			検索フィールド名
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	same_company		同一測量会社(SOURCEフィールド)で絞り込むか
	 *
	 * @retval	true	存在する
	 * @retval	false	存在しない
	 *
	 * @deprecated	DBより取得するため頻発利用するとパフォーマンスに影響あり。使い方に注意。
	 */
	bool ExistRow(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * SOUCREID が一致するフィーチャのGLOBALID取得
	 *
	 * @param[in]	ipFC				検索対象のフィーチャクラス
	 * @param[in]	source_id			検索値
	 * @param[in]	field_name_sid		検索フィールド名
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	field_name_guid		GLOBALIDフィールド名
	 * @param[in]	same_company		同一測量会社(SOURCEフィールド)で絞り込むか
	 *
	 * @return	GLOBALID	取得できない場合は _T("")
	 *
	 * @TODO	DBより取得。キャッシュした方がパフォーマンスがいい。
	 */
	uh::tstring GetGID(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), uh::tstring field_name_guid = _T("GLOBALID"), bool same_company = true);

	/**
	 * SOUCREID が一致するフィーチャのOBJECTID取得
	 *
	 * @param[in]	ipFC				検索対象のフィーチャクラス
	 * @param[in]	source_id			検索値
	 * @param[in]	field_name_sid		検索フィールド名
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	field_name_oid		OBJECTIDフィールド名
	 * @param[in]	same_company		同一測量会社(SOURCEフィールド)で絞り込むか
	 *
	 * @return	GLOBALID	取得できない場合は -1
	 *
	 * @TODO	DBより取得。キャッシュした方がパフォーマンスがいい。
	 */
	long long GetOID(const ITablePtr& ipFC, long long source_id, uh::tstring field_name_sid = _T("SOURCEID"), uh::tstring field_name_source = _T("SOURCE"), uh::tstring field_name_oid = _T("OBJECTID"), bool same_company = true);

	/**
	 * 属性値の取得
	 *
	 * @param[in]	ipRow				インポート先
	 * @param[in]	source_id			検索値
	 * @param[in]	field_name_sid		検索フィールド名
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	same_company		同一測量会社(SOURCEフィールド)で絞り込むか
	 *
	 * @return		属性値(取得できない場合は、0(この値を利用されることは想定していない))
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
	 * リンクの構成点数を取得
	 *
	 * @param[in]	ipF	リンクフィーチャ
	 * @return		構成点数
	 */
	long GetPointCount4Link(const IFeaturePtr& ipF);

	/**
	 * ポイントの構成点数を取得
	 *
	 * リンクID
	 *
	 * @param[in]	ipLinkFC			ポイントフィーチャ
	 * @param[in]	ipNodeT				ポイント列テーブル
	 * @param[in]	fieldNama_link		検索対象のポイント列のリンクフィールド名
	 * @param[in]	linkSid				検索対象のリンクID
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	same_company		同一測量会社(SOURCEフィールド)で絞り込むか
	 *
	 * @return		構成点数
	 */
	long GetPointCount4Point(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNodeT, const uh::tstring& fieldNama_link, long long linkSid, uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * リンクに紐付くポイントID列を取得
	 *
	 * @param[in]	ipLinkFC			ポイントフィーチャ
	 * @param[in]	ipNodeT				ポイント列テーブル
	 * @param[in]	fieldNama_link		検索対象のポイント列のリンクフィールド名
	 * @param[in]	linkSid				検索対象のリンクID
	 * @param[in]	field_name_target	取得対象のフィールド名
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	same_company		同一測量会社(SOURCEフィールド)で絞り込むか
	 *
	 * @return		キー:シーケンス番号、値:取得対象値
	 */
	std::map<int, uh::tstring> GetPointIDs(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNodeT, const uh::tstring& fieldNama_link, long long linkSid, uh::tstring field_name_target, uh::tstring field_name_sequence = _T("SEQUENCE"), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * シーケンス最小/最大のノードGID取得
	 *
	 * @param[in]	ipNodeFC			ポイントフィーチャ
	 * @param[in]	link_id				リンクID
	 * @param[in]	field_name_link_gid	NQリンクフィールド名
	 * @param[in]	field_name_node_gid	NQノードフィールド名
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	same_company		同一測量会社(SOURCEフィールド)で絞り込むか
	 *
	 */
	uh::tstring GetMinSequenceNodeGID(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNqT, long long link_id, uh::tstring field_name_link_gid, uh::tstring field_name_node_gid, uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);
	uh::tstring GetMaxSequenceNodeGID(const IFeatureClassPtr& ipLinkFC, const ITablePtr& ipNqT, long long link_id, uh::tstring field_name_link_gid, uh::tstring field_name_node_gid, uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * 関連テーブルに紐付くフィーチャGID列の取得
	 *
	 * @param[in]	ipRelT					関連テーブル
	 * @param[in]	ipObjFC					取得対象フィーチャ
	 * @param[in]	ipTargetFC				検索対象フィーチャ
	 * @param[in]	targetSID				SID
	 * @param[in]	field_name_obj_gid		関連テーブルの取得対象フィーチャGIDフィールド名
	 * @param[in]	field_name_target_gid	関連テーブルの取得対象フィーチャGIDフィールド名
	 * @param[in]	query					追加クエリ
	 * @param[in]	field_name_source		SOUCREフィールド名
	 * @param[in]	same_company			同一測量会社(SOURCEフィールド)で絞り込むか
	 */
	std::vector<uh::tstring> GetFeature4Rel(const ITablePtr& ipRelT, const IFeatureClassPtr& ipObjFC, const IFeatureClassPtr& ipTargetFC, long long targetSID, uh::tstring field_name_obj_gid, uh::tstring field_name_target_gid, uh::tstring query = _T(""), uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * クエリヒット件数の取得
	 */
	long GetCount( const ITablePtr& ipT, const uh::tstring& whereQuery, uh::tstring field_name_source = _T("SOURCE"), bool same_company = true);

	/**
	 * 登録済みSOURCE_IDリストの取得
	 *
	 * @param[out]	list				登録済みSOURCE_IDリスト
	 * @param[in]	ipTable				取得対象フィーチャ/テーブル
	 * @param[in]	field_name_sid		SOUCREIDフィールド名
	 * @param[in]	field_name_source	SOUCREフィールド名
	 * @param[in]	same_source			同一ソース(作成会社 + データ受領日 + ロット名) で絞り込むか
	 * @retval	true	リスト取得成功
	 * @retval	false	リスト取得失敗(0件リストも含む)
	 */
	bool GetRegisterdSIDList(
		std::set<long long>& list,
		const ITablePtr& ipTable,
		uh::tstring field_name_sid = _T("SOURCEID"),
		uh::tstring field_name_source = _T("SOURCE"),
		bool same_source = true
		);

	/**
	* 登録済みSOURCE_IDリストの取得
	*
	* @param[out]	dic					BJECTID と GLOBALID の辞書
	* @param[in]	ipTable				取得対象フィーチャ/テーブル
	* @param[in]	field_name_sid		SOUCREIDフィールド名
	* @param[in]	field_name_oid      OBJECTIDフィールド名
	* @param[in]	field_name_gid      GLOBADIDフィールド名
	* @param[in]	field_name_source	SOUCREフィールド名
	* @param[in]	same_source			同一ソース(作成会社 + データ受領日 + ロット名) で絞り込むか
	* @retval	true	リスト取得成功
	* @retval	false	リスト取得失敗(0件リストも含む)
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
	//対応する測地系SRID
	static const long long SRID_TOKYO;		///< 世界測地系(JGD2011)、緯度経度
	static const long long SRID_JGD2000;	///< 世界測地系(JGD2000)、緯度経度
	static const long long SRID_JGD2011;	///< 日本測地系(TOKYO)、緯度経度
protected:
	IWorkspacePtr	_workspace;		///< ワークスペース

	long long		_srid;					///< SRID
	uh::tstring		_source;				///< 情報ソース
	uh::tstring		_company;				///< 作成会社
	uh::tstring		_date;					///< データ受領日
	uh::tstring		_lot;					///< ロット名

#ifdef _USE_FLUSH_COUNT
	static const long long FLUSH_COUNT_LIMIT;
	long long _flush_count;
#endif
};
typedef std::shared_ptr<SchemaWriter> SchemaWriterPtr;

};	//namespace adam_v2

