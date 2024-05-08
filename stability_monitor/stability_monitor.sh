#!/bin/bash

# README
# This is the script for sending file from SDCC to inttdaq and excute the command in inttdaq

file=$1

scp /sphenix/u/wctang/workspace/my_INTT/stability_monitor/${file} inttdaq:/home/inttdev/work/wctang/my_INTT/stability_monitor/

ssh inttdaq "cd /home/inttdev/work/wctang/my_INTT/stability_monitor; ./psql_test.sh 'test'; python3 reading_data.py"
