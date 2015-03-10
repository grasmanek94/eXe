/*
REGEX SEARCH (in 'libodb-vc10.vcxproj.filters')

.*\<ClCompile\sInclude\=\"(.*)\"\>\r\n.*\<Filter\>.*\<\/Filter\>\r\n.*\<\/ClCompile\>

REPLACE:

#include "odb\$1"

*/
#include "odb\callback.cxx"
#include "odb\exceptions.cxx"
#include "odb\database.cxx"
#include "odb\vector-impl.cxx"
#include "odb\connection.cxx"
#include "odb\lazy-ptr-impl.cxx"
#include "odb\prepared-query.cxx"
#include "odb\query-dynamic.cxx"
#include "odb\result.cxx"
#include "odb\schema-catalog.cxx"
#include "odb\section.cxx"
#include "odb\session.cxx"
#include "odb\statement.cxx"
#include "odb\statement-processing.cxx"
#include "odb\tracer.cxx"
#include "odb\transaction.cxx"
#include "odb\details\buffer.cxx"
#include "odb\details\condition.cxx"
#include "odb\details\lock.cxx"
#include "odb\details\mutex.cxx"
#include "odb\details\shared-ptr\base.cxx"
#include "odb\details\win32\init.cxx"
#include "odb\details\win32\once.cxx"
#include "odb\details\win32\tls.cxx"
#include "odb\details\win32\condition.cxx"
#include "odb\details\win32\thread.cxx"
#include "odb\details\win32\exceptions.cxx"