#include "plan13.h"
#include <stdio.h>

#define _USE_MATH_DEFINES
#include "math.h"

#define DEBUG false

/*
 * Convert degrees to radians
 */
double Plan13::rad(double deg){
  return (M_PI / 180.0 * deg);
}

double Plan13::deg(double rad){
  return (rad * 180.0 / M_PI);
}

double Plan13::FNatn(double y, double x){
  return atan2(y, x);
}

/* Convert date to day number
 *
 * Function returns a general day number from year, month, and day.
 * Value is (JulianDay - 1721409.5) or (AmsatDay + 722100)
 *
 */
double Plan13::FNday(int year, int month, int day){
  if (month <= 2){
    year -= 1;
    month += 12;
  }
 
  return (long)(year * YM) + (int)((month + 1) * 30.6) + (day - 428);
}


void Plan13::initSat(void){
  // Observer's location
  if (DEBUG) {printf("Start initSat()\n");}

  LA = rad( observer_lat );
  LO = rad( observer_lon );
  HT = ((float) observer_height)/1000.0; // this needs to be in km
  CL = cos(LA);
  SL = sin(LA);
  CO = cos(LO);
  SO = sin(LO);
  // WGS-84 Earth Ellipsoid
  RE = 6378.137;
  FL = 1.0 / 298.257224;

  // IAU-76 Earth Ellipsoid
  // RE = 6378.140;
  // FL = 1.0 / 298.257;

  RP = RE * (1.0 - FL);
  XX = RE * RE;
  ZZ = RP * RP;

  D = sqrt( XX * CL * CL + ZZ * SL * SL );

  Rx = XX / D + HT;
  Rz = ZZ / D + HT;

  // Observer's unit vectors Up EAST and NORTH in geocentric coordinates 
  Ux = CL * CO;
  Ex = -SO;
  Nx = -SL * CO;

  Uy = CL * SO;
  Ey = CO;
  Ny = -SL * SO;

  Uz = SL;
  Ez = 0;
  Nz = CL;

  // Observer's XYZ coordinates at earth's surface 
  Ox = Rx * Ux;
  Oy = Rx * Uy;
  Oz = Rz * Uz;

  // Convert angles to radians, etc. 
  RA = rad(RA);
  IN = rad(IN);
  WP = rad(WP);
  MA = rad(MA);
  MM = MM * 2.0 * M_PI;
  M2 = M2 * 2.0 * M_PI;

  // YM = 365.25;       // Mean year, days                       
  // YT = 365.2421970;  // Tropical year, days                   
  WW = 2.0 * M_PI / YT; // Earth's rotation rate, rads/whole day 
  WE = 2.0 * M_PI + WW; // Earth's rotation rate, rads/day       
  W0 = WE / 86400;      // Earth's rotation rate, rads/sec       

  // Observer's velocity, geocentric coordinates 
  VOx = -Oy * W0;
  VOy =  Ox * W0;

  // Convert satellite epoch to Day No. and fraction of a day 
  DE = FNday(YE, 1, 0) + (int)TE;

  TE = TE - (int)TE;
  if (DEBUG) {
    printf("DE: ");
    printf("%1f\n", DE);
    printf("TE: ");
    printf("%1f\n", TE);
  }
  // Average Precession rates 
  GM = 3.986E5;               // Earth's gravitational constant km^3/s^2  
  J2 = 1.08263E-3;            // 2nd Zonal coeff, Earth's gravity Field   
  N0 = MM / 86400.0;                  // Mean motion rads/s               
  A0 = pow(GM / N0 / N0, 1.0 / 3.0);  // Semi major axis km               
  b0 = A0 * sqrt(1.0 - EC * EC);      // Semi minor axis km               
  SI = sin(IN);
  CI = cos(IN);
  PC = RE * A0 / (b0 * b0);
  PC = 1.5 * J2 * PC * PC * MM;       // Precession const, rad/day        
  QD = -PC * CI;                      // Node Precession rate, rad/day    
  WD = PC *(5.0 * CI*CI - 1.0) / 2.0; // Perigee Precession rate, rad/day 
  DC = -2.0 * M2 / MM / 3.0;          // Drag coeff                       

  // Sideral and solar data. Never needs changing. Valid to year 2000+ 


  // GHAA, Year YG, Jan 0.0 
  YG = 2010;
  G0 = 99.5578;
  // MA Sun and rate, deg, deg/day 
  MAS0 = 356.4485;
  MASD = 0.98560028;
  // Sun's inclination 
  INS = rad(23.4380);
  CNS = cos(INS);
  SNS = sin(INS);
  // Sun's equation of center terms 
  EQC1 = 0.03341;
  EQC2 = 0.00035;


  // Bring Sun data to satellite epoch 
  TEG = (DE - FNday(YG, 1, 0)) + TE;  // Elapsed Time: Epoch - YG         
  GHAE = rad(G0) + TEG * WE;          // GHA Aries, epoch                 
  MRSE = rad(G0) + (TEG * WW) + M_PI; // Mean RA Sun at Sat Epoch         
  MASE = rad(MAS0 + MASD * TEG);      // Mean MA Sun                      

  // Antenna unit vector in orbit plane coordinates 
  CO = cos(rad(ALON));
  SO = sin(rad(ALON));
  CL = cos(rad(ALAT));
  SL = sin(rad(ALAT));
  ax = -CL * CO;
  ay = -CL * SO;
  az = -SL;

  if (DEBUG) { printf("End initSat()\n");}
}

