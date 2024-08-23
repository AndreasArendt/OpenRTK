.. _introduction:
Introduction to GNSS Signals
====
GNSS satellites broadcast signals from which various measurements can be obtained. The following sections will explain the `Pseudorange`, `Carrier phase`, and `Doppler` measurements in detail.

Pseudorange
----
The `Pseudorange` measurement represents the distance between the satellite and the receiver. This distance is calculated by multiplying the speed of light (:math:`c`) by the difference between the satellite clock time at signal transmission (:math:`t^s`) and the receiver clock time at signal reception (:math:`t_r`).

Various errors, such as satellite and receiver clock errors (:math:`\delta t^s, \delta t_r`), as well as ionospheric (:math:`I`) tropospheric delays (:math:`T`), need to be accounted for.

.. math::    
    \rho^s_r(t) = \| r_r(t) - r^s(t)\|

Carrier phase
----
The `Carrier phase` measurement represents the phase of the received signal at the time of reception. This measurement consists of the unknown integer number of full cycles between the receiver and the transmitter, plus the current phase of the signal, which together is the distance between the receiver and the satellite.

To calculate an accurate position, the unknown integer number (:math:`N`) of full phase cycles must be estimated and multiplied with the signal frequency (:math:`\lambda`)

.. math::
    \phi(t) = \phi_r(t) - \phi^s(t) + N \lambda


Doppler
----
The `Doppler` measurements represent the change in the received frequency of the signal by the Doppler effect.