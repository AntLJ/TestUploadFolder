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

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <direct.h>

#pragma warning (disable : 4172)

using namespace std;

/////////////////////////////////////////////
// @ファイルの総行数を数える
//
// @param fp[in] 入力ファイルポインタ
// @param pszPath 入力ファイルディレクトリポインタ
// @return 総行数
/////////////////////////////////////////////
int CountLines(char *pszPath)
{
	//ログファイルの入力
	FILE *fp = fopen(pszPath, "r");

	char aLineData[200];	//一行データ
	int iNum = 0;			//総行数

	while(fp){
		//配列の初期化
		for(int i = 0; i < 200; i++)
			aLineData[i] = NULL;

		//ログファイルから一行データ読み込み
		char* apLineData = fgets(aLineData, 200,fp);

		//データが入っていない場合、エラー
		if(apLineData == NULL)
		{
			break;
		}

		//行数増やす
		iNum++;

	}
	
	fclose(fp);

	//総行数を返す
	return iNum;
}

/////////////////////////////////////////////////////////////
//@brief 出力ファイルのディレクトリ(入力ファイルと同じディレクトリ）
//　     パスを作成する関数
//
//@param pszPath [in] 入力ファイルポインタ
//@param apDirectory [in, out] 出力ファイルポインタ
/////////////////////////////////////////////////////////////

void MakeDirectory(const char* pszPath, char *apDirectory)
{
	int iDirectoryPoint = 0;	//最後の'\\'の位置
	char chPath[100];			//出力ディレクトリ

	//出力ファイル先のコピー
	strcpy(chPath, pszPath);	

	//配列初期化
	for(int i = 0; i<100; i++)
		chPath[i] = NULL;

	//最終'\\'の位置の記録
	for(int i = 0;i<100;i++)
	{
		if(chPath[i] == '\\')
			iDirectoryPoint = i - 1;
	}

	//パスからファイル名を削除
	for(int i = iDirectoryPoint ; i<100; i++)
		chPath[i] = NULL;

	//出力先ディレクトリを返す
	strcpy(apDirectory, chPath);
}
/////////////////////////////////////////////////////////
//@breif 型変換関数
//
//@param iStart_pos[in]			数字読み込み開始位置
//@param iUnderbar_pos[in]	'_'の場所
//@param apLineData[in]		一行データ
//@param oft			出力ファイルポインタ
//
/////////////////////////////////////////////////////////
void ConvertFormat(int iStart_pos, int iUnderbar_pos,const char *apLineData, FILE *ofp)
{
	//基本データ読み込み
	for(int i = iStart_pos; i<100; i++)
	{
	//メッシュコードの後ろに'-'を付ける
		//SCALEレベルが'M'の場合
		if(apLineData[iStart_pos + 4] == 'M')
		{
			if(i == iStart_pos + 4)
				fprintf(ofp,"-");
		}
		
		//SCALE B1の場合
		if(apLineData[iStart_pos + 6] == 'B')
		{
			if(i == iStart_pos + 6)
				fprintf(ofp,"-");
		}

		//ログデータの'_'位置の記録、そして'_'から':'へ
		if(apLineData[i] == '_')
		{
			iUnderbar_pos = i;
			fprintf(ofp,";");
			continue;
		}

		//X,Y部分のユニット表現に変換
		if(i == iUnderbar_pos + 2)
		{
			fprintf(ofp,",");
			fputc(apLineData[i],ofp);
			fprintf(ofp,"]");
			continue;
		}

		//空白の後、文字がきていたら空白を消去
		if(apLineData[i] == ' ' && isdigit(apLineData[i + 1]) == 0)
			continue;

		//NULL文字がきたら1行データ読み込み終了
		if(apLineData[i] == NULL)
			break;

		//上記以外の場合、その文字を出力
		fputc(apLineData[i],ofp);
	}
}

////////////////////////////////////////////////////////
//@breif Rファイル作成関数
//
//@param apDirectory [in] 出力ファイルディレクトリポインタ
//@param apLineData	[in]	 一行データポインタ
//@param oft		 出力ファイルポインタ
//@param apNewDirectory [in,out] 変換後の出力ファイルディレクトリ
///////////////////////////////////////////////////////
void ChangeFiles(char* apDirectory, char* apLineData, FILE *ofp, char* apNewDirectory)
{
	char cPath[100];	//新しい出力ファイルパス

	//配列初期化
	for(int i = 0;i < 100;i++)
	{
		cPath[i] = NULL;
	}

	//SCALE M3もしくはSCALE B1の場合
	if((apLineData[6] == 'M' && apLineData[7] == '3')||apLineData[6] == 'B')
	{
		//これまでの出力ファイルを閉じる
		fclose(ofp);

		//出力先ディレクトリのコピー
		strcpy(cPath, apDirectory);

		//SCALE M3の場合、R2ファイルの作成
		if(apLineData[6] == 'M' && apLineData[7] == '3')
			strcat(cPath, "\\R2.rgn");

		//SCALE B1の場合、R1ファイルの作成
		if(apLineData[6] == 'B')
			strcat(cPath, "\\R1.rgn");
	}
	//新しい出力ファイル先をコピー
	strcpy(apNewDirectory, cPath);
}

