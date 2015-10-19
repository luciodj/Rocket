/* 
 * Tiny Basic Intermediate Language Interpreter 
 * 
 * Copyright 1976 Itty Bitty Computers, used by permission
 * Tom Pittman * rev. 2004 July 19 
 * http://www.ittybittycomputers.com/IttyBitty/TinyBasic/index.htm
 * 
 * Modified for PIC16F1619 on PICDEM Curiosity 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/mcc.h"  

#define ScreenChar(ch)     putch(ch)
#define KeyInChar          (char)getch()
#define NeedsEcho          true
#define BreakTest          (eusartRxCount != 0)

/* Constants: */
#define aByte unsigned char
#define CoreTop 800   /* Core size */
#define UserProg 32   /* Core address of front of Basic program */
#define EndUser 34    /* Core address of end of stack/user space */
#define EndProg 36    /* Core address of end of Basic program */
#define GoStkTop 38   /* Core address of Gosub stack top */
#define LinoCore 40   /* Core address of "Current BASIC line number" */
#define ILPCcore 42   /* Core address of "IL Program Counter" */
#define BPcore 44     /* Core address of "Basic Pointer" */
#define SvPtCore 46   /* Core address of "Saved Pointer" */
#define InLine 48     /* Core address of input line */
#define ExpnStk 128   /* Core address of expression stack (empty) */
#define TabHere 191   /* Core address of output line size, for tabs */
#define WachPoint 255 /* Core address of debug watchpoint USR */

#define ColdGo 256    /* Core address of nominal restart USR */
#define WarmGo 259    /* Core address of nominal warm start USR */
#define InchSub 262   /* Core address of nominal char input USR */
#define OutchSub 265  /* Core address of nominal char output USR */
#define BreakSub 268  /* Core address of nominal break test USR */
#define DumpSub 273   /* Core address of debug core dump USR */
#define PeekSub 276   /* Core address of nominal byte peek USR */
#define Peek2Sub 277  /* Core address of nominal 2-byte peek USR */
#define PokeSub 280   /* Core address of nominal byte poke USR */
#define TrLogSub 283  /* Core address of debug trace log USR */
#define BScode 271    /* Core address of backspace code */
#define CanCode 272   /* Core address of line cancel code */
#define ILfront 286   /* Core address of IL code address */
#define BadOp 15      /* illegal op, default IL code */
/* Pascal habits die hard.. */
#define true 1
#define false 0

/* debugging stuff... */
#define DEBUGON 1         /* 1 enables \t Debugging toggle, 0 disables */
#define LOGSIZE 8                                   /* how much to log */
static int Debugging = 0;                     /* >0 enables debug code */
int DebugLog[LOGSIZE];                 /* quietly logs recent activity */
int LogHere = 0;                          /* current index in DebugLog */
int Watcher = 0, Watchee;                         /* memory watchpoint */

const char code[];                         // interpreter IL code in ROM

/* Static/global data: */
aByte Core[CoreTop];                           /* BASIC program + var */
int Lino, ILPC;                 /* current line #, IL program counter */
int BP, SvPt;                      /* current, saved TB parse pointer */
int SubStk, ExpnTop;                                /* stack pointers */
int InLend, SrcEnd;             /* current input line & TB source end */
int UserEnd;
int ILend, XQhere;           /* end of IL code, start of execute loop */
int Broken = false;             /* =true to stop execution or listing */

/************************* Memory Utilities.. *************************/

void Poke2(int loc, int valu) { /* store integer as two bytes */
    Core[loc] = (aByte) ((valu >> 8)&255); /* nominally Big-Endian */
    Core[loc + 1] = (aByte) (valu & 255);
}

int Peek2(int loc) { /* fetch integer from two bytes */
    return ((int) Core[loc])*256 + ((int) Core[loc + 1]);
}

int lit16(int loc) { // fetch a 16-bit literal from code
    return (((int) code[loc] << 8) + ((int) code[loc + 1]));
}

/************************** I/O Utilities... **************************/

void Ouch(char ch) {                        /* output char to stdout */
    if (ch == '\r') {
        Core[TabHere] = 0; /* keep count of how long this line is */
        ScreenChar('\n');
    } 
    else if (ch >= ' ') if (ch <= '~') { /* ignore non-print control chars */
        Core[TabHere]++;
        ScreenChar(ch);
    }
} /* ~Ouch */

char Inch(void) {           /* read input character from stdin or file */
    char ch;
    ch = KeyInChar;                           /* get input from stdin */
    if (NeedsEcho) ScreenChar(ch); /* alternative input may need this */
    return ch;
} 

int StopIt(void) {
    return BreakTest;
} 

void OutStr(const char* theMsg) { /* output a string to the console */
    while (*theMsg != '\0') Ouch(*theMsg++);
} 

void OutLn(void) { /* terminate output line to the console */
    Ouch('\r');
}

void OutInt(int theNum) { /* output a number to the console */
    int digit = 10000;
    if (theNum < 0) {
        Ouch('-');
        theNum = -theNum;
    }
    while (theNum < digit) digit /= 10;
    while (digit >= 1) {
        Ouch((char) (theNum / digit + '0'));
        theNum %= digit;
        digit /= 10;
    }

}

/*********************** Debugging Utilities... ***********************/
void OutHexDigit(char num) {
    num &= 0xf;
    if (num > 9) Ouch(num + '7');
    else Ouch(num + '0');
}

void OutHex(int num, int nd) { /* output a hex number to the console */
    while (nd-- > 0) {
        OutHexDigit((char) (num >> (nd * 4)));
    }
}

void ShowSubs(void) { /* display subroutine stack for debugging */
    int ix;
    OutLn();
    OutStr(" [Stk ");
    OutHex(SubStk, 5);
    for (ix = SubStk; ix < UserEnd; ix++) {
        OutStr(" ");
        OutInt(Peek2(ix++));
    }
    OutStr("]");
} /* ~ShowSubs */

void ShowExSt(void) { /* display expression stack for debugging */
    int ix;
    OutLn();
    OutStr(" [Exp ");
    OutHex(ExpnTop, 3);
    if ((ExpnTop & 1) == 0) for (ix = ExpnTop; ix < ExpnStk; ix++) {
            OutStr(" ");
            OutInt((int) ((short) Peek2(ix++)));
        } else for (ix = ExpnTop; ix < ExpnStk; ix++) {
            OutStr(".");
            OutInt((int) Core[ix]);
        }
    OutStr("]");
} /* ~ShowExSt */

void ShowVars(int whom) { /* display vars for debugging */
    int ix, valu = 1, prior = 1;
    if (whom == 0) whom = 26;
    else {
        whom = (whom >> 1)&31; /* whom is a specified var, or 0 */
        valu = whom;
    }
    OutLn();
    OutStr("  [Vars");
    for (ix = valu; ix <= whom; ix++) { /* all non-zero vars, or else whom */
        valu = (int) ((short) Peek2(ix * 2 + ExpnStk));
        if (valu == 0) if (prior == 0) continue; /* omit multiple 0s */
        prior = valu;
        OutStr(" ");
        Ouch((char) (ix + 64)); /* show var name */
        OutStr("=");
        OutInt(valu);
    }
    OutStr("]");
} /* ~ShowVars */

void ShoMemDump(int here, int nlocs) { /* display hex memory dump */
    int temp, thar = here&-16;
    while (nlocs > 0) {
        temp = thar;
        OutLn();
        OutHex(here, 4);
        OutStr(": ");
        while (thar < here) {
            OutStr("   ");
            thar++;
        }
        do {
            OutStr(" ");
            if (nlocs-- > 0) OutHex(Core[here], 2);
            else OutStr("  ");
        } while (++here % 16 != 0);
        OutStr("  ");
        while (temp < thar) {
            OutStr(" ");
            temp++;
        }
        while (thar < here) {
            if (nlocs < 0) if ((thar & 15) >= nlocs + 16) break;
            temp = Core[thar++];
            if (temp == (int) '\r') Ouch('\\');
            else if (temp < 32) Ouch('`');
            else if (temp > 126) Ouch('~');
            else Ouch((char) temp);
        }
    }
    OutLn();
} /* ~ShoMemDump */

void ShoLogVal(int item) { /* format & output one activity log item */
    int valu = DebugLog[item];
    OutLn();
    //  if (valu < -65536) {                         /* store to a variable */
    //    Ouch((char)(((valu>>17)&31)+64));
    //    OutStr("=");
    //    OutInt((valu&0x7FFF)-(valu&0x8000));}
    //  else if (valu < -32768) {                                /* error # */
    //    OutStr("Err ");
    //    OutInt(-valu-32768);}
    //  else 
    if (valu < 0) { /* only logs IL sequence changes */
        OutStr("  IL+");
        OutHex(-Peek2(ILfront) - valu, 3);
    }        //  else if (valu<65536) {                          /* TinyBasic line # */
        //    OutStr("#");
        //    OutInt(valu);}
    else { /* poke memory byte */
        OutStr("!");
        OutHex(valu, 4);
        OutStr("=");
        //        OutInt(valu>>16);
    }
} /* ~ShoLogVal */

