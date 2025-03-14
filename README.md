# Custom C Shell Implementation
## Introduction

This project involves developing a custom shell in C, implementing essential features such as command execution, input/output redirection, piping, and background processes. The shell supports multiple commands separated by semicolons or ampersands, handles foreground and background processes, and includes a history log feature. It also supports custom shell configuration files for aliases and functions.

## Key Functionalities

- **Command Execution:** Supports running system commands like `emacs`, `gedit`, etc.
- **Input/Output Redirection:** Handles redirection operators (`>`, `>>`, `<`) for file input/output.
- **Piping:** Allows passing output from one command as input to another.
- **Background Processes:** Manages background processes with `&`, displaying process IDs and exit statuses.
- **History Log:** Stores and displays command history across sessions.
- **Custom Configuration:** Supports custom shell configuration files for aliases and functions.
- **System Commands:** Executes other system commands present in Bash, both in foreground and background processes.
- **Signal Handling:** Handles signals like SIGINT, SIGTERM, and SIGTSTP for process management.
- **Process Management:** Provides commands like `fg`, `bg`, and `activities` to manage running processes.

### Specific Commands

- **hop:** Changes the directory that the shell is currently in, supporting `.` , `..`, `~`, and `-` flags.
- **reveal:** Lists all files and directories in the specified directories in lexicographic order, supporting `-a` and `-l` flags.
- **log:** Displays command history, storing up to 15 commands by default.
- **proclore:** Obtains information regarding a process, including pid, process status, process group, virtual memory, and executable path.
- **seek:** Searches for a file/directory in the specified target directory, supporting flags like `-d`, `-f`, and `-e`.
- **iMan:** Fetches man pages from the internet using sockets.

## Setup Instructions

### Prerequisites

- **Language:** C
- **Environment:** Linux or similar Unix-based systems
- **Tools:** GCC compiler

### Steps to Run

1. **Clone the Repository:**

2. **Navigate to the Project Directory:**

3. **Compile the Code:**
Use the provided Makefile to compile the code. If you don't have a Makefile, you can compile each component separately using GCC.


### Example Usage

- **Command Execution:** Run system commands like `echo "Hello World"`.
- **Input/Output Redirection:** Use redirection operators like `echo "Hello" > file.txt`.
- **Piping:** Use pipes like `echo "Hello" | wc`.
- **Background Processes:** Run commands in the background with `&`, e.g., `sleep 10 &`.

## Technical Skills

- **Programming Language:** C
- **System Calls:** POSIX C libraries for system operations
- **Input/Output Management:** Handling redirection and piping
- **Process Management:** Foreground and background process handling
- **Concurrency:** Managing concurrent commands
- **Configuration Management:** Custom shell configuration files
- **Error Handling:** Custom error messages for invalid commands or operations
- **Signal Handling:** Managing signals for process control

## Broader Skills

- **Operating Systems**
- **System Programming**
- **Concurrency**
- **Threading**
- **Data Structures**
- **System Design**

### Additional Notes
- Use `Ctrl + D` to exit the shell.
- Use `Ctrl + C` to interrupt foreground processes.
- Use `Ctrl + Z` to push foreground processes to the background.
