#ifndef MYGL_EXPORT_H
#define MYGL_EXPORT_H

#ifdef MYGL_STATIC_DEFINE
#  define MYGL_EXPORT
#  define MYGL_NO_EXPORT
#else
#  if !defined(MYGL_EXPORT)
#    if !defined(GLAD_GLAPI_EXPORT)
#      define GLAD_GLAPI_EXPORT
#    endif
#    if defined(_WIN32) || defined(__CYGWIN__)
#      if defined(MyGL_EXPORTS)
         /* We are building this library */
#        define MYGL_EXPORT __declspec(dllexport)
#        if !defined(GLAD_GLAPI_EXPORT_BUILD)
#          define GLAD_GLAPI_EXPORT_BUILD
#        endif
#      else
         /* We are using this library */
#        define MYGL_EXPORT __declspec(dllimport)
#      endif
#    else
#      define MYGL_EXPORT __attribute__((visibility("default")))
#      if defined(MyGL_EXPORTS) && !defined(GLAD_GLAPI_EXPORT_BUILD)
#        define GLAD_GLAPI_EXPORT_BUILD
#      endif
#    endif
#  endif

#  ifndef MYGL_NO_EXPORT
#    define MYGL_NO_EXPORT 
#  endif
#endif

#ifndef MYGL_DEPRECATED
#  define MYGL_DEPRECATED __declspec(deprecated)
#endif

#ifndef MYGL_DEPRECATED_EXPORT
#  define MYGL_DEPRECATED_EXPORT MYGL_EXPORT MYGL_DEPRECATED
#endif

#ifndef MYGL_DEPRECATED_NO_EXPORT
#  define MYGL_DEPRECATED_NO_EXPORT MYGL_NO_EXPORT MYGL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MYGL_NO_DEPRECATED
#    define MYGL_NO_DEPRECATED
#  endif
#endif

#endif /* MYGL_EXPORT_H */
