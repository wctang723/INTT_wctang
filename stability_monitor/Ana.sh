#!/bin/bash

RunStart=$1
RunEnd=$2
RunEnd=$((${RunEnd}+1))

dir=/sphenix/tg/tg01/commissioning/INTT/merged_root_files/
dir_code=/sphenix/u/wctang/workspace/my_INTT/stability_monitor/

until [ "${RunStart}" == "${RunEnd}" ]
do
    filename=${dir}beam_inttall-000${RunStart}-0000_event_base_ana.root
    #echo "This is the filename: ${filename}"
    test -f ${filename}
    #echo $?
    if [ $? == 0 ]
    then
        echo -e "${filename} exists! \n"
        echo -e "Running the hit_per_event.cc... \n"
        root -l -q ${dir_code}/hit_per_event.cc\(\"${filename}\"\)
    else
        echo -e "${filename} doesn't exist! \n"
    fi
    RunStart=$(($RunStart+1))
done

#echo "The result of is ==> $s"