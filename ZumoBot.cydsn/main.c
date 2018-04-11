/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include <project.h>
#include <stdio.h>
#include "Systick.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "I2C_made.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "IR.h"
#include "Ambient.h"
#include "Beep.h"
#include <time.h>
#include <sys/time.h>
int rread(void);

/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

struct sensors_value 
 {
    float l3;
    float l2; 
    float l1;
    float r1;
    float r2;
    float r3;
    };

void go_ahead(int l_speed, int r_speed, int delay, int l_dir, int r_dir);

void normalize(struct sensors_value *, struct sensors_ *);

#if 0
//battery level//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    ADC_Battery_Start();        

    int16 adcresult =0;
    float volts = 0.0;

    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed

    for(;;)
    {
        
        ADC_Battery_StartConvert();
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            
            // Print both ADC results and converted value
            printf("%d %f\r\n",adcresult, volts);
        }
        CyDelay(500);
        
    }
 }   
#endif

#if 0
// button
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    for(;;)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            ShieldLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) CyDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif


#if 0
//ultrasonic sensor//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    Ultra_Start();                          // Ultra Sonic Start function
    while(1) {
        int d = Ultra_GetDistance();
        //If you want to print out the value  
        printf("distance = %d\r\n", d);
        CyDelay(200);
    }
}   
#endif


#if 0
//IR receiver//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    IR_wait(); // wait for IR command
    printf("IR command received\n");
    
    // print received IR pulses and their lengths
    for(;;)
    {
        if(IR_get(&IR_val)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
            //printf("%d %lu\r\n",IR_val & IR_SIGNAL_HIGH ? 1 : 0, (unsigned long) (IR_val & IR_SIGNAL_MASK));
        }
    }    
 }   
#endif


#if 1
    
    //reflectance//
int main(){

    int last_Black; //0 = right, 1 = left

    struct sensors_ ref;
    struct sensors_ dig;
    struct sensors_value Val;
    Systick_Start();

    CyGlobalIntEnable; 
    UART_1_Start();
    
    //battery
    ADC_Battery_Start();        

    int16 adcresult =0;
    float volts = 0.0;
    
    //printf("\nBoot\n");
    int reference=9000;
    
    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //reflectance
    reflectance_start();
   // reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    motor_start();
    
    
    for(;;)
    {
        ADC_Battery_StartConvert();
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            
            // Print both ADC results and converted value
           // printf("%d %f\r\n",adcresult, volts);
        }
        // read raw sensor values
        
            reflectance_read(&ref);
           // normalize(&Val, &ref);
       //printf("%5d %5d %5d %5d %5d %5d\r\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       // print out each period of reflectance sensors
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
       printf("%d %d %d %d %d %d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);        //print out 0 or 1 according to results of reflectance period
       
       // printf("\n%f %f %f %f %f %f\n", Val.l3, Val.l2, Val.l1, Val.r1, Val.r2, Val.r3);
        
        if( dig.l1 == 1 && dig.r1 == 1){
            motor_forward(100,200);
        }
        else if(dig.l1 == 0 && dig.r1==1){
         
           go_ahead(100, 50, 100, 0,1);
            
        }
        else if(dig.l1 == 1 && dig.r1==0){
            
            go_ahead(50, 100, 100, 1 ,0);
            
        }
        else if(dig.r2 == 1 && dig.r1==0){
           
            go_ahead(100,50,10,0,1);
        }
        else if(dig.l2 == 1 && dig.l1==0){
           
            go_ahead(50,100,10,1,0);
        }
        
        else if(dig.l3==1 && dig.l1==0){
            last_Black=1;
            go_ahead(50,150,50,1,0);
        
        }
       
        else if(dig.r3==1 && dig.r1==0){
             last_Black=0;
            go_ahead(150,50,50,0,1);
        }
        else if(last_Black){
            go_ahead(50,150,100,1,0);
        }
        else if(!last_Black){
            go_ahead(150,50,100,0,1);
        }
         
        
        if(dig.l1 == 0 && dig.r1 == 0 && dig.l3==0 &&  dig.r3==0 && dig.l2==0 && dig.r2==0){
            motor_forward(0,0);
        
        }
    CyDelay(10);
}   
}
#endif


#if 0
void normalize(struct sensors_value *val, struct sensors_ *ref){
    float max_value=24000, min_value=7000;
       
        val->l3=(max_value-ref->l3)/(max_value-min_value);
        val->l2=(max_value-ref->l2)/(max_value-min_value);
        val->l1=(max_value-ref->l1)/(max_value-min_value);
        val->r1=(max_value-ref->r1)/(max_value-min_value);
        val->r2=(max_value-ref->r2)/(max_value-min_value);
        val->r3=(max_value-ref->r3)/(max_value-min_value);
     
}
#endif

void go_ahead(int l_speed, int r_speed, int delay, int l_dir, int r_dir){
    
    MotorDirLeft_Write(l_dir);      // set LeftMotor forward mode
    MotorDirRight_Write(r_dir);     // set RightMotor forward mode
    PWM_WriteCompare1(l_speed); 
    PWM_WriteCompare2(r_speed); 
    CyDelay(delay);
}



#if 0
//motor//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();

    motor_start();              // motor start

    motor_forward(100,2000);     // moving forward
    motor_turn(200,50,2000);     // turn
    motor_turn(50,200,2000);     // turn
    motor_backward(100,2000);    // movinb backward
       
    motor_stop();               // motor stop
    
    for(;;)
    {

    }
}
#endif


/* [] END OF FILE */
