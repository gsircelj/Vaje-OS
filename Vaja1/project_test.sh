#!/bin/bash

source <(grep -E '^\w+=' my_bashrc)

uspesni=0
neuspesni=0

if [[ "$#" == 0 ]]
then
    echo "Napačno št. argumentov!"
exit -1
elif [[ "$#" == 1 ]]
then
     if [[ "$1" == -l ]]
   	then
	cd $PROJECTNAME/tests/
	ls | cut -d "_" -f 2 | cut -d "." -f 1
	exit 0
     elif [[ "$1" == -v ]]
     then
	for f in $PROJECTNAME/tests/*
	do	
		echo "Testiram test: "
		echo $f | cut -d "_" -f 3 | cut -d "." -f 1
		ime=`echo $f | cut -d "_" -f 3 | cut -d "." -f 1`
		valgrind --quiet --trace-children=yes --error-exitcode=1 ./$PROJECTNAME/tests/test_$ime.sh
		if [ $? == 0 ]
           	then
			let "uspesni=uspesni+1"
            	else
                	let "neuspesni=neuspesni+1"
           	fi
	done
	echo "st. uspesnih testov: " $uspesni "st. neuspesnih testov: " $neuspesni
	exit 0
     fi
elif [[ "$#" == 2 ]]
then
     if [[ "$1" == -l ]]
     then
	echo "-l nima drugega argumenta"
	exit 0
     elif [[ "$1" == -v ]]
     then
	testname=$2
	valgrind --quiet --trace-children=yes --error-exitcode=1 ./$PROJECTNAME/tests/test_$testname.sh
	if [ $? == 0 ]
        then
		echo "Test je uspesen!"
        else
                echo "Test je neuspesen!"
        fi
	exit 0
     fi
fi