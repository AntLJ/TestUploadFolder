#pragma once
#include "CnvData.h"

class MakeSHP
{
public:
	MakeSHP(void);
	~MakeSHP(void);

	/*
	 *	@brief	staticメンバ変数の初期化・空Shape作成
	 *	@param	outputDir [in] 汎用相対高さShape出力ディレクトリ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Init(LPCTSTR outputDir);

	/*
	 *	@brief　レコード変換
	 *	@note	変換データコンテナのうち、objectid == startIDであるレコードから、レコードを変換する
	 *			startID == 0の場合は、コンテナの最初から変換する。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool InsertRecord(const CnvData& cnvData, const long startID = 0);

	/*
	 *	@brief	ファイルリネーム
	 *	@note	作成したShapeファイルが一つの場合のみ行う
	 *	@param	outputDir [in] 汎用相対高さShape出力ディレクトリ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool ReName(LPCTSTR outputDir);


private:
	/*
	 *	@brief	HeightNode空Shape作成
	 *	@param	shpName
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool MakeVacuityShp(LPCTSTR shpName);
	/*
	 *	@brief	属性フィールド作成
	 *	@param	fieldName [in] フィールド名
	 *	@param	fieldType [in] フィールドタイプ
	 *	@param	editalbe [in] 編集可否
	 *	@param	nullable [in] NULL許可
	 *	@param	fieldLength [in] フィールド長
	 *	@param	precision [in] 精度
	 *	@param	ipFields [out] フィールドポインタ
	 *	@retval	true
	 *	@retval false
	 */
	bool MakeAttrField(LPCTSTR fieldName,
					   const esriFieldType fieldType,
					   const VARIANT_BOOL editable,
					   const VARIANT_BOOL nullable,
					   const long fieldLength,
					   const long precision,
					   IFieldsEditPtr& ipFields);
	/*
	 *	@brief	形状フィールド作成
	 *	@param	geometryType	[in] ジオメトリタイプ
	 *	@param	ipFields [out]	フィールドポインタ
	 *	@retval	true
	 *	@retval false
	 */
	bool MakeShapeField(const esriGeometryType geoType, IFieldsEditPtr& ipFields);

	/*
	 *	@brief	フィールドインデックス取得
	 *	@retval true 成功
	 *	@retval	false 失敗
	 */
	bool SetFieldIndex();


private:
	IWorkspacePtr m_ipWorkspace;
	IFeatureClassPtr m_ipFeatureClass;
	long m_fileCount;

	long m_objectidIndex;
	long m_linkid1Index;
	long m_divid1Index;
	long m_relHeight1Index;
	long m_linkid2Index;
	long m_divid2Index;
	long m_relHeight2Index;

	long m_shpTotalSize;
	long m_dbfTotalSize;
};

