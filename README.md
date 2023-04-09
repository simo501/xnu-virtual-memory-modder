
  

# XNU Virtual Memory Modder

  **this code is not good, one day i'll work again on this** 

Due to the lack of documentation about the _mach_ library, I decided to share this project on Github, in the hope that would be useful for someone working with same libraries

  

  

**This is not already finished**

  

# Run

  
  

Run the project is pretty simple, you just need three arguments

  
  

```sudo ./main (pid) (virtual mem addr) [r/w]```

  
  

If you need to get some view about the virtual memory of a **pid** you can use the ```vmmap``` command

  

# Compile

  

  

Just clone the repository with git clone -> ```cd repo_name && cd src``` -> ```clang main.c -o main```

  
# Overcome OS/Kern failure


### SIP (System Integrity Protection)

  

The purpose of the _System Integrity Protection_ in three points:

  

- Protection of contents and file-system permissions of system files and directories;

- ```Protection of processes against code injection, runtime attachment (like debugging) and DTrace;```

  

- Protection against unsigned kernel extensions ("kexts").

  

The highlighted one is of our interest, because due to this we would not be able to

attach the program to another that is signed by the developer.

  

A workaround is to disable the System Integrity Protection by typing into the terminal in _recovery mode:_

```csrutil disable```

  

**Of course, this expose MacOS to a large amount of vulnerabilities so it's in your interest to enable again this option after the usage of the program**

### Unsign the executable  

There is another way to make this work without disabling the _SIP_
This consist in removing the signature from an application. This method **can be very dangerous** because could generates broken exec.

```codesign --remove-signature ~/app/path/```

# Useful links

  

[vm_read](http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/vm_read.html) from MIT
