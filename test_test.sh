#!/bin/bash

for fichier in ./Tests_TP7/*

do

    if [[ $fichier == ./Tests_TP7/test* ]]
    then
        echo "<<<<<<<<<<<<Test:" $fichier">>>>>>>>>>>>>>>>"
        ./test_e2 $fichier
        echo
    fi
done