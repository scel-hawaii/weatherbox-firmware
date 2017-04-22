#include "Vtest.h"
#include "verilated.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv, char **env) {
    pid_t pid = fork();
    if(pid == 0){
        system("cd ../../ && python run_emu.py ga_stub");
    }
    else{
        Verilated::commandArgs(argc, argv);
        Vtest* top = new Vtest;
        while (!Verilated::gotFinish()) {
            top->eval();
        }
        delete top;
        waitpid( pid, NULL, 0 );
        exit(0);
    }
}
