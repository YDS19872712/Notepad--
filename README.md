# Notepad--

A simple Scintilla-based Notepad implementation.
Only UTF-8 encoding is supported.

Solution consists of three projects:
+ Core library
+ Scintilla component
+ User interface

## Build

Microsoft Visual Studio 2010 is the minimal requirement because
Notepad-- utilizes some features of C++ 11 standard.

* Open the solution
* Main menu > Build > **Configuration Manager...**
* In the open dialog window set **Active solution configuration** to **Release**
* Main menu > Build > **Build Solution**

After the build is complete, the executable file will be
located in `SolutionDir\Build\Release`

## Libraries

* **WTL** is a thin wrapper over WinAPI for providing a user interface.
  The library is header-only and requires no separate build.
* **Scintilla** full-featured text editing component.
  It is capable of adding new text at the end of currently edited buffer,
  so that a user can start working with the file long before it was
  totally loaded.
