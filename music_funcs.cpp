#include "music.h"
#include "mbed.h"
#include <chrono>

using namespace std::chrono;

//Define speaker 
PwmOut speaker(PA_15);

// Define durations
auto dur2 = milliseconds(200);
auto dur4 = milliseconds(400);
auto dur1 = milliseconds(100);
auto dur250 = milliseconds(250);
auto dur125 = milliseconds(125);
auto dur500 = milliseconds(500);


// Define note frequencies
#define NOTE_C4  262    
#define NOTE_G3  196
#define NOTE_F3  175
#define NOTE_E3  165
#define NOTE_D3  147
#define NOTE_C3  131
#define NOTE_E5 659
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_A6 1760
#define NOTE_C6  1046.50f
#define NOTE_D6  1174.66f
#define NOTE_B6  1975.53f
#define NOTE_E5  659
#define NOTE_A5  880







void play_startup(){
    
    int notes[6] = {NOTE_E5, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_C5, NOTE_A4}; // Stores notes to be used in an array
    milliseconds durations[6] = {dur2, dur2, dur2, dur2, dur2, dur4}; // Stores durations to be used in an array

    for(int i = 0; i < 6; i++)
    {
        speaker.period(1.0/notes[i]);
        speaker.write(0.5f);
        ThisThread::sleep_for(durations[i]);
        //plays note 1 for duration 1 and so on
    }
    
    speaker.write(0.0f);
    //turn off speaker

}

void play_off()
{


        int notes[6] = {NOTE_C4, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, NOTE_C3}; // Stores notes to be used in an array
        milliseconds durations[6] = {dur250, dur125, dur125, dur250, dur250, 1s}; // Stores durations to be used in an array

        for (int i = 0; i < 6; i++) {
            speaker.period(1.0f / notes[i]);    
            speaker.write(0.5f);    
            ThisThread::sleep_for(durations[i]);
            //plays note 1 for duration 1 and so on
            speaker.write(0.0f);   
            ThisThread::sleep_for(milliseconds(50)); 
            //turns off speaker and delay before next note played

        }

        
    

}



void errorTone() {
    for (int i = 0; i < 3; i++) {

        speaker.period(1.0/NOTE_E5);
        speaker.write(0.5);
        ThisThread::sleep_for(dur2);
        speaker.write(0);
        ThisThread::sleep_for(dur1);
        //Repeat same note 3 times

    }
    
    speaker.period(1.0/NOTE_E5);
    speaker.write(0.5);
    ThisThread::sleep_for(1s);
    speaker.write(0);
    //Repeat note but for longer and turn off speaker

}

void start_sequence() {
    
    int notes[5] = {NOTE_E5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5}; // Stores notes to be used in an array
    milliseconds durations[5] = {dur125, dur125, dur250, dur250, dur500}; // Stores durations to be used in an array
    for (int i = 0; i < 5; i++) {
        
        speaker.period(1.0f / notes[i]); 
        speaker.write(0.5f);
        ThisThread::sleep_for((durations[i])); 
        //plays note 1 for duration 1 and so on
        
        speaker.write(0.0f); 
        ThisThread::sleep_for(milliseconds(10));
        // turns off speaker and delay before playing next note

    }
}

void completion_sequence() {
    // Play a rising arpeggio
    float notes[4] = {NOTE_C6,NOTE_E6, NOTE_G6, NOTE_B6};
    for(int i = 0; i < 4; i++)
    {
        speaker.period(1.0f / notes[i]);
        speaker.write(0.5f);
        ThisThread::sleep_for(dur1);

    }
    
    
    speaker.write(0.0f);
}

void zelda()
{
    float notes[4] = {NOTE_E5, NOTE_A5, NOTE_C6, NOTE_D6};
    milliseconds durations[4] = {dur250, dur250, dur250, dur125};

    for(int i = 0; i < 4; i++)
    {
        speaker.period(1.0f / notes[i]);
        speaker.write(0.5f);
        ThisThread::sleep_for(durations[i]);
    }
    speaker.write(0.0f);
    
    
}