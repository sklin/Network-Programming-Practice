#!/bin/sh

while [ 1 ]
do
    ps auxww | grep sklin | grep server | grep -v grep | grep -v mosh
    ps auxww | grep sklin | grep client | grep -v grep | grep -v mosh
    echo '  '

    sleep 2
done
