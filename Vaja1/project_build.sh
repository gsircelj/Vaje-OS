#!/bin/bash

#d = debug
#r = run
#privzeto je debug

source <(grep -E '^\w+=' my_bashrc)


if [[ "$#" == 1 ]]
then
   if [[ "$1" == -d ]]
   	then
	cd $PROJECTNAME
	make clean
	make debug
   	exit 0
   elif [[ "$1" == -r ]]
   	then
	cd $PROJECTNAME
	make clean
	make release
  	exit 0
   fi
else
    cd $PROJECTNAME
    make clean
    make debug
    exit 0
    exit -1
fi
