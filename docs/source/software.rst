.. _software:

Software
========

Currently, the software is separated into two main projects: C++ and MATLAB. The C++ project handles at the moment conversion of ``obs``, ``nav``, ``clk`` and ``sp3`` files to ``json`` files. The MATLAB project uses the ``json`` file to run the (precise) GNSS algorithms.

Single Point Positioning (SPP)
==============================

Implementation can be found here: ``Analysis/+SPP``

The latest version uses a least squares approach to solve the SPP problem.

The following states are estimated:

.. math::
    \begin{pmatrix}
    x_\text{ECEF__m} \\
    y_\text{ECEF__m} \\
    z_\text{ECEF__m} \\
    \text{rx_clock_offset__m} \\
    \text{zenith_tropo_delay__m}
    \end{pmatrix}
    

Precise Point Positioning (PPP)
===============================
Implementation can be found here: ``Analysis/+PPP``

The latest version uses a least squares approach to solve the PPP problem. Precise satellite ephemerides data aswell as precise clock data are used to calculate the PPP position of the receiver. For this, ``sp3`` and ``clk`` files are used. The ``sp3`` file contains the precise satellite ephemerides data, while the ``clk`` file contains the precise clock data. The ``obs`` file is used to calculate the pseudorange and carrier phase measurements.
The ``OpenRTK.exe`` needs to be used with flags ``obs``, ``sp3`` and ``clk`` to generate the ``json`` file. The ``json`` file is then used to run the PPP algorithm.

The following states are estimated:

.. math::
    \begin{pmatrix}
    x_\text{ECEF__m} \\
    y_\text{ECEF__m} \\
    z_\text{ECEF__m} \\
    \text{rx_clock_offset__m} \\
    \text{zenith_wet_delay__m} \\
    \text{ambiguities} \\
    \end{pmatrix}

Ambiguities for all satellites in ionosphere-free-combination are estimated.