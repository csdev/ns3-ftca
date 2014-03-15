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

#ifndef PHYSIM_TRACE_COUNTER_GROUP_H_
#define PHYSIM_TRACE_COUNTER_GROUP_H_

#include <iostream>
#include <list>
#include <string>

#include "ns3/core-module.h"
#include "ns3/cross-layer-module.h"
#include "ns3/physim-wifi-module.h"

#include "trace-counter-group.h"

#include "trace-utils.h" //TODO

//FIXME logging in header file???
//NS_LOG_COMPONENT_DEFINE("physim-trace-counter-group");

namespace sim {

using namespace ns3;

/*
 * Interface to low-level PhySim trace sources.
 *
 * Counts frame collisions and capture events based on the location of the
 * frame overlap.
 */
template <int N>
class PhySimTraceCounterGroup : public sim::TraceCounterGroup<N>
{
protected:
  PhySimTraceCounterSet<N> m_mainCounters;
  PhySimTraceCounterSet<N> m_counters[DpiTag::DPI_PACKET_TYPES];

public:
  PhySimTraceCounterGroup();

  virtual
  ~PhySimTraceCounterGroup();

  virtual void
  enableCounters(uint32_t nodeId = -1);

  virtual void
  printMainCounters(uint32_t s, uint32_t d, std::ostream& out = std::cout) const;

  virtual void
  printCsvPacketCounters(enum DpiTag::PacketType counterType, uint32_t s, uint32_t d, std::ostream& out = std::cout) const;

  virtual void
  printCounters(enum DpiTag::PacketType counterType, uint32_t s, uint32_t d, std::ostream& out = std::cout) const;

  virtual void
  printCounters(uint32_t s, uint32_t d, std::ostream& out = std::cout) const;

private:
// List all callback functions available for PHY layer tracing
#define _TRACE_CALLBACK_LISTING                 \
  _TRACE_TX_CALLBACK(Tx)                        \
  _TRACE_CALLBACK(StartRx)                      \
  _TRACE_ERROR_CALLBACK(StartRxError)           \
  _TRACE_CALLBACK(EnergyDetectionFailed)        \
  _TRACE_CALLBACK(PreambleOk)                   \
  _TRACE_ERROR_CALLBACK(PreambleError)          \
  _TRACE_CALLBACK(HeaderOk)                     \
  _TRACE_ERROR_CALLBACK(HeaderError)            \
  _TRACE_CALLBACK(RxOk)                         \
  _TRACE_ERROR_CALLBACK(RxError)

// Function template for TX event callbacks
#define _TRACE_TX_CALLBACK(x) _TRACE_CALLBACK(x)

// Function template for RX event callbacks
#define _TRACE_CALLBACK(x) \
  void Phy ## x ## Trace (std::string context, Ptr<const Packet> p, Ptr<const PhySimWifiPhyTag> tag);

// Function template for RX error callbacks
#define _TRACE_ERROR_CALLBACK(x) \
  void Phy ## x ## Trace (std::string context, Ptr<const Packet> p, \
      Ptr<const PhySimWifiPhyTag> tag, enum PhySimWifiPhy::ErrorReason reason);

    // Emit declarations for all callback functions previously listed
    _TRACE_CALLBACK_LISTING

#undef _TRACE_TX_CALLBACK
#undef _TRACE_CALLBACK
#undef _TRACE_ERROR_CALLBACK


protected:

// Function for updating TX event counters
#define _TRACE_TX_CALLBACK(x) \
    void Phy ## x ## Update (PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d);

// Function for updating RX event counters
#define _TRACE_CALLBACK(x) \
  void Phy ## x ## Update (PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d, bool isCaptured);

// Function for updating RX error counters
#define _TRACE_ERROR_CALLBACK(x) \
  void Phy ## x ## Update (PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d, enum PhySimWifiPhy::ErrorReason reason);

    // Emit declarations for all counter update functions
    _TRACE_CALLBACK_LISTING

#undef _TRACE_TX_CALLBACK
#undef _TRACE_CALLBACK
#undef _TRACE_ERROR_CALLBACK

};

template <int N>
PhySimTraceCounterGroup<N>::PhySimTraceCounterGroup()
{
}

template <int N>
PhySimTraceCounterGroup<N>::~PhySimTraceCounterGroup()
{
}

