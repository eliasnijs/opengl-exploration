internal uint32
shader_init_module(const char *path, int32 type, uint32 *module_id)
{
  int32 errcode = 0;

  char *source = readfile(path);
  if (!source)
  {
    DebugLogError("failed to read shader file");
    errcode = 1;
    goto err1;
  }

  uint32 id = glCreateShader(type);
  glShaderSource(id, 1, (const char * const *)&source, 0);
  glCompileShader(id);
  bool32 success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char info_log[512] = {0};
    glGetShaderInfoLog(id, 512, 0, info_log);
    DebugLogError("failed to compile shader module \"%s\"...\n%s", path, info_log);
    errcode = 2;
    goto err2;
  }
  *module_id = id;

err2:
  free(source);
err1:
  return errcode;
}


internal uint32
shader_init(const char *vertex_path, const char *fragment_path, Shader *shader)
{
  int32 errcode = 0;

  uint32 vertex_shader;
  int32 vertex_success = shader_init_module(vertex_path, GL_VERTEX_SHADER, &vertex_shader);
  if (vertex_success != 0)
    errcode = 1;

  uint32 fragment_shader;
  int32 fragment_success = shader_init_module(fragment_path, GL_FRAGMENT_SHADER, &fragment_shader);
  if (fragment_success != 0)
    errcode = 1;

  uint32 id = glCreateProgram();
  glAttachShader(id, vertex_shader);
  glAttachShader(id, fragment_shader);
  glLinkProgram(id);
  shader->id = id;

  if (fragment_success == 0)
    glDeleteShader(fragment_shader);
  if (vertex_success == 0)
    glDeleteShader(vertex_shader);

  bool32 success;
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success)
  {
    char info_log[512] = {0};
    glGetShaderInfoLog(id, 512, 0, info_log);
    DebugLogError("failed to configure shader program...\n%s", info_log);
    errcode = 1;
  }

  return errcode;
}

internal void
shader_die(Shader *shader)
{
  glDeleteProgram(shader->id);
}

internal void
shader_use(Shader *shader)
{
  glUseProgram(shader->id);
}

internal void
shader_set_bool32(Shader *shader, char *name, bool32 value)
{
  glUniform1i(glGetUniformLocation(shader->id, name), (int32)value);
}

internal void
shader_set_int32(Shader *shader, char *name, int32 value)
{
  glUniform1i(glGetUniformLocation(shader->id, name), value);
}

internal void
shader_set_real32(Shader *shader, char *name, real32 value)
{
  glUniform1f(glGetUniformLocation(shader->id, name), value);
}

internal void
shader_set_vec3(Shader *shader, char *name, vec3 v)
{
  glUniform3fv(glGetUniformLocation(shader->id, name), 1, v);
}

internal void
shader_set_mat4x4(Shader *shader, char *name, mat4x4 m)
{
  glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, &m[0][0]);
}

