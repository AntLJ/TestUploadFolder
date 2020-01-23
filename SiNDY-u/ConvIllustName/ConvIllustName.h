#pragma once
#include <CStringTokenizer.h>

struct Param;
struct IllustSettingsFields;
struct IllustNameFields;

/**
 * @brief �l�������l���`�F�b�N
 * @param CSTypeFigure [in] CString�^�̈���
 * @retval true  �����������l
 * @retval false �����������l�łȂ�
 */
bool canCheckFigure(CString CSTypeFigure);

/**
 * @brief �C���X�g��ʃR�[�h�l�����킩�`�F�b�N
 * @param illustKind [in] �C���X�g��ʃR�[�h�l
 * @param targetFilePass [in] �`�F�b�N�Ώۂ̃t�@�C����
 * @retval true  ����
 * @retval false ���s
 */
bool canCheckIllustkind(int illustKind,CString &targetFilePass);

/**
 * @brief �d������R�[�h�l�����킩�`�F�b�N
 * @param illustKind [in] �d������R�[�h�l
 * @param targetFilePass [in] �`�F�b�N�Ώۂ̃t�@�C����
 * @retval true  ����
 * @retval false ���s
 */
bool canCheckDestination(int destination,CString &targetFilePass);