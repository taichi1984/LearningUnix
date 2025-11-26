# ８章で出るシステムコールまとめ

## execvp
PATH検索を行ってファイルを実行する。

### include
#include<unistd.h>

### 書式
result = execvp(const char *file, const char *argv[]);

### 引数
file 実行するファイルの名前
argv 文字列配列

### 戻り値
-1 ・・・エラーのとき

### 備考
execvpは成功時は何も戻さない。エラーのときのみ-1を返す。
execvpは厳密に言えばシステムコールではない。execveがほぼ同様の機能を持つシステムコールであるが、
execveは第１引数のpathでPATH検索を行わないため、既存のPATHが通っているプログラムなどもフルパスで書かないといけない問題がある。
その問題を解決したのが、execvpというこのラッパー関数ということになる。


## fork
プロセスを作成する。

### include
#include<unistd.h>

### 書式
pid_t result = fork(void);

### 引数
なし

### 戻り値
-1 ・・・　エラーのとき
0  ・・・　作成された子プロセスのとき
pid ・・・　親プロセスに返される子プロセスのPID


## waitのまとめ
子プロセスの終了を待つ

### include
#include<sys/types.h>
#include<sys/wait.h>

### 書式
pid_t result = wait(int *statusptr);

### 引数
statusptr 子のステータス値が書き込まれるアドレス

### 戻り値
-1 --- エラーのとき
pid --- 終了した子プロセスのPID

### 参照
waitpid(2), wait3(2)

