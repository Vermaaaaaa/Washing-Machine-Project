#include "mbed.h"
#include <string>
#include "music.h"




// Object Definitions
DigitalOut redled(PC_0);
DigitalOut gled1(PA_4);
DigitalOut gled2(PB_0);
DigitalOut gled3(PC_1);
DigitalIn lmb(PC_10);
DigitalIn mmb(PC_11);
DigitalIn rmb(PD_2);
BusOut SegDis(PB_12,PA_12, PB_1, PB_15, PB_14, PA_11, PB_11);
BufferedSerial serial(USBTX, USBRX);
AnalogIn pot1(PA_5);
AnalogIn pot2(PA_6);
AnalogIn pot3(PA_7);
PwmOut red_LED(PB_5);   //PWM output to red led
PwmOut green_LED(PB_3); //PWM output to green led
PwmOut blue_LED(PB_4);  //PWM output to blue led
AnalogIn FSR(PA_1);

    


constexpr char escape_seq[] = "\033[2J\033[3J\033[1;1H"; // C string which is written to the serial monitor to clear it 

bool check_on = false; // Boolean variable to check if device is on or off

float pot1_val = 0.0f; //floats to store the pot values 
float pot2_val = 0.0f;
float pot3_val = 0.0f;

uint8_t pot1_select = 0; // Unsigned integers as it is easier to read a hex value from the display 
uint8_t pot2_select = 0; // Then float voltage and use switch statements as faster
uint8_t pot3_select = 0; // uint8_t also takes less space in memory than int 

string cycle = ""; // When displaying the cycle we have to use a string so like Whites, Perm Press etc



// All of my function prototypes 
void init_on();
void init_off();
void cycle_select(bool check_cond, float& pot_val, uint8_t& pot_select, AnalogIn& pot, DigitalOut& gled);
void cycle_decision(bool check_cond);
void start_cycle(bool check_cond);
void para_select();
bool check_para();
void error_code();
void end_cycle();
void timer(int m, int s);



int main()
{

    while(1){ // Infinite loop to check if the on button is pressed
        if(lmb.read() == 1){
            check_on = !check_on; //Inverts the global to start program
            init_on();//Runs init_on method which is a standby mode for the WM
            ThisThread::sleep_for(1s); 

        while(check_on){ //While check_on is true meaning, WM is on we run our methods, also allows for multiple spins 
            if(check_on){ //Want to see if Check_on is true other wise we can switch off device
                cycle_select(check_on, pot1_val, pot1_select, pot1, gled1);
                cycle_select(check_on, pot2_val, pot2_select, pot2, gled2);
                cycle_select(check_on, pot3_val, pot3_select, pot3, gled3);
                // Run the 3 methods which determine spin cycle, temp and time 
                if(check_para()){ // Check_para method returns true or false to see if all 3 parameters have been selected
                    redled.write(0);
                    SegDis.write(0x3F);
                    // Change in IO to show user the WM is ready for use 
                    para_select();
                    // Changes values from users from hex to int/string
                    start_cycle(check_on);
                    // Starts the cycle from the user 
                    thread_sleep_for(500);
                    end_cycle();
                    // End cycle runs
                }
            }

            else{ // If check_on is false we run init_off
                init_off();
                }
            thread_sleep_for(500);
            }

        }
    }
}

void init_on(){
    serial.write(escape_seq, sizeof(escape_seq));
    serial.write("Washing Machine ON\n", strlen("Washing Machine ON\n"));
    // Writes the c string defined before to clear the serial monitor
    SegDis.write(0x3F);
    redled.write(1);
    play_startup();
    pot1_val = 0.0f;
    pot2_val = 0.0f;
    pot3_val = 0.0f;
    serial.write("To select a function spin the dials, once you have selected a function push the middle button to select this function, afterwards press the last button to start the cycle\n", strlen("To select a function spin the dials, once you have selected a function push the middle button to select this function, afterwards press the last button to start the cycle\n"));
}
// Sets our WM to on mode/standby and writes to user that it is on and some instructions

