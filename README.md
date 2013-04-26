###DStatus - A Status bar for dwm
---------------------------------

This is "hardcoded" for my system and was not meant to be flexible.  Flexibility costs memory and processor time.
This can, however, be used as a template for your own status bar app.<br>
__Note:__ This was written to work with the status colors patch and use symbols from the ohsnap font. Both the colors and the symbols are into the format strings of the sprintf commands.<br>
To remove the dependency on status colors and ohsnap font, simply remove these characters from the format strings.

**Dependencies:**
* `libmpdclient`
* `alsa-lib`
* `libX11`

_Forked from: [Jesse Mcclure](http://github.com/trilbywhite)_<br>
_Thanks to: [Jente](http://github.com/unia) for alsa sound level query function._
