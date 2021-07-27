# spxmit ![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-A22846.svg?logo=raspberrypi)

spxmit "single-pin transmit" (and later maybe transceive) is a linux (kernel 4.8+) tool to work with those single-pin 433Mhz China transmitters (e.g. [FS1000A](https://user-images.githubusercontent.com/5485569/127217479-3b0be086-d2a6-457e-b7d8-37e67f42744c.jpg)) commonly used
in hobby home automation projects. The tool is somewhat similar to [pilight-send](https://manual.pilight.org/programs/send.html) or [rc-switch](https://github.com/sui77/rc-switch) with less functionality but only the things I needed. And without dependencies, because accessing GPIOs is not that hard.

Its not really meant to be used by you. Of course you can. You can also extend it with more protocols and I'll gladly accept those submissions. 
A more realistic purpose for it though is to be dismantled and customized. It's cut right to the chase, very lean, not much fancyness, not even 350 LoC.

## Compiling
Local: You'll need gcc in PATH.
```
make
```

Cross Compiling: You'll need a cross compiler
```
make CROSS_COMPILE=/usr/bin/arm-linux-gnueabi-
```

There are also VS Code tasks included for this.

## Usage

```
spxmit - single pin transmit
Synopsis: 

  spxmit -t 14 -p kaku -i 5652480 -u 1 -s 1

  -t  <tx-pin>     pin number on chip for TX
  -p  <protocol>   protocol name
  -i  <id>         device id
  -u  <unit>       unit number
  -s  <state>      0=inactive, 1=active
  -m  <mode>       0=single, 1=all
  -h               this help
```

### Protocol
Currently there is just one protocol (but it supports a variety of devices apparently): https://manual.pilight.org/protocols/433.92/switch/intertechno.html

Arguments for `-p` option, for convenience, but all resulting in this same protocol:
```
kaku
intertechno
arctech
```

### Device Id
Because there is no receive functionality yet, you'll have to use [pilight-receive](https://manual.pilight.org/programs/receive.html) or [pilight-raw](https://manual.pilight.org/programs/raw.html) to discover the devices id.