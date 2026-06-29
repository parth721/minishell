#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<cstdlib>

using namespace std;

string sh_read(){
    string cmd_line;
    getline(cin, cmd_line);
    return cmd_line;
}

vector<string>sh_parse(const string& line){ //dangling pointers issue fixed!!! 
    vector<string> tokens;
    string token;
    istringstream iss(line);

    while(iss >> token){
        tokens.push_back(token);
    }

    return tokens;
}

vector<const char*> make_args(const vector<string>& tokens){
    vector<const char*> args;
    for(auto &t : tokens){
        args.push_back(t.data());  
    }
    args.push_back(nullptr); // execvp requires a null-terminated array of char*

    return args;
}

int sh_cd(const vector<const char*>& args){
    if(args.size() < 2){
        cerr << "psh: cd: missing argument\n";
        return 1; // continue the loop
    }

    if(chdir(args[1]) != 0){
        perror("psh");
    }

    return 1; // continue the loop
}

int sh_help(const vector<const char*>& args){
    cout << "psh: a simple shell implementation\n";
    cout << "Built-in commands:\n";
    cout << "  cd <directory> : change the current directory\n";
    cout << "  help           : display this help message\n";
    cout << "  exit           : exit the shell\n";
    return 1; // continue the loop
}

int sh_nonbuiltin(const vector<const char*>& args){ //parallel execution of processes
    pid_t pid;
    int status;

    pid = fork();
    if(pid == 0){ //child process created
        if(execvp(args[0], const_cast<char* const*>(args.data())) == -1){ //because execvp requires char* const* but we have const char* const* so we need to cast it to char* const*
            perror("psh");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0){ //error forking
        perror("psh");
    }else { //parent process
        do{
            waitpid(pid, &status, WUNTRACED);  //study WUNTRACED flag.
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1; // continue the loop
}

int sh_execute(const vector<const char*>& args){
    if(args.empty()){
        return 1; // empty command, continue the loop
    }

    if(strcmp(args[0], "exit") == 0){
        return 0; // exit the shell
    }else if(strcmp(args[0], "cd") == 0){
        return sh_cd(args);
    }else if(strcmp(args[0], "help") == 0){
        return sh_help(args);
    }

    return sh_nonbuiltin(args);
}


void sh_loop(){
    string line;
    vector<const char*> args;
    int status;

    do{
      cout << "psh> ";
      line = sh_read();
      args = make_args(sh_parse(line));
      status = sh_execute(args);

    }while(status == 1);
}

int main(int argc, char **argv) {
    
    sh_loop();

    return EXIT_SUCCESS;
}


/*
every program has 3 steps in its life cycle
1. initialize 
2. Interpret
3. Terminate

Basic loop of a shell, there are also 3 steps (it falls under the Interpret step of the program life cycle):
1. Read
2. Parse
3. Execute

Now as we delve into the shell. Our code slowly slowly becomes more specific to the shell.
a. we need a fork function to create a child process so that the parent process can continue to run and accept more commands from the user.
b. we need to use execvp function to execute the command that the user has entered.
c. we need to use waitpid function to wait for the child process to finish executing before the parent process continues to accept more commands from the user.
d. we need to use signal function to handle the SIGINT signal (Ctrl+C) so that the shell does not terminate when the user presses Ctrl+C.
e. we need buffer to store the command as we are not sure how long the user-input will be.
f. we need something to parse the command and its arguments so that we can pass them to the execvp function.
g. we need to handle the case when the user enters an empty command or a command that does not exist.
h. while parsing also we need to take care of the buffer size. 
i. one way to handle shell built-in commands (like cd, exit, etc.) is to check for them before calling execvp and handle them separately in the parent process.
 
Let's delve into implementation and be more specific to the shell.
1. int main() going to take argument since shells usually are not interactive. argv & argc to take command line arguments.
2. we need to create a function called sh_loop() which will contain the main loop of the shell. This function will be responsible for reading, parsing, and executing commands.
3. the sh_loop() not going to return anything since it will be an infinite loop until the user exits the shell. It will also not take any arguments since it will be using global variables to store the command and its arguments.
4. sh_loop() will have 3 functions: sh_read(), sh_parse(), and sh_execute().
5. till now we didn't implement loop we just called a function sh_loop() to show our intent. Do-while loop is a good choice.
6. status variable should be a specific to the function not just the loop.
7. we need to free the memory allocated for the command and its arguments after each iteration of the loop to avoid memory leaks.
8. how we going to read the shell command:
    8.1. in C++ we don't need to worry for memory allocation and deallocation.
    8.2. we can use getline() function to read the command from the user and store it in a string variable.
    8.3. we can use the string variable to store the command and its arguments.
9. how we going to parse the shell command:
    9.1. we can use istringstream to parse the command
    9.2. we can use a vector to store the sting arguments.
    9.3. we can use a vector of const char* to store the arguments for execvp function.
    # avoid dangling pointers issue.
    # best practice is to use const char* first then only use non-const char* only when we need to modify the string.
10. how we going to execute the shell command (concurrency):
    10.1. we can use fork() function to create a child process.
    10.2. we can use execvp() function to execute the command in the child process.
    10.3. we can use waitpid() function to wait for the child process to finish executing before the parent process continues to accept more commands from the user.
    10.4. parallel excution of child & parent process.
    10.5. wake up parent when child process is stopped. (WUNTRACED flag)
    10.6. add built-in commands like cd, exit, etc. in the parent process before calling execvp() function.
    10.7. handle the case when the user enters an empty command or a command that does not exist.
11. add support for built-in commands like cd, exit, etc. in the parent process before calling execvp() function.
    11.1. we can use strcmp() function to compare the command with the built-in commands.
    11.2. we can use chdir() wrapper to change directory
    11.3. check the argument 1 for cd command and handle the case when the user does not provide any argument.
    11.4. return 1 to continue the loop after executing the built-in command.
    11.5. only for exit command we will return 0 to exit the shell.
    11.6. we can use perror() function to print the error message when the command does not exist or when the user does not have permission to execute the command.

# we opt with char* instead of string because execvp() requires char* const* as its second argument.

*/