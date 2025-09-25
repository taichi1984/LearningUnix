# 第5章で学ぶlibc
## tcgetattr関数
ttyドライバから属性を読み出す

### インクルード
```
#include <termio.h>
#include <unistd.h>
```

### 書式
int result = tcgetattr(int fd,struct termios *info);

### 引数
fd ... 端末に接続しているファイルディスクリプタ
info ... strcut termiosを指すポインタ

### 戻り値
-1 ... エラーの時
0  ... 成功したとき


## tcsetattr関数
ttyドライバに属性を設定する。

### インクルード
```
#include <termio.h>
#include <unistd.h>
```

### 書式
int result = tcsetattr(int fd , int when , struct termios *info);

### 引数
fd ... 端末に接続しているファイルディスクリプタ
when ... 設定を変更するタイミング
info ... termios構造体を指すポインタ


### 戻り値
-1 ... エラーの時
0  ... 成功したとき


