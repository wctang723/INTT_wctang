#! /bin/bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

if [[ $# -ne 3 ]]; then
	echo "inputs needs 3 variables"
	exit 1
fi

# if [[ ! -f "$1" ]]; then
# 	printf "The file is not exist!\n"
# 	exit 2
# fi

source /opt/sphenix/core/bin/sphenix_setup.sh -n ana.437

# export MYINSTALL=
# export LD_LIBRARY_PATH=
# export ROOT_INCLUDE_PATH=

# source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL
source /sphenix/tg/tg01/commissioning/INTT/repositories/libraries/intt_setup_v2.sh ~/workspace/intt_config_paths.txt

# This only need when you wanna use your own macros instead of defalut(or official) one
# export ROOT_INCLUDE_PATH=/sphenix/user/ChengWei/sPH_macros/common:$ROOT_INCLUDE_PATH

echo $ROOT_INCLUDE_PATH | sed -e "s/:/\n/g"


# print the environment (for debugging)
# printenv
# this is how you run your Fun4All_G4_sPHENIX.C macro in batch:
skipnum=$(($2*$3))

# $1 = run number, $2 = run events, $3 = skip, $4 = process 
root -l -q -b "CosmicTracking_Fun4All.C($1, $2, ${skipnum}, $3)"

printf "all done!\n"