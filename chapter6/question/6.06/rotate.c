#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

void tty_mode(int how);
void set_cr_noecho_mode();
void signal_setting();
void keyboard_signal_handler(int);
void backspace();
int main(void) {

  tty_mode(0);
  set_cr_noecho_mode();
  signal_setting();

  int c;
  int linelen = 0;
  int wordlen = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\b' || c == 0x7f || c == 0x10) {
      backspace();
      if (linelen > 0) {
        linelen--;
      }
      if (wordlen > 0) {
        wordlen--;
      }
    } else if (c == 0x15) {
      // 行削除処理
      for (int i = 0; i < linelen; i++) {
        backspace();
      }
      linelen = 0;
    } else if (c == 0x17) {
      // 単語削除処理
      for (int i = 0; i < wordlen; i++) {
        backspace();
      }
      linelen -= wordlen;
      wordlen = 0;

    } else {
      //通常の文字入力
      putchar(c);
      linelen++;
      wordlen++;
      //改行処理
      if (c == ' ') //スペース入力時
        wordlen = 0;
      if(c == '\n' || c == '\r') { //改行時
        wordlen = 0;
        linelen = 0;
      }
    }
  }
  return 0;
}

void tty_mode(int how) {
  static struct termios original_mode;
  static int original_flags;
  if (how == 0) {
    tcgetattr(0, &original_mode);
    original_flags = fcntl(0, F_GETFL);
  } else {
    tcsetattr(0, TCSANOW, &original_mode);
    fcntl(0, F_SETFL, original_flags);
  }
}

void set_cr_noecho_mode() {
  struct termios ttystate;

  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON;
  ttystate.c_lflag &= ~ECHO;
  ttystate.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &ttystate);
}

void signal_setting() {
  signal(SIGINT, keyboard_signal_handler);
  signal(SIGQUIT, keyboard_signal_handler);
  signal(SIGTSTP, keyboard_signal_handler);
}

void keyboard_signal_handler(int signum) {
  {
    tty_mode(1);
    exit(2);
  }
}

void backspace() {
  putchar('\b');
  putchar(' ');
  putchar('\b');
}
