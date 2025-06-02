<h1 align='center'>Arduino stupid HUD </h1>
  <p align='center'>
<img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white" />
<img src="https://img.shields.io/badge/%20%20%20-written%20in%20kate-blue?style=for-the-badge&logo=kdeplasma&logoColor=black" />
  </p>

KISS (keep it simple, stupid!) embeddable hud implementation using SSD1306 and arduino with GyverOLED library. \
Implemented scroll on str3 and substring on others 
# Documentation
GyverOled required! \
Nothing else than "str1|str2|str3|str4\n" in UTF-8 on serial, that makes this highly embeddable to many programs (see examples)
Result will be something like this: \
str1 - str2 \
———————————
### str3
str4

# Examples (will be updated)

Source SDK (TODO: windows support): https://github.com/denz8028/adruino-stupid-HUD/examples/source-sdk \
Playerctl: https://github.com/denz8028/adruino-stupid-HUD/examples/playerctl.py
