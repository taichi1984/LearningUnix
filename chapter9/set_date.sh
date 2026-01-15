#! /bin/sh
# 環境変数がコマンドに渡される様子を示す。
# TZは標準時間帯で、date , ls-lなどに影響を受ける
#

echo "The time in Boston is"
  TZ=EST5EDT
  export TZ
  date

echo "The time in Chicago is"
  TZ=CST6CDT
  date

echo "The time in LA is"
  TZ=PST8PDT
  date



