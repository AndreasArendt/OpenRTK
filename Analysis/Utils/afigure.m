classdef afigure < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (Access=public)
        handle
    end

    properties(Access=private)
        vBar = []
    end
    
    methods
        function obj = afigure(varargin)
            obj.handle = figure(varargin{:});
            set(obj.handle,'WindowButtonDownFcn', @obj.AxisClick_Callback);
        end                

        function linkaxes(obj, axes)
            linkaxes(obj.handle.Children, axes);
        end
    end

    methods (Access = protected)
        function AxisClick_Callback(obj, hObject, eventdata)             
            % in case outside of axis is clicked (e.g. plus/minus sign)
            if eventdata.Source.CurrentAxes.CurrentPoint(1,2) > eventdata.Source.CurrentAxes.YLim
                return;
            end

            if ~isempty(obj.vBar)
                % delete line
                for ii = 1:numel(obj.vBar)
                    set(obj.vBar(ii), 'Value', eventdata.Source.CurrentAxes.CurrentPoint(1));                    
                end
            else
                % plot line
                for c = hObject.Children.'
                    obj.vBar(end+1) = xline(c, eventdata.Source.CurrentAxes.CurrentPoint(1), 'Color', Color.RED);
                end  
            end
        end
    end
end

