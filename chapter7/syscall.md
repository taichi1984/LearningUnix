# 第７章で出てくるシステムコール

## alarm()システムコール
SIGALRMが送られるタイミングを指定するアラームタイマを設定する。

### include
#include<unistd.h>

### 書式
unsigned old = alarm(unsigned seconds);
### 引数 
seconds --- 待ち時間(単位秒)

### 戻り値
-1 --- エラー時
old --- 設定した時間までの残り時間

## pauseシステムコール
シグナルを待つ

### include
#include<unistd.h>

### 書式
result = pause()

### 引数
なし

### 戻り値
-1  --- 常に


## getitimer/setitimer
インターバルタイマの値を取得、設定する。

### include
#include <sys/time.h>

### 書式
result = getitimer(int which , struct itimerval *val);
result = setitimer(int which , const struct itimerval *newval , struct itimerval * oldval);

### 引数
which  --- 取得、設定しようとしているタイマ
val    --- 現在の設定を指すポインタ
newval --- 組み込もうとしている設定を指すポインタ
oldval --- 置き換えられた設定を指すポインタ

### 戻り値
-1 エラーのとき
0  成功したとき

## sigactionシステムコール
シグナルの処理方法を指定する。

### インクルード
#include<signal.h>

### 書式
res = sigaction(int signum , const struct sigaction *action, strcut sigaction *prevaction);

### 引数
signum --- 処理するシグナル
action --- 処理を記述する構造体を指すポインタ
prevacation --- 元のアクションの記述を受け取る構造体ポインタ

### 戻り値
-1  --- エラー時
0   --- 成功したとき

## sigprocmaskシステムコール
現在のシグナルマスクを変更する

### include
#include<signal.h>

### 書式
int res = sigprocmask(int how, const sigset_t *sigs, sigset_t * prev);

### 引数
how --- シグナルマスクをどのように変更するか
sigs --- 使おうとしているシグナルのリストを指すポインタ
prev --- もとのシグナルマスクのリストを指すポインタ(またはNULL)

### 戻り値
-1 --- エラーのとき
 0 --- 成功したとき


