Waveform hot tower analysis:

1. Build the Towerv2 and Waveform tree analysis modules located at towerv2_src and waveform_src respectively

2. Go to the directory corresponding to the calorimeter you are using (emcal / ihcal / ohcal)

3. Create a new queue.list file for your condor jobs using the make_queue_list.py script

4. Run jobs using condor_submit condor.job. This will run macros Fun4All_Towerv2.C and analyze_{detector}_ht.C to find possible hot towers from chi2 value to the template fit and will output 
root files with tower information as well as text files with towers flagged as hot

5. Run bit_flip_list_maker.py to analyze text files and find hot towers that correspond to bit flips versus hot towers that correspond to bit shifts
