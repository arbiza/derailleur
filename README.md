Derailleur
====================

Derailleur is an OpenFlow controller written in C++, built using libfluid, the winner of [Open Networking Foundation Driver Contest](https://www.opennetworking.org/component/content/article/26-news-and-events/press-releases/1431-open-networking-foundation-announces-openflow-driver-contest-winner). Going in a different direction of other SDN controllers Derailleur aims to be very simple controller written in a low level language.


## License

Derailleur OpenFlow controller is licensed under LGPL v3; you can see details about the license [here](http://www.gnu.org/licenses/lgpl-3.0.html)


## Building

Derailleur requires some packages for building. The versions indicated bellow were tested; lower versions may work too.

* Cmake 3.3.1+
* Boost C++ 1.57+
* libfluid (base and message) [here](http://opennetworkingfoundation.github.io/libfluid/md_doc_QuickStart.html)

Building Derailleur is quite simple. Procedures bellow illustrate steps for out of source building using Cmake executed at derailleur root directory:

 ...derailleur$ mkdir controller/build
 ...derailleur$ cd controller/build
 ...derailleur$ cmake ../
 ...derailleur$ make
 ...derailleur$ sudo make install

