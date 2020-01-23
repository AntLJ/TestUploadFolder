#pragma once

// VC9ビルドとそれ以外でNULLとnullptrを使い分け
#if _MSC_VER == 1500
#define NULLPTR NULL
#else
#define NULLPTR nullptr
#endif // _MSC_VER == 1500

/**
 * @brief setIMEConversionStatus()のモード
 */
enum CONVERSIONMODE {
	NOCHANGE,      //!< 変更なし
	HIRAGANA,      //!< 全角ひらがな入力
	KATAKANA_FULL, //!< 全角カタカナ入力
	KATAKANA_HALF, //!< 半角カタカナ入力
	ALPHA_FULL,    //!< 全角英数字入力
	ALPHA_HALF,    //!< 半角英数字入力
};


/**
 * @brief IME入力モードを設定するクラス
 */
class CIMEStatus{
private:
	HIMC m_himc; //!< 入力コンテキストハンドル
	HWND m_hWnd; //!< ウィンドウハンドル
public:
	/**
	 * @brief コンストラクタ
	 * @param hWnd	[in]	ウィンドウハンドル
	 */
	CIMEStatus() : m_himc(NULLPTR), m_hWnd(NULLPTR){}
	CIMEStatus(HWND hWnd) : m_himc(NULLPTR), m_hWnd(hWnd) { m_himc =::ImmGetContext( hWnd ); }
	/**
	 * @brief デストラクタ
	 */
	virtual ~CIMEStatus(){ if(m_himc != NULLPTR){::ImmReleaseContext( m_hWnd, m_himc );} }

	/**
	 * @brief IMEの入力モードを制御する
	 * @param mode	[in]	切り替え先の入力モード
	 */
	void setIMEConversionStatus(CONVERSIONMODE mode);
};
