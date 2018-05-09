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
#include <math.h>
int rread(void);

/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

//int max_speed=255;
float kp=255, kd=0.01;
float max_value=20000, min_value=8000;
int l_speed, r_speed;

struct sensors_value 
 {
    float l3;
    float l2; 
    float l1;
    float r1;
    float r2;
    float r3;
    };


void go_ahead(int left, int right, int delay);
void normalize(struct sensors_value *, struct sensors_ *);
void diff_(struct sensors_value *, struct sensors_value *, struct sensors_value *, uint32_t , uint32_t);



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
            while(SW1_Read() == 0) CyDelay(1000); // wait while button is being pressed
        }
        else{
            printf("button not pressed!!\n");
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
            if((IR_val & IR_SIGNAL_HIGH) != 0) {
                 
            }
            printf("%d %d\r\n",b, l);
            //printf("%d %lu\r\n",IR_val & IR_SIGNAL_HIGH ? 1 : 0, (unsigned long) (IR_val & IR_SIGNAL_MASK));
        }
    }    
 }   
#endif


#if 0
    
    //reflectance//
int main(){

     
    Systick_Start();
    CyGlobalIntEnable; 
    UART_1_Start();
    IR_Start();
    ADC_Battery_Start();      
    IR_flush(); // clear IR receive buffer
   // IR_wait(); // wait for IR command
    
    uint32_t IR_val;
    int16 adcresult =0;
    
   
    int last_Black; //0 = last black on right, 1 = last black on left
    struct sensors_ ref;
    struct sensors_ dig;
    struct sensors_value Val;
     
    float volts = 0.0;
    int count=0;
    
    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //reflectance
    reflectance_start();
   // reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
   
    
  //  if((IR_val & IR_SIGNAL_HIGH) != 0) 
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
           
        //normalize(&Val, &ref);
        //printf("%5d %5d %5d %5d %5d %5d\r\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       // print out each period of reflectance sensors
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
        //printf("%d %d %d %d %d %d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);        //print out 0 or 1 according to results of reflectance period
       
      //  printf("\n normalized value \nleft value: %f \nright value %f\n", Val.l3+ Val.l2+ Val.l1, Val.r1+ Val.r2+ Val.r3);
        printf("\n l3=%d l2=%d l1=%d",dig.l3, dig.l2, dig.l1);
        
     
}
}
#endif

