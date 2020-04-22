/***** Autogenerated from runpath3d.in; changes will be overwritten *****/

#line 1 "runtimebase.in"
/*****
 * runtimebase.in
 * Andy Hammerlindl  2009/07/28
 *
 * Common declarations needed for all code-generating .in files.
 *
 *****/


#line 1 "runpath3d.in"
/*****
 * runpath3.in
 *
 * Runtime functions for path3 operations.
 *
 *****/

#line 1 "runtimebase.in"
#include "stack.h"
#include "types.h"
#include "builtin.h"
#include "entry.h"
#include "errormsg.h"
#include "array.h"
#include "triple.h"
#include "callable.h"
#include "opsymbols.h"

using vm::stack;
using vm::error;
using vm::array;
using vm::read;
using vm::callable;
using types::formal;
using types::function;
using camp::triple;

#define PRIMITIVE(name,Name,asyName) using types::prim##Name;
#include <primitives.h>
#undef PRIMITIVE

typedef double real;

void unused(void *);

namespace run {
array *copyArray(array *a);
array *copyArray2(array *a);
array *copyArray3(array *a);

double *copyTripleArray2Components(array *a, size_t &N,
                                   GCPlacement placement=NoGC);
triple *copyTripleArray2C(array *a, size_t &N,
                          GCPlacement placement=NoGC);
}

function *realRealFunction();

#define CURRENTPEN processData().currentpen

#line 17 "runpath3d.in"
#include "path3.h"
#include "array.h"
#include "drawsurface.h"
#include "predicates.h"

using namespace camp;
using namespace vm;

typedef array boolarray;
typedef array realarray;
typedef array realarray2;
typedef array triplearray;
typedef array triplearray2;

using types::booleanArray;
using types::realArray;
using types::realArray2;
using types::tripleArray;
using types::tripleArray2;

// Autogenerated routines:



#ifndef NOSYM
#include "runpath3d.symbols.h"

