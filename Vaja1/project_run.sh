#!/bin/bash

source <(grep -E '^\w+=' my_bashrc)

chmod 755 $PROJECTNAME/build/$PROJECTNAME
./$PROJECTNAME/build/$PROJECTNAME