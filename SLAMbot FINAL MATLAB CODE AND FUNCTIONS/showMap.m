function showMap(row, column, aMap, aDir)
%Takes in position of car and current map
%and displays the map with the car on top.
%@param x X-coordinate of the car.
%@param y Y-coordinate of the car.
%@param map Current map of the world contains
%no information about the car.

    global UNKNOWN;
    global CARCRITICAL;
    global CARBODY;
    global SONIC;
    global TERRAIN;
    global NOOBJECT;
    global EDGE;
    global OBJECT;

%%    Legend
      POSR = 10;   %Positive Rows
      NEGR = 20;   %Negative Rows
      POSC = 30;   %Positive Columns
      NEGC = 40;   %Negative Columns

    theDir = aDir;
    
    %% Rows
    if theDir == POSR
        %Center Block
        aMap(row:row+1, column:column+1) = CARCRITICAL;
        
        %Left Wheel
        aMap(row-3:row-1, column-2) = CARCRITICAL;
        
        %Right Wheel
        aMap(row-3:row-1, column+3) = CARCRITICAL;
        
        %Body
        aMap(row-3:row-1, column-1:column+2) = CARBODY;
        aMap(row:row+1, column-1) = CARBODY;
        aMap(row:row+1, column+2) = CARBODY;
        aMap(row+2, column-1:column+2) = CARBODY;
        
        %Ultrasonic Sensors
        aMap(row+1:row+2, column-2) = SONIC;
        aMap(row+3, column:column+1) = SONIC;
        aMap(row+1:row+2, column+3) = SONIC;
    
    elseif theDir == NEGR
        %Center Block
        aMap(row-1:row, column-1:column) = CARCRITICAL;
        
        %Left Wheel
        aMap(row+1:row+3, column-3) = CARCRITICAL;
        
        %Right Wheel
        aMap(row+1:row+3, column+2) = CARCRITICAL;
        
         %Body
         aMap(row+1:row+3, column-2:column+1) = CARBODY;
         aMap(row-1:row, column-2) = CARBODY;
         aMap(row-1:row, column+1) = CARBODY;
         aMap(row-2, column-2:column+1) = CARBODY;
         
        %Ultrasonic Sensors
        aMap(row-2:row-1, column-3) = SONIC;
        aMap(row-3, column-1:column) = SONIC;
        aMap(row-2:row-1, column+2) = SONIC;

    %% Columns       
    elseif theDir == POSC   
        %Center Block
        aMap(row-1:row, column:column+1) = CARCRITICAL;
        
        %Left Wheel
        aMap(row-3, column-3:column-1) = CARCRITICAL;
        
        %Right Wheel
        aMap(row+2, column-3:column-1) = CARCRITICAL;
        
        %Body
        aMap(row-2:row+1, column-3:column-1) = CARBODY;
        aMap(row-2, column:column+1) = CARBODY;
        aMap(row+1, column:column+1) = CARBODY;
        aMap(row-2:row+1, column+2) = CARBODY;
        
        %Ultrasonic Sensors
        aMap(row+2, column+1:column+2) = SONIC;
        aMap(row-3, column+1:column+2) = SONIC;
        aMap(row-1:row, column+3) = SONIC;

    elseif theDir == NEGC
        %Center Block
        aMap(row:row+1, column-1:column) = CARCRITICAL;
        
        %Left Wheel
        aMap(row+3, column+1:column+3) = CARCRITICAL;
        
        %Right Wheel
        aMap(row-2, column+1:column+3) = CARCRITICAL;
        
        %Body
        aMap(row-1:row+2, column+1:column+3) = CARBODY;
        aMap(row-1, column-1:column) = CARBODY;
        aMap(row+2, column-1:column) = CARBODY;
        aMap(row-1:row+2, column-2) = CARBODY;
        
        %Ultrasonic Sensors
        aMap(row+3, column-2:column-1) = SONIC;
        aMap(row-2, column-2:column-1) = SONIC;
        aMap(row:row+1, column-3) = SONIC;
        
    end
    
    % get the handle to the image
    image(aMap);
    
    
end