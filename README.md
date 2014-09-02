midi-track-extractor
====================

Give a midi file, get every single track as a midi file. 

This program depends on jdk midi c++ library. 
https://github.com/jdkoftinoff/jdksmidi

I provide a CMakeFile.txt to create a project. 
It will ask you a root directory. 
This root directory has "boost_1_56_0" and "libjdkmidi-20051014" directories. 
I only use src and include directories' contents in libjdkmidi-20051014 directory. 
I only use lexical_cast from boost library. 

Unfortunately jdk midi c++ library is under GPL license. 
There is no way I can change it. 
I would expect it to be under LGPL license as least. 

