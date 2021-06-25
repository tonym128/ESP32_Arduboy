import os
import shutil

rootdir = '.'

for subdir, dirs, files in os.walk(rootdir):
    for file in files:
        if (file[-4:].lower() == '.ino'):
            dirstart = subdir.rfind(os.path.sep)
            dirend = len(subdir)
            gameName = subdir[dirstart+1:dirend]
        
            if (file.lower() == gameName.lower()+'.ino'):
                print('Converting ' + gameName)
                if (os.path.isfile(os.path.join(subdir, file+'.orig'))==0):
                    # Copy File to Backup
                    shutil.copyfile(os.path.join(subdir, file), os.path.join(subdir, file+'.bak'))
                    shutil.move(os.path.join(subdir, file), os.path.join(subdir, file+'.orig'))
                else:
                    shutil.copyfile(os.path.join(subdir, file)+'.bak', os.path.join(subdir, file+'.orig'))
                    os.remove(os.path.join(subdir, file))
 
                # Remove ESP8266
                command = "sed -i 's/#include <ESP8266WiFi.h>//' "+os.path.join(subdir, file+'.orig')
                os.system(command)

                # Change setup and loop to inogamesetup and inogameloop
                command = "sed -i 's/void setup()/void inogamesetup()/' "+os.path.join(subdir, file+'.orig')
                os.system(command)

                command = "sed -i 's/void loop()/void inogameloop()/' "+os.path.join(subdir, file+'.orig')
                os.system(command)

                # Change avr/pgmspace to pgmspace
                # Change avr/pgmspace to pgmspace as a search in VS Studio Code
                command = "sed -i 's/avr\/pgmspace/pgmspace/' "+os.path.join(subdir, file+'.orig')
                os.system(command)
                
                command = "sed -i 's///' "+os.path.join(subdir, file+'.orig')
                os.system(command)

                with open(os.path.join(subdir, file),'wb') as wfd:
                    for f in [os.path.join(subdir, file)+'.orig','gameRunner.ino']:
                        with open(f,'rb') as fd:
                            shutil.copyfileobj(fd, wfd)
