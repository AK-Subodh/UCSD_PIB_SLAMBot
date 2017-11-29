

function updatedMap = updateMap(row, column, aMap, aDir, uSensor, LTick, RTick);
%, frontBotDir, rightDir, leftDir, frontDir, thetaDir, LTick, RTick 
%Change update the values of the blocks that the uSonic sensors see.
%Also update the number of blocks = number of encoder ticks to show 
%"explored" terrain.

%LTick = Left Wheel Encoder Tick 
%RTick = Right Wheel Encoder Tick


    global UNKNOWN;
    global CARCRITICAL;
    global CARBODY;
    global SONIC;
    global TERRAIN;
    global NOOBJECT;
    global EDGE;
    global OBJECT;
    
%%    Directions and Respective Values
      POSR = 10;   %Positive Rows
      NEGR = 20;   %Negative Rows
      POSC = 30;   %Positive Columns
      NEGC = 40;   %Negative Columns
      

    theDir = aDir;
    
    left = TERRAIN;
    right = TERRAIN;
    frontDown = TERRAIN;
    frontObject = NOOBJECT;
    
    n = 2;
    
    for i = 1:LTick;
        if i == LTick;
            if strcmp(uSensor, 'FRONTEDGE');
                frontDown = EDGE;
            elseif strcmp(uSensor, 'LEFTEDGE');
                left = EDGE;
            elseif strcmp(uSensor, 'RIGHTEDGE');
                right = EDGE;
            elseif strcmp(uSensor, 'OBJECT');
                fronObject = OBJECT;
            end
        end

        %% Rows
        if theDir == POSR;
            row = row + 1;
            aMap(row:row+2, column+1:column+3) = left;        
            aMap(row:row+2, column-2:column-1) = right;
            aMap(row:row+3, column:column+1) = frontDown;
            aMap(row+4:row+n, column:column+1) = frontObject;

        elseif theDir == NEGR;
            row = row - 1;
            aMap(row-2:row, column-3:column-2) = left;
            aMap(row-2:row, column+1:column+2) = right;
            aMap(row-3:row, column-1:column) = frontDown;
            aMap(row-n:row-4, column-1:column) = frontObject;

        %% Columns
        elseif theDir == POSC;
            column = column + 1;
            aMap(row-3:row-2, column:column+2) = left;
            aMap(row+1:row+2, column:column+2) = right;
            aMap(row-1:row, column+3) = frontDown;
            aMap(row-1:row, column+4:column+n) = frontObject;        

        elseif theDir == NEGC;
            column = column - 1;
            aMap(row+2:row+3, column-2:column-1) = left;
            aMap(row-2:row-1, column-2:column-1) = right;
            aMap(row:row+1, column-3) = frontDown;
            aMap(row:row+1, column-n:column-4) = frontObject;

        end
        
        showMap(row, column, aMap, theDir);
    end
        


    
    %% Update map      
    updatedMap = aMap;
    
end