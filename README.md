# An MNG Tale

More and more I found myself unable to work with the general structure of the original MNGame project. This unmaintainability has causes born out of having ported the code directly from its original Python state which I began writing only 6 months into my programming education. Suffice it to say, the architecture was lacking.

***
## The Problem
Although I tried to make some tweaks and improvements when I moved into C++ territory, the fundamental flaw remained the same - almost everything was hardcoded. And this butterflied into a myriad of problems:

* Tweaking world data required recompile after recompile.
* Scripting was not supported. Behaviour was managed by dozens of flags and if statements.
* All scenes were a complete mess of spaghetti code.
* Endless unnecessary polymorphism and dynamic allocation.

Therefore, I remade the entire thing from scratch.

***
## The solution
This time around I forced myself to think in terms of flexibility via deserialisation. From the get-go I wrote my own fully customisable GUI library and was therefore able to merge all Menus into a single configurable Menu class. I continued in this spirit with the scenes, piece by piece moving away from hardcoded data. First the world colliders, then the camera, then audio and so forth until the entire scene was perfectly described by a single JSON file which remained mutable at runtime.