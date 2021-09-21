#!/bin/bash

command=$1
shift

case $command in
"")
    echo "Error vpišite ime"
    exit -1
;;
*)
  touch project_$command/src/$command.c
  echo "#include \""$command".h\"" > project_$command/src/$command.c
  touch project_$command/include/$command.h
  echo "#ifndef "$command"_H_  
#define "$command"_H_ 
 
// tukaj implementirate funkcionalnosti objekta
       
#endif // "$command"_H_" > project_$command/include/$command.h

exit 0
esac