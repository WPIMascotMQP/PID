#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/igmp.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "soc/rtc_periph.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"
#include "esp_intr_alloc.h"

#define GPIO_HANDSHAKE 2
#define GPIO_MOSI 12
#define GPIO_MISO 13
#define GPIO_SCLK 15
#define GPIO_CS 14

#ifdef CONFIG_IDF_TARGET_ESP32
#define SENDER_HOST HSPI_HOST
#define DMA_CHAN    2
#endif

#define Motor 16
#define MotorDir 17

#define PB_PIN = 15

const int touchPin1 = 4;
const int touchPin2 = 2;
int threshold = 40;
int touchValue1;
int touchValue2;

int currentdelay = 200;
int currentdelaylarge = 200000;
int timetrack = 0;

String inputString = "";
String arg1, arg2;
char receivedChar;

bool limitswitchflag;
bool debounce;

void IRAM_ATTR ISR() {
  
    if(debounce == false){
    Serial.println("LIMIT SWITCH TRIGGERED");
    limitswitchflag = true;
    }
    //debounce IMPLIMENT
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(MotorDir, OUTPUT);
  pinMode(Motor, OUTPUT);
  digitalWrite(MotorDir, LOW);
  attachInterrupt(18, ISR, FALLING);
}

/*
void touchRead(int V)
{
  touchValue1 = touchRead(touchPin1);
  touchValue2 = touchRead(touchPin2);
  
}
*/

void loop() {
  /*
  timetrack = timetrack+1;
  if(timetrack % 100 == 0 && timetrack != 0){
  touchValue1 = touchRead(touchPin1);
  touchValue2 = touchRead(touchPin2);
  }
  //thread M1(touchRead,0);
  //Serial.println(touchValue1);

  
  if (currentdelaylarge > 300000) {
    currentdelaylarge = 300000;
  }
  if (currentdelaylarge < 1000) {
    currentdelaylarge = 1000;
  }
  
  if (touchValue1 < threshold) {
    currentdelaylarge = currentdelaylarge - 10000;
    //Serial.println("reducing delay number");
    //Serial.println(currentdelay);
  }
  if (touchValue2 < threshold) {
    currentdelaylarge = currentdelaylarge + 10000;
    //Serial.println("increasing delay number");
    //Serial.println(currentdelay);
  }
  currentdelay = currentdelaylarge / 1000;
  */
  if (Serial.available() > 0) {
   receivedChar = Serial.read();
   currentdelay = currentdelay + (receivedChar + '0');
   Serial.println("NUMBER NUMBER NUMBER");
   Serial.println(receivedChar);
   Serial.println(currentdelay);
   }

  if (currentdelay > 300) {
    currentdelay = 300;
  }
  if (currentdelay < 10) {
    currentdelay = 10;
  }
  

 
  delayMicroseconds(currentdelay);
  //Serial.println(currentdelay);
  //delayMicroseconds(100);
  if(limitswitchflag == false){
  digitalWrite(Motor,HIGH);
  }
  delayMicroseconds(5);
  digitalWrite(Motor,LOW);
  
}

/*
void handle_cmd() {
  inputString.trim(); // removes beginning and ending white spaces
  char cmd = inputString.charAt(0); // for comprehension: cmd is technically arg0
  
  if ((cmd)) {
   
    // Be warned, this is a VERY poor and inefficient way to implement multiple args, but we only need max 2, and this is quick  
    int cur  = inputString.indexOf(' ');
    int cur1 = inputString.indexOf(',');
   
    if ((cur1 != -1)) {  
      arg1 = inputString.substring(cur + 1, cur1);
      arg2 = inputString.substring(cur1 + 1);  
    } else {
      arg1 = inputString.substring(cur + 1); 
      arg2 = "";
    }
    
    switch (cmd) {
      case 'p': 
                currentdelay = currentdelay + (arg1.toInt()); 
                Serial.println("+VE delay number");
                break;
      case 'n': 
                currentdelay = currentdelay + (arg1.toInt());
                Serial.println("+VE delay number");
 
                break;
      default: 
               break;
    }
    inputString = "";
  }  
}
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == 13) {
      handle_cmd();
    }
  }
}
*/



//Called after a transaction is queued and ready for pickup by master. We use this to set the handshake line high.
void my_post_setup_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1<<GPIO_HANDSHAKE));
}

//Called after transaction is sent/received. We use this to set the handshake line low.
void my_post_trans_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, (1<<GPIO_HANDSHAKE));
}

//Main application
void app_main(void)
{
    int n=0;
    esp_err_t ret;

    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .mode=0,
        .spics_io_num=GPIO_CS,
        .queue_size=3,
        .flags=0,
        .post_setup_cb=my_post_setup_cb,
        .post_trans_cb=my_post_trans_cb
    };

    //Configuration for the handshake line
    gpio_config_t io_conf={
        .intr_type=GPIO_INTR_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .pin_bit_mask=(1<<GPIO_HANDSHAKE)
    };

    //Configure handshake line as output
    gpio_config(&io_conf);
    //Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    //Initialize SPI slave interface
    ret=spi_slave_initialize(RCV_HOST, &buscfg, &slvcfg, DMA_CHAN);
    assert(ret==ESP_OK);

    WORD_ALIGNED_ATTR char sendbuf[129]="";
    WORD_ALIGNED_ATTR char recvbuf[129]="";
    
    memset(recvbuf, 0, 33);
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));

    while(1) {
        //Clear receive buffer, set send buffer to something sane
        memset(recvbuf, 0xA5, 129);
        sprintf(sendbuf, "This is the receiver, sending data for transmission number %04d.", n);

        //Set up a transaction of 128 bytes to send/receive
        t.length=128*8;
        t.tx_buffer=sendbuf;
        t.rx_buffer=recvbuf;
        This call enables the SPI slave interface to send/receive to the sendbuf and recvbuf. The transaction is
        initialized by the SPI master, however, so it will not actually happen until the master starts a hardware transaction
        by pulling CS low and pulsing the clock etc. In this specific example, we use the handshake line, pulled up by the
        .post_setup_cb callback that is called as soon as a transaction is ready, to let the master know it is free to transfer
        data.
        
        ret=spi_slave_transmit(RCV_HOST, &t, portMAX_DELAY);

        //spi_slave_transmit does not return until the master has done a transmission, so by here we have sent our data and
        //received data from the master. Print it.
        printf("Received: %s\n", recvbuf);
        n++;
    }

}
