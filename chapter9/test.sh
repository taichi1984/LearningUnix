
ls
who
if diff file1 file1.bak 
then
  echo no differences found , removing backup ;
  rm file1.bak 
else  
  echo backup differs, making it readonly
  chmod -w file1.bak
fi
date
