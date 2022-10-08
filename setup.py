#! /usr/bin/python

import urllib2
import tarfile
import zipfile
import sys, os
import platform
import glob
import subprocess
import shutil
import argparse



### Establish our system
isLinux = platform.system() == 'Linux'
isWindows = os.name == 'nt'
isMac = platform.system() == 'Darwin'



### Parse command line arguments
parser = argparse.ArgumentParser(usage = 'Usage: %(prog)s <options> release|debug', description='')

parser.add_argument('-a', '--architecture', help='The architecture type')
parser.add_argument('buildType', nargs='?', default='release', help='The build type to setup for - must be either "release" or "debug"')
parser.add_argument('compiler', nargs='?', default=None, help='The to use (i.e. gcc, clang, etc)')

architecture = 'x64'
buildType = None
compiler = 'gcc'
compilerVersion = '6'
platform = 'linux'
sharedLibraryExt = 'so'
staticLibraryExt = 'a'
extension = 'tar.gz'
downloadDepsUrl = 'https://artifact-repo.icebreakersentertainment.com/files'
dependenciesDirectory = 'deps/'
librariesDirectory = 'lib/'

# Set defaults
if isWindows:
	architecture = 'x64'
	compiler = 'msvc'
	platform = 'windows'
	sharedLibraryExt = 'dll'
	staticLibraryExt = 'lib'
	extension = 'zip'
if isMac:
	compiler = 'clang'
	compilerVersion = '8'
	platform = 'mac'
	sharedLibraryExt = 'dylib'

args = parser.parse_args()

if args.architecture is not None:
	architecture = args.architecture
if args.compiler is not None:
	compiler = args.compiler

buildType = args.buildType

if (buildType != 'release' and buildType != 'debug'):
	print('Invalid buildType - must be either "release" or "debug"')
	exit()
if (architecture is not None and architecture != 'x86' and architecture != 'x64'):
	print('Invalid architecture - must be either "x86" or "x64"')
	exit()
if (compiler is not None and compiler != 'gcc' and compiler != 'clang' and compiler != 'msvc'):
	print('Invalid compiler - must be either "gcc", "clang" or "msvc"')
	exit()

### Validate the architecture
if (architecture == 'x86' and isLinux):
	print('x86 architecture is not yet implemented in setup.py for Linux - sorry!')
	exit()
if (architecture == 'x86' and isWindows):
	print('x86 architecture is not yet implemented in setup.py for Windows - sorry!')
	exit()
if (architecture == 'x86' and isMac):
	print('x86 architecture is not yet implemented in setup.py for Mac - sorry!')
	exit()

### Validate the compiler
if (compiler == 'msvc' and isLinux):
	print('msvc compiler is not available for Linux')
	exit()
if (compiler == 'msvc' and isMac):
	print('msvc compiler is not available for Mac')
	exit()

if (compiler == 'msvc'):
	compilerVersion = '16'
if (compiler == 'clang'):
	compilerVersion = '8'

# Debug not ready yet...
if (buildType == 'debug'):
	print('Debug build type is not ready yet - sorry!')
	exit()

print('Setup running')
print('-------------')
print('  Platform: ' + platform)
print('Build Type: ' + buildType)
print('  Compiler: ' + compiler + ' (Version ' + compilerVersion + ')')
print('')

dependencies = dict()
dependencies['boost'] = {'name': 'Boost', 'version': '1.68.0', 'extension': extension}
dependencies['glm'] = {'name': 'GLM', 'version': '0.9.8.3', 'extension': extension}
dependencies['angelscript'] = {'name': 'Angelscript', 'version': '2.32.0', 'extension': extension}
dependencies['assimp'] = {'name': 'Asset Importer', 'version': 'v5.0.1', 'extension': extension}
dependencies['entityx'] = {'name': 'Entityx', 'version': 'master', 'extension': extension}
dependencies['glew'] = {'name': 'GLEW', 'version': '2.1.0', 'extension': extension}
dependencies['sdl'] = {'name': 'SDL', 'version': '2.0.8', 'extension': extension}
dependencies['ctpl'] = {'name': 'CTPL', 'version': 'master', 'extension': extension}
dependencies['freeimage'] = {'name': 'Free Image', 'version': '3.18.0', 'extension': extension}
dependencies['celero'] = {'name': 'Celero', 'version': 'v2.1.0', 'extension': extension}

