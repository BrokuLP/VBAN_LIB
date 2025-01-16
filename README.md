# Important this is a Work and Progress and none functional

# VBAN_LIB
library to decode vban packets on a microcontroller

Implements the VBAN spec as specified here: https://vb-audio.com/Voicemeeter/VBANProtocol_Specifications.pdf (revision 10)

## How to use

### configure supported sub protocols
Different sub protocols are can be supported depending on which macros are defined.

- `#define VBAN_SUPPORT_AUDIO` enables support for audio 
- `#define VBAN_SUPPORT_TEXT`
- `#define VBAN_SUPPORT_SERVICE`
- `#define VBAN_SUPPORT_SERIAL`