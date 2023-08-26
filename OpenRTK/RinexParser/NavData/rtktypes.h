#pragma once

#include <ctime>

typedef __time64_t time_t;

typedef struct {        /* time struct */
    time_t time;        /* time (s) expressed by standard time_t */
    double sec;         /* fraction of second under 1 s */
} gtime_t;

typedef struct {        /* GPS/QZS/GAL broadcast ephemeris type */
    int sat;            /* satellite number */
    int iode, iodc;      /* IODE,IODC */
    int sva;            /* SV accuracy (URA index) */
    int svh;            /* SV health (0:ok) */
    int week;           /* GPS/QZS: gps week, GAL: galileo week */
    int code;           /* GPS/QZS: code on L2 */
    /* GAL: data source defined as rinex 3.03 */
    /* BDS: data source (0:unknown,1:B1I,2:B1Q,3:B2I,4:B2Q,5:B3I,6:B3Q) */
    int flag;           /* GPS/QZS: L2 P data flag */
    /* BDS: nav type (0:unknown,1:IGSO/MEO,2:GEO) */
    double toe, toc, ttr; /* Toe,Toc,T_trans */
    /* SV orbit parameters */
    double A, e, i0, OMG0, omg, M0, deln, OMGd, idot;
    double crc, crs, cuc, cus, cic, cis;
    double toes;        /* Toe (s) in week */
    double fit;         /* fit interval (h) */
    double f0, f1, f2;    /* SV clock parameters (af0,af1,af2) */
    double tgd[6];      /* group delay parameters */
    /* GPS/QZS:tgd[0]=TGD */
    /* GAL:tgd[0]=BGD_E1E5a,tgd[1]=BGD_E1E5b */
    /* CMP:tgd[0]=TGD_B1I ,tgd[1]=TGD_B2I/B2b,tgd[2]=TGD_B1Cp */
    /*     tgd[3]=TGD_B2ap,tgd[4]=ISC_B1Cd   ,tgd[5]=ISC_B2ad */
    double Adot, ndot;   /* Adot,ndot for CNAV */
} eph_t;