void ShowLog(void) { /* display activity log for debugging */
    int ix;
    OutLn();
    OutStr("*** Activity Log @ ");
    OutInt(LogHere);
    OutStr(" ***");
    if (LogHere >= LOGSIZE) /* circular, show only last 4K activities */
        for (ix = (LogHere & (LOGSIZE - 1)); ix < LOGSIZE; ix++) ShoLogVal(ix);
    for (ix = 0; ix < (LogHere & (LOGSIZE - 1)); ix++) ShoLogVal(ix);
    OutLn();
    OutStr("*****");
    OutLn();
} /* ~ShowLog */

void LogIt(int valu) { /* insert this valu into activity log */
    DebugLog[(LogHere++)&(LOGSIZE - 1)] = valu;
}

/************************ Utility functions... ************************/

void WarmStart(void) { /* initialize existing program */
    UserEnd = Peek2(EndUser);
    SubStk = UserEnd; /* empty subroutine, expression stacks */
    Poke2(GoStkTop, SubStk);
    ExpnTop = ExpnStk;
    Lino = 0; /* not in any line */
    ILPC = 0; /* start IL at front */
    SvPt = InLine;
    BP = InLine;
    Core[BP] = 0;
    Core[TabHere] = 0;
    InLend = InLine;
} /* ~WarmStart */

void ColdStart(void) { /* initialize program to empty */
    //  if (Peek2(ILfront) != ILfront+2) ILend = Peek2(ILfront)+0x800;
    //  Poke2(UserProg,(ILend+255)&-256);   /* start Basic shortly after IL */
    //  if (CoreTop>65535) {
    //    Poke2(EndUser,65534);
    //    Poke2(65534,0xDEAD);}
    //  else 
    Poke2(UserProg, ILend+1);
    Poke2(EndUser, CoreTop);
    WarmStart();
    SrcEnd = Peek2(UserProg);
    Poke2(SrcEnd++, 0);
    Poke2(EndProg, ++SrcEnd);
} /* ~ColdStart */

void TBerror(void) { /* report interpreter error */
    if (ILPC == 0) return; /* already reported it */
    OutLn();
    LogIt(-ILPC - 32768);
    OutStr("Tiny Basic error #"); /* IL address is the error # */
    OutInt(ILPC - Peek2(ILfront));
    if (Lino > 0) { /* Lino=0 if in command line */
        OutStr(" at line ");
        OutInt(Lino);
    }
    OutLn();
    if (Debugging > 0) { /* some extra info if debugging.. */
        ShowSubs();
        ShowExSt();
        ShowVars(0);
        OutStr(" [BP=");
        OutHex(BP, 4);
        OutStr(", TB@");
        OutHex(Peek2(UserProg), 4);
        OutStr(", IL@");
        OutHex(Peek2(ILfront), 4);
        OutStr("]");
        ShoMemDump((BP - 30)&-16, 64);
    }
    Lino = 0; /* restart interpreter at front */
    ExpnTop = ExpnStk; /* with empty expression stack */
    ILPC = 0; /* cheap error test; interp reloads it from ILfront */
    BP = InLine;
} /* ~TBerror */

void PushSub(int valu) { /* push value onto Gosub stack */
    if (SubStk <= SrcEnd) TBerror(); /* overflow: bumped into program end */
    else {
        SubStk = SubStk - 2;
        Poke2(GoStkTop, SubStk);
        Poke2(SubStk, valu);
    }
    if (Debugging > 0) ShowSubs();
} /* ~PushSub */

int PopSub(void) { /* pop value off Gosub stack */
    if (SubStk >= Peek2(EndUser) - 1) { /* underflow (nothing in stack).. */
        TBerror();
        return -1;
    } else {
        if (Debugging > 1) ShowSubs();
        SubStk = SubStk + 2;
        Poke2(GoStkTop, SubStk);
        return Peek2(SubStk - 2);
    }
} /* ~PopSub */

void PushExBy(int valu) { /* push byte onto expression stack */
    if (ExpnTop <= InLend) TBerror(); /* overflow: bumped into input line */
    else Core[--ExpnTop] = (aByte) (valu & 255);
    if (Debugging > 0) ShowExSt();
} /* ~PushExBy */

int PopExBy(void) { /* pop byte off expression stack */
    if (ExpnTop < ExpnStk) return (int) Core[ExpnTop++];
    TBerror(); /* underflow (nothing in stack) */
    return -1;
} /* ~PopExBy */

void PushExInt(int valu) { /* push integer onto expression stack */
    ExpnTop = ExpnTop - 2;
    if (ExpnTop < InLend) TBerror(); /* overflow: bumped into input line */
    else Poke2(ExpnTop, valu);
    //  if (Debugging>0) ShowExSt();
} /* ~PushExInt */

int PopExInt(void) { /* pop integer off expression stack */
    if (++ExpnTop < ExpnStk) return (int) ((short) Peek2((ExpnTop++) - 1));
    TBerror(); /* underflow (nothing in stack) */
    return -1;
} /* ~PopExInt */

aByte DeCaps(aByte ch) { // capitalize ASCII char
    if ((ch >= 'a') && (ch <= 'z')) ch -= ('a' - 'A');
    else if (ch == 9) ch = ' ';
    else if (ch == '\n') ch = 'r';
    return ch;
}

int DeHex(const char* txt, int ndigs) { /* decode hex -> int */
    int num = 0;
    char ch = ' ';
    while (ch < '0') /* first skip to num... */
        if (ch == '\0') return -1;
        else ch = DeCaps(((int) *txt++)&127);
    if (ch > 'F' || ch > '9' && ch < 'A') return -1; /* not hex */
    while ((ndigs--) > 0) { /* only get requested digits */
        if (ch < '0' || ch > 'F') return num; /* not a hex digit */
        if (ch >= 'A')
            num = num * 16 - 55 + ((int) ch); /* A-F */
        else if (ch <= '9') num = num * 16 - 48 + ((int) ch); /* 0-9 */
        else return num; /* something in between, i.e. not hex */
        ch = DeCaps(((int) *txt++)&127);
    }
    return num;
} /* ~DeHex */

int SkipTo(int here, char fch) { /* search for'd past next marker */
    while (true) {
        char ch = (char) Core[here++]; /* look at next char */
        if (ch == fch) break; /* got it */
        if (ch == '\0') {
            --here;
            break;
        }
    }
    return here;
} /* ~SkipTo */

int FindLine(int theLine) { /* find theLine in TB source code */
    int ix;
    int here = Peek2(UserProg); /* start at front */
    while (true) {
        ix = Peek2(here++);
        if (theLine <= ix || ix == 0) {
            --here; /* found it or overshot */
            break;
        }
        here = SkipTo(++here, '\r');
    } /* skip to end of this line */
    return here;
} /* ~FindLine */

void GoToLino(void) { /* find line # Lino and set BP to its front */
    int here;
    if (Lino <= 0) { /* Lino=0 is just command line (OK).. */
        BP = InLine;
        if (DEBUGON > 0) LogIt(0);
        return;
    }
    if (DEBUGON > 0) LogIt(Lino);
    if (Debugging > 0) {
        OutStr(" [#");
        OutInt(Lino);
        OutStr("]");
    }
    BP = FindLine(Lino); /* otherwise try to find it.. */
    here = Peek2(BP++);
    if (here == 0) TBerror(); /* ran off the end, error off */
    else if (Lino != here) TBerror(); /* not there */
    else BP++;
} /* ~GoToLino */ /* got it */

void ListIt(int frm, int too) { /* list the stored program */
    char ch;
    int here;
    if (frm == 0) { /* 0,0 defaults to all; n,0 defaults to n,n */
        too = 32767;
        frm = 1;
    } else if (too == 0) too = frm;
    here = FindLine(frm); /* try to find first line.. */
    while (!StopIt()) {
        frm = Peek2(here++); /* get this line's # to print it */
        if (frm > too || frm == 0) break;
        here++;
        OutInt(frm);
        Ouch(' ');
        do { /* print the text */
            ch = (char) Core[here++];
            Ouch(ch);
        } while (ch > '\r');
    }
} /* ~ListIt */

void ConvtIL(void) { /* convert & load TBIL code */
    ILend = 0x158;
    Poke2(ILfront, 1); 
    Poke2(ColdGo + 1, ILend); // ???
    XQhere = 0;                 /* requires new XQ to initialize */
    //    Core[ILend] = 0; ???
} /* ~ConvtIL */

void LineSwap(int here) { /* swap SvPt/BP if here is not in InLine  */
    if (here < InLine || here >= InLend) {
        here = SvPt;
        SvPt = BP;
        BP = here;
    } else SvPt = BP;
} /* ~LineSwap */

/************************** Main Interpreter **************************/

