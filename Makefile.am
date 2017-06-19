srcdir = @srcdir@
VPATH = @srcdir@

AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CXXFLAGS = -I$(top_srcdir) -pthread -Wall -Wextra -pedantic -Werror=return-type @BAACK_CPPFLAGS@
 



# docu, if doxygen, CORSIKA so far never with doxygen
if HAVE_DOXYGEN
BAACK_Documentation:
	$(DOXYGEN) $(top_srcdir)/docs 
endif






nobase_include_HEADERS = 

lib_LIBRARIES = libBaack.a 


libBaack_a_SOURCES = lib/io/network/server.cpp \
  lib/io/network/client.cpp \
  lib/io/network/dns_lookup.cpp \
  lib/io/network/win_socket.cpp \
  lib/io/network/unix_socket.cpp \
  lib/io/file.cpp \
  basic/header_manager.cpp \
  basic/header_set.cpp \
  basic/export.cpp \
  basic/basic.cpp 

if WITHTEST
libBaack_a_CPPFLAGS = -Og -g @BAACK_CPPFLAGS@ 
else
libBaack_a_CPPFLAGS = -O3 -g @BAACK_CPPFLAGS@
endif

bin_PROGRAMS =

if WITHTEST

bin_PROGRAMS += library_test

library_test_SOURCES = test/lib/data/particle_test.cpp \
  test/lib/io/server_test.cpp \
  test/lib/io/client_test.cpp \
  test/lib/io/socket_test.cpp \
  test/lib/io/dns_lookup_test.cpp \
  test/lib/unit_test.cpp \
  test/lib/meta/tuple_test.cpp \
  test/lib/meta/list_test.cpp \
  test/lib/meta/is_callable_test.cpp
library_test_LDADD = libBaack.a
library_test_CPPFLAGS = -Og -g3

#basic_test_SOURCES =
#basic_test_LDADD = libBaack.a 
#basic_test_CPPFLAGS = -Og -g

endif

if WITHBENCH

bin_PROGRAMS += dynstack_benchmark

dynstack_benchmark_SOURCES = benchmark/benchmark.cpp \
  benchmark/stack/real.cpp
dynstack_benchmark_LDADD = libDynstack.a
dynstack_benchmark_CPPFLAGS = -O3 -g

endif

EXTRA_DIST = LICENSE README


distclean-local:
	find -name "*.o" -exec -f rm {} \;
	rm -f *.a ; rm -f *.la ; rm -f *.so* ; 









