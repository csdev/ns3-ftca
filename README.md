Fair TCP Channel Access: NS-3 Simulations
=========================================

Introduction
------------

Fair TCP Channel Access (FTCA) is a MAC-layer procedure developed to improve
TCP fairness in IEEE 802.11 WLANs. FTCA prioritizes TCP control packets using
quality-of-service mechanisms in the 802.11 standard. It reduces TCP ACK
delays and prevents collisions between TCP control packets and data segments
that could cause improper protocol operation. It addresses issues including
packet capture, path asymmetry, and ACK compression. FTCA is designed to be
easily deployable in existing WLANs, and is compatible with most TCP variants.

This package contains an implementation of FTCA in the NS-3 network simulator.
Various changes have been made to NS-3 to enable highly detailed modeling and
analysis of wireless connections. The PhySimWifiPhy module is used to simulate
signal-level effects such as interference and capture. Packet collision
tracing has been enhanced.


Prerequisites
-------------

NS-3 / FTCA should run properly on most modern Linux distros.
The following packages are required:

* g++ v4.6 or newer
* python 2.7
* it++ signal processing libraries (v4.2 recommended)
* boost c++ libraries

:exclamation: *Python 3 is not supported.*

:exclamation: *Note: Newer versions of it++ (>= 4.3) do not supply the proper
pkgconfig files and may require messy workarounds to avoid build problems with
PhySimWiFi. Use it++ v4.2 if possible.*

The following packages are useful, but not strictly required:

* doxygen (for building the API docs)
* wireshark (for analyzing pcap traces)
* gnuplot, matlab, and other software for data analysis


Setup
-----

### Step 1: Configuring NS-3

You can configure NS-3 in either debug mode or optimized mode. Debug mode
enables internal error logging and assertions in NS-3. It is also necessary if
you need to use tools such as gdb and valgrind. However, simulations will be
much slower.

To configure NS-3 in debug mode:

    ./waf -d debug --enable-examples --enable-tests configure

**OR**, to configure in optimized mode:

    ./waf -d optimized --enable-examples --enable-tests configure

### Step 2: Compiling

After configuring NS-3, you can compile using the following command:

    ./waf


Usage
-----

To turn on FTCA in your simulations, insert the following line of code:
```C++
Config::SetDefault ("ns3::TcpSocketBase::AckPriority", BooleanValue (true));
```

Also, please be sure to seed the IT++ RNG in addition to the NS-3 RNG.
This ensures that you are conducting statistically independent experiments.
(See also http://www.nsnam.org/docs/release/3.13/manual/html/random-variables.html)
```C++
#include <ctime>
#include <itpp/base/random.h>

itpp::RNG_reset (time (NULL));
```

An example is provided in `scratch/ftca/`. You can run it using:

    ./waf --run ftca

:exclamation: *The included example simulates multiple 30-second connections
using PhySimWifiPhy. It is quite CPU-intensive (will max out 1 core), outputs
50 to 60 MiB of data, and takes a couple of hours to complete on a typical
desktop PC.*


Credits/License
---------------

All code is open source under the GNU General Public License version 2.
See the included LICENSE file for full information.

FTCA Copyright (c) 2014 Christopher Sang

FTCA was developed as part of my master's thesis:
> C. Sang, "Fair TCP Channel Access for IEEE 802.11 WLANs," Master's thesis,
The Cooper Union, New York, 2014.

NS-3 source code and copyright info can be found at http://www.nsnam.org/.

PhySimWiFi source code and copyright info can be found at
http://dsn.tm.kit.edu/english/ns3-physim.php.

