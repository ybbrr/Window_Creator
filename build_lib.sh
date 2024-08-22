#!/bin/bash

rm -rf __build_dir__

# List of packages to check
packages="libwayland-dev libxkbcommon-dev xorg-dev"
# Loop through each package and check if it is installed
for pkg in $packages; do
    if dpkg -s "$pkg" 1> /dev/null; then
        echo "$pkg is installed"
    else
        echo "$pkg is not installed, $pkg is a dependency of GLFW, which you need to compile the OpenGL_3D_Graphics_Engine project"
    fi
done

# check if ninja is installed
if command -v ninja 1> /dev/null
then
    echo "ninja Cmake Generator is installed"
else
    echo "\"ninja-build\" is not installed. You can run \"sudo apt install ninja-build\" and use as a CMake Generator."
    echo "If you do not want to install \"ninja-build\" please select \"Make\" as CMake Generator."
fi

echo ""
# Prompt for build type
read -p "Choose build type (1 for Release, 2 for Debug): " build_choice
if [ "$build_choice" -eq 1 ]; then
    BUILD_TYPE="Release"
elif [ "$build_choice" -eq 2 ]; then
    BUILD_TYPE="Debug"
else
    echo "Invalid build type choice. Please enter 1 or 2."
    exit 1
fi

echo
echo "Selected build type: $BUILD_TYPE"
echo

# Prompt for CMake generator
read -p "Choose CMake Generator (1 for Ninja, 2 for Make): " generator_choice
if [ "$generator_choice" -eq 1 ]; then
    CMAKE_GENERATOR="Ninja"
elif [ "$generator_choice" -eq 2 ]; then
    CMAKE_GENERATOR="Unix Makefiles"
else
    echo "Invalid CMake Generator choice. Please enter 1 or 2."
    exit 1
fi

echo
echo "Selected CMake Generator: $CMAKE_GENERATOR"
echo

read -p "Choose library type (1 for Static, 2 for Shared): " library_choice
if [ "$library_choice" -eq 1 ]; then
    LINK_TYPE="Static"
    SHARED_ENABLE="OFF"
    STATIC_ENABLE="ON"
elif [ "$library_choice" -eq 2 ]; then
    LINK_TYPE="Shared"
    SHARED_ENABLE="ON"
    STATIC_ENABLE="OFF"
else
    echo "Invalid library type choice. Please enter 1 or 2."
    exit 1
fi

echo
echo "Selected Library Type: $LINK_TYPE"
echo

NUM_THREADS=$(($(nproc) - 2))

# Run CMake
cmake -G "$CMAKE_GENERATOR" \
    -D CMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -D SHARED_EXPORT="$SHARED_ENABLE" \
    -D STATIC_EXPORT="$STATIC_ENABLE" \
    -D CMAKE_INSTALL_PREFIX="./__build_out__/$BUILD_TYPE/$LINK_TYPE" \
    -B "./__build_dir__/$BUILD_TYPE" .

cmake --build "__build_dir__/$BUILD_TYPE" --config "$BUILD_TYPE" -j"$NUM_THREADS"
cmake --build "__build_dir__/$BUILD_TYPE" -- install

# end of file