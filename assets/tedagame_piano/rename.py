import os
import shutil
for filename in os.listdir():
    if filename[0] == '4':
        shutil.copyfile(filename, filename[18:].capitalize())
            
