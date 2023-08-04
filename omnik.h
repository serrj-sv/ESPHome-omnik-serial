#include "esphome.h"

class omnik : public Component, public UARTDevice {
 public:
  omnik(UARTComponent *parent) : UARTDevice(parent) {}
  
  struct OmnkMetrics {
    uint16_t temp;
    uint16_t pv1_v;
    uint16_t pv2_v;
    uint16_t pv3_v;
    uint16_t pv1_i;
    uint16_t pv2_i;
    uint16_t pv3_i;
    uint16_t ac1_i;
    uint16_t ac2_i;
    uint16_t ac3_i;
    uint16_t ac1_v;
    uint16_t ac2_v;
    uint16_t ac3_v;
    uint16_t ac1_f;
    uint16_t ac1_p;
    uint16_t ac2_f;
    uint16_t ac2_p;
    uint16_t ac3_f;
    uint16_t ac3_p;
    uint16_t e_today;
    uint32_t e_total;
    uint32_t h_total;
    uint16_t mode;
    uint16_t fault_gv;
    uint16_t fault_gf;
    uint16_t fault_gz;
    uint16_t fault_tmp;
    uint16_t fault_pv;
    uint16_t fault_gfci;
    uint32_t err_msg;
  }__attribute__((__packed__));;

  Sensor *temperature_sensor = new Sensor;
  Sensor *pv1_v_sensor = new Sensor;
  Sensor *pv2_v_sensor = new Sensor;
  Sensor *pv1_i_sensor = new Sensor;
  Sensor *pv2_i_sensor = new Sensor;
  Sensor *ac1_i_sensor = new Sensor;
  Sensor *ac1_v_sensor = new Sensor;
  Sensor *ac1_f_sensor = new Sensor;
  Sensor *ac1_p_sensor = new Sensor;
  Sensor *e_today_sensor = new Sensor;
  Sensor *e_total_sensor = new Sensor;
  Sensor *h_total_sensor = new Sensor;
  
  void setup() override {
  }
  

  void loop() override {
    static int pos = 0;
    static uint8_t msg[266]; //9 bytes header + 255 bytes data (max) + 2 bytes crc
    static uint8_t data_size;
    static uint32_t msg_timer = 0;
    
    //new message, starting timer
    if ((pos == 0) && available()) msg_timer = millis(); 

    //re-setting read marker due to timeout
    if ((msg_timer > 0) && (millis() - msg_timer > 300)) { 
      msg_timer = 0;
      pos = 0;
    }
    
    //read bytes from UART buffer
    while (available()) {
      uint8_t b = read();
      msg[pos] = b;
      pos++; 
    }

    //lets check if we received enough bytes to get data size
    //also check message header
    if (pos > 8) {
      if ((msg[0] == 0x3a) && (msg[1] == 0x3a)) {
        data_size = msg[8];
      } else {
        pos = 0;
      }
    }
    
    if (pos >= data_size + 11) {
      
      //just in case if we reveived extra garbage bytes
      uint16_t msg_size = data_size + 11;
      
      //calculating and checking checksum
      uint16_t chksum = (msg[msg_size-2]<<8) + (msg[msg_size-1]);
      uint16_t cm=0;

      for (int i = 0; i < msg_size-2; i++) cm = cm + msg[i];

      uint8_t func_code;
      if (cm == chksum) {
        func_code = msg[7];
        id(registration_timeout).stop();
      } else {
        func_code = 0xfe;
      }

      switch(func_code) {
        case 0x80: {
            ESP_LOGD("omnik","Finishing registration");
            char inv_sn[data_size+1];
            memcpy(inv_sn, msg+9, data_size);
            inv_sn[data_size]= '\0';        // Null-terminate the string
            ESP_LOGD("omnik","Finishing registration, Inverter SN: %s", inv_sn);
            id(inverter_serial).publish_state(inv_sn);
            //swap sender/receiver
            msg[2] = 0x1;
            msg[4] = 0x0;
            //function code
            msg[7] = 0x1;
            //data size
            msg[8]++;
            //correcting checksum
            chksum = chksum - 0x7D;
            // confirm registration
            msg[9 + data_size] = 0x1;
            //message lenght now increased
            msg_size++;
            //writing new checksum
            msg[msg_size - 2] = (chksum & 0xff00) >> 8;
            msg[msg_size - 1] = chksum & 0x00ff;
            //sending back
            for (int i = 0; i < msg_size; i++) write_byte(msg[i]);
            break;
          }    
        case 0x81:
          id(inverter_isRegistered).publish_state(true);
          break;
        case 0x90: {
            OmnkMetrics m;
            memcpy(&m, msg+9, sizeof(m));
        
            float temp = (float)htons(m.temp)/10;
            temperature_sensor->publish_state(temp);
        
            float pv1_v = (float)htons(m.pv1_v)/10;
            pv1_v_sensor->publish_state(pv1_v);
        
            float pv2_v = (float)htons(m.pv2_v)/10;
            pv2_v_sensor->publish_state(pv2_v);
        
            float pv1_i = (float)htons(m.pv1_i)/10;
            pv1_i_sensor->publish_state(pv1_i);
        
            float pv2_i = (float)htons(m.pv2_i)/10;
            pv2_i_sensor->publish_state(pv2_i);
        
            float ac1_i = (float)htons(m.ac1_i)/10;
            ac1_i_sensor->publish_state(ac1_i);
        
            float ac1_v = (float)htons(m.ac1_v)/10;
            ac1_v_sensor->publish_state(ac1_v);
        
            float ac1_f = (float)htons(m.ac1_f)/100;
            ac1_f_sensor->publish_state(ac1_f);
        
            short ac1_p = htons(m.ac1_p);
            ac1_p_sensor->publish_state(ac1_p);
        
            float e_today = (float)htons(m.e_today)/100;
            e_today_sensor->publish_state(e_today);
        
            float e_total = (float)htonl(m.e_total)/10;
            e_total_sensor->publish_state(e_total);
        
            long h_total = htonl(m.h_total);
            h_total_sensor->publish_state(h_total);
            break;
          }    
        case 0xfe:
          ESP_LOGD("omnik","Wrong checksum, ignoring");
          break;
        default:
          ESP_LOGD("omnik","Unknown code received: %x, ignoring", func_code);
      }  
      pos = 0;
    } 
  } //loop
}; //class