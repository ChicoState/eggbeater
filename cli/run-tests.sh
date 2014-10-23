#! /bin/sh

make testsuite

pushd tests

find *.elf | sh -r

popd