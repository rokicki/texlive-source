/***** Autogenerated from runfile.in; changes will be overwritten *****/

#line 1 "runtimebase.in"
/*****
 * runtimebase.in
 * Andy Hammerlindl  2009/07/28
 *
 * Common declarations needed for all code-generating .in files.
 *
 *****/


#line 1 "runfile.in"
/*****
 * runfile.in
 *
 * Runtime functions for file operations.
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

using vm::stack;
using vm::error;
using vm::array;
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

double *copyArrayC(const array *a, size_t dim=0);
double *copyArray2C(const array *a, bool square=true, size_t dim2=0);

triple *copyTripleArrayC(const array *a, size_t dim=0);
triple *copyTripleArray2C(const array *a, bool square=true, size_t dim2=0);
double *copyTripleArray2Components(array *a, bool square=true, size_t dim2=0);
}

function *realRealFunction();

// Return the component of vector v perpendicular to a unit vector u.
inline triple perp(triple v, triple u)
{
  return v-dot(v,u)*u;
}

#define CURRENTPEN processData().currentpen

#line 10 "runfile.in"
#include "fileio.h"
#include "callable.h"
#include "triple.h"
#include "array.h"

using namespace camp;
using namespace settings;
using namespace vm;

string commentchar="#";

// Autogenerated routines:



namespace run {
#line 24 "runfile.in"
// bool ==(file *a, file *b);
void gen_runfile0(stack *Stack)
{
  file * b=vm::pop<file *>(Stack);
  file * a=vm::pop<file *>(Stack);
#line 25 "runfile.in"
  {Stack->push<bool>(a == b); return;}
}

#line 29 "runfile.in"
// bool !=(file *a, file *b);
void gen_runfile1(stack *Stack)
{
  file * b=vm::pop<file *>(Stack);
  file * a=vm::pop<file *>(Stack);
#line 30 "runfile.in"
  {Stack->push<bool>(a != b); return;}
}

#line 34 "runfile.in"
void nullFile(stack *Stack)
{
#line 35 "runfile.in"
  {Stack->push<file*>(&camp::nullfile); return;}
}

#line 39 "runfile.in"
// file* input(string name, bool check=true, string comment=commentchar);
void gen_runfile3(stack *Stack)
{
  string comment=vm::pop<string>(Stack,commentchar);
  bool check=vm::pop<bool>(Stack,true);
  string name=vm::pop<string>(Stack);
#line 40 "runfile.in"
  char c=comment.empty() ? (char) 0 : comment[0];
  file *f=new ifile(name,c,check);
  f->open();
  {Stack->push<file*>(f); return;}
}

#line 47 "runfile.in"
// file* output(string name, bool update=false, string comment=commentchar);
void gen_runfile4(stack *Stack)
{
  string comment=vm::pop<string>(Stack,commentchar);
  bool update=vm::pop<bool>(Stack,false);
  string name=vm::pop<string>(Stack);
#line 48 "runfile.in"
  file *f;
  if(update) {
    char c=comment.empty() ? (char) 0 : comment[0];
    f=new iofile(name,c);
  } else f=new ofile(name);
  f->open();
  if(update) f->seek(0,false);
  {Stack->push<file*>(f); return;}
}

#line 59 "runfile.in"
// file* xinput(string name, bool check=true);
void gen_runfile5(stack *Stack)
{
  bool check=vm::pop<bool>(Stack,true);
  string name=vm::pop<string>(Stack);
#line 60 "runfile.in"
#ifdef HAVE_RPC_RPC_H
  file *f=new ixfile(name,check);
  f->open();
  {Stack->push<file*>(f); return;}
#else  
  ostringstream buf;
  buf << name << ": XDR read support not enabled";
  error(buf);
  unused(&check); // Suppress unused variable warning
#endif
}

#line 73 "runfile.in"
// file* xoutput(string name, bool update=false);
void gen_runfile6(stack *Stack)
{
  bool update=vm::pop<bool>(Stack,false);
  string name=vm::pop<string>(Stack);
#line 74 "runfile.in"
#ifdef HAVE_RPC_RPC_H
  file *f;
  if(update)
    f=new ioxfile(name);
  else f=new oxfile(name);
  f->open();
  if(update) f->seek(0,false);
  {Stack->push<file*>(f); return;}
#else  
  ostringstream buf;
  buf << name << ": XDR write support not enabled";
  error(buf);
  unused(&update); // Suppress unused variable warning
#endif
}

#line 91 "runfile.in"
// file* binput(string name, bool check=true);
void gen_runfile7(stack *Stack)
{
  bool check=vm::pop<bool>(Stack,true);
  string name=vm::pop<string>(Stack);
#line 92 "runfile.in"
  file *f=new ibfile(name,check);
  f->open();
  {Stack->push<file*>(f); return;}
}

#line 98 "runfile.in"
// file* boutput(string name, bool update=false);
void gen_runfile8(stack *Stack)
{
  bool update=vm::pop<bool>(Stack,false);
  string name=vm::pop<string>(Stack);
#line 99 "runfile.in"
  file *f;
  if(update) f=new iobfile(name);
  else f=new obfile(name);
  f->open();
  if(update) f->seek(0,false);
  {Stack->push<file*>(f); return;}
}

#line 108 "runfile.in"
// bool eof(file *f);
void gen_runfile9(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 109 "runfile.in"
  {Stack->push<bool>(f->eof()); return;}
}

#line 113 "runfile.in"
// bool eol(file *f);
void gen_runfile10(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 114 "runfile.in"
  {Stack->push<bool>(f->eol()); return;}
}

#line 118 "runfile.in"
// bool error(file *f);
void gen_runfile11(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 119 "runfile.in"
  {Stack->push<bool>(f->error()); return;}
}

#line 123 "runfile.in"
// void clear(file *f);
void gen_runfile12(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 124 "runfile.in"
  f->clear();
}

#line 128 "runfile.in"
// void close(file *f);
void gen_runfile13(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 129 "runfile.in"
  f->close();
}

#line 133 "runfile.in"
// Int precision(file *f=NULL, Int digits=0);
void gen_runfile14(stack *Stack)
{
  Int digits=vm::pop<Int>(Stack,0);
  file * f=vm::pop<file *>(Stack,NULL);
#line 134 "runfile.in"
  if(f == 0) f=&camp::Stdout;
  {Stack->push<Int>(f->precision(digits)); return;}
}

#line 139 "runfile.in"
// void flush(file *f);
void gen_runfile15(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 140 "runfile.in"
  f->flush();
}

#line 144 "runfile.in"
// string getc(file *f);
void gen_runfile16(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 145 "runfile.in"
  char c=0;
  if(f->isOpen()) f->read(c);
  static char str[1];
  str[0]=c;
  {Stack->push<string>(string(str)); return;}
}

#line 153 "runfile.in"
// Int tell(file *f);
void gen_runfile17(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 154 "runfile.in"
  {Stack->push<Int>(f->tell()); return;}
}

#line 158 "runfile.in"
// void seek(file *f, Int pos);
void gen_runfile18(stack *Stack)
{
  Int pos=vm::pop<Int>(Stack);
  file * f=vm::pop<file *>(Stack);
#line 159 "runfile.in"
  f->seek(pos,pos >= 0);
}

#line 163 "runfile.in"
// void seekeof(file *f);
void gen_runfile19(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 164 "runfile.in"
  f->seek(0,false);
}

#line 168 "runfile.in"
void namePart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 169 "runfile.in"
  {Stack->push<string>(f.filename()); return;}
}

#line 173 "runfile.in"
void modePart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 174 "runfile.in"
  {Stack->push<string>(f.FileMode()); return;}
}

// Set file dimensions
#line 179 "runfile.in"
void dimensionSetHelper(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
  Int nz=vm::pop<Int>(Stack,-1);
  Int ny=vm::pop<Int>(Stack,-1);
  Int nx=vm::pop<Int>(Stack,-1);
#line 180 "runfile.in"
  f->dimension(nx,ny,nz);
  {Stack->push<file*>(f); return;}
}

#line 185 "runfile.in"
void dimensionSet(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 186 "runfile.in"
  {Stack->push<callable*>(new thunk(new bfunc(dimensionSetHelper),f)); return;}
}

#line 190 "runfile.in"
void dimensionPart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 191 "runfile.in"
  array *a=new array(3);
  (*a)[0]=f.Nx();
  (*a)[1]=f.Ny();
  (*a)[2]=f.Nz();
  {Stack->push<array*>(a); return;}
}

// Set file f to read arrays in line-at-a-time mode
#line 200 "runfile.in"
void lineSetHelper(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
  bool b=vm::pop<bool>(Stack,true);
#line 201 "runfile.in"
  f->LineMode(b);
  {Stack->push<file*>(f); return;}
}

#line 206 "runfile.in"
void lineSet(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 207 "runfile.in"
  {Stack->push<callable*>(new thunk(new bfunc(lineSetHelper),f)); return;}
}

#line 211 "runfile.in"
void linePart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 212 "runfile.in"
  {Stack->push<bool>(f.LineMode()); return;}
}

// Set file to read comma-separated values
#line 217 "runfile.in"
void csvSetHelper(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
  bool b=vm::pop<bool>(Stack,true);
#line 218 "runfile.in"
  f->CSVMode(b);
  {Stack->push<file*>(f); return;}
}

#line 223 "runfile.in"
void csvSet(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 224 "runfile.in"
  {Stack->push<callable*>(new thunk(new bfunc(csvSetHelper),f)); return;}
}

#line 228 "runfile.in"
void csvPart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 229 "runfile.in"
  {Stack->push<bool>(f.CSVMode()); return;}
}

// Set file to read whitespace-separated values
#line 234 "runfile.in"
void wordSetHelper(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
  bool b=vm::pop<bool>(Stack,true);
#line 235 "runfile.in"
  f->WordMode(b);
  {Stack->push<file*>(f); return;}
}

#line 240 "runfile.in"
void wordSet(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 241 "runfile.in"
  {Stack->push<callable*>(new thunk(new bfunc(wordSetHelper),f)); return;}
}

#line 245 "runfile.in"
void wordPart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 246 "runfile.in"
  {Stack->push<bool>(f.WordMode()); return;}
}

// Set file to read/write single precision real XDR values.
#line 251 "runfile.in"
void singlerealSetHelper(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
  bool b=vm::pop<bool>(Stack,true);
#line 252 "runfile.in"
  f->SingleReal(b);
  {Stack->push<file*>(f); return;}
}

#line 257 "runfile.in"
void singlerealSet(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 258 "runfile.in"
  {Stack->push<callable*>(new thunk(new bfunc(singlerealSetHelper),f)); return;}
}

#line 262 "runfile.in"
void singlerealPart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 263 "runfile.in"
  {Stack->push<bool>(f.SingleReal()); return;}
}

// Set file to read/write single precision int XDR values.
#line 268 "runfile.in"
void singleintSetHelper(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
  bool b=vm::pop<bool>(Stack,true);
#line 269 "runfile.in"
  f->SingleInt(b);
  {Stack->push<file*>(f); return;}
}

#line 274 "runfile.in"
void singleintSet(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 275 "runfile.in"
  {Stack->push<callable*>(new thunk(new bfunc(singleintSetHelper),f)); return;}
}

#line 279 "runfile.in"
void singleintPart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 280 "runfile.in"
  {Stack->push<bool>(f.SingleInt()); return;}
}

// Set file to read/write signed int XDR values.
#line 285 "runfile.in"
void signedintSetHelper(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
  bool b=vm::pop<bool>(Stack,true);
#line 286 "runfile.in"
  f->SignedInt(b);
  {Stack->push<file*>(f); return;}
}

#line 291 "runfile.in"
void signedintSet(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 292 "runfile.in"
  {Stack->push<callable*>(new thunk(new bfunc(signedintSetHelper),f)); return;}
}

#line 296 "runfile.in"
void signedintPart(stack *Stack)
{
  file f=vm::pop<file>(Stack);
#line 297 "runfile.in"
  {Stack->push<bool>(f.SignedInt()); return;}
}

// Set file to read an arrayi (i int sizes followed by an i-dimensional array)
#line 302 "runfile.in"
void readSetHelper(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
  Int i=vm::pop<Int>(Stack);
#line 303 "runfile.in"
  switch(i) {
    case 1:
      f->dimension(-2);
      break;
      
    case 2:
      f->dimension(-2,-2);
      break;
      
    case 3:
      f->dimension(-2,-2,-2);
      break;
      
    default:
      f->dimension();
  }
  
  {Stack->push<file*>(f); return;}
}

#line 324 "runfile.in"
void readSet(stack *Stack)
{
  file * f=vm::pop<file *>(Stack);
#line 325 "runfile.in"
  {Stack->push<callable*>(new thunk(new bfunc(readSetHelper),f)); return;}
}

// Delete file named s.
#line 330 "runfile.in"
// Int delete(string *s);
void gen_runfile45(stack *Stack)
{
  string * s=vm::pop<string *>(Stack);
#line 331 "runfile.in"
  checkLocal(*s);
  Int rc=unlink(s->c_str());
  if(rc == 0 && verbose > 0) 
    cout << "Deleted " << *s << endl;
  {Stack->push<Int>(rc); return;}
}

// Rename file "from" to file "to".
#line 340 "runfile.in"
// Int rename(string *from, string *to);
void gen_runfile46(stack *Stack)
{
  string * to=vm::pop<string *>(Stack);
  string * from=vm::pop<string *>(Stack);
#line 341 "runfile.in"
  checkLocal(*from);
  checkLocal(*to);
  Int rc=rename(from->c_str(),to->c_str());
  if(rc == 0 && verbose > 0) 
    cout << "Renamed " << *from << " to " << *to << endl;
  {Stack->push<Int>(rc); return;}
}

} // namespace run

namespace trans {

void gen_runfile_venv(venv &ve)
{
#line 24 "runfile.in"
  addFunc(ve, run::gen_runfile0, primBoolean(), "==", formal(primFile(), "a", false, false), formal(primFile(), "b", false, false));
#line 29 "runfile.in"
  addFunc(ve, run::gen_runfile1, primBoolean(), "!=", formal(primFile(), "a", false, false), formal(primFile(), "b", false, false));
#line 34 "runfile.in"
  REGISTER_BLTIN(run::nullFile,"nullFile");
#line 39 "runfile.in"
  addFunc(ve, run::gen_runfile3, primFile(), "input", formal(primString() , "name", false, false), formal(primBoolean(), "check", true, false), formal(primString() , "comment", true, false));
#line 47 "runfile.in"
  addFunc(ve, run::gen_runfile4, primFile(), "output", formal(primString() , "name", false, false), formal(primBoolean(), "update", true, false), formal(primString() , "comment", true, false));
#line 59 "runfile.in"
  addFunc(ve, run::gen_runfile5, primFile(), "xinput", formal(primString() , "name", false, false), formal(primBoolean(), "check", true, false));
#line 73 "runfile.in"
  addFunc(ve, run::gen_runfile6, primFile(), "xoutput", formal(primString() , "name", false, false), formal(primBoolean(), "update", true, false));
#line 91 "runfile.in"
  addFunc(ve, run::gen_runfile7, primFile(), "binput", formal(primString() , "name", false, false), formal(primBoolean(), "check", true, false));
#line 98 "runfile.in"
  addFunc(ve, run::gen_runfile8, primFile(), "boutput", formal(primString() , "name", false, false), formal(primBoolean(), "update", true, false));
#line 108 "runfile.in"
  addFunc(ve, run::gen_runfile9, primBoolean(), "eof", formal(primFile(), "f", false, false));
#line 113 "runfile.in"
  addFunc(ve, run::gen_runfile10, primBoolean(), "eol", formal(primFile(), "f", false, false));
#line 118 "runfile.in"
  addFunc(ve, run::gen_runfile11, primBoolean(), "error", formal(primFile(), "f", false, false));
#line 123 "runfile.in"
  addFunc(ve, run::gen_runfile12, primVoid(), "clear", formal(primFile(), "f", false, false));
#line 128 "runfile.in"
  addFunc(ve, run::gen_runfile13, primVoid(), "close", formal(primFile(), "f", false, false));
#line 133 "runfile.in"
  addFunc(ve, run::gen_runfile14, primInt(), "precision", formal(primFile(), "f", true, false), formal(primInt(), "digits", true, false));
#line 139 "runfile.in"
  addFunc(ve, run::gen_runfile15, primVoid(), "flush", formal(primFile(), "f", false, false));
#line 144 "runfile.in"
  addFunc(ve, run::gen_runfile16, primString() , "getc", formal(primFile(), "f", false, false));
#line 153 "runfile.in"
  addFunc(ve, run::gen_runfile17, primInt(), "tell", formal(primFile(), "f", false, false));
#line 158 "runfile.in"
  addFunc(ve, run::gen_runfile18, primVoid(), "seek", formal(primFile(), "f", false, false), formal(primInt(), "pos", false, false));
#line 163 "runfile.in"
  addFunc(ve, run::gen_runfile19, primVoid(), "seekeof", formal(primFile(), "f", false, false));
#line 168 "runfile.in"
  REGISTER_BLTIN(run::namePart,"namePart");
#line 173 "runfile.in"
  REGISTER_BLTIN(run::modePart,"modePart");
#line 178 "runfile.in"
  REGISTER_BLTIN(run::dimensionSetHelper,"dimensionSetHelper");
#line 185 "runfile.in"
  REGISTER_BLTIN(run::dimensionSet,"dimensionSet");
#line 190 "runfile.in"
  REGISTER_BLTIN(run::dimensionPart,"dimensionPart");
#line 199 "runfile.in"
  REGISTER_BLTIN(run::lineSetHelper,"lineSetHelper");
#line 206 "runfile.in"
  REGISTER_BLTIN(run::lineSet,"lineSet");
#line 211 "runfile.in"
  REGISTER_BLTIN(run::linePart,"linePart");
#line 216 "runfile.in"
  REGISTER_BLTIN(run::csvSetHelper,"csvSetHelper");
#line 223 "runfile.in"
  REGISTER_BLTIN(run::csvSet,"csvSet");
#line 228 "runfile.in"
  REGISTER_BLTIN(run::csvPart,"csvPart");
#line 233 "runfile.in"
  REGISTER_BLTIN(run::wordSetHelper,"wordSetHelper");
#line 240 "runfile.in"
  REGISTER_BLTIN(run::wordSet,"wordSet");
#line 245 "runfile.in"
  REGISTER_BLTIN(run::wordPart,"wordPart");
#line 250 "runfile.in"
  REGISTER_BLTIN(run::singlerealSetHelper,"singlerealSetHelper");
#line 257 "runfile.in"
  REGISTER_BLTIN(run::singlerealSet,"singlerealSet");
#line 262 "runfile.in"
  REGISTER_BLTIN(run::singlerealPart,"singlerealPart");
#line 267 "runfile.in"
  REGISTER_BLTIN(run::singleintSetHelper,"singleintSetHelper");
#line 274 "runfile.in"
  REGISTER_BLTIN(run::singleintSet,"singleintSet");
#line 279 "runfile.in"
  REGISTER_BLTIN(run::singleintPart,"singleintPart");
#line 284 "runfile.in"
  REGISTER_BLTIN(run::signedintSetHelper,"signedintSetHelper");
#line 291 "runfile.in"
  REGISTER_BLTIN(run::signedintSet,"signedintSet");
#line 296 "runfile.in"
  REGISTER_BLTIN(run::signedintPart,"signedintPart");
#line 301 "runfile.in"
  REGISTER_BLTIN(run::readSetHelper,"readSetHelper");
#line 324 "runfile.in"
  REGISTER_BLTIN(run::readSet,"readSet");
#line 329 "runfile.in"
  addFunc(ve, run::gen_runfile45, primInt(), "delete", formal(primString(), "s", false, false));
#line 339 "runfile.in"
  addFunc(ve, run::gen_runfile46, primInt(), "rename", formal(primString(), "from", false, false), formal(primString(), "to", false, false));
}

} // namespace trans
