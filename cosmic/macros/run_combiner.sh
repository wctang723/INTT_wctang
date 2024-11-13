#! /bin/bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

if [[ $# -ne 5 ]]; then
	echo "inputs needs 5 variables"
	exit 1
fi

source /opt/sphenix/core/bin/sphenix_setup.sh -n ana.437

# export MYINSTALL=
# export LD_LIBRARY_PATH=
# export ROOT_INCLUDE_PATH=

# source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL
source /sphenix/tg/tg01/commissioning/INTT/repositories/libraries/intt_setup_v2.sh ~/workspace/intt_config_paths.txt

# This only need when you wanna use your own macros instead of defalut(or official) one
# export ROOT_INCLUDE_PATH=/sphenix/user/ChengWei/sPH_macros/common:$ROOT_INCLUDE_PATH

echo $ROOT_INCLUDE_PATH | sed -e "s/:/\n/g"

# $1 = run number, $2 = nEvents, $3 = process, $4 = use cdb, $5 = is debug
# skipnum=$(($2*$3))
skipnum=0

# print the environment (for debugging)
# printenv
# this is how you run your Fun4All_G4_sPHENIX.C macro in batch:

# 1 = run number, 2 = nEvents, 3 = skip, 4 = process, 5 = use cdb, 6 = is debug
root -l -q -b "Fun4All_Intt_Combiner_run_base.C($1, $2, ${skipnum}, $3, $4, $5)"

echo all done