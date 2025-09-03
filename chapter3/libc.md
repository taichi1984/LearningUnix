# 第３章で学んだCの標準ライブラリの内容(libc)    

## opendir()      
ディレクトリの内容を開く関数      
    
### 形式      
    
    
```      
#include<dirent.h>      
    
DIR *opendir(const char *name);      
    
```      
    
### 引数      
name : 開きたいディレクトリのパス      
    
### 戻り値      
成功時:DIR * 型のポインタ(readdir()等で使う)      
失敗 :NULL(例：存在しないパス、ディレクトリでないパス、権限がないなど)      
    
### 特徴      
opendir()はopen()と違い、ディレクトリ専用のハンドル(ディレクトリ専用のファイルディスクリプタのような構造のハンドル)を返します。      
readdir()を使うことで、DIR* ハンドルから中身(ファイル名)を１件ずつ取得できます。      
読み取り中にディレクトリ構造が変わると、未定義動作になります。      
    
## readdir()      
opendir()で開いたディレクトリから１件ずつエントリを読み取る関数      
    
### 形式      
    
```      
#include<dirent.h>      
    
struct dirent *readdir(DIR *dirp);      
    
```      
    
### 引数      
dirp : opendir()によって得られたDIR * ポインタ      
    
### 戻り値      
成功 : 次のディレクトリエントリへのポインタ(struct dirent *)      
終端 :(全て読み終えたとき) NULL      
エラー時 : NULL(errnoをチェックすることで区別可能)      
    
### direntに関して      
戻り値であるディレクトリエントリの内容を理解する必要がある。      
ディレクトリエントリの内容は以下の通り。      
    
```      
struct dirent {      
    ino_t          d_ino;       // inode番号      
    off_t          d_off;       // 次のdirentへのオフセット（無視可）      
    unsigned short d_reclen;    // 構造体の長さ      
    unsigned char  d_type;      // ファイルの種類（DT_REGなど）      
    char           d_name[];    // NULL終端のファイル名（最大255文字）      
};      
    
```      
    
### direntのファイルの種類(d_type)の例      
    
| 値            | 意味                    |  
| ------------ | --------------------- |  
| `DT_REG`     | 通常のファイル               |  
| `DT_DIR`     | ディレクトリ                |  
| `DT_LNK`     | シンボリックリンク             |  
| `DT_FIFO`    | 名前付きパイプ               |  
| `DT_CHR`     | キャラクタデバイス             |  
| `DT_BLK`     | ブロックデバイス              |  
| `DT_SOCK`    | ソケット                  |  
| `DT_UNKNOWN` | 不明（必要なら `stat()` で確認） |  
    
ファイルの種類を判定することで、処理の内容を変える必要がある場面も多い。      
例えばディレクトリは青く表示するなど。      
重要な内容なので覚えておく必要がある。      
    
### 特徴      
dirent構造体を返す関数である。      
システムコールのread()のように扱える。      
構造体ごとにバッファリングされたdirent()を１件ずつ返す仕組みです。      
    
## closedir()      
ディレクトリストリームを閉じる。      
    
### 形式       
    
```      
#include<dirent.h>      
    
int closedir(DIR *dirp);      
    
```      
    
### 引数      
dirp : opendir()によって得られたDIR * 型ポインタ。      
    
### 戻り値      
成功 : 0      
失敗 : -1(errnoに詳細なエラー理由がセットされる)      
    
### 内部的に何をしているのか？      
    
- DIR * 構造体に含まれるファイルディスクリプタをclose(fd)      
- malloc()されたバッファをfree()      
- 状態構造体も解放      
    
ディレクトリ読み込みに使っていた全リソースをカーネル+ユーザー空間の両方で開放する。      

## telldir()
現在のディレクトリストリームの読み込み位置を記録する。

### 形式
```
#include<dirent.h>

long telldir(dirp)

```
### 引数
dirp : opendir()で開いたディレクトリストリームへのポインタ

### 戻り値
seekdir()に渡せる位置情報



## seekdir()
ディレクトリストリームの読み込み位置を移動する。

### 形式
```
#include<dirent.h>

void seekdir(DIR *dirp, long loc);
```

### 引数
dirp : opendir()で開いたディレクトリストリームへのポインタ
loc : telldir()で取得した位置情報



