#!/bin/bash

function echo_tmp_id(){
    uuid=$(uuidgen)
    echo "tmp id is" $uuid
}

function process_input(){
    iconv -f GBK -t UTF-8 $1 -o $1
    sed -i 's/1/s/g' $1 
    list_num=`cat $1 | wc -l`
    echo "list_num" $list_num

    while read line
    do
        arr1+=($line)
        echo $line
    done < $1 

    echo $arr1 # array

    for line in "${arr1[@]}"
    do
        echo "arr" $line
    done

    i=0
    for i in ${!arr1[@]};do
        echo "arr2" ${arr1[i]}
        ((i++))
    done

    echo $i

    return $i
}

echo "argc num" $#
echo "argc num"$#  #cat string

if [ $# -ne 3 ];then
    echo "usage : sh run.sh [input_list] [input_path] [input_num]"
fi

#-eq ���ڣ�-ne �����ڣ�-gt ���ڣ�-lt С�ڣ�-ge ���ڵ��ڣ�-le С�ڵ���
if [ $3 -eq 10 ]; then
    echo "input is 3"
elif [ $3 -ne 4 ]; then
    echo "input != 4"
elif [ $3 -lt 5 ]; then 
    echo "input < 5"
else
    echo "else"
fi

#if file/path exits
if [ ! -f $1 ];then
    echo "input list" $1 "not exit"
fi

if [ ! -d $2 ];then
    echo "output path" $2 "not exit"
fi

#date, time
echo "$(date  +%Y%m%d-%H:%M:%S)"

echo_tmp_id

process_input $1
echo "output" $?
