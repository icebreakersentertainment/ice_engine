#! /usr/bin/python

import subprocess
import sys
import os
import shutil

def mkdirs(dir):
	try:
		os.makedirs(dir)
	except:
		pass

def cp(fromFile, toFile):
	try:
		shutil.copy(fromFile, toFile)
	except:
		pass




rc = subprocess.call( 'doxygen herculesdox', shell=True )
if (rc != 0):
	print("Script halted due to error(s)!")
	sys.exit(1)

mkdirs('dox/html/static')
mkdirs('dox/html/static/css')
mkdirs('dox/html/static/js')

cp('bootstrap.min.css', 'dox/html/static/css/')
cp('customdoxygen.css', 'dox/html/static/css/')
cp('bootstrap.min.js', 'dox/html/static/js/')
cp('jquery-1.11.0.min.js', 'dox/html/static/js/')
cp('doxy-boot.js', 'dox/html/static/js/')
