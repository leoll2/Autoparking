# Autoparking

 [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://leoll2.github.io/Autoparking/)
 [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
 [![C++](https://img.shields.io/badge/C++-blue.svg?style=flat&logo=c++)]()
 [![GitHub stars](https://img.shields.io/github/stars/leoll2/Autoparking.svg?style=social&label=Star&maxAge=2592000)](https://github.com/leoll2/Autoparking/stargazers/)
 [![GitHub forks](https://img.shields.io/github/forks/leoll2/Autoparking.svg?style=social&label=Fork&maxAge=2592000)](https://github.com/leoll2/Autoparking/network/)
 [![DOI](https://zenodo.org/badge/137659116.svg)](https://zenodo.org/badge/latestdoi/137659116)


***Autoparking*** simulates a typical car parking scenario, where the goal is to re-position a car with a series of manuevers, while avoiding obstacles nearby. Thanks to Q-Learning, a popular reinforcement learning algorithm, I show that the car is able to park on its own, without any external input.  

Everything, including the AI logic, is implemented from scratch in C++. The only dependency is the open-source graphics library ([Allegro](https://liballeg.org/)).

Want to know more about how it works? Watch the [**video**](https://www.youtube.com/watch?v=UjPmsWEHtsU) or read the short [**paper**](https://leoll2.github.io/Autoparking/docs/paper_short.pdf). Enjoy!

![Autoparking GIF](img/autoparking.gif)  
*Demo of Autoparking trained model*


Table of Contents
=================

  * [Folders organization](#folders-organization)
  * [Setup](#setup)
     * [Install dependencies](#install-dependencies)
     * [Build](#build)
     * [Run](#run)
  * [About](#about)

## Folders organization

**bin** -> contains the executable binaries after building  
**build** -> contains the .o object files generated during the compilation  
**cache** -> contains pre-trained weights of Q and R so that you can skip the learning phase  
**conf** -> contains the configuration files  
**docs** -> contains .pdf files with interesting information about this project. Give it a look!  
**font** -> contains few fonts used by the application  
**img** -> contains the gif image above  
**Makefile** -> just a makefile  
**README** -> this file  
**src** -> contains the most important source files, here's the core of the application  
**stats** -> contains some data for statistical purposes generated during the training, if logging is enabled  

## Setup

First of all, download this repository with:
```
git clone https://www.github.com/leoll2/Autoparking.git
```

### Install dependencies

The only dependency is Allegro5. The following steps refer to Debian/Ubuntu and are based on the official Allego [wiki](https://wiki.allegro.cc/index.php?title=Install_Allegro5_From_Git/Linux/Debian). Installing on other distros is possible, though some dependencies may differ. For instance, Centos/RHEL users shall refer to the relative [documentation](https://wiki.allegro.cc/index.php?title=Install_Allegro5_From_Git/Linux/Centos).
Honestly, I'm not sure which deps are mandatory and which are optional, but unless you have limited storage capacity (or enough time to figure out), I recommend to download them all.

Setup your repo list:
```
cd /etc/apt/
sudo gedit sources.list
```
and add `contrib` and `free` at the end of those lines starting with deb or deb-src. Then update:

```sudo apt-get update```

Now install required dependencies:
```
sudo apt-get install build-essential git cmake cmake-curses-gui xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```
and optional ones:
```
sudo apt-get install -y cmake g++ freeglut3-dev libxcursor-dev libpng-dev libjpeg-dev libfreetype6-dev libgtk2.0-dev libasound2-dev libpulse-dev libopenal-dev libflac-dev libdumb1-dev libvorbis-dev libphysfs-dev
```

From the Autoparking directory, clone the Allegro5 git repository:
```
git clone https://github.com/liballeg/allegro5.git
```
and switch to version 5.2.7:
```
cd allegro5
git checkout 5.2.7
```
Get ready to compile:
```
mkdir build
cd build
ccmake -DCMAKE_INSTALL_PREFIX=/usr ..
```
Inside the cmake environment, press 'C' (configure) and 'E' if it complains about few missing libraries (don't worry, it still works). Then press 'G' (generate).

Here comes the fun, compile:
```
make
```
You can optionally add the -j option to parallelize the compilation on multiple cores.  
Finally, install:
```
sudo make install
```

If all the previous steps went fine, Allegro5 is properly installed and setup.  

### Build

Compiling Autoparking is straightforward. From the project root directory:
```
make
```

### Run

Start the simulation:
```
bin/main
```

## About

This work has first proposed as final project for the 'Neural Networks' exam at SSSUP, and later presented at [EEML](https://www.eeml.eu/home) 2020. [[video](https://www.youtube.com/watch?v=UjPmsWEHtsU)] [[paper](https://leoll2.github.io/Autoparking/docs/paper_short.pdf)]

Anyone is encouraged to fork, modify or extend it for non-commercial purposes, as long as the original author is explicitly credited (cite as below). Feel free to contact me for any doubt!

```
@misc{LaiAutoparking2018,  
  author = {Lai, Leonardo},  
  title = {Autoparking},  
  year = {2018},  
  publisher = {GitHub},  
  journal = {GitHub repository},  
  howpublished = {\url{https://github.com/leoll2/Autoparking}},  
  doi = {10.5281/zenodo.4568892},
}
```
