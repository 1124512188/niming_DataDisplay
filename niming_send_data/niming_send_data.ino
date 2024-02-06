
void usart1_niming_reportV7(uint16_t source_addr, uint16_t target_addr , uint16_t fun_code , uint16_t len ,uint16_t *data)
{
  uint16_t send_buf[32] = {0x00};
  uint16_t i;
  if (len > 28) return; //超过28个字节，无效
  send_buf[0] = 0xAB;                 //帧头0xAB          
  send_buf[1] = source_addr;          //源地址帧      
  send_buf[2] = target_addr;          //目标地址帧   
  send_buf[3] = fun_code;             //功能码        
  send_buf[4] = len & 0xFF;           //获得数据长度帧LEN低端位 
  send_buf[5] = (len >> 8) & 0xFF;    //获得数据长度帧LEN高端位
  for (i = 0; i < len; i++) 
    send_buf[i + 6] = data[i];
  for (i = 0; i < len + 6; i++){
    send_buf[len + 6] += send_buf[i];
    send_buf[len + 7] += send_buf[len + 6];
  }
  for (i = 0; i < len + 8; i++){
    Serial.write(send_buf[i]);//发送数据到串口1
  } 
}

void pressure_send_dataV7(uint16_t pressure,uint16_t send_pressure)
{
  uint16_t buf[4];
  buf[0] = pressure & 0xFF;
  buf[1] = (pressure >> 8) & 0xFF;
  buf[2] = send_pressure & 0xFF;
  buf[3] = (send_pressure >> 8) & 0xFF;
  usart1_niming_reportV7(0xFD,0xFE,0xF1,4,buf);
}



void setup() {
  Serial.begin(115200);
}

void loop() {
  for(int i = 0; i < 180; i++){
    uint16_t voltage = 2 * i;
    pressure_send_dataV7(voltage,voltage);
    delay(10);
  }
}
