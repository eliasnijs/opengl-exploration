#!/bin/sh


# 1. File information
input="src/minecraftclone.c"
output="build/build"

# 2. Flags

# 2.1 Debug Flags
debug_dflags="-D_POSIX_C_SOURCE=199309L -DENABLE_ASSERT=1 -DENABLE_DEBUGLOG"
debug_wflags="-Wno-unused-variable -Wno-unused-function"
debug_flags="-std=c99 -g -Wall ${debug_wflags} ${debug_dflags}"

# 2.2 Release Flags
release_dflags="-D_POSIX_C_SOURCE=199309L"
release_wflags="-Wno-unused-variable -Wno-unused-function"
release_flags="-std=c99 -O3 -Wall ${release_wflags} ${release_dflags}"

# 3. Linker
libglad="-L libs/glad/bin -lglad"
libglfw="-L libs/glfw/bin/src -lglfw3"
libassmip="-L libs/assimp/bin -lassimp"
libs="$libglfw $libglad -ldl -pthread -lm"

# 4. Other files to compile

# 5. Compiler
compiler="cc"

# Command
if [ "$1" = "release" ]; then
  echo compiling release build...
  ${compiler} ${release_flags} -o ${output} ${input} ${libs}
  echo finished compiling...
else
  echo compiling debug build...
  ${compiler} ${debug_flags} -o ${output} ${input} ${libs}
  echo finished compiling
fi
