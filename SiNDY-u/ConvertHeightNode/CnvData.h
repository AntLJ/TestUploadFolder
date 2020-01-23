#pragma once
#include "HeightNodeInfo.h"
#include "RoadWalkLinkInfo.h"

struct OutSHPRec
{
	IGeometryPtr e_ipGeoemtry;
	long e_linkid1;
	long e_divid1;
	long e_height1;
	long e_linkid2;
	long e_divid2;
	long e_height2;

	OutSHPRec(IGeometryPtr ipGeometry,
			  long linkid1, long divid1, long height1,
			  long linkid2, long divid2, long height2)
		:e_ipGeoemtry(ipGeometry),
		 e_linkid1(linkid1), e_divid1(divid1), e_height1(height1),
		 e_linkid2(linkid2), e_divid2(divid2), e_height2(height2)
	{}
};

class CnvData
{
public:
	CnvData(void);
	~CnvData(void);

	/*
	 *	@brief	staticメンバ変数の初期化
	 *	@note	交差判定時のバッファの設定
	 *	@param	buffer [in] バッファ
	 */
	void Init(const double& buffer) {
		m_buffer = buffer;
	}

	/*
	 *	@brief	相対高さShape変換
	 *	@param	secondMesh [in] 変換対象メッシュ
	 *	@param  heightNodeInfo [in] HeightNode情報
	 *	@param	roadWalkLinkInfo [in] 道路・歩行者リンク情報
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool ConverteHeightNode(const long secondMesh, const HeightNodeInfo& heightNodeInfo, const RoadWalkLinkInfo& roadWalkLinkInfo);

	/*
	 *	@brief	変換データコンテナの先頭を指すイテレータ取得(const_iterator)
	 *	@return コンストイテレータ
	 */
	std::map<long, OutSHPRec>::const_iterator begin() const
	{
		return m_recordInfo.begin();
	}
	/*
	 *	@brief  任意のobjectidを持つ変換データコンテナのイテレータを取得(const_iterator)
	 *	@param	objectid [in] 検索するobjectid
	 *	@return コンストイテレータ
	 */
	std::map<long, OutSHPRec>::const_iterator find(const long objectid) const
	{
		return m_recordInfo.find(objectid);
	}

	/*
	 *	@brief	変換データコンテナの終端を指すイテレータ取得(const_iterator)
	 *	@return コンストイテレータ
	 */
	std::map<long, OutSHPRec>::const_iterator end() const
	{
		return m_recordInfo.end();
	}

#ifdef _DEBUG_DIV
	long size() const
	{
		return m_recordInfo.size();
	}
#endif // _DEBUG_DIV

private:
	/*
	 *	@brief	交差リンク取得
	 *	@param	secondMesh [in] メッシュ
	 *	@param	heightNodeID [in] 相対高さノードID
	 *	@param	linkID [in] 検索対象リンクID(相対高さノードが保持しているID)
	 *	@param	tableCode [in] linkIDのテーブル種別
	 *	@param	ipHeightNodeGeo [in] 相対高さ形状
	 *	@param	ipSpRef [in] 相対高さの空間参照
	 *	@param	roadWalkLinkInfo [in] 道路・歩行者リンク情報
	 *	@param	roadWalkID [out] 交差リンクのOBJECTID
	 *	@param	roadWalkLinkRec [out] 交差リンクの属性情報
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SelectTgtLink(const long secondMesh,
					   const long heightNodeID, 
					   const long linkID,
					   const long tableCode,
					   const IGeometryPtr& ipHeightNodeGeo, 
					   const ISpatialReferencePtr& ipSpRef,
					   const RoadWalkLinkInfo& roadWalkLinkInfo,
					   long& roadWalkID,
					   RoadWalkLinkRec& roadWalkLinkRec);
   

private:
	static double				m_buffer;		//!<	交差判定時のバッファ
	std::map<long, OutSHPRec>	m_recordInfo;	//!<	変換後相対高さレコード情報
};

