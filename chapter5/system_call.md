# 第５章で学ぶシステムコール

## fcntlシステムコール
ファイルディスクリプタの操作を行う。

### インクルード
```
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
```

### 書式
```
int result = fcntl(int fd,int cmd);
int result = fcntl(int fd,int cmd,long arg);
int result = fcntl(int fd,int cmd,struct flock *lockp);

```
引数が可変なのは、可変引数関数だからである。
この場合は第１引数、第２引数は決まった引数だが、第３引数以降は呼び出しごとに変えられる。
この方法は第３引数以降の型安全性はなく、呼び出し側と実装側の取り決めに依存する。

### 引数
fd  ... 操作するファイルディスクリプタ
cmd ... 実行する処理
arg ... 処理に対する引数
lock ... lock情報

### 戻り値
-1 ... エラーの時
0  ... 成功したとき


## ioctl システムコール
デバイスを制御する

### インクルード
#include<sys/ioctl.h>

### 書式
int result = ioctl(int fd, int operation[,arg..]);

### 引数
fd        ... デバイスに接続しているファイルディスクリプタ
operation ... 実行すべき処理
arg       ... 処理で必要とされる任意個の引数

### 戻り値
-1 ... エラー時
0  ... 成功したとき



