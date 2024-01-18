#!/bin/bash

root.exe -q -b "Fun4All_Towerv2.C(1000, \"$1\", \"$2\")"
root.exe -q -b "analyze_emcal_ht.C(\"$2\", \"$3\")" > "$4"
echo "JOB COMPLETE!"
