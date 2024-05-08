AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
	-I$(includedir) \
	-I$(OFFLINE_MAIN)/include \
	-isystem$(ROOTSYS)/include

AM_LDFLAGS = \
	-L$(libdir) \
	-L$(ROOTSYS)/lib \
	-L$(OFFLINE_MAIN)/lib

lib_LTLIBRARIES = \
	libinttcosmicsjaein.la

pkginclude_HEADERS = \
	InttCosmicsHotMap.h \
	InttCosmicsBCOFinder.h \
	AnalysisInttCosmicCommissioning.h

#pcmdir = $(libdir)

libinttcosmicsjaein_la_SOURCES = \
	InttCosmicsHotMap.cc \
	AnalysisInttCosmicCommissioning.cc \
	InttCosmicsBCOFinder.cc

libinttcosmicsjaein_la_LIBADD = \
	-L$(libdir) \
	-L$(OFFLINE_MAIN)/lib \
	-lphool \
	-lSubsysReco \
	-lffarawmodules \
	-ltrack_io \
	-lcalo_io \
	-lfun4all \
	-lg4detectors_io \
	-lphg4hit \
	-lg4dst \
	-lg4eval \
	-lintt 
  



################################################
# linking tests

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD = libinttcosmicsjaein.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

%_Dict.cc: %.h %LinkDef.h
	rootcling -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

#just to get the dependency
%_Dict_rdict.pcm: %_Dict.cc ;




clean-local:
	rm -f *Dict* $(BUILT_SOURCES) *.pcm
