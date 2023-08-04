```
It seems Inverter never sends anything on its own, it only replies to Logger.

== Register phase:

Logger ==> Inverter:
 3a 3a 01 00 00 00 10 04 00 00 89
 3a 3a 01 00 00 00 10 04 00 00 89
 3a 3a 01 00 00 00 10 04 00 00 89
 3a 3a 01 00 00 00 10 00 00 00 85
Logger <== Inverter:
 3a 3a 00 00 01 00 10 80 10 4e 4c 44 4e 33 30 32 30 31 33 36 52 31 30 35 37 04 bf
                             N  L  D  N  3  0  2  0  1  3  6  R  1  0  5  7


Logger ==> Inverter:
 3a 3a 01 00 00 00 10 01 11 4e 4c 44 4e 33 30 32 30 31 33 36 52 31 30 35 37 01 04 42
                             N  L  D  N  3  0  2  0  1  3  6  R  1  0  5  7
Logger <== Inverter:
 3a 3a 00 01 01 00 10 81 01 06 01 0e

== Exchange data phase:

Logger ==> Inverter: read somth., receive 0
 3a 3a 01 00 00 01 11 43 00 00 ca
Logger <== Inverter:
 3a 3a 00 01 01 00 11 c3 01 00 01 4b


Logger=>Inverter: announce my IP
 3a 3a 01 00 00 01 12 41 10 31 39 32 2e 31 36 38 2e 31 2e 35 30 00 00 00 00 03 34
                             1  9  2  .  1  6  8  .  1  .  5  0
Logger <== Inverter:
 3a 3a 00 01 01 00 12 c1 01 06 01 50


Logger ==> Inverter: announce my SN
 3a 3a 01 00 00 01 12 40 10 36 30 34 36 35 32 30 36 30 00 00 00 00 00 00 00 02 a5
                             6  0  4  6  5  2  0  6  0
Logger <== Inverter:
 3a 3a 00 01 01 00 12 c0 01 06 01 4f

Logger ==> Inverter: request model/serial/fw from Inverter
 3a 3a 01 00 00 01 11 03 00 00 8a
Logger <== Inverter:
 3a 3a 00 01 01 00 11 83 
 4d 31 20 20 33 30 30 30 42 45 31
  M  1        3  0  0  0  B  E  1
 86 dd 20 03 0d 58 - ? 
 6f 6d 6e 69 6b 33 30 30 30 74 6c
  o  m  n  I  k  3  0  0  0  t  l
 00  
 4f 6d 6e 69 6b 2d 53 6f 6c 61 72 20 20 20 20 20 4e 4c 44 4e 33 30 32 30 31 33 36 52 31 30 35 37 33 36 30 30 20 20 56 20 20 30 30 30 30 33 2d 30 36
  O  m  n  i  k  -  S  o  l  a  r                 N  L  D  N  3  0  2  0  1  3  6  R  1  0  5  7  3  6  0  0        V        0  0  0  0  3  -  0  6
 14 8a


Logger ==> Inverter: request metrics
 3a 3a 01 00 00 01 11 10 00 00 97
Logger <== Inverter:
 3a 3a - header
 00 01 - src (00xx - Inverter, xx00 - MCU)
 01 00 - dst  ^^^^^^^^^^^^^^^^^^^^^^^^^^^
 11 - control code (10 - register, 11 - read, 12 - write, 13 - execute)
 90 - function code
 42 - data size (66)
 --
 01 24 - Temp: (29.2)
 0d 33 - PV1 V: (337.9)
 0d 35 - PV2 V: (338.1)
 ff ff - PV3 V: 
 00 08 - PV1 I: (0.8)
 00 08 - PV2 I: (0.8)
 ff ff - PV3 I:
 00 14 - I1: (2.0)
 ff ff - I2:
 ff ff - I3:
 09 5f - V1: (239,9)
 ff ff - V2:
 ff ff - V3:
 13 8c - F1: (50.04)
 01 e9 - P1: (489)
 ff ff - F2:
 ff ff - P2:
 ff ff - F3:
 ff ff - P3:
 00 e0 - E Today (2.24)
 00 03 e4 00 - E Total (25497.6)
 00 00 99 e7 - H Total (39399)
 00 01 mode (79)
 00 00 GVFaultValue (81)
 00 00 GFFaultValue (83)
 ff ff GZFaultValue (85)
 00 00 TmpFaultValue (87)
 00 00 PVFaultValue (89)
 00 00 GFCIFaultValue (91)
 00 00 00 00 errorMsg (93) 
 --
 1d 47 - checksum (sum of all above ^^)


== Maintenance phase

Logger ==> Inverter: ping (every 30sec)
 3a 3a 01 00 00 01 11 43 00 00 ca
Logger <== Inverter:
 3a 3a 00 01 01 00 11 c3 01 00 01 4b
```
