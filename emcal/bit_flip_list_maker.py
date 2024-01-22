import re
import os
from collections import defaultdict

def extract_numbers(line):
    hot_tower_match = re.search(r'hot tower at channel (\d+)', line)
    chi2_match = re.search(r'average chi2 (\d+\.\d+)', line)

    if hot_tower_match:
        key = int(hot_tower_match.group(1))
    else:
        key = None

    if chi2_match:
        value = float(chi2_match.group(1))
    else:
        value = None

    return key, value

directory_path = '/sphenix/user/egm2153/calib_study/waveform_analysis_ht/emcal'
all_files = os.listdir(directory_path)
file_names = [file for file in all_files if file.endswith(".txt")]

number_files = defaultdict(list)  # Using float as values

for file_name in file_names:
    with open(file_name, 'r') as file:
        # Extract numbers from each line in the file
        for line in file:
            key, value = extract_numbers(line)
            if key is not None and value is not None:
                number_files[key].append((value, file_name.split(".")[0].split("_")[-2], file_name.split(".")[0].split("_")[-1]))
                #number_files[key].append(file_name.split(".")[0].split("_")[-1])

for key, value in number_files.items():
    print(f"{key}: {value}")