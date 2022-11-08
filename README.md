# Introduction

This repository focuses on creating shared libraries of the MapReduce project. The libraries would then
be used within another project via explicit linking.

## FileProcessor

This folder will hold the following - 

1) FileProcessorBase.hpp

    Abstract implementation of the FileProcessor class. This class has two functions - 

       i) Create a memory object that represents all text files within a directory
    
       ii) Facilitate the loading of intermediate memory objects (Mapper, Reducer, Shuffler) to disk


2) FileProcessorInput.cpp
    The actual implementation that will load all raw data in a directory as a memory object. This is done by the runOperations virtual method.

3) FileProcessorMapOutput.cpp
   The actual implementation that will write the Mapper memory object to disk. This is done by the runOperations virtual method.

4) FileProcessorShufOutput.cpp
   The actual implementation that will write the Shuffler memory object to disk. This is done by the runOperations virtual method.

4) FileProcessorRedOutput.cpp
   The actual implementation that will write the Reducer memory object to disk. This is done by the runOperations virtual method.


### Building

The commands to build each implementation as a library are as follows - 

### FileProcessorInput.cpp

Let's start by cd'ing to FileProcessor directory and creating the object file by running the g++ command

      cd FileProcessor/
      g++ -std=c++17 -c -o FileProcessorInput.o FileProcessorInput.cpp -fPIC

Now we will create a shared library using gcc

      gcc -shared -o FileProcessorInput.so FileProcessorInput.o

You will see a file `FileProcessorInput.so` created. We will move it to the libs folder.

      mv FileProcessorInput.so libs/

Let's remove the object file

      rm -f FileProcessorInput.o


### FileProcessorMapOutput.cpp

Let's start by cd'ing to FileProcessor directory and creating the object file by running the g++ command

      cd FileProcessor/
      g++ -std=c++17 -c -o FileProcessorMapOutput.o FileProcessorMapOutput.cpp -fPIC

Now we will create a shared library using gcc

      gcc -shared -o FileProcessorMapOutput.so FileProcessorMapOutput.o

You will see a file `FileProcessorMapOutput.so` created. We will move it to the libs folder.

      mv FileProcessorMapOutput.so libs/

Let's remove the object file

      rm -f FileProcessorMapOutput.o


### FileProcessorShufOutput.cpp

Let's start by cd'ing to FileProcessor directory and creating the object file by running the g++ command

      cd FileProcessor/
      g++ -std=c++17 -c -o FileProcessorShufOutput.o FileProcessorShufOutput.cpp -fPIC

Now we will create a shared library using gcc

      gcc -shared -o FileProcessorShufOutput.so FileProcessorShufOutput.o

You will see a file `FileProcessorShufOutput.so` created. We will move it to the libs folder.

      mv FileProcessorShufOutput.so libs/

Let's remove the object file

      rm -f FileProcessorShufOutput.o


### FileProcessorRedOutput.cpp

Let's start by cd'ing to FileProcessor directory and creating the object file by running the g++ command

      cd FileProcessor/
      g++ -std=c++17 -c -o FileProcessorRedOutput.o FileProcessorRedOutput.cpp -fPIC

Now we will create a shared library using gcc

      gcc -shared -o FileProcessorRedOutput.so FileProcessorRedOutput.o

You will see a file `FileProcessorRedOutput.so` created. We will move it to the libs folder.

      mv FileProcessorRedOutput.so libs/

Let's remove the object file

      rm -f FileProcessorRedOutput.o

### Class Factories

We rely on class factories and the extern keyword to extend the presence of class instances to downstream implementations.

E.g.

      // Class factories - constructor
      extern "C" FileProcessorBase* createInputObj(const std::string &operation, const std::string &path){
      return new FileProcessorInput(operation, path);
      }
      
      // Class factories - destructor
      extern "C" void removeInputObj(FileProcessorBase* fpObj){
      delete fpObj;
      }


We are function pointers (`createInputObj` and `removeInputObj`) that will internally create instances that points to *BASE* implementation.
These class factories are exposed using the extern keyword which specifies these symbols  (`createInputObj` and `removeInputObj`)  have external linkage.

In the base interface, there are type declarations which expose the class factory.

E.g.

      // Signifies the Factory that will read input directories using createInputObj within FileProcessorInput.cpp implementation
      typedef FileProcessorBase* create_t(const std::string &operation, const std::string &path);
      
      // Signify the Factory that will remove the instance within FileProcessInput.cpp implementation
      typedef void destroy_t(FileProcessorBase*);

These type declarations along with symbols (factory) are used to create symbolic links using the DLOPEN api. This is how we accomplish external linking.

