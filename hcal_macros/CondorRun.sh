#!/bin/bash

source /opt/sphenix/core/bin/sphenix_setup.sh -n
export HOME=/sphenix/u/egm2153
export MYINSTALL=$HOME/install
source $OPT_SPHENIX/bin/setup_local.sh $MYINSTALL
export SPHENIX=$MYINSTALL

echo "------------------setting up environment--------------------"
export Cur_dir=$(pwd)
echo "running area:" ${Cur_dir}
echo "-------------------------running----------------------------"
cd ${Cur_dir}
ls
date
root.exe -q -b "Fun4All_Towerv2.C(1000, \"$1\", \"$2\")"
root.exe -q -b "analyze_hcal_ht.C(\"$2\", \"$3\")" > "$4"
root.exe -q -b "Fun4All_Waveform.C(1000, \"$1\", \"$3\", \"$5\")"
root.exe -q -b "analyze_hcal_ht_waves.C(\"$5\", \"$6\")"
date
echo "JOB COMPLETE!"