void init_off(){
    serial.write(escape_seq, sizeof(escape_seq)); // Writes the c string defined before to clear the serial monitor
    serial.write("Washing Machine OFF\n", strlen("Washing Machine OFF\n")); 
    SegDis.write(0);
    redled.write(0);
    play_off();
    check_on = false;
    pot1_val = 0.0f;
    pot2_val = 0.0f;
    pot3_val = 0.0f;
    pot1_select = 0;
    pot2_select = 0;
    pot3_select = 0;
    cycle = "";
    gled1.write(0);
    gled2.write(0);
    gled3.write(0);
    //sets everything to an off mode e.g. sets all numbers to 0, clears string, boolean set to false etc.
}
// Sets our WM to off mode 

void cycle_select(bool check_cond, float& pot_val, uint8_t& pot_select, AnalogIn& pot, DigitalOut& gled){

    // Uses internal copy check_on as check_cond we may want to end our method but not turn off our WM
   
    while(check_cond){

        if(lmb.read() == 1){ //Checks if the left most button is pressed again to turn off the device 
            check_cond = !check_cond;
            init_off();
            thread_sleep_for(500);
        }
        else if(rmb.read() == 1){ // Checks if the run button is pressed and if the parameter hasn't been selected it
            error_code(); // will run the error code
            continue;
            thread_sleep_for(500);
        }
        else{
            pot_val = pot.read() * 3.3;
            thread_sleep_for(500);
            if(pot_val < 0.4){
                SegDis.write(0x3F);
            }
            else if(pot_val > 0.4 && pot_val < 1.0){
                SegDis.write(0x06);
            }
            else if(pot_val >= 1.01 && pot_val < 1.8){
                SegDis.write(0x5B);
            }
            else if(pot_val >= 1.81 && pot_val < 2.6){
                SegDis.write(0x4F);
            }
            else{
                SegDis.write(0xE6);
            }
            // Constantly reads the pot values and writes to the display these values

            if(mmb.read() == 1){
                if(SegDis.read() != 0x3F){ // Checks to see if the current value displayed to the user is not 0
                    pot_select = SegDis.read(); // Stores the hex read from the bus into pot select
                    gled.write(1);
                    completion_sequence(); // plays music
                    check_cond = false; // Disables the while loop so the next method can run
                }   
                    thread_sleep_for(500);
                }
                
            }
            
        }
    }

void para_select(){
    switch(pot1_select){ // Takes the pot values which are hex and assigns numbers or strings to it 
            case 0x06:{
                cycle = "Perm Press\n";
                break;
            }
            case 0x5B:{
               cycle = "Whites\n";
                break;
            }
            case 0x4F:{
                cycle = "Colours\n";
                break;
            }
            default:{
                cycle = "Cotton\n";
                break;
            }

    }
    switch(pot2_select){
        case 0x06:{
            pot2_select = 30;
            break;
        }
        case 0x5B:{
            pot2_select = 40;
            break;
        }
        case 0x4F:{
            pot2_select = 50;
            break;
        }
        default:{
            pot2_select = 60;
            break;
        }

    }
    switch(pot3_select){
        case 0x06:{
            pot3_select = 15;
            break;
        }
        case 0x5B:{
            pot3_select = 30;
            break;
        }
        case 0x4F:{
            pot3_select = 45;
            break;
        }
        default:{
            pot3_select = 60;
            break;
        }
    }

    std::string temp_str = "The temperature you have selected is " + std::to_string(pot2_select) + " Degrees Celsius\n";
    std::string time_str = "The time you have selected is " + std::to_string(pot3_select) + " minutes\n";
    cycle = "The cycle you have selected is " + cycle;
    //formats all of our selections to be written to the user


    serial.write(cycle.c_str(),strlen(cycle.c_str()));
    serial.write(temp_str.c_str(), strlen(temp_str.c_str()));
    serial.write(time_str.c_str(), strlen(time_str.c_str()));
    // Prints out what the user has selected


}

