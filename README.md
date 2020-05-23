# deconz-xbee-plugin

This repository contains the old XBee plugin for deCONZ by dresden elektronik, which no longer is maintained.  I'm trying to get it compiled for deCONZ v2.05.35 and later for educational purposes and to see what functionality could be incorparated in the [REST API plugin](https://github.com/dresden-elektronik/deconz-rest-plugin).

See https://github.com/dresden-elektronik/deconz-rest-plugin/issues/643 for some background info.

To pair the XBee, configure it as follows (from https://www.dresden-elektronik.de/faq.html#deconz):

Abbr. | Value | Comment
-- | -- | --
ID | 0 | extended pan id will be discovered during network search
SC | FFFF | scan on all channels (11-26) for a network
ZS | 2 | ZigBee Stack Profile 2 (ZigBee PRO)
EE | 1 | Security
EO | 3 | Security
NK | 0 | Network Key will be transferred from trust center (or set here if known)
KY | 5a6967426565416c6c69616e63653039 | HA default trust center link key: `ZigBeeAlliance09`
