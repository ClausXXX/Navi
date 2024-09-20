/*------------------------------------------------------------------------------
* ionex.c : ionex functions
*
*          Copyright (C) 2011-2013 by T.TAKASU, All rights reserved.
*
* references:
*     [1] S.Schear, W.Gurtner and J.Feltens, IONEX: The IONosphere Map EXchange
*         Format Version 1, February 25, 1998
*     [2] S.Schaer, R.Markus, B.Gerhard and A.S.Timon, Daily Global Ionosphere
*         Maps based on GPS Carrier Phase Data Routinely producted by CODE
*         Analysis Center, Proceeding of the IGS Analysis Center Workshop, 1996
*
* version : $Revision:$ $Date:$
* history : 2011/03/29 1.0 new
*           2013/03/05 1.1 change api readtec()
*                          fix problem in case of lat>85deg or lat<-85deg
*           2014/02/22 1.2 fix problem on compiled as C++
*-----------------------------------------------------------------------------*/
#include "Ionex.h"

#include <stdio.h>
#include <dir.h>
#include <math.h>
//#include <alloc.h>
#include <stdlib.h>
#include <dirent.h>

#define SQR(x)      ((x)*(x))
#define VAR_NOTEC   SQR(30.0)   /* variance of no tec */
#define MIN_EL      0.0         /* min elevation angle (rad) */
#define MIN_HGT     -1000.0     /* min user height (m) */

/* string to number ------------------------------------------------------------
* convert substring in string to number
* args   : char   *s        I   string ("... nnn.nnn ...")
*          int    i,n       I   substring position and width
* return : converted number (0.0:error)
*-----------------------------------------------------------------------------*/
extern double str2num(const char *s, int i, int n)
{
    double value;
    char str[256],*p=str;

    if (i<0||(int)strlen(s)<i||(int)sizeof(str)-1<n) return 0.0;
    for (s+=i;*s&&--n>=0;s++) *p++=*s=='d'||*s=='D'?'E':*s; *p='\0';
    return sscanf(str,"%lf",&value)==1?value:0.0;
}

/* convert calendar day/time to time -------------------------------------------
* convert calendar day/time to gtime_t struct
* args   : double *ep       I   day/time {year,month,day,hour,min,sec}
* return : gtime_t struct
* notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
*-----------------------------------------------------------------------------*/
gtime_t epoch2time(const double *ep)
{
    const int doy[]={1,32,60,91,121,152,182,213,244,274,305,335};
    gtime_t time={0};
    int days,sec,year=(int)ep[0],mon=(int)ep[1],day=(int)ep[2];

    if (year<1970||2099<year||mon<1||12<mon) return time;

    /* leap year if year%4==0 in 1901-2099 */
    days=(year-1970)*365+(year-1969)/4+doy[mon-1]+day-2+(year%4==0&&mon>=3?1:0);
    sec=(int)floor(ep[5]);
    time.time=(time_t)days*86400+(int)ep[3]*3600+(int)ep[4]*60+sec;
    time.sec=ep[5]-sec;
    return time;
}

/* string to time --------------------------------------------------------------
* convert substring in string to gtime_t struct
* args   : char   *s        I   string ("... yyyy mm dd hh mm ss ...")
*          int    i,n       I   substring position and width
*          gtime_t *t       O   gtime_t struct
* return : status (0:ok,0>:error)
*-----------------------------------------------------------------------------*/
extern int str2time(const char *s, int i, int n, gtime_t *t)
{
	double ep[6];
    char str[256],*p=str;

	if (i<0||(int)strlen(s)<i||(int)sizeof(str)-1<i) return -1;
	for (s+=i;*s&&--n>=0;) *p++=*s++; *p='\0';
	if (sscanf(str,"%lf %lf %lf %lf %lf %lf",ep,ep+1,ep+2,ep+3,ep+4,ep+5)<6)
		return -1;
	if (ep[0]<100.0) ep[0]+=ep[0]<80.0?2000.0:1900.0;
	*t=epoch2time(ep);
	return 0;
}

/* time difference -------------------------------------------------------------
* difference between gtime_t structs
* args   : gtime_t t1,t2    I   gtime_t structs
* return : time difference (t1-t2) (s)
*-----------------------------------------------------------------------------*/
extern double timediff(gtime_t t1, gtime_t t2)
{
	return difftime(t1.time,t2.time)+t1.sec-t2.sec;
}