/*
* Calculate satellite position at DN, TN
*/
  void Plan13::satvec(void)
{
  if (DEBUG) {printf("Start satvec()\n");}
  T = (DN - DE) + (TN - TE);          // Elapsed T since epoch            
  if (DEBUG) {
    printf("T: ");
    printf("%1f\n", T);
  }
  DT = DC * T / 2.0;                  // Linear drag terms                
  KD = 1.0 + 4.0 * DT;
  KDP = 1.0 - 7.0 * DT;
  M = MA + MM * T * (1.0 - 3.0 * DT); // Mean anomaly at YR,/ TN          
  DR = (int)(M / (2.0 * M_PI));       // Strip out whole no of revs       
  M = M - DR * 2.0 * M_PI;            // M now in range 0 - 2PI           
  RN = RV + DR + 1;                   // Current orbit number             

  // Solve M = EA - EC * sin(EA) for EA given M, by Newton's method       
  EA = M;                             // Initail solution                 
  do {
    C = cos(EA);
    S = sin(EA);
    DNOM = 1.0 - EC * C;
    D = (EA - EC * S - M) / DNOM;     // Change EA to better resolution   
    EA = EA - D;                      // by this amount until converged   
  } while (fabs(D) > 1.0E-5);

  // Distances 
  A = A0 * KD;
  B = b0 * KD;
  RS = A * DNOM;

  // Calculate satellite position and velocity in plane of ellipse 
  Sx = A * (C - EC);
  Vx = -A * S / DNOM * N0;
  Sy = B * S;
  Vy = B * C / DNOM * N0;

  AP = WP + WD * T * KDP;
  CWw = cos(AP);
  SW = sin(AP);
  RAAN = RA + QD * T * KDP;
  CQ = cos(RAAN);
  SQ = sin(RAAN);

  // Plane -> celestial coordinate transformation, [C] = [RAAN]*[IN]*[AP] 
  CXx = CWw * CQ - SW * CI * SQ;
  CXy = -SW * CQ - CWw * CI * SQ;
  CXz = SI * SQ;
  CYx = CWw * SQ + SW * CI * CQ;
  CYy = -SW * SQ + CWw * CI * CQ;
  CYz = -SI * CQ;
  CZx = SW * SI;
  CZy = CWw * SI;
  CZz = CI;

  // Compute satellite's position vector, ANTenna axis unit vector    
  //   and velocity  in celestial coordinates. (Note: Sz = 0, Vz = 0) 
  SATx = Sx * CXx + Sy * CXy;
  ANTx = ax * CXx + ay * CXy + az * CXz;
  VELx = Vx * CXx + Vy * CXy;
  SATy = Sx * CYx + Sy * CYy;
  ANTy = ax * CYx + ay * CYy + az * CYz;
  VELy = Vx * CYx + Vy * CYy;
  SATz = Sx * CZx + Sy * CZy;
  ANTz = ax * CZx + ay * CZy + az * CZz;
  VELz = Vx * CZx + Vy * CZy;

  // Also express SAT, ANT, and VEL in geocentric coordinates 
  GHAA = GHAE + WE * T;       // GHA Aries at elaprsed time T 
  C = cos(-GHAA);
  S = sin(-GHAA);
  Sx = SATx * C - SATy * S;
  Ax = ANTx * C - ANTy * S;
  Vx = VELx * C - VELy * S;
  Sy = SATx * S + SATy * C;
  Ay = ANTx * S + ANTy * C;
  Vy = VELx * S + VELy * C;
  Sz = SATz;
  Az = ANTz;
  Vz = VELz;

  if (DEBUG) {printf("End satvec()\n");}

}

