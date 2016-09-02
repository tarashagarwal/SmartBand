#include <VirtualWire.h>
void setup()
{
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_rx_pin(1);
    vw_setup(4000);  // Bits per sec
    pinMode(0, OUTPUT);
    
    pinMode(10,OUTPUT);
    pinMode(10,LOW);

    vw_rx_start();       // Start the receiver PLL running
}
    void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      if((buf[0]=='B')&&(buf[1]=='1')){   
      digitalWrite(0,1);
      delay(1000);
      }  
      }
      else{
  digitalWrite(0,0);
    }

}
