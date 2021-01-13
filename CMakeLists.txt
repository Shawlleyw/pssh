cmake_minimum_required(VERSION 3.16)
project(sh)


FIND_LIBRARY(readline REQUIRED)
AUX_SOURCE_DIRECTORY(src SRC)
AUX_SOURCE_DIRECTORY(builtincmd BIC)
AUX_SOURCE_DIRECTORY(filecmd FTC)

add_compile_options(-fsanitize=leak)
add_compile_options(-l ncurses)
#add_compile_options(-std=c++11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/)
ADD_EXECUTABLE(sh ${SRC} main.c ${BIC})
ADD_EXECUTABLE(cat filecmd/cat.c)
ADD_EXECUTABLE(ls filecmd/ls.c)
ADD_EXECUTABLE(rm filecmd/rm.c)
ADD_EXECUTABLE(touch filecmd/touch.c)
ADD_EXECUTABLE(mkdir filecmd/mkdir.c)
ADD_EXECUTABLE(cp filecmd/cp.c)
ADD_EXECUTABLE(mv filecmd/mv.c)
ADD_EXECUTABLE(ln filecmd/ln.c)
ADD_EXECUTABLE(xargs filecmd/xargs.c)
#ADD_EXECUTABLE(more filecmd/more.c)


TARGET_LINK_LIBRARIES(sh readline)

