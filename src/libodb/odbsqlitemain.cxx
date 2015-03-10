/*
REGEX SEARCH (in 'libodb-sqlite-vc10.vcxproj.filters')

.*\<ClCompile\sInclude\=\"(.*)\"\>\r\n.*\<Filter\>.*\<\/Filter\>\r\n.*\<\/ClCompile\>

REPLACE:

#include "odb\sqlite\$1"

*/
#include "odb\sqlite\connection.cxx"
#include "odb\sqlite\connection-factory.cxx"
#include "odb\sqlite\database.cxx"
#include "odb\sqlite\error.cxx"
#include "odb\sqlite\exceptions.cxx"
#include "odb\sqlite\prepared-query.cxx"
#include "odb\sqlite\query.cxx"
#include "odb\sqlite\query-dynamic.cxx"
#include "odb\sqlite\query-const-expr.cxx"
#include "odb\sqlite\simple-object-statements.cxx"
#include "odb\sqlite\statement.cxx"
#include "odb\sqlite\statements-base.cxx"
#include "odb\sqlite\statement-cache.cxx"
#include "odb\sqlite\tracer.cxx"
#include "odb\sqlite\traits.cxx"
#include "odb\sqlite\transaction.cxx"
#include "odb\sqlite\transaction-impl.cxx"
#include "odb\sqlite\details\options.cxx"