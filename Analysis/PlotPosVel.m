idx = cellfun(@(x) strcmp(x, 'G18'), S.SvSystem_t_eph);
% idx = cellfun(@(x) strcmp(x, 'E34'), S.SvSystem_t_eph);

station_pos__m = [4498451.8100  1708266.8300  4173591.7800]; %ORID
r = sqrt( (S.x(idx) - station_pos__m(1)).^2 + ...
          (S.y(idx) - station_pos__m(2)).^2 + ... 
          (S.z(idx) - station_pos__m(3)).^2);

figure;
subplot(3,1,1)
hold on;
plot(S.xdot(idx))
plot(S.ydot(idx))
plot(S.zdot(idx))
plot(diff(S.x(idx)) ./ diff(S.ObsToc(idx)), '--');
plot(diff(S.y(idx)) ./ diff(S.ObsToc(idx)), '--');
plot(diff(S.z(idx)) ./ diff(S.ObsToc(idx)), '--');

subplot(3,1,2)
hold on;
plot( r )
plot(S.Phase_1(idx) .* Transformation.SpeedOfLight__mDs / (1575.420 .* 1e6), 'DisplayName', 'Phase1');
% plot( sqrt(S.xdot(idx).^2 + S.ydot(idx).^2 + S.zdot(idx).^2) ./ 3.6, 'DisplayName','rinex')
% plot( sqrt( (diff(S.x(idx)) ./ diff(S.ObsToc(idx))).^2 + ...
            % (diff(S.y(idx)) ./ diff(S.ObsToc(idx))).^2 + ...
            % (diff(S.z(idx)) ./ diff(S.ObsToc(idx))).^2 ) ./3.6, 'DisplayName', 'diff eph' );
% plot((S.Phase_1(idx) .* 1575.420), 'DisplayName', 'Phase1');
% plot(S.Phase_2(idx) .* 1176.450, 'DisplayName', 'Phase2');
legend('show')

subplot(3,1,3)
hold on;
plot(S.x(idx))
plot(S.y(idx))
plot(S.z(idx))