def getFilename(name, version):
	return '{0}_{1}_{2}_{3}{4}.{5}'.format(
		name,
		version,
		buildType,
		compiler,
		compilerVersion,
		extension
	)

def copyFile(fromFile, toFile):
	try:
		shutil.copy(fromFile, toFile)
	except:
		pass

def copyDirectory(fromDirectory, toDirectory):
	try:
		shutil.copytree(fromDirectory, toDirectory)
	except:
		#print('Failed to copy directory!')
		pass

def checkSystemDependencies():
	"""Check for required system dependencies, and if they are not present, inform the user and quit"""
	
	if isLinux:
		packages = ['build-essential']
		
		for p in packages:
			devNull = open(os.devnull, "w")
			retVal = subprocess.call(["dpkg", "-s", p], stdout = devNull, stderr = subprocess.STDOUT)
			devNull.close()
			
			if (retVal != 0):
				print("Required package '{0}' is not installed.  You will need to install it before you can run setup.py.".format(p))
				sys.exit(1)

# TODO: Implement this?
def installSystemDependencies():
	"""Install any system libraries we require"""
	pass

def download():
	"""Download external library files"""
	
	if (not os.path.exists(dependenciesDirectory)):
		os.makedirs(dependenciesDirectory)
	
	for k in dependencies.keys():
		print('Downloading ' + dependencies[k]['name'])
		
		filename = getFilename(k, dependencies[k]['version'])
		
		url = '{0}/{1}/{2}/{3}'.format(downloadDepsUrl, k, platform, filename)
		print('url: ' + url)
		
		response = urllib2.urlopen(url)
		
		contentLength = response.headers["Content-Length"]
		
		data = ''
		chunk = 4096
		savedSize = 0
		with open(dependenciesDirectory + filename, 'wb+') as f:
			while True:
				data = response.read(chunk)
				if not data:
					print("\n")
					break

				f.write( bytearray(data) )

				savedSize += len(data)
				sys.stdout.write( "\rRead {0} / {1} KB".format(((savedSize)/1000), int(contentLength)/1000) )
				sys.stdout.flush()
					
def extract():
	"""Extract external library files"""
	
	for k in dependencies.keys():
		name = dependencies[k]['name']
		print('Extracting ' + name)
		
		filename = dependenciesDirectory + getFilename(k, dependencies[k]['version'])

		if not isWindows:
			with tarfile.TarFile.open(filename, 'r:gz') as f:
	
	import os
	
	def is_within_directory(directory, target):
		
		abs_directory = os.path.abspath(directory)
		abs_target = os.path.abspath(target)
	
		prefix = os.path.commonprefix([abs_directory, abs_target])
		
		return prefix == abs_directory
	
	def safe_extract(tar, path=".", members=None, *, numeric_owner=False):
	
		for member in tar.getmembers():
			member_path = os.path.join(path, member.name)
			if not is_within_directory(path, member_path):
				raise Exception("Attempted Path Traversal in Tar File")
	
		tar.extractall(path, members, numeric_owner=numeric_owner) 
		
	
	safe_extract(f, path=dependenciesDirectory)
				folders = [f for f in glob.glob(dependenciesDirectory + k + '*') if os.path.isdir(f)]
				for f in folders:
					os.rename(f, dependenciesDirectory+k)
		else:
			with zipfile.ZipFile(filename) as f:
				f.extractall(path=dependenciesDirectory)
				folders = [f for f in glob.glob(dependenciesDirectory + k + '*') if os.path.isdir(f)]
				for f in folders:
					os.rename(f, dependenciesDirectory+k)

#checkSystemDependencies()
installSystemDependencies()
download()
extract()
