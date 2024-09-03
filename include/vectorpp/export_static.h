
#ifndef VECTORPP_EXPORT_H
#define VECTORPP_EXPORT_H

#ifdef VECTORPP_STATIC_DEFINE
#  define VECTORPP_EXPORT
#  define VECTORPP_NO_EXPORT
#else
#  ifndef VECTORPP_EXPORT
#    ifdef vectorpp_EXPORTS
        /* We are building this library */
#      define VECTORPP_EXPORT 
#    else
        /* We are using this library */
#      define VECTORPP_EXPORT 
#    endif
#  endif

#  ifndef VECTORPP_NO_EXPORT
#    define VECTORPP_NO_EXPORT 
#  endif
#endif

#ifndef VECTORPP_DEPRECATED
#  define VECTORPP_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VECTORPP_DEPRECATED_EXPORT
#  define VECTORPP_DEPRECATED_EXPORT VECTORPP_EXPORT VECTORPP_DEPRECATED
#endif

#ifndef VECTORPP_DEPRECATED_NO_EXPORT
#  define VECTORPP_DEPRECATED_NO_EXPORT VECTORPP_NO_EXPORT VECTORPP_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VECTORPP_NO_DEPRECATED
#    define VECTORPP_NO_DEPRECATED
#  endif
#endif

#endif /* VECTORPP_EXPORT_H */
