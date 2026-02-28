#pragma once

#include "Vars.h"
#include "Properties.h"

bool parseMac(const char *macStr, uint8_t *macOut)
{
  int values[6];
  if (sscanf(macStr, "%x:%x:%x:%x:%x:%x",
             &values[0], &values[1], &values[2],
             &values[3], &values[4], &values[5]) != 6)
  {
    return false;
  }
  for (int i = 0; i < 6; i++)
    macOut[i] = (uint8_t)values[i];
  return true;
}

bool sendWOL(uint8_t *mac)
{
  uint8_t packet[102];

  memset(packet, 0xFF, 6);
  for (int i = 6; i < 102; i += 6)
    memcpy(&packet[i], mac, 6);

  IPAddress broadcastIP = ~WiFi.subnetMask() | WiFi.gatewayIP();

  Serial.print("Broadcast IP: ");
  Serial.println(broadcastIP);

  if (udp.beginPacket(broadcastIP, WOL_PORT) == 0)
    return false;
  if (udp.write(packet, sizeof(packet)) != sizeof(packet))
    return false;
  if (!udp.endPacket())
    return false;

  return true;
}