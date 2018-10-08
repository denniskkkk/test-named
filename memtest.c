#include <libmemcached/memcached.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  //memcached_servers_parse (char *server_strings);
  memcached_server_st *servers = NULL;
  memcached_st *memc;
  memcached_return rc;
  char *key = "keystring";
  char *value = "keyvalue";

  char *retrieved_value;
  size_t value_length;
  uint32_t flags;
  unsigned long cnt ;
  char bufout [1024];
  char bufin  [1024];
  char bufval [1024];
  long asize = 100000;
  //
  memc = memcached_create(NULL);
  servers = memcached_server_list_append(servers, "192.168.1.48", 11212, &rc);
  rc = memcached_server_push(memc, servers);

  if (rc == MEMCACHED_SUCCESS) {
    fprintf(stderr, "Added server successfully\n");
  }
  else {
    fprintf(stderr, "Couldn't add server: %s\n", memcached_strerror(memc, rc));
    exit -1;
  }
  for (cnt =0; cnt < asize; cnt ++ ) {
  memset (bufout, 0, strlen (bufout));
  memset (bufval, 0, strlen (bufval));
  //set value
  sprintf (bufout, "key%08d", cnt);
  sprintf (bufval, "value is a magic value of %010d \ntimes 1000000 in magic number\n", cnt);
  rc = memcached_set(memc, bufout, strlen(bufout), bufval, strlen(bufval), (time_t)0, (uint32_t)0);
  if (rc == MEMCACHED_SUCCESS) {
  //  fprintf(stderr, "Keyok:%010d\n", cnt);
  }
  else {
    fprintf(stderr, "Couldn't store key: %s\n", memcached_strerror(memc, rc));
    exit -1;
  }
  }

  // get it back
  for (cnt =0; cnt < asize; cnt ++ ) {
  memset (bufin, 0, strlen (bufin));	  
  sprintf (bufin, "key%08d", cnt);
  retrieved_value = memcached_get(memc, bufin, strlen(bufin), &value_length, &flags, &rc);
  if (rc == MEMCACHED_SUCCESS) {
    printf("key '%s' retvalue '%s'.\n", bufin , retrieved_value);
    free(retrieved_value);
  }
  else {
    fprintf(stderr, "Couldn't retrieve key: %s\n", memcached_strerror(memc, rc));
    exit -1;
  }
  
  }

  return 0;
}