template <int N>
void
PhySimTraceCounterGroup<N>::enableCounters(uint32_t nodeId)
{
  Config::Connect (sim::TraceUtils::phySimWifiTracePath("Tx", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyTxTrace, this));

  Config::Connect (sim::TraceUtils::phySimWifiTracePath("StartRx", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyStartRxTrace, this));

  Config::Connect (sim::TraceUtils::phySimWifiTracePath("StartRxError", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyStartRxErrorTrace, this));

  Config::Connect (sim::TraceUtils::phySimWifiTracePath("EnergyDetectionFailed", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyEnergyDetectionFailedTrace, this));


  Config::Connect (sim::TraceUtils::phySimWifiTracePath("PreambleOk", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyPreambleOkTrace, this));

  Config::Connect (sim::TraceUtils::phySimWifiTracePath("PreambleError", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyPreambleErrorTrace, this));

  Config::Connect (sim::TraceUtils::phySimWifiTracePath("HeaderOk", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyHeaderOkTrace, this));

  Config::Connect (sim::TraceUtils::phySimWifiTracePath("HeaderError", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyHeaderErrorTrace, this));

  Config::Connect (sim::TraceUtils::phySimWifiTracePath("RxOk", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyRxOkTrace, this));

  Config::Connect (sim::TraceUtils::phySimWifiTracePath("RxError", nodeId),
      MakeCallback(&sim::PhySimTraceCounterGroup<N>::PhyRxErrorTrace, this));
}

template <int N>
void
PhySimTraceCounterGroup<N>::printMainCounters(uint32_t s, uint32_t d, std::ostream& out) const
{
  out << "Main Counters:" << std::endl;
  m_mainCounters.printCounters(s, d, out);
}

template <int N>
void
PhySimTraceCounterGroup<N>::printCsvPacketCounters(enum DpiTag::PacketType counterType, uint32_t s, uint32_t d, std::ostream& out) const
{
  out << m_counters[counterType].traceDetectOkCount[s][d] << ", " << m_counters[counterType].tracePayloadOkCount[s][d] << "\n";
}

template <int N>
void
PhySimTraceCounterGroup<N>::printCounters(enum DpiTag::PacketType counterType, uint32_t s, uint32_t d, std::ostream& out) const
{
  out << "Counters for PacketType = " << DpiTag::PacketTypeName[counterType] << std::endl;
  m_counters[counterType].printCounters(s, d, out);
}

template <int N>
void
PhySimTraceCounterGroup<N>::printCounters(uint32_t s, uint32_t d, std::ostream& out) const
{
  out << "Main Counters:" << std::endl;
  m_mainCounters.printCounters(s, d, out);

  for (int i = 0; i < DpiTag::DPI_PACKET_TYPES; i++)
    {
      out << "Counter " << i << ": PacketType = " << DpiTag::PacketTypeName[i] << std::endl;
      m_counters[i].printCounters(s, d, out);
    }
}

#define _TRACE_TX_CALLBACK(x) \
  template <int N>      \
  void                  \
  PhySimTraceCounterGroup<N>::Phy ## x ## Trace (std::string context, Ptr<const Packet> p, \
      Ptr<const PhySimWifiPhyTag> tag)                                          \
  {                                                                             \
    uint32_t sourceId = tag->GetTxNetDevice()->GetNode()->GetId();              \
    uint32_t destId = sourceId;                                                 \
    Phy ## x ## Update (m_mainCounters, sourceId, destId);                      \
    DpiTag t;                                                                   \
    if (p->PeekPacketTag (t))                                                   \
      {                                                                         \
        Phy ## x ## Update (m_counters[t.GetPacketType()], sourceId, destId);   \
      }                                                                         \
  }

#define _TRACE_CALLBACK(x) \
  template <int N>      \
  void                  \
  PhySimTraceCounterGroup<N>::Phy ## x ## Trace (std::string context, Ptr<const Packet> p,      \
      Ptr<const PhySimWifiPhyTag> tag)                                                          \
  {                                                                                             \
    MacTag addrTag;                                                                             \
    bool hasAddrTag = p->PeekPacketTag (addrTag);                                               \
    if (!hasAddrTag || (addrTag.GetAddress () != tag->GetRxNetDevice()->GetAddress()) )         \
        return;                                                                                 \
    uint32_t sourceId = tag->GetTxNetDevice()->GetNode()->GetId();                              \
    uint32_t destId = tag->GetRxNetDevice()->GetNode()->GetId();                                \
    bool isCaptured = tag->IsCaptured();                                                        \
    Phy ## x ## Update (m_mainCounters, sourceId, destId, isCaptured);                          \
    DpiTag t;                                                                                   \
    if (p->PeekPacketTag (t))                                                                   \
      {                                                                                         \
        Phy ## x ## Update (m_counters[t.GetPacketType()], sourceId, destId, isCaptured);       \
      }                                                                                         \
  }

