prev=$(mktemp)
curr=$(mktemp)
out_log=$(mktemp)
in_log=$(mktemp)


cleanup(){
  rm -f "$prev" "$curr" "$out_log" "$in_log"
}
trap cleanup EXIT

who | awk '{print $1}' | sort -u > "$prev"
while true ; do
  sleep 10
  who | awk '{print $1}' | sort -u > "$curr"
  if ! cmp -s "$prev" "$curr"
  then
  comm -23 "$prev" "$curr" > "$out_log"
  comm -13 "$prev" "$curr" > "$in_log"
  echo "### logout ### "
  awk 'NR==FNR { users[$1]; next }  $1 in users' "$1" "$out_log"
  echo ""    
  echo "### login ### "
    awk 'NR==FNR { users[$1]; next } $1 in users' "$1" "$in_log"
  echo ""
  : > "$out_log" 
  : > "$in_log"
  fi
  cp "$curr" "$prev"
done
