% LoadData;

plotCov = false;

usePreciseEph = true;
% usePreciseEph = false;

refpos__m = [NaN, NaN, NaN];

pos_E__m           = zeros(numel(SatelliteData), 3);
rx_clock_offset__m = zeros(numel(SatelliteData), 1);
zwd__m             = zeros(numel(SatelliteData), 1); %zenith wet delay
ambig              = zeros(numel(SatelliteData), meta.NumberSatellites); % float ambiguities

P_pos         = NaN(numel(SatelliteData), 3); % covariance for position
n_sat         = NaN(numel(SatelliteData), 1); % number of satellites used
elevation_out = NaN(numel(SatelliteData), meta.NumberSatellites);

for ii = 1:numel(SatelliteData)
    %% Pre-Processing
    Observations = generic.getValidObservations(SatelliteData(ii).Observations, 'excludeSv', {'G30'});

    if isempty(Observations)
        continue;
    end

    % index of current satellites
    [~, idx_sv] = ismember({Observations.SatelliteSystem}, meta.AvailableSatelliteSystems);

    % get additional offsets    
    sv_relativistic__m = Transformation.SpeedOfLight__mDs * [Observations.RelativisticError]';

    if usePreciseEph == true
        % TODO: SV order is implicitly used here
        d = Generic.GetPreciseEphemeris(PreciseEphemerisData, {Observations.SatelliteSystem}, SatelliteData(ii).PosixEpochTime__s);
        v = [d.entries('Struct').Value];
        eph_x_E__m = [v.x].';
        eph_y_E__m = [v.y].';
        eph_z_E__m = [v.z].';

        d = Generic.GetPreciseClock(PreciseClockData, {Observations.SatelliteSystem}, SatelliteData(ii).PosixEpochTime__s);
        v = [d.entries('Struct').Value];

        sv_clock_offset__m = v.' .* const.c__mDs;
    else
        ECEF_Position = [Observations.ECEF_Position];
        eph_x_E__m = [ECEF_Position.x]';
        eph_y_E__m = [ECEF_Position.y]';
        eph_z_E__m = [ECEF_Position.z]';

        sv_clock_offset__m = Transformation.SpeedOfLight__mDs * [Observations.ClockOffset]';    
    end

    %% Algorithm    
    POS_E__M           = pos_E__m(ii,:);
    RX_CLOCK_OFFSET__M = rx_clock_offset__m(ii);
    ZWD__m             = zwd__m(ii);
    AMBIG              = ambig(ii,idx_sv);

    % initialize ambiguities
    idx = AMBIG == 0;
    Code = [Observations.Code];
    rho_iono_free = Generic.CalcIonoFreeLinearCombination([Code.Band_1]', [Code.Band_2]', gnss.F_L1_GPS__Hz, gnss.F_L2_GPS__Hz);

    dist_svrx__m = generic.calcDistanceSvRx(eph_x_E__m, eph_y_E__m, eph_z_E__m, POS_E__M(1), POS_E__M(2), POS_E__M(3));
    N_initial = (rho_iono_free - dist_svrx__m) / 0.1070;
    
    if any(idx)
        AMBIG(idx) = N_initial(idx);
    end

    for jj = 1:10 
        % Apply Pseudoranges
        [A_rho, y_rho] = generic.PseudorangeUpdate( POS_E__M(1), POS_E__M(2), POS_E__M(3), ...
                                            RX_CLOCK_OFFSET__M, ...
                                            ZWD__m, ...
                                            [Observations.Code], ...
                                            eph_x_E__m, eph_y_E__m, eph_z_E__m, ...
                                            sv_clock_offset__m, sv_relativistic__m);
        
        [A_phi, y_phi] = generic.CarrierPhaseUpdate( POS_E__M(1), POS_E__M(2), POS_E__M(3), ...
                                             RX_CLOCK_OFFSET__M, ...
                                             ZWD__m, ...
                                             AMBIG, ...
                                             [Observations.Carrier], ...
                                             eph_x_E__m, eph_y_E__m, eph_z_E__m, ...
                                             sv_clock_offset__m, sv_relativistic__m);

        % calculate satellites elevation
        elevation = Generic.CalcElevation(POS_E__M(1), POS_E__M(2), POS_E__M(3), eph_x_E__m, eph_y_E__m, eph_z_E__m);

        % Apply elevation filter only if more than 4SVs available
        idx_el = elevation > deg2rad(15) & elevation < deg2rad(165);
        if nnz(idx_el) > 4 
            idx_col = [true(5,1);idx_el]; % columns are dependent on available satellites

            A_rho = A_rho(idx_el,idx_col);
            y_rho = y_rho(idx_el);
            
            A_phi = A_phi(idx_el, idx_col);
            y_phi = y_phi(idx_el);
        else
            break;
        end 
        
        THETA = [A_rho; A_phi];
        Z = [y_rho; y_phi];

        [x_hat, P] = estimation.lls(THETA, Z);

        % State update
        POS_E__M           = POS_E__M + x_hat(1:3)';        
        RX_CLOCK_OFFSET__M = RX_CLOCK_OFFSET__M + x_hat(4);
        ZWD__m             = ZWD__m + x_hat(5);

        if nnz(idx_el) > 4 
            AMBIG(idx_el) = AMBIG(idx_el) + x_hat(6:end)';
        else
            AMBIG = AMBIG + x_hat(6:end)';
        end

        if all(abs(x_hat(1:3)) < 1e-3)
            break;
        end
    end
        
    pos_E__m(ii+1,:)          = POS_E__M;
    rx_clock_offset__m(ii+1)  = RX_CLOCK_OFFSET__M;
    zwd__m(ii+1)              = ZWD__m;
    ambig(ii+1,idx_sv)        = AMBIG;

    n_sat(ii+1)   = nnz(idx_el);
    P_pos(ii+1,:) = diag(P(1:3, 1:3));

    [~, ib] = ismember({Observations.SatelliteSystem}, meta.AvailableSatelliteSystems);
    elevation_out(ii+1,ib(idx_el)) = elevation(idx_el)';