///////////////////////////////////////////////
//@breif 型チェック関数
//
//@param iStart_pos	数字読み込み開始位置
//@param apLineData	一行データ
///////////////////////////////////////////////
void FileCheck(int iStart_pos, char* apLineData)
{
	//SCALEがBクラスの場合、メッシュコードを6桁取り込む
	if(apLineData[iStart_pos + 6] == 'B')
		iStart_pos = iStart_pos + 2;

	//基本ログデータをチェックする
	for(int i = iStart_pos; i < iStart_pos + 9; i++)
	{
		//文字が入る場所を除いて
		if(i != iStart_pos + 4&& i != iStart_pos + 6)
		{
			//数字以外が入っていたらプログラムエラー
			if(isdigit(apLineData[i]) == 0)
			{
				cerr << "file error!" << endl;
				exit(0);
			}
		}
	}

	//SCALEレベルに'M'以外が入っていたた、もしくは'_'の場所にそれ以外が入っていたら
	if(apLineData[iStart_pos+4] != 'M'||apLineData[iStart_pos+6] != '_')
	{
		//SCALEレベルに'B'以外が入っていたらエラー
		if(apLineData[iStart_pos+4] != 'B')
		{
			cerr << "file error!" << endl;
			exit(0);
		}
	}
}


///////////////////////////////////////////
//@breif 作業関数
//
//@param iNum[in]		総行数
//@param apDirectory[in]	出力ファイルディレクトリ
//@param fp[in]			入力ファイルポインタ
//@param ofp[in]		出力ファイルポインタ
//@param apNewDirectory[in,out]		新しい出力ファイルディレクトリ
//////////////////////////////////////////
void MainWorks(int iNum, char* apDirectory, FILE *fp, FILE *ofp, char* apNewDirectory)
{
	char aUpper[10];					//UPPERを入れる配列
	char aLineData[400];				//ログファイルの一行分データ

	//一行ずつデータをとりだし、型変換
	for(int i = 0; i < iNum-1; i++)
	{
		int iUnderbar_pos = -5;		//一行データに入っている'_'の位置

		//初期化
		for(int j = 0; j < 200; j++)
			aLineData[j] = 0;

		//一行データが取れない場合はエラーを返す
		if( fgets( aLineData, 400, fp ) == NULL)
		{
			cout << "fgets error" << endl;
			break;
		}

		//1，2行目はスキップ
		if(i < 2)
		{
			continue;
		}

		else
		{
			int iStart_pos = 0;		//メッシュコード読み込み開始位置

			//読み込み最終行なら、その行読み込んで関数終了
			if(i == iNum - 1)
			{
				fputs(aLineData,ofp);
				break;
			}

		//メッシュコードの位置を記憶し、コードの頭に'['を付ける
			//最初が数字の場合

			if(isdigit(aLineData[0]) != 0)
			{
				fprintf(ofp,"[");
				iStart_pos = 0;
			}

			//一行データの最初が'S'の場合
			if(aLineData[0] == 'S')
			{	
				//新しいリージョンファイルの作成
				ChangeFiles(apDirectory, aLineData, ofp, apNewDirectory);
				
				//SCALE M1の場合、処理をスキップ
				if(aLineData[6] == 'M' && aLineData[7] == '1')
					continue;
				
				//作成した新しいリージョンファイルに出力先を変更
				FILE *ofp2 = fopen(apNewDirectory,"w");
				ofp = ofp2;
				continue;
			}
				
			//タブの場合
			if(aLineData[0] == '\t')
			{
				fputc(aLineData[0],ofp);
				fprintf(ofp, "[");
				iStart_pos = 1;
			}

			//UPPERが入った場合

			if(aLineData[0] == 'U' && aLineData[5] == ' ')
			{
				//UPPERの文字を出力
				for(int j = 0;j < 6;j++)
				{
					fputc(aLineData[j],ofp);
					aUpper[j] = aLineData[j];
				}

				//頭文字UでUPPER以外の場合
				if(strncmp(aUpper,"UPPER", 5) != 0)
				{
					//エラーを出力し、プログラム終了
					cerr << "file miss!" << endl;
					exit(0);
				}

				fprintf(ofp,"[");
				iStart_pos = 6;
			}

			//型変換処理
			ConvertFormat(iStart_pos, iUnderbar_pos, aLineData, ofp);
			
			//型チェック
			FileCheck(iStart_pos, aLineData);

		}
	}
}


//////////////////////////////////////////
//@breif  メインの処理(コード変換）
//
//@param argc		引数
//@param argv		引数
/////////////////////////////////////////
int main(int argc, char* argv[])
{
	int iNum = 0;			//ログファイルの総行数
	char aDirectory[100];       //リージョンファイルの出力先
	char aNewDirectory[100];    //新しいリージョンファイルの出力先
	char aFile[100];			//出力ファイルのパス

	//配列の初期化
	for(int i = 0; i < 100; i++)
	{
		aFile[i] = NULL;
		aDirectory[i] = NULL;
		aNewDirectory[i] = NULL;
	}

	//ログファイルの総行数の取得
	iNum = CountLines(argv[1]);

	//ログファイルを開く
	FILE *fp = fopen(argv[1], "r");

	//ログファイルの中身が見つからない場合
	if(fp == NULL)
	{
		return 0;
	}

	//リージョンファイルの出力ディレクトリ作成
	MakeDirectory(argv[1], aDirectory);

	//最初のリージョンファイル作成
	strcpy(aFile, aDirectory);
	strcat(aFile, "\\R3.rgn");

	//作成したリージョンファイルを出力先に
	FILE *ofp = fopen(aFile, "w");

	//ログファイルからリージョンファイルへ変換
	MainWorks(iNum, aDirectory, fp, ofp, aNewDirectory);

	fclose(fp);
	fclose(ofp);
	return 0;
}
