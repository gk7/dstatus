## DStatus

### A Status bar for dwm.

Forked from: Jesse Mcclure a.k.a Trilby
github.com/trilbywhite
Thanks to Unia: github.com/unia for alsa sound level query function.

This is "hardcoded" for my system and was not meant to be flexible.  Flexibility costs memory and processor time.
This can, however, be used as a template for your own status bar app.
Note that this was written to work with the status colors patch and use symbols from the ohsnap font.  Both the colors and the symbols are int the format strings of the sprintf commands.

To remove the dependecy on status colors and ohsnap font, simply remove these characters from the format strings.
Dependencies:
* libmpdclient
* alsa-lib
* libX11