/* expand file path ------------------------------------------------------------
* expand file path with wild-card (*) in file
* args   : char   *path     I   file path to expand (captal insensitive)
*          char   *paths    O   expanded file paths
*          int    nmax      I   max number of expanded file paths
* return : number of expanded file paths
* notes  : the order of expanded files is alphabetical order
*-----------------------------------------------------------------------------*/
extern int expath(const char *path, char *paths[], int nmax)
{
    int i,j,n=0;
	char tmp[1024];
#ifdef WIN32
    WIN32_FIND_DATA file;
	HANDLE h;
    char dir[1024]="",*p;

	//trace(3,"expath  : path=%s nmax=%d\n",path,nmax);

	if ((p=strrchr(path,'\\'))) {
        strncpy(dir,path,p-path+1); dir[p-path+1]='\0';
	}
    if ((h=FindFirstFile((LPCTSTR)path,&file))==INVALID_HANDLE_VALUE) {
		strcpy(paths[0],path);
		return 1;
    }
	sprintf(paths[n++],"%s%s",dir,file.cFileName);
	while (FindNextFile(h,&file)&&n<nmax) {
		if (file.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) continue;
        sprintf(paths[n++],"%s%s",dir,file.cFileName);
    }
	FindClose(h);
#else
    struct dirent *d;
    DIR *dp;
	const char *file=path;
    char dir[1024]="",s1[1024],s2[1024],*p,*q,*r;

   // trace(3,"expath  : path=%s nmax=%d\n",path,nmax);

    if ((p=strrchr(path,'/'))||(p=strrchr(path,'\\'))) {
        file=p+1; strncpy(dir,path,p-path+1); dir[p-path+1]='\0';
    }
	if (!(dp=opendir(*dir?dir:"."))) return 0;
    while ((d=readdir(dp))) {
        if (*(d->d_name)=='.') continue;
		sprintf(s1,"^%s$",d->d_name);
		sprintf(s2,"^%s$",file);
		for (p=s1;*p;p++) *p=(char)tolower((int)*p);
        for (p=s2;*p;p++) *p=(char)tolower((int)*p);

        for (p=s1,q=strtok_r(s2,"*",&r);q;q=strtok_r(NULL,"*",&r)) {
			if ((p=strstr(p,q))) p+=strlen(q); else break;
        }
        if (p&&n<nmax) sprintf(paths[n++],"%s%s",dir,d->d_name);
	}
	closedir(dp);
#endif
	/* sort paths in alphabetical order */
	for (i=0;i<n-1;i++) {
		for (j=i+1;j<n;j++) {
			if (strcmp(paths[i],paths[j])>0) {
				strcpy(tmp,paths[i]);
                strcpy(paths[i],paths[j]);
				strcpy(paths[j],tmp);
            }
		}
    }
   // for (i=0;i<n;i++) trace(3,"expath  : file=%s\n",paths[i]);

	return n;
}

/* ionospheric pierce point position -------------------------------------------
* compute ionospheric pierce point (ipp) position and slant factor
* args   : double *pos      I   receiver position {lat,lon,h} (rad,m)
*          double *azel     I   azimuth/elevation angle {az,el} (rad)
*          double re        I   earth radius (km)
*          double hion      I   altitude of ionosphere (km)
*          double *posp     O   pierce point position {lat,lon,h} (rad,m)
* return : slant factor
* notes  : see ref [2], only valid on the earth surface
*          fixing bug on ref [2] A.4.4.10.1 A-22,23
*-----------------------------------------------------------------------------*/
extern double ionppp(const double *pos, const double *azel, double re,
					 double hion, double *posp)
{
	double cosaz,rp,ap,sinap,tanap;

	rp=re/(re+hion)*cos(azel[1]);
	ap=PI/2.0-azel[1]-asin(rp);
	sinap=sin(ap);
	tanap=tan(ap);
	cosaz=cos(azel[0]);
	posp[0]=asin(sin(pos[0])*cos(ap)+cos(pos[0])*sinap*cosaz);

	if ((pos[0]> 70.0*D2R&& tanap*cosaz>tan(PI/2.0-pos[0]))||
		(pos[0]<-70.0*D2R&&-tanap*cosaz>tan(PI/2.0+pos[0]))) {
		posp[1]=pos[1]+PI-asin(sinap*sin(azel[0])/cos(posp[0]));
	}
	else {
		posp[1]=pos[1]+asin(sinap*sin(azel[0])/cos(posp[0]));
	}
	return 1.0/sqrt(1.0-rp*rp);
}


