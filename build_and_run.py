import subprocess, sys, os
import shlex

args = ''
for arg in sys.argv:
	if (arg != 'build_and_run.py'):
		args += ' ' + arg

exitCode = subprocess.call( 'scons ' + args, shell=True)

if (exitCode is not 0):
	sys.exit(1)

print("Running Dark Horizon")

os.chdir( 'build/' )
exitCode = subprocess.call( "./darkhorizon" )

if (exitCode is not 0):
	sys.exit(1)
