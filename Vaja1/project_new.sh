#!/bin/bash

if [[ "$#" == 1 ]] 
then 
    #1 argument
    ime=$1
    echo "ustvarjam nov projekt" $ime
    mkdir project_$ime
    mkdir project_$ime/src
    touch project_$ime/main.c
    echo "#include <iostream>
using namespace std;
int main() 
{
    cout << \"Hello, World!\";
    return 0;
}" > project_$ime/main.c
    mkdir project_$ime/include
    mkdir project_$ime/build
    mkdir project_$ime/tests
    touch project_$ime/tests/test_okay.sh
    echo "#!/bin/bash
exit 1" > project_$ime/tests/test_okay.sh
    chmod 755 project_$ime/tests/test_okay.sh
    touch project_$ime/tests/test_failed.sh
    echo "#!/bin/bash
exit 0" > project_$ime/tests/test_failed.sh
    chmod 755 project_$ime/tests/test_failed.sh
    touch project_$ime/Makefile
    echo -e "# prevajalnik za C++
CXX= g++
# dodatne zastavice, ki se posredujejo prevajalniku
CXXFLAGS= -Wall -Iinclude
# ime cilja - programa v katerega prevedemo program
TARGET= project_$@

# avtomatsko iskanje izvornih datotek, v src in trenutnem direktoriju
SRCS := \$(wildcard src/*.c *.c)
OBJS := \$(SRCS:src/%.c=build/%.o)

# izpis informacij
\$(info Compiler: \$(CXX))
\$(info Flags: \$(CXXFLAGS))
\$(info Target: \$(TARGET))
\$(info Source files: \$(SRCS))
\$(info Object files: \$(OBJS))
\$(info )

#  cilj ki prevede celoten projekt
all: build/\$(TARGET)

# cilj, ki prevede program za razhroscevanje
debug: CXXFLAGS:= -g -O0 \$(CXXFLAGS)
debug: all

# cilj, ki prevede program za izdajo
release: CXXFLAGS:= -O3 \$(CXXFLAGS)
release: all

# cilj, ki pocisti prevedene zbirke
clean:
	rm -rf build/*

# cilj, ki prevede posamezno izvorno zbirko v objektni modul
build/%.o: src/%.c
	\$(CXX) \$(CXXFLAGS) -c -o \$@ \$^ 

# cilj, ki poveze objektne module v program
build/\$(TARGET): \$(OBJS)
	\$(CXX) \$(CXXFLAGS) -o \$@ \$^ 
" >> project_$ime/Makefile
    exit 0

elif [[ "$#" == 3 ]]
then 
    #3 argumenti
    if [[ "$2" == -d ]]
    then
        ime=$1
	novipath=$3
        cd $novipath
   	echo "ustvarjam nov projekt" $ime
   	mkdir project_$ime
   	mkdir project_$ime/src
   	touch project_$ime/main.c
   	echo "#include <iostream>
	using namespace std;
	int main() 
	{
 	   cout << \"Hello, World!\";
	    return 0;
	}" > project_$ime/main.c
 	mkdir project_$ime/include
 	mkdir project_$ime/build
  	mkdir project_$ime/tests
	touch project_$ime/tests/test_okay.sh
    echo "#!/bin/bash
exit 1" > project_$ime/tests/test_okay.sh
    	chmod 755 project_$ime/tests/test_okay.sh
   	touch project_$ime/tests/test_failed.sh
  	echo "#!/bin/bash
exit 0" > project_$ime/tests/test_failed.sh
    	chmod 755 project_$ime/tests/test_failed.sh
	touch project_$ime/Makefile
	echo "# prevajalnik za C++
CXX= g++
# dodatne zastavice, ki se posredujejo prevajalniku
CXXFLAGS= -Wall -Iinclude
# ime cilja - programa v katerega prevedemo program
TARGET= $@

# avtomatsko iskanje izvornih datotek, v src in trenutnem direktoriju
SRCS := \$(wildcard src/*.c *.c)
OBJS := \$(SRCS:src/%.c=build/%.o)

# izpis informacij
\$(info Compiler: \$(CXX))
\$(info Flags: \$(CXXFLAGS))
\$(info Target: \$(TARGET))
\$(info Source files: \$(SRCS))
\$(info Object files: \$(OBJS))
\$(info )

#  cilj ki prevede celoten projekt
all: build/\$(TARGET)

# cilj, ki prevede program za razhroscevanje
debug: CXXFLAGS:= -g -O0 \$(CXXFLAGS)
debug: all

# cilj, ki prevede program za izdajo
release: CXXFLAGS:= -O3 \$(CXXFLAGS)
release: all

# cilj, ki pocisti prevedene zbirke
clean:
	rm -rf build/*

# cilj, ki prevede posamezno izvorno zbirko v objektni modul
build/%.o: src/%.c
	\$(CXX) \$(CXXFLAGS) -c -o $@ \$^ 

# cilj, ki poveze objektne module v program
build/\$(TARGET): \$(OBJS)
	\$(CXX) \$(CXXFLAGS) -o $@ \$^ 
" >> project_$ime/Makefile
  	exit 0
    else
        echo "Manjkajoča zastavica"
        exit -1
    fi
else
    echo "Napačno št. argumentov!"
    exit -1
fi

