###DStatus - A Status bar for dwm
---------------------------------

This is "hardcoded" for my system and was not meant to be flexible.  Flexibility costs memory and processor time.
This can, however, be used as a template for your own status bar app.<br>
_Note:_ that this was written to work with the status colors patch and use symbols from the ohsnap font. Both the colors and the symbols are int the format strings of the sprintf commands.

To remove the dependecy on status colors and ohsnap font, simply remove these characters from the format strings.

__Dependencies:__<br>
* libmpdclient
* alsa-lib
* libX11

_Forked from: [Jesse Mcclure a.k.a Trilby](http://github.com/trilbywhite)_<br>
_Thanks to: [Unia](http://github.com/unia) for alsa sound level query function._
