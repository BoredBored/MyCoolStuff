# -*- coding: UTF-8 -*-
from subprocess import call
from os.path import expanduser
from os.path import isdir
from os.path import isfile
from datetime import datetime
from datetime import date
from os import chdir
from os import mkdir
from urllib2 import urlopen
from urllib2 import Request
from json import loads

SeeDebugMessages 		= False
SetDestopWallpaper 		= False
CreateFolderForTheImageOfTheDay = False
home = expanduser('~')
folder = "Pictures"
BingBackFolder = "%s/%s/BingDailyImages" % (home, folder) #change to "" if you don't care
LogPath = "%s/BING_BACK_LOG.txt" % (BingBackFolder)
PrintedDateInLog = False
now = datetime.now()
todaydate = "%s\%s\%s"%(now.month, now.day, now.year)
def log(str):
    if SeeDebugMessages == True:
        print("%s ERROR: %s"%(todaydate, str))
    with open(LogPath, "a+") as f:
        try: 
            f.write("%s ERROR: %s"%(todaydate, str))
        except:
            f.close()
            print("%s ERROR: LOGGING INTERRUPTED"%(todaydate))
            quit()
        f.close()
        quit()

def makeFolder(name):
    #str = str.replace("/", "\\") #comment out if recursively creating folders
    if name == "":
        if SeeDebugMessages == True:
            print("ERROR: Please specify a folder name -- \"\"/NULL is not a folder name")
        return 2
    elif isdir(name) == False:
        if SeeDebugMessages == True:
            print("Creating folder \'%s\'" % (name))
        mkdir(name)
        return 0
    else:
        if SeeDebugMessages == True:
            print("Folder \'%s\' already exists" % (name))
        return 1

def downloadPic(url, name):
    if str == "":
        if SeeDebugMessages == True:
            print("ERROR: Please specify a file name for your picture -- \"\"/NULL is not a file name")
        return 1
    elif isfile(name) == False:
        if SeeDebugMessages == True:
            print("Downloading picture from: %s\nSaving pic as: %s" % (url, name))
        with open(name,'wb')as f:
            f.write(urlopen(url).read())
            f.close()
        return 0
    else:
        if SeeDebugMessages == True:
            print("File \'%s\' already exists" % (name))
        return 1


def ImgPathFunc():
    #make request to get the image json
    req = Request("http://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=en-US")
    ImgJSON = ""
    res = ""
    try:
        res = urlopen(req)
    except:
        print("Error")

    data = res.read()

    #parse json to get the image url
    ImgJSON = loads(data)

    return ImgJSON['images'][0]['url']

def GETName(picPath):
    #check if it is the right path
    if ("/az/hprichbg/rb/" in picPath) is False:
        log("String \'/az/hprichbg/rb/\' not in string \'%s\'" % (picPath))
    else:
        return picPath.replace("/az/hprichbg/rb/", "")

def BingImg():
    ImgPath = ImgPathFunc()
    #               ImgURL                          ImgName            
    return "http://www.bing.com%s"%(ImgPath), GETName(ImgPath)

def BingFolder():
    makeFolder(BingBackFolder)
    todayfolder = "%s/%s" %(BingBackFolder, todaydate)

    if CreateFolderForTheImageOfTheDay == True :
	makeFolder(todayfolder)
	chdir(todayfolder)
    else:
        chdir(BingBackFolder)

def printArr(arr):
  print ' '.join(arr)

def change_wallpaper_win(SPISETDESKWALLPAPER, WALLPAPER_PATH):
    import struct
    import ctypes
    #not tested
    sys_parameters_info = ctypes.windll.user32.SystemParametersInfoW if struct.calcsize('P') * 8 == 64 else ctypes.windll.user32.SystemParametersInfoA
    r = sys_parameters_info(SPISETDESKWALLPAPER, 0, WALLPAPER_PATH, 3)

    # When the SPI_SETDESKWALLPAPER flag is used,
    # SystemParametersInfo returns TRUE
    # unless there is an error (like when the specified file doesn't exist).
    if not r:
        print(ctypes.WinError())
	return False
    return True

