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

#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <string>

#include <iostream>


// System Information
class cSystemInfo
{
public:
    cSystemInfo() {};
    virtual ~cSystemInfo() {};

    void setIPAddress( char *input );
    const char* getIPAddress() const;
    void setPort( char * input );
    const char* getPort() const;
    void setType( char *input );
    const char* getType() const;
    bool isTypeValid() const;
    int getParsedPort() const;
    void setTestMode( bool input );
    bool InTestMode() const;

private:
    // Variables
    char * ipAddress;
    char * port;
    char * type;
    bool testMode;

};
