#ifndef PLAN13
#define PLAN13

class Plan13 {
 public:
  void setLocation  (double lon, double lat, int height);
  void setTime      (int yearIn, int monthIn, int mDayIn,
      int hourIn, int minIn, float secIn);
  void setElements  (double YE_in, double TE_in, double IN_in,
      double RA_in, double EC_in, double WP_in,
      double MA_in, double MM_in, double M2_in,
      double RV_in, double ALON_in );
  void calculate    ();
  void printdata    (void) const;

  double getAzimuth() const;
  double getElevation() const;
  double getLongitude() const;
  double getLatitude() const;
 private:
  inline double rad      (double deg);
  inline double deg      (double rad);
  double FNatn    (double y, double x);
  double FNday    (int year, int month, int day);
  void   initSat  (void);
  void   satvec   (void);
  void   rangevec (void);

  double observer_lon;
  double observer_lat;
  int    observer_height;

  const static double   YM = 365.25;  // Days in a year
  double   EL;                        // Elevation 
  double   TN;

  double   E;
  double   N;
  double   AZ;
  double   SLON;
  double   SLAT;
  double   RR;

  double   CL;
  double   CS;
  double   SL;
  double   CO;
  double   SO;
  double   RE;
  double   FL;
  double   RP;
  double   XX;
  double   ZZ;
  double   D;
  double   R;
  double   Rx;
  double   Ry;
  double   Rz;
  double   Ex;
  double   Ey;
  double   Ez;
  double   Ny;
  double   Nx;
  double   Nz;
  double   Ox;
  double   Oy;
  double   Oz;
  double   U;
  double   Ux;
  double   Uy;
  double   Uz;
  const static double   YT = 365.2421970;
  double   WW;
  double   WE;
  double   W0;
  double   VOx;
  double   VOy;
  double   VOz;
  double   DE;
  double   GM;
  double   J2;
  double   N0;
  double   A0;
  double   b0;
  double   SI;
  double   CI;
  double   PC;
  double   QD;
  double   WD;
  double   DC;
  double   YG;
  double   G0;
  double   MAS0;
  double   MASD;
  double   INS;
  double   CNS;
  double   SNS;
  double   EQC1;
  double   EQC2;
  double   TEG;
  double   GHAE;
  double   MRSE;
  double   MASE;
  double   ax;
  double   ay;
  double   az;
  int      OLDRN;

  double   T;
  double   DT;
  double   KD;
  double   KDP;
  double   M;
  int      DR;
  long     RN;
  double   EA;
  double   C;
  double   S;
  double   DNOM;
  double   A;
  double   B;
  double   RS;
  double   Sx;
  double   Sy;
  //double   Sz;
  double   Vx;
  double   Vy;
  double   Vz;
  double   AP;
  double   CWw;
  double   SW;
  double   RAAN;
  double   CQ;
  double   SQ;
  double   CXx;
  double   CXy;
  double   CXz;
  double   CYx;
  double   CYy;
  double   CYz;
  double   CZx;
  double   CZy;
  double   CZz;
  double   SATx;
  double   SATy;
  double   SATz;
  double   ANTx;
  double   ANTy;
  double   ANTz;
  double   VELx;
  double   VELy;
  double   VELz;
  double   Ax;
  double   Ay;
  double   Az;
  double   Sz;
  //double   Vz;
  double   GHAA;

  double   DS;
  double   DF;

  // keplerians

  char     SAT[20];
  long     SATNO;
  double   YE;
  double   TE;
  double   IN;
  double   RA;
  double   EC;
  double   WP;
  double   MA;
  double   MM;
  double   M2;
  long     RV;
  double   ALON;
  double   ALAT;
  double   rxOut;
  double   txOut;

  // location
  char     LOC[20];
  double   LA;
  double   LO;
  double   HT;

  double   HR; // Hours
  double   DN;
};

#endif