/* get index -----------------------------------------------------------------*/
static int getindex(double value, const double *range)
{
    if (range[2]==0.0) return 0;
    if (range[1]>0.0&&(value<range[0]||range[1]<value)) return -1;
    if (range[1]<0.0&&(value<range[1]||range[0]<value)) return -1;
    return (int)floor((value-range[0])/range[2]+0.5);
}
/* get number of items -------------------------------------------------------*/
static int nitem(const double *range)
{
    return getindex(range[1],range)+1;
}
/* data index (i:lat,j:lon,k:hgt) --------------------------------------------*/
static int dataindex(int i, int j, int k, const int *ndata)
{
    if (i<0||ndata[0]<=i||j<0||ndata[1]<=j||k<0||ndata[2]<=k) return -1;
    return i+ndata[0]*(j+ndata[1]*k);
}
/* add tec data to navigation data -------------------------------------------*/
static tec_t *addtec(const double *lats, const double *lons, const double *hgts,
					 double rb, nav_t *nav)
{
	tec_t *p,*nav_tec;
	gtime_t time0={0};
	int i,n,ndata[3];

	//trace(3,"addtec  :\n");

	ndata[0]=nitem(lats);
	ndata[1]=nitem(lons);
	ndata[2]=nitem(hgts);
	if (ndata[0]<=1||ndata[1]<=1||ndata[2]<=0) return NULL;

	if (nav->nt>=nav->ntmax) {
		nav->ntmax+=256;
        if (!(nav_tec=(tec_t *)realloc(nav->tec,sizeof(tec_t)*nav->ntmax))) {
            //trace(1,"readionex malloc error ntmax=%d\n",nav->ntmax);
            free(nav->tec); nav->tec=NULL; nav->nt=nav->ntmax=0;
            return NULL;
		}
        nav->tec=nav_tec;
	}
	p=nav->tec+nav->nt;
    p->time=time0;
	p->rb=rb;
	for (i=0;i<3;i++) {
		p->ndata[i]=ndata[i];
        p->lats[i]=lats[i];
		p->lons[i]=lons[i];
		p->hgts[i]=hgts[i];
	}
	n=ndata[0]*ndata[1]*ndata[2];

	if (!(p->data=(double *)malloc(sizeof(double)*n))||
		!(p->rms =(float  *)malloc(sizeof(float )*n))) {
		return NULL;
	}
    for (i=0;i<n;i++) {
        p->data[i]=0.0;
        p->rms [i]=0.0f;
    }
    nav->nt++;
    return p;
}

