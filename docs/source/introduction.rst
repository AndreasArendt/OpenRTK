.. _introduction:

Introduction to GNSS Signals
============================
GNSS satellites broadcast signals from which various measurements can be obtained. The following sections will explain the `Pseudorange`, `Carrier phase`, and `Doppler` measurements in detail.

Pseudorange
-----------
The `Pseudorange` measurement represents the distance between the satellite and the receiver. This distance is calculated by multiplying the speed of light (:math:`c`) by the difference between the satellite clock time at signal transmission (:math:`t^s`) and the receiver clock time at signal reception (:math:`t_r`).

.. math::    
    \rho^s_r(t) = \| r_r(t) - r^s(t)\|

Carrier phase
-------------
The `Carrier phase` measurement represents the phase of the received signal at the time of reception. This measurement consists of the unknown integer number of full cycles between the receiver and the transmitter, plus the current phase of the signal, which together is the distance between the receiver and the satellite.

To calculate an accurate position, the unknown integer number (:math:`N`) of full phase cycles must be estimated and multiplied with the signal frequency (:math:`\lambda`)

.. math::
    \phi(t) = \phi_r(t) - \phi^s(t) + N \lambda


Doppler
-------
The `Doppler` measurements represent the change in the received frequency of the signal by the Doppler effect. It represents the line-of-sight velocity.

Timebases
=========

.. list-table:: Timebases
   :widths: 10 20 70
   :header-rows: 1

   * - Abbreviation
     - Name
     - Description
   * - TOE
     - Time of Ephemeris
     - Time when the epoch corresponds to the time when the satellite's Keplerian elements are valid and define the actual location.
   * - TOC
     - Time of Clock / Epoch time
     - Clock correction data reference time of week, when clock correction data is valid.   
   * - GPST
     - GPS Time
     - GPS time starts at :math:`6^{th}` January 1980 00:00:00.000 UTC. It is not affected by leap seconds.
   * - POSIX
     - POSIX Time
     - Time since 00:00:00 UTC on 1 January 1970.
   * - GST
     - Galileo System Time
     - | 32-bit binary number composed of two parameters: week number and time of week (TOW). 
       | The week number is represented as 12-bit number and covers 4096 weeks (~78.7 years) whereafter the counter is reset to 0.
       | GST Start is defined as 13 seconds before midnight between :math:`21^{st}` August and :math:`22^{nd}` August 1999.
       | e.g. GST was 13 seconds at :math:`22^{nd}` August 1999 00:00:00.000 UTC.
       | As GST is a continuous time scale, it is not affected by leap seconds.
   * - TOW
     - Time of Week
     - | number of seconds that have occurred since the transition from the previous week \\
       | Its range is :math:`[0; 604799] (7*24*60*60-1)` and is equal to 7 days in seconds minus 1 second.
