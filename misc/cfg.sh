#!/bin/bash

type=${1:-pdf}
opt -passes=dot-cfg >&/dev/null
sed -i -e '2d' -e 's/style=filled,//' .main.dot
dot -T $type .main.dot
rm -f .main.dot

