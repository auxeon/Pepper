#!/bin/bash

echo "#include <pepper.h>" > header_.h
cat ps_*.h > pepper_.h
cat ps_*.c > pepper_.c

cat pepper_.h > pepper.h
cat header_.h pepper_.c > pepper.c

rm header_.h
rm pepper_.c
rm pepper_.h