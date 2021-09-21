#!/bin/bash

if [[ "$#" == 0 ]]
then
    echo "Napačno št. argumentov!"
exit -1
elif [[ "$#" == 2 ]]
then
     if [[ "$1" == -d ]]
   	then
	pot=$2
	ime=$(echo $pot| cut -d'_' -f 2)
     else
	echo "Manjkajoča zastavica"
	exit -1
     fi
else
    echo "Napačno št. argumentov!"
    exit -1
fi

complete -W 'HELP NEW ACTIVATE ADD BUILD RUN TEST' bash project.sh
complete -W 'help new activate add build run test' project.sh
complete -W 'HELP NEW ACTIVATE ADD BUILD RUN TEST' bash project.sh
complete -W 'help new activate add build run test' project.sh


rm my_bashrc
touch my_bashrc
echo "source ~/.bashrc
PS1='\e[0;32m\u-\W/project_$ime$ \e[m'
PROJECTNAME=project_$ime
PROJECTDIR=$pot" >> my_bashrc
bash --rcfile $(dirname $pot)/my_bashrc