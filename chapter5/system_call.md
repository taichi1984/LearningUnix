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
  
  
## mknod システムコール  
指定されたpathnameに新しいInodeを作成する。  
modeに応じて、そのi-nodeを通常ファイル、FIFO、キャラクタデバイス、ブロックデバイスにする。  
デバイスファイルの場合は、st_rdevフィールドにdevの値が格納される。  
  
### 書式  
```  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>  
  
int mknod(const char *pathname, mode_t mode, dev_t dev);  
  
```  
  
### 引数  
- pathname  
作成するファイルのパス名。通常は /dev/...以下になる。  
  
- mode  
ファイルタイプ+パーミッションビットを含む。  
ファイルタイプは以下のいずれかになる。  
```  
S_IFREG : 通常ファイル  
S_IFIFO : FIFO（名前付きパイプ）  
S_IFCHR : キャラクタデバイスファイル  
S_IFBLK : ブロックデバイスファイル  
```  
  
- dev   
デバイスID　(major/minorをエンコードした値)  
キャラクタ/ブロックデバイスのときだけ有効。  
makedev(major,minor)マクロを使って作るのが定石。  
  
### 返り値  
成功で0、失敗して-1  
  
  
  
