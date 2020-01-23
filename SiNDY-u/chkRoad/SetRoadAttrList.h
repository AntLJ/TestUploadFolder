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

typedef set<int> ISet;				///< 値の集合
typedef map<int, LPCTSTR>	MError;	///< キーはエラーコード, 値はエラーメッセージ


/////////////////////////////////////////////
//  INヘッダ関連
/////////////////////////////////////////////
/**
 * @brief ヘッダ情報属性
 */
struct HeadAttrList
{
	ISet		eMnt;			///< .mnt関連レイヤ
	ISet		eRdg;			///< .rdg関連レイヤ
	MError		eError;			///< エラーメッセージ
};

/////////////////////////////////////////////
//  リンクレイヤ関連
/////////////////////////////////////////////
/**
 * @brief リンク情報
 */
struct LinkInfo
{
	ISet	eManager;			///< 管理者コード
	ISet	eLinkAttr;			///< リンク種別
	ISet	eBypass;			///< バイパスフラグ
	ISet	eAuto;				///< 自動車専用フラグ
	ISet	eWidth;				///< 道路幅員
	ISet	eLane;				///< 車線数コード
	ISet	eRest;				///< 交通規制種別コード
	ISet	eCond;				///< 交通規制条件種別コード
};

/**
 * @brief リンク属性
 */
struct LinkAttrList
{
	LinkInfo	eLinkInfo;		///< リンク情報
	ISet		eAdminCode;		///< 行政区域コード
	ISet		eDispClass;		///< 表示用種別コード
	ISet		eNaviClass;		///< 経路用種別コード
	ISet		eAreaFlag;		///< 広域フラグ
	ISet		eParkClass;		///< 駐車場種別
	ISet		eParkLinkClass;	///< 駐車場リンク種別
	ISet		eUpDown;		///< 上下関係
	ISet		eAttrClass;		///< 属性種別コード
	ISet		eGradFlag;		///< 傾斜フラグ
	ISet		eGradValue;		///< 傾斜角度
	MError		eError;			///< エラーメッセージ
}; 

/////////////////////////////////////////////
//  ノードレイヤ関連
/////////////////////////////////////////////
/**
 * @brief ノード属性
 */
struct NodeAttrList
{
	ISet		eSignal;		///< 信号機フラグ
	ISet		eNodeClass;		///< ノード種別コード
	ISet		eHeight;		///< 道路高さ有効フラグ
	MError		eError;			///< エラーメッセージ
};

/////////////////////////////////////////////
//  新交通規制レイヤ関連
/////////////////////////////////////////////
/**
 * @brief 新交通規制属性
 */
struct RegAttrList
{
	ISet			eDays;			///< 曜日コード
	ISet			eCond;			///< 規制条件
	ISet			eLink;			///< 通行規制コード
	MError			eError;			///< エラーメッセージ
};

/////////////////////////////////////////////
//  ノード情報レイヤ関連
/////////////////////////////////////////////
/**
 * @brief ノード情報属性
 */
struct NInfoAttrList
{
	ISet			eClass;			///< ノード情報種別コード
	ISet			eGuideClass;	///< 地先種別
	ISet			eGuideCode;		///< 案内コード
	ISet			eUseOKCharDirCode;	///< 使用許可文字（方面案内用記号）
	ISet			eUseOKCharNodeCode;	///< 使用許可文字（記号）
	MError			eError;			///< エラーメッセージ
};

/////////////////////////////////////////////
//  レーン情報レイヤ関連
/////////////////////////////////////////////
/**
 * @brief レーン情報属性
 */
struct LaneAttrList
{
	MError			eError;			///< エラーメッセージ
};

/////////////////////////////////////////////
//  リンク情報レイヤ関連
/////////////////////////////////////////////
/**
 * @brief リンク情報属性
 */
struct LInfoAttrList
{
	ISet			eDisp;			///< 表示種別コード
	ISet			eSpeed;			///< 規制速度
	ISet			eSection;		///< 主従区分
	MError			eError;			///< エラーメッセージ
};

/////////////////////////////////////////////
//  リンク拡張レイヤ関連
/////////////////////////////////////////////
/**
 * @brief リンク拡張属性
 */
struct LinkExAttrList
{
	ISet			eKey;			///< リンク情報キー
	ISet			eSpeed;			///< 規制速度
	ISet			eSection;		///< 主従区分
	ISet			eIsland;		///< 浮島フラグ
	ISet			eDetail;		///< 細街路作業年度フラグ
	MError			eError;			///< エラーメッセージ
};

/**
 * @brief リンク情報属性
 */
