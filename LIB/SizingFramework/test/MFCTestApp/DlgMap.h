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

// IDC_STATIC�͂��̂܂܎g�p���Ă͂����Ȃ��iIDC_STATIC = -1�A-1�͓��ʁH�H�j
BEGIN_WINDOW_MAP(DEFAULT)
	// �s�O���[�v�̊J�n OK/CANCEL�{�^���s�E����ȊO�̍s
	// �S�ẴR���g���[���̓_�C�A���O�G�b�W����4�s�N�Z��������
	BEGINROWS(WRCT_REST, 0, RCMARGINS(4,4))
		// ��O���[�v�̊J�n�i�c���[�r���[�Ƃ���ȊO�j
		BEGINCOLS(WRCT_REST, 0, RCMARGINS(0,0))
			RCTOFIT(IDC_TREE1)
			RCFIXED(IDC_SIZERBAR, 4)
			BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0)) // �s�O���[�v�̊J�n�i�R���{�{�b�N�X�ƃO���[�v�{�b�N�X�j
				RCTOFIT(IDC_COMBO1)
				RCSPACE(4) // �R���{�{�b�N�X���O���[�v�{�b�N�X�̊Ԃ�4�s�N�Z������
				BEGINROWS(WRCT_REST, IDC_STATIC2, RCMARGINS(0,0)) // �O���[�v�{�b�N�X
					RCSPACE(12) // �O���[�v�{�b�N�X�̃��x�����l�����Ă�����Ɨ���
					// ���̂܂܂��ƃO���[�v�{�b�N�X�̃G�b�W�ɒ���t���Ă��܂��̂ŁA
					// �O���[�v�{�b�N�X���̃R���g���[���͑S��8�s�N�Z�����G�b�W���痣��
					BEGINROWS(WRCT_REST, 0, RCMARGINS(8,8))
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_STATIC3)
							RCSPACE(4)
							RCREST(IDC_EDIT1)
						ENDGROUP()
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,4))
							RCREST(IDC_COMBO2)
							RCSPACE(4)
							RCTOFIT(IDC_BUTTON1)
						ENDGROUP()
						RCREST(IDC_LIST2)
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(4)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0)) // ��O���[�v�̊J�n�iOK��CACEL�j
			RCREST(-1) // �������󂯂�
			RCTOFIT(IDOK)
			RCSPACE(4) // 4�s�N�Z����������
			RCTOFIT(IDCANCEL)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