end

% adjust off-by-one index
pos_E__m           = pos_E__m(2:end,:);
rx_clock_offset__m = rx_clock_offset__m(2:end);
zwd__m             = zwd__m(2:end);

P_pos = P_pos(2:end,:);
n_sat = n_sat(2:end);

%%
af = afigure(42);
subplot(5,2,1);
hold on; grid on;
yline(refpos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,1), 'DisplayName', 'LSQ (5 States)')
if plotCov
    plot(pos_E__m(:,1) + sqrt(P_pos(:,1)), 'Color', Color.RED, 'DisplayName', 'Cov');
    plot(pos_E__m(:,1) - sqrt(P_pos(:,1)), 'Color', Color.RED, 'DisplayName', 'Cov');
end

subplot(5,2,2);
hold on; grid on;
plot(refpos__m(1) - pos_E__m(:,1), 'DisplayName', 'LSQ (5 States)')

subplot(5,2,3);
hold on; grid on;
yline(refpos__m(2), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,2), 'DisplayName', 'LSQ (5 States)')
if plotCov
    plot(pos_E__m(:,2) + sqrt(P_pos(:,2)), 'Color', Color.RED, 'DisplayName', 'Cov');
    plot(pos_E__m(:,2) - sqrt(P_pos(:,2)), 'Color', Color.RED, 'DisplayName', 'Cov');
end

subplot(5,2,4);
hold on; grid on;
plot(refpos__m(2) - pos_E__m(:,2), 'DisplayName', 'LSQ (5 States)')

subplot(5,2,5);
hold on; grid on;
yline(refpos__m(3), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(pos_E__m(:,3), 'DisplayName', 'LSQ (5 States)')
if plotCov
    plot(pos_E__m(:,3) + sqrt(P_pos(:,3)), 'Color', Color.RED, 'DisplayName', 'Cov');
    plot(pos_E__m(:,3) - sqrt(P_pos(:,3)), 'Color', Color.RED, 'DisplayName', 'Cov');
end

subplot(5,2,6);
hold on; grid on;
plot(refpos__m(3) - pos_E__m(:,3), 'DisplayName', 'LSQ (5 States)')

subplot(5,2,7);
hold on; grid on;
title('dt_r')
plot(rx_clock_offset__m ./ Transformation.SpeedOfLight__mDs, 'DisplayName', 'LSQ (5 States)')

subplot(5,2,8);
hold on; grid on;
title('ZTD_w')
plot(zwd__m, 'DisplayName','zwd__m')

subplot(5,2,9);
hold on; grid on;
title('nSat')
plot(n_sat, 'DisplayName', 'nsat');

subplot(5,2,10);
hold on; grid on;
title('elevations')
plot(rad2deg(elevation_out), 'DisplayName', 'elevation');

af.linkaxes('x');
af.showlegend();