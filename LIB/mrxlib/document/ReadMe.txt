========================================================================
       昭文社MAPPLEルーティングデータハンドリングライブラリHTMLヘルプ作成プロジェクト
========================================================================

このファイルには、ライブラリのHTMLヘルプを作成するためのファイルの概要説明が含まれ
ています。

Doxyfile
	このファイルには doxygen によるドキュメント生成に関する情報が記述されており、
	カスタムビルドの対象となります。

/////////////////////////////////////////////////////////////////////////////
必要となる設定:

このプロジェクトをビルドするには、「ツール」メニューの「オプション」において
「ディレクトリ」タブの「実行可能ファイル」に以下のディレクトリを指定する必要
があります。

1. doxygen.exe が存在するディレクトリ
	C:\Program Files\doxygen\bin

2. dot.exe が存在するディレクトリ
	C:\Program Files\ATT\Graphviz\bin

3. hhc.exe が存在するディレクトリ
	C:\Program Files\HTML Help Workshop

/////////////////////////////////////////////////////////////////////////////
必要となるファイル:

このプロジェクトをビルドするには、以下のファイルが必要となります。

1. doxygen によるドキュメント生成に使われるフッタ定義ファイル
	../../../Documents/Doxygen/doxyfooter.html

2. doxygen によるドキュメント生成に使われるIPCロゴマークファイル
	../../../Documents/Doxygen/ipc_logo.png
