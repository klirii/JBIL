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
# Requirements
x64 Windows 7, 8, 8.1, 10, 11
# Including the library in a Visual Studio project
Visual Studio 2022 will be used as an example
1. Clone the repository to a directory you are comfortable with
2. Go to [releases](https://github.com/klirii/JBIL/releases "JBIL releases") and download the latest available version of the static libraries
3. Extract the archive to the directory where you cloned the repository
4. Open your project in Visual Studio, right-click on the project in the Solution Explorer and select "Properties"
5. In the "Configuration Properties" section, select "All Configurations" from the "Configuration" dropdown menu
6. In the "VC++ Directories" section, select "Include Directories", click on the dropdown menu, click "Edit" and add **(path to cloned repository)/include** there and click "OK"
7. In the same section, select "Library Directories" and add there the path to the static library folder for the desired configuration
8. In the "Linker" section, open the "Input" subsection and add JBIL.lib to "Additional Dependencies"
9. Done!
# Notes
You can find more information about [classfile structure](https://docs.oracle.com/javase/specs/jvms/se17/html/jvms-4.html "Chapter 4. The class File Format") and [bytecode](https://docs.oracle.com/javase/specs/jvms/se17/html/jvms-6.html "Chapter 6. The Java Virtual Machine Instruction Set") on the Oracle website.
