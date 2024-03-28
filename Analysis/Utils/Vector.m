classdef Vector < handle
    %VECTOR Summary of this class goes here
    %   Detailed explanation goes here
        
    methods (Static = true)
        function dist = EuclidianDistance_3D(x1, y1, z1, x2, y2, z2)
            dx = (x1 - x2);
            dy = (y1 - y2);
            dz = (z1 - z2);
            dist = sqrt( dx.^2 + dy.^2 + dz.^2);            
        end

        function d12 = DistanceVector_3D(x1, y1, z1, x2, y2, z2)
            dx = (x1 - x2);
            dy = (y1 - y2);
            dz = (z1 - z2);
            
            d12 = [dx, dy, dz];
        end

        function e = NormalizedDistanceVector(x1, y1, z1, x2, y2, z2)
            d = Vector.DistanceVector_3D(x1, y1, z1, x2, y2, z2);
            e = d ./ sqrt(sum(d.^2,2));
        end
    end
end

