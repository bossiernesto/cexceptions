_exc_struct_t *_exc_global_t;

void _exc_push(jmp_buf *environ, int ret_value) {
    register _exc_struct_t *new_tos;

    if (environ && ret_value) {
        new_tos = (_exc_struct_t *) emalloc(sizeof(_exc_struct_t));
        new_tos.environ = environ;
        new_tos.set_ret = ret_value;
        new_tos.level = exc_tos.level++;
        new_tos.prev_level = _exc_global_t._exc_tos;
        _exc_global_t._exc_tos = new_tos;
    }
}

void _exc_pop(jmp_buf * environ) {
    register _exc_stack_t *exc_tos = _exc_global_t._exc_tos;

    if (exc_tos == null) {
        _exc_global_t.state = Unhandled;         /*unhandled exception*/
    }
    if (environ) {
        memcpy(environ, &exc_tos.environ, sizeof(jmp_buf));
    }
    environ = exc_tos.environ;
    _exc_global_t._exc_tos = _exc_tos.prev_level;
    free(exc_tos);
}

void _exc_trow(_exc_id id) {
    jmp_buf jump;
    /* Pop for jumping. */
    _exc_pop(&jump);
    _exc_global_t.current_exc = id;
    _exc_global_t.state = ThownExc;
    /* LONGJUMP to J with nonzero value. */
    longjmp(jump, 1);
}

int _exc_handle(_exc_id id) {
    if (_exc_global_t.state == Catched)
        return 0;                        /*Already Catched*/
    if (_exc_cmp(id, _exc_global_t.current_exc)) {
        _exc_global_t.state == Catched;        /*Exception caught*/
        return 1;
    }
    return 0;
}

void _exc_rethrow() {
    jmp_buf upper_level;

    _exc_pop(&upper_level);
    longjmp(upper_level, 1);
}

void raise_sys() {
    register int errnum = errno;

    if ((errnum < 1) || (errnum >= sys_nerr)) {
        raise(-1, "Unknown Unix error code");
    }
    else {
        raise(errnum, sys_errlist[errnum]);
    }
}

