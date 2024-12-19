.. _errormodel:

Error Modelling
===============

Pseudorange
-----------
Various errors, such as satellite and receiver clock errors (:math:`\delta t^s, \delta t_r`), as well as ionospheric (:math:`I`) tropospheric delays (:math:`T`), need to be accounted for.

.. math::    
    \rho^s_r(t) = p^r_s + c(\delta t^s - \delta t_r) + T + I + \epsilon_r^s

Carrierphase
------------
The carrier phase measurement is affected by similar errors as the Pseudorange, in addition the carrier wavelength (:math:`\lambda`) and the number of waves (:math:`N`) need to be accounted for.

.. math::   
    \phi^s_r(t) = p^r_s + c(\delta t^s - \delta t_r) + T - I + N \lambda + \epsilon_r^s