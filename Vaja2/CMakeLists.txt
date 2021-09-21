cmake_minimum_required (VERSION 2.6)
file(GLOB sources
    "*.h"
    "*.c"
)
# dodajmo program, ki ga bomo prevedli in navedimo izvorne zbirke
add_executable (mergeSort ${sources})
target_link_libraries (mergeSort pthread)
target_link_libraries (mergeSort rt)