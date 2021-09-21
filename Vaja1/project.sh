#!/bin/bash

command=$1
shift

case $command in
help)
    bash project_help.sh $@
;;
activate)
    bash project_activate.sh $@
;;
new)
    bash project_new.sh $@
;;
add)
    bash project_add.sh $@
;;
build)
    bash project_build.sh $@
;;
test)
    bash project_test.sh $@
;;
run)
    bash project_run.sh $@
;;
*)
    echo Napačen ukaz: $command
    exit -1
esac