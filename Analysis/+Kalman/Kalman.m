classdef Kalman < handle
    %KALMAN Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private, GetAccess = public)
        X % State Matrix
        P % Covarainace Matrix
        Q % Process noise Matrix
        R % Measurement noise Matrix
        F % Transition Matrix                

        nStates = NaN;
    end
        
    methods
        function obj = Kalman(nStates, f, q)
            obj.X = zeros(nStates,1);
            obj.P = ones(nStates,nStates);
            obj.Q = q;
            obj.F = f;

            obj.nStates = nStates;

            assert(size(f,1) == nStates, 'Wrong size of Transition matrix');
            assert(size(f,2) == nStates, 'Wrong size of Transition matrix');
        end
        
        function SetInitialStates(obj, x0)
            obj.X = x0;
        end

        function SetInitialCovariance(obj, p0)
            obj.P = p0;
        end

        function Predict(obj)            
            obj.X = obj.F * obj.X; % + Bu
            obj.P = obj.F * obj.P * obj.F.' + obj.Q;
        end  

        function v = Correct(obj, H, z, R)
            K = obj.P * H.' / (H * obj.P * H.' + R);
            v = z - H * obj.X; % residual
            obj.X = obj.X + K * v;

            IKH = (eye(obj.nStates) - K * H);

            obj.P = IKH * obj.P * IKH.' + K*R*K.';
        end

        function v = CorrectEKF(obj, H, v, R)
            K = obj.P * H.' / (H * obj.P * H.' + R);            
            obj.X = obj.X + K * v;

            IKH = (eye(obj.nStates) - K * H);

            obj.P = IKH * obj.P * IKH.' + K*R*K.';
        end

        function v = UpdateEKF_State(obj, H, v, R)
            K = obj.P * H.' / (H * obj.P * H.' + R);            
            obj.X = obj.X + K * v;
        end

        function v = UpdateEKF_Covariance(obj, H, v, R)
            K = obj.P * H.' / (H * obj.P * H.' + R);            
            IKH = (eye(obj.nStates) - K * H);
            obj.P = IKH * obj.P * IKH.' + K*R*K.';
        end
    end
end

