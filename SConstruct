#! /bin/python
# Dark Horizon SConstruct file
import subprocess, sys, os
import platform
import glob
import shlex
import shutil

from colorizer import colorizer
from BuildHelper import *

setup(ARGUMENTS)



def setupDependencies():
	### Set our libraries
	bulletDynamicsLib = 'BulletDynamics'
	bulletCollisionLib = 'BulletCollision'
	bulletLinearMathLib = 'LinearMath'
	glLib = 'GL'
	glewLib = 'GLEW'
	libPThread = 'pthread'
	#cefLib = 'cef'
	#cefDllWrapperLib = 'cef_dll_wrapper'
	angelscriptLib = 'Angelscript'
	threadPool11Lib = 'threadpool11'
	entityXLib = 'entityx'
	boostLogLib = 'boost_log'
	boostLogSetupLib = 'boost_log_setup'
	boostDateTimeLib = 'boost_date_time'
	boostChronoLib = 'boost_chrono'
	boostThreadLib = 'boost_thread'
	boostWaveLib = 'boost_wave'
	boostRegexLib = 'boost_regex'
	boostProgramOptionsLib = 'boost_program_options'
	boostFilesystemLib = 'boost_filesystem'
	boostSystemLib = 'boost_system'
	boostSerializationLib = 'boost_serialization'
	sdlLib = 'SDL2'
	
	
	if (isWindows):
		glLib = 'opengl32'
		glewLib = 'glew32'
		libPThread = ''
		angelscriptLib = 'Angelscript'
		boostLogLib = 'libboost_log-vc140-mt-1_60'
		boostLogSetupLib = 'libboost_log_setup-vc140-mt-1_60'
		boostDateTimeLib = 'libboost_date_time-vc140-mt-1_60'
		boostChronoLib = 'libboost_chrono-vc140-mt-1_60'
		boostThreadLib = 'libboost_thread-vc140-mt-1_60'
		boostWaveLib = 'libboost_wave-vc140-mt-1_60'
		boostRegexLib = 'libboost_regex-vc140-mt-1_60'
		boostProgramOptionsLib = 'libboost_program_options-vc140-mt-1_60'
		boostFilesystemLib = 'libboost_filesystem-vc140-mt-1_60'
		boostSystemLib = 'libboost_system-vc140-mt-1_60'
		boostSerializationLib = 'libboost_serialization-vc140-mt-1_60'


	# Set our required libraries
	#libraries.append('glr')
	libraries.append('sqlite3')
	libraries.append(bulletDynamicsLib)
	libraries.append(bulletCollisionLib)
	libraries.append(bulletLinearMathLib)
	libraries.append(sdlLib)
	libraries.append(glLib)
	libraries.append(glewLib)
	libraries.append(libPThread)
	#libraries.append('sfml-system')
	#libraries.append('sfml-window')
	libraries.append('assimp')
	libraries.append('freeimage')
	libraries.append(angelscriptLib)
	libraries.append(threadPool11Lib)
	libraries.append(entityXLib)
	libraries.append(boostLogLib)
	libraries.append(boostLogSetupLib)
	#libraries.append(boostDateTimeLib)
	#libraries.append(boostChronoLib)
	libraries.append(boostThreadLib)
	#libraries.append(boostWaveLib)
	libraries.append(boostRegexLib)
	libraries.append(boostProgramOptionsLib)
	libraries.append(boostFilesystemLib)
	libraries.append(boostSystemLib)
	#libraries.append(boostSerializationLib)
	
	if (not isWindows):
		# XInput for linux
		libraries.append( 'Xi' )
	
	### Set our library paths
	library_paths.append(dependenciesDirectory + 'assimp/lib')
	library_paths.append(dependenciesDirectory + 'boost/lib')
	library_paths.append(dependenciesDirectory + 'bullet/lib')
	library_paths.append(dependenciesDirectory + 'sdl/lib')
	library_paths.append(dependenciesDirectory + 'freeimage/lib')
	#library_paths.append(dependenciesDirectory + 'cef3/Release')
	#library_paths.append(dependenciesDirectory + 'sfml/lib')
	library_paths.append(dependenciesDirectory + 'glew/lib')
	library_paths.append(dependenciesDirectory + 'angelscript/lib')
	library_paths.append(dependenciesDirectory + 'entityx/lib')
	library_paths.append(dependenciesDirectory + 'threadpool11/lib')
	library_paths.append(dependenciesDirectory + 'sqlite3/lib')
	
	#library_paths.append('../glr/build')
	#library_paths.append('./lib')
	#library_paths.append('./lib_d')

	cpp_defines.append( ('PACKAGE_VERSION', '\\"0.0.1\\"' ) )
	cpp_defines.append( ('PACKAGE_BUGREPORT', '') )

def setupEnvironment(env):
	col = colorizer()
	col.colorize(env)
	
	### Set our environment variables
	env.Append( CPPFLAGS = cpp_flags )
	env.Append( CPPDEFINES = cpp_defines )
	env.Append( CPPPATH = cpp_paths )
	env.Append( LINKFLAGS = link_flags )
	
	env.SetOption('num_jobs', buildFlags['num_jobs'])
	
	if isLinux:
		# Set our runtime library locations
		env.Append( RPATH = env.Literal(os.path.join('\\$$ORIGIN', '.')))
		
		# include cflags and libs for gtk+-2.0
		env.ParseConfig('pkg-config --cflags --libs gtk+-2.0')

