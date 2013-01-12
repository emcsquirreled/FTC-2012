#pragma config(Sensor, S1, snsr_accel, sensorI2CCustom)

#include <HTAC-driver.h>
#include "datlog.h"

#define LOG_VAL accel_x
#define COUNTDOWN 5
#define MAX_FILESIZE 100
#define FILENAME "test4"

void readAccel(int &x, int &y, int &z);
void checkFileLength(int &file_length, int &file_number);

task main() {
    int counter, counter_write, counter_file;

    #define ARRAY_LEN 1800

    int i;
    int temp[3][ARRAY_LEN];
    int time[ARRAY_LEN];


    i = 0;

    ClearTimer(T1);

    while(time1[T1] < (COUNTDOWN * 1000)) {
        readAccel(temp[0][i], temp[1][i], temp[2][i]);
        time[i++] = time1[T1];
    }

    counter = 0;
    counter_write = 0;
    counter_file = 1;

    string file = FILENAME;
    file += ".dat";

    bOpenWriteTextFile(file, kMaxFileSize);

    for(i = 0; i < ARRAY_LEN; i++) {
        writeIntegerNumber(counter++);
        writeIntegerNumber(time1[T1]);
        writeIntegerNumber(temp[0][i]);
        writeIntegerNumber(temp[1][i]);
        writeIntegerNumber(temp[2][i]);
        writeNewLine();
        counter_write++;
        checkFileLength(counter_write, counter_file);
    }

    closeWriteTextFile();
    StopAllTasks();
}

void readAccel(int &x, int &y, int &z) {
    HTACreadAllAxes(snsr_accel, x, y, z);
    return;
}

void checkFileLength(int &file_length, int &file_number) {
    if(file_length > MAX_FILESIZE) {
        file_length = 0;
        closeWriteTextFile();
        file_number++;
        string new_file;
        new_file = FILENAME;
        new_file += file_number;
        new_file += ".dat";
        bOpenWriteTextFile(new_file, kMaxFileSize);
    }
    return;
}
