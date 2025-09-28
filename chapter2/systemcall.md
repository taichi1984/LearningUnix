# 習ったシステムコール    
    
## open    
    
目的 : ファイルへの接続を作る      
include : #include<fcntl.h>      
書式 : int fd = open(char *name , int how);      
引数 : name = ファイル名 , how = O_RDONLY , O_WRONLY, O_RDWR　のいずれか。      
戻り値 : -1 エラーの時, int 成功したとき      
    
サンプルコード    
    
```    
    
int fd;    
    
if(fd = open(sample.txt , O_RDONLY) == -1){    
    perror("エラー処理をここに書く");    
    exit(1);    
 }    
    
```    
上記のサンプルコードではif文の中でopen自体は行ってしまい、-1が返ってきたらエラーコードを返すという使い方をしている。既存のファイルを開く方法の基本の形になる。    
    
## read    
    
目的 : fd から buf にqtyバイト以下の情報を転送する。    
include : #include<unistd.h>    
書式 ssize_t numread = read(int fd , void *buf , size_t qty);    
引数 :fd データソース（ファイルディスクリプタ) ,    
      buf データのディスティネーション(転送先のメモリのポインタ),    
      qty 転送すべきバイト数    
戻り値 : -1 エラーの時    
       : numread 成功したとき    
    
サンプルコード     
    
```    
#define BUFSIZE 1024    
    
char buf[BUFSIZE];    
/* fdの宣言、openは省略*/     
    
while((n =  read(fd , buf , BUFSIZE - 1)) > 0){    
    buf[n] = '\0';    
    printf("%s",buf);    
}    
    
```    
上記のコードは、fdで指してるファイルからqtyバイト分 bufに読み込んで、戻り値が0より大きいなら      
ファイルのデータを少しは読みこんでいるので表示する    
読み込まなくなったら勝手にループを抜けていく。    
    
## close    
目的 : ファイルをクローズする    
インクルード : #include<unistd.h>    
書式 : int result = close(int fd);    
引数 : fd    
戻り値 : -1(エラー時)    
       : 0(成功したとき)    
    
サンプルコード    
```    
 if(close(fd) == -1){    
    perror("Can't close fd");    
    exit(1);    
}    
```    
    
ファイルクローズとエラー処理を同時にやったサンプル。    
    
## creat    
    
目的:ファイルを作成するか、空にする    
インクルード : #include<fcntl.h>    
書式 : int fd = creat(char *filename , mode_t mode);    
引数 : filename , ファイル名    
     : mode , アクセス許可    
戻り値 : -1 , エラーの時    
       : fd , 成功したとき    
    
サンプルコード    
```    
fd = creat("addressbook" , 0644);    
```    
    
シンプルに新規のファイルを指定されたファイル名で作成する機能。    
同名のファイルがあった場合はそのファイルを空にする。パーミッションは変更しない。    
    
## write    
    
目的: メモリからデータにファイルを書き込む。    
インクルード : #include<unistd.h>    
書式 : ssize_t result = write(int fd , void *buf , size_t amt);    
引数 : fd , ファイルディスクリプタ    
     : buf , 配列    
        : amt, 書き込むべきバイト数      
戻り値  : -1 (エラーの時)    
        : 書き込んだバイト数 (成功したとき)    
    
    
サンプルコード    
    
```    
/*インクルードとファイルディスクリプタのオープンは省略*/    
char *buf;    
    
buf = "aaaaa";    
size_t len = strlen(buf);    
ssize_t n = write(fd , buf ,len);    
if( n < 0 ){    
    perror("write");    
    exit(EXIT_FAILURE);    
    
    
```    
    
    
