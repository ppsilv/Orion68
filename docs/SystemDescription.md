# Orion68

**Orion68** is a modular hobby computer based on the Motorola 68000 microprocessor.
The project was created as a platform for learning, experimentation, and exploring classic computer architecture using discrete logic and modular hardware design.

The system uses a backplane architecture with a 96-pin system bus that allows multiple expansion boards. Each board implements a specific subsystem such as CPU, memory, serial I/O, or experimental peripherals.

Orion68 is designed in the spirit of classic modular computer systems from the 1980s, prioritizing simplicity, transparency of design, and ease of experimentation.

---

## Features

* Motorola 68000 CPU
* Modular backplane architecture
* 96-pin system bus
* Up to 9 expansion boards
* Discrete logic design
* Multiple serial interfaces
* Experimental video support
* Designed for learning and hardware experimentation

---

## Hardware Overview

Current system configuration:

| Component        | Description                                                |
| ---------------- | ---------------------------------------------------------- |
| CPU              | Motorola 68000                                             |
| ROM              | 16 KB (planned expansion to 64 KB)                         |
| SRAM             | 1 MB (planned expansion to 2–3 MB)                         |
| Serial           | Quad UART (4 serial ports)                                 |
| Address decoding | GAL + 74HC138                                              |
| Bus              | 96-pin backplane                                           |
| Expansion        | Up to 5 boards (3 cm spacing) or 9 boards (1.5 cm spacing) |

---

## System Architecture

The Orion68 uses a memory-mapped architecture typical of 68000 systems.
All peripherals and memory devices are accessed through the processor address space.

Address decoding is implemented using GAL devices and standard 74HC logic.

---

## Boards

The system is divided into modular boards connected through the backplane.

Typical board types include:

* **CPU Board** – Motorola 68000 processor and clock logic
* **Memory Board** – ROM and SRAM
* **Serial I/O Board** – quad UART providing four serial ports
* **Video Interface Board** – experimental video system using Raspberry Pi Pico
* **Backplane** – passive bus connecting all boards

---

## Current Status

The first prototype of the Orion68 system has been successfully built and tested.

The current hardware revision introduces:

* a redesigned modular backplane
* improved expansion capability
* increased memory capacity
* additional peripheral interfaces

Development is ongoing and the system continues to evolve.

---

## Project Goals

* Explore the Motorola 68000 architecture
* Build a modular retro-style computer
* Learn about hardware design using discrete logic
* Create a flexible experimentation platform
* Share the design as an open hardware project

---

## Repository Contents

This repository contains:

* Schematics
* PCB layouts
* Firmware and monitor software
* Hardware documentation
* System architecture notes

---

## License

This project is released as open hardware.
See the LICENSE file for details.

---

Orion68 is a personal hobby project focused on learning, experimentation, and the enjoyment of building computers from the ground up.
