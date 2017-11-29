function startMap = initMap(row, column, aMap, aDir)
%Sets the value of the blocks under the car body
%to be terrain.
% Wheel terrain extends to under sonic!!!

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
        aMap(row:row+1, column:column+1) = TERRAIN;
        
        %Left Wheel
        aMap(row-3:row, column-2) = TERRAIN;
        
        %Right Wheel
        aMap(row-3:row, column+3) = TERRAIN;
        
        %Body
        aMap(row-3:row, column-1:column+2) = TERRAIN;
        aMap(row:row+1, column-1) = TERRAIN;
        aMap(row:row+1, column+2) = TERRAIN;
        aMap(row+2, column-1:column+2) = TERRAIN;
    
    elseif theDir == NEGR
        %Center Block
        aMap(row-1:row, column-1:column) = TERRAIN;
        
        %Left Wheel
        aMap(row:row+3, column-3) = TERRAIN;
        
        %Right Wheel
        aMap(row:row+3, column+2) = TERRAIN;
        
         %Body
         aMap(row+1:row+3, column-2:column+1) = TERRAIN;
         aMap(row-1:row, column-2) = TERRAIN;
         aMap(row-1:row, column+1) = TERRAIN;
         aMap(row-2, column-2:column+1) = TERRAIN;

    %% Columns       
    elseif theDir == POSC   
        %Center Block
        aMap(row-1:row, column:column+1) = TERRAIN;
        
        %Left Wheel
        aMap(row-3, column-3:column) = TERRAIN;
        
        %Right Wheel
        aMap(row+2, column-3:column) = TERRAIN;
        
        %Body
        aMap(row-2:row+1, column-3:column) = TERRAIN;
        aMap(row-2, column:column+1) = TERRAIN;
        aMap(row+1, column:column+1) = TERRAIN;
        aMap(row-2:row+1, column+2) = TERRAIN;
        aMap(row-1:row, column+3) = TERRAIN;
        

    elseif theDir == NEGC
        %Center Block
        aMap(row:row+1, column-1:column) = TERRAIN;
        
        %Left Wheel
        aMap(row+3, column-1:column+2) = TERRAIN;
        
        %Right Wheel
        aMap(row-2, column-1:column+2) = TERRAIN;
        
        %Body
        aMap(row-1:row+2, column:column+2) = TERRAIN;
        aMap(row-1, column-1:column) = TERRAIN;
        aMap(row+2, column-1:column) = TERRAIN;
        aMap(row-1:row+2, column-2) = TERRAIN;
        aMap(row:row+1, column-3) = TERRAIN; % USonic Sensor
        
    end
    
    startMap = aMap;
end