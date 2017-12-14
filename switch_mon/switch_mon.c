#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

/* The power sense pin is physical pin 26, wiringPi pin 11
 */
#define PWR_SENSE_PIN 11

typedef enum
{
  TURNED_ON,
  PROCESS_KILLED,
} shutdownState_t;

int main (void)
{
  shutdownState_t shutdownState = TURNED_ON;
  /* Assume the pin is low at the start */
  uint8_t pinHistory = 0x00;

  /* Initialize the wiring library */
  wiringPiSetup();

  /* This should already be initialized by /etc/rc.local, but just
   * in case, set the power sense pin as an input with pullup
   */
  pinMode(PWR_SENSE_PIN, INPUT);
  pullUpDnControl (PWR_SENSE_PIN, PUD_UP);

  //system("echo starting_shutdown >> /home/pi/shutdown_log.txt");

  /* To not hog the CPU */
  while (1)
  {
    /* Read the pin once a second */
    pinHistory <<= 1;
    if(HIGH == digitalRead(PWR_SENSE_PIN))
    {
      pinHistory |= 1;
    }

    switch(shutdownState)
    {
      case TURNED_ON:
        {
          /* If the pin has been high for four consecutive seconds */
          if((pinHistory & 0x0F) == 0x0F)
          {
            /* Kill emulation station, but leave the GPIO on */
            //system("echo killing_progs >> /home/pi/shutdown_log.txt");
            system("killall emulationstation");
            system("killall retroarch");
            shutdownState = PROCESS_KILLED;
          }
          break;
        }
      case PROCESS_KILLED:
        {
          /* If the pin has been high for four consecutive seconds
           * then it's safe to turn off the GPIO and shut down
           */
          if((pinHistory & 0x0F) == 0x00)
          {
            //system("echo shutting_down >> /home/pi/shutdown_log.txt");
            /* Turn the GPIO off */
            digitalWrite(4, 0);
            /* Start the shutdown process */
            system("shutdown -P now");
            return 0;
          }
          break;
        }
    }

    /* Don't hog the CPU */
    sleep(1);
  }

  return 0 ;
}