/*
* Compute and manipulate range/velocity/antenna vectors
*/
void Plan13::rangevec(void)
{
  if (DEBUG) {printf("Start rangevec()\n");}

  // Range vector = sat vector - observer vector 
  Rx = Sx - Ox;
  Ry = Sy - Oy;
  Rz = Sz - Oz;

  // Range Magnitute 
  R = sqrt(Rx * Rx + Ry * Ry + Rz * Rz);

  // Normalize range vector 
  Rx = Rx / R;
  Ry = Ry / R;
  Rz = Rz / R;
  U = Rx * Ux + Ry * Uy + Rz * Uz;
  E = Rx * Ex + Ry * Ey;
  N = Rx * Nx + Ry * Ny + Rz * Nz;

  AZ = deg(FNatn(E, N));
  EL = deg(asin(U));

  // Solve antenna vector along unit range vector, -r.a = cos(SQ) 
  // SQ = deg(acos(-(Ax * Rx + Ay * Ry + Az * Rz)));   
  // Calculate sub-satellite Lat/Lon 
  SLON = deg(FNatn(Sy, Sx));   // Lon, + East  
  SLAT = deg(asin(Sz / RS));   // Lat, + North 

  // Resolve Sat-Obs velocity vector along unit range vector. (VOz = 0) 
  RR = (Vx - VOx) * Rx + (Vy - VOy) * Ry + Vz * Rz; // Range rate, km/sec 

  if (DEBUG) {printf("End rangevec()\n");}
}


void Plan13::printdata(void) const {
  printf("> ");
  printf("AZ:");
  printf("%1f", AZ);
  printf(" EL: ");
  printf("%1f", EL);
  printf(" Sat Lat: ");
  printf("%1f", SLAT);
  printf(" Sat Lon.: ");
  printf("%1f;\n", SLON);
}


/*
 * Setter method for indicating the location of the ground station. 
 * This and setElements() must be done before the calculate method 
 * is applied for the first time.
 * Thereafter, however, it doesn't matter unless the groundstation changes position.
 *
 * \param observer_lon_in the longitude of the observer, with east positive and west negative
 * \param observer_lat_in the latitude of the observer with north positive and south negative
 * \param height height of the observer, in meters
 */
void Plan13::setLocation(double observer_lon_in, double observer_lat_in, int height) {
  observer_lon = observer_lon_in; // lon east is positive, west is negative
  observer_lat = observer_lat_in; //45.8958; //52.21; //Cambridge UK
  observer_height = height;       //60m height in meters
}

/*
 * Setter method for UTC time at which the satellite is to be observed. 
 * This is usually the current tim.
 * \param yearIn The current year in four digits, like 2010
 * \param  monthIn The month in two digits, like '10' for October
 * \param mDayIn the day of the month, like '1' for the first day
 * \param hourIn the hour of the day, like '19' for 19
 * \param minIn the minute of the day, like '00' for the first
 * \param secIn the seconds
 */
void Plan13::setTime(int yearIn, int monthIn, int mDayIn, int hourIn, int minIn, float secIn) {
  int aYear = yearIn;
  int aMonth = monthIn;
  int aMday = mDayIn;
  int aHour = hourIn;
  int aMin  = minIn;
  float aSec  = secIn;

  DN = FNday(aYear, aMonth, aMday);
  TN = ((float)aHour + ((float)aMin + ((float)aSec/60.0)) /60.0)/24.0;
  DN = (long)DN;
}

/*
 * Sets the keplerian elements for the following calculations. If the TEST variable
 * is defined 'true', some sample elements will be applied.
 * \param YE_in the YE element
 * \param TE_in the TE element
 * \param IN_in the IN element
 * \param RA_in the RA element
 * \param EC_in the EC element
 * \param WP_in the WP element
 * \param MA_in the MA element
 * \param MM_in the MM element
 * \param M2_in the M2 element
 * \param RV_in the RV element
 * \param ALON_in the ALON element
 */


void   Plan13::setElements(double YE_in, double TE_in, double IN_in, double 
			   RA_in, double EC_in, double WP_in, double MA_in, double MM_in, 
			   double M2_in, double RV_in, double ALON_in ) {
	
  YE = YE_in;
  TE = TE_in;
  IN = IN_in;
  RA = RA_in;
  EC = EC_in;
  WP = WP_in;
  MA = MA_in;
  MM = MM_in;
  M2 = M2_in;
  RV = RV_in;
  ALON = ALON_in;
}

/*
 * A function that joins together the necessary functions for calculating 
 * the satellite position. You must set the keplerian elements, time and observer lat/long
 * before using this.
 */
void  Plan13::calculate(){
  initSat();
  satvec();
  rangevec();
}

double Plan13::getAzimuth() const{
  return AZ;
}
double Plan13::getElevation() const{
  return EL;
}
double Plan13::getLongitude() const{
  return SLON;
}
double Plan13::getLatitude() const{
  return SLAT;
}
