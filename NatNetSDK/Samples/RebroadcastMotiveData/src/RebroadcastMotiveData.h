//=============================================================================
// Copyright © NaturalPoint, Inc. All Rights Reserved.
// 
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall NaturalPoint, Inc. or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//=============================================================================

/*

RebroadcastMotiveData

This program will take arguments from a user that will determine a Message and how the message.
is transmitted. It will then connect to a NatNet server, encode the data and output it based on
the parameters passed.

Usage:

RebroadcastMotiveData.exe [a- address][p- port][ string ]

string values consist of:
unity | lightcraft

*/

#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <winsock2.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "NatNetTypes.h"
#include "NatNetCAPI.h"
#include "NatNetClient.h"

#include "NatNetRepeater.h"   //== for transport of data over UDP to Unity3D



//== Slip Stream globals ==--

extern cSlipStream *gSlipStream;
std::map<int, std::string> gBoneNames;

void NATNET_CALLCONV DataHandler( sFrameOfMocapData* data, void* pUserData );   // receives data from the server
void NATNET_CALLCONV MessageHandler( Verbosity msgType, const char* msg );     // receives NatNet error messages
void resetClient();
int CreateClient( ConnectionType connectionType );

unsigned int MyServersDataPort = 3130;
unsigned int MyServersCommandPort = 3131;

NatNetClient* theClient;
FILE* fp;

char szMyIPAddress[128] = "";
char szServerIPAddress[128] = "";
char szUnityIPAddress[128] = "";

int analogSamplesPerMocapFrame = 0;
ConnectionType iConnectionType = ConnectionType_Multicast;
//extern ConnectionType iConnectionType = ConnectionType_Unicast;

//== Helpers for text indicator of data flowing... ==--
