classdef Kalman < handle
    %KALMAN Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (SetAccess = private, GetAccess = public)
        X % State Matrix
        P % Covarainace Matrix
        Q % Process noise Matrix
        F % Transition Matrix                

        nStates = NaN;
    end
        
    methods
        function obj = Kalman(f, q)
            nStates = size(f,1);

            obj.X = zeros(nStates,1);
            obj.P = eye(nStates,nStates);
            obj.Q = q;
            obj.F = f;

            obj.nStates = nStates;

            assert(size(f,1) == nStates, 'Wrong size of Transition matrix');
            assert(size(f,2) == nStates, 'Wrong size of Transition matrix');
            assert(size(q,1) == nStates, 'Wrong size of Process Noise matrix')
            assert(size(q,2) == nStates, 'Wrong size of Process Noise matrix')
        end
        
        function SetInitialStates(obj, x0)
            assert(all(size(x0) == size(obj.X)), 'Check if all States have been initialized!');
            obj.X = x0;
        end

        function SetInitialCovariance(obj, p0)
            obj.P = p0;
        end

        function Predict(obj)            
            obj.X = obj.F * obj.X; % + Bu
            obj.P = obj.F * obj.P * obj.F.' + obj.Q;
        end  

        function PredictIdx(obj, idx)
            idx = idx == 1;
            obj.X(idx)     = obj.F(idx,idx) * obj.X(idx); % + Bu
            obj.P(idx,idx) = obj.F(idx,idx) * obj.P(idx,idx) * obj.F(idx,idx).' + obj.Q(idx,idx);
        end  

        function v = Correct(obj, H, z, R)
            K = obj.P * H.' / (H * obj.P * H.' + R);
            v = z - H * obj.X; % residual
            obj.X = obj.X + K * v;

            IKH = (eye(obj.nStates) - K * H);

            obj.P = IKH * obj.P * IKH.' + K*R*K.';
        end

        function X = CorrectIterativeEKF(obj, H, dy, R)
            K = obj.P * H.' / (H * obj.P * H.' + R);            
            X = obj.X + K * dy;
            obj.X = X;
        end

        function CorrectIterativeEKFIdx(obj, H, dy, R, idx)
            idx = idx==1;
            
            K = obj.P(idx,idx) * H.' / (H * obj.P(idx,idx) * H.' + R);            
            obj.X(idx) = obj.X(idx) + K * dy;            
        end

        function CorrectEKF(obj, H, dy, R)
            I = eye(obj.nStates);

            K = obj.P * H.' / (H * obj.P * H.' + R);            
            obj.X = obj.X + K * dy;

            IKH = I - K * H;
            obj.P = IKH * obj.P * IKH.' + K*R*K.'; % Joseph's Form
        end

        function CorrectEKFIdx(obj, H, dy, R, idx)
            idx = idx == 1;
            I = eye(nnz(idx));

            K = obj.P(idx,idx) * H' / (H * obj.P(idx,idx) * H.' + R);
            obj.X(idx) = obj.X(idx) + K * dy;

            IKH = I - K * H;
            obj.P(idx,idx) = IKH * obj.P(idx,idx) * IKH.' + K*R*K.'; % Joseph's Form
        end

        function v = UpdateEKF_State(obj, H, v, R, varargin)
            ip = inputParser;
            ip.addOptional('Index',ones(size(obj.nStates)));
            ip.parse(varargin{:});
            idx = ip.Results.Index;

            K = obj.P(idx,idx) * H.' / (H * obj.P(idx,idx) * H.' + R);            
            obj.X(idx) = obj.X(idx) + K * v;
        end

        function UpdateEKF_Covariance(obj, H, R, varargin)
            ip = inputParser;
            ip.addOptional('Index',ones(size(obj.nStates)));
            ip.parse(varargin{:});
            idx = ip.Results.Index;
            
            K = obj.P(idx,idx) * H.' / (H * obj.P(idx,idx) * H.' + R);            
            IKH = (eye(sum(idx)) - K * H);
            obj.P(idx,idx) = IKH * obj.P(idx,idx) * IKH.' + K*R*K.';
        end
    end
end

