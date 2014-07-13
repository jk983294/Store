#!/usr/bin/python
# Filename: backup_ver1.py

import os
import time

# 1. The files and directories to be backed up are specified in a list.
#source = ['/home/swaroop/byte', '/home/swaroop/bin']
source = [r'D:\namepwd.txt', r'D:\InitPos150W.txt']
# If you are using Windows, use source = [r'C:\Documents', r'D:\Work'] or something like that
sourceFile = ' '.join(source)
# 2. The backup must be stored in a main backup directory
# Remember to change this to what you will be using
target_dir = 'D:\\' 

# 3. The files are backed up into a zip file.
# 4. The name of the zip archive is the current date and time
target = target_dir + time.strftime('%Y%m%d%H%M%S') + '.rar'
print ( 'target file location to', target )
# 5. We use the zip command (in Unix/Linux) to put the files in a zip archive
#zip_command = "zip -qr '%s' %s" % (target, ' '.join(source))
zip_command = 'rar a "%s" "%s"' % (target, sourceFile)   # 字符串处理 windows
# Run the backup
if os.system(zip_command) == 0:
    print ( 'Successful backup to', target )
else:
    print ( 'Backup FAILED' )
