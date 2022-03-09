#include "microRTOS_interface.h"

moduleMessage microRTOS_interface::parse(const char* message)
{
  moduleMessage returnMessage;
  DynamicJsonDocument msg(128);
  deserializeJson(msg, message, DeserializationOption::NestingLimit(10));
  returnMessage.state = (const char*)msg["state"];
  serializeJson(msg["data"], returnMessage.data);
  return returnMessage;
}

moduleMessage microRTOS_interface::parse(String message)
{
  return microRTOS_interface::parse(message.c_str());
}