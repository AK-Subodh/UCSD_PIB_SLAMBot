

% Interfacting w/ HC-06 Bluetooth Module 
% Pair with your bluetooth device before continuing 
% Create a bluetooth variable and open it at channel 1
device = Bluetooth('HC-06', 1);

%Pair with device
fopen(device);
disp('Matlab paired with Bluetooth device');

%% Map Design
%Map variables to colors.
global UNKNOWN;
global CARCRITICAL;
global CARBODY;
global SONIC;
global TERRAIN;
global NOOBJECT;
global EDGE;
global OBJECT;
UNKNOWN = 8;        %White
CARCRITICAL = 2;    %Red
CARBODY = 1;        %Black
SONIC = 3;          %Blue
TERRAIN = 4;        %Green
NOOBJECT = 7;       %Yellow
EDGE = 5;           %Cyan
OBJECT = 6;         %Magenta

%% Directional Input
POSR = 10;   %Positive Rows
NEGR = 20;   %Negative Rows
POSC = 30;   %Positive Columns
NEGC = 40;   %Negative Columns

myDir = POSC;

%Maps numbers to colors
%When 1 it is interpreted as 0,0,0 (Black)
%When 2 it is interpreted as 1,0,0 (Red)
%And so on
mapColor = [0,0,0
    1,0,0
    0,0,1
    0,1,0
    0,1,1
    1,0,1
    1,1,0
    1,1,1];
%Sets the color on
%Used when image function interprets integers
%as colors.
colormap(mapColor);

%Creates a map 100 x 100 map and sets all values
%to UNKNOWN value.
map(1:100,1:100) = UNKNOWN;

%% Initiate Center and Create Map
%Sets starting coordinate of the car as 50,50
%or center of image.
row = 50;
column = 50;

%Sets blocks under car body as terrain.
myMap = initMap(row, column, map, myDir);

%Shows the map with car using image.
showMap(row, column, myMap, myDir);

% Move right 40 blocks, same as moving 40 columns to the right
% for i = 1:20
%     lastPosition = [row, column]
%     lastState = getState(lastPosition, myMap)
%     %row = row - 1;
%     column = column + 1;
%     currentPosition = [row, column]
%     %Update map according to what uSonic sensors see.
%     myMap = updateMap(row, column, myMap, myDir);
%     currentState = getState(currentPosition, myMap)
%     %Display the updated map using new position of car.
%     showMap(row, column, myMap, myDir);
%     %Wait one second.
%     pause(1);
% end

% Collection of Data and Update Map
fprintf(device, 'START');
while(1)
        data = fscanf(device, '%s', 100);
        dataparts = strsplit(data,',');
        disp(data);
        if strcmp(dataparts{1},'SPACE')
            Usensor = dataparts{2}; %Usensor => Telling us which sensor was triggered.
            disp(Usensor);
            Ltick = str2num(dataparts{3}); %Ltick = The number of Left wheel encoder ticks
            Rtick = str2num(dataparts{4}); %Rtick = The number of Right wheel encoder ticks
            myMap = updateMap(row, column, myMap, myDir, Usensor, Ltick, Rtick );
            position = updatePosition([row column], myDir, Ltick);
            row = position(1,1);
            column = position(1,2);
            % These conditions check for what kind of edge the car has
            % detected. These edges will be detected using the ultrasonic
            % sensors (Called Usensor in the condition)
            % Depending on what type of edge the car detected (forward,
            % left or right), the car will turn in a specific direction. 
            
            
            if strcmp( Usensor, 'FORWARDEDGE'); %If car detects an edge in the front
               myDir = updateDir(device,myDir , 'LEFT'); %Then tell it to turn left 
               
            end
            if strcmp( Usensor, 'LEFTEDGE'); %If car detects an edge on the left hand side
                myDir = updateDir(device, myDir, 'RIGHT'); % Then tell it to turn right 
            end
            if strcmp( Usensor, 'RIGHTEDGE'); % If car detects an edge on the right hand side
                myDir = updateDir(device, myDir, 'LEFT'); % Then tell it to turn left 
            end
           if strcmp( Usensor, 'OBJECT'); % If car detects an object in the front
               disp(Usensor);
               disp('Sending left commmand.');
               myDir = updateDir(device, myDir, 'LEFT'); % Then turn left
            end
        end
        
        
        if strcmp( dataparts{1}, 'CONFIRMATION');
            flag = dataparts{2};
            if strcmp( flag, 'FAIL') 
                fprintf(device, 'RIGHT');
    %         else updateMap(row, column, aMap, aDir);
        
            end
        end
%     
%         
% 
end

%% Clear Bluetooth Object when Finished
% fclose(device);
% clear('device')
        
    