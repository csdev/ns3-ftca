/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Christopher Sang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef NODE_SYSTEM_H_
#define NODE_SYSTEM_H_

#include <iostream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/physim-wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

#include "ns3/qos-wifi-mac-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/ssid.h"

#include "ns3/trace-helper.h"

namespace sim {

using namespace ns3;

/*
 * Defines a system of WLAN nodes. Provides methods for setting up common
 * network layouts and setting up the network stack.
 */
class NodeSystem
{
protected:
  PhySimWifiPhyHelper m_wifiPhy;
  //QosWifiMacHelper m_qosWifiMac;
  QosWifiMacHelper m_wifiMac;
  WifiHelper m_wifi;

  NodeContainer m_nodes;
  NodeContainer m_apNodes;
  NodeContainer m_staNodes;

  NetDeviceContainer m_devices;
  NetDeviceContainer m_apDevices;
  NetDeviceContainer m_staDevices;

  Ipv4InterfaceContainer m_wifiInterfaces;

private:
  double m_endTime;

public:
  NodeSystem ();
  virtual ~NodeSystem ();

  virtual void Configure ();

  virtual void ConnectAllDownlinkTcp (int basePort, double startTime, double endTime, double offsetTime, uint8_t qosTid = 0);
  virtual void ConnectAllUplinkTcp (int basePort, double startTime, double endTime, double offsetTime, uint8_t qosTid = 0);

  virtual void ConnectTcp (int sourceId, int destId, int port, double startTime, double endTime, uint8_t qosTid = 0);
  virtual void ConnectUdp (int sourceId, int destId, int port, double startTime, double endTime);

  virtual void EnablePcap ();
  virtual void EnablePcap (int nodeId);
  virtual void EnablePcapPromisc ();
  virtual void EnablePcapPromisc (int nodeId);

  virtual double GetEndTime() const;
  virtual uint32_t GetN () const;
  virtual uint32_t GetApId () const = 0;
  virtual uint32_t GetServerId () const = 0;

protected:
  virtual void MakeNodes () = 0;
  virtual void ConfigurePhy ();
  virtual void ConfigureMac ();
  virtual void ConfigureDevices () = 0;
  virtual void ConfigureIpv4 ();
  virtual void ConfigureLayout () = 0;
  virtual std::string GetPcapFileName () const = 0;
};

} /* namespace sim */
#endif /* NODE_SYSTEM_H_ */
