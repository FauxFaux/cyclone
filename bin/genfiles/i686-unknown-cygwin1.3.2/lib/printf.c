 extern void exit( int); extern void* abort(); struct Cyc_Core_Opt{ void* v; } ;
extern unsigned char Cyc_Core_InvalidArg[ 15u]; struct Cyc_Core_InvalidArg_struct{
unsigned char* tag; struct _tagged_arr f1; } ; extern unsigned char Cyc_Core_Failure[
12u]; struct Cyc_Core_Failure_struct{ unsigned char* tag; struct _tagged_arr f1;
} ; extern unsigned char Cyc_Core_Impossible[ 15u]; struct Cyc_Core_Impossible_struct{
unsigned char* tag; struct _tagged_arr f1; } ; extern unsigned char Cyc_Core_Not_found[
14u]; extern unsigned char Cyc_Core_Unreachable[ 16u]; struct Cyc_Core_Unreachable_struct{
unsigned char* tag; struct _tagged_arr f1; } ; extern struct _RegionHandle* Cyc_Core_heap_region;
extern unsigned char* string_to_Cstring( struct _tagged_arr); extern
unsigned char* underlying_Cstring( struct _tagged_arr); extern struct
_tagged_arr Cstring_to_string( unsigned char*); extern struct _tagged_arr
wrap_Cstring_as_string( unsigned char*, unsigned int); extern struct _tagged_arr
ntCsl_to_ntsl( unsigned char**); extern int system( unsigned char*); struct Cyc_std___sFILE;
extern struct Cyc_std___sFILE* Cyc_std_stdout; extern int Cyc_std_putc( int __c,
struct Cyc_std___sFILE* __stream); extern unsigned char Cyc_std_FileCloseError[
19u]; extern unsigned char Cyc_std_FileOpenError[ 18u]; struct Cyc_std_FileOpenError_struct{
unsigned char* tag; struct _tagged_arr f1; } ; static const int Cyc_std_String_pa=
0; struct Cyc_std_String_pa_struct{ int tag; struct _tagged_arr f1; } ; static
const int Cyc_std_Int_pa= 1; struct Cyc_std_Int_pa_struct{ int tag; unsigned int
f1; } ; static const int Cyc_std_Double_pa= 2; struct Cyc_std_Double_pa_struct{
int tag; double f1; } ; static const int Cyc_std_ShortPtr_pa= 3; struct Cyc_std_ShortPtr_pa_struct{
int tag; short* f1; } ; static const int Cyc_std_IntPtr_pa= 4; struct Cyc_std_IntPtr_pa_struct{
int tag; unsigned int* f1; } ; extern int Cyc_std_fprintf( struct Cyc_std___sFILE*,
struct _tagged_arr fmt, struct _tagged_arr); extern int Cyc_std_printf( struct
_tagged_arr fmt, struct _tagged_arr); extern int Cyc_std_sprintf( struct
_tagged_arr s, struct _tagged_arr fmt, struct _tagged_arr); extern struct
_tagged_arr Cyc_std_aprintf( struct _tagged_arr fmt, struct _tagged_arr); extern
struct _tagged_arr Cyc_std_raprintf( struct _RegionHandle*, struct _tagged_arr
fmt, struct _tagged_arr ap); extern int Cyc_std_vfprintf( struct Cyc_std___sFILE*,
struct _tagged_arr fmt, struct _tagged_arr ap); extern int Cyc_std_vprintf(
struct _tagged_arr fmt, struct _tagged_arr); extern int Cyc_std_vsprintf( struct
_tagged_arr s, struct _tagged_arr fmt, struct _tagged_arr); extern struct
_tagged_arr Cyc_std_vraprintf( struct _RegionHandle* r1, struct _tagged_arr fmt,
struct _tagged_arr ap); static const int Cyc_std_ShortPtr_sa= 0; struct Cyc_std_ShortPtr_sa_struct{
int tag; short* f1; } ; static const int Cyc_std_UShortPtr_sa= 1; struct Cyc_std_UShortPtr_sa_struct{
int tag; unsigned short* f1; } ; static const int Cyc_std_IntPtr_sa= 2; struct
Cyc_std_IntPtr_sa_struct{ int tag; int* f1; } ; static const int Cyc_std_UIntPtr_sa=
3; struct Cyc_std_UIntPtr_sa_struct{ int tag; unsigned int* f1; } ; static const
int Cyc_std_StringPtr_sa= 4; struct Cyc_std_StringPtr_sa_struct{ int tag; struct
_tagged_arr f1; } ; static const int Cyc_std_DoublePtr_sa= 5; struct Cyc_std_DoublePtr_sa_struct{
int tag; double* f1; } ; static const int Cyc_std_FloatPtr_sa= 6; struct Cyc_std_FloatPtr_sa_struct{
int tag; float* f1; } ; struct Cyc_List_List{ void* hd; struct Cyc_List_List* tl;
} ; extern unsigned char Cyc_List_List_empty[ 15u]; extern unsigned char Cyc_List_List_mismatch[
18u]; extern unsigned char Cyc_List_Nth[ 8u]; extern unsigned int Cyc_std_strlen(
struct _tagged_arr s); extern int isalnum( int); extern int isalpha( int);
extern int isascii( int); extern int iscntrl( int); extern int isdigit( int);
extern int isgraph( int); extern int islower( int); extern int isprint( int);
extern int ispunct( int); extern int isspace( int); extern int isupper( int);
extern int isxdigit( int); extern int toascii( int); extern int tolower( int);
extern int toupper( int); extern int _tolower( int); extern int _toupper( int);
struct Cyc_std__Div{ int quot; int rem; } ; struct Cyc_std__Ldiv{ int quot; int
rem; } ; extern int abs( int __x); extern int atexit( void(* __func)()); extern
struct Cyc_std__Div div( int __numer, int __denom); extern struct Cyc_std__Ldiv
ldiv( int __numer, int __denom); extern int random(); extern void srandom(
unsigned int __seed); extern int rand(); extern void srand( unsigned int __seed);
extern int rand_r( unsigned int* __seed); extern int grantpt( int __fd); extern
int unlockpt( int __fd); extern double acos( double); extern float acosf( float);
extern double acosh( double); extern float acoshf( float); extern double asin(
double); extern float asinf( float); extern double asinh( double); extern float
asinhf( float); extern double atan( double); extern double atan2( double, double);
extern float atan2f( float, float); extern float atanf( float); extern double
atanh( double); extern float atanhf( float); extern double cbrt( double); extern
float cbrtf( float); extern double ceil( double); extern float ceilf( float);
extern double copysign( double, double); extern float copysignf( float, float);
extern double cos( double); extern float cosf( float); extern double cosh(
double); extern float coshf( float); extern double drem( double, double); extern
float dremf( float, float); extern double erf( double); extern double erfc(
double); extern float erfcf( float); extern float erff( float); extern double
exp( double); extern float expf( float); extern double expm1( double); extern
float expm1f( float); extern double fabs( double); extern float fabsf( float);
extern int finite( double); extern int finitef( float); extern double floor(
double); extern float floorf( float); extern double fmod( double, double);
extern float fmodf( float, float); extern double frexp( double, int*); extern
float frexpf( float, int*); extern double gamma( double); extern float gammaf(
float); extern double hypot( double, double); extern float hypotf( float, float);
extern int ilogb( double); extern int ilogbf( float); extern int isinf( double);
extern int isinff( float); extern int isnan( double); extern int isnanf( float);
extern double j0( double); extern float j0f( float); extern double j1( double);
extern float j1f( float); extern double jn( int, double); extern float jnf( int,
float); extern double ldexp( double, int); extern float ldexpf( float, int);
extern double lgamma( double); extern float lgammaf( float); extern double log(
double); extern float logf( float); extern double log10( double); extern float
log10f( float); extern double log1p( double); extern float log1pf( float);
extern double modf( double, double*); extern float modff( float, float*); extern
double nextafter( double, double); extern float nextafterf( float, float);
extern double pow( double, double); extern float powf( float, float); extern
double remainder( double, double); extern float remainderf( float, float);
extern double rint( double); extern float rintf( float); extern double scalbn(
double, int); extern float scalbnf( float, int); extern double sin( double);
extern float sinf( float); extern double sinh( double); extern float sinhf(
float); extern double sqrt( double); extern float sqrtf( float); extern double
tan( double); extern float tanf( float); extern double tanh( double); extern
float tanhf( float); extern double y0( double); extern float y0f( float); extern
double y1( double); extern float y1f( float); extern double yn( int, double);
extern float ynf( int, float); static struct _tagged_arr Cyc_std_parg2string(
void* x){ void* _temp0= x; _LL2: if(*(( int*) _temp0) ==  Cyc_std_String_pa){
goto _LL3;} else{ goto _LL4;} _LL4: if(*(( int*) _temp0) ==  Cyc_std_Int_pa){
goto _LL5;} else{ goto _LL6;} _LL6: if(*(( int*) _temp0) ==  Cyc_std_Double_pa){
goto _LL7;} else{ goto _LL8;} _LL8: if(*(( int*) _temp0) ==  Cyc_std_ShortPtr_pa){
goto _LL9;} else{ goto _LL10;} _LL10: if(*(( int*) _temp0) ==  Cyc_std_IntPtr_pa){
goto _LL11;} else{ goto _LL1;} _LL3: return _tag_arr("string", sizeof(
unsigned char), 7u); _LL5: return _tag_arr("int", sizeof( unsigned char), 4u);
_LL7: return _tag_arr("double", sizeof( unsigned char), 7u); _LL9: return
_tag_arr("short *", sizeof( unsigned char), 8u); _LL11: return _tag_arr("unsigned long *",
sizeof( unsigned char), 16u); _LL1:;} static void* Cyc_std_badarg( struct
_tagged_arr s){ return( void*) _throw(( void*)({ struct Cyc_Core_InvalidArg_struct*
_temp12=( struct Cyc_Core_InvalidArg_struct*) GC_malloc( sizeof( struct Cyc_Core_InvalidArg_struct));
_temp12[ 0]=({ struct Cyc_Core_InvalidArg_struct _temp13; _temp13.tag= Cyc_Core_InvalidArg;
_temp13.f1= s; _temp13;}); _temp12;}));} static int Cyc_std_va_arg_int( struct
_tagged_arr ap){ void* _temp14=*(( void**) _check_unknown_subscript( ap, sizeof(
void*), 0u)); unsigned int _temp20; _LL16: if(*(( int*) _temp14) ==  Cyc_std_Int_pa){
_LL21: _temp20=(( struct Cyc_std_Int_pa_struct*) _temp14)->f1; goto _LL17;}
else{ goto _LL18;} _LL18: goto _LL19; _LL17: return( int) _temp20; _LL19: return((
int(*)( struct _tagged_arr s)) Cyc_std_badarg)( _tag_arr("printf expected int",
sizeof( unsigned char), 20u)); _LL15:;} static int Cyc_std_va_arg_long( struct
_tagged_arr ap){ void* _temp22=*(( void**) _check_unknown_subscript( ap, sizeof(
void*), 0u)); unsigned int _temp28; _LL24: if(*(( int*) _temp22) ==  Cyc_std_Int_pa){
_LL29: _temp28=(( struct Cyc_std_Int_pa_struct*) _temp22)->f1; goto _LL25;}
else{ goto _LL26;} _LL26: goto _LL27; _LL25: return( int) _temp28; _LL27: return((
int(*)( struct _tagged_arr s)) Cyc_std_badarg)( _tag_arr("printf expected int",
sizeof( unsigned char), 20u)); _LL23:;} static unsigned int Cyc_std_va_arg_ulong(
struct _tagged_arr ap){ void* _temp30=*(( void**) _check_unknown_subscript( ap,
sizeof( void*), 0u)); unsigned int _temp36; _LL32: if(*(( int*) _temp30) ==  Cyc_std_Int_pa){
_LL37: _temp36=(( struct Cyc_std_Int_pa_struct*) _temp30)->f1; goto _LL33;}
else{ goto _LL34;} _LL34: goto _LL35; _LL33: return( unsigned int) _temp36;
_LL35: return(( unsigned int(*)( struct _tagged_arr s)) Cyc_std_badarg)(
_tag_arr("printf expected int", sizeof( unsigned char), 20u)); _LL31:;} static
unsigned int Cyc_std_va_arg_uint( struct _tagged_arr ap){ void* _temp38=*(( void**)
_check_unknown_subscript( ap, sizeof( void*), 0u)); unsigned int _temp44; _LL40:
if(*(( int*) _temp38) ==  Cyc_std_Int_pa){ _LL45: _temp44=(( struct Cyc_std_Int_pa_struct*)
_temp38)->f1; goto _LL41;} else{ goto _LL42;} _LL42: goto _LL43; _LL41: return(
unsigned int) _temp44; _LL43: return(( unsigned int(*)( struct _tagged_arr s))
Cyc_std_badarg)( _tag_arr("printf expected int", sizeof( unsigned char), 20u));
_LL39:;} static double Cyc_std_va_arg_double( struct _tagged_arr ap){ void*
_temp46=*(( void**) _check_unknown_subscript( ap, sizeof( void*), 0u)); double
_temp52; _LL48: if(*(( int*) _temp46) ==  Cyc_std_Double_pa){ _LL53: _temp52=((
struct Cyc_std_Double_pa_struct*) _temp46)->f1; goto _LL49;} else{ goto _LL50;}
_LL50: goto _LL51; _LL49: return _temp52; _LL51:( int) _throw( Cyc_std_badarg((
struct _tagged_arr)({ struct Cyc_std_String_pa_struct _temp55; _temp55.tag= Cyc_std_String_pa;
_temp55.f1=( struct _tagged_arr) Cyc_std_parg2string(*(( void**)
_check_unknown_subscript( ap, sizeof( void*), 0u)));{ void* _temp54[ 1u]={&
_temp55}; Cyc_std_aprintf( _tag_arr("printf expected double but found %s",
sizeof( unsigned char), 36u), _tag_arr( _temp54, sizeof( void*), 1u));}})));
_LL47:;} static short* Cyc_std_va_arg_short_ptr( struct _tagged_arr ap){ void*
_temp56=*(( void**) _check_unknown_subscript( ap, sizeof( void*), 0u)); short*
_temp62; _LL58: if(*(( int*) _temp56) ==  Cyc_std_ShortPtr_pa){ _LL63: _temp62=((
struct Cyc_std_ShortPtr_pa_struct*) _temp56)->f1; goto _LL59;} else{ goto _LL60;}
_LL60: goto _LL61; _LL59: return _temp62; _LL61:( int) _throw( Cyc_std_badarg(
_tag_arr("printf expected short pointer", sizeof( unsigned char), 30u))); _LL57:;}
static unsigned int* Cyc_std_va_arg_int_ptr( struct _tagged_arr ap){ void*
_temp64=*(( void**) _check_unknown_subscript( ap, sizeof( void*), 0u));
unsigned int* _temp70; _LL66: if(*(( int*) _temp64) ==  Cyc_std_IntPtr_pa){
_LL71: _temp70=(( struct Cyc_std_IntPtr_pa_struct*) _temp64)->f1; goto _LL67;}
else{ goto _LL68;} _LL68: goto _LL69; _LL67: return _temp70; _LL69:( int) _throw(
Cyc_std_badarg( _tag_arr("printf expected int pointer", sizeof( unsigned char),
28u))); _LL65:;} static struct _tagged_arr Cyc_std_va_arg_string( struct
_tagged_arr ap){ void* _temp72=*(( void**) _check_unknown_subscript( ap, sizeof(
void*), 0u)); struct _tagged_arr _temp78; _LL74: if(*(( int*) _temp72) ==  Cyc_std_String_pa){
_LL79: _temp78=(( struct Cyc_std_String_pa_struct*) _temp72)->f1; goto _LL75;}
else{ goto _LL76;} _LL76: goto _LL77; _LL75: return _temp78; _LL77:( int) _throw(
Cyc_std_badarg( _tag_arr("printf expected string", sizeof( unsigned char), 23u)));
_LL73:;} int Cyc_std___cvt_double( double number, int prec, int flags, int*
signp, int fmtch, struct _tagged_arr startp, struct _tagged_arr endp); enum  Cyc_std_BASE{
Cyc_std_OCT  =  0u, Cyc_std_DEC  =  1u, Cyc_std_HEX  =  2u}; static int Cyc_std__IO_sputn(
int(* ioputc)( int, void*), void* ioputc_env, struct _tagged_arr s, int howmany){
int i= 0; while( i <  howmany) { if( _get_arr_size( s, sizeof( unsigned char))
==  0? 1: ioputc(( int)*(( const unsigned char*) _check_unknown_subscript( s,
sizeof( unsigned char), 0u)), ioputc_env) ==  - 1){ return i;}
_tagged_arr_inplace_plus_post(& s, sizeof( unsigned char), 1); i ++;} return i;}
static int Cyc_std__IO_padn( int(* ioputc)( int, void*), void* ioputc_env,
unsigned char c, int howmany){ int i= 0; while( i <  howmany) { if( ioputc(( int)
c, ioputc_env) ==  - 1){ return i;} i ++;} return i;} static struct _tagged_arr
Cyc_std_memchr( struct _tagged_arr s, unsigned char c, int n){ int sz=( int)
_get_arr_size( s, sizeof( unsigned char)); n= n <  sz? n: sz; for( 0; n !=  0;(
n --, _tagged_arr_inplace_plus_post(& s, sizeof( unsigned char), 1))){ if(*((
const unsigned char*) _check_unknown_subscript( s, sizeof( unsigned char), 0u))
==  c){ return s;}} return( struct _tagged_arr) _tag_arr( 0u, 0u, 0u);} int Cyc_std__IO_vfprintf(
int(* ioputc)( int, void*), void* ioputc_env, struct _tagged_arr fmt0, struct
_tagged_arr ap){ struct _tagged_arr fmt; int ch; int n; struct _tagged_arr cp=
_tag_arr( 0u, 0u, 0u); struct _tagged_arr cp2=( struct _tagged_arr) _tag_arr( 0u,
0u, 0u); int use_cp2; struct _tagged_arr fmark; int flags; int ret; int width;
int prec; unsigned char sign; int softsign= 0; double _double; int fpprec;
unsigned int _ulong; int dprec; int dpad; int fieldsz; int size= 0;
unsigned char buf[ 348u];{ unsigned int _temp124= 348u; unsigned int i; for( i=
0; i <  _temp124; i ++){ buf[ i]='\000';}}{ unsigned char ox[ 2u]={'\000','\000'};
enum  Cyc_std_BASE base; fmt= fmt0; ret= 0; for( 0; 1; 0){ for( fmark= fmt;( ch=(
int)*(( const unsigned char*) _check_unknown_subscript( fmt, sizeof(
unsigned char), 0u))) != '\000'? ch != '%': 0; _tagged_arr_inplace_plus_post(&
fmt, sizeof( unsigned char), 1)){;} if(( n=( fmt.curr -  fmark.curr) /  sizeof(
unsigned char)) !=  0){ do { if( Cyc_std__IO_sputn( ioputc, ioputc_env, fmark, n)
!=  n){ goto error;}} while ( 0); ret += n;} if( ch == '\000'){ goto done;}
_tagged_arr_inplace_plus_post(& fmt, sizeof( unsigned char), 1); flags= 0; dprec=
0; fpprec= 0; width= 0; prec= - 1; sign='\000'; rflag: ch=( int)*(( const
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& fmt,
sizeof( unsigned char), 1), sizeof( unsigned char), 0u)); reswitch: use_cp2= 0;
switch( ch){ case ' ': _LL80: if( !(( int) sign)){ sign=' ';} goto rflag; case '#':
_LL81: flags |= 8; goto rflag; case '*': _LL82: width= Cyc_std_va_arg_int( ap);
_tagged_arr_inplace_plus_post(& ap, sizeof( void*), 1); if( width >=  0){ goto
rflag;} width= - width; goto _LL83; case '-': _LL83: flags |= 16; flags &= ~ 32;
goto rflag; case '+': _LL84: sign='+'; goto rflag; case '.': _LL85: if(( ch=(
int)*(( const unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& fmt, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))) == '*'){ n= Cyc_std_va_arg_int( ap);
_tagged_arr_inplace_plus_post(& ap, sizeof( void*), 1); prec= n <  0? - 1: n;
goto rflag;} n= 0; while(( unsigned int)( ch - '0') <=  9) { n= 10 *  n + ( ch - '0');
ch=( int)*(( const unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& fmt, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u));} prec= n <  0? - 1: n; goto reswitch; case '0': _LL86: if(
!( flags &  16)){ flags |= 32;} goto rflag; case '1': _LL87: goto _LL88; case '2':
_LL88: goto _LL89; case '3': _LL89: goto _LL90; case '4': _LL90: goto _LL91;
case '5': _LL91: goto _LL92; case '6': _LL92: goto _LL93; case '7': _LL93: goto
_LL94; case '8': _LL94: goto _LL95; case '9': _LL95: n= 0; do { n= 10 *  n + (
ch - '0'); ch=( int)*(( const unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& fmt, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u));} while (( unsigned int)( ch - '0') <=  9); width= n; goto
reswitch; case 'L': _LL96: flags |= 2; goto rflag; case 'h': _LL97: flags |= 4;
goto rflag; case 'l': _LL98: flags |= 1; goto rflag; case 'c': _LL99: cp=
_tag_arr( buf, sizeof( unsigned char), 348u);*(( unsigned char*)
_check_unknown_subscript( cp, sizeof( unsigned char), 0u))=( unsigned char) Cyc_std_va_arg_int(
ap); _tagged_arr_inplace_plus_post(& ap, sizeof( void*), 1); size= 1; sign='\000';
break; case 'D': _LL100: flags |= 1; goto _LL101; case 'd': _LL101: goto _LL102;
case 'i': _LL102: _ulong=( unsigned int)( flags &  1? Cyc_std_va_arg_long( ap):(
flags &  4?( int)(( short) Cyc_std_va_arg_int( ap)):( int) Cyc_std_va_arg_int(
ap))); _tagged_arr_inplace_plus_post(& ap, sizeof( void*), 1); if(( int) _ulong
<  0){ _ulong= - _ulong; sign='-';} base= Cyc_std_DEC; goto number; case 'e':
_LL103: goto _LL104; case 'E': _LL104: goto _LL105; case 'f': _LL105: goto
_LL106; case 'F': _LL106: goto _LL107; case 'g': _LL107: goto _LL108; case 'G':
_LL108: _double= Cyc_std_va_arg_double( ap); _tagged_arr_inplace_plus_post(& ap,
sizeof( void*), 1); if( prec >  39){ if(( ch != 'g'? ch != 'G': 0)? 1: flags & 
8){ fpprec= prec -  39;} prec= 39;} else{ if( prec ==  - 1){ prec= 6;}} cp=
_tag_arr( buf, sizeof( unsigned char), 348u);*(( unsigned char*)
_check_unknown_subscript( cp, sizeof( unsigned char), 0u))='\000'; size= Cyc_std___cvt_double(
_double, prec, flags,& softsign, ch, cp, _tagged_arr_plus( _tag_arr( buf,
sizeof( unsigned char), 348u), sizeof( unsigned char),( int) sizeof( buf))); if(
softsign){ sign='-';} if(*(( unsigned char*) _check_unknown_subscript( cp,
sizeof( unsigned char), 0u)) == '\000'){ _tagged_arr_inplace_plus_post(& cp,
sizeof( unsigned char), 1);} break; case 'n': _LL109: if( flags &  1){* Cyc_std_va_arg_int_ptr(
ap)=( unsigned int) ret;} else{ if( flags &  4){* Cyc_std_va_arg_short_ptr( ap)=(
short) ret;} else{* Cyc_std_va_arg_int_ptr( ap)=( unsigned int) ret;}}
_tagged_arr_inplace_plus_post(& ap, sizeof( void*), 1); continue; case 'O':
_LL110: flags |= 1; goto _LL111; case 'o': _LL111: _ulong= flags &  1? Cyc_std_va_arg_ulong(
ap):( flags &  4?( unsigned int)(( unsigned short) Cyc_std_va_arg_int( ap)):(
unsigned int) Cyc_std_va_arg_uint( ap)); _tagged_arr_inplace_plus_post(& ap,
sizeof( void*), 1); base= Cyc_std_OCT; goto nosign; case 'p': _LL112: _ulong=(
unsigned int) Cyc_std_va_arg_long( ap); _tagged_arr_inplace_plus_post(& ap,
sizeof( void*), 1); base= Cyc_std_HEX; flags |= 64; ch=( int)'x'; goto nosign;
case 's': _LL113: use_cp2= 1; cp2= Cyc_std_va_arg_string( ap);
_tagged_arr_inplace_plus_post(& ap, sizeof( void*), 1); if( cp2.curr == ((
struct _tagged_arr) _tag_arr( 0u, 0u, 0u)).curr){ cp2= _tag_arr("(NULL)",
sizeof( unsigned char), 7u);} if( prec >=  0){ struct _tagged_arr p= Cyc_std_memchr(
cp2,'\000', prec); if( p.curr != (( struct _tagged_arr) _tag_arr( 0u, 0u, 0u)).curr){
size=( p.curr -  cp2.curr) /  sizeof( unsigned char); if( size >  prec){ size=
prec;}} else{ size= prec;}} else{ size=( int) Cyc_std_strlen( cp2);} sign='\000';
break; case 'U': _LL114: flags |= 1; goto _LL115; case 'u': _LL115: _ulong=
flags &  1? Cyc_std_va_arg_ulong( ap):( flags &  4?( unsigned int)((
unsigned short) Cyc_std_va_arg_int( ap)):( unsigned int) Cyc_std_va_arg_uint( ap));
_tagged_arr_inplace_plus_post(& ap, sizeof( void*), 1); base= Cyc_std_DEC; goto
nosign; case 'X': _LL116: goto _LL117; case 'x': _LL117: _ulong= flags &  1? Cyc_std_va_arg_ulong(
ap):( flags &  4?( unsigned int)(( unsigned short) Cyc_std_va_arg_int( ap)):(
unsigned int) Cyc_std_va_arg_uint( ap)); _tagged_arr_inplace_plus_post(& ap,
sizeof( void*), 1); base= Cyc_std_HEX; if( flags &  8? _ulong !=  0: 0){ flags |=
64;} nosign: sign='\000'; number: if(( dprec= prec) >=  0){ flags &= ~ 32;} cp=
_tagged_arr_plus( _tag_arr( buf, sizeof( unsigned char), 348u), sizeof(
unsigned char),( 308 +  39) +  1); if( _ulong !=  0? 1: prec !=  0){ struct
_tagged_arr xdigs; switch( base){ case Cyc_std_OCT: _LL119: do {*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus(& cp, sizeof(
unsigned char), -1), sizeof( unsigned char), 0u))=( unsigned char)(( _ulong &  7)
+ '0'); _ulong >>= 3;} while (( int) _ulong); if( flags &  8?*(( unsigned char*)
_check_unknown_subscript( cp, sizeof( unsigned char), 0u)) != '0': 0){*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus(& cp, sizeof(
unsigned char), -1), sizeof( unsigned char), 0u))='0';} break; case Cyc_std_DEC:
_LL120: while( _ulong >=  10) {*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus(& cp, sizeof( unsigned char), -1), sizeof(
unsigned char), 0u))=( unsigned char)( _ulong %  10 + '0'); _ulong /= 10;}*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus(& cp, sizeof(
unsigned char), -1), sizeof( unsigned char), 0u))=( unsigned char)( _ulong + '0');
break; case Cyc_std_HEX: _LL121: if( ch == 'X'){ xdigs= _tag_arr("0123456789ABCDEF",
sizeof( unsigned char), 17u);} else{ xdigs= _tag_arr("0123456789abcdef", sizeof(
unsigned char), 17u);} do {*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus(& cp, sizeof( unsigned char), -1), sizeof(
unsigned char), 0u))=*(( const unsigned char*) _check_unknown_subscript( xdigs,
sizeof( unsigned char),( int)( _ulong &  15))); _ulong >>= 4;} while (( int)
_ulong); break;}} size=(( _tagged_arr_plus( _tag_arr( buf, sizeof( unsigned char),
348u), sizeof( unsigned char),( 308 +  39) +  1)).curr -  cp.curr) /  sizeof(
unsigned char); skipsize: break; default: _LL118: if( ch == '\000'){ goto done;}
cp= _tag_arr( buf, sizeof( unsigned char), 348u);*(( unsigned char*)
_check_unknown_subscript( cp, sizeof( unsigned char), 0u))=( unsigned char) ch;
size= 1; sign='\000'; break;} fieldsz= size +  fpprec; dpad= dprec -  size; if(
dpad <  0){ dpad= 0;} if(( int) sign){ fieldsz ++;} else{ if( flags &  64){
fieldsz += 2;}} fieldsz += dpad; if(( flags & ( 16 |  32)) ==  0){ if( Cyc_std__IO_padn(
ioputc, ioputc_env,' ', width -  fieldsz) <  width -  fieldsz){ goto error;}}
if(( int) sign){ do { if( Cyc_std__IO_sputn( ioputc, ioputc_env, _tag_arr(& sign,
sizeof( unsigned char), 1u), 1) !=  1){ goto error;}} while ( 0);} else{ if(
flags &  64){ ox[ _check_known_subscript_notnull( 2u, 0)]='0'; ox[
_check_known_subscript_notnull( 2u, 1)]=( unsigned char) ch; do { if( Cyc_std__IO_sputn(
ioputc, ioputc_env, _tag_arr( ox, sizeof( unsigned char), 2u), 2) !=  2){ goto
error;}} while ( 0);}} if(( flags & ( 16 |  32)) ==  32){ if( Cyc_std__IO_padn(
ioputc, ioputc_env,'0', width -  fieldsz) <  width -  fieldsz){ goto error;}}
if( Cyc_std__IO_padn( ioputc, ioputc_env,'0', dpad) <  dpad){ goto error;} if(
use_cp2){ do { if( Cyc_std__IO_sputn( ioputc, ioputc_env, cp2, size) !=  size){
goto error;}} while ( 0);} else{ do { if( Cyc_std__IO_sputn( ioputc, ioputc_env,(
struct _tagged_arr) cp, size) !=  size){ goto error;}} while ( 0);} if( Cyc_std__IO_padn(
ioputc, ioputc_env,'0', fpprec) <  fpprec){ goto error;} if( flags &  16){ if(
Cyc_std__IO_padn( ioputc, ioputc_env,' ', width -  fieldsz) <  width -  fieldsz){
goto error;}} ret += width >  fieldsz? width: fieldsz;} done: return ret; error:
return - 1;}} static struct _tagged_arr Cyc_std_exponent( struct _tagged_arr p,
int exp, int fmtch){ struct _tagged_arr t; unsigned char expbuf[ 308u];{
unsigned int _temp125= 308u; unsigned int i; for( i= 0; i <  _temp125; i ++){
expbuf[ i]='0';}}*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& p, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))=( unsigned char) fmtch; if( exp <  0){ exp= - exp;*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& p,
sizeof( unsigned char), 1), sizeof( unsigned char), 0u))='-';} else{*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& p,
sizeof( unsigned char), 1), sizeof( unsigned char), 0u))='+';} t=
_tagged_arr_plus( _tag_arr( expbuf, sizeof( unsigned char), 308u), sizeof(
unsigned char), 308); if( exp >  9){ do {*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus(& t, sizeof( unsigned char),
-1), sizeof( unsigned char), 0u))=( unsigned char)( exp %  10 + '0');} while ((
exp /= 10) >  9);*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus(& t, sizeof( unsigned char), -1), sizeof( unsigned char),
0u))=( unsigned char)( exp + '0'); for( 0; t.curr < ( _tagged_arr_plus( _tag_arr(
expbuf, sizeof( unsigned char), 308u), sizeof( unsigned char), 308)).curr;*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& p,
sizeof( unsigned char), 1), sizeof( unsigned char), 0u))=*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& t, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))){;}} else{*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& p, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))='0';*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& p, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))=( unsigned char)( exp + '0');}
return p;} static struct _tagged_arr Cyc_std_round( double fract, int* exp,
struct _tagged_arr start, struct _tagged_arr end, unsigned char ch, int* signp){
double tmp=( double) 0.0; if( fract !=  0.0){ modf( fract *  10,& tmp);} else{
tmp=( double)( ch - '0');} if( tmp >  4){ for( 0; 1; _tagged_arr_inplace_plus(&
end, sizeof( unsigned char), -1)){ if(*(( unsigned char*)
_check_unknown_subscript( end, sizeof( unsigned char), 0u)) == '.'){
_tagged_arr_inplace_plus(& end, sizeof( unsigned char), -1);} if( ++(*((
unsigned char*) _check_unknown_subscript( end, sizeof( unsigned char), 0u))) <= '9'){
break;}*(( unsigned char*) _check_unknown_subscript( end, sizeof( unsigned char),
0u))='0'; if( end.curr ==  start.curr){ if(( unsigned int) exp){*((
unsigned char*) _check_unknown_subscript( end, sizeof( unsigned char), 0u))='1';
++(*(( int*) _check_null( exp)));} else{*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus(& end, sizeof( unsigned char),
-1), sizeof( unsigned char), 0u))='1'; _tagged_arr_inplace_plus(& start, sizeof(
unsigned char), -1);} break;}}} else{ if(* signp == '-'){ for( 0; 1;
_tagged_arr_inplace_plus(& end, sizeof( unsigned char), -1)){ if(*((
unsigned char*) _check_unknown_subscript( end, sizeof( unsigned char), 0u)) == '.'){
_tagged_arr_inplace_plus(& end, sizeof( unsigned char), -1);} if(*((
unsigned char*) _check_unknown_subscript( end, sizeof( unsigned char), 0u)) != '0'){
break;} if( end.curr ==  start.curr){* signp= 0;}}}} return start;} int Cyc_std___cvt_double(
double number, int prec, int flags, int* signp, int fmtch, struct _tagged_arr
startp, struct _tagged_arr endp){ struct _tagged_arr p; struct _tagged_arr t;
double fract; int dotrim= 0; int expcnt; int gformat= 0; double integer=( double)
0.0; double tmp=( double) 0.0; expcnt= 0; if( number <  0){ number= - number;*
signp=( int)'-';} else{* signp= 0;} fract= modf( number,& integer); t=
_tagged_arr_inplace_plus(& startp, sizeof( unsigned char), 1); for( p=
_tagged_arr_plus( endp, sizeof( unsigned char), - 1); p.curr >=  startp.curr?
integer !=  0.0: 0; ++ expcnt){ tmp= modf( integer /  10,& integer);*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& p,
sizeof( unsigned char), -1), sizeof( unsigned char), 0u))=( unsigned char)(( int)((
tmp +  .01) *  10) + '0');} switch( fmtch){ case 'f': _LL126: goto _LL127; case
'F': _LL127: if( expcnt){ for( 0;( _tagged_arr_inplace_plus(& p, sizeof(
unsigned char), 1)).curr <  endp.curr;*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& t, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))=*(( unsigned char*)
_check_unknown_subscript( p, sizeof( unsigned char), 0u))){;}} else{*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& t,
sizeof( unsigned char), 1), sizeof( unsigned char), 0u))='0';} if( prec? 1:
flags &  8){*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))='.';} if( fract !=  0.0){ if( prec){ do { fract= modf(
fract *  10,& tmp);*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))=( unsigned char)(( int) tmp + '0');} while ( -- prec? fract
!=  0.0: 0);} if( fract !=  0.0){ startp= Cyc_std_round( fract, 0, startp,
_tagged_arr_plus( t, sizeof( unsigned char), - 1),( unsigned char) 0, signp);}}
for( 0; prec --;*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))='0'){;} break; case 'e': _LL128: goto _LL129; case 'E':
_LL129: eformat: if( expcnt){*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))=*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus(& p, sizeof( unsigned char), 1), sizeof( unsigned char),
0u)); if( prec? 1: flags &  8){*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))='.';} for( 0; prec?( _tagged_arr_inplace_plus(& p, sizeof(
unsigned char), 1)).curr <  endp.curr: 0; -- prec){*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& t, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))=*(( unsigned char*)
_check_unknown_subscript( p, sizeof( unsigned char), 0u));} if( ! prec?(
_tagged_arr_inplace_plus(& p, sizeof( unsigned char), 1)).curr <  endp.curr: 0){
fract=( double) 0; startp= Cyc_std_round(( double) 0,( int*)& expcnt, startp,
_tagged_arr_plus( t, sizeof( unsigned char), - 1),*(( unsigned char*)
_check_unknown_subscript( p, sizeof( unsigned char), 0u)), signp);} -- expcnt;}
else{ if( fract !=  0.0){ for( expcnt= - 1; 1; -- expcnt){ fract= modf( fract * 
10,& tmp); if( tmp !=  0.0){ break;}}*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& t, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))=( unsigned char)(( int) tmp + '0');
if( prec? 1: flags &  8){*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))='.';}} else{*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))='0'; if( prec? 1: flags &  8){*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& t, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))='.';}}} if( fract !=  0.0){ if(
prec){ do { fract= modf( fract *  10,& tmp);*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& t, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))=( unsigned char)(( int) tmp + '0');} while (
-- prec? fract !=  0.0: 0);} if( fract !=  0.0){ startp= Cyc_std_round( fract,(
int*)& expcnt, startp, _tagged_arr_plus( t, sizeof( unsigned char), - 1),(
unsigned char) 0, signp);}} for( 0; prec --;*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus_post(& t, sizeof(
unsigned char), 1), sizeof( unsigned char), 0u))='0'){;} if( gformat? !( flags & 
8): 0){ while( t.curr >  startp.curr?*(( unsigned char*)
_check_unknown_subscript( _tagged_arr_inplace_plus(& t, sizeof( unsigned char),
-1), sizeof( unsigned char), 0u)) == '0': 0) {;} if(*(( unsigned char*)
_check_unknown_subscript( t, sizeof( unsigned char), 0u)) == '.'){
_tagged_arr_inplace_plus(& t, sizeof( unsigned char), -1);}
_tagged_arr_inplace_plus(& t, sizeof( unsigned char), 1);} t= Cyc_std_exponent(
t, expcnt, fmtch); break; case 'g': _LL130: goto _LL131; case 'G': _LL131: if( !
prec){ ++ prec;} if( expcnt >  prec? 1:(( ! expcnt? fract !=  0.0: 0)? fract < 
.0001: 0)){ -- prec; fmtch -= 2; gformat= 1; goto eformat;} if( expcnt){ for( 0;(
_tagged_arr_inplace_plus(& p, sizeof( unsigned char), 1)).curr <  endp.curr;(*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& t,
sizeof( unsigned char), 1), sizeof( unsigned char), 0u))=*(( unsigned char*)
_check_unknown_subscript( p, sizeof( unsigned char), 0u)), -- prec)){;}} else{*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& t,
sizeof( unsigned char), 1), sizeof( unsigned char), 0u))='0';} if( prec? 1:
flags &  8){ dotrim= 1;*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))='.';} else{ dotrim= 0;} if( fract !=  0.0){ if( prec){ do {
fract= modf( fract *  10,& tmp);*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))=( unsigned char)(( int) tmp + '0');} while ( tmp ==  0.0? !
expcnt: 0); while( -- prec? fract !=  0.0: 0) { fract= modf( fract *  10,& tmp);*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus_post(& t,
sizeof( unsigned char), 1), sizeof( unsigned char), 0u))=( unsigned char)(( int)
tmp + '0');}} if( fract !=  0.0){ startp= Cyc_std_round( fract, 0, startp,
_tagged_arr_plus( t, sizeof( unsigned char), - 1),( unsigned char) 0, signp);}}
if( flags &  8){ for( 0; prec --;*(( unsigned char*) _check_unknown_subscript(
_tagged_arr_inplace_plus_post(& t, sizeof( unsigned char), 1), sizeof(
unsigned char), 0u))='0'){;}} else{ if( dotrim){ while( t.curr >  startp.curr?*((
unsigned char*) _check_unknown_subscript( _tagged_arr_inplace_plus(& t, sizeof(
unsigned char), -1), sizeof( unsigned char), 0u)) == '0': 0) {;} if(*((
unsigned char*) _check_unknown_subscript( t, sizeof( unsigned char), 0u)) != '.'){
_tagged_arr_inplace_plus(& t, sizeof( unsigned char), 1);}}} break; default:
_LL132:( int) _throw(( void*)({ struct Cyc_Core_Impossible_struct* _temp134=(
struct Cyc_Core_Impossible_struct*) GC_malloc( sizeof( struct Cyc_Core_Impossible_struct));
_temp134[ 0]=({ struct Cyc_Core_Impossible_struct _temp135; _temp135.tag= Cyc_Core_Impossible;
_temp135.f1= _tag_arr("stdio::__cvt_double", sizeof( unsigned char), 20u);
_temp135;}); _temp134;}));} return( t.curr -  startp.curr) /  sizeof(
unsigned char);} int Cyc_std_vfprintf( struct Cyc_std___sFILE* f, struct
_tagged_arr fmt, struct _tagged_arr ap){ int ans; ans=(( int(*)( int(* ioputc)(
int, struct Cyc_std___sFILE*), struct Cyc_std___sFILE* ioputc_env, struct
_tagged_arr fmt0, struct _tagged_arr ap)) Cyc_std__IO_vfprintf)( Cyc_std_putc, f,
fmt, ap); return ans;} int Cyc_std_fprintf( struct Cyc_std___sFILE* f, struct
_tagged_arr fmt, struct _tagged_arr ap){ return Cyc_std_vfprintf( f, fmt, ap);}
int Cyc_std_vprintf( struct _tagged_arr fmt, struct _tagged_arr ap){ int ans;
ans=(( int(*)( int(* ioputc)( int, struct Cyc_std___sFILE*), struct Cyc_std___sFILE*
ioputc_env, struct _tagged_arr fmt0, struct _tagged_arr ap)) Cyc_std__IO_vfprintf)(
Cyc_std_putc, Cyc_std_stdout, fmt, ap); return ans;} int Cyc_std_printf( struct
_tagged_arr fmt, struct _tagged_arr ap){ int ans; ans= Cyc_std_vprintf( fmt, ap);
return ans;} static int Cyc_std_putc_string( int c, struct _tagged_arr* sptr){
struct _tagged_arr s=* sptr; if( _get_arr_size( s, sizeof( unsigned char)) ==  0){
return - 1;}*(( unsigned char*) _check_unknown_subscript( s, sizeof(
unsigned char), 0u))=( unsigned char) c;* sptr= _tagged_arr_plus( s, sizeof(
unsigned char), 1); return 1;} int Cyc_std_vsprintf( struct _tagged_arr s,
struct _tagged_arr fmt, struct _tagged_arr ap){ int ans; struct _tagged_arr
_temp136= s; ans=(( int(*)( int(* ioputc)( int, struct _tagged_arr*), struct
_tagged_arr* ioputc_env, struct _tagged_arr fmt0, struct _tagged_arr ap)) Cyc_std__IO_vfprintf)(
Cyc_std_putc_string,& _temp136, fmt, ap); if( 0 <=  ans){*(( unsigned char*)
_check_unknown_subscript( s, sizeof( unsigned char), ans))='\000';} return ans;}
int Cyc_std_sprintf( struct _tagged_arr s, struct _tagged_arr fmt, struct
_tagged_arr ap){ return Cyc_std_vsprintf( s, fmt, ap);} static int Cyc_std_putc_void(
int c, int dummy){ return 1;} struct _tagged_arr Cyc_std_vraprintf( struct
_RegionHandle* r1, struct _tagged_arr fmt, struct _tagged_arr ap){ int size=((
int(*)( int(* ioputc)( int, int), int ioputc_env, struct _tagged_arr fmt0,
struct _tagged_arr ap)) Cyc_std__IO_vfprintf)( Cyc_std_putc_void, 0, fmt, ap) + 
1; struct _tagged_arr s=({ unsigned int _temp137=( unsigned int) size;
unsigned char* _temp138=( unsigned char*) _region_malloc( r1, sizeof(
unsigned char) *  _temp137); struct _tagged_arr _temp140= _tag_arr( _temp138,
sizeof( unsigned char),( unsigned int) size);{ unsigned int _temp139= _temp137;
unsigned int i; for( i= 0; i <  _temp139; i ++){ _temp138[ i]='\000';}};
_temp140;}); Cyc_std_vsprintf( s, fmt, ap); return s;} struct _tagged_arr Cyc_std_raprintf(
struct _RegionHandle* r1, struct _tagged_arr fmt, struct _tagged_arr ap){ return
Cyc_std_vraprintf( r1, fmt, ap);} struct _tagged_arr Cyc_std_aprintf( struct
_tagged_arr fmt, struct _tagged_arr ap){ return Cyc_std_vraprintf( Cyc_Core_heap_region,
fmt, ap);}