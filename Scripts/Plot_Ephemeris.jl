using Plots
using CSV
using DataFrames

df = CSV.read("D:/Projekte/OpenRTK/Scripts/Ephemeris.csv", DataFrame; header=[:SV, :time, :x, :y, :z])

x = df."x"
y = df."y"
z = df."z"

plot(x)