#endif
namespace run {
#line 40 "runpath3d.in"
// path3 path3(triplearray *pre, triplearray *point, triplearray *post,            boolarray *straight, bool cyclic);
void gen_runpath3d0(stack *Stack)
{
  bool cyclic=vm::pop<bool>(Stack);
  boolarray * straight=vm::pop<boolarray *>(Stack);
  triplearray * post=vm::pop<triplearray *>(Stack);
  triplearray * point=vm::pop<triplearray *>(Stack);
  triplearray * pre=vm::pop<triplearray *>(Stack);
#line 42 "runpath3d.in"
  size_t n=checkArrays(pre,point);
  checkEqual(n,checkArray(post));
  checkEqual(n,checkArray(straight));
  mem::vector<solvedKnot3> nodes(n);
  for(size_t i=0; i < n; ++i) {
    nodes[i].pre=read<triple>(pre,i);
    nodes[i].point=read<triple>(point,i);
    nodes[i].post=read<triple>(post,i);
    nodes[i].straight=read<bool>(straight,i);
  }

  {Stack->push<path3>(path3(nodes,(Int) n,cyclic)); return;}
}

#line 57 "runpath3d.in"
void nullPath3(stack *Stack)
{
#line 58 "runpath3d.in"
  {Stack->push<path3>(nullpath3); return;}
}

#line 62 "runpath3d.in"
// bool ==(path3 a, path3 b);
void gen_runpath3d2(stack *Stack)
{
  path3 b=vm::pop<path3>(Stack);
  path3 a=vm::pop<path3>(Stack);
#line 63 "runpath3d.in"
  {Stack->push<bool>(a == b); return;}
}

#line 67 "runpath3d.in"
// bool !=(path3 a, path3 b);
void gen_runpath3d3(stack *Stack)
{
  path3 b=vm::pop<path3>(Stack);
  path3 a=vm::pop<path3>(Stack);
#line 68 "runpath3d.in"
  {Stack->push<bool>(!(a == b)); return;}
}

#line 72 "runpath3d.in"
// triple point(path3 p, Int t);
void gen_runpath3d4(stack *Stack)
{
  Int t=vm::pop<Int>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 73 "runpath3d.in"
  {Stack->push<triple>(p.point((Int) t)); return;}
}

#line 77 "runpath3d.in"
// triple point(path3 p, real t);
void gen_runpath3d5(stack *Stack)
{
  real t=vm::pop<real>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 78 "runpath3d.in"
  {Stack->push<triple>(p.point(t)); return;}
}

#line 82 "runpath3d.in"
// triple precontrol(path3 p, Int t);
void gen_runpath3d6(stack *Stack)
{
  Int t=vm::pop<Int>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 83 "runpath3d.in"
  {Stack->push<triple>(p.precontrol((Int) t)); return;}
}

#line 87 "runpath3d.in"
// triple precontrol(path3 p, real t);
void gen_runpath3d7(stack *Stack)
{
  real t=vm::pop<real>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 88 "runpath3d.in"
  {Stack->push<triple>(p.precontrol(t)); return;}
}

#line 92 "runpath3d.in"
// triple postcontrol(path3 p, Int t);
void gen_runpath3d8(stack *Stack)
{
  Int t=vm::pop<Int>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 93 "runpath3d.in"
  {Stack->push<triple>(p.postcontrol((Int) t)); return;}
}

#line 97 "runpath3d.in"
// triple postcontrol(path3 p, real t);
void gen_runpath3d9(stack *Stack)
{
  real t=vm::pop<real>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 98 "runpath3d.in"
  {Stack->push<triple>(p.postcontrol(t)); return;}
}

#line 102 "runpath3d.in"
// triple dir(path3 p, Int t, Int sign=0, bool normalize=true);
void gen_runpath3d10(stack *Stack)
{
  bool normalize=vm::pop<bool>(Stack,true);
  Int sign=vm::pop<Int>(Stack,0);
  Int t=vm::pop<Int>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 103 "runpath3d.in"
  {Stack->push<triple>(p.dir(t,sign,normalize)); return;}
}

#line 107 "runpath3d.in"
// triple dir(path3 p, real t, bool normalize=true);
void gen_runpath3d11(stack *Stack)
{
  bool normalize=vm::pop<bool>(Stack,true);
  real t=vm::pop<real>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 108 "runpath3d.in"
  {Stack->push<triple>(p.dir(t,normalize)); return;}
}

#line 112 "runpath3d.in"
// triple accel(path3 p, Int t, Int sign=0);
void gen_runpath3d12(stack *Stack)
{
  Int sign=vm::pop<Int>(Stack,0);
  Int t=vm::pop<Int>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 113 "runpath3d.in"
  {Stack->push<triple>(p.accel(t,sign)); return;}
}

#line 117 "runpath3d.in"
// triple accel(path3 p, real t);
void gen_runpath3d13(stack *Stack)
{
  real t=vm::pop<real>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 118 "runpath3d.in"
  {Stack->push<triple>(p.accel(t)); return;}
}

#line 122 "runpath3d.in"
// real radius(path3 p, real t);
void gen_runpath3d14(stack *Stack)
{
  real t=vm::pop<real>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 123 "runpath3d.in"
  triple v=p.dir(t,false);
  triple a=p.accel(t);
  real d=dot(a,v);
  real v2=v.abs2();
  real a2=a.abs2();
  real denom=v2*a2-d*d;
  real r=v2*sqrt(v2);
  {Stack->push<real>(denom > 0 ? r/sqrt(denom) : 0.0); return;}
}

#line 134 "runpath3d.in"
// real radius(triple z0, triple c0, triple c1, triple z1, real t);
void gen_runpath3d15(stack *Stack)
{
  real t=vm::pop<real>(Stack);
  triple z1=vm::pop<triple>(Stack);
  triple c1=vm::pop<triple>(Stack);
  triple c0=vm::pop<triple>(Stack);
  triple z0=vm::pop<triple>(Stack);
#line 135 "runpath3d.in"
  triple v=(3.0*(z1-z0)+9.0*(c0-c1))*t*t+(6.0*(z0+c1)-12.0*c0)*t+3.0*(c0-z0);
  triple a=6.0*(z1-z0+3.0*(c0-c1))*t+6.0*(z0+c1)-12.0*c0;
  real d=dot(a,v);
  real v2=v.abs2();
  real a2=a.abs2();
  real denom=v2*a2-d*d;
  real r=v2*sqrt(v2);
  {Stack->push<real>(denom > 0 ? r/sqrt(denom) : 0.0); return;}
}

#line 146 "runpath3d.in"
// path3 reverse(path3 p);
void gen_runpath3d16(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 147 "runpath3d.in"
  {Stack->push<path3>(p.reverse()); return;}
}

#line 151 "runpath3d.in"
// path3 subpath(path3 p, Int a, Int b);
void gen_runpath3d17(stack *Stack)
{
  Int b=vm::pop<Int>(Stack);
  Int a=vm::pop<Int>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 152 "runpath3d.in"
  {Stack->push<path3>(p.subpath((Int) a, (Int) b)); return;}
}

#line 156 "runpath3d.in"
// path3 subpath(path3 p, real a, real b);
void gen_runpath3d18(stack *Stack)
{
  real b=vm::pop<real>(Stack);
  real a=vm::pop<real>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 157 "runpath3d.in"
  {Stack->push<path3>(p.subpath(a,b)); return;}
}

#line 161 "runpath3d.in"
// Int length(path3 p);
void gen_runpath3d19(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 162 "runpath3d.in"
  {Stack->push<Int>(p.length()); return;}
}

#line 166 "runpath3d.in"
// bool cyclic(path3 p);
void gen_runpath3d20(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 167 "runpath3d.in"
  {Stack->push<bool>(p.cyclic()); return;}
}

#line 171 "runpath3d.in"
// bool straight(path3 p, Int t);
void gen_runpath3d21(stack *Stack)
{
  Int t=vm::pop<Int>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 172 "runpath3d.in"
  {Stack->push<bool>(p.straight(t)); return;}
}

#line 176 "runpath3d.in"
// path3 unstraighten(path3 p);
void gen_runpath3d22(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 177 "runpath3d.in"
  {Stack->push<path3>(p.unstraighten()); return;}
}

// return the maximum distance squared of points c0 and c1 from 
// the respective internal control points of z0--z1.
#line 183 "runpath3d.in"
// real straightness(triple z0, triple c0, triple c1, triple z1);
void gen_runpath3d23(stack *Stack)
{
  triple z1=vm::pop<triple>(Stack);
  triple c1=vm::pop<triple>(Stack);
  triple c0=vm::pop<triple>(Stack);
  triple z0=vm::pop<triple>(Stack);
#line 184 "runpath3d.in"
  {Stack->push<real>(Straightness(z0,c0,c1,z1)); return;}
}

// return the straightness of segment i of path3 g.
#line 189 "runpath3d.in"
// real straightness(path3 p, Int t);
void gen_runpath3d24(stack *Stack)
{
  Int t=vm::pop<Int>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 190 "runpath3d.in"
  if(p.straight(t)) {Stack->push<real>(0); return;}
  {Stack->push<real>(Straightness(p.point(t),p.postcontrol(t),p.precontrol(t+1),
                      p.point(t+1))); return;}
}

#line 196 "runpath3d.in"
// bool piecewisestraight(path3 p);
void gen_runpath3d25(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 197 "runpath3d.in"
  {Stack->push<bool>(p.piecewisestraight()); return;}
}

#line 201 "runpath3d.in"
// real arclength(path3 p);
void gen_runpath3d26(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 202 "runpath3d.in"
  {Stack->push<real>(p.arclength()); return;}
}

#line 206 "runpath3d.in"
// real arclength(triple z0, triple c0, triple c1, triple z1);
void gen_runpath3d27(stack *Stack)
{
  triple z1=vm::pop<triple>(Stack);
  triple c1=vm::pop<triple>(Stack);
  triple c0=vm::pop<triple>(Stack);
  triple z0=vm::pop<triple>(Stack);
#line 207 "runpath3d.in"
  {Stack->push<real>(arcLength(z0,c0,c1,z1)); return;}
}

#line 211 "runpath3d.in"
// real arctime(path3 p, real dval);
void gen_runpath3d28(stack *Stack)
{
  real dval=vm::pop<real>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 212 "runpath3d.in"
  {Stack->push<real>(p.arctime(dval)); return;}
}

#line 216 "runpath3d.in"
// realarray* intersect(path3 p, path3 q, real fuzz=-1);
void gen_runpath3d29(stack *Stack)
{
  real fuzz=vm::pop<real>(Stack,-1);
  path3 q=vm::pop<path3>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 217 "runpath3d.in"
  bool exact=fuzz <= 0.0;
  if(fuzz < 0)
    fuzz=BigFuzz*::max(::max(length(p.max()),length(p.min())),
                       ::max(length(q.max()),length(q.min())));
  
  std::vector<real> S,T;
  real s,t;
  if(intersections(s,t,S,T,p,q,fuzz,true,exact)) {
    array *V=new array(2);
    (*V)[0]=s;
    (*V)[1]=t;
    {Stack->push<realarray*>(V); return;}
  } else
    {Stack->push<realarray*>(new array(0)); return;}
}

#line 234 "runpath3d.in"
// realarray2* intersections(path3 p, path3 q, real fuzz=-1);
void gen_runpath3d30(stack *Stack)
{
  real fuzz=vm::pop<real>(Stack,-1);
  path3 q=vm::pop<path3>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 235 "runpath3d.in"
  bool exact=fuzz <= 0.0;
  if(fuzz < 0)
    fuzz=BigFuzz*::max(::max(length(p.max()),length(p.min())),
                       ::max(length(q.max()),length(q.min())));
  bool single=!exact;
  
  real s,t;
  std::vector<real> S,T;
  bool found=intersections(s,t,S,T,p,q,fuzz,single,exact);
  if(!found) {Stack->push<realarray2*>(new array(0)); return;}
  array *V;
  if(single) {
    V=new array(1);
    array *Vi=new array(2);
    (*V)[0]=Vi;
    (*Vi)[0]=s;
    (*Vi)[1]=t;
  } else {
    size_t n=S.size();
    V=new array(n);
    for(size_t i=0; i < n; ++i) {
      array *Vi=new array(2);
      (*V)[i]=Vi;
      (*Vi)[0]=S[i];
      (*Vi)[1]=T[i];
    }
  }
  stable_sort(V->begin(),V->end(),run::compare2<real>());
  {Stack->push<realarray2*>(V); return;}
}

#line 267 "runpath3d.in"
// realarray* intersect(path3 p, triplearray2 *P, real fuzz=-1);
void gen_runpath3d31(stack *Stack)
{
  real fuzz=vm::pop<real>(Stack,-1);
  triplearray2 * P=vm::pop<triplearray2 *>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 268 "runpath3d.in"
  triple *A;
  copyArray2C(A,P,true,4);
  if(fuzz <= 0) fuzz=BigFuzz*::max(::max(length(p.max()),length(p.min())),
                                   norm(A,16));
  std::vector<real> T,U,V;
  bool found=intersections(T,U,V,p,A,fuzz,true);
  delete[] A;
  if(found) {
    array *W=new array(3);
    (*W)[0]=T[0];
    (*W)[1]=U[0];
    (*W)[2]=V[0];
    {Stack->push<realarray*>(W); return;}
  } else
    {Stack->push<realarray*>(new array(0)); return;}
}

#line 286 "runpath3d.in"
// realarray2* intersections(path3 p, triplearray2 *P, real fuzz=-1);
void gen_runpath3d32(stack *Stack)
{
  real fuzz=vm::pop<real>(Stack,-1);
  triplearray2 * P=vm::pop<triplearray2 *>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 287 "runpath3d.in"
  triple *A;
  copyArray2C(A,P,true,4);
  if(fuzz <= 0) fuzz=BigFuzz*::max(::max(length(p.max()),length(p.min())),
                                   norm(A,16));
  std::vector<real> T,U,V;
  intersections(T,U,V,p,A,fuzz,false);
  delete[] A;
  size_t n=T.size();
  array *W=new array(n);
  for(size_t i=0; i < n; ++i) {
    array *Wi=new array(3);
    (*W)[i]=Wi;
    (*Wi)[0]=T[i];
    (*Wi)[1]=U[i];
    (*Wi)[2]=V[i];
  }
  {Stack->push<realarray2*>(W); return;} // Sorting will done in asy.
}

#line 307 "runpath3d.in"
// Int size(path3 p);
void gen_runpath3d33(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 308 "runpath3d.in"
  {Stack->push<Int>(p.size()); return;}
}

#line 312 "runpath3d.in"
// path3 &(path3 p, path3 q);
void gen_runpath3d34(stack *Stack)
{
  path3 q=vm::pop<path3>(Stack);
  path3 p=vm::pop<path3>(Stack);
#line 313 "runpath3d.in"
  {Stack->push<path3>(camp::concat(p,q)); return;}
}

#line 317 "runpath3d.in"
// triple min(path3 p);
void gen_runpath3d35(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 318 "runpath3d.in"
  {Stack->push<triple>(p.min()); return;}
}

#line 322 "runpath3d.in"
// triple max(path3 p);
void gen_runpath3d36(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 323 "runpath3d.in"
  {Stack->push<triple>(p.max()); return;}
}

#line 327 "runpath3d.in"
// realarray* mintimes(path3 p);
void gen_runpath3d37(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 328 "runpath3d.in"
  array *V=new array(3);
  triple v=p.mintimes();
  (*V)[0]=v.getx();
  (*V)[1]=v.gety();
  (*V)[2]=v.getz();
  {Stack->push<realarray*>(V); return;}
}

#line 337 "runpath3d.in"
// realarray* maxtimes(path3 p);
void gen_runpath3d38(stack *Stack)
{
  path3 p=vm::pop<path3>(Stack);
#line 338 "runpath3d.in"
  array *V=new array(3);
  triple v=p.maxtimes();
  (*V)[0]=v.getx();
  (*V)[1]=v.gety();
  (*V)[2]=v.getz();
  {Stack->push<realarray*>(V); return;}
}

#line 347 "runpath3d.in"
// path3 *(realarray2 *t, path3 g);
void gen_runpath3d39(stack *Stack)
{
  path3 g=vm::pop<path3>(Stack);
  realarray2 * t=vm::pop<realarray2 *>(Stack);
#line 348 "runpath3d.in"
  {Stack->push<path3>(transformed(*t,g)); return;}
}

#line 352 "runpath3d.in"
// pair minratio(path3 g);
void gen_runpath3d40(stack *Stack)
{
  path3 g=vm::pop<path3>(Stack);
#line 353 "runpath3d.in"
  {Stack->push<pair>(g.ratio(::min)); return;}
}

#line 357 "runpath3d.in"
// pair maxratio(path3 g);
void gen_runpath3d41(stack *Stack)
{
  path3 g=vm::pop<path3>(Stack);
#line 358 "runpath3d.in"
  {Stack->push<pair>(g.ratio(::max)); return;}
}

// Return a negative (positive) value if a--b--c--cycle is oriented
// counterclockwise (clockwise) when viewed from d or zero if all four
// points are coplanar. 
// The value returned is the determinant
// |a.x a.y a.z 1|
// |b.x b.y b.z 1|
// |c.x c.y c.z 1|
// |d.x d.y d.z 1|
#line 370 "runpath3d.in"
// real orient(triple a, triple b, triple c, triple d);
void gen_runpath3d42(stack *Stack)
{
  triple d=vm::pop<triple>(Stack);
  triple c=vm::pop<triple>(Stack);
  triple b=vm::pop<triple>(Stack);
  triple a=vm::pop<triple>(Stack);
#line 371 "runpath3d.in"
  real A[]={a.getx(),a.gety(),a.getz()};
  real B[]={b.getx(),b.gety(),b.getz()};
  real C[]={c.getx(),c.gety(),c.getz()};
  real D[]={d.getx(),d.gety(),d.getz()};
  {Stack->push<real>(orient3d(A,B,C,D)); return;}
}

// Return a positive (negative) value if e lies inside (outside)
// the sphere passing through the points a,b,c,d oriented so that
// a--b--c--cycle appears in clockwise order when viewed from d
// or zero if all five points are cospherical.
// The value returned is the determinant
// |a.x a.y a.z a.x^2+a.y^2+a.z^2 1|
// |b.x b.y b.z b.x^2+b.y^2+b.z^2 1|
// |c.x c.y c.z c.x^2+c.y^2+c.z^2 1|
// |d.x d.y d.z d.x^2+d.y^2+d.z^2 1|
// |e.x e.y e.z e.x^2+e.y^2+e.z^2 1|
#line 389 "runpath3d.in"
// real insphere(triple a, triple b, triple c, triple d, triple e);
void gen_runpath3d43(stack *Stack)
{
  triple e=vm::pop<triple>(Stack);
  triple d=vm::pop<triple>(Stack);
  triple c=vm::pop<triple>(Stack);
  triple b=vm::pop<triple>(Stack);
  triple a=vm::pop<triple>(Stack);
#line 390 "runpath3d.in"
  real A[]={a.getx(),a.gety(),a.getz()};
  real B[]={b.getx(),b.gety(),b.getz()};
  real C[]={c.getx(),c.gety(),c.getz()};
  real D[]={d.getx(),d.gety(),d.getz()};
  real E[]={e.getx(),e.gety(),e.getz()};
  {Stack->push<real>(insphere(A,B,C,D,E)); return;}
}

} // namespace run

