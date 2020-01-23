#pragma once


struct HeightNodeRec
{
	IGeometryPtr e_ipGeometry;
	long e_linkid1;
	long e_height1;
	long e_table1;
	long e_linkid2;
	long e_height2;
	long e_table2;

	HeightNodeRec()
		:e_linkid1(-1),e_height1(0),e_table1(-1),
		 e_linkid2(-1),e_height2(0),e_table2(-1)
	{}
};


/**
 *	@brief	HeightNodeInfo情報クラス
 */

class HeightNodeInfo
{
public:
	HeightNodeInfo(const IFeatureClassPtr& ipFeatureClass, const ISpatialReferencePtr& ipSpRef);
	~HeightNodeInfo(void);

	/*
	 *	@brief	staticメンバ変数の初期化
	 *	@note	インデックス取得
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Init();

	/*
	 *	@brief	変換対象HeightNodeセット
	 *	@param	ipMeshGeoemtry [in] メッシュ形状
	 *	@retval true 成功
	 *	@retval	false 失敗
	 */
	bool SetHeightNodeInfo(const IGeometryPtr ipMeshGeometry);

	/*
	 *	@brief HeightNodeコンテナの先頭を指すイテレータ取得(const_iterator)
	 *	@return コンストイテレータ
	 */
	std::map<long, HeightNodeRec>::const_iterator begin() const
	{
		return m_recordInfo.begin();
	}

	/*
	 *	@brief HeightNodeコンテナの終端を指すイテレータ取得(const_iterator)
	 *	@return コンストイテレータ
	 */
	std::map<long, HeightNodeRec>::const_iterator end() const
	{
		return m_recordInfo.end();
	}

	/*
	 *	@brief	HeightNodeレイヤの空間参照取得
	 *	@return ipSpatialReferencePtr
	 */
	ISpatialReferencePtr GetSpRef() const
	{
		return m_ipSpRef;
	}

private:
	/*
	 *	@brief	HeightNodeレコード情報取得
	 *	@param	ipFeature [in] HeightNodeフィーチャ
	 *	@param	heightNodeRec [out] HeightNodeレコード情報
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetRecInfo(const IFeaturePtr& ipFeature, HeightNodeRec& heightNodeRec);


private:
	IFeatureClassPtr m_ipFeatureClass;
	ISpatialReferencePtr m_ipSpRef;
	
	// 以下各種フィールドのindexはInit実行時に取得し、インスタンス間で使いまわしたいのでstaticとしている
	static long m_linkid1Index;
	static long m_height1Index;
	static long m_linkTable1Index;
	static long m_linkid2Index;
	static long m_height2Index;
	static long m_linkTable2Index;

	std::map<long, HeightNodeRec> m_recordInfo;	//!< HeightNode情報(キー：objectid)
};

