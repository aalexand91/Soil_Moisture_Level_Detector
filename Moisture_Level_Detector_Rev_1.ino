/*****************************************************************************************************
 * Project:             Moisture Level Detector
 * 
 * Description:         Firmware to measure the moisture level of plant/potting soil up
 *                      to six plants. The MCU will output a pulse of a certain duration
 *                      that will cause the moisture sensors to activate. The feedback signal
 *                      of the sensors is then fed to the onboard ADC of the MCU which is then
 *                      translated into a moisture level. The higher the voltage reading, the
 *                      more moist the soil is. If the moisture level of a plant is below a
 *                      certain value, then the MCU will output to an LED that will indicate
 *                      to the user to water their plants.
 *                      
 *                      NOTE: THIS FIRMWARE IS FOR A MCU WITH 12 digital GPIO and 8 ADC pins
 *                      
 * Original Author(s):  Anthony Alexander
 * 
 * Date           Author                Description of change
 * 06/17/2020     Anthony Alexander     Initial release
 *****************************************************************************************************/

#define LOW_MOISTURE_LEVEL      200             // low moisture level
#define MAX_NUM_PLANTS          6               // the maximum number of moisure levels the MCU can measure
#define ADC_VOLTAGE_CONVERTER   (4.9 / 1000)    // number to convert the ADC measurement into a readable voltage
#define DELAY_TIME              2000            // amount of time in milliseconds to delay taking an ADC measurement

// define the LED/Plant number per digital output pin
#define LED1 7
#define LED2 6
#define LED3 5
#define LED4 4
#define LED5 3
#define LED6 2

// define the sensor trigger voltage pins per digital output pin
#define VCC1 13
#define VCC2 12
#define VCC3 11
#define VCC4 10
#define VCC5 9
#define VCC6 8

/*****************************************************************
 * Function:    setup
 * 
 * Description: Initial setup of the I/O pins of the MCU
 * 
 * Parameters:  N/A
 * 
 * Returns:     N/A
 ******************************************************************/
void setup() 
{
  // set up ADC pins to be as inputs
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  // set up digital output pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(VCC1, OUTPUT);
  pinMode(VCC2, OUTPUT);
  pinMode(VCC3, OUTPUT);
  pinMode(VCC4, OUTPUT);
  pinMode(VCC5, OUTPUT);
  pinMode(VCC6, OUTPUT);

  // set all digital pins to low as they should initially be off
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(VCC1, LOW);
  digitalWrite(VCC2, LOW);
  digitalWrite(VCC3, LOW);
  digitalWrite(VCC4, LOW);
  digitalWrite(VCC5, LOW);
  digitalWrite(VCC6, LOW);

  // serial printing to allow easy debugging
  Serial.begin(9600);
}

/*****************************************************************
 * Function:    loop
 * 
 * Description: Infinite routine the MCU will perform
 * 
 * Parameters:  N/A
 * 
 * Returns:     N/A
 *****************************************************************/
void loop() 
{
  // place the LED pins, VCC pins, and ADC pins into arrays to make it easier
  // to traverse through each one to measure the moisture level and
  // determine if the respective LED needs to turn on
  int ledPins[] = { LED1, LED2, LED3, LED4, LED5, LED6 };
  int vccPins[] = { VCC1, VCC2, VCC3, VCC4, VCC5, VCC6 };
  int adcPins[] = { A5,   A4,   A3,   A2,   A1,   A0   };

  // iterate through each sensor and determine if the
  // plant needs to be watered
  for (int i = 0; i < MAX_NUM_PLANTS; i++)
  {
    // if the current plant moisture level needs to be watered
    // turn on the indicator LED
    // otherwise, turn off the indicator LED
    if (NeedToWater(vccPins[i], adcPins[i]))
    {
      digitalWrite(ledPins[i], HIGH);
    }
    else
    {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

/*************************************************************************************
 * Function: NeedToWater
 * 
 * Description: Takes the current sensor VCC pin and relative ADC pin to determine
 *              if the moisture level of the respective plant is low.
 *              
 * Parameters:
 *      vccPin - The sensor voltage pin that applies the voltage to the sensor
 *      adcPin - ADC pin of the MCU to query the measurement from
 * 
 * Returns:
 *      true, if the plant needs to be watered
 *      false, if the plant does not need to be watered yet
 **************************************************************************************/
bool NeedToWater(int vccPin, int adcPin)
{
  // variable to return
  bool flag = false;

  Serial.print("Turn on sensor voltage ");
  Serial.print(vccPin);
  Serial.print('\n');
  
  // output the sensor voltage to turn the sensor on
  // this will only be on temporarily to prevent corroding the sensor probes
  digitalWrite(vccPin, HIGH);

  Serial.print("Delay to allow sensor to settle");
  
  // allow some delay to allow the sensor to settle
  delay(DELAY_TIME);

  Serial.print("Measure ADC pin ");
  Serial.print(adcPin);
  Serial.print('\n');
  
  // measure the voltage at the ADC pin
  int measurement = analogRead(adcPin);

  Serial.print("Measured: ");
  Serial.print(adcPin * ADC_VOLTAGE_CONVERTER);
  Serial.print('\n');
  
  // if the measured voltage at the adc pin is less than
  // or equal to a low moisture level, then set the flag variable
  // to true to let the MCU know it is time to water
  if (measurement <= LOW_MOISTURE_LEVEL)
  {
    flag = true;
  }

  Serial.print("Turning off sensor voltage \n");
  
  // turn off the sensor voltage pin
  digitalWrite(vccPin, LOW);

  Serial.print("Returning ");
  Serial.print(flag);
  Serial.print('\n');
  
  // return if the plan needs to be watered
  return flag;
}