#pragma once

struct RoadWalkLinkRec
{
	long			e_divid;
	IGeometryPtr	e_ipGeoemtry;

	RoadWalkLinkRec()
		:e_divid(-1)
	{}

	bool operator < (const RoadWalkLinkRec roadWalkLinkRec) const {
		return e_divid < roadWalkLinkRec.e_divid;
	}
};

class RoadWalkLinkInfo
{
public:
	RoadWalkLinkInfo(void);
	~RoadWalkLinkInfo(void);


	/*
	 *	@brief	staticメンバ変数の初期化
	 *	@note	ルートパスの設定のみ
	 *	@param	shpRootPath [in] 道路・歩行者Shape格納フォルダルートパス
	 */
	void Init(LPCTSTR shpRootPath){
		m_shpRootPath = shpRootPath;
	};

	/*
	 *	@brief	変換対象道路歩行者リンクセット
	 *	@param	mesh [in] 変換対象メッシュ
	 *	@retval	true 成功
	 *	@retval false 失敗
	 */
	bool SetRoadWalkLinkInfo(const long secondMesh);

	/*
	 *	@brief	任意のobjectidを持つ道路・歩行者Shape情報のイテレータを取得(const_iterator)
	 *	@param	objectid [in] 検索するobjectid
	 *	@return	コンストイテレータ
	 */
	std::map<long, std::set<RoadWalkLinkRec>>::const_iterator find(const long objectid) const
	{
		return m_recordInfo.find(objectid);
	}

	/*
	 *	@brief	道路・歩行者Shape情報コンテナの終端を指すイテレータ取得(const_iterator)
	 *	@return コンストイテレータ
	 */
	std::map<long, std::set<RoadWalkLinkRec>>::const_iterator end() const
	{
		return m_recordInfo.end();
	}


private:
	/*
	 *	@brief	インデックス取得
	 *	@param	ipFeatureClass [in] 道路・歩行者リンクフィーチャクラス
	 *	@param	shpName [in] shpファイル名
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetIndex(const IFeatureClassPtr& ipFeatureClass, LPCTSTR shpName);

	/*
	 *	@brief	道路・歩行者リンクレコード情報取得
	 *	@param	ipFeature [in] 道路・歩行者リンクフィーチャ
	 *	@param	shpName [in] shpファイル名
	 *	@param	objectid [out] objectid
	 *	@param	roadWalkLinkRec [out] 道路・歩行者レコード情報
	 *	@retval	true 成功
	 *	@retval false 失敗
	 */
	bool GetRecInfo(const IFeaturePtr& ipFeature, LPCTSTR shpName, long& objectid, RoadWalkLinkRec& roadWalkLinkRec);

private:
	static	CString m_shpRootPath;								//!<	道路・歩行者Shape格納フォルダルートパス
	std::map<long, std::set<RoadWalkLinkRec>>	m_recordInfo;	//!<	道路・歩行者Shape情報(キーobjectid)

	static	long m_objectidIndex;
	static	long m_dividIndex;
};

