clear;

x = 0:0.1:20;
y = cos(x)';

% Parameters
rng('default');  % Set the random seed for reproducibility
numSamples = numel(x);  % Number of samples
meanValue = 0;  % Mean of the Gaussian distribution
stdDeviation = 0.1;  % Standard deviation of the Gaussian distribution

% Generate white Gaussian noise
whiteNoise = meanValue + stdDeviation * randn(numSamples, 1);

meas = y + whiteNoise;

k = Kalman.Kalman(1, 1, 1);
k.SetInitialStates(1);

Hx = @(x) -sin(x);
Fx = @(x) 1-acos(x);

for ii = 1:numel(x)    
    % k.Predict();
    k.PredictEKF(Fx);
    v(ii) = k.CorrectEKF(Hx, meas(ii),0.1);
    X(ii) = k.X;
end

%%
figure;
subplot(2,1,1)
hold on;
plot(x, y, 'DisplayName', 'real')
plot(x, meas, 'DisplayName', 'meas')
plot(x, cos(X),'DisplayName', 'KF')
plot(x, X,'DisplayName', 'KF')
legend('show')

subplot(2,1,2)
hold on;
plot(x, v)