def set_wallpaper_windows(ab_path):
  change_wallpaper_win(20, ab_path)

def set_wallpaper_mac(file_loc, debug):
	from sys import stderr
        from subprocess import Popen as ex
	try:
		SCRIPT = """/usr/bin/osascript<<END\ntell application "Finder"\nset desktop picture to POSIX file "%s"\nend tell\nEND"""%(file_loc)
		ex(SCRIPT, shell=True)
	except:
		stderr.write("ERROR: Failed to set wallpaper. There might be a bug.\n")
        	return False

def set_wallpaper_linux(file_loc, debug):
        from platform import linux_distribution
        from subprocess import Popen as ex
        from sys import stderr
        desktop_env = linux_distribution()
        if debug == True: 
            print ("Desktop_env = %s"%(str(desktop_env)))
            print (file_loc)
        try:
            uri = "\"file://%s\""%(file_loc)
            if "Ubuntu" in desktop_env or "ubuntu" in desktop_env or "Gnome" in desktop_env or"gnome" in desktop_env:
                if debug == True: print ("Ubuntu/Gnome Detected")
                args = ["gsettings", "set", "org.gnome.desktop.background", "picture-uri", uri]
                printArr(args)
                ex(args)
            elif "gnome2" in destop_env: # Not tested
                if debug == True: print ("Gnome2 detected")
                args = ["gconftool-2","-t","string","--set","/desktop/gnome/background/picture_filename", '%s'%(file_loc)]
                printArr(args)
                ex(args)
            elif ["kde3", "trinity"] in destop_env:
                if debug == True: print ("KDE3/Trinity detected")
                args = 'dcop kdesktop KBackgroundIface setWallpaper 0 "%s" 6' % file_loc
                printArr(args)
                ex(args, shell=True)
            elif "LinuxMint" in desktop_env:
                args = ["gsettings", "set", "org.cinnamon.desktop.background", "picture-uri", uri]
                if debug == True: printArr(args)
                ex(args)
            else:
                stderr.write("Warning: Failed to set wallpaper. Your desktop environment is not supported.\n")
                stderr.write("You can try to manually to set your wallpaper to \'%s\'\n" % file_loc)
                return False
            return True
        except:
            stderr.write("ERROR: Failed to set wallpaper. There might be a bug.\n")
            return False

def setWallMain(pathy, debug):
  from platform import system
  ostype = system()
  if debug == True: print ("OS type: %s"%(ostype))
  if ostype == "Linux":
    return set_wallpaper_linux(pathy, debug)
  elif ostype == "Windows":
    return set_wallpaper_windows(pathy)
  elif ostype == "Darwin":
    return set_wallpaper_mac(pathy, debug)
  else:
    stderr.write("ERROR: Failed to set wallpaper. There might be a bug.\n")
    return False

def setWallpaper(path, debug):
  from os.path import isfile
  from os.path import isabs
  from os import getcwd
  from sys import stderr
  pathy = path
  if isfile(pathy) == False:
    stderr.write("ERROR: \"%s\" is not a file\n"%(pathy))
    return False
  if isabs(pathy) == False:
    pathy = "%s/%s"%(getcwd(), path) # change to absolute path
    if debug == True: print ("Changed \'%s\' to %s"%(path, pathy))
  setWallMain(pathy, debug)
  

def main():
    ImgURL, ImgN = BingImg()

    BingFolder() # Comment out if you just want to download in your current working dir

    downloadPic(ImgURL, ImgN)
	
    if SetDestopWallpaper == True:
        print("~ Setting as wallpaper ~")
        if setWallpaper(ImgN, SeeDebugMessages=False) == False:
            print("Error setting wallpaper")
        
main()