void Interp(void) {
    char ch; /* comments from TinyBasic Experimenter's Kit, pp.15-21 */
    int op, ix, here, chpt; /* temps */
    Broken = false; /* initialize this for possible later test */
    while (true) {
        if (StopIt()) {
//            Inch();  // consume input
            OutLn();
            OutStr("*** User Break ***");
            TBerror();
        }
        if (ILPC == 0) {
            ILPC = Peek2(ILfront);
            if (DEBUGON > 0) LogIt(-ILPC);
            if (Debugging > 0) {
                OutLn();
                OutStr("[IL=");
                OutHex(ILPC, 4);
                OutStr("]");
            }
        }
        if (DEBUGON > 0) if (Watcher > 0) { /* check watchpoint.. */
                if (((Watchee < 0) && (Watchee + 256 + (int) Core[Watcher]) != 0)
                        || ((Watchee >= 0) && (Watchee == (int) Core[Watcher]))) {
                    OutLn();
                    OutStr("*** Watched ");
                    OutHex(Watcher, 4);
                    OutStr(" = ");
                    OutInt((int) Core[Watcher]);
                    OutStr(" *** ");
                    Watcher = 0;
                    TBerror();
                    continue;
                }
            }
        op = (int) code[ILPC++];
        if (Debugging > 0) {
            OutLn();
            OutStr("[IL+");
            OutHex(ILPC - Peek2(ILfront) - 1, 3);
            OutStr("=");
            OutHex(op, 2);
            OutStr("]");
        }
        switch (op >> 5) {
            default: switch (op) {
                    case 15:
                        TBerror();
                        return;

                        /* SX n    00-07   Stack Exchange. */
                        /*                 Exchange the top byte of computational stack with  */
                        /* that "n" bytes into the stack. The top/left byte of the stack is   */
                        /* considered to be byte 0, so SX 0 does nothing.                     */
                    case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                        if (ExpnTop + op >= ExpnStk) { /* swap is below stack depth */
                            TBerror();
                            return;
                        }
                        ix = (int) Core[ExpnTop];
                        Core[ExpnTop] = Core[ExpnTop + op];
                        Core[ExpnTop + op] = (aByte) ix;
                        if (Debugging > 0) ShowExSt();
                        break;

                        /* LB n    09nn    Push Literal Byte onto Stack.                      */
                        /*                 This adds one byte to the expression stack, which  */
                        /* is the second byte of the instruction. An error stop will occur if */
                        /* the stack overflows. */
                    case 9:
                        PushExBy((int) code[ILPC++]); /* push IL byte */
                        break;

                        /* LN n    0Annnn  Push Literal Number.                               */
                        /*                 This adds the following two bytes to the           */
                        /* computational stack, as a 16-bit number. Stack overflow results in */
                        /* an error stop. Numbers are assumed to be Big-Endian.               */
                    case 10:
                        PushExInt(lit16(ILPC++)); /* get next 2 IL bytes */
                        ILPC++;
                        break;

                        /* DS      0B      Duplicate Top Number (two bytes) on Stack.         */
                        /*                 An error stop will occur if there are less than 2  */
                        /* bytes (1 int) on the expression stack or if the stack overflows.   */
                    case 11:
                        op = ExpnTop;
                        ix = PopExInt();
                        if (ILPC == 0) break; /* underflow */
                        ExpnTop = op;
                        PushExInt(ix);
                        break;

                        /* SP      0C      Stack Pop.                                         */
                        /*                 The top two bytes are removed from the expression  */
                        /* stack and discarded. Underflow results in an error stop.           */
                    case 12:
                        ix = PopExInt();
                        if (Debugging > 0) ShowExSt();
                        break;

                        /* SB      10      Save BASIC Pointer.                                */
                        /*                 If BASIC pointer is pointing into the input line   */
                        /* buffer, it is copied to the Saved Pointer; otherwise the two       */
                        /* pointers are exchanged.                                            */
                    case 16:
                        LineSwap(BP);
                        break;

                        /* RB      11      Restore BASIC Pointer.                             */
                        /*                 If the Saved Pointer points into the input line    */
                        /* buffer, it is replaced by the value in the BASIC pointer;          */
                        /* otherwise the two pointers are exchanged.                          */
                    case 17:
                        LineSwap(SvPt);
                        break;

                        /* FV      12      Fetch Variable.                                    */
                        /*                 The top byte of the computational stack is used to */
                        /* index into Page 00. It is replaced by the two bytes fetched. Error */
                        /* stops occur with stack overflow or underflow.                      */
                    case 18:
                        op = PopExBy();
                        if (ILPC != 0) PushExInt(Peek2(op));
                        //          if (Debugging>1) ShowVars(op);
                        break;

                        /* SV      13      Store Variable.                                    */
                        /*                 The top two bytes of the computational stack are   */
                        /* stored into memory at the Page 00 address specified by the third   */
                        /* byte on the stack. All three bytes are deleted from the stack.     */
                        /* Underflow results in an error stop.                                */
                    case 19:
                        ix = PopExInt();
                        op = PopExBy();
                        if (ILPC == 0) break;
                        Poke2(op, ix);
                        //          if (DEBUGON>0) LogIt((ix&0xFFFF)+((op-256)<<16));
                        //          if (Debugging>0) {ShowVars(op); if (Debugging>1) ShowExSt();}
                        break;

                        /* GS      14      GOSUB Save.                                        */
                        /*                 The current BASIC line number is pushed            */
                        /* onto the BASIC region of the control stack. It is essential that   */
                        /* the IL stack be empty for this to work properly but no check is    */
                        /* made for that condition. An error stop occurs on stack overflow.   */
                    case 20:
                        PushSub(Lino); /* push line # (possibly =0) */
                        break;

                        /* RS      15      Restore Saved Line.                                */
                        /*                 Pop the top two bytes off the BASIC region of the  */
                        /* control stack, making them the current line number. Set the BASIC  */
                        /* pointer at the beginning of that line. Note that this is the line  */
                        /* containing the GOSUB which caused the line number to be saved. As  */
                        /* with the GS opcode, it is essential that the IL region of the      */
                        /* control stack be empty. If the line number popped off the stack    */
                        /* does not correspond to a line in the BASIC program an error stop   */
                        /* occurs. An error stop also results from stack underflow.           */
                    case 21:
                        Lino = PopSub(); /* get line # (possibly =0) from pop */
                        if (ILPC != 0) GoToLino(); /* stops run if error */
                        break;

                        /* GO      16      GOTO.                                              */
                        /*                 Make current the BASIC line whose line number is   */
                        /* equal to the value of the top two bytes in the expression stack.   */
                        /* That is, the top two bytes are popped off the computational stack, */
                        /* and the BASIC program is searched until a matching line number is  */
                        /* found. The BASIC pointer is then positioned at the beginning of    */
                        /* that line and the RUN mode flag is turned on. Stack underflow and  */
                        /* non-existent BASIC line result in error stops.                     */
                    case 22:
                        ILPC = XQhere; /* the IL assumes an implied NX */
                        if (DEBUGON > 0) LogIt(-ILPC);
                        Lino = PopExInt();
                        if (ILPC != 0) GoToLino(); /* stops run if error */
                        break;

                        /* NE      17      Negate (two's complement).                         */
                        /*                 The number in the top two bytes of the expression  */
                        /* stack is replaced with its negative.                               */
                    case 23:
                        ix = PopExInt();
                        if (ILPC != 0) PushExInt(-ix);
                        break;

                        /* AD      18      Add.                                               */
                        /*                 Add the two numbers represented by the top four    */
                        /* bytes of the expression stack, and replace them with the two-byte  */
                        /* sum. Stack underflow results in an error stop.                     */
                    case 24:
                        ix = PopExInt();
                        op = PopExInt();
                        if (ILPC != 0) PushExInt(op + ix);
                        break;

                        /* SU      19      Subtract.                                          */
                        /*                 Subtract the two-byte number on the top of the     */
                        /* expression stack from the next two bytes and replace the 4 bytes   */
                        /* with the two-byte difference.                                      */
                    case 25:
                        ix = PopExInt();
                        op = PopExInt();
                        if (ILPC != 0) PushExInt(op - ix);
                        break;

                        /* MP      1A      Multiply.                                          */
                        /*                 Multiply the two numbers represented by the top 4  */
                        /* bytes of the computational stack, and replace them with the least  */
                        /* significant 16 bits of the product. Stack underflow is possible.   */
                    case 26:
                        ix = PopExInt();
                        op = PopExInt();
                        if (ILPC != 0) PushExInt(op * ix);
                        break;

                        /* DV      1B      Divide.                                            */
                        /*                 Divide the number represented by the top two bytes */
                        /* of the computational stack into that represented by the next two.  */
                        /* Replace the 4 bytes with the quotient and discard the remainder.   */
                        /* This is a signed (two's complement) integer divide, resulting in a */
                        /* signed integer quotient. Stack underflow or attempted division by  */
                        /* zero result in an error stop. */
                    case 27:
                        ix = PopExInt();
                        op = PopExInt();
                        if (ix == 0) TBerror(); /* divide by 0.. */
                        else if (ILPC != 0) PushExInt(op / ix);
                        break;

                        /* CP      1C      Compare.                                           */
                        /*                 The number in the top two bytes of the expression  */
                        /* stack is compared to (subtracted from) the number in the 4th and   */
                        /* fifth bytes of the stack, and the result is determined to be       */
                        /* Greater, Equal, or Less. The low three bits of the third byte mask */
                        /* a conditional skip in the IL program to test these conditions; if  */
                        /* the result corresponds to a one bit, the next byte of the IL code  */
                        /* is skipped and not executed. The three bits correspond to the      */
                        /* conditions as follows:                                             */
                        /*         bit 0   Result is Less                                     */
                        /*         bit 1   Result is Equal                                    */
                        /*         bit 2   Result is Greater                                  */
                        /* Whether the skip is taken or not, all five bytes are deleted from  */
                        /* the stack. This is a signed (two's complement) comparison so that  */
                        /* any positive number is greater than any negative number. Multiple  */
                        /* conditions, such as greater-than-or-equal or unequal (i.e.greater- */
                        /* than-or-less-than), may be tested by forming the condition mask    */
                        /* byte of the sum of the respective bits. In particular, a mask byte */
                        /* of 7 will force an unconditional skip and a mask byte of 0 will    */
                        /* force no skip. The other 5 bits of the control byte are ignored.   */
                        /* Stack underflow results in an error stop.                          */
                    case 28:
                        ix = PopExInt();
                        op = PopExBy();
                        ix = PopExInt() - ix; /* <0 or =0 or >0 */
                        if (ILPC == 0) return; /* underflow.. */
                        if (ix < 0) ix = 1;
                        else if (ix > 0) ix = 4; /* choose the bit to test */
                        else ix = 2;
                        if ((ix & op) > 0) ILPC++; /* skip next IL op if bit =1 */
                        if (Debugging > 0) ShowExSt();
                        break;

                        /* NX      1D      Next BASIC Statement.                              */
                        /*                 Advance to next line in the BASIC program, if in   */
                        /* RUN mode, or restart the IL program if in the command mode. The    */
                        /* remainder of the current line is ignored. In the Run mode if there */
                        /* is another line it becomes current with the pointer positioned at  */
                        /* its beginning. At this time, if the Break condition returns true,  */
                        /* execution is aborted and the IL program is restarted after         */
                        /* printing an error message. Otherwise IL execution proceeds from    */
                        /* the saved IL address (see the XQ instruction). If there are no     */
                        /* more BASIC statements in the program an error stop occurs.         */
                    case 29:
                        if (Lino == 0) ILPC = 0;
                        else {
                            BP = SkipTo(BP, '\r'); /* skip to end of this line */
                            Lino = Peek2(BP++); /* get line # */
                            if (Lino == 0) { /* ran off the end */
                                TBerror();
                                break;
                            } else BP++;
                            ILPC = XQhere; /* restart at saved IL address (XQ) */
                            if (DEBUGON > 0) LogIt(-ILPC);
                        }
                        if (DEBUGON > 0) LogIt(Lino);
                        if (Debugging > 0) {
                            OutStr(" [#");
                            OutInt(Lino);
                            OutStr("]");
                        }
                        break;

                        /* LS      1F      List The Program.                                  */
                        /*                 The expression stack is assumed to have two 2-byte */
                        /* numbers. The top number is the line number of the last line to be  */
                        /* listed, and the next is the line number of the first line to be    */
                        /* listed. If the specified line numbers do not exist in the program, */
                        /* the next available line (i.e. with the next higher line number) is */
                        /* assumed instead in each case. If the last line to be listed comes  */
                        /* before the first, no lines are listed. If Break condition comes    */
                        /* true during a List operation, the remainder of the listing is      */
                        /* aborted. Zero is not a valid line number, and an error stop occurs */
                        /* if either line number specification is zero. The line number       */
                        /* specifications are deleted from the stack.                         */
                    case 31:
                        op = 0;
                        ix = 0; /* The IL seems to assume we can handle zero */
                        while (ExpnTop < ExpnStk) { /* or more numbers, so get them.. */
                            op = ix;
                            ix = PopExInt();
                        } /* get final line #, then initial.. */
                        if (op < 0 || ix < 0) TBerror();
                        else ListIt(ix, op);
                        break;

                        /* PN      20      Print Number.                                      */
                        /*                 The number represented by the top two bytes of the */
                        /* expression stack is printed in decimal with leading zero           */
                        /* suppression. If it is negative, it is preceded by a minus sign     */
                        /* and the magnitude is printed. Stack underflow is possible.         */
                    case 32:
                        ix = PopExInt();
                        if (ILPC != 0) OutInt(ix);
                        break;

                        /* PQ      21      Print BASIC String.                                */
                        /*                 The ASCII characters beginning with the current    */
                        /* position of BASIC pointer are printed on the console. The string   */
                        /* to be printed is terminated by quotation mark ("), and the BASIC   */
                        /* pointer is left at the character following the terminal quote. An  */
                        /* error stop occurs if a carriage return is imbedded in the string.  */
                    case 33:
                        while (true) {
                            ch = (char) Core[BP++];
                            if (ch == '\"') break; /* done on final quote */
                            if (ch < ' ') { /* error if return or other control char */
                                TBerror();
                                break;
                            }
                            Ouch(ch);
                        } /* print it */
                        break;

                        /* PT      22      Print Tab.                                         */
                        /*                 Print one or more spaces on the console, ending at */
                        /* the next multiple of eight character positions (from the left      */
                        /* margin).                                                           */
                    case 34:
                        do {
                            Ouch(' ');
                        } while (Core[TabHere] % 8 > 0);
                        break;

                        /* NL      23      New Line.                                          */
                        /*                 Output a carriage-return-linefeed sequence to the  */
                        /* console.                                                           */
                    case 35:
                        Ouch('\r');
                        break;

                        /* PC "xxxx"  24xxxxxxXx   Print Literal String.                      */
                        /*                         The ASCII string follows opcode and its    */
                        /* last byte has the most significant bit set to one.                 */
                    case 36:
                        do {
                            ix = (int) code[ILPC++];
                            Ouch((char) (ix & 127)); /* strip high bit for output */
                        } while ((ix & 128) == 0);
                        break;

                        /* GL      27      Get Input Line.                                    */
                        /*                 ASCII characters are accepted from console input   */
                        /* to fill the line buffer. If the line length exceeds the available  */
                        /* space, the excess characters are ignored and bell characters are   */
                        /* output. The line is terminated by a carriage return. On completing */
                        /* one line of input, the BASIC pointer is set to point to the first  */
                        /* character in the input line buffer, and a carriage-return-linefeed */
                        /* sequence is [not] output.                                          */
                    case 39:
                        InLend = InLine;
                        while (true) { /* read input line characters... */
                            ch = Inch();
                            if (ch == '\r') break; /* end of the line */
                            else if (ch == '\t') {
                                Debugging = (Debugging + DEBUGON)&1; /* maybe toggle debug */
                                ch = ' ';
                            }/* convert tabs to space */
                            else if (ch == (char) Core[BScode]) { /* backspace code */
                                if (InLend > InLine) {
                                    InLend--;           /* assume console already */
                                }
                                else { /* backing up over front of line: just kill it.. */
                                    Ouch('\n');
                                    break;
                                }
                            } else if (ch == (char) Core[CanCode]) { /* cancel this line */
                                InLend = InLine;
                                Ouch('\n'); /* also start a new input line */
                                break;
                            } else if (ch < ' ') continue; /* ignore non-ASCII & controls */
                            else if (ch > '~') continue;
                            if (InLend > ExpnTop - 2) continue; /* discard overrun chars */
                            Core[InLend++] = (aByte) ch;
                        } /* insert this char in buffer */
                        while (InLend > InLine && Core[InLend - 1] == ' ')
                            InLend--; /* delete excess trailing spaces */
                        Core[InLend++] = (aByte) '\r'; /* insert final return & null */
                        Core[InLend] = 0;
                        BP = InLine;
                        break;

                        /* IL      2A      Insert BASIC Line.                                 */
                        /*                 Beginning with the current position of the BASIC   */
                        /* pointer and continuing to the [end of it], the line is inserted    */
                        /* into the BASIC program space; for a line number, the top two bytes */
                        /* of the expression stack are used. If this number matches a line    */
                        /* already in the program it is deleted and the new one replaces it.  */
                        /* If the new line consists of only a carriage return, it is not      */
                        /* inserted, though any previous line with the same number will have  */
                        /* been deleted. The lines are maintained in the program space sorted */
                        /* by line number. If the new line to be inserted is a different size */
                        /* than the old line being replaced, the remainder of the program is  */
                        /* shifted over to make room or to close up the gap as necessary. If  */
                        /* there is insufficient memory to fit in the new line, the program   */
                        /* space is unchanged and an error stop occurs (with the IL address   */
                        /* decremented). A normal error stop occurs on expression stack       */
                        /* underflow or if the number is zero, which is not a valid line      */
                        /* number. After completing the insertion, the IL program is          */
                        /* restarted in the command mode.                                     */
                    case 42:
                        Lino = PopExInt(); /* get line # */
                        if (Lino <= 0) { /* don't insert line #0 or negative */
                            if (ILPC != 0) TBerror();
                            else return;
                            break;
                        }
                        while (((char) Core[BP]) == ' ') BP++; /* skip leading spaces */
                        if (((char) Core[BP]) == '\r') ix = 0; /* nothing to add */
                        else ix = InLend - BP + 2; /* the size of the insertion */
                        op = 0; /* this will be the number of bytes to delete */
                        chpt = FindLine(Lino); /* try to find this line.. */
                        if (Peek2(chpt) == Lino) /* there is a line to delete.. */
                            op = (SkipTo(chpt + 2, '\r') - chpt);
                        if (ix == 0) if (op == 0) { /* nothing to add nor delete; done */
                                Lino = 0;
                                break;
                            }
                        op = ix - op; /* = how many more bytes to add or (-)delete */
                        if (SrcEnd + op >= SubStk) { /* too big.. */
                            TBerror();
                            break;
                        }
                        SrcEnd = SrcEnd + op; /* new size */
                        if (op > 0) for (here = SrcEnd; (here--) > chpt + ix;)
                                Core[here] = Core[here - op]; /* shift backend over to right */
                        else if (op < 0) for (here = chpt + ix; here < SrcEnd; here++)
                                Core[here] = Core[here - op]; /* shift it left to close gap */
                        if (ix > 0) Poke2(chpt++, Lino); /* insert the new line # */
                        while (ix > 2) { /* insert the new line.. */
                            Core[++chpt] = Core[BP++];
                            ix--;
                        }
                        Poke2(EndProg, SrcEnd);
                        ILPC = 0;
                        Lino = 0;
                        if (Debugging > 0) ListIt(0, 0);
                        break;

                        /* MT      2B      Mark the BASIC program space Empty.                */
                        /*                 Also clears the BASIC region of the control stack  */
                        /* and restart the IL program in the command mode. The memory bounds  */
                        /* and stack pointers are reset by this instruction to signify empty  */
                        /* program space, and the line number of the first line is set to 0,  */
                        /* which is the indication of the end of the program.                 */
                    case 43:
                        ColdStart();
                        //          if (Debugging>0) {ShowSubs(); ShowExSt(); ShowVars(0);}
                        break;

                        /* XQ      2C      Execute.                                           */
                        /*                 Turns on RUN mode. This instruction also saves     */
                        /* the current value of the IL program counter for use of the NX      */
                        /* instruction, and sets the BASIC pointer to the beginning of the    */
                        /* BASIC program space. An error stop occurs if there is no BASIC     */
                        /* program. This instruction must be executed at least once before    */
                        /* the first execution of a NX instruction.                           */
                    case 44:
                        XQhere = ILPC;
                        BP = Peek2(UserProg);
                        Lino = Peek2(BP++);
                        BP++;
                        if (Lino == 0) TBerror();
                        else if (Debugging > 0) {
                            OutStr(" [#");
                            OutInt(Lino);
                            OutStr("]");
                        }
                        break;

                        /* WS      2D      Stop.                                              */
                        /*                 Stop execution and restart the IL program in the   */
                        /* command mode. The entire control stack (including BASIC region)    */
                        /* is also vacated by this instruction. This instruction effectively  */
                        /* jumps to the Warm Start entry of the ML interpreter.               */
                    case 45:
                        WarmStart();
                        if (Debugging > 0) ShowSubs();
                        break;

                        /* US      2E      Machine Language Subroutine Call.                  */
                        /*                 The top six bytes of the expression stack contain  */
                        /* 3 numbers with the following interpretations: The top number is    */
                        /* loaded into the A (or A and B) register; the next number is loaded */
                        /* into 16 bits of Index register; the third number is interpreted as */
                        /* the address of a machine language subroutine to be called. These   */
                        /* six bytes on the expression stack are replaced with the 16-bit     */
                        /* result returned by the subroutine. Stack underflow results in an   */
                        /* error stop.                                                        */
                    case 46:
                        Poke2(LinoCore, Lino); /* bring these memory locations up.. */
                        Poke2(ILPCcore, ILPC); /* ..to date, in case user looks.. */
                        Poke2(BPcore, BP);
                        Poke2(SvPtCore, SvPt);
                        ix = PopExInt()&0xFFFF; /* datum A */
                        here = PopExInt()&0xFFFF; /* datum X */
                        op = PopExInt()&0xFFFF; /* nominal machine address */
                        if (ILPC == 0) break;
                        if (op >= Peek2(ILfront) && op < ILend) { /* call IL subroutine.. */
                            PushExInt(here);
                            PushExInt(ix);
                            PushSub(ILPC); /* push return location */
                            ILPC = op; 
                            if (DEBUGON > 0) LogIt(-ILPC);
                            break;
                        }
                        switch (op) {
                            case WachPoint: /* we only do a few predefined functions.. */
                                Watcher = here;
                                //          if (ix>32767) ix = -(int)Core[here]-256;
                                Watchee = ix;
                                if (Debugging > 0) {
                                    OutLn();
                                    OutStr("[** Watch ");
                                    OutHex(here, 4);
                                    OutStr("]");
                                }
                                PushExInt((int) Core[here]);
                                break;
                            case ColdGo:
                                ColdStart();
                                break;
                            case WarmGo:
                                WarmStart();
                                break;
                            case InchSub:
                                PushExInt((int) Inch());
                                break;
                            case OutchSub:
                                Ouch((char) (ix & 127));
                                PushExInt(0);
                                break;
                            case BreakSub:
                                PushExInt(StopIt());
                                break;
                            case PeekSub:
                                PushExInt((int) Core[here]);
                                break;
                            case Peek2Sub:
                                PushExInt(Peek2(here));
                                break;
                            case PokeSub:
                                ix = ix & 0xFF;
                                Core[here] = (aByte) ix;
                                PushExInt(ix);
                                //          if (DEBUGON>0) LogIt(((ix+256)<<16)+here);
                                Lino = Peek2(LinoCore); /* restore these pointers.. */
                                ILPC = Peek2(ILPCcore); /* ..in case user changed them.. */
                                BP = Peek2(BPcore);
                                SvPt = Peek2(SvPtCore);
                                break;
                            case DumpSub:
                                ShoMemDump(here, ix);
                                PushExInt(here + ix);
                                break;
                            case TrLogSub:
                                ShowLog();
                                PushExInt(LogHere);
                                break;
                            default: TBerror();
                        }
                        break;

                        /* RT      2F      IL Subroutine Return.                              */
                        /*                 The IL control stack is popped to give the address */
                        /* of the next IL instruction. An error stop occurs if the entire     */
                        /* control stack (IL and BASIC) is empty.                             */
                    case 47:
                        ix = PopSub(); /* get return from pop */
                        if (ix < Peek2(ILfront) || ix >= ILend) TBerror();
                        else if (ILPC != 0) {
                            ILPC = ix;
                            if (DEBUGON > 0) LogIt(-ILPC);
                        }
                        break;

                        /* JS a    3000-37FF       IL Subroutine Call.                        */
                        /*                         The least significant eleven bits of this  */
                        /* 2-byte instruction are added to the base address of the IL program */
                        /* to become address of the next instruction. The previous contents   */
                        /* of the IL program counter are pushed onto the IL region of the     */
                        /* control stack. Stack overflow results in an error stop.            */
                    case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55:
                        PushSub(ILPC + 1); /* push return location there */
                        if (ILPC == 0) break;
                        ILPC = (lit16(ILPC - 1) & 0x7FF) + Peek2(ILfront);
                        if (DEBUGON > 0) LogIt(-ILPC);
                        break;

                        /* J a     3800-3FFF       Jump.                                      */
                        /*                         The low eleven bits of this 2-byte         */
                        /* instruction are added to the IL program base address to determine  */
                        /* the address of the next IL instruction. The previous contents of   */
                        /* the IL program counter is lost. */
                    case 56: case 57: case 58: case 59: case 60: case 61: case 62: case 63:
                        ILPC = (lit16(ILPC - 1) & 0x7FF) + Peek2(ILfront);
                        if (DEBUGON > 0) LogIt(-ILPC);
                        break;

                        /* NO      08      No Operation.                                      */
                        /*                 This may be used as a space filler (such as to     */
                        /* ignore a skip).                                                    */
                    default: break;
                } /* last of inner switch cases */
                break; /* end of outer switch cases 0,1 */

                /* BR a    40-7F   Relative Branch.                                   */
                /*                 The low six bits of this instruction opcode are    */
                /* added algebraically to the current value of the IL program counter */
                /* to give the address of the next IL instruction. Bit 5 of opcode is */
                /* the sign, with + signified by 1, - by 0. The range of this branch  */
                /* is +/-31 bytes from address of the byte following the opcode. An   */
                /* offset of zero (i.e. opcode 60) results in an error stop. The      */
                /* branch operation is unconditional.                                 */
            case 2: case 3:
                ILPC = ILPC + op - 96;
                if (DEBUGON > 0) LogIt(-ILPC);
                break;

                /* BC a "xxx"   80xxxxXx-9FxxxxXx  String Match Branch.               */
                /*                                 The ASCII character string in IL   */
                /* following this opcode is compared to the string beginning with the */
                /* current position of the BASIC pointer, ignoring blanks in BASIC    */
                /* program. The comparison continues until either a mismatch, or an   */
                /* IL byte is reached with the most significant bit set to one. This  */
                /* is the last byte of the string in the IL, compared as a 7-bit      */
                /* character; if equal, the BASIC pointer is positioned after the     */
                /* last matching character in the BASIC program and the IL continues  */
                /* with the next instruction in sequence. Otherwise the BASIC pointer */
                /* is not altered and the low five bits of the Branch opcode are      */
                /* added to the IL program counter to form the address of the next    */
                /* IL instruction. If the strings do not match and the branch offset  */
                /* is zero an error stop occurs.                                      */
            case 4:
                if (op == 128) here = 0; /* to error if no match */
                else here = ILPC + op - 128;
                chpt = BP;
                ix = 0;
                while ((ix & 128) == 0) {
                    while (((char) Core[BP]) == ' ') BP++; /* skip over spaces */
                    ix = (int) code[ILPC++];
                    if (((char) (ix & 127)) != DeCaps(((int) Core[BP++])&127)) {
                        BP = chpt; /* back up to front of string in Basic */
                        if (here == 0) TBerror();
                        else ILPC = here; /* jump forward in IL */
                        break;
                    }
                }
                if (DEBUGON > 0) if (ILPC > 0) LogIt(-ILPC);
                break;

                /* BV a    A0-BF   Branch if Not Variable.                            */
                /*                 If the next non-blank character pointed to by the  */
                /* BASIC pointer is a capital letter, its ASCII code is [doubled and] */
                /* pushed onto the expression stack and the IL program advances to    */
                /* next instruction in sequence, leaving the BASIC pointer positioned */
                /* after the letter; if not a letter the branch is taken and BASIC    */
                /* pointer is left pointing to that character. An error stop occurs   */
                /* if the next character is not a letter and the offset of the branch */
                /* is zero, or on stack overflow.                                     */
            case 5:
                while (((char) Core[BP]) == ' ') BP++; /* skip over spaces */
                ch = (char) Core[BP];
                if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z')
                    PushExBy((((int) Core[BP++])&0x5F)*2);
                else if (op == 160) TBerror(); /* error if not letter */
                else ILPC = ILPC + op - 160;
                if (DEBUGON > 0) if (ILPC > 0) LogIt(-ILPC);
                break;

                /* BN a    C0-DF   Branch if Not a Number.                            */
                /*                 If the next non-blank character pointed to by the  */
                /* BASIC pointer is not a decimal digit, the low five bits of the     */
                /* opcode are added to the IL program counter, or if zero an error    */
                /* stop occurs. If the next character is a digit, then it and all     */
                /* decimal digits following it (ignoring blanks) are converted to a   */
                /* 16-bit binary number which is pushed onto the expression stack. In */
                /* either case the BASIC pointer is positioned at the next character  */
                /* which is neither blank nor digit. Stack overflow will result in an */
                /* error stop.                                                        */
            case 6:
                while (((char) Core[BP]) == ' ') BP++; /* skip over spaces */
                ch = (char) Core[BP];
                if (ch >= '0' && ch <= '9') {
                    op = 0;
                    while (true) {
                        here = (int) Core[BP++];
                        if (here == 32) continue; /* skip over spaces */
                        if (here < 48 || here > 57) break; /* not a decimal digit */
                        op = op * 10 + here - 48;
                    } /* insert into value */
                    BP--; /* back up over non-digit */
                    PushExInt(op);
                } else if (op == 192) TBerror(); /* error if no digit */
                else ILPC = ILPC + op - 192;
                if (DEBUGON > 0) if (ILPC > 0) LogIt(-ILPC);
                break;

                /* BE a    E0-FF   Branch if Not Endline.                             */
                /*                 If the next non-blank character pointed to by the  */
                /* BASIC pointer is a carriage return, the IL program advances to the */
                /* next instruction in sequence; otherwise the low five bits of the   */
                /* opcode (if not 0) are added to the IL program counter to form the  */
                /* address of next IL instruction. In either case the BASIC pointer   */
                /* is left pointing to the first non-blank character; this            */
                /* instruction will not pass over the carriage return, which must     */
                /* remain for testing by the NX instruction. As with the other        */
                /* conditional branches, the branch may only advance the IL program   */
                /* counter from 1 to 31 bytes; an offset of zero results in an error  */
                /* stop.                                                              */
            case 7:
                while (((char) Core[BP]) == ' ') BP++; /* skip over spaces */
                if (((char) Core[BP]) == '\r');
                else if (op == 224) TBerror(); /* error if no offset */
                else ILPC = ILPC + op - 224;
                if (DEBUGON > 0) if (ILPC > 0) LogIt(-ILPC);
                break;
        }
    }

} /* ~Interp */

