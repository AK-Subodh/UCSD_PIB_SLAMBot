function newDir = updateDir(device, aDir, newDirString);
    %% Directional Input
    POSR = 10;   %Positive Rows
    NEGR = 20;   %Negative Rows
    POSC = 30;   %Positive Columns
    NEGC = 40;   %Negative Columns
    
    
    if strcmp(newDirString, 'LEFT')
        if aDir == POSR
            newDir = POSC
        elseif aDir == NEGR
            newDir = NEGC
        elseif aDir == NEGC
            newDir = POSR
        elseif aDir == POSC
            newDir = NEGR
        else
            disp('ERROR IN LEFT DIRECTIONS')
        end
    elseif strcmp(newDirString, 'RIGHT')
        if aDir == POSR
            newDir = NEGC
        elseif aDir == NEGR
            newDir = POSC
        elseif aDir == POSC
            newDir = POSR
        elseif aDir == NEGC
            newDir = NEGR
        else
            disp('ERROR IN RIGHT DIRECTIONS');
        end
    end
        fprintf(device, newDirString);
end