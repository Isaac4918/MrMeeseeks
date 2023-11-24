#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h>

void text_request(int currentLevel);
void math_request(int currentLevel);
void program_request(int currentLevel);
void text_request_aux(int currentLevel, char user_input[256]);

int requests = 0;
int successful_tasks = 0;
int failed_tasks = 0;
int option;

void meeseeks(int pid, int ppid, int level, int instance) {
    printf("Hi I'm Mr Meeseeks! Look at Meeeee. (pid=%d, ppid=%d, N=%d, i=%d)\n", pid, ppid, level, instance);
    printf("Select a type of request:\n");
    printf("1) Text request\n");
    printf("2) Math request\n");
    printf("3) Program request\n");
    printf("4) Exit\n\n");
    printf("Select a type of request:\n");

    int option;
    scanf("%d", &option);

    switch (option) {
        case 1:
            text_request(level);
            break;
        case 2:
            math_request(level);
            break;
        case 3:
            program_request(level);
            break;
        case 4:
            printf("Bye!\n");
            exit(0);
        default:
            printf("Invalid option\n");
            exit(1);
    }
    //sleep(rand() % 5 + 1);  // Simulando el tiempo de ejecución real
}

void assistantMeeseeks(int pid, int ppid, int level, int instance){
    printf("Hi I'm Mr Meeseeks! Look at Meeeee. (pid=%d, ppid=%d, N=%d, i=%d)\n", pid, ppid, level, instance);
}

void text_request(int currentLevel) {
    printf("Tell me your request:\n");
    char user_input[256];
    scanf("%s", user_input);
    printf("Yesss sireee!");
    sleep(0.5);
    text_request_aux(currentLevel, user_input);
}

void text_request_aux(int currentLevel, char user_input[256]) {
    int difficulty;
    printf("How difficult is your request? (0-100) (Submit -1 if you don't know): ");
    scanf("%d", &difficulty);
    if (difficulty == -1) {
        difficulty = rand() % 101;
    }
    else if (difficulty > 80) {
        //create a lot of forks to simulate a hard task and call the assistant meeseeks
        for (int i = 0; i < 30; i++) {
            int pid = fork();
            if (pid == 0) {
                assistantMeeseeks(getpid(), getppid(), currentLevel+1, i);
                exit(0);
            }
            else if (pid > 0) {
                continue;
            }
            else {
                perror("Error creating Meeseeks process");
                exit(EXIT_FAILURE);
            }
        }
        sleep(10);
        printf("I can't do that!\n");
        failed_tasks++;
        exit(1);
    }
    else {
        if (difficulty > 40) {
            int assistantMeeseeksQuantity = rand() % 5 + 1;
            for (int i = 0; i < assistantMeeseeksQuantity; i++) {
                int pid = fork();
                if (pid == 0) {
                    assistantMeeseeks(getpid(), getppid(), currentLevel+1, i);
                    exit(0);
                }
                else if (pid > 0) {
                    continue;
                }
                else {
                    perror("Error creating Meeseeks process");
                    exit(EXIT_FAILURE);
                }
            }
        }
        printf("Working...\n\n");
        //wait a random time between 0.5 and 5 seconds
        sleep(rand() % 5 + 1);
        printf("Done! I have completed your request '%s'\n", user_input);   
    }

}

void math_request(int currentLevel) {
    //ask for a math operation and solves it, it should parse simple operations like 1+2, 3*4, 5/6, 7-8
    printf("Tell me your math request(In this format, example 1+1, 2*4):\n");
    char operation[256];
    char operator;
    int operand1;
    int operand2;
    int result;
    sscanf(operation, "%d%c%d", &operand1, &operator, &operand2);
    printf("Yesss sireee!");

    switch (operator) {
        case '+':
            result = operand1 + operand2;
            break;
        case '-':
            result = operand1 - operand2;
            break;
        case '*':
            result = operand1 * operand2;
            break;
        case '/':
            if (operand2 != 0) {
                result = operand1 / operand2;
            } else {
                printf("Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            printf("Error: Invalid operator\n");
            exit(EXIT_FAILURE);
    }
    printf("Working...\n\n");
    printf("Done! Your answer is '%d'\n", result);

}

void program_request(int currentLevel) {
    printf("Program request\n");
    printf("Yesss sireee!");
    printf("Working...\n\n");
    sleep(rand() % 5 + 1);
    printf("Done! I have completed your request\n");
}

int handle_request(int level, int instance) {
    int pid = fork();

    if (pid == 0) {  // Proceso hijo (Meeseeks)
        meeseeks(getpid(), getppid(), level, instance);
        exit(0);
    } else if (pid > 0) {  // Proceso padre (Box)
        return pid;
    } else {
        perror("Error creating Meeseeks process");
        exit(EXIT_FAILURE);
    }
}

int main() {
    srand(time(NULL));

    while (1) {
        printf("====================================\n");
        printf("             Meeseeks Box\n");
        printf("====================================\n\n");
        printf("1) Press Button\n");
        printf("2) Exit\n\n");
        printf("Select an option: ");
        scanf("%d", &option);
        // char user_input[100];
        // printf("Ingrese una solicitud (o escriba 'exit' para salir): ");
        // fgets(user_input, sizeof(user_input), stdin);

        if (option == 2) {
            break;
        }
        else{
            time_t start_time = time(NULL);
            int instance = 1;  // Inicializar la instancia en 1
            int pid = handle_request(1, instance);
            waitpid(pid, NULL, 0);  // Esperar a que el Meeseeks termine

            successful_tasks++;
            requests++;
            time_t end_time = time(NULL);
            double execution_time = difftime(end_time, start_time);

            printf("Execution Time: %.2f seconds\n\n", execution_time);
        }
    }

    printf("\nOverview:\n");
    printf("Total requests: %d\n", requests);
    printf("Successful tasks: %d\n", successful_tasks);
    printf("Failed Tasks: %d\n", failed_tasks);

    return 0;
}
