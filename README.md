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

- **Label (Bits 0-8):** Identifies the type of information carried by the word.
- **SDI (Bits 9-10):** Identifies the source or destination of the data. Not fully implemented on this project.
- **Data (Bits 11-29):** Contains the data.
- **SSM (Bits 30-31):** Indicates sign/status of transmitted data. Only status portion is implemented on this project.
- **Parity (Bit 32):** Provides odd parity for single-bit error detection.

The project represents an ARINC 429 word using a 32-bit unsigned integer.

### MIL-STD-1553 Command Word Structure
MIL-STD-1553 command words consist of 16 bits. A command word is transmitted by the Bus Controller (BC) and contains the information required to address a Remote Terminal (RT) and define the requested operation.

- **Word Count (Bits 0-4):** Specifies the number of data words involved in the transfer.
- **Subaddress (Bits 5-9):** Identifies the subaddress within the selected Remote Terminal.
 - **T/R (Bit 10):** Defines the direction of data transfer between Flight Computer and Remote Terminals. 0 indicates Receive and 1 indicates Transmit.
- **RT Address (Bits 11-15):** Identifies the Remote Terminal addressed by the command.

The project represents a command word using a 16-bit unsigned integer.

The fields are constructed using bitwise operations:
```
uint16_t cmd = 0;

cmd |= (rtAddress << 11);
cmd |= (transmit << 10);
cmd |= (subAddress << 5);
cmd |= wordCount;
```

### MIL-STD-1553 Data Word Structure
MIL-STD-1553 data words consist of 16 bits. Data words carry the actual information exchanged between the Bus Controller (BC) and Remote Terminals (RT).

- **Data (Bits 0-15):** Contains the information transmitted between the Bus Controller and the Remote Terminal.

The project represents a data word using a 16-bit unsigned integer.

Multiple data words can be transmitted within a single MIL-STD-1553 message according to the Word Count specified in the command word.

### MIL-STD-1553 Status Word Structure
MIL-STD-1553 status words consist of 16 bits. This project uses 7 of these bits.

- **Busy (Bit 5):** Indicates that the Remote Terminal is unable to perform the requested operation. In this project, the Busy flag is implemented at Bit 9.
- **Message Error (Bit 10):** Indicates that an error was detected in the received message.
- **RT Address (Bits 11-15):** Identifies the Remote Terminal transmitting the status word.

The project represents a status word using a 16-bit unsigned integer.

## Communication Flow
The system consists of two communication layers: MIL-STD-1553 and ARINC 429. The Flight Computer acts as the interface between these two layers.

### ARINC 429 Communication
The simulated sensors (GPS, ADC, IRS, Radio Altimeter) transmit ARINC 429 words to the Flight Computer using UDP communication.

Each sensor generates ARINC 429 words containing the corresponding sensor data. The Flight Computer receives and processes these words before making the data available to the MIL-STD-1553 side of the system.

### MIL-STD-1553 Communication
The Flight Computer communicates with Remote Terminals through the simulated MIL-STD-1553 bus.

The Bus Controller(Flight Computer) initiates communication by transmitting a Command Word to the addressed Remote Terminal. Depending on the command, one or more Data Words may follow. The Remote Terminal then responds with a Status Word.

The Bus Monitor listens to the simulated bus traffic and displays the exchanged messages for monitoring and debugging purposes.

## Software Architecture
| Application | Description |
| --- | --- |
| FlightComputer | Acts as the interface between the ARINC 429 and MIL-STD-1553 communication systems. |
| BUS | Simulates the MIL-STD-1553 bus and manages communication between the Flight Computer and Remote Terminals. |
| RemoteTerminal | Simulates a MIL-STD-1553 Remote Terminal. Four instances are used to represent the Flight Display, Navigation Display, Autopilot, and Mission Computer. |
| BusMonitor | Monitors and displays traffic on the simulated MIL-STD-1553 bus. |
| GPS | Simulates a GPS sensor and transmits navigation data to the Flight Computer through ARINC 429. |
| ADC | Simulates an Air Data Computer and transmits air data to the Flight Computer through ARINC 429. |
| IRS | Simulates an Inertial Reference System and transmits inertial data to the Flight Computer through ARINC 429. |
| Radio | Simulates a Radio Altimeter and transmits altitude data to the Flight Computer through ARINC 429. |
| Display | Displays data received from the Flight Computer. |
| Launcher | Starts the simulation components and arranges their application windows. |