void start_cycle(bool check_cond){
    while(check_cond){
        if(lmb.read() == 1){//Checks if the left most button is pressed again to turn off the device 
            check_cond = !check_cond;
            init_off();
            thread_sleep_for(500);
        }
        else{
                if(rmb.read() == 1 && check_para()){ //Checks to see if rmb has been pressed and all parameters have been selected
                    
                    start_sequence(); // Plays cycle start sequence 
                    SegDis.write(0x4F);
                    gled3.write(0);
                    thread_sleep_for(1000);
                    SegDis.write(0x5B);
                    gled2.write(0);
                    thread_sleep_for(1000);
                    SegDis.write(0x06);
                    gled1.write(0);
                    thread_sleep_for(1000);
                    SegDis.write(0);
                    redled.write(1);
                    // Runs through a sequence which is very quick so it seems like multi threading but isn't
                    timer(pot3_select,0);
                    // Runs timer function using the time selected by the user


                    check_cond = false; //ends function
                    
                }
        }
    }
}

bool check_para(){

    if (pot1_select == 0 || pot2_select == 0 || pot3_select == 0){
        return false;
    }
    else{
        return true;
    }
// Checks if the pot values are equal to 0 and if they are returns false else returns true

}

void error_code(){

    red_LED.period(1/10.0);        
    green_LED.period(1/10.0);
    blue_LED.period(1/10.0);
    //Sets the periods of the Multi-LEDs

    if(pot1_select == 0){
        red_LED.write(1.0f);
        errorTone();
        // shows red to indicate no cycle selected
        
    }
    else if(pot2_select == 0){
        blue_LED.write(1.0f);
        errorTone();
        // shows blue to indicate no temp selected selected
    }
    else{
        green_LED.write(1.0f);
        errorTone();
        // shows green to indicate no time selected
    }
    thread_sleep_for(1000);
    red_LED.write(0.0f);
    green_LED.write(0.0f);
    blue_LED.write(0.0f);
    //turns off multi-leds

// Uses multi led to show a colour indicating if something is not selected 
}

void end_cycle(){
    if(check_on){// Checks to see if washing machine is still on
        redled.write(0);
        gled1.write(1);
        gled2.write(1);
        gled3.write(1);
        zelda();
        serial.write(escape_seq, sizeof(escape_seq));
        serial.write("Washing Cycle Complete\n", strlen("Washing Cycle Complete\n"));
        thread_sleep_for(1000);
        SegDis.write(0x3F);
        redled.write(1);
        gled1.write(0);
        gled2.write(0);
        gled3.write(0); 
        pot1_val = 0.0f;
        pot2_val = 0.0f;
        pot3_val = 0.0f;
        pot1_select = 0;
        pot2_select = 0;
        pot3_select = 0;
        cycle = "";
        //clears all variables so wash cycle can loop with no unexpected behaviours
    }
   
// Similar to init_on but plays noise 
}

void timer(int m, int s)
{
    bool internal_check = true; // Track if timer needs to end
    bool paused = false; // track whether the timer is paused

    while(internal_check)
    {
        if (!paused) { // only count down if the timer is not paused
            if ( m == 0 && s == 0) 
            {
                internal_check = false; //If time = 0 end function
            }
            if (m > 0 && s == 0)
            {
                s = 59; // If seconds are 0 decrement minutes and set seconds back to 59
                m--;
            }
            
            std::string m_str = std::to_string(m) + ":";
            std::string s_str = std::to_string(s) + "\n";
            serial.write(m_str.c_str(), strlen(m_str.c_str()));
            serial.write(s_str.c_str(), strlen(s_str.c_str()));
            ThisThread::sleep_for(1s);
            s--;
            //Formats our time to be written to the monitor and decrement a second every second
        }

        if(rmb.read() == 1)// If right button is pressed
        {
            paused = !paused; // invert the paused state
            if (paused) {
                serial.write("WASHING PAUSED\n", strlen("WASHING PAUSED\n"));
            } else {
                serial.write("WASHING RESUMED\n", strlen("WASHING RESUMED\n"));
            }
            ThisThread::sleep_for(500ms);
            // Shows the user if the timer has been paused
        }

        if(FSR.read() > 0.4)
        {
            internal_check = false;
            serial.write("WASHING FORCEFULLY STOPPED\n", strlen("WASHING FORCEFULLY STOPPED\n"));
            ThisThread::sleep_for(500ms);
            // Ends the cycle if force on the fsr is greater than arbitrary 0.4 force and writes to the user it has been forcefully stopped

        }
    }

    // Timer which takes in the times we have previously selected and counts down
}
