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

#ifndef TRACE_UTILS_H_
#define TRACE_UTILS_H_

#include <iostream>
#include <string>
#include <sstream>

#include <boost/format.hpp>

#include "ns3/core-module.h"

namespace sim {

  /*
   * Helper methods for tracing.
   */
  class TraceUtils
  {
  public:
    static std::string
    phySimWifiTracePath(const std::string& traceSource, int nodeId = -1, int deviceId = -1);

    static std::string
    TcpTracePath (const std::string& traceSource, int nodeId = -1, int socketId = -1);

    TraceUtils();
    virtual
    ~TraceUtils();
  };

  template <int N>
  struct PhySimTraceCounterSet
  {
#define _TRACE_COUNTER_LISTING \
    _TRACE_COUNTER(traceTxCount);                       \
    _TRACE_COUNTER(traceDetectOkCount);                 \
    _TRACE_COUNTER(traceDetectErrorCount);              \
    _TRACE_COUNTER(traceStateErrorSyncCount);           \
    _TRACE_COUNTER(traceStateErrorTxCount);             \
    _TRACE_COUNTER(traceStateErrorRxCount);             \
    _TRACE_COUNTER(tracePreambleOkCount);               \
    _TRACE_COUNTER(tracePreambleOkCaptureCount);        \
    _TRACE_COUNTER(tracePreambleErrorTxCount);          \
    _TRACE_COUNTER(tracePreambleErrorRxCount);          \
    _TRACE_COUNTER(tracePreambleErrorProcessingCount);  \
    _TRACE_COUNTER(traceHeaderOkCount);                 \
    _TRACE_COUNTER(traceHeaderOkCaptureCount);          \
    _TRACE_COUNTER(traceHeaderErrorTxCount);            \
    _TRACE_COUNTER(traceHeaderErrorProcessingCount);    \
    _TRACE_COUNTER(traceHeaderErrorCaptureCount);       \
    _TRACE_COUNTER(tracePayloadOkCount);                \
    _TRACE_COUNTER(tracePayloadOkCaptureCount);         \
    _TRACE_COUNTER(tracePayloadErrorTxCount);           \
    _TRACE_COUNTER(tracePayloadErrorProcessingCount);   \
    _TRACE_COUNTER(tracePayloadErrorCaptureCount);

    typedef unsigned long counter_t;

#define _TRACE_COUNTER(x) counter_t x[N][N]

    _TRACE_COUNTER_LISTING

#undef _TRACE_COUNTER

    PhySimTraceCounterSet();

    virtual
    ~PhySimTraceCounterSet();

    void
    printCounters(uint32_t s, uint32_t d, std::ostream& out = std::cout) const;
  };

  template <int N>
  PhySimTraceCounterSet<N>::PhySimTraceCounterSet()
  {
#define _TRACE_COUNTER(x) memset(x, 0, sizeof(x[0][0]) * N * N)

    _TRACE_COUNTER_LISTING

#undef _TRACE_COUNTER
  }

  template <int N>
  PhySimTraceCounterSet<N>::~PhySimTraceCounterSet()
  {

  }

  template <int N>
  void
  PhySimTraceCounterSet<N>::printCounters(uint32_t s, uint32_t d, std::ostream& out) const
  {
    NS_ASSERT(s >= 0 && s < N);
    NS_ASSERT(d >= 0 && d < N);

    counter_t tempStateErrorTotal = traceStateErrorSyncCount[s][d] + traceStateErrorTxCount[s][d] + traceStateErrorRxCount[s][d];
    counter_t tempPreambleErrorTotal = tracePreambleErrorTxCount[s][d] + tracePreambleErrorRxCount[s][d] + tracePreambleErrorProcessingCount[s][d];
    counter_t tempHeaderErrorTotal = traceHeaderErrorTxCount[s][d] + traceHeaderErrorProcessingCount[s][d] + traceHeaderErrorCaptureCount[s][d];
    counter_t tempPayloadErrorTotal = tracePayloadErrorTxCount[s][d] + tracePayloadErrorProcessingCount[s][d] + tracePayloadErrorCaptureCount[s][d];

    //FIXME destination for tx trace
    out << "Packet Counters for Node " << s << " --> Node " << d << " (Tx: " << traceTxCount[s][s] << ")\n";

    out << boost::format("%-10s %10s %10s %10s %10s %10s %10s %10s %10s\n")
        % "" % "Ok" % "CapOk" % "Error" % "Sync" % "Tx" % "Rx" % "Proc" % "Cap";

    out << boost::format("%-10s %10d %10s %10d %10s %10s %10s %10s %10s\n")
        % "Detect:" % traceDetectOkCount[s][d] % "---" % traceDetectErrorCount[s][d] % "---" % "---" % "---" % "---" % "---";

    out << boost::format("%-10s %10s %10s %10d %10d %10d %10d %10s %10s\n")
        % "State:" % "---" % "---" % tempStateErrorTotal % traceStateErrorSyncCount[s][d] % traceStateErrorTxCount[s][d] % traceStateErrorRxCount[s][d] % "---" % "---";

    out << boost::format("%-10s %10d %10d %10d %10s %10d %10d %10d %10s\n")
        % "Preamble:" % tracePreambleOkCount[s][d] % tracePreambleOkCaptureCount[s][d] % tempPreambleErrorTotal % "---" % tracePreambleErrorTxCount[s][d] % tracePreambleErrorRxCount[s][d] % tracePreambleErrorProcessingCount[s][d] % "---";

    out << boost::format("%-10s %10d %10d %10d %10s %10d %10s %10d %10d\n")
        % "Header:" % traceHeaderOkCount[s][d] % traceHeaderOkCaptureCount[s][d] % tempHeaderErrorTotal % "---" % traceHeaderErrorTxCount[s][d] % "---" % traceHeaderErrorProcessingCount[s][d] % traceHeaderErrorCaptureCount[s][d];

    out << boost::format("%-10s %10d %10d %10d %10s %10d %10s %10d %10d\n")
        % "Payload:" % tracePayloadOkCount[s][d] % tracePayloadOkCaptureCount[s][d] % tempPayloadErrorTotal % "---" % tracePayloadErrorTxCount[s][d] % "---" % tracePayloadErrorProcessingCount[s][d] % tracePayloadErrorCaptureCount[s][d];

    out << std::endl;
  }




#undef _TRACE_COUNTER_LISTING

} /* namespace sim */
#endif /* TRACE_UTILS_H_ */