/* read ionex header ---------------------------------------------------------*/
static double readionexh(FILE *fp, double *lats, double *lons, double *hgts,
						 double *rb, double *nexp, double *dcb, double *rms)
{
	double ver=0.0;
	char buff[1024],*label;

   // trace(3,"readionexh:\n");

	while (fgets(buff,sizeof(buff),fp)) {

		if (strlen(buff)<60) continue;
		label=buff+60;

		if (strstr(label,"IONEX VERSION / TYPE")==label) {
			if (buff[20]=='I') ver=str2num(buff,0,8);
		}
		else if (strstr(label,"BASE RADIUS")==label) {
			*rb=str2num(buff,0,8);
		}
		else if (strstr(label,"HGT1 / HGT2 / DHGT")==label) {
			hgts[0]=str2num(buff, 2,6);
			hgts[1]=str2num(buff, 8,6);
            hgts[2]=str2num(buff,14,6);
        }
        else if (strstr(label,"LAT1 / LAT2 / DLAT")==label) {
            lats[0]=str2num(buff, 2,6);
            lats[1]=str2num(buff, 8,6);
            lats[2]=str2num(buff,14,6);
        }
        else if (strstr(label,"LON1 / LON2 / DLON")==label) {
            lons[0]=str2num(buff, 2,6);
            lons[1]=str2num(buff, 8,6);
            lons[2]=str2num(buff,14,6);
        }
        else if (strstr(label,"EXPONENT")==label) {
            *nexp=str2num(buff,0,6);
        }
//		else if (strstr(label,"START OF AUX DATA")==label&&
//				 strstr(buff,"DIFFERENTIAL CODE BIASES")) {
//			readionexdcb(fp,dcb,rms);
//        }
        else if (strstr(label,"END OF HEADER")==label) {
            return ver;
        }
    }
    return 0.0;
}
/* read ionex body -----------------------------------------------------------*/
static int readionexb(FILE *fp, const double *lats, const double *lons,
                      const double *hgts, double rb, double nexp, nav_t *nav)
{
    tec_t *p=NULL;
    gtime_t time={0};
    double lat,lon[3],hgt,x;
    int i,j,k,n,m,index,type=0;
    char buff[1024],*label=buff+60;

  //  trace(3,"readionexb:\n");

    while (fgets(buff,sizeof(buff),fp)) {

        if (strlen(buff)<60) continue;

        if (strstr(label,"START OF TEC MAP")==label) {
            if ((p=addtec(lats,lons,hgts,rb,nav))) type=1;
        }
        else if (strstr(label,"END OF TEC MAP")==label) {
            type=0;
            p=NULL;
        }
        else if (strstr(label,"START OF RMS MAP")==label) {
            type=2;
            p=NULL;
        }
        else if (strstr(label,"END OF RMS MAP")==label) {
            type=0;
            p=NULL;
        }
        else if (strstr(label,"EPOCH OF CURRENT MAP")==label) {
            if (str2time(buff,0,36,&time)) {
               // trace(2,"ionex epoch invalid: %-36.36s\n",buff);
                continue;
            }
            if (type==2) {
                for (i=nav->nt-1;i>=0;i--) {
                    if (fabs(timediff(time,nav->tec[i].time))>=1.0) continue;
                    p=nav->tec+i;
                    break;
                }
            }
            else if (p) p->time=time;
        }
        else if (strstr(label,"LAT/LON1/LON2/DLON/H")==label&&p) {
            lat   =str2num(buff, 2,6);
            lon[0]=str2num(buff, 8,6);
            lon[1]=str2num(buff,14,6);
            lon[2]=str2num(buff,20,6);
            hgt   =str2num(buff,26,6);

            i=getindex(lat,p->lats);
            k=getindex(hgt,p->hgts);
            n=nitem(lon);

            for (m=0;m<n;m++) {
                if (m%16==0&&!fgets(buff,sizeof(buff),fp)) break;

                j=getindex(lon[0]+lon[2]*m,p->lons);
                if ((index=dataindex(i,j,k,p->ndata))<0) continue;

                if ((x=str2num(buff,m%16*5,5))==9999.0) continue;

                if (type==1) p->data[index]=x*pow(10.0,nexp);
                else p->rms[index]=(float)(x*pow(10.0,nexp));
            }
        }
    }
    return 1;
}
/* combine tec grid data -----------------------------------------------------*/
static void combtec(nav_t *nav)
{
    tec_t tmp;
    int i,j,n=0;

   // trace(3,"combtec : nav->nt=%d\n",nav->nt);

    for (i=0;i<nav->nt-1;i++) {
        for (j=i+1;j<nav->nt;j++) {
            if (timediff(nav->tec[j].time,nav->tec[i].time)<0.0) {
                tmp=nav->tec[i];
                nav->tec[i]=nav->tec[j];
                nav->tec[j]=tmp;
            }
        }
    }
    for (i=0;i<nav->nt;i++) {
        if (i>0&&timediff(nav->tec[i].time,nav->tec[n-1].time)==0.0) {
            free(nav->tec[n-1].data);
            free(nav->tec[n-1].rms );
            nav->tec[n-1]=nav->tec[i];
            continue;
        }
        nav->tec[n++]=nav->tec[i];
    }
    nav->nt=n;

   // trace(4,"combtec : nav->nt=%d\n",nav->nt);
}
/* read ionex tec grid file ----------------------------------------------------
* read ionex ionospheric tec grid file
* args   : char   *file       I   ionex tec grid file
*                                 (wind-card * is expanded)
*          nav_t  *nav        IO  navigation data
*                                 nav->nt, nav->ntmax and nav->tec are modified
*          int    opt         I   read option (1: no clear of tec data,0:clear)
* return : none
* notes  : see ref [1]
*-----------------------------------------------------------------------------*/
extern void readtec(const char *file, nav_t *nav, int opt)
{
    FILE *fp;
    double lats[3]={0},lons[3]={0},hgts[3]={0},rb=0.0,nexp=-1.0;
    double dcb[MAXSAT]={0},rms[MAXSAT]={0};
    int i,n;
    char *efiles[MAXEXFILE];

  //  trace(3,"readtec : file=%s\n",file);

    /* clear of tec grid data option */
    if (!opt) {
        free(nav->tec); nav->tec=NULL; nav->nt=nav->ntmax=0;
    }
    for (i=0;i<MAXEXFILE;i++) {
        if (!(efiles[i]=(char *)malloc(1024))) {
            for (i--;i>=0;i--) free(efiles[i]);
            return;
        }
    }
    /* expand wild card in file path */
    n=expath(file,efiles,MAXEXFILE);

    for (i=0;i<n;i++) {
        if (!(fp=fopen(efiles[i],"r"))) {
           // trace(2,"ionex file open error %s\n",efiles[i]);
            continue;
        }
        /* read ionex header */
        if (readionexh(fp,lats,lons,hgts,&rb,&nexp,dcb,rms)<=0.0) {
           // trace(2,"ionex file format error %s\n",efiles[i]);
            continue;
        }
        /* read ionex body */
        readionexb(fp,lats,lons,hgts,rb,nexp,nav);

        fclose(fp);
    }
    for (i=0;i<MAXEXFILE;i++) free(efiles[i]);

    /* combine tec grid data */
    if (nav->nt>0) combtec(nav);

	/* P1-P2 dcb */
    for (i=0;i<MAXSAT;i++) {
        //nav->cbias[i][0]=CLIGHT*dcb[i]*1E-9; /* ns->m */
    }
}
/* interpolate tec grid data -------------------------------------------------*/
static int interptec(const tec_t *tec, int k, const double *posp, double *value,
                     double *rms)
{
    double dlat,dlon,a,b,d[4]={0},r[4]={0};
    int i,j,n,index;

	//trace(3,"interptec: k=%d posp=%.2f %.2f\n",k,posp[0]*R2D,posp[1]*R2D);
  //  *value=*rms=0.0;

    if (tec->lats[2]==0.0||tec->lons[2]==0.0) return 0;

    dlat=posp[0]*R2D-tec->lats[0];
    dlon=posp[1]*R2D-tec->lons[0];
    if (tec->lons[2]>0.0) dlon-=floor( dlon/360)*360.0; /*  0<=dlon<360 */
    else                  dlon+=floor(-dlon/360)*360.0; /* -360<dlon<=0 */

    a=dlat/tec->lats[2];
    b=dlon/tec->lons[2];
    i=(int)floor(a); a-=i;
    j=(int)floor(b); b-=j;

    /* get gridded tec data */
    for (n=0;n<4;n++) {
        if ((index=dataindex(i+(n%2),j+(n<2?0:1),k,tec->ndata))<0) continue;
        d[n]=tec->data[index];
        r[n]=tec->rms [index];
    }
    if (d[0]>0.0&&d[1]>0.0&&d[2]>0.0&&d[3]>0.0) {

        /* bilinear interpolation (inside of grid) */
        *value=(1.0-a)*(1.0-b)*d[0]+a*(1.0-b)*d[1]+(1.0-a)*b*d[2]+a*b*d[3];
        *rms  =(1.0-a)*(1.0-b)*r[0]+a*(1.0-b)*r[1]+(1.0-a)*b*r[2]+a*b*r[3];
    }
    /* nearest-neighbour extrapolation (outside of grid) */
    else if (a<=0.5&&b<=0.5&&d[0]>0.0) {*value=d[0]; *rms=r[0];}
    else if (a> 0.5&&b<=0.5&&d[1]>0.0) {*value=d[1]; *rms=r[1];}
    else if (a<=0.5&&b> 0.5&&d[2]>0.0) {*value=d[2]; *rms=r[2];}
    else if (a> 0.5&&b> 0.5&&d[3]>0.0) {*value=d[3]; *rms=r[3];}
    else {
        i=0;
        for (n=0;n<4;n++) if (d[n]>0.0) {i++; *value+=d[n]; *rms+=r[n];}
        if(i==0) return 0;
        *value/=i; *rms/=i;
    }
    return 1;
}
/* ionosphere delay by tec grid data -----------------------------------------*/
static int iondelay(gtime_t time, const tec_t *tec, const double *pos,
                    const double *azel, int opt, double *delay, double *var)
{
    const double fact=40.30E16/FREQ1/FREQ1; /* tecu->L1 iono (m) */
    double fs,posp[3]={0},vtec,rms,hion,rp;
    int i;

   // trace(3,"iondelay: time=%s pos=%.1f %.1f azel=%.1f %.1f\n",time_str(time,0),
        //  pos[0]*R2D,pos[1]*R2D,azel[0]*R2D,azel[1]*R2D);

    *delay=*var=0.0;

    for (i=0;i<tec->ndata[2];i++) { /* for a layer */

        hion=tec->hgts[0]+tec->hgts[2]*i;

        /* ionospheric pierce point position */
        fs=ionppp(pos,azel,tec->rb,hion,posp);

        if (opt&2) {
            /* modified single layer mapping function (M-SLM) ref [2] */
            rp=tec->rb/(tec->rb+hion)*sin(0.9782*(PI/2.0-azel[1]));
            fs=1.0/sqrt(1.0-rp*rp);
        }
        if (opt&1) {
            /* earth rotation correction (sun-fixed coordinate) */
            posp[1]+=2.0*PI*timediff(time,tec->time)/86400.0;
        }
        /* interpolate tec grid data */
        if (!interptec(tec,i,posp,&vtec,&rms)) return 0;

        *delay+=fact*fs*vtec;
        *var+=fact*fact*fs*fs*rms*rms;
    }
//    trace(4,"iondelay: delay=%7.2f std=%6.2f\n",*delay,sqrt(*var));

    return 1;
}