namespace trans {

void gen_runpath3d_venv(venv &ve)
{
#line 40 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d0, primPath3(), SYM(path3), formal(tripleArray(), SYM(pre), false, false), formal(tripleArray(), SYM(point), false, false), formal(tripleArray(), SYM(post), false, false), formal(booleanArray(), SYM(straight), false, false), formal(primBoolean(), SYM(cyclic), false, false));
#line 57 "runpath3d.in"
  REGISTER_BLTIN(run::nullPath3,"nullPath3");
#line 62 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d2, primBoolean(), SYM_EQ, formal(primPath3(), SYM(a), false, false), formal(primPath3(), SYM(b), false, false));
#line 67 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d3, primBoolean(), SYM_NEQ, formal(primPath3(), SYM(a), false, false), formal(primPath3(), SYM(b), false, false));
#line 72 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d4, primTriple(), SYM(point), formal(primPath3(), SYM(p), false, false), formal(primInt(), SYM(t), false, false));
#line 77 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d5, primTriple(), SYM(point), formal(primPath3(), SYM(p), false, false), formal(primReal(), SYM(t), false, false));
#line 82 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d6, primTriple(), SYM(precontrol), formal(primPath3(), SYM(p), false, false), formal(primInt(), SYM(t), false, false));
#line 87 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d7, primTriple(), SYM(precontrol), formal(primPath3(), SYM(p), false, false), formal(primReal(), SYM(t), false, false));
#line 92 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d8, primTriple(), SYM(postcontrol), formal(primPath3(), SYM(p), false, false), formal(primInt(), SYM(t), false, false));
#line 97 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d9, primTriple(), SYM(postcontrol), formal(primPath3(), SYM(p), false, false), formal(primReal(), SYM(t), false, false));
#line 102 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d10, primTriple(), SYM(dir), formal(primPath3(), SYM(p), false, false), formal(primInt(), SYM(t), false, false), formal(primInt(), SYM(sign), true, false), formal(primBoolean(), SYM(normalize), true, false));
#line 107 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d11, primTriple(), SYM(dir), formal(primPath3(), SYM(p), false, false), formal(primReal(), SYM(t), false, false), formal(primBoolean(), SYM(normalize), true, false));
#line 112 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d12, primTriple(), SYM(accel), formal(primPath3(), SYM(p), false, false), formal(primInt(), SYM(t), false, false), formal(primInt(), SYM(sign), true, false));
#line 117 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d13, primTriple(), SYM(accel), formal(primPath3(), SYM(p), false, false), formal(primReal(), SYM(t), false, false));
#line 122 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d14, primReal(), SYM(radius), formal(primPath3(), SYM(p), false, false), formal(primReal(), SYM(t), false, false));
#line 134 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d15, primReal(), SYM(radius), formal(primTriple(), SYM(z0), false, false), formal(primTriple(), SYM(c0), false, false), formal(primTriple(), SYM(c1), false, false), formal(primTriple(), SYM(z1), false, false), formal(primReal(), SYM(t), false, false));
#line 146 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d16, primPath3(), SYM(reverse), formal(primPath3(), SYM(p), false, false));
#line 151 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d17, primPath3(), SYM(subpath), formal(primPath3(), SYM(p), false, false), formal(primInt(), SYM(a), false, false), formal(primInt(), SYM(b), false, false));
#line 156 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d18, primPath3(), SYM(subpath), formal(primPath3(), SYM(p), false, false), formal(primReal(), SYM(a), false, false), formal(primReal(), SYM(b), false, false));
#line 161 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d19, primInt(), SYM(length), formal(primPath3(), SYM(p), false, false));
#line 166 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d20, primBoolean(), SYM(cyclic), formal(primPath3(), SYM(p), false, false));
#line 171 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d21, primBoolean(), SYM(straight), formal(primPath3(), SYM(p), false, false), formal(primInt(), SYM(t), false, false));
#line 176 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d22, primPath3(), SYM(unstraighten), formal(primPath3(), SYM(p), false, false));
#line 181 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d23, primReal(), SYM(straightness), formal(primTriple(), SYM(z0), false, false), formal(primTriple(), SYM(c0), false, false), formal(primTriple(), SYM(c1), false, false), formal(primTriple(), SYM(z1), false, false));
#line 188 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d24, primReal(), SYM(straightness), formal(primPath3(), SYM(p), false, false), formal(primInt(), SYM(t), false, false));
#line 196 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d25, primBoolean(), SYM(piecewisestraight), formal(primPath3(), SYM(p), false, false));
#line 201 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d26, primReal(), SYM(arclength), formal(primPath3(), SYM(p), false, false));
#line 206 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d27, primReal(), SYM(arclength), formal(primTriple(), SYM(z0), false, false), formal(primTriple(), SYM(c0), false, false), formal(primTriple(), SYM(c1), false, false), formal(primTriple(), SYM(z1), false, false));
#line 211 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d28, primReal(), SYM(arctime), formal(primPath3(), SYM(p), false, false), formal(primReal(), SYM(dval), false, false));
#line 216 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d29, realArray(), SYM(intersect), formal(primPath3(), SYM(p), false, false), formal(primPath3(), SYM(q), false, false), formal(primReal(), SYM(fuzz), true, false));
#line 234 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d30, realArray2(), SYM(intersections), formal(primPath3(), SYM(p), false, false), formal(primPath3(), SYM(q), false, false), formal(primReal(), SYM(fuzz), true, false));
#line 267 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d31, realArray(), SYM(intersect), formal(primPath3(), SYM(p), false, false), formal(tripleArray2(), SYM(p), false, false), formal(primReal(), SYM(fuzz), true, false));
#line 286 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d32, realArray2(), SYM(intersections), formal(primPath3(), SYM(p), false, false), formal(tripleArray2(), SYM(p), false, false), formal(primReal(), SYM(fuzz), true, false));
#line 307 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d33, primInt(), SYM(size), formal(primPath3(), SYM(p), false, false));
#line 312 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d34, primPath3(), SYM_AMPERSAND, formal(primPath3(), SYM(p), false, false), formal(primPath3(), SYM(q), false, false));
#line 317 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d35, primTriple(), SYM(min), formal(primPath3(), SYM(p), false, false));
#line 322 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d36, primTriple(), SYM(max), formal(primPath3(), SYM(p), false, false));
#line 327 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d37, realArray(), SYM(mintimes), formal(primPath3(), SYM(p), false, false));
#line 337 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d38, realArray(), SYM(maxtimes), formal(primPath3(), SYM(p), false, false));
#line 347 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d39, primPath3(), SYM_TIMES, formal(realArray2(), SYM(t), false, false), formal(primPath3(), SYM(g), false, false));
#line 352 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d40, primPair(), SYM(minratio), formal(primPath3(), SYM(g), false, false));
#line 357 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d41, primPair(), SYM(maxratio), formal(primPath3(), SYM(g), false, false));
#line 362 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d42, primReal(), SYM(orient), formal(primTriple(), SYM(a), false, false), formal(primTriple(), SYM(b), false, false), formal(primTriple(), SYM(c), false, false), formal(primTriple(), SYM(d), false, false));
#line 379 "runpath3d.in"
  addFunc(ve, run::gen_runpath3d43, primReal(), SYM(insphere), formal(primTriple(), SYM(a), false, false), formal(primTriple(), SYM(b), false, false), formal(primTriple(), SYM(c), false, false), formal(primTriple(), SYM(d), false, false), formal(primTriple(), SYM(e), false, false));
}

} // namespace trans
