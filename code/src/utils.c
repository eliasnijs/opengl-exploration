/* function declarations */

internal uint32 filelen(FILE *f);
internal char * readfile(const char *path);
internal uint64 nanos();
internal real32 deg_to_rad(real32 deg);

/* function implementations */

internal uint32
filelen(FILE *f)
{
  uint32 len = 0;
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  fseek(f, 0, 0);
  return len;
}

internal char *
readfile(const char *path)
{
  FILE *f = fopen(path, "rb");
  if (!f)
  {
    logerr("failed to open file %s", path);
    return 0x0;
  }
  uint32 len = filelen(f);
  char *buffer = (char *)malloc(len);
  if (!buffer)
  {
    logerr("failed to allocate memory");
    return 0x0;
  }
  fread(buffer, 1, len, f);
  /// TODO(Elias): Why tf does this need to happen???
  /// for some reaons the end of file chacter misses or soemthing
  buffer[len-1] = '\0';
  fclose(f);
  return buffer;
}

internal uint64
nanos()
{
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  return (uint64)start.tv_sec*10e9 + (uint64)start.tv_nsec;
}

internal real32
deg_to_rad(real32 deg)
{
  return deg*(pi32/180);
}