struct AttrList
{
	HeadAttrList	eINHead;		///< ヘッダ情報
	LinkAttrList	eLinkLayer;		///< リンクレイヤ
	NodeAttrList	eNodeLayer;		///< ノードレイヤ
	RegAttrList		eRegLayer;		///< 新交通規制レイヤ
	NInfoAttrList	eNInfoLayer;	///< ノード情報レイヤ
	LaneAttrList	eLaneLayer;		///< レーン情報レイヤ
	LInfoAttrList	eLInfoLayer;	///< リンク情報レイヤ
	LinkExAttrList	eLinkExLayer;	///< リンク拡張レイヤ
};

/**
 * @brief 10進数→16進数表示に変換する。
 * @note  指定の10進数値を16進数表示に変換する。
 *
 * @param	rCstrID		[out]	16進数表示用文字列
 * @param	rId			[in]	10進数値
 *
 * @return	なし
 */
extern void SetObjectID(CString& rCstrID, int rId);


/**
 * @brief INデータ基準値を設定するクラス
 */
class CSetRoadAttrList
{
public:
	CSetRoadAttrList(void);
	~CSetRoadAttrList(void);

	/**
	 * @brief INデータの基準値を格納する。
	 * @note  判定基準として、INデータの基準値を格納する。
	 *
	 * @param	rPolDir		[in]	pol_code.txt格納ディレクトリパス
	 * @param	rAttr		[out]	基準値格納先
	 *
	 * @return	true	正常終了
	 * @return false	異常終了
	 */
	bool setList( CString& rPolDir, AttrList& rAttr );

private:

	/**
	 * @brief INデータヘッダ情報の基準値を格納する。
	 * @note  判定基準として、INデータヘッダ情報の基準値を格納する。
	 *
	 * @param	rINHead		[out]	INデータヘッダ情報基準値格納先
	 *
	 * @return	なし
	 */
	void setINHead( HeadAttrList& rINHead );

	/**
	 * @brief 道路リンクの基準値を格納する。
	 * @note  判定基準として、道路リンクの基準値を格納する。
	 *
	 * @param	rLinkAttr		[out]	道路リンク基準値格納先
	 * @param	rPolDir			[in]	pol_code.txt格納ディレクトリパス
	 *
	 * @return	true	正常終了
	 * @return	false	異常終了
	 */
	bool setLinkLayer( LinkAttrList& rLinkAttr, CString& rPolDir );

	/**
	 * @brief 道路ノードの基準値を格納する。
	 * @note  判定基準として、道路ノードの基準値を格納する。
	 *
	 * @param	rNodeAttr		[out]	道路ノード基準値格納先
	 *
	 * @return	なし
	 */
	void setNodeLayer( NodeAttrList& rNodeAttr );

	/**
	 * @brief 新交通規制の基準値を格納する。
	 * @note  判定基準として、新交通規制の基準値を格納する。
	 *
	 * @param	rRegAttr		[out]	新交通規制基準値格納先
	 *
	 * @return	なし
	 */
	void setRegLayer( RegAttrList& rRegAttr );

	/**
	 * @brief ノード情報の基準値を格納する。
	 * @note  判定基準として、ノード情報の基準値を格納する。
	 *
	 * @param	rNInfoAttr		[out]	ノード情報基準値格納先
	 *
	 * @return	なし
	 */
	void setNInfoLayer( NInfoAttrList& rNInfoAttr );

	/**
	 * @brief レーン情報の基準値を格納する。
	 * @note  判定基準として、レーン情報の基準値を格納する。
	 *
	 * @param	rLaneAttr		[out]	レーン情報基準値格納先
	 *
	 * @return	なし
	 */
	void setLaneLayer( LaneAttrList& rLaneAttr );

	/**
	 * @brief リンク情報の基準値を格納する。
	 * @note  判定基準として、リンク情報の基準値を格納する。
	 *
	 * @param	rLInfoAttr		[out]	リンク情報基準値格納先
	 * @param	rLinkAttr		[in]	リンク基準値格納先
	 *
	 * @return	なし
	 */
	void setLInfoLayer( LInfoAttrList& rLInfoAttr, LinkAttrList& rLinkAttr );

	/**
	 * @brief リンク拡張の基準値を格納する。
	 * @note  判定基準として、リンク拡張の基準値を格納する。
	 *
	 * @param	rLinkExAttr		[out]	リンク拡張基準値格納先
	 * @param	rLInfoAttr		[in]	リンク情報基準値格納先
	 *
	 * @return	なし
	 */
	void setLinkExLayer( LinkExAttrList& rLinkExAttr, LInfoAttrList& rLInfoAttr );
};
