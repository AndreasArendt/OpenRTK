% LoadData;

idx_E    = ([S.SvSystemID_t_eph] >= 17664) & ([S.SvSystemID_t_eph] <= 17664+255); %17664 is 69 ('E') << 8
SvId_E   = unique(S.SvSystemID_t_eph(idx_E));
number_E = numel(unique(S.SvSystemID_t_eph(idx_E)));

rho = S.Code_1(idx_E);

Theta = [0;0;0];
pos = [0;0;0];
cdtr = 0;

ctr = 1;

timestamps = unique(S.ObsToc);
for tt = timestamps(1:180).'   
    idx = S.ObsToc == tt;

    % filter out unhealthy satellites    
    idx = idx & S.Health == 0;
    
    % Indices
    code_idx   = idx & cellfun(@(x)strcmp(x(1), 'E'), S.SvSystem_t_eph) & ...   % Galileo only!
                       (S.Code_1 > 1e3) & (S.Code_1 > 1e5) & ...                % valid Pseudoranges
                       ((S.Band == 1) | (S.Band == 5));                         % E1 and E5
    code_1_idx = code_idx & (S.Band == 1);

    
    POS = pos(:,ctr);

    for ii = 1:10
        [r, d, e] = CalcDistance( S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx), ...
                                  POS(1), POS(2), POS(3));
    
        cdt_sv  = Transformation.SpeedOfLight__mDs * S.SvClockOffset(code_1_idx);    
        cdt_rel = Transformation.SpeedOfLight__mDs * S.RelativisticError(code_1_idx);
    
        tropo_offset = CalcTropoOffset(POS(1), POS(2), POS(3), ...
                                       d(:,1), d(:,2), d(:,3));
    
        % delta_pseudorange_1 = S.Code_1(code_1_idx) - cdtr(ctr) + cdt_sv - tropo_offset - cdt_rel;
        % delta_pseudorange_1 = S.Code_1(code_1_idx) + cdt_sv - tropo_offset - cdt_rel;
        delta_pseudorange_1 = S.Code_1(code_1_idx) + cdt_sv - cdt_rel;
        
        % A = [-e, ones(numel(e(:,1)),1)];
        A = -e;
        y = delta_pseudorange_1 - r;
    
        x_hat = (A.'*A)\A.'*y;
        
        POS = POS + x_hat;        

        if all(x_hat < 1e-3)
            break;
        end
    end
        
    ctr = ctr+1;
    pos(:,ctr) = POS;
end

%%
afigure(42);
subplot(3,2,1);
hold on;
yline(station_pos__m(1), 'Color', Color.BLACK)
plot(pos(1,:))

subplot(3,2,2);
hold on;
plot(station_pos__m(1) - pos(1,:))

subplot(3,2,3);
hold on;
yline(station_pos__m(2), 'Color', Color.BLACK)
plot(pos(2,:))

subplot(3,2,4);
hold on;
plot(station_pos__m(2) - pos(2,:))

subplot(3,2,5);
hold on;
yline(station_pos__m(3), 'Color', Color.BLACK)
plot(pos(3,:))

subplot(3,2,6);
hold on;
plot(station_pos__m(3) - pos(3,:))