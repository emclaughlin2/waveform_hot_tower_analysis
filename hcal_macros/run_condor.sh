#!/bin/bash

cat >>condor.job< EOF
Executable                    = CondorRun.sh
Arguments                     = $(flist) $(outflist) $(rootflist) $(analog)
request_memory                = 12GB
Universe                      = vanilla
Notification                  = Never
GetEnv                        = True
Priority                      = +20
Output                        = test_$(process).out
Error                         = test_$(process).err
Log                           = /tmp/egm2153/log/test_$(process).log
Notify_user                   = egm2153@columbia.edu
Queue flist, outflist, rootflist, analog from queue.list
EOF

