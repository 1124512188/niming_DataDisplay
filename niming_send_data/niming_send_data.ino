
void uint16_usart1_niming_report(uint16_t source_addr, uint16_t target_addr , uint16_t fun_code , uint16_t len ,uint16_t *data)
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
    send_buf[i + 6] = data[i];        //获得数据内容
  for (i = 0; i < len + 6; i++){
    send_buf[len + 6] += send_buf[i];       //计算校验和
    send_buf[len + 7] += send_buf[len + 6]; //附加校验
  }
  for (i = 0; i < len + 8; i++){
    Serial.write(send_buf[i]);//发送数据到串口1
  } 
}

/*
 *  发送两个uint16_t类型的数据
 *  数据区使用小端模式，低字节在前
 *  根据灵活数据页面中的数据长度可知两个uint16_t类型的数据长度一共为4
 *  所以函数中的buf的长度为4，遵循小端模式，分别计算一个uint16_t类型数据的高低字节，并有低到高的顺序存入数组中
 */
void uint16_pressure_send_data(uint16_t pressure,uint16_t send_pressure)
{
  uint16_t buf[4];
  buf[0] = pressure & 0xFF;
  buf[1] = (pressure >> 8) & 0xFF;
  buf[2] = send_pressure & 0xFF;
  buf[3] = (send_pressure >> 8) & 0xFF;
  uint16_usart1_niming_report(0xFD,0xFE,0xF1,4,buf);
}


void floatToBytes(float value, byte* byteArray) {
  // 将float类型的数据转换为字节数组
  memcpy(byteArray, &value, sizeof(float));
}

void float_usart1_niming_report(uint16_t source_addr, uint16_t target_addr , uint16_t fun_code , uint16_t len ,uint16_t *data)
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
    send_buf[i + 6] = data[i];        //获得数据内容
  for (i = 0; i < len + 6; i++){
    send_buf[len + 6] += send_buf[i];       //计算校验和
    send_buf[len + 7] += send_buf[len + 6]; //附加校验
  }
  for (i = 0; i < len + 8; i++){
    Serial.write(send_buf[i]);//发送数据到串口1
  } 
}



void float_pressure_send_data(float pressure,float send_pressure)
{
  uint16_t buf[8];
  byte byteArray1[sizeof(float)];
  byte byteArray2[sizeof(float)];
  floatToBytes(pressure, byteArray1); 
  floatToBytes(send_pressure, byteArray2); 

  buf[0] = byteArray1[0];
  buf[1] = byteArray1[1];
  buf[2] = byteArray1[2];
  buf[3] = byteArray1[3];

  buf[4] = byteArray2[0];
  buf[5] = byteArray2[1];
  buf[6] = byteArray2[2];
  buf[7] = byteArray2[3];

  float_usart1_niming_report(0xFD,0xFE,0xF1,8,buf);
}





void setup() {
  Serial.begin(115200);
}

void loop() {
  //测试float类型数据发送到匿名串口助手
  for(int i = 0; i < 180; i++){
    float voltage = 2.3 * i ;
    float_pressure_send_data(voltage,voltage);
    delay(10);
  }


  //测试uint16_t类型数据发送到匿名串口助手
  // for(int i = 0; i < 180; i++){
  //   uint16_t voltage = 2 * i ;
  //   uint16_pressure_send_data(voltage,voltage);
  //   delay(10);
  // }
}



