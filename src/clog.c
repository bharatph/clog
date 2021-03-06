#include <clog/clog.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#if defined(__linux__) || defined(__unix__) || \
    defined(__APPLE__) && defined(__MACH__) // TODO Add Solaris
#include <pthread.h>
#include <unistd.h>
#elif _WIN32
#include <Windows.h>
#else
#error Platform not supprted
#endif

#define CLOG_BUFFER_SIZE 64

#if defined(__linux__) || defined(__unix__) || \
    defined(__APPLE__) && defined(__MACH__) // TODO Add Solaris
#define _CLOG_LOCK() pthread_mutex_lock(&__clog_lock);
#elif defined(_WIN32)
#define _CLOG_LOCK()
#endif

#if defined(__linux__) || defined(__unix__) || \
    defined(__APPLE__) && defined(__MACH__) // TODO Add Solaris
#define _CLOG_UNLOCK() pthread_mutex_unlock(&__clog_lock);
#elif defined(_WIN32)
#define _CLOG_UNLOCK()
#endif

#define _CLOG_LOG(__CLOG_LEVEL, __CLOG_FILE, __CLOG__LINE)  \
  {                                                         \
    int len = (int)strlen(msg);                             \
    char *str = (char *)malloc(len + CLOG_BUFFER_SIZE);     \
    va_list vl;                                             \
    va_start(vl, msg);                                      \
    vsnprintf(str, len + CLOG_BUFFER_SIZE, msg, vl);        \
    va_end(vl);                                             \
    clog_item *ci = (clog_item *)malloc(sizeof(clog_item)); \
    ci->level = __CLOG_LEVEL;                               \
    ci->tag = tag;                                          \
    ci->msg = str;                                          \
    add_to_stack(ci);                                       \
  }

static FILE *__output_file = NULL;

#if defined(__linux__) || defined(__unix__) || \
    defined(__APPLE__) && defined(__MACH__) // TODO Add Solaris
static pthread_mutex_t __clog_lock = PTHREAD_MUTEX_INITIALIZER;
#else
// windows locks
#endif

static char *log_levels[] = {"INFO", "WARN", "ERROR", "FATAL", "VERBOSE"};

static int __clog_enable = 0; //0 for false

void clog_enable()
{
  if(__output_file == NULL){
    __output_file = stderr;
  }
  __clog_enable = 1;
}

void clog_disable(){
  __clog_enable = 0;
}

static void add_to_stack(clog_item *ci)
{
  if (__clog_enable == 1)
  {
    _CLOG_LOCK();
    fprintf(__output_file, "%s [%s] %s: %s\n", "", log_levels[ci->level], ci->tag,
            ci->msg);
    _CLOG_UNLOCK();
  }
}

void clog_out(FILE *output_file) { __output_file = output_file; }
void clog_i(const char *tag, const char *msg, ...)
{
  _CLOG_LOG(CLOG_INFO, __FILE__, __LINE__);
}

void clog_w(const char *tag, const char *msg, ...)
{
  _CLOG_LOG(CLOG_WARN, __FILE__, __LINE__);
}

void clog_e(const char *tag, const char *msg, ...)
{
  _CLOG_LOG(CLOG_ERROR, __FILE__, __LINE__);
}

void clog_v(const char *tag, const char *msg, ...)
{
    _CLOG_LOG(CLOG_ERROR, __FILE__, __LINE__);
}

void clog_f(const char *tag, const char *msg, ...)
{
  _CLOG_LOG(CLOG_FATAL, __FILE__, __LINE__);
}
