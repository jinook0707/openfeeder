# openfeeder
Open source feeder for animal experiments

Before building this feeder, please read a paper `Jinook Oh, Riccardo Hofer and W. Tecumseh Fitch (in prep) Open source automatic feeder for animal experiments'.

Contact info :  
jinook.oh@univie.ac.at  
riccardo.hofer@univie.ac.at  
tecumseh.fitch@univie.ac.at  

---

After building the feeder and uploading Arduino code, you can test the feeder in one of the following ways.<br>
1) Click 'Serial Monitor' button on the upper right corner of the Arduino app, a serial monitor window will be opened,<br>
   then type 'feed' in the upper text box of the new window.<br>
2) Download 'test.py' file and run it with Python ('pyserial', https://pypi.python.org/pypi/pyserial, is required),<br>
   then click 'Feed' button.

---

A geared DC motor has been also used to increase torque and reduce motor noise.

Arduino code in 'withDCMotor' folder has been used to run the feeder with a geared DC motor + L298N motor driver, instead of a stepper motor, described in the original paper.

---

Copyright CogBio Department in University of Vienna 2016.<br>
In this repository, the software is licesed under the GNU GPL Version 3 and <br>
the hardware is licensed under the CERN OHL v. 1.2 (http://ohwr.org/cernohl).<br>
This documentation is distributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A PARTICULAR PURPOSE.<br>
Please see LICENSE and LICENSE_HARDWARE.pdf for detailed information.
