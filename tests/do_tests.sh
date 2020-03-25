#!/bin/bash

OUT=/tmp/out
REF=ref/

# list of tests to perform
TESTS="test_flat test_class"

if [ ! -d "$OUT" ];
then
    mkdir "$OUT"
fi

echo "Output directory is $OUT"

echo "Compiling rc4.c"
gcc -Wall -Wextra -o $OUT/rc4.o -c rc4.c

echo "Creating librc4.a archive"
ar -crs $OUT/librc4.a $OUT/rc4.o 

echo "Prepare rc4.h"
cp -vf rc4.h "$OUT/"

function one_test()
{
    t="$1"
    echo "************************ test $t ************************"
    echo "Rendering $OUT/$t.c"
    python3 ../render.py $t.yaml >& "$OUT/$t.c"

    diff -Naur "$REF/$t.c" "$OUT/$t.c"
    
    echo "Building $OUT/setup.py"
    cat > $OUT/setup.py <<EOF
import distutils.core

# .so compilation
my_module = distutils.core.Extension(
    "pyrc4",
    sources = ["$t.c"],
    include_dirs=["."],
    # version depuis la librairie statique
    libraries=["rc4"],
    library_dirs=["."],
)

distutils.core.setup(
    name = "PyRC4",
    ext_modules = [my_module],

    version = "1.0",
    description = "Python Package with C Extension",
    url="https://github.com/FredB0ss",
    author="Fred",
    author_email="someone at nowhere dot com")

EOF

    echo "Building Python C Module"
    pushd $OUT/
    python3 setup.py build
    echo "Moving pyrc4.so"
    cp -vf build/lib.*/pyrc4*.so pyrc4.so

    echo "Cleaning"
    rm -rf build
    
    echo "Checking Module"
    python3 -c "import pyrc4; print(pyrc4.__doc__, [ (a,getattr(pyrc4,a).__doc__) for a in dir(pyrc4) if callable(getattr(pyrc4, a))])" && echo "$t OK"
    popd
}

for t in $TESTS;
do
    echo "Test $t: $t.yaml -> $OUT/$t.c"
    one_test $t
done

