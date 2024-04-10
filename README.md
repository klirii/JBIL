# JBIL – Java Bytecode Instrumentation Library
A simple, small, lightweight library for instrumenting Java bytecode when retransforming classes with jvmti.

# Library can:
- Finding entries in the constant pool
- Add your own entries to the constant pool
- Finding methods
- Change methods bytecode

# Examples
Before you start working with the library, you must include the "ClassFile.hpp" header and initialize the class you want to work with

![image](https://github.com/klirii/JBIL/assets/83013244/c6e2d9f7-3800-4dd2-8fb7-064267ba03aa)  
Further work with the class will be done via cf object
### Finding entries in the constant pool
![image](https://github.com/klirii/JBIL/assets/83013244/4a761652-0c71-48e1-ae9a-2a39a970f9c8)  
All types of constants can be found [here](https://docs.oracle.com/javase/specs/jvms/se17/html/jvms-4.html#jvms-4.4 "The constant pool")
### Adding new entries
![image](https://github.com/klirii/JBIL/assets/83013244/95017bec-8084-422e-b1a3-52f3a3edcd32)
### Finding methods and changing bytecode
Note: The "Bytecodes.hpp" header must be included before changing method bytecode

![image](https://github.com/klirii/JBIL/assets/83013244/e4ed3886-b784-4b11-a168-0f02e8640255)
### Dumping modified class file:
![image](https://github.com/klirii/JBIL/assets/83013244/6cbc4af9-4e94-427b-8e93-eca6f0152adb)  
or

![image](https://github.com/klirii/JBIL/assets/83013244/598e81d3-9b28-4943-b9df-f4a677f92574)
