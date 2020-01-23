#pragma once

// VC9�r���h�Ƃ���ȊO��NULL��nullptr���g������
#if _MSC_VER == 1500
#define NULLPTR NULL
#else
#define NULLPTR nullptr
#endif // _MSC_VER == 1500

/**
 * @brief setIMEConversionStatus()�̃��[�h
 */
enum CONVERSIONMODE {
	NOCHANGE,      //!< �ύX�Ȃ�
	HIRAGANA,      //!< �S�p�Ђ炪�ȓ���
	KATAKANA_FULL, //!< �S�p�J�^�J�i����
	KATAKANA_HALF, //!< ���p�J�^�J�i����
	ALPHA_FULL,    //!< �S�p�p��������
	ALPHA_HALF,    //!< ���p�p��������
};


/**
 * @brief IME���̓��[�h��ݒ肷��N���X
 */
class CIMEStatus{
private:
	HIMC m_himc; //!< ���̓R���e�L�X�g�n���h��
	HWND m_hWnd; //!< �E�B���h�E�n���h��
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param hWnd	[in]	�E�B���h�E�n���h��
	 */
	CIMEStatus() : m_himc(NULLPTR), m_hWnd(NULLPTR){}
	CIMEStatus(HWND hWnd) : m_himc(NULLPTR), m_hWnd(hWnd) { m_himc =::ImmGetContext( hWnd ); }
	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~CIMEStatus(){ if(m_himc != NULLPTR){::ImmReleaseContext( m_hWnd, m_himc );} }

	/**
	 * @brief IME�̓��̓��[�h�𐧌䂷��
	 * @param mode	[in]	�؂�ւ���̓��̓��[�h
	 */
	void setIMEConversionStatus(CONVERSIONMODE mode);
};
