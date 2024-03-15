#!/bin/bash

set -e
set -u
#set -x

if [ $# != 1 ]; then
    echo "please enter a table suffix"
fi

export TSUFF=$1

psql \
    -X \
    --echo-all \
    --set AUTOCOMMIT=off \
    --set ON_ERROR_STOP=on \
    --set TSUFF=$TSUFF \
    --set QTSTUFF=\'$TSUFF\' \
    -f ./test.sql 

psql_exit_status=$?

if [ psql_exit_status != 0 ]; then
   echo "psql failed while trying to run this sql script" 1>&2
   exit $psql_exit_status
fi

echo "sql script successful"
