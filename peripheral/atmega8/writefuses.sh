#!/bin/bash

avrdude -c usbasp -P /dev/ttyUSB1 -p m8  -U lfuse:w:0xa4:m -U hfuse:w:0xc7:m
