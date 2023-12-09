clear;

eph = table2struct(readtable("Analysis\Ephemeris_exp.csv"), 'ToScalar',true);
obs = table2struct(readtable("Analysis\Observation_exp.csv"), 'ToScalar',true);

eph.Time_Sv_Tuple = strcat(string(eph.ObsToc), "_", string(eph.SvSystem));
obs.Time_Sv_Tuple = strcat(string(obs.UTC), "_", string(obs.SvSystem));

t_eph = struct2table(eph);
t_obs = struct2table(obs);

T = join(t_eph, t_obs, 'Keys',{'Time_Sv_Tuple'}');
S = table2struct(T, 'ToScalar', true);

% idx = cellfun(@(x)strcmp(x(1),'E'), {S.SvSystem_t_eph});
% S = S(idx);
% S = table2struct(struct2table(S), 'ToScalar', true);

S.SV_numbers = cellfun(@(x)(str2num(x(2:end))), S.SvSystem_t_eph);

% station_pos__m = [4164313.0000, 803525.9000, 4748474.9000]; %AUBG
station_pos__m = [4498451.8100  1708266.8300  4173591.7800]; %ORID
% station_pos__m = [  3814004.8279   361287.8653  5082327.1526  ]; %HARL

F_E1_Galileo__Hz  = 1575.420e6;
F_E5a_Galileo__Hz = 1176.450e6;

clear eph;
clear obs;