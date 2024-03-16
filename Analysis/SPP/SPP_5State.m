% LoadData;

idx_E    = ([S.SvSystemID_t_eph] >= 17664) & ([S.SvSystemID_t_eph] <= 17664+255); %17664 is 69 ('E') << 8
SvId_E   = unique(S.SvSystemID_t_eph(idx_E));
number_E = numel(unique(S.SvSystemID_t_eph(idx_E)));

pos = station_pos__m';%[0;0;0];
cdr = 0;
dztd = 0;

ctr = 1;

idx_Galileo = cellfun(@(x)strcmp(x(1), 'E'), S.SvSystem_t_eph);
idx_GPS     = cellfun(@(x)strcmp(x(1), 'G'), S.SvSystem_t_eph);

timestamps = unique(S.ObsToc);
for tt = timestamps.'   
    idx = S.ObsToc == tt;

    % filter out unhealthy satellites    
    idx = idx & S.Health == 0;
    
    % Indices
    code_idx   = idx & (idx_Galileo | idx_GPS) & ...
                       (S.Code_1 > 1e3) & (S.Code_1 > 1e3) & ...                % valid Pseudoranges
                       ((S.Band == 1) | (S.Band == 5));                         % E1 and E5
    code_1_idx = code_idx & (S.Band == 1);
    code_5_idx = code_idx & (S.Band == 5);
    
    [ia,ib] = ismember(S.SvSystemID_t_eph(code_1_idx), S.SvSystemID_t_eph(code_5_idx));
    tmp = find(code_1_idx);
    code_1_idx(tmp(~ia)) = 0;
    
    POS = pos(:,ctr);
    CDR = cdr(ctr);
    ZTD = dztd(ctr);

    for ii = 1:10           
        [r, d, e] = CalcDistance( S.x(code_1_idx), S.y(code_1_idx), S.z(code_1_idx), ...
                                  POS(1), POS(2), POS(3));
    
        cdt_sv  = Transformation.SpeedOfLight__mDs * S.SvClockOffset(code_1_idx);    
        cdt_rel = Transformation.SpeedOfLight__mDs * S.RelativisticError(code_1_idx);
    
        % elevation mask
        elevation = CalcElevation(POS(1), POS(2), POS(3), d(:,1), d(:,2), d(:,3));
        
        idx_el = elevation > (15/180*pi) & elevation < (165/180*pi);

        [lat__rad, lon__rad, alt__m] = Transformation.ecef2wgs84(POS(1), POS(2), POS(3));

        d_dry = 2.3 .* exp(-0.116e-3 .* alt__m);
    
        if isinf(d_dry)
            d_dry = 1e9;
        end
          
        d_wet = 0.1 + ZTD;        
        M_E = 1.001./sqrt(0.002001 + sin(elevation).^2);                        
        tropo_offset = (d_dry + d_wet) .* M_E;
        
        rho_iono_free = ((F_E1_Galileo__Hz^2 * S.Code_1(code_1_idx)) - (F_E5a_Galileo__Hz^2 * S.Code_5(code_5_idx))) ./ ...
            (F_E1_Galileo__Hz^2 - F_E5a_Galileo__Hz^2);
                
        pseudorange_est = rho_iono_free + cdt_sv + cdt_rel - CDTR - tropo_offset;        
        
        A = [-e, ones(numel(e(:,1)),1), M_E];
       
        y = pseudorange_est - r;

        % if numel(y) > 5
        %     [~,idx_y] = max(y);
        %     idx_yy = 1:numel(y)~=idx_y;
        %     A = A(idx_yy,:);
        %     y = y(idx_yy);
        % else
        %     a = 1;
        % end
        
        % idx_yy = 1:numel(y);
        % if sum(idx_el(idx_yy)) > 5
            % A = A(idx_el(idx_yy),:);
            % y = y(idx_el(idx_yy));
        % end 

        x_hat = (A.'*A)\A.'*y;
        
        POS = POS + x_hat(1:3);        
        CDR = CDR + x_hat(4);
        ZTD = ZTD + x_hat(5);

        if all(abs(x_hat(1:3)) < 1e-3)
            break;
        end
    end
        
    ctr = ctr+1;
    pos(:,ctr) = POS;
    cdr(ctr) = CDR;
    dztd(ctr) = ZTD;

    if ZTD > 5
        a = 1;
    end

end

ReadRtkPos;
return;
%%
afigure(42);
subplot(3,2,1);
hold on;
yline(station_pos__m(1), 'Color', Color.BLACK)
plot(pos(1,:))
ylim([station_pos__m(1)-50,  station_pos__m(1)+50])

subplot(3,2,2);
hold on;
plot(station_pos__m(1) - pos(1,:))
ylim([-20 20])

subplot(3,2,3);
hold on;
yline(station_pos__m(2), 'Color', Color.BLACK)
plot(pos(2,:))
ylim([station_pos__m(2)-50, station_pos__m(2)+50])

subplot(3,2,4);
hold on;
plot(station_pos__m(2) - pos(2,:))
ylim([-20 20])

subplot(3,2,5);
hold on;
yline(station_pos__m(3), 'Color', Color.BLACK)
plot(pos(3,:))
ylim([station_pos__m(3)-50, station_pos__m(3)+50])

subplot(3,2,6);
hold on;
plot(station_pos__m(3) - pos(3,:))
ylim([-20 20])