#ifndef ESQUEMA_NATIVE_PROCS_HH_INCLUDED
#define ESQUEMA_NATIVE_PROCS_HH_INCLUDED

#include "parser.hh"

namespace esquema {
    class Environment;
    Cell add(List const & args, Environment * env);
    Cell sub(List const & args, Environment * env);
    Cell mul(List const & args, Environment * env);
    Cell div(List const & args, Environment * env);
    Cell less(List const & args, Environment * env);
    Cell less_equal(List const & args, Environment * env);
    Cell greater(List const & args, Environment * env);
    Cell greater_equal(List const & args, Environment * env);
    Cell equal(List const & args, Environment * env);
    Cell negate(List const & args, Environment * env);
}

#endif
