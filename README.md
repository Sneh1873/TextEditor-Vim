# TextEditor

This editor is a simple text editor implemented in C++.

## Table of Contents

- [Introduction](#introduction)
- [Project Structure](#project-structure)
- [Compilation](#compilation)
- [Usage](#usage)
- [Contributing](#contributing)

## Introduction

This project implements a basic text editor in C++. The editor follows the Model-View-Controller (MVC) design pattern, and it includes various components such as commands, observers, and a text view implementation.

## Project Structure

- `.gitignore`: Specifies files and directories to be ignored by Git.
- `ECCommand.cpp` and `ECCommand.h`: Implementation and header files for editor commands.
- `ECConcreteObserver.cpp` and `ECConcreteObserver.h`: Implementation and header files for concrete observers.
- `ECEditorTest.cpp`: Test file for the editor.
- `ECMVC.cpp` and `ECMVC.h`: Implementation and header files for the MVC architecture.
- `ECObserver.h`: Header file for the observer interface.
- `ECTextViewImp.cpp` and `ECTextViewImp.h`: Implementation and header files for the text view.
- `Makefile`: Makefile for compiling the project.
- `README.md`: Documentation file (you are here).

## Compilation

Compile the project using the provided Makefile. Open a terminal and navigate to the project directory:

```bash
$ make
```
## Usage

Run the editor by providing the name of the file you want to edit:
```bash
$ ./edit [file_name]
```
For Example,
```bash
$ ./edit sample.txt
```

## Contributing
If you'd like to contribute to the project, please follow these steps:

1. Fork the repository.
2. Create a new branch.
3. Make your changes.
4. Submit a pull request.


