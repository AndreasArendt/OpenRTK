classdef Transformation
    %TRANSFORMATION Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (Constant = true)
        MeanAngularVelocityOfEarth__radDs = 7.2921151467e-5;
        SpeedOfLight__mDs = 299792458;
    end
    
    methods (Static = true)
        function M = M_x(phi)
            s = sin(phi);
            c = cos(phi);

            M = [ 1  0  0;
                  0  c -s;
                  0  s  c];
        end

        function M = M_y(theta)
            s = sin(theta);
            c = cos(theta);

            M = [ c  0  s;
                  0  1  0;
                 -s  0  c];
        end

        function M = M_z(psi)
            s = sin(psi);
            c = cos(psi);

            M = [ c -s  0;
                  s  c  0;
                  0  0  1];
        end

        function [E, N, U] = ecef2enu(lat__rad, lon__rad, x__m, y__m, z__m)
            Mx = Transformation.M_x(pi/2 - lat__rad);
            Mz = Transformation.M_z(pi/2 + lon__rad);

            ENU = zeros(numel(x__m),3);
            for ii = 1:numel(x__m)
                ENU(ii,:) = Mx.' * Mz.' * [x__m(ii); y__m(ii); z__m(ii)];
            end            

            E = ENU(:,1);
            N = ENU(:,2);
            U = ENU(:,3);
        end

        function [lat__rad, lon__rad, alt__m] = ecef2wgs84(x__m, y__m, z__m) 
        %input in m, output in rad and meters
        
            % WGS84 ellipsoid constants:
            a = 6378137;
            es = (8.1819190842622e-2)^2;
            
            % calculations:
            b   = sqrt(a^2*(1-es));
            ep  = (a^2-b^2)/b^2;
            
            p   = sqrt(x__m.^2+y__m.^2);
            th  = atan2(a*z__m,b*p);
            lon__rad = atan2(y__m,x__m);
            lat__rad = atan2((z__m+ep.*b.*sin(th).^3),(p-es.*a.*cos(th).^3));
            N   = a./sqrt(1-es.*sin(lat__rad).^2);
            alt__m = p./cos(lat__rad)-N;
            
            % return lon in range [0,2*pi)
            %lon = mod(lon,2*pi); %atan2 should handle this fine
            
            % correct for numerical instability in altitude near exact poles:
            % (after this correction, error is about 2 millimeters, which is about
            % the same as the numerical precision of the overall function)    
            k=abs(x__m)<1 & abs(y__m)<1;
            alt__m(k) = abs(z__m(k))-b;
        end

    end
end

