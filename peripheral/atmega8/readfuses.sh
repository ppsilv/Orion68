#!/bin/bash


avrdude -c usbasp -P /dev/ttyUSB1 -p m8 -F -U lfuse:r:-:h -U hfuse:r:-:h
