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
#include "resource.h"

enum EKiseiNum{
	KISEIJIKAN1,
	KISEIJIKAN2,
	KISEIJIKAN3,
	KISEIJIKAN4
};

typedef struct RegulationTime{
	CString strSMon;
	CString strEMon;
	CString strSDay;
	CString strEDay;
	CString strSHour;
	CString strEHour;
	CString strSMin;
	CString strEMin;
	int nYobiIndex;
	bool bIsEveryMonth;
}REGTIME;


/**
 * @class CRegulationTime
 * @brief 規制時間１～規制時間４の各コントロールを操作、制御、チェックするためのクラス
 *
 * コントロールIDを渡してインスタンスを作成すると、関連する（同じ規制時間の）各コントロールを
 * 意識せずに扱うことができます。
 */
class CRegulationTime
{
public:
	CRegulationTime(void);
	~CRegulationTime(void);
	CRegulationTime( HWND hDlg, EKiseiNum Enum );

	/**
	 * @brief コントロールIDから規制時間の番号を返す静的関数
	 * 
	 * @param	nID	[in]	コントロールID
	 *
	 * @retval	規制時間番号（KISEIJIKAN1～KISEIJIKAN4）
	 */
	static EKiseiNum CtrlID2KiseiNum( const int nID ); 

	/**
	 * @brief エディットボックスに入力されている文字列を返す
	 * 
	 * @param	cEdit	[in]	エディットコントロール
	 *
	 * @retval	入力されている文字列
	 */
	CString GetEditStr( CEdit cEdit );

	/**
	 * @brief 開始、終了分が"0"だったら"00"を表示する
	 * 
	 * @retval	なし
	 */
	void DisplayZeroZero();

	/**
	 * @brief 「毎月」ボタンをnBtnStateの状態にする
	 * @brief  nBtnState = 0: トグルをOffにして「月」をDisableにする
	 * @brief  nBtnState = 1: トグルをOnにして「月」をEnableにする
	 * 
	 * @param	nBtnState	[in]	トグルボタンの状態
	 * @retval	なし
	 */
	void ToggleZeroGatu( const int nBtnState );

	/**
	 * @brief 「毎月」ボタンを押された状態にする
	 * @note 「毎月」ボタンをトグルOnにして「月」エディットをEnableにします
	 * 
	 * @param	nBtnState	[in]	日にちを同時にセットして0月1日～0月31日にするかどうか
	 * @retval	なし
	 */
	void SetZeroGatu( bool bIsSetDay = true );

	/**
	 * @brief 「毎月」ボタンを押されていない状態にする
	 * @note 「毎月」ボタンをトグルOFFにして「月」エディットをDisableにします
	 * 
	 * @retval	なし
	 */
	void UnsetZeroGatu();

	/**
	 * @brief  規制期間のエディットボックスに値を入力する
	 * 
	 * @param	lSMon	[in]	開始月
	 * @param	lSDay	[in]	開始日
	 * @param	lEMon	[in]	終了月
	 * @param	lEDay	[in]	終了日
	 * @retval	なし
	 */
	void SetRegDate( const long lSMon, const long lSDay, const long lEMon, const long lEDay ); 

	/**
	 * @brief  規制時間のエディットボックスに値を入力する
	 * 
	 * @param	lSHour	[in]	開始時
	 * @param	lSMin	[in]	開始分
	 * @param	lEHour	[in]	終了時
	 * @param	lEMin	[in]	終了分
	 * @retval	なし
	 */
	void SetRegTime( const long lSHour, const long lSMin, const long lEHour, const long lEMin );

	/**
	 * @brief  規制時間をクリアする
	 * @note  各エディットボックスは_T("")でクリアされ、コンボボックスは<null>となります。
	 * 
	 * @retval	なし
	 */
	void Clear();

	/**
	 * @brief  「0月」の入力を監視する
	 * 
	 * @param	hWnd	[in]	子ダイアログのハンドル
	 * @param	strErr	[out]	エラーメッセージ。結果がfalseの時は空。
	 * @param	bFirst	[in]	true: 初回登録のため、0を入力させる。　	false: 直接入力時で0を入力させない。
	 * @retval	true: 開始または終了月EditBoxに「0」が入力された
	 * @retval  false: 入力は「0」以外 
	 */
	bool CheckZeroGatu( HWND hWnd, CString &strErr, bool bFinishInit );

	/**
	 * @brief  「0月」が入力されているかどうか
	 * @note   IsEveryMonth()とは意味が異なります。
	 * 
	 * @retval	true: 開始月、終了月が「0月」
	 */
	bool IsZeroGatu();

	/**
	 * @brief  規制期間が1月～12月か
	 * @note  IsZeroGatu()とは意味が異なります。
	 * 
	 * @retval	true: 開始月 = 1 && 終了月 = 12
	 */
	bool IsEveryMonth();

	/**
	 * @brief  対象の規制の日にち期間を包含しているか
	 * 
	 * @param	tReg	[in]	比較対象となる規制
	 * @retval	true例: this: 1日～31日 ⇔ tReg: 1日～7日
	 */
	bool IsContainDays(  REGTIME &tReg  );

	/**
	 * @brief  対象の規制の日にち期間を包含しているか
	 * 
	 * @param	tReg	[in]	比較対象となる規制
	 * @retval	true例: this: 1日～31日 ⇔ tReg: 1日～7日
	 */
	bool IsContainTime(  REGTIME &tReg  );

	/**
	 * @brief  規制時間構造体を返す
	 * 
	 * @retval	入力されている規制時間構造体
	 */
	REGTIME GetRegTime();

	/**
	 * @brief  規制時間をセットする
	 * 
	 * @retval	入力する規制時間構造体
	 */
	void SetRegTime( REGTIME & tRegTime );

	
	//bool IsEmpty();
	//bool IsTunen();
	//bool IsSyujitu();
	//bool IsEverydayEveryTime();

private:
	CEdit m_cEditStartMonth, m_cEditStartDay, m_cEditEndMonth, m_cEditEndDay,
		  m_cEditStartHour, m_cEditStartMin, m_cEditEndHour, m_cEditEndMin;

	CButton m_cBtnZeroGatu;
	CComboBox m_cCmbYobi;
};