## char *ctime(const time_t * timep);    
time_tの型の値(UNIX時間、エポック秒)をローカル時間に変換し、人間が読める形式のヌル終端文字列として返す関数。    
    
### 形式    
char *ctime(const time_t *timep);    
    
### 引数    
timep : UNIX時間を格納しているtime_t型のポインタ    
実態は1970年1月1日 00:00:00(UTC)からの経過秒数を格納したポインタ    
    
### 戻り値    
引数で受け取った時間を人間が読める形にした文字列のポインタ    
**静的領域に格納された**ヌル終端文字列へのポインタ    
    
    
### 注意点    
戻り値は静的バッファへのポインタなので、毎回上書きされる。    
ctimeを呼び出すたびに静的バッファの内容は変わるので、たくさんのポインタ変数に順々に格納していった場合も    
全ての内容が一番最後にctimeを使ったときの文字列に変わってしまう。    
    
    
## struct passwd *getpwuid(uid_t uid);
ユーザーIDに対応するユーザーエントリを取得する。

### 形式
```
#include<pwd.h>
struct passwd *getpwuid(uid_t uid);
```

### 引数
uid  uid_t型のユーザーIDを格納した変数

### 戻り値
uidに対応する struct passwd構造体へのポインタを返す。
見つからない場合はNULLを返す。

### passwd構造体

```
struct passwd {
    char   *pw_name;    // ユーザー名
    char   *pw_passwd;  // パスワード（暗号化されているか、"x" のこともある）
    uid_t   pw_uid;     // ユーザーID
    gid_t   pw_gid;     // グループID
    char   *pw_gecos;   // ユーザーの本名など
    char   *pw_dir;     // ホームディレクトリ
    char   *pw_shell;   // ログインシェル
};
```

以上の構造体になる。

## struct group *getgrgid(gid_t gid);
グループIDに対応するグループエントリを取得する。

### 形式
```
#include <grp.h>

struct group *getgrgid(gid_t gid);
```

### 引数
gid : グループidを格納しているgid_t型の変数

### 戻り値
成功時 : gidに対応するstruct groupへのポインタを返す。
見つからない時はNULLを返す。

### struct group 構造体

```
struct group {
    char   *gr_name;   // グループ名
    char   *gr_passwd; // グループパスワード（通常 "x"）
    gid_t   gr_gid;    // グループID
    char  **gr_mem;    // グループのメンバー名（char *の配列）
};
```

## int atoi(const char *nptr);
文字列を数値に変換する。

### 形式
```
#include <stdlib.h>

int atoi(const char *nptr);
```

### 引数
*nptr : 数字列を含むヌル終端文字列へのポインタ。
先頭に空白文字、タブ、改行などがあれば無視される。
そのあとに + または- があれば富豪として解釈される。
続く文字が数字'0'～'9'であれば、順に数値に変換
数字以外が出たところで変換を打ち切る。

### 戻り値
変換したint値を返す。
変換できなかった場合は0を返す。

## int isdigit
与えられた文字が、数字であるかを判定する。
### 形式
```
#include <ctype.h>

int isdigit(int c);
```

### 引数
c : unsinged char に変換可能な値、または特殊値EOF
通常は(unsinged char)にキャストして渡す。
文字の内容が数字かどうかを確認する。


### 戻り値
0以外 : 真
0 : 偽

## futimens 関数
openなどでfdを取得しているファイルのatimeとmtimeを書き換える。

### 形式
```
#include<sys/stat.h>

int futimens(int fd , const struct timespec times[2]);

```

### 引数
fd : 時刻を変更したいファイルのファイルディスクリプタ。
読み書き用にオープンされている必要がある。

times : struct timespecの配列で要素数は2。
times[0]　... atime(最終アクセス時刻)
times[1] ... mtime(最終修正時刻)
timesがNULLの場合は両方を現在時刻に設定する。
各要素のtv_nsecに特殊値を設定できる。
 UTIME_NOW  : 現在時刻にする。
 UTIME_OMIT : そのフィールドは変更しない。

### 戻り値
成功時 ... 0
失敗時 ... -1を返し、errnoにエラー内容が設定される。

### 振る舞い
ファイルのatimeとmtimeを直接設定する関数。
実装としてはLinuxではutimensat(fd , NULL, times , 0)を呼んでいる。
ctime(状態変更時刻)はシステムが自動で更新するため、ユーザーは設定できない。