#if 0 // PID main
int main(){

    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    ADC_Battery_Start();  
    IR_Start();        
   
    uint32_t time1, time; 
    uint32_t IR_val;
    int16 adcresult =0;
    
    struct sensors_ ref, dig;
    struct sensors_value Val, last_Val={0,0,0,0,0,0}, diff_Val={0,0,0,0,0,0};
    
    float volts = 0.0;
    int flag=0,count, speed,last_Black=0;
    
    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //reflectance
    reflectance_start(); 
    CyDelay(2);
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    
    reflectance_digital(&dig);
    
   //printf("l3=%d l2=%d l1=%d",dig.l3, dig.l2, dig.l1);
   //while loop to follow black line until first stop(a horizontal black line)
    while(true){ 
        
        reflectance_digital(&dig); //returns value sensor values in o and 1 and stores on dig. 
        motor_start();
        go_ahead(100,100,0);
         printf("go condition...\n");
            if(dig.r3+dig.r2+dig.r1+dig.l3+dig.l2+dig.l1>3)  {      //if  all/most are on black summation will be atleast more than 3
                //printf("%d\n",dig.l3+dig.l2+dig.l1+dig.r1+dig.r2+dig.r3);
                flag+=1;
                go_ahead(0,0,0);
              
            break; 
            }
    }
   // printf("out condition...\n");
   // int temp;
    Beep(100,150);
    Beep(100,200);
    IR_flush(); // clear IR receive buffer
    IR_wait(); // wait for IR command
    motor_forward(100,100);  
   // printf("go\n");
    //CyDelay(100);
    
    for(;;)
    {
       
        
        //check battery
        ADC_Battery_StartConvert();
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            volts=(adcresult/4095)*5*1.5;
            // Print both ADC results and converted value
           // printf("\ncode: %d Volts: %f\r\n",adcresult, volts);        

        }

            
            reflectance_read(&ref); 
            normalize(&Val, &ref);      //summarizes the reflectance value from sensors from 0 to 1
                       
            time=GetTicks();        //gets current time
            
            diff_(&diff_Val, &Val, &last_Val, time, time1);     //differentiates the current value from last value with respect to time difference
            
            count=(Val.l1+Val.l2+Val.l3+Val.r1+Val.r2+Val.r3);      //counts the summation of all sensor's normalized value
            
            l_speed = (Val.r1 * kp - diff_Val.r1*kd)+ (Val.r2*kp - diff_Val.r2*kd)*3 + (Val.r3*kp - diff_Val.r3*kd)*5; //as the farther sensor gets black value, the speed gets higher hence sharper turn.
            r_speed = (Val.l1 * -kp - diff_Val.l1*-kd) + (Val.l2*-kp - diff_Val.l2 * -kd)*3 + (Val.l3*-kp - diff_Val.l3 *-kd)*5; // negating right speed to bring the contrast on speed
        
            speed = l_speed + r_speed; //depending upon values from sensor, speed is either positive or negative indicating left sensors are in black or right are in black.
            
        
            
        
        if(count<=0){       //if all on white
            
            if(last_Black==2){      //checks whether the last sensor to detect black was right sensor
                
                    go_ahead(250, -250,0);
                
            }  
            else if(last_Black==1){  //checks whether the last sensor to detect black was left sensor
                
                    go_ahead(-250, 250,0);
               
            }
        }
        else if(count<5&&count>0){      //if more than one sensor and less than 5 sensor are reading black
            
            if(Val.l3)    last_Black=1;     //
            if(Val.r3)    last_Black=2;
           
            if(speed<=0)    // if speed is negative, right speed is higher so it turns left.
                {
                    go_ahead(l_speed, -r_speed,0);
                }   
                   
             else {           //if speed is positive, left speed is higher so it turns right  
                go_ahead(l_speed, -r_speed, 0); 
                    }
      
        }
        
        else {   
            if(count==6&&flag<2){       //if reads black line for second time
                //go_ahead(50,50,0);
                flag+=1;                //raises the flag count
                CyDelay(200);           //gives enough time to run ahead so sensor wont count same black line more than once
                
            }else if(count==6&&flag>=2){        //if read black line and flag is 2 or more
                go_ahead(0,0,0);    //stop the robot
                motor_stop();                   //stop the motor
                Beep(100,150);
                Beep(100,200);
            }
        
            
        }
            time1=time;                         //assign current time to time1 so it can be use as last_value in next loop
            last_Val=Val;                       //assign Val to last_Val so it can be used as last_Value in next loop
    }
}

