========================================================================
       スタティック ライブラリ : mrxlib
========================================================================


AppWizard はこの mrxlib ライブラリを作成しました。  

このファイルには mrxlib アプリケーションを構成している各ファイルの概要
説明が含まれています。

/////////////////////////////////////////////////////////////////////////////

StdAfx.h, StdAfx.cpp
    これらのファイルはプリコンパイル済みヘッダーファイル (PCH) mrxlib.pch
    やプリコンパイル済み型ファイル StdAfx.obj を構築するために使用されます。

/////////////////////////////////////////////////////////////////////////////
コンパイラとリンカのスイッチは MFC をサポートするように変更されていますが、
このプロジェクトに対し MFC ClassWizard を使用するためには、プロジェクトに
次に示すファイルを追加する必要があります。( "resource.h" と、これをインクルード
する "mrxlib.rc" , "mrxlib.h" )  RC ファイルをスタティック ライブラリに
追加する場合には注意が必要です。DLL または EXE には 複数の RC ファイルを含む
ことができないためです。この問題は、ライブラリの .RC ファイルを親プロジェクトの
.RC ファイルでインクルードすることで回避できます。
/////////////////////////////////////////////////////////////////////////////
その他の注意:

AppWizard は "TODO:" で始まるコメントで、ソースコードの追加やカスタマイズの必要
な場所を示します。


/////////////////////////////////////////////////////////////////////////////
