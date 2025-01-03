
# Ubuntu 24.04.1 LTS setup

### Requirement
cmake v3.22 (I'm currently using cmake 3.28.3, works fine)

### Get a local version of JUCE v8.0.4 in a JUCE subfolder

````bash
# Git clone
git clone https://github.com/juce-framework/JUCE.git 

# OR, if JUCE latest version is not v8.0.4 anymore download the source code from github https://github.com/juce-framework/JUCE/releases/tag/8.0.4 and put everything in a JUCE subfolder.
````

### Build and launch

````bash
# Create a build folder
mkdir build
cd build
# Generate a Cmake project buildsystem
cmake ..
# Compile & link the project
cmake --build .
# Launch the app
./AudioPlayerApp_artefacts/AudiPlayerApp
````


### Missing dependency errors 

For me, running :
````bash
cmake ..
````

...was not able to generate the project buildsystem because some headers related to `freetype2` and `fontconfig` were missing.

Installing the following package did the trick :

````bash
sudo apt install libfontconfig1-dev   
````

## Dependency hack

````bash
# If you have a lot of missing dependencies, you should consider installing `juce-tools` package which comes with all the dependencies needed. Drawback: it also installs the Projucer (not needed when using Cmake flows).
# Note: Idk why it only installs v7.0.5 of JUCE dependencies and Projucer on my computer, I've not been able to get the last current version (v8.0.4) 

sudo apt install juce-tools
````

## Draft

Could be interesting to add he following lines in `CmakeLists.txt` to ensure the libs are correctly installed

````
# Ensure the PkgConfig module is available
find_package(PkgConfig REQUIRED)

# Use pkg-config to find the freetype2 package
pkg_check_modules(FREETYPE2 REQUIRED freetype2)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)

# Include the freetype2 headers
include_directories(${FREETYPE2_INCLUDE_DIRS})
include_directories(${FONTCONFIG_INCLUDE_DIRS})

# Add the necessary flags for compilation
add_definitions(${FREETYPE2_CFLAGS_OTHER})
add_definitions(${FONTCONFIG_CFLAGS_OTHER})

````