#endif
#if 1 //ZUMO
int main (void){
    
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    Ultra_Start();  
    ADC_Battery_Start();  
    IR_Start();        
   
    uint32_t time1, time; 
    uint32_t IR_val;
    int16 adcresult =0;
    
    struct sensors_ ref, dig;struct sensors_value Val;
    int a=0; 
    float volts = 0.0;
    int flag=0,count,l_count,r_count, speed,last_Black=0;
    int d = Ultra_GetDistance();
    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //reflectance
    reflectance_start(); 
    CyDelay(2);
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    
    reflectance_digital(&dig);
    
    while(true){
        
        reflectance_digital(&dig);
        motor_start();
        go_ahead(250,250,0);
         printf("go condition...\n");
            if(dig.r3+dig.r2+dig.r1+dig.l3+dig.l2+dig.l1>3)  {
                printf("%d\n",dig.l3+dig.l2+dig.l1+dig.r1+dig.r2+dig.r3);
                flag+=1;
                go_ahead(0,0,0);
              
            break; 
            }
    }
    printf("out condition...\n");
    
    IR_flush(); // clear IR receive buffer
    IR_wait(); // wait for IR command
    motor_forward(50,0);  
    printf("go\n");
    CyDelay(500);
    
    for(;;)
    {
        d = Ultra_GetDistance();
       //printf("%d\n",d);
        l_count=(Val.l3+Val.l2+Val.l2);
        r_count=(Val.r3+Val.r2+Val.r1);
        count=l_count+r_count;
        //check battery
        ADC_Battery_StartConvert();
       
        
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            
            volts=(adcresult/4095)*5*1.5;
        }
        
        reflectance_digital(&dig);
       
        
            reflectance_read(&ref);
            normalize(&Val, &ref);
            reflectance_digital(&dig);
            CyDelay(20);
            
            //printf("%d\n",150*(!a));
       
            
            if(l_count>=1){
                    go_ahead(150,-150,200);
                    printf("turn right\n");
            }
            else if(r_count>=1){
                    go_ahead(-150,150,200);
                    printf("turn left\n");
            }
            else if(count>3){
                
                    go_ahead(-150,-150,200);
                    go_ahead(-250,250,500);
                    printf("turn around\n");
            }
            while(count<1){
                if(d<15&&d>0){
                    printf("following\n");
                    go_ahead(250,250,00);
                    if(count>1){
                     break;
                    }
                }
                else{
                    break;
                }
            } 
        
        
    }
    
}
#endif
void normalize(struct sensors_value *val, struct sensors_ *ref){//functon to normalize sensors value from 0 to 1
    
        val->l3=(ref->l3-min_value)/(max_value-min_value); //black-ref/18000 1 is black ,0 is white;
        if(val->l3>1) val->l3=1;
            else if(val->l3<0) val->l3=0;
            
        val->l2=(ref->l2-min_value)/(max_value-min_value);
        if(val->l2>1) val->l2=1;
            else if(val->l2<0) val->l2=0;
        
        val->l1=(ref->l1-min_value)/(max_value-min_value);
        if(val->l1>1) val->l1=1;
            else if(val->l1<0) val->l1=0;
        
        val->r1=(ref->r1-min_value)/(max_value-min_value);
        if(val->r1>1) val->r1=1;
            else if(val->r1<0) val->r1=0;
        
        val->r2=(ref->r2-min_value)/(max_value-min_value);
        if(val->r2>1) val->r2=1;
            else if(val->r2<0) val->r2=0;
        
        val->r3=(ref->r3-min_value)/(max_value-min_value);
        if(val->r3>1) val->r3=1;
            else if(val->r3<0) val->r3=0;
}

void go_ahead(int left, int right, int delay){ //function to command motors speed and delay
    
  
    if(left > 255) {                    //if speed is above 255,
        left=255;                       // it assigns speed as max speed (255)
        MotorDirLeft_Write(0);          //motor direction set to forward
    } 
        
    else if(left<0)  { 
       left=left*(-1);
        MotorDirLeft_Write(1);          //motor direction set to backward
       
    }else{
        MotorDirLeft_Write(0);
    }
        
    if(right > 255){
        right=255;
        MotorDirRight_Write(0); 
    } 
       
    else if(right<0){
        right=right*(-1); 
        MotorDirRight_Write(1); 
        
    
    }  else{
        MotorDirRight_Write(0);
    }
    
      
    PWM_WriteCompare1(left);       //sets left motor speed to value  "left", recieved from main function as parameter
    PWM_WriteCompare2(right);      //sets right motor speed to value  "right", recieved from main function as parameter
    
    CyDelay(delay);
}

void diff_(struct sensors_value *diff_val, struct sensors_value *val, struct sensors_value *last_Val, uint32_t time, uint32_t time1){
    
    
    diff_val->l3 = (val->l3 - (last_Val->l3))/1000*(time-time1); //diff_val = old error value - new error value/ time difference;
    diff_val->l2 = (val->l2 - (last_Val->l2))/1000*(time-time1);
    diff_val->l1 = (val->l1 - (last_Val->l1))/1000*(time-time1);
    diff_val->r3 = (val->r3 - (last_Val->r3))/1000*(time-time1);
    diff_val->r2 = (val->r2 - (last_Val->r2))/1000*(time-time1);
    diff_val->r1 = (val->r1 - (last_Val->r1))/1000*(time-time1);
   // printf("\n%f %d \n",diff_val->l3, time1);
    
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
