# Pepper

## what it is 
* Pepper is an engine to develop and prototype things quick in a sandbox environment

## key principles behind this project
* object oriented doesn't scale well - when the volume of data increases, using a data driven design with separate data and methods that act on that data is vastly easier to maintain scale and less error prone - this is why using C which prohibits the bells and whistles of ```object orientation``` makes sense. just focus on the methods.
* the goal with this engine is to develop a single source file engine that has all the most essential functionality needed to start showing stuff on screen and building apps if needed.
* this seeks to a rapid easy to use and fast sandbox prototyping engine.

## why C
* this engine is based on ```C99``` primarily because it has all the simplicity and expressiveness of ```C```.
* it also has a few added benefits of ```inline``` functions.
* ```variadic macros```.
* new types like ```bool```, ```complex``` etc..
* the key idea is to use the simplest tools available for the job but not any simpler than that whilst still maintaining a good level of abstraction.
* there is an intent to add functionality for multiple platforms so ```C``` makes the most sense if i want things to be portable.
* ```C++``` has a lot of bells and whistles and most importantly ```stl``` - but this also has a downside that executable sizes are bigger and even though it is a zero overhead abstraction philosophy - you're using something that is way to overengineered and bulky for the task you want to accomplish.
* ```stl``` is beautiful since it has a very sound mathematical foundation - but i find over reliance on it takes away from the learning aspect.
* something that I find really interesting is that using the (C++) ```clang++ v12.0.0``` compiler with -O3 level optimization turned on produces a smaller executable than (C) ```clang v12.0.0```

## naming convention
* all files that are part of the engine fall under the ```pepper_space``` so files have a format like ```ps_[filename]```
* functions are similarly named with the ```ps_[functionname]``` format
* variables are named sensibly to be more decriptive of what they do
* macros that act in a functionesque sense are named with the ```ps_[macroname]``` style