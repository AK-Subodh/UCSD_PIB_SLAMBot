function direction = getDirection( aPosition, aMap )
% Takes sensor input and current location to derive where to go next
% Moves either right or left. The car will choose to go left by default

    UNKNOWN = 8;
    NOOBJECT = 7;

    thePosition = aPosition;

    % Format: Positive/Negative Direction Column/Row
    % Possible movements if car moving positive column direction
    PLEFTC = thePosition - [1, 0];
    PRIGHTC = thePosition + [1, 0];

    % Possible movements if car moving positve row direction
    PLEFTR = thePosition - [0, 1];
    PRIGHTR = thePosition + [0, 1];

    % Possible movements if car moving negative row direction
    NLEFTR = thePosition - [0, 1];
    NRIGHTR = thePosition + [0, 1];

    % Possible movements if car moving negative column direction
    NLEFTC = thePosition + [1, 0];
    NRIGHTC = thePosition - [1, 0];


    CENTER = 50;

    % Positive row
    if( (thePosition(1, 1) - CENTER) > 0 )
        if( (getState(PLEFTR, aMap) == UNKNOWN) ||
            (getState(PLEFTR, aMap) == NOOBJECT) )
            direction = 'left';
        else
            direction = 'right';
        end
    end

    % Negative row
    if( (thePosition(1, 1) - CENTER) < 0 )
        if( (getState(NLEFTR, aMap) == UNKNOWN)
            || (getState(NLEFTR, aMap) == NOOBJECT) )
            direction = 'left';
        else
            direction = 'right';
        end
    end

    % Positive column
    if( (thePosition(1, 2) - CENTER) > 0 )
        if( (getState(PLEFTC, aMap) == UNKNOWN)
            || (getState(PLEFTC, aMap) == NOOBJECT) )
            direction = 'left';
        else
            direction = 'right';
        end
    end

    % Negative column
    if( (thePosition(1, 2) - CENTER) < 0 )
        if( (getState(NLEFTC, aMap) == UNKNOWN)
            || (getState(NLEFTC, aMap) == NOOBJECT) )
            direction = 'left';
        else
            direction = 'right';
        end
    end

end