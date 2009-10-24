dnl ---------------------------------------------------------------------------
dnl Macro: MYSQL_SRC
dnl ---------------------------------------------------------------------------
AC_DEFUN([MYSQL_SRC_TEST], [
  AC_MSG_CHECKING(for mysql source code)
  AC_ARG_WITH(mysql,
  [[  --with-mysql[=mysql src directory]      
                        Source requir to build engine.]],
  [
    dir="$(cd $withval ; pwd)"
    
    if test -d "$dir/sql" -a \
            -d "$dir/include" -a \
            -f "$dir/sql/mysql_priv.h" -a \
            -f "$dir/sql/handler.h" -a \
            -f "$dir/include/mysql/plugin.h"; then
      AC_DEFINE([MYSQL_SRC], [1], [Source directory for MySQL])
      MYSQL_INC="-I$dir/sql -I$dir/include -I$dir/regex -I$dir"
      AC_MSG_RESULT(["$dir"])
    else
      AC_MSG_ERROR(["not found"]);
    fi
  ],
  [
    AC_MSG_ERROR(["no mysql source provided"])
  ])
])

dnl ---------------------------------------------------------------------------
dnl Macro: MYSQL_SRC
dnl ---------------------------------------------------------------------------
