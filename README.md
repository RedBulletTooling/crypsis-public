This project has been created and developed by Matthijs Vogel and Luc Pluimakers.

# Contents of repo
This repo contains all hardware and software files to recreate Crypsis.

Detailed documenatation can be found on our gitbook link here: https://redbullettooling.gitbook.io/keyvilboard-apt/

# Motivation
We would hate to see the knowledge that we build up gone to waste, given we decided to halt this product. This product has not been as commercially viable as we had hoped. But we have put time, effort and a decent chunk of money into this project to create something that hasn't been replicated since. Our goal with publicizing this project is that goal of it's creation will be reached. To prevent harms to those who can't protect themselves. May that be terrorism, child exploitation or other crimes. If you work for an organisation that has these same goals and have a need, contact us. We are happy to help.

# What is Crypsis
Crypsis is a hardware keylogger that communicates using a NFR9160 module. By doing so we can do very advanced methods of communicating covertly, aswell as propper implementation of encryption and other security measures. Apart from the hardware the server is dockerized, meaning it's modular and highly adjustable.

Copied from our website (menura.org);

Hardware
- Every chip, fully programmable, fully open-source. 4G (NB-iot/LTE-M) with encrypted TLS/SSL communication and authentication.
- Secure modem with the latest anti-tamper measurements.
- Only communicate, when you want it. True silent mode, undectectable on the RF spectrum.
- Remotely disable the keyboard for damage prevention on entry.

Software
- Fully dockerized server setup, fully open-source. Setup a standalone Crypsis C2 in under minutes.
- Customize and integrate the API to your needs.
- Everything encrypted, with integrated TLS/SSL based authentication.
- Server infrastructure is modular. The direct communication server can be placed in the target country, whilst all sensitive datastorage is kept within your own borders.

USB
- Crypsis device that sits between the PC USB and the USB cable of the keyboard.

Embedded
- Crypsis device that is integrated in the keyboard of choice with soldering. Keep in mind you have to be directly on the USB bus of the keyboard.

# Disclaimer

The code was still heavily under development and under testing (alpha), before deciding to stall the project. There are serious security issues present in this repository. Think, weak default passwords or injection vulnerabilities. Expect potential issues / conflicts.

Apache2 license applies.
