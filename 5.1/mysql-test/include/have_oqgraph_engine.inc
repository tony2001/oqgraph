disable_query_log;
INSTALL PLUGIN oqgraph SONAME 'oqgraph_engine.so';
--require r/true.require
select (PLUGIN_LIBRARY LIKE 'oqgraph_engine%') as `TRUE` from information_schema.plugins where PLUGIN_NAME='OQGRAPH';
enable_query_log;
