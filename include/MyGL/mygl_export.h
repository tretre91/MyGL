#ifndef MYGL_EXPORT_H
#define MYGL_EXPORT_H

#ifdef MYGL_STATIC
#    define MYGL_EXPORT
#    define MYGL_NO_EXPORT
#else
#    ifndef GLAD_API_CALL_EXPORT
#        define GLAD_API_CALL_EXPORT
#    endif
#    ifdef MYGL_BUILD
#        define GLAD_API_CALL_EXPORT_BUILD
#    endif
#    if defined(_WIN32) || defined(__CYGWIN__)
#        ifdef MYGL_BUILD
#            define MYGL_EXPORT __declspec(dllexport)
#        else
#            define MYGL_EXPORT __declspec(dllimport)
#        endif
#        define MYGL_DEPRECATED __declspec(deprecated)
#        define MYGL_NO_EXPORT
#    else
#       define MYGL_EXPORT __attribute__((visibility("default")))
#       define MYGL_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#endif
