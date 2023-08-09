#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#define MAX_ALARMS 10

/*
 All the functions are working but there is one exception :

 When the time entered manually,time doesn't keep moving


 Sometimes timer reset button has bugging

*/

// Function to display the time in the hh:mm:ss format
void timeInfo(struct tm* infotime) {
    printf("\t%d:%d:%d\n\n\n", infotime->tm_hour, infotime->tm_min, infotime->tm_sec);
}

// Function to display the main menu and get user's choice
int menu() {
    int choice;
    printf("1. Set time manually\n");
    printf("2. Set alarm(s)\n");
    printf("3. Set timer\n");
    printf("4. Show current time\n");
    printf("5. Press 55555 to lock the screen\n");
    printf("6. Set time automatically\n");
    printf("Enter your choice: \n");
    scanf("%d", &choice);
    return choice;
}

// Function to set the time manually
void setManualTime(struct tm* manualtime) {
    printf("Enter hour, minute, and second to set the time : ");
    scanf("%d%d%d", &manualtime->tm_hour, &manualtime->tm_min, &manualtime->tm_sec);

    // Validate the user input
    while (manualtime->tm_hour > 23 || manualtime->tm_hour < 0 || manualtime->tm_min > 59 || manualtime->tm_min < 0 || manualtime->tm_sec > 59 || manualtime->tm_sec < 0) {
        printf("Invalid input(s)\n");
        printf("Enter hour, minute, and second to set the time: ");
        scanf("%d%d%d", &manualtime->tm_hour, &manualtime->tm_min, &manualtime->tm_sec);
    }

    printf("New time is: ");
    timeInfo(manualtime);
}

// Function to set alarm time(s)
void setAlarmTime(int numalarms, struct tm* setalarm[]) {
    for (int i = 0; i < numalarms; i++) {
        printf("Enter the hour and minute to set an %d. alarm: ", i + 1);
        setalarm[i] = (struct tm*)malloc(sizeof(struct tm));
        scanf("%d%d", &setalarm[i]->tm_hour, &setalarm[i]->tm_min);
        setalarm[i]->tm_sec = 0;
    }
}

// Function to check if there are active alarms
int checkAlarm(int alarms, struct tm* setalarm[]) {
    time_t now = time(NULL);
    struct tm* cur_time = localtime(&now);
    int activeAlarms = 0;

    for (int i = 0; i < alarms; i++) {
        if (setalarm[i]->tm_hour == cur_time->tm_hour && setalarm[i]->tm_min == cur_time->tm_min) {
            activeAlarms++;
        }
    }

    return activeAlarms;
}

// Function to set and handle the timer
void setTimer() {
    time_t start_time = time(NULL);
    time_t current_time;
    int duration, choice, starter;
    int paused = 0; // Flag to track if the timer is paused
    printf("Enter a duration: ");
    scanf("%d", &duration);

    // Validate the user input for the duration
    while (duration < 0 || duration > 10000) {
        printf("Invalid input, try again\n\n");
        printf("Enter a duration: ");
        scanf("%d", &duration);
    }

    printf("Press 1 to start timer\n");
    scanf("%d", &starter);

    printf("Time has started\n\n");
    while (1) {
        if (!paused) { // If not paused, decrement the duration
            if (duration <= 0) {
                printf("Time's up!\n\n");
                break;
            }
            Sleep(1000);
            printf("%d (Press 2 to pause or resume, 3 to reset)\n", duration);
            duration--;
        }

        // Check for non-blocking input
        if (_kbhit()) {
            int input = _getch(); // Read the input character
            if (input == '2') {
                paused = !paused; // Toggle the paused flag
                printf("Time has %s\n", paused ? "paused" : "resumed");
            }
            else if (input == '3') {
                printf("Time has reset\n");
                duration = 0;
            }
        }
    }
}

int main(void) {
    // Variable Declarations
    time_t settime;
    struct tm* alarmtime[MAX_ALARMS];
    struct tm manualtime;
    struct tm timeflag;
    int flag = 0;
    int numalarm;
    int checktimer = 0;
    int chalarm = 0;
    int seconds = 10;
    int paused = 0;

    // Main loop to handle the user input and actions
    while (flag != 55555) {
        time_t now = time(NULL);
        struct tm* cur_time = localtime(&now);

        flag = menu();
        switch (flag) {
        case 1:
            setManualTime(&manualtime);
            checktimer++;
            break;

        case 2:
            printf("Enter the number(s) of alarm(s) to be set: (MAX 10)");
            scanf("%d", &numalarm);
            while (numalarm < 0 || numalarm > MAX_ALARMS) {
                printf("Invalid number of alarms.(MAX %d)\n", MAX_ALARMS);
                printf("Enter the number(s) of alarm(s) to be set: (MAX %d)\n", MAX_ALARMS);
                scanf("%d", &numalarm);
            }

            for (int i = 0; i < numalarm; i++) {
                alarmtime[i] = (struct tm*)malloc(sizeof(struct tm));
            }
            setAlarmTime(numalarm, alarmtime);
            chalarm = numalarm;
            break;

        case 3:
            setTimer();
            break;

        case 4:
            if (checktimer == 0) {
                timeInfo(cur_time);
            }
            else {
                cur_time = &manualtime;
                timeInfo(cur_time);
            }
            break;

        case 55555:
            return (0);
            break;
        case 6:
            int flag;

            checktimer = 0;
            cur_time = localtime(&now);
            printf("\nTime has setted to localtime\n\n");
            timeInfo(cur_time);
            break;
        }

        // Check and handle alarms
        if (chalarm != 0 && chalarm <= numalarm) {
            if (checkAlarm(numalarm, alarmtime) > 0) {
                int activeAlarms = checkAlarm(numalarm, alarmtime);
                int checker = 60;
                paused = 0;

                while (activeAlarms > 0 && checker > 0) {
                    if (!paused) {
                        printf("ALERT ALERT ALERT (PRESS 2 TO STOP)\n\n");
                        Sleep(1000);
                        checker--;
                    }

                    // Check for non-blocking input
                    if (_kbhit()) {
                        int input = _getch(); // Read the input character
                        if (input == '2') {
                            paused = !paused; // Toggle the paused flag
                            printf("ALARM HAS STOPPED\n");

                            // Remove the stopped alarm from the array
                            for (int i = 0; i < numalarm; i++) {
                                if (checkAlarm(1, &alarmtime[i])) {
                                    free(alarmtime[i]);
                                    for (int j = i; j < numalarm - 1; j++) {
                                        alarmtime[j] = alarmtime[j + 1]; // Shift the remaining alarms up in the array
                                    }
                                    numalarm--; // Decrement the number of active alarms
                                    chalarm--; // Decrement the number of active alarms for checking
                                    break;
                                }
                            }
                        }
                    }

                    // Update the number of active alarms
                    activeAlarms = checkAlarm(numalarm, alarmtime);
                }
            }
        }
    }

    // Free memory for remaining alarms
    for (int i = 0; i < chalarm; i++) {
        free(alarmtime[i]);
    }

    return 0;
}

