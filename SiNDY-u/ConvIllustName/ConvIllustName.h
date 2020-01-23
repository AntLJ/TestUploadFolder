#pragma once
#include <CStringTokenizer.h>

struct Param;
struct IllustSettingsFields;
struct IllustNameFields;

/**
 * @brief 値が整数値かチェック
 * @param CSTypeFigure [in] CString型の引数
 * @retval true  引数が整数値
 * @retval false 引数が整数値でない
 */
bool canCheckFigure(CString CSTypeFigure);

/**
 * @brief イラスト種別コード値が正常かチェック
 * @param illustKind [in] イラスト種別コード値
 * @param targetFilePass [in] チェック対象のファイル名
 * @retval true  成功
 * @retval false 失敗
 */
bool canCheckIllustkind(int illustKind,CString &targetFilePass);

/**
 * @brief 仕向け先コード値が正常かチェック
 * @param illustKind [in] 仕向け先コード値
 * @param targetFilePass [in] チェック対象のファイル名
 * @retval true  成功
 * @retval false 失敗
 */
bool canCheckDestination(int destination,CString &targetFilePass);