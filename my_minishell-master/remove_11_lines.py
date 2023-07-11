import os

# Directory containing the files
directory = 'lib/srcs'

# List all the `.c` files in the directory
files = [file for file in os.listdir(directory) if file.endswith('.c')]

# Iterate over the files
for file in files:
    # Open the file in read mode
    with open(os.path.join(directory, file), 'r') as f:
        # Read all the lines of the file
        lines = f.readlines()
    
    with open(os.path.join(directory, file), 'w') as f:
        # Iterate over the lines
        for i, line in enumerate(lines[:11]):
            # Replace "vrubio" with "hgoncalv"
            if 'vrubio' in line:
                if 'vrubio' in line and i == 5:
                    start = line.index('<')+1
                    end = line.index('>')
                    line = line[:start] + 'hgoncalv@student.42lisboa.com' + line[end:]
                    line = line.replace('vrubio', 'hgoncalv')
                    
                    start = line.index('>')+1
                    line = line[:start] + line[start + 2:]
                if 'vrubio' in line and (i == 7 or i==8):
                    line = line.replace('vrubio', 'hgoncalv')
                    start = line.index('v')+1
                    line = line[:start] + line[start+2:]
            # Replace "codespace" with "hgoncalv"
            elif 'codespace' in line:
                if 'codespace' in line and i == 5:
                    start = line.index('<')+1
                    end = line.index('>')
                    line = line[:start] + 'hgoncalv@student.42lisboa.com' + line[end:]
                    line = line.replace('codespace', 'hgoncalv')
                    
                    start = line.index('>')+1
                    line = line[:start] + line[start + 5:]
                if 'codespace' in line and (i == 7 or i==8):
                    line = line.replace('codespace', 'hgoncalv')
                    start = line.index('v')+1
                    line = line[:start] +  " " +line[start:]
            f.write(line)
        
        
        f.writelines(lines[11:])