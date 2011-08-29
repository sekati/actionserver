
Sekati ActionServer
============================


* Version: 1.1.0
* Developer: Jason M Horwitz & Marcus Stoeger
* Website: [http://sekati.com](http://sekati.com)
* Copyright: © 2004-2011 Jason M Horwitz / Sekati LLC & Marcus Stoeger,.

  
Description
-------------------------------------

Sekati ActionServer is a light-weight, C based, TCP/IP _Echo_ Socket Server. It functions as the _networking glue_ for any _real-time multi-user_ or _hybrid technology_ application.

The daemonized server opens `TCP port 2229` for incoming connections, awaits a `channel` join command (_this allows for a single server instance to host many different applications without message overflow_), then broadcasts any messages received from clients to all other connected clients in real-time.

**The Sekati ActionServer has been successfully deployed in many different use-cases such as:**

* Multi-user Flash Websites:
	* To allow users to view each others cursors & clicks in realtime by acting as an ``XMLSocket`` server: [classic.sekati.com](http://classic.sekati.com).
* Physical Computing Projects:
	* To communicate between X-Bee Wireless Arduino's & a receiving Web Application: [metrics.sarkissianmason.com](http://metrics.sarkissianmason.com).
	* To communicate between an SMS Text Message Gateway & an interactive projections: [Pathways SMS Installation](http://pathways.sarkissianmason.com).


Server Details
-------------------------------------

In addition to the primary `actionserver` daemon & `actionserverctl` control script - a second daemon `socketpolicyserver` is shipped/installed to support flash & air applications which require a special [crossdomain XML policy](http://www.lightsphere.com/dev/articles/flash_socket_policy.html) be served on a specific socket.

_The following is key usage information to know about the `actionserver` & `socketpolicyserver` daemons:_

**ActionServer Daemon** (defaults may be changed in ``actionserver.c``)

* TCP Port: ``2229``
* Max Clients: ``500``
* Max Message Buffer: ``4Kb``
* Log Channel: ``*gimmeallchannel*`` - this is a special control channel which a client may join in order to monitor traffic across all "channels".
* Log File: ``/tmp/actionserver.log`` - automatically rotated.
* Messages: ``\0`` - a ``NULL`` character is used to terminate each transmitted & received message (_this is default behavior in AS3 & Wiring / Processing_).
* Joining: When connecting to the actionserver the 1st message string defines your ``"room"`` or ``channel``: you will only receive messages transmitted by other clients who have joined the same ``"room"``. This allows you to use the server for multiple applications. __Please note this means immediately upon connecting your client must send a string defining it's room followed by the ``\0`` NULL character.


**SocketPolicyServer Daemon**

* TCP Port: ``1843`` - while the default adobe socket server is ``843`` running daemons on ports below ``1024`` requires running as the ``root`` user so we run on a high port (be sure to point any flash/air clients at the modified port: this is illustrated in the ``example`` application).
* Log File: ``/tmp/socketpolicy.log``


Installation
-------------------------------------

1. Unarchive & in your terminal change to the actionserver directory: ``cd /path/to/sk-actionserver/``
2. Compile the ActionServer for your environment: ``make``
3. Install the ActionServer binaries: ``make install`` (you will be prompted for your ``root`` password for installation).
4. Run the control script (see _Packaging List_ section below for more details): ``actionserverctl start``
	* This will daemonize the ``actionserver`` & ``socketpolicyserver`` making them ready for use.
5. To check the status of the servers type: ``actionserverctl status``
6. To follow the log files for the servers type: ``actionscriptctl log``
7. To stop the servers type: ``actionscriptctl stop``

_If you run in to trouble while compiling or installing the binaries you may run ``make uninstall`` to start over or type ``make help`` for more information._

  
Build Requirements
-------------------------------------

* GNU Make
* GNU GCC Compiler
* Perl (_for companion socket policy server_)


Runtime Requirements
-------------------------------------

__*The ActionServer is an extremely light-weight & low requirement 
application meant to run on nearly any operating system:*__
	
* Any unix-like Operating System (_happily compiles on most Linux, Unix, BSD, Mac OS X, et. al_)
* ActionServer should compile/run on Windows systems with only minor modifications; _CYGWin is recommended as a good place to start_.

  
Packaging List
-------------------------------------
	  
* __*Makefile*__ - The ActionServer Makefile: see the _Installation_ section above for more.

* __*src/*__ - ActionServer C source code.

* __*examples/*__ - Flash example app which illustrates connecting, sending, receiving & disconnecting.

* __*bin/*__ - runtime binaries.

	* __*actionserverctl*__ - Control interface designed to help the administrator control the functioning of the daemon.
		* _usage_: `./actionserverctl [ command ]`
		* `start` - Start the ActionServer & Socket Policy Server daemons.
		* `stop` - Stop the ActionServer & Socket Policy Server daemons.
		* `status` - Displays a brief status report on the daemons statuses.
		* `log` - Tail the log files for both daemons.
		
	* __*actionserver*__ - The ActionServer Daemon: _do not run this application directly; instead use `actionserverctl`_.
	
	* __*socketpolicyserver*__ - Adobe Socket Policy Server Daemon to support Flash & Air applications.  

  
Changelog
-------------------------------------

	8/28/11 - *v1.1.0
		- Added a proper Makefile
		- Repackaged for Github release.
		- Revises `actionscriptctl` control script
		- Revised build.sh compilation script
		- Revised channel password
		- Added detailed README instructions (this document)
		- added includes to string.h & stdlib.h to surpress gcc warnings

	2/8/10 - *v1.0.9*
		- Added proper LICENSE.txt
		- added socketpolicyserver.pl & integrated actionserverctl script
		- pointed logging facility to /tmp/actionserver.log
		- updated actionserverctl
		- created new binary build
		- updated actionscript examples.

	1/18/10 - *v.1.0.8*
		- Changed message buffer to 8192 (8k) from 2048 (2k)
		- Changed max users to 500 (from 100)

  	
License
-------------------------------------

Copyright © 2011 Jason M Horwitz / Sekati LLC & Marcus Stoeger. All Rights Reserved.

Released under the MIT License: [http://www.opensource.org/licenses/mit-license.php](http://www.opensource.org/licenses/mit-license.php)

	The MIT License

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.	
	