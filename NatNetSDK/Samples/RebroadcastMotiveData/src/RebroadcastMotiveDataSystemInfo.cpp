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



#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <string>

#include <iostream>
#include "RebroadcastMotiveDataSystemInfo.h"


void cSystemInfo::setIPAddress( char * input )
{
    ipAddress = input;
}

const char* cSystemInfo::getIPAddress() const
{
    return ipAddress;
}

void cSystemInfo::setPort( char * input )
{
    port = input;
}

const char* cSystemInfo::getPort() const
{
    return port;
}

void cSystemInfo::setType( char * input )
{
    type = input;
}

const char* cSystemInfo::getType() const
{
    return type;
}

 bool cSystemInfo::isTypeValid() const
{
    if ( !strcmp( type, "unity" ) )
        return true;
    if ( !strcmp( type, "lightcraft" ) )
        return true;

    return false;
}

int cSystemInfo::getParsedPort() const
{
    int val = atoi( port );
    return val;
}

void cSystemInfo::setTestMode( bool input )
{
    testMode = input;
}

bool cSystemInfo::InTestMode() const 
{
    return testMode;
}