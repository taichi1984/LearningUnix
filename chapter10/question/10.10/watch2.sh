who | sort > prev
while true ; do
  sleep 10
  who | sort > curr
  if ! cmp -s prev curr
  then
  comm -23 prev curr > out_log
  comm -13 prev curr > in_log
  echo "### logout ### "
  awk 'NR==FNR { users[$1]; next }  $1 in users' "$1" out_log
  echo ""    
  echo "### login ### "
    awk 'NR==FNR { users[$1]; next } $1 in users' "$1" in_log
  echo ""
  rm out_log in_log
  fi
  mv curr prev


done