def copyFile(filename):
	try:
		shutil.copy(filename, './build/')
	except:
		#print('Failed to copy cef wrapper!')
		pass

def copyAllFiles(directory):
	try:
		for filename in glob.glob(os.path.join(directory, '*.*')):
			shutil.copy(filename, './build/')
	except:
		#print('Failed to copy cef wrapper!')
		pass

def copyResources():
	"""Copies over resources to the build directory.
	"""
	
	os.chdir( '../dark_horizon' )
	
	if (not os.path.exists('build')):
		os.makedirs('build')
	
	try:
		if (not os.path.exists('./build/scripts')):
			shutil.copytree('scripts', 'build/scripts')
			os.chmod('build/scripts', 0755)
			print("Copied scripts");
	except:
		print("Couldn't copy scripts");
		
	try:
		if (not os.path.exists('./build/data')):
			shutil.copytree('data', 'build/data')
			os.chmod('build/data', 0755)
			print("Copied data");
	except:
		print("Couldn't copy data");
	
	try:
		if (not os.path.exists('./build/settings.ini')):
			shutil.copyfile('settings.ini', 'build/settings.ini')
			os.chmod('build/settings.ini', 0755)
			print("Copied settings.ini");
	except:
		print("Couldn't copy settings.ini");

		

	#copyAllFiles(dependenciesDirectory + 'cef3/Release/')
	#copyAllFiles(dependenciesDirectory + 'cef3/Resources/')
	copyFile(dependenciesDirectory + 'assimp/lib/libassimp.so')
	copyFile(dependenciesDirectory + 'assimp/lib/assimp.dll')
	copyFile(dependenciesDirectory + 'assimp/lib/libassimp.so.3')
	copyFile(dependenciesDirectory + 'assimp/lib/libassimp.so.3.1.1')
	#copyFile(dependenciesDirectory + 'sfml/lib/libsfml-system.so')
	#copyFile(dependenciesDirectory + 'sfml/lib/libsfml-system.so.2')
	#copyFile(dependenciesDirectory + 'sfml/lib/libsfml-system.so.2.3')
	#copyFile(dependenciesDirectory + 'sfml/lib/libsfml-window.so')
	#copyFile(dependenciesDirectory + 'sfml/lib/libsfml-window.so.2')
	#copyFile(dependenciesDirectory + 'sfml/lib/libsfml-window.so.2.3')
	copyFile(dependenciesDirectory + 'freeimage/lib/FreeImage.dll')
	copyFile(dependenciesDirectory + 'sdl/lib/SDL2.dll')
	copyFile(dependenciesDirectory + 'glew/lib/glew32.dll')
	copyFile(dependenciesDirectory + 'glew/lib/libGLEW.so')
	copyFile(dependenciesDirectory + 'glew/lib/libGLEW.so.1.12')
	copyFile(dependenciesDirectory + 'glew/lib/libGLEW.so.1.12.0')
	copyFile(dependenciesDirectory + 'threadpool11/lib/threadpool11.dll')
	copyFile(dependenciesDirectory + 'sqlite3/lib/sqlite3.dll')
	
	for l in libraries:
		copyFile(dependenciesDirectory + 'boost/lib/lib{0}.so'.format(l))
		copyFile(dependenciesDirectory + 'boost/lib/lib{0}.so.1.60.0'.format(l))



print("Compiling Dark Horizon")

# Tell SCons to create our build files in the 'build' directory
VariantDir('build', 'src', duplicate=0)

# Set our source files
source_files = Glob('build/*.cpp')

source_files = source_files + Glob('build/logger/*.cpp')
source_files = source_files + Glob('build/utilities/*.cpp')
source_files = source_files + Glob('build/model/*.cpp')
source_files = source_files + Glob('build/as_wrapper/*.cpp')
source_files = source_files + Glob('build/as_wrapper/scriptstdstring/*.cpp')
source_files = source_files + Glob('build/as_wrapper/scriptbuilder/*.cpp')
source_files = source_files + Glob('build/as_wrapper/scriptarray/*.cpp')
source_files = source_files + Glob('build/as_wrapper/glm_bindings/*.cpp')
source_files = source_files + Glob('build/world/*.cpp')
source_files = source_files + Glob('build/graphics/*.cpp')
source_files = source_files + Glob('build/physics/*.cpp')
source_files = source_files + Glob('build/physics/bullet/*.cpp')
source_files = source_files + Glob('build/audio/*.cpp')
source_files = source_files + Glob('build/entities/*.cpp')
source_files = source_files + Glob('build/ai/*.cpp')
source_files = source_files + Glob('build/angel_script/*.cpp')
# Source files from pyliteserializer
source_files = source_files + Glob('build/data_store/*.cpp')
# Source files from SQLiteCpp, a c++ sqlite wrapper
source_files = source_files + Glob('build/data_store/sqlitec++/*.cpp')

# glr 'Extras'
source_files = source_files + Glob('build/extras/*.cpp')

# Simplex Noise source
source_files = source_files + Glob('build/noise/*.cpp')
source_files = source_files + Glob('build/noise/simplexnoise/*.cpp')

setupDependencies()

### Create our environment
env = Environment(ENV = os.environ, TOOLS = [buildFlags['compiler']], TARGET_ARCH='x86')
setupEnvironment(env)

print("Build type: " + buildFlags['build'])
print('compiler: ' + buildFlags['compiler'])

# Tell SCons the program to build
env.Program('build/darkhorizon', source_files, LIBS = libraries, LIBPATH = library_paths)

### Copy all of our required resources to the build directory
copyResources()
