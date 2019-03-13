# tesv-cobb-api
CobbAPI: A script-extending resource for Skyrim Classic mod authors.

This DLL depends on [the Shared Items Project I use for my reverse-engineered findings](https://github.com/DavidJCobb/skyrim-classic-re). If you were linked here in search of my reverse-engineered findings for Skyrim Classic, then you should go to that repo instead.

In order to get the CobbAPI solution and project to load properly, you will need to manually edit the files (current.sln and CobbBugFixes.vcxproj) and fix up the file paths for the shared project. Visual Studio 2015 will not allow you to do this through the GUI; it refuses to load anything that has incorrect paths, but also refuses to allow you to tell it where the missing files are. I apologize for the inconvenience; doing things this way will allow me to consolidate all of my reverse-engineered code so that I don't end up with multiple copies that have fallen out of synch, and it would be a perfectly viable approach were Visual Studio even a little less brittle.

SKSE source code and project files are not included.

The finished work is available for download [from NexusMods](https://www.nexusmods.com/skyrim/mods/95546).
