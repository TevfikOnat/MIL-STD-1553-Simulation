# Flight Computer Simulator
A software-based simulation of a MIL-STD-1553 and ARINC 429 communication system developed for testing and experimentation.

## Project Overview
This project simulates a flight computer system using MIL-STD-1553 and ARINC 429 communication protocols. The project consists multiple independent applications. MIL-STD-1553 side consists of 4 Remote Terminals, Bus Monitor and BUS. ARINC 429 side consists of GPS, ADC(Aircraft Data Interface), IRS(Inertial Reference System) and Radio Altimeter. 

The two sides are connected through the Flight Computer application.

The applications communicate with each other using UDP communication.

## System Architecture

<img width="1200" alt="Flight Computer Simulator System Architecture" src="https://github.com/user-attachments/assets/99b950ef-a354-4203-966c-364ab9db62ff" />

- **GPS, ADC, IRS and Radio Altimeter** simulate physical sensors, transmitting data to the Flight Computer through ARINC 429 communication.
- **Remote Terminals** simulate Flight Display, Navigation Display, Autopilot and Mission Computer. Each terminal receives Command and Data Words and then sends a Status Word.  
- **BUS** simulates Channel A of MIL-STD-1553 bus and connects all Remote Terminals.
- **Flight Computer** collects data through ARINC 429 communication and distributes it to the Remote Terminals through MIL-STD-1553 communication.

### ARINC 429 Word Structure
ARINC 429 words consist of 32 bits.

-**Label (Bits 0-8):** Identifies the type of information carried by the word.
-**SDI (Bits 9-10):** Identifies the source or destination of the data. Not fully implemented on this project.
-**Data (Bits 11-29):** Contains the data.
-**SSM (Bits 30-31):** Indicates sign/status of transmitted data. Only status portion is implemented on this project.
-**Parity (Bit 32):** Provides odd parity for single-bit error detection.

The project represents an ARINC 429 word using a 32-bit unsigned integer.

### MIL-STD-1553 Command Word Structure
MIL-STD-1553 command words consist of 16 bits. A command word is transmitted by the Bus Controller (BC) and contains the information required to address a Remote Terminal (RT) and define the requested operation.

-**Word Count (Bits 0-4):** Specifies the number of data words involved in the transfer.
-**Subaddress (Bits 5-9):** Identifies the subaddress within the selected Remote Terminal.
 -**T/R (Bit 10):** Defines the direction of data transfer between Flight Computer and Remote Terminals. 0 indicates Receive and 1 indicates Transmit.
-**RT Address (Bits 11-15):** Identifies the Remote Terminal addressed by the command.

The project represents a command word using a 16-bit unsigned integer.

The fields are constructed using bitwise operations:

uint16_t cmd = 0;
cmd |= (rtAddress << 11);
cmd |= (transmit << 10);
cmd |= (subAddress << 5);
cmd |= wordCount;

### MIL-STD-1553 Data Word Structure
MIL-STD-1553 data words consist of 16 bits. Data words carry the actual information exchanged between the Bus Controller (BC) and Remote Terminals (RT).

-**Data (Bits 0-15):** Contains the information transmitted between the Bus Controller and the Remote Terminal.

The project represents a data word using a 16-bit unsigned integer.

Multiple data words can be transmitted within a single MIL-STD-1553 message according to the Word Count specified in the command word.
