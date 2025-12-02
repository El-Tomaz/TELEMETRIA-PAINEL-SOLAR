/*

  Basic.pde - example using ModbusMaster library

  Library:: ModbusMaster
  Author:: Doc Walker <4-20ma@wvfans.net>

  Copyright:: 2009-2016 Doc Walker

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*/

#include <ModbusMaster.h>


#define DE 18
#define RE 19
// instantiate ModbusMaster object
ModbusMaster node;


void preTransmission() {
  digitalWrite(RE, HIGH);
  digitalWrite(DE, HIGH);
}

void posTransmission() {
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
}

void setup() {
  // use Serial (port 0); initialize Modbus communication baud rate
  Serial.begin(115200);
  Serial1.begin(4800, SERIAL_8N1, 16,17);

  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);


  posTransmission();
  // communicate with Modbus slave ID 2 over Serial (port 0)
  node.begin(1, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(posTransmission);

}


void loop() {

  uint8_t result = node.readHoldingRegisters(0,1);
    delay (1000);

  Serial.println(result);
  // do something with data if read is successful
  if (result == node.ku8MBSuccess) {
   Serial.println("deu bom!!");

   Serial.println(((uint8_t) node.getResponseBuffer(0)));
  }else{
    Serial.println("deu ruim!!");
  }
}
