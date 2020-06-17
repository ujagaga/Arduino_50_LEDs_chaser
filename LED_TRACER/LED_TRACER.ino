#include <I2cDiscreteIoExpander.h>

// Pin definitions
// SDA => A4, SCL => A5

#define POT_PIN   A7
#define LED_49    10
#define LED_50    11


I2cDiscreteIoExpander expDevice[] = {0, 1, 2};

uint8_t LED_counter = 0;          // A counter to be used for selecting which LED's will be lit.


// Function to wait a specific number of ms.
void wait_ms(int ms){
  int i;
  for(i = 0; i < ms; i++){
    delay(1);
  }
}


/* Function to light up all LED's but in a time sharing maner.  
 * Maximum 9 LED's are on at one time.
 */
void fastTraceAll(int delayTime){
  int i;
  // We have 6 blocks each lasting 1ms.
  for(i = 0; i < (delayTime / 6); i++){

    // Activate first 8 bits on first port extender with one LED from the MCU board
    setExpDevice(0, 0x00FF);
    setExpDevice(1, 0);
    setExpDevice(2, 0);
    digitalWrite(LED_49, HIGH);
    digitalWrite(LED_50, LOW);    
    delay(1);

    // Activate second 8 bits on first port extender with one LED from the MCU board
    setExpDevice(0, 0xFF00);   
    digitalWrite(LED_49, LOW);
    digitalWrite(LED_50, HIGH);       
    delay(1);

    // Activate first 8 bits on second port extender
    setExpDevice(0, 0);   
    setExpDevice(1, 0x00FF);   
    delay(1);

    // Activate second 8 bits on second port extender
    setExpDevice(1, 0xFF00);   
    delay(1);

    // Activate first 8 bits on third port extender with one LED from the MCU board
    setExpDevice(1, 0);   
    setExpDevice(2, 0x00FF);  
    digitalWrite(LED_49, HIGH);
    digitalWrite(LED_50, LOW);  
    delay(1);

    // Activate second 8 bits on thirt port extender with one LED from the MCU board
    setExpDevice(2, 0xFF00); 
    digitalWrite(LED_49, LOW);
    digitalWrite(LED_50, HIGH);    
    delay(1);
  }

  // De-activate all pins
  setExpDevice(0, 0);
  setExpDevice(1, 0);
  setExpDevice(2, 0);
  digitalWrite(LED_49, LOW);
  digitalWrite(LED_50, LOW); 
}


/* Function to output data to a specified port extender. */
void setExpDevice(int id, uint16_t val){ 
  uint8_t devstatus = expDevice[id].digitalWrite(val);

  if (TWI_SUCCESS != devstatus){
    Serial.print("write error on device ");
    Serial.print(devstatus, DEC);
  }
}


/* This is called only once at startup and sets up the starting conditions. */
void setup() {  

  // initialize i2c interface
  Wire.begin();

  // initialize serial interface
  Serial.begin(9600);

  pinMode(LED_49, OUTPUT);
  pinMode(LED_50, OUTPUT);
  pinMode(POT_PIN, INPUT);
  
  LED_counter = 0;
  
}


/* A main loop that gets called periodically. */
void loop() {

  // Read potentiometer to get requested delay time
  int delayTime = analogRead(POT_PIN);
  if(delayTime < 50){
      delayTime = 50;
  }
  
  // Calculate which LED should be on based on the LED_counter value
  if(LED_counter == 0){
    // All on
    fastTraceAll(delayTime);    
  }else{
    // LED_counter value 1 should turn all off, so we only process values higher than 1.
    uint16_t val;

    if(LED_counter == 1){
      // All off
      setExpDevice(0, 0);
      setExpDevice(1, 0);
      setExpDevice(2, 0);
      digitalWrite(LED_49, LOW);
      digitalWrite(LED_50, LOW);
      
    }else if(LED_counter < 18){
      // Device 0
      val = 1 << (LED_counter - 2);
      
      setExpDevice(0, val);
      setExpDevice(1, 0);
      setExpDevice(2, 0);
      digitalWrite(LED_49, LOW);
      digitalWrite(LED_50, LOW);
      
    }else if(LED_counter < 34){
      // Device 1
      val = 1 << (LED_counter - 18);
      
      setExpDevice(0, 0);
      setExpDevice(1, val);
      setExpDevice(2, 0);
      digitalWrite(LED_49, LOW);
      digitalWrite(LED_50, LOW);
      
    }else if(LED_counter < 50){
      // Device 2
      val = 1 << (LED_counter - 34);
      
      setExpDevice(0, 0);
      setExpDevice(1, 0);
      setExpDevice(2, val);
      digitalWrite(LED_49, LOW);
      digitalWrite(LED_50, LOW);
      
    }else{      
      setExpDevice(0, 0);
      setExpDevice(1, 0);
      setExpDevice(2, 0);

      if(LED_counter == 50){
        digitalWrite(LED_49, HIGH);
        digitalWrite(LED_50, LOW);
      }else if(LED_counter == 51){
        digitalWrite(LED_49, LOW);
        digitalWrite(LED_50, HIGH);
      }
    }

    wait_ms(delayTime);
  }  

  // Process LED counter
  LED_counter++;
  if(LED_counter > 51){
    LED_counter = 0;
  }
}
