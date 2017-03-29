## System of IoT "Oona"
---
This repo contains codes for Java Server, Operating controllers & GUI for the system of IoT "Oona"

The purpose of "Oona" is to spare a user from the effort of regulating illumination, temperature and other parameters. It allows to simplify regulation of environment in a house, an office and even in a spaceship. People will be able to do personal affairs with comfort.

The purpose of this project is to create the automated control system for location including the controller interacting with control units, the server accepting changes from the controller and the client part which is responsible for convenient information output about a status of indoor environment for the user.

Write up issues and answer questions on [link](http://bit.ly/2ee4MIL).

See Oona's screenshots on [link](https://github.com/Wact/Control-system-of-IoT-Oona/tree/master/Screenshots).

### Instructions for how to develop, use, and test the code
---
##### Hardware:
- prerequisites:
    - iBoard Pro
    - Foca v2.2
    - Arduino(for example, UNO)
    - 2 modules nRF24L01+ (and more)
    - a lot of bonding wires
    - different analog/digital sensors for your system
- download arduino libraries and put them in path ".\Documents\Arduino\libraries":
   - [RF24](https://github.com/maniacbug/RF24)
   - [iBoardRF24](https://github.com/andykarpov/iBoardRF24)
- create LAN and connect iBoard Pro to your router by means of fiber-optical wire

##### Software:
- project based on virtual server. You can use [WampServer](http://www.wampserver.com)
- enter data base from MySQL folder in path ".\wamp\bin\mysql\mysql5.6.17\data"
- compile "ServerMaven" project and create .jar file. Launch it
- download "iBoard.ino" in your iBoard Pro and "UNO.ino" in you Arduino UNO
- import other files on your server(for example, in "www" folder on WampServer)
### License
---
Control system of IoT "Oona" is licensed with the MIT License. For more details, see [LICENSE](https://github.com/Wact/Control-system-of-IoT-Oona/blob/master/LICENSE)

© Malygin Artyom, 2017