#define _TRACE_ERROR_CALLBACK(x) \
  template <int N>      \
  void                  \
  PhySimTraceCounterGroup<N>::Phy ## x ## Trace (std::string context, Ptr<const Packet> p,      \
      Ptr<const PhySimWifiPhyTag> tag,                                                          \
      enum PhySimWifiPhy::ErrorReason reason)                                                   \
  {                                                                                             \
    MacTag addrTag;                                                                             \
    bool hasAddrTag = p->PeekPacketTag (addrTag);                                               \
    if (!hasAddrTag || (addrTag.GetAddress () != tag->GetRxNetDevice()->GetAddress()) )         \
        return;                                                                                 \
    uint32_t sourceId = tag->GetTxNetDevice()->GetNode()->GetId();                              \
    uint32_t destId = tag->GetRxNetDevice()->GetNode()->GetId();                                \
    Phy ## x ## Update (m_mainCounters, sourceId, destId, reason);                              \
    DpiTag t;                                                                                   \
    if (p->PeekPacketTag (t))                                                                   \
      {                                                                                         \
        Phy ## x ## Update (m_counters[t.GetPacketType()], sourceId, destId, reason);           \
      }                                                                                         \
  }

  _TRACE_CALLBACK_LISTING

#undef _TRACE_TX_CALLBACK
#undef _TRACE_CALLBACK
#undef _TRACE_ERROR_CALLBACK






template <int N>
void
PhySimTraceCounterGroup<N>::PhyTxUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d)
{
  c.traceTxCount[s][d]++;
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyStartRxUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d, bool _unused_)
{
  c.traceDetectOkCount[s][d]++;
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyStartRxErrorUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d,
    enum PhySimWifiPhy::ErrorReason reason)
{
  switch (reason)
  {
  case PhySimWifiPhy::STATE_SYNC:
    c.traceStateErrorSyncCount[s][d]++;
    break;

  case PhySimWifiPhy::STATE_TX:
    c.traceStateErrorTxCount[s][d]++;
    break;

  case PhySimWifiPhy::STATE_RX:
    c.traceStateErrorRxCount[s][d]++;
    break;

  default:
    //FIXME NS_LOG_ERROR("Unexpected ErrorReason in StartRxError trace");
    break;
  }
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyEnergyDetectionFailedUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d, bool _unused_)
{
  c.traceDetectErrorCount[s][d]++;
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyPreambleOkUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d, bool isCaptured)
{
  c.tracePreambleOkCount[s][d]++;
  if (isCaptured)
    c.tracePreambleOkCaptureCount[s][d]++;
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyPreambleErrorUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d,
    enum PhySimWifiPhy::ErrorReason reason)
{
  switch (reason)
  {
  case PhySimWifiPhy::STATE_TX:
    c.tracePreambleErrorTxCount[s][d]++;
    break;

  case PhySimWifiPhy::STATE_RX:
    c.tracePreambleErrorRxCount[s][d]++;
    break;

  case PhySimWifiPhy::PROCESSING:
    c.tracePreambleErrorProcessingCount[s][d]++;
    break;

  default:
    //FIXME NS_LOG_ERROR("Unexpected ErrorReason in PreambleError trace");
    break;
  }
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyHeaderOkUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d, bool isCaptured)
{
  c.traceHeaderOkCount[s][d]++;
  if (isCaptured)
    c.traceHeaderOkCaptureCount[s][d]++;
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyHeaderErrorUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d,
    enum PhySimWifiPhy::ErrorReason reason)
{
  switch (reason)
  {
  case PhySimWifiPhy::STATE_TX:
    c.traceHeaderErrorTxCount[s][d]++;
    break;

  case PhySimWifiPhy::PROCESSING:
    c.traceHeaderErrorProcessingCount[s][d]++;
    break;

  case PhySimWifiPhy::CAPTURE:
    c.traceHeaderErrorCaptureCount[s][d]++;
    break;

  default:
    //FIXME NS_LOG_ERROR("Unexpected ErrorReason in HeaderError trace");
    break;
  }
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyRxOkUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d, bool isCaptured)
{
  c.tracePayloadOkCount[s][d]++;
  if (isCaptured)
    c.tracePayloadOkCaptureCount[s][d]++;
}

template <int N>
void
PhySimTraceCounterGroup<N>::PhyRxErrorUpdate(PhySimTraceCounterSet<N>& c, uint32_t s, uint32_t d,
    enum PhySimWifiPhy::ErrorReason reason)
{
  switch (reason)
  {
  case PhySimWifiPhy::STATE_TX:
    c.tracePayloadErrorTxCount[s][d]++;
    break;

  case PhySimWifiPhy::PROCESSING:
    c.tracePayloadErrorProcessingCount[s][d]++;
    break;

  case PhySimWifiPhy::CAPTURE:
    c.tracePayloadErrorCaptureCount[s][d]++;
    break;

  default:
    //FIXME NS_LOG_ERROR("Unexpected ErrorReason in RxError trace");
    break;
  }
}


#undef _TRACE_CALLBACK_LISTING

} /* namespace sim */
#endif /* PHYSIM_TRACE_COUNTER_GROUP_H_ */
