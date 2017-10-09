wad2udmf.exe
By Chris Freund, (c) 2018, 2017-2009
z34chris@yahoo.com

type 'wad2udmf -h' for help

Usage: wad2udmf [switch] [value]...
=======================================================
Switches: -h        displays this help dialogue
          -input    (-i) binary wadfile to convert
          -output   (-o) converted udmf output file
	-hexen    convert maps to hexen format
          -maps     specified maps to convert
          (previous two features will be added later)


This is a utility for Doom and Doom2 (possibly others)
that converts map(s) from a wadfile into the more modern
UDMF format and then creates a wadfile from that.

Please note that UDMF is many times larger than the binary
format.  Converting an entire IWAD will create a UDMF wadfile
of equal similar minus the graphics.


***Please Note***
Upcoming utility to create doom levels in the works!


current status:
Version 0.3.7.6 beta October 8, 2017
*fixed problem where lump entries would not write to the file
*fixed problem on unconverted maps where DM things would appear in single play
*fixed problem with sector secret flag

*tested on Dark7.wad
*tested on tab8.wad
*tested on void.wad, minor glitches with playerfall script
*tested on demo.wad, door sparks do not work
*tested on unconverted doom2.wad with no mechanical problems
*tested on HR.wad with no mechanical problems
*has problem with map entries where sectors will not write due to
 maps missing ssegs nodes and ssectors entries

Version 0.3.7.5 beta September 17, 2017
*Visual Studio 2015 Community Build
*extraneous vertices should be fixed

Version 0.3.7 beta January 13, 2012
*fixed problem with activation triggers

Version 0.3.6 beta December 30, 2010
what works:
*added dynamic array support so theoretically any number of wad lumps
 can be handled
*added fix for playercross flag when converting a zdoom wad, otherwise
 walkable lines and teleports were never activated

Version 0.3.4 beta December 8, 2010
what works:
*added support for midtex3d and wrapmidtex

Version 0.3.4 beta December 2, 2010
what works:
*increased array size for output wad directory entry writing
 this was set to 200 and is now set to 4000, future release will be dynamic
*other wad lumps are copied over

Version 0.3.3 beta November 2, 2010
what works:
*removed stray number '4' that would appear just before vertexes section
*fixed renderstyle = translucent to renderstyle = "translucent"
*added a comment banner signifying that this utility was used

Version 0.3.2 beta August 30, 2009
what works:
*polyobjects

what may not work:
*zdoom specific stuff
*polyobjects

what does not work:
*other wad lumps are not copied over

Version 0.3.1 beta August 28, 2009
what works:
*behavior and script lumps are now copied over(!)

what may not work:
*zdoom specific stuff

what does not work:
*other non-related but important lumps are not copied over(should I?)
*polyobjects and minor glitches with linedef/sector/thing flags

Version 0.3 beta August 23, 2009
what works:
*zdoom specific stuff

what may not work:
*zdoom specific stuff
*various flags or certain line specials

what does not work:
*behavior lumps are not copied over, go grab slumped and do this yourself
*textmap lumps incorrectly have the 'zdoomtranslated' namespace, slumped helps here
*polyobjects

comments:

nearly all of the zdoom demo wad works after conversion


Version 0.2.1 beta August 20, 2009

what works:
*fixed program error that caused every other map to be converted,
now every map should be converted

what may not work:
*same as before but also includes sector types and/or flags

Version 0.2 beta August 19, 2009

what works:
*should now work on another machine because the runtime library
was previously linked dynamic and not static, *many apologies*

Version 0.1 beta

what works:
*Doom and Doom2 maps are converted to udmf format
*Command line interface

what may not work:
*Skill/single/deathmatch flags associated with Things
*Linedef triggers and sector tags

what does not work:
*For some reason, every other map is converted...
(Programming glitch on my part due to file record pointer handling)
*Maps feature, where a user can convert certain maps and not just all maps
*Actually performing a rough check of the input file to be sure it is a wad file

operating systems tested on so far:
*Windows XP 2002 SP3 via Command Prompt
*Windows XP 2002 Home Edition SP2 via Command Prompt
