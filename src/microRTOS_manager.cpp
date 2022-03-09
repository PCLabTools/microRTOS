#include "microRTOS_manager.h"

microRTOS_manager::microRTOS_manager()
{
  #if defined microRTOS_DEBUG && microRTOS_DEBUG > 2
    printf("microRTOS_manager()\n");
  #endif
  for (size_t i = 0; i < microRTOS_MAX_MODULES; i++)
  {
    names[i] = "";
  }
}

uint8_t microRTOS_manager::reg(const char* name, xQueueHandle* command, xQueueHandle* response = NULL)
{
  #if defined microRTOS_DEBUG && microRTOS_DEBUG > 2
    printf("microRTOS_manager: reg(name=\"%s\", command=pQueue[%p], response=pQueue[%p])\n", name, command, response);
  #endif
  for (size_t i = 0; i < microRTOS_MAX_MODULES; i++)
  {
    if(!strcmp(names[i],""))
    {
      names[i] = name;
      hCommands[i] = command;
      hResponses[i] = response;
      break;
    }
  }
  return 0;
}

uint8_t microRTOS_manager::reg(String name, xQueueHandle* command, xQueueHandle* response = NULL)
{
  return microRTOS_manager::reg(name.c_str(), command, response);
}

uint8_t microRTOS_manager::dereg(const char* name)
{
  #if defined microRTOS_DEBUG && microRTOS_DEBUG > 2
    printf("microRTOS_manager: dereg(name=\"%s\")\n", name);
  #endif
  for (size_t i = 0; i < microRTOS_MAX_MODULES; i++)
  {
    if(!strcmp(names[i],name))
    {
      names[i] = "";
      hCommands[i] = NULL;
      hResponses[i] = NULL;
      break;
    }
  }
  
  return 0;
}

uint8_t microRTOS_manager::dereg(String name)
{
  return microRTOS_manager::dereg(name.c_str());
}

const char* microRTOS_manager::cmd(const char* name, const char* command, TickType_t timeout)
{
  #if defined microRTOS_DEBUG && microRTOS_DEBUG > 2
    printf("microRTOS_manager: cmd(name=\"%s\", command=\"%s\", timeout=%d)\n", name, command, timeout);
  #endif
  const char* response;
  for (size_t i = 0; i < microRTOS_MAX_MODULES; i++)
  {
    if(!strcmp(names[i],name))
    {
      xQueueSend(*hCommands[i], &command, portMAX_DELAY);
      xQueueReceive(*hResponses[i], &response, timeout);
      break;
    }
  }
  return response;
}

String microRTOS_manager::cmd(String name, String command, TickType_t timeout)
{
  return String(microRTOS_manager::cmd(name.c_str(), command.c_str(), timeout));
}

uint8_t microRTOS_manager::cmd_all(const char* command, TickType_t timeout)
{
  #if defined microRTOS_DEBUG && microRTOS_DEBUG > 2
    printf("microRTOS_manager: cmd_all(command=\"%s\", timeout=%d)\n", command, timeout);
  #endif
  const char* response;
  for (size_t i = 0; i < microRTOS_MAX_MODULES; i++)
  {
    xQueueSend(*hCommands[i], &command, portMAX_DELAY);
    xQueueReceive(*hResponses[i], &response, timeout);
  }
  return 0;
}

uint8_t microRTOS_manager::cmd_all(String command, TickType_t timeout)
{
  return microRTOS_manager::cmd_all(command.c_str(), timeout);
}