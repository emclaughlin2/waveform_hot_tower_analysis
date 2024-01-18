#!/bin/bash

root.exe -q -b "Fun4All_Towerv2.C(1000, \"$1\", \"$2\")"
root.exe -q -b "analyze_emcal_ht.C(\"$2\", \"$3\", \"$4\")" > "$5"
root.exe -q -b "Fun4All_Waveform.C(1000, \"$1\", \"$3\", \"$6\")"
root.exe -q -b "analyze_emcal_ht_waves.C(\"$6\", \"$7\")"
echo "JOB COMPLETE!"
