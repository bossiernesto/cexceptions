#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include 'hashtable.c'

#define _exc_id unsigned int

#define __exc_block(start, end)              \
  for (start, _exc_global_t->exc_block_pass = 1;          \
       _exc_global_t->exc_block_pass;                     \
       end, _exc_global_t->exc_block_pass = 0)

/* Likewise, but START block is empty. */

#define __exc_end(end)                       \
  for ( _exc_global_t->exc_block_pass = 1;                 \
        _exc_global_t->exc_block_pass;                     \
       end, _exc_global_t->exc_block_pass = 0)

#ifndef _exc_cmp
#  define _exc_cmp(c1, c2)         ((c1) == (c2))
#endif


/*Primitives for the exception module*/

/*wrapper macro*/
#define throw(exception...) _exc_throw((exception))

#define try				            \
	if((	{jmp_buf buf;		    \
	     	int  value;		    \
			value=setjmp(buf);	    \
			_exc_global_t->state == OnTry;\
			_exc_push(&buf,value);}))	\
			_exc_end(_exc_pop(0))

#define catch				                     \
	else __exc_block(__exc_handled = 0,          \
                 ({ if (_exc_global_t->state == ThownExc)      \
                     		_exc_rethrow(); })	     \


/*wrapper macro*/
#define handle (exception...)                                  \
    if(_exc_handle(exception))

#define	finally			                           \
    if(_exc_global_t.state==ThownExc || _exc_global_t.state==Unhandled)		\

/*States of Exception System*/
enum ExceptionState
{
	Nothing=0x00,
	OnTry=0x01,
	ThownExc=0x02,
	Catched=0x03,
	Unhandled=0x04
};

enum ExceptionType
{
	Critical=0x00,
	Error=0x01,
    Exception=0x02,
	NotDefined=0x99
};


/*begin of Exception Hash Table*/

#define HASH_TABLE_SIZE 256

#define _add_exception(exception_id,func,message,type)      \


#define _del_exception(exception_id)



#define _parse_unhandled_exc()


extern _exc_exeption_t exc_hash_t[HASH_TABLE_SIZE];
/*end of Exception Hash Table*/
typedef struct _exc_buffer {		/*Stack of cachers*/
    struct _exc_buffer *prev_level;
    jmp_buf *environ;
    unsigned int set_ret;
    unsigned int level;
}_exc_stack_t;

typedef struct _exc_struct {		/*Exception Global Structure*/
	struct _exc_buffer  *_exc_tos; /* global exception chain header */
	_exc_id	current_exc;
	volatile unsigned int exc_block_pass;
    unsigned int state;
}_exc_struct_t;

typedef struct _exc_exeption {
    unsigned int code; /*Exception Code*/
    int (*func) (void*); /*Exception Snipplet: OPTIONAL*/
    char *Message;			/* Human-readable cause */
    unsigned int type; /*Type of Exception*/
}_exc_exeption_t;

extern int errno;
extern int sys_nerr;
extern char *sys_errlist[];
extern _exc_struct_t *_exc_global_t;



/*Exception Stack primitives*/
extern void _exc_push(jmp_buf *environ,int ret_value);
extern void _exc_pop (jmp_buf *environ);
/*Throw an exception, if exception is not caught, then it'll become an unhandled one*/
extern void _exc_trow(_exc_id id);
/*Handle current thrown exception*/
extern int _exc_handle(_exc_id id);
/* Throw it in upper level of catcher blocks. */
extern void _exc_rethrow();

