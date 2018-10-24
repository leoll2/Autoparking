# Autoparking

The project simulates a typical car parking scenario, where a car has to make maneuvers to correctly re-position itself while avoiding near obstacles. The vehicle is 'intelligent', which means that it autonomously learns how to accomplish the task. This is done using Q-learning, a popular reinforcement learning technique.

This is how the trained model looks like:

![](img/autoparking.gif)

## Implementation

+ Everything has been implemented from scratch by me, with the only exception of graphics libraries (Allegro).  
+ Everything has been implemented in C/C++.  
+ Nothing has been implemented using frameworks or high-level libraries, including the AI stuff.

## Files and Folders

**bin** -> contains the executable binaries after building  
**build** -> contains the .o object files generated during the compilation  
**cache** -> contains pre-trained weights of Q and R so that you can skip the learning phase  
**conf** -> contains the configuration files  
**docs** -> contains a pdf file with interesting information about this project. Give it a look!  
**img** -> contains the gif image above  
**Makefile** -> just a makefile  
**README** -> this file  
**src** -> contains the most important source files, here's the core of the application  
**stats** -> contains some data for statistical purposes generated during the training, if logging is enabled  

## Disclaimer

This work has been carried out as final project for the 'Neural Networks' exam at SSSUP. Anyway, anyone is encouraged to fork, modify or extend it for non-commercial purposes, as long as the original author (that's me) is explicitly credited. Feel free to contact me for any doubt!
