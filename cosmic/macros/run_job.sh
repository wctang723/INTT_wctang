#! /bin/bash
# export USER="$(id -u -n)"
# export LOGNAME=${USER}
# export HOME=/sphenix/u/${LOGNAME}

if [ ! -f "$3"]; then
    echo "input file $3 does not exist"
    exit 1
fi

source /opt/sphenix/core/bin/sphenix_setup.sh -n ana.427

# export MYINSTALL=
# export LD_LIBRARY_PATH=
# export ROOT_INCLUDE_PATH=

source /sphenix/tg/tg01/commissioning/INTT/repositories/libraries/intt_setup_v2.sh ~/intt_config_paths.txt
# source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

# This only need when you wanna use your own macros instead of defalut(or official) one
# export ROOT_INCLUDE_PATH=/sphenix/user/ChengWei/sPH_macros/common:$ROOT_INCLUDE_PATH

echo $ROOT_INCLUDE_PATH | sed -e "s/:/\n/g"

# print the environment (for debugging)
# printenv
# this is how you run your Fun4All_G4_sPHENIX.C macro in batch:
root.exe -q -b CosmicTracking_Fun4All.C\(\"$1\", $2, $3, $4\)

echo all done