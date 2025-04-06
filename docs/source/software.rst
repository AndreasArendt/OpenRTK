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
    \mathbf{x} = \begin{pmatrix}
    x_\text{ECEF} \\
    y_\text{ECEF} \\
    z_\text{ECEF} \\
    \text{rx\_clock\_offset} \\
    \text{zenith\_tropo\_delay}
    \end{pmatrix}
    

Precise Point Positioning (PPP)
===============================
Implementation can be found here: `Analysis/+PPP`
