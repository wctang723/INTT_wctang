#!/bin/bash

# README
# This is the script for sending file from SDCC to inttdaq and excute some command in inttdaq

scp /sphenix/u/wctang/workspace/my_INTT/stability_monitor/dummydata2_scptest.csv inttdaq:/home/inttdev/work/wctang/my_INTT/stability_monitor/

ssh inttdaq "cd /home/inttdev/work/wctang/my_INTT/stability_monitor; ./psql_test.sh 'test'; python3 reading_data.py"
