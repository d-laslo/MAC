#/usr/bash


if [ -d ./obj/ ]
then 
    echo 'Folder "obj" is existed'
else mkdir ./obj
fi

if [ -d ./result/ ]
then 
    echo 'Folder "result" is existed'
else mkdir ./result
fi
