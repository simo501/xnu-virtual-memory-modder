# XNU Virtual Memory Modder
Due to the lack of documentation about the _mach_ library, I decided to share this project on Github, in the hope that would be useful for someone working with same libraries

**This is not already finished**
# Run

  

Run the project is pretty simple, you just need three arguments

  

```./main (pid) (virtual mem addr) [r/w]```

  

If you need to get some view about the virtual memory of a **pid** you can use the ```vmmap``` command

# Compile

Just clone the repository with git clone ->  ```cd repo_name && cd src``` -> ```clang main.c -o main``` 

# Useful links

  

[vm_read](http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/vm_read.html) from MIT
