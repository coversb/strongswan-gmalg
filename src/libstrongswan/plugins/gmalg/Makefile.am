AM_CPPFLAGS = \
	-I$(top_srcdir)/src/libstrongswan \
	-I$(top_srcdir)/src/libstrongswan/plugins/gmalg/gmalg/include \
	-DGMALG_INTERIOR=${gmalg_interior}

AM_CFLAGS = \
	$(PLUGIN_CFLAGS)

if MONOLITHIC
noinst_LTLIBRARIES = libstrongswan-gmalg.la
else
plugin_LTLIBRARIES = libstrongswan-gmalg.la
endif

libstrongswan_gmalg_la_SOURCES = \
	gmalg_plugin.h gmalg_plugin.c \
	gmalg_crypter.h gmalg_crypter.c \
	gmalg_hasher.h gmalg_hasher.c \
	gmalg_rng.h gmalg_rng.c \
	gmalg_ec_diffie_hellman.h gmalg_ec_diffie_hellman.c\
	gmalg_ec_private_key.h gmalg_ec_private_key.c \
	gmalg_ec_public_key.h gmalg_ec_public_key.c \
	gmalg_ec_util.h gmalg_ec_util.c \
	gmalg/gmalg.c \
	gmalg/sm3.c \
	gmalg/sm4.c \
	gmalg/sm2.c \
	gmalg/gmalg_ecc.c \
	gmalg/gmalg_random.c \
	gmalg/gmalg_debug.c

libstrongswan_gmalg_la_LDFLAGS = -module -avoid-version
libstrongswan_gmalg_la_LIBADD  = $(GMALG_LIB)
