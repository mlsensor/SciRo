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

    This program will take arguments from a user that will determine a Message and how the message
    is transmitted. It will then connect to a NatNet server, encode the data and output it based on
    the parameters passed.

    Usage:

    RebroadcastMotiveData.exe [address] [port/address] [type] [test(optional)]

    SUPPORTED TYPES:
    lightcraft

*/

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <iostream>


#include "RebroadcastMotiveDataSystemInfo.h"
#include "LightCraft Protocol\LightCraftProtocol.h"
#include "Unity Protocol\UnityProtocol.h"


// Set up system info
extern cSystemInfo G_systemInfo;


// Main Execution
int main( int argc, char* argv[] )
{

    // Start by getting command line arguments for server IP address and port

    // Checking Command line arguments
    if ( !(argc > 3) )
    {
        // Arguments are incomplete puting in default args
        printf( "\nArguments incomplete.\nRefer to documentation to use command line arguments:\nRebroadcastMotive.exe [MotiveIp] [COM PORT] [protocol] [(optional)test mode]\n\n" );
		return 1;
	}
    else
    {
        G_systemInfo.setIPAddress( argv[1] ); // Assigning IP Address
        printf( "\nObtaining IP address of %s...", G_systemInfo.getIPAddress() ); 
        G_systemInfo.setPort( argv[2] ); // Assigning Port
        printf( "\nSetting to port\alternate IP of %s...", G_systemInfo.getPort() );
        G_systemInfo.setType( argv[3] ); // Assigning the type
        printf( "\nUsing Type %s...", G_systemInfo.getType() );
    }

    // Puting into test mode to add print statements *REDUCES PERFORMANCE*
    if ( argc > 4 )
    {
        if ( !strcmp( argv[4], "test" ) )
        {
            printf( "\nEntering in test mode..." );
            G_systemInfo.setTestMode( true );
        }
        else
            G_systemInfo.setTestMode( false );
    }

    
    //Validating type
    if ( G_systemInfo.isTypeValid() )
    {
        printf( "\nType is Valid." );
    }
    else
    {
        printf( "\nType is not Valid." );
        return 1;
    }

    // Code for calling all the different types in this app
    if ( !strcmp( G_systemInfo.getType(), "lightcraft") )
    {
        // Lightcraft protocal
        RunLightcraftSender(G_systemInfo);
    }
	/* Unity protocol depricated
    else if ( !strcmp( G_systemInfo.getType(), "unity" ) )
    {
        // Unity protocal
        runUnityProtocol( G_systemInfo );
    }
	*/

    printf( "\n\nCompleted Process..." );
    
    return 0;
    
}