/* ionosphere model by tec grid data -------------------------------------------
* compute ionospheric delay by tec grid data
* args   : gtime_t time     I   time (gpst)
*          nav_t  *nav      I   navigation data
*          double *pos      I   receiver position {lat,lon,h} (rad,m)
*          double *azel     I   azimuth/elevation angle {az,el} (rad)
*          int    opt       I   model option
*                                bit0: 0:earth-fixed,1:sun-fixed
*                                bit1: 0:single-layer,1:modified single-layer
*          double *delay    O   ionospheric delay (L1) (m)
*          double *var      O   ionospheric dealy (L1) variance (m^2)
* return : status (1:ok,0:error)
* notes  : before calling the function, read tec grid data by calling readtec()
*          return ok with delay=0 and var=VAR_NOTEC if el<MIN_EL or h<MIN_HGT
*-----------------------------------------------------------------------------*/
extern int iontec(gtime_t time, const nav_t *nav, const double *pos,
                  const double *azel, int opt, double *delay, double *var)
{
    double dels[2],vars[2],a,tt;
    int i,stat[2];

   // trace(3,"iontec  : time=%s pos=%.1f %.1f azel=%.1f %.1f\n",time_str(time,0),
        //  pos[0]*R2D,pos[1]*R2D,azel[0]*R2D,azel[1]*R2D);

    if (azel[1]<MIN_EL||pos[2]<MIN_HGT) {
        *delay=0.0;
        *var=VAR_NOTEC;
        return 1;
    }
    for (i=0;i<nav->nt;i++) {
        if (timediff(nav->tec[i].time,time)>0.0) break;
    }
    if (i==0||i>=nav->nt) {
		//trace(2,"%s: tec grid out of period\n",time_str(time,0));
        return 0;
    }
    if ((tt=timediff(nav->tec[i].time,nav->tec[i-1].time))==0.0) {
      //  trace(2,"tec grid time interval error\n");
        return 0;
    }
    /* ionospheric delay by tec grid data */
    stat[0]=iondelay(time,nav->tec+i-1,pos,azel,opt,dels  ,vars  );
    stat[1]=iondelay(time,nav->tec+i  ,pos,azel,opt,dels+1,vars+1);

    if (!stat[0]&&!stat[1]) {
	   // trace(2,"%s: tec grid out of area pos=%6.2f %7.2f azel=%6.1f %5.1f\n",
            //  time_str(time,0),pos[0]*R2D,pos[1]*R2D,azel[0]*R2D,azel[1]*R2D);
        return 0;
    }
    if (stat[0]&&stat[1]) { /* linear interpolation by time */
        a=timediff(time,nav->tec[i-1].time)/tt;
        *delay=dels[0]*(1.0-a)+dels[1]*a;
        *var  =vars[0]*(1.0-a)+vars[1]*a;
    }
    else if (stat[0]) { /* nearest-neighbour extrapolation by time */
        *delay=dels[0];
        *var  =vars[0];
    }
    else {
        *delay=dels[1];
        *var  =vars[1];
    }
  //  trace(3,"iontec  : delay=%5.2f std=%5.2f\n",*delay,sqrt(*var));
    return 1;
}
