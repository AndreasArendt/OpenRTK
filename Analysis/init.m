BasePath = fileparts(mfilename("fullpath"));
addpath(genpath(BasePath))
addpath(genpath(fullfile(BasePath, 'Misc')))
addpath(genpath(fullfile(BasePath, 'SPP')))
addpath(genpath(fullfile(BasePath, 'PPP')))
addpath(genpath(fullfile(fileparts(BasePath), 'submodules', 'utils', 'matlab')))