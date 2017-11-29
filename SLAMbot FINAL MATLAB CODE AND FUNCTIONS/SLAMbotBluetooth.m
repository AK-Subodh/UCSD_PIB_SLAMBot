%% Interfacting w/ HC-06 Bluetooth Module 
% Pair with your bluetooth device before continuing 
clear all

% Lists available Bluetooth devices
instrhwinfo('Bluetooth');

% Create a bluetooth variable and open it at channel 1
device = Bluetooth('HC-06', 1);

% Pair with device
fopen(device);
disp('Matlab paired with Bluetooth device');

%% Write command to HC-06/Arduino
while(1)

        data = fscanf(device, '%s', 100);
        dataparts = strsplit(data,',');
        if strcmp(dataparts{1},'EVENT')
            distance1 = str2num(dataparts{2});
            disp(distance1);
            distance2 = str2num(dataparts{3});
            disp(distance2);
            distance3 = str2num(dataparts{4});
            disp(distance3);
            distance4 = str2num(dataparts{5});
            disp(distance4);
            theta = str2num(dataparts{6});
            disp(theta);
        end 
end

%% Clear Bluetooth Object when Finished
fclose(device);
clear('device')
