import re

# Function to check if the value is greater than 12
def is_value_greater_than_12(line):
    match = re.search(r'average chi2 (\d+\.\d+)', line)
    if match:
        chi2_value = float(match.group(1))
        return chi2_value > 12
    return False

# List to store good files
good_files = []

# List of file names
file_names = ["hta_ohcal_16_28114.txt", "hta_ohcal_16_28115.txt", "hta_ohcal_16_28116.txt", "hta_ohcal_16_28117.txt", "hta_ohcal_16_28118.txt", "hta_ohcal_16_28119.txt", "hta_ohcal_16_28120.txt", "hta_ohcal_16_28121.txt", "hta_ohcal_16_28122.txt", "hta_ohcal_16_28123.txt", "hta_ohcal_16_28124.txt", "hta_ohcal_16_28125.txt", "hta_ohcal_16_28126.txt", "hta_ohcal_16_28127.txt", "hta_ohcal_16_28129.txt", "hta_ohcal_16_28130.txt", "hta_ohcal_16_28131.txt", "hta_ohcal_16_28132.txt", "hta_ohcal_16_28133.txt", "hta_ohcal_16_28134.txt", "hta_ohcal_16_28135.txt", "hta_ohcal_16_28136.txt", "hta_ohcal_16_28137.txt", "hta_ohcal_16_28138.txt", "hta_ohcal_16_28139.txt", "hta_ohcal_16_28140.txt", "hta_ohcal_16_28141.txt", "hta_ohcal_16_28142.txt", "hta_ohcal_16_28143.txt", "hta_ohcal_16_28144.txt", "hta_ohcal_16_28145.txt", "hta_ohcal_16_28146.txt", "hta_ohcal_16_28147.txt", "hta_ohcal_16_28148.txt", "hta_ohcal_16_28149.txt", "hta_ohcal_16_28150.txt", 
"hta_ohcal_16_28151.txt", "hta_ohcal_16_28152.txt", "hta_ohcal_16_28153.txt", "hta_ohcal_16_28154.txt", "hta_ohcal_16_28155.txt", "hta_ohcal_16_28156.txt", "hta_ohcal_16_28157.txt", "hta_ohcal_16_28158.txt", "hta_ohcal_16_28159.txt", "hta_ohcal_16_28160.txt", "hta_ohcal_16_28161.txt", "hta_ohcal_16_28162.txt", "hta_ohcal_16_28163.txt", "hta_ohcal_16_28164.txt", "hta_ohcal_16_28165.txt", "hta_ohcal_16_28166.txt", "hta_ohcal_16_28167.txt", "hta_ohcal_16_28168.txt", "hta_ohcal_16_28169.txt", "hta_ohcal_16_28170.txt", "hta_ohcal_16_28171.txt", "hta_ohcal_16_28172.txt", "hta_ohcal_16_28173.txt", "hta_ohcal_16_28174.txt", "hta_ohcal_16_28175.txt", "hta_ohcal_16_28176.txt", "hta_ohcal_16_28177.txt", "hta_ohcal_16_28178.txt", "hta_ohcal_16_28179.txt", "hta_ohcal_16_28180.txt", "hta_ohcal_16_28181.txt", "hta_ohcal_16_28182.txt", "hta_ohcal_16_28183.txt", "hta_ohcal_16_28184.txt", "hta_ohcal_16_28185.txt", "hta_ohcal_16_28186.txt", 
"hta_ohcal_16_28187.txt", "hta_ohcal_16_28188.txt", "hta_ohcal_16_28189.txt", "hta_ohcal_16_28190.txt", "hta_ohcal_16_28191.txt", "hta_ohcal_16_28192.txt", "hta_ohcal_16_28193.txt", "hta_ohcal_16_28194.txt", "hta_ohcal_16_28195.txt", "hta_ohcal_16_28196.txt", "hta_ohcal_16_28197.txt", "hta_ohcal_16_28198.txt", "hta_ohcal_16_28199.txt", "hta_ohcal_16_28200.txt", "hta_ohcal_16_28201.txt", "hta_ohcal_16_28202.txt", "hta_ohcal_16_28203.txt", "hta_ohcal_16_28204.txt", "hta_ohcal_16_28205.txt", "hta_ohcal_16_28206.txt", "hta_ohcal_16_28207.txt", "hta_ohcal_16_28208.txt", "hta_ohcal_16_28209.txt", "hta_ohcal_16_28210.txt", "hta_ohcal_16_28211.txt", "hta_ohcal_16_28212.txt", "hta_ohcal_16_28213.txt", "hta_ohcal_17_28114.txt", "hta_ohcal_17_28115.txt", "hta_ohcal_17_28116.txt", "hta_ohcal_17_28117.txt", "hta_ohcal_17_28118.txt", "hta_ohcal_17_28119.txt", "hta_ohcal_17_28120.txt", "hta_ohcal_17_28121.txt", "hta_ohcal_17_28122.txt", 
"hta_ohcal_17_28123.txt", "hta_ohcal_17_28124.txt", "hta_ohcal_17_28125.txt", "hta_ohcal_17_28126.txt", "hta_ohcal_17_28127.txt", "hta_ohcal_17_28128.txt", "hta_ohcal_17_28129.txt", "hta_ohcal_17_28130.txt", "hta_ohcal_17_28131.txt", "hta_ohcal_17_28133.txt", "hta_ohcal_17_28134.txt", "hta_ohcal_17_28135.txt", "hta_ohcal_17_28136.txt", "hta_ohcal_17_28137.txt", "hta_ohcal_17_28138.txt", "hta_ohcal_17_28139.txt", "hta_ohcal_17_28140.txt", "hta_ohcal_17_28141.txt", "hta_ohcal_17_28142.txt", "hta_ohcal_17_28143.txt", "hta_ohcal_17_28144.txt", "hta_ohcal_17_28145.txt", "hta_ohcal_17_28146.txt", "hta_ohcal_17_28147.txt", "hta_ohcal_17_28148.txt", "hta_ohcal_17_28149.txt", "hta_ohcal_17_28150.txt", "hta_ohcal_17_28151.txt", "hta_ohcal_17_28152.txt", "hta_ohcal_17_28153.txt", "hta_ohcal_17_28154.txt", "hta_ohcal_17_28155.txt", "hta_ohcal_17_28156.txt", "hta_ohcal_17_28157.txt", "hta_ohcal_17_28158.txt", "hta_ohcal_17_28159.txt", 
"hta_ohcal_17_28160.txt", "hta_ohcal_17_28162.txt", "hta_ohcal_17_28163.txt", "hta_ohcal_17_28164.txt", "hta_ohcal_17_28165.txt", "hta_ohcal_17_28166.txt", "hta_ohcal_17_28167.txt", "hta_ohcal_17_28168.txt", "hta_ohcal_17_28169.txt", "hta_ohcal_17_28170.txt", "hta_ohcal_17_28171.txt", "hta_ohcal_17_28172.txt", "hta_ohcal_17_28173.txt", "hta_ohcal_17_28174.txt", "hta_ohcal_17_28175.txt", "hta_ohcal_17_28176.txt", "hta_ohcal_17_28177.txt", "hta_ohcal_17_28178.txt", "hta_ohcal_17_28179.txt", "hta_ohcal_17_28180.txt", "hta_ohcal_17_28181.txt", "hta_ohcal_17_28182.txt", "hta_ohcal_17_28183.txt", "hta_ohcal_17_28184.txt", "hta_ohcal_17_28185.txt", "hta_ohcal_17_28186.txt", "hta_ohcal_17_28187.txt", "hta_ohcal_17_28188.txt", "hta_ohcal_17_28189.txt", "hta_ohcal_17_28190.txt", "hta_ohcal_17_28191.txt", "hta_ohcal_17_28192.txt", "hta_ohcal_17_28193.txt", "hta_ohcal_17_28194.txt", "hta_ohcal_17_28195.txt", "hta_ohcal_17_28196.txt", 
"hta_ohcal_17_28197.txt", "hta_ohcal_17_28198.txt", "hta_ohcal_17_28199.txt", "hta_ohcal_17_28200.txt", "hta_ohcal_17_28201.txt", "hta_ohcal_17_28202.txt", "hta_ohcal_17_28203.txt", "hta_ohcal_17_28204.txt", "hta_ohcal_17_28205.txt", "hta_ohcal_17_28207.txt", "hta_ohcal_17_28208.txt", "hta_ohcal_17_28209.txt", "hta_ohcal_17_28211.txt", "hta_ohcal_17_28212.txt", "hta_ohcal_17_28213.txt", "hta_ohcal_17_28214.txt"]


# Process each file
for file_name in file_names:
    with open(file_name, 'r') as file:
        lines = file.readlines()
        # Check if any line has a value greater than 12
        if any(is_value_greater_than_12(line) for line in lines):
            print(f"{file_name} has a value greater than 12.")
        else:
            print(f"{file_name} is a good file.")
            good_files.append(file_name)

# Print the list of good files
print("\nList of good files:")
print(good_files)
