% Optitrack Matlab / NatNet Polling Sample
%  Requirements:
%   - OptiTrack Motive 2.0 or later
%   - OptiTrack NatNet 3.0 or later
%   - Matlab R2013
% This sample connects to the server and displays rigid body data.
% natnet.p, needs to be located on the Matlab Path.


% properties('natnet')
% methods('natnet')


%% LCR Meter Reader 
clear; close all; clc

% Find a VISA-USB object.
obj1 = instrfind('Type', 'visa-usb', 'RsrcName', 'USB0::0x0957::0x0909::MY54202935::0::INSTR', 'Tag', '');
 
% Create the VISA-USB object if it does not exist
% otherwise use the object that was found.
if isempty(obj1)
    obj1 = visa('AGILENT', 'USB0::0x0957::0x0909::MY54202935::0::INSTR');
else
    fclose(obj1);
    obj1 = obj1(1);
end
 
% Connect to instrument object, obj1.  
fopen(obj1);
 
% Communicating with instrument object, obj1. 
type = query(obj1, ':FUNCtion:IMPedance:TYPE?');
disp('LCR Connected.');
%% Main Data Collection

% Figures Setup
timeStepEnd = 4000; % 400 ~= 1 min
out = zeros(timeStepEnd,4);
                 
figure(1)
 
% LCR 1 Plot
subplot(3,1,1)
h1 = animatedline('Color','b','lineWidth',1.5);
ax1 = gca;
ax1.YGrid = 'on';
ylabel('Resistance');

% LCR 2 Plot
subplot(3,1,2)
h2 = animatedline('Color','g','lineWidth',1.5); 
ax2 = gca;
ax2.YGrid = 'on';

%OPTI
subplot(3,1,3)
h3 = animatedline('Color','g','lineWidth',1.5); 
ax3 = gca;
ax3.YGrid = 'on';

%Pause to signal start recording
pause;
disp('Ready. Start recording now')


%% 
fprintf( 'NatNet Polling Sample Start\n' )

% create an instance of the natnet client class
fprintf( 'Creating natnet class object\n' )
natnetclient = natnet;

% connect the client to the server (multicast over local loopback) -
% modify for your network
fprintf( 'Connecting to the server\n' )
natnetclient.HostIP = '127.0.0.1';
natnetclient.ClientIP = '127.0.0.1';
natnetclient.ConnectionType = 'Multicast';
natnetclient.connect;
if ( natnetclient.IsConnected == 0 )
    fprintf( 'Client failed to connect\n' )
    fprintf( '\tMake sure the host is connected to the network\n' )
    fprintf( '\tand that the host and client IP addresses are correct\n\n' )
    return
end

% get the asset descriptions for the asset names
model = natnetclient.getModelDescription;
if ( model.RigidBodyCount < 1 )
    return
end

% Poll for the rigid body data a regular intervals (~1 sec) for 10 sec.
fprintf( '\nPrinting rigid body frame data approximately every second for 10 seconds...\n\n' )

startTime = datetime('now');
for idx = 1 : timeStepEnd
    java.lang.Thread.sleep( 996 );%%should be millisecond
    data = natnetclient.getFrame; % method to get current frame
    
    if (isempty(data.RigidBody(1)))
        fprintf( '\tPacket is empty/stale\n' )
        fprintf( '\tMake sure the server is in Live mode or playing in playback\n\n')
        return
    end
    fprintf( 'Frame:%6d  ' , data.Frame )
    fprintf( 'Time:%0.2f\n' , data.Timestamp )
    for i = 1:model.RigidBodyCount
        fprintf( 'Name:"%s"  ', model.RigidBody( i ).Name )
        fprintf( 'X:%0.1fmm  ', data.RigidBody( i ).x * 1000 )
        fprintf( 'Y:%0.1fmm  ', data.RigidBody( i ).y * 1000 )
        fprintf( 'Z:%0.1fmm\n', data.RigidBody( i ).z * 1000 )
    end
    
        % Get current time 
    t(i,1) = datetime('now') - startTime;
   
    data = query(obj1, ':FETCh:IMPedance:FORMatted?');
    splt = strsplit(data,',');
    
    out(i,1) = str2double(splt(1)); % LCR 1 
    out(i,2) = str2double(splt(2)); % LCR 2
    
    figure(1) 
    
    % LCR1 Graph
    subplot(2,1,1)
    addpoints(h1,datenum(t(i,1)),out(i,1));
    ax1.XLim = datenum([t(i,1) - seconds(15) t(i,1)]);
    datetick('x','keeplimits');
    drawnow update
    
    % LCR2 Graph 
    subplot(2,1,2)
    addpoints(h2,datenum(t(i,1)),out(i,2));
    ax2.XLim = datenum([t(i,1) - seconds(15) t(i,1)]);
    datetick('x','keeplimits');
    drawnow update
    
        % LCR2 Graph 
    subplot(3,1,3)
    addpoints(h3,datenum(t(i,1)),data.RigidBody( i ).x * 1000);
    ax2.XLim = datenum([t(i,1) - seconds(15) t(i,1)]);
    datetick('x','keeplimits');
    drawnow update
    
    
end
disp('NatNet Polling Sample End' )








