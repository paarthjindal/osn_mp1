[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Qiz9msrr)
# mini-project-1-template

in specification no 8 for seek 
- instead of printing the relative path , i am printing the full path as according to me its almost the same thing

#  specification 1
Functions
prompt(char *home_dir): Displays the shell prompt with the username, system name, and current working directory. The directory is shown as ~ if it is the home directory, and the path is color-coded for easy identification.

assumptions
used getlogin function to get the username 

#  specification 2
Functions
execute_terminal(char *s, queue *q, int *flag, char *home_dir, char *prev_dir): Parses and executes the command entered by the user. Supports multiple commands like hop, reveal, proclore, log, and seek. If the command is not recognized, it attempts to execute it as a system command.

write_queue_to_file(queue *q, const char *filename, const char *home_dir): Writes the contents of the circular queue (command history) to a specified file in the home directory.(newfile.txt)

read_queue_from_file(queue *q, const char *filename, const char *home_dir): Reads commands from a specified file in the home directory(newfile.txt) and populates the circular queue with this command history.
assumptions
no assumption i tried to do each and every possible given thing but yeah there are some innecessayy things printing in between sometimes , but not always

#  specification 3
Functions

resolve_path(char *input, char *home_dir, char *prev_dir):

This function takes a directory path as input and resolves it to an absolute path by handling special symbols like ~ (home directory), - (previous directory), . (current directory), and .. (parent directory).
It returns the resolved absolute path as a string.
hop(char *input, char *home_dir, char *prev_dir):

This function changes the current working directory based on the input path. It uses resolve_path to determine the absolute path.
It checks if the directory exists and attempts to change to the specified directory using chdir.
It updates the prev_dir variable to store the previous directory path and sets the PWD environment variable to the new path.
Finally, it prints the new current directory in red color.


assumptions
i properly calculated multiple commands in hop
after implementing i am printing full directory in which we are currently in 


#  specification 4
Functions

resolve_path_reveal(char *input, char *home_dir, char *prev_dir):

This function resolves the provided path to an absolute path, considering various special symbols:
~: Home directory
.: Current directory
-: Previous directory
/: Absolute path
For other cases, it considers the path as a relative path to the current directory.
It returns the resolved path as a string.
print_file_details(const char *path, const char *name):

This function prints detailed file information for the reveal command when the -l flag is used:
It includes the file's permissions, number of links, owner, group, size, and last modification time.
The file name is color-coded: directories are blue, executables are green, and regular files are white.
It uses stat to gather file metadata and formats the output according to Unix-style long listing.
reveal(const char *path, int show_all, int long_format):

This function implements the main functionality of the reveal command:
It opens the directory at the specified path using opendir.
It reads directory contents using readdir, skipping hidden files if show_all is not set.
It stores the file names in an array and sorts them lexicographically using qsort.
If the -l flag is set (long_format), it calls print_file_details to print detailed information about each file. Otherwise, it simply prints the file names.
It frees the allocated memory after use.

assumptions

#  specification 5
Functions

create_queue(queue *q):

Allocates memory for a new queue structure and its internal log storage.
Initializes the front and rear indices to -1 and the size to 0.
Returns a pointer to the created queue.
enqueue(queue *q, char *command):

Adds a command to the queue while ensuring no duplicate commands are added.
If the queue is full (size equals 15), it overwrites the oldest command.
Handles wrapping around the indices using modulo operation.
dequeue(queue *q):

Removes the oldest command from the queue if it's not empty.
Updates the front index and decrements the size.
display_log(queue *q):

Prints all commands in the queue.
Wraps around if needed to ensure all commands are displayed.
execute(queue *q, int index):

Forks a new process to execute a command at the specified index.
However, the function currently does not execute the command as the child process code is missing.
purge(queue *q):

Resets the queue to its initial empty state by setting front, rear, and size to 0.
.

assumptions
i have made a circular queeue

#  specification 6
Functions


assumptions
i have implemented both background and foreground processes in the main file
assumed that background process will not be applied command made by me

also i assumed that background process can end irrespective of the result of whether the user is entering any input or not

#  specification 7
Functions

describe_process
Prints detailed information about a process with the given PID. It retrieves and displays the process status, group ID, virtual memory size, and executable path. It also determines if the process is in the foreground or background based on the foreground process group ID

assumptions
i assumed the process to be runnign on foreground by comparing process_goup==foreground_group



#  specification 8
Functions

resolve_path_seek
Resolves the input path based on special cases (~, -, ., .., etc.) and returns the absolute path. Handles the user's home directory and previous directory as well as relative and absolute paths.

print_with_color
Prints the given path and name with color coding: blue for directories and green for files. The path is prefixed to the name for complete clarity.

search_directory
Recursively searches the specified directory for files or directories matching the seek_name. It prints matches and counts occurrences based on the flags provided (a, b, c) which indicate directory, file, or both.

seek
Searches for files or directories based on the seek_name in the specified resolved path. If c is set to 1 and exactly one match is found, it attempts to change directory or read the file based on its type and permissions. It handles errors for invalid flags and permissions..

assumptions
i am not printing relative path instead printing full path as i find it better looking and for better understanding 



- in specification activities
i am assuming that if a proccess is completed , then it will be showing exited when we type the command activities , but this will happen when i am using fg command and bringing background process to foreground process , cause it wont be decremented in my back_process_list
so i am taking the above assumption

- i am assuming that hop .. | wc wont change the directory just like a normal terminal works

- i am assumign that when i am doing i-o redirection for background process , the line background process started : pid wont be shown in the terminal , instead it will be shown in the file 

echo hi >mehul.txt & | wc
Invalid use of pipe
<paarth@Paarthjindal:~> Process 'echo hi ' with PID 1489010 ended normally with exit status 0

<paarth@Paarthjindal:~> cat mehul.txt
Background process started: PID 1489010
hi
Chat gpt ai link 
https://chatgpt.com/c/97fa0cdb-2968-41bc-924e-22156c1bcb73


https://chatgpt.com/c/66e0ae49-dea8-8007-baac-68b52fc355b1