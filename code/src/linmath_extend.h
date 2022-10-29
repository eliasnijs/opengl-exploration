internal void
vec3_normalise()
{

}

internal void
vec3_print(vec3 v)
{
  printf("{");
  for (uint32 i = 0; i < 3; ++i)
    printf("% .2f, ", v[i]);
  printf("}\n");
}

internal void
vec4_print(vec4 v)
{
  printf("{");
  for (uint32 i = 0; i < 4; ++i)
    printf("% .2f, ", v[i]);
  printf("}\n");
}

internal void
mat4x4_print(mat4x4 m)
{
  printf("{\n");
  for (uint32 i = 0; i < 4; ++i)
  {
    printf("  {");
    for (uint32 j = 0; j < 4; ++j)
      printf("% .2f, ", m[i][j]);
    printf("}\n");
  }
  printf("}\n");
}
