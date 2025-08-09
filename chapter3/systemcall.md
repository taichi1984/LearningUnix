# 第３章で学ぶシステムコール      
    
## stat()    
ファイルについての情報を取得する。    
    
### 形式    
```    
#include<sys/stat.h>    
    
int result = stat(char *fname, struct stat *bufp);    
    
```    
    
### 引数    
fname : ファイル名    
bufp  : ファイルの情報を格納するためのバッファを指すポインタ    
    
### 戻り値    
-1 : エラーの時    
0  : 成功の時    
    
## chmod()  
ファイルのパーミッション、特殊ビットを変更する。  
  
### 形式  
```  
#include<sys/types.h>  
#include<sys/stat.h>  
  
int result = chmod(char *path, mode_t mode);  
```  
  
### 引数  
path ファイルのパス  
mode 新しいモード値  
  
### 戻り値  
エラー時 : -1  
成功時   : 0  
  
### 使い方  
  
chmod("file",04764); とかで書き換えることが可能。  
  
※C言語は数値の一番大きい桁を0にすると、その値は８進数になる。  
  
chmod("file", S_ISUID | S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH) ;
  
など第二引数をorでビット演算した結果を渡してやると、見てる人もわかりやすいコードになる。  
  
  
## chown()  
ファイルのオーナー、グループIDを変更する。  
  
### 形式  
```  
#include<unistd.h>  
  
int chown(char *path, uid_t owner , gid_t group);  
```  
  
  
### 引数  
path ファイルのパス  
owner ファイルのユーザーID   
group ファイルのグループID   
  
### 戻り値  
-1 エラーの時  
0  成功したとき  
  
## utime()関数  
ファイルの変更、アクセス日時を変更する。  
  
### 形式  
  
```  
#include<sys/time.h>  
#include<utime.h>  
  
int utime(char *path, struct utimbuf *newtimes);  
```  
  
### 引数  
path ファイルのパス  
newtimes utimbuf構造体のポインタ。詳細はutime.hを参照。  
  
### 戻り値  
エラー時 : -1  
成功時   : 0  
  
  
## rename()  
ファイルの名前の変更と他のディレクトリへの移動  
  
### 形式  
```  
#include<stdio.h>  //rename() は POSIX ではなく C標準関数（実際はrename(2)システムコールを呼び出す）  
  
int result = rename(char *old, char *new);  
  
```  
  
### 引数  
old  ファイル、ディレクトリの元の名前  
new 新しいファイル、ディレクトリのパス名  
  
### 戻り値  
エラーしたとき : -1  
成功したとき  :  0  
  
  
  
