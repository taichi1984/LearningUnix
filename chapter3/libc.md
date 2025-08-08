# 第３章で学んだCの標準ライブラリの内容(libc)    
    
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


