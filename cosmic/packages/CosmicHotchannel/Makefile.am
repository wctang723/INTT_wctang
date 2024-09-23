AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -isystem$(ROOTSYS)/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OFFLINE_MAIN)/lib64

pkginclude_HEADERS = \
  CosmicHotchannel.h

lib_LTLIBRARIES = \
  libCosmicHotchannel.la

libCosmicHotchannel_la_SOURCES = \
  CosmicHotchannel.cc

libCosmicHotchannel_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lffarawmodules \
  -ltrack_io \
  -lintt_io

  
BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD   = libCosmicHotchannel.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
