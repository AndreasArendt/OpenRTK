% initialize with final estimation
x_N = kf.X;
P_N = kf.P;

N = size(X_post,1);

F = eye(nStates);
P_RTS = zeros(nStates,nStates,N);
X_RTS = zeros(N,nStates);

P_RTS(:,:,end) = kf.P;
X_RTS(end,:)   = kf.X.';

for k = N-1:-1:1     
    % RTS    
    K_k = (P_post(:,:,k) * F.') / P_pri(:,:,k+1);
    P_RTS(:,:,k) = P_post(:,:,k) - K_k * (P_pri(:,:,k+1) - P_RTS(:,:,k+1)) * K_k.';
    X_RTS(k,:) = (X_post(k,:).' + K_k * (X_RTS(k+1,:)' - X_pri(k+1,:).')).';
end

%%
afigure(42);
subplot(4,2,1);
hold on; grid on;
yline(refpos__m(1), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(X_RTS(:,1), 'DisplayName', 'RTS (5 States)')
ylim([refpos__m(1)-50,  refpos__m(1)+50])
legend('show')

subplot(4,2,2);
hold on; grid on;
plot(refpos__m(1) - X_RTS(:,1), 'DisplayName', 'RTS (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,3);
hold on; grid on;
yline(refpos__m(2), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(X_RTS(:,2), 'DisplayName', 'RTS (5 States)')
ylim([refpos__m(2)-50, refpos__m(2)+50])
legend('show')

subplot(4,2,4);
hold on; grid on;
plot(refpos__m(2) - X_RTS(:,2), 'DisplayName', 'RTS (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,5);
hold on; grid on;
yline(refpos__m(3), 'Color', Color.BLACK, 'HandleVisibility','off')
plot(X_RTS(:,3), 'DisplayName', 'RTS (5 States)')
ylim([refpos__m(3)-50, refpos__m(3)+50])
legend('show')

subplot(4,2,6);
hold on; grid on;
plot(refpos__m(3) - X_RTS(:,3), 'DisplayName', 'RTS (5 States)')
ylim([-20 20])
legend('show')

subplot(4,2,7);
hold on; grid on;
title('dt_r')
plot(X_RTS(:,4) ./ Transformation.SpeedOfLight__mDs, 'DisplayName', 'RTS (5 States)')
legend('show')

subplot(4,2,8);
hold on; grid on;
title('ZTD_w')
plot(X_RTS(:,5), 'DisplayName', 'RTS (5 States)')
legend('show')