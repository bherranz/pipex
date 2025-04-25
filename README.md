# pipex

## 📚 Table of Contents

- [📄Description](#-description)
- [⚙️Usage](#-usage)
- [📌Examples](#-examples)
- [🛠️Compilation](#-compilation)
- [🔥Bonus](#-bonus)
  - [🔁Multiple Pipes](#-multiple-pipes)
  - [🔂Support for `<<` and `>>`](#-support-for--and-)
- [⚠️Error Handling](#-error-handling)

---

## 📄 Description

**pipex** is a small recreation of shell piping behavior.

It simulates the following shell command:

```bash
< file1 cmd1 | cmd2 > file2
````

The program executes cmd1 with file1 as input, passes its output to cmd2, and writes the final result into file2.

This project is an introduction to process creation, redirection, pipes, and command execution using system calls like fork, execve, and pipe.

## ⚙️ Usage
````bash
./pipex file1 cmd1 cmd2 file2
````
file1: input file

cmd1: first command with its arguments

cmd2: second command with its arguments

file2: output file

## 📌 Examples
````bash
./pipex infile "ls -l" "wc -l" outfile
````
Equivalent to:

````bash
< infile ls -l | wc -l > outfile
````
---
````bash
./pipex infile "grep hello" "wc -w" outfile
````
Equivalent to:

````bash
< infile grep hello | wc -w > outfile
````

## 🛠️ Compilation
To compile the project, simply run:

````bash
make
````

To clean object files:

````bash
make clean
````
To clean all compiled files and the executable:

````bash
make fclean
````

## 🔥 Bonus
The bonus part extends the original functionality in two major ways:

### 🔁 Multiple Pipes
````bash
./pipex file1 cmd1 cmd2 cmd3 ... cmdN file2
````
This simulates:

````bash
< file1 cmd1 | cmd2 | cmd3 | ... | cmdN > file2
````
The program dynamically creates a pipeline for any number of commands, allocating the correct number of pipes and managing file descriptors accordingly.

This requires careful handling of child processes and redirection, ensuring each command reads from the output of the previous one and writes to the input of the next.

### 🔂 Support for << and >>
The bonus also includes support for:

<< (here-document): allows passing input directly in the terminal until a delimiter is found.

>> (append mode): allows appending the output to a file instead of overwriting it.

Here-document example:

````bash
./pipex here_doc LIMITER cmd1 cmd2 file
````
Equivalent to:

````bash
<< LIMITER cmd1 | cmd2 >> file
````


In this mode:

- The program reads from standard input until it encounters LIMITER.

- The result is piped through the given commands.

- The final output is appended to the given file.

- This feature closely mimics the shell behavior for scripting and interactive input.

## ⚠️ Error Handling
The program includes robust error checking and handles:

- Invalid files or commands

- Wrong number of arguments

- Permission errors

- Execution failures

- Errors are displayed using perror() to help debug issues, just like the shell would.
