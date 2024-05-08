AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -isystem$(ROOTSYS)/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -ltrack_io \
  -L$(OFFLINE_MAIN)/lib64

pkginclude_HEADERS = \
  hot_channel_finder_wctang.h

lib_LTLIBRARIES = \
  libhot_channel_finder_wctang.la

libhot_channel_finder_wctang_la_SOURCES = \
  hot_channel_finder_wctang.cc

libhot_channel_finder_wctang_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lffarawobjects

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD   = libhot_channel_finder_wctang.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
