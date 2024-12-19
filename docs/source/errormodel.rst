.. _errormodel:

Error Modelling
===============

Pseudorange
-----------
Various errors, such as satellite and receiver clock errors (:math:`\delta t^s, \delta t_r`), as well as ionospheric (:math:`I`) tropospheric delays (:math:`T`), need to be accounted for.

.. math::    
    \rho^s_r(t) = \| r_r(t) - r^s(t)\| + c(\delta t^s - \delta t_r) + I + T + \epsilon_r^s
