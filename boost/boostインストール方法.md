# Macにboostライブラリインストール

# 1. コマンドライン(brew)でインストール

```bash
brew install boost
```

# パスの設定

```bash
alias gpp='g++ -wall -03 -std=c++17 -I/usr/local/include'
```

# エイリアス更新
- なんかうまく動かない
    - bashrcファイルがそもそもなかった
    - touchで作成したけどこれでいいのか不明
    
    - 一応参照を追加するための情報を.bash_profileの最後に追加しておいた
    ```bash
    if [ -f ~/.bashrc ] ; then
        . ~/.bashrc
    fi
    ```
```bash
. ~/.bashrc
```

# 実行方法(gcc)
```
-I /usr/local/Cellar/boost/1.71.0 (他のライブラリがあるなら追加) [.cppファイル]
```