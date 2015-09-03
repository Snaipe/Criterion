set repo="https://github.com/Snaipe/dyncall.git"
set tag="master"

mkdir dependencies
git clone --branch %tag% --depth 1 %repo% dependencies/dyncall
cd dependencies/dyncall
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=%LOCAL_INSTALL% -G "NMake Makefiles" ..
nmake
