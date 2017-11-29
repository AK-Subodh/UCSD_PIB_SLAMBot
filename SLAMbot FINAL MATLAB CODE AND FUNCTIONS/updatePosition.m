function newPosition = updatePosition(position, aDir, leftTicks);
    %%    Directions and Respective Values
    POSR = 10;   %Positive Rows
    NEGR = 20;   %Negative Rows
    POSC = 30;   %Positive Columns
    NEGC = 40;   %Negative Columns

    row = position(1,1);
    column = position(1,2);
    
    if aDir == POSR
        row = row + leftTicks;
    elseif aDir == NEGR
        row = row - leftTicks;
    elseif aDir == NEGC
        column = column - leftTicks;
    elseif aDir == POSC
        column = column + leftTicks;
    else
        disp('ERROR IN LEFT DIRECTIONS') 
    end
    
    newPosition = [row column];
end