/***************** Intermediate Interpreter Assembled *****************/

        //  ("0000 ;       1 .  ORIGINAL TINY BASIC INTERMEDIATE INTERPRETER\n");
        //  ("0000 ;       2 .\n");
        //  ("0000 ;       3 .  EXECUTIVE INITIALIZATION\n");
        //  ("0000 ;       4 .\n");
        //  ("0000 ;       5 :STRT PC \":Q^\"        COLON, X-ON\n");
const char code[] = {
    0x00, // add an offset as 0 is used as a sentinel value
    0x24,
    0x3A,
    0x91, // ("0000 243A91;                         \n");
          // ("0003 ;       6       GL              \n");
    0x27, // ("0003 27;     7       SB              \n");
    0x10, // ("0004 10;     8       BE L0                    BRANCH IF NOT EMPTY\n");
    0xE1, // ("0005 E1;     9       BR STRT                  TRY AGAIN IF NULL LINE\n");
    0x59, // ("0006 59;    10 :L0   BN STMT                  TEST FOR LINE NUMBER\n");
    0xC5, // ("0007 C5;    11       IL                       IF SO, INSERT INTO PROGRAM\n");
    0x2A, // ("0008 2A;    12       BR STRT                  GO GET NEXT\n");
    0x56, // ("0009 56;    13 :XEC  SB                       SAVE POINTERS FOR RUN WITH\n");
    0x10, // ("000A 10;    14       RB                       CONCATENATED INPUT\n");
    0x11, // ("000B 11;    15       XQ              \n");
    0x2C, // ("000C 2C;    16 .                     \n");
          // ("000D ;      17 .  STATEMENT EXECUTOR \n");
          // ("000D ;      18 .                     \n");
          // ("000D ;      19 :STMT BC GOTO \"LET\" \n");
    0x8B,
    0x4C,
    0x45,
    0xD4, // ("000D 8B4C45D4;\n");
          // ("0011 ;              20       BV *            MUST BE A VARIABLE NAME\n");
    0xA0, // ("0011 A0;            21       BC * \"=\"\n");
    0x80,
    0xBD, // ("0012 80BD;          22 :LET  JS EXPR         GO GET EXPRESSION\n");
    0x30,
    0xBC, // ("0014 30BC;          23       BE *            IF STATEMENT END,\n");
    0xE0, // ("0016 E0;            24       SV                STORE RESULT\n");
    0x13, // ("0017 13;            25       NX\n");
    0x1D, // ("0018 1D;            26 .\n");
          // ("0019 ;              27 :GOTO BC PRNT \"GO\"\n");
    0x94,
    0x47,
    0xCF, // ("0019 9447CF;\n");
    // ("001C ;              28       BC GOSB \"TO\"\n");
    0x88,
    0x54,
    0xCF, // ("001C 8854CF;\n");
    // ("001F ;              29       JS EXPR         GET LINE NUMBER\n");
    0x30,
    0xBC, // ("001F 30BC;          30       BE *\n");
    0xE0, // ("0021 E0;            31       SB              (DO THIS FOR STARTING)\n");
    0x10, // ("0022 10;            32       RB\n");
    0x11, // ("0023 11;            33       GO              GO THERE\n");
    0x16, // ("0024 16;            34 .\n");
    // ("0025 ;              35 :GOSB BC * \"SUB\"      NO OTHER WORD BEGINS \"GO...\"\n");
    0x80,
    0x53,
    0x55,
    0xC2, // ("0025 805355C2;\n");
    // ("0029 ;              36       JS EXPR\n");
    0x30,
    0xBC, // ("0029 30BC;          37       BE *\n");
    0xE0, // ("002B E0;            38       GS\n");
    0x14, // ("002C 14;            39       GO\n");
    0x16, // ("002D 16;            40 .\n");
    // ("002E ;              41 :PRNT BC SKIP \"PR\"\n");
    0x90,
    0x50,
    0xD2, // ("002E 9050D2;\n");
    // ("0031 ;              42       BC P0 \"INT\"     OPTIONALLY OMIT \"INT\"\n");
    0x83,
    0x49,
    0x4E,
    0xD4, // ("0031 83494ED4;\n");
    // ("0035 ;              43 :P0   BE P3\n");
    0xE5, // ("0035 E5;            44       BR P6           IF DONE, GO TO END\n");
    0x71, // ("0036 71;            45 :P1   BC P4 \";\"\n");
    0x88,
    0xBB, // ("0037 88BB;          46 :P2   BE P3\n");
    0xE1, // ("0039 E1;            47       NX              NO CRLF IF ENDED BY ; OR ,\n");
    0x1D, // ("003A 1D;            48 :P3   BC P7 '\"'\n");
    0x8F,
    0xA2, // ("003B 8FA2;          49       PQ              QUOTE MARKS STRING\n");
    0x21, // ("003D 21;            50       BR P1           GO CHECK DELIMITER\n");
    0x58, // ("003E 58;            51 :SKIP BR IF           (ON THE WAY THRU)\n");
    0x6F, // ("003F 6F;    52 :P4   BC P5 \",\"\n");
    0x83,
    0xAC, // ("0040 83AC;          53       PT              COMMA SPACING\n");
    0x22, // ("0042 22;            54       BR P2\n");
    0x55, // ("0043 55;            55 :P5   BC P6 \":\"\n");
    0x83,
    0xBA, // ("0044 83BA;          56       PC \"S^\"         OUTPUT X-OFF\n");
    0x24,
    0x93, // ("0046 2493;          57 :P6   BE *\n");
    0xE0, // ("0048 E0;            58       NL              THEN CRLF\n");
    0x23, // ("0049 23;            59       NX\n");
    0x1D, // ("004A 1D;            60 :P7   JS EXPR         TRY FOR AN EXPRESSION\n");
    0x30,
    0xBC, // ("004B 30BC;          61       PN\n");
    0x20, // ("004D 20;            62       BR P1\n");
    0x48, // ("004E 48;            63 .\n");
    // ("004F ;              64 :IF   BC INPT \"IF\"\n");
    0x91,
    0x49,
    0xC6, // ("004F 9149C6;\n");
    // ("0052 ;              65       JS EXPR\n");
    0x30,
    0xBC, // ("0052 30BC;          66       JS RELO\n");
    0x31,
    0x34, // ("0054 3134;          67       JS EXPR\n");
    0x30,
    0xBC, // ("0056 30BC;          68       BC I1 \"THEN\"    OPTIONAL NOISEWORD\n");
    0x84,
    0x54,
    0x48,
    0x45,
    0xCE, // ("0058 84544845CE;\n");
    // ("005D ;              69 :I1   CP              COMPARE SKIPS NEXT IF TRUE\n");
    0x1C, // ("005D 1C;            70       NX              FALSE.\n");
    0x1D, // ("005E 1D;            71       J STMT          TRUE. GO PROCESS STATEMENT\n");
    0x38,
    0x0D, // ("005F 380D;          72 .\n");
    // ("0061 ;              73 :INPT BC RETN \"INPUT\"\n");
    0x9A,
    0x49,
    0x4E,
    0x50,
    0x55,
    0xD4, // ("0061 9A494E5055D4;\n");
    // ("0067 ;              74 :I2   BV *            GET VARIABLE\n");
    0xA0, // ("0067 A0;            75       SB              SWAP POINTERS\n");
    0x10, // ("0068 10;            76       BE I4\n");
    0xE7, // ("0069 E7;            77 :I3   PC \"? Q^\"       LINE IS EMPTY; TYPE PROMPT\n");
    0x24,
    0x3F,
    0x20,
    0x91, // ("006A 243F2091;\n");
    // ("006E ;              78       GL              READ INPUT LINE\n");
    0x27, // ("006E 27;            79       BE I4           DID ANYTHING COME?\n");
    0xE1, // ("006F E1;            80       BR I3           NO, TRY AGAIN\n");
    0x59, // ("0070 59;            81 :I4   BC I5 \",\"       OPTIONAL COMMA\n");
    0x81,
    0xAC, // ("0071 81AC;          82 :I5   JS EXPR         READ A NUMBER\n");
    0x30,
    0xBC, // ("0073 30BC;          83       SV              STORE INTO VARIABLE\n");
    0x13, // ("0075 13;            84       RB              SWAP BACK\n");
    0x11, // ("0076 11;            85       BC I6 \",\"       ANOTHER?\n");
    0x82,
    0xAC, // ("0077 82AC;          86       BR I2           YES IF COMMA\n");
    0x4D, // ("0079 4D;            87 :I6   BE *            OTHERWISE QUIT\n");
    0xE0, // ("007A E0;            88       NX\n");
    0x1D, // ("007B 1D;            89 .\n");
    // ("007C ;              90 :RETN BC END \"RETURN\"\n");
    0x89,
    0x52,
    0x45,
    0x54,
    0x55,
    0x52,
    0xCE, // ("007C 895245545552CE;\n");
    // ("0083 ;              91       BE *\n");
    0xE0, // ("0083 E0;            92       RS              RECOVER SAVED LINE\n");
    0x15, // ("0084 15;            93       NX\n");
    0x1D, // ("0085 1D;            94 .\n");
    // ("0086 ;              95 :END  BC LIST \"END\"\n");
    0x85,
    0x45,
    0x4E,
    0xC4, // ("0086 85454EC4;\n");
          // ("008A ;              96       BE *\n");
    0xE0, // ("008A E0;            97       WS\n");
    0x2D, // ("008B 2D;            98 .\n");
          // ("008C ;              99 :LIST BC RUN \"LIST\"\n");
    0x98,
    0x4C,
    0x49,
    0x53,
    0xD4, // ("008C 984C4953D4;\n");
          // ("0091 ;             100       BE L2\n");
    0xEC, // ("0091 EC;           101 :L1   PC \"@^@^@^@^J^@^\" PUNCH LEADER\n");
    0x24,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0A,
    0x80, // ("0092 24000000000A80;\n");
          // ("0099 ;             102       LS              LIST\n");
    0x1F, // ("0099 1F;           103       PC \"S^\"         PUNCH X-OFF\n");
    0x24,
    0x93, // ("009A 2493;         104       NL\n");
    0x23, // ("009C 23;           105       NX\n");
    0x1D, // ("009D 1D;           106 :L2   JS EXPR         GET A LINE NUMBER\n");
    0x30,
    0xBC, // ("009E 30BC;         107       BE L3\n");
    0xE1, // ("00A0 E1;           108       BR L1\n");
    0x50, // ("00A1 50;           109 :L3   BC * \",\"        SEPARATED BY COMMAS\n");
    0x80,
    0xAC, // ("00A2 80AC;         110       BR L2\n");
    0x59, // ("00A4 59;           111 .\n");
          // ("00A5 ;             112 :RUN  BC CLER \"RUN\"\n");
    0x85,
    0x52,
    0x55,
    0xCE, // ("00A5 855255CE;\n");
          // ("00A9 ;             113       J XEC\n");
    0x38,
    0x0A, // ("00A9 380A;         114 .\n");
          // ("00AB ;             115 :CLER BC REM \"CLEAR\"\n");
    0x86,
    0x43,
    0x4C,
    0x45,
    0x41,
    0xD2, // ("00AB 86434C4541D2;\n");
          // ("00B1 ;             116       MT\n");
    0x2B, // ("00B1 2B;           117 .\n");
          // ("00B2 ;             118 :REM  BC DFLT \"REM\"\n");
    0x84,
    0x52,
    0x45,
    0xCD, // ("00B2 845245CD;\n");
          // ("00B6 ;             119       NX\n");
    0x1D, // ("00B6 1D;           120 .\n");
          // ("00B7 ;             121 :DFLT BV *            NO KEYWORD...\n");
    0xA0, // ("00B7 A0;           122       BC * \"=\"        TRY FOR LET\n");
    0x80,
    0xBD, // ("00B8 80BD;         123       J LET           IT'S A GOOD BET.\n");
    0x38,
    0x14, // ("00BA 3814;         124 .\n");
          // ("00BC ;             125 .  SUBROUTINES\n");
          // ("00BC ;             126 .\n");
          // ("00BC ;             127 :EXPR BC E0 \"-\"       TRY FOR UNARY MINUS\n");
    0x85,
    0xAD, // ("00BC 85AD;         128       JS TERM         AHA\n");
    0x30,
    0xD3, // ("00BE 30D3;         129       NE\n");
    0x17, // ("00C0 17;           130       BR E1\n");
    0x64, // ("00C1 64;           131 :E0   BC E4 \"+\"       IGNORE UNARY PLUS\n");
    0x81,
    0xAB, // ("00C2 81AB; 132 :E4   JS TERM\n");
    0x30,
    0xD3, // ("00C4 30D3;         133 :E1   BC E2 \"+\"       TERMS SEPARATED BY PLUS\n");
    0x85,
    0xAB, // ("00C6 85AB;         134       JS TERM\n");
    0x30,
    0xD3, // ("00C8 30D3;         135       AD\n");
    0x18, // ("00CA 18;           136       BR E1\n");
    0x5A, // ("00CB 5A;           137 :E2   BC E3 \"-\"       TERMS SEPARATED BY MINUS\n");
    0x85,
    0xAD, // ("00CC 85AD;         138       JS TERM\n");
    0x30,
    0xD3, // ("00CE 30D3;         139       SU\n");
    0x19, // ("00D0 19;           140       BR E1\n");
    0x54, // ("00D1 54;           141 :E3   RT\n");
    0x2F, // ("00D2 2F;           142 .\n");
          // ("00D3 ;             143 :TERM JS FACT\n");
    0x30,
    0xE2, // ("00D3 30E2;         144 :T0   BC T1 \"*\"       FACTORS SEPARATED BY TIMES\n");
    0x85,
    0xAA, // ("00D5 85AA;         145       JS FACT\n");
    0x30,
    0xE2, // ("00D7 30E2;         146       MP\n");
    0x1A, // ("00D9 1A;           147       BR T0\n");
    0x5A, // ("00DA 5A;           148 :T1   BC T2 \"/\"       FACTORS SEPARATED BY DIVIDE\n");
    0x85,
    0xAF, // ("00DB 85AF;         149       JS  FACT\n");
    0x30,
    0xE2, // ("00DD 30E2;         150       DV\n");
    0x1B, // ("00DF 1B;           151       BR T0\n");
    0x54, // ("00E0 54;           152 :T2   RT\n");
    0x2F, // ("00E1 2F;           153 .\n");
          // ("00E2 ;             154 :FACT BC F0 \"RND\"     *RND FUNCTION*\n");
    0x97,
    0x52,
    0x4E,
    0xC4, // ("00E2 97524EC4;\n");
          // ("00E6 ;             155       LN 257*128      STACK POINTER FOR STORE\n");
    0x0A, // ("00E6 0A;\n");
    0x80,
    0x80, // ("00E7 8080;         156       FV              THEN GET RNDM\n");
    0x12, // ("00E9 12;   157       LN 2345         R:=R*2345+6789\n");
    0x0A, // ("00EA 0A;\n");
    0x09,
    0x29, // ("00EB 0929;         158       MP\n");
    0x1A, // ("00ED 1A;           159       LN 6789\n");
    0x0A, // ("00EE 0A;\n");
    0x1A,
    0x85, // ("00EF 1A85;         160       AD\n");
    0x18, // ("00F1 18;           161       SV\n");
    0x13, // ("00F2 13;           162       LB 128          GET IT AGAIN\n");
    0x09,
    0x80, // ("00F3 0980;         163       FV\n");
    0x12, // ("00F5 12;           164       DS\n");
    0x0B, // ("00F6 0B;           165       JS FUNC         GET ARGUMENT\n");
    0x31,
    0x30, // ("00F7 3130;         166       BR F1\n");
    0x61, // ("00F9 61;           167 :F0   BR F2           (SKIPPING)\n");
    0x73, // ("00FA 73;           168 :F1   DS\n");
    0x0B, // ("00FB 0B;           169       SX 2            PUSH TOP INTO STACK\n");
    0x02, // ("00FC 02;           170       SX 4\n");
    0x04, // ("00FD 04;           171       SX 2\n");
    0x02, // ("00FE 02;           172       SX 3\n");
    0x03, // ("00FF 03;           173       SX 5\n");
    0x05, // ("0100 05;           174       SX 3\n");
    0x03, // ("0101 03;           175       DV              PERFORM MOD FUNCTION\n");
    0x1B, // ("0102 1B;           176       MP\n");
    0x1A, // ("0103 1A;           177       SU\n");
    0x19, // ("0104 19;           178       DS              PERFORM ABS FUNCTION\n");
    0x0B, // ("0105 0B;           179       LB 6\n");
    0x09,
    0x06, // ("0106 0906;         180       LN 0\n");
    0x0A, // ("0108 0A;\n");
    0x00,
    0x00, // ("0109 0000;         181       CP              (SKIP IF + OR 0)\n");
    0x1C, // ("010B 1C;           182       NE\n");
    0x17, // ("010C 17;           183       RT\n");
    0x2F, // ("010D 2F;           184 :F2   BC F3 \"USR\"     *USR FUNCTION*\n");
    0x8F,
    0x55,
    0x53,
    0xD2, // ("010E 8F5553D2;\n");
          // ("0112 ;             185       BC * \"(\"        3 ARGUMENTS POSSIBLE\n");
    0x80,
    0xA8, // ("0112 80A8;         186       JS EXPR         ONE REQUIRED\n");
    0x30,
    0xBC, // ("0114 30BC;         187       JS ARG\n");
    0x31,
    0x2A, // ("0116 312A;         188       JS ARG\n");
    0x31,
    0x2A, // ("0118 312A;         189       BC * \")\"\n");
    0x80,
    0xA9, // ("011A 80A9;         190       US              GO DO IT\n");
    0x2E, // ("011C 2E;           191       RT\n");
    0x2F, // ("011D 2F;           192 :F3   BV F4           VARIABLE?\n");
    0xA2, // ("011E A2;           193       FV              YES.  GET IT\n");
    0x12, // ("011F 12;           194       RT\n");
    0x2F, // ("0120 2F;           195 :F4   BN F5           NUMBER?\n");
    0xC1, // ("0121 C1;           196       RT              GOT IT.\n");
    0x2F, // ("0122 2F;           197 :F5   BC * \"(\"        OTHERWISE MUST BE (EXPR)\n");
    0x80,
    0xA8, // ("0123 80A8;         198 :F6   JS EXPR\n");
    0x30,
    0xBC, // ("0125 30BC;         199       BC * \")\"\n");
    0x80,
    0xA9, // ("0127 80A9;         200       RT\n");
    0x2F, // ("0129 2F;           201 .\n");
          // ("012A ;             202 :ARG  BC A0 \",\"        COMMA?\n");
    0x83,
    0xAC, // ("012A 83AC;         203       J  EXPR          YES, GET EXPRESSION\n");
    0x38,
    0xBC, // ("012C 38BC;         204 :A0   DS               NO, DUPLICATE STACK TOP\n");
    0x0B, // ("012E 0B;           205       RT\n");
    0x2F, // ("012F 2F;           206 .\n");
          // ("0130 ;             207 :FUNC BC * \"(\"\n");
    0x80,
    0xA8, // ("0130 80A8;         208       BR F6\n");
    0x52, // ("0132 52;           209       RT\n");
    0x2F, // ("0133 2F;           210 .\n");
          // ("0134 ;             211 :RELO BC R0 \"=\"        CONVERT RELATION OPERATORS\n");
    0x84,
    0xBD, // ("0134 84BD;         212       LB 2             TO CODE BYTE ON STACK\n");
    0x09,
    0x02, // ("0136 0902;         213       RT               =\n");
    0x2F, // ("0138 2F;           214 :R0   BC R4 \"<\"\n");
    0x8E,
    0xBC, // ("0139 8EBC;         215       BC R1 \"=\"\n");
    0x84,
    0xBD, // ("013B 84BD;         216       LB 3             <=\n");
    0x09,
    0x03, // ("013D 0903;         217       RT\n");
    0x2F, // ("013F 2F;           218 :R1   BC R3 \">\"\n");
    0x84,
    0xBE, // ("0140 84BE;         219       LB 5             <>\n");
    0x09,
    0x05, // ("0142 0905;         220       RT\n");
    0x2F, // ("0144 2F;           221 :R3   LB 1             <\n");
    0x09,
    0x01, // ("0145 0901;         222       RT\n");
    0x2F, // ("0147 2F;           223 :R4   BC * \">\"\n");
    0x80,
    0xBE, // ("0148 80BE;         224       BC R5 \"=\"\n");
    0x84,
    0xBD, // ("014A 84BD;         225       LB 6             >=\n");
    0x09,
    0x06, // ("014C 0906;         226       RT\n");
    0x2F, // ("014E 2F;           227 :R5   BC R6 \"<\"\n");
    0x84,
    0xBC, // ("014F 84BC;         228       LB 5             ><\n");
    0x09,
    0x05,
    0x05, // ("0151 0905;         229       RT\n");
    0x2F, // ("0153 2F;           230 :R6   LB 4             >\n");
    0x09,
    0x04, // ("0154 0904;         231       RT\n");
    0x2F, // ("0156 2F;           232 .\n");
    0x00
}; // ("0157 ;    0000\n");
//} /* ~DefaultIL */

/**************************** Startup Code ****************************/

void StartTinyBasic(char* ILtext) {
    int nx;
    for (nx = 0; nx < CoreTop; nx++) Core[nx] = 0; /* clear Core.. */
    Poke2(ExpnStk, 8191);   /* random number seed */
    Core[BScode] = 8;       /* backspace */
    Core[CanCode] = 27;     /* escape */
    ConvtIL();              /* convert IL assembly code to binary */
    ColdStart();
    Interp();               /* go do it */
}

void main(int argc, char* argv[]) {
    char* IL = NULL;

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    SYSTEM_Initialize();

    puts("TinyBasic");
    StartTinyBasic(IL); /* go do it */
} 