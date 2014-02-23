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

#include "ns3/log.h"
#include "ns3/address.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/tcp-socket-factory.h"
#include "qos-bulk-send-application.h"

#include "ns3/qos-tag.h"

NS_LOG_COMPONENT_DEFINE ("QosBulkSendApplication");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (QosBulkSendApplication);

TypeId
QosBulkSendApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QosBulkSendApplication")
      .SetParent<BulkSendApplication> ()
      .AddConstructor<QosBulkSendApplication> ()
      .AddAttribute ("QosTid", "The QoS TID value that specifies the priority of frames sent by this application.",
          UintegerValue (0),
          MakeUintegerAccessor (&QosBulkSendApplication::m_qosTid),
          MakeUintegerChecker<uint8_t> (0, 15)) //TODO EDCA TID should be <= 7
  ;
  return tid;
}

QosBulkSendApplication::QosBulkSendApplication () :
    BulkSendApplication (),
    m_qosTid (0)
{
}

QosBulkSendApplication::~QosBulkSendApplication ()
{
}

void
QosBulkSendApplication::SetQosTid (uint8_t tid)
{
  m_qosTid = tid;
}

void
QosBulkSendApplication::SendData ()
{
  NS_LOG_FUNCTION (this);

  while (m_maxBytes == 0 || m_totBytes < m_maxBytes)
    { // Time to send more
      uint32_t toSend = m_sendSize;
      // Make sure we don't send too many
      if (m_maxBytes > 0)
        {
          toSend = std::min (m_sendSize, m_maxBytes - m_totBytes);
        }
      NS_LOG_LOGIC ("sending packet at " << Simulator::Now ());

      Ptr<Packet> packet = Create<Packet> (toSend);
      QosTag tag;
      tag.SetTid (m_qosTid);
      packet->AddPacketTag (tag);

      m_txTrace (packet);
      int actual = m_socket->Send (packet);
      if (actual > 0)
        {
          m_totBytes += actual;
        }
      // We exit this loop when actual < toSend as the send side
      // buffer is full. The "DataSent" callback will pop when
      // some buffer space has freed ip.
      if ((unsigned)actual != toSend)
        {
          break;
        }
    }
  // Check if time to close (all sent)
  if (m_totBytes == m_maxBytes && m_connected)
    {
      m_socket->Close ();
      m_connected = false;
    }
}

} /* namespace ns3 */
