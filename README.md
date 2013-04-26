###DStatus - A Status bar for dwm
---------------------------------

This is "hardcoded" for my system and was not meant to be flexible.  Flexibility costs memory and processor time.
This can, however, be used as a template for your own status bar app.<br>
__Note:__ This was written to work with the status colors patch and use symbols from the ohsnap font. Both the colors and the symbols are int the format strings of the sprintf commands.<br>
To remove the dependecy on status colors and ohsnap font, simply remove these characters from the format strings.

__Dependencies:__
* _libmpdclient_
* _alsa-lib_
* _libX11_

_Forked from: [Jesse Mcclure](http://github.com/trilbywhite)_<br>
_Thanks to: [Jente](http://github.com/unia) for alsa sound level query function._
