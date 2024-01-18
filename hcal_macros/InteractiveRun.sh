#!/bin/bash

root.exe -q -b "Fun4All_Towerv2.C(1000, \"$1\", \"$2\")"
root.exe -q -b "analyze_hcal_ht.C(\"$2\", \"$3\")" > "$4"
root.exe -q -b "Fun4All_Waveform.C(1000, \"$1\", \"$3\", \"$5\")"
root.exe -q -b "analyze_hcal_ht_waves.C(\"$5\", \"$6\")"
echo "JOB COMPLETE!"
